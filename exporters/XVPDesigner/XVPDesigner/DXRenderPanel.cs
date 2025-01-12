using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel.Design;

namespace XVPDesigner.UserControls
{
	class DXRenderPanel : Panel
	{
		protected override void OnPaintBackground(PaintEventArgs e)
		{
			//Only repaint the background if we have the focus.  This prevents
			//a horrible flicker that happens when the form gains/loses
			//focus. Only paint if we are in visual studio designer.
			if (GetService(typeof(IDesignerHost)) != null)
			{
				base.OnPaintBackground(e);
			}
		}


	}
}
