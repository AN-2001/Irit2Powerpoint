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
        private const string __PATH_RELATIVE_KEY__ = "_I2P_PATH_RELATIVE_";
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
            this.Application.PresentationSave += OnSave;

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

        private bool IsSubPath(string Parent, string Child)
        {
            string
                NormalizedParent = Path.GetFullPath(Parent)
                                       .TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar)
                                       .ToUpperInvariant(),
                NormalizedChild = Path.GetFullPath(Child)
                                       .TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar)
                                       .ToUpperInvariant();
            return NormalizedChild.StartsWith(NormalizedParent + Path.DirectorySeparatorChar) ||
                   NormalizedChild.StartsWith(NormalizedParent + Path.AltDirectorySeparatorChar);
        }

        public static string GetRelativePath(string basePath, string targetPath)
        {
            basePath = Path.GetFullPath(basePath.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar);
            targetPath = Path.GetFullPath(targetPath);
            Uri baseUri = new Uri(basePath);
            Uri targetUri = new Uri(targetPath);
            Uri relativeUri = baseUri.MakeRelativeUri(targetUri);
            string relativePath = Uri.UnescapeDataString(relativeUri.ToString());
            relativePath = relativePath.Replace('/', Path.DirectorySeparatorChar);
            return relativePath;
        }

        private void OnSave(PowerPoint.Presentation Pres)
        {
            PowerPoint.Slide Slide;
            int i;
            string SaveDirectory = Path.GetDirectoryName( Pres.FullName );
            string FilePath;

            for (i = 1; i <= Pres.Slides.Count; i++)
            {
                Slide = Pres.Slides[i];
                if (!SlideContainsDummy(Slide))
                    continue;
                FilePath = Slide.Tags[__PATH_KEY__];
                if (IsSubPath(SaveDirectory, FilePath))
                {
                    FilePath = GetRelativePath(SaveDirectory, FilePath);
                    Slide.Tags.Add(__PATH_RELATIVE_KEY__, "true");
                }
                Slide.Tags.Add(__PATH_KEY__, FilePath);
            }
            Pres.Save();
            
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
            string FilePath;

            for (i = 1; i <= Presentation.Slides.Count; i++)
            {
                Slide = Presentation.Slides[i];
                if (!SlideContainsDummy(Slide))
                    continue;
                FilePath = Slide.Tags[__PATH_KEY__];
                if (Slide.Tags[__PATH_RELATIVE_KEY__] == "true")
                {
                    FilePath = Path.Combine(SaveDirectory, FilePath);
                    Slide.Tags.Add(__PATH_KEY__, FilePath);
                }
            }

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
                Request = new LoadRequest();
                Request.Path = Slide.Tags[__PATH_KEY__];
                Request.ImportSettings = Slide.Tags[__IMPORT_SETTINGS_KEY__];
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
           
            if (!GetDummyFromSlide(Slide, out Dummy))
                AddDummyShape(Slide);
            Slide.Tags.Add(__DUMMY_KEY__ , "true");
            Slide.Tags.Add(__PATH_KEY__, Path);
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

            GlWindow.funny(IntPtr.Zero);
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

        Color getAvgColour(PowerPoint.Slide Slide)
        {
            string
                TempDir = Path.GetTempPath(),
                ImagePath = Path.Combine(TempDir, "slide.bmp");
            int r, g, b;

            Slide.Export(ImagePath, "BMP", 128, 128);
        

            using (Bitmap bmp = new Bitmap(ImagePath))
            {
                int i, j;
                int Total = 128 * 128;

                r = g = b = 0;

                for (i = 0; i < 128; i++)
                {
                    for (j = 0; j < 128; j++)
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

            if (SlideAverageColours.Count == 0)
            {
                foreach (PowerPoint.Slide Slide in Application.ActivePresentation.Slides)
                    SlideAverageColours.Add(SlideContainsDummy(Slide) ? getAvgColour(Slide) : Color.Empty);
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

                x = PointToPixelX((Dummy.Left) * ZoomFactor, (IntPtr)Wn.HWND);
                y = PointToPixelY((Dummy.Top) * ZoomFactor, (IntPtr)Wn.HWND);
                w = PointToPixelX((Dummy.Width) * ZoomFactor, (IntPtr)Wn.HWND);
                h = PointToPixelY((Dummy.Height) * ZoomFactor, (IntPtr)Wn.HWND);


                /* Account for black boxes when the aspect ratio isn't hte same. */
                x = (int)(x + LetterBoxOffsetX);
                y = (int)(y + LetterBoxOffsetY);

                GlWindow.funny((IntPtr)Wn.HWND);
                GlWindow.SetPosition(x, y, WinLeft, WinTop);
                GlWindow.SetSize(w, h);
                WindowStateTimer.Start();

                Path = Wn.View.Slide.Tags[__PATH_KEY__];
                RenderSettings = GlRenderer.DefaultRenderSettings;
                Color Col = SlideAverageColours[Wn.View.Slide.SlideIndex - 1];

                RenderSettings.BackgroundColour = new OpenTK.Vector3(Col.R / 255.0f,
                                                                     Col.G / 255.0f,
                                                                     Col.B / 255.0f);
                GlWindow.funny2(Col);
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
