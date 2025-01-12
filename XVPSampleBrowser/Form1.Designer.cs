namespace SampleBrowser
{
	partial class Form1
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
			this.scMain = new System.Windows.Forms.SplitContainer();
			this.comboBox1 = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.scMain.Panel1.SuspendLayout();
			this.scMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// scMain
			// 
			this.scMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.scMain.Location = new System.Drawing.Point(0,0);
			this.scMain.Name = "scMain";
			this.scMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scMain.Panel1
			// 
			this.scMain.Panel1.BackColor = System.Drawing.Color.DarkGray;
			this.scMain.Panel1.Controls.Add(this.label1);
			this.scMain.Panel1.Controls.Add(this.comboBox1);
			this.scMain.Size = new System.Drawing.Size(685,485);
			this.scMain.SplitterDistance = 90;
			this.scMain.TabIndex = 0;
			// 
			// comboBox1
			// 
			this.comboBox1.FormattingEnabled = true;
			this.comboBox1.Location = new System.Drawing.Point(64,28);
			this.comboBox1.Name = "comboBox1";
			this.comboBox1.Size = new System.Drawing.Size(175,21);
			this.comboBox1.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(9,31);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(49,13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Category";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(685,485);
			this.Controls.Add(this.scMain);
			this.Name = "Form1";
			this.Text = "Form1";
			this.scMain.Panel1.ResumeLayout(false);
			this.scMain.Panel1.PerformLayout();
			this.scMain.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer scMain;
		private System.Windows.Forms.ComboBox comboBox1;
		private System.Windows.Forms.Label label1;
	}
}

