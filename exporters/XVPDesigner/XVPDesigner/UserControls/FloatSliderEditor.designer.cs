namespace XVPDesigner.TypeEditors
{
    partial class FloatSliderEditor
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.tbMain = new DevComponents.DotNetBar.Controls.Slider();
			this.lblCurrentValue = new DevComponents.DotNetBar.LabelX();
			this.lblMaxValue = new DevComponents.DotNetBar.LabelX();
			this.labelX3 = new DevComponents.DotNetBar.LabelX();
			this.lblMinValue = new DevComponents.DotNetBar.LabelX();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			this.labelX1 = new DevComponents.DotNetBar.LabelX();
			this.SuspendLayout();
			// 
			// tbMain
			// 
			this.tbMain.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbMain.LabelVisible = false;
			this.tbMain.Location = new System.Drawing.Point(19,3);
			this.tbMain.Name = "tbMain";
			this.tbMain.Size = new System.Drawing.Size(440,23);
			this.tbMain.TabIndex = 2;
			this.tbMain.Text = "slider1";
			this.tbMain.Value = 0;
			this.tbMain.ValueChanged += new System.EventHandler(this.tbMain_ValueChanged);
			// 
			// lblCurrentValue
			// 
			this.lblCurrentValue.Location = new System.Drawing.Point(208,37);
			this.lblCurrentValue.Name = "lblCurrentValue";
			this.lblCurrentValue.Size = new System.Drawing.Size(63,23);
			this.lblCurrentValue.TabIndex = 12;
			this.lblCurrentValue.Text = "0";
			this.lblCurrentValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// lblMaxValue
			// 
			this.lblMaxValue.Location = new System.Drawing.Point(420,37);
			this.lblMaxValue.Name = "lblMaxValue";
			this.lblMaxValue.Size = new System.Drawing.Size(58,23);
			this.lblMaxValue.TabIndex = 10;
			this.lblMaxValue.Text = "15000000";
			this.lblMaxValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX3
			// 
			this.labelX3.Location = new System.Drawing.Point(433,20);
			this.labelX3.Name = "labelX3";
			this.labelX3.Size = new System.Drawing.Size(32,23);
			this.labelX3.TabIndex = 11;
			this.labelX3.Text = "MAX";
			this.labelX3.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// lblMinValue
			// 
			this.lblMinValue.Location = new System.Drawing.Point(-2,37);
			this.lblMinValue.Name = "lblMinValue";
			this.lblMinValue.Size = new System.Drawing.Size(60,23);
			this.lblMinValue.TabIndex = 8;
			this.lblMinValue.Text = "-15000000";
			this.lblMinValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX2
			// 
			this.labelX2.Location = new System.Drawing.Point(16,20);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(25,23);
			this.labelX2.TabIndex = 9;
			this.labelX2.Text = "MIN";
			this.labelX2.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX1
			// 
			this.labelX1.Location = new System.Drawing.Point(205,20);
			this.labelX1.Name = "labelX1";
			this.labelX1.Size = new System.Drawing.Size(68,23);
			this.labelX1.TabIndex = 13;
			this.labelX1.Text = "VALUE";
			this.labelX1.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// FloatSliderEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.lblCurrentValue);
			this.Controls.Add(this.labelX1);
			this.Controls.Add(this.lblMaxValue);
			this.Controls.Add(this.lblMinValue);
			this.Controls.Add(this.tbMain);
			this.Controls.Add(this.labelX3);
			this.Controls.Add(this.labelX2);
			this.Name = "FloatSliderEditor";
			this.Size = new System.Drawing.Size(479,64);
			this.ResumeLayout(false);

        }

        #endregion

		private DevComponents.DotNetBar.Controls.Slider tbMain;
		private DevComponents.DotNetBar.LabelX lblCurrentValue;
		private DevComponents.DotNetBar.LabelX lblMaxValue;
		private DevComponents.DotNetBar.LabelX labelX3;
		private DevComponents.DotNetBar.LabelX lblMinValue;
		private DevComponents.DotNetBar.LabelX labelX2;
		private DevComponents.DotNetBar.LabelX labelX1;
    }
}
