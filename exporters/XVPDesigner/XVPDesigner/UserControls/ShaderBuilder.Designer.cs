namespace XVPDesigner.UserControls
{
    partial class ShaderBuilder
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ShaderBuilder));
			this.gbShaderCreation = new System.Windows.Forms.GroupBox();
			this.btnVerifyAll = new DevComponents.DotNetBar.ButtonX();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			this.labelX3 = new DevComponents.DotNetBar.LabelX();
			this.labelX5 = new DevComponents.DotNetBar.LabelX();
			this.btnGeneratePreview = new DevComponents.DotNetBar.ButtonX();
			this.btnCreateShader = new DevComponents.DotNetBar.ButtonX();
			this.lblLightingModelDesc = new DevComponents.DotNetBar.LabelX();
			this.labelX1 = new DevComponents.DotNetBar.LabelX();
			this.rtxtDescription = new System.Windows.Forms.RichTextBox();
			this.cbLightingModel = new DevComponents.DotNetBar.Controls.ComboBoxEx();
			this.lvShaderFeatures = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.cbObjectType = new DevComponents.DotNetBar.Controls.ComboBoxEx();
			this.labelX4 = new DevComponents.DotNetBar.LabelX();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.btnDeleteAll = new DevComponents.DotNetBar.ButtonX();
			this.btnRebuildShader = new DevComponents.DotNetBar.ButtonX();
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.btnViewSource = new DevComponents.DotNetBar.ButtonX();
			this.btnRebuildAllShaders = new DevComponents.DotNetBar.ButtonX();
			this.btnDeleteShader = new DevComponents.DotNetBar.ButtonX();
			this.lvShaderConfiguration = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
			this.lvAvailableShaders = new DevComponents.DotNetBar.Controls.ListViewEx();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.panel1 = new System.Windows.Forms.Panel();
			this.superTooltip1 = new DevComponents.DotNetBar.SuperTooltip();
			this.gbShaderCreation.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// gbShaderCreation
			// 
			this.gbShaderCreation.Controls.Add(this.btnVerifyAll);
			this.gbShaderCreation.Controls.Add(this.labelX2);
			this.gbShaderCreation.Controls.Add(this.labelX3);
			this.gbShaderCreation.Controls.Add(this.labelX5);
			this.gbShaderCreation.Controls.Add(this.btnGeneratePreview);
			this.gbShaderCreation.Controls.Add(this.btnCreateShader);
			this.gbShaderCreation.Controls.Add(this.lblLightingModelDesc);
			this.gbShaderCreation.Controls.Add(this.labelX1);
			this.gbShaderCreation.Controls.Add(this.rtxtDescription);
			this.gbShaderCreation.Controls.Add(this.cbLightingModel);
			this.gbShaderCreation.Controls.Add(this.lvShaderFeatures);
			this.gbShaderCreation.Controls.Add(this.cbObjectType);
			this.gbShaderCreation.Controls.Add(this.labelX4);
			this.gbShaderCreation.Dock = System.Windows.Forms.DockStyle.Top;
			this.gbShaderCreation.Location = new System.Drawing.Point(0,0);
			this.gbShaderCreation.Name = "gbShaderCreation";
			this.gbShaderCreation.Size = new System.Drawing.Size(770,384);
			this.gbShaderCreation.TabIndex = 18;
			this.gbShaderCreation.TabStop = false;
			this.gbShaderCreation.Text = "Shader Creation";
			// 
			// btnVerifyAll
			// 
			this.btnVerifyAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnVerifyAll.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnVerifyAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnVerifyAll.Image = ((System.Drawing.Image)(resources.GetObject("btnVerifyAll.Image")));
			this.btnVerifyAll.Location = new System.Drawing.Point(427,345);
			this.btnVerifyAll.Name = "btnVerifyAll";
			this.btnVerifyAll.Size = new System.Drawing.Size(83,23);
			this.superTooltip1.SetSuperTooltip(this.btnVerifyAll,new DevComponents.DotNetBar.SuperTooltipInfo("","",resources.GetString("btnVerifyAll.SuperTooltip"),null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnVerifyAll.TabIndex = 22;
			this.btnVerifyAll.Text = "Verify All";
			// 
			// labelX2
			// 
			this.labelX2.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.labelX2.Location = new System.Drawing.Point(16,70);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(136,15);
			this.labelX2.TabIndex = 12;
			this.labelX2.Text = "Object Type:";
			// 
			// labelX3
			// 
			this.labelX3.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.labelX3.Location = new System.Drawing.Point(221,70);
			this.labelX3.Name = "labelX3";
			this.labelX3.Size = new System.Drawing.Size(136,15);
			this.labelX3.TabIndex = 13;
			this.labelX3.Text = "Lighting Model:";
			// 
			// labelX5
			// 
			this.labelX5.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.labelX5.Location = new System.Drawing.Point(413,70);
			this.labelX5.Name = "labelX5";
			this.labelX5.Size = new System.Drawing.Size(136,15);
			this.labelX5.TabIndex = 20;
			this.labelX5.Text = "Description:";
			// 
			// btnGeneratePreview
			// 
			this.btnGeneratePreview.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnGeneratePreview.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnGeneratePreview.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnGeneratePreview.Enabled = false;
			this.btnGeneratePreview.Image = ((System.Drawing.Image)(resources.GetObject("btnGeneratePreview.Image")));
			this.btnGeneratePreview.Location = new System.Drawing.Point(516,345);
			this.btnGeneratePreview.Name = "btnGeneratePreview";
			this.btnGeneratePreview.Size = new System.Drawing.Size(120,23);
			this.superTooltip1.SetSuperTooltip(this.btnGeneratePreview,new DevComponents.DotNetBar.SuperTooltipInfo("","","Generate a preview of the shader with the selected features.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnGeneratePreview.TabIndex = 21;
			this.btnGeneratePreview.Text = "Generate Preview";
			// 
			// btnCreateShader
			// 
			this.btnCreateShader.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCreateShader.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnCreateShader.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCreateShader.Enabled = false;
			this.btnCreateShader.Image = ((System.Drawing.Image)(resources.GetObject("btnCreateShader.Image")));
			this.btnCreateShader.Location = new System.Drawing.Point(642,345);
			this.btnCreateShader.Name = "btnCreateShader";
			this.btnCreateShader.Size = new System.Drawing.Size(110,23);
			this.superTooltip1.SetSuperTooltip(this.btnCreateShader,new DevComponents.DotNetBar.SuperTooltipInfo("","","Creates the shader configuration with the selected features.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnCreateShader.TabIndex = 17;
			this.btnCreateShader.Text = "Create Shader...";
			this.btnCreateShader.Click += new System.EventHandler(this.btnCreateShader_Click);
			// 
			// lblLightingModelDesc
			// 
			this.lblLightingModelDesc.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblLightingModelDesc.Location = new System.Drawing.Point(413,91);
			this.lblLightingModelDesc.Name = "lblLightingModelDesc";
			this.lblLightingModelDesc.Size = new System.Drawing.Size(339,53);
			this.lblLightingModelDesc.TabIndex = 2;
			this.lblLightingModelDesc.TextLineAlignment = System.Drawing.StringAlignment.Near;
			this.lblLightingModelDesc.WordWrap = true;
			// 
			// labelX1
			// 
			this.labelX1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.labelX1.Location = new System.Drawing.Point(16,345);
			this.labelX1.Name = "labelX1";
			this.labelX1.SingleLineColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.labelX1.Size = new System.Drawing.Size(411,33);
			this.labelX1.TabIndex = 19;
			this.labelX1.Text = "<font color=\"#8C8C8C\">*Dependencies/exclusions are automatically checked/unchecke" +
    "d when necessary</font>\r\n";
			this.labelX1.TextLineAlignment = System.Drawing.StringAlignment.Near;
			this.labelX1.WordWrap = true;
			// 
			// rtxtDescription
			// 
			this.rtxtDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.rtxtDescription.BackColor = System.Drawing.SystemColors.Control;
			this.rtxtDescription.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.rtxtDescription.Location = new System.Drawing.Point(16,18);
			this.rtxtDescription.Name = "rtxtDescription";
			this.rtxtDescription.ReadOnly = true;
			this.rtxtDescription.Size = new System.Drawing.Size(743,46);
			this.rtxtDescription.TabIndex = 18;
			this.rtxtDescription.TabStop = false;
			this.rtxtDescription.Text = resources.GetString("rtxtDescription.Text");
			// 
			// cbLightingModel
			// 
			this.cbLightingModel.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.cbLightingModel.DisplayMember = "Text";
			this.cbLightingModel.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.cbLightingModel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbLightingModel.FormattingEnabled = true;
			this.cbLightingModel.ItemHeight = 14;
			this.cbLightingModel.Location = new System.Drawing.Point(221,91);
			this.cbLightingModel.MaxDropDownItems = 25;
			this.cbLightingModel.Name = "cbLightingModel";
			this.cbLightingModel.PreventEnterBeep = true;
			this.cbLightingModel.Size = new System.Drawing.Size(180,20);
			this.cbLightingModel.TabIndex = 7;
			this.cbLightingModel.SelectedIndexChanged += new System.EventHandler(this.cbLightingModel_SelectedIndexChanged);
			// 
			// lvShaderFeatures
			// 
			this.lvShaderFeatures.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			// 
			// 
			// 
			this.lvShaderFeatures.Border.Class = "ListViewBorder";
			this.lvShaderFeatures.CheckBoxes = true;
			this.lvShaderFeatures.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
			this.lvShaderFeatures.FullRowSelect = true;
			this.lvShaderFeatures.GridLines = true;
			this.lvShaderFeatures.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvShaderFeatures.Location = new System.Drawing.Point(16,153);
			this.lvShaderFeatures.MultiSelect = false;
			this.lvShaderFeatures.Name = "lvShaderFeatures";
			this.lvShaderFeatures.Size = new System.Drawing.Size(736,186);
			this.lvShaderFeatures.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvShaderFeatures.TabIndex = 5;
			this.lvShaderFeatures.UseCompatibleStateImageBehavior = false;
			this.lvShaderFeatures.View = System.Windows.Forms.View.Details;
			this.lvShaderFeatures.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.lvShaderFeatures_ItemChecked);
			this.lvShaderFeatures.SelectedIndexChanged += new System.EventHandler(this.lvShaderFeatures_SelectedIndexChanged);
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Feature Name";
			this.columnHeader3.Width = 225;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Feature Description";
			this.columnHeader4.Width = 2000;
			// 
			// cbObjectType
			// 
			this.cbObjectType.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.cbObjectType.DisplayMember = "Text";
			this.cbObjectType.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.cbObjectType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbObjectType.FormattingEnabled = true;
			this.cbObjectType.ItemHeight = 14;
			this.cbObjectType.Location = new System.Drawing.Point(16,92);
			this.cbObjectType.MaxDropDownItems = 25;
			this.cbObjectType.Name = "cbObjectType";
			this.cbObjectType.PreventEnterBeep = true;
			this.cbObjectType.Size = new System.Drawing.Size(190,20);
			this.cbObjectType.TabIndex = 6;
			this.cbObjectType.SelectedIndexChanged += new System.EventHandler(this.cbObjectType_SelectedIndexChanged);
			// 
			// labelX4
			// 
			this.labelX4.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.labelX4.Location = new System.Drawing.Point(16,133);
			this.labelX4.Name = "labelX4";
			this.labelX4.Size = new System.Drawing.Size(136,14);
			this.labelX4.TabIndex = 14;
			this.labelX4.Text = "Shader Features";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.btnDeleteAll);
			this.groupBox4.Controls.Add(this.btnRebuildShader);
			this.groupBox4.Controls.Add(this.richTextBox1);
			this.groupBox4.Controls.Add(this.btnViewSource);
			this.groupBox4.Controls.Add(this.btnRebuildAllShaders);
			this.groupBox4.Controls.Add(this.btnDeleteShader);
			this.groupBox4.Controls.Add(this.lvShaderConfiguration);
			this.groupBox4.Controls.Add(this.lvAvailableShaders);
			this.groupBox4.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox4.Location = new System.Drawing.Point(0,384);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(770,486);
			this.groupBox4.TabIndex = 19;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Shader Management";
			// 
			// btnDeleteAll
			// 
			this.btnDeleteAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeleteAll.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnDeleteAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeleteAll.Location = new System.Drawing.Point(516,455);
			this.btnDeleteAll.Name = "btnDeleteAll";
			this.btnDeleteAll.Size = new System.Drawing.Size(83,23);
			this.superTooltip1.SetSuperTooltip(this.btnDeleteAll,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete all shaders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDeleteAll.TabIndex = 27;
			this.btnDeleteAll.Text = "Delete All";
			this.btnDeleteAll.Click += new System.EventHandler(this.btnDeleteAll_Click);
			// 
			// btnRebuildShader
			// 
			this.btnRebuildShader.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRebuildShader.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnRebuildShader.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRebuildShader.Image = ((System.Drawing.Image)(resources.GetObject("btnRebuildShader.Image")));
			this.btnRebuildShader.Location = new System.Drawing.Point(150,455);
			this.btnRebuildShader.Name = "btnRebuildShader";
			this.btnRebuildShader.Size = new System.Drawing.Size(116,23);
			this.superTooltip1.SetSuperTooltip(this.btnRebuildShader,new DevComponents.DotNetBar.SuperTooltipInfo("","","Rebuild selected shaders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnRebuildShader.TabIndex = 26;
			this.btnRebuildShader.Text = "Rebuild Selected";
			this.btnRebuildShader.Click += new System.EventHandler(this.btnRebuildShader_Click);
			// 
			// richTextBox1
			// 
			this.richTextBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.richTextBox1.BackColor = System.Drawing.SystemColors.Control;
			this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.richTextBox1.Location = new System.Drawing.Point(13,19);
			this.richTextBox1.Name = "richTextBox1";
			this.richTextBox1.ReadOnly = true;
			this.richTextBox1.Size = new System.Drawing.Size(749,21);
			this.richTextBox1.TabIndex = 25;
			this.richTextBox1.TabStop = false;
			this.richTextBox1.Text = "The shader library is listed below.  Select shaders to view their current feature" +
    " set, rebuild, delete, view source, etc...";
			// 
			// btnViewSource
			// 
			this.btnViewSource.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnViewSource.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnViewSource.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnViewSource.Location = new System.Drawing.Point(659,455);
			this.btnViewSource.Name = "btnViewSource";
			this.btnViewSource.Size = new System.Drawing.Size(97,23);
			this.superTooltip1.SetSuperTooltip(this.btnViewSource,new DevComponents.DotNetBar.SuperTooltipInfo("","","View the final source code for the selected shader.",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnViewSource.TabIndex = 24;
			this.btnViewSource.Text = "View Source";
			this.btnViewSource.Click += new System.EventHandler(this.btnViewSource_Click);
			// 
			// btnRebuildAllShaders
			// 
			this.btnRebuildAllShaders.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRebuildAllShaders.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnRebuildAllShaders.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnRebuildAllShaders.Location = new System.Drawing.Point(272,455);
			this.btnRebuildAllShaders.Name = "btnRebuildAllShaders";
			this.btnRebuildAllShaders.Size = new System.Drawing.Size(85,23);
			this.superTooltip1.SetSuperTooltip(this.btnRebuildAllShaders,new DevComponents.DotNetBar.SuperTooltipInfo("","","Rebuild all shaders",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnRebuildAllShaders.TabIndex = 23;
			this.btnRebuildAllShaders.Text = "Rebuild All ";
			this.btnRebuildAllShaders.Click += new System.EventHandler(this.btnRebuildAllShaders_Click);
			// 
			// btnDeleteShader
			// 
			this.btnDeleteShader.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnDeleteShader.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.btnDeleteShader.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnDeleteShader.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteShader.Image")));
			this.btnDeleteShader.Location = new System.Drawing.Point(395,455);
			this.btnDeleteShader.Name = "btnDeleteShader";
			this.btnDeleteShader.Size = new System.Drawing.Size(115,23);
			this.superTooltip1.SetSuperTooltip(this.btnDeleteShader,new DevComponents.DotNetBar.SuperTooltipInfo("","","Delete selected shader",null,null,DevComponents.DotNetBar.eTooltipColor.Yellow));
			this.btnDeleteShader.TabIndex = 18;
			this.btnDeleteShader.Text = "Delete Selected";
			this.btnDeleteShader.Click += new System.EventHandler(this.btnDeleteShader_Click);
			// 
			// lvShaderConfiguration
			// 
			this.lvShaderConfiguration.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			// 
			// 
			// 
			this.lvShaderConfiguration.Border.Class = "ListViewBorder";
			this.lvShaderConfiguration.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader2,
            this.columnHeader6});
			this.lvShaderConfiguration.FullRowSelect = true;
			this.lvShaderConfiguration.GridLines = true;
			this.lvShaderConfiguration.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvShaderConfiguration.Location = new System.Drawing.Point(13,269);
			this.lvShaderConfiguration.MultiSelect = false;
			this.lvShaderConfiguration.Name = "lvShaderConfiguration";
			this.lvShaderConfiguration.Size = new System.Drawing.Size(742,180);
			this.lvShaderConfiguration.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvShaderConfiguration.TabIndex = 19;
			this.lvShaderConfiguration.UseCompatibleStateImageBehavior = false;
			this.lvShaderConfiguration.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Current Features of Selected Shader";
			this.columnHeader2.Width = 267;
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "Feature Description";
			this.columnHeader6.Width = 1500;
			// 
			// lvAvailableShaders
			// 
			this.lvAvailableShaders.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			// 
			// 
			// 
			this.lvAvailableShaders.Border.Class = "ListViewBorder";
			this.lvAvailableShaders.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader7,
            this.columnHeader5});
			this.lvAvailableShaders.FullRowSelect = true;
			this.lvAvailableShaders.GridLines = true;
			this.lvAvailableShaders.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvAvailableShaders.Location = new System.Drawing.Point(13,46);
			this.lvAvailableShaders.Name = "lvAvailableShaders";
			this.lvAvailableShaders.Size = new System.Drawing.Size(742,217);
			this.lvAvailableShaders.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvAvailableShaders.TabIndex = 6;
			this.lvAvailableShaders.UseCompatibleStateImageBehavior = false;
			this.lvAvailableShaders.View = System.Windows.Forms.View.Details;
			this.lvAvailableShaders.SelectedIndexChanged += new System.EventHandler(this.lvAvailableShaders_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Shader Name";
			this.columnHeader1.Width = 183;
			// 
			// columnHeader7
			// 
			this.columnHeader7.Text = "Shader Template";
			this.columnHeader7.Width = 181;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Shader Description";
			this.columnHeader5.Width = 1500;
			// 
			// panel1
			// 
			this.panel1.AutoScroll = true;
			this.panel1.AutoSize = true;
			this.panel1.Controls.Add(this.groupBox4);
			this.panel1.Controls.Add(this.gbShaderCreation);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(0,0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(787,749);
			this.panel1.TabIndex = 20;
			// 
			// superTooltip1
			// 
			this.superTooltip1.LicenseKey = "F962CEC7-CD8F-4911-A9E9-CAB39962FC1F";
			this.superTooltip1.MinimumTooltipSize = new System.Drawing.Size(0,0);
			// 
			// ShaderBuilder
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.panel1);
			this.Name = "ShaderBuilder";
			this.Size = new System.Drawing.Size(787,749);
			this.gbShaderCreation.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox gbShaderCreation;
		private DevComponents.DotNetBar.ButtonX btnCreateShader;
		private DevComponents.DotNetBar.LabelX lblLightingModelDesc;
        private DevComponents.DotNetBar.Controls.ComboBoxEx cbLightingModel;
        private DevComponents.DotNetBar.Controls.ListViewEx lvShaderFeatures;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private DevComponents.DotNetBar.Controls.ComboBoxEx cbObjectType;
        private DevComponents.DotNetBar.LabelX labelX2;
        private DevComponents.DotNetBar.LabelX labelX3;
        private DevComponents.DotNetBar.LabelX labelX4;
		private System.Windows.Forms.GroupBox groupBox4;
        private DevComponents.DotNetBar.Controls.ListViewEx lvShaderConfiguration;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private DevComponents.DotNetBar.ButtonX btnDeleteShader;
        private DevComponents.DotNetBar.Controls.ListViewEx lvAvailableShaders;
		private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.RichTextBox rtxtDescription;
		private DevComponents.DotNetBar.ButtonX btnRebuildAllShaders;
		private DevComponents.DotNetBar.ButtonX btnViewSource;
		private DevComponents.DotNetBar.LabelX labelX1;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private DevComponents.DotNetBar.LabelX labelX5;
		private System.Windows.Forms.RichTextBox richTextBox1;
		private DevComponents.DotNetBar.ButtonX btnRebuildShader;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private DevComponents.DotNetBar.ButtonX btnGeneratePreview;
		private DevComponents.DotNetBar.ButtonX btnVerifyAll;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private DevComponents.DotNetBar.ButtonX btnDeleteAll;
		private DevComponents.DotNetBar.SuperTooltip superTooltip1;
    }
}
