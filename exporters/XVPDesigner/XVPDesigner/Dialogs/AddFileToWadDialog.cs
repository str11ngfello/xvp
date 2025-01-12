using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.Dialogs
{
    public partial class AddFileToWadDialog : Form
    {
        public ListBox.ObjectCollection Paths
        {
            get { return lbFiles.Items; }
        }

        public bool Compress
        {
            get { return chkCompress.Checked; }
        }

        public AddFileToWadDialog()
        {
            InitializeComponent();
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            //Get a filename from the user
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = true;

            lbFiles.Items.Clear();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                foreach (string s in ofd.FileNames)
                    lbFiles.Items.Add(s);
            }

            ofd.Dispose();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}