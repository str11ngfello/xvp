using System;
using System.Collections.Generic;
using System.Text;
using XVPDesigner.Managers;
using XVPDesigner.Objects;
using XVPDesigner;
using XVPDesigner.UserControls;
using System.Windows.Forms;
using XVPDesigner.Interfaces;
using System.Drawing;
using XVPManaged;

namespace PhysXPlugin
{
	[ControllerPluginDescriptor]
	public class PhysXControllerPlugin : BasePlugin
	{
		private PhysXActionBar m_physXActionBar;
		
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Ageia PhysX";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\camera.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}

			//Create exactly one instance of the action control
			m_physXActionBar = new PhysXActionBar();

			return true;
		}

		public override Controller createControllerInstance(string name)
		{
			PhysXController physXController = new PhysXController(name);
			return physXController;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				PhysXController physXController = sceneExplorer.getNode(node.Name) as PhysXController;
				physXController.Proxy = XVPM.initPhysXController(XVPMHandle);
				physXController.XVPMHandle = XVPMHandle;
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				PhysXController physXController = sceneExplorer.getNode(node.Name) as PhysXController;

				//Now destroy the physX controller itelf
				XVPM.destroyPhysXController(physXController.Proxy);
			}

			return true;
		}

		public override ActionExplorerBase getActionExplorer()
		{
			return m_physXActionBar;
		}
	}


	[Serializable]
	public class PhysXController : Controller
	{
		public PhysXController(string name)
			: base(name)
		{
			PluginID = "Ageia PhysX";
		}

		public override void setCurrentValues()
		{
			base.setCurrentValues();
		}

		public void addForce(float x,float y ,float z)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.addForce(Proxy,x,y,z);
			}
		}
	}

}
