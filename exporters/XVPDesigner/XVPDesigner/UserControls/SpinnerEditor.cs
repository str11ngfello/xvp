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
	public partial class SpinnerEditor : UserControl
	{
		public decimal Value
		{
			get { return spinnerValue.Value; }
			set { spinnerValue.Value = value; }
		}

		public decimal MinValue
		{
			set
			{
				spinnerValue.Minimum = value;
				lblMinValue.Text = value.ToString();
			}
		}

		public decimal MaxValue
		{
			set
			{
				spinnerValue.Maximum = value;
				lblMaxValue.Text = value.ToString();
			}
		}

		public NumericUpDown Spinner
		{
			get { return spinnerValue; }
		}

		public SpinnerEditor()
		{
			InitializeComponent();
		}
	}
}
