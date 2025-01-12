namespace XVPDCCTools
{
	partial class mainForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainForm));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lbDCCs = new System.Windows.Forms.ListBox();
			this.btnCancel = new System.Windows.Forms.Button();
			this.btnInstall = new System.Windows.Forms.Button();
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.lbDCCs);
			this.groupBox1.Controls.Add(this.btnCancel);
			this.groupBox1.Controls.Add(this.btnInstall);
			this.groupBox1.Controls.Add(this.rtxtDescription);
			this.groupBox1.Location = new System.Drawing.Point(12,12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(520,186);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Supported DCCs";
			// 
			// lbDCCs
			// 
			this.lbDCCs.FormattingEnabled = true;
			this.lbDCCs.Location = new System.Drawing.Point(6,66);
			this.lbDCCs.Name = "lbDCCs";
			this.lbDCCs.Size = new System.Drawing.Size(505,82);
			this.lbDCCs.TabIndex = 0;
			// 
			// btnCancel
			// 
			this.btnCancel.Location = new System.Drawing.Point(354,156);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75,23);
			this.btnCancel.TabIndex = 12;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// btnInstall
			// 
			this.btnInstall.Location = new System.Drawing.Point(437,156);
			this.btnInstall.Name = "btnInstall";
			this.btnInstall.Size = new System.Drawing.Size(75,23);
			this.btnInstall.TabIndex = 11;
			this.btnInstall.Text = "Install";
			this.btnInstall.UseVisualStyleBackColor = true;
			this.btnInstall.Click += new System.EventHandler(this.btnInstall_Click);
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(6,19);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(505,48);
			this.rtxtDescription.TabIndex = 10;
			this.rtxtDescription.TabStop = false;
			this.rtxtDescription.Text = resources.GetString("rtxtDescription.Text");
			// 
			// mainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(542,202);
			this.Controls.Add(this.groupBox1);
			this.Name = "mainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "XVP DCC Tools Installer";
			this.TopMost = true;
			this.Load += new System.EventHandler(this.mainForm_Load);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.ListBox lbDCCs;
		private System.Windows.Forms.RichTextBox rtxtDescription;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Button btnInstall;
	}
}

