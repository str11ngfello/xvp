namespace XVPDesigner.Dialogs
{
	partial class CreateShaderDialog
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
			this.btnOK = new DevComponents.DotNetBar.ButtonX();
			this.btnCancel = new DevComponents.DotNetBar.ButtonX();
			this.labelX1 = new DevComponents.DotNetBar.LabelX();
			this.txtShaderName = new DevComponents.DotNetBar.Controls.TextBoxX();
			this.txtShaderDescription = new DevComponents.DotNetBar.Controls.TextBoxX();
			this.labelX2 = new DevComponents.DotNetBar.LabelX();
			this.labelX3 = new DevComponents.DotNetBar.LabelX();
			this.labelX4 = new DevComponents.DotNetBar.LabelX();
			this.labelX5 = new DevComponents.DotNetBar.LabelX();
			this.chkViewSource = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// btnOK
			// 
			this.btnOK.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnOK.Location = new System.Drawing.Point(418,208);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(71,21);
			this.btnOK.TabIndex = 4;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCancel.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(341,208);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(71,21);
			this.btnCancel.TabIndex = 3;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// labelX1
			// 
			this.labelX1.Location = new System.Drawing.Point(12,7);
			this.labelX1.Name = "labelX1";
			this.labelX1.Size = new System.Drawing.Size(479,44);
			this.labelX1.TabIndex = 17;
			this.labelX1.Text = "Name the shader and give it a description.  Be as descriptive as possible for bot" +
    "h the name and the description.  These are used later on to identify the shader " +
    "throughout various tools.";
			this.labelX1.WordWrap = true;
			// 
			// txtShaderName
			// 
			// 
			// 
			// 
			this.txtShaderName.Border.Class = "TextBoxBorder";
			this.txtShaderName.Location = new System.Drawing.Point(116,64);
			this.txtShaderName.MaxLength = 128;
			this.txtShaderName.Name = "txtShaderName";
			this.txtShaderName.Size = new System.Drawing.Size(375,20);
			this.txtShaderName.TabIndex = 0;
			// 
			// txtShaderDescription
			// 
			// 
			// 
			// 
			this.txtShaderDescription.Border.Class = "TextBoxBorder";
			this.txtShaderDescription.Location = new System.Drawing.Point(116,114);
			this.txtShaderDescription.Multiline = true;
			this.txtShaderDescription.Name = "txtShaderDescription";
			this.txtShaderDescription.Size = new System.Drawing.Size(375,53);
			this.txtShaderDescription.TabIndex = 1;
			// 
			// labelX2
			// 
			this.labelX2.Location = new System.Drawing.Point(41,62);
			this.labelX2.Name = "labelX2";
			this.labelX2.Size = new System.Drawing.Size(79,23);
			this.labelX2.TabIndex = 20;
			this.labelX2.Text = "Shader Name:";
			// 
			// labelX3
			// 
			this.labelX3.Location = new System.Drawing.Point(12,112);
			this.labelX3.Name = "labelX3";
			this.labelX3.Size = new System.Drawing.Size(108,22);
			this.labelX3.TabIndex = 21;
			this.labelX3.Text = "Shader Description:";
			this.labelX3.WordWrap = true;
			// 
			// labelX4
			// 
			this.labelX4.AutoSize = true;
			this.labelX4.ForeColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.labelX4.Location = new System.Drawing.Point(116,86);
			this.labelX4.Name = "labelX4";
			this.labelX4.Size = new System.Drawing.Size(120,13);
			this.labelX4.TabIndex = 22;
			this.labelX4.Text = "*Example: Monster Skin";
			this.labelX4.WordWrap = true;
			// 
			// labelX5
			// 
			this.labelX5.ForeColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.labelX5.Location = new System.Drawing.Point(116,169);
			this.labelX5.Name = "labelX5";
			this.labelX5.Size = new System.Drawing.Size(343,29);
			this.labelX5.TabIndex = 23;
			this.labelX5.Text = "*Example: Shader for green orcs on level 3.  Uses normal mapping for bumpy skin a" +
    "nd includes motion blur.";
			this.labelX5.WordWrap = true;
			// 
			// chkViewSource
			// 
			this.chkViewSource.AutoSize = true;
			this.chkViewSource.Location = new System.Drawing.Point(116,211);
			this.chkViewSource.Name = "chkViewSource";
			this.chkViewSource.Size = new System.Drawing.Size(194,17);
			this.chkViewSource.TabIndex = 2;
			this.chkViewSource.Text = "View shader source when complete";
			this.chkViewSource.UseVisualStyleBackColor = true;
			// 
			// CreateShaderDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(500,236);
			this.ControlBox = false;
			this.Controls.Add(this.chkViewSource);
			this.Controls.Add(this.txtShaderDescription);
			this.Controls.Add(this.labelX5);
			this.Controls.Add(this.labelX4);
			this.Controls.Add(this.txtShaderName);
			this.Controls.Add(this.labelX3);
			this.Controls.Add(this.labelX2);
			this.Controls.Add(this.labelX1);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.btnCancel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "CreateShaderDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Shader Name and Description...";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private DevComponents.DotNetBar.ButtonX btnOK;
		private DevComponents.DotNetBar.ButtonX btnCancel;
		private DevComponents.DotNetBar.LabelX labelX1;
		private DevComponents.DotNetBar.Controls.TextBoxX txtShaderName;
		private DevComponents.DotNetBar.Controls.TextBoxX txtShaderDescription;
		private DevComponents.DotNetBar.LabelX labelX2;
		private DevComponents.DotNetBar.LabelX labelX3;
		private DevComponents.DotNetBar.LabelX labelX4;
		private DevComponents.DotNetBar.LabelX labelX5;
		private System.Windows.Forms.CheckBox chkViewSource;

	}
}