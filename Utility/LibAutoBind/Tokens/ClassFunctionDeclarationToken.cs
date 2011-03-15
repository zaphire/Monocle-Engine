using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;
using System.Text.RegularExpressions;

namespace LibAutoBind.Tokens
{
    class ClassFunctionDeclarationToken : Token
    {
        public ClassFunctionDeclarationToken() { }

        internal override void Detect(Lexer l)
        {
            if (!(l.Char == ' ' || (l.Char >= '0' && l.Char <= '9') || (l.Char >= 'a' && l.Char <= 'z') ||
                (l.Char >= 'A' && l.Char <= 'Z') || l.Char == '\n' || l.Char == '\t' || l.Char == '\r' ||
                l.Char == '{' || l.Char == '_' || l.Char == ',' || l.Char == '(' || l.Char == ')' ||
                l.Char == ':' || l.Char == ';' || l.Char == '*' || l.Char == '<' || l.Char == '>' ||
                l.Char == '[' || l.Char == ']' || l.Char == '-' || l.Char == '.' || l.Char == '"' ||
                l.Char == '&'))
            {
                l.ForceExclude(); // contains characters we can't accept.
                return;
            }
            if (l.Text.Contains('='))
            {
                l.ForceExclude(); // contains characters we can't accept.
                return;
            }
            if (!(l.GetParent() is ClassDefinitionToken))
            {
                l.ForceExclude(); // not within a class.
                return;
            }

            KeywordResult res = Keywords.GetKeywords(l.Text);

            if (res.Keywords.Count > 0 && res.PossibleKeyword == false)
            {
                // Now use regular expression checking against the substring starting
                // at res.DeclIndex to determine whether it's a variable declaration.

                string decl = l.Text.Substring(res.DeclIndex).Trim();
                if (decl.EndsWith(";"))
                {
                    // Show 'expected {' error.
                    Console.WriteLine("Error: expected '{', not ';' on line " + l.LineNumber + " within file " + l.FileName + ".\nError: Check the end of your function definitions.");
                    l.Abort();
                    return;
                }
                if (!decl.EndsWith("{"))
                    return; // Skip if we don't have a terminating character.
                Regex r = new Regex(
                    "(?<Type>[a-zA-Z][a-zA-z0-9_\\:\\<\\>]+[ \t\r\n\\*]+)?" +
                    "(?<Name>[a-zA-Z][a-zA-z0-9_]*[ \t\r\n]*\\([ \t\r\na-zA-Z0-9_&,-\\.\\:\\*\\<\\>\\[\\]\\\"]*\\))[ \t\r\n]*\\{"
                    );
                Match m = r.Match(decl);
                if (m.Success)
                {
                    // It's a function declaration.
                    l.TakeOwnership();
                    l.AddNode(new DirectNode("\n"));
                    l.AddNode(new ClassFunctionDeclarationNode(res.Keywords, m.Groups["Type"].Value.Trim(), m.Groups["Name"].Value.Trim()));
                    l.AddParent();
                    l.EndOwnership();
                }
                else
                {
                    // A different kind of declaration that we aren't handling...
                    l.ForceExclude();
                }
            }
        }

        internal override bool DetectEnd(Lexer l)
        {
            if (l.Char == '}')
            {
                l.AddNode(new DirectNode(l.Text));
                return true;
            }

            return false;
        }
    }
}
