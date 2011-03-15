using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class PreprocessorIncludeToken : Token
    {
        private bool m_KeepOwnership = false;

        internal PreprocessorIncludeToken() { }

        internal override void Detect(Lexer l)
        {
            if (l.Text == "#include")
                l.TakeOwnership();
            else if (!l.Text.TrimStart().StartsWith("#"))
                l.ForceExclude();

            if (l.HasOwnership())
            {
                if (l.Char == '\\' && l.MatchNext("\n"))
                {
                    this.m_KeepOwnership = true;
                }
                else if (l.Char == '\n' && !this.m_KeepOwnership)
                {
                    l.AddNode(new IncludeNode(l.Text.Substring("#include".Length).Trim()));
                    l.EndOwnership();
                }
                else if (this.m_KeepOwnership)
                    this.m_KeepOwnership = false;
            }
        }
    }
}
