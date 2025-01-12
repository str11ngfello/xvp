namespace XVPDesigner.Dialogs
{
	partial class DuplicateNameDialog
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
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			this.txtNewName = new System.Windows.Forms.TextBox();
			this.btnCancel = new DevComponents.DotNetBar.ButtonX();
			this.btnOK = new DevComponents.DotNetBar.ButtonX();
			this.SuspendLayout();
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(15,18);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(355,43);
			this.rtxtDescription.TabIndex = 9;
			this.rtxtDescription.Text = "The node you\'re adding conflicts with the name of an existing node in the hierarc" +
    "hy.  Please provide a new name for the node you\'re adding in the box below.";
			// 
			// txtNewName
			// 
			this.txtNewName.Location = new System.Drawing.Point(15,67);
			this.txtNewName.Name = "txtNewName";
			this.txtNewName.Size = new System.Drawing.Size(355,20);
			this.txtNewName.TabIndex = 0;
			// 
			// btnCancel
			// 
			this.btnCancel.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCancel.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(225,97);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(65,21);
			this.btnCancel.TabIndex = 11;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// btnOK
			// 
			this.btnOK.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnOK.Location = new System.Drawing.Point(305,97);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(65,21);
			this.btnOK.TabIndex = 12;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// DuplicateNameDialog
			// 
			this.AcceptButton = this.btnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(387,130);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.txtNewName);
			this.Controls.Add(this.rtxtDescription);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DuplicateNameDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Duplicate Node Name";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.RichTextBox rtxtDescription;
		private DevComponents.DotNetBar.ButtonX btnCancel;
		private DevComponents.DotNetBar.ButtonX btnOK;
		public System.Windows.Forms.TextBox txtNewName;
	}
}