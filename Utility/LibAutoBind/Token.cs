using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    abstract class Token
    {
        /// <summary>
        /// Called when the lexer is asking the token to detect whether
        /// or not the current text state is suitable for it's use.
        /// </summary>
        /// <param name="l">The lexer.</param>
        abstract internal void Detect(Lexer l);

        /// <summary>
        /// Called when the previous Detect() call has loaded the token
        /// into the parent stack and the lexer is now asking the token
        /// whether or not it wants to be unloaded as a parent (e.g the
        /// current code block has finished).
        /// </summary>
        /// <param name="l">The lexer.</param>
        /// <returns>Whether or not to unload the token as a parent.</returns>
        virtual internal bool DetectEnd(Lexer l) { return true; }
    }
}
