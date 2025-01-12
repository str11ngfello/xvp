using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using XVPManaged;
using System.Collections;

namespace XVPDesigner.Objects
{
	[Serializable]
	public class ActorContainer : SceneNode
	{
		//Transform
		float[] m_transform = new float[] {1.0f,0.0f,0.0f,0.0f,
										0.0f,1.0f,0.0f,0.0f,
										0.0f,0.0f,1.0f,0.0f,
										0.0f,0.0f,0.0f,1.0f};
		[Browsable(false)]
		public float[] Transform
		{
			get { return m_transform; }
			set { m_transform = value; }
		}

		public ActorContainer(string name)
			: base(name)
		{
			AcceptsControllers = false;
			Renameable = true;

			setDefaultValues();
		}

		public void createProxyInstance(uint XVPMHandle)
		{
			if (m_proxy != IntPtr.Zero)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				m_proxy = XVPM.initActorContainer(XVPMHandle);
			}	
		}

		public void destroyProxyInstance()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy actor container proxy
				XVPM.destroyActorContainer(m_proxy);
				m_proxy = IntPtr.Zero;
			}
		}

		public unsafe float* getTransform()
		{
			unsafe
			{
				return XVPM.getActorContainerTransform(m_proxy);
			}
		}

		public void upSyncTransform()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				unsafe
				{
					float* t = XVPM.getActorContainerTransform(m_proxy);
					m_transform[0] = t[0];
					m_transform[1] = t[1];
					m_transform[2] = t[2];
					m_transform[3] = t[3];
					m_transform[4] = t[4];
					m_transform[5] = t[5];
					m_transform[6] = t[6];
					m_transform[7] = t[7];
					m_transform[8] = t[8];
					m_transform[9] = t[9];
					m_transform[10] = t[10];
					m_transform[11] = t[11];
					m_transform[12] = t[12];
					m_transform[13] = t[13];
					m_transform[14] = t[14];
					m_transform[15] = t[15];
				}
			}
		}

		public void downSyncTransform()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				unsafe
				{
					float* t = XVPM.getActorContainerTransform(m_proxy);
					t[0] =    m_transform[0];
					t[1]=	  m_transform[1];
					t[2]=	  m_transform[2];
					t[3]=	  m_transform[3];
					t[4]=	  m_transform[4];
					t[5]=	  m_transform[5];
					t[6]=	  m_transform[6];
					t[7]=	  m_transform[7];
					t[8]=	  m_transform[8];
					t[9]=	  m_transform[9];
					t[10] =    m_transform[10];
					t[11] =    m_transform[11];
					t[12] =    m_transform[12];
					t[13] =    m_transform[13];
					t[14] =    m_transform[14];
					t[15] =    m_transform[15];
				}
			}
		}
	}
}
