using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar.Controls;

namespace XVPDesigner.UserControls
{
	public partial class UIntSliderEditor : UserControl
	{
		public uint Value
		{
			get { return (uint)tbValue.Value; }
			set { tbValue.Value = (int)value; }
		}

		public uint MinValue
		{
			set
			{
				tbValue.Minimum = (int)value;
				lblMinValue.Text = value.ToString();
			}
		}

		public uint MaxValue
		{
			set
			{
				tbValue.Maximum = (int)value;
				lblMaxValue.Text = value.ToString();
			}
		}

		public Slider Slider
		{
			get { return tbValue; }
		}

		public UIntSliderEditor()
		{
			InitializeComponent();
		}

		private void tbValue_ValueChanged(object sender,EventArgs e)
		{
			lblCurrentValue.Text = tbValue.Value.ToString();
		}


	}
}
