using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Diagnostics;


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

        int LastMouseX, LastMouseY;
        int dx, dy;
        bool IsMoving, IsRotating;

        /* Called when the window detects mouse movement. */
        public void OnMouseMove(int x, int y)
        {
            OpenTK.Matrix4 Trans, Tmp;
            float Zoom;

            /* Handle the mouse moving */
            dx = x - LastMouseX;
            dy = y - LastMouseY;
            LastMouseX = x;
            LastMouseY = y;

            if (IsMoving)
            {
                Trans = Renderer.TransCtx.GetActiveWorld();
                Zoom = Renderer.TransCtx.GetZoom();
                Renderer.TransCtx.SetActiveWorld(Trans * OpenTK.Matrix4.CreateTranslation(dx * Zoom, -dy * Zoom, 0));
            }

            if (IsRotating)
            {
                Trans = Renderer.TransCtx.GetActiveWorld();
                Zoom = Renderer.TransCtx.GetZoom();
                Zoom = Math.Min(Zoom, 0.01f);
                Tmp = OpenTK.Matrix4.CreateRotationY(dx * Zoom) * OpenTK.Matrix4.CreateRotationX(dy * Zoom);
                Renderer.TransCtx.SetActiveWorld(Trans * Tmp);
            }
        }

        /* Called when the window detects a mouse press. */
        public void OnMousePress(MouseButton Button, MouseState State)
        {

            /* Handle a mouse press */
            if (State == MouseState.MOUSE_UP) {
                Renderer.TransCtx.SaveActiveWorld();
                IsMoving = false;
                IsRotating = false;
                return;
            }


            if (Button == MouseButton.MOUSE_BUTTON_RIGHT)
                IsMoving = true;
            if (Button == MouseButton.MOUSE_BUTTON_LEFT)
                IsRotating = true;
        }

        /* Called when the mouse wheel moves. */
        public void OnMouseWheel(int Direction)
        {
            /* Handle a mosue wheel move. */
            Renderer.TransCtx.SetZoom(Direction);
        }

        /* Called when the window detects a key press. */

        public void OnKey(char Key, KeyState State)
        {
            /* Handle a key press. */
            if (Key == 'R' && State == KeyState.KEY_DOWN)
                Renderer.TransCtx.Reset();
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
            int mouseWheel;

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
                case WM_MOUSEWHEEL:
                    mouseWheel = (short)( ((long)wParam >> 16) & 0xFFFF);
                    OnMouseWheel(mouseWheel);
                    InvalidateRect(this.hWnd, IntPtr.Zero, true);
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
