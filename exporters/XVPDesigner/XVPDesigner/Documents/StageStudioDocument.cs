using System;
using System.Collections.Generic;
using System.Text;
using XVPDesigner.UserControls;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Collections;
using System.Windows.Forms;
using System.Drawing;
using DevComponents.DotNetBar;
using XVPDesigner.Objects;
using System.Diagnostics;
using XVPManaged;
using XVPDesigner.Managers;
using System.Runtime.InteropServices;

namespace XVPDesigner.Documents
{
	public class StageStudioDocument : XVPMDocument
	{
		private SceneExplorer m_sceneExplorer;
		private PropertyExplorer m_stagePropertyExplorer;
		private XVPMControl m_XVPMControl;
		public XVPMControl XVPMControl
		{
			get { return m_XVPMControl; }
		}
		private ViewportConfig m_viewportConfig;
		StreamReader m_streamReader;
		List<string> m_filesForWad = new List<string>();
		private DevComponents.DotNetBar.TabItem m_tabItem;

		private string m_currentStagePath = null;
		public string StageFilename
		{
			get { return m_currentStagePath; }
		}

		public bool Dirty
		{
			get { return (m_sceneExplorer.Dirty || m_stagePropertyExplorer.Dirty); }
		}

		public uint XVPMHandle
		{
			get { return m_XVPMControl.XVPMHandle; }
		}

		public SceneExplorer SceneExplorer
		{
			get { return m_sceneExplorer; }
		}

		public PropertyExplorer PropertyExplorer
		{
			get { return m_stagePropertyExplorer; }
		}

		override public void setActive(bool active)
		{
			if (m_XVPMControl != null)
				m_XVPMControl.activate(active);

		/*	if (active)
			{
				RibbonPanel rp = (RibbonPanel)mainRibbon.Controls["ribpanStageStudio"];
				RibbonBar rb = rp.Controls["ribbarStageMovement"];
				.Controls["btnStageMove"].Enabled = false;
			}*/

			m_active = active;
		}

		public void Dispose()
		{
			if (m_sceneExplorer != null)
			{
				m_sceneExplorer.Dispose();
				m_sceneExplorer = null;
			}
			if (m_stagePropertyExplorer != null)
			{
				m_stagePropertyExplorer.Dispose();
				m_stagePropertyExplorer = null;
			}
			if (m_XVPMControl != null)
			{
				m_XVPMControl.Release();
				m_XVPMControl = null;
			}

		}

		public override bool create(TabItem tab,string name)
		{
			Dispose();

			m_viewportConfig = new ViewportConfig();

			m_sceneExplorer = new SceneExplorer();
			m_sceneExplorer.Dock = DockStyle.Fill;
			m_sceneExplorer.CreateActorGroup = true;
			m_stagePropertyExplorer = new PropertyExplorer();
			m_stagePropertyExplorer.Dock = DockStyle.Fill;
			m_sceneExplorer.Dirty = false;
			m_stagePropertyExplorer.Dirty = false;

			m_sceneExplorer.PropertyExplorer = m_stagePropertyExplorer;

			m_name = name;
			m_currentStagePath = null;


			//Open an XVP control instance and name it after the stage
			m_XVPMControl = new XVPMControl(this);
			m_XVPMControl.Hide();
			m_XVPMControl.Dock = DockStyle.Fill;

			//Apply viewport config
			m_XVPMControl.applyViewportConfig(m_viewportConfig);

			//Add XVPM control to the tab
			m_tabItem = tab;
			tab.AttachedControl.Controls.Add(m_XVPMControl);


			return true;
		}

		public override bool open(TabItem tab,string filename)
		{
			Dispose();

			//Open an XVP control instance and name it after the stage. The XVPM control
			//should be the first thing created in open()
			m_XVPMControl = new XVPMControl(this);
			m_XVPMControl.Dock = DockStyle.Fill;

			//Add XVPM control to the tab
			m_tabItem = tab;
			tab.AttachedControl.Controls.Add(m_XVPMControl);

			m_sceneExplorer = new SceneExplorer();
			m_sceneExplorer.Dock = DockStyle.Fill;
			m_sceneExplorer.CreateActorGroup = true;
			m_stagePropertyExplorer = new PropertyExplorer();
			m_stagePropertyExplorer.Dock = DockStyle.Fill;

			m_sceneExplorer.init(XVPMHandle,Program.g_mainForm.getShaderMgr().getShaderNames());
			m_sceneExplorer.PropertyExplorer = m_stagePropertyExplorer;

			FileStream fs = null;

			//We deserialize in this order.  Then we must call fixup
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				m_viewportConfig = (ViewportConfig)bf.Deserialize(fs);
				m_sceneExplorer.reset();
				m_sceneExplorer.Hashtable = (Hashtable)bf.Deserialize(fs);
				m_sceneExplorer.setNodeArray((ArrayList)bf.Deserialize(fs));

				m_sceneExplorer.postDeserializeFixup();
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not open stage.  Check the message log for details.","Open Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (fs != null)
					fs.Close();
				return false;
			}

			if (fs != null)
				fs.Close();

			m_currentStagePath = filename;
			m_sceneExplorer.Dirty = false;
			m_stagePropertyExplorer.Dirty = false;
			m_viewportConfig.Dirty = false;

			Program.g_mainForm.addRecentStage(filename);
			//SceneDockWindow.Text = Path.GetFileName(m_currentStagePath);

			return true;
		}

		public override bool save()
		{
			FileStream fs = null;

			if (m_currentStagePath == null)
			{
				SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
				dlgSaveFileDialog.Title = "Save Stage...";
				dlgSaveFileDialog.Filter = "Stage Files (*.sta)|*.sta|All files (*.*)|*.*";
				dlgSaveFileDialog.FileName = "Stage.sta";
				dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ActorLibraryPath;
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result != DialogResult.OK)
					return false;

				m_currentStagePath = dlgSaveFileDialog.FileName;
				m_tabItem.Name = dlgSaveFileDialog.FileName;
				m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				dlgSaveFileDialog.Dispose();
			}


			try
			{
				fs = File.Open(m_currentStagePath,FileMode.Create);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + m_currentStagePath + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return false;
			}

			//Grab all the actor transforms and store them before the save
			List<TreeNode> actorNodes = new List<TreeNode>();
			foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
				findActorContainersRecursive(node,actorNodes);

			for (int i = 0;i < actorNodes.Count;++i)
			{
				XVPDesigner.Objects.ActorContainer actorContainer = m_sceneExplorer.getNode(actorNodes[i].Name) as XVPDesigner.Objects.ActorContainer;
				actorContainer.upSyncTransform();
			}

			BinaryFormatter bf = new BinaryFormatter();
			bf.Serialize(fs,m_viewportConfig);
			bf.Serialize(fs,m_sceneExplorer.Hashtable);
			bf.Serialize(fs,m_sceneExplorer.getNodeArray());

			fs.Close();

			m_stagePropertyExplorer.Dirty = false;
			m_sceneExplorer.Dirty = false;
			m_viewportConfig.Dirty = false;

			Program.g_mainForm.addRecentStage(m_currentStagePath);

			return true;
		}

		public override bool saveAs()
		{
			SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
			dlgSaveFileDialog.Title = "Save Stage As...";
			dlgSaveFileDialog.Filter = "Stage Files (*.sta)|*.sta|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Stage.sta";
			dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ActorLibraryPath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();
			if (result == DialogResult.OK)
			{
				FileStream fs = null;
				try
				{
					fs = File.Open(dlgSaveFileDialog.FileName,FileMode.Create);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + dlgSaveFileDialog.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					dlgSaveFileDialog.Dispose();
					return false;
				}

				//Grab all the actor transforms and store them before the save
				List<TreeNode> actorNodes = new List<TreeNode>();
				foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
					findActorContainersRecursive(node,actorNodes);

				for (int i = 0;i < actorNodes.Count;++i)
				{
					XVPDesigner.Objects.ActorContainer actorContainer = m_sceneExplorer.getNode(actorNodes[i].Name) as XVPDesigner.Objects.ActorContainer;
					actorContainer.upSyncTransform();
				}

				BinaryFormatter bf = new BinaryFormatter();
				bf.Serialize(fs,m_viewportConfig);
				bf.Serialize(fs,m_sceneExplorer.Hashtable);
				bf.Serialize(fs,m_sceneExplorer.getNodeArray());

				fs.Close();
				m_stagePropertyExplorer.Dirty = false;
				m_sceneExplorer.Dirty = false;
				m_currentStagePath = dlgSaveFileDialog.FileName;
				m_tabItem.Name = dlgSaveFileDialog.FileName;
				m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				dlgSaveFileDialog.Dispose();

				Program.g_mainForm.addRecentStage(m_currentStagePath);

				return true;
			}

			return false;
		}

		public override bool close()
		{
			if (m_sceneExplorer.Dirty || m_stagePropertyExplorer.Dirty || m_viewportConfig.Dirty)
			{
				//Show document
				m_tabItem.Parent.SelectedTab = m_tabItem;

				bool success = true;
				DialogResult dr = MessageBox.Show(m_tabItem.Text + " has been modified.  Would you like to save?","Stage Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				switch (dr)
				{
					case DialogResult.Yes:
						success = save();
						break;
					case DialogResult.Cancel:
						return false;
				}
				if (!success)
					return false;
			}
			
			//First, detach all controllers from all plugins
			foreach (TreeNode controllerGroupNode in m_sceneExplorer.getControllerTreeView().Nodes)
			{
				//Find the matching hierarchy node
				TreeNode[] matchingHierarchyNode = m_sceneExplorer.getHierarchyTreeView().Nodes.Find(controllerGroupNode.Name,true);
				if (matchingHierarchyNode.Length != 1)
					throw new Exception("Expected to find exactly one hierarchy node.");

				foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
				{
					TreeNode[] controllerNodes = matchingHierarchyNode[0].Nodes.Find(controllerNode.Name,true);
					for (int i = 0; i < controllerNodes.Length; ++i)
					{
						//Grab the root hierarchy node so we can send the detach notification to the particular plugin
						//that has the controller attached
						SceneNode sceneNode = m_sceneExplorer.Hashtable[controllerGroupNode.Name] as SceneNode;
						Controller controller = m_sceneExplorer.Hashtable[controllerNodes[i].Name] as Controller;
						sceneNode.detachController(controller.Proxy,controllerNodes[i].Parent);
					}
				}
			}

			//Then destroy controller proxy instances for all plugins
			foreach (TreeNode node in m_sceneExplorer.getControllerTreeView().Nodes)
			{
				foreach (TreeNode controllerNode in node.Nodes)
				{
					//Get the node's type ID
					SceneNode sceneNode = (SceneNode)m_sceneExplorer.Hashtable[controllerNode.Name];
					Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_sceneExplorer,controllerNode);
				}
			}

			//Now, destroy all other proxy instances for plugins
			foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
			{
				destroyProxyInstancesRecursive(node);
				//Get the node's type ID
				/*SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[node.Name];
				if (sceneNode == null)
					continue;

				Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_actorSceneExplorer,node);*/
			}

			Dispose();
			m_currentStagePath = null;
			base.close();
			return true;
		}

		private void destroyProxyInstancesRecursive(TreeNode node)
		{
			//Skip controllers since they've already been destroyed
			if (node.ImageKey != "Controller")
			{
				//Get the node's type ID
				SceneNode sceneNode = (SceneNode)m_sceneExplorer.Hashtable[node.Name];

				//If the scene node is an actor container, don't call the plugin destroy
				if (sceneNode != null)
				{
					if (sceneNode as Objects.ActorContainer != null)
					{
						Objects.ActorContainer ac = sceneNode as Objects.ActorContainer;
						ac.destroyProxyInstance();
					}
					else
						Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_sceneExplorer,node);
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				destroyProxyInstancesRecursive(childNode);
		}

		public override bool postLoad()
		{
			//Add proxy instances
			foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
				createProxyInstanceRecursive(node);

			//Then create controller proxy instances for all controllers
			foreach (TreeNode node in m_sceneExplorer.getControllerTreeView().Nodes)
			{
				foreach (TreeNode controllerNode in node.Nodes)
				{
					Controller controller = m_sceneExplorer.Hashtable[controllerNode.Name] as Controller;
					Program.g_mainForm.PluginMgr.createProxyInstance(controller.PluginID,m_sceneExplorer,controllerNode);
				}
			}

			//Attach controllers to the necessary nodes
			foreach (TreeNode controllerGroupNode in m_sceneExplorer.getControllerTreeView().Nodes)
			{
				//Find the matching hierarchy node
				TreeNode[] matchingHierarchyNode = m_sceneExplorer.getHierarchyTreeView().Nodes.Find(controllerGroupNode.Name,true);
				if (matchingHierarchyNode.Length != 1)
					throw new Exception("Expected to find exactly one hierarchy node.");

				foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
				{
					TreeNode[] controllerNodes = matchingHierarchyNode[0].Nodes.Find(controllerNode.Name,true);
					for (int i = 0; i < controllerNodes.Length; ++i)
					{
						//Grab the root hierarchy node so we can send the attach notification to the particular plugin
						//that has the controller attached
						SceneNode sceneNode = m_sceneExplorer.Hashtable[controllerGroupNode.Name] as SceneNode;
						Controller controller = m_sceneExplorer.Hashtable[controllerNodes[i].Name] as Controller;
						sceneNode.attachController(controller.Proxy,controllerNodes[i].Parent);
					}
				}
			}

			//Grab all the actor transforms and store them before the load
			List<TreeNode> actorNodes = new List<TreeNode>();
			foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
				findActorContainersRecursive(node,actorNodes);

			for (int i = 0;i < actorNodes.Count;++i)
			{
				XVPDesigner.Objects.ActorContainer actorContainer = m_sceneExplorer.getNode(actorNodes[i].Name) as XVPDesigner.Objects.ActorContainer;
				actorContainer.downSyncTransform();

				//For each plugin in the actor container, set the actor container
				foreach (TreeNode childNode in actorNodes[i].Nodes)
					m_sceneExplorer.setActorContainerRecursive(childNode,actorContainer);
			}

			//Check to see if any actors source files have been updated
			CheckForUpdatedActorFiles(actorNodes);

			//Apply the viewport config
			m_XVPMControl.applyViewportConfig(m_viewportConfig);

			return true;
		}

		private void createProxyInstanceRecursive(TreeNode node)
		{
			//Get the node's type ID and creaty plugin instance, skip controllers though.
			//Also, if we see an ActorContainer, it's not a plugin an needs a seperate
			//create proxy call
			SceneNode sceneNode = (SceneNode)m_sceneExplorer.Hashtable[node.Name];
			if (sceneNode != null)
			{
				if (sceneNode as XVPDesigner.Objects.ActorContainer != null)
				{
					XVPDesigner.Objects.ActorContainer ac = sceneNode as XVPDesigner.Objects.ActorContainer;
					ac.createProxyInstance(XVPMHandle);
				}
				else
				{
					if (node.ImageKey != "Controller")
						Program.g_mainForm.PluginMgr.createProxyInstance(sceneNode.PluginID,m_sceneExplorer,node);
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				createProxyInstanceRecursive(childNode);
		}

		public override bool export()
		{
			FileStream fs = null;
			BinaryWriter bw = null;
			
			try
			{
				//Make sure there is something to export
				if (m_sceneExplorer.Hashtable.Count == 0)
				{
					MessageBox.Show("The stage is empty.  There is nothing to export.","Empty Stage",MessageBoxButtons.OK,MessageBoxIcon.Error);
					return false;
				}

				/*SaveFileDialog exportFileDialog = new SaveFileDialog();
				exportFileDialog.Title = "Export Stage...";
				exportFileDialog.Filter = "Stage Files (*.wad)|*.wad|All files (*.*)|*.*";
				exportFileDialog.FileName = "Stage.wad";
				exportFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ActorLibraryPath;
				DialogResult result = exportFileDialog.ShowDialog();
				if (result != DialogResult.OK)
					return false;*/
				FolderBrowserDialog dialog = new FolderBrowserDialog();
				string wadPath = "";
				string smPath = "";
				if (dialog.ShowDialog() != DialogResult.OK)
					return false;

				wadPath = dialog.SelectedPath + "\\" +Path.GetFileNameWithoutExtension(m_currentStagePath) + ".wad";
				smPath = dialog.SelectedPath + "\\" + "stage.manifest";
				
				//Collect the actor instances. 
				m_filesForWad.Clear();

				List<TreeNode> actorNodes = new List<TreeNode>();
				foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
					findActorContainersRecursive(node,actorNodes);

				List<string> unexportedActors = new List<string>();
				foreach (TreeNode node in actorNodes)
				{
					//Check to see if the actor has been exported after it's last save.  If not, add the
					//actor name to the string.  We're going to build a list of actors that
					//should be exported before the stage is exported
					string actorFilename = (node.Tag as SceneExplorer.ActorInfo).m_path;
					string actorWadFilename = actorFilename + "_obj\\" + Path.GetFileNameWithoutExtension(actorFilename) + ".wad";
					if (!File.Exists(actorWadFilename) ||
					(File.GetLastWriteTime(actorFilename) > File.GetLastWriteTime(actorWadFilename)))
					{
						//Only add actor wad once
						if (!unexportedActors.Contains(actorFilename))
							unexportedActors.Add(actorFilename);
					}

					//Only add wad once
					if (!m_filesForWad.Contains(actorWadFilename))
						m_filesForWad.Add(actorWadFilename);
				}

				//If there are out of date actors then export them
				if (unexportedActors.Count != 0)
				{
					DialogResult dr = MessageBox.Show("Some actors are out of date and need to be exported before the stage can be exported.  Press \"Ok\" to continue the export.  Press \"Cancel\" to abort the export.","Out of Date Actors",MessageBoxButtons.OKCancel,MessageBoxIcon.Question);
					if (dr == DialogResult.Cancel)
						return false;

					for (int i = 0;i < unexportedActors.Count;++i)
					{
						ActorStudioDocument actorDoc = new ActorStudioDocument();

						//We need to open the actor and export it.  Passing null
						//for the tab item allows us to work with the actor without
						//creating a GUI
						if (!actorDoc.open(null,unexportedActors[i]))
						{
							string errorMsg = "Could not open actor \"" + unexportedActors[i] + "\".  Open this actor manually to determine errors.  This stage cannot be exported until the actor has be fixed.";
							Program.g_mainForm.logMessage(errorMsg,Color.Red);
							MessageBox.Show(errorMsg,"Actor Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return false;
						}
						if (!actorDoc.export())
						{
							string errorMsg = "Could not export actor \"" + unexportedActors[i] + "\".  Open this actor and export manually to determine errors.  This stage cannot be exported until the actor has be fixed.";
							Program.g_mainForm.logMessage(errorMsg,Color.Red);
							MessageBox.Show(errorMsg,"Actor Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return false;
						}
						actorDoc.close();
					}
				}


				fs = File.Open(smPath,FileMode.Create);
				bw = new BinaryWriter(fs);

				//Write out num actors, then all actor information
				bw.Write(actorNodes.Count);
				Program.g_mainForm.logMessage("\nExporting stage \"" + m_currentStagePath + "\" with " + actorNodes.Count + " actors...",Color.Black);
				for (int i = 0;i < actorNodes.Count;++i)
				{
					XVPDesigner.Objects.ActorContainer actorContainer = m_sceneExplorer.getNode(actorNodes[i].Name) as XVPDesigner.Objects.ActorContainer;

					//Plugin root node should be directly below the actor container node
					Utility.writeCString(bw,actorNodes[i].Text);
					Utility.writeCString(bw,Path.GetFileNameWithoutExtension((actorNodes[i].Tag as SceneExplorer.ActorInfo).m_path) + ".wad");
					unsafe
					{
						float* f = actorContainer.getTransform();
						for (int ctr = 0;ctr < 16;++ctr)
							bw.Write(f[ctr]);
					}
					Program.g_mainForm.logMessage("Actor -> " + actorNodes[i].Text,Color.Black);
				}
				bw.Close();
				fs.Close();

				//Before creating the stage wad, delete previous wad if it exists
				if (File.Exists(wadPath))
					File.Delete(wadPath);
				
				//Add the stage manifest and create wad
				m_filesForWad.Add(smPath);
				addFilesToWad(wadPath,"-a",m_filesForWad);
				m_filesForWad.Clear();

				dialog.Dispose();

				Program.g_mainForm.logMessage("Finished exporting stage.",Color.Black);

			}
			catch (Exception exception)
			{
				MessageBox.Show("Stage export failed, see Message Log for details.","Export Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (bw != null)
					bw.Close();
				if (fs != null)
					fs.Close();
				return false;
			}

			if (bw != null)
				bw.Close();
			if (fs != null)
				fs.Close();

			return true;
		}

		private void addFilesToWad(string wadFilename,string options,List<string> filesToAdd)
		{
			Process xwadProcess = new Process();
			xwadProcess.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
			xwadProcess.StartInfo.UseShellExecute = false;
			xwadProcess.StartInfo.RedirectStandardOutput = true;
			xwadProcess.StartInfo.CreateNoWindow = true;

			//Build file list.  Enclose each path in quotes to handle whitespace properly.
			string files = "";
			foreach (string s in filesToAdd)
				files += "\"" + s + "\" ";

			xwadProcess.StartInfo.Arguments = options + " \"" + wadFilename + "\" " + files;
			xwadProcess.Start();
			m_streamReader = xwadProcess.StandardOutput;
			while (!xwadProcess.HasExited || !m_streamReader.EndOfStream)
			{
				string s = m_streamReader.ReadLine();
				if (s != null)
				{
					if (s.Contains("Adding") || s.Contains("Compressing"))
					{
						Program.g_mainForm.logMessage(s,Color.Green);
					}
					else
						Program.g_mainForm.logMessage(s,Color.Black);
				}
			}
			Application.DoEvents();

			xwadProcess.Dispose();
		}

		override public void addActor(string actorPath,string actorName,uint viewportIndex,int screenX,int screenY)
		{
			IntPtr buffer = Marshal.AllocHGlobal(sizeof(float)*16);
			float[] transform = new float[16];

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.getPlacementTransform(XVPMHandle,buffer,viewportIndex,screenX,screenY);
			}
			Marshal.Copy(buffer,transform,0,16);
			m_sceneExplorer.addActor(actorPath,actorName,null,transform);

			Marshal.FreeHGlobal(buffer);
		}

		public void findActorContainersRecursive(TreeNode node,List<TreeNode> actorNodes)
		{
			if (m_sceneExplorer.getNode(node.Name) as XVPDesigner.Objects.ActorContainer != null)
				actorNodes.Add(node);

			foreach (TreeNode childNode in node.Nodes)
				findActorContainersRecursive(childNode,actorNodes);
		}

		public void CheckForUpdatedActorFiles(List<TreeNode> actorNodes)
		{
			//For each actor node, check to see if the actor source file is
			//newer than when this actor node was created.  If so, we want to re-create
			//the actor node to grab the new changes in the source actor
			foreach (TreeNode node in actorNodes)
			{
				SceneExplorer.ActorInfo ai = node.Tag as SceneExplorer.ActorInfo;
				if (File.Exists(ai.m_path) &&
					(File.GetLastWriteTime(ai.m_path) > ai.m_creationTimestamp))
				{
					//Add the new node then delete the old one in that order...keep the name of the
					//actor node and the transform
					Program.g_mainForm.logMessage("Updating actor instance " + node.Text + " with source actor " + ai.m_path,Color.Black);
					XVPDesigner.Objects.ActorContainer oldActorContainer = m_sceneExplorer.getNode(node.Name) as XVPDesigner.Objects.ActorContainer;
					m_sceneExplorer.addActor(ai.m_path,node.Text,node.Parent,oldActorContainer.Transform);
					
					m_sceneExplorer.handleTreeDelete(node,false);

				}

			}
		}

		override public void setActiveViewportColor(Color color)
		{
			m_XVPMControl.ActiveViewportColor = color;
		}

		public void bakeLightmaps(uint maxTexSize,uint quality,float blurSize)
		{
			save();

			//Create the light map path
			string stageLightmapPath = m_currentStagePath + "_obj\\Lightmaps";

			try
			{
				//Make sure temp directory exists
				if (!Directory.Exists(stageLightmapPath))
					Directory.CreateDirectory(stageLightmapPath);

				//Empty directory
				foreach (string f in Directory.GetFiles(stageLightmapPath,"*"))
					File.Delete(f);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Light map bake failed.  See message log for details","Bake Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return;
			}

			//Spin through the stage looking for lightmappable objects
			List<IntPtr> models = new List<IntPtr>();
			List<TreeNode> modelInstances = new List<TreeNode>();
			List<IntPtr> lights = new List<IntPtr>();
			List<string> LMFilenames = new List<string>();
			List<string> AOFilenames = new List<string>();
			foreach (TreeNode node in m_sceneExplorer.getHierarchyTreeView().Nodes)
				generateLightmapsRecursive(stageLightmapPath,node,modelInstances,models,lights,LMFilenames,AOFilenames);

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.generateLightmap(models.ToArray(),lights.ToArray(),LMFilenames.ToArray(),maxTexSize,quality,0,blurSize);
				//if (chkAOMaps.Checked)
					//XVPM.generateAOMaps(models.ToArray(),AOFilenames.ToArray(),uint.Parse(cbAOMaxTexSize.SelectedItem.ToString()),uint.Parse(txtAOQuality.ControlText),0,float.Parse(txtAOBlurSize.ControlText));
			}

			//Now apply all the lightmaps to the materials they belong to
			foreach (TreeNode modelInstance in modelInstances)
			{
				SceneNode sceneNode = m_sceneExplorer.getNode(modelInstance.Name);
				if (sceneNode != null)
				{
					//Find the lightmap path that matches the node name
					foreach (string s in LMFilenames)
					{
						if (s.Contains(modelInstance.Name))
						{
							if (!Program.g_mainForm.PluginMgr.setLightmap(sceneNode.PluginID,m_sceneExplorer,modelInstance,s))
								throw new Exception("Could not set lightmap " + s + " to model " + modelInstance.Text);
						}
					}
				}
			}

			models.Clear();
			modelInstances.Clear();
			lights.Clear();
			LMFilenames.Clear();
			AOFilenames.Clear();
		}

		public void generateLightmapsRecursive(string stageLightmapPath,TreeNode node,List<TreeNode> modelInstances,List<IntPtr> models,List<IntPtr> lights,List<string> LMFilenames,List<string> AOFilenames)
		{
			SceneNode sceneNode = (SceneNode)m_sceneExplorer.Hashtable[node.Name];
			if (sceneNode != null)
			{
				if (sceneNode.PluginID == "Model")
				{
					modelInstances.Add(node);
					models.Add(sceneNode.Proxy);
					LMFilenames.Add(stageLightmapPath + "\\" + node.Name + ".LM.bmp");
					AOFilenames.Add(stageLightmapPath + "\\" + node.Name + ".AO.bmp");
				}
				else if (sceneNode.PluginID == "Light")
				{
					lights.Add(sceneNode.Proxy);
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				generateLightmapsRecursive(stageLightmapPath,childNode,modelInstances,models,lights,LMFilenames,AOFilenames);
		}
	}
}
