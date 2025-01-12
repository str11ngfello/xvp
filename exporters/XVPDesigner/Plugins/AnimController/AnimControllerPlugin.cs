using System;
using System.Collections.Generic;
using System.Text;
using XVPDesigner.Managers;
using XVPDesigner.Objects;
using XVPDesigner;
using System.Drawing;
using System.Windows.Forms;
using XVPDesigner.Dialogs;
using XVPDesigner.UserControls;
using XVPManaged;
using System.ComponentModel;
using System.Windows.Forms.Design;
using System.Runtime.InteropServices;
using System.IO;
using XVPDesigner.Interfaces;

namespace AnimControllerPlugin
{
	[ControllerPluginDescriptor]
	public class AnimControllerPlugin : BasePlugin
	{
		private AnimPreviewBar m_animPreviewControl;
		//private AnimController m_animController = null;

		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Animation";
			
			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\camera.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}

			//Create exactly one instance of the action control
			m_animPreviewControl = new AnimPreviewBar();

			return true;
		}

		public override Controller createControllerInstance(string name)
		{
			AnimController animController = new AnimController(name);
			return animController;
		}

		/*public override bool createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			TreeNode selectedNode = sceneExplorer.getHierarchyTreeView().SelectedNode;
			AttachControllerDialog dialog = new AttachControllerDialog();
			dialog.init(PluginID,selectedNode);

			if (dialog.ShowDialog() == DialogResult.OK)
			{
				//Create new anim controller 
				AnimController animController = new AnimController(dialog.txtName.Text);
				string nodeGUID = System.Guid.NewGuid().ToString();
				sceneExplorer.addNode(nodeGUID,animController);

				TreeNode aNode = null;
				for (int x = 0; x < dialog.m_selectedNodeGUIDs.Count; ++x)
				{
					//Find the node in the scene
					TreeNode[] nodes = null;
					if (selectedNode.Name == (string)dialog.m_selectedNodeGUIDs[x])
					{
						nodes = new TreeNode[1];
						nodes[0] = selectedNode;
					}
					else
					{
						nodes = selectedNode.Nodes.Find((string)dialog.m_selectedNodeGUIDs[x],true);
					}
					for (int i = 0; i < nodes.Length; ++i)
					{
						TreeNode newNode = nodes[i].Nodes.Add(nodeGUID,dialog.txtName.Text);
						newNode.ImageKey = newNode.SelectedImageKey = "Controller";
						sceneExplorer.hookupMenus(newNode);

						//Store the client in the controller itself
						//animController.Clients.Add(nodes[i]);

						//Save at least one node to create the proxy instance with later
						aNode = newNode;
					}
				}

				//Create the proxy instance.
				createProxyInstance(XVPMHandle,sceneExplorer,aNode);

				//Break out new controller to controller view
				sceneExplorer.findControllers(selectedNode);

				//Don't keep a reference to the controller.  The variable was just used
				//temporarily
				//m_animController = null;
			}
			else
				return false;

			dialog.Dispose();

			return true;
		}*/

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				AnimController animController = sceneExplorer.getNode(node.Name) as AnimController;
				animController.Proxy = XVPM.initAnimController(XVPMHandle);
				animController.XVPMHandle = XVPMHandle;
			}

			return true;
		}

		/*public override bool attachControllerInstance()
		{
			//Get the root tree node and send the attachment notification to it  
			TreeNode rootNode = node;
			while (rootNode.Parent != null)
				rootNode = rootNode.Parent;
			SceneNode rootSceneNode = sceneExplorer.getNode(rootNode.Name) as SceneNode;
			rootSceneNode.attachController(m_animController.Proxy,node);

			//Program.g_mainForm.PluginMgr.attachControllerInstance(rootNode,m_animControllerTreeNode,node);

		}*/

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				AnimController animController = sceneExplorer.getNode(node.Name) as AnimController;

				//First, we need to detach the controller from all the scene nodes it may be attached to
				/*foreach (TreeNode client in animController.Clients)
				{
					//Get the root tree node and send the attachment notification to it  
					TreeNode rootNode = client;
					while (rootNode.Parent != null)
						rootNode = rootNode.Parent;
					SceneNode rootSceneNode = sceneExplorer.getNode(rootNode.Name) as SceneNode;
					rootSceneNode.detachController(animController.Proxy,client);
				}*/
				//animController.Clients.Clear();

				//Now destroy the animation controller itelf
				XVPM.destroyAnimController(animController.Proxy);
			}

			return true;
		}

		public override ActionExplorerBase getActionExplorer()
		{
			return m_animPreviewControl;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Stick the number of lods in the manifest
			//results.manifestLines.Add("NumLODs");
			//results.manifestLines.Add(topLevelNode.Nodes.Count.ToString());

			//Get the anim controller node				
			AnimController animController = sceneExplorer.getNode(topLevelNode.Name) as AnimController;

			//Create a new file for the instance we're exporting
			/*FileStream fsout = null;
			try
			{
				fsout = File.Open(exportDir + "\\" + topLevelNode.Name + ".con",FileMode.Create);
				results.filenames.Add(topLevelNode.Name + ".con");
				results.manifestLines.Add(topLevelNode.Text);
				results.manifestLines.Add(topLevelNode.Name + ".con");
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + exportDir + "\\" + topLevelNode.Name + ".con" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return null;
			}
			BinaryWriter bw = new BinaryWriter(fsout);
			*/
			//Spit out the animations and build a string of anim track names
			if (animController.Animations != null)
			{
				string s = "";

				results.manifestLines.Add("NumTracks");
				results.manifestLines.Add(animController.Animations.Length.ToString());

				foreach (AnimController.AnimationTrack at in animController.Animations)
				{
					//Stick the anim track into the wad
					results.filenames.Add(at.Filename);

					s += Path.GetFileName(at.Filename) + ":";

					//Add the track name to the list
					//Utility.writeCString(bw,Path.GetFileName(at.Filename));
				}

				results.manifestLines.Add("Tracks");
				results.manifestLines.Add(s);
			}


		/*	else
			{
				bw.Write((uint)0);
			}*/

			//bw.Close();
			//fsout.Close();

			return results;
		}
	}

	[Serializable]
	public class AnimController : Controller
	{
		[Serializable]
		public class AnimationTrack
		{
			private string m_name;
			[CategoryAttribute("Animation Track"),
			DescriptionAttribute("Animation Track Name")]
			public string Name
			{
				get { return m_name; }
				set { m_name = value; }
			}

			private string filename;
			[CategoryAttribute("Animation Track"),
			DescriptionAttribute("Animation Track Filename"),
			Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]
			public string Filename
			{
				get { return filename; }
				set { filename = value; }
			}
		};

		private AnimationTrack[] animations;
		[CategoryAttribute("Animation Tracks"),
		DescriptionAttribute("Animation track list.")]
		public AnimationTrack[] Animations
		{
			get { return animations; }
			set
			{
				if (value != null)
				{
					//Check to make sure filenames are valid
					for (uint i = 0; i < value.Length; ++i)
					{
						if (value[i].Filename == null)
						{
							MessageBox.Show("One or more animation track filenames for Animation Controller \"" + Name + "\" is null.","Invalid Parameters",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return;
						}
						else if (!File.Exists(value[i].Filename))
						{
							MessageBox.Show("Animation Controller \"" + Name + "\" could not find one of its animation files - \"" + value[i].Filename + "\"","Missing Files",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return;
						}
					}
				}

				//We need to load the animation tracks into the controller
				lock (Program.g_mainForm.XVPMLockObject)
				{
					if (value != null)
					{
						XVPM.setNumAnimTracks(Proxy,(uint)value.Length);
						for (uint i = 0; i < value.Length; ++i)
						{
							IntPtr pTrackFilename = Marshal.StringToHGlobalAnsi(value[i].Filename);
							XVPM.setAnimTrack(Proxy,i,pTrackFilename);
							Marshal.FreeHGlobal(pTrackFilename);
						}
					}
					else
						XVPM.setNumAnimTracks(Proxy,0);
				}

				animations = value;

				//We must refresh the action explorer
				Program.g_mainForm.refreshActionExplorer(this);
			}
		}
		
		public AnimController(string name)
			: base(name)
		{
			PluginID = "Animation";
		}

		public override void setCurrentValues()
		{
			Animations = Animations;
			base.setCurrentValues();
		}

		public void setTrackTime(uint trackIndex,uint milliseconds)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setAnimTrackTime(Proxy,trackIndex,milliseconds);
			}
		}

		public uint getTrackTime(uint trackIndex)
		{
			if (Animations == null)
				return 0;
			if (trackIndex >= Animations.Length)
				return 0;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				return XVPM.getAnimTrackTime(Proxy,trackIndex);
			}
		}

		public uint getTrackDuration(uint trackIndex)
		{
			if (Animations == null)
				return 0;
			if (trackIndex >= Animations.Length)
				return 0;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				return XVPM.getAnimTrackDuration(Proxy,trackIndex);
			}
		}
		
		public void playTrack(uint trackIndex,float speedScale,int playCount,float weight)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.playAnimTrack(Proxy,trackIndex,speedScale,playCount,weight);
			}
		}

		public void pauseTrack(uint trackIndex)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.pauseAnimTrack(Proxy,trackIndex);
			}
		}

		public void stopTrack(uint trackIndex)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.stopAnimTrack(Proxy,trackIndex);
			}
		}

		public void stopAllTracks()
		{
			if (Animations == null)
				return;
		
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.stopAllTracks(Proxy);
			}
		}

		public void setTrackSpeed(uint trackIndex,float speed)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setAnimTrackSpeed(Proxy,trackIndex,speed);
			}
		}

		public void setTrackPlayCount(uint trackIndex,int playCount)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setAnimTrackPlayCount(Proxy,trackIndex,playCount);
			}
		}

		public void setTrackWeight(uint trackIndex,float weight)
		{
			if (Animations == null)
				return;
			if (trackIndex >= Animations.Length)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setAnimTrackWeight(Proxy,trackIndex,weight);
			}
		}
	}

}
