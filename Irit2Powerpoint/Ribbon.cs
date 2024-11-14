using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;
using Office = Microsoft.Office.Core;

namespace Irit2Powerpoint
{
    [ComVisible(true)]
    public class Ribbon : Office.IRibbonExtensibility
    {
        private Office.IRibbonUI ribbon;

        public Ribbon()
        {
        }

        #region IRibbonExtensibility Members

        public string GetCustomUI(string ribbonID)
        {
            return GetResourceText("Irit2Powerpoint.Ribbon.xml");
        }

        #endregion

        #region Ribbon Callbacks

        public void Ribbon_Load(Office.IRibbonUI ribbonUI)
        {
            this.ribbon = ribbonUI;
        }

        private string GetRelativePath(string AbsPath)
        {
            string
                CurrentDir = Directory.GetCurrentDirectory();
            Uri
                BaseUri = new Uri(CurrentDir + Path.DirectorySeparatorChar);
            Uri
                DestUri = new Uri(AbsPath);
            Uri
                RelUri = BaseUri.MakeRelativeUri(DestUri);
            return Uri.UnescapeDataString(RelUri.ToString())
                            .Replace('/', Path.DirectorySeparatorChar);
        }

        public void OnImportButton(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "itd file (*.itd)|*.itd|obj file (*.obj)|*.obj|stl file (*.stl)|*.stl|All files|*.obj;*.itd;*.stl";
                openFileDialog.FilterIndex = 0;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    AddIn.InitDummyRect(GetRelativePath(openFileDialog.FileName));
                }
            }
        }

        public void OnSettingsButton(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;
            ITDParser.ImportSettings
                Settings = AddIn.GetCurrentImportSettings();
            Resources.ImportSettings
                SettingsForm = new Resources.ImportSettings(Settings);
            if (SettingsForm.ShowDialog() == DialogResult.OK)
                AddIn.SetCurrentImportSettings(SettingsForm.PickedSettings);
        }

        public void OnRenderButton(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;
            GlRenderer.RenderSettings
                Settings = AddIn.GetRenderSettingsFromActiveSlide();
            Resources.RenderSettings
                SettingsForm = new Resources.RenderSettings(Settings);

            if (SettingsForm.ShowDialog() == DialogResult.OK)
                AddIn.SetRenderSettingsInActiveSlide(SettingsForm.PickedSettings);
        }

        public bool OnGetRenderEnabled(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;
            return AddIn.ActiveSlideContainsDummy();
        }

        public void Refresh()
        {
            this.ribbon.InvalidateControl("RenderSettingsButton");
        }

        #endregion

        

        #region Helpers

        private static string GetResourceText(string resourceName)
        {
            Assembly asm = Assembly.GetExecutingAssembly();
            string[] resourceNames = asm.GetManifestResourceNames();
            for (int i = 0; i < resourceNames.Length; ++i)
            {
                if (string.Compare(resourceName, resourceNames[i], StringComparison.OrdinalIgnoreCase) == 0)
                {
                    using (StreamReader resourceReader = new StreamReader(asm.GetManifestResourceStream(resourceNames[i])))
                    {
                        if (resourceReader != null)
                        {
                            return resourceReader.ReadToEnd();
                        }
                    }
                }
            }
            return null;
        }

        #endregion
    }
}
