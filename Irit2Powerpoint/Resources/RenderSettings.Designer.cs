
namespace Irit2Powerpoint.Resources
{
    partial class RenderSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.DfltCrvClr = new System.Windows.Forms.Button();
            this.DfltSolidClr = new System.Windows.Forms.Button();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.LightPosZ = new System.Windows.Forms.TextBox();
            this.LightPosY = new System.Windows.Forms.TextBox();
            this.LightPosX = new System.Windows.Forms.TextBox();
            this.BgClr = new System.Windows.Forms.Button();
            this.LightClr = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.OK = new System.Windows.Forms.Button();
            this.flowLayoutPanel2 = new System.Windows.Forms.FlowLayoutPanel();
            this.flowLayoutPanel1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.flowLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // DfltCrvClr
            // 
            this.DfltCrvClr.Location = new System.Drawing.Point(10, 182);
            this.DfltCrvClr.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DfltCrvClr.Name = "DfltCrvClr";
            this.DfltCrvClr.Size = new System.Drawing.Size(240, 77);
            this.DfltCrvClr.TabIndex = 0;
            this.DfltCrvClr.Text = "Default Curve Colour";
            this.DfltCrvClr.UseVisualStyleBackColor = true;
            this.DfltCrvClr.Click += new System.EventHandler(this.Colour_Click);
            // 
            // DfltSolidClr
            // 
            this.DfltSolidClr.Location = new System.Drawing.Point(10, 269);
            this.DfltSolidClr.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DfltSolidClr.Name = "DfltSolidClr";
            this.DfltSolidClr.Size = new System.Drawing.Size(240, 77);
            this.DfltSolidClr.TabIndex = 0;
            this.DfltSolidClr.Text = "Default Solid Colour";
            this.DfltSolidClr.UseVisualStyleBackColor = true;
            this.DfltSolidClr.Click += new System.EventHandler(this.Colour_Click);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Controls.Add(this.groupBox3);
            this.flowLayoutPanel1.Controls.Add(this.BgClr);
            this.flowLayoutPanel1.Controls.Add(this.DfltCrvClr);
            this.flowLayoutPanel1.Controls.Add(this.DfltSolidClr);
            this.flowLayoutPanel1.Controls.Add(this.LightClr);
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(18, 18);
            this.flowLayoutPanel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Padding = new System.Windows.Forms.Padding(6, 6, 0, 0);
            this.flowLayoutPanel1.Size = new System.Drawing.Size(267, 454);
            this.flowLayoutPanel1.TabIndex = 2;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.LightPosZ);
            this.groupBox3.Controls.Add(this.LightPosY);
            this.groupBox3.Controls.Add(this.LightPosX);
            this.groupBox3.Location = new System.Drawing.Point(10, 11);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Size = new System.Drawing.Size(202, 74);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Light Position";
            // 
            // LightPosZ
            // 
            this.LightPosZ.Location = new System.Drawing.Point(116, 29);
            this.LightPosZ.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LightPosZ.Name = "LightPosZ";
            this.LightPosZ.Size = new System.Drawing.Size(42, 26);
            this.LightPosZ.TabIndex = 1;
            this.LightPosZ.TextChanged += new System.EventHandler(this.LightPos_TextChanged);
            // 
            // LightPosY
            // 
            this.LightPosY.Location = new System.Drawing.Point(62, 29);
            this.LightPosY.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LightPosY.Name = "LightPosY";
            this.LightPosY.Size = new System.Drawing.Size(42, 26);
            this.LightPosY.TabIndex = 1;
            this.LightPosY.TextChanged += new System.EventHandler(this.LightPos_TextChanged);
            // 
            // LightPosX
            // 
            this.LightPosX.Location = new System.Drawing.Point(9, 29);
            this.LightPosX.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LightPosX.Name = "LightPosX";
            this.LightPosX.Size = new System.Drawing.Size(42, 26);
            this.LightPosX.TabIndex = 1;
            this.LightPosX.TextChanged += new System.EventHandler(this.LightPos_TextChanged);
            // 
            // BgClr
            // 
            this.BgClr.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.BgClr.Location = new System.Drawing.Point(10, 95);
            this.BgClr.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.BgClr.Name = "BgClr";
            this.BgClr.Size = new System.Drawing.Size(240, 77);
            this.BgClr.TabIndex = 0;
            this.BgClr.Text = "Background Colour";
            this.BgClr.UseVisualStyleBackColor = true;
            this.BgClr.Click += new System.EventHandler(this.Colour_Click);
            // 
            // LightClr
            // 
            this.LightClr.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.LightClr.Location = new System.Drawing.Point(10, 356);
            this.LightClr.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LightClr.Name = "LightClr";
            this.LightClr.Size = new System.Drawing.Size(240, 77);
            this.LightClr.TabIndex = 0;
            this.LightClr.Text = "Light Colour";
            this.LightClr.UseVisualStyleBackColor = true;
            this.LightClr.Click += new System.EventHandler(this.Colour_Click);
            // 
            // Cancel
            // 
            this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Cancel.Location = new System.Drawing.Point(139, 5);
            this.Cancel.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(112, 35);
            this.Cancel.TabIndex = 3;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // OK
            // 
            this.OK.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.OK.Location = new System.Drawing.Point(19, 5);
            this.OK.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(112, 35);
            this.OK.TabIndex = 4;
            this.OK.Text = "OK";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // flowLayoutPanel2
            // 
            this.flowLayoutPanel2.Controls.Add(this.OK);
            this.flowLayoutPanel2.Controls.Add(this.Cancel);
            this.flowLayoutPanel2.Location = new System.Drawing.Point(18, 482);
            this.flowLayoutPanel2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.flowLayoutPanel2.Name = "flowLayoutPanel2";
            this.flowLayoutPanel2.Padding = new System.Windows.Forms.Padding(15, 0, 0, 15);
            this.flowLayoutPanel2.Size = new System.Drawing.Size(267, 44);
            this.flowLayoutPanel2.TabIndex = 5;
            // 
            // RenderSettings
            // 
            this.AcceptButton = this.OK;
            this.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.Cancel;
            this.ClientSize = new System.Drawing.Size(299, 550);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.flowLayoutPanel2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RenderSettings";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "RenderSettings";
            this.Load += new System.EventHandler(this.RenderSettings_Load);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.flowLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button DfltCrvClr;
        private System.Windows.Forms.Button DfltSolidClr;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button BgClr;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox LightPosX;
        private System.Windows.Forms.Button LightClr;
        private System.Windows.Forms.TextBox LightPosZ;
        private System.Windows.Forms.TextBox LightPosY;
    }
}