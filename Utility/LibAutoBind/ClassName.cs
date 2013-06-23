using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class ClassName
    {
        internal static string ResolveToHeaderFilename(string cls)
        {
            string path = cls.Replace('.', '/') + ".h";
            if (path.StartsWith("Engine/Collections/") ||
                path.StartsWith("Engine/Library/") ||
                path.StartsWith("Engine/Unmanaged/"))
                path = "E." + path.Substring("Engine/".Length);
            else if (path.StartsWith("Engine/"))
                path = path.Substring("Engine/".Length);
            return path;
        }
    }
}
