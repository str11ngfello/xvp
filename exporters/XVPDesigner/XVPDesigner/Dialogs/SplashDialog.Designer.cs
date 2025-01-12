namespace XVPDesigner.Dialogs
{
    partial class SplashDialog
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SplashDialog));
			this.picSplashImage = new System.Windows.Forms.PictureBox();
			this.btnClose = new DevComponents.DotNetBar.ButtonX();
			((System.ComponentModel.ISupportInitialize)(this.picSplashImage)).BeginInit();
			this.SuspendLayout();
			// 
			// picSplashImage
			// 
			this.picSplashImage.BackColor = System.Drawing.Color.White;
			this.picSplashImage.Dock = System.Windows.Forms.DockStyle.Fill;
			this.picSplashImage.Image = ((System.Drawing.Image)(resources.GetObject("picSplashImage.Image")));
			this.picSplashImage.Location = new System.Drawing.Point(0,0);
			this.picSplashImage.Name = "picSplashImage";
			this.picSplashImage.Size = new System.Drawing.Size(473,263);
			this.picSplashImage.TabIndex = 0;
			this.picSplashImage.TabStop = false;
			// 
			// btnClose
			// 
			this.btnClose.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnClose.Location = new System.Drawing.Point(405,12);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(60,26);
			this.btnClose.TabIndex = 1;
			this.btnClose.Text = "Close";
			this.btnClose.Visible = false;
			this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
			// 
			// SplashDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoValidate = System.Windows.Forms.AutoValidate.Disable;
			this.BackColor = System.Drawing.Color.White;
			this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
			this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.CausesValidation = false;
			this.ClientSize = new System.Drawing.Size(473,263);
			this.ControlBox = false;
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.picSplashImage);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SplashDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "XVP Designer";
			((System.ComponentModel.ISupportInitialize)(this.picSplashImage)).EndInit();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.PictureBox picSplashImage;
		private DevComponents.DotNetBar.ButtonX btnClose;
    }
}