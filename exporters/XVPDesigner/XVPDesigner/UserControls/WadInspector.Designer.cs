using System.Windows.Forms;
namespace XVPDesigner.UserControls
{
    partial class WadInspector
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WadInspector));
			this.lvFiles = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.colFilename = new System.Windows.Forms.ColumnHeader();
			this.colOrgSize = new System.Windows.Forms.ColumnHeader();
			this.colCompressed = new System.Windows.Forms.ColumnHeader();
			this.colCompressedSize = new System.Windows.Forms.ColumnHeader();
			this.colCompressionRatio = new System.Windows.Forms.ColumnHeader();
			this.cmFiles = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.tsmiExtractSelectedFiles = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmiRemoveSelectedFiles = new System.Windows.Forms.ToolStripMenuItem();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.btnRemoveFiles = new DevComponents.DotNetBar.ButtonX();
			this.lblWadSize = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.btnAddFile = new DevComponents.DotNetBar.ButtonX();
			this.panel1 = new System.Windows.Forms.Panel();
			this.cmFiles.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// lvFiles
			// 
			this.lvFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			// 
			// 
			// 
			this.lvFiles.Border.Class = "ListViewBorder";
			this.lvFiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colFilename,
            this.colOrgSize,
            this.colCompressed,
            this.colCompressedSize,
            this.colCompressionRatio});
			this.lvFiles.ContextMenuStrip = this.cmFiles;
			this.lvFiles.Font = new System.Drawing.Font("Microsoft Sans Serif",8F,System.Drawing.FontStyle.Regular,System.Drawing.GraphicsUnit.Point,((byte)(0)));
			this.lvFiles.FullRowSelect = true;
			this.lvFiles.GridLines = true;
			this.lvFiles.Location = new System.Drawing.Point(3,16);
			this.lvFiles.Name = "lvFiles";
			this.lvFiles.Size = new System.Drawing.Size(756,426);
			this.lvFiles.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvFiles.TabIndex = 0;
			this.lvFiles.UseCompatibleStateImageBehavior = false;
			this.lvFiles.View = System.Windows.Forms.View.Details;
			this.lvFiles.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lvFiles_MouseDoubleClick);
			// 
			// colFilename
			// 
			this.colFilename.Text = "Filename";
			this.colFilename.Width = 241;
			// 
			// colOrgSize
			// 
			this.colOrgSize.Text = "Original Size";
			this.colOrgSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.colOrgSize.Width = 86;
			// 
			// colCompressed
			// 
			this.colCompressed.Text = "Compressed";
			this.colCompressed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.colCompressed.Width = 83;
			// 
			// colCompressedSize
			// 
			this.colCompressedSize.Text = "Compressed Size";
			this.colCompressedSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.colCompressedSize.Width = 101;
			// 
			// colCompressionRatio
			// 
			this.colCompressionRatio.Text = "Compression Ratio";
			this.colCompressionRatio.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.colCompressionRatio.Width = 103;
			// 
			// cmFiles
			// 
			this.cmFiles.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiExtractSelectedFiles,
            this.tsmiRemoveSelectedFiles});
			this.cmFiles.Name = "cmFiles";
			this.cmFiles.Size = new System.Drawing.Size(191,48);
			// 
			// tsmiExtractSelectedFiles
			// 
			this.tsmiExtractSelectedFiles.Name = "tsmiExtractSelectedFiles";
			this.tsmiExtractSelectedFiles.Size = new System.Drawing.Size(190,22);
			this.tsmiExtractSelectedFiles.Text = "Extract Selected Files";
			this.tsmiExtractSelectedFiles.Click += new System.EventHandler(this.tsmiExtractSelectedFiles_Click);
			// 
			// tsmiRemoveSelectedFiles
			// 
			this.tsmiRemoveSelectedFiles.Name = "tsmiRemoveSelectedFiles";
			this.tsmiRemoveSelectedFiles.Size = new System.Drawing.Size(190,22);
			this.tsmiRemoveSelectedFiles.Text = "Remove Selected Files";
			this.tsmiRemoveSelectedFiles.Click += new System.EventHandler(this.tsmiRemoveSelectedFiles_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.groupBox2);
			this.groupBox1.Controls.Add(this.lvFiles);
			this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBox1.Location = new System.Drawing.Point(0,0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(768,558);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Filelist";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.btnRemoveFiles);
			this.groupBox2.Controls.Add(this.lblWadSize);
			this.groupBox2.Controls.Add(this.label1);
			this.groupBox2.Controls.Add(this.btnAddFile);
			this.groupBox2.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.groupBox2.Location = new System.Drawing.Point(3,448);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(762,107);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Actions";
			// 
			// btnRemoveFiles
			// 
			this.btnRemoveFiles.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRemoveFiles.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRemoveFiles.Image = ((System.Drawing.Image)(resources.GetObject("btnRemoveFiles.Image")));
			this.btnRemoveFiles.Location = new System.Drawing.Point(6,72);
			this.btnRemoveFiles.Name = "btnRemoveFiles";
			this.btnRemoveFiles.Size = new System.Drawing.Size(146,23);
			this.btnRemoveFiles.TabIndex = 4;
			this.btnRemoveFiles.Text = "Remove Selected Files";
			this.btnRemoveFiles.Click += new System.EventHandler(this.btnRemoveFiles_Click);
			// 
			// lblWadSize
			// 
			this.lblWadSize.AutoSize = true;
			this.lblWadSize.Location = new System.Drawing.Point(100,18);
			this.lblWadSize.Name = "lblWadSize";
			this.lblWadSize.Size = new System.Drawing.Size(27,13);
			this.lblWadSize.TabIndex = 3;
			this.lblWadSize.Text = "1KB";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6,18);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(96,13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Total size of WAD:";
			// 
			// btnAddFile
			// 
			this.btnAddFile.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnAddFile.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnAddFile.Image = ((System.Drawing.Image)(resources.GetObject("btnAddFile.Image")));
			this.btnAddFile.Location = new System.Drawing.Point(6,43);
			this.btnAddFile.Name = "btnAddFile";
			this.btnAddFile.Size = new System.Drawing.Size(88,23);
			this.btnAddFile.TabIndex = 1;
			this.btnAddFile.Text = "Add Files...";
			this.btnAddFile.Click += new System.EventHandler(this.btnAddFile_Click);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.groupBox1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(0,0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(768,558);
			this.panel1.TabIndex = 2;
			// 
			// WadInspector
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.panel1);
			this.Name = "WadInspector";
			this.Size = new System.Drawing.Size(768,558);
			this.cmFiles.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private DevComponents.DotNetBar.Controls.ListViewEx lvFiles;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ColumnHeader colFilename;
        private System.Windows.Forms.ColumnHeader colOrgSize;
        private System.Windows.Forms.ColumnHeader colCompressed;
        private System.Windows.Forms.ColumnHeader colCompressedSize;
		private System.Windows.Forms.ColumnHeader colCompressionRatio;
        private DevComponents.DotNetBar.ButtonX btnAddFile;
        private Label lblWadSize;
        private Label label1;
        private ContextMenuStrip cmFiles;
        private ToolStripMenuItem tsmiRemoveSelectedFiles;
		private DevComponents.DotNetBar.ButtonX btnRemoveFiles;
		private ToolStripMenuItem tsmiExtractSelectedFiles;
    }
}
