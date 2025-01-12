
using Utilities;
namespace XVPDesigner.UserControls
{
    partial class StageExplorer
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StageExplorer));
			this.tabControlPanel1 = new DevComponents.DotNetBar.TabControlPanel();
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.tabsScene = new DevComponents.DotNetBar.TabControl();
			this.tabHierarchyPanel = new DevComponents.DotNetBar.TabControlPanel();
			this.tsHierarchy = new System.Windows.Forms.ToolStrip();
			this.tsbExpandAllHierarchyNodes = new System.Windows.Forms.ToolStripButton();
			this.tsbCollapseAllHierarchyNodes = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.tsbHierarchyView = new System.Windows.Forms.ToolStripButton();
			this.tsbMoveUp = new System.Windows.Forms.ToolStripButton();
			this.tsbMoveDown = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			this.tabHierarchy = new DevComponents.DotNetBar.TabItem(this.components);
			this.superTooltip = new DevComponents.DotNetBar.SuperTooltip();
			this.tvHierarchy = new Utilities.TreeViewMS();
			this.tabControlPanel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.tabsScene)).BeginInit();
			this.tabsScene.SuspendLayout();
			this.tabHierarchyPanel.SuspendLayout();
			this.tsHierarchy.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControlPanel1
			// 
			this.tabControlPanel1.Controls.Add(this.treeView1);
			this.tabControlPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControlPanel1.Location = new System.Drawing.Point(0,0);
			this.tabControlPanel1.Name = "tabControlPanel1";
			this.tabControlPanel1.Padding = new System.Windows.Forms.Padding(1);
			this.tabControlPanel1.Size = new System.Drawing.Size(372,391);
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
			this.treeView1.Size = new System.Drawing.Size(370,389);
			this.treeView1.TabIndex = 1;
			// 
			// tabsScene
			// 
			this.tabsScene.CanReorderTabs = true;
			this.tabsScene.Controls.Add(this.tabHierarchyPanel);
			this.tabsScene.Controls.Add(this.tabControlPanel1);
			this.tabsScene.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabsScene.Font = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Regular,System.Drawing.GraphicsUnit.Point,((byte)(0)));
			this.tabsScene.Location = new System.Drawing.Point(0,0);
			this.tabsScene.Name = "tabsScene";
			this.tabsScene.SelectedTabFont = new System.Drawing.Font("Microsoft Sans Serif",8.25F,System.Drawing.FontStyle.Bold);
			this.tabsScene.SelectedTabIndex = 0;
			this.tabsScene.Size = new System.Drawing.Size(372,416);
			this.tabsScene.Style = DevComponents.DotNetBar.eTabStripStyle.Office2007Document;
			this.tabsScene.TabAlignment = DevComponents.DotNetBar.eTabStripAlignment.Bottom;
			this.tabsScene.TabIndex = 0;
			this.tabsScene.TabLayoutType = DevComponents.DotNetBar.eTabLayoutType.MultilineNoNavigationBox;
			this.tabsScene.Tabs.Add(this.tabHierarchy);
			this.tabsScene.Text = "tabNodes";
			// 
			// tabHierarchyPanel
			// 
			this.tabHierarchyPanel.AutoSize = true;
			this.tabHierarchyPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.tabHierarchyPanel.ColorSchemeStyle = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.tabHierarchyPanel.Controls.Add(this.tvHierarchy);
			this.tabHierarchyPanel.Controls.Add(this.tsHierarchy);
			this.tabHierarchyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabHierarchyPanel.Location = new System.Drawing.Point(0,0);
			this.tabHierarchyPanel.Name = "tabHierarchyPanel";
			this.tabHierarchyPanel.Padding = new System.Windows.Forms.Padding(1);
			this.tabHierarchyPanel.Size = new System.Drawing.Size(372,391);
			this.tabHierarchyPanel.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(247)))),((int)(((byte)(247)))),((int)(((byte)(247)))));
			this.tabHierarchyPanel.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(195)))),((int)(((byte)(195)))),((int)(((byte)(195)))));
			this.tabHierarchyPanel.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			this.tabHierarchyPanel.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(70)))),((int)(((byte)(70)))),((int)(((byte)(70)))));
			this.tabHierarchyPanel.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right)
						| DevComponents.DotNetBar.eBorderSide.Top)));
			this.tabHierarchyPanel.Style.GradientAngle = -90;
			this.tabHierarchyPanel.TabIndex = 4;
			this.tabHierarchyPanel.TabItem = this.tabHierarchy;
			// 
			// tsHierarchy
			// 
			this.tsHierarchy.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.tsHierarchy.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbExpandAllHierarchyNodes,
            this.tsbCollapseAllHierarchyNodes,
            this.toolStripSeparator1,
            this.tsbHierarchyView,
            this.tsbMoveUp,
            this.tsbMoveDown,
            this.toolStripSeparator4});
			this.tsHierarchy.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.tsHierarchy.Location = new System.Drawing.Point(1,1);
			this.tsHierarchy.Name = "tsHierarchy";
			this.tsHierarchy.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.tsHierarchy.Size = new System.Drawing.Size(370,25);
			this.tsHierarchy.TabIndex = 2;
			this.tsHierarchy.Text = "hierarchyToolStrip";
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
			this.tsbExpandAllHierarchyNodes.Click += new System.EventHandler(this.tsbExpandAllHierarchyNodes_Click);
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
			this.tsbCollapseAllHierarchyNodes.Click += new System.EventHandler(this.tsbCollapseAllHierarchyNodes_Click);
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
			// tsbMoveUp
			// 
			this.tsbMoveUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tsbMoveUp.Image = ((System.Drawing.Image)(resources.GetObject("tsbMoveUp.Image")));
			this.tsbMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbMoveUp.Name = "tsbMoveUp";
			this.tsbMoveUp.Size = new System.Drawing.Size(23,22);
			this.tsbMoveUp.Text = "toolStripButton1";
			this.tsbMoveUp.ToolTipText = "Move Node Up";
			this.tsbMoveUp.Click += new System.EventHandler(this.tsbMoveUp_Click);
			// 
			// tsbMoveDown
			// 
			this.tsbMoveDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tsbMoveDown.Image = ((System.Drawing.Image)(resources.GetObject("tsbMoveDown.Image")));
			this.tsbMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tsbMoveDown.Name = "tsbMoveDown";
			this.tsbMoveDown.Size = new System.Drawing.Size(23,22);
			this.tsbMoveDown.Text = "toolStripButton2";
			this.tsbMoveDown.ToolTipText = "Move Node Down";
			this.tsbMoveDown.Click += new System.EventHandler(this.tsbMoveDown_Click);
			// 
			// toolStripSeparator4
			// 
			this.toolStripSeparator4.Name = "toolStripSeparator4";
			this.toolStripSeparator4.Size = new System.Drawing.Size(6,25);
			// 
			// tabHierarchy
			// 
			this.tabHierarchy.AttachedControl = this.tabHierarchyPanel;
			this.tabHierarchy.Image = ((System.Drawing.Image)(resources.GetObject("tabHierarchy.Image")));
			this.tabHierarchy.Name = "tabHierarchy";
			this.tabHierarchy.Text = "Hierarchy";
			this.tabHierarchy.Click += new System.EventHandler(this.tabHierarchy_Click);
			// 
			// superTooltip
			// 
			this.superTooltip.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			// 
			// tvHierarchy
			// 
			this.tvHierarchy.AllowDrop = true;
			this.tvHierarchy.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvHierarchy.HideSelection = false;
			this.tvHierarchy.LabelEdit = true;
			this.tvHierarchy.Location = new System.Drawing.Point(1,26);
			this.tvHierarchy.Name = "tvHierarchy";
			this.tvHierarchy.SelectedNodes = ((System.Collections.ArrayList)(resources.GetObject("tvHierarchy.SelectedNodes")));
			this.tvHierarchy.Size = new System.Drawing.Size(370,364);
			this.tvHierarchy.TabIndex = 1;
			this.tvHierarchy.EnabledChanged += new System.EventHandler(this.tvHierarchy_EnabledChanged);
			this.tvHierarchy.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.tvHierarchy_AfterLabelEdit);
			this.tvHierarchy.DragDrop += new System.Windows.Forms.DragEventHandler(this.tvHierarchy_DragDrop);
			this.tvHierarchy.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvHierarchy_AfterSelect);
			this.tvHierarchy.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tvHierarchy_MouseDown);
			this.tvHierarchy.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tvHierarchy_KeyDown);
			this.tvHierarchy.DragOver += new System.Windows.Forms.DragEventHandler(this.tvHierarchy_DragOver);
			// 
			// StageExplorer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tabsScene);
			this.Name = "StageExplorer";
			this.Size = new System.Drawing.Size(372,416);
			this.tabControlPanel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.tabsScene)).EndInit();
			this.tabsScene.ResumeLayout(false);
			this.tabsScene.PerformLayout();
			this.tabHierarchyPanel.ResumeLayout(false);
			this.tabHierarchyPanel.PerformLayout();
			this.tsHierarchy.ResumeLayout(false);
			this.tsHierarchy.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

		private DevComponents.DotNetBar.TabControlPanel tabControlPanel1;
		private System.Windows.Forms.TreeView treeView1;
		private DevComponents.DotNetBar.TabControl tabsScene;
		private DevComponents.DotNetBar.TabControlPanel tabHierarchyPanel;
		private DevComponents.DotNetBar.TabItem tabHierarchy;
		//private System.Windows.Forms.TreeView tvHierarchy;
		//private System.Windows.Forms.TreeView tvControllers;
		private System.Windows.Forms.ToolStrip tsHierarchy;
		private System.Windows.Forms.ToolStripButton tsbExpandAllHierarchyNodes;
		private System.Windows.Forms.ToolStripButton tsbCollapseAllHierarchyNodes;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tsbHierarchyView;
		private DevComponents.DotNetBar.SuperTooltip superTooltip;
		//private System.Windows.Forms.TreeView tvMaterials;
		private System.Windows.Forms.ToolStripButton tsbMoveUp;
		private System.Windows.Forms.ToolStripButton tsbMoveDown;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
		private TreeViewMS tvHierarchy;


    }
}
