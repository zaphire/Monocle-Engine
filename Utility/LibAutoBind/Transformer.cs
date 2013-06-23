using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    abstract class Transformer
    {
        protected Machine m_M;

        abstract public void WriteHeaderFile(List<Node> nodes);
        abstract public void WriteCodeFile(List<Node> nodes);
    }
}
