using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.Objects;
using System.Collections;

namespace XVPDesigner.UserControls
{
    public partial class PropertyExplorer : UserControl
    {
		public object SelectedObject
		{
			get { return pgridNodeProperties.SelectedObject; }
		}
		
		public GridItem SelectedGridItem
		{
			get { return pgridNodeProperties.SelectedGridItem; }
		}

		private bool m_dirty = false;
		public bool Dirty
		{
			get { return m_dirty; }
			set { m_dirty = value; }
		}
			
		public PropertyExplorer()
        {
            InitializeComponent();
        }

		/*public void setNoder(SceneNode node)
		{
			//Set the property object to the property grid
			//this.pgridNodeProperties.SelectedObject = node;
			SceneNode[] nodes = null;
			if (node != null)
				nodes = new SceneNode[1] { node };

			this.pgridNodeProperties.SelectedObjects = nodes;
		}*/

		public void setNodes(ArrayList nodes)
		{
			//Set the property object to the property grid
			if (nodes == null)
				this.pgridNodeProperties.SelectedObjects = null;
			else
				this.pgridNodeProperties.SelectedObjects = nodes.ToArray();
		}

		public ArrayList getNodes()
		{
			//Set the property object to the property grid
			if (pgridNodeProperties.SelectedObjects == null)
				return null;
			else
				return ArrayList.Adapter(pgridNodeProperties.SelectedObjects);
		}
		
		private void pgridNodeProperties_PropertyValueChanged(object s,PropertyValueChangedEventArgs e)
		{
			string selectedNodeName = null;
			string parentNodeName = null;
			string propertyName = null;
			
			//Mark the explorer as dirty
			m_dirty = true;

			SceneExplorer explorer = (SceneExplorer)Program.g_mainForm.ActiveSceneExplorer;
			string tabText = explorer.getSelectedTabText();
			switch (tabText)
			{
				case "Hierarchy":
					selectedNodeName = explorer.getHierarchyTreeView().SelectedNode.Text;
					parentNodeName = explorer.findRootOfSelectedHierarchyNode().Text;
					break;
				case "Materials":
					selectedNodeName = explorer.getMaterialTreeView().SelectedNode.Text;
					parentNodeName = explorer.findRootOfSelectedMaterialsNode().Text;		
					break;
				case "Controllers":
					selectedNodeName = explorer.getHierarchyTreeView().SelectedNode.Text;
					parentNodeName = explorer.findRootOfSelectedControllersNode().Text;		
					break;
				default:
					throw new Exception("No case for handling tab.");
			}
				
			//Determing property name and value
			propertyName = e.ChangedItem.Label;
		
			//Notify object 
			SceneNode sceneNode = (SceneNode)((PropertyGrid)s).SelectedObject;
			sceneNode.handlePropertyChange(parentNodeName,selectedNodeName,propertyName,e.ChangedItem.Value);
/*
			MessageBox.Show(e.ChangedItem.Label);
			MessageBox.Show(e.ChangedItem.Value.ToString());

			SceneNode sceneNode = (SceneNode)((PropertyGrid)s).SelectedObject;*/
			
		}

		private void pgridNodeProperties_SelectedObjectsChanged(object sender,EventArgs e)
		{
			//If the selected node is a material, refresh the texture browser,
			//otherwise clear it
			Material material = ((PropertyGrid)sender).SelectedObject as Material;
			if (material != null)
			{
				Material.TextureNameList texList = material.getTextureNameList();
				Program.g_mainForm.refreshTextureBrowser(texList);
			}
			else
				Program.g_mainForm.clearTextureBrowser();
		}

		public SceneNode getSelectedNode()
		{
			return pgridNodeProperties.SelectedObject as SceneNode;
		}

		private void pgridNodeProperties_EnabledChanged(object sender,EventArgs e)
		{
			if (!pgridNodeProperties.Enabled)
				pgridNodeProperties.ViewBackColor = SystemColors.Control;
			else
				pgridNodeProperties.ViewBackColor = Color.White;
		}

		private void pgridNodeProperties_DragOver(object sender,DragEventArgs e)
		{
			PropertyGrid pgrid = (PropertyGrid)sender;
			e.Effect = DragDropEffects.None;
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			if (nodeSource != null)
			{
				switch (nodeSource.ImageKey)
				{
					case "Texture":
						e.Effect = DragDropEffects.Copy;
						break;
				}
			}
		}

		private void pgridNodeProperties_DragDrop(object sender,DragEventArgs e)
		{
			PropertyGrid pgrid = (PropertyGrid)sender;
			Point pt = new Point(e.X,e.Y);
			pt = pgrid.PointToClient(pt);
			object o = pgrid.GetChildAtPoint(pt);
						
		}

		
    }
}
