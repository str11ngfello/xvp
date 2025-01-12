using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using XVPDesigner.Dialogs;

namespace XVPDesigner.UserControls
{
    public partial class WadInspector : UserControl
    {
		private string m_filename;
		public string Filename
		{
			get { return m_filename; }
			set 
			{ 
				m_filename = value;
			}
		}

        public WadInspector()
        {
            InitializeComponent();
        }

        private void buttonX1_Click(object sender, EventArgs e)
        {
			refreshContents();
           
        }

        
        private void btnAddFile_Click(object sender, EventArgs e)
        {
            AddFileToWadDialog diag = new AddFileToWadDialog();
            diag.ShowDialog();


            //Do we have a filename? 
            if (diag.Paths.Count == 0)
                return;

            try
            {
                Process p = new Process();
                p.StartInfo.CreateNoWindow = true;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";

                string options;
                if (diag.Compress)
                    options = "-ac ";
                else
                    options = "-a ";
                                
                p.StartInfo.Arguments = options + "\"" + m_filename + "\"";
                
                foreach (string s in diag.Paths)
                   p.StartInfo.Arguments += " \"" + s + "\" ";

                p.StartInfo.WorkingDirectory = Path.GetDirectoryName(m_filename);
                p.Start();
                p.WaitForExit();

                p.Dispose();
                diag.Dispose();

				refreshContents();
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }          
        }

        private void tsmiRemoveSelectedFiles_Click(object sender, EventArgs e)
        {
			removeSelectedFiles();
        }

		public void refreshContents()
		{
			try
			{
				//Set file size of wad
				FileInfo fi = new FileInfo(m_filename);
				lblWadSize.Text = Utility.FormatFileSize(fi.Length);

				Process p = new Process();
				p.StartInfo.CreateNoWindow = true;
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
				p.StartInfo.Arguments = "-l " + "\"" + m_filename + "\"";
				p.StartInfo.WorkingDirectory = Path.GetDirectoryName(m_filename);
				p.Start();

				string output = p.StandardOutput.ReadToEnd();
				p.WaitForExit();


				lvFiles.Items.Clear();

				string[] files = output.Split(new char[] {'\n'},StringSplitOptions.RemoveEmptyEntries);

				//Skip the first 
				for (int i = 1; i < files.Length; ++i)
				{
					if (files[i] != "" && files[i] != "\r")
					{
						//Format of line should be something like:  "myfile.png"   original(288) compressed(101)
						files[i] = files[i].Trim();
						string[] data = files[i].Split(new char[1] { '"' },StringSplitOptions.RemoveEmptyEntries);
						data[0] = data[0].TrimEnd('"');

						string[] data2 = data[1].Split(new char[1] { ' ' },StringSplitOptions.RemoveEmptyEntries);

						if (data2.Length == 2)
						{
							ListViewItem lvi = new ListViewItem();
							lvi.Text = data[0];

							string sOrgSize = data2[0].Split('(')[1].Trim(')');
							long orgSize = long.Parse(sOrgSize);
							string sCompressedSize = data2[1].Split('(')[1].Trim(')');
							long compressedSize = long.Parse(sCompressedSize);

							lvi.SubItems.Add(Utility.FormatFileSize(orgSize));
							lvi.SubItems.Add("yes");
							lvi.SubItems.Add(Utility.FormatFileSize(compressedSize));



							if (compressedSize != 0)
							{
								float ratio = (compressedSize / (float)orgSize) * 100;
								lvi.SubItems.Add(ratio.ToString("N2") + "%");
							}
							lvFiles.Items.Add(lvi);
						}
						else if (data2.Length == 1)
						{
							ListViewItem lvi = new ListViewItem();
							lvi.Text = data[0];

							string sOrgSize = data2[0].Split('(')[1].Trim(')');
							long orgSize = long.Parse(sOrgSize);
							lvi.SubItems.Add(Utility.FormatFileSize(orgSize));

							lvi.SubItems.Add("no");
							lvFiles.Items.Add(lvi);
						}
						else
							throw new Exception("Unexpected number of tokens");

					}



				}

				p.Dispose();
			}
			catch (Exception exception)
			{
				MessageBox.Show(exception.ToString());
			}
		}

		private void lvFiles_MouseDoubleClick(object sender,MouseEventArgs e)
		{
			//Launch the selected item in the shell... only support one selected
			//item though
			if (lvFiles.SelectedItems.Count != 0)
			{
				try
				{
					Process p = new Process();
					p.StartInfo.CreateNoWindow = true;
					p.StartInfo.UseShellExecute = false;
					p.StartInfo.RedirectStandardOutput = true;
					p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
					p.StartInfo.Arguments = "-x " + m_filename + " \"" + lvFiles.SelectedItems[0].Text + "\"";
					p.StartInfo.WorkingDirectory = System.IO.Path.GetTempPath();
					p.Start();
					p.WaitForExit();
					p.Dispose();

					p = new Process();
					p.StartInfo.CreateNoWindow = false;
					p.StartInfo.UseShellExecute = true;
					p.StartInfo.RedirectStandardOutput = false;
					p.StartInfo.FileName = System.IO.Path.GetTempPath() + lvFiles.SelectedItems[0].Text;
					p.Start();
					p.Dispose();
				}
				catch (Exception)
				{
				}

			}
		}

		private void btnRemoveFiles_Click(object sender,EventArgs e)
		{
			removeSelectedFiles();	
		}

		private void removeSelectedFiles()
		{
			//Remove selected files
			string filesToRemove = "";
			foreach (ListViewItem lvi in lvFiles.Items)
			{
				if (lvi.Selected)
					filesToRemove += " \"" + lvi.Text + "\"";
			}

			if (filesToRemove != "")
			{
				Process p = new Process();
				p.StartInfo.CreateNoWindow = true;
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.RedirectStandardOutput = true;
				p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
				p.StartInfo.Arguments = "-x " + m_filename + " \"" + lvFiles.SelectedItems[0].Text + "\"";
				p.StartInfo.WorkingDirectory = System.IO.Path.GetTempPath();
				p.Start();
				p.WaitForExit();
				p.Dispose();
			}
		}

		private void tsmiExtractSelectedFiles_Click(object sender,EventArgs e)
		{
			//Extract selected files
			string filesToExtract = "";
			foreach (ListViewItem lvi in lvFiles.Items)
			{
				if (lvi.Selected)
					filesToExtract += " \"" + lvi.Text + "\"";
			}

			if (filesToExtract != "")
			{
				try
				{
					//Get an extraction path
					FolderBrowserDialog fbd = new FolderBrowserDialog();
					if (fbd.ShowDialog() != DialogResult.OK)
						return;

					Process p = new Process();
					p.StartInfo.CreateNoWindow = true;
					p.StartInfo.UseShellExecute = false;
					p.StartInfo.RedirectStandardOutput = true;
					p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\xvpwad.exe";
					p.StartInfo.Arguments = "-x " + "\"" + m_filename + "\"" + filesToExtract;
					p.StartInfo.WorkingDirectory = fbd.SelectedPath;
					p.Start();
					string output = p.StandardOutput.ReadToEnd();
					p.WaitForExit();
					p.Dispose();
				}
				catch (Exception exception)
				{
					MessageBox.Show(exception.ToString());
				}
			}
		}
    }
}
