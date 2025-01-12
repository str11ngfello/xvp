using System;
using System.Collections.Generic;
using System.Text;
using DevComponents.DotNetBar;
using System.Windows.Forms;
using System.Drawing;

namespace XVPDesigner.Documents
{
	public class BaseDocument
	{
		protected bool m_active = false;

		protected string m_name = "Untitled";
		public string Name
		{
			get { return m_name; }
			set { m_name = value; }
		}

		virtual public void setActive(bool active)
		{
			m_active = active;
		}
		
		virtual public bool create(TabItem tab,string name)
		{
			return true;
		}

		virtual public bool open(TabItem tab,string filename)
		{
			return true;
		}

		virtual public bool save()
		{
			return true;
		}

		virtual public bool saveAs()
		{
			return true;
		}

		virtual public bool close()
		{
			return true;
		}

		virtual public bool postLoad()
		{
			return true;
		}

		virtual public bool export()
		{
			return true;
		}
	}
}
