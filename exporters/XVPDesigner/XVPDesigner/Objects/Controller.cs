using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using System.IO;
using System.Drawing;
using System.Runtime.Serialization.Formatters.Binary;
using System.ComponentModel;
using System.Collections;

namespace XVPDesigner.Objects
{
	[Serializable]
	public class Controller : SceneNode
	{
		[NonSerialized]
		private uint m_XVPMHandle = 0xFFFFFFFF;
		[Browsable(false)]
		public uint XVPMHandle
		{
			get { return m_XVPMHandle; }
			set { m_XVPMHandle = value; }
		}

		public Controller(string name) : base(name)
		{
			PluginID = "Controller";
			AcceptsControllers = false;
		}

		protected override void createExtendedContextMenu()
		{

			m_contextMenu.MenuItems.Add("Show in Controller View",new EventHandler(handleContextMenu));
			m_contextMenu.MenuItems.Add("Save in Controller Library",new EventHandler(handleContextMenu));
			m_contextMenu.MenuItems.Add("-",new EventHandler(handleContextMenu));
			m_contextMenu.MenuItems.Add("Detach",new EventHandler(handleContextMenu));
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

			if (item == "Detach")
			{
				scene.detachControllerFromHierarchyViewNode(selectedNode);
			}
			else if (item == "Show in Controller View")
			{
				TreeNode[] nodes = scene.getControllerTreeView().Nodes.Find(selectedNode.Name,true);
				if (nodes.Length != 0)
				{
					nodes[0].EnsureVisible();
					scene.getControllerTreeView().SelectedNode = nodes[0];
					scene.showControllerTab();
				}
			}
			else if (item == "Save in Controller Library")
			{
				SaveFileDialog sfd = new SaveFileDialog();
				sfd.Title = "Save Controller...";
				sfd.Filter = "Controller Files (*.con)|*.con|All files (*.*)|*.*";
				sfd.FileName = "Controller.con";
				sfd.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ControllerLibraryPath;
				sfd.FileName = Name;
				DialogResult result = sfd.ShowDialog();
				if (result == DialogResult.OK)
				{
					FileStream fs = null;
					try
					{
						fs = File.Open(sfd.FileName,FileMode.Create);
					}
					catch (Exception exception)
					{
						MessageBox.Show("Cannot open \"" + sfd.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
						Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
						return;
					}
					BinaryFormatter bf = new BinaryFormatter();

					bf.Serialize(fs,this);
					fs.Close();
				}
			}

			base.handleContextMenu(sender,e);

		}
	}
}
