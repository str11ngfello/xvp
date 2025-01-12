using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Threading;
using XVPManaged;
using System.Runtime.InteropServices;
using XVPDesigner.Dialogs;
using System.Collections;

namespace XVPDesigner.UserControls
{
    public partial class ShaderBuilder : UserControl
    {
		private bool m_regenerationOccurred = false;
		public bool RegenerationOccurred
		{
			get { return m_regenerationOccurred; }
		}

        public ShaderBuilder()
        {
            InitializeComponent();
        }

        public void init()
        {
			StreamReader sr = null;
			try
			{
				//If the ShaderDesigner.wad file doesn't exist, regenerate all shaders...
				bool noWadFile = !File.Exists(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");

				//Look at each tfx file... if it is newer than shaders that were generated from the tfx file,
				//we need to regenerate the shader
				foreach (string templateFile in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.tfx",SearchOption.TopDirectoryOnly))
				{
					//Look at each shader generated from this template
					string templateName = Path.GetFileNameWithoutExtension(templateFile) + ".fx";
					foreach (string shaderFile in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.fx",SearchOption.TopDirectoryOnly))
					{
						//Was this fx file generated from the template fx file?
						if (shaderFile.EndsWith("#"+templateName))
						{
							bool needsRegeneration = false;

							//Check the timestamp
							if (File.GetLastWriteTime(templateFile) > File.GetLastWriteTime(shaderFile))
								needsRegeneration = true;

							//Check include files, if any .fxh or files are newer, regenerate the shader
							//to catch any potential changes.  We could be a little more exact here, but
							//the process is pretty quick
							if (!needsRegeneration)
							{
								foreach (string headerFile in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.hfx",SearchOption.TopDirectoryOnly))
								{
									//Open up shader file and check to see if the header is included, if it is, and the
									//header file is newer, we need to regenerate
									bool finished = false;
									sr = File.OpenText(templateFile);
									string input = null;
									while ((input = sr.ReadLine()) != null)
									{
										if (input.Contains(Path.GetFileName(headerFile)) && input.Contains("include") && !input.Trim().StartsWith("//"))
										{
											if (File.GetLastWriteTime(headerFile) > File.GetLastWriteTime(shaderFile))
											{
												needsRegeneration = true;
												finished = true;
											}
											break;
										}
									}
									if (finished)
										break;
									sr.Close();
								}
							}

							//Regenerate if we need to
							if (needsRegeneration || noWadFile)
							{
								//Open up shader file and get the features used to generate it so we can
								//use them to regenerate the shader
								Program.g_mainForm.logMessage("Generating shader " + Path.GetFileName(shaderFile) + "...",Color.Black);
								string defines = "";
								string description = null;
								if (Utility.getShaderMetaData(shaderFile,out description,out defines))
								{
									generateShader("ShaderDesigner.wad",description,templateFile,shaderFile.Split('#')[0].Trim(),defines,true);
									m_regenerationOccurred = true;
								}
							}
						}
					}
				}
			}
			catch (Exception exception)
			{
				if (sr != null)
					sr.Close();
				MessageBox.Show("There was a problem while regenerating shaders from their templates. See the message log for more information.","Shader Regeneration Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}

            //For each file, check to see if it meets the naming conventions for a shader.  If so,
            //add the object type string to the object type drop down...
			foreach (string filename in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\", "*.tfx", SearchOption.TopDirectoryOnly))
            {
                //File naming conventions we are looking for look like this ->
                //"Model-Blinn Phong.fx" where the first part "Model" is the object type,
                //and the second part "Blinn Phong" is the lighting model.  If the filename
                //doesn't adhere to this convention, we aren't interested in it...
                string[] parts = Path.GetFileNameWithoutExtension(filename).Split('-');
                if (parts.Length != 2)
                    continue;

                //Add shader object type
				if (!cbObjectType.Items.Contains(parts[0]))
					cbObjectType.Items.Add(parts[0]);
            }

			//For each generated shader file, add it to the available list of shaders
			try
			{
				foreach (string filename in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.fx",SearchOption.TopDirectoryOnly))
				{
					if (!filename.Contains("#"))
						continue;
					else
					{
						sr = File.OpenText(filename);
						ListViewItem lvi = new ListViewItem(Path.GetFileName(filename).Split('#')[0].Trim());
						lvi.SubItems.Add(Path.GetFileNameWithoutExtension(filename).Split('#')[1].Trim() + ".tfx");

						string input = null;
						while ((input = sr.ReadLine()) != null)
						{
							if (input.Contains("//!!|"))
							{
								string[] parts = input.Split(new char[] { '|' },StringSplitOptions.RemoveEmptyEntries);
								if (parts.Length == 2)
								{
									lvi.SubItems.Add(parts[1].Trim());
									break;
								}
							}
						}
						lvAvailableShaders.Items.Add(lvi);
						sr.Close();
					}
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show("Could not populate available shader list view.  See the message log for more information.","Could Not Populate List View",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (sr != null)
					sr.Close();
			}
        }

        private void cbObjectType_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Clear out existing lighting models and shader features
            cbLightingModel.Items.Clear();
            lvShaderFeatures.Items.Clear();
			lblLightingModelDesc.Text = "";

            //Find all the files that match the object type selected and add their
            //lighting model strings to the lighting model drop down...
            foreach (string filename in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\", cbObjectType.SelectedItem.ToString() +"-*.tfx", SearchOption.TopDirectoryOnly))
            {
                //File naming conventions we are looking for look like this ->
                //"Model-Blinn Phong.fx" where the first part "Model" is the object type,
                //and the second part "Blinn Phong" is the lighting model.  If the filename
                //doesn't adhere to this convention, we aren't interested in it...
                string[] parts = Path.GetFileNameWithoutExtension(filename).Split('-');
                if (parts.Length != 2)
                    continue;

                //Add shader lighting model...
				if (!cbLightingModel.Items.Contains(parts[1]))
					cbLightingModel.Items.Add(parts[1]);
            }

			btnCreateShader.Enabled = false;
			btnGeneratePreview.Enabled = false;
            
        }

        private void cbLightingModel_SelectedIndexChanged(object sender, EventArgs e)
        {
            //clear out existing shader features..
            lvShaderFeatures.Items.Clear();

            StreamReader sr = null;
            try
            {
                sr = File.OpenText(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\"+cbObjectType.SelectedItem.ToString()+"-"+cbLightingModel.SelectedItem.ToString()+".tfx");
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString(),"Error locating file...",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (sr != null)
					sr.Close();
                return;
            }

            string input = null;
            while ((input = sr.ReadLine()) != null)
            {
				if (input.Contains("//!!|"))
				{
					string[] parts = input.Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries);
					if (parts.Length == 2)
					{
						lblLightingModelDesc.Text = parts[1].Trim();
					}
					else
					{
						MessageBox.Show("Malformed shader description string - " + input, "Shader Parsing Error",MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
				}
				else if (input.Contains("//##|"))
				{
					string[] featureParts = input.Split(new char[] { '|' }, StringSplitOptions.RemoveEmptyEntries);
					if (featureParts.Length >= 4)
					{
						//We assume the following
						//1. the first part is the required special symbol "//##"
						//2. is the required feature symbol used as a #define during compilation
						//3. is the required feature name 
						//4. is the required feature description 
						//5. the reset of the parts are optional dependencies of the feature
						//featureDefines.Add(featureParts[1].Trim());
						ListViewItem lvi = new ListViewItem(featureParts[2].Trim());
						lvi.Name = featureParts[1].Trim();
						lvShaderFeatures.Items.Add(lvi);

						//Add description
						lvi.SubItems.Add(featureParts[3]);

						//Stick all the dependent features into a string in the first subitem
						string dependents = "";
						for (int i = 4;i < featureParts.Length;++i)
							dependents += featureParts[i] + "|";
						lvi.SubItems.Add(dependents);
					}
					else
					{
						MessageBox.Show("Malformed feature string - " + input + " Expected at least 4 tokens.", "Shader Parsing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
				}
            }
            sr.Close();

			if (cbLightingModel.Text == "")
			{
				btnCreateShader.Enabled = false;
				btnGeneratePreview.Enabled = false;
			}
			else
			{
				btnCreateShader.Enabled = true;
				btnGeneratePreview.Enabled = true;
			}
        }

		private void lvShaderFeatures_SelectedIndexChanged(object sender, EventArgs e)
		{
			
		}

		private void btnCreateShader_Click(object sender, EventArgs e)
		{
			//Don't let the user save anywhere else but the initial directory
		/*	bool validDir = false;
			SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
			dlgSaveFileDialog.Title = "Save Shader...";
			dlgSaveFileDialog.Filter = "Shader Files (*.fx)|*.fx|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Untitled.fx";
			dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ShaderPath;



			while (!validDir)
			{
				dlgSaveFileDialog.FileName = "Untitled.fx";
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result != DialogResult.OK)
					return;

				if (Path.GetDirectoryName(dlgSaveFileDialog.FileName) != Program.g_mainForm.CurrentWorkspace.ShaderPath)
				{
					validDir = false;
					MessageBox.Show("The Shader Builder is limited to saving into the specified folder.","Specific Save Location Required",MessageBoxButtons.OK,MessageBoxIcon.Information);
					continue;
				}
				if (dlgSaveFileDialog.FileName.Contains("#"))
				{
					validDir = false;
					MessageBox.Show("The shader name cannot contain a '#'","Illegal Character",MessageBoxButtons.OK,MessageBoxIcon.Information);
					continue;
				}
				if (!dlgSaveFileDialog.FileName.EndsWith(".fx"))
				{
					validDir = false;
					MessageBox.Show("The shader name must end with \".fx\"","Missing .fx extension",MessageBoxButtons.OK,MessageBoxIcon.Information);
					continue;
				}
				validDir = true;
			}
			*/

			CreateShaderDialog dialog = new CreateShaderDialog(cbObjectType.SelectedItem.ToString() + "-" + cbLightingModel.SelectedItem.ToString() + ".fx");
			dialog.ShowDialog();
			if (!dialog.ValidData)
				return;
			string filename = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + dialog.ShaderName.Trim();
			if (!filename.EndsWith(".fx"))
				filename += ".fx";
			
			string defines = " ";
			foreach (ListViewItem item in lvShaderFeatures.Items)
			{
				if (item.Checked)
					defines += "-D" + item.Name + "=" + item.Name + " ";
			}

			if (generateShader("ShaderDesigner.wad",dialog.ShaderDescription,Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + cbObjectType.SelectedItem.ToString() + "-" + cbLightingModel.SelectedItem.ToString() + ".tfx",filename,defines,true))
			{
				//Add shader to available box if it doesn't exist
				bool exists = false;
				string displayName = Path.GetFileName(filename);
				foreach (ListViewItem lvi in lvAvailableShaders.Items)
				{
					if (lvi.ToString() == displayName)
					{
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					ListViewItem lvi = new ListViewItem(displayName);
					lvi.SubItems.Add(cbObjectType.SelectedItem.ToString() + "-" + cbLightingModel.SelectedItem.ToString() + ".tfx");
					lvi.SubItems.Add(dialog.ShaderDescription);
					lvAvailableShaders.Items.Add(lvi);
				}

				DialogResult dr = MessageBox.Show("Successfully created shader.  Designer must restart in order for changes to take effect.  Restart?","Shader Creation Complete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
				if (dr == DialogResult.Yes)
					Application.Restart();
				 
				lock (Program.g_mainForm.XVPMLockObject)
				{
					IntPtr pWadFile = Marshal.StringToHGlobalAnsi(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");
					XVPM.loadShaderWad(pWadFile);
					Marshal.FreeHGlobal(pWadFile);
				}

				dialog.Dispose();
			}
			else
			{
				MessageBox.Show("Shader creation failed.  Check the message log for more information.","Shader Creation Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}

		public bool generateShader(string wadFilename,string desc,string templateFilename,string shaderFilename,string defines,bool log)
		{
			string compoundFilename = null;
			string baseFilename = null;
			FileStream inFS = null;
			FileStream outFS = null;
			TextReader tr = null;
			TextWriter tw = null;
			bool success = true;

			try
			{
				//First, if there is a generated shader already, rename it to back it up.
				//If we fail we want to restore what was there
				compoundFilename = shaderFilename + "#" + Path.GetFileNameWithoutExtension(templateFilename) + ".fx";
				if (File.Exists(compoundFilename + ".backup"))
					File.Delete(compoundFilename + ".backup");
				if (File.Exists(compoundFilename))
					File.Move(compoundFilename,compoundFilename+".backup");
				
				//Run filepp to preprocess the file and include only selected feature defines
				Process p = new Process();
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\filepp.pl";
				p.StartInfo.UseShellExecute = false;
				//p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.CreateNoWindow = true;
				p.StartInfo.UseShellExecute = true;
				p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;				
				p.StartInfo.Arguments =  "\"" + templateFilename + "\" -o\"" + templateFilename + ".preprocessed\" -I\"" + Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\" " + defines;
				//if (log)
				//	Program.g_mainForm.logMessage("Starting filepp with arguments -" + p.StartInfo.Arguments,Color.Black);
				p.Start();
				while (!p.HasExited)
					Application.DoEvents();

				//Now with a preprocessed file, let's replace enumerations for textures/samplers, etc
				if (!File.Exists(templateFilename+".preprocessed"))
					throw new Exception("Could not find preprocessed file! - " + templateFilename+".preprocessed");

				//Create a new version of the shader using the template
				uint textureEnum = 0;
				uint samplerEnum = 0;

				inFS = File.Open(templateFilename+".preprocessed",FileMode.Open,FileAccess.Read);
				outFS = File.Open(compoundFilename,FileMode.Create,FileAccess.Write);
				tr = new StreamReader(inFS);
				tw = new StreamWriter(outFS);

				string input = "";
				while ((input = tr.ReadLine()) != null)
				{
					if (desc != null)
					{
						if (input.Contains("//!!|"))
						{
							input = "//!!| " + desc.Replace('\n',' ');
						}
					}
					if (input.Contains("#TEX_ENUM"))
					{
						input = input.Replace("#TEX_ENUM","t" + textureEnum.ToString());
						textureEnum++;
					}
					if (input.Contains("#SAM_ENUM"))
					{
						input = input.Replace("#SAM_ENUM","s" + samplerEnum.ToString());
						samplerEnum++;
					}

					tw.WriteLine(input);
				}

				tr.Close();
				tw.Close();
				inFS.Close();
				outFS.Close();

				//Delete intermediate preprocessed file
				if (File.Exists(templateFilename + ".preprocessed"))
					File.Delete(templateFilename + ".preprocessed");

				//Next, generate the shader object code
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpshadergen";
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.CreateNoWindow = true;
				p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;

				p.StartInfo.Arguments =  "\"" + compoundFilename + "\" XVP_PC";
				p.Start();
				StreamReader streamReader = p.StandardOutput;
				while ((!p.HasExited) || (!streamReader.EndOfStream))
				{
					
					string s = streamReader.ReadLine();
					if (log)
					{	
						if (s != null)
						{
							if (s.Contains("failed") || s.Contains(": error"))
								Program.g_mainForm.logMessage(s,Color.Red);
							else
								Program.g_mainForm.logMessage(s,Color.Black);
						}
					}
					Application.DoEvents();
				}

				//Test xvpshadergen's success
				if (p.ExitCode != -1)
				{
					//Next, add the shader object code to the designer shader wad
					p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
					p.StartInfo.UseShellExecute = false;
					p.StartInfo.RedirectStandardOutput = true;
					p.StartInfo.CreateNoWindow = true;

					//Build file list.  Enclose each path in quotes to handle whitespace properly.
					string files = "";
					baseFilename = Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename);
					if (File.Exists(baseFilename + ".vso"))
					{
						File.Move(baseFilename + ".vso",shaderFilename + ".vso");
						files += "\"" + shaderFilename + ".vso\" ";
					}
					if (File.Exists(baseFilename + ".gso"))
					{
						File.Move(baseFilename + ".gso",shaderFilename + ".gso");
						files += "\"" + shaderFilename + ".gso\" ";
					}
					if (File.Exists(baseFilename + ".pso"))
					{
						File.Move(baseFilename + ".pso",shaderFilename + ".pso");
						files += "\"" + shaderFilename + ".pso\" ";
					}

					p.StartInfo.Arguments = "-a " + " \"" + Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + wadFilename +"\" " + files;
					p.Start();
					streamReader = p.StandardOutput;
					while (!p.HasExited || !streamReader.EndOfStream)
					{
						
						string s = streamReader.ReadLine();
						if (log)
						{
							if (s != null)
							{
								if (s.Contains("Adding") || s.Contains("Compressing"))
								{
									Program.g_mainForm.logMessage(s,Color.Green);
								}
								else
									Program.g_mainForm.logMessage(s,Color.Black);
							}
						}
					}

					//Lastly, if we backed up a file, then delete it since we succeeded
					//in creating a new file
					if (File.Exists(compoundFilename + ".backup"))
						File.Delete(compoundFilename + ".backup");

					success = true;
				}
				else
				{
					if (File.Exists(compoundFilename))
						File.Delete(compoundFilename);
					if (compoundFilename != null)
					{
						if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".vso"))
							File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".vso");
						if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".gso"))
							File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".gso");
						if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".pso"))
							File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".pso");
					}

					//Lastly, if we backed up a file, then restore it to the way it was
					if (File.Exists(compoundFilename + ".backup"))
						File.Move(compoundFilename + ".backup",compoundFilename);
					success = false;
				}

				//Delete intermediate code files
				if (File.Exists(shaderFilename + ".vso"))
					File.Delete(shaderFilename + ".vso");
				if (File.Exists(shaderFilename + ".gso"))
					File.Delete(shaderFilename + ".gso");
				if (File.Exists(shaderFilename + ".pso"))
					File.Delete(shaderFilename + ".pso");

				p.Dispose();
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Unabled to create/update shader from template - " + exception.ToString(),Color.Red);
				if (tr != null)
					tr.Close();
				if (tw != null)
					tw.Close();
				if (inFS != null)
					inFS.Close();
				if (outFS != null)
					outFS.Close();
				if (File.Exists(templateFilename + ".preprocessed"))
					File.Delete(templateFilename + ".preprocessed");
				if (compoundFilename != null)
					if (File.Exists(compoundFilename))
						File.Delete(compoundFilename);
				if (compoundFilename != null)
				{
					if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".vso"))
						File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".vso");
					if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".gso"))
						File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".gso");
					if (File.Exists(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".pso"))
						File.Delete(Path.GetDirectoryName(compoundFilename) + "\\" + Path.GetFileNameWithoutExtension(compoundFilename) + ".pso");
				}
				//Lastly, if we backed up a file, then restore it to the way it was
				if (File.Exists(compoundFilename + ".backup"))
					File.Move(compoundFilename + ".backup",compoundFilename);
				success =  false;
			}

			return success;
		}

		private void btnRebuildAllShaders_Click(object sender,EventArgs e)
		{
			if (lvAvailableShaders.Items.Count == 0)
			{
				MessageBox.Show("There are no shaders to rebuild.","Rebuild All Shaders",MessageBoxButtons.OK,MessageBoxIcon.Question);
				return;
			}

			DialogResult dr = MessageBox.Show("Rebuilding all shaders could take several minutes depending on the size of the shader library.  Do you want to rebuild?","Rebuild All Shaders?",MessageBoxButtons.YesNo,MessageBoxIcon.Question);
			if (dr == DialogResult.No)
				return;

			//Regenerate each shader into a temporary shader wad file. If we succeed, replace
			//the current shader library with the new one
			StreamReader sr = null;
			bool success = true;
			try
			{
				foreach (string templateFile in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.tfx",SearchOption.TopDirectoryOnly))
				{
					//Look at each shader generated from this template
					string templateName = Path.GetFileNameWithoutExtension(templateFile) + ".fx";
					foreach (string shaderFile in Directory.GetFiles(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\","*.fx",SearchOption.TopDirectoryOnly))
					{
						if (!shaderFile.EndsWith("#"+templateName))
							continue;

						//Open up shader file and get the features used to generate it so we can
						//use them to regenerate the shader
						Program.g_mainForm.logMessage("Generating shader " + Path.GetFileName(shaderFile) + "...",Color.Black);
						string defines = "";
						string description = null;
						if (Utility.getShaderMetaData(shaderFile,out description,out defines))
						{
							if (!generateShader("ShaderDesigner.wad.temp",description,templateFile,shaderFile.Split('#')[0].Trim(),defines,true))
							{
								success = false;
								break;
							}
						}
					}
				}

				if (success)
				{
					//Replace wad file
					if (File.Exists(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad"))
						File.Delete(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");
					File.Move(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad.temp",Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");								
						
					dr = MessageBox.Show("Successfully rebuilt shaders.  Designer must restart in order for changes to take effect.  Restart?","Shader Rebuild Complete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
					if (dr == DialogResult.Yes)
						Application.Restart();
				}
				else
				{
					MessageBox.Show("Failed to rebuild all shaders.  See the message log for more information?","Shader Rebuild Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				}
			}
			catch (Exception exception)
			{
				if (sr != null)
					sr.Close();
				MessageBox.Show("There was a problem while regenerating shaders from their templates. See the message log for more information.","Shader Regeneration Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
		}

		private void btnDeleteShader_Click(object sender,EventArgs e)
		{
			if (lvAvailableShaders.SelectedItems.Count == 0)
			{
				MessageBox.Show("Select one or more shaders from the shader library to delete.","No Shaders Selected",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			DialogResult dr = MessageBox.Show("Are you sure you want to delete the selected shaders?","Confirm Delete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
			if (dr == DialogResult.No)
				return;
			
			try
			{
				Process p = new Process();
				StreamReader streamReader;
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.CreateNoWindow = true;
				p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;

				foreach (ListViewItem lvi in lvAvailableShaders.SelectedItems)
				{
					string filename = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\"+lvi.Text + "#" + Path.GetFileNameWithoutExtension(lvi.SubItems[1].Text) + ".fx";

					//Delete the shader
					Program.g_mainForm.logMessage("Deleting shader " + Path.GetFileName(filename) + "...",Color.Black);
					if (File.Exists(filename))
						File.Delete(filename);

					p.StartInfo.Arguments = "-d " + " \"" + Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad\" " + lvi.Text + ".vso " + lvi.Text + ".gso "+ lvi.Text + ".pso ";
					p.Start();
					streamReader = p.StandardOutput;
					while (!p.HasExited || !streamReader.EndOfStream)
					{
						string s = streamReader.ReadLine();
						if (s != null)
						{
							if (s.Contains("Deleting") || s.Contains("Compressing"))
							{
								Program.g_mainForm.logMessage(s,Color.Green);
							}
							else
								Program.g_mainForm.logMessage(s,Color.Black);
						}
					}

					lvi.Remove();

				}

				p.Dispose();

				dr = MessageBox.Show("Successfully deleted shaders.  Designer must restart in order for changes to take effect.  Restart?","Shader Deletion Complete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
				if (dr == DialogResult.Yes)
					Application.Restart();
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not delete selected shaders. " + exception.ToString(),Color.Red);
				MessageBox.Show("Problems were encountered while trying to delete selected shaders.  See the message log for details.","Error Deleting Selected Shaders",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}

		private void lvShaderFeatures_ItemChecked(object sender,ItemCheckedEventArgs e)
		{
			//Check for dependencies/exclusions and check/uncheck them 
			if (e.Item.Checked)
			{
				if (e.Item.SubItems != null)
					if (e.Item.SubItems[2] != null)
					{
						string[] parts = e.Item.SubItems[2].Text.Split('|');
						if (parts.Length != 0)
						{
							//For each dependency/exclusion...
							for (int i = 0;i < parts.Length;++i)
							{
								bool dependency = true;
								string featureDefine = parts[i].Trim();
								if (parts[i].StartsWith("!"))
								{
									featureDefine = featureDefine.Trim(new char[] { '!' });
									dependency = false;
								}
								foreach (ListViewItem lvi2 in lvShaderFeatures.Items)
								{
									if (lvi2.Name == featureDefine)
										lvi2.Checked = dependency;
								}
							}
						}
					}
			}

		}

		private void btnViewSource_Click(object sender,EventArgs e)
		{
			if (lvAvailableShaders.SelectedItems.Count == 0)
			{
				MessageBox.Show("Select one or more shaders from the shader library to view source.","No Shaders Selected",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			try
			{
				foreach (ListViewItem lvi in lvAvailableShaders.SelectedItems)
				{
					Process wordpad = new Process();
					wordpad.StartInfo.FileName = "notepad.exe";
					wordpad.StartInfo.Arguments = "\""+Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + lvi.SubItems[0].Text + "#" + Path.GetFileNameWithoutExtension(lvi.SubItems[1].Text) + ".fx\"";
					wordpad.Start();
				}
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not open source for viewing. " + exception.ToString(),Color.Red);
				MessageBox.Show("Problems were encountered while trying to view source.  See the message log for details.","Error Viewing Source",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}	
		}

		private void lvAvailableShaders_SelectedIndexChanged(object sender,EventArgs e)
		{
			//Only show features if exactly ONE shader is selected
			if (lvAvailableShaders.SelectedItems.Count != 1)
				return;

			//Display the features built into this shader
			StreamReader sr = null;
			try
			{
				//clear out existing shader features..
				lvShaderConfiguration.Items.Clear();

				sr = File.OpenText(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\"+lvAvailableShaders.SelectedItems[0].Text + "#" + Path.GetFileNameWithoutExtension(lvAvailableShaders.SelectedItems[0].SubItems[1].Text) + ".fx");

				string input = null;
				while ((input = sr.ReadLine()) != null)
				{
					if (input.Contains("//##|"))
					{
						string[] featureParts = input.Split(new char[] { '|' },StringSplitOptions.RemoveEmptyEntries);
						if (featureParts.Length >= 4)
						{
							//We assume the following
							//1. the first part is the required special symbol "//##"
							//2. is the required feature symbol used as a #define during compilation
							//3. is the required feature name 
							//4. is the required feature description 
							//5. the reset of the parts are optional dependencies of the feature
							//featureDefines.Add(featureParts[1].Trim());
							ListViewItem lvi = new ListViewItem(featureParts[2].Trim());
							lvi.Name = featureParts[1].Trim();
							lvShaderConfiguration.Items.Add(lvi);

							//Add description
							lvi.SubItems.Add(featureParts[3]);
						}
						else
						{
							MessageBox.Show("Malformed feature string - " + input + " Expected at least 4 tokens.","Shader Parsing Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return;
						}
					}
				}
				sr.Close();
			}
			catch (Exception exception)
			{
				MessageBox.Show("There was a problem while trying to get the shaders features.  See the message log for more information.","Could Not Populate List View",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (sr != null)
					sr.Close();
			}

		}

		private void btnRebuildShader_Click(object sender,EventArgs e)
		{
			if (lvAvailableShaders.SelectedItems.Count == 0)
			{
				MessageBox.Show("Select one or more shaders from the shader library to rebuild them.","No Shaders Selected",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			StreamReader sr = null;
			try
			{
				foreach (ListViewItem lvi in lvAvailableShaders.SelectedItems)
				{
					string filename = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\"+lvi.Text + "#" + Path.GetFileNameWithoutExtension(lvi.SubItems[1].Text) + ".fx";
					
					//Open up shader file and get the features used to generate it so we can
					//use them to regenerate the shader
					Program.g_mainForm.logMessage("Generating shader " + Path.GetFileName(filename) + "...",Color.Black);
					string defines = "";
					string description = null;
					if (Utility.getShaderMetaData(filename,out description,out defines))
					{
						if (!generateShader("ShaderDesigner.wad",description,Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + lvi.SubItems[1].Text,Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + lvi.SubItems[0].Text,defines,true))
						{
							MessageBox.Show("Failed to rebuild shader " + lvi.Text,"Shader Rebuild Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
							return;
						}
					}
				}
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not rebuild selected shaders. " + exception.ToString(),Color.Red);
				MessageBox.Show("Problems were encountered while trying to rebuild selected shaders.  See the message log for details.","Error Viewing Source",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (sr != null)
					sr.Close();
			}

			DialogResult dr = MessageBox.Show("Successfully rebuilt selected shaders.  Designer must restart in order for changes to take effect.  Restart?","Shader Rebuild Complete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
			if (dr == DialogResult.Yes)
				Application.Restart();
		}

		private void btnDeleteAll_Click(object sender,EventArgs e)
		{
			if (lvAvailableShaders.Items.Count == 0)
			{
				MessageBox.Show("There are no shaders to delete.","Delete All Shaders",MessageBoxButtons.OK,MessageBoxIcon.Question);
				return;
			}

			DialogResult dr = MessageBox.Show("Are you sure you want to delete all shaders?","Delete All Shaders?",MessageBoxButtons.YesNo,MessageBoxIcon.Question);
			if (dr == DialogResult.No)
				return;

			try
			{
				foreach (ListViewItem lvi in lvAvailableShaders.Items)
				{
					string filename = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\"+lvi.Text + "#" + Path.GetFileNameWithoutExtension(lvi.SubItems[1].Text) + ".fx";

					//Delete the shader
					Program.g_mainForm.logMessage("Deleting shader " + Path.GetFileName(filename) + "...",Color.Black);
					if (File.Exists(filename))
						File.Delete(filename);
					lvi.Remove();

					//Delete the wad file
					if (File.Exists(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad"))
						File.Delete(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");
				}

				dr = MessageBox.Show("Successfully deleted shaders.  Designer must restart in order for changes to take effect.  Restart?","Shader Deletion Complete",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
				if (dr == DialogResult.Yes)
					Application.Restart();
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not delete all shaders. " + exception.ToString(),Color.Red);
				MessageBox.Show("Problems were encountered while trying to delete all shaders.  See the message log for details.","Error Deleting All Shaders",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
    }
}
