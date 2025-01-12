namespace XVPDesigner.UserControls
{
	partial class GraphExplorer
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
			this.scMain = new System.Windows.Forms.SplitContainer();
			this.pgridProperties = new System.Windows.Forms.PropertyGrid();
			this.graphViewer = new GraphViewer();
			this.scMain.Panel1.SuspendLayout();
			this.scMain.Panel2.SuspendLayout();
			this.scMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// scMain
			// 
			this.scMain.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.scMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.scMain.Location = new System.Drawing.Point(0,0);
			this.scMain.Name = "scMain";
			this.scMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// scMain.Panel1
			// 
			this.scMain.Panel1.Controls.Add(this.graphViewer);
			// 
			// scMain.Panel2
			// 
			this.scMain.Panel2.Controls.Add(this.pgridProperties);
			this.scMain.Size = new System.Drawing.Size(463,385);
			this.scMain.SplitterDistance = 202;
			this.scMain.TabIndex = 0;
			// 
			// pgridProperties
			// 
			this.pgridProperties.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pgridProperties.Location = new System.Drawing.Point(0,0);
			this.pgridProperties.Name = "pgridProperties";
			this.pgridProperties.Size = new System.Drawing.Size(461,177);
			this.pgridProperties.TabIndex = 0;
			this.pgridProperties.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.pgridProperties_PropertyValueChanged);
			// 
			// graphViewer
			// 
			this.graphViewer.BackColor = System.Drawing.Color.Lavender;
			this.graphViewer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.graphViewer.Location = new System.Drawing.Point(0,0);
			this.graphViewer.Name = "graphViewer";
			this.graphViewer.Size = new System.Drawing.Size(461,200);
			this.graphViewer.TabIndex = 0;
			this.graphViewer.Load += new System.EventHandler(this.graphViewer_Load);
			this.graphViewer.SelectedArrowChanged += new GraphViewer.SelectedArrowChangedHandler(this.graphViewer_SelectedArrowChanged);
			// 
			// GraphExplorer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.scMain);
			this.Name = "GraphExplorer";
			this.Size = new System.Drawing.Size(463,385);
			this.scMain.Panel1.ResumeLayout(false);
			this.scMain.Panel2.ResumeLayout(false);
			this.scMain.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer scMain;
		private GraphViewer graphViewer;
		private System.Windows.Forms.PropertyGrid pgridProperties;
	}
}
