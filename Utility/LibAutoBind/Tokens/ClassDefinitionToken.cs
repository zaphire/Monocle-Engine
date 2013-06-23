using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;
using System.Text.RegularExpressions;

namespace LibAutoBind.Tokens
{
    class ClassDefinitionToken : Token
    {
        public ClassDefinitionToken() { }

        internal override void Detect(Lexer l)
        {
            if (l.Text.TrimStart().StartsWith("class ") && !l.HasOwnership())
                l.TakeOwnership();
            else if (!l.HasOwnership() && !"class ".StartsWith(l.Text.TrimStart()))
                l.ForceExclude();

            if (l.HasOwnership())
            {
                if (l.Char == '{')
                {
                    string n = l.Text.TrimStart().Substring("class ".Length, l.Text.TrimStart().Length - "class ".Length - 1);
                    Regex r = new Regex("[ \n\r]as[ \n\r]", RegexOptions.Multiline);
                    Match m = r.Match(n);
                    if (m.Success)
                    {
                        l.AddNode(new ClassDefinitionNode(
                            n.Substring(0, m.Index),
                            n.Substring(m.Index + m.Length)
                            ));
                    }
                    else
                    {
                        l.AddNode(new ClassDefinitionNode(
                            n,
                            ""
                            ));
                    }
                    l.AddParent();
                    l.EndOwnership();
                }
                else if (l.Char == ';')
                {
                    l.AddNode(new DirectNode(l.Text.Substring(0, l.Text.Length - 1)));
                    l.EndOwnership();
                }
            }
        }

        internal override bool DetectEnd(Lexer l)
        {
            if (l.Char == '}' && l.Text.Trim() == "}") // Forces the end detection to only work when it's got a single } at the end.
                                                       // Ensures that we don't discard any data that we might want.
            {
                // HACK: Excludes the end bracket for the class because more transformers
                //       don't need to know about it, and it frigs with the DirectNode
                //       compactor.
                //l.AddNode(new DirectNode(l.Text));
                return true;
            }

            return false;
        }
    }
}
