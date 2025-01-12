using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.Objects;
using XVPDesigner.Dialogs;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using DevComponents.DotNetBar;
using XVPManaged;
using System.Runtime.InteropServices;
using Utilities;

namespace XVPDesigner.UserControls
{
	[Serializable]
	public partial class SceneExplorer : UserControl
    {
		[Serializable]
		public class ActorInfo
		{
			public string m_path;
			public DateTime m_creationTimestamp;

			public ActorInfo(string path,DateTime creationTimestamp)
			{
				m_path = path;
				m_creationTimestamp = creationTimestamp;
			}
		};

		uint m_XVPMHandle;
		private SuperTooltip m_superTooltip = new SuperTooltip();
		//private uint m_nextIndex = 0;

		private bool m_createActorGroup = false;
		public bool CreateActorGroup
		{
			get { return m_createActorGroup; }
			set { m_createActorGroup = value; }
		}

		private PropertyExplorer m_propertyExplorer;
		public PropertyExplorer PropertyExplorer
		{
			get { return m_propertyExplorer; }
			set { m_propertyExplorer = value; }
		}

		private bool m_dirty = false;
		public bool Dirty
		{
			get { return m_dirty; }
			set { m_dirty = value; }
		}

		//Hashtable used to store each node's hashtable. 
		private Hashtable m_hashtable = new Hashtable(500);
		public Hashtable Hashtable
		{
			get { return m_hashtable; }
			set { m_hashtable = value; }
		}
	
        public SceneExplorer()
        {
            InitializeComponent();

			tvHierarchy.ImageList = Program.g_mainForm.Icons;
			tvMaterials.ImageList = Program.g_mainForm.Icons;
			tvControllers.ImageList = Program.g_mainForm.Icons;
        }

		public SceneExplorer(SerializationInfo info,StreamingContext context): this() 
		{
			SerializationInfoEnumerator infoEnumerator = info.GetEnumerator();

			//First, we deserialize the hashtable
			/*if (infoEnumerator.ObjectType != m_hashtable.GetType())
				MessageBox.Show("Unrecognized file format.  Could not find hashtable.","File Open Error");
			else
			{
				Hashtable hashtable;
				hashtable = (Hashtable)info.GetValue(infoEnumerator.Name,infoEnumerator.ObjectType);
				
				
			}*/
			

			while (infoEnumerator.MoveNext()) 
			{
				TreeNode node = info.GetValue(infoEnumerator.Name, infoEnumerator.ObjectType) as TreeNode;

				//Add the tree assuming hashtable of scenenodes already exists by passing null for
				//second param
				addTree(node,false);
			}			
		}

		public bool init(uint XVPMHandle,ArrayList shaderNames)
		{
			m_XVPMHandle = XVPMHandle;
			return setShaderNames(shaderNames);
		}

		public bool setShaderNames(ArrayList shaderNames)
		{
			tscbShader.Items.Clear();

			foreach (string s in shaderNames)
			{
				//Make sure the shader is registered,loaded and compiled or there's no point in 
				//displaying it because XVP won't know how to create an instance of it
				//if the user is allowed to select it.
				bool load = false;
				string shaderName = s;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					IntPtr pShaderName;
					IntPtr pTemplateName;
					if (Utility.isCompoundShaderName(s))
					{
						string templateName = null;
						Utility.getCompoundShaderNameParts(s,out shaderName,out templateName);
						pTemplateName = Marshal.StringToHGlobalAnsi(templateName);
						pShaderName = Marshal.StringToHGlobalAnsi(shaderName);
						load = XVPM.isShaderRegistered(pTemplateName) && 
								XVPM.shaderExists(pShaderName) &&
								File.Exists(Application.StartupPath + "\\" + s + ".dll");
						Marshal.FreeHGlobal(pShaderName);
						Marshal.FreeHGlobal(pTemplateName);
					}
					else
					{
						pShaderName = Marshal.StringToHGlobalAnsi(shaderName);
						load = XVPM.isShaderRegistered(pShaderName) && 
								XVPM.shaderExists(pShaderName) &&
								File.Exists(Application.StartupPath + "\\" + s + ".dll");
						Marshal.FreeHGlobal(pShaderName);
					}
					
					
				}
				if (!load)
					continue;

				if (Utility.isCompoundShaderName(s))
					tscbShader.Items.Add(Utility.convertToShaderDisplayName(s));
				else
					tscbShader.Items.Add(shaderName);
			}
			return true;
		}

		public TreeNode findRootOfSelectedHierarchyNode()
		{
			//THIS FUNCTION HAS A HORRIBLE BUG, REMOVE IT ASAP
			//IF MULTIPLE SELECTION THEN WHAT???
			//REMOVED ALL BUT ONE PLACE... INVESTIGATE

			//If there is no selected node, we can't continue
			if (tvHierarchy.SelectedNode == null)
				return null;

			TreeNode parentNode = tvHierarchy.SelectedNode;
			while (true)
			{
				if (parentNode.Parent == null)
					break;
				else if (parentNode.Parent.Name == "Folder")
					break;
				else if (getNode(parentNode.Parent.Name) as Objects.ActorContainer != null)
					break;
				parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		public TreeNode findRootOfSelectedMaterialsNode()
		{
			//If there is no selected node, we can't continue
			if (tvMaterials.SelectedNode == null)
				return null;

			TreeNode parentNode = tvMaterials.SelectedNode;
			while (true)
			{
				if (parentNode.Parent == null)
					break;
				else if (parentNode.Parent.Name == "Folder")
					break;
				else if (getNode(parentNode.Parent.Name) as Objects.ActorContainer != null)
					break;
				else
					parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		public TreeNode findRootOfSelectedControllersNode()
		{
			//If there is no selected node, we can't continue
			if (tvControllers.SelectedNode == null)
				return null;

			TreeNode parentNode = tvControllers.SelectedNode;
			while (true)
			{
				if (parentNode.Parent == null)
					break;
				else if (parentNode.Parent.Name == "Folder")
					break;
				else if (getNode(parentNode.Parent.Name) as Objects.ActorContainer != null)
					break;
				else
					parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		private TreeNode findRootOfSelectedNode(TreeView treeView)
		{
			//If there is no selected node, we can't continue
			if (treeView.SelectedNode == null)
				return null;

			TreeNode parentNode = treeView.SelectedNode;
			while (true)
			{
				if (parentNode.Parent == null)
					break;
				else if (parentNode.Parent.Name == "Folder")
					break;
				else if (getNode(parentNode.Parent.Name) as Objects.ActorContainer != null)
					break;
				else
					parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		private TreeNode findRootOfNode(TreeNode node)
		{
			//If the node is invalid
			if (node == null)
				return null;

			TreeNode parentNode = node;
			while (true)
			{
				if (parentNode.Parent == null)
					break;
				else if (parentNode.Parent.Name == "Folder")
					break;
				else if (getNode(parentNode.Parent.Name) as Objects.ActorContainer != null)
					break;
				else
					parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		private bool isPluginRoot(TreeNode node)
		{
			//If the node is invalid
			if (node == null)
				return false;

			if (node.Parent != null)
			{
				if (((node.Parent.Name == "Folder") || (getNode(node.Parent.Name) as Objects.ActorContainer != null)) &&
					(node.Name != "Folder" && (getNode(node.Name) as Objects.ActorContainer == null)))
					return true;
			}
			else
			{
				if (node.Name != "Folder" && (getNode(node.Name) as Objects.ActorContainer == null))
					return true;
			}

			return false;
		}

		private void tvMaterials_AfterSelect(object sender,TreeViewEventArgs e)
		{
			//If nothing is selected, we're done
			if (((TreeViewMS)sender).SelectedNodes.Count == 0)
				return;

			//Find the selected material nodes in the database and set them current so their properties become
			//editable. Also, if all nodes selected are material nodes, enable the shader selection box,
			//otherwise, disable it
			string shaderName = "";
			bool sameShaderNames = true;
			ArrayList sceneNodes = new ArrayList(((TreeViewMS)sender).SelectedNodes.Count);
			for (int i = 0; i < ((TreeViewMS)sender).SelectedNodes.Count; ++i)
			{
				SceneNode sceneNode = (SceneNode)m_hashtable[(((TreeViewMS)sender).SelectedNodes[i] as TreeNode).Name];
				Material mat = sceneNode as Material;
				if (mat == null)
				{
					tscbShader.Enabled = false;
					tscbShader.SelectedItem = null;
					m_propertyExplorer.setNodes(null);
					Program.g_mainForm.setActionExplorer(null);
					return;
				}
				else
				{
					if (i == 0)
						shaderName = mat.ShaderName;
					else
					{
						if (shaderName != mat.ShaderName)
							sameShaderNames = false;
					}
					sceneNodes.Add(mat);
				}
			}

			if (!sameShaderNames)
				tscbShader.SelectedItem = null;
			else
			{
				if (Utility.isCompoundShaderName(shaderName))
					tscbShader.SelectedItem = Utility.convertToShaderDisplayName(shaderName);
				else
					tscbShader.SelectedItem = shaderName;
			}
			tscbShader.Enabled = true;
			m_propertyExplorer.setNodes(sceneNodes);

			if (sceneNodes.Count > 1)
				Program.g_mainForm.setActionExplorer(null);
			else if (sceneNodes.Count == 1)
				Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);
		}

		private void tvHierarchy_AfterSelect(object sender,TreeViewEventArgs e)
		{
			TreeViewMS tvms = (TreeViewMS)sender;

			//If nothing is selected, we're done
			if (tvms.SelectedNodes.Count == 0)
				return;

			//Find the selected nodes in the database and set them current so their properties become
			//editable.
			ArrayList sceneNodes = new ArrayList(tvms.SelectedNodes.Count);
			for (int i = 0; i < tvms.SelectedNodes.Count; ++i)
			{
				SceneNode sceneNode = (SceneNode)m_hashtable[(tvms.SelectedNodes[i] as TreeNode).Name];
				if (sceneNode != null)
					sceneNodes.Add(sceneNode);
			}

			if (sceneNodes.Count > 1)
				Program.g_mainForm.setActionExplorer(null);
			else if (sceneNodes.Count == 1)
				Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);

			m_propertyExplorer.setNodes(sceneNodes);

			//Next, update the scene box to display the scene this selection is associated with
			bool allSelectedValid = true;
			for (int i = 0; i < tvms.SelectedNodes.Count; ++i)
			{
				if (!isPluginRoot(tvms.SelectedNodes[i] as TreeNode))
				{
					allSelectedValid = false;
					break;
				}
			}
			if (allSelectedValid)
			{
				if (tvms.SelectedNodes.Count > 1)
					btnSceneSelection.Text = "Multiple";//btnSceneSelection.Text = "Multiple";
				else
				{
					SceneNode sceneNode = (SceneNode)m_hashtable[(tvms.SelectedNodes[0] as TreeNode).Name];
					if (sceneNode != null)
						btnSceneSelection.Text = sceneNode.SceneName;
				}
				btnSceneSelection.Enabled = true;// btnSceneSelection.Enabled = true;
			}
			else
			{
				btnSceneSelection.Text = "";
				btnSceneSelection.Enabled = false;// btnSceneSelection.Enabled = false;
			}

		}

		private void tvControllers_AfterSelect(object sender,TreeViewEventArgs e)
		{
			//If nothing is selected, we're done
			if (((TreeViewMS)sender).SelectedNodes.Count == 0)
				return;

			//Find the selected nodes in the database and set them current so their properties become
			//editable.
			ArrayList sceneNodes = new ArrayList(((TreeViewMS)sender).SelectedNodes.Count);
			for (int i = 0; i < ((TreeViewMS)sender).SelectedNodes.Count; ++i)
			{
				SceneNode sceneNode = (SceneNode)m_hashtable[(((TreeViewMS)sender).SelectedNodes[i] as TreeNode).Name];
				if (sceneNode != null)
					sceneNodes.Add(sceneNode);
			}

			if (sceneNodes.Count > 1)
				Program.g_mainForm.setActionExplorer(null);
			else if (sceneNodes.Count == 1)
				Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);

			m_propertyExplorer.setNodes(sceneNodes);
		}

		public void findMaterials(TreeNode treeNode)
		{
			//Look for any material nodes in the tree we are adding.
			//We want to duplicate those into the material tab for easy access
			//TreeNode rootNode = findRootOfNode(treeNode);

			//Special case stand alone materials, we don't want to create a group node
			//for them in the materal view, just add the material directly
			SceneNode topNode = (SceneNode)m_hashtable[treeNode.Name];
			/*if (topNode as UserMaterial != null)
			{
				TreeNode newNode = tvMaterials.Nodes.Add(treeNode.Name,treeNode.Text);
				newNode.ImageKey = newNode.SelectedImageKey = "Material";
				newNode.ContextMenu = new ContextMenu();
				MenuItem mi = new MenuItem("Rename",new EventHandler(materialViewPopup));
				mi.Name = "RenameSceneNode";
				newNode.ContextMenu.MenuItems.Add(mi);
				return;
			}*/

			ArrayList matNodes = new ArrayList();

			isMaterial(treeNode,matNodes);
			findMaterialsRecursive(treeNode,matNodes);

			//If we found some material nodes in the tree
			if (matNodes.Count != 0)
			{
				//If the root node already exists in the material view, we want to add any
				//new materials found to that node...otherwise, this is a brand new material
				//group node and we want create it in the material tree view.  
				for (int i = 0;i < matNodes.Count;++i)
				{
					TreeNode matNode = (TreeNode)matNodes[i];

					TreeNode matGroupNode = null;
					TreeNode rootNode = findRootOfNode(matNode);
					TreeNode[] matchingNodes = tvMaterials.Nodes.Find(rootNode.Name,false);
					if (matchingNodes.Length == 0)
					{
						matGroupNode = tvMaterials.Nodes.Add(rootNode.Name,rootNode.Text,rootNode.ImageKey,rootNode.SelectedImageKey);
						matGroupNode.ContextMenu = new ContextMenu();
						MenuItem mi = new MenuItem("Rename",new EventHandler(materialViewPopup));
						mi.Name = "RenameSceneNode";
						matGroupNode.ContextMenu.MenuItems.Add(mi);
						mi = new MenuItem("Show in Hierarchy View",new EventHandler(materialViewPopup));
						mi.Name = "ShowInHierarchyView";
						matGroupNode.ContextMenu.MenuItems.Add(mi);
					}
					else
						matGroupNode = matchingNodes[0];

					//If the root node is a material node, then we are done
					if (isPluginRoot(matNode))
					//if (getNode(matNode.Name) != null)
					//	if (getNode(matNode.Name) as Material != null)
							continue;

					//Only add each unique node into the tree once
					TreeNode[] treeNodes = matGroupNode.Nodes.Find(matNode.Name,true);
					if (treeNodes.Length == 0)
					{
						TreeNode newNode = matGroupNode.Nodes.Add(matNode.Name,matNode.Text);
						newNode.ImageKey = newNode.SelectedImageKey = "Material";
						newNode.ContextMenu = new ContextMenu();
						MenuItem mi = new MenuItem("Rename",new EventHandler(materialViewPopup));
						mi.Name = "RenameSceneNode";
						newNode.ContextMenu.MenuItems.Add(mi);
					}
				}
			}

		}

		private void findMaterialsRecursive(TreeNode treeNode,ArrayList matNodes)
		{
			foreach (TreeNode node in treeNode.Nodes)
			{
				isMaterial(node,matNodes);
				findMaterialsRecursive(node,matNodes);
			}
		}

		public void isMaterial(TreeNode node,ArrayList matNodes)
		{
			SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
			Material mat = sceneNode as Material;
			if (mat != null)
				matNodes.Add(node);
		}

		public void findControllers(TreeNode treeNode)
		{
			//Look for any controller nodes in the tree we are adding.
			//We want to duplicate those into the controller tab for easy access
			//TreeNode rootNode = findRootOfNode(treeNode);
			ArrayList controllerNodes = new ArrayList();
			findControllersRecursive(treeNode,controllerNodes);

			//If we found some controller nodes in the tree
			if (controllerNodes.Count != 0)
			{
				//If the root node already exists in the controller view, we want to add any
				//new controllers found to that node...otherwise, this is a brand new controller
				//group node and we want create it in the controller tree view
				

				for (int i = 0; i < controllerNodes.Count; ++i)
				{
					TreeNode controllerNode = (TreeNode)controllerNodes[i];
					TreeNode rootNode = findRootOfNode(controllerNode);
					TreeNode controllerGroupNode = null;
					TreeNode[] matchingNodes = tvControllers.Nodes.Find(rootNode.Name,false);
					if (matchingNodes.Length == 0)
					{
						controllerGroupNode = tvControllers.Nodes.Add(rootNode.Name,rootNode.Text,rootNode.ImageKey,rootNode.SelectedImageKey);
						controllerGroupNode.ContextMenu = new ContextMenu();
						MenuItem mi = new MenuItem("Rename",new EventHandler(controllerViewPopup));
						mi.Name = "RenameSceneNode";
						controllerGroupNode.ContextMenu.MenuItems.Add(mi);
						mi = new MenuItem("Show in Hierarchy View",new EventHandler(controllerViewPopup));
						mi.Name = "ShowInHierarchyView";
						controllerGroupNode.ContextMenu.MenuItems.Add(mi);
					}
					else
						controllerGroupNode = matchingNodes[0];

					//Only add each unique node into the tree once
					TreeNode[] treeNodes = controllerGroupNode.Nodes.Find(controllerNode.Name,true);
					if (treeNodes.Length == 0)
					{
						TreeNode newNode = controllerGroupNode.Nodes.Add(controllerNode.Name,controllerNode.Text);
						newNode.ImageKey = newNode.SelectedImageKey = "Controller";
						newNode.ContextMenu = new ContextMenu();
						MenuItem mi = new MenuItem("Rename",new EventHandler(controllerViewPopup));
						mi.Name = "RenameSceneNode";
						newNode.ContextMenu.MenuItems.Add(mi);
						mi = new MenuItem("Detach",new EventHandler(controllerViewPopup));
						mi.Name = "Detach";
						newNode.ContextMenu.MenuItems.Add(mi);
					}
				}
			}

		}
		
		private void findControllersRecursive(TreeNode treeNode,ArrayList controllerNodes)
		{
			foreach (TreeNode node in treeNode.Nodes)
			{
				SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
				Controller controller = sceneNode as Controller;
				if (controller != null)
					controllerNodes.Add(node);

				findControllersRecursive(node,controllerNodes);
			}
		}

		public void removeMaterials(TreeNode treeNode,bool removeGroupNode)
		{
			//Look for any material nodes in the tree we are removing.
			//We want to remove the duplicates in the material view
			TreeNode rootNode = findRootOfNode(treeNode);
			ArrayList matNodes = new ArrayList();

			//Find the same named material group node and any material nodes
			TreeNode matGroupNode = null;
			TreeNode[] matchingNodes = tvMaterials.Nodes.Find(this.findRootOfNode(treeNode).Name,false);
			if (matchingNodes.Length == 0)
				return;
			else
				matGroupNode = matchingNodes[0];
			findMaterialsRecursive(treeNode,matNodes);

			//If we found some material nodes in the tree
			if (matNodes.Count != 0)
			{
				for (int i = 0;i < matNodes.Count;++i)
				{
					TreeNode matNode = (TreeNode)matNodes[i];

					//Each material should only be listed once in the tree
					TreeNode[] treeNodes = matGroupNode.Nodes.Find(matNode.Name,true);
					if (treeNodes.Length != 0)
						matGroupNode.Nodes.Remove(treeNodes[0]);
				}
			}

			//Remove top level group node if told to do so
			if (removeGroupNode)
				matGroupNode.Remove();

		}


		/*public void removeControllers(TreeNode treeNode,bool removeGroupNode)
		{
			System.Console.WriteLine("The removeControllers function does not remove the proxy instance for controllers...need to refactor");
			//Look for any controller nodes in the tree we are removing.
			//We want to remove the duplicates in the controller view
			TreeNode rootNode = findRootOfNode(treeNode);
			ArrayList controllerNodes = new ArrayList();

			//Find the same named controller group node and any controller nodes
			TreeNode controllerGroupNode = null;
			TreeNode[] matchingNodes = tvControllers.Nodes.Find(this.findRootOfNode(treeNode).Name,false);
			if (matchingNodes.Length == 0)
				return;
			else
				controllerGroupNode = matchingNodes[0];
			findControllersRecursive(treeNode,controllerNodes);

			//If we found some controller nodes in the tree
			if (controllerNodes.Count != 0)
			{
				for (int i = 0; i < controllerNodes.Count; ++i)
				{
					TreeNode controllerNode = (TreeNode)controllerNodes[i];

					//Each controller should only be listed once in the tree
					TreeNode[] treeNodes = controllerGroupNode.Nodes.Find(controllerNode.Name,true);
					if (treeNodes.Length != 0)
						controllerGroupNode.Nodes.Remove(treeNodes[0]);
				}
			}

			//Remove top level group node if told to do so
			if (removeGroupNode)
				controllerGroupNode.Remove();

		}*/
		
		/*public void addController(TreeNode rootNode,string nodeGUID,Controller controller)
		{
			//Check to see if the controller group node already exists, if not,
			//create it.
			TreeNode controllerGroupNode = null;
			TreeNode[] tempNodes = tvControllers.Nodes.Find(rootNode.Name,false);
			if (tempNodes.Length > 1)
				throw new Exception("Found more than one controller group node with the same name.");
			if (tempNodes.Length == 0)
				controllerGroupNode = tvControllers.Nodes.Add(rootNode.Name,rootNode.Text,rootNode.ImageKey,rootNode.SelectedImageKey);
			else
				controllerGroupNode = tempNodes[0];

			//Only add each unique node into the tree once
			TreeNode[] treeNodes = controllerGroupNode.Nodes.Find(nodeGUID,true);
			if (treeNodes.Length == 0)
			{
				controllerGroupNode.Nodes.Add(nodeGUID,controller.Name,"Controller","Controller");
			}
		}*/

		public void removeController(TreeNode rootNode,TreeNode controllerNode)
		{
			//Get the controller group node (it must exist)
			TreeNode controllerGroupNode = null;
			TreeNode[] tempNodes = tvControllers.Nodes.Find(rootNode.Name,false);
			if (tempNodes.Length > 1)
				throw new Exception("Found more than one controller group node with the same name.");
			if (tempNodes.Length == 0)
				throw new Exception("Couldn't find controller group node.");
			else
				controllerGroupNode = tempNodes[0];

			//Now remove the controller and hashtable entry.  If there are no more controllers left under
			//the group node after removal, remove the group node too.
			TreeNode[] treeNodes = controllerGroupNode.Nodes.Find(controllerNode.Name,true);
			if (treeNodes.Length != 0)
			{
				treeNodes[0].Remove();
				m_hashtable.Remove(controllerNode.Name);
			}
			else
				throw new Exception("Found controller group node but did not find controller node.");

			if (controllerGroupNode.Nodes.Count == 0)
				controllerGroupNode.Remove();
		}

		//Adds a new tree to the scene explorer tree views assuming the data base nodes
		//have already been added.
		public bool addTree(TreeNode treeNode,bool addToSelection)
		{
			//Validate data
			if (treeNode.Text == "" || treeNode.Name == "")
				throw new Exception("Cannot add tree without a GUID or name");
			//if (m_hashtable.Contains(treeNode.Name))
			//	throw new Exception("A tree node with this GUID already exists in the hashtable.");

			//Add the tree to the treeview.  If a folder is currently selected add it to the folder,
			//otherwise, search up into the tree to find the nearest folder.  If there is no
			//folder, add it to the root node list
			if ((tvHierarchy.SelectedNode != null) && (addToSelection))
			{
				if (tvHierarchy.SelectedNode.Name == "Folder")
					tvHierarchy.SelectedNode.Nodes.Add(treeNode);
				else
				{
					TreeNode folderNode = tvHierarchy.SelectedNode;
					while (folderNode != null)
					{
						if (folderNode.Name == "Folder")
							break;
						folderNode = folderNode.Parent;
					}
					if (folderNode != null)
						folderNode.Nodes.Add(treeNode);
					else
						tvHierarchy.Nodes.Add(treeNode);
				}
			}
			else
				tvHierarchy.Nodes.Add(treeNode);

			//Hookup root context menu
			hookupRootContextMenu(treeNode);

			//Hookup sub node context menus
			hookupContextMenusRecursive(treeNode);
				
			//Break out the materials into the material view
			findMaterials(treeNode);

			//Break out the controllers into the controller view
			findControllers(treeNode);

			//Make visible and current selection
			tvHierarchy.SelectedNode = treeNode;
			treeNode.EnsureVisible();

			m_dirty = true;

			return true;
		}

		public void removeTree(TreeNode treeNode)
		{
			//Find the treenode that matches this tree node in the Hierarchy view.
			TreeNode[] foundNodes = tvHierarchy.Nodes.Find(treeNode.Name,true);
			if (foundNodes.Length != 1)
			{
				throw new Exception("While removing tree, did not find the root node in Hierarchy view.");
			}

			//Remove controllers from the controller view
			//removeControllers(foundNodes[0],true);

			//Remove materials from the material view
			removeMaterials(foundNodes[0],true);

			//Remove all nodes from the tree and all nodes entries from the hash table
			removeAllHashtableEntries(foundNodes[0]);
			/*
			foreach (TreeNode node in foundNodes[0].Nodes)
				removeNode(node.Name);
			m_hashtable.Remove(foundNodes[0].Name);*/

			//Finally, remove the tree itself and dispose of the top level node
			//SceneNode topLevelSceneNode = foundNodes[0] as SceneNode;
			//topLevelSceneNode.Dispose();
			foundNodes[0].Remove();


			m_dirty = true;
		}

		private void removeAllHashtableEntries(TreeNode treeNode)
		{
			foreach (TreeNode node in treeNode.Nodes)
				removeAllHashtableEntries(node);

			//Call dispose for each object before removal
			SceneNode sceneNode = m_hashtable[treeNode.Name] as SceneNode;
			if (sceneNode != null)
			{
				sceneNode.Dispose();
				m_hashtable.Remove(treeNode.Name);
			}
		}

		public void postDeserializeFixup()
		{
			//We need to spin through all the tree nodes in the tree view and
			//hook them up to their respective context menus
			foreach (TreeNode node in tvHierarchy.Nodes)
			{
				fixupNode(node);

				/*//Break out materials into material view
				findMaterials(node);

				//Break out controllers into controller view
				findControllers(node);
				
				//Hookup root context menu and sub node menus
				hookupRootContextMenu(node);
				hookupContextMenusRecursive(node);*/
			}

			//Check ranges in case data was saved with old range limits.  For example, if
			//a property called "Speed" was saved with a value of 50, but later the valid 
			//range for "Speed" becomes 0 - 25, the old value will be clamped to the new range.
			foreach (string key in m_hashtable.Keys)
			{
				SceneNode sceneNode = m_hashtable[key] as SceneNode;
				if (sceneNode != null)
				{
					sceneNode.postDeserializeRangeCheck();
				}
			}
		}

		private void fixupNode(TreeNode node)
		{
			//Break out materials into material view
			findMaterials(node);
			
			//Break out controllers into controller view
			findControllers(node);

			//Hookup root context menu and sub node menus
			hookupRootContextMenu(node);
			hookupContextMenusRecursive(node);
		}

		private void hookupRootContextMenu(TreeNode treeNode)
		{
			//Do we have a scene node registered with this tree node? If we do,
			//prepend some standard menu options onto it's context menu.  Otherwise,
			//we will create a new context menu.
			SceneNode sceneNode = (SceneNode)m_hashtable[treeNode.Name];
			if (sceneNode != null)
			{
				treeNode.ContextMenu = sceneNode.PopupMenu;

				//If we are a top level node (plugin instance) then we can add a delete option
				//if it does not exist
				if (treeNode.Parent == null)
				{
					if (treeNode.ContextMenu.MenuItems.Count > 0 && treeNode.ContextMenu.MenuItems[0].Name != "DeleteSceneNode")
					{
						MenuItem mi = new MenuItem("Delete",new EventHandler(hierarchyViewPopup));
						mi.Name = "DeleteSceneNode";
						treeNode.ContextMenu.MenuItems.Add(0,mi);
					}
				}
			}
			else
			{
				treeNode.ContextMenu = new ContextMenu();
				MenuItem mi = new MenuItem("Delete",new EventHandler(hierarchyViewPopup));
				mi.Name = "DeleteSceneNode";
				treeNode.ContextMenu.MenuItems.Add(mi);
				mi = new MenuItem("Rename",new EventHandler(hierarchyViewPopup));
				mi.Name = "RenameSceneNode";
				treeNode.ContextMenu.MenuItems.Add(mi);
				treeNode.ContextMenu.MenuItems.Add("-");
				mi = new MenuItem("Expand All Children",new EventHandler(hierarchyViewPopup));
				mi.Name = "ExpandAllChildren";
				treeNode.ContextMenu.MenuItems.Add(mi);
				mi = new MenuItem("Collapse All Children",new EventHandler(hierarchyViewPopup));
				mi.Name = "CollapseAllChildren";
				treeNode.ContextMenu.MenuItems.Add(mi);
			}
		}

		protected void hookupContextMenusRecursive(TreeNode treeNode)
		{
			foreach (TreeNode node in treeNode.Nodes)
			{
				//Do we have a scene node registered with this tree node?
				SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
				if (sceneNode != null)
					node.ContextMenu = sceneNode.PopupMenu;

				hookupContextMenusRecursive(node);
			}
		}

		public void hookupMenus(TreeNode treeNode)
		{
			hookupRootContextMenu(treeNode);
			hookupContextMenusRecursive(treeNode);
		}

		protected virtual void materialViewPopup(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			TreeNode selectedNode = tvMaterials.SelectedNode;
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;

			if (item == "Rename")
			{
				selectedNode.TreeView.LabelEdit = true;
				selectedNode.BeginEdit();
			}
			else if (item == "Show in Hierarchy View")
			{
				TreeNode[] nodes = scene.getHierarchyTreeView().Nodes.Find(selectedNode.Name,true);
				if (nodes.Length != 0)
				{
					nodes[0].EnsureVisible();
					scene.getHierarchyTreeView().SelectedNode = nodes[0];
					scene.showHierarchyTab();
				}
			}
			else if (item == "Expand All Children")
			{
				selectedNode.ExpandAll();
			}
			else if (item == "Collapse All Children")
			{
				selectedNode.Collapse();
			}
		}

		protected virtual void controllerViewPopup(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			TreeNode selectedNode = tvControllers.SelectedNode;
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;

			if (item == "Rename")
			{
				selectedNode.TreeView.LabelEdit = true;
				selectedNode.BeginEdit();
			}
			else if (item == "Show in Hierarchy View")
			{
				TreeNode[] nodes = scene.getHierarchyTreeView().Nodes.Find(selectedNode.Name,true);
				if (nodes.Length != 0)
				{
					nodes[0].EnsureVisible();
					scene.getHierarchyTreeView().SelectedNode = nodes[0];
					scene.showHierarchyTab();
				}
			}
			else if (item == "Expand All Children")
			{
				selectedNode.ExpandAll();
			}
			else if (item == "Collapse All Children")
			{
				selectedNode.Collapse();
			}
			else if (item == "Detach")
			{
				detachControllerFromControllerView(selectedNode);
			}
		}
		
		protected virtual void hierarchyViewPopup(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			TreeNode selectedNode = tvHierarchy.SelectedNode;

			if (item == "Delete")
			{
				//If nothing to delete return
				if (tvHierarchy.SelectedNodes.Count == 0)
					return;

				string nodeNames = "";
				for (int i = 0;i < tvHierarchy.SelectedNodes.Count;++i)
					nodeNames += ((TreeNode)tvHierarchy.SelectedNodes[i]).Text + "\n";		
				DialogResult result = MessageBox.Show("Delete the following? \n\n" + nodeNames,"Confirm Delete",
						MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (result == DialogResult.Yes)
				{
					for (int i = 0; i < tvHierarchy.SelectedNodes.Count; ++i)
						handleTreeDelete((TreeNode)tvHierarchy.SelectedNodes[i],true);
				}

				btnSceneSelection.Text = "";
				btnSceneSelection.Enabled = false;// btnSceneSelection.Enabled = false;
			} 
			if (item == "Rename")
			{
				selectedNode.TreeView.LabelEdit = true;
				selectedNode.BeginEdit();
			}
			if (item == "Expand All Children")
			{
				selectedNode.ExpandAll();
			}
			if (item == "Collapse All Children")
			{
				selectedNode.Collapse();
			}

		}


		public TreeView getHierarchyTreeView()
		{
			return tvHierarchy;
		}

		public TreeView getMaterialTreeView()
		{
			return tvMaterials;
		}

		public TreeView getControllerTreeView()
		{
			return tvControllers;
		}

		public bool addNode(string nodeGUID,SceneNode node)
		{
			//Does the node GUID already exist in the hash table?
			if (!m_hashtable.ContainsKey(nodeGUID))
			{
				m_hashtable.Add(nodeGUID,node);
				m_dirty = true;
				return true;
			}
			else
				return false;
		}

		public bool removeNode(string nodeName)
		{
			//Does the node name exist in the hash table?
			if (!m_hashtable.ContainsKey(nodeName))
				return false;
			else
			{
				m_hashtable.Remove(nodeName);
				m_dirty = true;
				return true;
			}
		}

		public SceneNode getNode(string nodeName)
		{
			return (SceneNode)m_hashtable[nodeName];
		}

		public bool containsRootNode(string rootNodeName)
		{
			if (!m_hashtable.ContainsKey(rootNodeName))
				return false;
			else
			{
				return true;
			}
		}

		/*public string generateUniqueName(string baseName)
		{
			TreeNode[] matchingNodes;
			bool goodNameFound = true;
			int index = 0;
			string newName = null;

			//Generate names until we find one that doesn't exist anywhere in the scene.
			do
			{
				++index;
				goodNameFound = true;
				newName = baseName + index.ToString();
				matchingNodes = tvHierarchy.Nodes.Find(newName,true);
				if (matchingNodes.Length > 0)
					goodNameFound = false;
				matchingNodes = tvMaterials.Nodes.Find(newName,true);
				if (matchingNodes.Length > 0)
					goodNameFound = false;
				matchingNodes = tvControllers.Nodes.Find(newName,true);
				if (matchingNodes.Length > 0)
					goodNameFound = false;
					
			} while (!goodNameFound);

			return newName;
		}*/

		public bool nodeNameExists(string name)
		{
			TreeNode[] matchingNodes = tvHierarchy.Nodes.Find(name,true);
			if (matchingNodes.Length > 0)
				return true;
			matchingNodes = tvMaterials.Nodes.Find(name,true);
			if (matchingNodes.Length > 0)
				return true;
			matchingNodes = tvControllers.Nodes.Find(name,true);
			if (matchingNodes.Length > 0)
				return true;

			return false;
		}

		/*public Hashtable getNodesHashtable(string hashTableName)
		{
			return (Hashtable)m_hashtable[hashTableName];
		}*/

		private void tvHierarchy_AfterLabelEdit(object sender,NodeLabelEditEventArgs e)
		{
			//Can't rename a lable to nothing and or a label didn't change
			if (e.Label == "" || e.Label == null)
			{
				e.CancelEdit = true;
				tvHierarchy.LabelEdit = false;
				return;
			}

			if (!Utility.validateLabel(e.Label))
			{
				MessageBox.Show("The label/string \"" + e.Label + "\" is reserved.  Use a different label/string to avoid conflicts.","Label/String Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
				tvHierarchy.LabelEdit = false;
				return;
			}

			//We can't rename the node if the scene node isn't renameable
			SceneNode sceneNode = (SceneNode)m_hashtable[e.Node.Name];
			if (sceneNode != null)
			{
				if (!sceneNode.Renameable)
				{
					e.CancelEdit = true;
					tvHierarchy.LabelEdit = false;
					return;
				}
				else
					sceneNode.Name = e.Label;
			}

			//Rename the same node in other views appropriately
			TreeNode[] matchingNodes = tvMaterials.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;
			matchingNodes = tvControllers.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;
			
			e.CancelEdit = false;
			Dirty = true;
			tvHierarchy.LabelEdit = false;
		}

		private void tvMaterials_AfterLabelEdit(object sender,NodeLabelEditEventArgs e)
		{
			//Can't rename a lable to nothing and or a label didn't change
			if (e.Label == "" || e.Label == null)
			{
				e.CancelEdit = true;
				tvMaterials.LabelEdit = false;
				return;
			}

			if (!Utility.validateLabel(e.Label))
			{
				MessageBox.Show("The label/string \"" + e.Label + "\" is reserved.  Use a different label/string to avoid conflicts.","Label/String Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
				tvMaterials.LabelEdit = false;
				return;
			}

			//We can't rename the node if the scene node isn't renameable
			SceneNode sceneNode = (SceneNode)m_hashtable[e.Node.Name];
			if (sceneNode != null)
			{
				if (!sceneNode.Renameable)
				{
					e.CancelEdit = true;
					tvMaterials.LabelEdit = false;
					return;
				}
			}

			//Rename the same node in other views appropriately
			TreeNode[] matchingNodes = tvHierarchy.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;
			matchingNodes = tvControllers.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;

			//Let the object know it's being renamed
			sceneNode.Name = e.Label;

			e.CancelEdit = false;
			Dirty = true;
			tvMaterials.LabelEdit = false;
		}

		private void tvControllers_AfterLabelEdit(object sender,NodeLabelEditEventArgs e)
		{
			//Can't rename a lable to nothing and or a label didn't change
			if (e.Label == "" || e.Label == null)
			{
				e.CancelEdit = true;
				tvControllers.LabelEdit = false;
				return;
			}

			if (!Utility.validateLabel(e.Label))
			{
				MessageBox.Show("The label/string \"" + e.Label + "\" is reserved.  Use a different label/string to avoid conflicts.","Label/String Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
				tvControllers.LabelEdit = false;
				return;
			}

			//We can't rename the node if the scene node isn't renameable
			SceneNode sceneNode = (SceneNode)m_hashtable[e.Node.Name];
			if (sceneNode != null)
			{
				if (!sceneNode.Renameable)
				{
					e.CancelEdit = true;
					tvControllers.LabelEdit = false;
					return;
				}
			}

			//Rename the same node in other views appropriately
			TreeNode[] matchingNodes = tvMaterials.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;
			matchingNodes = tvHierarchy.Nodes.Find(e.Node.Name,true);
			foreach (TreeNode treeNode in matchingNodes)
				treeNode.Text = e.Label;

			//Let the object know it's being renamed
			sceneNode.Name = e.Label;

			e.CancelEdit = false;
			Dirty = true;
			tvControllers.LabelEdit = false;
		}

	
		private void tvHierarchy_MouseDown(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				//Select the clicked node
				tvHierarchy.SelectedNode = tvHierarchy.GetNodeAt(e.X,e.Y);
			}
			if (e.Button == MouseButtons.Left)
			{
				if (tvHierarchy.GetNodeAt(e.Location) == null)
				{
					//tvHierarchy.SelectedNode = null;
					tvHierarchy.ClearSelectedNodes();
					m_propertyExplorer.setNodes(null);
				}
			}

			
		}

		private void tvMaterials_MouseDown(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				//Select the clicked node
				tvMaterials.SelectedNode = tvMaterials.GetNodeAt(e.X,e.Y);
			}
		}

		private void tvControllers_MouseDown(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				//Select the clicked node
				tvControllers.SelectedNode = tvControllers.GetNodeAt(e.X,e.Y);
			}
		}

		private void tvHierarchy_KeyDown(object sender,KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Delete)
			{
				//If nothing to delete return
				if (tvHierarchy.SelectedNodes.Count == 0)
					return;

				//Only plugin roots and folders are deletable
				for (int i = 0; i < tvHierarchy.SelectedNodes.Count; ++i)
				{
					if (!isPluginRoot((TreeNode)tvHierarchy.SelectedNodes[i]) && 
						((TreeNode)tvHierarchy.SelectedNodes[i]).ImageKey != "Folder")
					{
						MessageBox.Show("The node \"" + ((TreeNode)tvHierarchy.SelectedNodes[i]).Text + "\" is not deletable.  Select the root of the plugin instance, \"" + findRootOfNode(((TreeNode)tvHierarchy.SelectedNodes[i])).Text + "\" which contains this node to perform the delete operation.","Unable to Delete",MessageBoxButtons.OK,MessageBoxIcon.Information);
						return;
					}
				}

				string nodeNames = "";
				for (int i = 0; i < tvHierarchy.SelectedNodes.Count; ++i)
					nodeNames += ((TreeNode)tvHierarchy.SelectedNodes[i]).Text + "\n";
				DialogResult result = MessageBox.Show("Delete the following? \n\n" + nodeNames,"Confirm Delete",
						MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (result == DialogResult.Yes)
				{
					//We could have a mixture controllers and scene nodes... since controllers
					//must be destroyed before the scene nodes they're attached to, lets
					//do this delete in two steps.  First the controllers, then everything else
					ArrayList controllers = new ArrayList();
					ArrayList sceneNodes = new ArrayList();
					
					for (int i = 0; i < tvHierarchy.SelectedNodes.Count; ++i)
					{
						if (((TreeNode)tvHierarchy.SelectedNodes[i]).ImageKey == "Controller")
							controllers.Add(tvHierarchy.SelectedNodes[i]);
						else
							sceneNodes.Add(tvHierarchy.SelectedNodes[i]);
					}
					for (int i = 0; i < controllers.Count; ++i)
						detachControllerFromHierarchyViewNode(((TreeNode)controllers[i]));
					for (int i = 0; i < sceneNodes.Count; ++i)
						handleTreeDelete((TreeNode)sceneNodes[i],!e.Shift);

					controllers.Clear();
					sceneNodes.Clear();

					btnSceneSelection.Text = "";
					btnSceneSelection.Enabled = false;// btnSceneSelection.Enabled = false;
				}
			}
		}

		public void handleTreeDelete(TreeNode topLevelNode,bool confirmDelete)
		{
			handleTreeDeleteRecursive(topLevelNode);
			topLevelNode.Remove();
			m_propertyExplorer.setNodes(null);
		}

		private void handleTreeDeleteRecursive(TreeNode node)
		{
			//Depending on the type of scene node that we find, perform the following:
			// 1. If it is an actor container, destroy the proxy and keep recursing
			// 2. If it is any other type of SceneNode, treat it as a plugin, destroy it and stop recursing
			// 3. If it is any other type of object, ignore it and keep recursing...
			SceneNode sceneNode = (SceneNode)Program.g_mainForm.ActiveSceneExplorer.Hashtable[node.Name];
			if (sceneNode != null)
			{
				if (sceneNode as Objects.ActorContainer != null)
				{
					Objects.ActorContainer ac = sceneNode as Objects.ActorContainer;
					ac.destroyProxyInstance();
				}
				else
				{
					//Destroy controller proxy instances for this plugin first
					TreeNode controllerGroupNode = null;
					TreeNode[] matchingNodes = tvControllers.Nodes.Find(findRootOfNode(node).Name,false);
					if (matchingNodes.Length != 0)
						controllerGroupNode = matchingNodes[0];
					if (controllerGroupNode != null)
					{
						foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
						{
							//Get the node's type ID
							detachController(controllerGroupNode,controllerNode);
							//SceneNode controllerSceneNode = (SceneNode)Hashtable[controllerNode.Name];
							//Program.g_mainForm.PluginMgr.destroyProxyInstance(controllerSceneNode.PluginID,this,controllerNode);
						}
					}

					//Now destroy the rest of the plugin
					Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,Program.g_mainForm.ActiveSceneExplorer,node);

					removeTree(node);

					return;
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				handleTreeDeleteRecursive(childNode);
		}

		private void tvHierarchy_DragOver(object sender,System.Windows.Forms.DragEventArgs e)
		{
			TreeView tree = (TreeView)sender;
			e.Effect = DragDropEffects.None;
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));
			Point pt = new Point(e.X,e.Y);
			pt = tree.PointToClient(pt);
			TreeNode nodeTarget = tree.GetNodeAt(pt);

			if (nodeSource != null)
			{
				if (nodeSource.TreeView != tree)
				{
					switch (nodeSource.ImageKey)
					{
						case "Actor":
							e.Effect = DragDropEffects.Copy;
							break;
						case "Material":
							if (nodeTarget != null)
							{
								if (m_hashtable[nodeTarget.Name] as Material != null)
									e.Effect = DragDropEffects.Copy;
								else
									e.Effect = DragDropEffects.None;
							}
							break;
						case "Controller":
							if (nodeTarget != null)
							{
								if (m_hashtable[nodeTarget.Name] as Controller != null)
									e.Effect = DragDropEffects.Copy;
								else
									e.Effect = DragDropEffects.None;
							}
							break;
						case "Audio":
							e.Effect = DragDropEffects.Copy;
							break;
						case "Texture":
							e.Effect = DragDropEffects.Copy;
							break;
					}
				}
			}
		}

		private void tvHierarchy_DragDrop(object sender,System.Windows.Forms.DragEventArgs e)
		{
			TreeView tree = (TreeView)sender;
			Point pt = new Point(e.X,e.Y);
			pt = tree.PointToClient(pt);
			TreeNode nodeTarget = tree.GetNodeAt(pt);
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Material":
					if (nodeTarget != null)
					{
						//Are we dropping a material onto a material?
						if (m_hashtable[nodeTarget.Name] as Material != null)
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?", "Set Material Template", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
							{
								try
								{
									fs = File.Open(nodeSource.Name, FileMode.Open, FileAccess.Read);
								}
								catch (Exception ex)
								{
									Program.g_mainForm.logMessage(ex.ToString(), Color.Red);
									if (fs != null)
										fs.Close();
									return;
								}
				
								try
								{
									Material newMaterial = bf.Deserialize(fs) as Material;
									newMaterial.Name = nodeTarget.Text; 
									m_hashtable.Remove(nodeTarget.Name);
									m_hashtable.Add(nodeTarget.Name,newMaterial);
								}
								catch (Exception exception)
								{
									MessageBox.Show("Error occurred while using material template.  See the message log for details.", "Material Template Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
									Program.g_mainForm.logMessage(exception.ToString(), Color.Red);
								}
								fs.Close();
								m_dirty = true;

								//Refresh the selection to fire events
								nodeTarget.TreeView.SelectedNode = null;
								nodeTarget.TreeView.SelectedNode = nodeTarget;
							}
						}
					}
					break;
				case "Controller":
					if (nodeTarget != null)
					{
						//Are we dropping a controller onto a controller?
						if (m_hashtable[nodeTarget.Name] as Controller != null)
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?","Set Controller Template",MessageBoxButtons.YesNo,MessageBoxIcon.Question) == DialogResult.Yes)
							{
								try
								{
									fs = File.Open(nodeSource.Name,FileMode.Open,FileAccess.Read);
								}
								catch (Exception ex)
								{
									Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
									if (fs != null)
										fs.Close();
									return;
								}

								try
								{
									Controller newController = bf.Deserialize(fs) as Controller;
									newController.Name = nodeTarget.Text;
									m_hashtable.Remove(nodeTarget.Name);
									m_hashtable.Add(nodeTarget.Name,newController);
								}
								catch (Exception exception)
								{
									MessageBox.Show("Error occurred while using controller template.  See the message log for details.","Controller Template Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
									Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
								}
								fs.Close();
								m_dirty = true;

								//Refresh the selection to fire events
								nodeTarget.TreeView.SelectedNode = null;
								nodeTarget.TreeView.SelectedNode = nodeTarget;
							}
						}
					}
					break;
				case "Audio":
					if (nodeTarget == null)
					{
						TreeNode node = Program.g_mainForm.PluginMgr.createInstance("Audio Clip");
						AudioClip audioClip = m_hashtable[node.Name] as AudioClip;
						if (audioClip == null)
							throw new Exception("Could not create Audio Clip node");
						else
							audioClip.Filename = nodeSource.Name;

					}
					else
					{
						//Are we dropping audio onto audio?
						AudioClip audioClip = m_hashtable[nodeTarget.Name] as AudioClip;
						if (audioClip != null)
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?","Set Audio Template",MessageBoxButtons.YesNo,MessageBoxIcon.Question) == DialogResult.Yes)
								audioClip.Filename = nodeSource.Name;
						}
						else
						{
							TreeNode node = Program.g_mainForm.PluginMgr.createInstance("Audio Clip");
							if (audioClip == null)
								throw new Exception("Could not create Audio Clip node");
							else
								audioClip.Filename = nodeSource.Name;
						}
					}
					break;
				case "Texture":
					if (nodeTarget == null)
					{
						TreeNode node = Program.g_mainForm.PluginMgr.createInstance("Texture");
						Texture texture = m_hashtable[node.Name] as Texture;
						if (texture == null)
							throw new Exception("Could not create Texture node");
						else
							texture.Filename = nodeSource.Name;

					}
					else
					{
						//Are we dropping audio onto audio?
						Texture texture = m_hashtable[nodeTarget.Name] as Texture;
						if (texture != null)
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?","Set Texture Template",MessageBoxButtons.YesNo,MessageBoxIcon.Question) == DialogResult.Yes)
								texture.Filename = nodeSource.Name;
						}
						else
						{
							TreeNode node = Program.g_mainForm.PluginMgr.createInstance("Texture");
							if (texture == null)
								throw new Exception("Could not create Texture node");
							else
								texture.Filename = nodeSource.Name;
						}
					}
					break;
				case "Actor":
					//Dropping onto a folder causes the actor to be added to that folder
					//otherwise it's added at the root
					TreeNode addToFolder = null;
					if (nodeTarget != null)
						if (nodeTarget.Name == "Folder")
							addToFolder = nodeTarget;
					addActor(nodeSource.Name,nodeSource.Text,addToFolder,null);
					
					break;
					
			}
		}

		private void createProxyInstanceRecursive(TreeNode node)
		{
			//If we come across a scene node, then assume this is the beginning of a 
			//plugin instance... create a proxy instance and return.  Skip controllers.
			SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
			if ((sceneNode != null) && (node.ImageKey != "Controller"))
			{
				Program.g_mainForm.PluginMgr.createProxyInstance(sceneNode.PluginID,this,node);
				return;
			}

			foreach (TreeNode childNode in node.Nodes)
				createProxyInstanceRecursive(childNode);
		}

		private void applyNewGUIDsRecursive(TreeNode node,Hashtable hashtable)
		{
			SceneNode sceneNode = (SceneNode)hashtable[node.Name];
			if (sceneNode == null &&
				node.Name != "Folder")
				node.Name = System.Guid.NewGuid().ToString();

			foreach (TreeNode childNode in node.Nodes)
				applyNewGUIDsRecursive(childNode,hashtable);
		}

		public void setActorContainerRecursive(TreeNode node,Objects.ActorContainer actorContainer)
		{
			//If we come across a scene node, then assume this is the beginning of a 
			//plugin instance... notify it that it's being added to an actor container
			//and the return.  Skip controllers.
			SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
			if ((sceneNode != null) && (node.ImageKey != "Controller"))
			{
				Program.g_mainForm.PluginMgr.setActorContainer(sceneNode.PluginID,this,node,actorContainer);
				return;
			}

			foreach (TreeNode childNode in node.Nodes)
				setActorContainerRecursive(childNode,actorContainer);
		}

/*		public void Serialize(Stream stream,IFormatter formatter)
		{
			formatter.Serialize(stream,m_hashtable);
			formatter.Serialize(stream,this);
		} */

		/*public void GetObjectData(SerializationInfo info,StreamingContext context)
		{
			//First we serialize the hash table objects
			//info.AddValue("lynnstest","lynn");
			foreach (TreeNode node in tvHierarchy.Nodes)
			{
				info.AddValue(System.Guid.NewGuid().ToString(),node);
			}
		}*/

		public void reset()
		{
			tvHierarchy.Nodes.Clear();
			tvMaterials.Nodes.Clear();
			tvControllers.Nodes.Clear();
			m_hashtable.Clear();
			
			m_dirty = false;
		}

		public void setNodeArray(ArrayList nodes)
		{
			for (int i = 0; i < nodes.Count; ++i)
			{
				addTree((TreeNode)nodes[i],false);
			}
		}
		public ArrayList getNodeArray()
		{
			ArrayList al = new ArrayList();
			foreach (TreeNode node in tvHierarchy.Nodes)
				al.Add(node);
			return al;
		}

		private void cbShader_SelectedIndexChangedCommitted(object sender,EventArgs e)
		{		
			//If the selected item is null, we're done
			if (tscbShader.SelectedItem == null)
				return;

			for (int i = 0; i < tvMaterials.SelectedNodes.Count; ++i)
			{
				//Get the material name if a material node is selected and replace 
				//the material node with a new instance of the selected material shader
				SceneNode sceneNode = (SceneNode)m_hashtable[((TreeNode)tvMaterials.SelectedNodes[i]).Name];
				Material materialNode = sceneNode as Material;
	
				if (materialNode != null)
				{
					//If the shader name is not changing, don't do anything
					if (Utility.isCompoundShaderName(materialNode.ShaderName))
					{
						if (Utility.convertToShaderDisplayName(materialNode.ShaderName) == tscbShader.SelectedItem.ToString())
							return;
					}
					else
					{
						if (materialNode.ShaderName == tscbShader.SelectedItem.ToString())
							return;
					}

					//Mark dirty for material change
					m_dirty = true;

					//Check to see if we have a templated shader name, if so we must
					//convert it from display format to original format.  That is...
					//Eyeball.fx (Model-BlinnPhong.fx) back to Eyeball.fx#Model-BlinnPhong.fx
					string convertedShaderName = tscbShader.SelectedItem.ToString();
					string shaderTemplateName = convertedShaderName;
					if (convertedShaderName.Contains("("))
					{
						string[] parts = convertedShaderName.Split('(');
						shaderTemplateName = parts[1].Replace(')',' ').Trim();
						convertedShaderName = parts[0].Trim() + "#" + shaderTemplateName;	
					}

					//Save a copy of the GUID to re-apply
					string orgGUID = ((TreeNode)tvMaterials.SelectedNodes[i]).Name;

					//Determine which shader to create.  "SimpleWhite" is a special case that is just
					//the base material
					Material newMaterial = null;
					if (tscbShader.SelectedItem.ToString() != "White.fx")
						newMaterial = Program.g_mainForm.getShaderMgr().createMaterial(materialNode.Name,convertedShaderName);
					else
						newMaterial = new Material(materialNode.Name);
					if (newMaterial == null)
						throw new Exception("Could not create new material instance.");

					//Carry over the proxy first.  THEN set the new shader name and current values in this
					//exact order!
					newMaterial.Proxy = materialNode.Proxy;
					newMaterial.ShaderName = convertedShaderName;// shaderTemplateName;// cb.SelectedItem.ToString();
					newMaterial.setCurrentValues();
					
					//Replace the material node in the hashtable
					m_hashtable.Remove(orgGUID);
					m_hashtable.Add(orgGUID,newMaterial);
				}
			}
			
			//Refresh the property view
			ArrayList sceneNodes = new ArrayList(tvMaterials.SelectedNodes.Count);
			for (int i = 0; i < tvMaterials.SelectedNodes.Count; ++i)
			{
				SceneNode sceneNode = (SceneNode)m_hashtable[(tvMaterials.SelectedNodes[i] as TreeNode).Name];
				if (sceneNode != null)
					sceneNodes.Add(sceneNode);
			}
			m_propertyExplorer.setNodes(sceneNodes);

			if (sceneNodes.Count > 1)
				Program.g_mainForm.setActionExplorer(null);
			else if (sceneNodes.Count == 1)
				Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);


		}

		private void tabHierarchy_Click(object sender,EventArgs e)
		{
			if (tvHierarchy.SelectedNodes.Count != 0)
			{
				ArrayList sceneNodes = new ArrayList(tvHierarchy.SelectedNodes.Count);
				for (int i = 0; i < tvHierarchy.SelectedNodes.Count; ++i)
				{
					SceneNode sceneNode = (SceneNode)m_hashtable[(tvHierarchy.SelectedNodes[i] as TreeNode).Name];
					if (sceneNode != null)
						sceneNodes.Add(sceneNode);
				}

				if (sceneNodes.Count > 1)
					Program.g_mainForm.setActionExplorer(null);
				else if (sceneNodes.Count == 1)
					Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);

				m_propertyExplorer.setNodes(sceneNodes);
			}
			else
			{
				m_propertyExplorer.setNodes(null);
				Program.g_mainForm.setActionExplorer(null);
			}
		}

		private void tabMaterials_Click(object sender,EventArgs e)
		{
			if (tvMaterials.SelectedNodes.Count != 0)
			{
				ArrayList sceneNodes = new ArrayList(tvMaterials.SelectedNodes.Count);
				for (int i = 0; i < tvMaterials.SelectedNodes.Count; ++i)
				{
					SceneNode sceneNode = (SceneNode)m_hashtable[(tvMaterials.SelectedNodes[i] as TreeNode).Name];
					if (sceneNode != null)
						sceneNodes.Add(sceneNode);
				}

				if (sceneNodes.Count > 1)
					Program.g_mainForm.setActionExplorer(null);
				else if (sceneNodes.Count == 1)
					Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);

				m_propertyExplorer.setNodes(sceneNodes);
			}
			else
			{
				m_propertyExplorer.setNodes(null);
				Program.g_mainForm.setActionExplorer(null);
			}
		}

		private void tabControllers_Click(object sender,EventArgs e)
		{
			if (tvControllers.SelectedNodes.Count != 0)
			{
				ArrayList sceneNodes = new ArrayList(tvControllers.SelectedNodes.Count);
				for (int i = 0; i < tvControllers.SelectedNodes.Count; ++i)
				{
					SceneNode sceneNode = (SceneNode)m_hashtable[(tvControllers.SelectedNodes[i] as TreeNode).Name];
					if (sceneNode != null)
						sceneNodes.Add(sceneNode);
				}

				if (sceneNodes.Count > 1)
					Program.g_mainForm.setActionExplorer(null);
				else if (sceneNodes.Count == 1)
					Program.g_mainForm.setActionExplorer((SceneNode)sceneNodes[0]);

				m_propertyExplorer.setNodes(sceneNodes);
			}
			else
			{
				m_propertyExplorer.setNodes(null);
				Program.g_mainForm.setActionExplorer(null);
			}
		}

		public string getSelectedTabText()
		{
			return tabsScene.SelectedTab.Text;
		}

		public void showHierarchyTab()
		{
			tabsScene.SelectedTab = tabHierarchy;
		}

		public void showMaterialTab()
		{
			tabsScene.SelectedTab = tabMaterials;
		}

		public void showControllerTab()
		{
			tabsScene.SelectedTab = tabControllers;
		}

		private void tvMaterials_DragOver(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			//If we are dragging an actor over the material view, 
			//switch to the hierarchy tab
			if ((getSelectedTabText() != "Hierarchy") && (nodeSource.ImageKey == "Actor"))
				showHierarchyTab(); 
			else
			{
				TreeView tree = (TreeView)sender;
				e.Effect = DragDropEffects.None;
				Point pt = new Point(e.X,e.Y);
				pt = tree.PointToClient(pt);
				TreeNode nodeTarget = tree.GetNodeAt(pt);

				if (nodeSource != null)
				{
					if (nodeSource.TreeView != tree)
					{
						switch (nodeSource.ImageKey)
						{
							case "Material":
								if (nodeTarget != null)
								{
									if (nodeTarget.ImageKey == "Material")
										e.Effect = DragDropEffects.Copy;
									else
										e.Effect = DragDropEffects.None;
								}
								break;
						}
					}
				}
			}

		}

		private void tvControllerss_DragOver(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			//If we are dragging an actor over the controller view, 
			//switch to the hierarchy tab
			if ((getSelectedTabText() != "Hierarchy") && (nodeSource.ImageKey == "Actor"))
				showHierarchyTab();
			else
			{
				TreeView tree = (TreeView)sender;
				e.Effect = DragDropEffects.None;
				Point pt = new Point(e.X,e.Y);
				pt = tree.PointToClient(pt);
				TreeNode nodeTarget = tree.GetNodeAt(pt);

				if (nodeSource != null)
				{
					if (nodeSource.TreeView != tree)
					{
						switch (nodeSource.ImageKey)
						{
							case "Controller":
								if (nodeTarget != null)
								{
									if (nodeTarget.ImageKey == "Controller")
										e.Effect = DragDropEffects.Copy;
									else
										e.Effect = DragDropEffects.None;
								}
								break;
						}
					}
				}
			}

		}
		
		private void tsbExpandAllHierarchyNodes_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvHierarchy.ExpandAll();
		}

		private void tsbCollapseAllHierarchyNodes_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvHierarchy.CollapseAll();
		}

		private void tsbExpandAllMaterials_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvMaterials.ExpandAll();
		}

		private void tsbCollapseAllMaterials_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvMaterials.CollapseAll();
		}

		private void tsbExpandAllControllers_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvControllers.ExpandAll();
		}

		private void tsbCollapseAllControllers_Click(object sender,EventArgs e)
		{
			m_propertyExplorer.setNodes(null);
			tvControllers.CollapseAll();
		}

		private void tvHierarchy_EnabledChanged(object sender,EventArgs e)
		{
			if (!tvHierarchy.Enabled)
				tvHierarchy.BackColor = SystemColors.Control;
			else
				tvHierarchy.BackColor = Color.White;
		}

		private void tvMaterials_EnabledChanged(object sender,EventArgs e)
		{
			if (!tvMaterials.Enabled)
				tvMaterials.BackColor = SystemColors.Control;
			else
				tvMaterials.BackColor = Color.White;
		}

		private void tvControllers_EnabledChanged(object sender,EventArgs e)
		{
			if (!tvControllers.Enabled)
				tvControllers.BackColor = SystemColors.Control;
			else
				tvControllers.BackColor = Color.White;
		}

		private void tvMaterials_KeyDown(object sender,KeyEventArgs e)
		{
			//Only root nodes can be deleted
			if (e.KeyCode == Keys.Delete)
			{
				//If nothing to delete return
				if (tvMaterials.SelectedNodes.Count == 0)
					return;

				string nodeNames = "";
				for (int i = 0; i < tvMaterials.SelectedNodes.Count; ++i)
					nodeNames += ((TreeNode)tvMaterials.SelectedNodes[i]).Text + "\n";
				DialogResult result = MessageBox.Show("Delete the following? \n\n" + nodeNames,"Confirm Delete",
						MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (result == DialogResult.Yes)
				{
					for (int i = 0; i < tvMaterials.SelectedNodes.Count; ++i)
					{
						if (((TreeNode)tvMaterials.SelectedNodes[i]).Parent == null)
							handleTreeDelete((TreeNode)tvMaterials.SelectedNodes[i],!e.Shift);
					}
				}
			}
		}
	
		public void detachControllerFromHierarchyViewNode(TreeNode controllerNode)
		{
			//Now find the rootNode in the hiearchy view (must exist there)
			TreeNode rootNode = findRootOfNode(controllerNode);
			if (rootNode == null)
				throw new Exception("Could not find root node in hiearchy view while detaching controller.");

			//Detach the controller from the scene node
			SceneNode sceneNode = m_hashtable[rootNode.Name] as SceneNode;
			Controller controller = m_hashtable[controllerNode.Name] as Controller;
			sceneNode.detachController(controller.Proxy,controllerNode.Parent);

			//Remove the controller tree node
			controllerNode.Remove();

			//Now find the rootNode in the hiearchy view (must exist there) and 
			//search it's children for the presense of this controller node.
			TreeNode[] foundNodes = rootNode.Nodes.Find(controllerNode.Name,true);
			if (foundNodes.Length == 0)
			{
				//Destroy the plugin instance as well
				Program.g_mainForm.PluginMgr.destroyProxyInstance(controller.PluginID,this,controllerNode);
				
				//Controller was completely removed
				removeController(rootNode,controllerNode);
			}
		}

		public void detachControllerFromControllerView(TreeNode controllerNode)
		{
			//Get the controller group node (it must exist)
			TreeNode controllerGroupNode = findRootOfSelectedControllersNode();

			//Now find the rootNode in the hiearchy view (must exist there) and 
			//search it's children for the presense of this controller node.
			TreeNode[] hierarchyNode = tvHierarchy.Nodes.Find(controllerGroupNode.Name,true);
			if (hierarchyNode.Length == 0)
				throw new Exception("Could not find the hierarchy node.");
			TreeNode[] foundNodes = hierarchyNode[0].Nodes.Find(controllerNode.Name,true);
			for (int i = 0; i < foundNodes.Length; ++i)
			{
				SceneNode sceneNode = m_hashtable[hierarchyNode[0].Name] as SceneNode;
				Controller controller = m_hashtable[controllerNode.Name] as Controller;
				sceneNode.detachController(controller.Proxy,foundNodes[i].Parent);
				foundNodes[i].Remove();
			}

			//Remove the controller and hashtable entry and destroy the proxy instance. 
			//If there are no more controllers left under the group node after removal, 
			//remove the group node too.
			SceneNode controllerSceneNode = (SceneNode)Hashtable[controllerNode.Name];
			Program.g_mainForm.PluginMgr.destroyProxyInstance(controllerSceneNode.PluginID,this,controllerNode);
			controllerNode.Remove();
			m_hashtable.Remove(controllerNode.Name);
			
			if (controllerGroupNode.Nodes.Count == 0)
				controllerGroupNode.Remove();
		}

		public void detachController(TreeNode controllerGroupNode,TreeNode controllerNode)
		{
			//Now find the rootNode in the hiearchy view (must exist there) and 
			//search it's children for the presense of this controller node.
			TreeNode[] hierarchyNode = tvHierarchy.Nodes.Find(controllerGroupNode.Name,true);
			if (hierarchyNode.Length == 0)
				throw new Exception("Could not find the hierarchy node.");
			TreeNode[] foundNodes = hierarchyNode[0].Nodes.Find(controllerNode.Name,true);

			//Detach all instances where this controller was found attached to hierarchy nodes
			for (int i = 0; i < foundNodes.Length; ++i)
			{
				SceneNode sceneNode = m_hashtable[hierarchyNode[0].Name] as SceneNode;
				Controller controller = m_hashtable[controllerNode.Name] as Controller;
				sceneNode.detachController(controller.Proxy,foundNodes[i].Parent);
				foundNodes[i].Remove();
			}

			//Remove the controller and hashtable entry and destroy the proxy instance. 
			//If there are no more controllers left under the group node after removal, 
			//remove the group node too.
			SceneNode controllerSceneNode = (SceneNode)Hashtable[controllerNode.Name];
			Program.g_mainForm.PluginMgr.destroyProxyInstance(controllerSceneNode.PluginID,this,controllerNode);
			controllerNode.Remove();
			m_hashtable.Remove(controllerNode.Name);

			if (controllerGroupNode.Nodes.Count == 0)
				controllerGroupNode.Remove();
		}

		private void tvControllers_KeyDown(object sender,KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Delete)
			{
				//If nothing to delete return
				if (tvControllers.SelectedNodes.Count == 0)
					return;

				string nodeNames = "";
				for (int j = 0; j < tvControllers.SelectedNodes.Count; ++j)
					nodeNames += ((TreeNode)tvControllers.SelectedNodes[j]).Text + "\n";
				DialogResult result = MessageBox.Show("Delete the following? \n\n" + nodeNames,"Confirm Delete",
						MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (result == DialogResult.Yes)
				{
					for (int i = 0; i < tvControllers.SelectedNodes.Count; ++i)
					{
						//If this is a root node
						if (((TreeNode)tvControllers.SelectedNodes[i]).Parent == null)
						{
							handleTreeDelete((TreeNode)tvControllers.SelectedNodes[i],!e.Shift);
						}
						//else it must be a controller selected...
						else
						{
							detachControllerFromControllerView((TreeNode)tvControllers.SelectedNodes[i]);
						}
					}
				}
			}
		}

		private void tvMaterials_DragDrop(object sender,DragEventArgs e)
		{
			TreeView tree = (TreeView)sender;
			Point pt = new Point(e.X,e.Y);
			pt = tree.PointToClient(pt);
			TreeNode nodeTarget = tree.GetNodeAt(pt);
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Material":
					if (nodeTarget != null)
					{
						//Are we dropping a material onto a material?
						if (nodeTarget.ImageKey == "Material")
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?","Set Material Template",MessageBoxButtons.YesNo,MessageBoxIcon.Question) == DialogResult.Yes)
							{
								try
								{
									fs = File.Open(nodeSource.Name,FileMode.Open,FileAccess.Read);
								}
								catch (Exception ex)
								{
									Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
									if (fs != null)
										fs.Close();
									return;
								}

								try
								{
									Material newMaterial = bf.Deserialize(fs) as Material;
									newMaterial.Name = nodeTarget.Text;

									//Get the old material, and transfer the proxy to the new material
									
									Material oldMaterial = (Material)m_hashtable[nodeTarget.Name];
									m_hashtable.Remove(nodeTarget.Name);
									m_hashtable.Add(nodeTarget.Name,newMaterial);	
									newMaterial.Proxy = oldMaterial.Proxy;
								}
								catch (Exception exception)
								{
									MessageBox.Show("Error occurred while using material template.  See the message log for details.","Material Template Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
									Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
								}
								fs.Close();
								m_dirty = true;

								//Refresh the selection to fire events
								nodeTarget.TreeView.SelectedNode = null;
								nodeTarget.TreeView.SelectedNode = nodeTarget;
							}
						}
					}
					break;
			}
		}

		private void tvControllers_DragDrop(object sender,DragEventArgs e)
		{
			TreeView tree = (TreeView)sender;
			Point pt = new Point(e.X,e.Y);
			pt = tree.PointToClient(pt);
			TreeNode nodeTarget = tree.GetNodeAt(pt);
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Controller":
					if (nodeTarget != null)
					{
						//Are we dropping a controller onto a controller?
						if (nodeTarget.ImageKey == "Controller")
						{
							if (MessageBox.Show("Change \"" + nodeTarget.Text + "\" to match template \"" + nodeSource.Text + "\"?","Set Controller Template",MessageBoxButtons.YesNo,MessageBoxIcon.Question) == DialogResult.Yes)
							{
								try
								{
									fs = File.Open(nodeSource.Name,FileMode.Open,FileAccess.Read);
								}
								catch (Exception ex)
								{
									Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
									if (fs != null)
										fs.Close();
									return;
								}

								try
								{
									Controller newController = bf.Deserialize(fs) as Controller;
									newController.Name = nodeTarget.Text;
									m_hashtable.Remove(nodeTarget.Name);
									m_hashtable.Add(nodeTarget.Name,newController);
								}
								catch (Exception exception)
								{
									MessageBox.Show("Error occurred while using controller template.  See the message log for details.","Controller Template Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
									Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
								}
								fs.Close();
								m_dirty = true;

								//Refresh the selection to fire events
								nodeTarget.TreeView.SelectedNode = null;
								nodeTarget.TreeView.SelectedNode = nodeTarget;
							}
						}
					}
					break;
			}
		}

		private void tsbMoveUp_Click(object sender,EventArgs e)
		{
			if (tvHierarchy.SelectedNode != null)
			{
				TreeNode node = tvHierarchy.SelectedNode;
				TreeNode parentNode = node.Parent;
				
				//See if the underlying scene node will allow this move operation
				SceneNode sceneNode = m_hashtable[node.Name] as SceneNode;
				if (sceneNode != null)
				{
					if (!sceneNode.Moveable)
						return;
				}

				int index = node.Index;
				if (index == 0)
					return;

				//A non-folder node cannot move between folders
				if (node.Name != "Folder")
				{
					if (parentNode == null)
					{
						if (tvHierarchy.Nodes[index - 1].Name == "Folder")
							return;
					}
					else
					{
						if (parentNode.Nodes[index - 1].Name == "Folder")
							return;
					}
				}

				//A folder cannot move between non folder nodes
				if (node.Name == "Folder")
				{
					if (parentNode == null)
					{
						if (tvHierarchy.Nodes[index - 1].Name != "Folder")
							return;
					}
					else
					{
						if (parentNode.Nodes[index - 1].Name != "Folder")
							return;
					}
				}

				//Cannot displace reserved HDR node
				if (parentNode == null)
				{
					if (tvHierarchy.Nodes[index - 1].Text == "High Dynamic Range")
						return;
				}
				else
				{
					if (parentNode.Nodes[index - 1].Text == "High Dynamic Range")
						return;
				}

				node.Remove();

				--index;
				if (parentNode == null)
					tvHierarchy.Nodes.Insert(index,node);
				else
					parentNode.Nodes.Insert(index,node);
				tvHierarchy.SelectedNode = node;

				//Notify scene node it was moved
				if (sceneNode != null)
					sceneNode.handleMove(true,node);
			}
		}

		private void tsbMoveDown_Click(object sender,EventArgs e)
		{
			if (tvHierarchy.SelectedNode != null)
			{
				TreeNode node = tvHierarchy.SelectedNode;
				TreeNode parentNode = node.Parent;
				
				//See if the underlying scene node will allow this move operation
				SceneNode sceneNode = m_hashtable[node.Name] as SceneNode;
				if (sceneNode != null)
				{
					if (!sceneNode.Moveable)
						return;
				}

				int index = node.Index;

				if (parentNode == null)
				{
					if (index == tvHierarchy.Nodes.Count - 1)
						return;
				}
				else
				{
					if (index == parentNode.Nodes.Count - 1)
						return;
				}

				//A non-folder node cannot move between folders
				if (node.Name != "Folder")
				{
					if (parentNode == null)
					{
						if (tvHierarchy.Nodes[index + 1].Name == "Folder")
							return;
					}
					else
					{
						if (parentNode.Nodes[index + 1].Name == "Folder")
							return;
					}
				}

				//A folder cannot move between non folder nodes
				if (node.Name == "Folder")
				{
					if (parentNode == null)
					{
						if (tvHierarchy.Nodes[index + 1].Name != "Folder")
							return;
					}
					else
					{
						if (parentNode.Nodes[index + 1].Name != "Folder")
							return;
					}
				}

				//Cannot displace reserved HDR node
				if (parentNode == null)
				{
					if (tvHierarchy.Nodes[index + 1].Text == "High Dynamic Range")
						return;
				}
				else
				{
					if (parentNode.Nodes[index + 1].Text == "High Dynamic Range")
						return;
				}

				node.Remove();

				++index;
				if (parentNode == null)
					tvHierarchy.Nodes.Insert(index,node);
				else
					parentNode.Nodes.Insert(index,node);
				tvHierarchy.SelectedNode = node;

				//Notify scene node it was moved
				if (sceneNode != null)
					sceneNode.handleMove(false,node);
			}
		}

		private void tsbNewFolder_Click(object sender,EventArgs e)
		{
			createNewFolder();
		}

		private void createNewFolder()
		{
			//Create a new folder in the current selection if it's a folder
			//otherwise create a new folder at top level
			TreeNode folderNode;
			if ((tvHierarchy.SelectedNode != null) && (tvHierarchy.SelectedNode.Name == "Folder"))
			{
				//Insert the new folder at the end of any previous folders
				int i = 0;
				while (i < tvHierarchy.SelectedNode.Nodes.Count)
				{
					if (tvHierarchy.SelectedNode.Nodes[i].Name != "Folder")
						break;
					++i;
				}
				folderNode = tvHierarchy.SelectedNode.Nodes.Insert(i,"Folder","New Folder","Folder","Folder");
				folderNode.ContextMenuStrip = cmsFolder;
			}
			else
			{
				//Insert the new folder at the end of any previous folders
				int i = 0;
				while (i < tvHierarchy.Nodes.Count)
				{
					if (tvHierarchy.Nodes[i].Name != "Folder")
						break;
					++i;
				}
				folderNode = tvHierarchy.Nodes.Insert(i,"Folder","New Folder","Folder","Folder");
				folderNode.ContextMenuStrip = cmsFolder;
			}
			
			//Select the new folder
			tvHierarchy.SelectedNode = folderNode;
			folderNode.EnsureVisible();
		}

		private void addNewFolderToolStripMenuItem_Click(object sender,EventArgs e)
		{
			createNewFolder();
		}

		private void tvHierarchy_MouseMove(object sender,MouseEventArgs e)
		{
		}

		public void addActor(string actorPath,string actorName,TreeNode targetFolder,float[] transform)
		{
			TreeNode addToFolder = targetFolder;
			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();

			try
			{
				fs = File.Open(actorPath,FileMode.Open,FileAccess.Read);
			}
			catch (Exception ex)
			{
				Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}

			//Create a temporary scene explorer for the actor.
			//We deserialize in this order.  Then we must call fixup 
			//SceneExplorer tempExplorer;// = new SceneExplorer();
			ViewportConfig viewportConfig = null;
			ArrayList nodes = null;
			Hashtable hashtable = null;
			try
			{
				viewportConfig = bf.Deserialize(fs) as ViewportConfig;
				nodes = bf.Deserialize(fs) as ArrayList;
				hashtable = bf.Deserialize(fs) as Hashtable;
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				fs.Close();
				MessageBox.Show("Could not import actor.  Check the message log for details.","Import Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			//tempExplorer.postDeserializeFixup();

			//Start by replacing all the GUIDs with new ones
			//TreeView hierarchy = tempExplorer.getHierarchyTreeView();
			//Hashtable newHashtable = tempExplorer.Hashtable;

			//First, we want to apply new GUIDs to those tree nodes that don't reference
			//scene nodes in the hash table..they must also be unique for ID purposes.
			Program.g_mainForm.initProgressBar("Generating new GUIDs...",nodes.Count);
			foreach (TreeNode node in nodes)
			{
				applyNewGUIDsRecursive(node,hashtable);
				Program.g_mainForm.MainProgressBar.PerformStep();
			}


			//Second, look at the tree nodes that do reference scene nodes in the hashtable
			//and apply new GUIDs.
			Program.g_mainForm.initProgressBar("Preparing new GUIDs...",hashtable.Keys.Count);
			foreach (string key in hashtable.Keys)
			{
				//Add the hashtable entry to the main hashtable with a brand new GUID
				string newGUID = System.Guid.NewGuid().ToString();
				m_hashtable.Add(newGUID,hashtable[key]);

				//Find all the tree nodes that referenced this hashtable entry and give
				//them the new GUID as well.
				foreach (TreeNode treeNode in nodes)
				{
					//Check top level node
					if (treeNode.Name == key)
						treeNode.Name = newGUID;

					//Check children
					TreeNode[] matchingNodes = treeNode.Nodes.Find(key,true);
					for (int i = 0; i < matchingNodes.Length; ++i)
						matchingNodes[i].Name = newGUID;
				}
				Program.g_mainForm.MainProgressBar.PerformStep();

			}

			//Next, clone the tree nodes in the temporary explorer and add them to the
			//main explorer...remember they now have new GUIDs so no conflict should arise
			Program.g_mainForm.initProgressBar("Adding trees...",nodes.Count);

			if (m_createActorGroup)
			{
				//Create a folder for the actor container
				TreeNode actorNode;
				string acGuid = System.Guid.NewGuid().ToString();
				if (addToFolder != null)
					actorNode = addToFolder.Nodes.Add(acGuid,actorName /*+ "_" + m_nextIndex.ToString()*/,"Actor","Actor");
				else
					actorNode = tvHierarchy.Nodes.Add(acGuid,actorName /*+ "_" + m_nextIndex.ToString()*/,"Actor","Actor");
				//m_nextIndex++;
				actorNode.Tag = new ActorInfo(actorPath,DateTime.Now);
				
				//Create an actor container proxy 
				Objects.ActorContainer actorContainer = new Objects.ActorContainer("ActorContainer");
				actorContainer.createProxyInstance(m_XVPMHandle);
				addNode(acGuid,actorContainer);

				//Set the transform if one was passed in
				if (transform != null)
				{
					actorContainer.Transform = transform;
					actorContainer.downSyncTransform();
				}
			
				foreach (TreeNode node in nodes)
				{
					actorNode.Nodes.Add(node);

					//Add proxy instances recursively
					createProxyInstanceRecursive(node);

					//Set actor container recursively
					setActorContainerRecursive(node,actorContainer);

					Program.g_mainForm.MainProgressBar.PerformStep();
				}
			}
			else
			{
				TreeNodeCollection parentNodes;
				if (addToFolder != null)
					parentNodes = addToFolder.Nodes;
				else
					parentNodes = tvHierarchy.Nodes;

				foreach (TreeNode node in nodes)
				{
					parentNodes.Add(node);

					//Add proxy instances recursively
					createProxyInstanceRecursive(node);
					Program.g_mainForm.MainProgressBar.PerformStep();
				}
			}

			postDeserializeFixup();

			//Then create controller proxy instances for all controllers
			foreach (TreeNode node in getControllerTreeView().Nodes)
			{
				foreach (TreeNode controllerNode in node.Nodes)
				{
					Controller controller = m_hashtable[controllerNode.Name] as Controller;

					//If a proxy instance has already been created for this controller, don't
					//create another
					if (controller.Proxy == IntPtr.Zero)
						Program.g_mainForm.PluginMgr.createProxyInstance(controller.PluginID,this,controllerNode);
				}
			}

			//Attach controllers to the necessary nodes
			foreach (TreeNode controllerGroupNode in getControllerTreeView().Nodes)
			{
				//Find the matching hierarchy node
				TreeNode[] matchingHierarchyNode = getHierarchyTreeView().Nodes.Find(controllerGroupNode.Name,true);
				if (matchingHierarchyNode.Length != 1)
					throw new Exception("Expected to find exactly one hierarchy node.");

				foreach (TreeNode controllerNode in controllerGroupNode.Nodes)
				{
					TreeNode[] controllerNodes = matchingHierarchyNode[0].Nodes.Find(controllerNode.Name,true);
					for (int i = 0; i < controllerNodes.Length; ++i)
					{
						//Grab the root hierarchy node so we can send the attach notification to the particular plugin
						//that has the controller attached
						SceneNode sceneNode = m_hashtable[controllerGroupNode.Name] as SceneNode;
						Controller controller = m_hashtable[controllerNodes[i].Name] as Controller;
						if (sceneNode == null || controller == null)
							throw new Exception("Expected a valid scene node and controller");
						sceneNode.attachController(controller.Proxy,controllerNodes[i].Parent);
					}
				}
			}

			Program.g_mainForm.initProgressBar("Done.",nodes.Count);

			m_dirty = true;

			fs.Close();
		}

		private void btnShowInShaderBuilder_Click(object sender,EventArgs e)
		{
			if (tscbShader.SelectedItem != null)
				Program.g_mainForm.showShaderBuilder(tscbShader.SelectedItem.ToString());
			else
				Program.g_mainForm.showShaderBuilder(null);
		}

		private void cbScene_SelectedIndexChanged(object sender,EventArgs e)
		{
			
		}

		private void cbScene_Click(object sender,EventArgs e)
		{

		}

		private void SceneChange(object sender,EventArgs e)
		{
			PopupItem pi = sender as PopupItem;
			//for each selected plugin root...
			foreach (TreeNode node in tvHierarchy.SelectedNodes)
			{
				//Make sure this node is the root of a plugin instance...
				if (isPluginRoot(node))
				{
					SceneNode sceneNode = (SceneNode)m_hashtable[node.Name];
					Program.g_mainForm.PluginMgr.setScene(sceneNode.PluginID,pi.Text,this,node);
				}
			}
			btnSceneSelection.Text = pi.Text;
		}
    }
}
