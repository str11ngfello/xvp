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
    /// <summary>Image loader utilizing managed Direct3D</summary>
    public class Direct3DLoader : IDisposable
    {
        /// <summary>The managed Direct3D device instance</summary>
        private GraphicsDevice _device;

		/// <summary>The context handle reference</summary>
		private System.IntPtr _contextHandle;

		/// <summary>Gets or sets the context handle reference</summary>
		/// <value>A System.IntPtr handle reference</value>
		public System.IntPtr ContextHandle
		{
			get { return _contextHandle; }
			set { _contextHandle = value; }
		}

        /// <summary>Loads a bitmap from the file system</summary>
        /// <param name="fileName">The file name of the bitmap</param>
        /// <returns>A bitmap object representing the image</returns>
        public Bitmap Load(string fileName,out TextureInformation imageInfo)
        {
            try
            {
                Bitmap result = null;

                using (Texture2D texture = Texture2D.FromFile(_device, fileName))
				{
					imageInfo = Texture2D.GetTextureInformation(fileName);
		
                /*    using (GraphicsStream stream = TextureLoader.SaveToStream(ImageFileFormat.Bmp, texture))
                    {
                        result = new Bitmap(stream);
                    }*/
                }
			
                return result;
            }
            catch
            {
				imageInfo = new TextureInformation();
                return null;
            }
        }

        /// <summary>Constructor</summary>
        /// <param name="contextHandle">The context handle to store for managed Direct3D</param>
        public Direct3DLoader(System.IntPtr contextHandle)
		{
			if (_device == null)
			{
                PresentationParameters presentParams = new PresentationParameters();
				presentParams.IsFullScreen = false;
				presentParams.SwapEffect = SwapEffect.Discard;
				_device = new GraphicsDevice(GraphicsAdapter.DefaultAdapter,DeviceType.Hardware,contextHandle,presentParams);
			}
		}

        #region IDisposable Members

        /// <summary>Releases memory resources</summary>
        public void Dispose()
        {
            if (_device != null)
            {
                _device.Dispose();
                _device = null;
            }
        }

        #endregion
    }
}