using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.CodeDom.Compiler;
using System.CodeDom;
using Microsoft.CSharp;
using System.Reflection;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using XVPDesigner.Interfaces;
using XVPDesigner.Managers;
using XVPManaged;

namespace XVPDesigner.Objects
{
	public class BasePlugin
	{
		protected string m_pluginID = "NO_PLUGIN_ID";		
		public string PluginID
		{
			get { return m_pluginID; }
			set { m_pluginID = value; }
		}

		protected Bitmap m_menuImage = null;
		public Bitmap MenuImage
		{
			get { return m_menuImage; }
			set { m_menuImage = value; }
		}

		public virtual bool init(MainForm mainForm)
		{
			return false;
		}

		public virtual TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			return null;
		}

		public virtual Controller createControllerInstance(string name)
		{
			return null;
		}

		public virtual bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			return false;
		}

		public virtual bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			return false;
		}

		public virtual bool setActorContainer(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node,Objects.ActorContainer actorContainer)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Tell proxy it is getting an actor container
				SceneNode sceneNode = sceneExplorer.getNode(node.Name);
				XVPM.setActorContainer(sceneNode.Proxy,actorContainer.Proxy);
			}

			return true;
		}

		public virtual ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode node)
		{
			return null;
		}

		public virtual ActionExplorerBase getActionExplorer()
		{
			return null;
		}

		public virtual void setScene(uint XVPMHandle,string sceneName,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
		}

		public virtual float[] getStageTransform()
		{
			return new float[16] {1.0f,0.0f,0.0f,0.0f,
								  0.0f,1.0f,0.0f,0.0f,
								  0.0f,0.0f,1.0f,0.0f,
								  0.0f,0.0f,0.0f,1.0f};
		}

		public virtual bool setLightmap(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node,string lightmapPath)
		{
			return true;
		}
	}
}
