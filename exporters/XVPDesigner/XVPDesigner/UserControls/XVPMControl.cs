
using System;
using System.Drawing;
using System.Windows.Forms;
using XVPManaged;
using System.Runtime.InteropServices;
using XVPDesigner.Objects;
using XVPDesigner.Documents;
using XVPDesigner.Managers;
using DevComponents.DotNetBar;

namespace XVPDesigner
{
	/// <summary>Class that encapsulates the managed XVP wrapper into a reusable control</summary>
	public partial class XVPMControl : UserControl
	{
		//This enum must match the version that exists in XVPManaged
		enum ViewType
		{
			PERSPECTIVE,
			LEFT,
			TOP,
			RIGHT,
			BOTTOM,
			FRONT,
			BACK
		};

		private ViewportConfig m_viewportConfig;

		private uint m_activeLayout = 0;
		private int m_activeViewport = 0;
		private int m_tempActiveViewport = 0;
		private bool m_lockSelection = false;
		public bool SelectionLocked
		{
			get { return m_lockSelection; }
		}

		private bool m_controlPressed = false;

		private bool m_enableCameraMovement = false;
		private bool m_rotateCamera = false;
		private bool m_panCamera = false;
		private bool m_zoomCamera = false;

		private int m_lastMouseX = 0;
		private int m_lastMouseY = 0;

		private bool m_movingHS = false;
		private int m_hsClickY = 0;
		private bool m_movingVS = false;
		private int m_vsClickX = 0;
		private bool m_movingCS = false;
		private Point m_CSPosBeforeMaximize;
		private bool m_maximized = false;


		//private XVPManaged.XVPM m_XVPM = null;
		private uint handle = 0;
		public uint XVPMHandle
		{ get { return handle; } }

		public XVPMDocument m_parentDoc;

		/*public XVPManaged.XVPM XVPM
		{
			get { return m_XVPM; }
		}*/

		private Color m_activeViewportColor = Color.Yellow;
		public Color ActiveViewportColor
		{
			set
			{
				btnHighlightLeft.BackColor = value;
				btnHighlightTop.BackColor = value;
				btnHighlightRight.BackColor = value;
				btnHighlightBottom.BackColor = value;
			}
		}

		/// <summary>Constructor</summary>
		public XVPMControl(XVPMDocument parentDoc)
		{
			InitializeComponent();

			m_parentDoc = parentDoc;

			//We need to get a handle to our new window before doing anything else
			Visible = false;
			panRender.Visible = false;
			lock (Program.g_mainForm.XVPMLockObject)
			{
				handle = XVPM.addWindow(panRender.Handle,(uint)panRender.Width,(uint)panRender.Height);
			}

			m_viewportConfig = new ViewportConfig();

			cbViewType0.SelectedItem = ciPerspective0;
			cbViewType1.SelectedItem = ciTop1;
			cbViewType2.SelectedItem = ciLeft2;
			cbViewType3.SelectedItem = ciFront3;
		}

		~XVPMControl()
		{
			Release();
		}

		public void showRendering()
		{
		    Visible = true;
		    panRender.Visible = true;
			//bar0.Visible = true;
			//bar1.Visible = true;
		}

		public void Release()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Hide the panel immediately to make drawing cleaner.
				panRender.Hide();
				XVPM.removeWindow(handle);
			}

			Dispose();
		}
		public void activate(bool activate)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.activate(handle,activate);
			}
		}

		private void panRender_MouseDown(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
				m_rotateCamera = true;
			if (e.Button == MouseButtons.Right)
				m_zoomCamera = true;
			if (e.Button == MouseButtons.Middle)
				m_panCamera = true;

			m_lastMouseX = e.X;
			m_lastMouseY = e.Y;
			XVPM.enableInput(handle,(uint)getViewportIndexFromMouseClick(e.X,e.Y));


	

			int viewportIndex = getViewportIndexFromMouseClick(e.X,e.Y);
			setActiveViewport(viewportIndex);

			//Do selection if no lock and no oneclickadd
			if (!m_lockSelection && 
				!Program.g_mainForm.m_libraryExplorer.OneClickAdd)
			{
				if ((e.Button == MouseButtons.Left) && !isMovementEnabled())
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						//Multiple selection mode?
						XVPM.selectSceneNodes(handle,e.X,e.Y,m_controlPressed);
					}
				}
			}

			//Check for oneclickadd if we aren't moving
			if (Program.g_mainForm.m_libraryExplorer.OneClickAdd &&
				MainForm.ModifierKeys != Keys.Alt)
			{
				TreeNode nodeSource = Program.g_mainForm.m_libraryExplorer.getSelectedActorNode();
				if (nodeSource != null)
				{
					m_parentDoc.addActor(nodeSource.Name,nodeSource.Text,(uint)getViewportIndexFromMouseClick(e.X,e.Y),e.X,e.Y);
				}
			}
		}

		private void panRender_MouseUp(object sender,MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
				m_rotateCamera = false;
			if (e.Button == MouseButtons.Right)
				m_zoomCamera = false;
			if (e.Button == MouseButtons.Middle)
				m_panCamera = false;

			if (e.Button == MouseButtons.Left)
			{
				lock (Program.g_mainForm.XVPMLockObject)
				{
					XVPM.gizmoOnMouseUp(handle);
				}
			}

			XVPM.disableInput(handle);
		}

		private void panRender_Resize(object sender,EventArgs e)
		{
			
		}

		public void setLayout(uint layout)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				switch (layout)
				{
					case 0:
						XVPM.setLayout(handle,0,
										new Rectangle(panRender.Left,panRender.Top,panRender.Right - panRender.Left,panRender.Bottom - panRender.Top),
										new Rectangle(0,0,0,0),
										new Rectangle(0,0,0,0),
										new Rectangle(0,0,0,0));

						bar0.Left = panRender.Left;
						bar0.Top = panRender.Top;
						bar0.Width = panRender.Right - panRender.Left;

						bar0.Show();
						bar1.Hide();
						bar2.Hide();
						bar3.Hide();

						btnVerticalSplitter.Hide();
						btnVerticalSplitterShadow.Hide();
						btnHorizontalSplitter.Hide();
						btnHorizontalSplitterShadow.Hide();
						btnCenterSplitter.Hide();
						btnCenterSplitterShadow.Hide();

						setActiveViewport(0);
						break;
					case 1:
						if (btnVerticalSplitter.Left >= Width)
						{
							btnVerticalSplitter.Left = Width - btnVerticalSplitter.Width - 1;
							btnVerticalSplitterShadow.Left = Width - btnVerticalSplitter.Width - 1;
						}
						if (btnVerticalSplitter.Left < 0)
						{
							btnVerticalSplitter.Left = 0;
							btnVerticalSplitterShadow.Left = 0;
						}
						XVPM.setLayout(handle,
										1,
										new Rectangle(panRender.Left,panRender.Top,btnVerticalSplitter.Left - panRender.Left,panRender.Bottom - panRender.Top),
										new Rectangle(btnVerticalSplitter.Right,panRender.Top,panRender.Right - btnVerticalSplitter.Right,panRender.Bottom - panRender.Top),
										new Rectangle(0,0,0,0),
										new Rectangle(0,0,0,0));

						bar0.Left = panRender.Left;
						bar0.Top = panRender.Top;
						bar0.Width = btnVerticalSplitter.Left - panRender.Left;
						bar1.Left = btnVerticalSplitter.Right;
						bar1.Top = panRender.Top;
						bar1.Width = panRender.Right - btnVerticalSplitter.Right;	

						bar0.Show();
						bar1.Show();
						bar2.Hide();
						bar3.Hide();

						btnVerticalSplitter.Show();
						btnVerticalSplitterShadow.Show();
						btnVerticalSplitter.BringToFront();
						btnHorizontalSplitter.Hide();
						btnHorizontalSplitterShadow.Hide();
						btnCenterSplitter.Hide();
						btnCenterSplitterShadow.Hide();

						if (m_activeViewport <= 1)
							setActiveViewport(m_activeViewport);
						else
							setActiveViewport(0);

						break;
					case 2:
						if (btnHorizontalSplitter.Top >= Height)
						{
							btnHorizontalSplitter.Top = Height - btnHorizontalSplitter.Height - 1;
							btnHorizontalSplitterShadow.Top = Height - btnHorizontalSplitter.Height - 1;
						}
						if (btnHorizontalSplitter.Top < 0)
						{
							btnHorizontalSplitter.Top = 0;
							btnHorizontalSplitterShadow.Top = 0;
						}
						XVPM.setLayout(handle,2,
								new Rectangle(panRender.Left,panRender.Top,panRender.Right - panRender.Left,btnHorizontalSplitter.Top - panRender.Top),
								new Rectangle(panRender.Left,btnHorizontalSplitter.Bottom,panRender.Right - panRender.Left,panRender.Bottom - btnHorizontalSplitter.Bottom),
								new Rectangle(0,0,0,0),
								new Rectangle(0,0,0,0));


						bar0.Left = panRender.Left;
						bar0.Top = panRender.Top;
						bar0.Width = panRender.Right - panRender.Left;
						bar1.Left = panRender.Left;
						bar1.Top = btnHorizontalSplitter.Bottom;
						bar1.Width = panRender.Right - panRender.Left;

						bar0.Show();
						bar1.Show();
						bar2.Hide();
						bar3.Hide();

						btnVerticalSplitter.Hide();
						btnVerticalSplitterShadow.Hide();
						btnHorizontalSplitter.Show();
						btnHorizontalSplitterShadow.Show();
						btnHorizontalSplitter.BringToFront();
						btnCenterSplitter.Hide();
						btnCenterSplitterShadow.Hide();

						if (m_activeViewport <= 1)
							setActiveViewport(m_activeViewport);
						else
							setActiveViewport(0);

						break;
					case 3:
						if (btnVerticalSplitter.Left >= Width)
						{
							btnVerticalSplitter.Left = Width - btnVerticalSplitter.Width - 1;
							btnVerticalSplitterShadow.Left = Width - btnVerticalSplitter.Width - 1;
						}
						if (btnVerticalSplitter.Left < 0)
						{
							btnVerticalSplitter.Left = 0;
							btnVerticalSplitterShadow.Left = 0;
						}
						if (btnHorizontalSplitter.Top >= Height)
						{
							btnHorizontalSplitter.Top = Height - btnHorizontalSplitter.Height - 1;
							btnHorizontalSplitterShadow.Top = Height - btnHorizontalSplitter.Height - 1;
						}
						if (btnHorizontalSplitter.Top < 0)
						{
							btnHorizontalSplitter.Top = 0;
							btnHorizontalSplitterShadow.Top = 0;
						}
						if (btnCenterSplitter.Left >= Width)
						{
							btnCenterSplitter.Left = Width - btnCenterSplitter.Width - 1;
							btnCenterSplitterShadow.Left = Width - btnCenterSplitter.Width - 1;
						}
						if (btnCenterSplitter.Left < 0)
						{
							btnCenterSplitter.Left = 0;
							btnCenterSplitterShadow.Left = 0;
						}
						if (btnCenterSplitter.Top >= Height)
						{
							btnCenterSplitter.Top = Height - btnCenterSplitter.Height - 1;
							btnCenterSplitterShadow.Top = Height - btnCenterSplitter.Height - 1;
						}
						if (btnCenterSplitter.Top < 0)
						{
							btnCenterSplitter.Top = 0;
							btnCenterSplitterShadow.Top = 0;
						}
						XVPM.setLayout(handle,
									layout,
									new Rectangle(panRender.Left,panRender.Top,btnVerticalSplitter.Left - panRender.Left,btnHorizontalSplitter.Top - panRender.Top),
									new Rectangle(btnVerticalSplitter.Right,panRender.Top,panRender.Right - btnVerticalSplitter.Right,btnHorizontalSplitter.Top - panRender.Top),
									new Rectangle(panRender.Left,btnHorizontalSplitter.Bottom,btnVerticalSplitter.Left - panRender.Left,panRender.Bottom - btnHorizontalSplitter.Bottom),
									new Rectangle(btnVerticalSplitter.Right,btnHorizontalSplitter.Bottom,panRender.Right - btnVerticalSplitter.Right,panRender.Bottom - btnHorizontalSplitter.Bottom));

					bar0.Left = panRender.Left;
					bar0.Top = panRender.Top;
					bar0.Width = btnVerticalSplitter.Left - panRender.Left;
					bar1.Left = btnVerticalSplitter.Right;
					bar1.Top = panRender.Top;
					bar1.Width = panRender.Right - btnVerticalSplitter.Right;
					bar2.Left = panRender.Left;
					bar2.Top = btnHorizontalSplitter.Bottom;
					bar2.Width = btnVerticalSplitter.Left - panRender.Left;
					bar3.Left = btnVerticalSplitter.Right;
					bar3.Top = btnHorizontalSplitter.Bottom;
					bar3.Width = panRender.Right - btnVerticalSplitter.Right;

					bar0.Show();
					bar1.Show();
					bar2.Show();
					bar3.Show();

					btnHorizontalSplitter.Show();
					btnHorizontalSplitterShadow.Show();
					btnHorizontalSplitter.BringToFront();
					btnVerticalSplitter.Show();
					btnVerticalSplitterShadow.Show();
					btnVerticalSplitter.BringToFront();
					btnCenterSplitter.Show();
					btnCenterSplitterShadow.Show();
					btnCenterSplitter.BringToFront();

					break;
				}
				m_activeLayout = layout;
				m_viewportConfig.Layout = layout;
			}
			

			setActiveViewport(m_activeViewport);
		}

		public void resetViewports()
		{
			cbViewType0.SelectedItem = ciPerspective0;
			cbViewType1.SelectedItem = ciTop1;
			cbViewType2.SelectedItem = ciLeft2;
			cbViewType3.SelectedItem = ciFront3;
			
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.resetViewports(handle);
			}
		}

		private void btnHorizontalSplitter_MouseDown(object sender,MouseEventArgs e)
		{
			m_movingHS = true;
			m_hsClickY = e.Y;

			//Clear active viewport temporarily while resizeing.  Then restore it later.
			m_tempActiveViewport = m_activeViewport;
			setActiveViewport(-1);
		}

		private void btnHorizontalSplitter_MouseMove(object sender, MouseEventArgs e)
		{
			if (m_movingHS)
			{
				int newTop = btnCenterSplitter.Top + e.Y - m_hsClickY;
				if (newTop < panRender.Top)
					btnCenterSplitter.Top = panRender.Top;
				else if (newTop + btnCenterSplitter.Height > panRender.Top + panRender.Height)
					btnCenterSplitter.Top = panRender.Top + panRender.Height - btnCenterSplitter.Height;
				else
					btnCenterSplitter.Top = newTop;


				newTop = btnHorizontalSplitter.Top + e.Y - m_hsClickY;
				if (newTop < panRender.Top)
					btnHorizontalSplitter.Top = panRender.Top;
				else if (newTop + btnHorizontalSplitter.Height > panRender.Top + panRender.Height)
					btnHorizontalSplitter.Top = panRender.Top + panRender.Height - btnHorizontalSplitter.Height;
				else
					btnHorizontalSplitter.Top = newTop;

				btnCenterSplitterShadow.Refresh();
				btnHorizontalSplitterShadow.Refresh();
				btnVerticalSplitterShadow.Refresh();
				btnCenterSplitter.Refresh();
				btnHorizontalSplitter.Refresh();
				btnVerticalSplitter.Refresh();
				bar0.Refresh();
				bar1.Refresh();
				bar2.Refresh();
				bar3.Refresh();
			}
		}

		private void btnHorizontalSplitter_MouseUp(object sender, MouseEventArgs e)
		{
			m_movingHS = false;
			btnHorizontalSplitterShadow.Location = btnHorizontalSplitter.Location;
			btnCenterSplitterShadow.Location = btnCenterSplitter.Location;

			setLayout(m_activeLayout);
			
			//Restore active viewport
			setActiveViewport(m_tempActiveViewport);

		}

		private void btnVerticalSplitter_MouseDown(object sender,MouseEventArgs e)
		{
			m_movingVS = true;
			m_vsClickX = e.X;

			//Clear active viewport temporarily while resizeing.  Then restore it later.
			m_tempActiveViewport = m_activeViewport;
			setActiveViewport(-1);
		}

		private void btnVerticalSplitter_MouseMove(object sender,MouseEventArgs e)
		{
			if (m_movingVS)
			{
				int newLeft = btnCenterSplitter.Left + e.X - m_vsClickX;
				if (newLeft < panRender.Left)
					btnCenterSplitter.Left = panRender.Left;
				else if (newLeft + btnCenterSplitter.Width > panRender.Left + panRender.Width)
					btnCenterSplitter.Left = panRender.Left + panRender.Width - btnCenterSplitter.Width;
				else
					btnCenterSplitter.Left = newLeft;

				newLeft = btnVerticalSplitter.Left + e.X - m_vsClickX;
				if (newLeft < panRender.Left)
					btnVerticalSplitter.Left = panRender.Left;
				else if (newLeft + btnVerticalSplitter.Width > panRender.Left + panRender.Width)
					btnVerticalSplitter.Left = panRender.Left + panRender.Width - btnVerticalSplitter.Width;
				else
					btnVerticalSplitter.Left = newLeft;

				btnCenterSplitterShadow.Refresh();
				btnHorizontalSplitterShadow.Refresh();
				btnVerticalSplitterShadow.Refresh();
				btnCenterSplitter.Refresh();
				btnHorizontalSplitter.Refresh();
				btnVerticalSplitter.Refresh();
				bar0.Refresh();
				bar1.Refresh();
				bar2.Refresh();
				bar3.Refresh();
			}
		}

		private void btnVerticalSplitter_MouseUp(object sender,MouseEventArgs e)
		{
			m_movingVS = false;
			btnVerticalSplitterShadow.Location = btnVerticalSplitter.Location;
			btnCenterSplitterShadow.Location = btnCenterSplitter.Location;

			setLayout(m_activeLayout);

			//Restore active viewport
			setActiveViewport(m_tempActiveViewport);

		}

		private void btnCenterSplitter_MouseDown(object sender,MouseEventArgs e)
		{
			m_vsClickX = e.X;
			m_hsClickY = e.Y;
			m_movingCS = true;

			//Clear active viewport temporarily while resizeing.  Then restore it later.
			m_tempActiveViewport = m_activeViewport;
			setActiveViewport(-1);
		}

		private void btnCenterSplitter_MouseMove(object sender,MouseEventArgs e)
		{
			if (m_movingCS)
			{

				int newTop = btnHorizontalSplitter.Top + e.Y - m_hsClickY;
				if (newTop < panRender.Top)
					btnHorizontalSplitter.Top = panRender.Top;
				else if (newTop + btnHorizontalSplitter.Height > panRender.Top + panRender.Height)
					btnHorizontalSplitter.Top = panRender.Top + panRender.Height - btnHorizontalSplitter.Height;
				else
					btnHorizontalSplitter.Top = newTop;

				int newLeft = btnVerticalSplitter.Left + e.X - m_vsClickX;
				if (newLeft < panRender.Left)
					btnVerticalSplitter.Left = panRender.Left;
				else if (newLeft + btnVerticalSplitter.Width > panRender.Left + panRender.Width)
					btnVerticalSplitter.Left = panRender.Left + panRender.Width - btnVerticalSplitter.Width;
				else
					btnVerticalSplitter.Left = newLeft;

				newLeft = btnCenterSplitter.Left + e.X - m_vsClickX;
				if (newLeft < panRender.Left)
					btnCenterSplitter.Left = panRender.Left;
				else if (newLeft + btnCenterSplitter.Width > panRender.Left + panRender.Width)
					btnCenterSplitter.Left = panRender.Left + panRender.Width - btnCenterSplitter.Width;
				else
					btnCenterSplitter.Left = newLeft;

				newTop = btnCenterSplitter.Top + e.Y - m_hsClickY;
				if (newTop < panRender.Top)
					btnCenterSplitter.Top = panRender.Top;
				else if (newTop + btnCenterSplitter.Height > panRender.Top + panRender.Height)
					btnCenterSplitter.Top = panRender.Top + panRender.Height - btnCenterSplitter.Height;
				else
					btnCenterSplitter.Top = newTop;

				btnCenterSplitterShadow.Refresh();
				btnHorizontalSplitterShadow.Refresh();
				btnVerticalSplitterShadow.Refresh();
				btnCenterSplitter.Refresh();
				btnHorizontalSplitter.Refresh();
				btnVerticalSplitter.Refresh();
				bar0.Refresh();
				bar1.Refresh();
				bar2.Refresh();
				bar3.Refresh();
				
			}

		}

		private void btnCenterSplitter_MouseUp(object sender,MouseEventArgs e)
		{
			m_movingCS = false;
			btnHorizontalSplitterShadow.Location = btnHorizontalSplitter.Location;
			btnVerticalSplitterShadow.Location = btnVerticalSplitter.Location;
			btnCenterSplitterShadow.Location = btnCenterSplitter.Location;
			this.Focus();

			setLayout(m_activeLayout);

			//Restore active viewport
			setActiveViewport(m_tempActiveViewport);
		}

		private void XVPMControl_Paint(object sender,PaintEventArgs e)
		{
			
	/*		Pen penCurrent = new Pen(Color.Black);

			e.Graphics.DrawRectangle(penCurrent,new Rectangle(20,20,248,162));*/

		}

		private void panRender_Paint(object sender,PaintEventArgs e)
		{
			/*e.ClipRectangle = Rectangle.Empty;
			Pen penCurrent = new Pen(Color.Black);

			e.Graphics.DrawRectangle(penCurrent,new Rectangle(20,20,248,162));
			 */
			/*lock (Program.g_mainForm.XVPMLockObject)
			{
				if (XVPM.inited())
					XVPM.update();
			}*/
			
		}

		private void panRender_MouseMove(object sender,MouseEventArgs e)
		{
			if (m_enableCameraMovement)
			{
				//The ratios below were formulated by examining the relationships between mouse movement and movement in world
				//space when taking into account camera zoom.  Sort of a trial and error process but eventually
				//it ended up very nice. E.g... The pan delta remains fairly constant regardless of zoom.
				//Also, zoom is tame at both up close viewports and far away.  


				lock (Program.g_mainForm.XVPMLockObject)
				{
					//Only one movement allowed at a time
					if (m_rotateCamera)
					{
						XVPM.addCameraRotation(handle,(uint)m_activeViewport,-(e.X - m_lastMouseX) * .003f,(e.Y - m_lastMouseY) * .003f);

						Utility.Vector2D v = new Utility.Vector2D();
						IntPtr buffer = Marshal.AllocHGlobal(Marshal.SizeOf(v));
						XVPM.getCameraRotation(handle,m_activeViewport,buffer);
						Marshal.PtrToStructure(buffer,v);
						Marshal.FreeHGlobal(buffer);
						m_viewportConfig.setCameraRotation(m_activeViewport,v);
					}
					else if (m_panCamera)
					{
						XVPM.addCameraPan(handle,(uint)m_activeViewport,m_lastMouseX,m_lastMouseY,e.X,e.Y);

						Utility.Vector3D v = new Utility.Vector3D();
						IntPtr buffer = Marshal.AllocHGlobal(Marshal.SizeOf(v));
						XVPM.getCameraPan(handle,m_activeViewport,buffer);
						Marshal.PtrToStructure(buffer,v);
						Marshal.FreeHGlobal(buffer);
						m_viewportConfig.setCameraPan(m_activeViewport,v);
					}
					else if (m_zoomCamera)
					{
						XVPM.addCameraZoom(handle,(uint)m_activeViewport,-(e.X - m_lastMouseX));
						m_viewportConfig.setCameraZoom(m_activeViewport,XVPM.getCameraZoom(handle,m_activeViewport));
					}
				}


				//Once the alt key is released, cease input.
				if (MainForm.ModifierKeys != Keys.Alt)
					enableMovement(false);
			}

			//Update gizmo hover
			//lock (Program.g_mainForm.XVPMLockObject)
			if (!m_enableCameraMovement)
			{
				lock (Program.g_mainForm.XVPMLockObject)
				{
					XVPM.updateGizmoHover(handle,e.X,e.Y);
				}
			}

			if (!m_enableCameraMovement)
			{
				lock (Program.g_mainForm.XVPMLockObject)
				{
					//XVPM.updateGizmoDrag(handle,(e.X - m_lastMouseX) * (XVPM.getCameraZoom(handle,(uint)m_activeViewport) / 1000.0f),(e.Y - m_lastMouseY) * (XVPM.getCameraZoom(handle,(uint)m_activeViewport) / 1000.0f));
					XVPM.updateGizmoDrag(handle,m_lastMouseX,m_lastMouseY,e.X,e.Y);
				}
			}

			if (MainForm.ModifierKeys != Keys.Control)
				setControlPressed(false);

			m_lastMouseX = e.X;
			m_lastMouseY = e.Y;
		}

		private void panRender_MouseWheel(object sender,MouseEventArgs e)
		{
			XVPM.addCameraZoom(handle,(uint)m_activeViewport,e.Delta * 2.0f);
		}

		private int getViewportIndexFromMouseClick(int x,int y)
		{
			//Switch based on our current view 
			switch (m_activeLayout)
			{
				case 0:
					return 0;
				case 1:
					if (x >= panRender.Left && x <= btnVerticalSplitter.Left)
						return 0;
					else
						return 1;
				case 2:
					if (y >= panRender.Top && y <= btnHorizontalSplitter.Top)
						return 0;
					else
						return 1;
				case 3:
					if ((x >= panRender.Left && x <= btnVerticalSplitter.Left) &&
						(y >= panRender.Top && y <= btnHorizontalSplitter.Top))
						return 0;
					else if ((x >= btnVerticalSplitter.Right && x <= panRender.Right) &&
							(y >= panRender.Top && y <= btnHorizontalSplitter.Top))
						return 1;
					else if ((x >= panRender.Left && x <= btnVerticalSplitter.Left) &&
							(y >= btnHorizontalSplitter.Top && y <= panRender.Bottom))
						return 2;
					else 
						return 3;
			}

			throw new Exception("Couldn't calculate viewport index from mouse click!");
		}

		void setActiveViewport(int viewportIndex)
		{
			//If we get a -1, that means deselect the currently highlighted viewport
			if (viewportIndex < 0)
			{
				btnHighlightLeft.Visible =
				btnHighlightTop.Visible =
				btnHighlightRight.Visible =
				btnHighlightBottom.Visible = false;
				m_activeViewport = -1;
				return;
			}

			btnHighlightLeft.Hide();
			btnHighlightTop.Hide();
			btnHighlightRight.Hide();
			btnHighlightBottom.Hide();
			
			//Switch based on our current view and set the active viewport 
			switch (m_activeLayout)
			{
				case 0:
					btnHighlightLeft.Left = panRender.Left;
					btnHighlightLeft.Top = bar0.Bottom;
					btnHighlightLeft.Height = panRender.Height;
					btnHighlightTop.Left = panRender.Left;
					btnHighlightTop.Top = bar0.Bottom;
					btnHighlightTop.Width = panRender.Width;
					btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
					btnHighlightRight.Top = bar0.Bottom;
					btnHighlightRight.Height = panRender.Height;
					btnHighlightBottom.Left = panRender.Left;
					btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
					btnHighlightBottom.Width = panRender.Width;
					m_activeViewport = 0;
					break;
				case 1:
					switch (viewportIndex)
					{
						case 0:
							btnHighlightLeft.Left = panRender.Left;
							btnHighlightLeft.Top = bar0.Bottom;
							btnHighlightLeft.Height = panRender.Height;
							btnHighlightTop.Left = panRender.Left;
							btnHighlightTop.Top = bar0.Bottom;
							btnHighlightTop.Width = btnVerticalSplitter.Left - panRender.Left;
							btnHighlightRight.Left = btnVerticalSplitter.Left - btnHighlightRight.Width;
							btnHighlightRight.Top = bar0.Bottom;
							btnHighlightRight.Height = panRender.Height;
							btnHighlightBottom.Left = panRender.Left;
							btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
							btnHighlightBottom.Width = btnVerticalSplitter.Left - panRender.Left;
							m_activeViewport = 0;
							break;
						case 1:
							btnHighlightLeft.Left = btnVerticalSplitter.Right;
							btnHighlightLeft.Top = bar1.Bottom;
							btnHighlightLeft.Height = panRender.Height;
							btnHighlightTop.Left = btnVerticalSplitter.Right;
							btnHighlightTop.Top = bar1.Bottom;
							btnHighlightTop.Width = panRender.Right - btnVerticalSplitter.Right;
							btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
							btnHighlightRight.Top = bar1.Bottom;
							btnHighlightRight.Height = panRender.Height;
							btnHighlightBottom.Left = btnVerticalSplitter.Right;
							btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
							btnHighlightBottom.Width = panRender.Right - btnVerticalSplitter.Right;
							m_activeViewport = 1;
							break;
					}
					break;
				case 2:
					switch (viewportIndex)
					{
						case 0:
							btnHighlightLeft.Left = panRender.Left;
							btnHighlightLeft.Top = bar0.Bottom;
							btnHighlightLeft.Height = btnHorizontalSplitter.Top - panRender.Top - bar0.Height;
							btnHighlightTop.Left = panRender.Left;
							btnHighlightTop.Top = bar0.Bottom;
							btnHighlightTop.Width = panRender.Width;
							btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
							btnHighlightRight.Top = bar0.Bottom;
							btnHighlightRight.Height = btnHorizontalSplitter.Top - panRender.Top - bar0.Height;
							btnHighlightBottom.Left = panRender.Left;
							btnHighlightBottom.Top = btnHorizontalSplitter.Top - btnHighlightBottom.Height;
							btnHighlightBottom.Width = panRender.Width;
							m_activeViewport = 0;
							break;
						case 1:
							btnHighlightLeft.Left = panRender.Left;
							btnHighlightLeft.Top = bar1.Bottom;
							btnHighlightLeft.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightTop.Left = panRender.Left;
							btnHighlightTop.Top = bar1.Bottom;
							btnHighlightTop.Width = panRender.Width;
							btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
							btnHighlightRight.Top = bar1.Bottom;
							btnHighlightRight.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightBottom.Left = panRender.Left;
							btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
							btnHighlightBottom.Width = panRender.Width;
							m_activeViewport = 1;
							break;
					}
					break;
				case 3:
					switch (viewportIndex)
					{
						case 0:
							btnHighlightLeft.Left = panRender.Left;
							btnHighlightLeft.Top = bar0.Bottom;
							btnHighlightLeft.Height = btnHorizontalSplitter.Top - panRender.Top - bar0.Height;
							btnHighlightTop.Left = panRender.Left;
							btnHighlightTop.Top = bar0.Bottom;
							btnHighlightTop.Width = btnVerticalSplitter.Left - panRender.Left;
							btnHighlightRight.Left = btnVerticalSplitter.Left - btnHighlightRight.Width;
							btnHighlightRight.Top = bar0.Bottom;
							btnHighlightRight.Height = btnHorizontalSplitter.Top - panRender.Top - bar0.Height;
							btnHighlightBottom.Left = panRender.Left;
							btnHighlightBottom.Top = btnHorizontalSplitter.Top - btnHighlightBottom.Height;
							btnHighlightBottom.Width = btnVerticalSplitter.Left - panRender.Left;
							m_activeViewport = 0;
							break;
						case 1:
							btnHighlightLeft.Left = btnVerticalSplitter.Right;
							btnHighlightLeft.Top = bar1.Bottom;
							btnHighlightLeft.Height = btnHorizontalSplitter.Top - panRender.Top - bar1.Height;
							btnHighlightTop.Left = btnVerticalSplitter.Right;
							btnHighlightTop.Top = bar1.Bottom;
							btnHighlightTop.Width = panRender.Right - btnVerticalSplitter.Right;
							btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
							btnHighlightRight.Top = bar1.Bottom;
							btnHighlightRight.Height = btnHorizontalSplitter.Top - panRender.Top - bar1.Height;
							btnHighlightBottom.Left = btnVerticalSplitter.Right;
							btnHighlightBottom.Top = btnHorizontalSplitter.Top - btnHighlightBottom.Height;
							btnHighlightBottom.Width = panRender.Right - btnVerticalSplitter.Right;
							m_activeViewport = 1;
							break;
						case 2:
							btnHighlightLeft.Left = panRender.Left;
							btnHighlightLeft.Top = bar2.Bottom;
							btnHighlightLeft.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightTop.Left = panRender.Left;
							btnHighlightTop.Top = bar2.Bottom;
							btnHighlightTop.Width = btnVerticalSplitter.Left - panRender.Left;
							btnHighlightRight.Left = btnVerticalSplitter.Left - btnHighlightRight.Width;
							btnHighlightRight.Top = bar2.Bottom;
							btnHighlightRight.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightBottom.Left = panRender.Left;
							btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
							btnHighlightBottom.Width = btnVerticalSplitter.Left - panRender.Left;
							m_activeViewport = 2;
							break;
						case 3:
							btnHighlightLeft.Left = btnVerticalSplitter.Right;
							btnHighlightLeft.Top = bar3.Bottom;
							btnHighlightLeft.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightTop.Left = btnVerticalSplitter.Right;
							btnHighlightTop.Top = bar3.Bottom;
							btnHighlightTop.Width = panRender.Right - btnVerticalSplitter.Right;
							btnHighlightRight.Left = panRender.Right - btnHighlightRight.Width;
							btnHighlightRight.Top = bar3.Bottom;
							btnHighlightRight.Height = panRender.Bottom - btnHorizontalSplitter.Bottom;
							btnHighlightBottom.Left = btnVerticalSplitter.Right;
							btnHighlightBottom.Top = panRender.Bottom - btnHighlightBottom.Height;
							btnHighlightBottom.Width = panRender.Right - btnVerticalSplitter.Right;
							m_activeViewport = 3;
							break;
					}
					break;
			}

			btnHighlightLeft.Show();
			btnHighlightTop.Show();
			btnHighlightRight.Show();
			btnHighlightBottom.Show();

			//Make sure highlight is visible
			//btnHighlightLeft.Visible =
			//btnHighlightTop.Visible =
			//btnHighlightRight.Visible =
			//btnHighlightBottom.Visible = true;
		}

		private void panRender_SizeChanged(object sender,EventArgs e)
		{
			//foreach (Control c in panRender.Controls)
			//	c.Hide();			

			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Keep width and height reasonable
				/*uint finalWidth = (uint)Width;
				if (finalWidth < 32)
					finalWidth = 32;
				uint finalHeight = (uint)Height;
				if (finalHeight < 32)
					finalHeight = 32;*/
				XVPM.resizeWindow(handle,(uint)Width,(uint)Height);

				//Resize highlight depending on the active viewport
				/*switch (m_activeLayoutport)
				{
					case 0:
						btnHighlightLeft.Left = Left;
						btnHighlightLeft.Top = Top + tsXVPMControl.Height;
						btnHighlightLeft = btnVerticalSplitter.Left;
						btnHighlightLeft.Left = btnVerticalSplitter.Left;

						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
				}*/
			}

			setLayout(m_activeLayout);

			//setViewportArray();
			//setActiveViewport(m_activeViewport);	
		}

		/*
		private void setViewportArray()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				switch (m_activeLayout)
				{
					case 0:
						m_XVPM.setLayout(handle,m_activeLayout,
								new Rectangle(panRender.Left,panRender.Top,panRender.Right - panRender.Left,panRender.Bottom - panRender.Top),
								new Rectangle(0,0,0,0),
								new Rectangle(0,0,0,0),
								new Rectangle(0,0,0,0));

						

						break;
					case 1:
						m_XVPM.setLayout(handle,m_activeLayout,
								new Rectangle(panRender.Left,panRender.Top,btnVerticalSplitter.Left - panRender.Left,panRender.Bottom - panRender.Top),
								new Rectangle(btnVerticalSplitter.Right,panRender.Top,panRender.Right - btnVerticalSplitter.Right,panRender.Bottom - panRender.Top),
								new Rectangle(0,0,0,0),
								new Rectangle(0,0,0,0));

						

						break;
					case 2:
						m_XVPM.setLayout(handle,m_activeLayout,
								new Rectangle(panRender.Left,panRender.Top,panRender.Right - panRender.Left,btnHorizontalSplitter.Top - panRender.Top),
								new Rectangle(panRender.Left,btnHorizontalSplitter.Bottom,panRender.Right - panRender.Left,panRender.Bottom - btnHorizontalSplitter.Bottom),
								new Rectangle(0,0,0,0),
								new Rectangle(0,0,0,0));

					


						break;
					case 3:
						m_XVPM.setLayout(handle,m_activeLayout,
								new Rectangle(panRender.Left,panRender.Top,btnVerticalSplitter.Left - panRender.Left,btnHorizontalSplitter.Top - panRender.Top),
								new Rectangle(btnVerticalSplitter.Right,panRender.Top,panRender.Right - btnVerticalSplitter.Right,btnHorizontalSplitter.Top - panRender.Top),
								new Rectangle(panRender.Left,btnHorizontalSplitter.Bottom,btnVerticalSplitter.Left - panRender.Left,panRender.Bottom - btnHorizontalSplitter.Bottom),
								new Rectangle(btnVerticalSplitter.Right,btnHorizontalSplitter.Bottom,panRender.Right - btnVerticalSplitter.Right,panRender.Bottom - btnHorizontalSplitter.Bottom));

						

						break;
				}
			}
		}*/

		public bool isMovementEnabled()
		{
			return m_enableCameraMovement;
		}

		public void enableMovement(bool enable)
		{
			m_enableCameraMovement = enable;	
		}

		public void setControlPressed(bool pressed)
		{
			m_controlPressed = pressed;
		}
		
		private void tsmiFullView_Click(object sender,EventArgs e)
		{
			setLayout(0);
		}

		private void tsmiVerticalSplitView_Click(object sender,EventArgs e)
		{
			setLayout(1);
		}

		private void tsmiHorizontalSplitView_Click(object sender,EventArgs e)
		{
			setLayout(2);
		}

		private void tsmiQuadView_Click(object sender,EventArgs e)
		{
			setLayout(3);
		}

		public void setBackgroundColor(Color color)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setBackgroundColor(handle,color.R/255.0f,color.G/255.0f,color.B/255.0f);
			}
			m_viewportConfig.BackgroundColor = color;
		}

		public void configureGrid(float unitSize,uint gridSize,uint majorLine)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.configureGrid(handle,unitSize,gridSize,majorLine);
			}
			m_viewportConfig.UnitSize = unitSize;
			m_viewportConfig.GridSize = gridSize;
			m_viewportConfig.MajorLine = majorLine;

		}
		private void panRender_MouseClick(object sender,MouseEventArgs e)
		{
			Focus();
		}

		private void cmsViewports_Opening(object sender,System.ComponentModel.CancelEventArgs e)
		{
			if (m_enableCameraMovement)
				e.Cancel = true;
		}

		private void resetAllViewportsToolStripMenuItem_Click(object sender,EventArgs e)
		{
			DialogResult dr = MessageBox.Show("All viewports will be returned to their default configuration.  Do you want to continue?","Reset All Viewports",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (dr == DialogResult.No)
				return;

			resetViewports();
		}

		private void XVPMControl_Resize(object sender,EventArgs e)
		{
			Visible = true;
		}

		private void XVPMControl_Layout(object sender,LayoutEventArgs e)
		{
			Visible = false;
		}

		private void btnShowGrid_Click(object sender,EventArgs e)
		{
			ButtonItem button = sender as ButtonItem;
			uint viewportIndex = uint.Parse(button.Tag.ToString());
			button.Checked = !button.Checked;
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.showGrid(handle,viewportIndex,button.Checked);
			}
			m_viewportConfig.setShowGrid((int)viewportIndex,button.Checked);
		}

		private void btnUserStandardCamera_Click(object sender,EventArgs e)
		{
			ButtonItem button = sender as ButtonItem;
			uint viewportIndex = uint.Parse(button.Tag.ToString());
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.setStandardCamera(handle,viewportIndex);
			}
			switch (viewportIndex)
			{
				case 0:
					btnCameraName0.Text = "Default";
					break;
				case 1:
					btnCameraName1.Text = "Default";
					break;
				case 2:
					btnCameraName2.Text = "Default";
					break;
				case 3:
					btnCameraName3.Text = "Default";
					break;
			}
			
			m_viewportConfig.setSelectedCameraGUID((int)viewportIndex,null);
		}

		private void setUserCamera(string GUID,uint viewportIndex)
		{
			CameraProvider cp = Program.g_mainForm.ActiveSceneExplorer.getNode(GUID) as CameraProvider;
			if (cp != null)
			{
				string name = cp.setAsCamera(viewportIndex);

				//Did we succeed in setting the camera?  If so, what's it's name
				/*if (name != null)
				{
					switch (viewportIndex)
					{
						case 0:
							lblCameraType0.Text = name;
							m_viewportConfig.SelectedCameraGUID1 = GUID;
							break;
						case 1:
							lblCameraType1.Text = name;
							m_viewportConfig.SelectedCameraGUID2 = GUID;
							break;
						case 2:
							lblCameraType2.Text = name;
							m_viewportConfig.SelectedCameraGUID3 = GUID;
							break;
						case 3:
							lblCameraType3.Text = name;
							m_viewportConfig.SelectedCameraGUID4 = GUID;
							break;
					}

				}*/
			}
		}

		private void btnUserSelectedCamera_Click(object sender,EventArgs e)
		{
			uint viewportIndex = uint.Parse((sender as ButtonItem).Tag.ToString());

			//Get root of selected hierarchy and check for a camera provider there
			TreeNode root = Program.g_mainForm.ActiveSceneExplorer.getHierarchyTreeView().SelectedNode;
			if (root == null)
				return;
			else
			{
				while (root.Parent != null)
					root = root.Parent;
			}

			CameraProvider cp = Program.g_mainForm.ActiveSceneExplorer.getNode(root.Name) as CameraProvider;
			if (cp != null)
			{
				string name = cp.setAsCamera(viewportIndex);

				//Did we succeed in setting the camera?  If so, what's it's name
				if (name != null)
				{
					switch (viewportIndex)
					{
						case 0:
							btnCameraName0.Text = name;
							break;
						case 1:
							btnCameraName1.Text = name;
							break;
						case 2:
							btnCameraName2.Text = name;
							break;
						case 3:
							btnCameraName3.Text = name;
							break;
					}
					m_viewportConfig.setSelectedCameraGUID((int)viewportIndex,root.Name);
				}
			}
		}

		private void setViewType(string viewType,uint viewportIndex)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				switch (viewType)
				{
					case "Perspective":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.PERSPECTIVE),true);
						break;
					case "Left":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.LEFT),true);
						break;
					case "Top":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.TOP),true);
						break;
					case "Right":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.RIGHT),true);
						break;
					case "Bottom":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.BOTTOM),true);
						break;
					case "Front":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.FRONT),true);
						break;
					case "Back":
						XVPM.setViewType(handle,viewportIndex,Convert.ToInt32(ViewType.BACK),true);
						break;
				}
			}

			switch (viewportIndex)
			{
				case 0:
					m_viewportConfig.setCameraView(0,viewType);
					break;
				case 1:
					m_viewportConfig.setCameraView(1,viewType);
					break;
				case 2:
					m_viewportConfig.setCameraView(2,viewType);
					break;
				case 3:
					m_viewportConfig.setCameraView(3,viewType);
					break;
			}
		}

		private void setViewTypeCombo(string viewType,int viewportIndex)
		{
			switch (viewType)
			{
				case "Perspective":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciPerspective0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciPerspective1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciPerspective2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciPerspective3;
							break;
					}
					break;
				case "Left":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciLeft0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciLeft1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciLeft2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciLeft3;
							break;
					} 
					break;
				case "Top":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciTop0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciTop1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciTop2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciTop3;
							break;
					}
					break;
				case "Right":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciRight0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciRight1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciRight2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciRight3;
							break;
					}
					break;
				case "Bottom":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciBottom0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciBottom1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciBottom2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciBottom3;
							break;
					}
					break;
				case "Front":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciFront0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciFront1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciFront2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciFront3;
							break;
					}
					break;
				case "Back":
					switch (viewportIndex)
					{
						case 0:
							cbViewType0.SelectedItem = ciBack0;
							break;
						case 1:
							cbViewType1.SelectedItem = ciBack1;
							break;
						case 2:
							cbViewType2.SelectedItem = ciBack2;
							break;
						case 3:
							cbViewType3.SelectedItem = ciBack3;
							break;
					}
					break;
			}
		}

		private void cbViewType0_ComboBoxTextChanged(object sender,EventArgs e)
		{
			setViewType(cbViewType0.SelectedItem.ToString(),0);
		}

		private void cbViewType1_ComboBoxTextChanged(object sender,EventArgs e)
		{
			setViewType(cbViewType1.SelectedItem.ToString(),1);
		}

		private void cbViewType2_ComboBoxTextChanged(object sender,EventArgs e)
		{
			setViewType(cbViewType2.SelectedItem.ToString(),2);
		}

		private void cbViewType3_ComboBoxTextChanged(object sender,EventArgs e)
		{
			setViewType(cbViewType3.SelectedItem.ToString(),3);
		}		
		
		public void applyViewportConfig(ViewportConfig viewportConfig)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				m_viewportConfig = viewportConfig;

				//Set any user cameras
				if (viewportConfig.getSelectedCameraGUID(0) != null)
					setUserCamera(viewportConfig.getSelectedCameraGUID(0),0);
				if (viewportConfig.getSelectedCameraGUID(1) != null)
					setUserCamera(viewportConfig.getSelectedCameraGUID(1),1);
				if (viewportConfig.getSelectedCameraGUID(2) != null)
					setUserCamera(viewportConfig.getSelectedCameraGUID(2),2);
				if (viewportConfig.getSelectedCameraGUID(3) != null)
					setUserCamera(viewportConfig.getSelectedCameraGUID(3),3);

				setBackgroundColor(viewportConfig.BackgroundColor);
				configureGrid(viewportConfig.UnitSize,viewportConfig.GridSize,viewportConfig.MajorLine);

				setLayout(viewportConfig.Layout);

				setViewType(viewportConfig.getCameraView(0),0);
				setViewTypeCombo(viewportConfig.getCameraView(0),0);
				setViewType(viewportConfig.getCameraView(1),1);
				setViewTypeCombo(viewportConfig.getCameraView(1),1);
				setViewType(viewportConfig.getCameraView(2),2);
				setViewTypeCombo(viewportConfig.getCameraView(2),2);
				setViewType(viewportConfig.getCameraView(3),3);
				setViewTypeCombo(viewportConfig.getCameraView(3),3);


				btnShowGrid0.Checked = viewportConfig.getShowGrid(0);
				btnShowGrid1.Checked = viewportConfig.getShowGrid(1);
				btnShowGrid2.Checked = viewportConfig.getShowGrid(2);
				btnShowGrid3.Checked = viewportConfig.getShowGrid(3);

				XVPM.showGrid(handle,0,btnShowGrid0.Checked);
				XVPM.showGrid(handle,1,btnShowGrid1.Checked);
				XVPM.showGrid(handle,2,btnShowGrid2.Checked);
				XVPM.showGrid(handle,3,btnShowGrid3.Checked);

				XVPM.setCameraZoom(handle,0,viewportConfig.getCameraZoom(0));
				XVPM.setCameraZoom(handle,1,viewportConfig.getCameraZoom(1));
				XVPM.setCameraZoom(handle,2,viewportConfig.getCameraZoom(2));
				XVPM.setCameraZoom(handle,3,viewportConfig.getCameraZoom(3));
				
				//Set camera pan
				IntPtr buffer = Marshal.AllocHGlobal(Marshal.SizeOf(viewportConfig.getCameraPan(0)));
				Marshal.StructureToPtr(viewportConfig.getCameraPan(0),buffer,false);
				XVPM.setCameraPan(handle,0,buffer);
				Marshal.StructureToPtr(viewportConfig.getCameraPan(1),buffer,false);
				XVPM.setCameraPan(handle,1,buffer);
				Marshal.StructureToPtr(viewportConfig.getCameraPan(2),buffer,false);
				XVPM.setCameraPan(handle,2,buffer);
				Marshal.StructureToPtr(viewportConfig.getCameraPan(3),buffer,false);
				XVPM.setCameraPan(handle,3,buffer);
				Marshal.FreeHGlobal(buffer);
				
				//Set camera rotation only if we are in perspective mode
				buffer = Marshal.AllocHGlobal(Marshal.SizeOf(viewportConfig.getCameraRotation(0)));
				if (viewportConfig.getCameraView(0) == "Perspective")
				{
					Marshal.StructureToPtr(viewportConfig.getCameraRotation(0),buffer,false);
					XVPM.setCameraRotation(handle,0,buffer);
				}
				if (viewportConfig.getCameraView(1) == "Perspective")
				{
					Marshal.StructureToPtr(viewportConfig.getCameraRotation(1),buffer,false);
					XVPM.setCameraRotation(handle,1,buffer);
				}
				if (viewportConfig.getCameraView(2) == "Perspective")
				{
					Marshal.StructureToPtr(viewportConfig.getCameraRotation(2),buffer,false);
					XVPM.setCameraRotation(handle,2,buffer);
				}
				if (viewportConfig.getCameraView(3) == "Perspective")
				{
					Marshal.StructureToPtr(viewportConfig.getCameraRotation(3),buffer,false);
					XVPM.setCameraRotation(handle,3,buffer);
				}
				Marshal.FreeHGlobal(buffer);

				
			}	
		}

		private void btnZoomSelection_Click(object sender,EventArgs e)
		{
			zoomSelection(int.Parse((sender as ButtonItem).Tag.ToString()));
		}

		public void zoomSelection(int viewportIndex)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				if (viewportIndex == -1)
					XVPM.zoomSelection(handle,(uint)m_activeViewport);
				else
					XVPM.zoomSelection(handle,(uint)viewportIndex);
			}
		}

		public void toggleLockSelection()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Can't lock a selection if there isn't one
				if (XVPM.getNumSelectedNodes(handle) != 0)
				{
					m_lockSelection = !m_lockSelection;
					if (m_lockSelection)
						XVPM.setSelectionColor(handle,1,.15f,.15f);
					else
						XVPM.setSelectionColor(handle,1,1,1);
				}
			}
		}
		
		private void btnResetCamera_Click(object sender,EventArgs e)
		{
			ButtonItem button = sender as ButtonItem;
			uint viewportIndex = uint.Parse(button.Tag.ToString());
			switch (viewportIndex)
			{
				case 0:
					 cbViewType0.SelectedItem = ciPerspective0;
					break;
				case 1:
					cbViewType1.SelectedItem = ciPerspective1;
					break;
				case 2:
					cbViewType2.SelectedItem = ciPerspective2;
					break;
				case 3:
					cbViewType3.SelectedItem = ciPerspective3;
					break;
			}
			
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.resetViewportCamera(handle,viewportIndex);
			}
		}

		private void btnMaximizeVP_Click(object sender,EventArgs e)
		{
			int x = 0;
			int y = 0;
			if (m_maximized)
			{
				x = m_CSPosBeforeMaximize.X - btnCenterSplitter.Left;
				y = m_CSPosBeforeMaximize.Y - btnCenterSplitter.Top;
				m_maximized = false;
			}
			else
			{
				x = 100000;
				y = 100000;
			
				m_CSPosBeforeMaximize.X = btnCenterSplitter.Left;
				m_CSPosBeforeMaximize.Y = btnCenterSplitter.Top;
				m_maximized = true;
			}

			//Simulate mouse dragging of the center splitter.  Basically simulate a huge 
			//cursor drag in the appropriate direction.
			MouseEventArgs args = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseDown(null,args);
			MouseEventArgs args2 = new MouseEventArgs(MouseButtons.Left,1,x,y,0);
			btnCenterSplitter_MouseMove(null,args2);
			MouseEventArgs args3 = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseUp(null,args3);
		}

		private void btnMaximizeVP1_Click(object sender,EventArgs e)
		{
			int x = 0;
			int y = 0;
			if (m_maximized)
			{
				x = m_CSPosBeforeMaximize.X - btnCenterSplitter.Left;
				y = m_CSPosBeforeMaximize.Y - btnCenterSplitter.Top;
				m_maximized = false;
			}
			else
			{
				x = -100000;

				if (m_activeLayout == 2)
					y = -100000;
				else
					y = 100000;

				m_CSPosBeforeMaximize.X = btnCenterSplitter.Left;
				m_CSPosBeforeMaximize.Y = btnCenterSplitter.Top;
				m_maximized = true;
			}

			//Simulate mouse dragging of the center splitter.  Basically simulate a huge 
			//cursor drag in the appropriate direction.
			MouseEventArgs args = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseDown(null,args);
			MouseEventArgs args2 = new MouseEventArgs(MouseButtons.Left,1,x,y,0);
			btnCenterSplitter_MouseMove(null,args2);
			MouseEventArgs args3 = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseUp(null,args3);
		}

		private void btnMaximizeVP2_Click(object sender,EventArgs e)
		{
			int x = 0;
			int y = 0;
			if (m_maximized)
			{
				x = m_CSPosBeforeMaximize.X - btnCenterSplitter.Left;
				y = m_CSPosBeforeMaximize.Y - btnCenterSplitter.Top;
				m_maximized = false;
			}
			else
			{
				x = 100000;
				y = -100000;

				m_CSPosBeforeMaximize.X = btnCenterSplitter.Left;
				m_CSPosBeforeMaximize.Y = btnCenterSplitter.Top;
				m_maximized = true;
			}

			//Simulate mouse dragging of the center splitter.  Basically simulate a huge 
			//cursor drag in the appropriate direction.
			MouseEventArgs args = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseDown(null,args);
			MouseEventArgs args2 = new MouseEventArgs(MouseButtons.Left,1,x,y,0);
			btnCenterSplitter_MouseMove(null,args2);
			MouseEventArgs args3 = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseUp(null,args3);
		}

		private void btnMaximizeVP3_Click(object sender,EventArgs e)
		{
			int x = 0;
			int y = 0;
			if (m_maximized)
			{
				x = m_CSPosBeforeMaximize.X - btnCenterSplitter.Left;
				y = m_CSPosBeforeMaximize.Y - btnCenterSplitter.Top;
				m_maximized = false;
			}
			else
			{
				x = -100000;
				y = -100000;

				m_CSPosBeforeMaximize.X = btnCenterSplitter.Left;
				m_CSPosBeforeMaximize.Y = btnCenterSplitter.Top;
				m_maximized = true;
			}

			//Simulate mouse dragging of the center splitter.  Basically simulate a huge 
			//cursor drag in the appropriate direction.
			MouseEventArgs args = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseDown(null,args);
			MouseEventArgs args2 = new MouseEventArgs(MouseButtons.Left,1,x,y,0);
			btnCenterSplitter_MouseMove(null,args2);
			MouseEventArgs args3 = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseUp(null,args3);
		}

		public void maximizeActiveVP()
		{
			int x = 0;
			int y = 0;
			if (m_maximized)
			{
				x = m_CSPosBeforeMaximize.X - btnCenterSplitter.Left;
				y = m_CSPosBeforeMaximize.Y - btnCenterSplitter.Top;
				m_maximized = false;
			}
			else
			{
				switch (m_activeViewport)
				{
					case 0:
						x = 100000;
						y = 100000;
						break;
					case 1:
						x = -100000;
						if (m_activeLayout == 2)
							y = -100000;
						else
							y = 100000;
						break;
					case 2:
						x = 100000;
						y = -100000;
						break;
					case 3:
						x = -100000;
						y = -100000;
						break;
				}
				m_CSPosBeforeMaximize.X = btnCenterSplitter.Left;
				m_CSPosBeforeMaximize.Y = btnCenterSplitter.Top;
				m_maximized = true;
			}

			//Simulate mouse dragging of the center splitter.  Basically simulate a huge 
			//cursor drag in the appropriate direction.
		
			MouseEventArgs args = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseDown(null,args);
			MouseEventArgs args2 = new MouseEventArgs(MouseButtons.Left,1,x,y,0);
			btnCenterSplitter_MouseMove(null,args2);
			MouseEventArgs args3 = new MouseEventArgs(MouseButtons.Left,1,0,0,0);
			btnCenterSplitter_MouseUp(null,args3);
		}

		private void btnWireShading_Click(object sender,EventArgs e)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.enableWireShading(handle,uint.Parse((sender as ButtonItem).Tag.ToString()),(sender as ButtonItem).Checked);
			}
		}

		private void panRender_DragOver(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			if (nodeSource != null)
			{
				switch (nodeSource.ImageKey)
				{
					case "Actor":
						e.Effect = DragDropEffects.Copy;
						break;
				}
			}
		}

		private void panRender_DragDrop(object sender,DragEventArgs e)
		{
			TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

			//Depending on the type of node we are dragging and dropping, check various things.
			switch (nodeSource.ImageKey)
			{
				case "Actor":
					Point clientLocation = panRender.PointToClient(new Point(e.X,e.Y));
					m_parentDoc.addActor(nodeSource.Name,nodeSource.Text,(uint)getViewportIndexFromMouseClick(clientLocation.X,clientLocation.Y),clientLocation.X,clientLocation.Y);
					//OnTexDrop(_texture.Name,nodeSource.Name);
					break;
			}	
		}

		public bool isMoveSnapEnabled()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				return XVPM.isMoveSnapEnabled(XVPMHandle);
			}
		}

		public void enableMoveSnap(bool enable)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.enableMoveSnap(XVPMHandle,enable);
			}
		}

		public bool isRotateSnapEnabled()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				return XVPM.isRotateSnapEnabled(XVPMHandle);
			}
		}

		public void enableRotateSnap(bool enable)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.enableRotateSnap(XVPMHandle,enable);
			}
		}
		
		public bool isScaleSnapEnabled()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				return XVPM.isScaleSnapEnabled(XVPMHandle);
			}
		}

		public void enableScaleSnap(bool enable)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.enableScaleSnap(XVPMHandle,enable);
			}
		}

		private void btnZoomAll_Click(object sender,EventArgs e)
		{
			zoomAll(int.Parse((sender as ButtonItem).Tag.ToString()));
		}

		public void zoomAll(int viewportIndex)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				if (viewportIndex == -1)
					XVPM.zoomAll(handle,(uint)m_activeViewport);
				else
					XVPM.zoomAll(handle,(uint)viewportIndex);
			}
		}

		private void tsmiSelectAll_Click(object sender,EventArgs e)
		{
			if (SelectionLocked)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.selectAll(handle);
			} 
		}

		private void invertSelectionToolStripMenuItem_Click(object sender,EventArgs e)
		{
			if (SelectionLocked)
				return;

			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.invertSelection(handle);
			} 
		}

		private void toggleLockToolStripMenuItem_Click(object sender,EventArgs e)
		{
			toggleLockSelection();
		}

		private void tsmiZoomSelection_Click(object sender,EventArgs e)
		{
			zoomSelection(m_activeViewport);
		}

		private void tsmiZoomAll_Click(object sender,EventArgs e)
		{
			zoomAll(m_activeViewport);
		}

	}
}
