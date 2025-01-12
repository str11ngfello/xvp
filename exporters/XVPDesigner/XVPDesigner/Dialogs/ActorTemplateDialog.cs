using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using XVPDesigner;
using System.IO;

namespace Dialogs
{
	public partial class ActorTemplateDialog : Form
	{
		public string ActorTemplateName
		{
			get { return cbActorTemplate.SelectedItem.ToString(); }
		}
		
		public ActorTemplateDialog()
		{
			InitializeComponent();

			//Scan directory for actor templates...
			foreach (string filename in Directory.GetFiles(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath))
			{
				cbActorTemplate.Items.Add(Path.GetFileNameWithoutExtension(filename));
			}
		}

		private void btnFinish_Click(object sender,EventArgs e)
		{
			//Create actor components
			foreach (ListViewItem lvi in lvActorComponents.Items)
			{
				if (lvi.Checked)
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

		private void cbActorTemplate_SelectedIndexChanged(object sender,EventArgs e)
		{
			FileStream fs = null;
			TextReader tr = null;
			try
			{
				fs = File.Open(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath + "\\" + cbActorTemplate.SelectedItem.ToString() + ".acttemp",FileMode.Open);
				tr = new StreamReader(fs);
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not load actor template \"" + cbActorTemplate.SelectedItem.ToString() + "\"",Color.Red);
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
				lvi.Checked = true;
			}

			tr.Close();
			fs.Close();
		}


	}
}