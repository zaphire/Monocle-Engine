using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassDefinitionNode : Node
    {
        private string p_Class;
        private string p_ClassOnly;
        private string p_Inheritance;
        private string p_Alias;

        internal ClassDefinitionNode(string cls, string alias)
        {
            // Get inheritance.
            this.p_Inheritance = "RObject";
            if (alias.Contains(":"))
                throw new InvalidClassDefinitionException("The inheritance ':' specifier must be before the alias 'as' specifier.");
            if (cls.Contains(":"))
            {
                string[] m = cls.Split(new char[] { ':' }, 2);
                this.p_Class = m[0].Trim();
                this.p_Inheritance = m[1].Trim();
            }
            else
            {
                this.p_Class = cls.Trim();
            }

            // Interpret the class string to find the class name excluding namespaces.
            if (this.p_Class.LastIndexOf('.') != -1)
                this.p_ClassOnly = this.p_Class.Substring(this.p_Class.LastIndexOf('.') + 1);
            else
                this.p_ClassOnly = this.p_Class;

            // Set the alias variable.
            this.p_Alias = alias.Trim();
        }

        internal override string Content
        {
            get { return this.p_Class + " AS " + this.p_Alias; }
        }

        internal string Class
        {
            get { return this.p_Class; }
        }

        internal string Inheritance
        {
            get { return this.p_Inheritance; }
        }

        internal string ClassOnly
        {
            get { return this.p_ClassOnly; }
        }

        internal string Alias
        {
            get { return this.p_Alias; }
        }
    }
}
