namespace XVPDesigner.UserControls
{
	partial class Output
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Output));
			this.rtxtOutput = new System.Windows.Forms.RichTextBox();
			this.dlgSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.tabsOutput = new DevComponents.DotNetBar.TabControl();
			this.tabControlPanelMessageLog = new DevComponents.DotNetBar.TabControlPanel();
			this.bar1 = new DevComponents.DotNetBar.Bar();
			this.btnSaveMessageLog = new DevComponents.DotNetBar.ButtonItem();
			this.btnClearMessageLog = new DevComponents.DotNetBar.ButtonItem();
			this.tabMessageLog = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanelTargetConsole = new DevComponents.DotNetBar.TabControlPanel();
			this.rtxtConsole = new System.Windows.Forms.RichTextBox();
			this.bar2 = new DevComponents.DotNetBar.Bar();
			this.btnSaveConsole = new DevComponents.DotNetBar.ButtonItem();
			this.btnClearConsole = new DevComponents.DotNetBar.ButtonItem();
			this.tabTargetConsole = new DevComponents.DotNetBar.TabItem(this.components);
			this.timerMessageQueue = new System.Windows.Forms.Timer(this.components);
			this.superTooltip1 = new DevComponents.DotNetBar.SuperTooltip();
			((System.ComponentModel.ISupportInitialize)(this.tabsOutput)).BeginInit();
			this.tabsOutput.SuspendLayout();
			this.tabControlPanelMessageLog.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).BeginInit();
			this.tabControlPanelTargetConsole.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bar2)).BeginInit();
			this.SuspendLayout();
			// 
			// rtxtOutput
			// 
			this.rtxtOutput.BackColor = System.Drawing.Color.White;
			this.rtxtOutput.Dock = System.Windows.Forms.DockStyle.Fill;
			this.rtxtOutput.Location = new System.Drawing.Point(1,26);
			this.rtxtOutput.Name = "rtxtOutput";
			this.rtxtOutput.ReadOnly = true;
			this.rtxtOutput.Size = new System.Drawing.Size(528,352);
			this.rtxtOutput.TabIndex = 2;
			this.rtxtOutput.Text = "";
			this.rtxtOutput.TextChanged += new System.EventHandler(this.rtxtOutput_TextChanged);
			// 
			// tabsOutput
			// 
			this.tabsOutput.CanReorderTabs = true;
			this.tabsOutput.Controls.Add(this.tabControlPanelTargetConsole);
			this.tabsOutput.Controls.Add(this.tabControlPanelMessageLog);
			this.tabsOutput.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabsOutput.Location = new System.Drawing.Point(0,0);
			this.tabsOutput.Name = "tabsOutput";
			this.tabsOutput.SelectedTabFont = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Bold);
			this.tabsOutput.SelectedTabIndex = 0;
			this.tabsOutput.Size = new System.Drawing.Size(530,404);
			this.tabsOutput.Style = DevComponents.DotNetBar.eTabStripStyle.Office2007Document;
			this.tabsOutput.TabAlignment = DevComponents.DotNetBar.eTabStripAlignment.Bottom;
			this.tabsOutput.TabIndex = 8;
			this.tabsOutput.TabLayoutType = DevComponents.DotNetBar.eTabLayoutType.MultilineNoNavigationBox;
			this.tabsOutput.Tabs.Add(this.tabMessageLog);
			this.tabsOutput.Tabs.Add(this.tabTargetConsole);
			this.tabsOutput.Text = "tabControl1";
			// 
			// tabControlPanelMessageLog
			// 
			this.tabControlPanelMessageLog.Controls.Add(this.rtxtOutput);
			this.tabControlPanelMessageLog.Controls.Add(this.bar1);
			this.tabControlPanelMessageLog.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanelMessageLog.Location = new System.Drawing.Point(0,0);
			this.tabControlPanelMessageLog.Name = "tabControlPanelMessageLog";
			this.tabControlPanelMessageLog.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanelMessageLog.Size = new System.Drawing.Size(530,379);
			this.tabControlPanelMessageLog.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanelMessageLog.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanelMessageLog.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanelMessageLog.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanelMessageLog.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanelMessageLog.Style.GradientAngle = -90;
			this.tabControlPanelMessageLog.TabIndex = 1;
			this.tabControlPanelMessageLog.TabItem = this.tabMessageLog;
			// 
			// bar1
			// 
			this.bar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar1.DockSide = DevComponents.DotNetBar.eDockSide.Document;
			this.bar1.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnSaveMessageLog,
            this.btnClearMessageLog});
			this.bar1.Location = new System.Drawing.Point(1,1);
			this.bar1.Name = "bar1";
			this.bar1.RoundCorners = false;
			this.bar1.Size = new System.Drawing.Size(528,25);
			this.bar1.Stretch = true;
			this.bar1.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar1.TabIndex = 9;
			this.bar1.TabStop = false;
			this.bar1.Text = "bar1";
			// 
			// btnSaveMessageLog
			// 
			this.btnSaveMessageLog.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveMessageLog.Image")));
			this.btnSaveMessageLog.ImagePaddingHorizontal = 8;
			this.btnSaveMessageLog.Name = "btnSaveMessageLog";
			this.superTooltip1.SetSuperTooltip(this.btnSaveMessageLog,new DevComponents.DotNetBar.SuperTooltipInfo("","","Save message log",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnSaveMessageLog.Text = "buttonItem1";
			this.btnSaveMessageLog.Click += new System.EventHandler(this.tsbSaveMessageLog_Click);
			// 
			// btnClearMessageLog
			// 
			this.btnClearMessageLog.Image = ((System.Drawing.Image)(resources.GetObject("btnClearMessageLog.Image")));
			this.btnClearMessageLog.ImagePaddingHorizontal = 8;
			this.btnClearMessageLog.Name = "btnClearMessageLog";
			this.superTooltip1.SetSuperTooltip(this.btnClearMessageLog,new DevComponents.DotNetBar.SuperTooltipInfo("","","Clear message log",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnClearMessageLog.Text = "buttonItem2";
			this.btnClearMessageLog.Click += new System.EventHandler(this.tsbClearMessageLog_Click);
			// 
			// tabMessageLog
			// 
			this.tabMessageLog.AttachedControl = this.tabControlPanelMessageLog;
			this.tabMessageLog.Image = ((System.Drawing.Image)(resources.GetObject("tabMessageLog.Image")));
			this.tabMessageLog.Name = "tabMessageLog";
			this.tabMessageLog.Text = "Message Log";
			// 
			// tabControlPanelTargetConsole
			// 
			this.tabControlPanelTargetConsole.Controls.Add(this.rtxtConsole);
			this.tabControlPanelTargetConsole.Controls.Add(this.bar2);
			this.tabControlPanelTargetConsole.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanelTargetConsole.Location = new System.Drawing.Point(0,0);
			this.tabControlPanelTargetConsole.Name = "tabControlPanelTargetConsole";
			this.tabControlPanelTargetConsole.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanelTargetConsole.Size = new System.Drawing.Size(530,379);
			this.tabControlPanelTargetConsole.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanelTargetConsole.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanelTargetConsole.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanelTargetConsole.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanelTargetConsole.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabControlPanelTargetConsole.Style.GradientAngle = -90;
			this.tabControlPanelTargetConsole.TabIndex = 2;
			this.tabControlPanelTargetConsole.TabItem = this.tabTargetConsole;
			// 
			// rtxtConsole
			// 
			this.rtxtConsole.Dock = System.Windows.Forms.DockStyle.Fill;
			this.rtxtConsole.Location = new System.Drawing.Point(1,26);
			this.rtxtConsole.Name = "rtxtConsole";
			this.rtxtConsole.Size = new System.Drawing.Size(528,352);
			this.rtxtConsole.TabIndex = 2;
			this.rtxtConsole.Text = "";
			this.rtxtConsole.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.rtxtConsole_KeyPress);
			// 
			// bar2
			// 
			this.bar2.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar2.DockSide = DevComponents.DotNetBar.eDockSide.Document;
			this.bar2.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnSaveConsole,
            this.btnClearConsole});
			this.bar2.Location = new System.Drawing.Point(1,1);
			this.bar2.Name = "bar2";
			this.bar2.RoundCorners = false;
			this.bar2.Size = new System.Drawing.Size(528,25);
			this.bar2.Stretch = true;
			this.bar2.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar2.TabIndex = 3;
			this.bar2.TabStop = false;
			this.bar2.Text = "bar2";
			// 
			// btnSaveConsole
			// 
			this.btnSaveConsole.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveConsole.Image")));
			this.btnSaveConsole.ImagePaddingHorizontal = 8;
			this.btnSaveConsole.Name = "btnSaveConsole";
			this.superTooltip1.SetSuperTooltip(this.btnSaveConsole,new DevComponents.DotNetBar.SuperTooltipInfo("","","Save console log",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnSaveConsole.Text = "buttonItem1";
			this.btnSaveConsole.Click += new System.EventHandler(this.tsbSaveConsole_Click);
			// 
			// btnClearConsole
			// 
			this.btnClearConsole.Image = ((System.Drawing.Image)(resources.GetObject("btnClearConsole.Image")));
			this.btnClearConsole.ImagePaddingHorizontal = 8;
			this.btnClearConsole.Name = "btnClearConsole";
			this.superTooltip1.SetSuperTooltip(this.btnClearConsole,new DevComponents.DotNetBar.SuperTooltipInfo("","","Clear console log",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnClearConsole.Text = "buttonItem2";
			this.btnClearConsole.Click += new System.EventHandler(this.tsbClearConsole_Click);
			// 
			// tabTargetConsole
			// 
			this.tabTargetConsole.AttachedControl = this.tabControlPanelTargetConsole;
			this.tabTargetConsole.Image = ((System.Drawing.Image)(resources.GetObject("tabTargetConsole.Image")));
			this.tabTargetConsole.Name = "tabTargetConsole";
			this.tabTargetConsole.Text = "Target Console";
			// 
			// timerMessageQueue
			// 
			this.timerMessageQueue.Enabled = true;
			this.timerMessageQueue.Interval = 33;
			this.timerMessageQueue.Tick += new System.EventHandler(this.timerMessageQueue_Tick);
			// 
			// superTooltip1
			// 
			this.superTooltip1.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			this.superTooltip1.MinimumTooltipSize = new System.Drawing.Size(0,0);
			// 
			// Output
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tabsOutput);
			this.Name = "Output";
			this.Size = new System.Drawing.Size(530,404);
			((System.ComponentModel.ISupportInitialize)(this.tabsOutput)).EndInit();
			this.tabsOutput.ResumeLayout(false);
			this.tabControlPanelMessageLog.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar1)).EndInit();
			this.tabControlPanelTargetConsole.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bar2)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.RichTextBox rtxtOutput;
		private System.Windows.Forms.SaveFileDialog dlgSaveFileDialog;
		private DevComponents.DotNetBar.TabControl tabsOutput;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanelMessageLog;
		private DevComponents.DotNetBar.TabItem tabMessageLog;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanelTargetConsole;
		private DevComponents.DotNetBar.TabItem tabTargetConsole;
		private System.Windows.Forms.RichTextBox rtxtConsole;
		private System.Windows.Forms.Timer timerMessageQueue;
		private DevComponents.DotNetBar.Bar bar1;
		private DevComponents.DotNetBar.ButtonItem btnSaveMessageLog;
		private DevComponents.DotNetBar.ButtonItem btnClearMessageLog;
		private DevComponents.DotNetBar.Bar bar2;
		private DevComponents.DotNetBar.ButtonItem btnSaveConsole;
		private DevComponents.DotNetBar.ButtonItem btnClearConsole;
		private DevComponents.DotNetBar.SuperTooltip superTooltip1;
	}
}
