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
    public class I2PSlideshowWindowInfo
    {
        public int x, y, w, h, WinLeft, WinTop, WinWidth, WinHeight;
        public float SlideWidth, SlideHeight,
              LetterBoxOffsetX, LetterBoxOffsetY,
              AspectScale, ZoomFactor;
    }

    public struct I2PSlideshowCache
    {
        public bool HasBeenBuilt;
        public List<Bitmap> ScreenCropCache;
        public List<I2PSlideshowWindowInfo> InfoCache;
    }

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
        private I2PSlideshowCache SlideshowCache;

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
                {
                    Logger.GetInstance().Trace("Detected end of slideshow black screen, hiding GlWindow");
                    GlWindow.SetVisibility(false);
                }
            };

            SlideshowCache.ScreenCropCache = null;
            SlideshowCache.InfoCache = null;
            SlideshowCache.HasBeenBuilt = false;

            Logger.GetInstance().Trace("I2P setup done.");
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

        public void OnSave(PowerPoint.Presentation Presentation)
        {
            try
            {
                string SaveDirectory = Path.GetDirectoryName(Presentation.FullName);
                Logger.GetInstance().MoveToDirectory(SaveDirectory);

                Logger.GetInstance().Trace($"Saved presentation at {Presentation.FullName}, moving log file to {SaveDirectory}.");
            } catch(Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
        }

        public void OnPresentation(PowerPoint.Presentation Presentation)
        {
            try
            {
                string SaveDirectory = Path.GetDirectoryName(Presentation.FullName);


                Logger.GetInstance().MoveToDirectory(SaveDirectory);

                Logger.GetInstance().Trace($"Opened new presentation at {Presentation.FullName}");

                List<LoadRequest> RequestsInUse = GetLoadRequestsInUse();
                foreach (LoadRequest Request in RequestsInUse)
                    ResourceManager.QueueLoadFromDisk(Request);
            }
            catch (Exception e)
            {
                Logger.GetInstance().Error($"Caught exception: {e}");
            }
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
                    Logger.GetInstance().Error($"Encoutered an error while handling load request {Slide.Tags[__IMPORT_SETTINGS_KEY__]}: {Ex})");
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
            try { 
            int i;
            PowerPoint.Shape Dummy;

                /* Make sure everything is visible as we hide them after a slideshow. */
                for (i = 1; i <= Application.ActivePresentation.Slides.Count; i++)
                {
                    if (GetDummyFromSlide(Application.ActivePresentation.Slides[i],
                                    out Dummy))
                        Dummy.Visible = MsoTriState.msoTrue;
                    ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
                }

            } catch(Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
        }

        public void AddDummyShape(PowerPoint.Slide Slide)
        {
            PowerPoint.Shape Dummy;

            Dummy = Slide.Shapes.AddShape(Office.MsoAutoShapeType.msoShapeRoundedRectangle, 0, 0, 400, 300);
            Dummy.Fill.Visible = MsoTriState.msoFalse;
            Dummy.Name = __DUMMY_NAME__;

            Dummy.Line.Visible = MsoTriState.msoTrue;
            Dummy.Line.ForeColor.RGB = Color.Black.ToArgb();
            Dummy.Line.Weight = 1f;

            Dummy.Line.DashStyle = MsoLineDashStyle.msoLineSolid;
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
                Logger.GetInstance().Error($"Encoutered an error while handling load request {Slide.Tags[__IMPORT_SETTINGS_KEY__]}: {Ex})");
                return;
            }

            Slide.Tags.Add(__DUMMY_KEY__ , "true");
            Slide.Tags.Add(__PATH_KEY__, Path);
            ResourceManager.QueueLoadFromDisk(Request);
            ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
            if (!GetDummyFromSlide(Slide, out Dummy))
                AddDummyShape(Slide);

            Logger.GetInstance().Trace($"Updated I2P structure in slide at index = {Slide.SlideIndex}.");
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
            try
            {
                ActiveSlideShowWin = Wn;
                ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
                SlideshowCache.HasBeenBuilt = false;
                Logger.GetInstance().Trace($"Starting new slideshow, hiding all I2P shapes and fetching screenshots.");
            } catch (Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
        }

        void EndSlideShow(PowerPoint.Presentation Pres)
        {
            try
            {
                WindowStateTimer.Stop();
                ActiveSlideShowWin = null;

                GlWindow.ChangeOwner(IntPtr.Zero);
                /* Once a slide show ends hide the GL window. */
                GlWindow.SetVisibility(false);

                SlideshowCache.HasBeenBuilt = false;

                Logger.GetInstance().Trace("Ended slideshow.");
                ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
            } catch(Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
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

        Bitmap GetScreenCrop(PowerPoint.Slide Slide,
                             Rectangle SlideRect,
                             Rectangle CropRect)
        {
            Rectangle Intersection, SrcRect, DestRect;
            Bitmap Cropped, Orig;
            string
                TempDir = Path.GetTempPath(),
                ImagePath = Path.Combine(TempDir, "slide.png");

            Logger.GetInstance().Trace($"Getting screenshot for slide={Slide.SlideIndex} at Rect={CropRect}");

            Slide.Export(ImagePath, "png",
                         SlideRect.Width,
                         SlideRect.Height);

            Orig = new Bitmap(ImagePath);
            Cropped = new Bitmap(CropRect.Width, CropRect.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            Intersection = Rectangle.Intersect(SlideRect, CropRect);
            if (Intersection.IsEmpty)
                return Cropped;
            SrcRect = new Rectangle(Intersection.X - SlideRect.X,
                                    Intersection.Y - SlideRect.Y,
                                    Intersection.Width,
                                    Intersection.Height);
            DestRect = new Rectangle(Intersection.X - CropRect.X,
                                     Intersection.Y - CropRect.Y,
                                     Intersection.Width,
                                     Intersection.Height);
            using (Graphics g = Graphics.FromImage(Cropped))
                g.DrawImage(Orig, DestRect, SrcRect, GraphicsUnit.Pixel);
            Orig.Dispose();

            /* Flip since we pass to the gpu, must be compatible with opengl convention. */
            Cropped.RotateFlip(RotateFlipType.RotateNoneFlipY);

            return Cropped;
        }

        I2PSlideshowWindowInfo CalculateWindowInfo(PowerPoint.SlideShowWindow Wn, PowerPoint.Shape Dummy)
        {
            I2PSlideshowWindowInfo Info;
            WinAPIDef.RECT Rect;

            Info = new I2PSlideshowWindowInfo();

            Info.SlideWidth = PointToPixelX(Wn.Presentation.PageSetup.SlideWidth, (IntPtr)Wn.HWND);
            Info.SlideHeight = PointToPixelY(Wn.Presentation.PageSetup.SlideHeight, (IntPtr)Wn.HWND);

            WinAPIDclr.GetWindowRect((IntPtr)Wn.HWND, out Rect);

            Info.WinLeft = Rect.Left;
            Info.WinTop = Rect.Top;
            Info.WinWidth = Rect.Right - Rect.Left;
            Info.WinHeight = Rect.Bottom - Rect.Top;

            Info.AspectScale = Math.Min(Info.WinWidth / Info.SlideWidth, Info.WinHeight / Info.SlideHeight);

            /* Calculate the letter box size using the aspect ratios. */

            Info.LetterBoxOffsetX = (Info.WinWidth - Info.SlideWidth * Info.AspectScale) / 2.0f;
            Info.LetterBoxOffsetY = (Info.WinHeight - Info.SlideHeight * Info.AspectScale) / 2.0f;

            /* Include the zoom factor when calculating positions. */
            Info.ZoomFactor = Wn.View.Zoom / 100f;

            /* Calculate the window's shape and send it to the renderer. */

            Info.x = PointToPixelX(Dummy.Left * Info.ZoomFactor, (IntPtr)Wn.HWND);
            Info.y = PointToPixelY(Dummy.Top * Info.ZoomFactor, (IntPtr)Wn.HWND);
            Info.w = PointToPixelX(Dummy.Width * Info.ZoomFactor, (IntPtr)Wn.HWND);
            Info.h = PointToPixelY(Dummy.Height * Info.ZoomFactor, (IntPtr)Wn.HWND);

            /* Account for black boxes when the aspect ratio isn't hte same. */
            Info.x = (int)(Info.x + Info.LetterBoxOffsetX);
            Info.y = (int)(Info.y + Info.LetterBoxOffsetY);

            return Info;
        }

        void InitSlideshowCache()
        {
            if (SlideshowCache.ScreenCropCache != null)
                foreach (Bitmap img in SlideshowCache.ScreenCropCache)
                    img?.Dispose();
            SlideshowCache.InfoCache = new List<I2PSlideshowWindowInfo>();
            SlideshowCache.ScreenCropCache = new List<Bitmap>();
        }

        /* Called when we go to a new slide in a slideshow. */
        void SlideShowOnSlide(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy;
            I2PSlideshowWindowInfo SlideInfo;
            Bitmap ScreenCrop; 
            string Path, Key;
            GlRenderer.RenderSettings RenderSettings;
            string ImportSettings;
            Rectangle CropRect, SlideRect;
            /* VERY UGLY BUT POWERPOINT SILENTLY IGNORES EXCEPTIONS.... */
            try
            {

                /* If we're the first slide in the slideshow perform our pre-processing. */
                if (!SlideshowCache.HasBeenBuilt)
                {


                    Logger.GetInstance().Trace("Detected first slide in slideshow, prepearing slideshow cache.");
                    InitSlideshowCache();

                    foreach (PowerPoint.Slide Slide in this.Application.ActivePresentation.Slides)
                    {
                        /* Hide the shape and grab screen crops. */
                        if (GetDummyFromSlide(Slide, out Dummy))
                        {
                            Dummy.Visible = MsoTriState.msoFalse;
                            SlideInfo = CalculateWindowInfo(Wn, Dummy);

                            CropRect = new Rectangle(SlideInfo.x, SlideInfo.y, SlideInfo.w, SlideInfo.h);
                            SlideRect = new Rectangle((int)SlideInfo.LetterBoxOffsetX,
                                                      (int)SlideInfo.LetterBoxOffsetY,
                                                      (int)SlideInfo.SlideWidth,
                                                      (int)SlideInfo.SlideHeight);
                            SlideshowCache.InfoCache.Add(CalculateWindowInfo(Wn, Dummy));
                            SlideshowCache.ScreenCropCache.Add(GetScreenCrop(Slide, SlideRect, CropRect));
                        }
                        else
                        {
                            SlideshowCache.InfoCache.Add(null);
                            SlideshowCache.ScreenCropCache.Add(null);
                        }
                    }

                    SlideshowCache.HasBeenBuilt = true;
                    Logger.GetInstance().Trace("Done populating slideshow cache.");

                    /* Force slide rerender. */
                    Wn.View.GotoSlide(Wn.View.Slide.SlideIndex, MsoTriState.msoTrue);
                }


                WindowStateTimer.Stop();
                GlWindow.SetVisibility(false);

                /* Check for the dummy and if it exists determine the size/position of the  */
                /* GLWindow from it. */
                if (GetDummyFromSlide(Wn.View.Slide, out Dummy))
                {

                    Logger.GetInstance().Trace("Detected I2P shape in slide, setting up GlWindow.");

                    SlideInfo = SlideshowCache.InfoCache[Wn.View.Slide.SlideIndex - 1];
                    ScreenCrop = SlideshowCache.ScreenCropCache[Wn.View.Slide.SlideIndex - 1];


                    GlWindow.GetRenderer().UpdateBackground(ScreenCrop);

                    GlWindow.ChangeOwner((IntPtr)Wn.HWND);
                    GlWindow.SetPosition(SlideInfo.x, SlideInfo.y, SlideInfo.WinLeft, SlideInfo.WinTop);
                    GlWindow.SetSize(SlideInfo.w, SlideInfo.h);
                    WindowStateTimer.Start();

                    Path = Wn.View.Slide.Tags[__PATH_KEY__];
                    RenderSettings = GlRenderer.DefaultRenderSettings;
                    ImportSettings = Wn.View.Slide.Tags[__IMPORT_SETTINGS_KEY__];
                    Key = ResourceManager.BuildResourceKey(Path, ImportSettings);
                    GlWindow.GetRenderer().SetActiveModel(Key, RenderSettings);

                    GlWindow.SetVisibility(true);
                    Logger.GetInstance().Trace($"Sat window to location=({SlideInfo.x},{SlideInfo.y}) with size=({SlideInfo.w},{SlideInfo.h}), using resource key={Key}");
                }

                ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
            } catch (Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            try
            {
                WindowStateTimer.Dispose();
                GlWindow.Destroy();
                ResourceManager.Destroy();
                Logger.GetInstance().Trace($"Destroyed I2P.");

                Logger.GetInstance().Destroy();
            } catch (Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
            }
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
