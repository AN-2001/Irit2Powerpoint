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
        private ITDParser.ImportSettings CurrentImportSettings;
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
            CurrentImportSettings = ITDParser.DefaultImportSettings;

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

        public ITDParser.ImportSettings GetCurrentImportSettings()
        {
            return CurrentImportSettings;
        }

        public void SetCurrentImportSettings(ITDParser.ImportSettings ImportSettings)
        {
            CurrentImportSettings = ImportSettings;
        }

        public void OnPresentation(PowerPoint.Presentation Presentation)
        {
            List<LoadRequest> RequestsInUse = GetLoadRequestsInUse();
            foreach (LoadRequest Request in RequestsInUse)
                ResourceManager.QueueLoadFromDisk(Request);
        }

        public static GlResourceManager GetResourceManager()
        {
            return Globals.I2P.ResourceManager;
        }

        public GlRenderer.RenderSettings GetRenderSettingsFromActiveSlide()
        {
            PowerPoint.Shape Dummy;
            if( GetDummyFromSlide(Application.ActiveWindow.View.Slide, out Dummy)) 
                return GlRenderer.DeserializeRenderSettings(Dummy.Tags[__RENDER_SETTINGS_KEY__]);
            return GlRenderer.DefaultRenderSettings;
        }

        public void SetRenderSettingsInActiveSlide(GlRenderer.RenderSettings Settings)
        {
            PowerPoint.Shape Dummy;
            if (GetDummyFromSlide(Application.ActiveWindow.View.Slide, out Dummy)) 
                Dummy.Tags.Add(__RENDER_SETTINGS_KEY__, GlRenderer.SerializeRenderSettings(Settings));
        }
        private List<LoadRequest> GetLoadRequestsInUse()
        {
            List<LoadRequest> Requests = new List<LoadRequest>();
            LoadRequest Request;
            int i;
            PowerPoint.Shape Dummy;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
            {
                if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                out Dummy))
                {
                    Request.Path = Dummy.Tags[__PATH_KEY__];
                    Request.ImportSettings = ITDParser.DeserializeImportSettings(Dummy.Tags[__IMPORT_SETTINGS_KEY__]);
                    Requests.Add(Request);
                }
            }
            return Requests;
        }

        private List<string> GetKeysInUse()
        {
            List<String> Keys = new List<string>();
            int i;
            string Key, Path, Settings;
            PowerPoint.Shape Dummy;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
            {
                if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                out Dummy))
                {
                    Path = Dummy.Tags[__PATH_KEY__];
                    Settings = Dummy.Tags[__IMPORT_SETTINGS_KEY__];

                    Key = ResourceManager.BuildResourceKey(Path, Settings);
                    Keys.Add(Key);
                }
            }
            return Keys;
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
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
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

            ITDParser.ImportSettings ImportSettings = CurrentImportSettings;
            GlRenderer.RenderSettings RenderSettings = GlRenderer.DefaultRenderSettings;
           
            if (GetDummyFromSlide(Slide, out Dummy)) {
                Dummy.Tags.Add( __PATH_KEY__ , Path);
                Dummy.Tags.Add(__IMPORT_SETTINGS_KEY__, ITDParser.SerializeImportSettings(ImportSettings));
                Request = new LoadRequest();
                Request.Path = Path;
                Request.ImportSettings = ImportSettings;
                ResourceManager.QueueLoadFromDisk(Request);
                ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
                return;
            }

            Dummy = Slide.Shapes.AddShape(Office.MsoAutoShapeType.msoShapeRectangle, 0, 0, 400, 300);
            Dummy.TextFrame.TextRange.Text = "IRIT2POWERPOINT";
            Dummy.TextFrame.TextRange.Font.Bold = MsoTriState.msoTrue;
            Dummy.Tags.Add(__DUMMY_KEY__ , "true");
            Dummy.Tags.Add(__PATH_KEY__, Path);
            Dummy.Tags.Add(__IMPORT_SETTINGS_KEY__, ITDParser.SerializeImportSettings(ImportSettings));
            Dummy.Tags.Add(__RENDER_SETTINGS_KEY__, GlRenderer.SerializeRenderSettings(RenderSettings));
            Dummy.Fill.ForeColor.RGB = Color.FromArgb(100, 100, 100).ToArgb();

            Request = new LoadRequest();
            Request.Path = Path;
            Request.ImportSettings = ImportSettings;
            ResourceManager.QueueLoadFromDisk(Request);
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
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
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
        }

        void EndSlideShow(PowerPoint.Presentation Pres)
        {
            /* Once a slide show ends hide the GL window. */
            GlWindow.SetVisibility(false);
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
        }

        /* Helpers to use to position the window. */
        int PointToPixelX(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)((Point * g.DpiX) / 72.0f);
            }

        }

        /* Helpers to use to position the window. */
        int PointToPixelY(float Point, IntPtr HWnd)
        {
            using (Graphics g = Graphics.FromHwnd(HWnd))
            {
                return (int)((Point * g.DpiY) / 72.0f);
            }
        }

        /* Called when we go to a new slide in a slideshow. */
        void SlideShowOnSlide(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy;
            int x, y, w, h, WinLeft, WinTop, WinWidth, WinHeight;
            float SlideWidth, SlideHeight,
                  LetterBoxOffsetX, LetterBoxOffsetY,
                  AspectScale;
            string Path, Key;
            GlRenderer.RenderSettings RenderSettings;
            ITDParser.ImportSettings ImportSettings;
            float ZoomFactor;

            /* Check for the dummy and if it exists determine the size/position of the  */
            /* GLWindow from it. */
            if (GetDummyFromSlide(Wn.View.Slide, out Dummy))
            {
                Dummy.Visible = MsoTriState.msoFalse;

                SlideWidth = PointToPixelX(Wn.Presentation.PageSetup.SlideWidth, (IntPtr)Wn.HWND);
                SlideHeight = PointToPixelY(Wn.Presentation.PageSetup.SlideHeight, (IntPtr)Wn.HWND);

                WinLeft = PointToPixelX(Wn.Left, (IntPtr)Wn.HWND);
                WinTop = PointToPixelY(Wn.Top, (IntPtr)Wn.HWND);
                WinWidth = PointToPixelX(Wn.Width, (IntPtr)Wn.HWND);
                WinHeight = PointToPixelY(Wn.Height, (IntPtr)Wn.HWND);

                AspectScale = Math.Min(WinWidth / SlideWidth, WinHeight / SlideHeight);

                /* Calculate the letter box size using the aspect ratios. */

                LetterBoxOffsetX = (WinWidth - SlideWidth * AspectScale) / 2.0f;
                LetterBoxOffsetY = (WinHeight - SlideHeight * AspectScale) / 2.0f;

                /* Include the zoom factor when calculating positions. */
                ZoomFactor = Wn.View.Zoom / 100f;

                /* Calculate the window's shape and send it to the renderer. */
                x = PointToPixelX(Dummy.Left * ZoomFactor, (IntPtr)Wn.HWND);
                y = PointToPixelY(Dummy.Top * ZoomFactor, (IntPtr)Wn.HWND);
                w = PointToPixelX(Dummy.Width * ZoomFactor, (IntPtr)Wn.HWND);
                h = PointToPixelY(Dummy.Height * ZoomFactor, (IntPtr)Wn.HWND);


                /* Account for black boxes when the aspect ratio isn't hte same. */
                x = (int)(x + LetterBoxOffsetX);
                y = (int)(y + LetterBoxOffsetY);

                GlWindow.SetPosition(x, y, WinLeft, WinTop);
                GlWindow.SetSize(w, h);
                GlWindow.SetVisibility(true);

                Path = Dummy.Tags[__PATH_KEY__];
                RenderSettings = GlRenderer.DeserializeRenderSettings(Dummy.Tags[__RENDER_SETTINGS_KEY__]);
                ImportSettings = ITDParser.DeserializeImportSettings(Dummy.Tags[__IMPORT_SETTINGS_KEY__]);
                Key = ResourceManager.BuildResourceKey(Path, ImportSettings);
                GlWindow.GetRenderer().SetActiveModel(Key, RenderSettings);
            } else
                GlWindow.SetVisibility(false);

            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
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
