using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms.Design;
using XVPDesigner.Managers;
using XVPDesigner.UserControls;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace XVPDesigner.Objects
{
	[SceneNodePluginDescriptor]
	public class TexturePlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Texture";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\picture.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}


			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//Create a top level node
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;
			Texture textureNode = new Texture(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,textureNode);
			//sceneExplorer.addTree(topLevelNode);

			return topLevelNode;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Get the texture node				
			Texture texture = sceneExplorer.getNode(topLevelNode.Name) as Texture;

			if (texture != null)
			{
				//If no texture is specified, we have nothing to export
				if (texture.Filename != null && texture.Filename != "")
				{
					//Stick the texture into the wad file.  Build the cache path
					//to the converted texture...it should have been converted by now.
					string temp = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(texture.Filename);
					string dir = Path.GetDirectoryName(temp);
					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(texture.Filename) + ".xif";
					if (!File.Exists(convertedPath))
						throw new Exception("Converted texture file " + convertedPath + " could not be found.");

					results.filenames.Add(convertedPath);

					//Output the texture name to the manifest
					results.manifestLines.Add("Name");
					results.manifestLines.Add(Path.GetFileName(convertedPath));
				}
				else
					Program.g_mainForm.logMessage("No texture filename was specified for texture plugin " + topLevelNode.Name + ". Texture will not be exported.",Color.Blue);
			}
			else
				throw new Exception("Expected a texture node");
				
			return results;
		}
	}



	[Serializable]
	class Texture : SceneNode
	{
		public enum EncodingType
		{
			NONE,
			MIPMAP_ONLY,
			RGBA,
			RGBa,
			XYZ
		}

		private string m_filename;
		[CategoryAttribute("Texture"),
		DescriptionAttribute("Filename"),
		Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]
		public string Filename
		{
			get { return m_filename; }
			set { m_filename = value; }
		}

		private EncodingType m_encoding = EncodingType.NONE;
		[CategoryAttribute("Texture"),
		DescriptionAttribute("Encoding")]
		public EncodingType Encoding
		{
			get { return m_encoding; }
			set { m_encoding = value; }
		}

		public Texture(string name)
			: base(name)
		{
			PluginID = "Texture";
			AcceptsControllers = false;
			Renameable = true;

			setDefaultValues();
		}

		static public EncodingType convertEncoding(string encodingType)
		{
			switch (encodingType)
			{
				case "NONE":
					return EncodingType.NONE;
				case "MIPMAP":
					return EncodingType.MIPMAP_ONLY;
				case "RGBA":
					return EncodingType.RGBA;
				case "RGBa":
					return EncodingType.RGBa;
				case "XYZ":
					return EncodingType.XYZ;
				default:
					return EncodingType.NONE;
			}
		}
	}
}
