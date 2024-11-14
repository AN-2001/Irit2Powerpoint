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
    public partial class ImportSettings : Form
    {
        public ITDParser.ImportSettings PickedSettings;
        public ImportSettings(ITDParser.ImportSettings Settings)
        {
            InitializeComponent();
            PickedSettings = Settings;
            PolygonFineness.Text = PickedSettings.PolygonFineness.ToString();
            PolylineFineness.Text = PickedSettings.PolylineFineness.ToString();
            IsolineSamples.Text = PickedSettings.IsolinesSamples.ToString();
            PolygonOptimal.Checked = PickedSettings.PolygonOptimal == 1;
            PolylineOptimal.Checked = PickedSettings.PolylineOptimal == 1;
            FlipNormals.Checked = PickedSettings.FlipNormals == 1;
        }

        private void PolygonFineness_TextChanged(object sender, EventArgs e)
        {
            TextBox
                Self = (TextBox)sender;
            PickedSettings.PolygonFineness = int.Parse(Self.Text);
        }

        private void PolylineFineness_TextChanged(object sender, EventArgs e)
        {
            TextBox
                Self = (TextBox)sender;
            PickedSettings.PolylineFineness = int.Parse(Self.Text);
        }

        private void IsolineSamples_TextChanged(object sender, EventArgs e)
        {
            TextBox
                Self = (TextBox)sender;
            PickedSettings.IsolinesSamples = int.Parse(Self.Text);
        }

        private void PolygonOptimal_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox
                Self = (CheckBox)sender;
            PickedSettings.PolygonOptimal = Self.Checked ? 1 : 0;
        }

        private void PolylineOptimal_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox
                Self = (CheckBox)sender;
            PickedSettings.PolylineOptimal = Self.Checked ? 1 : 0;
        }

        private void FlipNormals_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox
                Self = (CheckBox)sender;
            PickedSettings.FlipNormals = Self.Checked ? 1 : 0;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }
    }
}
