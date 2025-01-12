////////////////////////////////////////////////////////////////////////////////////////
//
//   Copyright (C) 2006 Graham Wihlidal
//   Game Engine Toolset Development
//   http://www.getdbook.com
//
//   MIT X.11 LICENSE
//
//   Permission is hereby granted, free of charge, to any person obtaining
//   a copy of this software and associated documentation files (the
//   "Software"), to deal in the Software without restriction, including
//   without limitation the rights to use, copy, modify, merge, publish,
//   distribute, sublicense, and/or sell copies of the Software, and to
//   permit persons to whom the Software is furnished to do so, subject to
//   the following conditions:
// 
//   The above copyright notice and this permission notice shall be
//   included in all copies or substantial portions of the Software.
// 
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////////

namespace TextureBrowserLibrary
{
    partial class TextureBrowser
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TextureBrowser));
			this.ThumbnailPanel = new System.Windows.Forms.Panel();
			this.bar1 = new DevComponents.DotNetBar.Bar();
			this.btnEdit = new DevComponents.DotNetBar.ButtonItem();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).BeginInit();
			this.SuspendLayout();
			// 
			// ThumbnailPanel
			// 
			this.ThumbnailPanel.AutoScroll = true;
			this.ThumbnailPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ThumbnailPanel.BackColor = System.Drawing.SystemColors.ControlDark;
			this.ThumbnailPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ThumbnailPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ThumbnailPanel.Location = new System.Drawing.Point(0,25);
			this.ThumbnailPanel.Name = "ThumbnailPanel";
			this.ThumbnailPanel.Size = new System.Drawing.Size(401,299);
			this.ThumbnailPanel.TabIndex = 0;
			this.ThumbnailPanel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ThumbnailPanel_MouseClick);
			// 
			// bar1
			// 
			this.bar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar1.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnEdit});
			this.bar1.Location = new System.Drawing.Point(0,0);
			this.bar1.Name = "bar1";
			this.bar1.RoundCorners = false;
			this.bar1.Size = new System.Drawing.Size(401,25);
			this.bar1.Stretch = true;
			this.bar1.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar1.TabIndex = 1;
			this.bar1.TabStop = false;
			this.bar1.Text = "bar1";
			// 
			// btnEdit
			// 
			this.btnEdit.Image = ((System.Drawing.Image)(resources.GetObject("btnEdit.Image")));
			this.btnEdit.ImagePaddingHorizontal = 8;
			this.btnEdit.Name = "btnEdit";
			this.btnEdit.Text = "buttonItem1";
			this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
			// 
			// TextureBrowser
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.ThumbnailPanel);
			this.Controls.Add(this.bar1);
			this.DoubleBuffered = true;
			this.Name = "TextureBrowser";
			this.Size = new System.Drawing.Size(401,324);
			this.SizeChanged += new System.EventHandler(this.TextureBrowser_Resize);
			((System.ComponentModel.ISupportInitialize)(this.bar1)).EndInit();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Panel ThumbnailPanel;
		private DevComponents.DotNetBar.Bar bar1;
		private DevComponents.DotNetBar.ButtonItem btnEdit;
    }
}
