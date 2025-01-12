using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.IO;
using System.Diagnostics;

namespace XVPDCCTools
{
	public partial class mainForm : Form
	{
		private string m_xvpPath = null;
		private Process m_colladaProcess = new Process();

		public mainForm()
		{
			InitializeComponent();
		}

		private void btnCancel_Click(object sender,EventArgs e)
		{
			TopMost = false;
			Application.Exit();
		}

		private void btnInstall_Click(object sender,EventArgs e)
		{
			TopMost = false;

			// First, let's make sure there is a valid XVP installation
			RegistryKey key = Registry.LocalMachine.OpenSubKey("Software\\Seventh Night Studios\\XVP",false);
			if (key == null)
			{
				MessageBox.Show("Could not locate XVP registry key.  Run the XVP installer.","XVP Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Application.Exit();
			}

			m_xvpPath = (string)key.GetValue("Install Folder");
			if (m_xvpPath == null)
			{
				MessageBox.Show("Could not locate XVP installation folder.  Run the XVP installer.","XVP Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Application.Exit(); ;
			}

			//Install scripts
			foreach (string s in lbDCCs.Items)
			{
				try
				{
					if (!Directory.Exists(s + "XVP"))
						Directory.CreateDirectory(s + "XVP");

					//Quick check to make sure source files exists.  If not, then the XVP install
					//may not have been run yet.
					if (!File.Exists(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPInstall.ms") ||
						!File.Exists(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPMacros.ms") ||
						!File.Exists(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\iconv.dll") ||
						!File.Exists(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPExport.exe") ||
						!File.Exists(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPCollada.exe"))
					{
						MessageBox.Show("Missing source files.  Make sure the XVP installer is run before this install.","Missing Source Files",MessageBoxButtons.OK,MessageBoxIcon.Error);
						continue;
					}

					File.SetAttributes(s + "Scripts\\Startup\\XVPInstall.ms",FileAttributes.Normal);
					File.SetAttributes(s + "stdplugs\\stdscripts\\XVPMacros.ms",FileAttributes.Normal);
					File.SetAttributes(s + "XVP\\iconv.dll",FileAttributes.Normal);
					File.SetAttributes(s + "XVP\\XVPExport.exe",FileAttributes.Normal);
					File.SetAttributes(s + "XVP\\XVPCollada.exe",FileAttributes.Normal);
					File.Copy(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPInstall.ms",s + "Scripts\\Startup\\XVPInstall.ms",true);
					File.Copy(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPMacros.ms",s + "stdplugs\\stdscripts\\XVPMacros.ms",true);
					File.Copy(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\iconv.dll",s + "XVP\\iconv.dll",true);
					File.Copy(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPExport.exe",s + "XVP\\XVPExport.exe",true);
					File.Copy(m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\XVPCollada.exe",s + "XVP\\XVPCollada.exe",true);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Failed to install for " + s + " Reason -> " + exception.ToString(),"DCC Install Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				}	
			}

			//If there were DCCs found, install COLLADA
			m_colladaProcess.StartInfo.FileName = m_xvpPath + "xvpsdk\\exporters\\3DStudioMax\\ColladaMax_FREE_3.05B.exe";
			//m_colladaProcess.StartInfo.Arguments ;
			m_colladaProcess.StartInfo.CreateNoWindow = false;
			m_colladaProcess.StartInfo.UseShellExecute = true;
			m_colladaProcess.StartInfo.RedirectStandardOutput = false;
			//m_colladaProcess.Exited += new EventHandler(OnExitProcess);
			m_colladaProcess.EnableRaisingEvents = true;
			
			//Start process and wait for exit
			m_colladaProcess.Start();
			m_colladaProcess.WaitForExit();

			
			Application.Exit();
		}

		private void mainForm_Load(object sender,EventArgs e)
		{
			//Let's search the registry for installed DCCs and their
			//installation folders.

			//Look for supported versions of 3D Studio Max
			RegistryKey keyMax = Registry.LocalMachine.OpenSubKey("Software\\Autodesk\\3dsMax",false);
			if (keyMax != null)
			{
				foreach (string s in keyMax.GetSubKeyNames())
				{
					RegistryKey keyFinal = keyMax.OpenSubKey(s + "\\MAX-1:409");
					if (keyFinal != null)
					{ 
						if (s == "10.0") // Max 2008
							lbDCCs.Items.Add((string)keyFinal.GetValue("Installdir"));
						if (s == "11.0") // Max 2009
							lbDCCs.Items.Add((string)keyFinal.GetValue("Installdir"));
						if (s == "12.0") // Max 2010
							lbDCCs.Items.Add((string)keyFinal.GetValue("Installdir"));
					}
				}
			}

			//If we didn't find any DCC's were done
			if (lbDCCs.Items.Count == 0)
				Application.Exit();

		}
	}
}