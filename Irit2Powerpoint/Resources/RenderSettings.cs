using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Irit2Powerpoint.Resources
{
    public partial class RenderSettings : Form
    {
        public GlRenderer.RenderSettings PickedSettings;

        public RenderSettings(GlRenderer.RenderSettings Settings)
        {
            InitializeComponent();
            PickedSettings = Settings;

            Helper.UpdateButtonColour(DfltSolidClr,
                            Helper.VectorToColor(PickedSettings.DefaultSolidColour));

            Helper.UpdateButtonColour(DfltCrvClr,
                            Helper.VectorToColor(PickedSettings.DefaultCurveColour));

            Helper.UpdateButtonColour(BgClr,
                            Helper.VectorToColor(PickedSettings.BackgroundColour));

            Helper.UpdateButtonColour(LightClr,
                            Helper.VectorToColor(PickedSettings.LightColour));

            LightPosX.Text = PickedSettings.LightPosition.X.ToString();
            LightPosY.Text = PickedSettings.LightPosition.Y.ToString();
            LightPosZ.Text = PickedSettings.LightPosition.Z.ToString();
        }

        private void RenderSettings_Load(object sender, EventArgs e)
        {

        }

        private void OK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;

        }

        private void LightPos_TextChanged(object sender, EventArgs e)
        {
            TextBox
                Self = (TextBox)sender;
            float
                Val = float.Parse(Self.Text);

            if (Self.Name == "LightPosX")
                PickedSettings.LightPosition.X = Val;
            else if (Self.Name == "LightPosY")
                PickedSettings.LightPosition.Y = Val;
            else
                PickedSettings.LightPosition.Z = Val;
        }

        private void Colour_Click(object sender, EventArgs e)
        {
            ColorDialog ColorSel = new ColorDialog();
            Button self = (Button)sender;

            if (ColorSel.ShowDialog() == DialogResult.OK)
            {
                Helper.UpdateButtonColour(self, ColorSel.Color);
                if (self.Name == "DfltSolidClr")
                    PickedSettings.DefaultSolidColour = Helper.ColorToVector(ColorSel.Color);
                else if (self.Name == "DfltCrvClr")
                    PickedSettings.DefaultCurveColour = Helper.ColorToVector(ColorSel.Color);
                else if (self.Name == "LightClr")
                    PickedSettings.LightColour = Helper.ColorToVector(ColorSel.Color);
                else
                    PickedSettings.BackgroundColour = Helper.ColorToVector(ColorSel.Color);
            }

        }
    }

    public class Helper
    {

        public static void UpdateButtonColour(in Button Button, Color Col)
        {
            Button.BackColor = Col;
            Button.ForeColor = Color.FromArgb(255 - Col.R,
                                              255 - Col.G,
                                              255 - Col.B);
        }

        public static Color VectorToColor(OpenTK.Vector3 Vec)
        {
            return Color.FromArgb((int)(Vec.X * 255), (int)(Vec.Y * 255), (int)(Vec.Z * 255));
        }

        public static OpenTK.Vector3 ColorToVector(Color Col)
        {
            return new OpenTK.Vector3(Col.R / 255.0f, Col.G / 255.0f, Col.B / 255.0f);
        }

    }
}
