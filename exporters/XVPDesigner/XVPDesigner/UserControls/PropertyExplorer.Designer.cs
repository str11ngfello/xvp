namespace XVPDesigner.UserControls
{
    partial class PropertyExplorer
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
			this.pgridNodeProperties = new System.Windows.Forms.PropertyGrid();
			this.SuspendLayout();
			// 
			// pgridNodeProperties
			// 
			this.pgridNodeProperties.AllowDrop = true;
			this.pgridNodeProperties.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pgridNodeProperties.Location = new System.Drawing.Point(0,0);
			this.pgridNodeProperties.Name = "pgridNodeProperties";
			this.pgridNodeProperties.Size = new System.Drawing.Size(234,252);
			this.pgridNodeProperties.TabIndex = 0;
			this.pgridNodeProperties.DragOver += new System.Windows.Forms.DragEventHandler(this.pgridNodeProperties_DragOver);
			this.pgridNodeProperties.SelectedObjectsChanged += new System.EventHandler(this.pgridNodeProperties_SelectedObjectsChanged);
			this.pgridNodeProperties.DragDrop += new System.Windows.Forms.DragEventHandler(this.pgridNodeProperties_DragDrop);
			this.pgridNodeProperties.EnabledChanged += new System.EventHandler(this.pgridNodeProperties_EnabledChanged);
			this.pgridNodeProperties.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.pgridNodeProperties_PropertyValueChanged);
			// 
			// PropertyExplorer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.pgridNodeProperties);
			this.Name = "PropertyExplorer";
			this.Size = new System.Drawing.Size(234,252);
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid pgridNodeProperties;
    }
}
