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
using XVPDesigner.Managers;

namespace XVPDesigner.Documents
{
	public class ActorStudioDocument : XVPMDocument
	{
		private SceneExplorer m_actorSceneExplorer;
		private PropertyExplorer m_actorPropertyExplorer;
		private XVPMControl m_XVPMControl;
		public XVPMControl XVPMControl
		{
			get { return m_XVPMControl; }
		}
		private ViewportConfig m_viewportConfig;
		public ViewportConfig ViewportConfig
		{
			get { return m_viewportConfig; }
		}

		private DevComponents.DotNetBar.TabItem m_tabItem;

		StreamReader m_streamReader;

		//List used during export
		List<string> texturesForWad = new List<string>();
		List<string> materialsForWad = new List<string>();

		private string m_currentActorPath = null;
		public string ActorFilename
		{
			get { return m_currentActorPath; }
		}

		public bool Dirty
		{
			get { return (m_actorSceneExplorer.Dirty || m_actorPropertyExplorer.Dirty); }
		}

		public uint XVPMHandle
		{
			get 
			{
				if (m_XVPMControl != null)
					return m_XVPMControl.XVPMHandle;
				else
					return 0;
			}
		}

		public SceneExplorer SceneExplorer
		{
			get { return m_actorSceneExplorer; }
		}
		
		public PropertyExplorer PropertyExplorer
		{
			get { return m_actorPropertyExplorer; }
		}

		override public void setActive(bool active)
		{
			if (m_XVPMControl != null)
				m_XVPMControl.activate(active);
			
			m_active = active;
		}

		public void Dispose()
		{
			if (m_actorSceneExplorer != null)
			{
				m_actorSceneExplorer.Dispose();
				m_actorSceneExplorer = null;
			}
			if (m_actorPropertyExplorer != null)
			{
				m_actorPropertyExplorer.Dispose();
				m_actorPropertyExplorer = null;
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

			//Open an XVP control instance and name it after the actor.  If tab
			//is null then we don't want to create an XVPMControl.  Create the XVPMControl
			//as soon as possible as other objects need a handle to it
			if (tab != null)
			{
				m_XVPMControl = new XVPMControl(this);
				m_XVPMControl.Hide();
				m_XVPMControl.Dock = DockStyle.Fill;

				//Apply viewport config
				m_XVPMControl.applyViewportConfig(m_viewportConfig);
			}
			else
				m_XVPMControl = null;

			m_actorSceneExplorer = new SceneExplorer();
			m_actorSceneExplorer.Dock = DockStyle.Fill;
			m_actorSceneExplorer.init(XVPMHandle,Program.g_mainForm.getShaderMgr().getShaderNames());
			m_actorPropertyExplorer = new PropertyExplorer();
			m_actorPropertyExplorer.Dock = DockStyle.Fill;
			m_actorSceneExplorer.Dirty = false;
			m_actorPropertyExplorer.Dirty = false;

			m_actorSceneExplorer.PropertyExplorer = m_actorPropertyExplorer;

			m_name = name;
			m_currentActorPath = null;

			
			
				
			//Add XVPM control to the tab.  If tab is null, then we
			//are creating this document without GUI support
			m_tabItem = tab;
			if (tab != null)
				tab.AttachedControl.Controls.Add(m_XVPMControl);
	
			return true;
		}

		public override bool open(TabItem tab,string filename)
		{
			Dispose();

			//Open an XVP control instance and name it after the actor.  If
			//the tab is null, then we aren't supporting a GUI.  XVPM control
			//should be the first object created during open()
			if (tab != null)
			{
				m_XVPMControl = new XVPMControl(this);
				m_XVPMControl.Dock = DockStyle.Fill;
			}
			else
				m_XVPMControl = null;

			//Add XVPM control to the tab if we are supporting a GUI
			m_tabItem = tab;
			if (tab != null)
				tab.AttachedControl.Controls.Add(m_XVPMControl);

			m_actorSceneExplorer = new SceneExplorer();
			m_actorSceneExplorer.Dock = DockStyle.Fill;
			m_actorPropertyExplorer = new PropertyExplorer();
			m_actorPropertyExplorer.Dock = DockStyle.Fill;
			m_actorSceneExplorer.init(XVPMHandle,Program.g_mainForm.getShaderMgr().getShaderNames());
			m_actorSceneExplorer.PropertyExplorer = m_actorPropertyExplorer;

			FileStream fs = null;

			//We deserialize in this order.  Then we must call fixup
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				m_viewportConfig = (ViewportConfig)bf.Deserialize(fs);
				m_actorSceneExplorer.reset();
				m_actorSceneExplorer.setNodeArray((ArrayList)bf.Deserialize(fs));
				m_actorSceneExplorer.Hashtable = (Hashtable)bf.Deserialize(fs);
				m_actorSceneExplorer.postDeserializeFixup();
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not open actor.  Check the message log for details.","Open Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (fs != null)
					fs.Close();
				return false;
			}

			if (fs != null)
				fs.Close();

			m_currentActorPath = filename;
			m_actorSceneExplorer.Dirty = false;
			m_actorPropertyExplorer.Dirty = false;
			m_viewportConfig.Dirty = false;

			Program.g_mainForm.addRecentActor(filename);
			//SceneDockWindow.Text = Path.GetFileName(m_currentActorPath);

			return true;
		}

		public override bool save()
		{
			FileStream fs = null;

			if (m_currentActorPath == null)
			{
				SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
				dlgSaveFileDialog.Title = "Save Actor...";
				dlgSaveFileDialog.Filter = "Actor Files (*.act)|*.act|All files (*.*)|*.*";
				dlgSaveFileDialog.FileName = "Actor.act";
				dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ActorLibraryPath;
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result != DialogResult.OK)
					return false;

				//Check to make sure the document is not already open under that name 
				if (Program.g_mainForm.m_docMgr.isDocumentOpen(dlgSaveFileDialog.FileName))
				{
					MessageBox.Show("The document " + dlgSaveFileDialog.FileName + " is currently open.  This document cannot be saved using the same filename.","Document Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
					return false;
				}

				m_currentActorPath = dlgSaveFileDialog.FileName;
				if (m_tabItem != null)
				{
					m_tabItem.Name = dlgSaveFileDialog.FileName;
					m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				}
				
				dlgSaveFileDialog.Dispose();
			}
			

			try
			{
				fs = File.Open(m_currentActorPath,FileMode.Create);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + m_currentActorPath + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return false;
			}

			BinaryFormatter bf = new BinaryFormatter();
			bf.Serialize(fs,m_viewportConfig);
			bf.Serialize(fs,m_actorSceneExplorer.getNodeArray());
			bf.Serialize(fs,m_actorSceneExplorer.Hashtable);

			fs.Close();

			m_actorPropertyExplorer.Dirty = false;
			m_actorSceneExplorer.Dirty = false;
			m_viewportConfig.Dirty = false;

			Program.g_mainForm.addRecentActor(m_currentActorPath);

			return true;
		}

		public override bool saveAs()
		{
			SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
			dlgSaveFileDialog.Title = "Save Actor As...";
			dlgSaveFileDialog.Filter = "Actor Files (*.act)|*.act|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Actor.act";
			dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ActorLibraryPath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();

			//Check to make sure the document is not already open under that name 
			if (Program.g_mainForm.m_docMgr.isDocumentOpen(dlgSaveFileDialog.FileName))
			{
				MessageBox.Show("The document " + dlgSaveFileDialog.FileName + " is currently open.  This document cannot be saved using the same filename.","Document Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return false;
			}

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
				BinaryFormatter bf = new BinaryFormatter();
				bf.Serialize(fs,m_viewportConfig);
				bf.Serialize(fs,m_actorSceneExplorer.getNodeArray());
				bf.Serialize(fs,m_actorSceneExplorer.Hashtable);

				fs.Close();
				m_actorPropertyExplorer.Dirty = false;
				m_actorSceneExplorer.Dirty = false;
				m_currentActorPath = dlgSaveFileDialog.FileName;
				if (m_tabItem != null)
				{
					m_tabItem.Name = dlgSaveFileDialog.FileName;
					m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				}
				dlgSaveFileDialog.Dispose();

				Program.g_mainForm.addRecentActor(m_currentActorPath);

				return true;
			}

			return false;
		}

		public override bool close()
		{
			//If we we are GUI-less, just dispose and return
			if (m_tabItem == null)
			{
				Dispose();
				return true;
			}

			if (m_actorSceneExplorer.Dirty || m_actorPropertyExplorer.Dirty || m_viewportConfig.Dirty)
			{
				//Show document
				m_tabItem.Parent.SelectedTab = m_tabItem;

				bool success = true;
				DialogResult dr = MessageBox.Show(m_tabItem.Text + " has been modified.  Would you like to save?","Actor Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
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
			foreach (TreeNode controllerGroupNode in m_actorSceneExplorer.getControllerTreeView().Nodes)
			{
				//Find the matching hierarchy node
				TreeNode[] matchingHierarchyNode = m_actorSceneExplorer.getHierarchyTreeView().Nodes.Find(controllerGroupNode.Name,true);
				if (matchingHierarchyNode.Length != 1)
					throw new Exception("Expected to find exactly one hierarchy node.");

				foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
				{
					TreeNode[] controllerNodes = matchingHierarchyNode[0].Nodes.Find(controllerNode.Name,true);
					for (int i = 0; i < controllerNodes.Length; ++i)
					{
						//Grab the root hierarchy node so we can send the detach notification to the particular plugin
						//that has the controller attached
						SceneNode sceneNode = m_actorSceneExplorer.Hashtable[controllerGroupNode.Name] as SceneNode;
						Controller controller = m_actorSceneExplorer.Hashtable[controllerNodes[i].Name] as Controller;
						sceneNode.detachController(controller.Proxy,controllerNodes[i].Parent);
					}
				}
			}

			//Then destroy controller proxy instances for all plugins
			foreach (TreeNode node in m_actorSceneExplorer.getControllerTreeView().Nodes)
			{
				foreach (TreeNode controllerNode in node.Nodes)
				{
					//Get the node's type ID
					SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[controllerNode.Name];
					Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_actorSceneExplorer,controllerNode);
				}
			}

			//Now, destroy all other proxy instances for plugins
			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
			{
				destroyProxyInstancesRecursive(node);
				//Get the node's type ID
				/*SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[node.Name];
				if (sceneNode == null)
					continue;

				Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_actorSceneExplorer,node);*/
			}

			Dispose();
			m_currentActorPath = null;

			base.close();
			return true;
		}

		private void destroyProxyInstancesRecursive(TreeNode node)
		{
			//Skip controllers since they've already been destroyed
			if (node.ImageKey != "Controller")
			{
				//Get the node's type ID
				SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[node.Name];

				//If the scene node is an actor container, don't call the plugin destroy
				if (sceneNode != null)
				{
					if (sceneNode as Objects.ActorContainer != null)
					{
						Objects.ActorContainer ac = sceneNode as Objects.ActorContainer;
						ac.destroyProxyInstance();
					}
					else
						Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,m_actorSceneExplorer,node);
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				destroyProxyInstancesRecursive(childNode);
		}

		public override bool postLoad()
		{
			//Add proxy instances
			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
			{
				createProxyInstanceRecursive(node);
			}

			//Then create controller proxy instances for all controllers
			foreach (TreeNode node in m_actorSceneExplorer.getControllerTreeView().Nodes)
			{
				foreach (TreeNode controllerNode in node.Nodes)
				{
					Controller controller = m_actorSceneExplorer.Hashtable[controllerNode.Name] as Controller;
					Program.g_mainForm.PluginMgr.createProxyInstance(controller.PluginID,m_actorSceneExplorer,controllerNode);
				}
			}

			//Attach controllers to the necessary nodes
			foreach (TreeNode controllerGroupNode in m_actorSceneExplorer.getControllerTreeView().Nodes)
			{
				//Find the matching hierarchy node
				TreeNode[] matchingHierarchyNode = m_actorSceneExplorer.getHierarchyTreeView().Nodes.Find(controllerGroupNode.Name,true);
				if (matchingHierarchyNode.Length != 1)
					throw new Exception("Expected to find exactly one hierarchy node.");

				foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
				{
					TreeNode[] controllerNodes = matchingHierarchyNode[0].Nodes.Find(controllerNode.Name,true);
					for (int i = 0; i < controllerNodes.Length; ++i)
					{
						//Grab the root hierarchy node so we can send the attach notification to the particular plugin
						//that has the controller attached
						SceneNode sceneNode = m_actorSceneExplorer.Hashtable[controllerGroupNode.Name] as SceneNode;
						Controller controller = m_actorSceneExplorer.Hashtable[controllerNodes[i].Name] as Controller;
						sceneNode.attachController(controller.Proxy,controllerNodes[i].Parent);
					}
				}
			}

			//Apply the viewport config
			m_XVPMControl.applyViewportConfig(m_viewportConfig);

			return true;
		}

		private void createProxyInstanceRecursive(TreeNode node)
		{
			//Get the node's type ID and creaty plugin instance, skip controllers though.  Once
			//we find a plugin instance root, we don't recurse any further.
			SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[node.Name];
			if (sceneNode != null && node.ImageKey != "Controller")
			{
				Program.g_mainForm.PluginMgr.createProxyInstance(sceneNode.PluginID,m_actorSceneExplorer,node);
				return;
			}

			foreach (TreeNode childNode in node.Nodes)
				createProxyInstanceRecursive(childNode);
		}

		public override bool export()
		{
			//Make sure there is something to export
			if (m_actorSceneExplorer.Hashtable.Count == 0)
			{
				MessageBox.Show("There are no actor nodes to export.","Empty Actor",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return false;
			}

			//Create the wad filename 
			string actorWadFilename = m_currentActorPath + "_obj\\" + Path.GetFileNameWithoutExtension(m_currentActorPath) + ".wad";
			string actorWadPath = m_currentActorPath + "_obj\\";

			try
			{
				//Make sure temp directory exists
				if (!Directory.Exists(actorWadPath))
					Directory.CreateDirectory(actorWadPath);

				//Empty directory
				foreach (string f in Directory.GetFiles(actorWadPath,"*"))
					File.Delete(f);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Actor export failed.  See message log for details","Export Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return false;
			}

			//Find scripts and convert....
			startScriptConversion();

			//Find audio and convert....
			startAudioConversion();

			//Find materials and convert...
			startMaterialConversion(actorWadFilename);

			//Create the actor manifest...
			BinaryWriter bwActorManifest = null;
			FileStream fsActorManifest = null;
			try
			{
				fsActorManifest = File.Open(actorWadPath + "Actor.manifest",FileMode.Create);
				bwActorManifest = new BinaryWriter(fsActorManifest);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Could not open a new file for Actor.manifest.  See the message log for details.","Actor.manifest",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.Message,Color.Red);
				if (fsActorManifest != null)
					fsActorManifest.Close();
				return false;
			}

			//Write out the number of entries to exist in the actor manifest
			int controllerCount = 0;
			foreach (TreeNode node in m_actorSceneExplorer.getControllerTreeView().Nodes)
				controllerCount += node.Nodes.Count;
			bwActorManifest.Write(m_actorSceneExplorer.getHierarchyTreeView().Nodes.Count + controllerCount);

			//Export each plugin instance...
			TextWriter swInstanceManifest = null;
			List<string> filesToAdd = new List<string>();
			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
			{
				//Get the node's type ID
				SceneNode sceneNode = (SceneNode)m_actorSceneExplorer.Hashtable[node.Name];
				if (sceneNode == null)
					continue;

				ExportResults results;
				results = Program.g_mainForm.PluginMgr.exportInstance(sceneNode.PluginID,actorWadPath,m_actorSceneExplorer,node);
				if (results == null)
				{
					MessageBox.Show("The node \"" + node.Text + "\" failed to export.  See the message log for details.","Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					if (fsActorManifest != null)
						fsActorManifest.Close();
					return false;
				}

				//Create instance manifest if there are manifest lines in the results
				if (results.manifestLines.Count != 0)
				{
					try
					{
						swInstanceManifest = new StreamWriter(actorWadPath + node.Name + ".manifest",false);
					}
					catch (Exception exception)
					{
						MessageBox.Show("Could not open a new file for the instance manifest.  See the message log for details.","Actor.manifest",MessageBoxButtons.OK,MessageBoxIcon.Error);
						Program.g_mainForm.logMessage(exception.Message,Color.Red);
						if (fsActorManifest != null)
							fsActorManifest.Close();
						return false;
					}
				}

				//Stick each data file in the wad.  If the path is rooted, then add the
				//file using the absolute path, otherwise assume the file is located in 
				//the actor wad path
				foreach (string s in results.filenames)
				{
					if (Path.IsPathRooted(s))
						filesToAdd.Add(s);
					else
						filesToAdd.Add(actorWadPath + s);
				}

				if (swInstanceManifest != null)
				{
					for (int i = 0; i < results.manifestLines.Count; ++i)
					{
						//Verify the correct name=value format of each line
						/*if (results.manifestLines[i].Split('=').Length != 2)
						{
							logMessage("The following line in the plugin manifest is not the correct name=value format.  The line will not be added to the manifest.  " + results.manifestLines[i],Color.Red);
							continue;
						}*/

						//Stick each line into the manifest
						swInstanceManifest.WriteLine(results.manifestLines[i]);
					}

					//Close instance manifest and add to wad
					swInstanceManifest.Close();
					filesToAdd.Add(actorWadPath + node.Name + ".manifest");
				}

				//Add an entry to the actor manifest
				Utility.writeCString(bwActorManifest,node.Text);
				Utility.writeCString(bwActorManifest,sceneNode.PluginID);
				Utility.writeCString(bwActorManifest,node.Name + ".manifest");

				//Determine if any controllers are attached to this plugin instance.
				//If so, export each controller.
				TreeNode[] controllerGroupNode = m_actorSceneExplorer.getControllerTreeView().Nodes.Find(node.Name,false);
				if (controllerGroupNode.Length == 1)
				{
					//Export each controller instance under the controller group node
					foreach (TreeNode cNode in controllerGroupNode[0].Nodes)
					{
						//Get the controllers's type ID
						SceneNode controllerNode = (SceneNode)m_actorSceneExplorer.Hashtable[cNode.Name];
						if (controllerNode == null)
							continue;

						ExportResults cResults;
						cResults = Program.g_mainForm.PluginMgr.exportInstance(controllerNode.PluginID,actorWadPath,m_actorSceneExplorer,cNode);
						if (cResults == null)
							continue;

						//Now we need to determine the controller attachements
						string attachmentString = "";
						TreeNode[] attachments = node.Nodes.Find(cNode.Name,true);
						foreach (TreeNode attachmentNode in attachments)
							if (attachmentNode.Parent.Parent == null)
								attachmentString += "NULL:";
							else
								attachmentString += attachmentNode.Parent.Text + ":";
						//attachmentString = node.Name + ":" + attachmentString;
						cResults.manifestLines.Add("_C0");
						cResults.manifestLines.Add(attachmentString);


						//Create instance manifest if there are manifest lines in the results
						if (cResults.manifestLines.Count != 0)
						{
							try
							{
								swInstanceManifest = new StreamWriter(actorWadPath + cNode.Name + ".manifest",false);
							}
							catch (Exception exception)
							{
								MessageBox.Show("Could not open a new file for the instance manifest.  See the message log for details.","Actor.manifest",MessageBoxButtons.OK,MessageBoxIcon.Error);
								Program.g_mainForm.logMessage(exception.Message,Color.Red);
								return false;
							}
						}

						//Stick each data file in the wad.  If the path is rooted, then add the
						//file using the absolute path, otherwise assume the file is located in 
						//the actor wad path
						foreach (string s in cResults.filenames)
						{
							if (Path.IsPathRooted(s))
								filesToAdd.Add(s);
							else
								filesToAdd.Add(actorWadPath + s);
						}

						if (swInstanceManifest != null)
						{
							for (int i = 0; i < cResults.manifestLines.Count; ++i)
							{
								//Verify the correct name=value format of each line
								/*if (cResults.manifestLines[i].Split('=').Length != 2)
								{
									logMessage("The following line in the plugin manifest is not the correct name=value format.  The line will not be added to the manifest.  " + cResults.manifestLines[i],Color.Red);
									continue;
								}*/

								//Stick each line into the manifest
								swInstanceManifest.WriteLine(cResults.manifestLines[i]);
							}

							//Close instance manifest and add to wad
							swInstanceManifest.Close();
							filesToAdd.Add(actorWadPath + cNode.Name + ".manifest");
						}

						//Add an entry to the actor manifest
						Utility.writeCString(bwActorManifest,cNode.Text);
						Utility.writeCString(bwActorManifest,controllerNode.PluginID);
						Utility.writeCString(bwActorManifest,cNode.Name + ".manifest");
					}
				}
			}


			//Close actor manifest and add it to the wad 
			bwActorManifest.Close();
			fsActorManifest.Close();
			filesToAdd.Add(actorWadPath + "Actor.manifest");

			//Check on final time to make sure all the necessary files have been generated
			bool success = true;
			foreach (string s in filesToAdd)
				if (!File.Exists(s))
				{
					Program.g_mainForm.logMessage("Cannot find " + s,Color.Red);
					success = false;
				}
			if (!success)
			{
				MessageBox.Show("Couldn't find one or more files needed for export.  Export failed.  See the message log for details.","Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return false;
			}

			addFilesToWad(actorWadFilename,"-ac",filesToAdd);
			Program.g_mainForm.logMessage("Finished building actor wad.",Color.Black);

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

		private void startAudioConversion()
		{
			Program.g_mainForm.logMessage("Converting audio...",Color.Black);
			Process p = new Process();
			p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\oggenc2.exe";
			p.StartInfo.UseShellExecute = false;
			p.StartInfo.RedirectStandardOutput = true;
			p.StartInfo.CreateNoWindow = true;

			/*Process xwadProcess = new Process();
			xwadProcess.StartInfo.FileName = "tools\\xvpwad.exe";
			xwadProcess.StartInfo.UseShellExecute = false;
			xwadProcess.StartInfo.RedirectStandardOutput = true;
			xwadProcess.StartInfo.CreateNoWindow = true;*/


			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
				convertAudioRecursive(node,p);

			p.Dispose();
		}

		private void convertAudioRecursive(TreeNode treeNode,Process p)
		{
			convertAudio(treeNode,p);

			foreach (TreeNode node in treeNode.Nodes)
				convertAudioRecursive(node,p);
		}

		private void convertAudio(TreeNode node,Process p)
		{
			AudioClip audioClip = m_actorSceneExplorer.Hashtable[node.Name] as AudioClip;
			if (audioClip != null)
			{
				if (audioClip.Filename != null)
				{
					string temp = null;
					string dir = null;

					//Make sure the file exists
					if (!File.Exists(audioClip.Filename))
					{
						Program.g_mainForm.logMessage("\"" + audioClip.Filename + "\" was not found.",Color.Red);
						return;
					}

					temp = Program.g_mainForm.CurrentWorkspace.buildCachedAudioPath(audioClip.Filename);
					try
					{
						dir = Path.GetDirectoryName(temp);

						if (!Directory.Exists(dir))
							Directory.CreateDirectory(dir);
					}
					catch (PathTooLongException)
					{
						Program.g_mainForm.logMessage("Path too long. \"" + dir + "\".  Make sure the file is in the library to keep paths within limits or move the library higher in the directory structure.  Full paths cannot exceed 260 characters.  File not processed.",Color.Red);
						return;
					}

					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(audioClip.Filename) + ".xau";

					//Check to see if the file even needs to be converted again, it may be up to date
					if (File.GetLastWriteTime(audioClip.Filename) > File.GetLastWriteTime(convertedPath))
					{
						//Is the file located outside the library root directory?
						if (!audioClip.Filename.Contains(Program.g_mainForm.CurrentWorkspace.AudioLibraryPath))
							Program.g_mainForm.logMessage("\"" + audioClip.Filename + "\" is located outside the current audio library.  This is ok, but it's recommended that all audio reside below the audio root directory.",Color.Blue);

						//If the file is already an ogg file just do a straight copy
						if (audioClip.Filename.EndsWith(".ogg"))
						{
							File.Delete(convertedPath);
							File.Copy(audioClip.Filename,convertedPath);
						}
						else
						{
							p.StartInfo.Arguments = "\"" + audioClip.Filename + "\" -o \"" + convertedPath + "\"";
							p.Start();
							m_streamReader = p.StandardOutput;
							while (!p.HasExited || !m_streamReader.EndOfStream)
							{
								string s = m_streamReader.ReadLine();
								if (s != null)
								{
									Program.g_mainForm.logMessage(s,Color.Black);
								}
							}
						}
						Application.DoEvents();
					}
					else
					{
						Program.g_mainForm.logMessage(audioClip.Filename + " is up to date",Color.Black);
					}

					//Stick it in the wad...
					List<string> filesToAdd = new List<string>();
					filesToAdd.Add(convertedPath);
					addFilesToWad("testwad.wad","-a",filesToAdd);
				}
			}
		}

		private void startScriptConversion()
		{
			Program.g_mainForm.logMessage("Converting scripts...",Color.Black);
			Process p = new Process();
			p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\luac-5.0.2.exe";
			p.StartInfo.UseShellExecute = false;
			p.StartInfo.RedirectStandardOutput = true;
			p.StartInfo.RedirectStandardError = true;
			p.StartInfo.CreateNoWindow = true;

			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
				convertScriptRecursive(node,p);

			p.Dispose();
		}

		private void convertScriptRecursive(TreeNode treeNode,Process p)
		{
			convertScript(treeNode,p);

			foreach (TreeNode node in treeNode.Nodes)
				convertScriptRecursive(node,p);
		}

		private void convertScript(TreeNode node,Process p)
		{
			LuaScript script = m_actorSceneExplorer.Hashtable[node.Name] as LuaScript;
			if (script != null)
			{
				if (script.Filename != null)
				{
					string temp = null;
					string dir = null;

					//Make sure the file exists
					if (!File.Exists(script.Filename))
					{
						Program.g_mainForm.logMessage("\"" + script.Filename + "\" was not found.",Color.Red);
						return;
					}

					temp = Program.g_mainForm.CurrentWorkspace.buildCachedScriptPath(script.Filename);
					try
					{
						dir = Path.GetDirectoryName(temp);

						if (!Directory.Exists(dir))
							Directory.CreateDirectory(dir);
					}
					catch (PathTooLongException)
					{
						Program.g_mainForm.logMessage("Path too long. \"" + dir + "\".  Make sure the file is in the library to keep paths within limits or move the library higher in the directory structure.  Full paths cannot exceed 260 characters.  File not processed.",Color.Red);
						return;
					}

					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(script.Filename) + ".luo";

					//Check to see if the file even needs to be converted again, it may be up to date
					if (File.GetLastWriteTime(script.Filename) > File.GetLastWriteTime(convertedPath))
					{
						//Is the file located outside the library root directory?
						if (!script.Filename.Contains(Program.g_mainForm.CurrentWorkspace.ScriptLibraryPath))
							Program.g_mainForm.logMessage("\"" + script.Filename + "\" is located outside the current script library.  This is ok, but it's recommended that all scripts reside below the script root directory.",Color.Blue);

						p.StartInfo.Arguments = "-v -o \"" + convertedPath + "\" \"" + script.Filename + "\"";
						p.Start();
						StreamReader errStreamReader;
						StreamReader outStreamReader;
						errStreamReader = p.StandardError;
						outStreamReader = p.StandardOutput;
						while (!p.HasExited || !outStreamReader.EndOfStream || !errStreamReader.EndOfStream)
						{
							string s = outStreamReader.ReadLine();
							if (s != null)
							{
								Program.g_mainForm.logMessage(s,Color.Black);
							}
							s = errStreamReader.ReadLine();
							if (s != null)
							{
								Program.g_mainForm.logMessage(s,Color.Red);
							}

							Application.DoEvents();
						}
					}
					else
					{
						Program.g_mainForm.logMessage(script.Filename + " is up to date",Color.Black);
					}

					//Stick it in the wad...
					List<string> filesToAdd = new List<string>();
					filesToAdd.Add(convertedPath);
					addFilesToWad("testwad.wad","-a",filesToAdd);
				}
			}
		}
		
		private void startMaterialConversion(string wadFile)
		{
			texturesForWad.Clear();
			materialsForWad.Clear();

			Program.g_mainForm.logMessage("Converting textures...",Color.Black);
			Process p = new Process();
			p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\nvdxt.exe";
			p.StartInfo.UseShellExecute = false;
			p.StartInfo.RedirectStandardOutput = true;
			p.StartInfo.CreateNoWindow = true;

			//Export each material in the material tree view
			foreach (TreeNode node in m_actorSceneExplorer.getMaterialTreeView().Nodes)
				convertMaterialRecursive(Path.GetDirectoryName(wadFile),node,p);

			//Export each stand alone texture in the hierarchy view
			foreach (TreeNode node in m_actorSceneExplorer.getHierarchyTreeView().Nodes)
			{
				Texture texture = m_actorSceneExplorer.getNode(node.Name) as Texture;
				if (texture != null)
				{
					if (texture.Filename != null && texture.Filename != "")
						convertTexture(texture.Filename,texture.Encoding,p);
					else
						throw new Exception("No texture filename, not sure how to continue");
				}
			}


			//Add textures to wad file
			Program.g_mainForm.logMessage("Adding textures to actor wad...",Color.Black);
			addFilesToWad(wadFile,"-a",texturesForWad);
			Program.g_mainForm.logMessage("Adding materials to actor wad...",Color.Black);
			addFilesToWad(wadFile,"-ac",materialsForWad);
			texturesForWad.Clear();
			materialsForWad.Clear();

			p.Dispose();
		}


		private void convertMaterialRecursive(string exportDir,TreeNode treeNode,Process p)
		{
			convertMaterial(exportDir,treeNode,p);

			foreach (TreeNode node in treeNode.Nodes)
				convertMaterialRecursive(exportDir,node,p);
		}

		private void convertMaterial(string exportDir,TreeNode node,Process p)
		{
			Material mat = m_actorSceneExplorer.Hashtable[node.Name] as Material;
			if (mat != null)
			{
				Material.TextureNameList texList = mat.getTextureNameList();
				if (texList != null)
				{
					for (int i = 0; i < texList.filenames.Count; ++i)
					{
						//Is this texture slot filled?
						if (texList.filenames[i] == null)
							continue;

						if ((string)texList.filenames[i] != "")
						{
							convertTexture((string)texList.filenames[i],Texture.convertEncoding((string)texList.encoding[i]),p);
						}
						else
						{
							throw new Exception("No texture filename, can't continue");
						}
					}
				}

				//Export material...
				FileStream fs = null;
				try
				{
					fs = File.Open(exportDir + "\\" + node.Name + ".xma",FileMode.Create);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + exportDir + "\\" + node.Name + ".xma" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					return;
				}
				BinaryWriter bw = new BinaryWriter(fs);

				try
				{
					mat.export(bw);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Could not export the material \"" + node.Name + "\" for the following reason.  " + exception.ToString(),"Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					return;
				}
				bw.Close();
				fs.Close();

				//Add the material to the list of files added to wad
				if (!materialsForWad.Contains(exportDir + "\\" + node.Name + ".xma"))
					materialsForWad.Add(exportDir + "\\" + node.Name + ".xma");
			}
		}

		private void convertTexture(string filename,Texture.EncodingType encoding,Process p)
		{
			string temp = null;
			string dir = null;

			//Make sure the file exists
			if (!File.Exists(filename))
			{
				Program.g_mainForm.logMessage("\"" + filename + "\" was not found.",Color.Red);
				return;
			}

			temp = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(filename);
			try
			{
				dir = Path.GetDirectoryName(temp);

				if (!Directory.Exists(dir))
					Directory.CreateDirectory(dir);
			}
			catch (PathTooLongException)
			{
				Program.g_mainForm.logMessage("Path too long. \"" + dir + "\".  Make sure the file is in the library to keep paths within limits or move the library higher in the directory structure.  Full paths cannot exceed 260 characters.  File not processed.",Color.Red);
				return;
			}

			//If we already have this file prepared and ready for wad insertion...we're done
			string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(filename) + ".xif";
			if (texturesForWad.Contains(convertedPath))
				return;

			//Is the file located outside the library root directory?
			if (!filename.Contains(Program.g_mainForm.CurrentWorkspace.TextureLibraryPath))
				Program.g_mainForm.logMessage("\"" + filename + "\" is located outside the current texture library.  This is ok, but it's recommended that all textures reside below the texture root directory.",Color.Blue);

			/*FOURCC Description Alpha premultiplied? Compression ratio Texture Type 
			DXT1 Opaque / 1-bit Alpha N/A 8:1 / 6:1 Simple non-alpha 
			DXT2 Explicit alpha Yes 4:1 Sharp alpha 
			DXT3 Explicit alpha No 4:1 Sharp alpha 
			DXT4 Interpolated alpha Yes 4:1 Gradient alpha 
			DXT5 Interpolated alpha No 4:1 Gradient alpha */

			//Check to see if the file even needs to be converted again, it may be up to date
			if (File.GetLastWriteTime(filename) > File.GetLastWriteTime(convertedPath))
			{
				//We don't convert dds files at this time.  So if this is a dds file
				//copy the texture to the converted path.  Also, if no encoding is specified
				//just copy the texture.
				if (filename.EndsWith(".dds") || encoding == Texture.EncodingType.NONE)
				{
					if (File.Exists(convertedPath))
						File.Delete(convertedPath);
					File.Copy(filename,convertedPath);
					File.SetAttributes(convertedPath,FileAttributes.Normal);
				}
				else
				{

					string encodingOption = "-u8888";
					switch (encoding)
					{
						case Texture.EncodingType.NONE:
							encodingOption = "";
							break;
						case Texture.EncodingType.MIPMAP_ONLY:
							encodingOption = "-u8888";
							break;
						case Texture.EncodingType.RGBa:
							encodingOption = "-dxt1";
							break;
						case Texture.EncodingType.RGBA:
						case Texture.EncodingType.XYZ:
							encodingOption = "-dxt5";
							break;
					}

					p.StartInfo.Arguments = "-file \"" + filename + "\" -outdir \"" + dir + "\" -output \"" + dir + "\\" + Path.GetFileNameWithoutExtension(filename) + ".xif\" " + encodingOption + " -timestamp";
					p.Start();
					m_streamReader = p.StandardOutput;
					while (!p.HasExited || !m_streamReader.EndOfStream)
					{
						string s = m_streamReader.ReadLine();
						if (s != null)
						{
							if (s.Contains("-->") || s.Contains("**"))
							{
								Program.g_mainForm.logMessage(s,Color.Red);
							}
							else if (s.Contains("8.22"))
							{
								//Do nothing
							}
							else
								Program.g_mainForm.logMessage(s,Color.Black);
						}
						Application.DoEvents();
					}
				}
			}
			texturesForWad.Add(convertedPath);
		}

		override public void addActor(string actorPath,string actorName,uint viewportIndex,int screenX,int screenY)
		{
			m_actorSceneExplorer.addActor(actorPath,actorName,null,null);
		}

		override public void setActiveViewportColor(Color color)
		{
			m_XVPMControl.ActiveViewportColor = color;
		}
	}
}
