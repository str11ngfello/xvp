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

using System;
using System.Globalization;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using System.IO;
using XVPDesigner;
using XVPDesigner.UserControls;

namespace TextureBrowserLibrary
{
    /// <summary>Class the describes the texture thumbnail control</summary>
    public partial class TextureThumbnail : UserControl,IDisposable
	{

		public delegate void TexDropHandler(string texName,string texFilename);
		public event TexDropHandler OnTexDrop;

		public enum Mode
		{
			Mini,
			Full
		}

        /// <summary>The texture handle for the thumbnail</summary>
        private TextureHandle _texture;

        /// <summary>Flag specifying whether or not the thumbnail is selected</summary>
        private bool _selected;

        /// <summary>Reference to the texture browser control. Used for event relaying and color settings</summary>
        private TextureBrowser _container;

        /// <summary>Gets or sets the texture handle</summary>
        public TextureHandle Texture
        {
            get { return _texture; }
        }

        /// <summary>Gets or sets the selected flag. Also updates the appearance accordingly</summary>
        public bool Selected
        {
            get { return _selected; }
            set
            {
                if (_container == null)
                    return;

                if (value)
                {
                    this.BackColor = Color.Blue;

                    this.FileNameLabel.BackColor = _container.BackgroundColorSelected;
                    this.FileNameLabel.ForeColor = _container.ForegroundColorSelected;

                    this.DimensionsLabel.BackColor = _container.BackgroundColorSelected;
                    this.DimensionsLabel.ForeColor = _container.ForegroundColorSelected;
                }
                else
                {
                    this.BackColor = SystemColors.ActiveCaption;

                    this.FileNameLabel.BackColor = _container.BackgroundColor;
                    this.FileNameLabel.ForeColor = _container.ForegroundColor;

                    this.DimensionsLabel.BackColor = _container.BackgroundColor;
                    this.DimensionsLabel.ForeColor = _container.ForegroundColor;
                }

                _selected = value;
            }
        }

        /// <summary>Constructor</summary>
        /// <param name="container">The texture browser control</param>
        /// <param name="texture">The texture handle reference</param>
        public TextureThumbnail(TextureBrowser container, TextureHandle texture)
        {
            InitializeComponent();

			if (texture.TexDropHandler != null)
			{
				MaterialPreview.AllowDrop = true;
				OnTexDrop += texture.TexDropHandler;
			}

			_container = container;
            _texture = texture;

            if (_container != null)
            {
                this.FileNameLabel.BackColor = _container.BackgroundColor;
                this.FileNameLabel.ForeColor = _container.ForegroundColor;

                this.DimensionsLabel.BackColor = _container.BackgroundColor;
                this.DimensionsLabel.ForeColor = _container.ForegroundColor;
            }

            GenerateThumbnail();
            DisplayInformation();
        }

        /// <summary>Generates a thumbnail using the texture handle reference</summary>
        private void GenerateThumbnail()
        {
			//If no image specified...
			if (_texture.Image == null && (_texture.FileName == "" || _texture.FileName == null))
			{
				lblReason.Text = "(No Image Specified)";
				lblReason.ForeColor = Color.Black;
				lblReason.Visible = true;
				return;
			}

			//If image specified, but could not generate a thumbnail...
			if (_texture.Image == null && _texture.FileName != null)
			{
				//Either the image format cannot be previewed or the file didn't exist
				if (!File.Exists(_texture.FileName))
				{
					lblReason.Text = "(Image Does Not Exist)";
					lblReason.ForeColor = Color.Red;
				}
				else
				{
					lblReason.Text = "(No Preview Available)";
					lblReason.ForeColor = Color.Green;
				}
				lblReason.Visible = true;


				SuperTooltipInfo info = new SuperTooltipInfo(_texture.Name,
															 null,
															 _texture.Description + "\n\n" + _texture.FileName,
															 null,
															 null,
															 eTooltipColor.Yellow);
				superTooltip.SetSuperTooltip((IComponent)MaterialPreview,info);

				return;
			}


			lblReason.Visible = false;

            int maxDimension = Math.Min(MaterialPreview.Width, MaterialPreview.Height);

            int resizedWidth = _texture.Image.Width;
            int resizedHeight = _texture.Image.Height;

            if (_texture.Image.Width > maxDimension || _texture.Image.Height > maxDimension)
            {
                if (_texture.Image.Width > _texture.Image.Height)
                {
                    resizedWidth = maxDimension;
                    resizedHeight = (int)(_texture.Image.Height * maxDimension / _texture.Image.Width);
                }
                else
                {
                    resizedWidth = (int)(_texture.Image.Width * maxDimension / _texture.Image.Height);
                    resizedHeight = maxDimension;
                }
            }

			MaterialPreview.Image = _texture.Image;// new Bitmap(_texture.Image,resizedWidth,resizedHeight);


			//We display the true dimensions of the image, not what D3D may have resized it to
			//when loading in the preview thumbnail.  I.e, if the image is not a power of two
			//it gets up-sized to nearest power of 2.
            Microsoft.Xna.Framework.Graphics.TextureInformation ii;
            ii = Microsoft.Xna.Framework.Graphics.Texture2D.GetTextureInformation(_texture.FileName);
			DimensionsLabel.Text = String.Format(CultureInfo.CurrentCulture,"{0} x {1}",ii.Width,ii.Height);
			
			string footer = ii.Width + " x " + ii.Height + " pixels.";
			if (_texture.Image.Width != ii.Width || _texture.Image.Height != ii.Height)
			{
				DimensionsLabel.ForeColor = Color.Red;
				footer += "  <b>Warning:</b> Image dimensions are not a power of 2.";

			}
			
			SuperTooltipInfo info2 = new SuperTooltipInfo(_texture.Name,
														 footer,
														  _texture.Description + "\n\n" + _texture.FileName + "\n\nMip Levels: " + _texture.Info.MipLevels + "\nDepth: " + _texture.Info.Depth + "\nFormat: " + _texture.Info.Format,
														 null,
														 null,
														 eTooltipColor.Yellow);
			info2.CustomSize = new Size(300,info2.CustomSize.Height);
			//info2.
			
			superTooltip.SetSuperTooltip((IComponent)MaterialPreview,info2);
     }

		void MaterialPreview_MouseHover(object sender,EventArgs e)
		{
			//if (!superTooltip.IsTooltipVisible)
			//	superTooltip.ShowTooltip(sender);
			//((PictureBox)sender)
		}

		

        /// <summary>Update the information labels using the texture handle</summary>
        private void DisplayInformation()
        {
			//Cap long strings to fit cleanly
			if (_texture.Name.Length > 18)
			{
				this.FileNameLabel.Text = _texture.Name.Substring(0,15) + "...";
			}
			else
				this.FileNameLabel.Text = _texture.Name;
        }

        /// <summary>Fire a select event to the texture browser control</summary>
        private void ToggleSelection()
        {
            if (_container != null)
                _container.PerformSelect(this);
        }

        /// <summary>Event fired when the mouse is clicked above the material preview panel</summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">The arguments of the event</param>
        private void MaterialPreview_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ToggleSelection();
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (_container != null)
                    _container.PerformRightClicked(this);
            }
        }

        /// <summary>Event fired when the mouse is double clicked above the material preview panel</summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">The arguments of the event</param>
        private void MaterialPreview_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            //if (_container != null)
              //  _container.PerformActivated(this);


			//Only allow double click browsing for texture thumbnails that have a handler
			//associated with them.  This will also prevent texture preview thumbnails
			//from getting this fucntionality since they don't have handlers for this event
			if (OnTexDrop != null)
			{
				OpenFileDialog dlgOpenFileDialog = new OpenFileDialog();
				dlgOpenFileDialog.Title = "Open Texture...";
				dlgOpenFileDialog.Filter = "All files (*.*)|*.*";
				dlgOpenFileDialog.FileName = "";
				dlgOpenFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.TextureLibraryPath;
				DialogResult result = dlgOpenFileDialog.ShowDialog();
				if (result == DialogResult.OK)
				{
					sendTexDropEvent(_texture.Name,dlgOpenFileDialog.FileName);
				}
				dlgOpenFileDialog.Dispose();
			}
		}

		private void lblReason_MouseClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseClick(sender, e);
		}

		private void DimensionsLabel_MouseClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseClick(sender, e);
		}

		private void FileNameLabel_MouseClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseClick(sender, e);
		}

		private void lblReason_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseDoubleClick(sender, e);
		}

		private void DimensionsLabel_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseDoubleClick(sender, e);
		}

		private void FileNameLabel_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			//Forward to thumbnail
			MaterialPreview_MouseDoubleClick(sender, e);
		}

        /// <summary> 
        /// Required designer variable.
        /// </summary>
        //private System.ComponentModel.IContainer components = null;
        private bool m_disposed = false;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (!m_disposed)
            {
                //Only release managed resources here
                if (disposing)
                {
                    if (components != null)
                        components.Dispose();

                    superTooltip.SetSuperTooltip((IComponent)MaterialPreview,null);
                }

                //Release unmanaged resources here

                //Call base dispose and mark the objects as disposed
                base.Dispose(disposing);
                m_disposed = true;
            }

            /*if (disposing && (components != null))
            {
                components.Dispose();
			}*/

        }
		
		private void MaterialPreview_DragOver(object sender,DragEventArgs e)
		{
			PictureBox pic = (PictureBox)sender;
			e.Effect = DragDropEffects.None;
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			if (nodeSource != null)
			{
				switch (nodeSource.ImageKey)
				{
					case "Texture":
						e.Effect = DragDropEffects.Copy;
						break;
				}
			}
		}

		private void lblReason_DragOver(object sender,DragEventArgs e)
		{
			Label lbl = (Label)sender;
			e.Effect = DragDropEffects.None;
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));
			
			if (nodeSource != null)
			{
				switch (nodeSource.ImageKey)
				{
					case "Texture":
						e.Effect = DragDropEffects.Copy;
						break;
				}
			}
		}

		private void MaterialPreview_DragDrop(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Texture":
					sendTexDropEvent(_texture.Name,nodeSource.Name);
					break;
			}		
		}

		private void lblReason_DragDrop(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Texture":
					sendTexDropEvent(_texture.Name,nodeSource.Name);
					break;
			}		
		}

		private void sendTexDropEvent(string textureName, string nodeSourceName)
		{
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;
			if (scene != null)
				scene.Dirty = true;
			OnTexDrop(textureName,nodeSourceName);
		}

		private void tsmiResetToDefaultTexture_Click(object sender,EventArgs e)
		{
		}

		private void blackWhiteCheckerboardToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/defaultTexture.png");
		}

		private void whiteToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/solidWhite.png");

		}

		private void blackToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/solidBlack.png");
		}

		private void grayToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/solidGray.png");
		}

		private void redWhiteCheckerboardToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/missingMaterial.png");
		}

		private void blueWhiteCheckerboardToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/missingTexture.png");
		}

		private void greenWhiteCheckerboardToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/streamingTexture.png");
		}

		private void randomPatternToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/patternTexture.png");
		}

		private void flatNormalMapToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,Program.g_mainForm.XVPPath + "xvpsdk/system/textures/defaultNormalMap.png");
		}

		private void rToolStripMenuItem_Click(object sender,EventArgs e)
		{
			OnTexDrop(_texture.Name,_texture.FileName);
		}

		private void refreshAllToolStripMenuItem_Click(object sender,EventArgs e)
		{
			
		}
    }
}