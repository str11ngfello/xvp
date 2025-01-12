namespace XVPDesigner.UserControls
{
	partial class DirVecEditor
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
			this.spnX = new System.Windows.Forms.NumericUpDown();
			this.lblMaxValue = new DevComponents.DotNetBar.LabelX();
			this.spnY = new System.Windows.Forms.NumericUpDown();
			this.labelX1 = new DevComponents.DotNetBar.LabelX();
			this.spnZ = new System.Windows.Forms.NumericUpDown();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			((System.ComponentModel.ISupportInitialize)(this.spnX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.spnY)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.spnZ)).BeginInit();
			this.SuspendLayout();
			// 
			// spnX
			// 
			this.spnX.DecimalPlaces = 5;
			this.spnX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            327680});
			this.spnX.Location = new System.Drawing.Point(23,8);
			this.spnX.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.spnX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
			this.spnX.Name = "spnX";
			this.spnX.Size = new System.Drawing.Size(75,20);
			this.spnX.TabIndex = 6;
			// 
			// lblMaxValue
			// 
			this.lblMaxValue.Location = new System.Drawing.Point(5,6);
			this.lblMaxValue.Name = "lblMaxValue";
			this.lblMaxValue.Size = new System.Drawing.Size(16,20);
			this.lblMaxValue.TabIndex = 4;
			this.lblMaxValue.Text = "X:";
			// 
			// spnY
			// 
			this.spnY.DecimalPlaces = 5;
			this.spnY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            327680});
			this.spnY.Location = new System.Drawing.Point(135,8);
			this.spnY.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.spnY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
			this.spnY.Name = "spnY";
			this.spnY.Size = new System.Drawing.Size(75,20);
			this.spnY.TabIndex = 8;
			// 
			// labelX1
			// 
			this.labelX1.Location = new System.Drawing.Point(117,6);
			this.labelX1.Name = "labelX1";
			this.labelX1.Size = new System.Drawing.Size(16,20);
			this.labelX1.TabIndex = 7;
			this.labelX1.Text = "Y:";
			// 
			// spnZ
			// 
			this.spnZ.DecimalPlaces = 5;
			this.spnZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            327680});
			this.spnZ.Location = new System.Drawing.Point(246,8);
			this.spnZ.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.spnZ.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
			this.spnZ.Name = "spnZ";
			this.spnZ.Size = new System.Drawing.Size(75,20);
			this.spnZ.TabIndex = 10;
			this.spnZ.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// labelX2
			// 
			this.labelX2.Location = new System.Drawing.Point(228,7);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(16,19);
			this.labelX2.TabIndex = 9;
			this.labelX2.Text = "Z:";
			// 
			// DirVecEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.spnZ);
			this.Controls.Add(this.labelX2);
			this.Controls.Add(this.spnY);
			this.Controls.Add(this.labelX1);
			this.Controls.Add(this.spnX);
			this.Controls.Add(this.lblMaxValue);
			this.Name = "DirVecEditor";
			this.Size = new System.Drawing.Size(336,32);
			((System.ComponentModel.ISupportInitialize)(this.spnX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.spnY)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.spnZ)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.NumericUpDown spnX;
		private DevComponents.DotNetBar.LabelX lblMaxValue;
		private System.Windows.Forms.NumericUpDown spnY;
		private DevComponents.DotNetBar.LabelX labelX1;
		private System.Windows.Forms.NumericUpDown spnZ;
		private DevComponents.DotNetBar.LabelX labelX2;
	}
}
