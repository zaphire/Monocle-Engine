using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class Keywords
    {
        // Keywords specific to C++, they are kept untouched.
        internal static string[] CPPVisibilityKeywords = { "private", "protected", "public", "friend" };
        internal static string[] CPPTypeKeywords = { "char", "bool", "short", "int", "long", "longlong", "float", "double", "void" };
        internal static string[] CPPStripKeywords = { "static", "virtual" };
        internal static string[] CPPMiscKeywords = { "const" };

        // Keywords specific to the automatic binding system.  These are read
        // by the tokens and removed in the final source code.
        internal static string[] LuaVisibilityKeywords = { "bound", "unbound" };
        internal static string[] LuaTypeKeywords = { "string", "::string", "numeric", "table", "function", "variant" };
        internal static string[] LuaDeclKeywords = { "property" };

        /// <summary>
        /// Detects keywords in the specified string and returns a list of them.  Stops
        /// scanning when the first non-keyword is read.
        /// </summary>
        /// <param name="text">The text to scan.</param>
        /// <returns>A list of keywords.</returns>
        internal static KeywordResult GetKeywords(string text)
        {
            string buf = "";
            KeywordResult ret = new KeywordResult();
            for (int i = 0; i < text.Length; i++)
            {
                if ((text[i] == ' ' || text[i] == '\t' || text[i] == '\n' || text[i] == '\r') && buf.Trim().Length > 0)
                {
                    buf = buf.Trim();
                    // End of building up the buffer.
                    if (Keywords.GetKeyword(buf) != "")
                    {
                        ret.Keywords.Add(buf);
                        ret.DeclIndex = i;
                        buf = "";
                    }
                    else
                    {
                        // No more keywords.  It's not possible
                        // to have a partial keyword here since
                        // the last character was whitespace.
                        ret.PossibleKeyword = false;
                        return ret;
                    }
                }
                else
                    buf += text[i];
            }
            if (buf.Length > 0)
            {
                if (Keywords.GetKeyword(buf) != "")
                {
                    ret.Keywords.Add(buf);
                    ret.DeclIndex = text.Length;
                    ret.PossibleKeyword = false;
                    buf = "";
                    return ret;
                }
                else
                {
                    // Check to see whether it's a partial keyword.
                    ret.PossibleKeyword = Keywords.IsPartialKeyword(buf);
                    return ret;
                }
            }
            else
            {
                ret.PossibleKeyword = false;
                return ret;
            }
        }

        /// <summary>
        /// Returns the keyword located in the text.  If the text is an exact match
        /// to one of the keywords specified in the static arrays, it is returned.
        /// Otherwise, a blank string is returned.
        /// </summary>
        /// <param name="text">The text to scan.</param>
        /// <returns>A string containing the keyword, or "".</returns>
        internal static string GetKeyword(string text)
        {
            foreach (string s in Keywords.CPPVisibilityKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.CPPTypeKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.CPPStripKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.CPPMiscKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.LuaVisibilityKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.LuaTypeKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.LuaDeclKeywords)
                if (s == text) return s;
            return "";
        }

        /// <summary>
        /// Returns whether or not the text is a partial keyword (i.e. looks like
        /// the start of a keyword).
        /// </summary>
        /// <param name="text">The text to scan.</param>
        /// <returns>Whether or not the text is a partial keyword.</returns>
        internal static bool IsPartialKeyword(string text)
        {
            text = text.Trim();
            foreach (string s in Keywords.CPPVisibilityKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.CPPTypeKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.CPPStripKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.CPPMiscKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.LuaVisibilityKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.LuaTypeKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            foreach (string s in Keywords.LuaDeclKeywords)
                if (s.TrimStart().StartsWith(text)) return true;
            return false;
        }
    }

    class KeywordResult
    {
        /// <summary>
        /// A list of keywords found in the search.
        /// </summary>
        internal List<string> Keywords = new List<string>();

        /// <summary>
        /// The index in the string at which the first unrecognized text was
        /// found (i.e. where the actual declaration of the variable or function
        /// starts).
        /// </summary>
        internal int DeclIndex;

        /// <summary>
        /// Whether or not the text after DeclIndex looks like it might be the
        /// start of another keyword.  If this is false, the token can check the
        /// rest of the text to see if it can handle it.
        /// </summary>
        internal bool PossibleKeyword;

        internal KeywordResult() { }
    }
}
