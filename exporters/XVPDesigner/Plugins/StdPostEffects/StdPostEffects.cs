using System;
using System.Collections.Generic;
using System.Text;
using XVPDesigner.Managers;
using XVPDesigner.Objects;
using Dialogs;
using System.Collections;
using XVPDesigner.UserControls;
using System.Windows.Forms;
using XVPDesigner;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using CameraPlugin;
using System.ComponentModel;
using XVPManaged;
using XVPDesigner.TypeEditors;

namespace StdPostEffects
{
	#region Depth of Field

	[PostEffectPluginDescriptor]
	public class DOFPostEffectPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Depth of Field";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\dof.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}


			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{		
			//A camera node must be selected
			SceneNode sceneNode = null;
			TreeNode selectedNode = sceneExplorer.getHierarchyTreeView().SelectedNode;
			if (selectedNode != null)
			{
				//Find camera at root of hierarchy
				while (selectedNode.Parent != null)
					selectedNode = selectedNode.Parent;

				sceneNode = sceneExplorer.getNode(selectedNode.Name) as CameraPlugin.Camera;
			}
			if (selectedNode == null || sceneNode == null)
			{
				MessageBox.Show("You must select a camera in the hierarchy view in order to add a post effect.","No Camera Selected");
				return null;
			}

			//Create a node for the post effect
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;

			//Find post effects node and add as a child
			TreeNode containerNode = null;
			foreach (TreeNode treeNode in selectedNode.Nodes)
			{
				if (sceneExplorer.getNode(treeNode.Name) as PostEffectContainer != null)
				{
					containerNode = treeNode;
					break;
				}
			}
			containerNode.Nodes.Add(topLevelNode);

			DOFPostEffect DOFNode = new DOFPostEffect(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,DOFNode);

			//Since we didn't call add tree, we need to hook up the menus manually
			sceneExplorer.hookupMenus(topLevelNode);

			//Create proxy instance
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				DOFPostEffect dof = sceneExplorer.getNode(node.Name) as DOFPostEffect;

				//Get camera 2 levels up
				CameraPlugin.Camera camera = sceneExplorer.getNode(node.Parent.Parent.Name) as CameraPlugin.Camera;
				if (camera == null)
					throw new Exception("Expected a camera node exactly 2 levels up from the DOF node");

				dof.Proxy = XVPM.initDOFPostEffect(camera.Proxy);
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy proxy
				DOFPostEffect dof = sceneExplorer.getNode(node.Name) as DOFPostEffect;
				if (dof.Proxy != IntPtr.Zero)
				{
					XVPM.destroyDOFPostEffect(dof.Proxy);
					dof.Proxy = IntPtr.Zero;
				}
			}

			return true;
		}
	}


	[Serializable]
	public class DOFPostEffect : XVPDesigner.Objects.BasePostEffect
	{
		private float m_focalPlane = 10.0f;
		[DisplayName("Focal Plane"),
		CategoryAttribute("Focal/Blur Distances"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1000.0",".00001")]
		public float FocalPlane
		{
			get { return m_focalPlane; }
			set
			{
				m_focalPlane = value;
				Program.g_mainForm.setFloatProperty(Proxy,"FocalPlane",value);
			}
		}

		private float m_nearBlurPlane = 1.0f;
		[DisplayName("Near Blur Plane"),
		CategoryAttribute("Focal/Blur Distances"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1000.0",".00001")]
		public float NearBlurPlane
		{
			get { return m_nearBlurPlane; }
			set
			{
				m_nearBlurPlane = value;
				Program.g_mainForm.setFloatProperty(Proxy,"NearBlurPlane",value);
			}
		}

		private float m_farBlurPlane = 150.0f;
		[DisplayName("Far Blur Plane"),
		CategoryAttribute("Focal/Blur Distances"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1000.0",".00001")]
		public float FarBlurPlane
		{
			get { return m_farBlurPlane; }
			set
			{
				m_farBlurPlane = value;
				Program.g_mainForm.setFloatProperty(Proxy,"FarBlurPlane",value);
			}
		}

		private float m_maxBlur = .5f;
		[DisplayName("Max Blur"),
		CategoryAttribute("Focal/Blur Distances"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1.0",".00001")]
		public float MaxBlur
		{
			get { return m_maxBlur; }
			set
			{
				m_maxBlur = value;
				Program.g_mainForm.setFloatProperty(Proxy,"MaxBlur",value);
			}
		}
		
		public DOFPostEffect(string name)
			: base(name)
		{
			PluginID = "Depth of Field";
			AcceptsControllers = false;
			Renameable = true;
		}

		public override void setDefaultValues()
		{
			FocalPlane = 10.0f;
			NearBlurPlane = 1.0f;
			FarBlurPlane = 150.0f;
			MaxBlur = .5f;
		}

		public override void setCurrentValues()
		{
			FocalPlane = FocalPlane;
			NearBlurPlane = NearBlurPlane;
			FarBlurPlane = FarBlurPlane;
			MaxBlur = MaxBlur;
		}
	}
	#endregion

	#region  Edge Detection

	[PostEffectPluginDescriptor]
	public class EdgePostEffectPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Edge Detection";
			
			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\dof.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}

			
			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//A camera node must be selected
			SceneNode sceneNode = null;
			TreeNode selectedNode = sceneExplorer.getHierarchyTreeView().SelectedNode;
			if (selectedNode != null)
			{
				//Find camera at root of hierarchy
				while (selectedNode.Parent != null)
					selectedNode = selectedNode.Parent;

				sceneNode = sceneExplorer.getNode(selectedNode.Name) as CameraPlugin.Camera;
			}
			if (selectedNode == null || sceneNode == null)
			{
				MessageBox.Show("You must select a camera in the hierarchy view in order to add a post effect.","No Camera Selected");
				return null;
			}

			//Create a node for the post effect
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;

			//Find post effects node and add as a child
			TreeNode containerNode = null;
			foreach (TreeNode treeNode in selectedNode.Nodes)
			{
				if (sceneExplorer.getNode(treeNode.Name) as PostEffectContainer != null)
				{
					containerNode = treeNode;
					break;
				}
			}
			containerNode.Nodes.Add(topLevelNode);

			EdgePostEffect EdgeNode = new EdgePostEffect(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,EdgeNode);

			//Since we didn't call add tree, we need to hook up the menus manually
			sceneExplorer.hookupMenus(topLevelNode);

			//Create proxy instance
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				EdgePostEffect edge = sceneExplorer.getNode(node.Name) as EdgePostEffect;
				if (edge.Proxy != IntPtr.Zero)
					return true;
				
				//Get camera 2 levels up
				CameraPlugin.Camera camera = sceneExplorer.getNode(node.Parent.Parent.Name) as CameraPlugin.Camera;
				if (camera == null)
					throw new Exception("Expected a camera node exactly 2 levels up from the node");

				edge.Proxy = XVPM.initEdgePostEffect(camera.Proxy);
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy proxy
				EdgePostEffect edge = sceneExplorer.getNode(node.Name) as EdgePostEffect;
				if (edge.Proxy != IntPtr.Zero)
				{
					XVPM.destroyEdgePostEffect(edge.Proxy);
					edge.Proxy = IntPtr.Zero;
				}
			}

			return true;
		}
	}


	[Serializable]
	public class EdgePostEffect : XVPDesigner.Objects.BasePostEffect
	{
		public EdgePostEffect(string name)
			: base(name)
		{
			PluginID = "Edge Detection";
			AcceptsControllers = false;
			Renameable = true;
		}
	}

	#endregion


	#region  Distortion

	[PostEffectPluginDescriptor]
	public class DistortionEffectPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Distortion";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\dof.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}


			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//A camera node must be selected
			SceneNode sceneNode = null;
			TreeNode selectedNode = sceneExplorer.getHierarchyTreeView().SelectedNode;
			if (selectedNode != null)
			{
				//Find camera at root of hierarchy
				while (selectedNode.Parent != null)
					selectedNode = selectedNode.Parent;

				sceneNode = sceneExplorer.getNode(selectedNode.Name) as CameraPlugin.Camera;
			}
			if (selectedNode == null || sceneNode == null)
			{
				MessageBox.Show("You must select a camera in the hierarchy view in order to add a post effect.","No Camera Selected");
				return null;
			}

			//Create a node for the post effect
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;

			//Find post effects node and add as a child
			TreeNode containerNode = null;
			foreach (TreeNode treeNode in selectedNode.Nodes)
			{
				if (sceneExplorer.getNode(treeNode.Name) as PostEffectContainer != null)
				{
					containerNode = treeNode;
					break;
				}
			}
			containerNode.Nodes.Add(topLevelNode);

			DistortionPostEffect DistortionNode = new DistortionPostEffect(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,DistortionNode);

			//Since we didn't call add tree, we need to hook up the menus manually
			sceneExplorer.hookupMenus(topLevelNode);

			//Create proxy instance
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				DistortionPostEffect distortion = sceneExplorer.getNode(node.Name) as DistortionPostEffect;
				if (distortion.Proxy != IntPtr.Zero)
					return true;

				//Get camera 2 levels up
				CameraPlugin.Camera camera = sceneExplorer.getNode(node.Parent.Parent.Name) as CameraPlugin.Camera;
				if (camera == null)
					throw new Exception("Expected a camera node exactly 2 levels up from the node");

				distortion.Proxy = XVPM.initDistortionPostEffect(XVPMHandle,camera.Proxy);
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy proxy
				DistortionPostEffect distortion = sceneExplorer.getNode(node.Name) as DistortionPostEffect;
				if (distortion.Proxy != IntPtr.Zero)
				{
					XVPM.destroyDistortionPostEffect(distortion.Proxy);
					distortion.Proxy = IntPtr.Zero;
				}
			}

			return true;
		}
	}


	[Serializable]
	public class DistortionPostEffect : XVPDesigner.Objects.BasePostEffect
	{
		public DistortionPostEffect(string name)
			: base(name)
		{
			PluginID = "Distortion";
			AcceptsControllers = false;
			Renameable = true;
		}
	}

	#endregion


	#region  MotionBlur

	[PostEffectPluginDescriptor]
	public class MotionBlurPostEffectPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Motion Blur";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\dof.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}


			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//A camera node must be selected
			SceneNode sceneNode = null;
			TreeNode selectedNode = sceneExplorer.getHierarchyTreeView().SelectedNode;
			if (selectedNode != null)
			{
				//Find camera at root of hierarchy
				while (selectedNode.Parent != null)
					selectedNode = selectedNode.Parent;

				sceneNode = sceneExplorer.getNode(selectedNode.Name) as CameraPlugin.Camera;
			}
			if (selectedNode == null || sceneNode == null)
			{
				MessageBox.Show("You must select a camera in the hierarchy view in order to add a post effect.","No Camera Selected");
				return null;
			}

			//Create a node for the post effect
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;

			//Find post effects node and add as a child
			TreeNode containerNode = null;
			foreach (TreeNode treeNode in selectedNode.Nodes)
			{
				if (sceneExplorer.getNode(treeNode.Name) as PostEffectContainer != null)
				{
					containerNode = treeNode;
					break;
				}
			}
			containerNode.Nodes.Add(topLevelNode);

			MotionBlurPostEffect MotionBlurNode = new MotionBlurPostEffect(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,MotionBlurNode);

			//Since we didn't call add tree, we need to hook up the menus manually
			sceneExplorer.hookupMenus(topLevelNode);

			//Create proxy instance
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				MotionBlurPostEffect motionBlur = sceneExplorer.getNode(node.Name) as MotionBlurPostEffect;
				if (motionBlur.Proxy != IntPtr.Zero)
					return true;

				//Get camera 2 levels up
				CameraPlugin.Camera camera = sceneExplorer.getNode(node.Parent.Parent.Name) as CameraPlugin.Camera;
				if (camera == null)
					throw new Exception("Expected a camera node exactly 2 levels up from the node");

				motionBlur.Proxy = XVPM.initMotionBlurPostEffect(XVPMHandle,camera.Proxy);
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy proxy
				MotionBlurPostEffect motionBlur = sceneExplorer.getNode(node.Name) as MotionBlurPostEffect;
				if (motionBlur.Proxy != IntPtr.Zero)
				{
					XVPM.destroyMotionBlurPostEffect(motionBlur.Proxy);
					motionBlur.Proxy = IntPtr.Zero;
				}
			}

			return true;
		}
	}


	[Serializable]
	public class MotionBlurPostEffect : XVPDesigner.Objects.BasePostEffect
	{
		public MotionBlurPostEffect(string name)
			: base(name)
		{
			PluginID = "Motion Blur";
			AcceptsControllers = false;
			Renameable = true;
		}
	}

	#endregion
}
