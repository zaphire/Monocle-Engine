using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class GenericBracketNode : Node
    {
        internal GenericBracketNode(string content)
        {
            this.p_Content = content;
        }
    }
}
