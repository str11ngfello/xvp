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
	public class LuaScriptPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Lua Script";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\script.png");

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
			LuaScript lsNode = new LuaScript(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,lsNode);
			//sceneExplorer.addTree(topLevelNode);

			return topLevelNode;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Get the lua script node				
			LuaScript luaScript = sceneExplorer.getNode(topLevelNode.Name) as LuaScript;

			if (luaScript != null)
			{
				//If no texture is specified, we have nothing to export
				if (luaScript.Filename != null && luaScript.Filename != "")
				{
					//Stick the lua script into the wad file... should we stick
					//a compiled version in or just the original text file?
					string temp = Program.g_mainForm.CurrentWorkspace.buildCachedScriptPath(luaScript.Filename);
					string dir = Path.GetDirectoryName(temp);
					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(luaScript.Filename) + ".luo";
					if (!File.Exists(convertedPath))
						throw new Exception("Converted script file " + convertedPath + " could not be found.");

					results.filenames.Add(convertedPath);

					//Output the texture name to the manifest
					results.manifestLines.Add("Name");
					results.manifestLines.Add(Path.GetFileName(convertedPath));
				}
				else
				{
					Program.g_mainForm.logMessage("No script filename was specified for lua script plugin \"" + topLevelNode.Text + "\". Lua script will not be exported.",Color.Red);
					return null;
				}
			}
			else
				throw new Exception("Expected a lua script node");
				
			return results;
		}
	}



	[Serializable]
	class LuaScript : SceneNode
	{
		private string m_filename;
		[CategoryAttribute("Lua Script"),
		DescriptionAttribute("Filename"),
		Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]
		public string Filename
		{
			get { return m_filename; }
			set { m_filename = value; }
		}

		private bool m_compiled = true;
		[CategoryAttribute("Compilation"),
		DescriptionAttribute("Compiled")]
		public bool Compiled
		{
			get { return m_compiled; }
			set { m_compiled = value; }
		}

		public LuaScript(string name)
			: base(name)
		{
			PluginID = "Lua Script";
			AcceptsControllers = false;
			Renameable = true;

			setDefaultValues();
		}
	}
}
