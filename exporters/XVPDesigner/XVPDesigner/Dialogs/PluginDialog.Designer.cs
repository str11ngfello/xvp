namespace XVPDesigner.Dialogs
{
	partial class PluginDialog
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PluginDialog));
			this.tcPlugins = new DevComponents.DotNetBar.TabControl();
			this.tabControlPanel3 = new DevComponents.DotNetBar.TabControlPanel();
			this.lvCustomDataPlugins = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader13 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader14 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader15 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader16 = new System.Windows.Forms.ColumnHeader();
			this.tiCustomData = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel4 = new DevComponents.DotNetBar.TabControlPanel();
			this.lvPostEffectPlugins = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader9 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader10 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader11 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader12 = new System.Windows.Forms.ColumnHeader();
			this.tiPostEffects = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel2 = new DevComponents.DotNetBar.TabControlPanel();
			this.lvShaderPlugins = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader8 = new System.Windows.Forms.ColumnHeader();
			this.tiShaders = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel5 = new DevComponents.DotNetBar.TabControlPanel();
			this.lvControllerPlugins = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.tiControllers = new DevComponents.DotNetBar.TabItem(this.components);
			this.tabControlPanel1 = new DevComponents.DotNetBar.TabControlPanel();
			this.lvSceneNodePlugins = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.colName = new System.Windows.Forms.ColumnHeader();
			this.colVersion = new System.Windows.Forms.ColumnHeader();
			this.colDescription = new System.Windows.Forms.ColumnHeader();
			this.colLocation = new System.Windows.Forms.ColumnHeader();
			this.tiSceneNodes = new DevComponents.DotNetBar.TabItem(this.components);
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			((System.ComponentModel.ISupportInitialize)(this.tcPlugins)).BeginInit();
			this.tcPlugins.SuspendLayout();
			this.tabControlPanel3.SuspendLayout();
			this.tabControlPanel4.SuspendLayout();
			this.tabControlPanel2.SuspendLayout();
			this.tabControlPanel5.SuspendLayout();
			this.tabControlPanel1.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// tcPlugins
			// 
			this.tcPlugins.AutoHideSystemBox = false;
			this.tcPlugins.CanReorderTabs = false;
			this.tcPlugins.CloseButtonOnTabsAlwaysDisplayed = false;
			this.tcPlugins.Controls.Add(this.tabControlPanel1);
			this.tcPlugins.Controls.Add(this.tabControlPanel3);
			this.tcPlugins.Controls.Add(this.tabControlPanel4);
			this.tcPlugins.Controls.Add(this.tabControlPanel2);
			this.tcPlugins.Controls.Add(this.tabControlPanel5);
			this.tcPlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tcPlugins.Location = new System.Drawing.Point(0,73);
			this.tcPlugins.Name = "tcPlugins";
			this.tcPlugins.SelectedTabFont = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Bold);
			this.tcPlugins.SelectedTabIndex = 0;
			this.tcPlugins.Size = new System.Drawing.Size(655,310);
			this.tcPlugins.Style = DevComponents.DotNetBar.eTabStripStyle.Office2007Document;
			this.tcPlugins.TabIndex = 0;
			this.tcPlugins.TabLayoutType = DevComponents.DotNetBar.eTabLayoutType.FixedWithNavigationBox;
			this.tcPlugins.Tabs.Add(this.tiSceneNodes);
			this.tcPlugins.Tabs.Add(this.tiControllers);
			this.tcPlugins.Tabs.Add(this.tiShaders);
			this.tcPlugins.Tabs.Add(this.tiPostEffects);
			this.tcPlugins.Tabs.Add(this.tiCustomData);
			this.tcPlugins.Text = "tabControl1";
			// 
			// tabControlPanel3
			// 
			this.tabControlPanel3.Controls.Add(this.lvCustomDataPlugins);
			this.tabControlPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel3.Location = new System.Drawing.Point(0,22);
			this.tabControlPanel3.Name = "tabControlPanel3";
			this.tabControlPanel3.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel3.Size = new System.Drawing.Size(655,288);
			this.tabControlPanel3.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel3.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel3.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel3.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel3.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Bottom)));
			this.tabControlPanel3.Style.GradientAngle = 90;
			this.tabControlPanel3.TabIndex = 3;
			this.tabControlPanel3.TabItem = this.tiCustomData;
			// 
			// lvCustomDataPlugins
			// 
			// 
			// 
			// 
			this.lvCustomDataPlugins.Border.Class = "ListViewBorder";
			this.lvCustomDataPlugins.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader13,
            this.columnHeader14,
            this.columnHeader15,
            this.columnHeader16});
			this.lvCustomDataPlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lvCustomDataPlugins.FullRowSelect = true;
			this.lvCustomDataPlugins.GridLines = true;
			this.lvCustomDataPlugins.Location = new System.Drawing.Point(1,1);
			this.lvCustomDataPlugins.Name = "lvCustomDataPlugins";
			this.lvCustomDataPlugins.Size = new System.Drawing.Size(653,286);
			this.lvCustomDataPlugins.TabIndex = 1;
			this.lvCustomDataPlugins.UseCompatibleStateImageBehavior = false;
			this.lvCustomDataPlugins.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader13
			// 
			this.columnHeader13.Text = "Name";
			this.columnHeader13.Width = 220;
			// 
			// columnHeader14
			// 
			this.columnHeader14.Text = "Version";
			this.columnHeader14.Width = 120;
			// 
			// columnHeader15
			// 
			this.columnHeader15.Text = "Description";
			this.columnHeader15.Width = 200;
			// 
			// columnHeader16
			// 
			this.columnHeader16.Text = "DLL Location";
			this.columnHeader16.Width = 400;
			// 
			// tiCustomData
			// 
			this.tiCustomData.AttachedControl = this.tabControlPanel3;
			this.tiCustomData.Name = "tiCustomData";
			this.tiCustomData.Text = "Custom Data";
			// 
			// tabControlPanel4
			// 
			this.tabControlPanel4.Controls.Add(this.lvPostEffectPlugins);
			this.tabControlPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel4.Location = new System.Drawing.Point(0,22);
			this.tabControlPanel4.Name = "tabControlPanel4";
			this.tabControlPanel4.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel4.Size = new System.Drawing.Size(655,288);
			this.tabControlPanel4.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel4.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel4.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel4.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel4.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Bottom)));
			this.tabControlPanel4.Style.GradientAngle = 90;
			this.tabControlPanel4.TabIndex = 4;
			this.tabControlPanel4.TabItem = this.tiPostEffects;
			// 
			// lvPostEffectPlugins
			// 
			// 
			// 
			// 
			this.lvPostEffectPlugins.Border.Class = "ListViewBorder";
			this.lvPostEffectPlugins.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader9,
            this.columnHeader10,
            this.columnHeader11,
            this.columnHeader12});
			this.lvPostEffectPlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lvPostEffectPlugins.FullRowSelect = true;
			this.lvPostEffectPlugins.GridLines = true;
			this.lvPostEffectPlugins.Location = new System.Drawing.Point(1,1);
			this.lvPostEffectPlugins.Name = "lvPostEffectPlugins";
			this.lvPostEffectPlugins.Size = new System.Drawing.Size(653,286);
			this.lvPostEffectPlugins.TabIndex = 1;
			this.lvPostEffectPlugins.UseCompatibleStateImageBehavior = false;
			this.lvPostEffectPlugins.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader9
			// 
			this.columnHeader9.Text = "Name";
			this.columnHeader9.Width = 220;
			// 
			// columnHeader10
			// 
			this.columnHeader10.Text = "Version";
			this.columnHeader10.Width = 120;
			// 
			// columnHeader11
			// 
			this.columnHeader11.Text = "Description";
			this.columnHeader11.Width = 200;
			// 
			// columnHeader12
			// 
			this.columnHeader12.Text = "DLL Location";
			this.columnHeader12.Width = 400;
			// 
			// tiPostEffects
			// 
			this.tiPostEffects.AttachedControl = this.tabControlPanel4;
			this.tiPostEffects.Name = "tiPostEffects";
			this.tiPostEffects.Text = "Post Effects";
			// 
			// tabControlPanel2
			// 
			this.tabControlPanel2.Controls.Add(this.lvShaderPlugins);
			this.tabControlPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel2.Location = new System.Drawing.Point(0,22);
			this.tabControlPanel2.Name = "tabControlPanel2";
			this.tabControlPanel2.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel2.Size = new System.Drawing.Size(655,288);
			this.tabControlPanel2.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel2.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel2.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel2.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel2.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Bottom)));
			this.tabControlPanel2.Style.GradientAngle = 90;
			this.tabControlPanel2.TabIndex = 2;
			this.tabControlPanel2.TabItem = this.tiShaders;
			// 
			// lvShaderPlugins
			// 
			// 
			// 
			// 
			this.lvShaderPlugins.Border.Class = "ListViewBorder";
			this.lvShaderPlugins.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8});
			this.lvShaderPlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lvShaderPlugins.FullRowSelect = true;
			this.lvShaderPlugins.GridLines = true;
			this.lvShaderPlugins.Location = new System.Drawing.Point(1,1);
			this.lvShaderPlugins.Name = "lvShaderPlugins";
			this.lvShaderPlugins.Size = new System.Drawing.Size(653,286);
			this.lvShaderPlugins.TabIndex = 1;
			this.lvShaderPlugins.UseCompatibleStateImageBehavior = false;
			this.lvShaderPlugins.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Name";
			this.columnHeader5.Width = 220;
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "Version";
			this.columnHeader6.Width = 120;
			// 
			// columnHeader7
			// 
			this.columnHeader7.Text = "Description";
			this.columnHeader7.Width = 200;
			// 
			// columnHeader8
			// 
			this.columnHeader8.Text = "DLL Location";
			this.columnHeader8.Width = 400;
			// 
			// tiShaders
			// 
			this.tiShaders.AttachedControl = this.tabControlPanel2;
			this.tiShaders.Name = "tiShaders";
			this.tiShaders.Text = "Shaders";
			// 
			// tabControlPanel5
			// 
			this.tabControlPanel5.Controls.Add(this.lvControllerPlugins);
			this.tabControlPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel5.Location = new System.Drawing.Point(0,22);
			this.tabControlPanel5.Name = "tabControlPanel5";
			this.tabControlPanel5.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel5.Size = new System.Drawing.Size(655,288);
			this.tabControlPanel5.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel5.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel5.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel5.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel5.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Bottom)));
			this.tabControlPanel5.Style.GradientAngle = 90;
			this.tabControlPanel5.TabIndex = 5;
			this.tabControlPanel5.TabItem = this.tiControllers;
			// 
			// lvControllerPlugins
			// 
			// 
			// 
			// 
			this.lvControllerPlugins.Border.Class = "ListViewBorder";
			this.lvControllerPlugins.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
			this.lvControllerPlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lvControllerPlugins.FullRowSelect = true;
			this.lvControllerPlugins.GridLines = true;
			this.lvControllerPlugins.Location = new System.Drawing.Point(1,1);
			this.lvControllerPlugins.Name = "lvControllerPlugins";
			this.lvControllerPlugins.Size = new System.Drawing.Size(653,286);
			this.lvControllerPlugins.TabIndex = 1;
			this.lvControllerPlugins.UseCompatibleStateImageBehavior = false;
			this.lvControllerPlugins.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Name";
			this.columnHeader1.Width = 220;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Version";
			this.columnHeader2.Width = 120;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Description";
			this.columnHeader3.Width = 200;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "DLL Location";
			this.columnHeader4.Width = 400;
			// 
			// tiControllers
			// 
			this.tiControllers.AttachedControl = this.tabControlPanel5;
			this.tiControllers.Name = "tiControllers";
			this.tiControllers.Text = "Controllers";
			// 
			// tabControlPanel1
			// 
			this.tabControlPanel1.Controls.Add(this.lvSceneNodePlugins);
			this.tabControlPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel1.Location = new System.Drawing.Point(0,22);
			this.tabControlPanel1.Name = "tabControlPanel1";
			this.tabControlPanel1.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel1.Size = new System.Drawing.Size(655,288);
			this.tabControlPanel1.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabControlPanel1.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabControlPanel1.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabControlPanel1.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabControlPanel1.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right) 
            | DevComponents.DotNetBar.eBorderSide.Bottom)));
			this.tabControlPanel1.Style.GradientAngle = 90;
			this.tabControlPanel1.TabIndex = 1;
			this.tabControlPanel1.TabItem = this.tiSceneNodes;
			// 
			// lvSceneNodePlugins
			// 
			// 
			// 
			// 
			this.lvSceneNodePlugins.Border.Class = "ListViewBorder";
			this.lvSceneNodePlugins.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colName,
            this.colVersion,
            this.colDescription,
            this.colLocation});
			this.lvSceneNodePlugins.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lvSceneNodePlugins.FullRowSelect = true;
			this.lvSceneNodePlugins.GridLines = true;
			this.lvSceneNodePlugins.Location = new System.Drawing.Point(1,1);
			this.lvSceneNodePlugins.Name = "lvSceneNodePlugins";
			this.lvSceneNodePlugins.Size = new System.Drawing.Size(653,286);
			this.lvSceneNodePlugins.TabIndex = 0;
			this.lvSceneNodePlugins.UseCompatibleStateImageBehavior = false;
			this.lvSceneNodePlugins.View = System.Windows.Forms.View.Details;
			// 
			// colName
			// 
			this.colName.Text = "Name";
			this.colName.Width = 220;
			// 
			// colVersion
			// 
			this.colVersion.Text = "Version";
			this.colVersion.Width = 120;
			// 
			// colDescription
			// 
			this.colDescription.Text = "Description";
			this.colDescription.Width = 200;
			// 
			// colLocation
			// 
			this.colLocation.Text = "DLL Location";
			this.colLocation.Width = 400;
			// 
			// tiSceneNodes
			// 
			this.tiSceneNodes.AttachedControl = this.tabControlPanel1;
			this.tiSceneNodes.Name = "tiSceneNodes";
			this.tiSceneNodes.Text = "Scene Nodes";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.rtxtDescription);
			this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox1.Location = new System.Drawing.Point(0,0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(655,73);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(12,19);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(637,43);
			this.rtxtDescription.TabIndex = 10;
			this.rtxtDescription.TabStop = false;
			this.rtxtDescription.Text = resources.GetString("rtxtDescription.Text");
			// 
			// PluginDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(655,383);
			this.Controls.Add(this.tcPlugins);
			this.Controls.Add(this.groupBox1);
			this.Name = "PluginDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "Plugin Information";
			((System.ComponentModel.ISupportInitialize)(this.tcPlugins)).EndInit();
			this.tcPlugins.ResumeLayout(false);
			this.tabControlPanel3.ResumeLayout(false);
			this.tabControlPanel4.ResumeLayout(false);
			this.tabControlPanel2.ResumeLayout(false);
			this.tabControlPanel5.ResumeLayout(false);
			this.tabControlPanel1.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private DevComponents.DotNetBar.TabControl tcPlugins;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel1;
		private DevComponents.DotNetBar.TabItem tiSceneNodes;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel3;
		private DevComponents.DotNetBar.TabItem tiCustomData;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel2;
		private DevComponents.DotNetBar.TabItem tiShaders;
		private DevComponents.DotNetBar.Controls.ListViewEx lvSceneNodePlugins;
		private System.Windows.Forms.ColumnHeader colName;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RichTextBox rtxtDescription;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel4;
		private DevComponents.DotNetBar.TabItem tiPostEffects;
		private System.Windows.Forms.ColumnHeader colVersion;
		private System.Windows.Forms.ColumnHeader colDescription;
		private System.Windows.Forms.ColumnHeader colLocation;
		private DevComponents.DotNetBar.TabControlPanel tabControlPanel5;
		private DevComponents.DotNetBar.TabItem tiControllers;
		private DevComponents.DotNetBar.Controls.ListViewEx lvCustomDataPlugins;
		private System.Windows.Forms.ColumnHeader columnHeader13;
		private System.Windows.Forms.ColumnHeader columnHeader14;
		private System.Windows.Forms.ColumnHeader columnHeader15;
		private System.Windows.Forms.ColumnHeader columnHeader16;
		private DevComponents.DotNetBar.Controls.ListViewEx lvPostEffectPlugins;
		private System.Windows.Forms.ColumnHeader columnHeader9;
		private System.Windows.Forms.ColumnHeader columnHeader10;
		private System.Windows.Forms.ColumnHeader columnHeader11;
		private System.Windows.Forms.ColumnHeader columnHeader12;
		private DevComponents.DotNetBar.Controls.ListViewEx lvShaderPlugins;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader8;
		private DevComponents.DotNetBar.Controls.ListViewEx lvControllerPlugins;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
	}
}