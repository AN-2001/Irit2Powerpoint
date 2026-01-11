using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Irit2Powerpoint
{
    class ParseException : Exception
    {
        public ParseException(string Message) : base(Message) { }
    }

    class StillLoadingException : Exception
    {
        public StillLoadingException() : base("Geometry is still loading") { }
    }

    class UnknownKeyException : Exception
    {
        public UnknownKeyException() : base("Attempting to fetch an unknown key") { }
    }


}
