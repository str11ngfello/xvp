namespace XVPDesigner.Dialogs
{
	partial class AttachControllerDialog
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AttachControllerDialog));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			this.btnCancel = new DevComponents.DotNetBar.ButtonX();
			this.btnAttach = new DevComponents.DotNetBar.ButtonX();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.lbSelectedNodes = new System.Windows.Forms.ListBox();
			this.txtName = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.lblType = new System.Windows.Forms.Label();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.btnDeselectDescendants = new DevComponents.DotNetBar.ButtonX();
			this.btnSelectDescendants = new DevComponents.DotNetBar.ButtonX();
			this.tvSceneNodes = new System.Windows.Forms.TreeView();
			this.imglistNodes = new System.Windows.Forms.ImageList(this.components);
			this.btnDeselectAll = new DevComponents.DotNetBar.ButtonX();
			this.btnSelectAll = new DevComponents.DotNetBar.ButtonX();
			this.groupBox1.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.groupBox4);
			this.groupBox1.Controls.Add(this.btnCancel);
			this.groupBox1.Controls.Add(this.btnAttach);
			this.groupBox1.Controls.Add(this.groupBox3);
			this.groupBox1.Controls.Add(this.txtName);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.groupBox2);
			this.groupBox1.Controls.Add(this.groupBox5);
			this.groupBox1.Location = new System.Drawing.Point(12,12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(628,428);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Controller Attachment Specification";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.rtxtDescription);
			this.groupBox4.Location = new System.Drawing.Point(6,19);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(285,67);
			this.groupBox4.TabIndex = 9;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Description";
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(15,17);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(264,42);
			this.rtxtDescription.TabIndex = 8;
			this.rtxtDescription.Text = "Description string for particular controller being attached goes here.";
			// 
			// btnCancel
			// 
			this.btnCancel.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnCancel.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(486,401);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(65,21);
			this.btnCancel.TabIndex = 8;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// btnAttach
			// 
			this.btnAttach.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnAttach.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnAttach.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnAttach.Location = new System.Drawing.Point(557,401);
			this.btnAttach.Name = "btnAttach";
			this.btnAttach.Size = new System.Drawing.Size(65,21);
			this.btnAttach.TabIndex = 7;
			this.btnAttach.Text = "Attach";
			this.btnAttach.Click += new System.EventHandler(this.btnAttach_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.groupBox3.Controls.Add(this.lbSelectedNodes);
			this.groupBox3.Location = new System.Drawing.Point(6,176);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(285,219);
			this.groupBox3.TabIndex = 6;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Selection";
			// 
			// lbSelectedNodes
			// 
			this.lbSelectedNodes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.lbSelectedNodes.FormattingEnabled = true;
			this.lbSelectedNodes.Location = new System.Drawing.Point(6,17);
			this.lbSelectedNodes.Name = "lbSelectedNodes";
			this.lbSelectedNodes.Size = new System.Drawing.Size(273,199);
			this.lbSelectedNodes.TabIndex = 1;
			// 
			// txtName
			// 
			this.txtName.Location = new System.Drawing.Point(105,133);
			this.txtName.Name = "txtName";
			this.txtName.Size = new System.Drawing.Size(180,20);
			this.txtName.TabIndex = 4;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(18,136);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(85,13);
			this.label3.TabIndex = 3;
			this.label3.Text = "Controller Name:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(18,114);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(81,13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Controller Type:";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.lblType);
			this.groupBox2.Location = new System.Drawing.Point(6,92);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(285,78);
			this.groupBox2.TabIndex = 5;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Properties";
			// 
			// lblType
			// 
			this.lblType.AutoSize = true;
			this.lblType.Location = new System.Drawing.Point(97,22);
			this.lblType.Name = "lblType";
			this.lblType.Size = new System.Drawing.Size(53,13);
			this.lblType.TabIndex = 2;
			this.lblType.Text = "Animation";
			// 
			// groupBox5
			// 
			this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox5.Controls.Add(this.btnDeselectDescendants);
			this.groupBox5.Controls.Add(this.btnSelectDescendants);
			this.groupBox5.Controls.Add(this.tvSceneNodes);
			this.groupBox5.Controls.Add(this.btnDeselectAll);
			this.groupBox5.Controls.Add(this.btnSelectAll);
			this.groupBox5.Location = new System.Drawing.Point(304,19);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(318,376);
			this.groupBox5.TabIndex = 15;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Scene Explorer";
			// 
			// btnDeselectDescendants
			// 
			this.btnDeselectDescendants.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeselectDescendants.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnDeselectDescendants.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeselectDescendants.Location = new System.Drawing.Point(169,53);
			this.btnDeselectDescendants.Name = "btnDeselectDescendants";
			this.btnDeselectDescendants.Size = new System.Drawing.Size(143,21);
			this.btnDeselectDescendants.TabIndex = 14;
			this.btnDeselectDescendants.Text = "Deselect All Descendants";
			this.btnDeselectDescendants.Click += new System.EventHandler(this.btnDeselectDescendants_Click);
			// 
			// btnSelectDescendants
			// 
			this.btnSelectDescendants.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnSelectDescendants.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnSelectDescendants.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnSelectDescendants.Location = new System.Drawing.Point(169,26);
			this.btnSelectDescendants.Name = "btnSelectDescendants";
			this.btnSelectDescendants.Size = new System.Drawing.Size(143,21);
			this.btnSelectDescendants.TabIndex = 13;
			this.btnSelectDescendants.Text = "Select All Descendants";
			this.btnSelectDescendants.Click += new System.EventHandler(this.btnSelectDescendants_Click);
			// 
			// tvSceneNodes
			// 
			this.tvSceneNodes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tvSceneNodes.CheckBoxes = true;
			this.tvSceneNodes.HideSelection = false;
			this.tvSceneNodes.ImageIndex = 0;
			this.tvSceneNodes.ImageList = this.imglistNodes;
			this.tvSceneNodes.Location = new System.Drawing.Point(6,82);
			this.tvSceneNodes.Name = "tvSceneNodes";
			this.tvSceneNodes.SelectedImageIndex = 0;
			this.tvSceneNodes.ShowNodeToolTips = true;
			this.tvSceneNodes.Size = new System.Drawing.Size(306,291);
			this.tvSceneNodes.TabIndex = 10;
			this.tvSceneNodes.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.tvSceneNodes_AfterCheck);
			this.tvSceneNodes.BeforeCheck += new System.Windows.Forms.TreeViewCancelEventHandler(this.tvSceneNodes_BeforeCheck);
			// 
			// imglistNodes
			// 
			this.imglistNodes.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imglistNodes.ImageStream")));
			this.imglistNodes.TransparentColor = System.Drawing.Color.Transparent;
			this.imglistNodes.Images.SetKeyName(0,"dummy.png");
			this.imglistNodes.Images.SetKeyName(1,"geometry.png");
			this.imglistNodes.Images.SetKeyName(2,"material.png");
			this.imglistNodes.Images.SetKeyName(3,"modelW.png");
			// 
			// btnDeselectAll
			// 
			this.btnDeselectAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeselectAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeselectAll.Location = new System.Drawing.Point(6,53);
			this.btnDeselectAll.Name = "btnDeselectAll";
			this.btnDeselectAll.Size = new System.Drawing.Size(78,21);
			this.btnDeselectAll.TabIndex = 12;
			this.btnDeselectAll.Text = "Deselect All";
			this.btnDeselectAll.Click += new System.EventHandler(this.btnDeselectAll_Click);
			// 
			// btnSelectAll
			// 
			this.btnSelectAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnSelectAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnSelectAll.Location = new System.Drawing.Point(6,26);
			this.btnSelectAll.Name = "btnSelectAll";
			this.btnSelectAll.Size = new System.Drawing.Size(78,21);
			this.btnSelectAll.TabIndex = 11;
			this.btnSelectAll.Text = "Select All";
			this.btnSelectAll.Click += new System.EventHandler(this.btnSelectAll_Click);
			// 
			// AttachControllerDialog
			// 
			this.AcceptButton = this.btnAttach;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(652,452);
			this.Controls.Add(this.groupBox1);
			this.Name = "AttachControllerDialog";
			this.Text = "Attach Controller";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox5.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblType;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.GroupBox groupBox4;
		private DevComponents.DotNetBar.ButtonX btnCancel;
		private DevComponents.DotNetBar.ButtonX btnAttach;
		private System.Windows.Forms.RichTextBox rtxtDescription;
		private System.Windows.Forms.TreeView tvSceneNodes;
		private DevComponents.DotNetBar.ButtonX btnSelectDescendants;
		private DevComponents.DotNetBar.ButtonX btnDeselectAll;
		private DevComponents.DotNetBar.ButtonX btnSelectAll;
		private System.Windows.Forms.GroupBox groupBox5;
		private DevComponents.DotNetBar.ButtonX btnDeselectDescendants;
		private System.Windows.Forms.ImageList imglistNodes;
		public System.Windows.Forms.ListBox lbSelectedNodes;
		public System.Windows.Forms.TextBox txtName;
	}
}