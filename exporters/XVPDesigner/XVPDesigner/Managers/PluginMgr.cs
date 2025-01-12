using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.CodeDom.Compiler;
using System.CodeDom;
using Microsoft.CSharp;
using System.Reflection;
using DevComponents.DotNetBar;
using XVPDesigner.Objects;
using System.Collections;
using XVPDesigner.UserControls;
using XVPDesigner.Dialogs;
using XVPDesigner.Documents;
using XVPDesigner.Interfaces;

namespace XVPDesigner.Managers
{
	public sealed class SceneNodePluginDescriptor : Attribute { }
	public sealed class ControllerPluginDescriptor : Attribute { }
	public sealed class PostEffectPluginDescriptor : Attribute { }
	
	public class ExportResults
	{
		public List<string> filenames;
		public List<string> manifestLines;

		public ExportResults()
		{
			filenames = new List<string>();
			manifestLines = new List<string>();
		}
	}

	public class PluginMgr
	{
		Hashtable m_loadedPlugins = new Hashtable(500);
		ButtonItem m_sceneNodesButton;
		ButtonItem m_controllersButton;
		ButtonItem m_postEffectsButton;
	
		public bool init(ButtonItem sceneNodesButton,ButtonItem controllersButton,ButtonItem postEffectsButton)
		{
			m_sceneNodesButton = sceneNodesButton;
			m_controllersButton = controllersButton;
			m_postEffectsButton = postEffectsButton;
			return updatePlugins();
		}

		public bool updatePlugins()
		{
			Program.g_mainForm.logMessage("Searching for new or updated plugins...",Color.Black);

			/*
			//Create compiler and add the common assemblies
			CodeDomProvider codeProvider = CodeDomProvider.CreateProvider("C#");
			CompilerParameters cp = new CompilerParameters();
			cp.ReferencedAssemblies.Add("XVPDesigner.exe");
			cp.ReferencedAssemblies.Add("System.dll");
			cp.ReferencedAssemblies.Add("System.Drawing.dll");
			cp.ReferencedAssemblies.Add("System.Windows.Forms.dll");
			cp.ReferencedAssemblies.Add("System.Design.dll");
			cp.ReferencedAssemblies.Add("DevComponents.DotNetBar2.dll");

			//For each file in the directory, if it's newer than the object file, we need
			//to compile it.
			foreach (string filename in Directory.GetFiles(Application.StartupPath + "\\Plugin Source"))
			{
				//Check filename formatting.  
				if (!filename.EndsWith(".cs"))
					continue;
				if (!filename.Contains("."))
					continue;

				string[] tokens = filename.Split('\\');
				string pluginName = tokens[tokens.Length - 1];
				string objFilename = Application.StartupPath + "\\" + Path.GetFileName(filename) + ".dll";


				//If the object file for this source file exists, let's check the time stamp to
				//determine if we need to re-compile it.
				if (!File.Exists(objFilename) ||
					(File.GetLastWriteTime(filename) > File.GetLastWriteTime(objFilename)))
				{
                    Program.g_mainForm.logMessage("Found plugin \"" + pluginName + "\", compiling...", Color.Black);
                    Program.g_mainForm.setSplashStatus("Compiling Plugin: \"" + pluginName + "\"");

					cp.OutputAssembly = objFilename;
					CompilerResults cr = codeProvider.CompileAssemblyFromFile(cp,filename);

					if (cr.Errors.HasErrors)
					{
						for (int i = 0;i < cr.Errors.Count;++i)
							Program.g_mainForm.logMessage(cr.Errors[i].ToString(),Color.Red);
						Program.g_mainForm.logMessage("Compilation failed.",Color.Red);
					}
					else
					{
						Program.g_mainForm.logMessage("Compilation succeeded.",Color.Green);
					}
				}
			}

			 */

			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));

			//Load any plugins included in the main assembly
			findPlugins(Application.ExecutablePath);

			//Now, for each compiled plugin, attemp to load it.
			foreach (string filename in Directory.GetFiles(Application.StartupPath))
			{
				//Make sure it's at least ending with ".plugin.dll" 
				if (!filename.EndsWith(".plugin.dll"))
					continue;
				else
					findPlugins(filename);
			}

			Program.g_mainForm.logMessage("Search complete.",Color.Black);

			return true;
		}

		private void findPlugins(string filename)
		{
			string[] tokens = filename.Split('\\');
			string pluginName = tokens[tokens.Length - 1];

			Program.g_mainForm.logMessage("Found plugin assembly \"" + pluginName + "\", loading...",Color.Black);
			Program.g_mainForm.setSplashStatus("Loading Plugin: \"" + pluginName + "\"");

			Assembly pluginAssembly = Assembly.LoadFile(filename);
			if (pluginAssembly == null)
			{
				Program.g_mainForm.logMessage("Could not load assembly for " + pluginName,Color.Red);
				return;
			}

			foreach (Type type in pluginAssembly.GetTypes())
			{
				ButtonItem createButton = null;
				if (type.IsAbstract) continue;
				if (type.IsDefined(typeof(SceneNodePluginDescriptor),true))
				{
					createButton = m_sceneNodesButton;
				}
				else if (type.IsDefined(typeof(ControllerPluginDescriptor),true))
				{
					createButton = m_controllersButton;
				}
				else if (type.IsDefined(typeof(PostEffectPluginDescriptor),true))
				{
					createButton = m_postEffectsButton;
				}
				else
					continue;

				//Try to create an instance of the plugin descriptor
				BasePlugin pluginInstance;
				try
				{
					pluginInstance = (BasePlugin)pluginAssembly.CreateInstance(type.ToString());
				}
				catch (Exception)
				{
					Program.g_mainForm.logMessage("Could not create an instance of " + pluginName,Color.Red);
					continue;
				}

				//If object creation was successful, we can save information about this loaded plugin
				//and create the GUI elements and load the assembly into the app domain.  Only do this
				//for external dlls
				if (pluginName.Contains("plugin.dll"))
					AppDomain.CurrentDomain.Load(pluginAssembly.FullName);

				if (pluginInstance != null)
				{
					//First, initialize the plugin instance.  This must be the first thing we do with the new
					//plugin instance.
					pluginInstance.init(Program.g_mainForm);
					//pluginInstance.PluginAssembly = pluginAssembly;//Before we go any further, make sure that we don't already have a 

					//plugin using the same name as the current plugin
					if (m_loadedPlugins[pluginInstance.PluginID] != null)
					{
						Program.g_mainForm.logMessage("A previously loaded plugin is already using the plugin ID \"" + pluginInstance.PluginID + "\".  This plugin will not be loaded.  Plugin IDs must be unique.",Color.Red);
						continue;
					}
					
					//Validate the plugin ID 
					if (!Utility.validateLabel(pluginInstance.PluginID))
					{
						MessageBox.Show("The plugin ID \"" + pluginInstance.PluginID + "\" is reserved.  Choose a different plugin ID to avoid conflicts.  This plugin will not be loaded.","Plugin ID Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
						continue;
					}

					//Now, create a new button
					ButtonItem newButton = new ButtonItem();
					newButton.Image = pluginInstance.MenuImage;
					newButton.Name = pluginInstance.PluginID;
					newButton.Text = pluginInstance.PluginID;
					if (type.IsDefined(typeof(ControllerPluginDescriptor),true))
						newButton.Click += new System.EventHandler(handleCreateControllerInstance);
					else if (type.IsDefined(typeof(PostEffectPluginDescriptor),true))
						newButton.Click += new System.EventHandler(handlePostEffectCreateInstance);
					else if (type.IsDefined(typeof(SceneNodePluginDescriptor),true))
						newButton.Click += new System.EventHandler(handleSceneNodeCreateInstance);
					else
						throw new Exception("Didn't recognize type");
					Utility.addButtonSorted(createButton,newButton);
					//createButton.SubItems.Add(newButton);
					newButton.CanCustomize = false;

					//Add the menu image to the main form if available
					if (pluginInstance.MenuImage != null)
						Program.g_mainForm.Icons.Images.Add(pluginInstance.PluginID,pluginInstance.MenuImage);

					//Finally, install the plugin into the plugin manager
					m_loadedPlugins.Add(pluginInstance.PluginID,pluginInstance);

					/*AssemblyName assemblyName = pluginAssembly.GetName();
					string description = null;

					object[] attributes = pluginAssembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute),false);
					if (attributes.Length > 0)
						description = ((AssemblyDescriptionAttribute)attributes[0]).Description;
					attributes = pluginAssembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute),false);*/

					//AssemblyName name = pluginAssembly.GetName();
					//MessageBox.Show(name.Version.ToString());
					//pluginAssembly.
				}
				else
				{
					Program.g_mainForm.logMessage("Could not instanciate plugin " + pluginName,Color.Red);
					continue;
				}
			}
			Program.g_mainForm.PluginDialog.addPluginInfo(pluginAssembly);
		}

		public TreeNode createInstance(string pluginName)
		{
			//Create a plugin instance from the passed in string
			BasePlugin plugin = m_loadedPlugins[pluginName] as BasePlugin;
			if (plugin != null)
			{
				//Ask the plugin to create an instance for us.  
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;
				return plugin.createInstance(XVPMHandle,Program.g_mainForm.ActiveSceneExplorer);
			}

			return null;
		}

		private void handleSceneNodeCreateInstance(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the type of object
			//we want the plugin manager to create
			ButtonItem buttonClicked = (ButtonItem)sender;
			createSceneNodeInstance(buttonClicked.Text);
		}

		public TreeNode createSceneNodeInstance(string pluginID)
		{
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			TreeNode topLevelNode = null;
			if (plugin != null)
			{
				//Ask the plugin to create an instance for us.  
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;

				topLevelNode = plugin.createInstance(XVPMHandle,Program.g_mainForm.ActiveSceneExplorer);
				if (topLevelNode != null)
				{
					//throw new Exception("createInstance() for plugin " + buttonClicked.Text + " did return a valid top level tree node");

					//Add the tag identifying this tree as a plugin instance and add the tree
					topLevelNode.Tag = pluginID;
					Program.g_mainForm.ActiveSceneExplorer.addTree(topLevelNode,true);
				}
			}
			return topLevelNode;
		}

		private void handlePostEffectCreateInstance(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the type of object
			//we want the plugin manager to create
			ButtonItem buttonClicked = (ButtonItem)sender;
			createPostEffectInstance(buttonClicked.Text);
		}

		public TreeNode createPostEffectInstance(string pluginID)
		{
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			TreeNode topLevelNode = null;
			if (plugin != null)
			{
				//Ask the plugin to create an instance for us.  
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;

				topLevelNode = plugin.createInstance(XVPMHandle,Program.g_mainForm.ActiveSceneExplorer);
				if (topLevelNode != null)
				{
					//if (topLevelNode == null)
					//throw new Exception("createInstance() for plugin " + buttonClicked.Text + " did return a valid top level tree node");

					//Add the tag identifying this tree as a plugin instance and add the tree
					topLevelNode.Tag = pluginID;
				}
			}

			return topLevelNode;
		}

		private void handleCreateControllerInstance(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the type of object
			//we want the plugin manager to create
			ButtonItem buttonClicked = (ButtonItem)sender;
			createControllerInstance(buttonClicked.Text);
		}

		public void createControllerInstance(string pluginID)
		{
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				//First, check to make sure there is a valid node selected
				TreeNode selectedNode = Program.g_mainForm.ActiveSceneExplorer.getHierarchyTreeView().SelectedNode;
				if (selectedNode == null)
				{
					MessageBox.Show("You must select a node in order to attach controllers.  You can select either a root node or a node in the hierarchy.","Node Selection");
					return;
				}

				//Can't attach controllers to certain nodes
				if (selectedNode.ImageKey == "Material" || 
					selectedNode.ImageKey == "Controller" ||
					selectedNode.Name == "Folder" ||
					Program.g_mainForm.ActiveSceneExplorer.getNode(selectedNode.Name) as Objects.ActorContainer != null)
				{
					MessageBox.Show("Controllers cannot be attached to your current selection.  For example, material and controller nodes do not accept controller attachments.","Node Selection");
					return;
				}

				//Ask the plugin to create an instance for us.  
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;

				//Create and display the controller attachment dialog
				AttachControllerDialog dialog = new AttachControllerDialog();
				dialog.init(pluginID,selectedNode);

				//If the user clicked OK, let's do some attaching...
				TreeNode rootNode = selectedNode;
				while (true)
				{
					if (rootNode.Parent == null)
						break;
					else if (rootNode.Parent.Name == "Folder")
						break;
					else if (Program.g_mainForm.ActiveSceneExplorer.getNode(rootNode.Parent.Name) as Objects.ActorContainer != null)
						break;
					else
						rootNode = rootNode.Parent;
				}

				//while (rootNode.Parent != null)
				//	rootNode = rootNode.Parent;
				SceneNode rootSceneNode = Program.g_mainForm.ActiveSceneExplorer.getNode(rootNode.Name) as SceneNode;

				if (dialog.ShowDialog() == DialogResult.OK)
				{
					//First, create the plugin instance
					Controller controller = plugin.createControllerInstance(dialog.txtName.Text);
					string nodeGUID = System.Guid.NewGuid().ToString();
					Program.g_mainForm.ActiveSceneExplorer.addNode(nodeGUID,controller);

					for (int x = 0; x < dialog.m_selectedNodeGUIDs.Count; ++x)
					{
						TreeNode[] searchNodes = null;

						//Find the node in the scene
						if (selectedNode.Name == (string)dialog.m_selectedNodeGUIDs[x])
						{
							searchNodes = new TreeNode[1];
							searchNodes[0] = selectedNode;
						}
						else
						{
							searchNodes = selectedNode.Nodes.Find((string)dialog.m_selectedNodeGUIDs[x],true);
						}
						for (int i = 0; i < searchNodes.Length; ++i)
						{
							TreeNode newNode = searchNodes[i].Nodes.Add(nodeGUID,dialog.txtName.Text);
							newNode.ImageKey = newNode.SelectedImageKey = "Controller";
							Program.g_mainForm.ActiveSceneExplorer.hookupMenus(newNode);

							//Create the proxy instance exactly once
							if (controller.Proxy == IntPtr.Zero)
								plugin.createProxyInstance(XVPMHandle,Program.g_mainForm.ActiveSceneExplorer,newNode);
							rootSceneNode.attachController(controller.Proxy,searchNodes[i]);
						}
					}

					//Break out new controller to controller view
					Program.g_mainForm.ActiveSceneExplorer.findControllers(selectedNode);
				}

				dialog.Dispose();
			}
		}

		public bool createProxyInstance(string pluginID,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
			//Get the plugin instance and tell it to create a proxy version of itself.
			//If we don't find a plugin that's okay, it might be a custom data node, etc...
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				//Ask the plugin to create a proxy instance of itself.  If we are at this point,
				//then there must be an active actor studio document
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;
				return plugin.createProxyInstance(XVPMHandle,sceneExplorer,treeNode);
			}
			else
				return false;
		}

		public void destroyProxyInstance(string pluginID,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
			if (pluginID == "NO_PLUGIN_ID")
				throw new Exception("Attempting to destroyProxyInstance on an object with plugin id = NO_PLUGIN_ID");

			//Get the plugin instance and tell it to destroy this proxy instance
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				//Ask the plugin to destroy a proxy instance of itself
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;
				plugin.destroyProxyInstance(XVPMHandle,sceneExplorer,treeNode);
			}
		}

		public void setActorContainer(string pluginID,SceneExplorer sceneExplorer,TreeNode treeNode,Objects.ActorContainer actorContainer)
		{
			//Get the plugin instance and let it know that it's being attached to an 
			//actor container
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;
				plugin.setActorContainer(XVPMHandle,sceneExplorer,treeNode,actorContainer);
			}
		}
		
		public ExportResults exportInstance(string pluginID,string exportDir,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
			//Get the plugin instance
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				//Ask the plugin to export an instance of itself
				return plugin.exportInstance(exportDir,sceneExplorer,treeNode);
			}
			else
				throw new Exception("Could not locate base plugin during export for pluginID " + pluginID);

		}

		public ActionExplorerBase getActionExplorer(SceneNode sceneNode)
		{
			//Null is ok, just return null
			if (sceneNode == null)
				return null;

			//Get the plugin instance
			BasePlugin plugin = m_loadedPlugins[sceneNode.PluginID] as BasePlugin;
			if (plugin != null)
			{
				//Ask the plugin for the UserControl we plan on sticking in the Action Explorer
				ActionExplorerBase uc = plugin.getActionExplorer();
				if (uc != null)
					uc.setSceneNode(sceneNode);
				return uc;
			}
			else
				return null;
		}

		public void setScene(string pluginID,string sceneName,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
			//Get the plugin instance and let it know that it's scene is being
			//modified
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				plugin.setScene(Program.g_mainForm.ActiveXVPMHandle,sceneName,sceneExplorer,treeNode);
			}
		}

		public bool setLightmap(string pluginID,SceneExplorer sceneExplorer,TreeNode node,string lightmapPath)
		{
			//Get the plugin instance and set the lightmap
			BasePlugin plugin = m_loadedPlugins[pluginID] as BasePlugin;
			if (plugin != null)
			{
				uint XVPMHandle = Program.g_mainForm.ActiveXVPMHandle;
				return plugin.setLightmap(XVPMHandle,sceneExplorer,node,lightmapPath);
			}
			else
				return false;
		}
	}
}
