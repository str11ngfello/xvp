using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Text;

using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Diagnostics;


namespace XVPExport
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
        private Process m_colladaConvertProcess = new Process();
		private Process m_refineryProcess = new Process();
        private StreamReader m_streamReader;
		private bool m_autoClose = false;
		private bool m_leaveTemps = false;
		private string inputFilename = null;

        private System.Windows.Forms.Timer ReadTimer;
        private RichTextBox rtxtConsole;
        private IContainer components;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			Show();
			m_colladaConvertProcess.StartInfo.FileName = "XVPCollada.exe";
			m_refineryProcess.StartInfo.FileName = "refinery.bat";

			//Build a string out of the command line arguments 
			string args = "";
			for (int i = 1; i < Environment.GetCommandLineArgs().Length; ++i)
			{
				//Skip options that are meant only for XVPExport, don't send them
				//to XCollada
				if (Environment.GetCommandLineArgs()[i].Contains("-autoClose"))
				{
					m_autoClose = true;
					continue;
				}
				if (Environment.GetCommandLineArgs()[i].Contains("-leaveTemps"))
				{
					m_leaveTemps = true;
					continue;
				}
		
				//Wrap input and output paths with quotes to handle whitespace
				if (i == 2 || i == 4)
				{
					args += "\"" + Environment.GetCommandLineArgs()[i] + "\" ";

					//Save a copy of the input filename so we can delete it when we're done with it.
					//Remove the "file:///"
					if (i == 2)
					{
						inputFilename = Environment.GetCommandLineArgs()[i];
						inputFilename = inputFilename.Remove(0,8);
					}

					//Add the output to the titlebar
					if (i == 4)
					{
						Text = "XVP Export - " + Environment.GetCommandLineArgs()[i];
					}
				}
				else
					args += Environment.GetCommandLineArgs()[i] + " ";
				//MessageBox.Show(Environment.GetCommandLineArgs()[i]);
			}


			rtxtConsole.AppendText("Processing \"" + Environment.GetCommandLineArgs()[2] + "\"\n");
			rtxtConsole.AppendText("Converting COLLADA geometry... \n");
			m_refineryProcess.StartInfo.Arguments = "-i \"" + Environment.GetCommandLineArgs()[2] + "\" -o \"" + Environment.GetCommandLineArgs()[2] + "\" -x polygons2triangles";
			m_refineryProcess.StartInfo.CreateNoWindow = true;
			m_refineryProcess.StartInfo.UseShellExecute = false;
			m_refineryProcess.StartInfo.RedirectStandardOutput = true;
			m_refineryProcess.Start();
			m_refineryProcess.WaitForExit();

			rtxtConsole.AppendText("Deindexing COLLADA geometry... \n");

			m_refineryProcess.StartInfo.Arguments = "-i \"" + Environment.GetCommandLineArgs()[2] + "\" -o \"" + Environment.GetCommandLineArgs()[2] + "\" -x deindexer";
			m_refineryProcess.Start();
			m_refineryProcess.WaitForExit();

			rtxtConsole.AppendText("Optimizing COLLADA geometry... \n");

			m_refineryProcess.StartInfo.Arguments = "-i \"" + Environment.GetCommandLineArgs()[2] + "\" -o \"" + Environment.GetCommandLineArgs()[2] + "\" -x Optimizer";
			m_refineryProcess.Start();
			m_refineryProcess.WaitForExit();

			m_colladaConvertProcess.StartInfo.Arguments = args;
            m_colladaConvertProcess.StartInfo.CreateNoWindow = true;
            m_colladaConvertProcess.StartInfo.UseShellExecute = false;
            m_colladaConvertProcess.StartInfo.RedirectStandardOutput = true;
			m_colladaConvertProcess.Exited += new EventHandler(OnExitProcess);
			m_colladaConvertProcess.EnableRaisingEvents = true;

			//Start process
			rtxtConsole.AppendText("XVPCollada Start... \n\n");

            m_colladaConvertProcess.Start();
            
            // Do not wait for the child process to exit before
            // reading to the end of its redirected stream.
            // p.WaitForExit();
            // Read the output stream first and then wait.
            //string output = p.StandardOutput.ReadToEnd();
            //p.WaitForExit();
            m_streamReader = m_colladaConvertProcess.StandardOutput; 
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.rtxtConsole = new System.Windows.Forms.RichTextBox();
			this.ReadTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// rtxtConsole
			// 
			this.rtxtConsole.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.rtxtConsole.BackColor = System.Drawing.Color.White;
			this.rtxtConsole.Location = new System.Drawing.Point(12,12);
			this.rtxtConsole.Name = "rtxtConsole";
			this.rtxtConsole.ReadOnly = true;
			this.rtxtConsole.Size = new System.Drawing.Size(608,473);
			this.rtxtConsole.TabIndex = 1;
			this.rtxtConsole.Text = "";
			// 
			// ReadTimer
			// 
			this.ReadTimer.Enabled = true;
			this.ReadTimer.Interval = 1;
			this.ReadTimer.Tick += new System.EventHandler(this.ReadTimer_Tick);
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5,13);
			this.ClientSize = new System.Drawing.Size(632,497);
			this.Controls.Add(this.rtxtConsole);
			this.Name = "MainForm";
			this.Text = "XVP Export";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
        static void Main(string[] args) 
		{
			Application.Run(new MainForm());
		}

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
			if (!m_colladaConvertProcess.HasExited)
				m_colladaConvertProcess.Kill();
        }

        private void ReadTimer_Tick(object sender, EventArgs e)
        {
			if (m_streamReader == null)
				return;

			//Read in several lines one at a time.  This keeps the
			//reader up to speed with the output of the command line tool
			string finalString = "";
			string s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;
			s = m_streamReader.ReadLine();
			if (s != null)
				finalString += "\n" + s;

			if (finalString != null && finalString != "")
            {
				rtxtConsole.AppendText(finalString);

				/*string searchString = "Found";
				int index = finalString.IndexOf(searchString,0);

				while (index >= 0 && index < finalString.Length)
				{
					MessageBox.Show(index.ToString());
					rtxtConsole.Select(index,searchString.Length);
					rtxtConsole.SelectionColor = Color.Red;
					rtxtConsole.DeselectAll();

					index += searchString.Length;
					index = finalString.IndexOf(searchString,index);
				}*/
				
                rtxtConsole.ScrollToCaret();
            }

			Application.DoEvents();
        }

		public void OnExitProcess(object sender,EventArgs e)
		{
			//Delete the original input file unless instructed otherwise
			try
			{
				if (!m_leaveTemps)
					File.Delete(inputFilename);
			}
			catch (Exception execption)
			{
				MessageBox.Show("Couldn't delete input file " + inputFilename + "   " + execption.ToString());
			}

			if (m_colladaConvertProcess.ExitCode != 0)
				MessageBox.Show("Export failed. See the message log for details.","Export Failed",MessageBoxButtons.OK,MessageBoxIcon.Error);
			else if (m_autoClose)
				Application.Exit();
		}

	}
}
