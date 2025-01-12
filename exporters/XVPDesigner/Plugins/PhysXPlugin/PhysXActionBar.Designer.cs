namespace PhysXPlugin
{
    partial class PhysXActionBar
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
			this.gbPhysX = new System.Windows.Forms.GroupBox();
			this.btnAddForce = new DevComponents.DotNetBar.ButtonX();
			this.gbPhysX.SuspendLayout();
			this.SuspendLayout();
			// 
			// gbPhysX
			// 
			this.gbPhysX.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.gbPhysX.BackColor = System.Drawing.SystemColors.Control;
			this.gbPhysX.Controls.Add(this.btnAddForce);
			this.gbPhysX.Location = new System.Drawing.Point(0,0);
			this.gbPhysX.Name = "gbPhysX";
			this.gbPhysX.Size = new System.Drawing.Size(247,208);
			this.gbPhysX.TabIndex = 0;
			this.gbPhysX.TabStop = false;
			this.gbPhysX.Text = "PhysX Actions";
			// 
			// btnAddForce
			// 
			this.btnAddForce.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnAddForce.Location = new System.Drawing.Point(79,98);
			this.btnAddForce.Name = "btnAddForce";
			this.btnAddForce.Size = new System.Drawing.Size(75,23);
			this.btnAddForce.TabIndex = 0;
			this.btnAddForce.Text = "Add Force";
			this.btnAddForce.Click += new System.EventHandler(this.btnAddForce_Click);
			// 
			// PhysXActionBar
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.gbPhysX);
			this.Name = "PhysXActionBar";
			this.Size = new System.Drawing.Size(250,211);
			this.gbPhysX.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.GroupBox gbPhysX;
		private DevComponents.DotNetBar.ButtonX btnAddForce;

    }
}
