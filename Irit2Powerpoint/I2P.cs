using System;
using PowerPoint = Microsoft.Office.Interop.PowerPoint;
using Office = Microsoft.Office.Core;
using System.Windows.Forms;
using Microsoft.Office.Core;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace Irit2Powerpoint
{
    public partial class I2P
    {
        private const string __PATH_KEY__ = "_I2P_PATH_";
        private const string __IMPORT_SETTINGS_KEY__ = "_I2P_IMPORT_SETTINGS_";
        private const string __DUMMY_KEY__ = "_I2P_DUMMY_";
        private const string __DUMMY_NAME__ = "__IRIT2POWERPOINT_CONTAINER__";

        private WindowWrapper GlWindow;
        private GlResourceManager ResourceManager;
        private Ribbon RibbonControl;
        private PowerPoint.SlideShowWindow ActiveSlideShowWin;
        private Timer WindowStateTimer;
        private List<Color> SlideAverageColours;

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

            this.ActiveSlideShowWin = null;

            ResourceManager = new GlResourceManager();

            /* Reuse the same GLWindow accross different slides. */
            GlWindow = new WindowWrapper(new IntPtr(this.Application.HWND));

            WindowStateTimer = new Timer();
            WindowStateTimer.Interval = 10;
            WindowStateTimer.Tick += (object S, EventArgs E) =>
            {
                if (ActiveSlideShowWin != null &&
                    ActiveSlideShowWin.View.State == PowerPoint.PpSlideShowState.ppSlideShowDone)
                    GlWindow.SetVisibility(false);
            };

            SlideAverageColours = new List<Color>();
        }

        private bool SlideContainsDummy(PowerPoint.Slide Slide)
        {
            return !String.IsNullOrEmpty(Slide.Tags[__DUMMY_KEY__]);
        }

        private bool SlideContainsSettings(PowerPoint.Slide Slide)
        {
            return !String.IsNullOrEmpty(Slide.Tags[__IMPORT_SETTINGS_KEY__]);
        }

        public string GetCurrentImportSettings()
        {
            PowerPoint.Slide
                Slide = Application.ActiveWindow.View.Slide;
            if (SlideContainsSettings(Slide))
                return Slide.Tags[__IMPORT_SETTINGS_KEY__];
            return "";
        }

        public void SetCurrentImportSettings(string ImportSettings)
        {
            PowerPoint.Slide CurrentSlide = Application.ActiveWindow.View.Slide;

            CurrentSlide.Tags.Add(__IMPORT_SETTINGS_KEY__, ImportSettings);
            if (SlideContainsDummy(CurrentSlide))
                InitDummyRect(CurrentSlide.Tags[__PATH_KEY__]);
        }

        public void OnPresentation(PowerPoint.Presentation Presentation)
        {
            int i;
            PowerPoint.Slide Slide;
            string SaveDirectory = Path.GetDirectoryName( Presentation.FullName );
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
            return GlRenderer.DefaultRenderSettings;
        }

        private List<LoadRequest> GetLoadRequestsInUse()
        {
            List<LoadRequest> Requests = new List<LoadRequest>();
            LoadRequest Request;
            int i;
            PowerPoint.Slide Slide;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
            {
                Slide = Application.ActivePresentation.Slides[i];
                if (!SlideContainsDummy(Slide))
                    continue;

                try
                {
                    Request = new LoadRequest(Application.ActivePresentation.Path,
                                              Slide.Tags[__PATH_KEY__],
                                              Slide.Tags[__IMPORT_SETTINGS_KEY__]);
                } catch(IOException Ex)
                {
                    MessageBox.Show("Error: " + Ex.Message, "I2P Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    continue;
                }
                Requests.Add(Request);
            }
            return Requests;
        }

        private List<string> GetKeysInUse()
        {
            List<String> Keys = new List<string>();
            int i;
            string Key, Path, Settings;
            PowerPoint.Slide Slide;

            for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
            {
                Slide = Application.ActivePresentation.Slides[i];
                if (!SlideContainsDummy(Slide))
                    continue;
                Path = Slide.Tags[__PATH_KEY__];
                Settings = Slide.Tags[__IMPORT_SETTINGS_KEY__];
                Key = ResourceManager.BuildResourceKey(Path, Settings);
                Keys.Add(Key);
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


        public void AddDummyShape(PowerPoint.Slide Slide)
        {
            PowerPoint.Shape Dummy;

            Dummy = Slide.Shapes.AddShape(Office.MsoAutoShapeType.msoShapeRoundedRectangle, 0, 0, 400, 300);
            Dummy.Fill.Visible = MsoTriState.msoFalse;
            Dummy.Name = __DUMMY_NAME__;

            Dummy.TextFrame.TextRange.Text = "IRIT2POWERPOINT";
            Dummy.TextFrame.TextRange.Font.Bold = MsoTriState.msoTrue;
            Dummy.TextFrame.TextRange.Font.Color.RGB = Color.Black.ToArgb();

            Dummy.Line.Visible = MsoTriState.msoTrue;
            Dummy.Line.ForeColor.RGB = Color.Black.ToArgb();
            Dummy.Line.Weight = 5f;

            Dummy.Line.DashStyle = MsoLineDashStyle.msoLineDash;
        }

        public void InitDummyRect(string Path)
        {
            LoadRequest Request;
            PowerPoint.Shape Dummy;
            PowerPoint.Slide
                Slide = Application.ActiveWindow.View.Slide;
            string ImportSettings = Slide.Tags[__IMPORT_SETTINGS_KEY__];

            try
            {
                Request = new LoadRequest(Application.ActivePresentation.Path,
                                          Path,
                                          ImportSettings);
            } catch(IOException Ex)
            {
                MessageBox.Show("Error: " + Ex.Message, "I2P Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Slide.Tags.Add(__DUMMY_KEY__ , "true");
            Slide.Tags.Add(__PATH_KEY__, Path);
            ResourceManager.QueueLoadFromDisk(Request);
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());

            if (!GetDummyFromSlide(Slide, out Dummy))
                AddDummyShape(Slide);
        }

        /* Extracts the Irit2Powerpoint dummy from a slide. */
        private bool GetDummyFromSlide(PowerPoint.Slide Slide,
                                      out PowerPoint.Shape Dummy)
        {
            Dummy = null;
            if (!SlideContainsDummy(Slide))
                return false;
            Dummy = Slide.Shapes
                         .Cast<PowerPoint.Shape>()
                         .FirstOrDefault(s => s.Name == __DUMMY_NAME__);
            return Dummy != null;
        }

        void NewSlideShow(PowerPoint.SlideShowWindow Wn)
        {
            ActiveSlideShowWin = Wn;
            SlideAverageColours.Clear();
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
        }

        void EndSlideShow(PowerPoint.Presentation Pres)
        {
            WindowStateTimer.Stop();
            ActiveSlideShowWin = null;

            GlWindow.ChangeOwner(IntPtr.Zero);
            /* Once a slide show ends hide the GL window. */
            GlWindow.SetVisibility(false);
            SlideAverageColours.Clear();

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

        Color CalculateAverageColour(PowerPoint.Slide Slide)
        {
            string
                TempDir = Path.GetTempPath(),
                ImagePath = Path.Combine(TempDir, "slide.bmp");
            int r, g, b;

            Slide.Export(ImagePath, "BMP", 64, 64);
            using (Bitmap bmp = new Bitmap(ImagePath))
            {
                int i, j;
                int Total = bmp.Width * bmp.Height;

                r = g = b = 0;

                for (i = 0; i < bmp.Width; i++)
                {
                    for (j = 0; j < bmp.Height; j++)
                    {
                        Color c = bmp.GetPixel(j, i);
                        r += c.R;
                        g += c.G;
                        b += c.B;
                    }
                }

                r = r / Total;
                g = g / Total;
                b = b / Total;
            }

            return Color.FromArgb(r, g, b);
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
            string ImportSettings;
            float ZoomFactor;
            WinAPIDef.RECT Rect;

            if (SlideAverageColours.Count == 0)
            {
                foreach (PowerPoint.Slide Slide in Application.ActivePresentation.Slides)
                    SlideAverageColours.Add(SlideContainsDummy(Slide) ? CalculateAverageColour(Slide) : Color.Empty);
            }

            WindowStateTimer.Stop();
            GlWindow.SetVisibility(false);

            /* Check for the dummy and if it exists determine the size/position of the  */
            /* GLWindow from it. */
            if (GetDummyFromSlide(Wn.View.Slide, out Dummy))
            {
                Dummy.Visible = MsoTriState.msoFalse;

                SlideWidth = PointToPixelX(Wn.Presentation.PageSetup.SlideWidth, (IntPtr)Wn.HWND);
                SlideHeight = PointToPixelY(Wn.Presentation.PageSetup.SlideHeight, (IntPtr)Wn.HWND);

                WinAPIDclr.GetWindowRect((IntPtr)Wn.HWND, out Rect);

                WinLeft = Rect.Left;// PointToPixelX(Wn.Left, (IntPtr)Wn.HWND);
                WinTop = Rect.Top;// PointToPixelY(Wn.Top, (IntPtr)Wn.HWND);
                WinWidth = Rect.Right - Rect.Left;// PointToPixelX(Wn.Width, (IntPtr)Wn.HWND);
                WinHeight = Rect.Bottom - Rect.Top;//PointToPixelY(Wn.Height, (IntPtr)Wn.HWND);

                AspectScale = Math.Min(WinWidth / SlideWidth, WinHeight / SlideHeight);

                /* Calculate the letter box size using the aspect ratios. */

                LetterBoxOffsetX = (WinWidth - SlideWidth * AspectScale) / 2.0f;
                LetterBoxOffsetY = (WinHeight - SlideHeight * AspectScale) / 2.0f;

                /* Include the zoom factor when calculating positions. */
                ZoomFactor = Wn.View.Zoom / 100f;

                /* Calculate the window's shape and send it to the renderer. */

                x = PointToPixelX((Dummy.Left) * ZoomFactor, (IntPtr)Wn.HWND);
                y = PointToPixelY((Dummy.Top) * ZoomFactor, (IntPtr)Wn.HWND);
                w = PointToPixelX((Dummy.Width) * ZoomFactor, (IntPtr)Wn.HWND);
                h = PointToPixelY((Dummy.Height) * ZoomFactor, (IntPtr)Wn.HWND);


                /* Account for black boxes when the aspect ratio isn't hte same. */
                x = (int)(x + LetterBoxOffsetX);
                y = (int)(y + LetterBoxOffsetY);

                GlWindow.ChangeOwner((IntPtr)Wn.HWND);
                GlWindow.SetPosition(x, y, WinLeft, WinTop);
                GlWindow.SetSize(w, h);
                WindowStateTimer.Start();

                Path = Wn.View.Slide.Tags[__PATH_KEY__];
                RenderSettings = GlRenderer.DefaultRenderSettings;
                Color Col = SlideAverageColours[Wn.View.Slide.SlideIndex - 1];

                RenderSettings.BackgroundColour = new OpenTK.Vector3(Col.R / 255.0f,
                                                                     Col.G / 255.0f,
                                                                     Col.B / 255.0f);
                GlWindow.MaskColour(Col);
                ImportSettings = Wn.View.Slide.Tags[__IMPORT_SETTINGS_KEY__];
                Key = ResourceManager.BuildResourceKey(Path, ImportSettings);
                GlWindow.GetRenderer().SetActiveModel(Key, RenderSettings);
                GlWindow.SetVisibility(true);
            }
            else
                GlWindow.SetVisibility(false);

            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            WindowStateTimer.Dispose();
            GlWindow.Destroy();
            ResourceManager.Destroy();
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
