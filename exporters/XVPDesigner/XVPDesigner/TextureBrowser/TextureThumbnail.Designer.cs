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
    partial class TextureThumbnail
    {
        
        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			this.FileNameLabel = new System.Windows.Forms.Label();
			this.DimensionsLabel = new System.Windows.Forms.Label();
			this.MaterialPreview = new System.Windows.Forms.PictureBox();
			this.superTooltip = new DevComponents.DotNetBar.SuperTooltip();
			this.lblReason = new System.Windows.Forms.Label();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.tsmiResetToDefaultTexture = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmiOpenInExternalEditor = new System.Windows.Forms.ToolStripMenuItem();
			this.blackWhiteCheckerboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.redWhiteCheckerboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.blueWhiteCheckerboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.greenWhiteCheckerboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.flatNormalMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.randomPatternToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.whiteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.blackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.grayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
			this.rToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.MaterialPreview)).BeginInit();
			this.contextMenuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// FileNameLabel
			// 
			this.FileNameLabel.BackColor = System.Drawing.SystemColors.Control;
			this.FileNameLabel.ContextMenuStrip = this.contextMenuStrip1;
			this.FileNameLabel.ForeColor = System.Drawing.SystemColors.ControlText;
			this.FileNameLabel.Location = new System.Drawing.Point(1,1);
			this.FileNameLabel.Name = "FileNameLabel";
			this.FileNameLabel.Size = new System.Drawing.Size(95,21);
			this.FileNameLabel.TabIndex = 1;
			this.FileNameLabel.Text = "File Name";
			this.FileNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.FileNameLabel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.FileNameLabel_MouseDoubleClick);
			this.FileNameLabel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.FileNameLabel_MouseClick);
			// 
			// DimensionsLabel
			// 
			this.DimensionsLabel.BackColor = System.Drawing.SystemColors.Control;
			this.DimensionsLabel.ContextMenuStrip = this.contextMenuStrip1;
			this.DimensionsLabel.Font = new System.Drawing.Font("Microsoft Sans Serif",7F,System.Drawing.FontStyle.Bold);
			this.DimensionsLabel.ForeColor = System.Drawing.SystemColors.ControlText;
			this.DimensionsLabel.Location = new System.Drawing.Point(1,118);
			this.DimensionsLabel.Name = "DimensionsLabel";
			this.DimensionsLabel.Size = new System.Drawing.Size(95,20);
			this.DimensionsLabel.TabIndex = 2;
			this.DimensionsLabel.Text = "Dimensions";
			this.DimensionsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.DimensionsLabel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.DimensionsLabel_MouseDoubleClick);
			this.DimensionsLabel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.DimensionsLabel_MouseClick);
			// 
			// MaterialPreview
			// 
			this.MaterialPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.MaterialPreview.BackColor = System.Drawing.Color.Beige;
			this.MaterialPreview.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.MaterialPreview.Location = new System.Drawing.Point(1,20);
			this.MaterialPreview.Name = "MaterialPreview";
			this.MaterialPreview.Size = new System.Drawing.Size(95,100);
			this.MaterialPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.MaterialPreview.TabIndex = 0;
			this.MaterialPreview.TabStop = false;
			this.MaterialPreview.DragOver += new System.Windows.Forms.DragEventHandler(this.MaterialPreview_DragOver);
			this.MaterialPreview.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.MaterialPreview_MouseDoubleClick);
			this.MaterialPreview.DragDrop += new System.Windows.Forms.DragEventHandler(this.MaterialPreview_DragDrop);
			this.MaterialPreview.MouseClick += new System.Windows.Forms.MouseEventHandler(this.MaterialPreview_MouseClick);
			// 
			// superTooltip
			// 
			this.superTooltip.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			this.superTooltip.ShowTooltipImmediately = true;
			// 
			// lblReason
			// 
			this.lblReason.AllowDrop = true;
			this.lblReason.BackColor = System.Drawing.Color.Beige;
			this.lblReason.ContextMenuStrip = this.contextMenuStrip1;
			this.lblReason.ForeColor = System.Drawing.SystemColors.ControlText;
			this.lblReason.Location = new System.Drawing.Point(1,32);
			this.lblReason.Name = "lblReason";
			this.lblReason.Size = new System.Drawing.Size(94,78);
			this.lblReason.TabIndex = 3;
			this.lblReason.Text = "No Image Specified";
			this.lblReason.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.lblReason.DragOver += new System.Windows.Forms.DragEventHandler(this.lblReason_DragOver);
			this.lblReason.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lblReason_MouseDoubleClick);
			this.lblReason.DragDrop += new System.Windows.Forms.DragEventHandler(this.lblReason_DragDrop);
			this.lblReason.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lblReason_MouseClick);
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.rToolStripMenuItem,
            this.toolStripMenuItem4,
            this.tsmiResetToDefaultTexture,
            this.tsmiOpenInExternalEditor});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(204,98);
			// 
			// tsmiResetToDefaultTexture
			// 
			this.tsmiResetToDefaultTexture.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.whiteToolStripMenuItem,
            this.blackToolStripMenuItem,
            this.grayToolStripMenuItem,
            this.toolStripMenuItem3,
            this.blackWhiteCheckerboardToolStripMenuItem,
            this.redWhiteCheckerboardToolStripMenuItem,
            this.greenWhiteCheckerboardToolStripMenuItem,
            this.blueWhiteCheckerboardToolStripMenuItem,
            this.randomPatternToolStripMenuItem,
            this.toolStripMenuItem1,
            this.toolStripMenuItem2,
            this.flatNormalMapToolStripMenuItem});
			this.tsmiResetToDefaultTexture.Name = "tsmiResetToDefaultTexture";
			this.tsmiResetToDefaultTexture.Size = new System.Drawing.Size(199,22);
			this.tsmiResetToDefaultTexture.Text = "Reset to Default Texture";
			this.tsmiResetToDefaultTexture.Click += new System.EventHandler(this.tsmiResetToDefaultTexture_Click);
			// 
			// tsmiOpenInExternalEditor
			// 
			this.tsmiOpenInExternalEditor.Name = "tsmiOpenInExternalEditor";
			this.tsmiOpenInExternalEditor.Size = new System.Drawing.Size(203,22);
			this.tsmiOpenInExternalEditor.Text = "Open in External Editor...";
			// 
			// blackWhiteCheckerboardToolStripMenuItem
			// 
			this.blackWhiteCheckerboardToolStripMenuItem.Name = "blackWhiteCheckerboardToolStripMenuItem";
			this.blackWhiteCheckerboardToolStripMenuItem.Size = new System.Drawing.Size(215,22);
			this.blackWhiteCheckerboardToolStripMenuItem.Text = "Black/White Checkerboard";
			this.blackWhiteCheckerboardToolStripMenuItem.Click += new System.EventHandler(this.blackWhiteCheckerboardToolStripMenuItem_Click);
			// 
			// redWhiteCheckerboardToolStripMenuItem
			// 
			this.redWhiteCheckerboardToolStripMenuItem.Name = "redWhiteCheckerboardToolStripMenuItem";
			this.redWhiteCheckerboardToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.redWhiteCheckerboardToolStripMenuItem.Text = "Red/White Checkerboard";
			this.redWhiteCheckerboardToolStripMenuItem.Click += new System.EventHandler(this.redWhiteCheckerboardToolStripMenuItem_Click);
			// 
			// blueWhiteCheckerboardToolStripMenuItem
			// 
			this.blueWhiteCheckerboardToolStripMenuItem.Name = "blueWhiteCheckerboardToolStripMenuItem";
			this.blueWhiteCheckerboardToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.blueWhiteCheckerboardToolStripMenuItem.Text = "Blue/White Checkerboard";
			this.blueWhiteCheckerboardToolStripMenuItem.Click += new System.EventHandler(this.blueWhiteCheckerboardToolStripMenuItem_Click);
			// 
			// greenWhiteCheckerboardToolStripMenuItem
			// 
			this.greenWhiteCheckerboardToolStripMenuItem.Name = "greenWhiteCheckerboardToolStripMenuItem";
			this.greenWhiteCheckerboardToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.greenWhiteCheckerboardToolStripMenuItem.Text = "Green/White Checkerboard";
			this.greenWhiteCheckerboardToolStripMenuItem.Click += new System.EventHandler(this.greenWhiteCheckerboardToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(215,6);
			// 
			// flatNormalMapToolStripMenuItem
			// 
			this.flatNormalMapToolStripMenuItem.Name = "flatNormalMapToolStripMenuItem";
			this.flatNormalMapToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.flatNormalMapToolStripMenuItem.Text = "Flat Normal Map";
			this.flatNormalMapToolStripMenuItem.Click += new System.EventHandler(this.flatNormalMapToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(215,6);
			// 
			// randomPatternToolStripMenuItem
			// 
			this.randomPatternToolStripMenuItem.Name = "randomPatternToolStripMenuItem";
			this.randomPatternToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.randomPatternToolStripMenuItem.Text = "Black/White Cell Pattern";
			this.randomPatternToolStripMenuItem.Click += new System.EventHandler(this.randomPatternToolStripMenuItem_Click);
			// 
			// whiteToolStripMenuItem
			// 
			this.whiteToolStripMenuItem.Name = "whiteToolStripMenuItem";
			this.whiteToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.whiteToolStripMenuItem.Text = "Solid White";
			this.whiteToolStripMenuItem.Click += new System.EventHandler(this.whiteToolStripMenuItem_Click);
			// 
			// blackToolStripMenuItem
			// 
			this.blackToolStripMenuItem.Name = "blackToolStripMenuItem";
			this.blackToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.blackToolStripMenuItem.Text = "Solid Black";
			this.blackToolStripMenuItem.Click += new System.EventHandler(this.blackToolStripMenuItem_Click);
			// 
			// grayToolStripMenuItem
			// 
			this.grayToolStripMenuItem.Name = "grayToolStripMenuItem";
			this.grayToolStripMenuItem.Size = new System.Drawing.Size(218,22);
			this.grayToolStripMenuItem.Text = "Solid Gray";
			this.grayToolStripMenuItem.Click += new System.EventHandler(this.grayToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(215,6);
			// 
			// toolStripMenuItem4
			// 
			this.toolStripMenuItem4.Name = "toolStripMenuItem4";
			this.toolStripMenuItem4.Size = new System.Drawing.Size(200,6);
			// 
			// rToolStripMenuItem
			// 
			this.rToolStripMenuItem.Name = "rToolStripMenuItem";
			this.rToolStripMenuItem.Size = new System.Drawing.Size(203,22);
			this.rToolStripMenuItem.Text = "Refresh";
			this.rToolStripMenuItem.Click += new System.EventHandler(this.rToolStripMenuItem_Click);
			// 
			// TextureThumbnail
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ActiveCaption;
			this.ContextMenuStrip = this.contextMenuStrip1;
			this.Controls.Add(this.lblReason);
			this.Controls.Add(this.DimensionsLabel);
			this.Controls.Add(this.FileNameLabel);
			this.Controls.Add(this.MaterialPreview);
			this.DoubleBuffered = true;
			this.Name = "TextureThumbnail";
			this.Size = new System.Drawing.Size(97,139);
			((System.ComponentModel.ISupportInitialize)(this.MaterialPreview)).EndInit();
			this.contextMenuStrip1.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Label FileNameLabel;
        private System.Windows.Forms.Label DimensionsLabel;
		private System.Windows.Forms.PictureBox MaterialPreview;
		private DevComponents.DotNetBar.SuperTooltip superTooltip;
		private System.Windows.Forms.Label lblReason;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem tsmiResetToDefaultTexture;
		private System.Windows.Forms.ToolStripMenuItem tsmiOpenInExternalEditor;
		private System.Windows.Forms.ToolStripMenuItem blackWhiteCheckerboardToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem redWhiteCheckerboardToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem greenWhiteCheckerboardToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem blueWhiteCheckerboardToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem flatNormalMapToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem whiteToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem blackToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem grayToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem randomPatternToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem rToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
    }
}
