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
using System.ComponentModel;
using XVPManaged;
using XVPDesigner.TypeEditors;

namespace LightPlugin
{
	[SceneNodePluginDescriptor]
	public class LightPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Light";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\light.png");

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

			Light lightNode = new Light(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,lightNode);

			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			//sceneExplorer.addTree(topLevelNode);



			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				Light light = sceneExplorer.getNode(node.Name) as Light;
				light.Proxy = XVPM.initLight(XVPMHandle);
				light.XVPMHandle = XVPMHandle;
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				Light light = sceneExplorer.getNode(node.Name) as Light;
				XVPM.destroyLight(light.Proxy);
			}

			return true;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Create a camera file (xcf) to stick in the wad
			results.manifestLines.Add("LightName");
			results.manifestLines.Add(topLevelNode.Text);
			results.manifestLines.Add("LightFile");
			results.manifestLines.Add(topLevelNode.Name + ".xlf");

			//Create a new file for the light instance we're exporting
			FileStream fsout = null;
			try
			{
				fsout = File.Open(exportDir + "\\" + topLevelNode.Name + ".xlf",FileMode.Create);
				results.filenames.Add(topLevelNode.Name + ".xlf");
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + exportDir + "\\" + topLevelNode.Name + ".xlf" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (fsout != null)
					fsout.Close();
				return null;
			}
			BinaryWriter bw = new BinaryWriter(fsout);

			//Ouput light data
			Light light = sceneExplorer.getNode(topLevelNode.Name) as Light;
			if (light == null)
				throw new Exception("Expected a light node");
			light.export(bw);

			bw.Close();
			fsout.Close();

			return results;
		}
	}

	[Serializable]
	public class Light : SceneNode
	{
		public enum LightType
		{
			POINT,			
			SPOT,			
			DIRECTIONAL	
		}

		[NonSerialized]
		private uint m_XVPMHandle = 0xFFFFFFFF;
		[Browsable(false)]
		public uint XVPMHandle
		{
			get { return m_XVPMHandle; }
			set { m_XVPMHandle = value; }
		}

		private LightType m_type = LightType.POINT;
		[DisplayName("Type"),
		CategoryAttribute("Emission"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public LightType LType
		{
			get { return m_type; }
			set
			{
				m_type = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"Type",Convert.ToInt32(value));
			}
		}
		
		private Color m_diffuseColor = Color.White;
		[DisplayName("Diffuse Color"),
		CategoryAttribute("Color"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public Color DiffuseColor
		{
			get { return m_diffuseColor; }
			set
			{
				m_diffuseColor = value;
				Program.g_mainForm.setColorProperty(m_proxy,"DiffuseColor",value.R/255.0f,value.G/255.0f,value.B/255.0f,1.0f);
			}
		}
		
		private Color m_specularColor = Color.White;
		[DisplayName("Specular Color"),
		CategoryAttribute("Color"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public Color SpecularColor
		{
			get { return m_specularColor; }
			set
			{
				m_specularColor = value;
				Program.g_mainForm.setColorProperty(m_proxy,"SpecularColor",value.R / 255.0f,value.G / 255.0f,value.B / 255.0f,1.0f);
			}
		}

		private float m_cutoffDist = 10.0f;
		[DisplayName("Cutoff Distance"),
		CategoryAttribute("Emission"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5000",".01")]
		public float CutoffDistance
		{
			get { return m_cutoffDist; }
			set
			{
				m_cutoffDist = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"CutoffDistance",value);
			}
		}
		
		private float m_hotSpotDist = 5.0f;
		[DisplayName("Hot Spot Distance"),
		CategoryAttribute("Emission"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5000",".01")]
		public float HotSpotDistance
		{
			get { return m_hotSpotDist; }
			set
			{
				m_hotSpotDist = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"HotSpotDistance",value);
			}
		}

		private float m_angleCutoff = .7f;
		[DisplayName("Angle Cutoff"),
		CategoryAttribute("Emission"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","3.14159",".00001")]
		public float AngleCutoff
		{
			get { return m_angleCutoff; }
			set
			{
				m_angleCutoff = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"AngleCutoff",value);
			}
		}
		
		private float m_angleDropoff = 1.7f;
		[DisplayName("Angle Cutoff"),
		CategoryAttribute("Emission"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","3.14159",".00001")]
		public float AngleDropoff
		{
			get { return m_angleDropoff; }
			set
			{
				m_angleDropoff = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"AngleDropoff",value);
			}
		}
		
		private uint m_priority = 100;
		[DisplayName("Priority"),
		CategoryAttribute("General"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public uint Priority
		{
			get { return m_priority; }
			set
			{
				m_priority = value;
				Program.g_mainForm.setUIntProperty(m_proxy,"Priority",value);
			}
		}
		
		public Light(string name)
			: base(name)
		{
			PluginID = "Light";
			AcceptsControllers = true;
			Renameable = true;
		}

		public override void setDefaultValues()
		{
			m_type = LightType.POINT;
			m_diffuseColor = Color.White;
			m_specularColor = Color.White;
			m_cutoffDist = 10;
			m_hotSpotDist = 5;
			m_angleCutoff = .7f;
			m_angleDropoff = 1.7f;
			m_priority = 100;
		}

		public override void setCurrentValues()
		{
			LType = LType;
			DiffuseColor = DiffuseColor;
			SpecularColor = SpecularColor;
			CutoffDistance = CutoffDistance;
			HotSpotDistance = HotSpotDistance;
			AngleCutoff = AngleCutoff;
			AngleDropoff = AngleDropoff;
			Priority = Priority;
		}

		override public void export(BinaryWriter bw)
		{
			//Call base export first
			base.export(bw);

			try
			{
				//Light properties
				bw.Write((uint)m_type);
				bw.Write(m_diffuseColor.R / 255.0f);
				bw.Write(m_diffuseColor.G / 255.0f);
				bw.Write(m_diffuseColor.B / 255.0f);
				bw.Write(1.0f);
				bw.Write(m_specularColor.R / 255.0f);
				bw.Write(m_specularColor.G / 255.0f);
				bw.Write(m_specularColor.B / 255.0f);
				bw.Write(1.0f);
				bw.Write(m_cutoffDist);
				bw.Write(m_hotSpotDist);
				bw.Write(m_angleCutoff);
				bw.Write(m_angleDropoff);
				bw.Write(m_priority);
			}
			catch (NullReferenceException)
			{
				throw new Exception("One or more properties in the node \"" + Name + "\" have no value.  Make sure all properties have a value and then try re-exporting");
			}
		}
	}
};


