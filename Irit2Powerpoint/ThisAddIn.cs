using System;
using PowerPoint = Microsoft.Office.Interop.PowerPoint;
using Office = Microsoft.Office.Core;
using System.Windows.Forms;
using Microsoft.Office.Core;
using System.Drawing;

namespace Irit2Powerpoint
{
    public partial class ThisAddIn
    {
        private WindowWrapper GlWindow;

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            this.Application.SlideShowBegin += NewSlideShow;
            this.Application.SlideShowNextSlide += NextSlide;
            this.Application.SlideShowEnd += EndSlideShow;
            this.Application.SlideSelectionChanged += SlideChanged;
            GlWindow = new WindowWrapper();
        }
        public void SlideChanged(PowerPoint.SlideRange SldRange)
        {
            int i;
            PowerPoint.Shape Dummy, File;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++) { 
                if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                        out Dummy, out File))
                    Dummy.Visible = MsoTriState.msoTrue;
             }
        }
        public void InitDummyRect(string Path)
        {
            PowerPoint.Shape Dummy, File;
            PowerPoint.Slide
                Slide = Application.ActiveWindow.View.Slide;

            if (GetDummyFromSlide(Slide, out Dummy, out File)) {
                MessageBox.Show("I2P: Window already exists.");
                return;
            }

            Dummy = Slide.Shapes.AddShape(Office.MsoAutoShapeType.msoShapeRectangle ,0, 0, 400, 300);
            Dummy.TextFrame.TextRange.Text = "IRIT2POWERPOINT DUMMY";
            Dummy.Tags.Add("I2PDUMMY", "true");
            Dummy.Fill.ForeColor.RGB = Color.FromArgb(100, 100, 100).ToArgb();
            try
            {
                File = Slide.Shapes.AddOLEObject(Left: 0, Top: 0, Width: 100, Height: 100,
                                     FileName: Path, Link: Microsoft.Office.Core.MsoTriState.msoFalse,
                                    DisplayAsIcon: Microsoft.Office.Core.MsoTriState.msoTrue);
            } catch (Exception e)
            {
                MessageBox.Show("I2P: Couldn't embed the file " + e.Message);
            }

            File.Visible = Microsoft.Office.Core.MsoTriState.msoFalse;
            File.Tags.Add("I2PDUMMY_FILE", "true");
        }

        private bool GetDummyFromSlide(PowerPoint.Slide Slide,
                                       out PowerPoint.Shape Dummy,
                                       out PowerPoint.Shape File)
        {
            int i, j,
                NumExtracted = 0;

            Dummy = File = null;

            /* It's n*m but n, m are relatively low (obvsly less than a 100) */
            for (i = 1; i <= Slide.Shapes.Count; i++)
                for (j = 1; j <= Slide.Shapes[i].Tags.Count; j++)
                {
                    if (Slide.Shapes[i].Tags.Name(j) == "I2PDUMMY")
                    {
                        Dummy = Slide.Shapes[i];
                        NumExtracted++;
                    }

                    if (Slide.Shapes[i].Tags.Name(j) == "I2PDUMMY_FILE")
                    {
                        File = Slide.Shapes[i];
                        NumExtracted++;
                    }
                    if (NumExtracted == 2)
                        return true;
                }

            if (NumExtracted == 1)
                MessageBox.Show("I2P: Detected corrupted slide, aborting. ");
            return false;
        }

        void NewSlideShow(PowerPoint.SlideShowWindow Wn)
        {
        }

        void EndSlideShow(PowerPoint.Presentation Pres)
        {
            GlWindow.SetVisibility(false);
        }

        int PointToPixelX(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)(Point * g.DpiX / 72.0f);
            }
        }

        int PointToPixelY(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)(Point * g.DpiY / 72.0f);
            }
        }

        void NextSlide(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy, File;
            int x, y, w, h;

            if (GetDummyFromSlide(Wn.View.Slide, out Dummy, out File))
            {
                Dummy.Visible = MsoTriState.msoFalse;

                x = PointToPixelX(Dummy.Left * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                y = PointToPixelY(Dummy.Top * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                w = PointToPixelX(Dummy.Width * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                h = PointToPixelY(Dummy.Height * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);

                GlWindow.SetPosition(x, y);
                GlWindow.SetSize(w, h);
                GlWindow.SetVisibility(true);
            }
            else
                GlWindow.SetVisibility(false) ;

        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
        }

        protected override Microsoft.Office.Core.IRibbonExtensibility CreateRibbonExtensibilityObject()
        {
            return new Ribbon();
        }


        #region VSTO generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InternalStartup()
        {
            this.Startup += new System.EventHandler(ThisAddIn_Startup);
            this.Shutdown += new System.EventHandler(ThisAddIn_Shutdown);
        }
        
        #endregion
    }
}
