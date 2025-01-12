namespace ParticleSystemPlugin
{
    partial class ParticleSystemActionBar
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
			this.btnRestart = new DevComponents.DotNetBar.ButtonX();
			this.btnStop = new DevComponents.DotNetBar.ButtonX();
			this.btnStart = new DevComponents.DotNetBar.ButtonX();
			this.gbPhysX.SuspendLayout();
			this.SuspendLayout();
			// 
			// gbPhysX
			// 
			this.gbPhysX.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.gbPhysX.BackColor = System.Drawing.SystemColors.Control;
			this.gbPhysX.Controls.Add(this.btnRestart);
			this.gbPhysX.Controls.Add(this.btnStop);
			this.gbPhysX.Controls.Add(this.btnStart);
			this.gbPhysX.Location = new System.Drawing.Point(0,0);
			this.gbPhysX.Name = "gbPhysX";
			this.gbPhysX.Size = new System.Drawing.Size(247,208);
			this.gbPhysX.TabIndex = 0;
			this.gbPhysX.TabStop = false;
			this.gbPhysX.Text = "Particle System Actions";
			// 
			// btnRestart
			// 
			this.btnRestart.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnRestart.Location = new System.Drawing.Point(137,22);
			this.btnRestart.Name = "btnRestart";
			this.btnRestart.Size = new System.Drawing.Size(69,23);
			this.btnRestart.TabIndex = 2;
			this.btnRestart.Text = "Restart";
			this.btnRestart.Click += new System.EventHandler(this.btnRestart_Click);
			// 
			// btnStop
			// 
			this.btnStop.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnStop.Location = new System.Drawing.Point(76,22);
			this.btnStop.Name = "btnStop";
			this.btnStop.Size = new System.Drawing.Size(55,23);
			this.btnStop.TabIndex = 1;
			this.btnStop.Text = "Stop";
			this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
			// 
			// btnStart
			// 
			this.btnStart.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnStart.Location = new System.Drawing.Point(15,22);
			this.btnStart.Name = "btnStart";
			this.btnStart.Size = new System.Drawing.Size(55,23);
			this.btnStart.TabIndex = 0;
			this.btnStart.Text = "Start";
			this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
			// 
			// ParticleSystemActionBar
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.gbPhysX);
			this.Name = "ParticleSystemActionBar";
			this.Size = new System.Drawing.Size(250,211);
			this.gbPhysX.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.GroupBox gbPhysX;
		private DevComponents.DotNetBar.ButtonX btnStart;
		private DevComponents.DotNetBar.ButtonX btnRestart;
		private DevComponents.DotNetBar.ButtonX btnStop;

    }
}
