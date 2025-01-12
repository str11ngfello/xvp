namespace XVPDesigner.UserControls
{
	partial class IntSliderEditor
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
			this.tbValue = new DevComponents.DotNetBar.Controls.Slider();
			this.lblMinValue = new DevComponents.DotNetBar.LabelX();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			this.lblMaxValue = new DevComponents.DotNetBar.LabelX();
			this.labelX3 = new DevComponents.DotNetBar.LabelX();
			this.lblCurrentValue = new DevComponents.DotNetBar.LabelX();
			this.labelX4 = new DevComponents.DotNetBar.LabelX();
			this.SuspendLayout();
			// 
			// tbValue
			// 
			this.tbValue.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbValue.LabelVisible = false;
			this.tbValue.Location = new System.Drawing.Point(19,3);
			this.tbValue.Name = "tbValue";
			this.tbValue.Size = new System.Drawing.Size(440,23);
			this.tbValue.TabIndex = 0;
			this.tbValue.Text = "slider1";
			this.tbValue.Value = 0;
			this.tbValue.ValueChanged += new System.EventHandler(this.tbValue_ValueChanged);
			// 
			// lblMinValue
			// 
			this.lblMinValue.Location = new System.Drawing.Point(-4,37);
			this.lblMinValue.Name = "lblMinValue";
			this.lblMinValue.Size = new System.Drawing.Size(63,23);
			this.lblMinValue.TabIndex = 2;
			this.lblMinValue.Text = "-15000000";
			this.lblMinValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX2
			// 
			this.labelX2.Location = new System.Drawing.Point(18,20);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(29,23);
			this.labelX2.TabIndex = 3;
			this.labelX2.Text = "MIN";
			// 
			// lblMaxValue
			// 
			this.lblMaxValue.Location = new System.Drawing.Point(420,37);
			this.lblMaxValue.Name = "lblMaxValue";
			this.lblMaxValue.Size = new System.Drawing.Size(58,23);
			this.lblMaxValue.TabIndex = 4;
			this.lblMaxValue.Text = "15000000";
			this.lblMaxValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX3
			// 
			this.labelX3.Location = new System.Drawing.Point(438,20);
			this.labelX3.Name = "labelX3";
			this.labelX3.Size = new System.Drawing.Size(31,23);
			this.labelX3.TabIndex = 5;
			this.labelX3.Text = "MAX";
			// 
			// lblCurrentValue
			// 
			this.lblCurrentValue.Location = new System.Drawing.Point(208,37);
			this.lblCurrentValue.Name = "lblCurrentValue";
			this.lblCurrentValue.Size = new System.Drawing.Size(63,23);
			this.lblCurrentValue.TabIndex = 6;
			this.lblCurrentValue.Text = "0";
			this.lblCurrentValue.TextAlignment = System.Drawing.StringAlignment.Center;
			// 
			// labelX4
			// 
			this.labelX4.Location = new System.Drawing.Point(221,20);
			this.labelX4.Name = "labelX4";
			this.labelX4.Size = new System.Drawing.Size(39,23);
			this.labelX4.TabIndex = 7;
			this.labelX4.Text = "VALUE";
			// 
			// TrackBarEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tbValue);
			this.Controls.Add(this.lblCurrentValue);
			this.Controls.Add(this.labelX4);
			this.Controls.Add(this.lblMaxValue);
			this.Controls.Add(this.labelX3);
			this.Controls.Add(this.lblMinValue);
			this.Controls.Add(this.labelX2);
			this.Name = "TrackBarEditor";
			this.Size = new System.Drawing.Size(479,64);
			this.ResumeLayout(false);

		}

		#endregion

		private DevComponents.DotNetBar.Controls.Slider tbValue;
		private DevComponents.DotNetBar.LabelX lblMinValue;
		private DevComponents.DotNetBar.LabelX labelX2;
		private DevComponents.DotNetBar.LabelX lblMaxValue;
		private DevComponents.DotNetBar.LabelX labelX3;
		private DevComponents.DotNetBar.LabelX lblCurrentValue;
		private DevComponents.DotNetBar.LabelX labelX4;
	}
}
