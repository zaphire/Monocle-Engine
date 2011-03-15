using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind;
using System.IO;

namespace AutoBind
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("usage: autobind CPPBASE HBASE INPUT1 ... INPUTN");
                return 1;
            }

            string cppbase = args[0].Trim();
            string hbase = args[1].Trim();

            for (int a = 2; a < args.Length; a += 1)
            {
                string input = args[a].Trim();
                int start = input.LastIndexOf('\\') + 1;
                string basename = input.Substring(start);
                int end = basename.IndexOf('.');
                basename = basename.Substring(0, end);

                if (!Directory.Exists(hbase))
                    Directory.CreateDirectory(hbase);
                if (!Directory.Exists(cppbase))
                    Directory.CreateDirectory(cppbase);
                Machine m = new Machine(input, hbase + '/' + basename + ".h.tmp", cppbase + '/' + basename + ".cpp");
                try
                {
                    m.Run();
                }
                catch (LibAutoBind.InvalidClassDefinitionException e)
                {
                    Console.WriteLine("Unable to automatically bind " + input + ".  Make sure there");
                    Console.WriteLine("is only one class defined in the file.");
                    Program.Cleanup(hbase, cppbase, basename);
                    return 1;
                }
                catch (LibAutoBind.InvalidCharacterException e)
                {
                    Program.Cleanup(hbase, cppbase, basename);
                    return 1;
                }
                catch (LibAutoBind.LexingAbortedException e)
                {
                    Program.Cleanup(hbase, cppbase, basename);
                    return 1;
                }
                m.Close();

                // Check to see if the generated files are different from those currently
                // on disk.  We don't want to update the current files on disk because
                // the Visual C compiler only looks at the date on the file, which causes
                // lots of code compilation if headers are regenerated (even if they aren't
                // changed).
                bool copyh = false;
                if (!File.Exists(hbase + '/' + basename + ".h"))
                    copyh = true;
                else
                {
                    StreamReader ch = new StreamReader(hbase + '/' + basename + ".h");
                    StreamReader gh = new StreamReader(hbase + '/' + basename + ".h.tmp");
                    copyh = (ch.ReadToEnd() != gh.ReadToEnd());
                    ch.Close();
                    gh.Close();
                }
                Console.Write(" [cpp] ");
                if (copyh)
                {
                    File.Delete(hbase + '/' + basename + ".h");
                    File.Move(hbase + '/' + basename + ".h.tmp", hbase + '/' + basename + ".h");
                    Console.Write("[h  ] ");
                }
                Console.WriteLine();
                Program.Cleanup(hbase, cppbase, basename);
            }

            return 0;
        }

        static void Cleanup(string hbase, string cppbase, string basename)
        {
            try
            {
                if (File.Exists(hbase + '/' + basename + ".h.tmp"))
                    File.Delete(hbase + '/' + basename + ".h.tmp");
            }
            catch (IOException) { }
        }
    }
}
