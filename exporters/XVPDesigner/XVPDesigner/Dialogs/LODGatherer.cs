using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Dialogs
{
	public partial class LODGatherer : Form
	{
		public LODGatherer()
		{
			InitializeComponent();
			txtModelName.Text = "Model";
		}

		private void btnAdd_Click(object sender,EventArgs e)
		{
			if (dlgOpenFileDialog.ShowDialog() == DialogResult.OK)
			{
				ListViewItem lvi = lvLODGatherer.Items.Add(Convert.ToString(lvLODGatherer.Items.Count),Convert.ToString(lvLODGatherer.Items.Count));
				lvi.SubItems.Add(dlgOpenFileDialog.FileName);
			}
		}

		private void btnFinish_Click(object sender,EventArgs e)
		{
			if (txtModelName.Text == "")
			{
				MessageBox.Show("You must enter a valid model name.","Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			if (lvLODGatherer.Items.Count == 0)
			{
				MessageBox.Show("You must enter at least one Model LOD.","Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
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