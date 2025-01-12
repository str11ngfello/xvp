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
	public partial class StageExplorer : UserControl
    {
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
	
        public StageExplorer()
        {
            InitializeComponent();

			tvHierarchy.ImageList = Program.g_mainForm.Icons;
        }

		public StageExplorer(SerializationInfo info,StreamingContext context): this() 
		{
			SerializationInfoEnumerator infoEnumerator = info.GetEnumerator();		

			while (infoEnumerator.MoveNext()) 
			{
				TreeNode node = info.GetValue(infoEnumerator.Name, infoEnumerator.ObjectType) as TreeNode;

				//Add the tree assuming hashtable of scenenodes already exists by passing null for
				//second param
				addTree(node);
			}			
		}

		public TreeNode findRootOfSelectedHierarchyNode()
		{
			//If there is no selected node, we can't continue
			if (tvHierarchy.SelectedNode == null)
				return null;

			TreeNode parentNode = tvHierarchy.SelectedNode;
			while (true)
			{
				if (parentNode.Parent == null)
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
				else
					parentNode = parentNode.Parent;
			}

			return parentNode;
		}

		private void tvHierarchy_AfterSelect(object sender,TreeViewEventArgs e)
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
		}

		//Adds a new tree to the scene explorer tree views assuming the data base nodes
		//have already been added.
		public bool addTree(TreeNode treeNode)
		{
			//Validate data
			if (treeNode.Text == "" || treeNode.Name == "")
				throw new Exception("Cannot add tree with no GUID or name");
			//if (m_hashtable.Contains(treeNode.Name))
			//	throw new Exception("A tree node with this GUID already exists in the hashtable.");

			//Add the tree to the treeview
			tvHierarchy.Nodes.Add(treeNode);
				
			//Hookup root context menu
			hookupRootContextMenu(treeNode);

			//Hookup sub node context menus
			hookupContextMenusRecursive(treeNode);
				
			m_dirty = true;

			return true;
		}

		public void removeTree(TreeNode treeNode)
		{
			//Find the treenode that matches this tree node in the Hierarchy view.
			TreeNode[] foundNodes = tvHierarchy.Nodes.Find(treeNode.Name,false);
			if (foundNodes.Length != 1)
			{
				throw new Exception("While removing tree, did not find the root node in Hierarchy view.");
			}

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
				if (treeNode.Parent == null)
				{
					MenuItem mi = new MenuItem("Delete",new EventHandler(hierarchyViewPopup));
					mi.Name = "DeleteSceneNode";
					treeNode.ContextMenu.MenuItems.Add(0,mi);
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

		public bool nodeNameExists(string name)
		{
			TreeNode[] matchingNodes = tvHierarchy.Nodes.Find(name,true);
			if (matchingNodes.Length > 0)
				return true;
	
			return false;
		}

		private void tvHierarchy_AfterLabelEdit(object sender,NodeLabelEditEventArgs e)
		{
			//Can't rename a lable to nothing and or a label didn't change
			if (e.Label == "" || e.Label == null)
			{
				e.CancelEdit = true;
				return;
			}

			if (!Utility.validateLabel(e.Label))
			{
				MessageBox.Show("The label/string \"" + e.Label + "\" is reserved.  Use a different label/string to avoid conflicts.","Label/String Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
				return;
			}

			//We can't rename the node if the scene node isn't renameable
			SceneNode sceneNode = (SceneNode)m_hashtable[e.Node.Name];
			if (sceneNode != null)
			{
				if (!sceneNode.Renameable)
				{
					e.CancelEdit = true;
					return;
				}
				else
					sceneNode.Name = e.Label;
			}
			
			e.CancelEdit = false;
			Dirty = true;
		}
	
		private void tvHierarchy_MouseDown(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				//Select the clicked node
				tvHierarchy.SelectedNode = tvHierarchy.GetNodeAt(e.X,e.Y);
			}
		}

		private void tvHierarchy_KeyDown(object sender,KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Delete)
			{
			}
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
				case "Actor":
					string filename = nodeSource.Name;
					try
					{
						fs = File.Open(filename, FileMode.Open, FileAccess.Read);
					}
					catch (Exception ex)
					{
						Program.g_mainForm.logMessage(ex.ToString(), Color.Red);
						if (fs != null)
							fs.Close();
						return;
					}

					//Create a temporary scene explorer for the actor.
					//We deserialize in this order.  Then we must call fixup 
					//StageExplorer tempExplorer;// = new StageExplorer();
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
						Program.g_mainForm.logMessage(exception.ToString(), Color.Red);
						fs.Close();
						MessageBox.Show("Could not import actor.  Check the message log for details.", "Import Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}

					//First, we want to apply new GUIDs to those tree nodes that don't reference
					//scene nodes in the hash table..they must also be unique for ID purposes.
					Program.g_mainForm.initProgressBar("Generating new GUIDs...",nodes.Count);
					foreach (TreeNode node in nodes)
					{
						applyNewGUIDsRecursive(node, hashtable);
						Program.g_mainForm.MainProgressBar.PerformStep();
					}

					//Second, look at the tree nodes that do reference scene nodes in the hashtable
					//and apply new GUIDs.
					Program.g_mainForm.initProgressBar("Preparing new GUIDs...", hashtable.Keys.Count);
					foreach (string key in hashtable.Keys)
					{
						//Add the hashtable entry to the main hashtable with a brand new GUID
						string newGUID = System.Guid.NewGuid().ToString();
						m_hashtable.Add(newGUID, hashtable[key]);

						//Find all the tree nodes that referenced this hashtable entry and give
						//them the new GUID as well.
						foreach (TreeNode treeNode in nodes)
						{
							//Check top level node
							if (treeNode.Name == key)
								treeNode.Name = newGUID;

							//Check children
							TreeNode[] matchingNodes = treeNode.Nodes.Find(key, true);
							for (int i = 0; i < matchingNodes.Length; ++i)
								matchingNodes[i].Name = newGUID;
						}
						Program.g_mainForm.MainProgressBar.PerformStep();

					}

					//Next, clone the tree nodes in the temporary explorer and add them to the
					//main explorer...remember they now have new GUIDs so no conflict should arise
					Program.g_mainForm.initProgressBar("Adding trees...", nodes.Count);
					foreach (TreeNode node in nodes)
					{
						tvHierarchy.Nodes.Add(node);
						Program.g_mainForm.MainProgressBar.PerformStep();
					}
					
					postDeserializeFixup();
					
					Program.g_mainForm.initProgressBar("Done.",nodes.Count);

					m_dirty = true;

					fs.Close();
					break;
					
			}
		}

		private void applyNewGUIDsRecursive(TreeNode node,Hashtable hashtable)
		{
			SceneNode sceneNode = (SceneNode)hashtable[node.Name];
			if (sceneNode == null)
				node.Name = System.Guid.NewGuid().ToString();

			foreach (TreeNode childNode in node.Nodes)
				applyNewGUIDsRecursive(childNode,hashtable);
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
			m_hashtable.Clear();
			
			m_dirty = false;
		}

		public void setNodeArray(ArrayList nodes)
		{
			for (int i = 0; i < nodes.Count; ++i)
			{
				addTree((TreeNode)nodes[i]);
			}
		}
		public ArrayList getNodeArray()
		{
			ArrayList al = new ArrayList();
			foreach (TreeNode node in tvHierarchy.Nodes)
				al.Add(node);
			return al;
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
			}
			else
			{
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
		
		private void tsbExpandAllHierarchyNodes_Click(object sender,EventArgs e)
		{
			tvHierarchy.ExpandAll();
		}

		private void tsbCollapseAllHierarchyNodes_Click(object sender,EventArgs e)
		{
			tvHierarchy.CollapseAll();
		}

		private void tvHierarchy_EnabledChanged(object sender,EventArgs e)
		{
			if (!tvHierarchy.Enabled)
				tvHierarchy.BackColor = SystemColors.Control;
			else
				tvHierarchy.BackColor = Color.White;
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
    }
}
