using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;

namespace Irit2Powerpoint
{
    class GlRenderer
    {
        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;
        private float k = 0;
        public GlRenderer(IntPtr hWnd)
        {

            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default, Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();

            GL.ClearColor(new Color4(1.0f, 0.0f, 0.0f, 0.0f));
            Render();
        }

        public void Render()
        {
            GL.ClearColor(new Color4(1.0f, k, 0.0f, 0.0f));
            GL.Clear(ClearBufferMask.ColorBufferBit);
            Context.SwapBuffers();
        }
    }
}
