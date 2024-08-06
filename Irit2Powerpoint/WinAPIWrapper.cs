using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Timers;

namespace Irit2Powerpoint
{
    using static WinAPIDef;
    using static WinAPIDclr;

    /* Wraps a WINAPI window and handles its events. */
    class WindowWrapper 
    {
        #region InnerDefs
        private IntPtr hWnd;
        private WinProc Proc;
        private GlRenderer Renderer;
        private int x, y, w, h;
        public WindowWrapper()
        {
            Proc = new WinProc(WindowProc);
            x = y = 0;
            w = 200;
            h = 100;

            // Create the window, only this class works for some reason.
            this.hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                                       "#32770", "",
                                       WS_VISIBLE | WS_POPUP, // Style
                                       x, y, // position
                                       w, h, // Size
                                       IntPtr.Zero,
                                       IntPtr.Zero,
                                       GetModuleHandle(null), IntPtr.Zero);
            if (this.hWnd == IntPtr.Zero)
                MessageBox.Show("Failed to create window. Error code: " + Marshal.GetLastWin32Error());

            // Swap out the winproc function.
            SetWindowLongPtr(hWnd, GWL_WNDPROC, Proc);

            ShowWindow(this.hWnd, SW_HIDE);
            Renderer = new GlRenderer(this.hWnd);
        }
        #endregion

        /* Called when the window detects mouse movement. */
        public void OnMouseMove(int x, int y)
        {
            /* Handle the mouse moving */
        }

        /* Called when the window detects a mouse press. */
        public void OnMousePress(MouseButton Button, MouseState State)
        {
            /* Handle a mouse press */
        }

        /* Called when the window detects a key press. */

        public void OnKey(char Key, KeyState State)
        {
            /* Handle a key press. */
        }

        /* Clean up function. */
        public void Destroy()
        {
            Renderer.Destroy();
        }

        #region InnerFunctions
        public void SetPosition(int x, int y)
        {
            this.x = x;
            this.y = y;
            SetWindowPos(hWnd, IntPtr.Zero, x, y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
            InvalidateRect(this.hWnd, IntPtr.Zero, true);
        }

        public void SetSize(int w, int h)
        {
            this.w = w;
            this.h = h;

            SetWindowPos(hWnd, IntPtr.Zero, 0, 0, w, h, SWP_NOMOVE | SWP_NOACTIVATE);
            Renderer.UpdateViewport(w, h);
            InvalidateRect(this.hWnd, IntPtr.Zero, true);
        }

        public void SetActiveModel(string Filepath)
        {
            Renderer.SetActiveModel(Filepath);
        }

        public void SetVisibility(bool Visib)
        {
            ShowWindow(this.hWnd, Visib ? SW_SHOW : SW_HIDE);
            InvalidateRect(this.hWnd, IntPtr.Zero, true);
        }
        private IntPtr WindowProc(IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam)
        {
            MouseButton MouseButton;
            MouseState MouseState;
            KeyState KeyState;
            char Key;

            switch (uMsg)
            {
                case WM_MOUSEMOVE:
                    OnMouseMove(LOWORD(lParam), HiWORD(lParam));
                    InvalidateRect(this.hWnd, IntPtr.Zero, true);
                    break;
                case WM_LBUTTONDOWN:
                    MouseState = MouseState.MOUSE_DOWN;
                    MouseButton = MouseButton.MOUSE_BUTTON_LEFT;
                    goto MOUSE_BUTTON_EVENT;
                case WM_LBUTTONUP:
                    MouseState = MouseState.MOUSE_UP;
                    MouseButton = MouseButton.MOUSE_BUTTON_LEFT;
                    goto MOUSE_BUTTON_EVENT;
                case WM_RBUTTONDOWN:
                    MouseState = MouseState.MOUSE_DOWN;
                    MouseButton = MouseButton.MOUSE_BUTTON_RIGHT;
                    goto MOUSE_BUTTON_EVENT;
                case WM_RBUTTONUP:
                    MouseState = MouseState.MOUSE_UP;
                    MouseButton = MouseButton.MOUSE_BUTTON_RIGHT;
                    goto MOUSE_BUTTON_EVENT;
                case WM_MBUTTONDOWN:
                    MouseState = MouseState.MOUSE_DOWN;
                    MouseButton = MouseButton.MOUSE_BUTTON_MIDDLE;
                    goto MOUSE_BUTTON_EVENT;
                case WM_MBUTTONUP:
                    MouseState = MouseState.MOUSE_UP;
                    MouseButton = MouseButton.MOUSE_BUTTON_MIDDLE;
                MOUSE_BUTTON_EVENT:
                    OnMousePress(MouseButton, MouseState);
                    InvalidateRect(this.hWnd, IntPtr.Zero, true);
                    break;
                case WM_KEYDOWN:
                    KeyState = KeyState.KEY_DOWN;
                    goto KEY_EVENT;
                case WM_KEYUP:
                    KeyState = KeyState.KEY_UP;
                    goto KEY_EVENT;
                KEY_EVENT:
                    Key = (char)wParam;
                    OnKey(Key, KeyState);
                    InvalidateRect(this.hWnd, IntPtr.Zero, true);
                    break;
                case WM_PAINT:
                    this.Renderer.Render();
                    ValidateRect(this.hWnd, IntPtr.Zero);
                    break;
            }
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        public enum MouseButton
        {
            MOUSE_BUTTON_LEFT,
            MOUSE_BUTTON_RIGHT,
            MOUSE_BUTTON_MIDDLE
        }

        public enum MouseState
        {
            MOUSE_UP,
            MOUSE_DOWN
        }
        
        public enum KeyState
        {
            KEY_UP,
            KEY_DOWN
        }

        #endregion
    }
}
