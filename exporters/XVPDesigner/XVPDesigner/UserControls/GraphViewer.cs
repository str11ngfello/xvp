using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Utilities;
using System.Collections;
using System.ComponentModel.Design;

namespace XVPDesigner.UserControls
{
	[Serializable]
    public partial class GraphViewer : UserControl
    {
		private int nodeCtr = 0;
		const int maxNodeWidth = 250;
		const int forcedNodeWidth = 300;

		public class NodeShape
		{
			private UserControl m_ownerControl;
			private Graph<NodeShape> m_parentGraph;
			private Font m_font;
			private RectangleF m_mainRect;
			private RectangleF m_shadowRect;
			private GraphicsPath m_mainPath;
			private GraphicsPath m_shadowPath;
			private Brush m_shadowBrush;
			private StringFormat m_drawFormat = new StringFormat();
			Pen m_selectedPen = new Pen(Color.Blue, 2);



			private Brush m_gradientBrush;
			private Color m_nodeColor = Color.FromArgb(137, 163, 169);
			[CategoryAttribute("General"),
			DescriptionAttribute("Color used for the background gradient of the node shape.")]
			public Color NodeColor
			{
				get { return m_nodeColor; }
				set
				{
					m_gradientBrush = new LinearGradientBrush(new RectangleF(m_pos.X, m_pos.Y, m_size.Width + 5, m_size.Height + 5), m_nodeColor, Color.White, 0, false);
					m_nodeColor = value;
					rebuildGraphics();
				}
			}

			private Size m_size;
			[Browsable(false)] 
			public Size Size
			{
				get { return m_size; }
			}

			
			private PointF m_pos;
			[Browsable(false)]
			public PointF Pos
			{
				get { return m_pos; }
				set 
				{ 
					m_pos = value;

					rebuildGraphics();
				}
			}

			private string m_text = null;
			[CategoryAttribute("General"),
			Editor(typeof(MultilineStringEditor),typeof(System.Drawing.Design.UITypeEditor)),
			DescriptionAttribute("Main text displayed withing a node.")]
			public string Text
			{
				get { return m_text; }
				set 
				{ 
					m_text = value;

					Size size = m_ownerControl.CreateGraphics().MeasureString(m_text.Split('\n')[0],m_font).ToSize();
					m_size = size;

					//Inflate the box a little
					m_size.Width += 50;
					if (m_size.Width > maxNodeWidth)
						m_size.Width = forcedNodeWidth;
					m_size.Height *= 2;

					rebuildGraphics();

					//We also have to rename any connections
					/*foreach (ArrowShape arrow in m_arrowShapes)
					{
						if (node.Value == this)
						{
							node
						}
					}*/
				}
			}

			private bool m_selected = false;
			[CategoryAttribute("General"),
			DescriptionAttribute("Determines whether or not the node is selected.")]
			public bool Selected
			{
				get { return m_selected; }
				set { m_selected = value; }
			}
			
			public NodeShape(UserControl ownerControl,Graph<NodeShape> parentGraph,PointF pos,string text,Font font)
			{
				//We must assign the owner user control first
				m_ownerControl = ownerControl;
				m_parentGraph = parentGraph; 

				m_font = font;
				Pos = pos;
				Text = text;

				//Set format of string.
				m_drawFormat.Alignment = StringAlignment.Center;
				m_drawFormat.LineAlignment = StringAlignment.Center;
			}

			public void Dispose()
			{
				m_mainPath.Dispose();
				m_shadowPath.Dispose();
				m_gradientBrush.Dispose();
				m_shadowBrush.Dispose();
				m_drawFormat.Dispose();
				m_selectedPen.Dispose();
			}
			
			public void rebuildGraphics()
			{
				m_mainRect = new RectangleF(m_pos.X,m_pos.Y,m_size.Width,m_size.Height);
				m_shadowRect = new RectangleF(m_pos.X + 5,m_pos.Y + 5,m_size.Width,m_size.Height);

				m_mainPath = RoundedRectangle.Create(m_mainRect,6);
				m_shadowPath = RoundedRectangle.Create(m_shadowRect,6);

				m_gradientBrush = new LinearGradientBrush(new RectangleF(m_pos.X, m_pos.Y, m_size.Width + 5, m_size.Height + 5), m_nodeColor, Color.White, 0, false);
				m_shadowBrush = new SolidBrush(Color.FromArgb(64, 0, 0, 0));
			}

			public void draw(Graphics g)
			{


				g.FillPath(m_shadowBrush, m_shadowPath);
				g.FillPath(m_gradientBrush,m_mainPath);
				
				if (m_selected)
					g.DrawPath(m_selectedPen, m_mainPath);
				else
					g.DrawPath(Pens.Gray,m_mainPath);


				SizeF size = m_ownerControl.CreateGraphics().MeasureString(m_text,m_font).ToSize();
				if (size.Width > maxNodeWidth)
					g.DrawString(m_text.Substring(0,50)+ "...",m_font,Brushes.Black,m_mainRect,m_drawFormat);
				else
					g.DrawString(m_text,m_font,Brushes.Black,m_mainRect,m_drawFormat);


				//Matrix m = new Matrix(1,0,0,1,100,100z);

				/*path.Transform(m);
				path2.Transform(m);

				g.FillPath(Brushes.LightGray,path2);
				g.FillPath(m_gradientBrush,path);
				g.DrawPath(Pens.Gray,path);*/
				//g.DrawString("Class Shape", m_font, Brushes.Black, mainRect, drawFormat);

				
				
			}

			public bool containsPoint(PointF point)
			{
				return m_mainPath.IsVisible(point);
			}
		}

		public class ArrowShape
		{
			private string m_name = null;
			[ReadOnly(true)]
			public string Name
			{
				get { return m_name; }
				set { m_name = value; }
			}

			private GraphNode<NodeShape> m_fromNode = null;
			[Browsable(false)]
			public GraphNode<NodeShape> FromNode
			{
				get { return m_fromNode; }
				set { m_fromNode = value; }
			}

			private GraphNode<NodeShape> m_toNode = null;
			[Browsable(false)]
			public GraphNode<NodeShape> ToNode
			{
				get { return m_toNode; }
				set { m_toNode = value; }
			}

			private bool m_selected;
			[Browsable(false)]
			public bool Selected
			{
				get { return m_selected; }
				set { m_selected = value; }
			}

			public ArrowShape(string name)
			{
				m_name = name;
			}
		}


		bool m_translating = false;
		bool m_scaling = false;
		float m_translationX = 0;
		float m_translationY = 0;
		float m_scale = 1.0f;
		PointF m_startingPos;

		
		bool m_highlighting = false;
		PointF m_highlightStartPos;
		PointF m_highlightEndPos;
		Brush m_highlightBrush = new SolidBrush(Color.FromArgb(64, 255, 255, 255));
		
		Pen m_selectedArrowPen = new Pen(Color.Blue,2);


		public delegate void SelectedNodesChangedHandler(List<NodeShape> m_selectedNodes);
		public event SelectedNodesChangedHandler SelectedNodesChanged;
		private List<NodeShape> m_selectedNodes = new List<NodeShape>();
		public List<NodeShape> SelectedNodes
		{
			get { return m_selectedNodes; }
		}

		public delegate void SelectedArrowChangedHandler(ArrowShape m_arrow);
		public event SelectedArrowChangedHandler SelectedArrowChanged;
		private List<ArrowShape> m_arrowShapes = new List<ArrowShape>();
		private ArrowShape m_selectedArrow = null;
		private ArrowShape m_lastSelectedArrow = null;
		public ArrowShape SelectedArrow
		{
			get { return m_selectedArrow; }
		}

		private bool m_shiftKeyDown = false;
        private ArrowRenderer arrowRenderer;
		private bool m_placingArrow = false;
		private bool m_movingNode = false;
		private PointF m_movingNodeOffset = new PointF(0,0);
		private PointF m_movingNodeStartPos = new PointF(0, 0);
		private PointF m_arrowStart = new PointF(0,0);
        private PointF m_arrowEnd = new PointF(0,0);

		private Font m_font = new Font(new FontFamily("Arial"),8,FontStyle.Bold);

		private GraphNode<NodeShape> m_downSelectedNode = null;
		private GraphNode<NodeShape> m_upSelectedNode = null;

		private Graph<NodeShape> m_graph = new Graph<NodeShape>();
		public Graph<NodeShape> Graph
		{
			get { return m_graph; }
		}

        public GraphViewer()
        {
			this.SetStyle(ControlStyles.AllPaintingInWmPaint |
						 ControlStyles.UserPaint |
						 ControlStyles.OptimizedDoubleBuffer,true);

            InitializeComponent();

			arrowRenderer = new ArrowRenderer(15,(float)Math.PI / 6,true);
			arrowRenderer.SetThetaInDegrees(90);

			MouseWheel += new MouseEventHandler(MouseWheelEventHandler);            
        }

        protected override void OnPaint(PaintEventArgs e)
        {
			
			e.Graphics.ResetTransform();
			e.Graphics.TranslateTransform(m_translationX,m_translationY);
			e.Graphics.TranslateTransform(((float)ClientSize.Width / 2.0f) - m_translationX,
										 ((float)ClientSize.Height / 2.0f) - m_translationY);
			
			e.Graphics.ScaleTransform(m_scale,m_scale);
			e.Graphics.TranslateTransform(-((float)ClientSize.Width / 2.0f) + m_translationX,
										 -((float)ClientSize.Height / 2.0f) + m_translationY);
			//e.Graphics.TranslateTransform(m_translationX,m_translationY);
			//e.Graphics.TranslateTransform((float)ClientSize.Width / 2,
			  // (float)ClientSize.Height / 2);
        
				base.OnPaint(e);
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

            
		

			//Draw all graph nodes
			foreach (GraphNode<NodeShape> node in m_graph.Nodes)
			{
				node.Value.draw(e.Graphics);
			}

			//Draw all graph arrows
			/*foreach (GraphNode<NodeShape> node in m_graph.Nodes)
			{
				foreach (GraphNode<NodeShape> neighbor in node.Neighbors)
				{
					//Try to guess the most pleasing connector point visually
					PointF[] node1Points = new PointF[4];
					node1Points[0] = new PointF(node.Value.Pos.X,node.Value.Pos.Y);
					node1Points[1] = new PointF(node.Value.Pos.X + node.Value.Size.Width,node.Value.Pos.Y);
					node1Points[2] = new PointF(node.Value.Pos.X + node.Value.Size.Width,node.Value.Pos.Y + node.Value.Size.Height);
					node1Points[3] = new PointF(node.Value.Pos.X,node.Value.Pos.Y + node.Value.Size.Height);

					PointF[] node2Points = new PointF[4];
					node2Points[0] = new PointF(neighbor.Value.Pos.X,neighbor.Value.Pos.Y);
					node2Points[1] = new PointF(neighbor.Value.Pos.X + neighbor.Value.Size.Width,neighbor.Value.Pos.Y);
					node2Points[2] = new PointF(neighbor.Value.Pos.X + neighbor.Value.Size.Width,neighbor.Value.Pos.Y + neighbor.Value.Size.Height);
					node2Points[3] = new PointF(neighbor.Value.Pos.X,neighbor.Value.Pos.Y + neighbor.Value.Size.Height);

					PointF p1 = new PointF(0,0);
					PointF p2 = new PointF(0,0);
					PointF p3 = new PointF(0,0);
					PointF p4 = new PointF(0,0);
					float smallestDistanceSquared = 99999999;
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							float a = (node1Points[i].X - node2Points[j].X);
							a = a * a;
							float b = (node1Points[i].Y - node2Points[j].Y);
							b = b * b;

							if (a + b < smallestDistanceSquared)
							{
								smallestDistanceSquared = a + b;
								p1 = node1Points[i];
								p2 = node2Points[j];
							}
						}
					}

					Vector boxCenter = new Vector(node.Value.Pos.X + (node.Value.Size.Width / 2),node.Value.Pos.Y + (node.Value.Size.Height / 2));
					Vector boxCenter2 = new Vector(neighbor.Value.Pos.X + (neighbor.Value.Size.Width / 2),neighbor.Value.Pos.Y + (neighbor.Value.Size.Height / 2));
					AABB aabb = new AABB(new Vector(neighbor.Value.Pos.X,neighbor.Value.Pos.Y,0),new Vector(neighbor.Value.Pos.X + neighbor.Value.Size.Width,neighbor.Value.Pos.Y + neighbor.Value.Size.Height));

					float tmin = 0.0f;
					Vector q = new Vector();
					Vector dir = boxCenter2 - boxCenter;
					dir = Vector.Normalize(dir);

					aabb.intersectRay(boxCenter,dir,tmin,out q);

					p2.X = (int)q.X;
					p2.Y = (int)q.Y;

					//Now fire a ray the other way and switch the aabb to point to first box
					dir = Vector.Negate(dir);
					AABB aabb2 = new AABB(new Vector(node.Value.Pos.X,node.Value.Pos.Y,0),new Vector(node.Value.Pos.X + node.Value.Size.Width,node.Value.Pos.Y + node.Value.Size.Height));

					aabb2.intersectRay(q,dir,tmin,out q);
					
					p1.X = (int)q.X;
					p1.Y = (int)q.Y;

					arrowRenderer.DrawArrow(e.Graphics,Pens.Green,Brushes.Green,p1,p2);
				}
			}*/
			//Draw all graph arrows. 
			PointF selectedArrowP1 = new PointF(0.0f,0.0f);
			PointF selectedArrowP2 = new PointF(0.0f,0.0f);
			foreach (ArrowShape arrow in m_arrowShapes)
			{
				//Try to guess the most pleasing connector point visually
				PointF[] node1Points = new PointF[4];

				node1Points[0] = new PointF(arrow.FromNode.Value.Pos.X, arrow.FromNode.Value.Pos.Y);
				node1Points[1] = new PointF(arrow.FromNode.Value.Pos.X + arrow.FromNode.Value.Size.Width, arrow.FromNode.Value.Pos.Y);
				node1Points[2] = new PointF(arrow.FromNode.Value.Pos.X + arrow.FromNode.Value.Size.Width, arrow.FromNode.Value.Pos.Y + arrow.FromNode.Value.Size.Height);
				node1Points[3] = new PointF(arrow.FromNode.Value.Pos.X, arrow.FromNode.Value.Pos.Y + arrow.FromNode.Value.Size.Height);

				PointF[] node2Points = new PointF[4];
				node2Points[0] = new PointF(arrow.ToNode.Value.Pos.X, arrow.ToNode.Value.Pos.Y);
				node2Points[1] = new PointF(arrow.ToNode.Value.Pos.X + arrow.ToNode.Value.Size.Width, arrow.ToNode.Value.Pos.Y);
				node2Points[2] = new PointF(arrow.ToNode.Value.Pos.X + arrow.ToNode.Value.Size.Width, arrow.ToNode.Value.Pos.Y + arrow.ToNode.Value.Size.Height);
				node2Points[3] = new PointF(arrow.ToNode.Value.Pos.X, arrow.ToNode.Value.Pos.Y + arrow.ToNode.Value.Size.Height);

				PointF p1 = new PointF(0, 0);
				PointF p2 = new PointF(0, 0);
				PointF p3 = new PointF(0, 0);
				PointF p4 = new PointF(0, 0);
				float smallestDistanceSquared = float.MaxValue;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						float a = (node1Points[i].X - node2Points[j].X);
						a = a * a;
						float b = (node1Points[i].Y - node2Points[j].Y);
						b = b * b;

						if (a + b < smallestDistanceSquared)
						{
							smallestDistanceSquared = a + b;
							p1 = node1Points[i];
							p2 = node2Points[j];
						}
					}
				}

				Vector boxCenter = new Vector(arrow.FromNode.Value.Pos.X + (arrow.FromNode.Value.Size.Width / 2), arrow.FromNode.Value.Pos.Y + (arrow.FromNode.Value.Size.Height / 2));
				Vector boxCenter2 = new Vector(arrow.ToNode.Value.Pos.X + (arrow.ToNode.Value.Size.Width / 2), arrow.ToNode.Value.Pos.Y + (arrow.ToNode.Value.Size.Height / 2));
				AABB aabb = new AABB(new Vector(arrow.ToNode.Value.Pos.X, arrow.ToNode.Value.Pos.Y, 0), new Vector(arrow.ToNode.Value.Pos.X + arrow.ToNode.Value.Size.Width, arrow.ToNode.Value.Pos.Y + arrow.ToNode.Value.Size.Height));

				float tmin = 0.0f;
				Vector q = new Vector();
				Vector dir = boxCenter2 - boxCenter;
				dir = Vector.Normalize(dir);

				aabb.intersectRay(boxCenter, dir, tmin, out q);

				p2.X = (int)q.X;
				p2.Y = (int)q.Y;

				//Now fire a ray the other way and switch the aabb to point to first box
				dir = Vector.Negate(dir);
				AABB aabb2 = new AABB(new Vector(arrow.FromNode.Value.Pos.X, arrow.FromNode.Value.Pos.Y, 0), new Vector(arrow.FromNode.Value.Pos.X + arrow.FromNode.Value.Size.Width, arrow.FromNode.Value.Pos.Y + arrow.FromNode.Value.Size.Height));

				aabb2.intersectRay(q, dir, tmin, out q);

				p1.X = (int)q.X;
				p1.Y = (int)q.Y;


				if (arrow.Selected)
				{
					selectedArrowP1 = p1;
					selectedArrowP2 = p2;
					//arrowRenderer.DrawArrow(e.Graphics, m_selectedArrowPen, Brushes.Blue, p1, p2);
				}
				else
					arrowRenderer.DrawArrow(e.Graphics, Pens.Green, Brushes.Green, p1, p2);
		
			}
			
			//Draw selected arrow on after all other arrows so it shows on top
			if (m_selectedArrow != null)
			{
				arrowRenderer.DrawArrow(e.Graphics, m_selectedArrowPen, Brushes.Blue, selectedArrowP1, selectedArrowP2);
			}

			//Draw placing arrow if we are currently drawing one
			if (m_placingArrow)
			{
				bool validPlacement = false;
				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				{
					if (node.Value.containsPoint(m_arrowEnd))
					{
						validPlacement = true;
						break;
					}
				}
				if (validPlacement)
					arrowRenderer.DrawArrow(e.Graphics,Pens.Green,Brushes.Green,m_arrowStart,m_arrowEnd);
				else
					arrowRenderer.DrawArrow(e.Graphics,Pens.Red,Brushes.Red,m_arrowStart,m_arrowEnd);
			}

			if (m_highlighting)
			{
				float smallestX = Math.Min(m_highlightStartPos.X, m_highlightEndPos.X);
				float smallestY = Math.Min(m_highlightStartPos.Y, m_highlightEndPos.Y);
				float largestX = Math.Max(m_highlightStartPos.X, m_highlightEndPos.X);
				float largestY = Math.Max(m_highlightStartPos.Y, m_highlightEndPos.Y);

				RectangleF highlightRect = new RectangleF(smallestX,smallestY,largestX - smallestX,largestY - smallestY);

				e.Graphics.FillRectangle(m_highlightBrush,smallestX,smallestY,largestX - smallestX,largestY - smallestY);
				e.Graphics.DrawRectangle(Pens.Blue, highlightRect.X, highlightRect.Y, highlightRect.Width, highlightRect.Height);

				//Highlight selected node
				RectangleF nodeRect;
				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				{
					nodeRect = new RectangleF(node.Value.Pos,node.Value.Size);
					if (highlightRect.IntersectsWith(nodeRect))
					{
						node.Value.Selected = true;
						if (!m_selectedNodes.Contains(node.Value))
						{
							m_selectedNodes.Add(node.Value);
							if (m_selectedArrow != null)
							{
								m_selectedArrow.Selected = false;
								m_selectedArrow = null;
								SelectedArrowChanged(m_selectedArrow);
							}
						}						
					}
					else
					{
						node.Value.Selected = false;
						m_selectedNodes.Remove(node.Value);
					}
				}
			}

        }

		private PointF transformPoint(PointF location)
		{
			PointF newPoint = location;/* new PointF(((location.X - m_translationX)),
							(((location.Y - m_translationY))));
			newPoint.X /=  m_scale;
			newPoint.Y /= m_scale;
			*/
			newPoint.X -= m_translationX;
			newPoint.Y -= m_translationY;


			newPoint.X += -((float)ClientSize.Width / 2.0f) + m_translationX;
			newPoint.Y += -((float)ClientSize.Height / 2.0f) + m_translationY;

			newPoint.X /= m_scale;
			newPoint.Y /= m_scale;

			newPoint.X += ((float)ClientSize.Width / 2.0f) - m_translationX;
			newPoint.Y += ((float)ClientSize.Height / 2.0f) - m_translationY;


			//e.Graphics.TranslateTransform(m_translationX,m_translationY);
			//e.Graphics.TranslateTransform(((float)ClientSize.Width / 2.0f) - m_translationX,
			//							 ((float)ClientSize.Height / 2.0f) - m_translationY);
			
			//e.Graphics.ScaleTransform(m_scale,m_scale);

			/*Matrix m = CreateGraphics().Transform.Clone();

			m.Invert();
			m.TransformPoints(new PointF[1] { newPoint });

			newPoint.X -= m_translationX;
			newPoint.Y -= m_translationY;*/

			/*newPoint.X /= m_scale;
			newPoint.Y /= m_scale;

			newPoint.X = (((float)ClientSize.Width / 2.0f) - m_translationX);
			newPoint.Y = (((float)ClientSize.Height / 2.0f) - m_translationY);*/
			
			return newPoint;
		}

		private void MouseWheelEventHandler(object sender,MouseEventArgs e)
		{
			if (e.Delta != 0)
			{
				if ((m_scale + e.Delta / 1500.0f <= 0.01) &&
					(m_scale + e.Delta / 1500.0f) < 0.0f)
					m_scale = 0.01f;
				else
					m_scale += e.Delta / 1500.0f;
				Invalidate();
			}
		}

        private void GraphExplorer_MouseDown(object sender, MouseEventArgs e)
        {
			PointF transformedPoint = transformPoint(e.Location);

			if ((ModifierKeys & Keys.Alt) != Keys.Alt &&
				(ModifierKeys & Keys.Shift) != Keys.Shift)
			{
				bool nodeMissed = true;

				//Unselect all arrows
				foreach (ArrowShape arrow in m_arrowShapes)
					arrow.Selected = false;
				m_selectedArrow = null;
				SelectedArrowChanged(m_selectedArrow);
				
				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				{
					if (node.Value.containsPoint(transformPoint(e.Location)))
					{
						m_downSelectedNode = node;
						nodeMissed = false;
						break;
					}
				}

				//Move the selected to the top 
				/*if (m_downSelectedNode != null)
				{
					m_graph.Nodes.Remove(m_downSelectedNode);
					m_graph.Nodes.Add(m_downSelectedNode);
				}*/

				if (nodeMissed)
				{
					m_downSelectedNode = null;
					foreach (GraphNode<NodeShape> node in m_graph.Nodes)
					{
						node.Value.Selected = false;
					}
					m_selectedNodes.Clear();
					SelectedNodesChanged(m_selectedNodes);

					//Now that we know there isn't a node selected, are we selecting
					//an arrow?
					ArrowShape closestArrow = null;
					ArrowShape lastSelectedArrow = null;
					float closestDistance = float.MaxValue;
					foreach (ArrowShape arrow in m_arrowShapes)
					{
						Point boxCenter = new Point((int)arrow.FromNode.Value.Pos.X + (arrow.FromNode.Value.Size.Width / 2), (int)arrow.FromNode.Value.Pos.Y + (arrow.FromNode.Value.Size.Height / 2));
						Point boxCenter2 = new Point((int)arrow.ToNode.Value.Pos.X + (arrow.ToNode.Value.Size.Width / 2), (int)arrow.ToNode.Value.Pos.Y + (arrow.ToNode.Value.Size.Height / 2));
						float distance = distPointToSegment(new Vector(transformPoint(e.Location)),new Vector(boxCenter),new Vector(boxCenter2));
						if ((distance < closestDistance) && (distance <= 20))
						{
							//If this arrow was already selected last time, let's keep looking for
							//another one. If we don't find another one to select (perphaps behind this
							//arrow) let the original arrow get selected again.
							if (m_lastSelectedArrow == arrow)
							{
								lastSelectedArrow = arrow;
								continue;
							}
							closestDistance = distance;
							closestArrow = arrow;
						}
					}

					if (closestArrow != null)
					{
						closestArrow.Selected = true;
						m_selectedArrow = closestArrow;
						m_lastSelectedArrow = closestArrow;
						SelectedArrowChanged(closestArrow);
					}
					else if (lastSelectedArrow != null)
					{
						lastSelectedArrow.Selected = true;
						m_selectedArrow = lastSelectedArrow;
						m_lastSelectedArrow = lastSelectedArrow;
						SelectedArrowChanged(lastSelectedArrow);
					}
				}
				else
				{
					//If control or alt is not held down or only one node is selected, remove current selection
					if (((ModifierKeys & Keys.Control) != Keys.Control) &&
						(!m_downSelectedNode.Value.Selected))
					{
						foreach (GraphNode<NodeShape> node in m_graph.Nodes)
						{
							node.Value.Selected = false;
						}
						m_selectedNodes.Clear();
						SelectedNodesChanged(m_selectedNodes);
					}
					m_downSelectedNode.Value.Selected = true;

					if (!m_selectedNodes.Contains(m_downSelectedNode.Value))
					{
						m_selectedNodes.Add(m_downSelectedNode.Value);
						SelectedNodesChanged(m_selectedNodes);
					}
				}

				Invalidate();
			}

			if (e.Button == MouseButtons.Left && m_shiftKeyDown)
			{
				m_graph.AddNode(new NodeShape(this,m_graph,transformedPoint,"State" + nodeCtr.ToString(),m_font));
				nodeCtr++;
				Invalidate();
			}

			if (e.Button == MouseButtons.Left &&
				!m_shiftKeyDown &&
				m_downSelectedNode != null)
			{
				m_movingNode = true;
				m_movingNodeStartPos = e.Location;
				m_movingNodeOffset.X = m_downSelectedNode.Value.Pos.X - transformedPoint.X;
				m_movingNodeOffset.Y = m_downSelectedNode.Value.Pos.Y - transformedPoint.Y;
				m_movingNodeOffset.X = (m_movingNodeOffset.X * m_scale); 
				m_movingNodeOffset.Y = (m_movingNodeOffset.Y * m_scale); 
			}

			if ((ModifierKeys & Keys.Alt) == Keys.Alt)
			{
				if ((e.Button & MouseButtons.Middle) == MouseButtons.Middle)
				{
					m_translating = true;
					m_startingPos = new PointF(e.Location.X,e.Location.Y);
				}
				if ((e.Button & MouseButtons.Right) == MouseButtons.Right)
				{
					m_scaling = true;
					m_startingPos = new PointF(e.Location.X,e.Location.Y);
				}
			}

			if (e.Button == MouseButtons.Right &&
				!m_shiftKeyDown &&
				m_downSelectedNode != null &&
				!m_scaling)
			{
				m_placingArrow = true;
				m_arrowStart = transformedPoint;
				m_arrowEnd = transformedPoint;
			}

			if (e.Button == MouseButtons.Left &&
				m_downSelectedNode == null &&
				!m_shiftKeyDown &&
				!m_translating)
			{
				m_highlighting = true;
				m_highlightStartPos = transformPoint(e.Location);
				m_highlightEndPos = transformPoint(e.Location);
			}
        }

        private void GraphExplorer_MouseMove(object sender, MouseEventArgs e)
        {
	        if (m_placingArrow)
            {
                m_arrowEnd = transformPoint(e.Location);
                Invalidate();
            }
			
			if (m_movingNode)
			{
				if (m_downSelectedNode != null)
				{
					PointF newLocation;

					//Calculate delta movement vector
					PointF movementVec = new PointF(m_movingNodeStartPos.X,m_movingNodeStartPos.Y);
					movementVec.X = e.Location.X - movementVec.X;
					movementVec.Y = e.Location.Y - movementVec.Y;

					PointF movmentVecBeforeScale = movementVec;
					
					movementVec.X = movementVec.X / m_scale;
					movementVec.Y = movementVec.Y / m_scale;

					
					//Move the selected nodes
					for (int i = 0; i < m_selectedNodes.Count;++i)
					{
						newLocation = m_selectedNodes[i].Pos;
						newLocation.X += movementVec.X;
						newLocation.Y += movementVec.Y;
						m_selectedNodes[i].Pos = newLocation;
					}

					Invalidate();

					m_movingNodeStartPos = e.Location;
				}		
			}

			if (m_translating)
			{
				m_translationX += (e.Location.X - m_startingPos.X) / m_scale;
				m_translationY += (e.Location.Y - m_startingPos.Y) / m_scale;
				m_startingPos = e.Location;
				Invalidate();
			}

			if (m_scaling)
			{
				if (((m_scale + ((e.Location.X - m_startingPos.X) / 500.0f)) <= .01f) &&
					((e.Location.X - m_startingPos.X) < 0))
					m_scale = .01f;
				else
				{
					m_scale += (e.Location.X - m_startingPos.X) / 500.0f;
					//m_translationX += -(e.Location.X - m_startingPos.X);
					//m_translationY += -(e.Location.X - m_startingPos.X) ;
					//System.Console.WriteLine(m_translationX.ToString());
				}
				m_startingPos = e.Location;
				Invalidate();
			}

			if (m_highlighting)
			{
				m_highlightEndPos = transformPoint(e.Location);
				Invalidate();
			}
        }

		private void GraphExplorer_MouseUp(object sender,MouseEventArgs e)
		{
			//Update selection for mouse up
			m_upSelectedNode = null;
			foreach (GraphNode<NodeShape> node in m_graph.Nodes)
			{ 
				if (node.Value.containsPoint(transformPoint(e.Location)))
				{
					m_upSelectedNode = node;
				}
			}

			//Add directed edge if we found a valid pair of nodes.  Notice how we only
			//take the top/last most node to make z order look a little better.  
			//Determine if we have a valid arrow connection.
			if ((m_downSelectedNode != null) &&
				(m_upSelectedNode != null) &&
				(m_downSelectedNode != m_upSelectedNode) &&
				(m_placingArrow))
			{
				//Create an arrow between the nodes if an edge does not already
				//exist
				if (!m_downSelectedNode.Neighbors.Contains(m_upSelectedNode))
				{
					//Add a directed edge to the graph and create a new arrow shape
					m_graph.AddDirectedEdge(m_downSelectedNode, m_upSelectedNode);

					ArrowShape arrow = new ArrowShape(m_downSelectedNode.Value.Text + " ---> " + m_upSelectedNode.Value.Text);
					m_arrowShapes.Add(arrow);
					arrow.FromNode = m_downSelectedNode;
					arrow.ToNode = m_upSelectedNode;
				}
	
			}

			m_placingArrow = false;
			m_movingNode = false;
			m_translating = false;
			m_scaling = false;
			m_highlighting = false;

			//Apply our selected objects
			SelectedNodesChanged(m_selectedNodes);
			if (m_selectedArrow != null)
				SelectedArrowChanged(m_selectedArrow);

			Invalidate();
		}

		private void GraphExplorer_KeyDown(object sender,KeyEventArgs e)
		{
			if (e.Shift)
				m_shiftKeyDown = true;

			if (e.KeyCode == Keys.Delete)
				removeSelectedNodes();

			//Select All
			if ((e.KeyCode == Keys.A) && ((ModifierKeys & Keys.Control) == Keys.Control))
			{
				m_selectedNodes.Clear();
				if (m_selectedArrow != null)
				{
					m_selectedArrow.Selected = false;
					m_selectedArrow = null;
					SelectedArrowChanged(m_selectedArrow);
				}

				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				{
					m_selectedNodes.Add(node.Value);
					node.Value.Selected = true;	
				}
				SelectedNodesChanged(m_selectedNodes);
				Invalidate();
			}
		}

		private void removeSelectedNodes()
		{
			//Remove selected graph nodes
			for (int i = 0; i < m_selectedNodes.Count;++i)
			{
				//Remove graph node and any arrow that connect to this node
				m_graph.Remove(m_selectedNodes[i]);
				m_selectedNodes[i].Dispose();

				//Remove arrow shapes
				List<ArrowShape> removeList = new List<ArrowShape>(10);
				foreach (ArrowShape arrow in m_arrowShapes)
				{
					if (arrow.FromNode.Value == m_selectedNodes[i] || arrow.ToNode.Value == m_selectedNodes[i])
						removeList.Add(arrow);
				}
				foreach (ArrowShape arrow in removeList)
				{
					m_arrowShapes.Remove(arrow);
				}

				removeList.Clear();
			}
			m_downSelectedNode = null;
			SelectedNodesChanged(m_selectedNodes);

			//Remove selected arrow
			if (m_selectedArrow != null)
			{
				m_selectedArrow.FromNode.Neighbors.Remove(m_selectedArrow.ToNode);

				//Remove arrow shapes
				List<ArrowShape> removeList = new List<ArrowShape>(1);
				foreach (ArrowShape arrow in m_arrowShapes)
				{
					if (arrow.FromNode == m_selectedArrow.FromNode && arrow.ToNode == m_selectedArrow.ToNode)
						removeList.Add(arrow);
				}
				foreach (ArrowShape arrow in removeList)
					m_arrowShapes.Remove(arrow);

				removeList.Clear();
				m_selectedArrow = null;
				SelectedArrowChanged(m_selectedArrow);
			}

			Invalidate();
		}

		private void GraphExplorer_KeyUp(object sender,KeyEventArgs e)
		{
			if (!e.Shift)
				m_shiftKeyDown = false;
		}

		private void toolStripButton1_Click(object sender,EventArgs e)
		{
			m_scale = 1.0f;
			m_translationX = m_translationY = 0.0f;
			Invalidate();
		}

		private float distPointToSegment(Vector P, Vector A,Vector B)
		{
			Vector v = B - A;
			Vector w = P - A;

			float c1 = Vector.DotProduct(w,v);
			if (c1 <= 0)
				return distancePointToPoint(P, A);

			float c2 = Vector.DotProduct(v, v);
			if (c2 <= c1)
				return distancePointToPoint(P, B);

			float b = c1 / c2;
			Vector Pb = A + b * v;
			return distancePointToPoint(P, Pb);
		}

		private float distancePointToPoint(Vector A, Vector B)
		{
			float x = B.X - A.X;
			float y = B.Y - A.Y;
			return (float)Math.Sqrt(x * x + y * y);
		}

		private void tsbClear_Click(object sender,EventArgs e)
		{
			//Clear all nodes and reset workspace
			//Dispose of the graph nodes
			foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				node.Value.Dispose();
			m_graph.Clear();

			m_selectedNodes.Clear();
			m_arrowShapes.Clear();
			m_selectedArrow = null;
			m_scale = 1.0f;
			m_translationX = m_translationY = 0.0f;
			nodeCtr = 0;
			Invalidate();
		}

		private void tsbStats_Click(object sender,EventArgs e)
		{
			MessageBox.Show("Number of states: " + m_graph.Nodes.Count +
							"\nNumber of transitions: " + m_arrowShapes.Count,
							"Statistics");
		}

		private void tsbZoomExtents_Click(object sender,EventArgs e)
		{
			//If there aren't any nodes, we're done
			if (m_graph.Nodes.Count == 0)
				return;

			//Create a bounding rectangle for graph node
			float minX = float.MaxValue;
			float minY = float.MaxValue;
			float maxX = float.MinValue;
			float maxY = float.MinValue;

			//If there's only one node in the graph we simply use the node's box
			//as the bounding box
			if (m_graph.Nodes.Count == 1)
			{
				minX = m_graph.Nodes[0].Value.Pos.X;
				minY = m_graph.Nodes[0].Value.Pos.Y;
				maxX = m_graph.Nodes[0].Value.Pos.X + m_graph.Nodes[0].Value.Size.Width;
				maxY = m_graph.Nodes[0].Value.Pos.Y + m_graph.Nodes[0].Value.Size.Height;
			}
			else
			{
				foreach (GraphNode<NodeShape> node in m_graph.Nodes)
				{
					//top-left corner
					if (node.Value.Pos.X < minX)
						minX = node.Value.Pos.X;
					if (node.Value.Pos.Y < minY)
						minY = node.Value.Pos.Y;
					if (node.Value.Pos.X > maxX)
						maxX = node.Value.Pos.X;
					if (node.Value.Pos.Y > maxY)
						maxY = node.Value.Pos.Y;

					//top-right corner
					if (node.Value.Pos.X + node.Value.Size.Width < minX)
						minX = node.Value.Pos.X + node.Value.Size.Width;
					if (node.Value.Pos.Y < minY)
						minY = node.Value.Pos.Y;
					if (node.Value.Pos.X + node.Value.Size.Width > maxX)
						maxX = node.Value.Pos.X + node.Value.Size.Width;
					if (node.Value.Pos.Y > maxY)
						maxY = node.Value.Pos.Y;

					//bottom-left corner
					if (node.Value.Pos.X < minX)
						minX = node.Value.Pos.X;
					if (node.Value.Pos.Y + node.Value.Size.Height < minY)
						minY = node.Value.Pos.Y + node.Value.Size.Height;
					if (node.Value.Pos.X > maxX)
						maxX = node.Value.Pos.X;
					if (node.Value.Pos.Y + node.Value.Size.Height > maxY)
						maxY = node.Value.Pos.Y + node.Value.Size.Height;

					//bottom-right corner
					if (node.Value.Pos.X + node.Value.Size.Width < minX)
						minX = node.Value.Pos.X + node.Value.Size.Width;
					if (node.Value.Pos.Y + node.Value.Size.Height < minY)
						minY = node.Value.Pos.Y + node.Value.Size.Height;
					if (node.Value.Pos.X + node.Value.Size.Width > maxX)
						maxX = node.Value.Pos.X + node.Value.Size.Width;
					if (node.Value.Pos.Y + node.Value.Size.Height > maxY)
						maxY = node.Value.Pos.Y + node.Value.Size.Height;
				}
			}

			float xScale = this.Width / (float)(maxX - minX);
			float yScale = this.Height / (float)(maxY - minY);

			if (xScale <= .01f || yScale <= .01f)
			{
				xScale = .01f;
				yScale = .01f;
				MessageBox.Show("The graph is too large to fit within the viewing window.  You can fix this by bringing nodes closer together.","Size Error",MessageBoxButtons.OK,MessageBoxIcon.Information);
			}
			
			PointF p = new PointF(minX,minY);

			m_translationX = p.X * -Math.Min(xScale,yScale) * .5f;
			m_translationY = p.Y * -Math.Min(xScale,yScale) * .5f;

			//m_translationY += tsActions.Height;

			m_scale = Math.Min(xScale,yScale) * .5f;
			Invalidate();

		}

		private void tsbAutoArrange_Click(object sender,EventArgs e)
		{
			PointF p = new PointF(0,bar1.Height);
			foreach (GraphNode<NodeShape> node in m_graph.Nodes)
			{
				node.Value.Pos = p;
				p.X += 10;
				p.Y += 10;
			}
			tsbZoomExtents_Click(null,null);
			Invalidate();
		}

		private void tsbAddNode_Click(object sender,EventArgs e)
		{
			//Add a node to the middle of the window
			m_graph.AddNode(new NodeShape(this,m_graph,transformPoint(new PointF(Width/2.0f,Height/2.0f)),"State" + nodeCtr.ToString(),m_font));
			nodeCtr++;

			Invalidate();
		}

		private void tsbRemoveNode_Click(object sender,EventArgs e)
		{
			removeSelectedNodes();
		}

		private void buttonItem1_Click(object sender,EventArgs e)
		{

		}
	}
}
