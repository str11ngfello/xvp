namespace XVPDesigner.UserControls
{
	partial class LibraryExplorer
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LibraryExplorer));
			this.tabsTemplateLibrary = new DevComponents.DotNetBar.TabControl();
			this.tabAudioLibraryPanel = new DevComponents.DotNetBar.TabControlPanel();
			this.scAudioLibrary = new System.Windows.Forms.SplitContainer();
			this.tvAudioLibrary = new System.Windows.Forms.TreeView();
			this.imglistAudio = new System.Windows.Forms.ImageList(this.components);
			this.bar5 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllAudioFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllAudioFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnAudioLocation = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.txtAudioFilter = new System.Windows.Forms.TextBox();
			this.chkEnableAudioFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.axWindowsMediaPlayer = new AxWMPLib.AxWindowsMediaPlayer();
			this.tabAudioLibrary = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel3 = new DevComponents.DotNetBar.TabControlPanel();
			this.scTextureLibrary = new System.Windows.Forms.SplitContainer();
			this.tvTextureLibrary = new System.Windows.Forms.TreeView();
			this.imglistTextures = new System.Windows.Forms.ImageList(this.components);
			this.bar4 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllTextureFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllTextureFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnTextureLocation = new DevComponents.DotNetBar.ButtonItem();
			this.btnDelteCompressed = new DevComponents.DotNetBar.ButtonItem();
			this.btnDeleteAllCompressed = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox7 = new System.Windows.Forms.GroupBox();
			this.txtTextureFilter = new System.Windows.Forms.TextBox();
			this.chkEnableTextureFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.tabTextureLibrary = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel2 = new DevComponents.DotNetBar.TabControlPanel();
			this.scControllerLibrary = new System.Windows.Forms.SplitContainer();
			this.tvControllerLibrary = new System.Windows.Forms.TreeView();
			this.imglistControllers = new System.Windows.Forms.ImageList(this.components);
			this.bar3 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllControllerFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllControllerFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnOpenControllerLocation = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox8 = new System.Windows.Forms.GroupBox();
			this.txtControllerFilter = new System.Windows.Forms.TextBox();
			this.chkEnableControllerFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.pgridControllers = new System.Windows.Forms.PropertyGrid();
			this.tabControllerLibrary = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabMaterialTemplatesPanel = new DevComponents.DotNetBar.TabControlPanel();
			this.scMaterialLibrary = new System.Windows.Forms.SplitContainer();
			this.tvMaterialLibrary = new System.Windows.Forms.TreeView();
			this.imglistMaterials = new System.Windows.Forms.ImageList(this.components);
			this.bar2 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllMaterialFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllMaterialFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnOpenMaterialLocation = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox9 = new System.Windows.Forms.GroupBox();
			this.txtMaterialFilter = new System.Windows.Forms.TextBox();
			this.chkEnableMaterialFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.pgridMaterials = new System.Windows.Forms.PropertyGrid();
			this.tabMaterials = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabTemplatePanel = new DevComponents.DotNetBar.TabControlPanel();
			this.scActorLibrary = new System.Windows.Forms.SplitContainer();
			this.tvActorLibrary = new System.Windows.Forms.TreeView();
			this.imglistActors = new System.Windows.Forms.ImageList(this.components);
			this.groupBox10 = new System.Windows.Forms.GroupBox();
			this.txtActorFilter = new System.Windows.Forms.TextBox();
			this.chkEnableActorFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.bar1 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllActorFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllActorFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnOpenExplorerLocation = new DevComponents.DotNetBar.ButtonItem();
			this.btnOneClickAdd = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.tvActorPreview = new System.Windows.Forms.TreeView();
			this.tabActors = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel1 = new DevComponents.DotNetBar.TabControlPanel();
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.actorDirWatcher = new System.IO.FileSystemWatcher();
			this.materialDirWatcher = new System.IO.FileSystemWatcher();
			this.audioDirWatcher = new System.IO.FileSystemWatcher();
			this.controllerDirWatcher = new System.IO.FileSystemWatcher();
			this.textureDirWatcher = new System.IO.FileSystemWatcher();
			this.bitmapLoadWorker = new System.ComponentModel.BackgroundWorker();
			this.actorPreviewWorker = new System.ComponentModel.BackgroundWorker();
			this.materialPreviewWorker = new System.ComponentModel.BackgroundWorker();
			this.controllerPreviewWorker = new System.ComponentModel.BackgroundWorker();
			this.tsbExpandAllHierarchyNodes = new System.Windows.Forms.ToolStripButton();
			this.tsbCollapseAllHierarchyNodes = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.tsbHierarchyView = new System.Windows.Forms.ToolStripButton();
			this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
			this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripButton5 = new System.Windows.Forms.ToolStripButton();
			this.superTooltip1 = new DevComponents.DotNetBar.SuperTooltip();
			this.scriptDirWatcher = new System.IO.FileSystemWatcher();
			this.tabScriptLibrary = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel4 = new DevComponents.DotNetBar.TabControlPanel();
			this.tabControlPanel5 = new DevComponents.DotNetBar.TabControlPanel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.tvScriptLibrary = new System.Windows.Forms.TreeView();
			this.bar6 = new DevComponents.DotNetBar.Bar();
			this.btnExpandAllScriptFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnCollapseAllScriptFolders = new DevComponents.DotNetBar.ButtonItem();
			this.btnOpenScriptLocation = new DevComponents.DotNetBar.ButtonItem();
			this.groupBox11 = new System.Windows.Forms.GroupBox();
			this.txtScriptFilter = new System.Windows.Forms.TextBox();
			this.chkEnableScriptFilter = new DevComponents.DotNetBar.Controls.CheckBoxX();
			this.groupBox12 = new System.Windows.Forms.GroupBox();
			this.imglistScripts = new System.Windows.Forms.ImageList(this.components);
			this.txtScriptPreview = new ICSharpCode.TextEditor.TextEditorControl();
			this.texturePreview = new TextureBrowserLibrary.TextureBrowser();
			((System.ComponentModel.ISupportInitialize)(this.tabsTemplateLibrary)).BeginInit();
			this.tabsTemplateLibrary.SuspendLayout();
			this.tabAudioLibraryPanel.SuspendLayout();
			this.scAudioLibrary.Panel1.SuspendLayout();
			this.scAudioLibrary.Panel2.SuspendLayout();
			this.scAudioLibrary.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar5)).BeginInit();
			this.groupBox6.SuspendLayout();
			this.groupBox5.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.axWindowsMediaPlayer)).BeginInit();
			this.tabControlPanel3.SuspendLayout();
			this.scTextureLibrary.Panel1.SuspendLayout();
			this.scTextureLibrary.Panel2.SuspendLayout();
			this.scTextureLibrary.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar4)).BeginInit();
			this.groupBox7.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.tabControlPanel2.SuspendLayout();
			this.scControllerLibrary.Panel1.SuspendLayout();
			this.scControllerLibrary.Panel2.SuspendLayout();
			this.scControllerLibrary.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar3)).BeginInit();
			this.groupBox8.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.tabMaterialTemplatesPanel.SuspendLayout();
			this.scMaterialLibrary.Panel1.SuspendLayout();
			this.scMaterialLibrary.Panel2.SuspendLayout();
			this.scMaterialLibrary.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar2)).BeginInit();
			this.groupBox9.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.tabTemplatePanel.SuspendLayout();
			this.scActorLibrary.Panel1.SuspendLayout();
			this.scActorLibrary.Panel2.SuspendLayout();
			this.scActorLibrary.SuspendLayout();
			this.groupBox10.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).BeginInit();
			this.groupBox2.SuspendLayout();
			this.tabControlPanel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.actorDirWatcher)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.materialDirWatcher)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.audioDirWatcher)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.controllerDirWatcher)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.textureDirWatcher)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.scriptDirWatcher)).BeginInit();
			this.tabControlPanel4.SuspendLayout();
			this.tabControlPanel5.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar6)).BeginInit();
			this.groupBox11.SuspendLayout();
			this.groupBox12.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabsTemplateLibrary
			// 
			this.tabsTemplateLibrary.CanReorderTabs = true;
			this.tabsTemplateLibrary.Controls.Add(this.tabControlPanel4);
			this.tabsTemplateLibrary.Controls.Add(this.tabTemplatePanel);
			this.tabsTemplateLibrary.Controls.Add(this.tabControlPanel3);
			this.tabsTemplateLibrary.Controls.Add(this.tabMaterialTemplatesPanel);
			this.tabsTemplateLibrary.Controls.Add(this.tabControlPanel2);
			this.tabsTemplateLibrary.Controls.Add(this.tabAudioLibraryPanel);
			this.tabsTemplateLibrary.Controls.Add(this.tabControlPanel1);
			this.tabsTemplateLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabsTemplateLibrary.Font = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Regular,System.Drawing.GraphicsUnit.Point,((byte)(0)));
			this.tabsTemplateLibrary.Location = new System.Drawing.Point(0,0);
			this.tabsTemplateLibrary.Name = "tabsTemplateLibrary";
			this.tabsTemplateLibrary.SelectedTabFont = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Bold);
			this.tabsTemplateLibrary.SelectedTabIndex = 0;
			this.tabsTemplateLibrary.Size = new System.Drawing.Size(419,578);
			this.tabsTemplateLibrary.Style = DevComponents.DotNetBar.eTabStripStyle.Office2007Document;
			this.tabsTemplateLibrary.TabAlignment = DevComponents.DotNetBar.eTabStripAlignment.Bottom;
			this.tabsTemplateLibrary.TabIndex = 1;
			this.tabsTemplateLibrary.TabLayoutType = DevComponents.DotNetBar.eTabLayoutType.MultilineNoNavigationBox;
			this.tabsTemplateLibrary.Tabs.Add(this.tabActors);
			this.tabsTemplateLibrary.Tabs.Add(this.tabMaterials);
			this.tabsTemplateLibrary.Tabs.Add(this.tabControllerLibrary);
			this.tabsTemplateLibrary.Tabs.Add(this.tabTextureLibrary);
			this.tabsTemplateLibrary.Tabs.Add(this.tabAudioLibrary);
			this.tabsTemplateLibrary.Tabs.Add(this.tabScriptLibrary);
			this.tabsTemplateLibrary.Text = "tabActors";
			// 
			// tabAudioLibraryPanel
			// 
			this.tabAudioLibraryPanel.Controls.Add(this.scAudioLibrary);
			this.tabAudioLibraryPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabAudioLibraryPanel.Location = new System.Drawing.Point(0,0);
			this.tabAudioLibraryPanel.Name = "tabAudioLibraryPanel";
			this.tabAudioLibraryPanel.Padding = new System.Windows.Forms.Padding(1);
			this.tabAudioLibraryPanel.Size = new System.Drawing.Size(419,531);
			this.tabAudioLibraryPanel.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabAudioLibraryPanel.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabAudioLibraryPanel.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabAudioLibraryPanel.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabAudioLibraryPanel.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabAudioLibraryPanel.Style.GradientAngle = -90;
			this.tabAudioLibraryPanel.TabIndex = 8;
			this.tabAudioLibraryPanel.TabItem = this.tabAudioLibrary;
			// 
			// scAudioLibrary
			// 
			this.scAudioLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scAudioLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scAudioLibrary.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
			this.scAudioLibrary.Location = new System.Drawing.Point(1,1);
			this.scAudioLibrary.Name = "scAudioLibrary";
			this.scAudioLibrary.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scAudioLibrary.Panel1
			// 
			this.scAudioLibrary.Panel1.Controls.Add(this.tvAudioLibrary);
			this.scAudioLibrary.Panel1.Controls.Add(this.bar5);
			this.scAudioLibrary.Panel1.Controls.Add(this.groupBox6);
			// 
			// scAudioLibrary.Panel2
			// 
			this.scAudioLibrary.Panel2.Controls.Add(this.groupBox5);
			this.scAudioLibrary.Panel2MinSize = 64;
			this.scAudioLibrary.Size = new System.Drawing.Size(417,529);
			this.scAudioLibrary.SplitterDistance = 437;
			this.scAudioLibrary.TabIndex = 0;
			// 
			// tvAudioLibrary
			// 
			this.tvAudioLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tvAudioLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvAudioLibrary.HideSelection = false;
			this.tvAudioLibrary.ImageIndex = 0;
			this.tvAudioLibrary.ImageList = this.imglistAudio;
			this.tvAudioLibrary.Location = new System.Drawing.Point(0,25);
			this.tvAudioLibrary.Name = "tvAudioLibrary";
			this.tvAudioLibrary.SelectedImageIndex = 0;
			this.tvAudioLibrary.Size = new System.Drawing.Size(415,363);
			this.tvAudioLibrary.TabIndex = 2;
			this.tvAudioLibrary.DoubleClick += new System.EventHandler(this.tvAudioLibrary_DoubleClick);
			this.tvAudioLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvAudioLibrary_AfterSelect);
			this.tvAudioLibrary.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvAudioLibrary_MouseDown);
			// 
			// imglistAudio
			// 
			this.imglistAudio.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistAudio.ImageStream")));
			this.imglistAudio.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistAudio.Images.SetKeyName(0,"Audio");
			this.imglistAudio.Images.SetKeyName(1,"Folder");
			// 
			// bar5
			// 
			this.bar5.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar5.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllAudioFolders,
            this.btnCollapseAllAudioFolders,
            this.btnAudioLocation});
			this.bar5.Location = new System.Drawing.Point(0,0);
			this.bar5.Name = "bar5";
			this.bar5.RoundCorners = false;
			this.bar5.Size = new System.Drawing.Size(415,25);
			this.bar5.Stretch = true;
			this.bar5.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar5.TabIndex = 5;
			this.bar5.TabStop = false;
			this.bar5.Text = "bar5";
			// 
			// btnExpandAllAudioFolders
			// 
			this.btnExpandAllAudioFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllAudioFolders.Image")));
			this.btnExpandAllAudioFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllAudioFolders.Name = "btnExpandAllAudioFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllAudioFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all audio folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllAudioFolders.Text = "buttonItem1";
			this.btnExpandAllAudioFolders.Click += new System.EventHandler(this.tsbExpandAllAudioFolders_Click);
			// 
			// btnCollapseAllAudioFolders
			// 
			this.btnCollapseAllAudioFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllAudioFolders.Image")));
			this.btnCollapseAllAudioFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllAudioFolders.Name = "btnCollapseAllAudioFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllAudioFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all audio folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllAudioFolders.Text = "buttonItem2";
			this.btnCollapseAllAudioFolders.Click += new System.EventHandler(this.tsbCollapseAllAudioFolders_Click);
			// 
			// btnAudioLocation
			// 
			this.btnAudioLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnAudioLocation.Image")));
			this.btnAudioLocation.ImagePaddingHorizontal = 8;
			this.btnAudioLocation.Name = "btnAudioLocation";
			this.superTooltip1.SetSuperTooltip(this.btnAudioLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Audio Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnAudioLocation.Text = "buttonItem3";
			this.btnAudioLocation.Click += new System.EventHandler(this.tsbAudioLocation_Click);
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.txtAudioFilter);
			this.groupBox6.Controls.Add(this.chkEnableAudioFilter);
			this.groupBox6.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox6.Location = new System.Drawing.Point(0,388);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(415,47);
			this.groupBox6.TabIndex = 4;
			this.groupBox6.TabStop = false;
			// 
			// txtAudioFilter
			// 
			this.txtAudioFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtAudioFilter.Enabled = false;
			this.txtAudioFilter.Location = new System.Drawing.Point(71,19);
			this.txtAudioFilter.Name = "txtAudioFilter";
			this.txtAudioFilter.Size = new System.Drawing.Size(338,20);
			this.txtAudioFilter.TabIndex = 4;
			this.txtAudioFilter.TextChanged += new System.EventHandler(this.txtAudioFilter_TextChanged);
			// 
			// chkEnableAudioFilter
			// 
			this.chkEnableAudioFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableAudioFilter.Location = new System.Drawing.Point(6,18);
			this.chkEnableAudioFilter.Name = "chkEnableAudioFilter";
			this.chkEnableAudioFilter.Size = new System.Drawing.Size(62,20);
			this.superTooltip1.SetSuperTooltip(this.chkEnableAudioFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Audio Library with a string.  Only audio files that contain the string" +
            " in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableAudioFilter.TabIndex = 3;
			this.chkEnableAudioFilter.Text = "Filter:";
			this.chkEnableAudioFilter.CheckedChanged += new System.EventHandler(this.chkEnableAudioFilter_CheckedChanged);
			// 
			// groupBox5
			// 
			this.groupBox5.Controls.Add(this.axWindowsMediaPlayer);
			this.groupBox5.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBox5.Location = new System.Drawing.Point(0,0);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(415,86);
			this.groupBox5.TabIndex = 1;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Audio Preview";
			// 
			// axWindowsMediaPlayer
			// 
			this.axWindowsMediaPlayer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.axWindowsMediaPlayer.Enabled = true;
			this.axWindowsMediaPlayer.Location = new System.Drawing.Point(3,16);
			this.axWindowsMediaPlayer.Name = "axWindowsMediaPlayer";
			this.axWindowsMediaPlayer.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axWindowsMediaPlayer.OcxState")));
			this.axWindowsMediaPlayer.Size = new System.Drawing.Size(409,67);
			this.axWindowsMediaPlayer.TabIndex = 0;
			// 
			// tabAudioLibrary
			// 
			this.tabAudioLibrary.AttachedControl = this.tabAudioLibraryPanel;
			this.tabAudioLibrary.Image = ((System.Drawing.Image)(resources.GetObject("tabAudioLibrary.Image")));
			this.tabAudioLibrary.Name = "tabAudioLibrary";
			this.superTooltip1.SetSuperTooltip(this.tabAudioLibrary,new DevComponents.DotNetBar.SuperTooltipInfo("Audio Library","",resources.GetString("tabAudioLibrary.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.tabAudioLibrary.Text = "Audio Library";
			// 
			// tabControlPanel3
			// 
			this.tabControlPanel3.Controls.Add(this.scTextureLibrary);
			this.tabControlPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel3.Location = new System.Drawing.Point(0,0);
			this.tabControlPanel3.Name = "tabControlPanel3";
			this.tabControlPanel3.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel3.Size = new System.Drawing.Size(419,531);
			this.tabControlPanel3.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel3.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel3.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel3.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel3.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanel3.Style.GradientAngle = -90;
			this.tabControlPanel3.TabIndex = 7;
			this.tabControlPanel3.TabItem = this.tabTextureLibrary;
			// 
			// scTextureLibrary
			// 
			this.scTextureLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scTextureLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scTextureLibrary.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
			this.scTextureLibrary.Location = new System.Drawing.Point(1,1);
			this.scTextureLibrary.Name = "scTextureLibrary";
			this.scTextureLibrary.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scTextureLibrary.Panel1
			// 
			this.scTextureLibrary.Panel1.Controls.Add(this.tvTextureLibrary);
			this.scTextureLibrary.Panel1.Controls.Add(this.bar4);
			this.scTextureLibrary.Panel1.Controls.Add(this.groupBox7);
			// 
			// scTextureLibrary.Panel2
			// 
			this.scTextureLibrary.Panel2.Controls.Add(this.groupBox1);
			this.scTextureLibrary.Size = new System.Drawing.Size(417,529);
			this.scTextureLibrary.SplitterDistance = 322;
			this.scTextureLibrary.TabIndex = 0;
			// 
			// tvTextureLibrary
			// 
			this.tvTextureLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvTextureLibrary.HideSelection = false;
			this.tvTextureLibrary.ImageIndex = 0;
			this.tvTextureLibrary.ImageList = this.imglistTextures;
			this.tvTextureLibrary.Location = new System.Drawing.Point(0,25);
			this.tvTextureLibrary.Name = "tvTextureLibrary";
			this.tvTextureLibrary.SelectedImageIndex = 0;
			this.tvTextureLibrary.Size = new System.Drawing.Size(415,249);
			this.tvTextureLibrary.TabIndex = 3;
			this.tvTextureLibrary.DoubleClick += new System.EventHandler(this.tvTextureLibrary_DoubleClick);
			this.tvTextureLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvTextureLibrary_AfterSelect);
			this.tvTextureLibrary.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvTextureLibrary_MouseDown);
			// 
			// imglistTextures
			// 
			this.imglistTextures.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistTextures.ImageStream")));
			this.imglistTextures.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistTextures.Images.SetKeyName(0,"Folder");
			this.imglistTextures.Images.SetKeyName(1,"Texture");
			// 
			// bar4
			// 
			this.bar4.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar4.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllTextureFolders,
            this.btnCollapseAllTextureFolders,
            this.btnTextureLocation,
            this.btnDelteCompressed,
            this.btnDeleteAllCompressed});
			this.bar4.Location = new System.Drawing.Point(0,0);
			this.bar4.Name = "bar4";
			this.bar4.RoundCorners = false;
			this.bar4.Size = new System.Drawing.Size(415,25);
			this.bar4.Stretch = true;
			this.bar4.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar4.TabIndex = 6;
			this.bar4.TabStop = false;
			this.bar4.Text = "bar4";
			// 
			// btnExpandAllTextureFolders
			// 
			this.btnExpandAllTextureFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllTextureFolders.Image")));
			this.btnExpandAllTextureFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllTextureFolders.Name = "btnExpandAllTextureFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllTextureFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all texture folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllTextureFolders.Text = "buttonItem1";
			this.btnExpandAllTextureFolders.Click += new System.EventHandler(this.tsbExpandAllTextureFolders_Click);
			// 
			// btnCollapseAllTextureFolders
			// 
			this.btnCollapseAllTextureFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllTextureFolders.Image")));
			this.btnCollapseAllTextureFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllTextureFolders.Name = "btnCollapseAllTextureFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllTextureFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all texture folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllTextureFolders.Text = "buttonItem2";
			this.btnCollapseAllTextureFolders.Click += new System.EventHandler(this.tsbCollapseAllTextureFolders_Click);
			// 
			// btnTextureLocation
			// 
			this.btnTextureLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnTextureLocation.Image")));
			this.btnTextureLocation.ImagePaddingHorizontal = 8;
			this.btnTextureLocation.Name = "btnTextureLocation";
			this.superTooltip1.SetSuperTooltip(this.btnTextureLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Texture Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnTextureLocation.Text = "buttonItem3";
			this.btnTextureLocation.Click += new System.EventHandler(this.tsbTextureLocation_Click);
			// 
			// btnDelteCompressed
			// 
			this.btnDelteCompressed.Image = ((System.Drawing.Image)(resources.GetObject("btnDelteCompressed.Image")));
			this.btnDelteCompressed.ImagePaddingHorizontal = 8;
			this.btnDelteCompressed.Name = "btnDelteCompressed";
			this.superTooltip1.SetSuperTooltip(this.btnDelteCompressed,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete cached version of selected texture",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDelteCompressed.Text = "buttonItem4";
			this.btnDelteCompressed.Click += new System.EventHandler(this.tsbDeleteCompressedVersion_Click);
			// 
			// btnDeleteAllCompressed
			// 
			this.btnDeleteAllCompressed.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteAllCompressed.Image")));
			this.btnDeleteAllCompressed.ImagePaddingHorizontal = 8;
			this.btnDeleteAllCompressed.Name = "btnDeleteAllCompressed";
			this.superTooltip1.SetSuperTooltip(this.btnDeleteAllCompressed,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete the entire texure cache",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDeleteAllCompressed.Text = "buttonItem5";
			this.btnDeleteAllCompressed.Click += new System.EventHandler(this.tsbDeleteAllCompressedVersions_Click);
			// 
			// groupBox7
			// 
			this.groupBox7.Controls.Add(this.txtTextureFilter);
			this.groupBox7.Controls.Add(this.chkEnableTextureFilter);
			this.groupBox7.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox7.Location = new System.Drawing.Point(0,274);
			this.groupBox7.Name = "groupBox7";
			this.groupBox7.Size = new System.Drawing.Size(415,46);
			this.groupBox7.TabIndex = 5;
			this.groupBox7.TabStop = false;
			// 
			// txtTextureFilter
			// 
			this.txtTextureFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtTextureFilter.Enabled = false;
			this.txtTextureFilter.Location = new System.Drawing.Point(74,19);
			this.txtTextureFilter.Name = "txtTextureFilter";
			this.txtTextureFilter.Size = new System.Drawing.Size(338,20);
			this.txtTextureFilter.TabIndex = 4;
			this.txtTextureFilter.TextChanged += new System.EventHandler(this.txtTextureFilter_TextChanged);
			// 
			// chkEnableTextureFilter
			// 
			this.chkEnableTextureFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableTextureFilter.Location = new System.Drawing.Point(8,18);
			this.chkEnableTextureFilter.Name = "chkEnableTextureFilter";
			this.chkEnableTextureFilter.Size = new System.Drawing.Size(62,19);
			this.superTooltip1.SetSuperTooltip(this.chkEnableTextureFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Texture Library with a string.  Only those textures that contain the s" +
            "tring in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableTextureFilter.TabIndex = 3;
			this.chkEnableTextureFilter.Text = "Filter:";
			this.chkEnableTextureFilter.CheckedChanged += new System.EventHandler(this.chkEnableTextureFilter_CheckedChanged);
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.texturePreview);
			this.groupBox1.Location = new System.Drawing.Point(3,2);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(409,196);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Texture Explorer";
			// 
			// tabTextureLibrary
			// 
			this.tabTextureLibrary.AttachedControl = this.tabControlPanel3;
			this.tabTextureLibrary.Image = ((System.Drawing.Image)(resources.GetObject("tabTextureLibrary.Image")));
			this.tabTextureLibrary.Name = "tabTextureLibrary";
			this.superTooltip1.SetSuperTooltip(this.tabTextureLibrary,new DevComponents.DotNetBar.SuperTooltipInfo("Texture Library","",resources.GetString("tabTextureLibrary.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.tabTextureLibrary.Text = "Texture Library";
			// 
			// tabControlPanel2
			// 
			this.tabControlPanel2.Controls.Add(this.scControllerLibrary);
			this.tabControlPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel2.Location = new System.Drawing.Point(0,0);
			this.tabControlPanel2.Name = "tabControlPanel2";
			this.tabControlPanel2.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel2.Size = new System.Drawing.Size(419,531);
			this.tabControlPanel2.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel2.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel2.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel2.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel2.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanel2.Style.GradientAngle = -90;
			this.tabControlPanel2.TabIndex = 6;
			this.tabControlPanel2.TabItem = this.tabControllerLibrary;
			// 
			// scControllerLibrary
			// 
			this.scControllerLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scControllerLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scControllerLibrary.Location = new System.Drawing.Point(1,1);
			this.scControllerLibrary.Name = "scControllerLibrary";
			this.scControllerLibrary.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scControllerLibrary.Panel1
			// 
			this.scControllerLibrary.Panel1.Controls.Add(this.tvControllerLibrary);
			this.scControllerLibrary.Panel1.Controls.Add(this.bar3);
			this.scControllerLibrary.Panel1.Controls.Add(this.groupBox8);
			// 
			// scControllerLibrary.Panel2
			// 
			this.scControllerLibrary.Panel2.Controls.Add(this.groupBox4);
			this.scControllerLibrary.Size = new System.Drawing.Size(417,529);
			this.scControllerLibrary.SplitterDistance = 302;
			this.scControllerLibrary.TabIndex = 0;
			// 
			// tvControllerLibrary
			// 
			this.tvControllerLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvControllerLibrary.HideSelection = false;
			this.tvControllerLibrary.ImageIndex = 0;
			this.tvControllerLibrary.ImageList = this.imglistControllers;
			this.tvControllerLibrary.Location = new System.Drawing.Point(0,25);
			this.tvControllerLibrary.Name = "tvControllerLibrary";
			this.tvControllerLibrary.SelectedImageIndex = 0;
			this.tvControllerLibrary.Size = new System.Drawing.Size(415,229);
			this.tvControllerLibrary.TabIndex = 2;
			this.tvControllerLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvControllerLibrary_AfterSelect);
			this.tvControllerLibrary.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvControllerLibrary_MouseDown);
			// 
			// imglistControllers
			// 
			this.imglistControllers.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistControllers.ImageStream")));
			this.imglistControllers.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistControllers.Images.SetKeyName(0,"Controller");
			this.imglistControllers.Images.SetKeyName(1,"Folder");
			// 
			// bar3
			// 
			this.bar3.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar3.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllControllerFolders,
            this.btnCollapseAllControllerFolders,
            this.btnOpenControllerLocation});
			this.bar3.Location = new System.Drawing.Point(0,0);
			this.bar3.Name = "bar3";
			this.bar3.RoundCorners = false;
			this.bar3.Size = new System.Drawing.Size(415,25);
			this.bar3.Stretch = true;
			this.bar3.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar3.TabIndex = 7;
			this.bar3.TabStop = false;
			this.bar3.Text = "bar3";
			// 
			// btnExpandAllControllerFolders
			// 
			this.btnExpandAllControllerFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllControllerFolders.Image")));
			this.btnExpandAllControllerFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllControllerFolders.Name = "btnExpandAllControllerFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllControllerFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all controller folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllControllerFolders.Text = "buttonItem1";
			this.btnExpandAllControllerFolders.Click += new System.EventHandler(this.tsbExpandAllControllerFolders_Click);
			// 
			// btnCollapseAllControllerFolders
			// 
			this.btnCollapseAllControllerFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllControllerFolders.Image")));
			this.btnCollapseAllControllerFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllControllerFolders.Name = "btnCollapseAllControllerFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllControllerFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all controller folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllControllerFolders.Text = "buttonItem2";
			this.btnCollapseAllControllerFolders.Click += new System.EventHandler(this.tsbCollapseAllControllerFolders_Click);
			// 
			// btnOpenControllerLocation
			// 
			this.btnOpenControllerLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnOpenControllerLocation.Image")));
			this.btnOpenControllerLocation.ImagePaddingHorizontal = 8;
			this.btnOpenControllerLocation.Name = "btnOpenControllerLocation";
			this.superTooltip1.SetSuperTooltip(this.btnOpenControllerLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Controller Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnOpenControllerLocation.Text = "buttonItem3";
			this.btnOpenControllerLocation.Click += new System.EventHandler(this.tsbControllerLocation_Click);
			// 
			// groupBox8
			// 
			this.groupBox8.Controls.Add(this.txtControllerFilter);
			this.groupBox8.Controls.Add(this.chkEnableControllerFilter);
			this.groupBox8.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox8.Location = new System.Drawing.Point(0,254);
			this.groupBox8.Name = "groupBox8";
			this.groupBox8.Size = new System.Drawing.Size(415,46);
			this.groupBox8.TabIndex = 6;
			this.groupBox8.TabStop = false;
			// 
			// txtControllerFilter
			// 
			this.txtControllerFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtControllerFilter.Enabled = false;
			this.txtControllerFilter.Location = new System.Drawing.Point(70,19);
			this.txtControllerFilter.Name = "txtControllerFilter";
			this.txtControllerFilter.Size = new System.Drawing.Size(339,20);
			this.txtControllerFilter.TabIndex = 4;
			this.txtControllerFilter.TextChanged += new System.EventHandler(this.txtControllerFilter_TextChanged);
			// 
			// chkEnableControllerFilter
			// 
			this.chkEnableControllerFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableControllerFilter.Location = new System.Drawing.Point(6,18);
			this.chkEnableControllerFilter.Name = "chkEnableControllerFilter";
			this.chkEnableControllerFilter.Size = new System.Drawing.Size(67,19);
			this.superTooltip1.SetSuperTooltip(this.chkEnableControllerFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Controller Library with a string.  Only those controllers that contain" +
            " the string in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableControllerFilter.TabIndex = 3;
			this.chkEnableControllerFilter.Text = "Filter:";
			this.chkEnableControllerFilter.CheckedChanged += new System.EventHandler(this.chkEnableControllerFilter_CheckedChanged);
			// 
			// groupBox4
			// 
			this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox4.Controls.Add(this.pgridControllers);
			this.groupBox4.Location = new System.Drawing.Point(3,3);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(409,215);
			this.groupBox4.TabIndex = 2;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Controller Preview";
			// 
			// pgridControllers
			// 
			this.pgridControllers.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pgridControllers.Location = new System.Drawing.Point(3,16);
			this.pgridControllers.Name = "pgridControllers";
			this.pgridControllers.Size = new System.Drawing.Size(403,196);
			this.pgridControllers.TabIndex = 1;
			this.pgridControllers.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.pgridControllers_PropertyValueChanged);
			// 
			// tabControllerLibrary
			// 
			this.tabControllerLibrary.AttachedControl = this.tabControlPanel2;
			this.tabControllerLibrary.Image = ((System.Drawing.Image)(resources.GetObject("tabControllerLibrary.Image")));
			this.tabControllerLibrary.Name = "tabControllerLibrary";
			this.superTooltip1.SetSuperTooltip(this.tabControllerLibrary,new DevComponents.DotNetBar.SuperTooltipInfo("Controller Library","",resources.GetString("tabControllerLibrary.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.tabControllerLibrary.Text = "Controller Library";
			// 
			// tabMaterialTemplatesPanel
			// 
			this.tabMaterialTemplatesPanel.Controls.Add(this.scMaterialLibrary);
			this.tabMaterialTemplatesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabMaterialTemplatesPanel.Location = new System.Drawing.Point(0,0);
			this.tabMaterialTemplatesPanel.Name = "tabMaterialTemplatesPanel";
			this.tabMaterialTemplatesPanel.Padding = new System.Windows.Forms.Padding(1);
			this.tabMaterialTemplatesPanel.Size = new System.Drawing.Size(419,531);
			this.tabMaterialTemplatesPanel.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabMaterialTemplatesPanel.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabMaterialTemplatesPanel.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabMaterialTemplatesPanel.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabMaterialTemplatesPanel.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabMaterialTemplatesPanel.Style.GradientAngle = -90;
			this.tabMaterialTemplatesPanel.TabIndex = 5;
			this.tabMaterialTemplatesPanel.TabItem = this.tabMaterials;
			// 
			// scMaterialLibrary
			// 
			this.scMaterialLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scMaterialLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scMaterialLibrary.Location = new System.Drawing.Point(1,1);
			this.scMaterialLibrary.Name = "scMaterialLibrary";
			this.scMaterialLibrary.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scMaterialLibrary.Panel1
			// 
			this.scMaterialLibrary.Panel1.Controls.Add(this.tvMaterialLibrary);
			this.scMaterialLibrary.Panel1.Controls.Add(this.bar2);
			this.scMaterialLibrary.Panel1.Controls.Add(this.groupBox9);
			// 
			// scMaterialLibrary.Panel2
			// 
			this.scMaterialLibrary.Panel2.Controls.Add(this.groupBox3);
			this.scMaterialLibrary.Size = new System.Drawing.Size(417,529);
			this.scMaterialLibrary.SplitterDistance = 308;
			this.scMaterialLibrary.TabIndex = 3;
			// 
			// tvMaterialLibrary
			// 
			this.tvMaterialLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvMaterialLibrary.HideSelection = false;
			this.tvMaterialLibrary.ImageIndex = 0;
			this.tvMaterialLibrary.ImageList = this.imglistMaterials;
			this.tvMaterialLibrary.Location = new System.Drawing.Point(0,25);
			this.tvMaterialLibrary.Name = "tvMaterialLibrary";
			this.tvMaterialLibrary.SelectedImageIndex = 0;
			this.tvMaterialLibrary.Size = new System.Drawing.Size(415,235);
			this.tvMaterialLibrary.TabIndex = 1;
			this.tvMaterialLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvMaterialLibrary_AfterSelect);
			this.tvMaterialLibrary.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvMaterialLibrary_MouseDown);
			// 
			// imglistMaterials
			// 
			this.imglistMaterials.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistMaterials.ImageStream")));
			this.imglistMaterials.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistMaterials.Images.SetKeyName(0,"Material");
			this.imglistMaterials.Images.SetKeyName(1,"Folder");
			// 
			// bar2
			// 
			this.bar2.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar2.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllMaterialFolders,
            this.btnCollapseAllMaterialFolders,
            this.btnOpenMaterialLocation});
			this.bar2.Location = new System.Drawing.Point(0,0);
			this.bar2.Name = "bar2";
			this.bar2.RoundCorners = false;
			this.bar2.Size = new System.Drawing.Size(415,25);
			this.bar2.Stretch = true;
			this.bar2.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar2.TabIndex = 8;
			this.bar2.TabStop = false;
			this.bar2.Text = "bar2";
			// 
			// btnExpandAllMaterialFolders
			// 
			this.btnExpandAllMaterialFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllMaterialFolders.Image")));
			this.btnExpandAllMaterialFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllMaterialFolders.Name = "btnExpandAllMaterialFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllMaterialFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all material folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllMaterialFolders.Text = "buttonItem1";
			this.btnExpandAllMaterialFolders.Click += new System.EventHandler(this.tsbExpandAllMaterialFolders_Click);
			// 
			// btnCollapseAllMaterialFolders
			// 
			this.btnCollapseAllMaterialFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllMaterialFolders.Image")));
			this.btnCollapseAllMaterialFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllMaterialFolders.Name = "btnCollapseAllMaterialFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllMaterialFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all material folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllMaterialFolders.Text = "buttonItem2";
			this.btnCollapseAllMaterialFolders.Click += new System.EventHandler(this.tsbCollapseAllMaterialFolders_Click);
			// 
			// btnOpenMaterialLocation
			// 
			this.btnOpenMaterialLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnOpenMaterialLocation.Image")));
			this.btnOpenMaterialLocation.ImagePaddingHorizontal = 8;
			this.btnOpenMaterialLocation.Name = "btnOpenMaterialLocation";
			this.superTooltip1.SetSuperTooltip(this.btnOpenMaterialLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Material Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnOpenMaterialLocation.Text = "buttonItem3";
			this.btnOpenMaterialLocation.Click += new System.EventHandler(this.tsbMaterialLocation_Click);
			// 
			// groupBox9
			// 
			this.groupBox9.Controls.Add(this.txtMaterialFilter);
			this.groupBox9.Controls.Add(this.chkEnableMaterialFilter);
			this.groupBox9.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox9.Location = new System.Drawing.Point(0,260);
			this.groupBox9.Name = "groupBox9";
			this.groupBox9.Size = new System.Drawing.Size(415,46);
			this.groupBox9.TabIndex = 7;
			this.groupBox9.TabStop = false;
			// 
			// txtMaterialFilter
			// 
			this.txtMaterialFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtMaterialFilter.Enabled = false;
			this.txtMaterialFilter.Location = new System.Drawing.Point(71,19);
			this.txtMaterialFilter.Name = "txtMaterialFilter";
			this.txtMaterialFilter.Size = new System.Drawing.Size(338,20);
			this.txtMaterialFilter.TabIndex = 4;
			this.txtMaterialFilter.TextChanged += new System.EventHandler(this.txtMaterialFilter_TextChanged);
			// 
			// chkEnableMaterialFilter
			// 
			this.chkEnableMaterialFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableMaterialFilter.Location = new System.Drawing.Point(7,18);
			this.chkEnableMaterialFilter.Name = "chkEnableMaterialFilter";
			this.chkEnableMaterialFilter.Size = new System.Drawing.Size(64,19);
			this.superTooltip1.SetSuperTooltip(this.chkEnableMaterialFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Material Library with a string.  Only those materials that contain the" +
            " string in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableMaterialFilter.TabIndex = 3;
			this.chkEnableMaterialFilter.Text = "Filter:";
			this.chkEnableMaterialFilter.CheckedChanged += new System.EventHandler(this.chkEnableMaterialFilter_CheckedChanged);
			// 
			// groupBox3
			// 
			this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox3.Controls.Add(this.pgridMaterials);
			this.groupBox3.Location = new System.Drawing.Point(3,3);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(409,209);
			this.groupBox3.TabIndex = 1;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Material Preview";
			// 
			// pgridMaterials
			// 
			this.pgridMaterials.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pgridMaterials.Location = new System.Drawing.Point(3,16);
			this.pgridMaterials.Name = "pgridMaterials";
			this.pgridMaterials.Size = new System.Drawing.Size(403,190);
			this.pgridMaterials.TabIndex = 0;
			this.pgridMaterials.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.pgridMaterials_PropertyValueChanged);
			// 
			// tabMaterials
			// 
			this.tabMaterials.AttachedControl = this.tabMaterialTemplatesPanel;
			this.tabMaterials.Image = ((System.Drawing.Image)(resources.GetObject("tabMaterials.Image")));
			this.tabMaterials.Name = "tabMaterials";
			this.superTooltip1.SetSuperTooltip(this.tabMaterials,new DevComponents.DotNetBar.SuperTooltipInfo("Material Library","",resources.GetString("tabMaterials.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.tabMaterials.Text = "Material Library";
			// 
			// tabTemplatePanel
			// 
			this.tabTemplatePanel.Controls.Add(this.scActorLibrary);
			this.tabTemplatePanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabTemplatePanel.Location = new System.Drawing.Point(0,0);
			this.tabTemplatePanel.Name = "tabTemplatePanel";
			this.tabTemplatePanel.Padding = new System.Windows.Forms.Padding(1);
			this.tabTemplatePanel.Size = new System.Drawing.Size(419,531);
			this.tabTemplatePanel.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabTemplatePanel.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabTemplatePanel.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabTemplatePanel.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabTemplatePanel.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabTemplatePanel.Style.GradientAngle = -90;
			this.tabTemplatePanel.TabIndex = 4;
			this.tabTemplatePanel.TabItem = this.tabActors;
			// 
			// scActorLibrary
			// 
			this.scActorLibrary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scActorLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scActorLibrary.Location = new System.Drawing.Point(1,1);
			this.scActorLibrary.Name = "scActorLibrary";
			this.scActorLibrary.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scActorLibrary.Panel1
			// 
			this.scActorLibrary.Panel1.Controls.Add(this.tvActorLibrary);
			this.scActorLibrary.Panel1.Controls.Add(this.groupBox10);
			this.scActorLibrary.Panel1.Controls.Add(this.bar1);
			// 
			// scActorLibrary.Panel2
			// 
			this.scActorLibrary.Panel2.Controls.Add(this.groupBox2);
			this.scActorLibrary.Size = new System.Drawing.Size(417,529);
			this.scActorLibrary.SplitterDistance = 264;
			this.scActorLibrary.TabIndex = 1;
			// 
			// tvActorLibrary
			// 
			this.tvActorLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvActorLibrary.HideSelection = false;
			this.tvActorLibrary.ImageIndex = 0;
			this.tvActorLibrary.ImageList = this.imglistActors;
			this.tvActorLibrary.Location = new System.Drawing.Point(0,25);
			this.tvActorLibrary.Name = "tvActorLibrary";
			this.tvActorLibrary.SelectedImageIndex = 0;
			this.tvActorLibrary.Size = new System.Drawing.Size(415,191);
			this.tvActorLibrary.TabIndex = 0;
			this.tvActorLibrary.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.tvActorLibrary_MouseDoubleClick);
			this.tvActorLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvActorLibrary_AfterSelect);
			this.tvActorLibrary.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvActorLibrary_MouseDown);
			// 
			// imglistActors
			// 
			this.imglistActors.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistActors.ImageStream")));
			this.imglistActors.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistActors.Images.SetKeyName(0,"Actor");
			this.imglistActors.Images.SetKeyName(1,"Folder");
			// 
			// groupBox10
			// 
			this.groupBox10.Controls.Add(this.txtActorFilter);
			this.groupBox10.Controls.Add(this.chkEnableActorFilter);
			this.groupBox10.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox10.Location = new System.Drawing.Point(0,216);
			this.groupBox10.Name = "groupBox10";
			this.groupBox10.Size = new System.Drawing.Size(415,46);
			this.groupBox10.TabIndex = 8;
			this.groupBox10.TabStop = false;
			// 
			// txtActorFilter
			// 
			this.txtActorFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtActorFilter.Enabled = false;
			this.txtActorFilter.Location = new System.Drawing.Point(71,19);
			this.txtActorFilter.Name = "txtActorFilter";
			this.txtActorFilter.Size = new System.Drawing.Size(338,20);
			this.txtActorFilter.TabIndex = 4;
			this.txtActorFilter.TextChanged += new System.EventHandler(this.txtActorFilter_TextChanged);
			// 
			// chkEnableActorFilter
			// 
			this.chkEnableActorFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableActorFilter.Location = new System.Drawing.Point(6,18);
			this.chkEnableActorFilter.Name = "chkEnableActorFilter";
			this.chkEnableActorFilter.Size = new System.Drawing.Size(59,19);
			this.superTooltip1.SetSuperTooltip(this.chkEnableActorFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Actor Library with a string.  Only those actors that contain the strin" +
            "g in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableActorFilter.TabIndex = 3;
			this.chkEnableActorFilter.Text = "Filter:";
			this.chkEnableActorFilter.CheckedChanged += new System.EventHandler(this.chkEnableActorFilter_CheckedChanged);
			// 
			// bar1
			// 
			this.bar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar1.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllActorFolders,
            this.btnCollapseAllActorFolders,
            this.btnOpenExplorerLocation,
            this.btnOneClickAdd});
			this.bar1.Location = new System.Drawing.Point(0,0);
			this.bar1.Name = "bar1";
			this.bar1.RoundCorners = false;
			this.bar1.Size = new System.Drawing.Size(415,25);
			this.bar1.Stretch = true;
			this.bar1.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar1.TabIndex = 9;
			this.bar1.TabStop = false;
			this.bar1.Text = "bar1";
			// 
			// btnExpandAllActorFolders
			// 
			this.btnExpandAllActorFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllActorFolders.Image")));
			this.btnExpandAllActorFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllActorFolders.Name = "btnExpandAllActorFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllActorFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all actor folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllActorFolders.Text = "buttonItem1";
			this.btnExpandAllActorFolders.Click += new System.EventHandler(this.tsbExpandAllActorFolders_Click);
			// 
			// btnCollapseAllActorFolders
			// 
			this.btnCollapseAllActorFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllActorFolders.Image")));
			this.btnCollapseAllActorFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllActorFolders.Name = "btnCollapseAllActorFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllActorFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all actor folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllActorFolders.Text = "buttonItem2";
			this.btnCollapseAllActorFolders.Click += new System.EventHandler(this.tsbCollaspeAllActorFolders_Click);
			// 
			// btnOpenExplorerLocation
			// 
			this.btnOpenExplorerLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnOpenExplorerLocation.Image")));
			this.btnOpenExplorerLocation.ImagePaddingHorizontal = 8;
			this.btnOpenExplorerLocation.Name = "btnOpenExplorerLocation";
			this.superTooltip1.SetSuperTooltip(this.btnOpenExplorerLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Actor Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnOpenExplorerLocation.Text = "buttonItem3";
			this.btnOpenExplorerLocation.Click += new System.EventHandler(this.tsbOpenExplorerLocation_Click);
			// 
			// btnOneClickAdd
			// 
			this.btnOneClickAdd.AutoCheckOnClick = true;
			this.btnOneClickAdd.Image = ((System.Drawing.Image)(resources.GetObject("btnOneClickAdd.Image")));
			this.btnOneClickAdd.ImagePaddingHorizontal = 8;
			this.btnOneClickAdd.Name = "btnOneClickAdd";
			this.superTooltip1.SetSuperTooltip(this.btnOneClickAdd,new DevComponents.DotNetBar.SuperTooltipInfo("One-Click Actor Creation","","Toggles once click actor creation.  Select an actor in the library, turn on one-c" +
            "lick mode and then click into a viewport to create the actor",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnOneClickAdd.Text = "buttonItem4";
			this.btnOneClickAdd.Click += new System.EventHandler(this.tsbOnceClickAdd_Click);
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.tvActorPreview);
			this.groupBox2.Location = new System.Drawing.Point(3,3);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(409,253);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Actor Preview";
			// 
			// tvActorPreview
			// 
			this.tvActorPreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvActorPreview.HideSelection = false;
			this.tvActorPreview.Location = new System.Drawing.Point(3,16);
			this.tvActorPreview.Name = "tvActorPreview";
			this.tvActorPreview.Size = new System.Drawing.Size(403,234);
			this.tvActorPreview.TabIndex = 0;
			// 
			// tabActors
			// 
			this.tabActors.AttachedControl = this.tabTemplatePanel;
			this.tabActors.Image = ((System.Drawing.Image)(resources.GetObject("tabActors.Image")));
			this.tabActors.Name = "tabActors";
			this.superTooltip1.SetSuperTooltip(this.tabActors,new DevComponents.DotNetBar.SuperTooltipInfo("Actor Libray","",resources.GetString("tabActors.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.tabActors.Text = "Actor Library";
			// 
			// tabControlPanel1
			// 
			this.tabControlPanel1.Controls.Add(this.treeView1);
			this.tabControlPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel1.Location = new System.Drawing.Point(0,0);
			this.tabControlPanel1.Name = "tabControlPanel1";
			this.tabControlPanel1.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel1.Size = new System.Drawing.Size(419,531);
			this.tabControlPanel1.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel1.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel1.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel1.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel1.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanel1.Style.GradientAngle = -90;
			this.tabControlPanel1.TabIndex = 3;
			// 
			// treeView1
			// 
			this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.treeView1.Location = new System.Drawing.Point(1,1);
			this.treeView1.Name = "treeView1";
			this.treeView1.Size = new System.Drawing.Size(417,529);
			this.treeView1.TabIndex = 1;
			// 
			// actorDirWatcher
			// 
			this.actorDirWatcher.EnableRaisingEvents = true;
			this.actorDirWatcher.IncludeSubdirectories = true;
			this.actorDirWatcher.SynchronizingObject = this;
			this.actorDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.actorDirWatcher_Renamed);
			this.actorDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.actorDirWatcher_Deleted);
			this.actorDirWatcher.Created += new System.IO.FileSystemEventHandler(this.actorDirWatcher_Created);
			// 
			// materialDirWatcher
			// 
			this.materialDirWatcher.EnableRaisingEvents = true;
			this.materialDirWatcher.IncludeSubdirectories = true;
			this.materialDirWatcher.SynchronizingObject = this;
			this.materialDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.materialDirWatcher_Renamed);
			this.materialDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.materialDirWatcher_Deleted);
			this.materialDirWatcher.Created += new System.IO.FileSystemEventHandler(this.materialDirWatcher_Created);
			// 
			// audioDirWatcher
			// 
			this.audioDirWatcher.EnableRaisingEvents = true;
			this.audioDirWatcher.IncludeSubdirectories = true;
			this.audioDirWatcher.SynchronizingObject = this;
			this.audioDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.audioDirWatcher_Renamed);
			this.audioDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.audioDirWatcher_Deleted);
			this.audioDirWatcher.Created += new System.IO.FileSystemEventHandler(this.audioDirWatcher_Created);
			// 
			// controllerDirWatcher
			// 
			this.controllerDirWatcher.EnableRaisingEvents = true;
			this.controllerDirWatcher.IncludeSubdirectories = true;
			this.controllerDirWatcher.SynchronizingObject = this;
			this.controllerDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.controllerDirWatcher_Renamed);
			this.controllerDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.controllerDirWatcher_Deleted);
			this.controllerDirWatcher.Created += new System.IO.FileSystemEventHandler(this.controllerDirWatcher_Created);
			// 
			// textureDirWatcher
			// 
			this.textureDirWatcher.EnableRaisingEvents = true;
			this.textureDirWatcher.IncludeSubdirectories = true;
			this.textureDirWatcher.SynchronizingObject = this;
			this.textureDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.textureDirWatcher_Renamed);
			this.textureDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.textureDirWatcher_Deleted);
			this.textureDirWatcher.Created += new System.IO.FileSystemEventHandler(this.textureDirWatcher_Created);
			// 
			// bitmapLoadWorker
			// 
			this.bitmapLoadWorker.WorkerReportsProgress = true;
			this.bitmapLoadWorker.WorkerSupportsCancellation = true;
			this.bitmapLoadWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bitmapLoadWorker_DoWork);
			this.bitmapLoadWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bitmapLoadWorker_RunWorkerCompleted);
			// 
			// actorPreviewWorker
			// 
			this.actorPreviewWorker.WorkerReportsProgress = true;
			this.actorPreviewWorker.WorkerSupportsCancellation = true;
			this.actorPreviewWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.actorPreviewWorker_DoWork);
			this.actorPreviewWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.actorPreviewWorker_RunWorkerCompleted);
			// 
			// materialPreviewWorker
			// 
			this.materialPreviewWorker.WorkerReportsProgress = true;
			this.materialPreviewWorker.WorkerSupportsCancellation = true;
			this.materialPreviewWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.materialPreviewWorker_DoWork);
			this.materialPreviewWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.materialPreviewWorker_RunWorkerCompleted);
			// 
			// controllerPreviewWorker
			// 
			this.controllerPreviewWorker.WorkerReportsProgress = true;
			this.controllerPreviewWorker.WorkerSupportsCancellation = true;
			this.controllerPreviewWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.controllerPreviewWorker_DoWork);
			this.controllerPreviewWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.controllerPreviewWorker_RunWorkerCompleted);
			// 
			// tsbExpandAllHierarchyNodes
			// 
			this.tsbExpandAllHierarchyNodes.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tsbExpandAllHierarchyNodes.Image = ((System.Drawing.Image)(resources.GetObject("tsbExpandAllHierarchyNodes.Image")));
			this.tsbExpandAllHierarchyNodes.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbExpandAllHierarchyNodes.Name = "tsbExpandAllHierarchyNodes";
			this.tsbExpandAllHierarchyNodes.Size = new System.Drawing.Size(23,22);
			this.tsbExpandAllHierarchyNodes.Text = "toolStripButton1";
			this.tsbExpandAllHierarchyNodes.ToolTipText = "Expand all hierarchy nodes";
			// 
			// tsbCollapseAllHierarchyNodes
			// 
			this.tsbCollapseAllHierarchyNodes.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tsbCollapseAllHierarchyNodes.Image = ((System.Drawing.Image)(resources.GetObject("tsbCollapseAllHierarchyNodes.Image")));
			this.tsbCollapseAllHierarchyNodes.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbCollapseAllHierarchyNodes.Name = "tsbCollapseAllHierarchyNodes";
			this.tsbCollapseAllHierarchyNodes.Size = new System.Drawing.Size(23,22);
			this.tsbCollapseAllHierarchyNodes.Text = "toolStripButton2";
			this.tsbCollapseAllHierarchyNodes.ToolTipText = "Collapse all hierarchy nodes";
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6,25);
			// 
			// tsbHierarchyView
			// 
			this.tsbHierarchyView.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.tsbHierarchyView.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tsbHierarchyView.Image = ((System.Drawing.Image)(resources.GetObject("tsbHierarchyView.Image")));
			this.tsbHierarchyView.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbHierarchyView.Name = "tsbHierarchyView";
			this.tsbHierarchyView.Size = new System.Drawing.Size(23,22);
			this.tsbHierarchyView.Text = "About Hierarchy View";
			// 
			// toolStripButton1
			// 
			this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
			this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButton1.Name = "toolStripButton1";
			this.toolStripButton1.Size = new System.Drawing.Size(23,22);
			this.toolStripButton1.Text = "toolStripButton1";
			this.toolStripButton1.ToolTipText = "Expand all folders";
			// 
			// toolStripButton2
			// 
			this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
			this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButton2.Name = "toolStripButton2";
			this.toolStripButton2.Size = new System.Drawing.Size(23,22);
			this.toolStripButton2.Text = "toolStripButton2";
			this.toolStripButton2.ToolTipText = "Collapse all folders";
			// 
			// toolStripSeparator5
			// 
			this.toolStripSeparator5.Name = "toolStripSeparator5";
			this.toolStripSeparator5.Size = new System.Drawing.Size(6,25);
			// 
			// toolStripButton5
			// 
			this.toolStripButton5.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.toolStripButton5.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButton5.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton5.Image")));
			this.toolStripButton5.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButton5.Name = "toolStripButton5";
			this.toolStripButton5.Size = new System.Drawing.Size(23,22);
			this.toolStripButton5.Text = "About Controller Library";
			// 
			// superTooltip1
			// 
			this.superTooltip1.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			this.superTooltip1.MinimumTooltipSize = new System.Drawing.Size(0,0);
			// 
			// scriptDirWatcher
			// 
			this.scriptDirWatcher.EnableRaisingEvents = true;
			this.scriptDirWatcher.IncludeSubdirectories = true;
			this.scriptDirWatcher.SynchronizingObject = this;
			this.scriptDirWatcher.Renamed += new System.IO.RenamedEventHandler(this.scriptDirWatcher_Renamed);
			this.scriptDirWatcher.Deleted += new System.IO.FileSystemEventHandler(this.scriptDirWatcher_Deleted);
			this.scriptDirWatcher.Created += new System.IO.FileSystemEventHandler(this.scriptDirWatcher_Created);
			// 
			// tabScriptLibrary
			// 
			this.tabScriptLibrary.AttachedControl = this.tabControlPanel4;
			this.tabScriptLibrary.Image = ((System.Drawing.Image)(resources.GetObject("tabScriptLibrary.Image")));
			this.tabScriptLibrary.Name = "tabScriptLibrary";
			this.tabScriptLibrary.Text = "Script Library";
			// 
			// tabControlPanel4
			// 
			this.tabControlPanel4.Controls.Add(this.tabControlPanel5);
			this.tabControlPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel4.Location = new System.Drawing.Point(0,0);
			this.tabControlPanel4.Name = "tabControlPanel4";
			this.tabControlPanel4.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel4.Size = new System.Drawing.Size(419,531);
			this.tabControlPanel4.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel4.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel4.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel4.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel4.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanel4.Style.GradientAngle = -90;
			this.tabControlPanel4.TabIndex = 9;
			this.tabControlPanel4.TabItem = this.tabScriptLibrary;
			// 
			// tabControlPanel5
			// 
			this.tabControlPanel5.Controls.Add(this.splitContainer1);
			this.tabControlPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel5.Location = new System.Drawing.Point(1,1);
			this.tabControlPanel5.Name = "tabControlPanel5";
			this.tabControlPanel5.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel5.Size = new System.Drawing.Size(417,529);
			this.tabControlPanel5.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel5.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel5.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel5.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel5.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanel5.Style.GradientAngle = -90;
			this.tabControlPanel5.TabIndex = 7;
			this.tabControlPanel5.TabItem = this.tabControllerLibrary;
			// 
			// splitContainer1
			// 
			this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(1,1);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.tvScriptLibrary);
			this.splitContainer1.Panel1.Controls.Add(this.bar6);
			this.splitContainer1.Panel1.Controls.Add(this.groupBox11);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.groupBox12);
			this.splitContainer1.Size = new System.Drawing.Size(415,527);
			this.splitContainer1.SplitterDistance = 300;
			this.splitContainer1.TabIndex = 0;
			// 
			// tvScriptLibrary
			// 
			this.tvScriptLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvScriptLibrary.HideSelection = false;
			this.tvScriptLibrary.ImageIndex = 0;
			this.tvScriptLibrary.ImageList = this.imglistScripts;
			this.tvScriptLibrary.Location = new System.Drawing.Point(0,25);
			this.tvScriptLibrary.Name = "tvScriptLibrary";
			this.tvScriptLibrary.SelectedImageIndex = 0;
			this.tvScriptLibrary.Size = new System.Drawing.Size(413,227);
			this.tvScriptLibrary.TabIndex = 2;
			this.tvScriptLibrary.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.tvScriptLibrary_MouseDoubleClick);
			this.tvScriptLibrary.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvScriptLibrary_AfterSelect);
			// 
			// bar6
			// 
			this.bar6.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar6.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnExpandAllScriptFolders,
            this.btnCollapseAllScriptFolders,
            this.btnOpenScriptLocation});
			this.bar6.Location = new System.Drawing.Point(0,0);
			this.bar6.Name = "bar6";
			this.bar6.RoundCorners = false;
			this.bar6.Size = new System.Drawing.Size(413,25);
			this.bar6.Stretch = true;
			this.bar6.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar6.TabIndex = 7;
			this.bar6.TabStop = false;
			this.bar6.Text = "bar6";
			// 
			// btnExpandAllScriptFolders
			// 
			this.btnExpandAllScriptFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnExpandAllScriptFolders.Image")));
			this.btnExpandAllScriptFolders.ImagePaddingHorizontal = 8;
			this.btnExpandAllScriptFolders.Name = "btnExpandAllScriptFolders";
			this.superTooltip1.SetSuperTooltip(this.btnExpandAllScriptFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Expand all script folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnExpandAllScriptFolders.Text = "buttonItem1";
			this.btnExpandAllScriptFolders.Click += new System.EventHandler(this.btnExpandAllScriptFolders_Click);
			// 
			// btnCollapseAllScriptFolders
			// 
			this.btnCollapseAllScriptFolders.Image = ((System.Drawing.Image)(resources.GetObject("btnCollapseAllScriptFolders.Image")));
			this.btnCollapseAllScriptFolders.ImagePaddingHorizontal = 8;
			this.btnCollapseAllScriptFolders.Name = "btnCollapseAllScriptFolders";
			this.superTooltip1.SetSuperTooltip(this.btnCollapseAllScriptFolders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Collapse all script folders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCollapseAllScriptFolders.Text = "buttonItem2";
			this.btnCollapseAllScriptFolders.Click += new System.EventHandler(this.btnCollapseAllScriptFolders_Click);
			// 
			// btnOpenScriptLocation
			// 
			this.btnOpenScriptLocation.Image = ((System.Drawing.Image)(resources.GetObject("btnOpenScriptLocation.Image")));
			this.btnOpenScriptLocation.ImagePaddingHorizontal = 8;
			this.btnOpenScriptLocation.Name = "btnOpenScriptLocation";
			this.superTooltip1.SetSuperTooltip(this.btnOpenScriptLocation,new DevComponents.DotNetBar.SuperTooltipInfo("","","Open Script Library Location in Explorer",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnOpenScriptLocation.Text = "buttonItem3";
			this.btnOpenScriptLocation.Click += new System.EventHandler(this.btnOpenScriptLocation_Click);
			// 
			// groupBox11
			// 
			this.groupBox11.Controls.Add(this.txtScriptFilter);
			this.groupBox11.Controls.Add(this.chkEnableScriptFilter);
			this.groupBox11.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox11.Location = new System.Drawing.Point(0,252);
			this.groupBox11.Name = "groupBox11";
			this.groupBox11.Size = new System.Drawing.Size(413,46);
			this.groupBox11.TabIndex = 6;
			this.groupBox11.TabStop = false;
			// 
			// txtScriptFilter
			// 
			this.txtScriptFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtScriptFilter.Enabled = false;
			this.txtScriptFilter.Location = new System.Drawing.Point(70,19);
			this.txtScriptFilter.Name = "txtScriptFilter";
			this.txtScriptFilter.Size = new System.Drawing.Size(337,20);
			this.txtScriptFilter.TabIndex = 4;
			this.txtScriptFilter.TextChanged += new System.EventHandler(this.txtScriptFilter_TextChanged);
			// 
			// chkEnableScriptFilter
			// 
			this.chkEnableScriptFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.chkEnableScriptFilter.Location = new System.Drawing.Point(6,18);
			this.chkEnableScriptFilter.Name = "chkEnableScriptFilter";
			this.chkEnableScriptFilter.Size = new System.Drawing.Size(67,19);
			this.superTooltip1.SetSuperTooltip(this.chkEnableScriptFilter,new DevComponents.DotNetBar.SuperTooltipInfo("","","Filter the Controller Library with a string.  Only those controllers that contain" +
            " the string in their name will be visible in the library.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.chkEnableScriptFilter.TabIndex = 3;
			this.chkEnableScriptFilter.Text = "Filter:";
			this.chkEnableScriptFilter.CheckedChanged += new System.EventHandler(this.chkEnableScriptFilter_CheckedChanged);
			// 
			// groupBox12
			// 
			this.groupBox12.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox12.Controls.Add(this.txtScriptPreview);
			this.groupBox12.Location = new System.Drawing.Point(3,3);
			this.groupBox12.Name = "groupBox12";
			this.groupBox12.Size = new System.Drawing.Size(407,215);
			this.groupBox12.TabIndex = 2;
			this.groupBox12.TabStop = false;
			this.groupBox12.Text = "Script Preview";
			// 
			// imglistScripts
			// 
			this.imglistScripts.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistScripts.ImageStream")));
			this.imglistScripts.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistScripts.Images.SetKeyName(0,"Script");
			this.imglistScripts.Images.SetKeyName(1,"Folder");
			// 
			// txtScriptPreview
			// 
			this.txtScriptPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtScriptPreview.IsReadOnly = false;
			this.txtScriptPreview.Location = new System.Drawing.Point(6,19);
			this.txtScriptPreview.Name = "txtScriptPreview";
			this.txtScriptPreview.ShowLineNumbers = false;
			this.txtScriptPreview.Size = new System.Drawing.Size(395,190);
			this.txtScriptPreview.TabIndex = 0;
			// 
			// texturePreview
			// 
			this.texturePreview.BackgroundColor = System.Drawing.SystemColors.Control;
			this.texturePreview.BackgroundColorSelected = System.Drawing.SystemColors.Highlight;
			this.texturePreview.CacheImages = false;
			this.texturePreview.CanvasColor = System.Drawing.SystemColors.ControlDark;
			this.texturePreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.texturePreview.ForegroundColor = System.Drawing.SystemColors.ControlText;
			this.texturePreview.ForegroundColorSelected = System.Drawing.SystemColors.HighlightText;
			this.texturePreview.Location = new System.Drawing.Point(3,16);
			this.texturePreview.Name = "texturePreview";
			this.texturePreview.Selection = TextureBrowserLibrary.TextureBrowser.SelectionMode.Single;
			this.texturePreview.Size = new System.Drawing.Size(403,177);
			this.texturePreview.TabIndex = 0;
			// 
			// LibraryExplorer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tabsTemplateLibrary);
			this.Name = "LibraryExplorer";
			this.Size = new System.Drawing.Size(419,578);
			((System.ComponentModel.ISupportInitialize)(this.tabsTemplateLibrary)).EndInit();
			this.tabsTemplateLibrary.ResumeLayout(false);
			this.tabAudioLibraryPanel.ResumeLayout(false);
			this.scAudioLibrary.Panel1.ResumeLayout(false);
			this.scAudioLibrary.Panel2.ResumeLayout(false);
			this.scAudioLibrary.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar5)).EndInit();
			this.groupBox6.ResumeLayout(false);
			this.groupBox6.PerformLayout();
			this.groupBox5.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.axWindowsMediaPlayer)).EndInit();
			this.tabControlPanel3.ResumeLayout(false);
			this.scTextureLibrary.Panel1.ResumeLayout(false);
			this.scTextureLibrary.Panel2.ResumeLayout(false);
			this.scTextureLibrary.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar4)).EndInit();
			this.groupBox7.ResumeLayout(false);
			this.groupBox7.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.tabControlPanel2.ResumeLayout(false);
			this.scControllerLibrary.Panel1.ResumeLayout(false);
			this.scControllerLibrary.Panel2.ResumeLayout(false);
			this.scControllerLibrary.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar3)).EndInit();
			this.groupBox8.ResumeLayout(false);
			this.groupBox8.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.tabMaterialTemplatesPanel.ResumeLayout(false);
			this.scMaterialLibrary.Panel1.ResumeLayout(false);
			this.scMaterialLibrary.Panel2.ResumeLayout(false);
			this.scMaterialLibrary.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar2)).EndInit();
			this.groupBox9.ResumeLayout(false);
			this.groupBox9.PerformLayout();
			this.groupBox3.ResumeLayout(false);
			this.tabTemplatePanel.ResumeLayout(false);
			this.scActorLibrary.Panel1.ResumeLayout(false);
			this.scActorLibrary.Panel2.ResumeLayout(false);
			this.scActorLibrary.ResumeLayout(false);
			this.groupBox10.ResumeLayout(false);
			this.groupBox10.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).EndInit();
			this.groupBox2.ResumeLayout(false);
			this.tabControlPanel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.actorDirWatcher)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.materialDirWatcher)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.audioDirWatcher)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.controllerDirWatcher)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.textureDirWatcher)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.scriptDirWatcher)).EndInit();
			this.tabControlPanel4.ResumeLayout(false);
			this.tabControlPanel5.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar6)).EndInit();
			this.groupBox11.ResumeLayout(false);
			this.groupBox11.PerformLayout();
			this.groupBox12.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private DevComponents.DotNetBar.TabControl tabsTemplateLibrary;
		private DevComponents.DotNetBar.TabControlPanel tabTemplatePanel;
		private DevComponents.DotNetBar.TabItem tabActors;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel1;
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.TreeView tvActorLibrary;
		private System.Windows.Forms.ImageList imglistActors;
		private System.IO.FileSystemWatcher actorDirWatcher;
		private DevComponents.DotNetBar.TabControlPanel tabMaterialTemplatesPanel;
		private System.Windows.Forms.TreeView tvMaterialLibrary;
		private DevComponents.DotNetBar.TabItem tabMaterials;
		private System.Windows.Forms.ImageList imglistMaterials;
		private System.IO.FileSystemWatcher materialDirWatcher;
		private DevComponents.DotNetBar.TabControlPanel tabAudioLibraryPanel;
		private DevComponents.DotNetBar.TabItem tabAudioLibrary;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel3;
		private DevComponents.DotNetBar.TabItem tabTextureLibrary;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel2;
		private DevComponents.DotNetBar.TabItem tabControllerLibrary;
		private System.Windows.Forms.SplitContainer scMaterialLibrary;
		private System.Windows.Forms.PropertyGrid pgridMaterials;
		private System.Windows.Forms.SplitContainer scAudioLibrary;
		private System.Windows.Forms.TreeView tvAudioLibrary;
		private System.Windows.Forms.ImageList imglistAudio;
		private System.IO.FileSystemWatcher audioDirWatcher;
		private System.Windows.Forms.SplitContainer scActorLibrary;
		private System.Windows.Forms.TreeView tvActorPreview;
		private System.Windows.Forms.SplitContainer scControllerLibrary;
		private System.Windows.Forms.TreeView tvControllerLibrary;
		private System.Windows.Forms.PropertyGrid pgridControllers;
		private System.IO.FileSystemWatcher controllerDirWatcher;
		private System.Windows.Forms.ImageList imglistControllers;
		private System.Windows.Forms.TreeView tvTextureLibrary;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox5;
		private AxWMPLib.AxWindowsMediaPlayer axWindowsMediaPlayer;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.IO.FileSystemWatcher textureDirWatcher;
		private System.Windows.Forms.ImageList imglistTextures;
		private TextureBrowserLibrary.TextureBrowser texturePreview;
		private System.ComponentModel.BackgroundWorker bitmapLoadWorker;
		private System.Windows.Forms.SplitContainer scTextureLibrary;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableAudioFilter;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.TextBox txtAudioFilter;
		private System.Windows.Forms.GroupBox groupBox7;
		private System.Windows.Forms.TextBox txtTextureFilter;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableTextureFilter;
		private System.Windows.Forms.GroupBox groupBox10;
		private System.Windows.Forms.TextBox txtActorFilter;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableActorFilter;
		private System.Windows.Forms.GroupBox groupBox9;
		private System.Windows.Forms.TextBox txtMaterialFilter;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableMaterialFilter;
		private System.Windows.Forms.GroupBox groupBox8;
		private System.Windows.Forms.TextBox txtControllerFilter;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableControllerFilter;
		private System.ComponentModel.BackgroundWorker actorPreviewWorker;
		private System.ComponentModel.BackgroundWorker materialPreviewWorker;
		private System.ComponentModel.BackgroundWorker controllerPreviewWorker;
		private System.Windows.Forms.ToolStripButton tsbExpandAllHierarchyNodes;
		private System.Windows.Forms.ToolStripButton tsbCollapseAllHierarchyNodes;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tsbHierarchyView;
		private System.Windows.Forms.ToolStripButton toolStripButton1;
		private System.Windows.Forms.ToolStripButton toolStripButton2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
		private System.Windows.Forms.ToolStripButton toolStripButton5;
		private DevComponents.DotNetBar.Bar bar1;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllActorFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllActorFolders;
		private DevComponents.DotNetBar.ButtonItem btnOpenExplorerLocation;
		private DevComponents.DotNetBar.ButtonItem btnOneClickAdd;
		private DevComponents.DotNetBar.Bar bar2;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllMaterialFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllMaterialFolders;
		private DevComponents.DotNetBar.ButtonItem btnOpenMaterialLocation;
		private DevComponents.DotNetBar.Bar bar3;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllControllerFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllControllerFolders;
		private DevComponents.DotNetBar.ButtonItem btnOpenControllerLocation;
		private DevComponents.DotNetBar.Bar bar4;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllTextureFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllTextureFolders;
		private DevComponents.DotNetBar.ButtonItem btnTextureLocation;
		private DevComponents.DotNetBar.ButtonItem btnDelteCompressed;
		private DevComponents.DotNetBar.ButtonItem btnDeleteAllCompressed;
		private DevComponents.DotNetBar.Bar bar5;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllAudioFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllAudioFolders;
		private DevComponents.DotNetBar.ButtonItem btnAudioLocation;
		private DevComponents.DotNetBar.SuperTooltip superTooltip1;
		private System.IO.FileSystemWatcher scriptDirWatcher;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel4;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel5;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.TreeView tvScriptLibrary;
		private DevComponents.DotNetBar.Bar bar6;
		private DevComponents.DotNetBar.ButtonItem btnExpandAllScriptFolders;
		private DevComponents.DotNetBar.ButtonItem btnCollapseAllScriptFolders;
		private DevComponents.DotNetBar.ButtonItem btnOpenScriptLocation;
		private System.Windows.Forms.GroupBox groupBox11;
		private System.Windows.Forms.TextBox txtScriptFilter;
		private DevComponents.DotNetBar.Controls.CheckBoxX chkEnableScriptFilter;
		private System.Windows.Forms.GroupBox groupBox12;
		private DevComponents.DotNetBar.TabItem tabScriptLibrary;
		private System.Windows.Forms.ImageList imglistScripts;
		private ICSharpCode.TextEditor.TextEditorControl txtScriptPreview;
	}
}
