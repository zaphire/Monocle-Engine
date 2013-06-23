using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class IgnoreNode : Node
    {
        internal IgnoreNode(string content)
        {
            this.p_Content = content;
        }
    }
}
