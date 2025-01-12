using Utilities;

namespace XVPDesigner.UserControls
{
    partial class GraphViewer
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
			if (disposing)
			{
				if (components != null)
					components.Dispose();
				
				//Dispose of the graph nodes
				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
					node.Value.Dispose();
				m_graph.Clear();

				//Dispose of shared font
				m_font.Dispose();
				m_highlightBrush.Dispose();
				m_selectedArrowPen.Dispose();
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(GraphViewer));
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.bar1 = new DevComponents.DotNetBar.Bar();
			this.btnAddNode = new DevComponents.DotNetBar.ButtonItem();
			this.btnRemoveNode = new DevComponents.DotNetBar.ButtonItem();
			this.btnFitInWindow = new DevComponents.DotNetBar.ButtonItem();
			this.btnAutoArrange = new DevComponents.DotNetBar.ButtonItem();
			this.btnClear = new DevComponents.DotNetBar.ButtonItem();
			this.btnStats = new DevComponents.DotNetBar.ButtonItem();
			this.buttonItem1 = new DevComponents.DotNetBar.ButtonItem();
			((System.ComponentModel.ISupportInitialize)(this.bar1)).BeginInit();
			this.SuspendLayout();
			// 
			// bar1
			// 
			this.bar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.bar1.Items.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.btnAddNode,
            this.btnRemoveNode,
            this.btnFitInWindow,
            this.btnAutoArrange,
            this.btnClear,
            this.btnStats,
            this.buttonItem1});
			this.bar1.Location = new System.Drawing.Point(0,0);
			this.bar1.Name = "bar1";
			this.bar1.RoundCorners = false;
			this.bar1.Size = new System.Drawing.Size(481,25);
			this.bar1.Stretch = true;
			this.bar1.Style = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
			this.bar1.TabIndex = 1;
			this.bar1.TabStop = false;
			this.bar1.Text = "bar1";
			// 
			// btnAddNode
			// 
			this.btnAddNode.Image = ((System.Drawing.Image)(resources.GetObject("btnAddNode.Image")));
			this.btnAddNode.ImagePaddingHorizontal = 8;
			this.btnAddNode.Name = "btnAddNode";
			this.btnAddNode.Text = "buttonItem3";
			this.btnAddNode.Click += new System.EventHandler(this.tsbAddNode_Click);
			// 
			// btnRemoveNode
			// 
			this.btnRemoveNode.Image = ((System.Drawing.Image)(resources.GetObject("btnRemoveNode.Image")));
			this.btnRemoveNode.ImagePaddingHorizontal = 8;
			this.btnRemoveNode.Name = "btnRemoveNode";
			this.btnRemoveNode.Text = "buttonItem4";
			this.btnRemoveNode.Click += new System.EventHandler(this.tsbRemoveNode_Click);
			// 
			// btnFitInWindow
			// 
			this.btnFitInWindow.Image = ((System.Drawing.Image)(resources.GetObject("btnFitInWindow.Image")));
			this.btnFitInWindow.ImagePaddingHorizontal = 8;
			this.btnFitInWindow.Name = "btnFitInWindow";
			this.btnFitInWindow.Text = "buttonItem1";
			this.btnFitInWindow.Click += new System.EventHandler(this.tsbZoomExtents_Click);
			// 
			// btnAutoArrange
			// 
			this.btnAutoArrange.Image = ((System.Drawing.Image)(resources.GetObject("btnAutoArrange.Image")));
			this.btnAutoArrange.ImagePaddingHorizontal = 8;
			this.btnAutoArrange.Name = "btnAutoArrange";
			this.btnAutoArrange.Text = "buttonItem2";
			this.btnAutoArrange.Click += new System.EventHandler(this.tsbAutoArrange_Click);
			// 
			// btnClear
			// 
			this.btnClear.Image = ((System.Drawing.Image)(resources.GetObject("btnClear.Image")));
			this.btnClear.ImagePaddingHorizontal = 8;
			this.btnClear.Name = "btnClear";
			this.btnClear.Text = "buttonItem1";
			this.btnClear.Click += new System.EventHandler(this.tsbClear_Click);
			// 
			// btnStats
			// 
			this.btnStats.Image = ((System.Drawing.Image)(resources.GetObject("btnStats.Image")));
			this.btnStats.ImagePaddingHorizontal = 8;
			this.btnStats.Name = "btnStats";
			this.btnStats.Text = "buttonItem2";
			this.btnStats.Click += new System.EventHandler(this.tsbStats_Click);
			// 
			// buttonItem1
			// 
			this.buttonItem1.ImagePaddingHorizontal = 8;
			this.buttonItem1.Name = "buttonItem1";
			this.buttonItem1.Text = "serialize\r\n";
			this.buttonItem1.Click += new System.EventHandler(this.buttonItem1_Click);
			// 
			// GraphViewer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.Lavender;
			this.Controls.Add(this.bar1);
			this.Name = "GraphViewer";
			this.Size = new System.Drawing.Size(481,432);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.GraphExplorer_MouseMove);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.GraphExplorer_KeyUp);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GraphExplorer_MouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.GraphExplorer_MouseUp);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.GraphExplorer_KeyDown);
			((System.ComponentModel.ISupportInitialize)(this.bar1)).EndInit();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.ToolTip toolTip1;
		private DevComponents.DotNetBar.Bar bar1;
		private DevComponents.DotNetBar.ButtonItem btnClear;
		private DevComponents.DotNetBar.ButtonItem btnStats;
		private DevComponents.DotNetBar.ButtonItem btnFitInWindow;
		private DevComponents.DotNetBar.ButtonItem btnAutoArrange;
		private DevComponents.DotNetBar.ButtonItem btnAddNode;
		private DevComponents.DotNetBar.ButtonItem btnRemoveNode;
		private DevComponents.DotNetBar.ButtonItem buttonItem1;


	}
}
