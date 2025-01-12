using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.TypeEditors
{
    public partial class FloatSliderEditor : UserControl
    {
        private float m_min;
        private float m_max;
        private float m_step = 1.0f;
        private string m_decimalPlaces;
        
        public float Value
        {
            get { return tbMain.Value*m_step + m_min; }
			set 
			{ 
				int v = Convert.ToInt32((value - m_min) / m_step);
				if (v >= tbMain.Minimum && v <= tbMain.Maximum)
					tbMain.Value = v;
			}
        }

		public DevComponents.DotNetBar.Controls.Slider MainTrackBar
		{
			get { return tbMain; }
		}

        public FloatSliderEditor()
        {
            SuspendLayout();
            InitializeComponent();
            ResumeLayout();
        }

        public void init(float minFloat,float maxFloat,float stepSize)
        {
            if (maxFloat < minFloat)
                throw new Exception("Max float cannot be less than min float for float slider");
            
            stepSize = Math.Abs(stepSize);
            if (stepSize == 0.0f)
                throw new Exception("Step size cannot be 0");

            int numSteps = Convert.ToInt32((maxFloat - minFloat) / stepSize);
            //if (numSteps > 2000000)
                //throw new Exception("Can't create float slider because range is too large.");
            //if ((maxFloat - minFloat) % stepSize != 0)
              //  throw new Exception("Step size must be a multiple of the total range. I.e, step size should be evenly divide the total range");

            tbMain.Minimum = 0;
            tbMain.Maximum = numSteps;
            tbMain.Value = 0;

            //Build decimal places "string"
            string s = stepSize.ToString();
            string[] tokens = s.Split('.');
            m_decimalPlaces = ".";
            if (tokens.Length > 1)
            {
                for (int i = 0; i < tokens[tokens.Length - 1].Length; ++i)
                {
                    m_decimalPlaces += "0";
                }
            }
            else
                m_decimalPlaces = "";

            m_min = minFloat;
            m_max = maxFloat;
            m_step = stepSize;

			lblMinValue.Text = String.Format("{0:0.######}",minFloat);
			lblMaxValue.Text = String.Format("{0:0.######}",maxFloat);

		}

		private void tbMain_ValueChanged(object sender,EventArgs e)
		{
			lblCurrentValue.Text = String.Format("{0:0.######}",Value);
		}
    }
}
