using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.Objects;

namespace XVPDesigner.Dialogs
{
	public partial class AttachControllerDialog : Form
	{
		private Color disabledColor = SystemColors.GrayText;
		public string m_controllerType;
		public ArrayList m_selectedNodeGUIDs = new ArrayList(10);

		public AttachControllerDialog()
		{
			InitializeComponent();
		}

		public void init(string controllerType,TreeNode selectedNode)
		{
			tvSceneNodes.ImageList = Program.g_mainForm.Icons;
			m_controllerType = controllerType;
			lblType.Text = controllerType;
			txtName.Text = controllerType;

			//Clear selected node GUIDs
			m_selectedNodeGUIDs.Clear();

			//Add the nodes to our tree view for selection
			TreeNode newNode = (TreeNode)selectedNode.Clone();
			tvSceneNodes.Nodes.Add(newNode);		
			
			//Now remove those nodes that can't accept controller attachments
			ArrayList nodesToRemove = new ArrayList();
			filterAttachNodes(newNode,nodesToRemove,selectedNode.Text);
			foreach (TreeNode nodeToRemove in nodesToRemove)
			{
				//Don't show pre-existing controllers or materials
				if ((nodeToRemove.ImageKey == "Controller") || (nodeToRemove.ImageKey == "Material"))
					tvSceneNodes.Nodes.Remove(nodeToRemove);
				else
				{
					//The color is used to signal that a node cannot recieve controller attachments
					nodeToRemove.ForeColor = disabledColor;//tvSceneNodes.Nodes.Remove(nodeToRemove);
					nodeToRemove.ToolTipText = "This node does not accept controller attachments.";
				
				}
			}

		}

		private void filterAttachNodes(TreeNode tn,ArrayList nodesToRemove,string rootNodeName)
		{
			foreach (TreeNode node in tn.Nodes)
			{
				SceneNode sceneNode = Program.g_mainForm.ActiveSceneExplorer.getNode(node.Name);
				if (sceneNode != null)
				{
					//If this node does not accept controllers, remove it and return.
					if (!sceneNode.AcceptsControllers)
						nodesToRemove.Add(node);// tn.Nodes.Remove(node);				
				}

				filterAttachNodes(node,nodesToRemove,rootNodeName);
			}
		}

		private void btnCancel_Click(object sender,EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Close();
		}

		private void btnAttach_Click(object sender,EventArgs e)
		{
			//Validate name
			if (txtName.Text == "")
			{
				MessageBox.Show("Enter a valid name for this controller.","Incorrect Information",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			//Make sure there are nodes selected
			if (lbSelectedNodes.Items.Count == 0)
			{
				MessageBox.Show("You must select at least one node in order to attach the controller.","Incorrect Information",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			DialogResult = DialogResult.OK;
			Close();
		}


		private void checkAllChildNodes(TreeNode treeNode,bool nodeChecked)
		{
			foreach (TreeNode node in treeNode.Nodes)
			{
				//If already checked or unchecked, don't reset it so a redundant event
				//doesn't fire
				if (node.Checked != nodeChecked)
					node.Checked = nodeChecked;
				if (node.Nodes.Count > 0)
				{
					// If the current node has child nodes, call the checkAllChildsNodes method recursively.
					this.checkAllChildNodes(node,nodeChecked);
				}
			}
		}

		private void tvSceneNodes_AfterCheck(object sender,TreeViewEventArgs e)
		{
			if (e.Node.ForeColor == disabledColor)
			{
				e.Node.Checked = false;
				return;
			}

			if (e.Node.Checked)
			{
				//Only add once
				if (!lbSelectedNodes.Items.Contains(e.Node.Text))
				{
					lbSelectedNodes.Items.Add(e.Node.Text);
					m_selectedNodeGUIDs.Add(e.Node.Name);
				}
			}
			else
			{
				lbSelectedNodes.Items.Remove(e.Node.Text);
				m_selectedNodeGUIDs.Remove(e.Node.Name);
			}
		}

		private void btnDeselectDescendants_Click(object sender,EventArgs e)
		{
			if (tvSceneNodes.SelectedNode != null)
			{
				if (tvSceneNodes.SelectedNode.Checked)
					tvSceneNodes.SelectedNode.Checked = false;
				this.checkAllChildNodes(tvSceneNodes.SelectedNode,false);
			}
		}

		private void btnSelectDescendants_Click(object sender,EventArgs e)
		{
			if (tvSceneNodes.SelectedNode != null)
			{
				if (!tvSceneNodes.SelectedNode.Checked)
					tvSceneNodes.SelectedNode.Checked = true;
				this.checkAllChildNodes(tvSceneNodes.SelectedNode,true);
			}
		}

		private void btnSelectAll_Click(object sender,EventArgs e)
		{
			foreach (TreeNode node in tvSceneNodes.Nodes)
			{
				if (!node.Checked)
					node.Checked = true;
				this.checkAllChildNodes(node,true);
			}
		}

		private void btnDeselectAll_Click(object sender,EventArgs e)
		{
			foreach (TreeNode node in tvSceneNodes.Nodes)
			{
				if (node.Checked)
					node.Checked = false;
				this.checkAllChildNodes(node,false);
			}
		}

		private void tvSceneNodes_BeforeCheck(object sender,TreeViewCancelEventArgs e)
		{
			if (e.Node.ForeColor == disabledColor)
				e.Cancel = true;
		}




	}
}