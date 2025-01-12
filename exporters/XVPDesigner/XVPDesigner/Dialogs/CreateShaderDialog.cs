using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace XVPDesigner.Dialogs
{
	public partial class CreateShaderDialog : Form
	{
		string m_templateName = "";

		public string ShaderName
		{
			get { return txtShaderName.Text; }
		}

		private bool m_validData = false;
		public bool ValidData
		{
			get { return m_validData; }
		}

		public string ShaderDescription
		{
			get { return txtShaderDescription.Text; }
		}

		public bool ViewSource
		{
			get { return chkViewSource.Checked; }
		}


		public CreateShaderDialog(string templateName)
		{
			InitializeComponent();
			m_templateName = templateName;
		}

		private void btnOK_Click(object sender,EventArgs e)
		{
			m_validData = false;

			//Check for presence of data
			if (txtShaderName.Text == "" || txtShaderDescription.Text == "")
			{
				MessageBox.Show("You must supply both a name and description for the shader.","Missing Information",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			//Check for invalid characters
			if (txtShaderName.Text.Contains("#"))
			{
				MessageBox.Show("The shader name cannot contain a '#'","Illegal Character",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			//Check for invalid characters
			if (Char.IsDigit(txtShaderName.Text[0]))
			{
				MessageBox.Show("The shader name cannot start with a number.","Illegal Character",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			//Check for pre-existing shader name and prompt for overwrite if necessary
			string filename = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + txtShaderName.Text;
			if (!filename.EndsWith(".fx"))
				filename += ".fx";
			filename += "#" + m_templateName;
			if (File.Exists(filename))
			{
				DialogResult dr = MessageBox.Show("A shader named " + txtShaderName.Text + " already exists.  Overwrite?","Shader Already Exists...",MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (dr == DialogResult.No)
					return;
			}

			m_validData = true;
			Close();
		}

		private void btnCancel_Click(object sender,EventArgs e)
		{
			m_validData = false;
			Close();
		}
	}
}
