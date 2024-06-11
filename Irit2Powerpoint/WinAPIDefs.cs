using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Irit2Powerpoint
{
    public class WinAPIDefs
    {
        public const int GWL_STYLE = -16;
        public const int WS_CAPTION = 0x00C00000;     
        public const int WS_THICKFRAME = 0x00040000;  
        public const int WS_MINIMIZEBOX = 0x00020000; 
        public const int WS_MAXIMIZEBOX = 0x00010000; 
        public const int WS_SYSMENU = 0x00080000;     
        public const int WS_OVERLAPPEDWINDOW = 0x00CF0000;
        public const int WS_VISIBLE = 0x10000000;
        public const int WS_EX_OVERLAPPEDWINDOW = 0x00000300;
        public const int WS_EX_TOPMOST = 0x00000008;
        public const int CW_USEDEFAULT = unchecked((int)0x80000000);
        public const uint WM_MOUSEMOVE = 0x0200;
    }
}
