
namespace Irit2Powerpoint.Resources
{
    partial class ImportSettings
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
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.PolylineFineness = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.PolygonFineness = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.IsolineSamples = new System.Windows.Forms.TextBox();
            this.PolygonOptimal = new System.Windows.Forms.CheckBox();
            this.PolylineOptimal = new System.Windows.Forms.CheckBox();
            this.flowLayoutPanel2 = new System.Windows.Forms.FlowLayoutPanel();
            this.OK = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.FlipNormals = new System.Windows.Forms.CheckBox();
            this.flowLayoutPanel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.flowLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Controls.Add(this.groupBox2);
            this.flowLayoutPanel1.Controls.Add(this.groupBox1);
            this.flowLayoutPanel1.Controls.Add(this.groupBox3);
            this.flowLayoutPanel1.Controls.Add(this.PolygonOptimal);
            this.flowLayoutPanel1.Controls.Add(this.PolylineOptimal);
            this.flowLayoutPanel1.Controls.Add(this.FlipNormals);
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(12, 2);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Padding = new System.Windows.Forms.Padding(10, 10, 0, 0);
            this.flowLayoutPanel1.Size = new System.Drawing.Size(254, 295);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.PolylineFineness);
            this.groupBox1.Location = new System.Drawing.Point(13, 76);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 57);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Polyline Fineness";
            // 
            // PolylineFineness
            // 
            this.PolylineFineness.Location = new System.Drawing.Point(12, 25);
            this.PolylineFineness.Name = "PolylineFineness";
            this.PolylineFineness.Size = new System.Drawing.Size(100, 26);
            this.PolylineFineness.TabIndex = 0;
            this.PolylineFineness.TextChanged += new System.EventHandler(this.PolylineFineness_TextChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.PolygonFineness);
            this.groupBox2.Location = new System.Drawing.Point(13, 13);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 57);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Polygon Fineness";
            // 
            // PolygonFineness
            // 
            this.PolygonFineness.Location = new System.Drawing.Point(12, 25);
            this.PolygonFineness.Name = "PolygonFineness";
            this.PolygonFineness.Size = new System.Drawing.Size(100, 26);
            this.PolygonFineness.TabIndex = 0;
            this.PolygonFineness.TextChanged += new System.EventHandler(this.PolygonFineness_TextChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.IsolineSamples);
            this.groupBox3.Location = new System.Drawing.Point(13, 139);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(200, 57);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Isoline Samples";
            // 
            // IsolineSamples
            // 
            this.IsolineSamples.Location = new System.Drawing.Point(12, 25);
            this.IsolineSamples.Name = "IsolineSamples";
            this.IsolineSamples.Size = new System.Drawing.Size(100, 26);
            this.IsolineSamples.TabIndex = 0;
            this.IsolineSamples.TextChanged += new System.EventHandler(this.IsolineSamples_TextChanged);
            // 
            // PolygonOptimal
            // 
            this.PolygonOptimal.AutoSize = true;
            this.PolygonOptimal.Location = new System.Drawing.Point(13, 202);
            this.PolygonOptimal.Name = "PolygonOptimal";
            this.PolygonOptimal.Size = new System.Drawing.Size(149, 24);
            this.PolygonOptimal.TabIndex = 2;
            this.PolygonOptimal.Text = "Polygon Optimal";
            this.PolygonOptimal.UseVisualStyleBackColor = true;
            this.PolygonOptimal.CheckedChanged += new System.EventHandler(this.PolygonOptimal_CheckedChanged);
            // 
            // PolylineOptimal
            // 
            this.PolylineOptimal.AutoSize = true;
            this.PolylineOptimal.Location = new System.Drawing.Point(13, 232);
            this.PolylineOptimal.Name = "PolylineOptimal";
            this.PolylineOptimal.Size = new System.Drawing.Size(146, 24);
            this.PolylineOptimal.TabIndex = 2;
            this.PolylineOptimal.Text = "Polyline Optimal";
            this.PolylineOptimal.UseVisualStyleBackColor = true;
            this.PolylineOptimal.CheckedChanged += new System.EventHandler(this.PolylineOptimal_CheckedChanged);
            // 
            // flowLayoutPanel2
            // 
            this.flowLayoutPanel2.Controls.Add(this.OK);
            this.flowLayoutPanel2.Controls.Add(this.Cancel);
            this.flowLayoutPanel2.Location = new System.Drawing.Point(12, 303);
            this.flowLayoutPanel2.Name = "flowLayoutPanel2";
            this.flowLayoutPanel2.Padding = new System.Windows.Forms.Padding(4);
            this.flowLayoutPanel2.Size = new System.Drawing.Size(254, 51);
            this.flowLayoutPanel2.TabIndex = 1;
            // 
            // OK
            // 
            this.OK.Location = new System.Drawing.Point(7, 7);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(112, 35);
            this.OK.TabIndex = 0;
            this.OK.Text = "OK";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(125, 7);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(112, 35);
            this.Cancel.TabIndex = 0;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            // 
            // FlipNormals
            // 
            this.FlipNormals.AutoSize = true;
            this.FlipNormals.Location = new System.Drawing.Point(13, 262);
            this.FlipNormals.Name = "FlipNormals";
            this.FlipNormals.Size = new System.Drawing.Size(122, 24);
            this.FlipNormals.TabIndex = 2;
            this.FlipNormals.Text = "Flip Normals";
            this.FlipNormals.UseVisualStyleBackColor = true;
            this.FlipNormals.CheckedChanged += new System.EventHandler(this.FlipNormals_CheckedChanged);
            // 
            // ImportSettings
            // 
            this.AcceptButton = this.OK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.Cancel;
            this.ClientSize = new System.Drawing.Size(273, 366);
            this.Controls.Add(this.flowLayoutPanel2);
            this.Controls.Add(this.flowLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ImportSettings";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "ImportSettings";
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.flowLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox PolygonFineness;
        private System.Windows.Forms.TextBox PolylineFineness;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox IsolineSamples;
        private System.Windows.Forms.CheckBox PolygonOptimal;
        private System.Windows.Forms.CheckBox PolylineOptimal;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel2;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.CheckBox FlipNormals;
    }
}