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
	public partial class IntSliderEditor : UserControl
	{
		public int Value
		{
			get { return tbValue.Value; }
			set { tbValue.Value = value; }
		}

		public int MinValue
		{
			set
			{
				tbValue.Minimum = value;
				lblMinValue.Text = value.ToString();
			}
		}

		public int MaxValue
		{
			set
			{
				tbValue.Maximum = value;
				lblMaxValue.Text = value.ToString();
			}
		}

		public Slider Slider
		{
			get { return tbValue; }
		}

		public IntSliderEditor()
		{
			InitializeComponent();
		}

		private void tbValue_ValueChanged(object sender,EventArgs e)
		{
			lblCurrentValue.Text = tbValue.Value.ToString();
		}


	}
}
