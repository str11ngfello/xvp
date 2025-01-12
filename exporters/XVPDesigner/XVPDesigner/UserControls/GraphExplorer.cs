using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.UserControls
{
	public partial class GraphExplorer : UserControl
	{
		public GraphExplorer()
		{
			InitializeComponent();

			//Listen for node selection
			graphViewer.SelectedNodesChanged += new GraphViewer.SelectedNodesChangedHandler(selectionChanged);
		}

		public void selectionChanged(List<GraphViewer.NodeShape> selectedNodes)
		{
			pgridProperties.SelectedObjects = selectedNodes.ToArray();
		}

		private void pgridProperties_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
		{
			graphViewer.Refresh();
		}

		private void graphViewer_SelectedArrowChanged(GraphViewer.ArrowShape m_arrow)
		{
			pgridProperties.SelectedObject = m_arrow;
		}

		private void graphViewer_Load(object sender,EventArgs e)
		{

		}

	}
}
