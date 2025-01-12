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
using System.Drawing;
using Microsoft.Xna.Framework.Graphics;

namespace TextureBrowserLibrary
{
    public class TextureHandle : IDisposable
    {
        private bool m_disposed = false;
		private string _filename;
		private string _name;
		private string _description;
		private Bitmap _image;
		private TextureInformation _info;
		private TextureThumbnail.TexDropHandler _texDropHandler;
        private bool _generate = true;
        private bool _loaded;

		/// <summary>Gets or sets the texture name</summary>
		public string FileName
		{
			get { return _filename; }
			set { _filename = value; }
		}
		
		/// <summary>Gets or sets the texture name</summary>
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

		/// <summary>Gets or sets the texture description</summary>
		public string Description
		{
			get { return _description; }
			set { _description = value; }
		}
		
		/// <summary>Gets or sets the texture bitmap object</summary>
        public Bitmap Image
        {
            get { return _image; }
            set { _image = value; }
        }


		/// <summary>Gets or sets the texture bitmap object</summary>
        public TextureInformation Info
		{
			get { return _info; }
			set { _info = value; }
		}

		/// <summary>Gets or sets the texture bitmap object</summary>
		public TextureThumbnail.TexDropHandler TexDropHandler
		{
			get { return _texDropHandler; }
			set { _texDropHandler = value; }
		}
		
		/// <summary>Gets or sets the requires generation flag</summary>
        public bool Generate
        {
            get { return _generate; }
            set { _generate = value; }
        }

        /// <summary>Gets or sets the texture loaded flag</summary>
        public bool Loaded
        {
            get { return _loaded; }
            set { _loaded = value; }
        }

        public TextureHandle(string name, string description, string filename, Bitmap image, TextureInformation info,
							 TextureThumbnail.TexDropHandler texDropHandler)
        {
			_filename = filename;
			_name = name;
			_description = description;
			_image = image;
			_info = info;
			_texDropHandler = texDropHandler;
        }


        public void Dispose()
        {
            Dispose(true);
            // Take yourself off the Finalization queue 
            // to prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        protected void Dispose(bool disposing)
        {
            if (!m_disposed)
            {
                //Only release managed resources here
                if (disposing)
                {
                }

                //Release unmanaged resources here
                if (_image != null)
                    _image.Dispose();

                //Call base dispose and mark the objects as disposed
                m_disposed = true;
            }

            /*if (disposing && (components != null))
            {
                components.Dispose();
			}*/

        }
    }
}
