using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using OpenTK;
using OpenTK.Graphics;


namespace Irit2Powerpoint
{
    using static WinAPIDef;
    using static WinAPIDclr;

    /* Wraps a WINAPI window and handles its events. */
    class WindowWrapper : OpenTK.GameWindow
    {
        #region InnerDefs
        private WinProc Proc, PrevProc;
        private GlRenderer Renderer;

        public WindowWrapper() : base(1, 1, new GraphicsMode(32, 24, 0, 16), "I2P")
        {

            IntPtr hWnd;
            this.WindowBorder = WindowBorder.Hidden;

            hWnd = this.WindowInfo.Handle;
            IntPtr PrevProcPtr = GetWindowLongPtr(hWnd, GWL_WNDPROC);
            PrevProc = (WinProc)Marshal.GetDelegateForFunctionPointer(PrevProcPtr, typeof(WinProc));
            Proc = new WinProc(WindowProc);

            // Swap out the winproc function.
            SetWindowLongPtr(hWnd, GWL_WNDPROC, Proc);
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            this.Visible = false;
            Renderer = new GlRenderer(this.Context);
        }
        #endregion

        int LastMouseX, LastMouseY;
        int dx, dy;
        bool IsMoving, IsRotating;

        /* Called when the window detects mouse movement. */
        public void OnMouseMove(int x, int y)
        {
            OpenTK.Matrix4 Trans, Tmp, Proj;
            OpenTK.Vector4 v;
            float Zoom, Width, Height;

            /* Handle the mouse moving */
            dx = x - LastMouseX;
            dy = y - LastMouseY;
            LastMouseX = x;
            LastMouseY = y;

            if (IsMoving)
            {
                Trans = Renderer.TransCtx.GetActiveWorld();
                Proj = Renderer.TransCtx.GetProjection();
                Width = Renderer.TransCtx.GetWidth();
                Height = Renderer.TransCtx.GetHeight();

                v = new OpenTK.Vector4(dx * (2.0f / Width), -dy * (2.0f / Height), 0, 0);
                v = Proj.Inverted() * v;

                Renderer.TransCtx.SetActiveModelView(Trans * OpenTK.Matrix4.CreateTranslation(v.Xyz));
            }

            /* Rotation is harder to get right... we'll just use an alright approximation.. */
            if (IsRotating)
            {
                Trans = Renderer.TransCtx.GetActiveWorld();
                Zoom = Renderer.TransCtx.GetZoom();
                Zoom = Math.Min(Zoom, 0.01f);
                Tmp = OpenTK.Matrix4.CreateRotationY(dx * Zoom) * OpenTK.Matrix4.CreateRotationX(dy * Zoom);
                Renderer.TransCtx.SetActiveModelView(Trans * Tmp);
            }
        }

        /* Called when the window detects a mouse press. */
        public void OnMousePress(MouseButton Button, MouseState State)
        {

            /* Handle a mouse press */
            if (State == MouseState.MOUSE_UP) {
                Renderer.TransCtx.SaveActiveModelView();
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
        public void SetPosition(int x, int y, int WinLeft, int WinTop)
        {
            X = x;
            Y = y;
        }

        public void SetSize(int w, int h)
        {
            Width = w;
            Height = h;
            Renderer.UpdateViewport(w, h);
        }

        public void SetVisibility(bool Visib)
        {
            Visible = Visib;
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
                    this.Renderer.Render();
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
                    this.Renderer.Render();
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
                    this.Renderer.Render();
                    break;
                case WM_PAINT:
                    this.Renderer.Render();
                    break;
                case WM_MOUSEWHEEL:
                    mouseWheel = (short)( ((long)wParam >> 16) & 0xFFFF);
                    OnMouseWheel(mouseWheel);
                    this.Renderer.Render();
                    break;
            }
            return PrevProc(hWnd, uMsg, wParam, lParam);
        }

        public GlRenderer GetRenderer()
        {
            return this.Renderer;
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
