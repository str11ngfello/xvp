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


namespace AnimControllerPlugin
{
    public partial class AnimPreviewBar : UserControl,ActionExplorerBase
    {
		public AnimController m_animController = null;

        public AnimPreviewBar()
        {
            InitializeComponent();
        }

		public void setSceneNode(SceneNode sceneNode)
		{
			AnimController ac = sceneNode as AnimController;
			m_animController = ac;

			//Clear any previous tracks
			cbTrackNames.Items.Clear();
			
			//Add new tracks to the combo box if the scene node has any
			if (ac.Animations != null)
			{
				foreach (AnimController.AnimationTrack track in ac.Animations)
					cbTrackNames.Items.Add(new ComboBoxItem(track.Name,track.Name));

				if (ac.Animations.Length != 0)
				{
					cbTrackNames.SelectedIndex = 0;
					enableButtons(true);
					setTrackProperties();
				}
				else
					enableButtons(false);
			}
			else
				enableButtons(false);
		}

		private void enableButtons(bool enable)
		{
			btnPlay.Enabled = enable;
			btnStop.Enabled = enable;
			btnStopAll.Enabled = enable;
			tbTrack.Enabled = enable;
			chkLoop.Enabled = enable;
		}

		private void setTrackProperties()
		{
			if (m_animController != null)
			{
				tbTrack.Minimum = 0;
				tbTrack.Maximum = (int)m_animController.getTrackDuration((uint)cbTrackNames.SelectedIndex);
				lblDuration.Text = tbTrack.Maximum.ToString();
				lblCurrentTime.Text = tbTrack.Value.ToString();
			}
		}

		private void btnPlay_Click(object sender,EventArgs e)
		{
			//Play selected track
			int playCount = 1;
			if (chkLoop.Checked)
				playCount = -1;
			m_animController.playTrack((uint)cbTrackNames.SelectedIndex,(float)spinnerSpeed.Value,playCount,(float)spinnerWeight.Value);
		}

		private void cbTrackNames_SelectedIndexChanged(object sender,EventArgs e)
		{
			if (m_animController != null)
			{
				setTrackProperties();
			}
		}

		private void chkLoop_CheckedChanged(object sender,EventArgs e)
		{
			if (chkLoop.Checked)
				m_animController.setTrackPlayCount((uint)cbTrackNames.SelectedIndex,- 1);
			else
				m_animController.setTrackPlayCount((uint)cbTrackNames.SelectedIndex,1);				
		}

		private void spinnerWeight_ValueChanged(object sender,EventArgs e)
		{
			m_animController.setTrackWeight((uint)cbTrackNames.SelectedIndex,(float)spinnerWeight.Value);
		}

		private void spinnerSpeed_ValueChanged(object sender,EventArgs e)
		{
			m_animController.setTrackSpeed((uint)cbTrackNames.SelectedIndex,(float)spinnerSpeed.Value);
		}

		private void btnStop_Click(object sender,EventArgs e)
		{
			m_animController.stopTrack((uint)cbTrackNames.SelectedIndex);
		}

		private void btnStopAll_Click(object sender,EventArgs e)
		{
			if (m_animController != null)
			{
				m_animController.stopAllTracks();
			}
		}

		private void tbTrack_ValueChanged(object sender,EventArgs e)
		{
			m_animController.playTrack((uint)cbTrackNames.SelectedIndex,(float)spinnerSpeed.Value,1,(float)spinnerWeight.Value);
			m_animController.pauseTrack((uint)cbTrackNames.SelectedIndex);
			m_animController.setTrackTime((uint)cbTrackNames.SelectedIndex,(uint)tbTrack.Value);
			lblCurrentTime.Text = tbTrack.Value.ToString();
		}
    }
}
