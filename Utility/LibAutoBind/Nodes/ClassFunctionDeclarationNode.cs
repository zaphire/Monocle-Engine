using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassFunctionDeclarationNode : Node
    {
        private List<string> m_Keywords;
        private string m_Name;
        private string m_Type;
        private List<string> m_Arguments = new List<string>();

        internal ClassFunctionDeclarationNode(List<string> keywords, string type, string name)
        {
            this.m_Keywords = keywords;
            name = name.Trim('{').Trim();
            this.m_Type = type;

            // We need to split up the name into the actual function
            // name and the arguments.
            string[] s = name.Trim(')').Split(new char[] {'('}, 2);
            this.m_Name = s[0].Trim();
            foreach (string a in s[1].Split(','))
            {
                this.m_Arguments.Add(a.Trim());
            }
            if (this.m_Arguments.Count == 1 && this.m_Arguments[0] == "")
                this.m_Arguments.Clear();

            // Construct a representation for the Content property.
            string keys = "";
            foreach (string k in keywords)
                keys += k + " ";
            if (type == "")
            {
                // Must a built-in type.
                foreach (string k in keywords)
                    if (Keywords.CPPTypeKeywords.Contains(k) && Keywords.LuaTypeKeywords.Contains(k))
                        this.m_Type = k;
            }
            keys = keys.Trim();
            this.p_Content = keys + " " + name;
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

        internal string Type
        {
            get { return this.m_Type; }
        }

        internal List<string> Arguments
        {
            get { return this.m_Arguments; }
        }
    }
}
