using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using XVPDesigner;

namespace Wizards
{
	public partial class ActorTemplateWizard : Form
	{
		private bool m_completed = false;
		public bool Completed
		{
			get { return m_completed; }
			set { m_completed = value; }
		}

		public string WorkspacePath
		{
			get 
			{ 
				if (txtWorkspaceName.Text.EndsWith(".wrk"))
					return txtWorkspaceLocation.Text + "\\"  + txtWorkspaceName.Text + "\\" + txtWorkspaceName.Text; 
				else
					return txtWorkspaceLocation.Text + "\\"  + txtWorkspaceName.Text + "\\" + txtWorkspaceName.Text + ".wrk"; 
			}
		}

		public ActorTemplateWizard()
		{
			InitializeComponent();
            
		}

		private void wizardPage2_BeforePageDisplayed(object sender, DevComponents.DotNetBar.WizardCancelPageChangeEventArgs e)
		{
			cbTargetDevice.SelectedIndex = 0;
            //txtShaderPath.Text = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\shaders";
		}

		private void wizNewProject_FinishButtonClick(object sender, CancelEventArgs e)
		{
			try
			{
				//Create directories if they don't exist
				if (!Directory.Exists(txtWorkspaceLocation.Text))
					Directory.CreateDirectory(txtWorkspaceLocation.Text);
								
				if (!Path.IsPathRooted(txtDiscPath.Text))
					txtDiscPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\" + txtDiscPath.Text;
				if (!Directory.Exists(txtDiscPath.Text))
					Directory.CreateDirectory(txtDiscPath.Text);
				txtDiscPath.Text = Path.GetFullPath(txtDiscPath.Text);

				if (!Path.IsPathRooted(txtActorPath.Text))
					txtActorPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtActorPath.Text;
				if (!Directory.Exists(txtActorPath.Text))
					Directory.CreateDirectory(txtActorPath.Text);
				txtActorPath.Text = Path.GetFullPath(txtActorPath.Text);

				if (!Path.IsPathRooted(txtMaterialPath.Text))
					txtMaterialPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtMaterialPath.Text;
				if (!Directory.Exists(txtMaterialPath.Text))
					Directory.CreateDirectory(txtMaterialPath.Text);
				txtMaterialPath.Text = Path.GetFullPath(txtMaterialPath.Text);

				if (!Path.IsPathRooted(txtTexturePath.Text))
					txtTexturePath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtTexturePath.Text;
				if (!Directory.Exists(txtTexturePath.Text))
					Directory.CreateDirectory(txtTexturePath.Text);
				txtTexturePath.Text = Path.GetFullPath(txtTexturePath.Text);

				if (!Path.IsPathRooted(txtAudioPath.Text))
					txtAudioPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtAudioPath.Text;
				if (!Directory.Exists(txtAudioPath.Text))
					Directory.CreateDirectory(txtAudioPath.Text);
				txtAudioPath.Text = Path.GetFullPath(txtAudioPath.Text);

				if (!Path.IsPathRooted(txtScriptPath.Text))
					txtScriptPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtScriptPath.Text;
				if (!Directory.Exists(txtScriptPath.Text))
					Directory.CreateDirectory(txtScriptPath.Text);
				txtScriptPath.Text = Path.GetFullPath(txtScriptPath.Text);

				if (!Path.IsPathRooted(txtControllerPath.Text))
					txtControllerPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtControllerPath.Text;
				if (!Directory.Exists(txtControllerPath.Text))
					Directory.CreateDirectory(txtControllerPath.Text);
				txtControllerPath.Text = Path.GetFullPath(txtControllerPath.Text);

				if (!Path.IsPathRooted(txtCDDPath.Text))
					txtCDDPath.Text = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\"  + txtCDDPath.Text;
				if (!Directory.Exists(txtCDDPath.Text))
					Directory.CreateDirectory(txtCDDPath.Text);
				txtCDDPath.Text = Path.GetFullPath(txtCDDPath.Text);

				if (!Directory.Exists(txtWorkspaceLocation.Text + "\\"  + txtWorkspaceName.Text + "\\Actor Templates"))
					Directory.CreateDirectory(txtWorkspaceLocation.Text + "\\"  + txtWorkspaceName.Text + "\\Actor Templates");

				//Copy sample fx files
                /*if (!File.Exists(txtShaderPath.Text + "\\White.fx"))
				    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\White.fx",txtShaderPath.Text + "\\White.fx",true);
                if (!File.Exists(txtShaderPath.Text + "\\Material Color.fx"))
					File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\Material Color.fx", txtShaderPath.Text + "\\Material Color.fx", true);
                if (!File.Exists(txtShaderPath.Text + "\\Vertex Color.fx")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\Vertex Color.fx", txtShaderPath.Text + "\\Vertex Color.fx", true);
                if (!File.Exists(txtShaderPath.Text + "\\SimpleTexture.fx")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\SimpleTexture.fx", txtShaderPath.Text + "\\SimpleTexture.fx", true);
				*/
				//Copy sample textures
                if (!Directory.Exists(txtTexturePath.Text + "\\Animal"))
				    Directory.CreateDirectory(txtTexturePath.Text + "\\Animal");
                if (!Directory.Exists(txtTexturePath.Text + "\\Building")) 
                    Directory.CreateDirectory(txtTexturePath.Text + "\\Building");
                if (!Directory.Exists(txtTexturePath.Text + "\\Wacky")) 
                    Directory.CreateDirectory(txtTexturePath.Text + "\\Wacky");
                if (!File.Exists(txtTexturePath.Text + "\\Animal\\giraffe.png")) 
				    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\giraffe.png",txtTexturePath.Text + "\\Animal\\giraffe.png",true);
                if (!File.Exists(txtTexturePath.Text + "\\Animal\\cow.png")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\cow.png", txtTexturePath.Text + "\\Animal\\cow.png", true);
                if (!File.Exists(txtTexturePath.Text + "\\Building\\rivets.png")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\rivets.png",txtTexturePath.Text + "\\Building\\rivets.png",true);
                if (!File.Exists(txtTexturePath.Text + "\\Building\\bricks.png")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\bricks.png", txtTexturePath.Text + "\\Building\\bricks.png", true);
                if (!File.Exists(txtTexturePath.Text + "\\Wacky\\bacteria.png")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\bacteria.png", txtTexturePath.Text + "\\Wacky\\bacteria.png", true);
                if (!File.Exists(txtTexturePath.Text + "\\Wacky\\strands.png")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\strands.png", txtTexturePath.Text + "\\Wacky\\strands.png", true);
				
				//Copy Sample Audio
                if (!Directory.Exists(txtAudioPath.Text + "\\Blades")) 
                    Directory.CreateDirectory(txtAudioPath.Text + "\\Blades");
                if (!Directory.Exists(txtAudioPath.Text + "\\Harp")) 
                    Directory.CreateDirectory(txtAudioPath.Text + "\\Harp");
                if (!Directory.Exists(txtAudioPath.Text + "\\Weather")) 
                    Directory.CreateDirectory(txtAudioPath.Text + "\\Weather");
                if (!File.Exists(txtAudioPath.Text + "\\Blades\\BladeImpact01.wav")) 
				    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BladeImpact01.wav",txtAudioPath.Text + "\\Blades\\BladeImpact01.wav",true);
                if (!File.Exists(txtAudioPath.Text + "\\Blades\\BladeImpact02.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BladeImpact02.wav", txtAudioPath.Text + "\\Blades\\BladeImpact02.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Blades\\BladeImpact03.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BladeImpact03.wav", txtAudioPath.Text + "\\Blades\\BladeImpact03.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Blades\\BladeImpact04.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BladeImpact04.wav", txtAudioPath.Text + "\\Blades\\BladeImpact04.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Blades\\BladeImpact05.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BladeImpact05.wav", txtAudioPath.Text + "\\Blades\\BladeImpact05.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Harp\\HarpChordDown.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\HarpChordDown.wav", txtAudioPath.Text + "\\Harp\\HarpChordDown.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Harp\\HarpChordUp.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\HarpChordUp.wav", txtAudioPath.Text + "\\Harp\\HarpChordUp.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Harp\\HarpFanfareC.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\HarpFanfareC.wav", txtAudioPath.Text + "\\Harp\\HarpFanfareC.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Harp\\HarpFanfareF.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\HarpFanfareF.wav", txtAudioPath.Text + "\\Harp\\HarpFanfareF.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Weather\\BirdsDawnShort.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\BirdsDawnShort.wav", txtAudioPath.Text + "\\Weather\\BirdsDawnShort.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Weather\\Thunderous.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\Thunderous.wav", txtAudioPath.Text + "\\Weather\\Thunderous.wav", true);
                if (!File.Exists(txtAudioPath.Text + "\\Weather\\ThunderDeep.wav")) 
                    File.Copy(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Sample Resources\\ThunderDeep.wav", txtAudioPath.Text + "\\Weather\\ThunderDeep.wav", true);

			}
			catch (Exception exception)
			{
				MessageBox.Show("A problem was encountered during the creation of the workspace. Details: " + exception.Message, "Wizard Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
				e.Cancel = true;
			}

			Completed = true;
			Close();
		}

		private void btnActorPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtActorPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void wizardPage2_NextButtonClick(object sender, CancelEventArgs e)
		{
			//Verify Data 
			if (txtWorkspaceName.Text == "")
			{
				MessageBox.Show("You must specify a name for the new workspace.", "Wizard Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtWorkspaceLocation.Text == "")
			{
				MessageBox.Show("You must specify a location for the new workspace.", "Wizard Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (!Path.IsPathRooted(txtWorkspaceLocation.Text))
			{
				MessageBox.Show("The workspace location must be an absolute path.", "Wizard Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}

			string error = null;
			error = Utility.checkIPAddress(txtIPAddress.Text);
			if (error != null)
			{
				MessageBox.Show(error,"Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			
			error = Utility.checkPort(txtPort.Text);
			if (error != null)
			{
				MessageBox.Show(error,"Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
		}

		private void btnWorkspaceLocationBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtWorkspaceLocation.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnDiscPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtDiscPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnTexturePathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtTexturePath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnMaterialPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtMaterialPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnAudioPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtAudioPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnControllerPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtControllerPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnScriptPathBrowse_Click(object sender,EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtScriptPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void btnCDDPathBrowse_Click(object sender, EventArgs e)
		{
			if (browseFoldersDialog.ShowDialog() == DialogResult.OK)
			{
				txtCDDPath.Text = browseFoldersDialog.SelectedPath;
			}
		}

		private void wizardPage4_BeforePageDisplayed(object sender, DevComponents.DotNetBar.WizardCancelPageChangeEventArgs e)
		{
			cbWindowSkin.SelectedItem = ciBlack;
		}

		public Workspace genWorkspace()
		{
			Workspace w = new Workspace();

			w.ActorLibraryPath = txtActorPath.Text;
			w.ActorTemplatePath = txtWorkspaceLocation.Text + "\\" + txtWorkspaceName.Text + "\\Actor Templates";
			w.MaterialLibraryPath = txtMaterialPath.Text;
			w.TextureLibraryPath = txtTexturePath.Text;
			w.AudioLibraryPath = txtAudioPath.Text;
			w.ScriptLibraryPath = txtScriptPath.Text;
			w.ControllerLibraryPath = txtControllerPath.Text;
			w.CDDPath = txtCDDPath.Text;
			w.DiscPath = txtDiscPath.Text;
			w.ShowToolTips = chkDisplayToolTips.Checked;
			w.PlayEventSounds = chkPlayAudioEvents.Checked;
			w.SkinColor = cbWindowSkin.SelectedIndex;
			w.TargetDevice = cbTargetDevice.SelectedItem.ToString();
			w.TargetIP = txtIPAddress.Text;
			w.TargetPort = txtPort.Text;

			return w;
		}

		private void wizNewWorkspace_NextButtonClick(object sender,CancelEventArgs e)
		{
			
		}

		private void wizardPage3_NextButtonClick(object sender,CancelEventArgs e)
		{
			if (txtActorPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Actor Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtTexturePath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Texture Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtMaterialPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Material Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtControllerPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Controller Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtAudioPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Audio Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtScriptPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Script Library.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
			if (txtCDDPath.Text == "")
			{
				MessageBox.Show("You must specify a location for Custom Data Definition searches.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}

			if (txtDiscPath.Text == "")
			{
				MessageBox.Show("You must specify a location for the Disc directory.","Wizard Error",MessageBoxButtons.OKCancel,MessageBoxIcon.Error);
				e.Cancel = true;
				return;
			}
		}
	}
}