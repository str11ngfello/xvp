using System;
using System.Collections.Generic;
using System.Text;

namespace Utilities
{
	class AABB
	{
		private Vector m_min;
		public Vector Min
		{
			get { return m_min; }
			set { m_min = value; }
		}

		private Vector m_max;
		public Vector Max
		{
			get { return m_max; }
			set { m_max = value; }
		}

		public AABB(Vector min,Vector max)
		{
			m_min = min;
			m_max = max;
		}

		public void intersectRay(Vector p,Vector d,float tmin,out Vector q)
		{
			tmin = 0.0f;

			float tmax = float.MaxValue;

			for (int i = 0; i < 3; ++i)
			{
				if (Math.Abs(d[i]) < .00001f)
				{
					if (p[i] < m_min[i] || p[i] > m_max[i])
					{
						q = new Vector();
						return;
					}
				}
				else
				{
					float ood = 1.0f / d[i];
					float t1 = (m_min[i] - p[i]) * ood;
					float t2 = (m_max[i] - p[i]) * ood;
					if (t1 > t2)
					{
						float temp = t1;
						t1 = t2;
						t2 = temp;
					}
					if (t1 > tmin)
						tmin = t1;
					if (t2 > tmax)
						tmax = t2;
					if (tmin > tmax)
					{
						q = new Vector();
						return;
					}
				}
			}

			q = p + d * tmin;
			return;
		}
	}
}
