using System;
using System.Collections.Generic;
using System.Text;
using XVPDesigner.UserControls;
using System.Windows.Forms;
using System.ComponentModel;
using XVPManaged;

namespace XVPDesigner.Objects
{
	[Serializable]
	public class BasePostEffect : SceneNode
	{
		public BasePostEffect(string name)
			: base(name)
		{
		}

		protected override void createExtendedContextMenu()
		{
			m_contextMenu.MenuItems.Add("Remove",new EventHandler(handleContextMenu));
		}

		override protected void handleContextMenu(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;
			TreeNode selectedNode = scene.getHierarchyTreeView().SelectedNode;

			if (item == "Remove")
			{
				//Destroy proxy instance
				SceneNode sceneNode = scene.getNode(selectedNode.Name) as SceneNode;
				if (sceneNode != null)
					Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,scene,selectedNode);

				//Remove materials from the material view
				scene.removeMaterials(selectedNode,false);

				//Remove post effect node 
				scene.removeNode(selectedNode.Name);

				//Remove nodes from tree view
				selectedNode.Remove();
			}

			base.handleContextMenu(sender,e);

		}

		public override void handleMove(bool up,TreeNode node)
		{
			//Get the camera node which is exactly two levels up
			CameraProvider cameraProvider = Program.g_mainForm.ActiveSceneExplorer.getNode(node.Parent.Parent.Name) as CameraProvider;
			if (cameraProvider == null)
				throw new Exception("Expected camera provider to be exactly two levels up from current node");

			//Remove all the post effects
			cameraProvider.removeAllPostEffects();

			//Now, add them back in their new order
			foreach (TreeNode childNode in node.Parent.Nodes)
			{
				BasePostEffect postEffect = Program.g_mainForm.ActiveSceneExplorer.getNode(childNode.Name) as BasePostEffect;
				if (postEffect != null)
				{
					cameraProvider.addPostEffect(postEffect);
				}
			}
		}
	}
}
