using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.Interfaces;
using XVPDesigner.Objects;
using DevComponents.DotNetBar;


namespace PhysXPlugin
{
    public partial class PhysXActionBar : UserControl,ActionExplorerBase
    {
		public PhysXController m_physXController = null;

		public PhysXActionBar()
        {
            InitializeComponent();
        }

		public void setSceneNode(SceneNode sceneNode)
		{
			PhysXController c = sceneNode as PhysXController;
			m_physXController = c;
		}

		private void btnAddForce_Click(object sender,EventArgs e)
		{
			m_physXController.addForce(0,10000,0);
		}
    }
}
