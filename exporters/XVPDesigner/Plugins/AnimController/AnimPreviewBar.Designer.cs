namespace AnimControllerPlugin
{
    partial class AnimPreviewBar
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
			this.lblWeight = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.spinnerWeight = new System.Windows.Forms.NumericUpDown();
			this.spinnerSpeed = new System.Windows.Forms.NumericUpDown();
			this.chkLoop = new System.Windows.Forms.CheckBox();
			this.label2 = new System.Windows.Forms.Label();
			this.lblDuration = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.lblCurrentTime = new System.Windows.Forms.Label();
			this.gbTrack = new System.Windows.Forms.GroupBox();
			this.btnStopAll = new DevComponents.DotNetBar.ButtonX();
			this.btnStop = new DevComponents.DotNetBar.ButtonX();
			this.lblTrack = new System.Windows.Forms.Label();
			this.cbTrackNames = new DevComponents.DotNetBar.Controls.ComboBoxEx();
			this.btnPlay = new DevComponents.DotNetBar.ButtonX();
			this.tbTrack = new DevComponents.DotNetBar.Controls.Slider();
			this.panel1 = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.spinnerWeight)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.spinnerSpeed)).BeginInit();
			this.gbTrack.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// lblWeight
			// 
			this.lblWeight.AutoSize = true;
			this.lblWeight.Location = new System.Drawing.Point(16,117);
			this.lblWeight.Name = "lblWeight";
			this.lblWeight.Size = new System.Drawing.Size(75,13);
			this.lblWeight.TabIndex = 4;
			this.lblWeight.Text = "Track Weight:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(16,91);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(72,13);
			this.label1.TabIndex = 5;
			this.label1.Text = "Track Speed:";
			// 
			// spinnerWeight
			// 
			this.spinnerWeight.DecimalPlaces = 2;
			this.spinnerWeight.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
			this.spinnerWeight.Location = new System.Drawing.Point(92,115);
			this.spinnerWeight.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.spinnerWeight.Name = "spinnerWeight";
			this.spinnerWeight.Size = new System.Drawing.Size(60,20);
			this.spinnerWeight.TabIndex = 2;
			this.spinnerWeight.Value = new decimal(new int[] {
            10,
            0,
            0,
            65536});
			this.spinnerWeight.ValueChanged += new System.EventHandler(this.spinnerWeight_ValueChanged);
			// 
			// spinnerSpeed
			// 
			this.spinnerSpeed.DecimalPlaces = 2;
			this.spinnerSpeed.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
			this.spinnerSpeed.Location = new System.Drawing.Point(92,89);
			this.spinnerSpeed.Name = "spinnerSpeed";
			this.spinnerSpeed.Size = new System.Drawing.Size(60,20);
			this.spinnerSpeed.TabIndex = 3;
			this.spinnerSpeed.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.spinnerSpeed.ValueChanged += new System.EventHandler(this.spinnerSpeed_ValueChanged);
			// 
			// chkLoop
			// 
			this.chkLoop.AutoSize = true;
			this.chkLoop.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.chkLoop.Location = new System.Drawing.Point(164,61);
			this.chkLoop.Name = "chkLoop";
			this.chkLoop.Size = new System.Drawing.Size(50,17);
			this.chkLoop.TabIndex = 6;
			this.chkLoop.Text = "Loop";
			this.chkLoop.UseVisualStyleBackColor = true;
			this.chkLoop.CheckedChanged += new System.EventHandler(this.chkLoop_CheckedChanged);
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(247,176);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(50,13);
			this.label2.TabIndex = 7;
			this.label2.Text = "Duration:";
			// 
			// lblDuration
			// 
			this.lblDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.lblDuration.AutoSize = true;
			this.lblDuration.Location = new System.Drawing.Point(296,176);
			this.lblDuration.Name = "lblDuration";
			this.lblDuration.Size = new System.Drawing.Size(55,13);
			this.lblDuration.TabIndex = 8;
			this.lblDuration.Text = "30000000";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.ForeColor = System.Drawing.Color.Black;
			this.label5.Location = new System.Drawing.Point(16,176);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(44,13);
			this.label5.TabIndex = 9;
			this.label5.Text = "Current:";
			// 
			// lblCurrentTime
			// 
			this.lblCurrentTime.AutoSize = true;
			this.lblCurrentTime.Location = new System.Drawing.Point(60,176);
			this.lblCurrentTime.Name = "lblCurrentTime";
			this.lblCurrentTime.Size = new System.Drawing.Size(55,13);
			this.lblCurrentTime.TabIndex = 10;
			this.lblCurrentTime.Text = "30000000";
			// 
			// gbTrack
			// 
			this.gbTrack.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.gbTrack.BackColor = System.Drawing.SystemColors.Control;
			this.gbTrack.Controls.Add(this.label5);
			this.gbTrack.Controls.Add(this.label2);
			this.gbTrack.Controls.Add(this.lblCurrentTime);
			this.gbTrack.Controls.Add(this.lblDuration);
			this.gbTrack.Controls.Add(this.btnStopAll);
			this.gbTrack.Controls.Add(this.btnStop);
			this.gbTrack.Controls.Add(this.lblTrack);
			this.gbTrack.Controls.Add(this.cbTrackNames);
			this.gbTrack.Controls.Add(this.btnPlay);
			this.gbTrack.Controls.Add(this.chkLoop);
			this.gbTrack.Controls.Add(this.spinnerSpeed);
			this.gbTrack.Controls.Add(this.spinnerWeight);
			this.gbTrack.Controls.Add(this.label1);
			this.gbTrack.Controls.Add(this.lblWeight);
			this.gbTrack.Controls.Add(this.tbTrack);
			this.gbTrack.Location = new System.Drawing.Point(3,3);
			this.gbTrack.Name = "gbTrack";
			this.gbTrack.Size = new System.Drawing.Size(355,201);
			this.gbTrack.TabIndex = 0;
			this.gbTrack.TabStop = false;
			this.gbTrack.Text = "Track Actions";
			// 
			// btnStopAll
			// 
			this.btnStopAll.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnStopAll.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnStopAll.Location = new System.Drawing.Point(101,58);
			this.btnStopAll.Name = "btnStopAll";
			this.btnStopAll.Size = new System.Drawing.Size(52,20);
			this.btnStopAll.TabIndex = 17;
			this.btnStopAll.Text = "Stop All";
			this.btnStopAll.Click += new System.EventHandler(this.btnStopAll_Click);
			// 
			// btnStop
			// 
			this.btnStop.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnStop.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnStop.Location = new System.Drawing.Point(60,58);
			this.btnStop.Name = "btnStop";
			this.btnStop.Size = new System.Drawing.Size(35,20);
			this.btnStop.TabIndex = 16;
			this.btnStop.Text = "Stop";
			this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
			// 
			// lblTrack
			// 
			this.lblTrack.AutoSize = true;
			this.lblTrack.Location = new System.Drawing.Point(16,30);
			this.lblTrack.Name = "lblTrack";
			this.lblTrack.Size = new System.Drawing.Size(38,13);
			this.lblTrack.TabIndex = 15;
			this.lblTrack.Text = "Track:";
			// 
			// cbTrackNames
			// 
			this.cbTrackNames.DisplayMember = "Text";
			this.cbTrackNames.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.cbTrackNames.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbTrackNames.FormattingEnabled = true;
			this.cbTrackNames.Location = new System.Drawing.Point(60,26);
			this.cbTrackNames.Name = "cbTrackNames";
			this.cbTrackNames.Size = new System.Drawing.Size(154,21);
			this.cbTrackNames.TabIndex = 14;
			this.cbTrackNames.SelectedIndexChanged += new System.EventHandler(this.cbTrackNames_SelectedIndexChanged);
			// 
			// btnPlay
			// 
			this.btnPlay.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
			this.btnPlay.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
			this.btnPlay.Location = new System.Drawing.Point(19,58);
			this.btnPlay.Name = "btnPlay";
			this.btnPlay.Size = new System.Drawing.Size(35,20);
			this.btnPlay.TabIndex = 13;
			this.btnPlay.Text = "Play";
			this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
			// 
			// tbTrack
			// 
			this.tbTrack.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tbTrack.LabelVisible = false;
			this.tbTrack.Location = new System.Drawing.Point(6,152);
			this.tbTrack.Name = "tbTrack";
			this.tbTrack.Size = new System.Drawing.Size(343,23);
			this.tbTrack.TabIndex = 12;
			this.tbTrack.Text = "slider1";
			this.tbTrack.Value = 0;
			this.tbTrack.ValueChanged += new System.EventHandler(this.tbTrack_ValueChanged);
			// 
			// panel1
			// 
			this.panel1.AutoScroll = true;
			this.panel1.Controls.Add(this.gbTrack);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(0,0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(361,207);
			this.panel1.TabIndex = 18;
			// 
			// AnimPreviewBar
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F,13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.panel1);
			this.Name = "AnimPreviewBar";
			this.Size = new System.Drawing.Size(361,207);
			((System.ComponentModel.ISupportInitialize)(this.spinnerWeight)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.spinnerSpeed)).EndInit();
			this.gbTrack.ResumeLayout(false);
			this.gbTrack.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Label lblWeight;
		private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown spinnerWeight;
        private System.Windows.Forms.NumericUpDown spinnerSpeed;
        private System.Windows.Forms.CheckBox chkLoop;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblDuration;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lblCurrentTime;
		private System.Windows.Forms.GroupBox gbTrack;
		private DevComponents.DotNetBar.ButtonX btnPlay;
		private DevComponents.DotNetBar.Controls.Slider tbTrack;
		private DevComponents.DotNetBar.Controls.ComboBoxEx cbTrackNames;
		private System.Windows.Forms.Label lblTrack;
		private DevComponents.DotNetBar.ButtonX btnStopAll;
		private DevComponents.DotNetBar.ButtonX btnStop;
		private System.Windows.Forms.Panel panel1;

    }
}
