using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class UsingToken : Token
    {
        internal UsingToken() { }

        internal override void Detect(Lexer l)
        {
            if (l.Text.TrimStart().StartsWith("using ") && !l.HasOwnership())
                l.TakeOwnership();
            else if (l.HasOwnership() && l.Char == ';')
            {
                l.AddNode(new UsingNode(l.Text.Substring("using ".Length, l.Text.Length - "using ".Length - 1)));
                l.EndOwnership();
            }
            else if (!l.HasOwnership() && !"using ".StartsWith(l.Text.TrimStart()))
                l.ForceExclude();
        }
    }
}
