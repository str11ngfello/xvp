namespace Dialogs
{
	partial class LODGatherer
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LODGatherer));
			this.lvLODGatherer = new System.Windows.Forms.ListView();
			this.lodIndex = new System.Windows.Forms.ColumnHeader();
			this.lodFilename = new System.Windows.Forms.ColumnHeader();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnMoveUp = new DevComponents.DotNetBar.ButtonX();
			this.btnMoveDown = new DevComponents.DotNetBar.ButtonX();
			this.btnCancel = new DevComponents.DotNetBar.ButtonX();
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.btnRemoveAll = new DevComponents.DotNetBar.ButtonX();
			this.btnRemove = new DevComponents.DotNetBar.ButtonX();
			this.btnAdd = new DevComponents.DotNetBar.ButtonX();
			this.label1 = new System.Windows.Forms.Label();
			this.btnFinish = new DevComponents.DotNetBar.ButtonX();
			this.dlgOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.txtModelName = new System.Windows.Forms.TextBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// lvLODGatherer
			// 
			this.lvLODGatherer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lvLODGatherer.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.lodIndex,
            this.lodFilename});
			this.lvLODGatherer.FullRowSelect = true;
			this.lvLODGatherer.GridLines = true;
			this.lvLODGatherer.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvLODGatherer.Location = new System.Drawing.Point(21,142);
			this.lvLODGatherer.Name = "lvLODGatherer";
			this.lvLODGatherer.Size = new System.Drawing.Size(504,191);
			this.lvLODGatherer.TabIndex = 0;
			this.lvLODGatherer.UseCompatibleStateImageBehavior = false;
			this.lvLODGatherer.View = System.Windows.Forms.View.Details;
			// 
			// lodIndex
			// 
			this.lodIndex.Text = "LOD Index";
			this.lodIndex.Width = 72;
			// 
			// lodFilename
			// 
			this.lodFilename.Text = "LOD Filename";
			this.lodFilename.Width = 426;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.btnMoveUp);
			this.groupBox1.Controls.Add(this.btnMoveDown);
			this.groupBox1.Controls.Add(this.btnCancel);
			this.groupBox1.Controls.Add(this.richTextBox1);
			this.groupBox1.Controls.Add(this.btnRemoveAll);
			this.groupBox1.Controls.Add(this.btnRemove);
			this.groupBox1.Controls.Add(this.btnAdd);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.btnFinish);
			this.groupBox1.Location = new System.Drawing.Point(11,71);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(523,305);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Level of Detail Specification";
			// 
			// btnMoveUp
			// 
			this.btnMoveUp.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnMoveUp.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.btnMoveUp.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnMoveUp.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveUp.Image")));
			this.btnMoveUp.Location = new System.Drawing.Point(106,270);
			this.btnMoveUp.Name = "btnMoveUp";
			this.btnMoveUp.Size = new System.Drawing.Size(22,21);
			this.btnMoveUp.TabIndex = 10;
			// 
			// btnMoveDown
			// 
			this.btnMoveDown.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnMoveDown.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.btnMoveDown.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnMoveDown.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveDown.Image")));
			this.btnMoveDown.Location = new System.Drawing.Point(134,270);
			this.btnMoveDown.Name = "btnMoveDown";
			this.btnMoveDown.Size = new System.Drawing.Size(23,21);
			this.btnMoveDown.TabIndex = 9;
			// 
			// btnCancel
			// 
			this.btnCancel.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnCancel.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(10,270);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(60,21);
			this.btnCancel.TabIndex = 8;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// richTextBox1
			// 
			this.richTextBox1.BackColor = System.Drawing.SystemColors.Control;
			this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.richTextBox1.Location = new System.Drawing.Point(10,19);
			this.richTextBox1.Name = "richTextBox1";
			this.richTextBox1.Size = new System.Drawing.Size(504,55);
			this.richTextBox1.TabIndex = 7;
			this.richTextBox1.Text = resources.GetString("richTextBox1.Text");
			// 
			// btnRemoveAll
			// 
			this.btnRemoveAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRemoveAll.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.btnRemoveAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRemoveAll.Location = new System.Drawing.Point(192,270);
			this.btnRemoveAll.Name = "btnRemoveAll";
			this.btnRemoveAll.Size = new System.Drawing.Size(69,21);
			this.btnRemoveAll.TabIndex = 6;
			this.btnRemoveAll.Text = "Remove All";
			// 
			// btnRemove
			// 
			this.btnRemove.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRemove.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.btnRemove.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRemove.Location = new System.Drawing.Point(271,270);
			this.btnRemove.Name = "btnRemove";
			this.btnRemove.Size = new System.Drawing.Size(57,21);
			this.btnRemove.TabIndex = 3;
			this.btnRemove.Text = "Remove";
			// 
			// btnAdd
			// 
			this.btnAdd.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnAdd.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.btnAdd.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnAdd.Image = ((System.Drawing.Image)(resources.GetObject("btnAdd.Image")));
			this.btnAdd.Location = new System.Drawing.Point(387,270);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(53,21);
			this.btnAdd.TabIndex = 2;
			this.btnAdd.Text = "Add...";
			this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(7,30);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(0,13);
			this.label1.TabIndex = 1;
			// 
			// btnFinish
			// 
			this.btnFinish.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnFinish.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnFinish.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnFinish.Location = new System.Drawing.Point(449,270);
			this.btnFinish.Name = "btnFinish";
			this.btnFinish.Size = new System.Drawing.Size(65,21);
			this.btnFinish.TabIndex = 0;
			this.btnFinish.Text = "Finish";
			this.btnFinish.Click += new System.EventHandler(this.btnFinish_Click);
			// 
			// dlgOpenFileDialog
			// 
			this.dlgOpenFileDialog.FileName = "OpenFileDialog";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(7,30);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(0,13);
			this.label2.TabIndex = 1;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(10,22);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(70,13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Model Name:";
			// 
			// txtModelName
			// 
			this.txtModelName.Location = new System.Drawing.Point(85,20);
			this.txtModelName.Name = "txtModelName";
			this.txtModelName.Size = new System.Drawing.Size(217,20);
			this.txtModelName.TabIndex = 3;
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.txtModelName);
			this.groupBox2.Controls.Add(this.label3);
			this.groupBox2.Controls.Add(this.label2);
			this.groupBox2.Location = new System.Drawing.Point(12,12);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(522,53);
			this.groupBox2.TabIndex = 2;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "General";
			// 
			// LODGatherer
			// 
			this.AcceptButton = this.btnFinish;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(547,388);
			this.ControlBox = false;
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.lvLODGatherer);
			this.Controls.Add(this.groupBox1);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(555,386);
			this.Name = "LODGatherer";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Import Model With Level of Detail";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ColumnHeader lodIndex;
		private System.Windows.Forms.GroupBox groupBox1;
		private DevComponents.DotNetBar.ButtonX btnFinish;
		private DevComponents.DotNetBar.ButtonX btnAdd;
		private System.Windows.Forms.Label label1;
		private DevComponents.DotNetBar.ButtonX btnRemove;
		private DevComponents.DotNetBar.ButtonX btnRemoveAll;
		private System.Windows.Forms.RichTextBox richTextBox1;
		private System.Windows.Forms.ColumnHeader lodFilename;
		private System.Windows.Forms.OpenFileDialog dlgOpenFileDialog;
		public System.Windows.Forms.ListView lvLODGatherer;
		private DevComponents.DotNetBar.ButtonX btnCancel;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.GroupBox groupBox2;
		public System.Windows.Forms.TextBox txtModelName;
		private DevComponents.DotNetBar.ButtonX btnMoveUp;
		private DevComponents.DotNetBar.ButtonX btnMoveDown;

	}
}