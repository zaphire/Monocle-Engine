using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Tokens;
using LibAutoBind.Nodes;
using System.Reflection;
using System.Text.RegularExpressions;

namespace LibAutoBind
{
    class Lexer
    {
        private string p_TextCache = "";
        private char p_Char;
        private int p_LineNumber = 1;

        private Machine m_Machine = null;
        private List<Token> m_Tokens = new List<Token>();
        private List<Token> m_ExcludedTokens = new List<Token>();
        private Stack<Token> m_ParentStack = new Stack<Token>();
        private Token m_TokenWithOwnership = null;
        private Token m_CurrentToken = null;
        private List<Node> p_LexerList = new List<Node>();
        private string m_SeekCache = "";
        private bool m_ShouldResetText = false;
        private bool m_IsComment = false;
        private bool m_IsLongComment = false;

        internal Lexer(Machine m)
        {
            this.m_Machine = m;

            // Add all the tokens we should recognise.
            this.m_Tokens.Add(new PreprocessorImportToken());
            this.m_Tokens.Add(new PreprocessorIncludeToken());
            this.m_Tokens.Add(new PreprocessorIgnoreToken());
            this.m_Tokens.Add(new UsingToken());
            this.m_Tokens.Add(new ClassDefinitionToken());
            this.m_Tokens.Add(new ClassVariableDeclarationToken());
            this.m_Tokens.Add(new ClassPropertyDeclarationToken());
            this.m_Tokens.Add(new ClassFunctionDeclarationToken());
            this.m_Tokens.Add(new GenericBracketToken());
        }

        /// <summary>
        /// Runs the lexer on the machine's input file.
        /// </summary>
        internal void Run()
        {
            this.p_LineNumber = 0;
            while (!this.m_Machine.InputFile.EndOfStream || this.m_SeekCache.Length > 0)
            {
                // Retrieve a character from the seek cache (used because
                // sometimes we need to read ahead multiple characters and
                // we can only peek the next character) or alternatively
                // the file if the seek cache is empty.
                char c;
                if (this.m_SeekCache.Length > 0)
                {
                    c = this.m_SeekCache[0];
                    this.m_SeekCache = this.m_SeekCache.Substring(1);
                    c = this.ConvertNewline(c);
                }
                else
                {
                    c = (char)this.m_Machine.InputFile.Read();
                    c = this.ConvertNewline(c);
                }

                if (c == '\n')
                    this.p_LineNumber += 1;

                this.p_Char = c;

                this.p_TextCache += c;

                // Check for changes between the comment statuses.
                if (this.p_Char == '/' && this.p_TextCache.Length > 1)
                {
                    if (this.p_TextCache[this.p_TextCache.Length - 2] == '/')
                        this.m_IsComment = true;
                    else if (this.p_TextCache[this.p_TextCache.Length - 2] == '*' && this.m_IsLongComment)
                    {
                        this.m_IsLongComment = false;
                        Regex r = new Regex("[ \t\r\n]*\\/\\*.*\\*\\/[ \t\r\n]*", RegexOptions.Multiline);
                        Match m = r.Match(this.p_TextCache);
                        this.p_TextCache = r.Replace(this.p_TextCache, "");
                        this.AddNode(new DirectNode(m.Value));
                        this.m_ExcludedTokens.Clear();
                        if (this.p_TextCache.Length > 0)
                            this.p_Char = this.p_TextCache[this.p_TextCache.Length - 1];
                        else
                            this.p_Char = '\0';
                        continue;
                    }
                }
                else if (this.p_Char == '*' && this.p_TextCache.Length > 1)
                {
                    if (this.p_TextCache[this.p_TextCache.Length - 2] == '/')
                        this.m_IsLongComment = true;
                }
                else if (this.p_Char == '\n' && this.m_IsComment)
                {
                    this.m_IsComment = false;
                    Regex r = new Regex("[ \t\r\n]*\\/\\/[^\n]*\n");
                    Match m = r.Match(this.p_TextCache);
                    this.p_TextCache = r.Replace(this.p_TextCache, "");
                    this.AddNode(new DirectNode(m.Value));
                    this.m_ExcludedTokens.Clear();
                    if (this.p_TextCache.Length > 0)
                        this.p_Char = this.p_TextCache[this.p_TextCache.Length - 1];
                    else
                        this.p_Char = '\0';
                    continue;
                }

                if (this.m_IsComment || this.m_IsLongComment)
                    continue;

                // The length of the string is 0, which causes problems with
                // tokens that rely on StartsWith.  Since there's no characters,
                // and hence nothing that any token can make a decision on, we
                // just continue the loop.
                if (this.p_TextCache.TrimStart().Length == 0)
                {
                    continue;
                }

                // First check the immediate parent to see whether or not it wants
                // to be unloaded from the list (and to consume the current text
                // state).
                if (this.m_ParentStack.Count > 0)
                {
                    Token t = this.m_ParentStack.Peek();
                    this.m_CurrentToken = t;
                    bool r = t.DetectEnd(this);
                    this.m_CurrentToken = null;

                    if (r)
                    {
                        // Find the current instance of the token type in the
                        // lexer list and replace it with the parent instance.
                        for (int i = 0; i < this.m_Tokens.Count; i++)
                        {
                            if (this.m_Tokens[i].GetType() == t.GetType())
                            {
                                this.m_Tokens[i] = this.m_ParentStack.Pop();
                                break;
                            }
                        }
                        this.p_TextCache = "";
                        this.p_Char = '\0';
                        this.m_ExcludedTokens.Clear();
                        continue;
                    }
                }

                if (this.m_TokenWithOwnership == null)
                {
                    // If all of the tokens have placed themselves in
                    // the excluded list, we add a new DirectNode (since
                    // we don't understand the content) and clear the
                    // text state.
                    if (this.m_Tokens.Count == this.m_ExcludedTokens.Count)
                    {
                        this.AddNode(new DirectNode(this.p_TextCache));
                        this.p_TextCache = "";
                        this.p_Char = '\0';
                        this.m_ExcludedTokens.Clear();
                        continue;
                    }

                    for (int a = 0; a < this.m_Tokens.Count; a++)
                    {
                        Token t = this.m_Tokens[a];
                        if (this.m_ExcludedTokens.Contains(t))
                            continue;

                        this.m_CurrentToken = t;
                        t.Detect(this);
                        
                        // The AddParent() call modifies the m_CurrentToken
                        // variable.  Make sure it's saved back into the
                        // token list.
                        int i = this.m_Tokens.IndexOf(t);
                        this.m_Tokens[i] = this.m_CurrentToken;

                        if (this.m_ShouldResetText)
                        {
                            this.m_TokenWithOwnership = null;
                            this.p_TextCache = "";
                            this.p_Char = '\0';
                            this.m_ExcludedTokens.Clear();
                            this.m_ShouldResetText = false;
                            break; // Skip the rest of the tokens.
                        }
                    }
                }
                else
                {
                    this.m_CurrentToken = this.m_TokenWithOwnership;
                    this.m_TokenWithOwnership.Detect(this);
                    if (this.m_ShouldResetText)
                    {
                        this.m_TokenWithOwnership = null;
                        this.p_TextCache = "";
                        this.p_Char = '\0';
                        this.m_ExcludedTokens.Clear();
                        this.m_ShouldResetText = false;
                    }
                }

                this.m_CurrentToken = null;
            }
        }

        internal void Abort()
        {
            throw new LexingAbortedException("An error occurred during lexing.  Check the console window.");
        }

        /// <summary>
        /// Compacts the number of direct nodes to allow for easier debugging when
        /// there is a whole bunch of unrecognized text.
        /// </summary>
        private void CompactDirectNodes()
        {
            if (this.p_LexerList.Count < 2) return;

            Node nl = this.p_LexerList[this.p_LexerList.Count - 1];
            Node ns = this.p_LexerList[this.p_LexerList.Count - 2];
            while (ns is DirectNode && nl is DirectNode && this.p_LexerList.Count >= 2)
            {
                ((DirectNode)ns).SetContent(ns.Content + nl.Content);
                this.p_LexerList.Remove(nl);

                nl = this.p_LexerList[this.p_LexerList.Count - 1];
                ns = this.p_LexerList[this.p_LexerList.Count - 2];
            }
        }

        /// <summary>
        /// Ensures that newlines are represented by single \n characters.
        /// </summary>
        private char ConvertNewline(char c)
        {
            if (c == '\r')
            {
                if (this.m_SeekCache.Length > 0)
                {
                    if (this.m_SeekCache[0] == '\n')
                        this.m_SeekCache = this.m_SeekCache.Substring(1);
                    c = '\n';
                }
                else
                {
                    if ((char)this.m_Machine.InputFile.Peek() == '\n')
                        c = (char)this.m_Machine.InputFile.Read();
                    else
                        c = '\n';
                }
            }

            return c;
        }

        /// <summary>
        /// Check the text before the current text state to see if
        /// it matches the specified string argument.
        /// </summary>
        internal bool MatchPrev(string str)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// The current block of text that is being evaluated.  The characters
        /// are continually added to the text until a Token takes ownership and
        /// handles the block of text.
        /// </summary>
        internal string Text
        {
            get { return this.p_TextCache; } //.TrimStart(); }
        }

        internal string FileName
        {
            get { return this.m_Machine.InputName; }
        }

        internal string FilePath
        {
            get { return this.m_Machine.InputPath; }
        }

        internal int LineNumber
        {
            get { return this.p_LineNumber; }
        }

        /// <summary>
        /// A list of nodes built from the lexer during scanning the input file.
        /// </summary>
        internal List<Node> Nodes
        {
            get { return this.p_LexerList; }
        }

        /// <summary>
        /// The current character that was last read.
        /// </summary>
        internal char Char
        {
            get { return this.p_Char; }
        }

        /// <summary>
        /// Check the text after the current text state (peek ahead) to see if
        /// it matches the specified string argument.  Useful to check whether
        /// or not there is whitespace (i.e. end of a token) before taking
        /// ownership of it.
        /// </summary>
        internal bool MatchNext(string str)
        {
            string res = "";
            for (int i = 0; i < str.Length; i += 1)
            {
                if (this.m_SeekCache.Length > i)
                    res += this.m_SeekCache[i];
                else
                {
                    char c = (char)this.m_Machine.InputFile.Read();
                    c = ConvertNewline(c);
                    res += c;
                    this.m_SeekCache += c;
                }
            }

            return (str == res);
        }

        /// <summary>
        /// Take ownership of the current text state.  After a Token calls this
        /// function, it is the only Token that will recieve Run() calls.  This
        /// ensures that other tokens don't add nodes when they are not meant to.
        /// </summary>
        internal void TakeOwnership()
        {
            this.m_TokenWithOwnership = this.m_CurrentToken;
        }

        /// <summary>
        /// Adds a node to the lexer list.  Nodes are then passed to the Transformer
        /// once lexing is complete.
        /// </summary>
        internal void AddNode(Node node)
        {
            node.LineNumber = this.LineNumber;
            node.FileName = this.FilePath;
            node.ParentCount = this.m_ParentStack.Count;
            this.p_LexerList.Add(node);
            this.CompactDirectNodes();
        }

        /// <summary>
        /// Ends ownership of the current text state.  Once this has been called by
        /// a Token that has previously taken ownership, the lexer clears the text
        /// state and moves onto reading the next set of characters.
        /// </summary>
        internal void EndOwnership()
        {
            if (this.m_TokenWithOwnership == this.m_CurrentToken)
            {
                this.m_ShouldResetText = true;
            }
        }

        /// <summary>
        /// Returns whether or not the Token currently has ownership of the text
        /// state.
        /// </summary>
        internal bool HasOwnership()
        {
            return (this.m_TokenWithOwnership == this.m_CurrentToken);
        }

        /// <summary>
        /// Forces the lexer to not include the current token in any further text
        /// state testing (i.e. Run() will not be called) until another Token
        /// successfully handles the current state.
        /// </summary>
        internal void ForceExclude()
        {
            if (!this.m_ExcludedTokens.Contains(this.m_CurrentToken))
                this.m_ExcludedTokens.Add(this.m_CurrentToken);
        }

        /// <summary>
        /// Adds the current token as a parent to future tokens (i.e. a
        /// ClassDefinitionToken becomes the parent so that variables within
        /// classes will be transformed correctly).  When a token is the
        /// immediate parent, it will have DetectEnd() called at the start of
        /// the lexer loop.  If this function returns true, it indicates that
        /// the token no longer wants to own the current block and the lexer
        /// loop restarts with the parent removed from the list.
        /// </summary>
        internal void AddParent()
        {
            // You can't be a parent if you currently own the text.  However,
            // it's assumed that if you are calling this function, you want to
            // implicitly end ownership of the text.  There's no harm done if
            // a token calls EndOwnership after AddParent (and it reads nicer
            // as well).
            if (this.HasOwnership())
            {
                this.EndOwnership();
            }

            // We actually create a new instance of the same token since
            // we may have token detectors that are children of the same
            // type of token detectors, and the parent status would be
            // shared if we were using the same token detector in the
            // parent list.
            this.m_ParentStack.Push(this.m_CurrentToken);
            Type t = this.m_CurrentToken.GetType();
            ConstructorInfo ci = t.GetConstructor(Type.EmptyTypes);
            if (ci == null)
            {
                throw new MethodAccessException("The token object which attempted to raise itself as a parent does not have a public constructor.");
            }
            this.m_CurrentToken = (Token)ci.Invoke(null);
        }

        /// <summary>
        /// Returns whether the current token is acting as a parent.
        /// </summary>
        /// <returns>Whether the current token is acting as a parent.</returns>
        internal bool IsParent()
        {
            return this.m_ParentStack.Contains(this.m_CurrentToken);
        }

        /// <summary>
        /// Gets the immediate parent.
        /// </summary>
        /// <returns>Returns the immediate parent, or null if there is none.</returns>
        internal Token GetParent()
        {
            if (this.m_ParentStack.Count == 0)
                return null;
            else
                return this.m_ParentStack.Peek();
        }
    }
}
