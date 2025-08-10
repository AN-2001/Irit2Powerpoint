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


        public Bitmap OnI2PButtonImage(Office.IRibbonControl Control)
        {
            return new Bitmap(Properties.Resources.button);
        }

        public void OnI2PButton(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;
            string InputPath;

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "itd file (*.itd)|*.itd|obj file (*.obj)|*.obj|stl file (*.stl)|*.stl|All files|*.obj;*.itd;*.stl";
                openFileDialog.FilterIndex = 0;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    InputPath = openFileDialog.FileName;
                    if (!Path.IsPathRooted(InputPath))
                        InputPath = Path.GetFullPath(InputPath);
                    AddIn.InitDummyRect(InputPath);
                }
            }
        }

        // public void OnSettingsButton(Office.IRibbonControl Control)
        // {
        //     I2P AddIn = Globals.I2P;
        //     string
        //         Settings = AddIn.GetCurrentImportSettings();
        //     Resources.ImportSettings
        //         SettingsForm = new Resources.ImportSettings(Settings);
        //     if (SettingsForm.ShowDialog() == DialogResult.OK)
        //         AddIn.SetCurrentImportSettings(SettingsForm.PickedSettings);
        // }

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
