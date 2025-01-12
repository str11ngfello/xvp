using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using DevComponents.Editors;
using XVPDesigner.UserControls;

using System.Net.Sockets;
using System.Net;
using TextureBrowserLibrary;
using XVPDesigner.Dialogs;
using XVPDesigner.Managers;
using XVPDesigner.Objects;
using System.CodeDom.Compiler;
using System.CodeDom;
using Microsoft.CSharp;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Threading;
using Microsoft.Win32;
using System.Diagnostics;
using XVPManaged;
using XVPDesigner.Documents;
using System.Runtime.InteropServices;
using System.Xml.XPath;
using XVPDesigner.Interfaces;
using Wizards;
using ICSharpCode.TextEditor;
using Dialogs;
using ICSharpCode.TextEditor.Document;


namespace XVPDesigner
{
	public partial class MainForm : DevComponents.DotNetBar.Office2007RibbonForm
	{
		//XVPM update thread
		Thread m_XVPMUpdateThread = null;
		ManualResetEvent m_XVPMUpdateStop = new ManualResetEvent(false);
		ManualResetEvent m_XVPMUpdateStopped = new ManualResetEvent(false);
		DateTime startTime = DateTime.Now;
		TimeSpan elapsedTime = new TimeSpan(0);

		
		private const int m_maxRecentItems = 25;
		private bool loadingComplete = false;

		//Explorer types
		public enum ExplorerType
		{
			Actor,
			Level
		}

		SplashDialog m_splash = null;

		Workspace m_currentWorkspace = null;
		string m_currentWorkspacePath = null;
		string m_currentActorPath = null;
		string m_currentStagePath = null;
		Direct3DLoader m_d3dLoader = null;


        private string m_xvpPath = null;
        public string XVPPath
        {
            get { return m_xvpPath; }
        }

		//Main application progress bar and status text available to whomever
		public ToolStripProgressBar MainProgressBar { get { return pbStatus; } }
		public String MainStatusText { set { lblStatusText.Text = value; } }

		//Scene Explorer Controls
		private SceneExplorer m_activeSceneExplorer;
		public SceneExplorer ActiveSceneExplorer { get { return m_activeSceneExplorer; } }

		//Property Explorer Controls
		private PropertyExplorer m_activePropertyExplorer;
		public PropertyExplorer ActivePropertyExplorer { get { return m_activePropertyExplorer; } }

		//Active XVPM control
		private uint m_activeXVPMHandle;
		public uint ActiveXVPMHandle { get { return m_activeXVPMHandle; } }

		//Library Explorer Controls
		public LibraryExplorer m_libraryExplorer;

		//Output Controls
		public Output m_output;

		//Graph Explorer Controls
		public GraphExplorer m_graphExplorer;

		//Global texture name list used to communicate between bitmap loader thread and texture
		//browser
		Material.TextureNameList m_queuedTexNameList;

		CustomDataBuilder m_customDataBuilder = new CustomDataBuilder();
		ShaderBuilder m_shaderBuilder = new ShaderBuilder();

		//One and only target manager object to manage network communications with target
		//device
		private TargetMgr m_targetMgr = new TargetMgr();

		//One and only plugin manager
		private PluginMgr m_pluginMgr = new PluginMgr();
		public PluginMgr PluginMgr
		{
			get { return m_pluginMgr; }
		}

		//One and only shader manager
		private ShaderMgr m_shaderMgr = new ShaderMgr();

		//One and only document manager
		public DocumentMgr m_docMgr = new DocumentMgr();

		private bool m_loadFormOK = false;

		//Plugin information dialog
		PluginDialog m_pluginDialog = null;
		public PluginDialog PluginDialog
		{
			get { return m_pluginDialog; }
		}

		public DevComponents.DotNetBar.TabControl PluginTools
		{
			get { return tcPluginTools; }
		}

		public DevComponents.DotNetBar.TabControl XVPMLockObject
		{
			get { return tcPluginTools; }
		}

		//Global image list for icons
		public ImageList Icons
		{
			get { return imglistGlobal; }
		}

		public void setSplashStatus(string statusText)
		{
			m_splash.setStatus(statusText);
		}

		public Workspace CurrentWorkspace
		{
			get { return m_currentWorkspace; }
		}

		public string CurrentWorkspacePath
		{
			get { return m_currentWorkspacePath; }
		}

		public string CurrentActorPath
		{
			get { return m_currentActorPath; }
		}

		public string CurrentStagePath
		{
			get { return m_currentStagePath; }
		}

		public MainForm()
		{
			//Create splash screen
			m_splash = new SplashDialog();
			m_splash.Show();
			m_splash.Refresh();
			m_splash.setStatus("Initializing...");
			InitializeComponent();
		}

		private void addRecentWorkspace(string workspacePath)
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentWorkspaces") != null)
			{
				//Only add if the last one added is different to keep
				//from having all recent items identical.  
				string recentWorkspaces = Convert.ToString(key.GetValue("RecentWorkspaces"));
				string[] workspaces = recentWorkspaces.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);

				if (workspaces.Length > 0)
				{
					if (workspaces[0] == workspacePath)
						return;
				}

				if (btnOpenRecentWorkspace.SubItems.Count >= m_maxRecentItems)
					btnOpenRecentWorkspace.SubItems.RemoveAt(btnOpenRecentWorkspace.SubItems.Count - 1);

				recentWorkspaces = workspacePath + "," + recentWorkspaces;

				int numCharsToRemove = 0;
				if (workspaces.Length != 0)
				{
					numCharsToRemove = workspaces[workspaces.Length - 1].Length + 1;
					recentWorkspaces.Remove(recentWorkspaces.Length - numCharsToRemove,numCharsToRemove);
				}
				key.SetValue("RecentWorkspaces",recentWorkspaces);
				ButtonItem bi = new ButtonItem();
				bi.Text = workspacePath;
				bi.Click += new System.EventHandler(handleOpenRecentWorkspace);
				btnOpenRecentWorkspace.SubItems.Insert(0,bi);
			}
			else
				key.SetValue("RecentWorkspaces",workspacePath);
		}

		public void addRecentActor(string actorPath)
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentActors") != null)
			{
				//Only add if the last one added is different to keep
				//from having all recent items identical.  
				string recentActors = Convert.ToString(key.GetValue("RecentActors"));
				string[] actors = recentActors.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);

				if (actors.Length > 0)
				{
					if (actors[0] == actorPath)
						return;
				}

				if (btnOpenRecentActor.SubItems.Count >= m_maxRecentItems)
					btnOpenRecentActor.SubItems.RemoveAt(btnOpenRecentActor.SubItems.Count - 1);

				recentActors = actorPath + "," + recentActors;

				int numCharsToRemove = 0;
				if (actors.Length != 0)
				{
					numCharsToRemove = actors[actors.Length - 1].Length + 1;
					recentActors.Remove(recentActors.Length - numCharsToRemove,numCharsToRemove);
				}
				key.SetValue("RecentActors",recentActors);
				ButtonItem bi = new ButtonItem();
				bi.Text = actorPath;
				bi.Click += new System.EventHandler(handleOpenRecentActor);
				btnOpenRecentActor.SubItems.Insert(0,bi);
			}
			else
				key.SetValue("RecentActors",actorPath);
		}

		public void addRecentStage(string stagePath)
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentStages") != null)
			{
				//Only add if the last one added is different to keep
				//from having all recent items identical.  
				string recentStages = Convert.ToString(key.GetValue("RecentStages"));
				string[] stages = recentStages.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);

				if (stages.Length > 0)
				{
					if (stages[0] == stagePath)
						return;
				}

				if (btnOpenRecentStage.SubItems.Count >= m_maxRecentItems)
					btnOpenRecentStage.SubItems.RemoveAt(btnOpenRecentStage.SubItems.Count - 1);

				recentStages = stagePath + "," + recentStages;

				int numCharsToRemove = 0;
				if (stages.Length != 0)
				{
					numCharsToRemove = stages[stages.Length - 1].Length + 1;
					recentStages.Remove(recentStages.Length - numCharsToRemove,numCharsToRemove);
				}
				key.SetValue("RecentStages",recentStages);
				ButtonItem bi = new ButtonItem();
				bi.Text = stagePath;
				bi.Click += new System.EventHandler(handleOpenRecentStage);
				btnOpenRecentStage.SubItems.Insert(0,bi);
			}
			else
				key.SetValue("RecentStages",stagePath);
		}

		public void addRecentScript(string scriptPath)
		{
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentScripts") != null)
			{
				//Only add if the last one added is different to keep
				//from having all recent items identical.  
				string recentScripts = Convert.ToString(key.GetValue("RecentScripts"));
				string[] scripts = recentScripts.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);

				if (scripts.Length > 0)
				{
					if (scripts[0] == scriptPath)
						return;
				}

				if (btnOpenRecentScript.SubItems.Count >= m_maxRecentItems)
					btnOpenRecentScript.SubItems.RemoveAt(btnOpenRecentScript.SubItems.Count - 1);

				recentScripts = scriptPath + "," + recentScripts;

				int numCharsToRemove = 0;
				if (scripts.Length != 0)
				{
					numCharsToRemove = scripts[scripts.Length - 1].Length + 1;
					recentScripts.Remove(recentScripts.Length - numCharsToRemove,numCharsToRemove);
				}
				key.SetValue("RecentScripts",recentScripts);
				ButtonItem bi = new ButtonItem();
				bi.Text = scriptPath;
				bi.Click += new System.EventHandler(handleOpenRecentScript);
				btnOpenRecentScript.SubItems.Insert(0,bi);
			}
			else
				key.SetValue("RecentScripts",scriptPath);
		}
		
		private void fillRecentMenus()
		{
			//Workspaces
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentWorkspaces") != null)
			{
				string recentWorkspaces = Convert.ToString(key.GetValue("RecentWorkspaces"));
				string[] workspaces = recentWorkspaces.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);
				foreach (string s in workspaces)
				{
					if (btnOpenRecentWorkspace.SubItems.Count < m_maxRecentItems)
					{
						ButtonItem bi = new ButtonItem();
						bi.Text = s;
						bi.Click += new System.EventHandler(handleOpenRecentWorkspace);
						btnOpenRecentWorkspace.SubItems.Add(bi);
					}
				}
			}
			else
				key.SetValue("RecentWorkspaces","");

			//Actors
			key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentActors") != null)
			{
				string recentActors = Convert.ToString(key.GetValue("RecentActors"));
				string[] actors = recentActors.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);
				foreach (string s in actors)
				{
					if (btnOpenRecentActor.SubItems.Count < m_maxRecentItems)
					{
						ButtonItem bi = new ButtonItem();
						bi.Text = s;
						bi.Click += new System.EventHandler(handleOpenRecentActor);
						btnOpenRecentActor.SubItems.Add(bi);
					}
				}
			}
			else
				key.SetValue("RecentActors","");

			//Stages
			key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentStages") != null)
			{
				string recentStages = Convert.ToString(key.GetValue("RecentStages"));
				string[] stages = recentStages.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);
				foreach (string s in stages)
				{
					if (btnOpenRecentStage.SubItems.Count < m_maxRecentItems)
					{
						ButtonItem bi = new ButtonItem();
						bi.Text = s;
						bi.Click += new System.EventHandler(handleOpenRecentStage);
						btnOpenRecentStage.SubItems.Add(bi);
					}
				}
			}
			else
				key.SetValue("RecentStages","");

			//Scripts
			key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("RecentScripts") != null)
			{
				string recentScripts = Convert.ToString(key.GetValue("RecentScripts"));
				string[] scripts = recentScripts.Split(new char[] { ',' },StringSplitOptions.RemoveEmptyEntries);
				foreach (string s in scripts)
				{
					if (btnOpenRecentScript.SubItems.Count < m_maxRecentItems)
					{
						ButtonItem bi = new ButtonItem();
						bi.Text = s;
						bi.Click += new System.EventHandler(handleOpenRecentScript);
						btnOpenRecentScript.SubItems.Add(bi);
					}
				}
			}
			else
				key.SetValue("RecentScripts","");
		}

		private void handleOpenRecentWorkspace(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the path to the
			//recent item
			ButtonItem buttonClicked = (ButtonItem)sender;

			//Does the item still exist
			if (!File.Exists(buttonClicked.Text))
			{
				MessageBox.Show("Could not find file \"" + buttonClicked.Text + "\"","File Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			if (m_currentWorkspace.Dirty)
			{
				DialogResult dr = MessageBox.Show("The current workspace has been modified.  Would you like to save the current workspace?","Workspace Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				bool successfulSave = true;
				switch (dr)
				{
					case DialogResult.Yes:
						successfulSave = saveWorkspace();
						break;
					case DialogResult.Cancel:
						return;
				}
				if (!successfulSave)
					return;
			}

			openWorkspaceFromFile(buttonClicked.Text);
		}

		private void handleOpenRecentActor(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the path to the
			//recent item
			ButtonItem buttonClicked = (ButtonItem)sender;

			//Does the item still exist
			if (!File.Exists(buttonClicked.Text))
			{
				MessageBox.Show("Could not find file \"" + buttonClicked.Text + "\"","File Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			openActorFromFile(buttonClicked.Text);
		}

		private void handleOpenRecentStage(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the path to the
			//recent item
			ButtonItem buttonClicked = (ButtonItem)sender;

			//Does the item still exist
			if (!File.Exists(buttonClicked.Text))
			{
				MessageBox.Show("Could not find file \"" + buttonClicked.Text + "\"","File Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			openStageFromFile(buttonClicked.Text);
		}

		private void handleOpenRecentScript(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the path to the
			//recent item
			ButtonItem buttonClicked = (ButtonItem)sender;

			//Does the item still exist
			if (!File.Exists(buttonClicked.Text))
			{
				MessageBox.Show("Could not find file \"" + buttonClicked.Text + "\"","File Not Found",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			openScriptFromFile(buttonClicked.Text);
		}
		
		private void loadLastWorkspaceCheck()
		{
			//Should we load last workspace?
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			bool loadLastWorkspace = false;
			string lastWorkspace = null;
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (key.GetValue("LoadLastWorkspace") != null)
			{
				loadLastWorkspace = Convert.ToBoolean(key.GetValue("LoadLastWorkspace"));
				btnLoadLastWorkspace.Checked = loadLastWorkspace;
				if (key.GetValue("LastWorkspace") != null)
					lastWorkspace = (string)key.GetValue("LastWorkspace");

				//Can't load the last workspace if there was none
				if (lastWorkspace == null)
					loadLastWorkspace = false;
			}
			else
				key.SetValue("LoadLastWorkspace",true);

			if (loadLastWorkspace)
			{
				FileStream fs = null;
				try
				{
					fs = File.Open(lastWorkspace,FileMode.Open,FileAccess.Read);
					BinaryFormatter bf = new BinaryFormatter();

					m_currentWorkspace = bf.Deserialize(fs) as Workspace;
					m_currentWorkspacePath = lastWorkspace;
				}
				catch (Exception)
				{
					m_currentWorkspace = new Workspace();
				}
				finally
				{
					if (fs != null)
						fs.Close();
				}
			}
			else
				m_currentWorkspace = new Workspace();

			applyWorkspace(m_currentWorkspace);
		}

		/*private void loadSampleWorkspace()
		{
			FileStream fs = null;
			try
			{
				fs = File.Open("Sample Workspace\\Sample.wrk",FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();

				m_currentWorkspace = bf.Deserialize(fs) as Workspace;
				m_currentWorkspacePath = "Sample Workspace\\Sample.wrk";
			}
			catch (Exception)
			{
				m_currentWorkspace = new Workspace();
			}
			finally
			{
				if (fs != null)
					fs.Close();
			}
		}*/

		private void MainForm_Load(object sender,EventArgs e)
		{
            //system directory, if we don't have it, bail
            // First, let's make sure there is a valid XVP installation
            string regKey = "Software\\Seventh Night Studios\\XVP";
            RegistryKey key = Registry.LocalMachine.OpenSubKey(regKey, false);
            if (key == null)
            {
                MessageBox.Show("Could not locate XVP registry key in local machine \"" + regKey + "\".  Run the XVP installer or create the key manually.", "XVP Key Not Found", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            }

            string installFolder = "Install Folder";
            m_xvpPath = (string)key.GetValue(installFolder);
            if (m_xvpPath == null)
            {
                MessageBox.Show("Could not locate XVP installation folder \""+installFolder+"\" in the registry.  Run the XVP installer or create the key manually under \"" + regKey + "\"", "XVP Install Folder Key Not Found", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit(); ;
            }

            // 
            // texBrowser
            // 
            this.texBrowser = new TextureBrowserLibrary.TextureBrowser();

            this.texBrowser.BackgroundColor = System.Drawing.SystemColors.Control;
            this.texBrowser.BackgroundColorSelected = System.Drawing.SystemColors.Highlight;
            this.texBrowser.CacheImages = true;
            this.texBrowser.CanvasColor = System.Drawing.SystemColors.ControlDark;
            this.texBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.texBrowser.ForegroundColor = System.Drawing.SystemColors.ControlText;
            this.texBrowser.ForegroundColorSelected = System.Drawing.SystemColors.HighlightText;
            this.texBrowser.Location = new System.Drawing.Point(0, 0);
            this.texBrowser.Name = "texBrowser";
            this.texBrowser.Selection = TextureBrowserLibrary.TextureBrowser.SelectionMode.Single;
            this.texBrowser.Size = new System.Drawing.Size(372, 101);
            this.texBrowser.TabIndex = 1;
			m_d3dLoader = new Direct3DLoader(this.Handle);

			//Fill out recent item menus
			fillRecentMenus();

			//Set some default settings
			btnUseDefaultLights.Checked = true;
			btnUseLights.Text = btnUseDefaultLights.Text;
			btnLightIntensity.Enabled = false;
			btnLightColor.Enabled = false;
			btnSetLightScenario.Enabled = false;
			cbLightAnimSpeed.Enabled = false;
			sliderLightAngle.Enabled = false;
			sliderLightAngle.TextColor = Color.DimGray;
			chkLightShadows.Enabled = false;
			cbLightAnimSpeed.SelectedItem = ciNoLightAnim;
			cbiMaxTexSize.SelectedItem = ci5;
			cbAOMaxTexSize.SelectedItem = ci5;

			//Apply last workspace if necessary.
			loadLastWorkspaceCheck();

			//Create a plugin information dialog to be populated later
			m_pluginDialog = new PluginDialog();

			//Create library explorer
			m_splash.setStatus("Loading Templates...");
			m_libraryExplorer = new LibraryExplorer();
			m_libraryExplorer.Dock = DockStyle.Fill;
			//m_libraryExplorer.Enabled = false;
			TemplateDockContainer.Controls.Add(m_libraryExplorer);
			//TemplateDockContainer.Controls.Add(new SplineEditor());
			//TemplateDockContainer.Controls[1].BringToFront();

			//Create output controls
			m_output = new Output();
			m_output.Dock = DockStyle.Fill;
			OutputDockContainer.Controls.Add(m_output);

			//Create graph explorer
			m_graphExplorer = new GraphExplorer();
			m_graphExplorer.Dock = DockStyle.Fill;
			GraphExplorerDockContainer.Controls.Add(m_graphExplorer);

			//Load layouts
			if (File.Exists(Application.StartupPath + "\\dock.layout"))
				DotNetBarManager.LoadLayout(Application.StartupPath + "\\dock.layout");
			if (File.Exists(Application.StartupPath + "\\qat.layout"))
			{
				FileStream fs = File.Open(Application.StartupPath + "\\qat.layout",FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				ribbonMainRibbon.QatLayout = (string)bf.Deserialize(fs);
				fs.Close();
			}

			//Initialize plugin manager
			m_splash.setStatus("Loading Plugins...");
			try
			{
				m_pluginMgr.init(btnCreateActorNode,btnAttachActorController,btnAddPostEffect);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encountered while initializing plugins.  Check workspace path settings to verify they are correct.  See the message log for details.","Plugin Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Initialize shader manager/builder and populate scene explorers with shader information.
			//First though, check the registry to see if we should delete fx plugin dlls so the shader
			//manager will rebuild them all
			try
			{
				key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
				if (key == null)
					key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
				object rebuildPlugins = key.GetValue("RebuildPlugins");
				if (rebuildPlugins != null)
				{
					if (rebuildPlugins.ToString() == "RebuildAll")
					{
						foreach (string f in Directory.GetFiles(Application.StartupPath,"*.cdd.dll"))
							File.Delete(f);
						foreach (string f in Directory.GetFiles(Application.StartupPath,"*.fx.dll"))
							File.Delete(f);
					}
					else if (rebuildPlugins.ToString() == "RebuildFX")
					{
						foreach (string f in Directory.GetFiles(Application.StartupPath,"*.fx.dll"))
							File.Delete(f);
					}
					else if (rebuildPlugins.ToString() == "RebuildCDD")
					{
						foreach (string f in Directory.GetFiles(Application.StartupPath,"*.cdd.dll"))
							File.Delete(f);
					}
				}
				key.SetValue("RebuildPlugins","none");
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while reading registry. See the message log for details.","Registry Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}
			m_splash.setStatus("Loading Shaders...");
			try
			{
				//Shader building should be initialized first
				m_shaderBuilder.init();	
				m_shaderMgr.init(m_currentWorkspace.ShaderPath);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while initializing shaders.  Check workspace path settings to verify they are correct.  See the message log for details.","Shader Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Initialize document manager 
			m_splash.setStatus("Loading Document Manager...");
			try
			{
				m_docMgr.init();
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while initializing Document Manager.  Check workspace path settings to verify they are correct.  See the message log for details.","Document Manager Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Initialize custom data builder
			m_splash.setStatus("Loading Custom Data Definitions...");
			try
			{
				m_customDataBuilder.init(m_currentWorkspace.CDDPath,btnCreateActorNode);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while initializing cdds.  Check workspace path settings to verify they are correct.  See the message log for details.","Custom Data Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Initialize the library explorer
			try
			{
				m_libraryExplorer.init(m_currentWorkspace.ActorLibraryPath,
									   m_currentWorkspace.MaterialLibraryPath,
									   m_currentWorkspace.ControllerLibraryPath,
									   m_currentWorkspace.AudioLibraryPath,
									   m_currentWorkspace.TextureLibraryPath,
									   m_currentWorkspace.ScriptLibraryPath);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while initializing the Library Explorer.  Check workspace path settings to verify they are correct.  See the message log for details.","Library Explorer Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Scan directory for actor templates...
			try
			{
				cbActorTemplate.Items.Add("No Template");
				foreach (string filename in Directory.GetFiles(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath))
				{
					cbActorTemplate.Items.Add(Path.GetFileNameWithoutExtension(filename));
				}
				
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while scanning for Actor Templates.  Check workspace path settings to verify they are correct.  See the message log for details.","Library Explorer Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}
			cbActorTemplate.SelectedItem = "No Template";

			//Initialize XVPM
			m_splash.setStatus("Initializing XVPM...");
			try
			{
				if (!XVPM.inited())
					XVPM.init(Process.GetCurrentProcess().Handle);
				
				m_XVPMUpdateThread = new Thread(new ThreadStart(XVPMUpdate));
				m_XVPMUpdateThread.Start();

				//If we regenerated any shaders than we need to reload the wad file
				if (m_shaderBuilder.RegenerationOccurred)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						IntPtr pWadFile = Marshal.StringToHGlobalAnsi(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\ShaderDesigner.wad");
						XVPM.loadShaderWad(pWadFile);
						Marshal.FreeHGlobal(pWadFile);
					}
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while initializing XVPM.  See the message log for details.","XVMP Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			//Initialize tool layout
			if (m_currentWorkspace.ShowStartPage)
				btnDesignerHome_Click(null,null);

			//Ensure ribbon is bottom most window and redraw it.  
			SuspendLayout();
			ribbonMainRibbon.BringToFront();
			ribbonMainRibbon.SendToBack();
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabWorkspace;
			ResumeLayout();

			m_loadFormOK = true;
			m_splash.Hide();

			loadingComplete = true;

			//Set the initial gui enablements
			enableActorStudio(false);
			enableStageStudio(false,null);
		}

		private void btnImportModelNoLODs_Click(object sender,EventArgs e)
		{
			if (dlgOpenFileDialog.ShowDialog() == DialogResult.OK)
			{
				//Make sure this model hasn't already been imported
				/*TreeNode[] tempNodes = m_tvSceneNodes.Nodes.Find(dlgOpenFileDialog.FileName,true);
				if (tempNodes.Length != 0)
				{
					MessageBox.Show(dlgOpenFileDialog.FileName + " has already been imported.","Model Import Error");
					return;
				}*/



				/*TreeNode treeNode = ImportMgr.importModel(false,dlgOpenFileDialog.FileName);


				
				m_tvSceneNodes.Nodes.Add(treeNode);*/

				//m_tvSceneNodes.ExpandAll();
				//m_tvSceneMaterials.ExpandAll();
			}
		}

		private void btnTargetSettings_PopupFinalized(object sender,EventArgs e)
		{
			//Set network settings
			ComboItem ci = cbTargetDevice.SelectedItem as ComboItem;
			if (ci != null)
				m_targetMgr.device = ci.Text;
			m_targetMgr.ipAddress = txtTargetIPAddress.ControlText;
			m_targetMgr.port = txtTargetPort.ControlText;
		}

		private void btnTargetConnect_Click(object sender,EventArgs e)
		{
			//Do have any errors?
			if ((errorProvider.GetError(txtTargetIPAddress.TextBox) != "") ||
				(errorProvider.GetError(txtTargetPort.TextBox) != ""))
			{
				MessageBox.Show("The target connection settings are not valid.  Correct the settings and try again.","Invalid Settings",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			//Disconnect in case we have a current connection, then re-connect.
			m_targetMgr.disconnect();
			m_targetMgr.connect();
		}

		public bool sendTargetCommand(string cmdString)
		{
			return m_targetMgr.sendCommand(cmdString);
		}

		public ShaderMgr getShaderMgr()
		{
			return m_shaderMgr; 
		}

		private void btnAbout_Click(object sender,EventArgs e)
		{
			m_splash.EnableCloseButton = true;
			m_splash.ShowDialog();
		}

		private void btnHelpContents_Click(object sender,EventArgs e)
		{
			
		}


		public void logMessage(string message,Color color)
		{
			m_output.logMessage(message,color);
		}

		private void cbSkinColor_Click(object sender,EventArgs e)
		{
			ComboItem ci = cbSkinColor.SelectedItem as ComboItem;
			if (ci == null)
				return;

			if (ci.Text == "Black")
				RibbonPredefinedColorSchemes.ChangeOffice2007ColorTable(this,DevComponents.DotNetBar.Rendering.eOffice2007ColorScheme.Black);
			else if (ci.Text == "Blue")
				RibbonPredefinedColorSchemes.ChangeOffice2007ColorTable(this,DevComponents.DotNetBar.Rendering.eOffice2007ColorScheme.Blue);
			else if (ci.Text == "Silver")
				RibbonPredefinedColorSchemes.ChangeOffice2007ColorTable(this,DevComponents.DotNetBar.Rendering.eOffice2007ColorScheme.Silver);
			else
				RibbonPredefinedColorSchemes.ChangeOffice2007ColorTable(this,DevComponents.DotNetBar.Rendering.eOffice2007ColorScheme.Black);
			m_currentWorkspace.SkinColor = cbSkinColor.SelectedIndex;
		}

		private void btnCleanPlugins_Click(object sender,EventArgs e)
		{
			//Delete all compiled plugins
			foreach (string filename in Directory.GetFiles(Application.StartupPath))
			{
				try
				{
					File.Delete(filename);
				}
				catch (Exception exception)
				{
					logMessage("Could not delete plugin object file \"" + filename + "\"",Color.Red);
					logMessage(exception.ToString(),Color.Red);
				}
			}
		}

		private void ribtabActorStudio_Click(object sender,EventArgs e)
		{


		}

		private void ribtabStageStudio_Click(object sender,EventArgs e)
		{

		}

		private void btnSaveActor_Click(object sender,EventArgs e)
		{
			saveActor();
		}

		private void btnSaveStage_Click(object sender,EventArgs e)
		{
			saveStage();
		}

		public bool openActorFromFile(string filename)
		{
			//Check to make sure the document is not already open
			if (m_docMgr.isDocumentOpen(filename))
			{
				MessageBox.Show("The document " + filename + " is already open.","Document Already Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return false;
			}

			//Document is not open so create a new tab and a document to load on the tab
			ActorStudioDocument doc = new ActorStudioDocument();

			tcPluginTools.SuspendLayout();
			TabItem tab = tcPluginTools.CreateTab(Path.GetFileName(filename));
			tab.Name = filename;

			//If the document open was a success finish loading the tab, otherwise
			//clean up and exit
			if (doc.open(tab,filename))
			{
				m_docMgr.addDocument(filename,doc);
				m_currentActorPath = filename;
				
				//Set explorers
				setSceneExplorer(doc.SceneExplorer);
				setPropertyExplorer(doc.PropertyExplorer);
				setXVPMHandle(doc.XVPMHandle);
				setActionExplorer(null);

				//Post load could fail, if so clean up the document
				if (!doc.postLoad())
				{
					doc.close();
					m_docMgr.Documents.Remove(filename);
					tcPluginTools.Tabs.Remove(tab);
					tab.Dispose();
					tab = null;
					tcPluginTools.ResumeLayout();
					tcPluginTools.Refresh();

					return false;
				}

				enableActorStudio(false);
				enableStageStudio(true,doc.XVPMControl);
				tcPluginTools.SelectedTab = tab;

				doc.setActive(true);				
			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}
			tcPluginTools.ResumeLayout();
			if (tab != null)
				(tab.AttachedControl.Controls[0] as XVPMControl).showRendering();

			return true;
		}

		private void openStageFromFile(string filename)
		{
			//Check to make sure the document is not already open
			if (m_docMgr.isDocumentOpen(filename))
			{
				MessageBox.Show("The document " + filename + " is already open.","Document Already Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			//Document is not open so create a new tab and a document to load on the tab
			StageStudioDocument doc = new StageStudioDocument();

			tcPluginTools.SuspendLayout();
			TabItem tab = tcPluginTools.CreateTab(Path.GetFileName(filename));
			tab.Name = filename;

			//If the document open was a success finish loading the tab, otherwise
			//clean up and exit
			if (doc.open(tab,filename))
			{
				m_docMgr.addDocument(filename,doc);
				m_currentStagePath = filename;
				doc.setActive(true);

				//Set explorers
				setSceneExplorer(doc.SceneExplorer);
				setPropertyExplorer(doc.PropertyExplorer);
				setXVPMHandle(doc.XVPMHandle);
				setActionExplorer(null);
				enableActorStudio(false);
				enableStageStudio(true,doc.XVPMControl);

				tcPluginTools.SelectedTab = tab;

				doc.postLoad();

			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}
			tcPluginTools.ResumeLayout();
			if (tab != null)
				(tab.AttachedControl.Controls[0] as XVPMControl).showRendering();
		}

		public void openScriptFromFile(string filename)
		{
			//Check to make sure the document is not already open
			if (m_docMgr.isDocumentOpen(filename))
			{
				MessageBox.Show("The document " + filename + " is already open.","Document Already Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return;
			}

			//Document is not open so create a new tab and a document to load on the tab
			ScriptDocument doc = new ScriptDocument();

			tcPluginTools.SuspendLayout();
			TabItem tab = tcPluginTools.CreateTab(Path.GetFileName(filename));
			tab.Name = filename;

			//If the document open was a success finish loading the tab, otherwise
			//clean up and exit
			if (doc.open(tab,filename))
			{
				m_docMgr.addDocument(filename,doc);
				m_currentActorPath = filename;

				//Set explorers
				setSceneExplorer(null);
				setPropertyExplorer(null);
				//setXVPMHandle(doc.XVPMHandle);
				setActionExplorer(null);

				doc.postLoad();
				enableActorStudio(false);
				enableStageStudio(false,null);
				tcPluginTools.SelectedTab = tab;

				doc.setActive(true);
			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}
			tcPluginTools.ResumeLayout();
		}
		
		private void btnOpenActor_Click(object sender,EventArgs e)
		{
			dlgOpenFileDialog.Title = "Open Actor...";
			dlgOpenFileDialog.Filter = "Actor Files (*.act)|*.act|All files (*.*)|*.*";
			dlgOpenFileDialog.FileName = "";
			dlgOpenFileDialog.InitialDirectory = m_currentWorkspace.ActorLibraryPath;
			DialogResult result = dlgOpenFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				openActorFromFile(dlgOpenFileDialog.FileName);
		}

		private void btnOpenStage_Click(object sender,EventArgs e)
		{
			dlgOpenFileDialog.Title = "Open Stage...";
			dlgOpenFileDialog.Filter = "Stage Files (*.sta)|*.sta|All files (*.*)|*.*";
			dlgOpenFileDialog.FileName = "";
			dlgOpenFileDialog.InitialDirectory = m_currentWorkspace.ActorLibraryPath;
			DialogResult result = dlgOpenFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				openStageFromFile(dlgOpenFileDialog.FileName);
		}

		private void btnNewActor_Click(object sender,EventArgs e)
		{
			//Create a new Actor Studio document
			ActorStudioDocument doc = new ActorStudioDocument();

			int ctr = 1;
			string newName = null;
			while (true)
			{
				newName = "Untitled" + ctr + ".act";
				if (m_docMgr.Documents[newName] == null)
					break;
				ctr++;
			}

			TabItem tab = tcPluginTools.CreateTab(newName);
			tcPluginTools.SuspendLayout();
			tab.Name = newName;

			if (doc.create(tab,newName))
			{
				m_docMgr.addDocument(newName,doc);
				doc.setActive(true);

				//Set explorers
				setSceneExplorer(doc.SceneExplorer);
				setPropertyExplorer(doc.PropertyExplorer);
				setXVPMHandle(doc.XVPMHandle);
				setActionExplorer(null);
				enableActorStudio(true);
				enableStageStudio(false,null);

				tcPluginTools.SelectedTab = tab;
			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}

			//If we lock XVPM while adding the tab we get a much cleaner
			//tab add visually
			lock (XVPMLockObject)
			{
				tcPluginTools.ResumeLayout();
				if (tab != null)
					(tab.AttachedControl.Controls[0] as XVPMControl).showRendering();
			}
		}

		private void btnNewStage_Click(object sender,EventArgs e)
		{
			//Create a new Stage Studio document
			StageStudioDocument doc = new StageStudioDocument();

			int ctr = 1;
			string newName = null;
			while (true)
			{
				newName = "Untitled" + ctr + ".sta";
				if (m_docMgr.Documents[newName] == null)
					break;
				ctr++;
			}

			TabItem tab = tcPluginTools.CreateTab(newName);
			tcPluginTools.SuspendLayout();
			tab.Name = newName;

			if (doc.create(tab,newName))
			{
				m_docMgr.addDocument(newName,doc);
				doc.setActive(true);

				//Set explorers
				setSceneExplorer(doc.SceneExplorer);
				setPropertyExplorer(doc.PropertyExplorer);
				setActionExplorer(null);
				setXVPMHandle(doc.XVPMHandle);
				enableActorStudio(false);
				enableStageStudio(true,doc.XVPMControl);

				tcPluginTools.SelectedTab = tab;
			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}

			//If we lock XVPM while adding the tab we get a much cleaner
			//tab add visually
			lock (XVPMLockObject)
			{
				tcPluginTools.ResumeLayout();
				if (tab != null)
					(tab.AttachedControl.Controls[0] as XVPMControl).showRendering();
			}
		}

		public void initProgressBar(string statusText,int max)
		{
			lblStatusText.Text = statusText;
			//lblStatusText.Refresh();
			pbStatus.Minimum = 0;
			pbStatus.Maximum = max;
			pbStatus.Value = 0;
			pbStatus.Step = 1;
			//pbStatus.Refresh();
		}

		public void setSceneExplorer(Control sceneExplorer)
		{
			m_activeSceneExplorer = sceneExplorer as SceneExplorer;
			
			SceneDockContainer.Controls.Clear();
			if (sceneExplorer != null)
				SceneDockContainer.Controls.Add(sceneExplorer);
		}

		public void setPropertyExplorer(Control propertyExplorer)
		{
			m_activePropertyExplorer = propertyExplorer as PropertyExplorer;
			
			PropertyDockContainer.Controls.Clear();
			if (propertyExplorer != null)
			{
				PropertyDockContainer.Controls.Add(propertyExplorer);
				((PropertyExplorer)propertyExplorer).setNodes(((PropertyExplorer)propertyExplorer).getNodes());
			}
		}

		public void setXVPMHandle(uint XVPMHandle)
		{
			m_activeXVPMHandle = XVPMHandle;
		}

		public void setActionExplorer(SceneNode sceneNode)
		{
			//Clear the previous control
			ActionDockContainer.Controls.Clear();

			//First, grab the action explorer user control from the plugin
			ActionExplorerBase actionExplorer = m_pluginMgr.getActionExplorer(sceneNode);
			
			//There might not be a user control to set for the new node, that's ok.
			if (actionExplorer == null)
				return;
			

			//It must be castable to a UserControl.
			UserControl uc = actionExplorer as UserControl;
			
			//There might not be a user control to set for the new node, that's ok.
			if (uc != null)
			{
				uc.Dock = DockStyle.Fill;
				ActionDockContainer.Controls.Add(uc);
			}
			else
				throw new Exception("The Action Explorer for Scene Node \"" + sceneNode.Name + "\" is not castable to a UserControl.");
		}

		public void refreshActionExplorer(SceneNode sceneNode)
		{
			//If the scene node passed in is currently set in the action explorer
			//refresh it.  If the node is in the property explorer, we should refresh.
			if (m_activePropertyExplorer.SelectedObject == sceneNode)
				setActionExplorer(sceneNode);
		}

		public string buildValidCodeString(string codeString)
		{
			if (codeString == null)
				return null;

			//Check for any character that isn't alphanumerica and replace it with
			//an underscore
			char[] chars = codeString.ToCharArray();
			for (int i = 0; i < chars.Length; ++i)
			{
				if (char.IsDigit(chars[i]) || char.IsLetter(chars[i]))
					continue;
				else
					chars[i] = '_';
			}

			string validCodeString = new string(chars);

			//The first char cannot be a number
			if (char.IsNumber(validCodeString[0]))
				validCodeString = "_" + validCodeString;

			return validCodeString;
		}

		public bool validateNodeName(string nodeName)
		{
			//Check for any character that isn't alphanumerica 
			for (int i = 0; i < nodeName.Length; ++i)
			{
				if (char.IsDigit(nodeName[i]) || char.IsLetter(nodeName[i]) || char.IsWhiteSpace(nodeName[i]))
					continue;
				else
					return false;
			}

			return true;
		}

		private void btnExportActor_Click(object sender,EventArgs e)
		{
			ActorStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			if (doc == null)
				throw new Exception("Expected an actor studio document to be the current document");

			//Verify if an actor template is selected.  If we fail the verification, we cannot export
			if (cbActorTemplate.SelectedItem.ToString() != "No Template")
			{
				if (!VerifyActorTemplateDialog.silentVerify(cbActorTemplate.SelectedItem.ToString(),ActiveSceneExplorer))
				{
					DialogResult dr = MessageBox.Show("The actor failed the template verification process.  View verification results?","Verification Failed?",MessageBoxButtons.YesNo,MessageBoxIcon.Error);
					if (dr == DialogResult.Yes)
						btnVerifyActor_Click(null,null);
					return;
				}
			}

			//Make sure the actor has been saved before the export
			if (doc.ActorFilename == null || doc.Dirty)
			{
				DialogResult dr = MessageBox.Show("The actor must be saved before export.  Save?","Save Before Export?",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
				if (dr == DialogResult.Yes)
					saveActor();
				else
					return;
			}

			doc.export();
		}

		private void btnExportStage_Click(object sender,EventArgs e)
		{
			StageStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument;
			if (doc == null)
				throw new Exception("Expected a stage studio document to be the current document");

			//Make sure the actor has been saved before the export
			if (doc.StageFilename == null || doc.Dirty)
			{
				DialogResult dr = MessageBox.Show("The stage must be saved before export.  Save?","Save Before Export?",MessageBoxButtons.YesNo,MessageBoxIcon.Information);
				if (dr == DialogResult.Yes)
					saveStage();
				else
					return;
			}

			doc.export();
		}

		public void clearTextureBrowser()
		{
			if (bitmapLoadWorker.IsBusy)
				bitmapLoadWorker.CancelAsync();

			Control[] controls = TextureExplorerContainer.Controls.Find("texBrowser",true);
			if (controls != null && controls.Length >= 1)
			{
				TextureBrowser tbl = controls[0] as TextureBrowser;
				if (tbl != null)
					tbl.Clear();
			}
		}

		public void refreshTextureBrowser(Material.TextureNameList texNameList)
		{
			//First, check to see if we are processing a previous refresh.  If so,
			//cancel the job so we can start a new one
			if (bitmapLoadWorker.IsBusy)
			{
				bitmapLoadWorker.CancelAsync();
				m_queuedTexNameList = texNameList;
			}
			else
			{
				Control[] controls = TextureExplorerContainer.Controls.Find("texBrowser",true);
				if (controls != null && controls.Length >= 1)
				{
					TextureBrowser tbl = controls[0] as TextureBrowser;
					if (tbl != null)
						tbl.Clear();
				}
				if (texNameList != null)
					bitmapLoadWorker.RunWorkerAsync(texNameList);
			}
		}

		private void bitmapLoadWorker_DoWork(object sender,DoWorkEventArgs e)
		{
			BackgroundWorker bgWorker = (BackgroundWorker)sender;
			Material.TextureNameList texNameList = e.Argument as Material.TextureNameList;
			for (int i = 0; i < texNameList.names.Count; ++i)
			{
				//Check for cancel event
				if (bgWorker.CancellationPending)
				{
					e.Cancel = true;
					return;
				}

				if (texNameList.filenames[i] == null)
				{
					//Add a null bitmap and a blank image info structure.
					texNameList.bitmaps.Add(null);
					texNameList.imageInfos.Add(new Microsoft.Xna.Framework.Graphics.TextureInformation());
				}
				else
				{
					//Try to load the bitmap in...if it fails for some reason,
					//then the preview will not be available.  Most likely a format
					//may not be supported if this happens.  E.g. tga.  Or,
					//the image does not exist.
					Bitmap bitmap = null;
					try
					{
						Microsoft.Xna.Framework.Graphics.TextureInformation imageInfo;
						bitmap = m_d3dLoader.Load((string)texNameList.filenames[i],out imageInfo);// new Bitmap((string)texNameList.filenames[i]);
						texNameList.bitmaps.Add(bitmap);
						texNameList.imageInfos.Add(imageInfo);
					}
					catch (Exception) { }

				}
				//Pause for while to keep the UI responsive
				Thread.Sleep(33);
			}
			e.Result = texNameList;
		}

		private void bitmapLoadWorker_RunWorkerCompleted(object sender,RunWorkerCompletedEventArgs e)
		{
			//If we were cancelled, then stop the thread.  Also, check to see if there is
			//a queued texture name list to process.  If so, process it.
			if (e.Cancelled)
			{
				if (m_queuedTexNameList != null)
				{
					lock (m_queuedTexNameList)
					{
						bitmapLoadWorker.RunWorkerAsync(m_queuedTexNameList);
						m_queuedTexNameList = null;
					}
				}
			}
			else
			{
				Material.TextureNameList texNameList = e.Result as Material.TextureNameList;
				Control[] controls = TextureExplorerContainer.Controls.Find("texBrowser",true);
				if (controls != null && controls.Length >= 1)
				{
					TextureBrowser tbl = controls[0] as TextureBrowser;
					if (tbl != null)
					{
						for (int i = 0;i < texNameList.bitmaps.Count;++i)
							tbl.AddTexture(new TextureHandle((string)texNameList.names[i],
															 (string)texNameList.description[i],
															 (string)texNameList.filenames[i],
															(Bitmap)texNameList.bitmaps[i],
															(Microsoft.Xna.Framework.Graphics.TextureInformation)texNameList.imageInfos[i],
															new TextureThumbnail.TexDropHandler(texNameList.ownerMaterial.OnTexDrop)));

						tbl.DisplayThumbnails();
					}
				}
			}
		}


		private void txtTargetPort_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,5);

			string errorString = Utility.checkPort(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.TargetPort = tb.TextBox.Text;
		}

		private void txtTargetIPAddress_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,5);

			string errorString = Utility.checkIPAddress(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.TargetIP = tb.TextBox.Text;
		}

		private void tcPlugins_TabItemClose(object sender,TabStripActionEventArgs e)
		{

			//Since we want the Custom Data Builder control to remain active even though
			//the tab is being closed, we need to remove the control so that
			//it doesn't get disposed.
			DevComponents.DotNetBar.TabControl tc = sender as DevComponents.DotNetBar.TabControl;
			if (tc.SelectedTab.Name == "Custom Data")
			{
				tc.SelectedTab.AttachedControl.Controls.Clear();

				//Dispose of the tab.
				tc.SelectedTab.Dispose();
				return;
			}

			//Since we want the Shader Builder control to remain active even though
			//the tab is being closed, we need to remove the control so that
			//it doesn't get disposed.
			if (tc.SelectedTab.Name == "Shader Builder")
			{
				tc.SelectedTab.AttachedControl.Controls.Clear();

				//Dispose of the tab.
				tc.SelectedTab.Dispose();
				return;
			}

			//Get active document and close...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					BaseDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as BaseDocument;
					if (doc != null)
					{
						if (doc.close())
						{
							m_docMgr.Documents.Remove(tcPluginTools.SelectedTab.Name);
							TabItem tabToRemove = tcPluginTools.SelectedTab;
							//tcPluginTools.Tabs.Remove(tabToRemove);
							tabToRemove.Dispose();
						}
						else
						{
							//Cancel the close event
							e.Cancel = true;
						}
					}
					else
					{
						//Dispose of the tab.
						tc.SelectedTab.Dispose();
					}
				}

	
		}

		private void btnTexturePreview_Click(object sender,EventArgs e)
		{
			//tabTexturePreview.Visible = true;
			//tcPlugins.SelectedTab = tabTexturePreview;
		}

		/*private void btnTargetConsole_Click(object sender,EventArgs e)
		{
			tabTargetConsole.Visible = true;
			tcPlugins.SelectedTab = tabTargetConsole;
		}*/

		private void btnDesignerHome_Click(object sender,EventArgs e)
		{
			foreach (TabItem tab in tcPluginTools.Tabs)
			{
				if (tab.Name == "XVP Home")
				{
					tcPluginTools.SelectedTab = tab;
					return;
				}
			}

			WebBrowser wb = new WebBrowser();
			wb.Dock = DockStyle.Fill;
			wb.Url = new Uri("http://www.xvpsdk.com");

			//wb.DocumentText = reedRSSFeed();

			tcPluginTools.SuspendLayout();
			TabItem ti = tcPluginTools.CreateTab("XVP Home");
			ti.Name = "XVP Home";
			wb.Name = "XVP Home";
			ti.Image = imglistGlobal.Images["Browser"];
			ti.AttachedControl.Controls.Add(wb);
			tcPluginTools.SelectedTab = ti;
			tcPluginTools.ResumeLayout();
		}

		private void btnCustomData_Click(object sender,EventArgs e)
		{
			foreach (TabItem tab in tcPluginTools.Tabs)
			{
				if (tab.Name == "Custom Data")
				{
					tcPluginTools.SelectedTab = tab;
					return;
				}
			}

			CustomDataBuilder customDataBuilder = m_customDataBuilder;// new CustomDataBuilder();
			customDataBuilder.Dock = DockStyle.Fill;
			
			tcPluginTools.SuspendLayout();
			TabItem ti = tcPluginTools.CreateTab("Custom Data");
			ti.Name = "Custom Data";
			ti.Image = imglistGlobal.Images["CustomData"];
			customDataBuilder.Name = "Custom Data";
			ti.AttachedControl.Controls.Add(customDataBuilder);
			tcPluginTools.SelectedTab = ti;
			tcPluginTools.ResumeLayout();
		}

		private void buttonItem28_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.ShowDialog();
		}

		private void btnQAT_Click(object sender,EventArgs e)
		{
			ribbonMainRibbon.ShowQatCustomizeDialog();
		}

		private void btnShowStartPage_Click(object sender,EventArgs e)
		{
			ButtonItem b = sender as ButtonItem;
			b.Checked = !b.Checked;
			m_currentWorkspace.ShowStartPage = b.Checked;
		}

		private bool saveActor()
		{
			//Get active document and save...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.save();
						if (success)
						{
							XVPMDocument oldDoc = m_docMgr.Documents[oldName] as XVPMDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}

						return success;
					}
				}

			return false;
		}

		private bool saveStage()
		{
			//Get active document and save...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.save();
						if (success)
						{
							XVPMDocument oldDoc = m_docMgr.Documents[oldName] as XVPMDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}

						return success;
					}
				}

			return false;
		}

		private void saveActorAs()
		{
			//Get active document and save as...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.saveAs();
						if (success)
						{
							XVPMDocument oldDoc = m_docMgr.Documents[oldName] as XVPMDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}
					}
				}
		}

		private void saveStageAs()
		{
			//Get active document and save as...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.saveAs();
						if (success)
						{
							XVPMDocument oldDoc = m_docMgr.Documents[oldName] as XVPMDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}
					}
				}
		}

		private void saveScriptAs()
		{
			//Get active document and save as...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					ScriptDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ScriptDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.saveAs();
						if (success)
						{
							ScriptDocument oldDoc = m_docMgr.Documents[oldName] as ScriptDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}
					}
				}
		}
		
		private void newWorkspace()
		{
			ActorTemplateWizard wiz = new ActorTemplateWizard();

			DialogResult dr = wiz.ShowDialog();
			if (!wiz.Completed)
				return;

			m_currentWorkspace = wiz.genWorkspace();

			//Set the workspace path THEN save it
			m_currentWorkspacePath = wiz.WorkspacePath;
			saveWorkspace();
			applyWorkspace(m_currentWorkspace);

			wiz.Dispose();

		}

		private bool saveWorkspace()
		{
			if (m_currentWorkspacePath == null)
			{
				dlgSaveFileDialog.Title = "Save Workspace...";
				dlgSaveFileDialog.Filter = "Workspace Files (*.wrk)|*.wrk|All files (*.*)|*.*";
				dlgSaveFileDialog.FileName = "Workspace.wrk";
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result == DialogResult.OK)
				{
					FileStream fs = null;
					try
					{
						fs = File.Open(dlgSaveFileDialog.FileName,FileMode.Create);
					}
					catch (Exception exception)
					{
						MessageBox.Show("Cannot open \"" + dlgSaveFileDialog.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
						logMessage(exception.ToString(),Color.Red);
						if (fs != null)
							fs.Close();
						return false;
					}
					BinaryFormatter bf = new BinaryFormatter();
					bf.Serialize(fs,m_currentWorkspace);
					fs.Close();
					m_currentWorkspace.Dirty = false;
					m_currentWorkspacePath = dlgSaveFileDialog.FileName;
				}
			}
			else
			{
				FileStream fs = null;
				try
				{
					fs = File.Open(m_currentWorkspacePath,FileMode.Create);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + m_currentWorkspacePath + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					logMessage(exception.ToString(),Color.Red);
					return false;
				}
				BinaryFormatter bf = new BinaryFormatter();
				bf.Serialize(fs,m_currentWorkspace);
				fs.Close();
				m_currentWorkspace.Dirty = false;
			}
			return true;
		}

		private void saveWorkspaceAs()
		{
			dlgSaveFileDialog.Title = "Save Workspace As...";
			dlgSaveFileDialog.Filter = "Workspace Files (*.wrk)|*.wrk|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Workspace.wrk";
			DialogResult result = dlgSaveFileDialog.ShowDialog();
			if (result == DialogResult.OK)
			{
				FileStream fs = null;
				try
				{
					fs = File.Open(dlgSaveFileDialog.FileName,FileMode.Create);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + dlgSaveFileDialog.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					logMessage(exception.ToString(),Color.Red);
					return;
				}
				BinaryFormatter bf = new BinaryFormatter();
				bf.Serialize(fs,m_currentWorkspace);
				fs.Close();
			}
		}

		private void applyWorkspace(Workspace workspace)
		{
			switch (workspace.TargetDevice)
			{
				case "PC":
					cbTargetDevice.SelectedItem = ciPC;
					break;
				case "XBOX 360":
					cbTargetDevice.SelectedItem = ciXBOX360;
					break;
				case "PlayStation 3":
					cbTargetDevice.SelectedItem = ciPlayStation3;
					break;
				case "Wii":
					cbTargetDevice.SelectedItem = ciWii;
					break;
				default:
					logMessage("Unrecognized device type \"" + workspace.TargetDevice + "\" in workspace. Defaulting to PC.",Color.Blue);
					cbTargetDevice.SelectedItem = ciPC;
					break;
			}

			txtTargetIPAddress.TextBox.Text = workspace.TargetIP;
			txtTargetPort.TextBox.Text = workspace.TargetPort;
			btnShowStartPage.Checked = workspace.ShowStartPage;
			cbSkinColor.SelectedIndex = workspace.SkinColor;
			btnShowToolTips.Checked = workspace.ShowToolTips;
			superTooltip.Enabled = btnShowToolTips.Checked;
			btnPlayEventSounds.Checked = workspace.PlayEventSounds;
			txtShaderPath.TextBox.Text = workspace.ShaderPath;
			txtCDDPath.TextBox.Text = workspace.CDDPath;
			txtActorLibraryPath.TextBox.Text = workspace.ActorLibraryPath;
			txtMaterialLibraryPath.TextBox.Text = workspace.MaterialLibraryPath;
			txtControllerLibraryPath.TextBox.Text = workspace.ControllerLibraryPath;
			txtTextureLibraryPath.TextBox.Text = workspace.TextureLibraryPath;
			txtAudioLibraryPath.TextBox.Text = workspace.AudioLibraryPath;
			txtScriptLibraryPath.TextBox.Text = workspace.ScriptLibraryPath;

			workspace.Dirty = false;

			if (m_currentWorkspacePath != null)
				Program.g_mainForm.Text = "XVP Designer - " + Path.GetFileNameWithoutExtension(m_currentWorkspacePath);
			else
				Program.g_mainForm.Text = "XVP Designer";
		}

		private void btnNewWorkspace_Click(object sender,EventArgs e)
		{
            //Run through all the documents in the document manager and see if any need to be saved before closing.
            //If any of them return false from close() then we cancel the new workspace action.
            foreach (string docName in m_docMgr.Documents.Keys)
            {
                bool success = (m_docMgr.Documents[docName] as BaseDocument).close();
				if (!success)
				{
					return;
				}
				else
                {
                    TabItem tabToRemove = tcPluginTools.Tabs[docName];
					tcPluginTools.Tabs.Remove(tabToRemove);
                }
            }
			m_docMgr.Documents.Clear();

			if (m_currentWorkspace.Dirty)
			{
				DialogResult dr = MessageBox.Show("The current workspace has been modified.  Would you like to save the current workspace?","Workspace Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				bool successfulSave = true;
				switch (dr)
				{
					case DialogResult.Yes:
						successfulSave = saveWorkspace();
						break;
					case DialogResult.Cancel:
						return;
				}
				if (!successfulSave)
					return;
			}
			newWorkspace();

			//Refresh library explorer
			m_libraryExplorer.init(m_currentWorkspace.ActorLibraryPath,
									m_currentWorkspace.MaterialLibraryPath,
									m_currentWorkspace.ControllerLibraryPath,
									m_currentWorkspace.AudioLibraryPath,
									m_currentWorkspace.TextureLibraryPath,
									m_currentWorkspace.ScriptLibraryPath);
		}

		private void btnLoadLastWorkspace_Click(object sender,EventArgs e)
		{
			ButtonItem b = sender as ButtonItem;
			b.Checked = !b.Checked;

			//Set the registry key
			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			key.SetValue("LoadLastWorkspace",b.Checked);
		}

		private void btnPlayEventSounds_Click(object sender,EventArgs e)
		{
			ButtonItem b = sender as ButtonItem;
			b.Checked = !b.Checked;
			m_currentWorkspace.PlayEventSounds = b.Checked;
		}

		private void txtCDDPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.CDDPath = tb.TextBox.Text;
		}

		private void txtActorLibraryPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.ActorLibraryPath = tb.TextBox.Text;
		}

		private void txtMaterialLibraryPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.MaterialLibraryPath = tb.TextBox.Text;
		}
		private void btnCDDPathBrowse_Click(object sender,EventArgs e)
		{
			//txtCDDPath.Focus();
			dlgFolderBrowse.Description = "Select path for CDDs...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtCDDPath.TextBox.Text = dlgFolderBrowse.SelectedPath;

			MessageBox.Show("Designer must be restarted for the new Custom Data path to take effect.","Restart Needed",MessageBoxButtons.OK,MessageBoxIcon.Information);
		}

		private void btnTemplatePathBrowse_Click(object sender,EventArgs e)
		{
			//txtActorLibraryPath.Focus();
			dlgFolderBrowse.Description = "Select path for Actor Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtActorLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath;
			m_libraryExplorer.ActorLibraryPath = txtActorLibraryPath.TextBox.Text;
		}

		private void cbTargetDevice_Click(object sender,EventArgs e)
		{
			m_currentWorkspace.TargetDevice = cbTargetDevice.SelectedItem.ToString();
		}

		private void btnSaveWorkspace_Click(object sender,EventArgs e)
		{
			saveWorkspace();
		}

		private void btnSaveWorkspaceAs_Click(object sender,EventArgs e)
		{
			saveWorkspaceAs();
		}

		private void openWorkspaceFromFile(string filename)
		{
			FileStream fs = null;
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				m_currentWorkspace = bf.Deserialize(fs) as Workspace;
				m_currentWorkspacePath = filename;
				addRecentWorkspace(m_currentWorkspacePath);
				applyWorkspace(m_currentWorkspace);
			}
			catch (Exception exception)
			{
				logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not open workspace.  Check the message log for details.","Open Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
			if (fs != null)
				fs.Close();
		}

		private void btnOpenWorkspace_Click(object sender,EventArgs e)
		{
			if (m_currentWorkspace.Dirty)
			{
				DialogResult dr = MessageBox.Show("The current workspace has been modified.  Would you like to save the current workspace?","Workspace Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				bool successfulSave = true;
				switch (dr)
				{
					case DialogResult.Yes:
						successfulSave = saveWorkspace();
						break;
					case DialogResult.Cancel:
						return;
				}
				if (!successfulSave)
					return;
			}

			dlgOpenFileDialog.Title = "Open Workspace...";
			dlgOpenFileDialog.Filter = "Workspace Files (*.wrk)|*.wrk|All files (*.*)|*.*";
			dlgOpenFileDialog.FileName = "";
			DialogResult result = dlgOpenFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				openWorkspaceFromFile(dlgOpenFileDialog.FileName);
		}

		private void MainForm_FormClosing(object sender,FormClosingEventArgs e)
		{
			//sec.Release();

			if (m_currentWorkspace.Dirty)
			{
				ribbonMainRibbon.SelectedRibbonTabItem = ribtabWorkspace;
				DialogResult dr = MessageBox.Show("The current workspace has been modified.  Would you like to save the current workspace?","Workspace Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				bool successfulSave = true;
				switch (dr)
				{
					case DialogResult.Yes:
						successfulSave = saveWorkspace();
						break;
					case DialogResult.Cancel:
						e.Cancel = true;
						return;
				}
				if (!successfulSave)
				{
					e.Cancel = true;
					return;
				}
			}

			//Run through all the documents in the document manager and see if any close them.
			//If any of them return false from close() then we cancel shutdown
			foreach (string docName in m_docMgr.Documents.Keys)
			{
				bool success = (m_docMgr.Documents[docName] as BaseDocument).close();
				if (!success)
				{
					e.Cancel = true;
					return;
				}
			}

			//Run through all the documents in the document manager and
			//remove them.  Unregister the tab closing event, we're shutting down.
			tcPluginTools.SelectedTabChanged -= tcPluginTools_SelectedTabChanged;
			foreach (string docName in m_docMgr.Documents.Keys)
			{
				TabItem tabToRemove = tcPluginTools.Tabs[docName];
				tcPluginTools.Tabs.Remove(tabToRemove);
			}
			m_docMgr.Documents.Clear();

			//Stop XVMP update worker and destroy it
			try
			{
				//To make things look a little cleaner, hide the DXRenderPanels so
				//they don't show garbage while form is exiting.
				foreach (TabItem t in tcPluginTools.Tabs)
				{
					if (t.AttachedControl != null)
					{
						if (t.AttachedControl.Controls.Count != 0)
						{
							XVPMControl control = t.AttachedControl.Controls[0] as XVPMControl;
							if (control != null)
								control.Hide();
						}
					}
				}
				tcPluginTools.Refresh();

				//Tell the update worker to cancel processing and wait for it to 
				//stop before destroy XVPM
				m_XVPMUpdateStop.Set();
				while (m_XVPMUpdateThread.IsAlive)
				{
					if (WaitHandle.WaitAll((new ManualResetEvent[] { m_XVPMUpdateStopped }),100,true))
						break;

					Application.DoEvents();
				}
			
				//Now that update thread has stopped, it's safe to destroy XVPM
				if (XVPM.inited())
					XVPM.destroy();
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error encounterd while shutting down XVPM.  See the message log for details.","XVMP Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				logMessage(exception.ToString(),Color.Red);
			}

			RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
			if (key == null)
				key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
			if (m_currentWorkspacePath != null)
				key.SetValue("LastWorkspace",m_currentWorkspacePath);

			//Only save the layout if the form loaded successfully so we don't corrupt
			//good settings
			if (m_loadFormOK)
			{
				//File shouldn't be read only
				bool canSave = true;
				if (File.Exists(Application.StartupPath + "\\dock.layout"))
				{
					if ((File.GetAttributes(Application.StartupPath + "\\dock.layout") & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
					{
						MessageBox.Show("Can't save window dock layout because \"" + Application.StartupPath + "\\dock.layout\" is marked read-only.","Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
						canSave = false;
					}
				}
				if (canSave)
					DotNetBarManager.SaveLayout(Application.StartupPath + "\\dock.layout");
			}

			if (ribbonMainRibbon.QatLayoutChanged)
			{
				FileStream fs = null;
				try
				{
					fs = File.Open(Application.StartupPath + "\\qat.layout",FileMode.Create);
					BinaryFormatter bf = new BinaryFormatter();
					bf.Serialize(fs,ribbonMainRibbon.QatLayout);
					fs.Close();
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + Application.StartupPath + "\\qat.layout" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					logMessage(exception.ToString(),Color.Red);
				}
			}

			//Does this stuff cause my exit crash -> targetInvocation exception???
			/*m_libraryExplorer.Dispose();
			m_graphExplorer.Dispose();
			m_output.Dispose();
			m_pluginDialog.Dispose();
			m_splash.Dispose();*/
		}

		private void btnShowToolTips_Click(object sender,EventArgs e)
		{
			ButtonItem b = sender as ButtonItem;
			b.Checked = !b.Checked;
			m_currentWorkspace.ShowToolTips = b.Checked;
			superTooltip.Enabled = b.Checked;
		}

		private void btnBrowseForShaderPath_Click(object sender,EventArgs e)
		{
			//txtShaderPath.Focus();
			dlgFolderBrowse.Description = "Select path for shaders...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtShaderPath.TextBox.Text = dlgFolderBrowse.SelectedPath;

			MessageBox.Show("The application must be restarted for the shader path change to take effect.","Restart Needed",MessageBoxButtons.OK,MessageBoxIcon.Information);
		}

		private void txtShaderPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.ShaderPath = tb.TextBox.Text;
		}

		private void btnSaveActorAs_Click(object sender,EventArgs e)
		{
			saveActorAs();
		}

		private void btnSaveLevelAs_Click(object sender,EventArgs e)
		{
			//saveLevelAs();
		}

		private void btnWindowSceneExplorer_Click(object sender,EventArgs e)
		{
			if (SceneDockWindow.AutoHide)
				return;

			if (SceneDockWindow.Visible)
			{
				SceneDockWindow.Hide();
				return;
			}

			SceneDockWindow.Show();
			if (SceneDockWindow.Docked && NodePropertiesDockWindow.Docked)
				DotNetBarManager.LeftDockSite.GetDocumentUIManager().Dock(NodePropertiesDockWindow,SceneDockWindow,eDockSide.Top);
		}

		private void SceneDockWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			SceneDockWindow.Hide();
		}

		private void NodePropertiesDockWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			NodePropertiesDockWindow.Hide();
		}

		private void btnWindowPropertyExplorer_Click(object sender,EventArgs e)
		{
			if (NodePropertiesDockWindow.AutoHide)
				return;

			if (NodePropertiesDockWindow.Visible)
			{
				NodePropertiesDockWindow.Hide();
				return;
			}


			NodePropertiesDockWindow.Show();
			if (NodePropertiesDockWindow.Docked && SceneDockWindow.Docked)
				DotNetBarManager.LeftDockSite.GetDocumentUIManager().Dock(SceneDockWindow,NodePropertiesDockWindow,eDockSide.Bottom);
		}

		private void TemplateDockWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			TemplateDockWindow.Hide();
		}

		private void btnWindowLibraryExplorer_Click(object sender,EventArgs e)
		{
			if (TemplateDockWindow.AutoHide)
				return;

			if (TemplateDockWindow.Visible)
			{
				TemplateDockWindow.Hide();
				return;
			}


			TemplateDockWindow.Show();
		}

		private void OutputDockWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			OutputDockWindow.Hide();
		}

		private void GraphExplorerDockWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			GraphExplorerDockWindow.Hide();
		}

		private void btnWindowOutput_Click(object sender,EventArgs e)
		{
			if (OutputDockWindow.AutoHide)
				return;

			if (OutputDockWindow.Visible)
			{
				OutputDockWindow.Hide();
				return;
			}

			OutputDockWindow.Show();
			if (TextureExplorerWindow.Docked && OutputDockWindow.Docked)
				DotNetBarManager.BottomDockSite.GetDocumentUIManager().Dock(TextureExplorerWindow,OutputDockWindow,eDockSide.Right);

		}

		private void btnWindowTexturePreview_Click(object sender,EventArgs e)
		{
			if (TextureExplorerWindow.AutoHide)
				return;

			if (TextureExplorerWindow.Visible)
			{
				TextureExplorerWindow.Hide();
				return;
			}

			TextureExplorerWindow.Show();
			if (TextureExplorerWindow.Docked && OutputDockWindow.Docked)
				DotNetBarManager.BottomDockSite.GetDocumentUIManager().Dock(OutputDockWindow,TextureExplorerWindow,eDockSide.Left);
		}

		private void TexturePreviewWindow_Closing(object sender,BarClosingEventArgs e)
		{
			e.Cancel = true;
			TextureExplorerWindow.Hide();
		}

		private void ribtabWorkspace_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabWorkspace;
		}

		private void ribtabActorStudio_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabActorStudio;
		}

		private void ribtabStageStudio_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabStageStudio;
		}

		private void ribtabFXStudio_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabDirectorStudio;
		}

		private void ribtabAnimationStudio_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabAnimationStudio;
		}

		private void ribtabTools_MouseDown(object sender,MouseEventArgs e)
		{
			ribbonMainRibbon.SelectedRibbonTabItem = ribtabTools;
		}

		private void txtControllerPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.ControllerLibraryPath = tb.TextBox.Text;
		}

		private void txtTexturePath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.TextureLibraryPath = tb.TextBox.Text;
		}

		private void txtAudioPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.AudioLibraryPath = tb.TextBox.Text;
		}

		private void txtScriptPath_InputTextChanged(object sender)
		{
			TextBoxItem tb = ((TextBoxItem)sender);
			errorProvider.SetIconPadding(tb.TextBox,2);

			string errorString = Utility.checkPath(tb.TextBox.Text);
			if (errorString != null)
			{
				errorProvider.SetError(tb.TextBox,errorString);
				return;
			}
			if (!Path.IsPathRooted(tb.TextBox.Text))
			{
				errorProvider.SetError(tb.TextBox,"Must be an absolute path.");
				return;
			}

			errorProvider.SetError(tb.TextBox,null);
			m_currentWorkspace.ScriptLibraryPath = tb.TextBox.Text;
		}
	
		public void XVPMUpdate()
		{
			while (true)
			{
				Thread.Sleep(1);


				if (m_XVPMUpdateStop.WaitOne(0,true))
				{
					m_XVPMUpdateStopped.Set();
					return;
				}

				//Don't update if mainform is minimized
				if (WindowState == FormWindowState.Minimized)
					continue;

				startTime = DateTime.Now;
				lock (XVPMLockObject)
				{
					
					if (XVPM.inited())
						XVPM.update((float)elapsedTime.TotalSeconds);
				}
				elapsedTime = DateTime.Now - startTime;
				
			}
		}

		private void tcPluginTools_SelectedTabChanged(object sender,TabStripTabChangedEventArgs e)
		{
			//Prevent modifcation when loading
			if (!loadingComplete)
				return;

			//Find the document that matches the old and new tabs.  Deactivate the old
			//document and activate the new
			if (e.OldTab != null)
			{
				BaseDocument doc = m_docMgr.Documents[e.OldTab.Name] as BaseDocument;
				if (doc != null)
					doc.setActive(false);
			}

			if (e.NewTab != null)
			{
				XVPMDocument doc = m_docMgr.Documents[e.NewTab.Name] as XVPMDocument;
				if (doc != null)
				{
					//Set the document active
					doc.setActive(true);

					//If the document is an actor studio document...
					if (doc as ActorStudioDocument != null)
					{
						ActorStudioDocument actorDoc = doc as ActorStudioDocument;

						//Set explorers and XVPM handle
						setSceneExplorer(actorDoc.SceneExplorer);
						setPropertyExplorer(actorDoc.PropertyExplorer);
						setActionExplorer(actorDoc.PropertyExplorer.getSelectedNode());
						setXVPMHandle(actorDoc.XVPMHandle);

						//Set actor ribbon controls to match the document
						if (!cbActorTemplate.Items.Contains(actorDoc.ViewportConfig.ActorTemplate))
						{
							//logMessage("The actor " + actorDoc.Name + " was saved with an actor templated called " + actorDoc.ViewportConfig.ActorTemplate + " but it could not be located in the actor template folder.  Setting the actor to use no template.",Color.Blue);
							cbActorTemplate.SelectedItem = "No Template";
						}
						else
							cbActorTemplate.SelectedItem = actorDoc.ViewportConfig.ActorTemplate;
						txtGridUnitSize.ControlText = actorDoc.ViewportConfig.UnitSize.ToString();
						txtGridSize.ControlText = actorDoc.ViewportConfig.GridSize.ToString();
						txtMajorLine.ControlText = actorDoc.ViewportConfig.MajorLine.ToString();
						switch (actorDoc.ViewportConfig.LightingMode)
						{
							case 0:
								setLightingButtons("Default Lights");
								break;
							case 1:
								setLightingButtons("Scenario Lights");
								break;
							case 2:
								setLightingButtons("Scene Lights");
								break;
						}
						setLightingMode(actorDoc.XVPMHandle,actorDoc.ViewportConfig.LightingMode);
						setLightingScenarioButton(actorDoc.ViewportConfig.LightingScenario);
						setLightingScenario(actorDoc.XVPMHandle,actorDoc.ViewportConfig.LightingScenario);
						if (actorDoc.ViewportConfig.getLightColor(0) != Color.Transparent)
							cpLight1Color.SelectedColor = actorDoc.ViewportConfig.getLightColor(0);
						if (actorDoc.ViewportConfig.getLightColor(1) != Color.Transparent)
							cpLight2Color.SelectedColor = actorDoc.ViewportConfig.getLightColor(1);
						if (actorDoc.ViewportConfig.getLightColor(2) != Color.Transparent)
							cpLight3Color.SelectedColor = actorDoc.ViewportConfig.getLightColor(2);
						sliderLight1Intensity.Value = actorDoc.ViewportConfig.getLightIntensity(0);
						sliderLight2Intensity.Value = actorDoc.ViewportConfig.getLightIntensity(1);
						sliderLight3Intensity.Value = actorDoc.ViewportConfig.getLightIntensity(2);
						sliderLightAngle.Value = actorDoc.ViewportConfig.LightingAngle;
						chkLightShadows.Checked = actorDoc.ViewportConfig.EnableShadows;

						switch (actorDoc.ViewportConfig.LightAnimSpeed)
						{
							case "No Animation":
								cbLightAnimSpeed.SelectedItem = ciNoLightAnim;
								break;
							case "Slow Animation":
								cbLightAnimSpeed.SelectedItem = ciSlowLightAnim;
								break;
							case "Medium Animation":
								cbLightAnimSpeed.SelectedItem =  ciMediumLightAnim;
								break;
							case "Fast Animation":
								cbLightAnimSpeed.SelectedItem = ciFastLightAnim;
								break;
						}

						//Enable studios
						enableActorStudio(true);
						enableStageStudio(false,null);
					}
					//If the document is stage studio document...
					if (doc as StageStudioDocument != null)
					{
						StageStudioDocument stageDoc = doc as StageStudioDocument;

						//Set explorers and XVPM handle
						setSceneExplorer(stageDoc.SceneExplorer);
						setPropertyExplorer(stageDoc.PropertyExplorer);
						setXVPMHandle(stageDoc.XVPMHandle);

						//Set the appropriate Action Explorer User Control
						setActionExplorer(stageDoc.PropertyExplorer.getSelectedNode());

						enableActorStudio(false);
						enableStageStudio(true,stageDoc.XVPMControl);

					}
				}
				else
				{
					setSceneExplorer(null);
					setPropertyExplorer(null);
					setActionExplorer(null);
					enableActorStudio(false);
					enableStageStudio(false,null);
				}
			}
		}

		public void enableActorStudio(bool enable)
		{
			if (enable)
			{
				//Enable Actor Studio ribbon tab
				foreach (Control c in ribtabActorStudio.Panel.Controls)
					c.Enabled = true;
				cbActorTemplate.Enabled = true;
				if (cbActorTemplate.SelectedItem.ToString() == "No Template")
					btnVerifyActor.Enabled = false;
				else
					btnVerifyActor.Enabled = true;
				btnExportActor.Enabled = true;
				btnSaveActor.Enabled = true;
			}
			else
			{
				//Disable various ribbons except for basic functionality
				foreach (Control c in ribtabActorStudio.Panel.Controls)
					c.Enabled = false;
				cbActorTemplate.Enabled = false;
				btnVerifyActor.Enabled = false;
				btnExportActor.Enabled = false;
				btnSaveActor.Enabled = false;
				ribbarActorScene.Enabled = true;
				sliderLightAngle.TextColor = Color.DimGray;
			}
		}

		public void enableStageStudio(bool enable,XVPMControl XVPMControl)
		{
			if (enable)
			{
				//Enable Actor Studio ribbon tab
				foreach (Control c in ribtabStageStudio.Panel.Controls)
					c.Enabled = true;
				btnCloseStage.Enabled = true;
				btnExportStage.Enabled = true;
				btnSaveStage.Enabled = true;

				btnMoveSnap.Checked = XVPMControl.isMoveSnapEnabled();
				btnRotateSnap.Checked = XVPMControl.isRotateSnapEnabled();
				btnScaleSnap.Checked = XVPMControl.isScaleSnapEnabled();
			}
			else
			{
				foreach (Control c in ribtabStageStudio.Panel.Controls)
					c.Enabled = false;
				btnSaveStage.Enabled = false;
				btnCloseStage.Enabled = false;
				btnExportStage.Enabled = false;
				ribbarStageScene.Enabled = true;
			}
		}

		/*public Document getActiveDocument()
		{
			return m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as Document;
		}*/

		private void MainForm_ResizeBegin(object sender,EventArgs e)
		{
			SuspendLayout();
		}

		private void MainForm_ResizeEnd(object sender,EventArgs e)
		{
			ResumeLayout();
		}

		private void btnFullView_Click(object sender,EventArgs e)
		{
			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
							control.setLayout(0);
					}
				}
			}
		}

		private void btnVerticalSplitView_Click(object sender,EventArgs e)
		{
			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
							control.setLayout(1);
					}
				}
			}
		}

		private void btnHorizontalSplitView_Click(object sender,EventArgs e)
		{
			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
							control.setLayout(2);
					}
				}
			}
		}

		private void btnQuadView_Click(object sender,EventArgs e)
		{
			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
							control.setLayout(3);
					}
				}
			}
		}

		private void MainForm_KeyDown(object sender,KeyEventArgs e)
		{
			//Check viewport hot keys
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
						{
							if (e.Alt)
								control.enableMovement(true);
							if (e.Control)
								control.setControlPressed(true);
							if (e.KeyData == Keys.F)
								control.zoomSelection(-1);
							if (e.KeyData == Keys.L)
								control.toggleLockSelection();
							if (e.KeyData == Keys.Space)
								control.maximizeActiveVP();
						}
					}
				}
			}
			
		}

		private void cpViewportBackground_SelectedColorChanged(object sender,EventArgs e)
		{
			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
						{
							ColorPickerDropDown cpdd = sender as ColorPickerDropDown;
							control.setBackgroundColor(Color.FromArgb(cpdd.SelectedColor.R,
																	  cpdd.SelectedColor.G,
																	  cpdd.SelectedColor.B));

						}
					}
				}
			}
		}

		private void btnCloseActor_Click(object sender,EventArgs e)
		{
			//Get active document and close...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						if (doc.close())
						{
							m_docMgr.Documents.Remove(tcPluginTools.SelectedTab.Name);
							TabItem tabToRemove = tcPluginTools.SelectedTab;
							tcPluginTools.Tabs.Remove(tabToRemove);
							tabToRemove.Dispose();
						}
					}
				}
		}

		private void btnCloseStage_Click(object sender,EventArgs e)
		{
			//Get active document and close...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					XVPMDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as XVPMDocument;
					if (doc != null)
					{
						if (doc.close())
						{
							m_docMgr.Documents.Remove(tcPluginTools.SelectedTab.Name);
							TabItem tabToRemove = tcPluginTools.SelectedTab;
							tcPluginTools.Tabs.Remove(tabToRemove);
							tabToRemove.Dispose();
						}
					}
				}
		}

		public void setIntProperty(IntPtr proxyObject,string propertyName,int value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setIntProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}
		}

		public void setUIntProperty(IntPtr proxyObject,string propertyName,uint value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setUIntProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}
		}

		public void setFloatProperty(IntPtr proxyObject,string propertyName,float value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setFloatProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}
		}

		public void setDoubleProperty(IntPtr proxyObject,string propertyName,double value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setDoubleProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}
		}

		public void setBoolProperty(IntPtr proxyObject,string propertyName,bool value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setBoolProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}
		}

		public void setColorProperty(IntPtr proxyObject,string propertyName,float r,float g,float b,float a)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setColorProperty(proxyObject,pPropertyName,r,g,b,a);
				Marshal.FreeHGlobal(pPropertyName);
			}

		}

		public void setEnumProperty(IntPtr proxyObject,string propertyName,int value)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setEnumProperty(proxyObject,pPropertyName,value);
				Marshal.FreeHGlobal(pPropertyName);
			}

		}

		public void setVector2DProperty(IntPtr proxyObject,string propertyName,float x,float y)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setVector2DProperty(proxyObject,pPropertyName,x,y);
				Marshal.FreeHGlobal(pPropertyName);
			}

		}

		public void setVector3DProperty(IntPtr proxyObject,string propertyName,float x,float y,float z)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setVector3DProperty(proxyObject,pPropertyName,x,y,z);
				Marshal.FreeHGlobal(pPropertyName);
			}

		}

		public void setVector4DProperty(IntPtr proxyObject,string propertyName,float x,float y,float z,float w)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pPropertyName = Marshal.StringToHGlobalAnsi(propertyName);
				XVPM.setVector4DProperty(proxyObject,pPropertyName,x,y,z,w);
				Marshal.FreeHGlobal(pPropertyName);
			}

		}

		public void setMaterialShaderName(IntPtr proxyObject,string shaderName)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				IntPtr pShaderName = Marshal.StringToHGlobalAnsi(shaderName);
				XVPM.setMaterialShaderName(proxyObject,pShaderName);
				Marshal.FreeHGlobal(pShaderName);
			}

		}

		public void setTexture(IntPtr proxyObject,uint samplerIndex,string filename)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			//Determine texture type
			lock (XVPMLockObject)
			{
				IntPtr pFilename = Marshal.StringToHGlobalAnsi(filename);
				XVPM.setMaterialTexture(proxyObject,samplerIndex,pFilename);
				Marshal.FreeHGlobal(pFilename);
			}
		}
		public void setSamplerAddressU(IntPtr proxyObject,uint samplerIndex,Material.TextureAddressType addressType)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerAddressU(proxyObject,samplerIndex,Convert.ToInt32(addressType));
			}
		}

		public void setSamplerAddressV(IntPtr proxyObject,uint samplerIndex,Material.TextureAddressType addressType)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerAddressV(proxyObject,samplerIndex,Convert.ToInt32(addressType));
			}
		}

		public void setSamplerAddressW(IntPtr proxyObject,uint samplerIndex,Material.TextureAddressType addressType)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerAddressW(proxyObject,samplerIndex,Convert.ToInt32(addressType));
			}
		}

		public void setSamplerMipLODBias(IntPtr proxyObject,uint samplerIndex,float bias)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerMipLODBias(proxyObject,samplerIndex,bias);
			}
		}

		public void setSamplerMaxAnisotropy(IntPtr proxyObject,uint samplerIndex,uint maxAnisotropy)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerMaxAnisotropy(proxyObject,samplerIndex,maxAnisotropy);
			}
		}

		public void setSamplerMinLOD(IntPtr proxyObject,uint samplerIndex,float minLOD)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerMinLOD(proxyObject,samplerIndex,minLOD);
			}
		}

		public void setSamplerMaxLOD(IntPtr proxyObject,uint samplerIndex,float maxLOD)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerMaxLOD(proxyObject,samplerIndex,maxLOD);
			}
		}

		public void setSamplerBorderColor(IntPtr proxyObject,uint samplerIndex,float r,float g,float b,float a)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerBorderColor(proxyObject,samplerIndex,r,g,b,a);
			}
		}

		public void setSamplerFilter(IntPtr proxyObject,uint samplerIndex,Material.TextureFilterType filterType)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerFilter(proxyObject,samplerIndex,Convert.ToInt32(filterType));
			}
		}

		public void setSamplerFunc(IntPtr proxyObject,uint samplerIndex,Material.CompareFuncType comparisonFunc)
		{
			if (proxyObject.ToInt32() == 0)
				return;

			lock (XVPMLockObject)
			{
				XVPM.setSamplerFunc(proxyObject,samplerIndex,Convert.ToInt32(comparisonFunc));
			}
		}

		private void btnInfo_Click(object sender,EventArgs e)
		{
			m_pluginDialog.ShowDialog();
		}

		private void btnResetLayout_Click(object sender,EventArgs e)
		{
			DialogResult dr = MessageBox.Show("All viewports will be returned to their default configuration.  Do you want to continue?","Reset All Viewports",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (dr == DialogResult.No)
				return;

			//Set the layout of the current actor
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
							control.resetViewports();
					}
				}
			}

		}

		private void btnToggleGrid_Click(object sender,EventArgs e)
		{
			/*lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument).XVPMHandle;

				txtGridUnitSize.ControlText = XVPM.getGridUnitSize(windowHandle).ToString();
				txtGridSize.ControlText = XVPM.getGridSize(windowHandle).ToString();
				txtMajorLine.ControlText = XVPM.getGridMajorLine(windowHandle).ToString();
			}*/
		}

		private void setGridConfig()
		{
			float unitSize;
			uint majorLine;
			uint gridSize;


			bool error = true;
			bool configureGrid = true;
			errorProvider.SetIconPadding(txtGridUnitSize.TextBox,-10);

			if (float.TryParse(txtGridUnitSize.ControlText.ToString(),out unitSize))
			{
				//Check limits
				if (unitSize >= .000001 && unitSize <= 1000)
					error = false;
			}
			if (error)
			{
				errorProvider.SetError(txtGridUnitSize.TextBox,"The grid unit size must be between .000001 and 1000 inclusive.  E.g. .01, 10, 100");
				configureGrid = false;
			}
			else
				errorProvider.SetError(txtGridUnitSize.TextBox,null);
			error = true;
			errorProvider.SetIconPadding(txtGridSize.TextBox,-10);

			if (uint.TryParse(txtGridSize.ControlText.ToString(),out gridSize))
			{
				//Check limits
				if (gridSize >= 2 && gridSize <= 10000)
					if (gridSize % 2 == 0)
						error = false;
			}
			if (error)
			{
				errorProvider.SetError(txtGridSize.TextBox,"The grid size must be between 2 and 10000 inclusive and be an even number.  E.g. 10, 100, 250");
				configureGrid = false;
			}
			else
				errorProvider.SetError(txtGridSize.TextBox,null);
			error = true;
			errorProvider.SetIconPadding(txtMajorLine.TextBox,-10);

			if (uint.TryParse(txtMajorLine.ControlText.ToString(),out majorLine))
			{
				//Check limits
				if (majorLine > 0 && majorLine <= 10000)
					if (gridSize % 2 == 0)
						error = false;
			}
			if (error)
			{
				errorProvider.SetError(txtMajorLine.TextBox,"The major line must be between 1 and 10000 inclusive.  E.g. 0, 5, 25");
				configureGrid = false;
			}
			else
				errorProvider.SetError(txtMajorLine.TextBox,null);

			if (configureGrid)
			{
				if (tcPluginTools.SelectedTab != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl != null)
					{
						if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
						{
							XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
							if (control != null)
							{
								control.configureGrid(unitSize,gridSize,majorLine);
							}
						}
					}
				}
			}
		}

		private void txtGridUnitSize_InputTextChanged(object sender)
		{
			setGridConfig();
		}

		private void txtGridSize_InputTextChanged(object sender)
		{
			setGridConfig();
		}

		private void txtMajorLine_InputTextChanged(object sender)
		{
			setGridConfig();
		}

		private void btnActionExplorer_Click(object sender,EventArgs e)
		{
			if (ActionDockWindow.AutoHide)
				return;

			if (ActionDockWindow.Visible)
			{
				ActionDockWindow.Hide();
				return;
			}

			ActionDockWindow.Show();
			if (ActionDockWindow.Docked && TemplateDockWindow.Docked)
				DotNetBarManager.RightDockSite.GetDocumentUIManager().Dock(TemplateDockWindow,ActionDockWindow,eDockSide.Top);
		}

		private void btnMaterialPathBrowse_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.Description = "Select path for Material Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtMaterialLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath; 
			m_libraryExplorer.MaterialLibraryPath = txtMaterialLibraryPath.TextBox.Text;
		}

		private void btnControllerPathBrowse_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.Description = "Select path for Controller Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtControllerLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath;
			m_libraryExplorer.ControllerLibraryPath = txtControllerLibraryPath.TextBox.Text;
		}

		private void btnTexturePathBrowse_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.Description = "Select path for Texture Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtTextureLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath;
			m_libraryExplorer.TextureLibraryPath = txtTextureLibraryPath.TextBox.Text;
		}

		private void btnAudioPathBrowse_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.Description = "Select path for Audio Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtAudioLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath;
			m_libraryExplorer.AudioLibraryPath = txtAudioLibraryPath.TextBox.Text;
		}

		private void btnScriptPathBrowse_Click(object sender,EventArgs e)
		{
			dlgFolderBrowse.Description = "Select path for Script Library...";
			if (dlgFolderBrowse.ShowDialog() == DialogResult.OK)
				txtScriptLibraryPath.TextBox.Text = dlgFolderBrowse.SelectedPath;
			m_libraryExplorer.ScriptLibraryPath = txtScriptLibraryPath.TextBox.Text;
		}

		private void setLightingButtons(string buttonString)
		{
			switch (buttonString)
			{
				case "Default Lights":
					btnUseDefaultLights.Checked = true;
					btnUseScenarioLights.Checked = false;
					btnUseSceneLights.Checked = false;
					btnLightColor.Enabled = false;
					btnLightIntensity.Enabled = false;
					btnSetLightScenario.Enabled = false;
					btnUseLights.Text = btnUseDefaultLights.Text;
					cbLightAnimSpeed.Enabled = false;
					sliderLightAngle.Enabled = false;
					sliderLightAngle.TextColor = Color.DimGray;
					chkLightShadows.Enabled = false;
					break;
				case "Scene Lights":
					btnUseDefaultLights.Checked = false;
					btnUseScenarioLights.Checked = false;
					btnUseSceneLights.Checked = true;
					btnLightColor.Enabled = false;
					btnLightIntensity.Enabled = false;
					btnSetLightScenario.Enabled = false;
					btnUseLights.Text = btnUseSceneLights.Text;
					cbLightAnimSpeed.Enabled = false;
					sliderLightAngle.Enabled = false;
					sliderLightAngle.TextColor = Color.DimGray;
					chkLightShadows.Enabled = false;
					break;
				case "Scenario Lights":
					btnUseDefaultLights.Checked = false;
					btnUseScenarioLights.Checked = true;
					btnUseSceneLights.Checked = false;
					btnLightColor.Enabled = true;
					btnLightIntensity.Enabled = true;
					btnSetLightScenario.Enabled = true;
					btnUseLights.Text = btnUseScenarioLights.Text;
					cbLightAnimSpeed.Enabled = true;
					sliderLightAngle.Enabled = true;
					sliderLightAngle.TextColor = Color.Black;
					chkLightShadows.Enabled = true;
					break;
			}
			btnUseLights.Refresh();
		}

		private void setLightingMode(uint windowHandle,uint mode)
		{
			lock (XVPMLockObject)
			{
				if (XVPM.getLightingMode(windowHandle) != mode)
					XVPM.setLightingMode(windowHandle,mode);
			}
		}

		private void btnChangeLightingMode_Click(object sender,EventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			ButtonItem button = sender as ButtonItem;
			uint lightingMode = uint.Parse(button.Tag as string);
			setLightingButtons(button.Text);
			setLightingMode(actorDoc.XVPMHandle,lightingMode);
			actorDoc.ViewportConfig.LightingMode = lightingMode;
		}

		private void setLightingScenarioButton(string buttonName)
		{
			switch (buttonName)
			{
				case "btnDirSW":
					btnSetLightScenario.Image = btnDirSW.Image;
					break;
				case "btnDirDW":
					btnSetLightScenario.Image = btnDirDW.Image;
					break;
				case "btnDirTW":
					btnSetLightScenario.Image = btnDirTW.Image;
					break;
				case "btnDirSRGB":
					btnSetLightScenario.Image = btnDirSRGB.Image;
					break;
				case "btnDirDRGB":
					btnSetLightScenario.Image = btnDirDRGB.Image;
					break;
				case "btnDirTRGB":
					btnSetLightScenario.Image = btnDirTRGB.Image;
					break;
			}
			btnSetLightScenario.Refresh();
		}

		private void setLightingScenario(uint windowHandle,string buttonName)
		{
			IntPtr pButtonName = Marshal.StringToHGlobalAnsi(buttonName);
			lock (XVPMLockObject)
			{
				XVPM.setLightScenario(windowHandle,pButtonName);
			}
			Marshal.FreeHGlobal(pButtonName);
		}

		private void ChangeLightingScenario(object sender,EventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			ButtonItem button = sender as ButtonItem;
			setLightingScenarioButton(button.Name);
			setLightingScenario(actorDoc.XVPMHandle,button.Name);
			actorDoc.ViewportConfig.LightingScenario = button.Name;
			actorDoc.ViewportConfig.setLightColor(0,Color.Transparent);
			actorDoc.ViewportConfig.setLightColor(1,Color.Transparent);
			actorDoc.ViewportConfig.setLightColor(2,Color.Transparent);
			sliderLight1Intensity.Value = 500;
			sliderLight2Intensity.Value = 500;
			sliderLight3Intensity.Value = 500;
		}

		private void ChangeLightColor(object sender,EventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			ColorPickerDropDown button = sender as ColorPickerDropDown;
			uint lightIndex = uint.Parse(button.Tag as string);
			lock (XVPMLockObject)
			{
				XVPM.setScenarioLightColor(actorDoc.XVPMHandle,lightIndex,button.SelectedColor.R/255.0f,button.SelectedColor.G/255.0f,button.SelectedColor.B/255.0f);
			}
			actorDoc.ViewportConfig.setLightColor(lightIndex,button.SelectedColor);
		}

		private void btnWindowGraphExplorer_Click(object sender,EventArgs e)
		{
			if (GraphExplorerDockWindow.AutoHide)
				return;

			if (GraphExplorerDockWindow.Visible)
			{
				GraphExplorerDockWindow.Hide();
				return;
			}

			GraphExplorerDockWindow.Show();
		}

		private bool checkSelectionLock()
		{
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
						{
							if (control.SelectionLocked)
							{
								MessageBox.Show("The current selecton is locked.  Unlock the selection and try again.","Selection Locked",MessageBoxButtons.OK,MessageBoxIcon.Information);
								return false;
							}
						}
					}
				}
			}

			return true;
		}

		private void btnSelectAll_Click(object sender,EventArgs e)
		{
			if (!checkSelectionLock())
				return;

			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument).XVPMHandle;
				XVPM.selectAll(windowHandle);
			}
		}

		private void btnSelectNone_Click(object sender,EventArgs e)
		{
			if (!checkSelectionLock())
				return;

			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument).XVPMHandle;
				XVPM.clearSelection(windowHandle);
			}
		}

		private void btnSelectInvert_Click(object sender,EventArgs e)
		{
			if (!checkSelectionLock())
				return;

			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument).XVPMHandle;
				XVPM.invertSelection(windowHandle);
			}
		}

		private void btnStageMove_Click(object sender,EventArgs e)
		{
			uint mode;
			if (btnStageMove.Checked)
			{
				mode = 1;
			}
			else
			{
				mode = 0;
			}
			btnStageRotate.Checked = false;
			btnStageScale.Checked = false;
			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
				XVPM.setGizmoMode(windowHandle,mode);
			}
		}

		private void btnStageRotate_Click(object sender,EventArgs e)
		{
			uint mode;
			if (btnStageRotate.Checked)
			{
				mode = 2;
			}
			else
			{
				mode = 0;
			}
			btnStageMove.Checked = false;
			btnStageScale.Checked = false;
			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
				XVPM.setGizmoMode(windowHandle,mode);
			}
		}

		private void btnStageScale_Click(object sender,EventArgs e)
		{
			uint mode;
			if (btnStageScale.Checked)
			{
				mode = 3;
			}
			else
			{
				mode = 0;
			}
			btnStageMove.Checked = false;
			btnStageRotate.Checked = false;
			lock (XVPMLockObject)
			{
				uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
				XVPM.setGizmoMode(windowHandle,mode);
			}
		}

		private void btnWadInspector_Click(object sender,EventArgs e)
		{
			//Prompt the user for a wad file to inspect
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.CheckPathExists = true;
			ofd.CheckFileExists = true;
			ofd.Title = "Open Wad...";
			ofd.Filter = "Wad Files (*.wad)|*.wad|All files (*.*)|*.*";
			ofd.FileName = "";
			if (ofd.ShowDialog() != DialogResult.OK)
				return;

			WadInspector wadInspector = new WadInspector();
			wadInspector.Dock = DockStyle.Fill;
			string wadName = Path.GetFileName(ofd.FileName);
			wadInspector.Filename = ofd.FileName;

			tcPluginTools.SuspendLayout();
			TabItem ti = tcPluginTools.CreateTab(wadName);
			ti.Name = wadName;
			ti.Image = imglistGlobal.Images["WadInspector"];
			wadInspector.Name = wadName;
			ti.AttachedControl.Controls.Add(wadInspector);
			tcPluginTools.SelectedTab = ti;
			tcPluginTools.ResumeLayout();

			wadInspector.refreshContents();

			ofd.Dispose();
		}

		private void buttonItem28_Click_1(object sender,EventArgs e)
		{

		}

		private void btnFile_Click(object sender,EventArgs e)
		{

		}
		public void showShaderBuilder(string shaderName)
		{
			btnShaderBuilder_Click(null,null);
		}

		private void btnShaderBuilder_Click(object sender, EventArgs e)
		{
			foreach (TabItem tab in tcPluginTools.Tabs)
			{
				if (tab.Name == "Shader Builder")
				{
					tcPluginTools.SelectedTab = tab;
					return;
				}
			}

			ShaderBuilder shaderBuilder = m_shaderBuilder;
			shaderBuilder.Dock = DockStyle.Fill;

			tcPluginTools.SuspendLayout();
			TabItem ti = tcPluginTools.CreateTab("Shader Builder");
			ti.Name = "Shader Builder";
			ti.Image = imglistGlobal.Images["ShaderBuilder"]; 
			shaderBuilder.Name = "Shader Builder";
			ti.AttachedControl.Controls.Add(shaderBuilder);
			tcPluginTools.SelectedTab = ti;
			tcPluginTools.ResumeLayout();
		}

		private void btnReload_Click(object sender,EventArgs e)
		{
			DialogResult result = MessageBox.Show("Designer needs to restart in order to reload plugins.  Restart?","Restart Required",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (result == DialogResult.Yes)
				Application.Restart();
		}

		private void btnRebuildFX_Click(object sender,EventArgs e)
		{
			DialogResult result = MessageBox.Show("Designer needs to restart in order to rebuild and reload plugins.  Restart?","Restart Required",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (result == DialogResult.Yes)
			{
				//Set a registry key that we'll check on start up that indicates we should
				//delete all fx plugins 
				RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
				if (key == null)
					key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
				key.SetValue("RebuildPlugins","RebuildFX");
				Application.Restart();
			}
		}

		private void btnRebuildCDD_Click(object sender,EventArgs e)
		{
			DialogResult result = MessageBox.Show("Designer needs to restart in order to rebuild and reload plugins.  Restart?","Restart Required",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (result == DialogResult.Yes)
			{
				//Set a registry key that we'll check on start up that indicates we should
				//delete all cdd plugins 
				RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
				if (key == null)
					key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
				key.SetValue("RebuildPlugins","RebuildCDD");
				Application.Restart();
			}
		}

		private void btnRebuildFXCDD_Click(object sender,EventArgs e)
		{
			DialogResult result = MessageBox.Show("Designer needs to restart in order to rebuild and reload plugins.  Restart?","Restart Required",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (result == DialogResult.Yes)
			{
				//Set a registry key that we'll check on start up that indicates we should
				//delete all cdd plugins 
				RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\XVPDesigner\\GlobalSettings",true);
				if (key == null)
					key = Registry.CurrentUser.CreateSubKey("Software\\XVPDesigner\\GlobalSettings");
				key.SetValue("RebuildPlugins","RebuildAll");
				Application.Restart();
			}
		}

		private void btnNewTextDoc_Click(object sender,EventArgs e)
		{
			TextEditorControl textEditor = new TextEditorControl();
			/*FileSyntaxModeProvider fsmProvider; 
			fsmProvider = new FileSyntaxModeProvider(Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Syntax Highlighting"); // Create new provider with the highlighting directory.
			HighlightingManager.Manager.AddSyntaxModeFileProvider(fsmProvider); // Attach to the text editor.*/
			//textEditor.SetHighlighting("lua"); // Activate the highlighting, use the name from the SyntaxDefinition node.
			textEditor.ShowEOLMarkers = false;
			textEditor.ShowVRuler = false;
			textEditor.ShowLineNumbers = false;

			//textEditor.LoadFile("d:/scratch/ui.lua",true,true);
			
			textEditor.Dock = DockStyle.Fill;
			
			tcPluginTools.SuspendLayout();
			TabItem ti = tcPluginTools.CreateTab("Untitled.txt");
			ti.Name = "Untitled.txt";
			ti.AttachedControl.Controls.Add(textEditor);
			tcPluginTools.SelectedTab = ti;
			tcPluginTools.ResumeLayout();
		}

		private void ribtabDirectorStudio_Click(object sender,EventArgs e)
		{

		}

		private void btnMoveSnap_Click(object sender,EventArgs e)
		{
			StageStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument;
			doc.XVPMControl.enableMoveSnap(btnMoveSnap.Checked);
		}

		private void btnRotateSnap_Click(object sender,EventArgs e)
		{
			StageStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument;
			doc.XVPMControl.enableRotateSnap(btnRotateSnap.Checked);
		}

		private void btnScaleSnap_Click(object sender,EventArgs e)
		{
			StageStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument;
			doc.XVPMControl.enableScaleSnap(btnScaleSnap.Checked);
		}

		private void tbiMoveSnapAmount_InputTextChanged(object sender)
		{
			float amount = 1.0f;
			if (float.TryParse(tbiMoveSnapAmount.ControlText.ToString(),out amount))
			{
				lock (XVPMLockObject)
				{
					uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
					XVPM.setMoveSnapAmount(windowHandle,amount);
				}
			}
		}

		private void tbiRotateSnapAmount_InputTextChanged(object sender)
		{
			float amount = 90.0f;
			if (float.TryParse(tbiRotateSnapAmount.ControlText.ToString(),out amount))
			{
				lock (XVPMLockObject)
				{
					uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
					XVPM.setRotateSnapAmount(windowHandle,amount);
				}
			}
		}

		private void tbiScaleSnapAmount_InputTextChanged(object sender)
		{
			float amount = 1.0f;
			if (float.TryParse(tbiScaleSnapAmount.ControlText.ToString(),out amount))
			{
				lock (XVPMLockObject)
				{
					uint windowHandle = (m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument).XVPMHandle;
					XVPM.setScaleSnapAmount(windowHandle,amount);
				}
			}
		}

		private void btnLockSelection_Click(object sender,EventArgs e)
		{
			//Check viewport hot keys
			if (tcPluginTools.SelectedTab != null)
			{
				if (tcPluginTools.SelectedTab.AttachedControl != null)
				{
					if (tcPluginTools.SelectedTab.AttachedControl.Controls.Count != 0)
					{
						XVPMControl control = tcPluginTools.SelectedTab.AttachedControl.Controls[0] as XVPMControl;
						if (control != null)
						{
							control.toggleLockSelection();
						}
					}
				}
			}
		}

		private void tcPluginTools_TabRemoved(object sender,EventArgs e)
		{
			//If there are no tabs left, disable studios...
			if (tcPluginTools.Tabs.Count == 0)
			{
				enableActorStudio(false);
				enableStageStudio(false,null);
			}
		}

		private void ChangeLightIntensity(object sender,EventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			SliderItem slider = sender as SliderItem;
			uint lightIndex = uint.Parse(slider.Tag as string);
			float sliderValue = (float)slider.Value;
			if (sliderValue == 0.0f)
				sliderValue = .00001f;
			lock (XVPMLockObject)
			{
				XVPM.setScenarioLightIntensity(actorDoc.XVPMHandle,lightIndex,sliderValue/500.0f);
			}
			actorDoc.ViewportConfig.setLightIntensity(lightIndex,slider.Value);
		}

		

		private void cbLightAnimSpeed_SelectedIndexChanged(object sender,EventArgs e)
		{
			if (!loadingComplete)
				return;

			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			lock (XVPMLockObject)
			{
				if (cbLightAnimSpeed.SelectedItem != null)
				{
					if (cbLightAnimSpeed.SelectedItem == ciNoLightAnim)
						XVPM.setScenarioLightAnimSpeed(actorDoc.XVPMHandle,0.0f);
					else if (cbLightAnimSpeed.SelectedItem == ciSlowLightAnim)
						XVPM.setScenarioLightAnimSpeed(actorDoc.XVPMHandle,0.7f);
					else if (cbLightAnimSpeed.SelectedItem == ciMediumLightAnim)
						XVPM.setScenarioLightAnimSpeed(actorDoc.XVPMHandle,1.5f);
					else if (cbLightAnimSpeed.SelectedItem == ciFastLightAnim)
						XVPM.setScenarioLightAnimSpeed(actorDoc.XVPMHandle,3.14f);
				}
			}
			actorDoc.ViewportConfig.LightAnimSpeed = cbLightAnimSpeed.SelectedItem.ToString();
		}

		private void sliderLightAngle_ValueChanged(object sender,EventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			lock (XVPMLockObject)
			{
				XVPM.setScenarioLightAngle(actorDoc.XVPMHandle,sliderLightAngle.Value);
			}
			actorDoc.ViewportConfig.LightingAngle = sliderLightAngle.Value;
		}

		private void chkLightShadows_CheckedChanged(object sender,CheckBoxChangeEventArgs e)
		{
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			lock (XVPMLockObject)
			{
				XVPM.enableShadows(actorDoc.XVPMHandle,chkLightShadows.Checked);
			}
			actorDoc.ViewportConfig.EnableShadows = chkLightShadows.Checked;
		}

		private void btnNewActorFromTemplate_Click(object sender,EventArgs e)
		{
			//Create a new actor
			btnNewActor_Click(null,null);

			//Now run the actor template wizard...
			ActorTemplateDialog dialog = new ActorTemplateDialog();
			DialogResult dr = dialog.ShowDialog();

			if (dr == DialogResult.OK)
			{
				cbActorTemplate.SelectedItem = dialog.ActorTemplateName;
				ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
				actorDoc.ViewportConfig.ActorTemplate = dialog.ActorTemplateName;
			}
		}

		private void btnSaveActorAsTemplate_Click(object sender,EventArgs e)
		{
			//Make sure there is something to save
			if (ActiveSceneExplorer.Hashtable.Count == 0)
			{
				MessageBox.Show("There are no actor nodes to create a template from.","Empty Actor",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			dlgSaveFileDialog.Title = "Save Actor Template...";
			dlgSaveFileDialog.Filter = "Workspace Files (*.acttemp)|*.acttemp|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Template.acttemp";
			dlgSaveFileDialog.InitialDirectory = CurrentWorkspace.ActorTemplatePath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();
			if (result == DialogResult.OK)
			{
				FileStream fs = null;
				TextWriter sw = null;
				try
				{
					fs = File.Open(dlgSaveFileDialog.FileName,FileMode.Create);
					sw = new StreamWriter(fs);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + dlgSaveFileDialog.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					logMessage(exception.ToString(),Color.Red);
					if (sw != null)
						sw.Close();	
					if (fs != null)
						fs.Close();

					return;
				}

				//Write out each plugin into the template
				foreach (TreeNode node in m_activeSceneExplorer.getHierarchyTreeView().Nodes)
				{
					SceneNode sceneNode = (SceneNode)m_activeSceneExplorer.Hashtable[node.Name];
					if (sceneNode == null)
						continue;

					sw.WriteLine(node.Text + ":" + sceneNode.PluginID + ":" + "A fake description.");
				}

				sw.Close();	
				fs.Close();

				//Refresh actor templates
				string oldItem = null;
				if (cbActorTemplate.SelectedItem != null)
					oldItem = cbActorTemplate.SelectedItem.ToString();
				cbActorTemplate.Items.Clear();
				cbActorTemplate.Items.Add("No Template");
				foreach (string filename in Directory.GetFiles(Program.g_mainForm.CurrentWorkspace.ActorTemplatePath))
				{
					cbActorTemplate.Items.Add(Path.GetFileNameWithoutExtension(filename));
				}

				if (oldItem != null)
					cbActorTemplate.SelectedItem = oldItem;
			}
		}

		private void cbActorTemplate_SelectedIndexChanged(object sender,EventArgs e)
		{
			if (!loadingComplete)
				return;
			if (cbActorTemplate.SelectedItem.ToString() == "No Template")
				btnVerifyActor.Enabled = false;
			else
				btnVerifyActor.Enabled = true;
			ActorStudioDocument actorDoc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ActorStudioDocument;
			actorDoc.ViewportConfig.ActorTemplate = cbActorTemplate.SelectedItem.ToString();
		}

		private void btnVerifyActor_Click(object sender,EventArgs e)
		{
			VerifyActorTemplateDialog dialog = new VerifyActorTemplateDialog();
			dialog.verify(cbActorTemplate.SelectedItem.ToString(),ActiveSceneExplorer);
			dialog.Show();
		}

		private void ribbarWorkspace_ItemClick(object sender,EventArgs e)
		{
		}

		private void ribbarWorkspace_LaunchDialog(object sender,EventArgs e)
		{
		}

		private void btnNewScript_Click(object sender,EventArgs e)
		{
			//Create a new script document
			ScriptDocument doc = new ScriptDocument();

			int ctr = 1;
			string newName = null;
			while (true)
			{
				newName = "Untitled" + ctr + ".lua";
				if (m_docMgr.Documents[newName] == null)
					break;
				ctr++;
			}

			TabItem tab = tcPluginTools.CreateTab(newName);
			tcPluginTools.SuspendLayout();
			tab.Name = newName;

			if (doc.create(tab,newName))
			{
				m_docMgr.addDocument(newName,doc);
				doc.setActive(true);

				//Set explorers
				setSceneExplorer(null);
				setPropertyExplorer(null);
				setActionExplorer(null);
				enableActorStudio(false);
				enableStageStudio(false,null);

				tcPluginTools.SelectedTab = tab;
				tcPluginTools.ResumeLayout();
			}
			else
			{
				doc.Dispose();
				tcPluginTools.Tabs.Remove(tab);
				tab.Dispose();
				tab = null;
			}
		}

		private void btnSaveScript_Click(object sender,EventArgs e)
		{
			saveScript();
		}

		private bool saveScript()
		{
			//Get active document and save...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					ScriptDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ScriptDocument;
					if (doc != null)
					{
						string oldName = tcPluginTools.SelectedTab.Name;
						bool success = doc.save();
						if (success)
						{
							ScriptDocument oldDoc = m_docMgr.Documents[oldName] as ScriptDocument;
							m_docMgr.Documents.Remove(oldName);
							m_docMgr.Documents.Add(tcPluginTools.SelectedTab.Name,oldDoc);
						}

						return success;
					}
				}

			return false;
		}

		private void btnOpenScript_Click(object sender,EventArgs e)
		{
			dlgOpenFileDialog.Title = "Open Script...";
			dlgOpenFileDialog.Filter = "Script Files (*.lua)|*.lua|All files (*.*)|*.*";
			dlgOpenFileDialog.FileName = "";
			dlgOpenFileDialog.InitialDirectory = m_currentWorkspace.ScriptLibraryPath;
			DialogResult result = dlgOpenFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				openScriptFromFile(dlgOpenFileDialog.FileName);
		}

		private void btnSaveScriptAs_Click(object sender,EventArgs e)
		{
			saveScriptAs();
		}

		private void btnPaths_Click(object sender,EventArgs e)
		{

		}

		private void btnSaveAny_Click(object sender,EventArgs e)
		{
			BaseDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as BaseDocument;
			if (doc != null)
				doc.save();
		}

		private void btnCompileScript_Click(object sender,EventArgs e)
		{
			//Get active document and compile...
			if (tcPluginTools.SelectedTab != null)
				if (tcPluginTools.SelectedTab.Name != null)
				{
					ScriptDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as ScriptDocument;
					if (doc != null)
					{
						StreamReader errStreamReader;
						StreamReader outStreamReader;
						Program.g_mainForm.logMessage("Compiling script " + tcPluginTools.SelectedTab.Name + "...",Color.Black);
						Process p = new Process();
						p.StartInfo.FileName = Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\tools\\luac-5.0.2.exe";
						p.StartInfo.UseShellExecute = false;
						p.StartInfo.RedirectStandardOutput = true;
						p.StartInfo.RedirectStandardError = true;
						p.StartInfo.CreateNoWindow = true;
						p.StartInfo.Arguments = "-v \"" + tcPluginTools.SelectedTab.Name.Replace("\\","/") +"\"";
						p.Start();
						errStreamReader = p.StandardError;
						outStreamReader = p.StandardOutput;
						while (!p.HasExited || !outStreamReader.EndOfStream || !errStreamReader.EndOfStream)
						{
							string s = outStreamReader.ReadLine();
							if (s != null)
							{
								Program.g_mainForm.logMessage(s,Color.Black);
							}
							s = errStreamReader.ReadLine();
							if (s != null)
							{
								Program.g_mainForm.logMessage(s,Color.Red);
							}

							Application.DoEvents();
						}
					}
				}
		}

		private void chkAOMaps_CheckedChanged(object sender,CheckBoxChangeEventArgs e)
		{
			btnAOMapSettings.Enabled = chkAOMaps.Checked;
		}

		private void chkLightMaps_CheckedChanged(object sender,CheckBoxChangeEventArgs e)
		{
			btnLightMapSettings.Enabled = chkLightMaps.Checked;
		}

		private void btnBakeStage_Click(object sender,EventArgs e)
		{
			if (!chkLightMaps.Checked)
				return;

			//Make sure stage is saved and create the lightmap folder
			saveStage();

			StageStudioDocument doc = m_docMgr.Documents[tcPluginTools.SelectedTab.Name] as StageStudioDocument;
			doc.bakeLightmaps(uint.Parse(cbiMaxTexSize.SelectedItem.ToString()),uint.Parse(txtQuality.ControlText),float.Parse(txtBlur.ControlText));

		}


	}
}


