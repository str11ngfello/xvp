using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using XVPDesigner.UserControls;
using System.IO;

namespace XVPDesigner.Objects
{
	[Serializable]
	public abstract class SceneNode
	{
		//Pointer to proxy object
		[NonSerialized]
		protected IntPtr m_proxy;
		[Browsable(false)]
		public IntPtr Proxy
		{
			get { return m_proxy; }
			set
			{
				//Only apply the values if the proxy object is new.  This
				//prevents multiple sets of the same properties.  
				if (m_proxy != value)
				{
					m_proxy = value;
					setCurrentValues();
				}
			}
		}

		private string m_pluginID = "NO_PLUGIN_ID";
		[DisplayName("Plugin ID"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("Plugin ID used to match the scene node to the plugin type that owns it."),
		ReadOnlyAttribute(true)]
		public string PluginID
		{
			get { return m_pluginID; }
			set { m_pluginID = value; }
		}

		private string m_name = "";
		[DisplayName("Name"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("Name."),
		ReadOnlyAttribute(true)]
		public string Name
		{
			get { return m_name; }
			set { if (Renameable) m_name = value; }
		}
	
		private bool m_acceptsControllers = false;
		[DisplayName("Accepts Controllers"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("Determines whether or not this node will accept controller attachments."),
		ReadOnlyAttribute(true)]
		public bool AcceptsControllers
		{
			get { return m_acceptsControllers; }
			set { m_acceptsControllers = value; }
		}

		private bool m_renameable = true;
		[DisplayName("Renameable"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("Determines whether or not this node can have its name changed."),
		ReadOnlyAttribute(true)]
		public bool Renameable
		{
			get { return m_renameable; }
			set 
			{
				PopupMenu.MenuItems.Find("RenameSceneNode",false)[0].Enabled = value;
				m_renameable = value; 
			} 
		}

		private bool m_moveable = true;
		[DisplayName("Moveable"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("Determines whether or not this node can be moved withing the treeview."),
		ReadOnlyAttribute(true)]
		public bool Moveable
		{
			get { return m_moveable; }
			set { m_moveable = value; }
		}
		
		[NonSerialized]
		protected ContextMenu m_contextMenu;
		[CategoryAttribute("_Reserved"),
		Browsable(false)]
		virtual public ContextMenu PopupMenu
		{
			get 
			{
				if (m_contextMenu == null)
				{
					createBaseContextMenu();
					createExtendedContextMenu();
				}
				return m_contextMenu; 
			}
			set { m_contextMenu = value; }
		}

		private string m_sceneName = "Main";
		[DisplayName("Scene"),
		CategoryAttribute("_Reserved"),
		DescriptionAttribute("The scene that contains this instance."),
		ReadOnlyAttribute(true)]
		public string SceneName
		{
			get { return m_sceneName; }
			set { m_sceneName = value; }
		}

		public SceneNode(string name)
		{
			m_name = name;
		}

		public virtual void Dispose()
		{
		}

		private void createBaseContextMenu()
		{
			m_contextMenu = new ContextMenu();
			MenuItem mi = new MenuItem("Rename",new EventHandler(handleContextMenu));
			mi.Name = "RenameSceneNode";
			m_contextMenu.MenuItems.Add(mi);
			m_contextMenu.MenuItems.Add("-");
			mi = new MenuItem("Expand All Children",new EventHandler(handleContextMenu));
			mi.Name = "ExpandAllChildren";
			m_contextMenu.MenuItems.Add(mi);
			mi = new MenuItem("Collapse All Children",new EventHandler(handleContextMenu));
			mi.Name = "CollapseAllChildren";
			m_contextMenu.MenuItems.Add(mi);
			m_contextMenu.MenuItems.Add("-");
			mi = new MenuItem("Reset to Default Values",new EventHandler(handleContextMenu));
			mi.Name = "ResetToDefaultValues";
			m_contextMenu.MenuItems.Add(mi);
			mi = new MenuItem("Show GUID",new EventHandler(handleContextMenu));
			mi.Name = "ShowGUID";
			m_contextMenu.MenuItems.Add(mi);
			mi = new MenuItem("Copy GUID to Clipboard",new EventHandler(handleContextMenu));
			mi.Name = "CopyGUIDToClipboard";
			m_contextMenu.MenuItems.Add(mi);
			m_contextMenu.MenuItems.Add("-");


		}

		protected virtual void createExtendedContextMenu()
		{
		}

		protected virtual void handleContextMenu(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;
			TreeNode selectedNode = scene.getHierarchyTreeView().SelectedNode;

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
			if (item == "Reset to Default Values")
			{
				SceneNode sceneNode = scene.Hashtable[selectedNode.Name] as SceneNode;
				if (sceneNode != null)
				{
					sceneNode.setDefaultValues();

					//Refresh the property grid by selected the same node already selected
					scene.getHierarchyTreeView().SelectedNode = null;
					scene.getHierarchyTreeView().SelectedNode = selectedNode;
				}
			}
			if (item == "Show GUID")
			{
				MessageBox.Show("Name: " + selectedNode.Text + "\nGUID: " + selectedNode.Name,"GUID",MessageBoxButtons.OK,MessageBoxIcon.Information);
			}
			if (item == "Copy GUID to Clipboard")
			{
				Clipboard.SetText(selectedNode.Name);
			}
		}

		public virtual void handlePropertyChange(string parentName,string selectedNodeName,string propName,object value)
		{
		}

		public virtual void export(BinaryWriter bw)
		{
		}

		public virtual void setDefaultValues()
		{
		}

		public virtual void setCurrentValues()
		{
		}

		public virtual void postDeserializeRangeCheck()
		{
		}

		public virtual void handleMove(bool up,TreeNode node)
		{
		}

		public virtual void attachController(IntPtr controllerProxy,TreeNode nodeToAttachTo)
		{
		}

		public virtual void detachController(IntPtr controllerProxy,TreeNode nodeToDetachFrom)
		{
		}
	}
}
