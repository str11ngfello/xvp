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
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using Microsoft.Xna.Framework.Graphics;

namespace TextureBrowserLibrary
{
    /// <summary>Class describing the texture browser control</summary>
    public partial class TextureBrowser : UserControl
    {
        #region Enumerated Types

        /// <summary>Selection modes available</summary>
        public enum SelectionMode
        {
            /// <summary>Single selection support</summary>
            Single,

            /// <summary>Multiple selection support</summary>
            Multiple
        }

        #endregion

        #region Event Declarations

        public event EventHandler<TextureBrowserEventArgs> TextureSelected;
        public event EventHandler<TextureBrowserEventArgs> TextureDeselected;
        public event EventHandler<TextureBrowserEventArgs> TextureActivated;
        public event EventHandler<TextureBrowserEventArgs> TextureRightClicked;

        #endregion

        #region Property Declarations

        private SelectionMode _selection = SelectionMode.Single;

        private int _margin = 5;
        private bool _cacheImages;

        private System.Drawing.Color _canvasColor = SystemColors.ControlDark;
        private System.Drawing.Color _backgroundColor = SystemColors.Control;
        private System.Drawing.Color _foregroundColor = SystemColors.ControlText;
        private System.Drawing.Color _backgroundColorSelected = SystemColors.Highlight;
        private System.Drawing.Color _foregroundColorSelected = SystemColors.HighlightText;

        private Size _oldSize;

        private bool _rebuildCache = true;
        private bool _applyAppearance = true;

        private readonly List<TextureHandle> _textures = new List<TextureHandle>();
        private readonly List<TextureThumbnail> _thumbnails = new List<TextureThumbnail>();

        #endregion

        #region Property Accessors

        /// <summary>Gets or sets the selection mode of the control</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Selection mode of the control")]
        public SelectionMode Selection
        {
            get { return _selection; }
            set { _selection = value; }
        }

        /// <summary>Gets or sets the caching enabled flag</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Whether or not to cache loaded images")]
        public bool CacheImages
        {
            get { return _cacheImages; }
            set { _cacheImages = value; }
        }

        /// <summary>Gets or sets the canvas background color</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Background color of the texture browser panel")]
        public System.Drawing.Color CanvasColor
        {
            get { return _canvasColor; }
            set
            {
                _canvasColor = value;
                ThumbnailPanel.BackColor = _canvasColor;
            }
        }

        /// <summary>Gets or sets the background color</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Background color of the thumbnail control")]
        public System.Drawing.Color BackgroundColor
        {
            get { return _backgroundColor; }
            set
            {
                _backgroundColor = value;
                _applyAppearance = true;
                DisplayThumbnails();
            }
        }

        /// <summary>Gets or sets the foreground color</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Foreground color of the thumbnail control")]
        public System.Drawing.Color ForegroundColor
        {
            get { return _foregroundColor; }
            set
            {
                _foregroundColor = value;
                _applyAppearance = true;
                DisplayThumbnails();
            }
        }

        /// <summary>Gets or sets the background color while selected</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Background color of the thumbnail control while selected")]
        public System.Drawing.Color BackgroundColorSelected
        {
            get { return _backgroundColorSelected; }
            set
            {
                _backgroundColorSelected = value;
                _applyAppearance = true;
                DisplayThumbnails();
            }
        }

        /// <summary>Gets or sets the foreground color while selected</summary>
        [CategoryAttribute("Texture Browser Settings"),
         DescriptionAttribute("Foreground color of the thumbnail control while selected")]
        public System.Drawing.Color ForegroundColorSelected
        {
            get { return _foregroundColorSelected; }
            set
            {
                _foregroundColorSelected = value;
                _applyAppearance = true;
                DisplayThumbnails();
            }
        }

        #endregion

        #region Construction

        /// <summary>Constructor</summary>
        public TextureBrowser()
        {
            InitializeComponent();
        }

        #endregion

		public void Clear()
		{
            //Remove thumbnail controls FIRST, then and only then can we destroy the
            //textures.  If the textures are disposed before the thumbnails, an
            //exception will occur
            while (ThumbnailPanel.Controls.Count > 0)
            {
                TextureThumbnail thumbnail = ThumbnailPanel.Controls[0] as TextureThumbnail;
                ThumbnailPanel.Controls.Remove(thumbnail);
                thumbnail.Dispose();
            }
            for (int i = 0; i < _textures.Count; ++i)
                _textures[i].Dispose();
            _textures.Clear();

			_rebuildCache = true;
			DisplayThumbnails();
		}
			

        /// <summary>Adds a texture to the internal collection</summary>
        /// <param name="texture">The texture handle to add</param>
        public void AddTexture(TextureHandle texture)
        {
            if (texture != null)
            {
				texture.Loaded = true;
                _textures.Add(texture);
                _rebuildCache = true;
                //DisplayThumbnails();
            }
        }


        #region Functionality :: Remove

        /// <summary>Removes a texture from the internal collection</summary>
        /// <param name="texture">The texture to remove</param>
        public void RemoveTexture(TextureHandle texture)
        {
            if (texture != null)
            {
                _textures.Remove(texture);
                _rebuildCache = true;
                DisplayThumbnails();
            }
        }

        /// <summary>Adds an array of textures to the internal collection</summary>
        /// <param name="textures">The textures to remove</param>
        public void RemoveTextures(TextureHandle[] textures)
        {
            foreach (TextureHandle texture in textures)
            {
                RemoveTexture(texture);
            }
        }

        /// <summary>Adds a collection of textures to the internal collection</summary>
        /// <param name="textures">The textures to remove</param>
        public void RemoveTextures(List<TextureHandle> textures)
        {
            foreach (TextureHandle texture in textures)
            {
                RemoveTexture(texture);
            }
        }

        #endregion

        #region Functionality :: Generate

        
        #endregion

        #region Functionality :: Find

        /// <summary>Finds a texture handle by name</summary>
        /// <param name="name">The name to search by</param>
        /// <returns>The texture handle if found, null if none</returns>
        public TextureHandle FindTexture(string name)
        {
            TextureHandle result = null;

            foreach (TextureHandle texture in _textures)
            {
                if (texture.Name.Equals(name))
                {
                    result = texture;
                    break;
                }
            }

            return result;
        }

        #endregion

        #region Functionality :: Selection

        /// <summary>Select all texture thumbnails</summary>
        public void SelectAll()
        {
            foreach (TextureThumbnail thumbnail in _thumbnails)
            {
                thumbnail.Selected = true;

                if (TextureSelected != null)
                    TextureSelected(this, new TextureBrowserEventArgs(thumbnail.Texture));
            }
        }

        /// <summary>Deselect all texture thumbnails</summary>
        public void DeselectAll()
        {
            DeselectAll(null);
        }

        /// <summary>Deselect all texture thumbnails</summary>
        /// <param name="skip">Thumbnail to skip deselection and event notification</param>
        private void DeselectAll(TextureThumbnail skip)
        {
            foreach (TextureThumbnail thumbnail in _thumbnails)
            {
                if (skip != null && thumbnail.Equals(skip))
                    continue;

                thumbnail.Selected = false;

                if (TextureDeselected != null)
                    TextureDeselected(this, new TextureBrowserEventArgs(thumbnail.Texture));
            }
        }

        #endregion

        #region Functionality :: Display

        /// <summary>Display the thumbnails from the internal collection</summary>
        public void DisplayThumbnails()
        {
			
            _thumbnails.Clear();

            if (_rebuildCache)
            {
              
                while (ThumbnailPanel.Controls.Count > 0)
                {
                    TextureThumbnail thumbnail = ThumbnailPanel.Controls[0] as TextureThumbnail;
                    ThumbnailPanel.Controls.Remove(thumbnail);
                    thumbnail.Dispose();
                }
  
                foreach (TextureHandle texture in _textures)
                {
                    if (texture.Loaded)
                    {
                        TextureThumbnail thumbnail = new TextureThumbnail(this, texture);
                        _thumbnails.Add(thumbnail);
                    }
                }

                _rebuildCache = false;
            }
            else
            {
                foreach (TextureThumbnail thumbnail in ThumbnailPanel.Controls)
                {
                    _thumbnails.Add(thumbnail);
                }

                ThumbnailPanel.Controls.Clear();
            }

            int numberHorizontal = -1;

            foreach (TextureThumbnail thumbnail in _thumbnails)
            {

				if (numberHorizontal < 0)
                {
                    // determine how many thumbnails can be displayed on one row
                    numberHorizontal = (int)(ThumbnailPanel.Width / (thumbnail.Width != 0 ? thumbnail.Width : 1));

                    if (numberHorizontal <= 0)
                        numberHorizontal = 1;
                }


                thumbnail.Left = _margin + (thumbnail.Width + _margin) * (ThumbnailPanel.Controls.Count % numberHorizontal);
                thumbnail.Top = _margin + (thumbnail.Height + _margin) * (ThumbnailPanel.Controls.Count / numberHorizontal);

                ThumbnailPanel.Controls.Add(thumbnail);
            }

            if (_applyAppearance)
            {
                foreach (TextureThumbnail thumbnail in ThumbnailPanel.Controls)
                {
                    if (thumbnail.Selected)
                    {
                        thumbnail.BackColor = BackgroundColorSelected;
                        thumbnail.ForeColor = ForegroundColorSelected;
                    }
                    else
                    {
                        thumbnail.BackColor = BackgroundColor;
                        thumbnail.ForeColor = ForegroundColor;
                    }
                }

                _applyAppearance = false;
            }
        }

        #endregion

        #region Thumbnail Notifications

        /// <summary>Fire a texture selected event</summary>
        /// <param name="thumbnail">The thumbnail associated with the event</param>
        internal void PerformSelect(TextureThumbnail thumbnail)
        {
            switch (_selection)
            {
                case SelectionMode.Single:
                {
                    DeselectAll();
                    thumbnail.Selected = true;
                    if (TextureSelected != null)
                        TextureSelected(this, new TextureBrowserEventArgs(thumbnail.Texture));
                    break;
                }

                case SelectionMode.Multiple:
                {
                    if (Control.ModifierKeys == Keys.Control)
                    {
                        thumbnail.Selected = !thumbnail.Selected;

                        if (thumbnail.Selected)
                        {
                            if (TextureSelected != null)
                                TextureSelected(this, new TextureBrowserEventArgs(thumbnail.Texture));
                        }
                        else
                        {
                            if (TextureDeselected != null)
                                TextureDeselected(this, new TextureBrowserEventArgs(thumbnail.Texture));
                        }

                        break;
                    }
                    else
                    {
                        DeselectAll();

                        thumbnail.Selected = true;

                        if (TextureSelected != null)
                            TextureSelected(this, new TextureBrowserEventArgs(thumbnail.Texture));
                    }
                    break;
                }
            }
        }

        /// <summary>Fire a texture activated event</summary>
        /// <param name="thumbnail">The thumbnail associated with the event</param>
        internal void PerformActivated(TextureThumbnail thumbnail)
        {
            if (TextureActivated != null)
                TextureActivated(this, new TextureBrowserEventArgs(thumbnail.Texture));
        }

        /// <summary>Fire a texture right clicked event</summary>
        /// <param name="thumbnail">The thumbnail associated with the event</param>
        internal void PerformRightClicked(TextureThumbnail thumbnail)
        {
            if (TextureRightClicked != null)
                TextureRightClicked(this, new TextureBrowserEventArgs(thumbnail.Texture));
        }

        #endregion

        #region Event Handlers

        /// <summary>Event fired when the mouse button is clicked above the thumbnail panel</summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">The arguments of the event</param>
        private void ThumbnailPanel_MouseClick(object sender, MouseEventArgs e)
        {
            DeselectAll();
        }

        /// <summary>Event fired when the texture browser is resized</summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">The arguments of the event</param>
        private void TextureBrowser_Resize(object sender, System.EventArgs e)
        {
            if (_oldSize != ThumbnailPanel.Size)
            {
                _oldSize = ThumbnailPanel.Size;
                this.DisplayThumbnails();
            }
        }

        #endregion

		private void btnEdit_Click(object sender,EventArgs e)
		{

		}

				
    }
}