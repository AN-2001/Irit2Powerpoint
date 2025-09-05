using System;
using System.Runtime.InteropServices;
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
        private IntPtr hWnd, OwnerhWnd;

        public WindowWrapper(IntPtr PowerPoint) : base(1, 1, new GraphicsMode(new ColorFormat(32), 24, 0, 16), "I2P")
        {
            this.WindowBorder = WindowBorder.Hidden;

            hWnd = this.WindowInfo.Handle;

            IntPtr PrevProcPtr = GetWindowLongPtr(hWnd, GWL_WNDPROC);
            PrevProc = (WinProc)Marshal.GetDelegateForFunctionPointer(PrevProcPtr, typeof(WinProc));
            Proc = new WinProc(WindowProc);
            // Swap out the winproc function.
            SetWindowLongPtr(hWnd, GWL_WNDPROC, Proc);

            Logger.GetInstance().Trace($"Overwrote OpenTK winProc");

            Renderer = new GlRenderer(this.Context);

            SetVisibility(false);

            Logger.GetInstance().Trace("I2P window setup done.");
        }
        #endregion

        int LastMouseX, LastMouseY;
        int dx, dy;
        bool IsMoving, IsRotating;

        public void ChangeOwner(IntPtr Owner)
        {
            this.OwnerhWnd = Owner;
            SetWindowLongPtr(hWnd, -8, Owner);
            SetWindowPos(hWnd, Owner, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

            Logger.GetInstance().Trace("Changed GlWindow Owner.");
        }

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

        /* Clean up function. */
        public void Destroy()
        {
            Renderer.Destroy();
            Logger.GetInstance().Trace("Destroyed GlWindow");
        }

        #region InnerFunctions
        public void SetPosition(int x, int y, int WinLeft, int WinTop)
        {
            X = WinLeft + x;
            Y = WinTop + y;

            InvalidateRect(this.hWnd, IntPtr.Zero, false);
            Logger.GetInstance().Trace($"Changed GlWindow position to ({X}, {Y})");
        }

        public void SetSize(int w, int h)
        {
            Width = w;
            Height = h;
            Renderer.UpdateViewport(w, h);

            Logger.GetInstance().Trace($"Changed GlWindow size to ({Width}, {Height})");
            InvalidateRect(this.hWnd, IntPtr.Zero, false);
        }

        public void SetVisibility(bool Visib)
        {

            /* A bit janky but should hopefully eliminate flicker. */
            if (Visib)
                Renderer.ForceFetchAndRender();
            Visible = Visib;
            Logger.GetInstance().Trace($"Changed GlWindow visibility to {Visib}");
        }

        private IntPtr WindowProc(IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam)
        {
            MouseButton MouseButton;
            MouseState MouseState;
            int mouseWheel;

            switch (uMsg)
            {
                /* Tell Windows we already handled background clear, so it doesn't turn the screen black. */
                case WM_ERASEBKGND:
                    return (IntPtr)1;
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
                case WM_KEYUP:
                case WM_CHAR:
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_SYSCHAR:
                    SetFocus(this.OwnerhWnd);
                    PostMessage(this.OwnerhWnd, uMsg, wParam, lParam);
                    return IntPtr.Zero;
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
        
        #endregion
    }
}
