using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using XVPDesigner.Objects;
using XVPDesigner;
using System.Drawing;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization;
using XVPManaged;
using System.Windows.Forms.Design;
using XVPDesigner.TypeEditors;

namespace ParticleSystemPlugin
{
	public enum EmitterType
	{
		Box,
		Sphere,
		Cylinder
	};

	[Serializable]
	public class Emitter : SceneNode
	{
		private uint m_maxParticles;
		[DisplayName("Max Particles"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("Maximum number of particles allowed at any given time.  If emission of a new particle will cause the total number of active particles from this emitter to exceed this number, emission will not occur.")]
		public uint MaxParticles
		{
			get { return m_maxParticles; }
			set 
			{ 
				m_maxParticles = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setUIntProperty(m_proxy,"MaxParticles",value);
				}
			}
		}

		private EmitterType m_emitterType;
		[DisplayName("Type"),
		CategoryAttribute("Emitter Shape"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public EmitterType EmitterType
		{
			get { return m_emitterType; }
			set 
			{ 
				m_emitterType = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setEnumProperty(m_proxy,"EmitterType",Convert.ToInt32(value));
				}
			}
		}

		private float m_emitterLength;
		[DisplayName("Length"),
	    CategoryAttribute("Emitter Shape"),
		DescriptionAttribute("If the emitter type is a sphere, this value reperesents the sphere's radius.")]
		public float EmitterLength
		{
			get { return m_emitterLength; }
			set 
			{ 
				m_emitterLength = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"EmitterLength",value);
				}
			}
		}

		private float m_emitterWidth;
		[DisplayName("Width"),
	    CategoryAttribute("Emitter Shape"),
		DescriptionAttribute("This value is ignored if the emitter type is a sphere.")]
		public float EmitterWidth
		{
			get { return m_emitterWidth; }
			set 
			{ 
				m_emitterWidth = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"EmitterWidth",value);
				}
			}
		}

		private float m_emitterHeight;
		[DisplayName("Height"),
	    CategoryAttribute("Emitter Shape"),
		DescriptionAttribute("This value is ignored if the emitter type is a sphere.")]
		public float EmitterHeight
		{
			get { return m_emitterHeight; }
			set 
			{
				m_emitterHeight = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"EmitterHeight",value);
				}
			}
		}

		private float m_birthRate;
		[DisplayName("Birth Rate"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("Particle emission rate in particles per second."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","10000.0","10")]
		public float BirthRate
		{
			get { return m_birthRate; }
			set 
			{
				m_birthRate = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"BirthRate",value);
				}
			}
		}
		
		private float m_particleGrowTime;
		[DisplayName("Grow Time"),
		CategoryAttribute("Particle Size"),
		DescriptionAttribute("The time in the beginning of a particles life over which it will grow to it's target size.")]
		public float ParticleGrowTime
		{
			get { return m_particleGrowTime; }
			set 
			{ 
				m_particleGrowTime = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleGrowTime",value);
				}
			}
		}

		private float m_particleShrinkTime;
		[DisplayName("Shrink Time"),
		CategoryAttribute("Particle Size"),
		DescriptionAttribute("The time in the end of a particles life over which it will shrink to it's target size.")]
		public float ParticleShrinkTime
		{
			get { return m_particleShrinkTime; }
			set 
			{ 
				m_particleShrinkTime = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleShrinkTime",value);
				}
			}
		}

		private float m_particleSpeed;
		[DisplayName("Speed"),
		CategoryAttribute("Particle Speed"),
		DescriptionAttribute("How fast a particle travels once emitted.")]
		public float ParticleSpeed
		{
			get { return m_particleSpeed; }
			set 
			{
				m_particleSpeed = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleSpeed",value);
				}
			}
		}

		private float m_particleSpeedVariation;
		[DisplayName("Speed Variation"),
		CategoryAttribute("Particle Speed"),
		DescriptionAttribute("A random +/- bias applied to a particle's initial speed.")]
		public float ParticleSpeedVariation
		{
			get { return m_particleSpeedVariation; }
			set 
			{ 
				m_particleSpeedVariation = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleSpeedVariation",value);
				}
			}
		}

		private float m_particleLife;
		[DisplayName("Life"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("How long a particle will live.")]
		public float ParticleLife
		{
			get { return m_particleLife; }
			set 
			{ 
				m_particleLife = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleLife",value);
				}
			}
		}

		private float m_particleLifeVariation;
		[DisplayName("Life Variation"),
		CategoryAttribute("Particle Generation"),
	    DescriptionAttribute("A random +/- bias applied to a particle's initial lifetime.")]
		public float ParticleLifeVariation
		{
			get { return m_particleLifeVariation; }
			set 
			{ 
				m_particleLifeVariation = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleLifeVariation",value);
				}
			}
		}

		private float m_particleRotSpeed;
		[DisplayName("Rotation Speed"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("How fast the particle rotates.")]
		public float ParticleRotSpeed
		{
			get { return m_particleRotSpeed; }
			set
			{
				m_particleRotSpeed = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleRotSpeed",value);
				}
			}
		}

		private float m_particleRotSpeedVariation;
		[DisplayName("Rotation Speed Variation"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("A random +/- bias applied to a particle's initial rotation speed.")]
		public float ParticleRotSpeedVariation
		{
			get { return m_particleRotSpeedVariation; }
			set
			{
				m_particleRotSpeedVariation = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleRotSpeedVariation",value);
				}
			}
		}
		
		private float m_particleSize;
		[DisplayName("Size"),
		CategoryAttribute("Particle Size"),
		DescriptionAttribute("Initial particle size.")]
		public float ParticleSize
		{
			get { return m_particleSize; }
			set 
			{ 
				m_particleSize = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleSize",value);
				}
			}
		}

		private float m_particleSizeVariation;
		[DisplayName("Size Variation"),
		CategoryAttribute("Particle Size"),
		DescriptionAttribute("A random +/- bias applied to a particle's initial size.")]
		public float ParticleSizeVariation
		{
			get { return m_particleSizeVariation; }
			set 
			{ 
				m_particleSizeVariation = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"ParticleSizeVariation",value);
				}
			}
		}

		private Color m_particleStartColor;
		[DisplayName("Start Color"),
		CategoryAttribute("Particle Color"),
		DescriptionAttribute("The initial color value given to a particle at the beginning of it's life.")]
		public Color ParticleStartColor
		{
			get { return m_particleStartColor; }
			set 
			{
				m_particleStartColor = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setColorProperty(m_proxy,
														"StartColor",
														m_particleStartColor.R / 255.0f,
														m_particleStartColor.G / 255.0f,
														m_particleStartColor.B / 255.0f,
														m_particleStartAlpha / 255.0f);
				}
			}
		}

		private byte m_particleStartAlpha;
		[DisplayName("Start Alpha"),
		CategoryAttribute("Particle Color"),
		DescriptionAttribute("The initial alpha value given to a particle at the beginning of it's life.")]
		public byte ParticleStartAlpha
		{
			get { return m_particleStartAlpha; }
			set 
			{ 
				m_particleStartAlpha = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setColorProperty(m_proxy,
														"StartColor",
														m_particleStartColor.R / 255.0f,
														m_particleStartColor.G / 255.0f,
														m_particleStartColor.B / 255.0f,
														m_particleStartAlpha / 255.0f);
				}
			}
		}
		
		private Color m_particleEndColor;
		[DisplayName("End Color"),
		CategoryAttribute("Particle Color"),
		DescriptionAttribute("The final color value for a particle which it will reach through linear interpolation from it's start color.")]
		public Color ParticleEndColor
		{
			get { return m_particleEndColor; }
			set 
			{ 
				m_particleEndColor = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setColorProperty(m_proxy,
														"EndColor",
														m_particleEndColor.R / 255.0f,
														m_particleEndColor.G / 255.0f,
														m_particleEndColor.B / 255.0f,
														m_particleEndAlpha / 255.0f);
				}
			}
		}

		private byte m_particleEndAlpha;
		[DisplayName("End Alpha"),
		CategoryAttribute("Particle Color"),
		DescriptionAttribute("The final alpha value given to a particle at the beginning of it's life.")]
		public byte ParticleEndAlpha
		{
			get { return m_particleEndAlpha; }
			set 
			{ 
				m_particleEndAlpha = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setColorProperty(m_proxy,
														"EndColor",
														m_particleEndColor.R / 255.0f,
														m_particleEndColor.G / 255.0f,
														m_particleEndColor.B / 255.0f,
														m_particleEndAlpha / 255.0f);
				}
			}
		}

	
		private float[] m_directionVector;
		[DisplayName("Direction Vector"),
		CategoryAttribute("Particle Generation"),
		EditorAttribute(typeof(DirVecTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		DescriptionAttribute("Initial emission direction vector for particles.")]
		public float[] DirectionVector
		{
			get { return m_directionVector; }
			set 
			{
				if (value.Length == 3)
				{
					m_directionVector = value;
					Program.g_mainForm.setVector3DProperty(m_proxy,"DirectionVec",value[0],value[1],value[2]);
				}
				else
					MessageBox.Show("The vector must have exactly 3 components - x,y and z.","Wrong Number of Elements",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}

		private float m_directionVectorVariation;
		[DisplayName("Direction Variation"),
		CategoryAttribute("Particle Generation"),
	    DescriptionAttribute("A random +/- bias applied to a particle's initial emission direction.")]
		public float DirectionVectorVariation
		{
			get { return m_directionVectorVariation; }
			set 
			{ 
				m_directionVectorVariation = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setFloatProperty(m_proxy,"DirectionVecVariation",value);
				}
			}
		}
		
		private bool m_oneShot = false;
		[DisplayName("One Shot"),
		CategoryAttribute("Particle Generation"),
		DescriptionAttribute("Particle system emits particles in one frame and never emits again.  The number of particles emitted is determined by birth rate.")]
		public bool OneShot
		{
			get { return m_oneShot; }
			set
			{
				m_oneShot = value;

				lock (Program.g_mainForm.XVPMLockObject)
				{
					Program.g_mainForm.setBoolProperty(m_proxy,"OneShot",value);
				}
			}
		}

		override public void setDefaultValues()
		{
			MaxParticles = 100;
			EmitterType = EmitterType.Sphere;
			EmitterLength = .5f;
			EmitterWidth = .5f;
			EmitterHeight = .5f;
			BirthRate = 10.0f;
			ParticleGrowTime = 0.0f;
			ParticleShrinkTime = 0.0f;
			ParticleSpeed = 1.0f;
			ParticleSpeedVariation = 0.0f;
			ParticleLife = 2.0f;
			ParticleLifeVariation = 0.0f;
			ParticleRotSpeed = 0.0f;
			ParticleRotSpeedVariation = 0.0f;
			ParticleSize = 1.0f;
			ParticleSizeVariation = 0.0f;
			ParticleStartColor = Color.White;
			ParticleStartAlpha = 255;
			ParticleStartColor = Color.White;
			ParticleEndAlpha = 255;
			DirectionVector = new float[3] { 0,1,0 };
			DirectionVectorVariation = 0.0f;
			OneShot = false;

			base.setDefaultValues();
		}

		override public void setCurrentValues()
		{
			MaxParticles = MaxParticles;
			EmitterType = EmitterType;
			EmitterLength = EmitterLength;
			EmitterWidth = EmitterWidth;
			EmitterHeight = EmitterHeight;
			BirthRate = BirthRate;
			ParticleGrowTime = ParticleGrowTime;
			ParticleShrinkTime = ParticleShrinkTime;
			ParticleSpeed = ParticleSpeed;
			ParticleSpeedVariation = ParticleSpeedVariation;
			ParticleLife = ParticleLife;
			ParticleLifeVariation = ParticleLifeVariation;
			ParticleRotSpeed = ParticleRotSpeed;
			ParticleRotSpeedVariation = ParticleRotSpeedVariation;
			ParticleSize = ParticleSize;
			ParticleSizeVariation = ParticleSizeVariation;
			ParticleStartColor = ParticleStartColor;
			ParticleStartAlpha = ParticleStartAlpha;
			ParticleStartColor = ParticleStartColor;
			ParticleEndAlpha = ParticleEndAlpha;
			DirectionVector = DirectionVector;
			DirectionVectorVariation = DirectionVectorVariation;
			OneShot = OneShot;

			base.setCurrentValues();
		}
		
		public Emitter(string name)
			: base(name)
		{
			PluginID = "Emitter";
			AcceptsControllers = true;
			Renameable = true;

			setDefaultValues();
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
			m_contextMenu.MenuItems.Add("Remove",new EventHandler(handleContextMenu));
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

			if (item == "Remove")
			{
				//Remove materials from the material view
				scene.removeMaterials(selectedNode,false);

				//Remove controllers from the controller view
				//scene.removeControllers(selectedNode,false);
				System.Console.WriteLine("I still need another method that removes controllers from sub trees.  I'd like to combine my detach/remove controller functions to make more sense if possible");

				//Remove emitter node and emitter material
				scene.removeNode(selectedNode.Name);
				scene.removeNode(selectedNode.Nodes[0].Name);

				//Remove nodes from tree view
				selectedNode.Remove();
			}

			base.handleContextMenu(sender,e);

		}

		override public void handlePropertyChange(string parentName,string selectedNodeName,string propName,object value)
		{
			Program.g_mainForm.sendTargetCommand("ParticleSystem.setEmitterProperty(\"" + parentName + "\",\"" + selectedNodeName + "\",\"" + propName + "\",\"" + value.ToString() + "\")");
		}
 
		override public void export(BinaryWriter bw)
		{
			//Call base export first
			base.export(bw);

			//Emitter name
			Utility.writeCString(bw,Name);

			try
			{
				//Emitter properties
				bw.Write(m_maxParticles);
				bw.Write((uint)m_emitterType);
				bw.Write(m_emitterLength);
				bw.Write(m_emitterWidth);
				bw.Write(m_emitterHeight);
				bw.Write(m_birthRate);
				bw.Write(m_particleGrowTime);
				bw.Write(m_particleShrinkTime);
				bw.Write(m_particleSpeed);
				bw.Write(m_particleSpeedVariation);
				bw.Write(m_particleLife);
				bw.Write(m_particleLifeVariation);
				bw.Write(m_particleRotSpeed);
				bw.Write(m_particleRotSpeedVariation);
				bw.Write(m_particleSize);
				bw.Write(m_particleSizeVariation);
				bw.Write(m_directionVector[0]);
				bw.Write(m_directionVector[1]);
				bw.Write(m_directionVector[2]);
				bw.Write(m_directionVectorVariation);
				bw.Write(m_particleStartColor.R / 255.0f);
				bw.Write(m_particleStartColor.G / 255.0f);
				bw.Write(m_particleStartColor.B / 255.0f);
				bw.Write(m_particleStartAlpha / 255.0f);
				bw.Write(m_particleEndColor.R / 255.0f);
				bw.Write(m_particleEndColor.G / 255.0f);
				bw.Write(m_particleEndColor.B / 255.0f); 
				bw.Write(m_particleEndAlpha / 255.0f);
				bw.Write(m_oneShot);
			}
			catch (NullReferenceException)
			{
				throw new Exception("One or more properties in the node \"" + Name + "\" have no value.  Make sure all properties have a value and then try re-exporting");
			}
		}
	}
}


