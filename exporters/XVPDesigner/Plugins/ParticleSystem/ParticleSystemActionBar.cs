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


namespace ParticleSystemPlugin
{
    public partial class ParticleSystemActionBar : UserControl,ActionExplorerBase
    {
		public ParticleSystem m_particleSystem = null;

		public ParticleSystemActionBar()
        {
            InitializeComponent();
        }

		public void setSceneNode(SceneNode sceneNode)
		{
			m_particleSystem = sceneNode as ParticleSystem;
		}

		private void btnStart_Click(object sender,EventArgs e)
		{
			m_particleSystem.start();
		}

		private void btnStop_Click(object sender,EventArgs e)
		{
			m_particleSystem.stop();
		}

		private void btnRestart_Click(object sender,EventArgs e)
		{
			m_particleSystem.restart();
		}
    }
}
