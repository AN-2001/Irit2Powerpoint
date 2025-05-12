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
        public string PickedSettings;
        public ImportSettings(string Settings)
        {
            InitializeComponent();
            textBox1.Text = Settings;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            TextBox
                self = (TextBox)sender;
            PickedSettings = self.Text;
        }
    }
}
