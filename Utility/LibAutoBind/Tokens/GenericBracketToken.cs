using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class GenericBracketToken : Token
    {
        public GenericBracketToken() { }

        internal override void Detect(Lexer l)
        {
            if (l.Char == '{' && !(l.GetParent() is ClassDefinitionToken))
            {
                l.TakeOwnership();
                l.AddNode(new DirectNode(l.Text));
                l.AddParent();
                l.EndOwnership();
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
