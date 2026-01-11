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
        public int x, y, w, h, WinLeft, WinTop, WinWidth, WinHeight,
                   LetterBoxOffsetX, LetterBoxOffsetY, EffectiveWidth, EffectiveHeight;
        public int  SlideWidth, SlideHeight;
        public float AspectScale;
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
        private bool HasHiddenShapes;

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
            HasHiddenShapes = false;

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

        public void OnSave(PowerPoint.Presentation Presentation)
        {
            try
            {
                string SaveDirectory = Path.GetDirectoryName(Presentation.FullName);
                GlWindow.SetAppDirectory(SaveDirectory);
                Logger.GetInstance().SetNextDir(SaveDirectory);

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

                GlWindow.SetAppDirectory(SaveDirectory);
                Logger.GetInstance().SetNextDir(SaveDirectory);

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
            try
            {
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

                HasHiddenShapes = false;
            }
            catch (Exception ex)
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

        public void InitDummyRect(string Path, string Settings)
        {
            LoadRequest Request;
            PowerPoint.Shape Dummy;
            PowerPoint.Slide
                Slide = Application.ActiveWindow.View.Slide;

            Slide.Tags.Add(__DUMMY_KEY__ , "true");
            Slide.Tags.Add(__PATH_KEY__, Path);
            Slide.Tags.Add(__IMPORT_SETTINGS_KEY__, Settings);

            try
            {
                Request = new LoadRequest(Application.ActivePresentation.Path,
                                          Path,
                                          Settings);
            } catch(IOException Ex)
            {
                Logger.GetInstance().Error($"Encoutered an error while handling load request {Slide.Tags[__IMPORT_SETTINGS_KEY__]}: {Ex})");
                return;
            }

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
                HasHiddenShapes = false;

                Logger.GetInstance().Trace("Ended slideshow.");
                ResourceManager.ConsistencyCleanup(GetKeysInUse().ToArray());
            } catch(Exception ex)
            {
                Logger.GetInstance().Error($"Caught exception: {ex}");
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

            Logger.GetInstance().Trace($"Getting screenshot for slide={Slide.SlideIndex} at CropRect = {CropRect} from SlideRect = {SlideRect}");

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
            int LetterBoxX, LetterBoxY, EffectiveWidth, EffectiveHeight;
            int TotalHeight, TotalWidth;

            Info = new I2PSlideshowWindowInfo();

            Info.SlideWidth = (int)(Wn.Presentation.PageSetup.SlideWidth);
            Info.SlideHeight = (int)(Wn.Presentation.PageSetup.SlideHeight);

            WinAPIDclr.GetWindowRect((IntPtr)Wn.HWND, out Rect);

            Info.WinLeft = Rect.Left;
            Info.WinTop = Rect.Top;
            Info.WinWidth = Rect.Right - Rect.Left;
            Info.WinHeight = Rect.Bottom - Rect.Top;

            Info.AspectScale = Math.Min((float)Info.WinWidth / Info.SlideWidth,
                                        (float)Info.WinHeight / Info.SlideHeight);

            EffectiveWidth = (int)(Info.SlideWidth * Info.AspectScale);
            EffectiveHeight = (int)(Info.SlideHeight * Info.AspectScale);

            /* Calculate the letter box size using the aspect ratios. */

            LetterBoxX = (Info.WinWidth - EffectiveWidth) / 2;
            LetterBoxY = (Info.WinHeight - EffectiveHeight) / 2;

            /* Calculate the window's shape and send it to the renderer. */

            Info.x = ((int)(Dummy.Left * Info.AspectScale)) + LetterBoxX;
            Info.y = ((int)(Dummy.Top * Info.AspectScale)) + LetterBoxY;
            Info.w = ((int)(Dummy.Width * Info.AspectScale));
            Info.h = ((int)(Dummy.Height * Info.AspectScale));

            Info.LetterBoxOffsetX = LetterBoxX;
            Info.LetterBoxOffsetY = LetterBoxY;
            Info.EffectiveWidth = EffectiveWidth;
            Info.EffectiveHeight = EffectiveHeight;

            TotalWidth = Info.EffectiveWidth + Info.LetterBoxOffsetX * 2;
            Info.LetterBoxOffsetX += (Info.WinWidth - TotalWidth) / 2 + (Info.WinWidth - TotalWidth) % 2;


            TotalHeight = Info.EffectiveHeight + Info.LetterBoxOffsetY * 2;
            Info.LetterBoxOffsetY += (Info.WinHeight - TotalHeight) / 2 + (Info.WinHeight - TotalHeight) % 2;

            return Info;
        }

        void InitSlideshowCache(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy;
            I2PSlideshowWindowInfo SlideInfo;
            Rectangle CropRect, SlideRect;

            Logger.GetInstance().Trace("Building slideshow cahce.");

            if (SlideshowCache.ScreenCropCache != null)
                foreach (Bitmap img in SlideshowCache.ScreenCropCache)
                    img?.Dispose();
            SlideshowCache.InfoCache = new List<I2PSlideshowWindowInfo>();
            SlideshowCache.ScreenCropCache = new List<Bitmap>();


            foreach (PowerPoint.Slide Slide in this.Application.ActivePresentation.Slides)
            {
                /* grab screen crops and window positions. */
                if (GetDummyFromSlide(Slide, out Dummy))
                {
                    SlideInfo = CalculateWindowInfo(Wn, Dummy);

                    CropRect = new Rectangle(SlideInfo.x, SlideInfo.y, SlideInfo.w, SlideInfo.h);
                    SlideRect = new Rectangle(SlideInfo.LetterBoxOffsetX,
                                              SlideInfo.LetterBoxOffsetY,
                                              SlideInfo.EffectiveWidth,
                                              SlideInfo.EffectiveHeight);
                    SlideshowCache.InfoCache.Add(CalculateWindowInfo(Wn, Dummy));
                    /* Possibly multi-thread this? could mess with powerpoint COM setup as it might not be friendly to multi-threading. */
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
        }

        void HideI2PShapes(PowerPoint.SlideShowWindow Wn)
        {

            PowerPoint.Shape Dummy;
            Logger.GetInstance().Trace("Hiding all I2P shapes." );          
            foreach (PowerPoint.Slide Slide in this.Application.ActivePresentation.Slides)
            {
                /* Hide the shape and grab screen crops. */
                if (GetDummyFromSlide(Slide, out Dummy))
                    Dummy.Visible = MsoTriState.msoFalse;
            }

            HasHiddenShapes = true;

            Logger.GetInstance().Trace( "Hid all I2P shapes, cancelling initial rendering event and triggerring another." );

            /* Force slide rerender. */
            Wn.View.GotoSlide(Wn.View.Slide.SlideIndex, MsoTriState.msoTrue);
        }

        /* Called when we go to a new slide in a slideshow. */
        void SlideShowOnSlide(PowerPoint.SlideShowWindow Wn)
        {
            PowerPoint.Shape Dummy;
            I2PSlideshowWindowInfo SlideInfo;
            Bitmap ScreenCrop; 
            string Path, Key;
            string ImportSettings;
            /* VERY UGLY BUT POWERPOINT SILENTLY IGNORES EXCEPTIONS.... */
            try
            {

                if (!HasHiddenShapes)
                {
                    /* Hide all shapes, this will trigger this same event, but the boolean above will protect us. */
                    HideI2PShapes(Wn);
                    return; /* No point continuing with this event, the function above will retrigger it. */
                }

                /* Build the slideshow cache to speed up further accesses to screenshots + window positions. */
                if (!SlideshowCache.HasBeenBuilt)
                    InitSlideshowCache( Wn );


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

                    /* This prevents the Windows compositor from treating this like a full screen window. */
                    /* It prevents the distracting black flicker. */
                    /* W E I R D */
                    if (SlideInfo.x == SlideInfo.WinLeft && SlideInfo.y == SlideInfo.WinTop &&
                        SlideInfo.w == SlideInfo.WinWidth && SlideInfo.h == SlideInfo.WinHeight)
                    {
                        SlideInfo.x--;
                        SlideInfo.y--;
                        SlideInfo.w++;
                        SlideInfo.h++;
                    }

                    GlWindow.ChangeOwner((IntPtr)Wn.HWND);
                    GlWindow.SetPosition(SlideInfo.x, SlideInfo.y, SlideInfo.WinLeft, SlideInfo.WinTop);
                    GlWindow.SetSize(SlideInfo.w, SlideInfo.h);
                    WindowStateTimer.Start();

                    Path = Wn.View.Slide.Tags[__PATH_KEY__];
                    ImportSettings = Wn.View.Slide.Tags[__IMPORT_SETTINGS_KEY__];
                    Key = ResourceManager.BuildResourceKey(Path, ImportSettings);
                    GlWindow.GetRenderer().SetActiveModel(Key);

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
