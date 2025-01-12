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
	public class AudioClipPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Audio Clip";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\audioclip.png");

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
			AudioClip audioClipNode = new AudioClip(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,audioClipNode);

			//sceneExplorer.addTree(topLevelNode);

			
			return topLevelNode;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Get the audio clip node				
			AudioClip clip = sceneExplorer.getNode(topLevelNode.Name) as AudioClip;

			if (clip != null)
			{
				//If no audio is specified, we have nothing to export
				if (clip.Filename != null)
				{
					//Stick the audio clip into the wad file.  Build the cache path
					//to the converted audio...it should have been converted by now.
					string temp = Program.g_mainForm.CurrentWorkspace.buildCachedAudioPath(clip.Filename);
					string dir = Path.GetDirectoryName(temp);
					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(clip.Filename) + ".xau";
					if (!File.Exists(convertedPath))
						throw new Exception("Converted audio file " + convertedPath + " could not be found.");
					results.filenames.Add(convertedPath);

					//Output the texture name to the manifest
					results.manifestLines.Add("Name");
					results.manifestLines.Add(Path.GetFileName(convertedPath));
				}
				else
				{
					Program.g_mainForm.logMessage("No audio clip filename was specified for audio clip plugin \"" + topLevelNode.Text + "\". Audio clip will not be exported.",Color.Red);
					return null;
				}
			}
			else
				throw new Exception("Expected an audio clip node");

			return results;
		}
	}



	[Serializable]
	class AudioClip : SceneNode
	{
		private string filename;
		[CategoryAttribute("Audio Clip"),
		DescriptionAttribute("Filename"),
		Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]
		public string Filename
		{
			get { return filename; }
			set { filename = value; }
		}

		public AudioClip(string name)
			: base(name)
		{
			PluginID = "Audio Clip";
			AcceptsControllers = true;
			Renameable = true;

			setDefaultValues();
		}
	}
}
