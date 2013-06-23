using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class IncludeNode : Node
    {
        internal IncludeNode(string content)
        {
            this.p_Content = content;
        }
    }
}
