using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class PreprocessorIgnoreToken : Token
    {
        private bool m_KeepOwnership = false;

        internal PreprocessorIgnoreToken() { }

        internal override void Detect(Lexer l)
        {
            if (l.Text.TrimStart().StartsWith("#") &&
                (l.MatchNext(" ") || l.MatchNext("\n")) &&
                l.Text != "#import" &&
                l.Text != "#include")
                l.TakeOwnership();
            else if (!l.Text.TrimStart().StartsWith("#") || l.Text == "#import" || l.Text == "#include")
                l.ForceExclude();

            if (l.HasOwnership())
            {
                if (l.Char == '\\' && l.MatchNext("\n"))
                {
                    this.m_KeepOwnership = true;
                }
                else if (l.Char == '\n' && !this.m_KeepOwnership)
                {
                    l.AddNode(new DirectNode(l.Text));
                    l.EndOwnership();
                }
                else if (this.m_KeepOwnership)
                    this.m_KeepOwnership = false;
            }
            
        }
    }
}
