using System;
using System.Collections.Generic;
using System.Text;
using DevComponents.DotNetBar;
using System.Windows.Forms;
using System.Drawing;

namespace XVPDesigner.Documents
{
	public class XVPMDocument : BaseDocument
	{
		new virtual public bool close()
		{
			Program.g_mainForm.clearTextureBrowser();
			return true;
		}

		virtual public void addActor(string actorPath,string actorName,uint viewportIndex,int screenX,int screenY)
		{
		}

		virtual public void setActiveViewportColor(Color color)
		{
		}
	}
}
