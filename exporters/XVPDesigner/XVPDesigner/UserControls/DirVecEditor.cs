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
	public partial class DirVecEditor : UserControl
	{
		public float XValue
		{
			get { return (float)spnX.Value; }
			set { spnX.Value = (decimal)value; }
		}

		public float YValue
		{
			get { return (float)spnY.Value; }
			set { spnY.Value = (decimal)value; }
		}

		public float ZValue
		{
			get { return (float)spnZ.Value; }
			set { spnZ.Value = (decimal)value; }
		}

		public NumericUpDown XSpinner
		{
			get { return spnX; }
		}

		public NumericUpDown YSpinner
		{
			get { return spnY; }
		}

		public NumericUpDown ZSpinner
		{
			get { return spnZ; }
		}

		public DirVecEditor()
		{
			InitializeComponent();
		}
	}
}
