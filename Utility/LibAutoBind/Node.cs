using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class Node
    {
        protected string p_Content;
        protected int p_ParentCount;
        private int p_LineNumber;
        private string p_FileName;

        internal virtual string Content
        {
            get { return this.p_Content; }
        }

        internal virtual int ParentCount
        {
            get { return this.p_ParentCount; }
            set { this.p_ParentCount = value; }
        }

        internal virtual int LineNumber
        {
            get
            {
                return this.p_LineNumber;
            }
            set
            {
                this.p_LineNumber = value;
            }
        }

        internal virtual string FileName
        {
            get
            {
                return this.p_FileName;
            }
            set
            {
                this.p_FileName = value;
            }
        }
    }
}
