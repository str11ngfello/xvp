namespace Dialogs
{
	partial class VerifyActorTemplateDialog
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(VerifyActorTemplateDialog));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnDoNotFix = new DevComponents.DotNetBar.ButtonX();
			this.lvActorComponents = new System.Windows.Forms.ListView();
			this.colName = new System.Windows.Forms.ColumnHeader();
			this.colPlugin = new System.Windows.Forms.ColumnHeader();
			this.colDescription = new System.Windows.Forms.ColumnHeader();
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.btnFinish = new DevComponents.DotNetBar.ButtonX();
			this.dlgOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.btnDoNotFix);
			this.groupBox1.Controls.Add(this.lvActorComponents);
			this.groupBox1.Controls.Add(this.richTextBox1);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.btnFinish);
			this.groupBox1.Location = new System.Drawing.Point(11,12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(523,364);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Verification Results";
			// 
			// btnDoNotFix
			// 
			this.btnDoNotFix.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDoNotFix.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnDoNotFix.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDoNotFix.Location = new System.Drawing.Point(378,337);
			this.btnDoNotFix.Name = "btnDoNotFix";
			this.btnDoNotFix.Size = new System.Drawing.Size(65,21);
			this.btnDoNotFix.TabIndex = 24;
			this.btnDoNotFix.Text = "Do Not Fix";
			this.btnDoNotFix.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// lvActorComponents
			// 
			this.lvActorComponents.CheckBoxes = true;
			this.lvActorComponents.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colName,
            this.colPlugin,
            this.colDescription});
			this.lvActorComponents.FullRowSelect = true;
			this.lvActorComponents.GridLines = true;
			this.lvActorComponents.Location = new System.Drawing.Point(10,65);
			this.lvActorComponents.Name = "lvActorComponents";
			this.lvActorComponents.Size = new System.Drawing.Size(504,266);
			this.lvActorComponents.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvActorComponents.TabIndex = 23;
			this.lvActorComponents.UseCompatibleStateImageBehavior = false;
			this.lvActorComponents.View = System.Windows.Forms.View.Details;
			// 
			// colName
			// 
			this.colName.Text = "Name";
			this.colName.Width = 143;
			// 
			// colPlugin
			// 
			this.colPlugin.Text = "Plugin";
			this.colPlugin.Width = 124;
			// 
			// colDescription
			// 
			this.colDescription.Text = "Description";
			this.colDescription.Width = 622;
			// 
			// richTextBox1
			// 
			this.richTextBox1.BackColor = System.Drawing.SystemColors.Control;
			this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.richTextBox1.Location = new System.Drawing.Point(10,19);
			this.richTextBox1.Name = "richTextBox1";
			this.richTextBox1.Size = new System.Drawing.Size(504,40);
			this.richTextBox1.TabIndex = 7;
			this.richTextBox1.Text = resources.GetString("richTextBox1.Text");
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
			this.btnFinish.Location = new System.Drawing.Point(449,337);
			this.btnFinish.Name = "btnFinish";
			this.btnFinish.Size = new System.Drawing.Size(65,21);
			this.btnFinish.TabIndex = 0;
			this.btnFinish.Text = "Fix";
			this.btnFinish.Click += new System.EventHandler(this.btnFinish_Click);
			// 
			// dlgOpenFileDialog
			// 
			this.dlgOpenFileDialog.FileName = "OpenFileDialog";
			// 
			// VerifyActorTemplateDialog
			// 
			this.AcceptButton = this.btnFinish;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(547,388);
			this.ControlBox = false;
			this.Controls.Add(this.groupBox1);
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(555,386);
			this.Name = "VerifyActorTemplateDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Actor Template Verification";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private DevComponents.DotNetBar.ButtonX btnFinish;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RichTextBox richTextBox1;
		private System.Windows.Forms.OpenFileDialog dlgOpenFileDialog;
		private System.Windows.Forms.ListView lvActorComponents;
		private System.Windows.Forms.ColumnHeader colName;
		private System.Windows.Forms.ColumnHeader colPlugin;
		private System.Windows.Forms.ColumnHeader colDescription;
		private DevComponents.DotNetBar.ButtonX btnDoNotFix;

	}
}