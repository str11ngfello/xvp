using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.Dialogs
{
    public partial class SplashDialog : Form
    {
		private Font drawFont = new Font("Verdana",7);
		private SolidBrush drawBrush = new SolidBrush(Color.White);
		private PointF drawPoint = new PointF(15,185);

		public bool EnableCloseButton
		{
			get { return btnClose.Visible; }
			set { btnClose.Visible = value; }
		}

        public SplashDialog()
        {
			SetStyle(ControlStyles.SupportsTransparentBackColor,true);
			BackColor = Color.Transparent;
	
            InitializeComponent();
        }

        public void setStatus(string statusText)
        {
			picSplashImage.Refresh();
			picSplashImage.CreateGraphics().DrawString(statusText,drawFont,drawBrush,drawPoint);
        }

		private void btnClose_Click(object sender,EventArgs e)
		{
			Close();
		}
    }
}