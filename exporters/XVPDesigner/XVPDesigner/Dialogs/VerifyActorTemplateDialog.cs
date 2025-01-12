using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using XVPDesigner;
using System.IO;
using XVPDesigner.UserControls;
using XVPDesigner.Objects;

namespace Dialogs
{
	public partial class VerifyActorTemplateDialog : Form
	{
		private bool m_valid = true;

		public VerifyActorTemplateDialog()
		{
			InitializeComponent();

			/*//Scan directory for actor templates...
			foreach (string filename in Directory.GetFiles(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath))
			{
				cbActorTemplate.Items.Add(Path.GetFileNameWithoutExtension(filename));
			}*/
		}

		public void verify(string actorTemplate,SceneExplorer sceneExplorer)
		{
			FileStream fs = null;
			TextReader tr = null;
			try
			{
				fs = File.Open(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath + "\\" + actorTemplate + ".acttemp",FileMode.Open);
				tr = new StreamReader(fs);
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not load actor template \"" + actorTemplate + "\"",Color.Red);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (tr != null)
					tr.Close();
				if (fs != null)
					fs.Close();
			}

			string line = null;
			while ((line = tr.ReadLine()) != null)
			{
				string[] parts = line.Split(':');
				ListViewItem lvi = lvActorComponents.Items.Add(parts[0]);
				lvi.SubItems.Add(parts[1]);
				lvi.SubItems.Add(parts[2]);
			}

			tr.Close();
			fs.Close();

			//Now verify each component exists in the scene explorer
			m_valid = true;
			foreach (ListViewItem lvi in lvActorComponents.Items)
			{
				bool found = false;
				foreach (TreeNode node in sceneExplorer.getHierarchyTreeView().Nodes)
				{
					SceneNode sceneNode = sceneExplorer.getNode(node.Name);
					if (sceneNode != null)
					{
						if ((node.Text == lvi.SubItems[0].Text) && 
							(sceneNode.PluginID == lvi.SubItems[1].Text))
						{
							found = true;
							break;
						}
					}
				}

				if (found)
					lvi.BackColor = Color.FromArgb(128,255,128);
				else
				{
					lvi.BackColor = Color.FromArgb(255,128,128);
					lvi.Checked = true;
					m_valid = false;
				}
			}

			//If we didn't pass the verification offer to fix the scene
			if (m_valid)
			{
				btnFinish.Text = "OK";
				btnDoNotFix.Visible = false;
			}
			else
			{
				btnFinish.Text = "Fix";
			}
		}

		public static bool silentVerify(string actorTemplate,SceneExplorer sceneExplorer)
		{
			FileStream fs = null;
			TextReader tr = null;
			try
			{
				fs = File.Open(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath + "\\" + actorTemplate + ".acttemp",FileMode.Open);
				tr = new StreamReader(fs);
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not load actor template \"" + actorTemplate + "\"",Color.Red);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (tr != null)
					tr.Close();
				if (fs != null)
					fs.Close();
			}

			string line = null;
			bool valid = true;
			while ((line = tr.ReadLine()) != null)
			{
				string[] parts = line.Split(':');
				
				bool found = false;
				foreach (TreeNode node in sceneExplorer.getHierarchyTreeView().Nodes)
				{
					SceneNode sceneNode = sceneExplorer.getNode(node.Name);
					if (sceneNode != null)
					{
						if ((node.Text == parts[0]) && 
							(sceneNode.PluginID == parts[1]))
						{
							found = true;
							break;
						}
					}
				}

				if (!found)
				{
					Program.g_mainForm.logMessage("Missing template requirement -> Name: " + parts[0] + " Plugin: " + parts[1],Color.Red);
					valid = false;
				}
			}

			tr.Close();
			fs.Close();

			return valid;
		}

		private void btnFinish_Click(object sender,EventArgs e)
		{
			//If this scene was not valid, fix it
			foreach (ListViewItem lvi in lvActorComponents.Items)
			{
				if (lvi.Checked && lvi.BackColor == Color.FromArgb(255,128,128))
				{
					TreeNode topLevelNode = Program.g_mainForm.PluginMgr.createSceneNodeInstance(lvi.SubItems[1].Text);
					if (topLevelNode != null)
						topLevelNode.Text = lvi.SubItems[0].Text;
				}
			}
			DialogResult = DialogResult.OK;
			Close();
		}

		private void btnCancel_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Close();
		}

	}
}