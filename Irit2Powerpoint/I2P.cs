using System;
using PowerPoint = Microsoft.Office.Interop.PowerPoint;
using Office = Microsoft.Office.Core;
using System.Windows.Forms;
using Microsoft.Office.Core;
using System.Drawing;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;

namespace Irit2Powerpoint
{


    public partial class I2P
    {
        private const string __PATH_KEY__ = "_I2P_PATH_";
        private const string __RENDER_SETTINGS_KEY__ = "_I2P_RENDER_SETTINGS_";
        private const string __IMPORT_SETTINGS_KEY__ = "_I2P_IMPORT_SETTINGS_";
        private const string __DUMMY_KEY__ = "_I2P_DUMMY_";
        private WindowWrapper GlWindow;
        private GlResourceManager ResourceManager;
        private Ribbon RibbonControl;
        private Timer RibbonUpdateTimer;

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            /* Bind the event handlers. */
            this.Application.SlideShowBegin += NewSlideShow;
            this.Application.SlideShowNextSlide += SlideShowOnSlide;
            this.Application.SlideShowOnNext += SlideShowOnSlide;
            this.Application.SlideShowOnPrevious += SlideShowOnSlide;
            this.Application.SlideShowEnd += EndSlideShow;
            this.Application.SlideSelectionChanged += SlideChanged;
            this.Application.AfterPresentationOpen += OnPresentation;

            ResourceManager = new GlResourceManager();

            /* Reuse the same GLWindow accross different slides. */
            GlWindow = new WindowWrapper();
            RibbonUpdateTimer = new Timer();
            RibbonUpdateTimer.Interval = 100;
            RibbonUpdateTimer.Tick += OnRibbonTimer;
            RibbonUpdateTimer.Start();
        }

        public void OnRibbonTimer(Object obj, EventArgs Args)
        {
            RibbonControl.Refresh();
        }

        public void OnPresentation(PowerPoint.Presentation Presentation)
        {
            List<string> PathsInUse = GetPathsInUse();
            LoadRequest Request;

            Request = new LoadRequest();
            foreach (string Str in PathsInUse)
            {
                Request.Path = Str;
                ResourceManager.QueueLoadFromDisk(Request);
            }
        }

        public static GlResourceManager GetResourceManager()
        {
            return Globals.I2P.ResourceManager;
        }


        private List<string> GetPathsInUse()
        {
            List<String> Paths = new List<string>();
            int i;
            PowerPoint.Shape Dummy;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
            {
                if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                out Dummy))
                {
                    Paths.Add(Dummy.Tags[__PATH_KEY__]);
                }
            }
            return Paths;
        }

        public void SlideChanged(PowerPoint.SlideRange SldRange)
        {
            int i;
            PowerPoint.Shape Dummy;

            /* Make sure everything is visible as we hide them after a slideshow. */
            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++) { 
                if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                out Dummy))
                    Dummy.Visible = MsoTriState.msoTrue;
            }
            ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
        }

        public bool ActiveSlideContainsDummy()
        {
            PowerPoint.Shape Dummy;
            try
            {
                return GetDummyFromSlide(Application.ActiveWindow.View.Slide, out Dummy);
            } catch (Exception)
            {
                return false;
            }
        }

        public void InitDummyRect(string Path)
        {
            LoadRequest Request;
            PowerPoint.Shape Dummy;
            PowerPoint.Slide
                Slide = Application.ActiveWindow.View.Slide;
           
            if (GetDummyFromSlide(Slide, out Dummy)) {
                Dummy.Tags.Add( __PATH_KEY__ , Path);
                Request = new LoadRequest();
                Request.Path = Path;
                ResourceManager.QueueLoadFromDisk(Request);
                ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
                return;
            }

            ITDParser.ImportSettings ImportSettings = ITDParser.DefaultImportSettings;
            GlRenderer.RenderSettings RenderSettings = GlRenderer.DefaultRenderSettings;

            Dummy = Slide.Shapes.AddShape(Office.MsoAutoShapeType.msoShapeRectangle ,0, 0, 400, 300);
            Dummy.TextFrame.TextRange.Text = "IRIT2POWERPOINT DUMMY";
            Dummy.Tags.Add(__DUMMY_KEY__ , "true");
            Dummy.Tags.Add(__PATH_KEY__, Path);
            Dummy.Tags.Add(__IMPORT_SETTINGS_KEY__, ITDParser.SerializeImportSettings(ImportSettings));
            Dummy.Tags.Add(__RENDER_SETTINGS_KEY__, GlRenderer.SerializeRenderSettings(RenderSettings));
            Dummy.Fill.ForeColor.RGB = Color.FromArgb(100, 100, 100).ToArgb();

            Request = new LoadRequest();
            Request.Path = Path;
            Request.ImportSettings = ITDParser.DefaultImportSettings;
            Request.RenderSettings = GlRenderer.DefaultRenderSettings;
            ResourceManager.QueueLoadFromDisk(Request);
            ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
        }

        /* Extracts the Irit2Powerpoint dummy from a slide. */
        private bool GetDummyFromSlide(PowerPoint.Slide Slide,
                                       out PowerPoint.Shape Dummy)
        {
            int i, j;

            Dummy = null;

            /* It's n*m but n, m are relatively low (obvsly less than a 100) */
            for (i = 1; i <= Slide.Shapes.Count; i++)
                for (j = 1; j <= Slide.Shapes[i].Tags.Count; j++)
                {
                    if (Slide.Shapes[i].Tags.Name(j) == __DUMMY_KEY__)
                    {
                        Dummy = Slide.Shapes[i];
                        return true;
                    }
                }

            return false;
        }

        void NewSlideShow(PowerPoint.SlideShowWindow Wn)
        {
            ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
        }

        void EndSlideShow(PowerPoint.Presentation Pres)
        {
            /* Once a slide show ends hide the GL window. */
            GlWindow.SetVisibility(false);
            ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
        }

        /* Helpers to use to position the window. */
        int PointToPixelX(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)(Point * g.DpiX / 72.0f);
            }
        }

        /* Helpers to use to position the window. */
        int PointToPixelY(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)(Point * g.DpiY / 72.0f);
            }
        }

        /* Called when we go to a new slide in a slideshow. */
        void SlideShowOnSlide(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy;
            int x, y, w, h;
            string Path;

            // MessageBox.Show("NEW SLIDE!");

            /* Check for the dummy and if it exists determine the size/position of the  */
            /* GLWindow from it. */
            if (GetDummyFromSlide(Wn.View.Slide, out Dummy))
            {
                Dummy.Visible = MsoTriState.msoFalse;

                /* Calculate the window's shape and send it to the renderer. */
                x = PointToPixelX(Dummy.Left * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                y = PointToPixelY(Dummy.Top * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                w = PointToPixelX(Dummy.Width * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);
                h = PointToPixelY(Dummy.Height * (Wn.View.Zoom / 100f), (IntPtr)Wn.HWND);

                GlWindow.SetPosition(x, y);
                GlWindow.SetSize(w, h);
                GlWindow.SetVisibility(true);

                Path = Dummy.Tags[__PATH_KEY__];
                GlWindow.GetRenderer().SetActiveModel(Path);
            } else
                GlWindow.SetVisibility(false);

            ResourceManager.ConsistencyCleanup(GetPathsInUse().ToArray());
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            GlWindow.Destroy();
            ResourceManager.Destroy();
            RibbonUpdateTimer.Stop();
        }

        protected override Microsoft.Office.Core.IRibbonExtensibility CreateRibbonExtensibilityObject()
        {
            RibbonControl = new Ribbon();
            return RibbonControl;
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
