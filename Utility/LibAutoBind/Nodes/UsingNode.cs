using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class UsingNode : Node
    {
        internal UsingNode(string content)
        {
            this.p_Content = content;
        }
    }
}
