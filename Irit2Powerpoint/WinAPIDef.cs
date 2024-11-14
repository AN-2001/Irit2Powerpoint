using System;
using System.Runtime.InteropServices;

namespace Irit2Powerpoint
{
    public class WinAPIDef
    {
        public const int GWL_STYLE = -16;
        public const uint WS_CAPTION = 0x00C00000;     
        public const uint WS_THICKFRAME = 0x00040000;
        public const uint WS_POPUP = 0x80000000;
        public const uint WS_MINIMIZEBOX = 0x00020000; 
        public const uint WS_MAXIMIZEBOX = 0x00010000; 
        public const uint WS_SYSMENU = 0x00080000;     
        public const uint WS_OVERLAPPEDWINDOW = 0x00CF0000;
        public const uint WS_VISIBLE = 0x10000000;
        public const uint WS_CHILD = 0x40000000;
        public const uint WS_EX_OVERLAPPEDWINDOW = 0x00000300;
        public const uint WS_EX_DLGMODALFRAME = 0x00000001;
        public const uint WS_EX_TOPMOST = 0x00000008;
        public const uint WS_EX_TOOLWINDOW = 0x00000080;
        public const uint WS_EX_LAYERED = 0x00080000;
        public const int CW_USEDEFAULT = unchecked((int)0x80000000);
        public const uint SWP_NOSIZE = 0x0001;
        public const uint SWP_NOMOVE = 0x0002;
        public const uint SWP_NOACTIVATE = 0x0010;
        public const uint SWP_NOZORDER = 0x0004;
        public const uint SWP_SHOWWINDOW = 0x0040;
        public const int SW_HIDE = 0;
        public const int SW_SHOW = 5;
        public const int SW_SHOWNOACTIVATE = 4;
        public const int GWL_WNDPROC = -4;
        public const int SW_SHOWNORMAL = 1;
        public const uint WM_DESTROY = 0x0002;
        public const uint WM_MOUSEMOVE = 0x0200;
        public const uint WM_LBUTTONDOWN = 0x0201;
        public const uint WM_LBUTTONUP = 0x0202;
        public const uint WM_RBUTTONDOWN = 0x0204;
        public const uint WM_RBUTTONUP = 0x0205;
        public const uint WM_MBUTTONDOWN = 0x0207;
        public const uint WM_MBUTTONUP = 0x0208;
        public const uint WM_MOUSEWHEEL = 0x020A;
        public const uint WM_KEYDOWN = 0x0100;
        public const uint WM_KEYUP = 0x0101;
        public const uint WM_PAINT = 0x000F;




        [StructLayout(LayoutKind.Sequential)]
        public struct WNDCLASS
        {
            public uint style;
            public IntPtr lpfnWndProc;
            public int cbClsExtra;
            public int cbWndExtra;
            public IntPtr hInstance;
            public IntPtr hIcon;
            public IntPtr hCursor;
            public IntPtr hbrBackground;
            public string lpszMenuName;
            public string lpszClassName;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;
        }

    }
}
