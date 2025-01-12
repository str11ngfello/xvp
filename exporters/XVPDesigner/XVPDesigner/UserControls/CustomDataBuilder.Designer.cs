namespace XVPDesigner.UserControls
{
	partial class CustomDataBuilder
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CustomDataBuilder));
			this.cbDataType = new System.Windows.Forms.ComboBox();
			this.txtDisplayName = new System.Windows.Forms.TextBox();
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			this.lvDataTypes = new System.Windows.Forms.ListView();
			this.colCategory = new System.Windows.Forms.ColumnHeader();
			this.colName = new System.Windows.Forms.ColumnHeader();
			this.colDataType = new System.Windows.Forms.ColumnHeader();
			this.colDefaultValue = new System.Windows.Forms.ColumnHeader();
			this.colMinValue = new System.Windows.Forms.ColumnHeader();
			this.colMaxValue = new System.Windows.Forms.ColumnHeader();
			this.colMinItemCount = new System.Windows.Forms.ColumnHeader();
			this.colMaxItemCount = new System.Windows.Forms.ColumnHeader();
			this.colDescription = new System.Windows.Forms.ColumnHeader();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.picImagePreview = new System.Windows.Forms.PictureBox();
			this.label9 = new System.Windows.Forms.Label();
			this.btnBrowseForImage = new DevComponents.DotNetBar.ButtonX();
			this.btnDeleteAll = new DevComponents.DotNetBar.ButtonX();
			this.txtImage = new System.Windows.Forms.TextBox();
			this.rbModify = new System.Windows.Forms.RadioButton();
			this.label8 = new System.Windows.Forms.Label();
			this.rbNew = new System.Windows.Forms.RadioButton();
			this.btnDeleteDataDefinition = new DevComponents.DotNetBar.ButtonX();
			this.cbDataDefinitions = new System.Windows.Forms.ComboBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.btnGenParsingCode = new DevComponents.DotNetBar.ButtonX();
			this.btnSave = new DevComponents.DotNetBar.ButtonX();
			this.btnRemoveDataType = new DevComponents.DotNetBar.ButtonX();
			this.btnClearDataTypes = new DevComponents.DotNetBar.ButtonX();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.txtDescription = new System.Windows.Forms.TextBox();
			this.txtMaxItemCount = new System.Windows.Forms.TextBox();
			this.lblMaxItemCount = new System.Windows.Forms.Label();
			this.txtMinItemCount = new System.Windows.Forms.TextBox();
			this.btnResetDataType = new DevComponents.DotNetBar.ButtonX();
			this.lblMinItemCount = new System.Windows.Forms.Label();
			this.lblDataTypeExamples = new System.Windows.Forms.Label();
			this.lblExamplesText = new System.Windows.Forms.Label();
			this.txtMaxValue = new System.Windows.Forms.TextBox();
			this.txtMinValue = new System.Windows.Forms.TextBox();
			this.txtDefaultValue = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.txtCategory = new System.Windows.Forms.TextBox();
			this.btnAddDataType = new DevComponents.DotNetBar.ButtonX();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.txtInstructions = new System.Windows.Forms.RichTextBox();
			this.errorProvider = new System.Windows.Forms.ErrorProvider(this.components);
			this.dlgSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.fileSystemWatcher = new System.IO.FileSystemWatcher();
			this.comboItem1 = new DevComponents.Editors.ComboItem();
			this.comboItem2 = new DevComponents.Editors.ComboItem();
			this.comboItem3 = new DevComponents.Editors.ComboItem();
			this.comboItem4 = new DevComponents.Editors.ComboItem();
			this.comboItem5 = new DevComponents.Editors.ComboItem();
			this.comboItem6 = new DevComponents.Editors.ComboItem();
			this.comboItem7 = new DevComponents.Editors.ComboItem();
			this.comboItem8 = new DevComponents.Editors.ComboItem();
			this.comboItem9 = new DevComponents.Editors.ComboItem();
			this.comboItem10 = new DevComponents.Editors.ComboItem();
			this.comboItem11 = new DevComponents.Editors.ComboItem();
			this.comboItem12 = new DevComponents.Editors.ComboItem();
			this.comboItem13 = new DevComponents.Editors.ComboItem();
			this.comboItem14 = new DevComponents.Editors.ComboItem();
			this.comboItem15 = new DevComponents.Editors.ComboItem();
			this.comboItem16 = new DevComponents.Editors.ComboItem();
			this.comboItem17 = new DevComponents.Editors.ComboItem();
			this.comboItem18 = new DevComponents.Editors.ComboItem();
			this.comboItem19 = new DevComponents.Editors.ComboItem();
			this.comboItem20 = new DevComponents.Editors.ComboItem();
			this.comboItem21 = new DevComponents.Editors.ComboItem();
			this.comboItem22 = new DevComponents.Editors.ComboItem();
			this.comboItem23 = new DevComponents.Editors.ComboItem();
			this.comboItem24 = new DevComponents.Editors.ComboItem();
			this.comboItem25 = new DevComponents.Editors.ComboItem();
			this.CustomNodeBuilderPanel = new System.Windows.Forms.Panel();
			this.dlgFileOpen = new System.Windows.Forms.OpenFileDialog();
			this.superTooltip1 = new DevComponents.DotNetBar.SuperTooltip();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.picImagePreview)).BeginInit();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.errorProvider)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher)).BeginInit();
			this.CustomNodeBuilderPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// cbDataType
			// 
			this.cbDataType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbDataType.FormattingEnabled = true;
			this.cbDataType.Items.AddRange(new object[] {
            "Decimal Number",
            "Decimal Number List",
            "Integer Number",
            "Integer Number List",
            "File Path",
            "File Path List",
            "RGB Color",
            "RGB Color List",
            "String",
            "String List",
            "Boolean",
            "Boolean List"});
			this.cbDataType.Location = new System.Drawing.Point(103,119);
			this.cbDataType.Name = "cbDataType";
			this.cbDataType.Size = new System.Drawing.Size(201,21);
			this.cbDataType.TabIndex = 2;
			this.cbDataType.SelectedIndexChanged += new System.EventHandler(this.cbDataType_SelectedIndexChanged);
			// 
			// txtDisplayName
			// 
			this.txtDisplayName.Location = new System.Drawing.Point(103,93);
			this.txtDisplayName.Name = "txtDisplayName";
			this.txtDisplayName.Size = new System.Drawing.Size(334,20);
			this.txtDisplayName.TabIndex = 1;
			this.txtDisplayName.TextChanged += new System.EventHandler(this.txtDisplayName_TextChanged);
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(8,17);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(649,60);
			this.rtxtDescription.TabIndex = 9;
			this.rtxtDescription.TabStop = false;
			this.rtxtDescription.Text = resources.GetString("rtxtDescription.Text");
			// 
			// lvDataTypes
			// 
			this.lvDataTypes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lvDataTypes.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colCategory,
            this.colName,
            this.colDataType,
            this.colDefaultValue,
            this.colMinValue,
            this.colMaxValue,
            this.colMinItemCount,
            this.colMaxItemCount,
            this.colDescription});
			this.lvDataTypes.FullRowSelect = true;
			this.lvDataTypes.GridLines = true;
			this.lvDataTypes.Location = new System.Drawing.Point(9,19);
			this.lvDataTypes.Name = "lvDataTypes";
			this.lvDataTypes.Size = new System.Drawing.Size(748,161);
			this.lvDataTypes.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvDataTypes.TabIndex = 21;
			this.lvDataTypes.UseCompatibleStateImageBehavior = false;
			this.lvDataTypes.View = System.Windows.Forms.View.Details;
			this.lvDataTypes.SelectedIndexChanged += new System.EventHandler(this.lvDataTypes_SelectedIndexChanged);
			// 
			// colCategory
			// 
			this.colCategory.Text = "Category";
			this.colCategory.Width = 130;
			// 
			// colName
			// 
			this.colName.Text = "Name";
			this.colName.Width = 130;
			// 
			// colDataType
			// 
			this.colDataType.Text = "Data Type";
			this.colDataType.Width = 130;
			// 
			// colDefaultValue
			// 
			this.colDefaultValue.Text = "Default Value";
			this.colDefaultValue.Width = 90;
			// 
			// colMinValue
			// 
			this.colMinValue.Text = "Min Value";
			this.colMinValue.Width = 80;
			// 
			// colMaxValue
			// 
			this.colMaxValue.Text = "Max Value";
			this.colMaxValue.Width = 80;
			// 
			// colMinItemCount
			// 
			this.colMinItemCount.Text = "Min Item Count";
			this.colMinItemCount.Width = 100;
			// 
			// colMaxItemCount
			// 
			this.colMaxItemCount.Text = "Max Item Count";
			this.colMaxItemCount.Width = 100;
			// 
			// colDescription
			// 
			this.colDescription.Text = "Description";
			this.colDescription.Width = 200;
			// 
			// groupBox1
			// 
			this.groupBox1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.groupBox1.Controls.Add(this.picImagePreview);
			this.groupBox1.Controls.Add(this.label9);
			this.groupBox1.Controls.Add(this.btnBrowseForImage);
			this.groupBox1.Controls.Add(this.btnDeleteAll);
			this.groupBox1.Controls.Add(this.txtImage);
			this.groupBox1.Controls.Add(this.rbModify);
			this.groupBox1.Controls.Add(this.label8);
			this.groupBox1.Controls.Add(this.rbNew);
			this.groupBox1.Controls.Add(this.btnDeleteDataDefinition);
			this.groupBox1.Controls.Add(this.cbDataDefinitions);
			this.groupBox1.Controls.Add(this.rtxtDescription);
			this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox1.Location = new System.Drawing.Point(0,0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(763,173);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Custom Data Definition";
			// 
			// picImagePreview
			// 
			this.picImagePreview.Location = new System.Drawing.Point(609,138);
			this.picImagePreview.Name = "picImagePreview";
			this.picImagePreview.Size = new System.Drawing.Size(16,16);
			this.picImagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.picImagePreview.TabIndex = 35;
			this.picImagePreview.TabStop = false;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(557,139);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(48,13);
			this.label9.TabIndex = 34;
			this.label9.Text = "Preview:";
			// 
			// btnBrowseForImage
			// 
			this.btnBrowseForImage.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnBrowseForImage.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnBrowseForImage.Location = new System.Drawing.Point(528,135);
			this.btnBrowseForImage.Name = "btnBrowseForImage";
			this.btnBrowseForImage.Size = new System.Drawing.Size(18,21);
			this.btnBrowseForImage.TabIndex = 12;
			this.btnBrowseForImage.Text = "...";
			this.btnBrowseForImage.Click += new System.EventHandler(this.btnBrowseForImage_Click);
			// 
			// btnDeleteAll
			// 
			this.btnDeleteAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeleteAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeleteAll.Enabled = false;
			this.btnDeleteAll.Location = new System.Drawing.Point(455,103);
			this.btnDeleteAll.Name = "btnDeleteAll";
			this.btnDeleteAll.Size = new System.Drawing.Size(67,21);
			this.superTooltip1.SetSuperTooltip(this.btnDeleteAll,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete all Custom Data Definitions",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDeleteAll.TabIndex = 4;
			this.btnDeleteAll.Text = "Delete All";
			this.btnDeleteAll.Click += new System.EventHandler(this.btnDeleteAll_Click);
			// 
			// txtImage
			// 
			this.txtImage.Location = new System.Drawing.Point(113,136);
			this.txtImage.Name = "txtImage";
			this.txtImage.Size = new System.Drawing.Size(409,20);
			this.txtImage.TabIndex = 11;
			// 
			// rbModify
			// 
			this.rbModify.AutoSize = true;
			this.rbModify.Location = new System.Drawing.Point(195,80);
			this.rbModify.Name = "rbModify";
			this.rbModify.Size = new System.Drawing.Size(142,17);
			this.rbModify.TabIndex = 1;
			this.rbModify.Text = "Modify Existing Definition";
			this.rbModify.UseVisualStyleBackColor = true;
			this.rbModify.CheckedChanged += new System.EventHandler(this.rbModify_CheckedChanged);
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(25,139);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(77,13);
			this.label8.TabIndex = 33;
			this.label8.Text = "Image (16x16):";
			// 
			// rbNew
			// 
			this.rbNew.AutoSize = true;
			this.rbNew.Checked = true;
			this.rbNew.Location = new System.Drawing.Point(23,80);
			this.rbNew.Name = "rbNew";
			this.rbNew.Size = new System.Drawing.Size(128,17);
			this.rbNew.TabIndex = 0;
			this.rbNew.TabStop = true;
			this.rbNew.Text = "Create New Definition";
			this.rbNew.UseVisualStyleBackColor = true;
			this.rbNew.CheckedChanged += new System.EventHandler(this.rbNew_CheckedChanged);
			// 
			// btnDeleteDataDefinition
			// 
			this.btnDeleteDataDefinition.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeleteDataDefinition.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeleteDataDefinition.Enabled = false;
			this.btnDeleteDataDefinition.Location = new System.Drawing.Point(405,103);
			this.btnDeleteDataDefinition.Name = "btnDeleteDataDefinition";
			this.btnDeleteDataDefinition.Size = new System.Drawing.Size(44,21);
			this.superTooltip1.SetSuperTooltip(this.btnDeleteDataDefinition,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete selected Custom Data Definition",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDeleteDataDefinition.TabIndex = 3;
			this.btnDeleteDataDefinition.Text = "Delete";
			this.btnDeleteDataDefinition.Click += new System.EventHandler(this.btnDeleteDataDefinition_Click);
			// 
			// cbDataDefinitions
			// 
			this.cbDataDefinitions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbDataDefinitions.Enabled = false;
			this.cbDataDefinitions.FormattingEnabled = true;
			this.cbDataDefinitions.ItemHeight = 13;
			this.cbDataDefinitions.Location = new System.Drawing.Point(195,103);
			this.cbDataDefinitions.Name = "cbDataDefinitions";
			this.cbDataDefinitions.Size = new System.Drawing.Size(204,21);
			this.cbDataDefinitions.TabIndex = 2;
			this.cbDataDefinitions.SelectedIndexChanged += new System.EventHandler(this.cbDataDefinitions_SelectedIndexChanged);
			// 
			// groupBox2
			// 
			this.groupBox2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.groupBox2.Controls.Add(this.btnGenParsingCode);
			this.groupBox2.Controls.Add(this.btnSave);
			this.groupBox2.Controls.Add(this.btnRemoveDataType);
			this.groupBox2.Controls.Add(this.btnClearDataTypes);
			this.groupBox2.Controls.Add(this.lvDataTypes);
			this.groupBox2.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox2.Location = new System.Drawing.Point(0,455);
			this.groupBox2.MinimumSize = new System.Drawing.Size(0,100);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(763,214);
			this.groupBox2.TabIndex = 20;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Current Data Types";
			// 
			// btnGenParsingCode
			// 
			this.btnGenParsingCode.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnGenParsingCode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnGenParsingCode.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnGenParsingCode.Image = ((System.Drawing.Image)(resources.GetObject("btnGenParsingCode.Image")));
			this.btnGenParsingCode.Location = new System.Drawing.Point(543,186);
			this.btnGenParsingCode.Name = "btnGenParsingCode";
			this.btnGenParsingCode.Size = new System.Drawing.Size(143,21);
			this.superTooltip1.SetSuperTooltip(this.btnGenParsingCode,new DevComponents.DotNetBar.SuperTooltipInfo("","","Generate parsing code snippet",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnGenParsingCode.TabIndex = 2;
			this.btnGenParsingCode.Text = "Generate Parsing Code";
			// 
			// btnSave
			// 
			this.btnSave.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnSave.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnSave.Image = ((System.Drawing.Image)(resources.GetObject("btnSave.Image")));
			this.btnSave.Location = new System.Drawing.Point(692,186);
			this.btnSave.Name = "btnSave";
			this.btnSave.Size = new System.Drawing.Size(65,21);
			this.superTooltip1.SetSuperTooltip(this.btnSave,new DevComponents.DotNetBar.SuperTooltipInfo("","","Save Custom Data Definition",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnSave.TabIndex = 3;
			this.btnSave.Text = "Save";
			this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
			// 
			// btnRemoveDataType
			// 
			this.btnRemoveDataType.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRemoveDataType.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnRemoveDataType.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRemoveDataType.Image = ((System.Drawing.Image)(resources.GetObject("btnRemoveDataType.Image")));
			this.btnRemoveDataType.Location = new System.Drawing.Point(9,186);
			this.btnRemoveDataType.Name = "btnRemoveDataType";
			this.btnRemoveDataType.Size = new System.Drawing.Size(122,21);
			this.superTooltip1.SetSuperTooltip(this.btnRemoveDataType,new DevComponents.DotNetBar.SuperTooltipInfo("","","Remove selected data type",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnRemoveDataType.TabIndex = 0;
			this.btnRemoveDataType.Text = "Remove Selected";
			this.btnRemoveDataType.Click += new System.EventHandler(this.btnRemoveDataType_Click);
			// 
			// btnClearDataTypes
			// 
			this.btnClearDataTypes.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnClearDataTypes.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnClearDataTypes.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnClearDataTypes.Location = new System.Drawing.Point(139,186);
			this.btnClearDataTypes.Name = "btnClearDataTypes";
			this.btnClearDataTypes.Size = new System.Drawing.Size(67,21);
			this.superTooltip1.SetSuperTooltip(this.btnClearDataTypes,new DevComponents.DotNetBar.SuperTooltipInfo("","","Remove all data types",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnClearDataTypes.TabIndex = 1;
			this.btnClearDataTypes.Text = "Remove All";
			this.btnClearDataTypes.Click += new System.EventHandler(this.btnClearDataTypes_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.groupBox3.Controls.Add(this.txtDescription);
			this.groupBox3.Controls.Add(this.txtMaxItemCount);
			this.groupBox3.Controls.Add(this.lblMaxItemCount);
			this.groupBox3.Controls.Add(this.txtMinItemCount);
			this.groupBox3.Controls.Add(this.btnResetDataType);
			this.groupBox3.Controls.Add(this.lblMinItemCount);
			this.groupBox3.Controls.Add(this.lblDataTypeExamples);
			this.groupBox3.Controls.Add(this.lblExamplesText);
			this.groupBox3.Controls.Add(this.txtMaxValue);
			this.groupBox3.Controls.Add(this.txtMinValue);
			this.groupBox3.Controls.Add(this.txtDefaultValue);
			this.groupBox3.Controls.Add(this.label7);
			this.groupBox3.Controls.Add(this.label6);
			this.groupBox3.Controls.Add(this.label5);
			this.groupBox3.Controls.Add(this.label4);
			this.groupBox3.Controls.Add(this.txtCategory);
			this.groupBox3.Controls.Add(this.btnAddDataType);
			this.groupBox3.Controls.Add(this.label3);
			this.groupBox3.Controls.Add(this.label2);
			this.groupBox3.Controls.Add(this.label1);
			this.groupBox3.Controls.Add(this.txtInstructions);
			this.groupBox3.Controls.Add(this.cbDataType);
			this.groupBox3.Controls.Add(this.txtDisplayName);
			this.groupBox3.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox3.Location = new System.Drawing.Point(0,173);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(763,282);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Data Type Editor";
			// 
			// txtDescription
			// 
			this.txtDescription.Location = new System.Drawing.Point(103,222);
			this.txtDescription.Name = "txtDescription";
			this.txtDescription.Size = new System.Drawing.Size(438,20);
			this.txtDescription.TabIndex = 10;
			this.txtDescription.TextChanged += new System.EventHandler(this.txtDescription_TextChanged);
			// 
			// txtMaxItemCount
			// 
			this.txtMaxItemCount.Location = new System.Drawing.Point(334,196);
			this.txtMaxItemCount.Name = "txtMaxItemCount";
			this.txtMaxItemCount.Size = new System.Drawing.Size(86,20);
			this.txtMaxItemCount.TabIndex = 9;
			this.txtMaxItemCount.TextChanged += new System.EventHandler(this.txtMaxItemCount_TextChanged);
			// 
			// lblMaxItemCount
			// 
			this.lblMaxItemCount.AutoSize = true;
			this.lblMaxItemCount.Location = new System.Drawing.Point(247,199);
			this.lblMaxItemCount.Name = "lblMaxItemCount";
			this.lblMaxItemCount.Size = new System.Drawing.Size(81,13);
			this.lblMaxItemCount.TabIndex = 31;
			this.lblMaxItemCount.Text = "Max Item Count";
			// 
			// txtMinItemCount
			// 
			this.txtMinItemCount.Location = new System.Drawing.Point(334,171);
			this.txtMinItemCount.Name = "txtMinItemCount";
			this.txtMinItemCount.Size = new System.Drawing.Size(86,20);
			this.txtMinItemCount.TabIndex = 8;
			this.txtMinItemCount.TextChanged += new System.EventHandler(this.txtMinItemCount_TextChanged);
			// 
			// btnResetDataType
			// 
			this.btnResetDataType.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnResetDataType.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnResetDataType.Location = new System.Drawing.Point(103,248);
			this.btnResetDataType.Name = "btnResetDataType";
			this.btnResetDataType.Size = new System.Drawing.Size(48,21);
			this.superTooltip1.SetSuperTooltip(this.btnResetDataType,new DevComponents.DotNetBar.SuperTooltipInfo("","","Reset data fields",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnResetDataType.TabIndex = 13;
			this.btnResetDataType.Text = "Reset";
			this.btnResetDataType.Click += new System.EventHandler(this.btnResetDataType_Click);
			// 
			// lblMinItemCount
			// 
			this.lblMinItemCount.AutoSize = true;
			this.lblMinItemCount.Location = new System.Drawing.Point(247,174);
			this.lblMinItemCount.Name = "lblMinItemCount";
			this.lblMinItemCount.Size = new System.Drawing.Size(81,13);
			this.lblMinItemCount.TabIndex = 29;
			this.lblMinItemCount.Text = "Min Item Count:";
			// 
			// lblDataTypeExamples
			// 
			this.lblDataTypeExamples.AutoSize = true;
			this.lblDataTypeExamples.Location = new System.Drawing.Point(367,123);
			this.lblDataTypeExamples.Name = "lblDataTypeExamples";
			this.lblDataTypeExamples.Size = new System.Drawing.Size(0,13);
			this.lblDataTypeExamples.TabIndex = 4;
			// 
			// lblExamplesText
			// 
			this.lblExamplesText.AutoSize = true;
			this.lblExamplesText.Location = new System.Drawing.Point(309,123);
			this.lblExamplesText.Name = "lblExamplesText";
			this.lblExamplesText.Size = new System.Drawing.Size(50,13);
			this.lblExamplesText.TabIndex = 3;
			this.lblExamplesText.Text = "Example:";
			// 
			// txtMaxValue
			// 
			this.txtMaxValue.Location = new System.Drawing.Point(103,196);
			this.txtMaxValue.Name = "txtMaxValue";
			this.txtMaxValue.Size = new System.Drawing.Size(103,20);
			this.txtMaxValue.TabIndex = 7;
			this.txtMaxValue.TextChanged += new System.EventHandler(this.txtMaxValue_TextChanged);
			// 
			// txtMinValue
			// 
			this.txtMinValue.Location = new System.Drawing.Point(103,171);
			this.txtMinValue.Name = "txtMinValue";
			this.txtMinValue.Size = new System.Drawing.Size(103,20);
			this.txtMinValue.TabIndex = 6;
			this.txtMinValue.TextChanged += new System.EventHandler(this.txtMinValue_TextChanged);
			// 
			// txtDefaultValue
			// 
			this.txtDefaultValue.Location = new System.Drawing.Point(103,146);
			this.txtDefaultValue.Name = "txtDefaultValue";
			this.txtDefaultValue.Size = new System.Drawing.Size(201,20);
			this.txtDefaultValue.TabIndex = 5;
			this.txtDefaultValue.TextChanged += new System.EventHandler(this.txtDefaultValue_TextChanged);
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(39,199);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(60,13);
			this.label7.TabIndex = 21;
			this.label7.Text = "Max Value:";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(42,174);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(57,13);
			this.label6.TabIndex = 20;
			this.label6.Text = "Min Value:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(25,149);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(74,13);
			this.label5.TabIndex = 19;
			this.label5.Text = "Default Value:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(45,70);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(52,13);
			this.label4.TabIndex = 0;
			this.label4.Text = "Category:";
			// 
			// txtCategory
			// 
			this.txtCategory.Location = new System.Drawing.Point(103,67);
			this.txtCategory.Name = "txtCategory";
			this.txtCategory.Size = new System.Drawing.Size(334,20);
			this.txtCategory.TabIndex = 0;
			this.txtCategory.TextChanged += new System.EventHandler(this.txtCategory_TextChanged);
			// 
			// btnAddDataType
			// 
			this.btnAddDataType.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnAddDataType.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnAddDataType.Image = ((System.Drawing.Image)(resources.GetObject("btnAddDataType.Image")));
			this.btnAddDataType.Location = new System.Drawing.Point(444,252);
			this.btnAddDataType.Name = "btnAddDataType";
			this.btnAddDataType.Size = new System.Drawing.Size(97,21);
			this.superTooltip1.SetSuperTooltip(this.btnAddDataType,new DevComponents.DotNetBar.SuperTooltipInfo("","","Add new or modify existsing data type.  The category and name make a unique key",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnAddDataType.TabIndex = 14;
			this.btnAddDataType.Text = "Add / Modify";
			this.btnAddDataType.Click += new System.EventHandler(this.btnAddDataType_Click);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(34,224);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(63,13);
			this.label3.TabIndex = 15;
			this.label3.Text = "Description:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(59,96);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(38,13);
			this.label2.TabIndex = 14;
			this.label2.Text = "Name:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(37,122);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(60,13);
			this.label1.TabIndex = 13;
			this.label1.Text = "Data Type:";
			// 
			// txtInstructions
			// 
			this.txtInstructions.BackColor = System.Drawing.SystemColors.Control;
			this.txtInstructions.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txtInstructions.Location = new System.Drawing.Point(9,17);
			this.txtInstructions.Name = "txtInstructions";
			this.txtInstructions.ReadOnly = true;
			this.txtInstructions.Size = new System.Drawing.Size(659,44);
			this.txtInstructions.TabIndex = 9;
			this.txtInstructions.TabStop = false;
			this.txtInstructions.Text = resources.GetString("txtInstructions.Text");
			// 
			// errorProvider
			// 
			this.errorProvider.ContainerControl = this;
			// 
			// fileSystemWatcher
			// 
			this.fileSystemWatcher.EnableRaisingEvents = true;
			this.fileSystemWatcher.NotifyFilter = System.IO.NotifyFilters.FileName;
			this.fileSystemWatcher.SynchronizingObject = this;
			this.fileSystemWatcher.Renamed += new System.IO.RenamedEventHandler(this.fileSystemWatcher_Renamed);
			this.fileSystemWatcher.Deleted += new System.IO.FileSystemEventHandler(this.fileSystemWatcher_Deleted);
			this.fileSystemWatcher.Created += new System.IO.FileSystemEventHandler(this.fileSystemWatcher_Created);
			// 
			// comboItem1
			// 
			this.comboItem1.Text = "comboItem1";
			// 
			// comboItem2
			// 
			this.comboItem2.Text = "comboItem2";
			// 
			// comboItem3
			// 
			this.comboItem3.Text = "comboItem3";
			// 
			// comboItem4
			// 
			this.comboItem4.Text = "comboItem4";
			// 
			// comboItem5
			// 
			this.comboItem5.Text = "comboItem5";
			// 
			// comboItem6
			// 
			this.comboItem6.Text = "comboItem6";
			// 
			// comboItem7
			// 
			this.comboItem7.Text = "comboItem7";
			// 
			// comboItem8
			// 
			this.comboItem8.Text = "comboItem8";
			// 
			// comboItem9
			// 
			this.comboItem9.Text = "comboItem9";
			// 
			// comboItem10
			// 
			this.comboItem10.Text = "comboItem10";
			// 
			// comboItem11
			// 
			this.comboItem11.Text = "comboItem11";
			// 
			// comboItem12
			// 
			this.comboItem12.Text = "comboItem12";
			// 
			// comboItem13
			// 
			this.comboItem13.Text = "comboItem13";
			// 
			// comboItem14
			// 
			this.comboItem14.Text = "comboItem14";
			// 
			// comboItem15
			// 
			this.comboItem15.Text = "comboItem15";
			// 
			// comboItem16
			// 
			this.comboItem16.Text = "comboItem16";
			// 
			// comboItem17
			// 
			this.comboItem17.Text = "comboItem17";
			// 
			// comboItem18
			// 
			this.comboItem18.Text = "comboItem18";
			// 
			// comboItem19
			// 
			this.comboItem19.Text = "comboItem19";
			// 
			// comboItem20
			// 
			this.comboItem20.Text = "comboItem20";
			// 
			// comboItem21
			// 
			this.comboItem21.Text = "comboItem21";
			// 
			// comboItem22
			// 
			this.comboItem22.Text = "comboItem22";
			// 
			// comboItem23
			// 
			this.comboItem23.Text = "comboItem23";
			// 
			// comboItem24
			// 
			this.comboItem24.Text = "comboItem24";
			// 
			// comboItem25
			// 
			this.comboItem25.Text = "comboItem25";
			// 
			// CustomNodeBuilderPanel
			// 
			this.CustomNodeBuilderPanel.AutoScroll = true;
			this.CustomNodeBuilderPanel.AutoSize = true;
			this.CustomNodeBuilderPanel.Controls.Add(this.groupBox2);
			this.CustomNodeBuilderPanel.Controls.Add(this.groupBox3);
			this.CustomNodeBuilderPanel.Controls.Add(this.groupBox1);
			this.CustomNodeBuilderPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.CustomNodeBuilderPanel.Location = new System.Drawing.Point(0,0);
			this.CustomNodeBuilderPanel.Name = "CustomNodeBuilderPanel";
			this.CustomNodeBuilderPanel.Size = new System.Drawing.Size(780,627);
			this.CustomNodeBuilderPanel.TabIndex = 13;
			// 
			// dlgFileOpen
			// 
			this.dlgFileOpen.FileName = "openFileDialog1";
			// 
			// superTooltip1
			// 
			this.superTooltip1.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			this.superTooltip1.MinimumTooltipSize = new System.Drawing.Size(0,0);
			// 
			// CustomDataBuilder
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoScroll = true;
			this.Controls.Add(this.CustomNodeBuilderPanel);
			this.Name = "CustomDataBuilder";
			this.Size = new System.Drawing.Size(780,627);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.picImagePreview)).EndInit();
			this.groupBox2.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.errorProvider)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher)).EndInit();
			this.CustomNodeBuilderPanel.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox cbDataType;
		private System.Windows.Forms.TextBox txtDisplayName;
		private System.Windows.Forms.RichTextBox rtxtDescription;
		private System.Windows.Forms.ListView lvDataTypes;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private DevComponents.DotNetBar.ButtonX btnRemoveDataType;
		private DevComponents.DotNetBar.ButtonX btnClearDataTypes;
		private System.Windows.Forms.ComboBox cbDataDefinitions;
		private DevComponents.DotNetBar.ButtonX btnDeleteDataDefinition;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RichTextBox txtInstructions;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ColumnHeader colName;
		private System.Windows.Forms.ColumnHeader colDataType;
		private System.Windows.Forms.ColumnHeader colDescription;
		private DevComponents.DotNetBar.ButtonX btnAddDataType;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox txtCategory;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox txtMaxValue;
		private System.Windows.Forms.TextBox txtMinValue;
		private System.Windows.Forms.TextBox txtDefaultValue;
		private DevComponents.DotNetBar.ButtonX btnSave;
		private System.Windows.Forms.Label lblExamplesText;
		private System.Windows.Forms.ColumnHeader colCategory;
		private System.Windows.Forms.ColumnHeader colDefaultValue;
		private System.Windows.Forms.ColumnHeader colMinValue;
		private System.Windows.Forms.ColumnHeader colMaxValue;
		private System.Windows.Forms.Label lblDataTypeExamples;
		private System.Windows.Forms.ErrorProvider errorProvider;
		private System.Windows.Forms.TextBox txtMaxItemCount;
		private System.Windows.Forms.Label lblMaxItemCount;
		private System.Windows.Forms.TextBox txtMinItemCount;
		private System.Windows.Forms.Label lblMinItemCount;
		private System.Windows.Forms.ColumnHeader colMinItemCount;
		private System.Windows.Forms.ColumnHeader colMaxItemCount;
		private System.Windows.Forms.RadioButton rbModify;
		private System.Windows.Forms.RadioButton rbNew;
		private DevComponents.DotNetBar.ButtonX btnDeleteAll;
		private DevComponents.DotNetBar.ButtonX btnResetDataType;
		private DevComponents.DotNetBar.ButtonX btnGenParsingCode;
		private System.Windows.Forms.SaveFileDialog dlgSaveFileDialog;
		private System.IO.FileSystemWatcher fileSystemWatcher;
        private System.Windows.Forms.TextBox txtDescription;
        private DevComponents.Editors.ComboItem comboItem1;
        private DevComponents.Editors.ComboItem comboItem2;
        private DevComponents.Editors.ComboItem comboItem3;
        private DevComponents.Editors.ComboItem comboItem4;
        private DevComponents.Editors.ComboItem comboItem5;
        private DevComponents.Editors.ComboItem comboItem6;
        private DevComponents.Editors.ComboItem comboItem7;
        private DevComponents.Editors.ComboItem comboItem8;
        private DevComponents.Editors.ComboItem comboItem9;
        private DevComponents.Editors.ComboItem comboItem10;
        private DevComponents.Editors.ComboItem comboItem11;
        private DevComponents.Editors.ComboItem comboItem12;
        private DevComponents.Editors.ComboItem comboItem13;
        private DevComponents.Editors.ComboItem comboItem14;
        private DevComponents.Editors.ComboItem comboItem15;
        private DevComponents.Editors.ComboItem comboItem16;
        private DevComponents.Editors.ComboItem comboItem17;
        private DevComponents.Editors.ComboItem comboItem18;
        private DevComponents.Editors.ComboItem comboItem19;
        private DevComponents.Editors.ComboItem comboItem20;
        private DevComponents.Editors.ComboItem comboItem21;
        private DevComponents.Editors.ComboItem comboItem22;
        private DevComponents.Editors.ComboItem comboItem23;
        private DevComponents.Editors.ComboItem comboItem24;
        private DevComponents.Editors.ComboItem comboItem25;
		private System.Windows.Forms.TextBox txtImage;
		private System.Windows.Forms.Label label8;
		private DevComponents.DotNetBar.ButtonX btnBrowseForImage;
		private System.Windows.Forms.Panel CustomNodeBuilderPanel;
		private System.Windows.Forms.OpenFileDialog dlgFileOpen;
		private System.Windows.Forms.PictureBox picImagePreview;
		private System.Windows.Forms.Label label9;
		private DevComponents.DotNetBar.SuperTooltip superTooltip1;

	}
}
