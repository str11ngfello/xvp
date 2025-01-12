using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using XVPDesigner.Objects;
using XVPDesigner;
using XVPManaged;
using System.Runtime.InteropServices;

namespace ParticleSystemPlugin
{
	[Serializable]
	public class ParticleSystem : SceneNode
	{
		public ParticleSystem(string name)
			: base(name)
		{
			PluginID = "Particle System";
			AcceptsControllers = false;
			Renameable = true;

			//m_contextMenu = new ContextMenu();
			//PopupMenu.MenuItems.Add("Add Emitter",new EventHandler(popup));
		}

		public override void Dispose()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.destroyParticleSystem(m_proxy);
			}
		}

		protected override void createExtendedContextMenu()
		{
			//Add our own commands
			m_contextMenu.MenuItems.Add("Add Emitter",new EventHandler(handleContextMenu));
		}

		override protected void handleContextMenu(object sender,EventArgs e)
		{
			MenuItem miClicked = (MenuItem)sender;
			string item = miClicked.Text;

			//Get the currently selected tree node we have right clicked.  This
			//works due to mouse down event selecting a tree node when it gets
			//right-clicked.
			SceneExplorer scene = Program.g_mainForm.ActiveSceneExplorer;
			TreeNode selectedNode = scene.getHierarchyTreeView().SelectedNode;
			ParticleSystem ps = scene.getNode(selectedNode.Name) as ParticleSystem;
			if (item == "Add Emitter")
			{
				//Add new emitter node
				string nodeGUID = System.Guid.NewGuid().ToString();
				string newEmitterName = "Emitter";
				TreeNode newEmitterNode = selectedNode.Nodes.Add(nodeGUID,newEmitterName);
				newEmitterNode.ImageKey = newEmitterNode.SelectedImageKey = "Emitter";				
				Emitter emitter = new Emitter(newEmitterName);
				newEmitterNode.ContextMenu = emitter.PopupMenu;
				scene.addNode(nodeGUID,emitter);

				//Add new emitter material
				nodeGUID = System.Guid.NewGuid().ToString(); 
				TreeNode newMatNode = newEmitterNode.Nodes.Add(nodeGUID,newEmitterName + " Material");
				newMatNode.ImageKey = newMatNode.SelectedImageKey = "Material";
				Material mat = new Material(newEmitterName + " Material");
				scene.addNode(nodeGUID,mat);
				newMatNode.ContextMenu = mat.PopupMenu;

				ps.addEmitter();
				
				//For the new emitter, create a native proxy object
				emitter.Proxy = XVPM.initEmitter(ps.Proxy,(uint)selectedNode.Nodes.Count-1);
				mat.Proxy = XVPM.getEmitterMaterial(emitter.Proxy);
				
				//Finally break out materials and controllers
				scene.findMaterials(newEmitterNode);
				scene.findControllers(newEmitterNode);
			}

			//Let the base scene node see the event
			base.handleContextMenu(sender,e);

		}

		public void start()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.startParticleSystem(Proxy);
			}
		}

		public void stop()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.stopParticleSystem(Proxy);
			}
		}

		public void restart()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.restartParticleSystem(Proxy);
			}
		}

		public void addEmitter()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.addParticleSystemEmitter(Proxy);
			}
		}
	}
}
