using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class DirectNode : Node
    {
        internal DirectNode(string content)
        {
            this.p_Content = content;
        }

        internal void SetContent(string content)
        {
            this.p_Content = content;
        }
    }
}
