namespace XVPDesigner.UserControls
{
	partial class SpinnerEditor
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
			this.lblMinValue = new DevComponents.DotNetBar.LabelX();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			this.lblMaxValue = new DevComponents.DotNetBar.LabelX();
			this.labelX3 = new DevComponents.DotNetBar.LabelX();
			this.spinnerValue = new System.Windows.Forms.NumericUpDown();
			((System.ComponentModel.ISupportInitialize)(this.spinnerValue)).BeginInit();
			this.SuspendLayout();
			// 
			// lblMinValue
			// 
			this.lblMinValue.Location = new System.Drawing.Point(62,35);
			this.lblMinValue.Name = "lblMinValue";
			this.lblMinValue.Size = new System.Drawing.Size(63,23);
			this.lblMinValue.TabIndex = 2;
			this.lblMinValue.Text = "-15000000";
			// 
			// labelX2
			// 
			this.labelX2.Location = new System.Drawing.Point(4,35);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(55,23);
			this.labelX2.TabIndex = 3;
			this.labelX2.Text = "Minimum:";
			// 
			// lblMaxValue
			// 
			this.lblMaxValue.Location = new System.Drawing.Point(62,53);
			this.lblMaxValue.Name = "lblMaxValue";
			this.lblMaxValue.Size = new System.Drawing.Size(66,23);
			this.lblMaxValue.TabIndex = 4;
			this.lblMaxValue.Text = "15000000";
			// 
			// labelX3
			// 
			this.labelX3.Location = new System.Drawing.Point(4,53);
			this.labelX3.Name = "labelX3";
			this.labelX3.Size = new System.Drawing.Size(55,23);
			this.labelX3.TabIndex = 5;
			this.labelX3.Text = "Maximum:";
			// 
			// spinnerValue
			// 
			this.spinnerValue.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.spinnerValue.Location = new System.Drawing.Point(4,4);
			this.spinnerValue.Name = "spinnerValue";
			this.spinnerValue.Size = new System.Drawing.Size(120,20);
			this.spinnerValue.TabIndex = 6;
			// 
			// SpinnerEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.spinnerValue);
			this.Controls.Add(this.lblMaxValue);
			this.Controls.Add(this.labelX3);
			this.Controls.Add(this.lblMinValue);
			this.Controls.Add(this.labelX2);
			this.Name = "SpinnerEditor";
			this.Size = new System.Drawing.Size(129,77);
			((System.ComponentModel.ISupportInitialize)(this.spinnerValue)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private DevComponents.DotNetBar.LabelX lblMinValue;
		private DevComponents.DotNetBar.LabelX labelX2;
		private DevComponents.DotNetBar.LabelX lblMaxValue;
		private DevComponents.DotNetBar.LabelX labelX3;
		private System.Windows.Forms.NumericUpDown spinnerValue;
	}
}
