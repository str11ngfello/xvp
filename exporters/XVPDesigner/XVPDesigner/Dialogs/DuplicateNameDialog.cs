using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.Dialogs
{
	public partial class DuplicateNameDialog : Form
	{
		public DuplicateNameDialog()
		{
			InitializeComponent();
		}

		private void btnOK_Click(object sender,EventArgs e)
		{
			DialogResult = DialogResult.OK;
			Close();
		}

		private void btnCancel_Click(object sender,EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Close();
		}
	}
}