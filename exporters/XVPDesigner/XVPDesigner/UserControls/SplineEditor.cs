using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.UserControls
{


		[DefaultEvent("PropertyChanged")]
		public partial class SplineEditor : UserControl,INotifyPropertyChanged
		{
			#region Fields
			private Pen _pen;
			private Pen _toolPen;
			private PointF[] _points;
			private bool _mouseDown;
			private int _pointClicked;
			#endregion

			public SplineEditor()
			{
				InitializeComponent();
				this.SetStyle(
					ControlStyles.AllPaintingInWmPaint |
					ControlStyles.UserPaint |
					ControlStyles.OptimizedDoubleBuffer,true);

				this.BackColor = SystemColors.AppWorkspace;
				this.BorderStyle = BorderStyle.Fixed3D;
			}

			#region Properties
			public event PropertyChangedEventHandler PropertyChanged;

			public PointF Point1
			{
				get { return _points[0]; }
			}
			public PointF Point2
			{
				get { return _points[1]; }
			}
			public PointF Point3
			{
				get { return _points[2]; }
			}
			public PointF Point4
			{
				get { return _points[3]; }
			}
			#endregion

			#region Methods
			protected override void OnLoad(EventArgs e)
			{
				base.OnLoad(e);
				_pen = new Pen(SystemColors.ControlLightLight);
				_toolPen = new Pen(Color.Red);

				float quarterx = this.Width / 4;
				float quartery = this.Height / 4;

				_points = new PointF[4] 
           {
               new PointF(2,this.Height-8),
               new PointF(quarterx, quartery),
               new PointF(quarterx*3, quartery*3),
               new PointF(this.Width-8, 2)
           };
				_pointClicked = 0;
			}

			protected override void OnPaint(PaintEventArgs e)
			{
				Graphics g = e.Graphics;
				//draw the control points
				for (int i = 0; i < _points.Length; i++)
				{
					g.DrawEllipse(
						_toolPen,
						_points[i].X - 2,
						_points[i].Y - 2,
						4,
						4);
				}

				//draw the handle from point 2 and 3
				g.DrawLine(_toolPen,_points[1],_points[2]);

				//draw the bezier line
				g.DrawBezier(_pen,_points[0],_points[1],_points[2],_points[3]);
			}

			protected override void OnMouseDown(MouseEventArgs e)
			{
				base.OnMouseDown(e);
				_mouseDown = true;

				float closest = float.MaxValue;
				for (int i = 0; i < _points.Length; i++)
				{
					PointF p = _points[i];
					PointF vector = new PointF(p.X - e.X,p.Y - e.Y);
					float lengthSquared = vector.X * vector.X + vector.Y * vector.Y;
					float length = (float)Math.Sqrt(lengthSquared);
					if (length < closest)
					{
						closest = length;
						_pointClicked = i;
					}
				}

				if (closest <= 10)
				{
					UpdateSelectedPoint(e);
				}
				else
				{
					//not close enough
					_mouseDown = false;
				}
			}

			protected override void OnMouseUp(MouseEventArgs e)
			{
				base.OnMouseUp(e);
				_mouseDown = false;
			}

			protected override void OnMouseMove(MouseEventArgs e)
			{
				base.OnMouseMove(e);

				if (_mouseDown)
				{
					UpdateSelectedPoint(e);
				}
			}

			private void UpdateSelectedPoint(MouseEventArgs e)
			{
				_points[_pointClicked].X = e.X;
				_points[_pointClicked].Y = e.Y;

				if (_points[_pointClicked].X < Left)
					_points[_pointClicked].X = Left;
				if (_points[_pointClicked].Y < Top)
					_points[_pointClicked].Y = Top;

				if (_points[_pointClicked].X > Right)
					_points[_pointClicked].X = Right;
				if (_points[_pointClicked].Y > Top)
					_points[_pointClicked].Y = Top;

				this.Invalidate();

				OnPropertyChanged();
			}

			protected void OnPropertyChanged()
			{
				PropertyChangedEventHandler e = this.PropertyChanged;
				if (e != null)
				{
					e(this,new PropertyChangedEventArgs("Point" + (_pointClicked - 1).ToString()));
				}
			}
			#endregion
		}
	



}
