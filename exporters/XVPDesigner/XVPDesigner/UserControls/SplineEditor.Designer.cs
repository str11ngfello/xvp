namespace XVPDesigner.UserControls
{
	partial class SplineEditor
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
			this.bar1 = new DevComponents.DotNetBar.Bar();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).BeginInit();
			this.SuspendLayout();
			// 
			// bar1
			// 
			this.bar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar1.Location = new System.Drawing.Point(0,0);
			this.bar1.Name = "bar1";
			this.bar1.RoundCorners = false;
			this.bar1.Size = new System.Drawing.Size(311,25);
			this.bar1.Stretch = true;
			this.bar1.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar1.TabIndex = 0;
			this.bar1.TabStop = false;
			this.bar1.Text = "bar1";
			// 
			// SplineEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.bar1);
			this.Name = "SplineEditor";
			this.Size = new System.Drawing.Size(311,280);
			((System.ComponentModel.ISupportInitialize)(this.bar1)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private DevComponents.DotNetBar.Bar bar1;

	}
}
