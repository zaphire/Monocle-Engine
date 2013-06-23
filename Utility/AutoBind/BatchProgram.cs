using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind;
using System.Xml;
using System.IO;

namespace AutoBind
{
    class XmlNode
    {
        public string Name;
        public Dictionary<string, string> Attributes = new Dictionary<string,string>();
        public string Text;
    }

	class BatchProgram
	{
        static Dictionary<string, string> BuildConfiguration = null;

		static void Main(string[] args)
		{
            // Check to make sure the build configuration file exists.
            if (!File.Exists("./AutoBind.xml"))
            {
                BatchProgram.CompilerError("A0001", null);
                return;
            }

            // Read the build configuration.
            Dictionary<string, string> info = BatchProgram.GetBuildConfiguration();

            // Check to make sure all of the output / input folders exist.
            if (Directory.Exists(info["Input"]))
            {
                BatchProgram.CompilerError("A0002", new string[] { info["Input"] });
                return;
            }
            if (!Directory.Exists(info["OutputH"]))
            {
                BatchProgram.CompilerError("A0003", new string[] { info["OutputH"] });
                return;
            }
            if (!Directory.Exists(info["OutputC"]))
            {
                BatchProgram.CompilerError("A0004", new string[] { info["OutputC"] });
                return;
            }

            BatchProgram.BuildConfiguration = info;

            // Now perform the automatic binding process.
            BatchProgram.BindFolder(info["Input"], info["OutputH"], info["OutputC"]);
		}

        private static void BindFolder(string input, string outputh, string outputc)
        {
            input = input.TrimEnd('/');
            outputh = outputh.TrimEnd('/');
            outputc = outputc.TrimEnd('/');

            // Get the DirectoryInfo for the input folder.
            DirectoryInfo dir = new DirectoryInfo(input);

            // Convert directories.
            foreach (DirectoryInfo d in dir.GetDirectories())
            {
                string namespc = input.Substring(BuildConfiguration["Input"].Length).Replace('/', '.');
                Console.WriteLine(BatchProgram.ConvertAliases(namespc + "." + d.Name));
                if (!Directory.Exists(outputh + "/" + d.Name))
                    Directory.CreateDirectory(outputh + "/" + d.Name);
                if (!Directory.Exists(outputc + "/" + d.Name))
                    Directory.CreateDirectory(outputc + "/" + d.Name);
                BatchProgram.BindFolder(input + "/" + d.Name, outputh + "/" + d.Name, outputc + "/" + d.Name);
            }
            foreach (FileInfo f in dir.GetFiles("*.bpp"))
            {
                string namespc = input.Substring(BuildConfiguration["Input"].Length).Replace('/', '.');
                Console.WriteLine(BatchProgram.ConvertAliases(namespc + "." + f.Name.Substring(0, f.Name.Length - f.Extension.Length)));
                BatchProgram.BindFile(input + "/" + f.Name, outputh + "/" + f.Name, outputc + "/" + f.Name);
            }
        }

        private static void BindFile(string input, string outputh, string outputc)
        {
            Machine m = new Machine(input, outputh, outputc);
            m.Run();
            m.Close();
        }

        private static string ConvertAliases(string clsname)
        {
            if (clsname.StartsWith("E."))
            {
                clsname = "Engine." + clsname.Substring("E.".Length);
            }
            return clsname;
        }

        /// <summary>
        /// Gets the build configuration information from the AutoBind.xml
        /// document located in the current directory.
        /// </summary>
        /// <returns>Returns the build configuration.</returns>
        static Dictionary<string, string> GetBuildConfiguration()
        {
            // Open and read the parameters from the AutoBind.xml file
            // located in the current working directory.
            XmlReader xml = XmlReader.Create("./AutoBind.xml");
            Stack<XmlNode> stack = new Stack<XmlNode>();
            Dictionary<string, string> info = new Dictionary<string, string>();
            while (xml.Read())
            {
                XmlNode xnode = null;
                switch (xml.NodeType)
                {
                    case XmlNodeType.Element:
                        xnode = new XmlNode();
                        xnode.Name = xml.Name;

                        switch (xml.Name)
                        {
                            case "output":
                                xnode.Attributes.Add("type", xml.GetAttribute("type"));
                                break;
                        }

                        stack.Push(xnode);
                        break;
                    case XmlNodeType.Text:
                    case XmlNodeType.CDATA:
                        xnode = stack.Peek();
                        xnode.Text += xml.Value;
                        break;
                    case XmlNodeType.EndElement:
                        xnode = stack.Pop();

                        // Handle nodes.
                        if (xnode.Name == "input")
                            info.Add("Input", xnode.Text);
                        else if (xnode.Name == "output")
                        {
                            if (xnode.Attributes["type"] == "code")
                                info.Add("OutputC", xnode.Text);
                            else if (xnode.Attributes["type"] == "headers")
                                info.Add("OutputH", xnode.Text);
                        }

                        break;
                }
            }
            return info;
        }

        static void CompilerError(string id, string[] args)
        {
            string msg = CompileErrorList.Default.Properties[id].DefaultValue.ToString();
            if (args != null)
            {
                for (int i = 0; i < args.Length; i++)
                    msg = msg.Replace("${" + (i + 1).ToString() + "}", args[i]);
            }
            Console.WriteLine("error " + id + ": " + msg);
        }
	}
}
