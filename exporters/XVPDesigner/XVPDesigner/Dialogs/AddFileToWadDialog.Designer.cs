namespace XVPDesigner.Dialogs
{
    partial class AddFileToWadDialog
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
			this.btnBrowse = new DevComponents.DotNetBar.ButtonX();
			this.chkCompress = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.btnOK = new DevComponents.DotNetBar.ButtonX();
			this.btnCancel = new DevComponents.DotNetBar.ButtonX();
			this.lbFiles = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// btnBrowse
			// 
			this.btnBrowse.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnBrowse.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnBrowse.Location = new System.Drawing.Point(6,154);
			this.btnBrowse.MaximumSize = new System.Drawing.Size(358,20);
			this.btnBrowse.MinimumSize = new System.Drawing.Size(358,20);
			this.btnBrowse.Name = "btnBrowse";
			this.btnBrowse.Size = new System.Drawing.Size(358,20);
			this.btnBrowse.TabIndex = 1;
			this.btnBrowse.Text = "Browse For Files...";
			this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
			// 
			// chkCompress
			// 
			this.chkCompress.Location = new System.Drawing.Point(6,190);
			this.chkCompress.Name = "chkCompress";
			this.chkCompress.Size = new System.Drawing.Size(100,16);
			this.chkCompress.TabIndex = 2;
			this.chkCompress.Text = "Compress files";
			// 
			// btnOK
			// 
			this.btnOK.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnOK.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnOK.Location = new System.Drawing.Point(300,189);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(65,20);
			this.btnOK.TabIndex = 3;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCancel.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(226,189);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(66,20);
			this.btnCancel.TabIndex = 4;
			this.btnCancel.Text = "Cancel";
			// 
			// lbFiles
			// 
			this.lbFiles.FormattingEnabled = true;
			this.lbFiles.Location = new System.Drawing.Point(6,4);
			this.lbFiles.Name = "lbFiles";
			this.lbFiles.Size = new System.Drawing.Size(358,147);
			this.lbFiles.TabIndex = 5;
			// 
			// AddFileToWadDialog
			// 
			this.AcceptButton = this.btnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(369,219);
			this.ControlBox = false;
			this.Controls.Add(this.lbFiles);
			this.Controls.Add(this.btnBrowse);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.chkCompress);
			this.Controls.Add(this.btnOK);
			this.MaximumSize = new System.Drawing.Size(385,255);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(380,255);
			this.Name = "AddFileToWadDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Add Files...";
			this.ResumeLayout(false);

        }

        #endregion

        private DevComponents.DotNetBar.ButtonX btnBrowse;
        private DevComponents.DotNetBar.Controls.CheckBoxX chkCompress;
        private DevComponents.DotNetBar.ButtonX btnOK;
        private DevComponents.DotNetBar.ButtonX btnCancel;
        private System.Windows.Forms.ListBox lbFiles;

    }
}