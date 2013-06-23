using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using LibAutoBind.Transformers;

namespace LibAutoBind
{
    public class Machine
    {
        private string p_FilePath = "";
        private StreamReader p_InputFile = null;
        private StreamWriter p_OutputHFile = null;
        private StreamWriter p_OutputCFile = null;
        private Lexer p_Lexer = null;
        private Transformer p_Transformer = null;

        public Machine(string inpath, string outhpath, string outcpath)
        {
            this.p_InputFile = new StreamReader(inpath);
            this.p_OutputHFile = new StreamWriter(outhpath);
            this.p_OutputCFile = new StreamWriter(outcpath);
            this.p_FilePath = inpath;

            this.p_OutputHFile.NewLine = "\n";
            this.p_OutputCFile.NewLine = "\n";

            this.p_Lexer = new Lexer(this);
            this.p_Transformer = new LuaTransformer(this);
        }

        /// <summary>
        /// Run the Lexer and output data using the Transformer.
        /// </summary>
        public void Run()
        {
            this.p_Lexer.Run();
            this.p_Transformer.WriteHeaderFile(this.p_Lexer.Nodes);
            this.p_Transformer.WriteCodeFile(this.p_Lexer.Nodes);

            this.p_OutputCFile.Flush();
            this.p_OutputHFile.Flush();
        }

        /// <summary>
        /// Closes the files associated with the lexer.  Do not call
        /// Run() after calling this function.
        /// </summary>
        public void Close()
        {
            this.p_OutputCFile.Close();
            this.p_OutputHFile.Close();
            this.p_InputFile.Close();
        }

        /// <summary>
        /// The input file.
        /// </summary>
        public StreamReader InputFile
        {
            get { return this.p_InputFile; }
        }

        /// <summary>
        /// The output header (.h) file.
        /// </summary>
        public StreamWriter OutputHFile
        {
            get { return this.p_OutputHFile; }
        }

        /// <summary>
        /// The output code (.cpp) file.
        /// </summary>
        public StreamWriter OutputCFile
        {
            get { return this.p_OutputCFile; }
        }

        public string InputName
        {
            get
            {
                FileInfo f = new FileInfo(this.p_FilePath);
                return f.Name;
            }
        }

        public string InputPath
        {
            get
            {
                FileInfo f = new FileInfo(this.p_FilePath);
                return f.FullName;
            }
        }
    }

    public class InvalidClassDefinitionException : Exception
    {
        public InvalidClassDefinitionException() { }
        public InvalidClassDefinitionException(string message) : base(message) { }
        public InvalidClassDefinitionException(string message, Exception inner) : base(message, inner) { }
    }
    public class InvalidCharacterException : Exception
    {
        public InvalidCharacterException() { }
        public InvalidCharacterException(string message) : base(message) { }
        public InvalidCharacterException(string message, Exception inner) : base(message, inner) { }
    }
    public class LexingAbortedException : Exception
    {
        public LexingAbortedException() { }
        public LexingAbortedException(string message) : base(message) { }
        public LexingAbortedException(string message, Exception inner) : base(message, inner) { }
    }
}
