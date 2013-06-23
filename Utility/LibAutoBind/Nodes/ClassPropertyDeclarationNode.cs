using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace LibAutoBind.Nodes
{
    class ClassPropertyDeclarationNode : Node
    {
        private List<string> m_Keywords;
        private string m_Name;
        private string m_Get = null;
        private string m_Set = null;
        private bool m_IsGetFunc = false;
        private bool m_IsSetFunc = false;

        internal ClassPropertyDeclarationNode(List<string> keywords, string name, string content)
        {
            this.m_Keywords = keywords;
            this.m_Name = name;

            // Now search for the get / set fields within the content.
            Regex sr = new Regex("set[ \r\n\t]*\\:[ \r\n\t]*(?<SetVal>[a-zA-Z0-9_ \r\n\t]*)\\;", RegexOptions.Multiline);
            Regex gr = new Regex("get[ \r\n\t]*\\:[ \r\n\t]*(?<GetVal>[a-zA-Z0-9_ \r\n\t]*)\\;", RegexOptions.Multiline);
            Match sm = sr.Match(content);
            Match gm = gr.Match(content);
            if (sm.Success)
                this.m_Set = sm.Groups["SetVal"].Value.Trim();
            if (gm.Success)
                this.m_Get = gm.Groups["GetVal"].Value.Trim();

            // Check to see whether or not they are function calls.
            Regex fc = new Regex("^[ \r\n\t]*call[ \r\n\t]*(?<Val>[a-zA-Z0-9_]*)[ \r\n\t]*$");
            if (this.m_Set != null)
            {
                Match sf = fc.Match(this.m_Set);
                if (sf.Success)
                {
                    this.m_IsSetFunc = true;
                    this.m_Set = sf.Groups["Val"].Value.Trim();
                }
                else
                    this.m_IsSetFunc = false;
            }
            else
                this.m_IsSetFunc = true; // This is a read-only property.
            if (this.m_Get != null)
            {
                Match gf = fc.Match(this.m_Get);
                if (gf.Success)
                {
                    this.m_IsGetFunc = true;
                    this.m_Get = gf.Groups["Val"].Value.Trim();
                }
                else
                    this.m_IsGetFunc = false;
            }

            // Create a string representation for the Content field.
            string keys = "";
            foreach (string s in keywords)
                keys += s + " ";
            keys = keys.Trim();
            this.p_Content = keys + " " + name + "{" + content + "}";
        }

        internal string GetVal
        {
            get { return this.m_Get; }
        }

        internal string SetVal
        {
            get { return this.m_Set; }
        }

        internal bool GetIsFunc
        {
            get { return this.m_IsGetFunc; }
        }

        internal bool SetIsFunc
        {
            get { return this.m_IsSetFunc; }
        }

        internal List<string> AllKeywords
        {
            get { return this.m_Keywords; }
        }

        internal List<string> CPPKeywords
        {
            get
            {
                List<string> ret = new List<string>();
                foreach (string k in this.m_Keywords)
                {
                    if (!Keywords.LuaDeclKeywords.Contains(k) && !Keywords.LuaVisibilityKeywords.Contains(k))
                    {
                        ret.Add(k);
                    }
                    else if (k == "bound")
                        ret.Add("public");
                }
                return ret;
            }
        }

        internal string Name
        {
            get { return this.m_Name; }
        }
    }
}
