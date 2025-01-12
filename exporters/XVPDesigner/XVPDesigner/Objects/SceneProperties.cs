using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms.Design;
using XVPDesigner.Managers;
using XVPDesigner.UserControls;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization;
using XVPManaged;
using XVPDesigner.TypeEditors;

namespace XVPDesigner.Objects
{
	[SceneNodePluginDescriptor]
	public class ScenePropertiesPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Scene Properties";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\globe2.png");

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
			//Create a top level node
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;
			SceneProperties spNode = new SceneProperties(PluginID);
			spNode.XVPMHandle = XVPMHandle;
			sceneExplorer.addNode(topLevelNode.Name,spNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			SceneProperties sceneProperties = sceneExplorer.getNode(node.Name) as SceneProperties;
			sceneProperties.XVPMHandle = XVPMHandle;
			sceneProperties.setCurrentValues();
			return true;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			/*//Get the texture node				
			Texture texture = sceneExplorer.getNode(topLevelNode.Name) as Texture;

			if (texture != null)
			{
				//If no texture is specified, we have nothing to export
				if (texture.Filename != null && texture.Filename != "")
				{
					//Stick the texture into the wad file.  Build the cache path
					//to the converted texture...it should have been converted by now.
					string temp = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(texture.Filename);
					string dir = Path.GetDirectoryName(temp);
					string convertedPath = dir + "\\" + Path.GetFileNameWithoutExtension(texture.Filename) + ".xif";
					if (!File.Exists(convertedPath))
						throw new Exception("Converted texture file " + convertedPath + " could not be found.");

					results.filenames.Add(convertedPath);

					//Output the texture name to the manifest
					results.manifestLines.Add("Name");
					results.manifestLines.Add(Path.GetFileName(convertedPath));
				}
				else
					Program.g_mainForm.logMessage("No texture filename was specified for texture plugin " + topLevelNode.Name + ". Texture will not be exported.",Color.Blue);
			}
			else
				throw new Exception("Expected a texture node");
				*/
			return results;
		}
	}



	[Serializable]
	class SceneProperties : SceneNode
	{
		public enum Scene
		{
			Main,
			Distortion
		}

		public SceneProperties(string name)
			: base(name)
		{
			PluginID = "Scene Properties";
			AcceptsControllers = false;
			Renameable = true;

			setDefaultValues();
		}

		static public Scene convertSceneName(string sceneName)
		{
			switch (sceneName)
			{
				case "Main":
					return Scene.Main;
				case "Distortion":
					return Scene.Distortion;
				default:
					return Scene.Main;
			}
		}

		public override void setDefaultValues()
		{
			FogColor = Color.FromArgb(128,128,128);
			FogStart = 0.0f;
			FogEnd = 10000.0f;
			FogDensity = 1.0f;
			GlobalAmbient = Color.FromArgb(51,51,51);
			base.setDefaultValues();
		}

		override public void setCurrentValues()
		{
			Active = Active;
			FogColor = FogColor;
			FogStart = FogStart;
			FogEnd = FogEnd;
			FogDensity = FogDensity;
			GlobalAmbient = GlobalAmbient;
			base.setCurrentValues();
		}

		[OnDeserializingAttribute()]
		internal void deserializeInit(StreamingContext context)
		{
			//Proxy pointers are old and not valid
			m_proxy = IntPtr.Zero;

			setDefaultValues();
		}

		protected override void createExtendedContextMenu()
		{
			//Add our own commands
			m_contextMenu.MenuItems.Add("Set Active",new EventHandler(handleContextMenu));
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

			if (item == "Set Active")
			{
				//Deactivate all scene property nodes
				foreach (string key in scene.Hashtable.Keys)
				{
					SceneProperties sceneNode = scene.Hashtable[key] as SceneProperties;
					if (sceneNode != null)
					{
						sceneNode.Active = false;
					}
				}

				//Set only this node active
				SceneProperties sceneProperties = scene.getNode(selectedNode.Name) as SceneProperties;
				sceneProperties.Active = true;
			}

			//Let the base scene node see the event
			base.handleContextMenu(sender,e);

		}

		private uint m_XVPMHandle = 0xFFFFFFFF;
		[Browsable(false)]
		public uint XVPMHandle
		{
			get { return m_XVPMHandle; }
			set { m_XVPMHandle = value; }
		}

		private bool m_active = false;
		[Browsable(false)]
		public bool Active
		{
			get { return m_active; }
			set
			{
				if (value && !m_active)
				{
					m_active = value;
					setCurrentValues();
				}
				else
					m_active = value;
			}
		}

		private Color m_fogColor;
		[DisplayName("Fog Color"),
		CategoryAttribute("Fog"),
		DescriptionAttribute("RGB value to use for the scene's fog color.")]
		public Color FogColor
		{
			get { return m_fogColor; }
			set
			{
				m_fogColor = value;
				if (m_active)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setFogColor(XVPMHandle,value.R/255.0f,value.G/255.0f,value.B/255.0f);
					}
				}
			}
		}

		private float m_fogStart;
		[DisplayName("Fog Start"),
		CategoryAttribute("Fog"),
		DescriptionAttribute("The distance from the camera at which fogging starts.  This only applies to linear fog."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1000000",".001")]
		public float FogStart
		{
			get { return m_fogStart; }
			set
			{
				m_fogStart = value;
				if (m_active)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setFogStart(XVPMHandle,value);
					}
				}
			}
		}

		private float m_fogEnd;
		[DisplayName("Fog End"),
		CategoryAttribute("Fog"),
		DescriptionAttribute("The distance from the camera at which fogging ends."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1000000",".001")]
		public float FogEnd
		{
			get { return m_fogEnd; }
			set
			{
				m_fogEnd = value;
				if (m_active)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setFogEnd(XVPMHandle,value);
					}
				}
			}
		}

		private float m_fogDensity;
		[DisplayName("Fog Density"),
		CategoryAttribute("Fog"),
		DescriptionAttribute("Controls how thick the fog is.  This only applies to EXP and EXP2 fog types."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","100",".00001")]
		public float FogDensity
		{
			get { return m_fogDensity; }
			set
			{
				m_fogDensity = value;
				if (m_active)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setFogDensity(XVPMHandle,value);
					}
				}
			}
		}

		private Color m_globalAmbient;
		[DisplayName("Global Ambient"),
		CategoryAttribute("Lighting"),
		DescriptionAttribute("RGB value to use for the indirect ambient lighting in the scene.")]
		public Color GlobalAmbient
		{
			get { return m_globalAmbient; }
			set
			{
				m_globalAmbient = value;
				if (m_active)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setGlobalAmbient(XVPMHandle,value.R/255.0f,value.G/255.0f,value.B/255.0f);
					}
				}
			}
		}
	}
}
