using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms.Design;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using XVPDesigner;
using XVPDesigner.UserControls;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using XVPDesigner.Managers;

namespace XVPDesigner.Objects
{
	[SceneNodePluginDescriptor]
	public class MaterialPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Material";

			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\material.png");

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
			Material matNode = new Material(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,matNode);

			//sceneExplorer.addTree(topLevelNode);

			//Finally, create the proxy instance
			//createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();
			Material mat = sceneExplorer.getNode(topLevelNode.Name) as Material;

			results.manifestLines.Add("Name");
			results.manifestLines.Add(topLevelNode.Name + ".xma");

			return results;
		}
	}

	[Serializable]
	public class UserMaterial : Material
	{
		public UserMaterial(string name)
			: base(name)
		{
			PluginID = "Material";
			AcceptsControllers = false;
			Renameable = true;

			setDefaultValues();
		}
	}

	[Serializable]
	public class Material : SceneNode
	{
		public enum FillModeType
		{
			WIRE = 1,				/*!< Geometry is rendered as wire frames. */
			SOLID					/*!< Geometry is rendered as solid polygons. */
		}

        public enum CullModeType
        {
            NONE = 1,	/*!< Specifies no culling will take place. */
            FRONT,		/*!< Specifies culling will occur for front faces. */
            BACK		/*!< Specifies culling will occur for back faces. */
        };

		public enum BlendType
		{
			ZERO = 1,				/*!< Blend factor is (0, 0, 0, 0). */
			ONE,					/*!< Blend factor is (1, 1, 1, 1). */
			SRC_COLOR,				/*!< Blend factor is (Rs,Gs,Bs,As). */
			INV_SRC_COLOR,			/*!< Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As. */
			SRC_ALPHA,				/*!< Blend factor is (As, As, As, As). */
			INV_SRC_ALPHA,			/*!< Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As). */
			DEST_ALPHA,				/*!< Blend factor is (Ad, Ad, Ad, Ad). */
			INV_DEST_ALPHA,			/*!< Blend factor is (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad). */
			DEST_COLOR,				/*!< Blend factor is (Rd, Gd, Bd, Ad).  */
			INV_DEST_COLOR,			/*!< Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad). */
			SRC_ALPHA_SAT,			/*!< Blend factor is (f, f, f, 1); f = min(A, 1 - Ad). */
            BLEND_FACTOR,
            INV_BLEND_FACTOR,
            SRC1_COLOR,
            INV_SRC1_COLOR,
            SRC1_ALPHA,
            INV_SRC1_ALPHA		
		};

		public enum CompareFuncType
		{
			NEVER = 1,				/*!< Always fail the test. */
			LESS,					/*!< Accept the new pixel if its value is less than the value of the current pixel. */
			EQUAL,					/*!< Accept the new pixel if its value equals the value of the current pixel. */
			LESS_EQUAL,				/*!< Accept the new pixel if its value is less than or equal to the value of the current pixel. */
			GREATER,				/*!< Accept the new pixel if its value is greater than the value of the current pixel. */
			NOT_EQUAL,				/*!< Accept the new pixel if its value does not equal the value of the current pixel. */
			GREATER_EQUAL,			/*!< Accept the new pixel if its value is greater than or equal to the value of the current pixel. */
			ALWAYS					/*!< Always pass the test. */
		}

		public enum BlendOpType
		{
			ADD = 1,				/*!< The result is the destination added to the source. */
			SUBTRACT,				/*!< The result is the destination subtracted from to the source. */
			REVSUBTRACT,			/*!< The result is the source subtracted from the destination. */
			MIN,				    /*!< The result is the minimum of the source and destination. */
			MAX					    /*!< The result is the maximum of the source and destination. */
		}

        public enum StencilOpType
        {
            KEEP = 1,		/*!< Do not update the entry in the stencil buffer. */
            ZERO,			/*!< Set the stencil-buffer entry to 0. */
            REPLACE,		/*!< Replace the stencil-buffer entry with reference value. */
            INCRSAT,		/*!< Increment the stencil-buffer entry, clamping to the maximum value. */
            DECRSAT,		/*!< Decrement the stencil-buffer entry, clamping to zero. */
            INVERT,		    /*!< Invert the bits in the stencil-buffer entry. */
            INCR,			/*!< Increment the stencil-buffer entry, wrapping to zero if the new value exceeds the maximum value. */
            DECR			/*!< Decrement the stencil-buffer entry, wrapping to the maximum value if the new value is less than zero. */
        }

		public enum TextureFilterType
		{
            MIN_MAG_MIP_POINT = 1,
            MIN_MAG_POINT_MIP_LINEAR,
            MIN_POINT_MAG_LINEAR_MIP_POINT,
            MIN_POINT_MAG_MIP_LINEAR,
            MIN_LINEAR_MAG_MIP_POINT,
            MIN_LINEAR_MAG_POINT_MIP_LINEAR,
            MIN_MAG_LINEAR_MIP_POINT,
            MIN_MAG_MIP_LINEAR,
            ANISOTROPIC,
            COMPARISON_MIN_MAG_MIP_POINT,
            COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
            COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
            COMPARISON_MIN_POINT_MAG_MIP_LINEAR0x85,
            COMPARISON_MIN_LINEAR_MAG_MIP_POINT0x90,
            COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
            COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
            COMPARISON_MIN_MAG_MIP_LINEAR,
            COMPARISON_ANISOTROPIC,
            TEXT_1BIT
        }

		public enum TextureAddressType
		{
			WRAP = 1,				/*!< Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed. */
			MIRROR,					/*!< Similar to X_TAT_WRAP, except that the texture is flipped at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again, and so on.. */
			CLAMP,					/*!< Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively. */
			BORDER,					/*!< Texture coordinates outside the range [0.0, 1.0] are set to the border color. */
			MIRROR_ONCE				/*!< Similar to X_TAT_MIRROR and X_TAT_CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value. The most common usage is for volume textures, where support for the full X_TAT_MIRROR_ONCE texture-addressing mode is not necessary, but the data is symmetric around the one axis. */
		};

		public class TextureNameList : IDisposable
		{

			public Material ownerMaterial;
			public ArrayList names = new ArrayList();
			public ArrayList filenames = new ArrayList();
			public ArrayList bitmaps = new ArrayList();
			public ArrayList imageInfos = new ArrayList();
			public ArrayList encoding = new ArrayList();
			public ArrayList description = new ArrayList();

			public void Dispose()
			{
				names.Clear();
				filenames.Clear();
				bitmaps.Clear();
				imageInfos.Clear();
				encoding.Clear();
				description.Clear();
			}
		}

        //Depth
		protected bool m_depthTest;
        [DisplayName("Depth Test"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Enable depth buffer testing.")]
        public bool DepthTest
        {
            get { return m_depthTest; }
            set 
			{ 
				m_depthTest = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"DepthTest",value);
			}
        }

		protected bool m_depthWrite;
        [DisplayName("Depth Write"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Enable depth buffer writing.")]
        public bool DepthWrite
        {
            get { return m_depthWrite; }
            set 
			{ 
				m_depthWrite = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"DepthWrite",value);
			}
        }

		protected CompareFuncType m_depthFunc;
        [DisplayName("Depth Function"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("A function that compares depth data against existing depth data.")]
        public CompareFuncType DepthFunc
        {
            get { return m_depthFunc; }
            set
			{ 
				m_depthFunc = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"DepthFunc",Convert.ToInt32(value));
			}
        }

		protected int m_depthBias;
        [DisplayName("Depth Bias"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Depth value added to a given pixel.")]
        public int DepthBias
        {
            get { return m_depthBias; }
            set 
			{ 
				m_depthBias = value;
				Program.g_mainForm.setIntProperty(m_proxy,"DepthBias",value);
			}
        }

		protected float m_depthBiasClamp;
        [DisplayName("Depth Bias Clamp"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Maximum depth bias of a pixel.")]
        public float DepthBiasClamp
        {
            get { return m_depthBiasClamp; }
            set 
			{ 
				m_depthBiasClamp = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"DepthBiasClamp",value);
			}
        }

		protected float m_slopeScaledDepthBias;
        [DisplayName("Slope Scaled Depth Bias"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Scalar on a given pixel's slope.")]
        public float SlopeScaledDepthBias
        {
            get { return m_slopeScaledDepthBias; }
            set 
			{ 
				m_slopeScaledDepthBias = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"SlopeScaledDepthBias",value);
			}
        }

		protected bool m_depthClip;
        [DisplayName("Depth Clip"),
        CategoryAttribute("Depth Buffer"),
        DescriptionAttribute("Enable clipping based on distance.")]
        public bool DepthClip
        {
            get { return m_depthClip; }
            set 
			{ 
				m_depthClip = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"DepthClip",value);
			}
        }

        //Rasterization
		protected FillModeType m_fillMode;
		[DisplayName("Fill Mode"),
		CategoryAttribute("Rasterization"),
        DescriptionAttribute("Determines the fill mode to use when rendering.")]
		public FillModeType FillMode
		{
			get { return m_fillMode; }
			set 
			{ 
				m_fillMode = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"FillMode",Convert.ToInt32(value));
			}
		}

		protected bool m_scissoring;
        [DisplayName("Scissor"),
        CategoryAttribute("Rasterization"),
        DescriptionAttribute("Enable scissor-rectangle culling. All pixels ouside an active scissor rectangle are culled.")]
        public bool Scissor
        {
            get { return m_scissoring; }
            set 
			{ 
				m_scissoring = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"Scissor",value);
			}
        }

		protected bool m_multisample;
        [DisplayName("Multisample"),
        CategoryAttribute("Rasterization"),
        DescriptionAttribute("Enable multisample antialiasing.")]
        public bool Multisample
        {
            get { return m_multisample; }
            set 
			{ 
				m_multisample = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"Multisample",value);
			}
        }

		protected bool m_aaLineEnable;
        [DisplayName("Anti-aliased Line"),
        CategoryAttribute("Rasterization"),
        DescriptionAttribute("Enable line antialiasing.")]
        public bool AALineEnable
        {
            get { return m_aaLineEnable; }
            set 
			{ 
				m_aaLineEnable = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"AntialiasedLine",value);
			}
        }
        
        //Blending
		protected bool m_alphaToCoverage;
        [DisplayName("Alpha To Coverage"),
        CategoryAttribute("Blending"),
        DescriptionAttribute("Determines whether or not to use alpha-to-coverage as a multisampling technique when setting a pixel to a rendertarget.")]
        public bool AlphaToCoverage
        {
            get { return m_alphaToCoverage; }
            set 
			{ 
				m_alphaToCoverage = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"AlphaToCoverage",value);
			}
        }

		protected bool m_alphaBlend;
        [DisplayName("Alpha Blend"),
        CategoryAttribute("Blending"),
        DescriptionAttribute("Enable (or disable) blending.")]
        public bool AlphaBlend
        {
            get { return m_alphaBlend; }
            set 
			{ 
				m_alphaBlend = value;
				Program.g_mainForm.setBoolProperty(m_proxy,"AlphaBlend",value);
			}
        }

		protected BlendOpType m_blendOp;
		[DisplayName("Blend Operation"),
		CategoryAttribute("Blending"),
        DescriptionAttribute("This blend operation defines how to combine the RGB data sources.")]
		public BlendOpType BlendOp
		{
			get { return m_blendOp; }
			set 
			{ 
				m_blendOp = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"BlendOpType",Convert.ToInt32(value));
			}
		}

		protected BlendType m_srcBlend;
		[DisplayName("Source Blend"),
		CategoryAttribute("Blending"),
        DescriptionAttribute("This blend option specifies the first RGB data source and includes an optional pre-blend operation")]
		public BlendType SrcBlend
		{
			get { return m_srcBlend; }
			set 
			{ 
				m_srcBlend = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"SrcBlend",Convert.ToInt32(value));
			}
		}

		protected BlendType m_destBlend;
		[DisplayName("Destination Blend"),
		CategoryAttribute("Blending"),
        DescriptionAttribute("This blend option specifies the second RGB data source and includes an optional pre-blend operation.")]
		public BlendType DestBlend
		{
			get { return m_destBlend; }
			set 
			{ 
				m_destBlend = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"DestBlend",Convert.ToInt32(value));
			}
		}

		protected BlendOpType m_blendOpAlpha;
        [DisplayName("Blend Operation Alpha"),
        CategoryAttribute("Blending"),
        DescriptionAttribute("This blend operation defines how to combine the alpha data sources.")]
        public BlendOpType BlendOpAlpha
        {
            get { return m_blendOpAlpha; }
            set 
			{ 
				m_blendOpAlpha = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"BlendOpAlpha",Convert.ToInt32(value));
			}
        }

		protected BlendType m_srcBlendAlpha;
        [DisplayName("Source Blend Alpha"),
        CategoryAttribute("Blending"),
        DescriptionAttribute("This blend option specifies the first alpha data source and includes an optional pre-blend operation.")]
        public BlendType SrcBlendAlpha
        {
            get { return m_srcBlendAlpha; }
            set 
			{ 
				m_srcBlendAlpha = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"SrcBlendAlpha",Convert.ToInt32(value));
			}
        }

		protected BlendType m_destBlendAlpha;
        [DisplayName("Destination Blend Alpha"),
        CategoryAttribute("Blending"),
        DescriptionAttribute("This blend option specifies the second alpha data source and includes an optional pre-blend operation.")]
        public BlendType DestBlendAlpha
        {
            get { return m_destBlendAlpha; }
            set 
			{ 
				m_destBlendAlpha = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"DestBlendAlpha",Convert.ToInt32(value));
			}
        }

		protected string m_shaderName = "White.fx";
		[Browsable(false)]
		public string ShaderName
		{
			get { return m_shaderName; }
            set 
			{
				lock (Program.g_mainForm.XVPMLockObject)
				{
					m_shaderName = value;
					Program.g_mainForm.setMaterialShaderName(m_proxy,m_shaderName);
				}
			}
		}

		public override void setDefaultValues()
		{
			m_depthTest = true;
			m_depthWrite = true;
			m_depthFunc = CompareFuncType.LESS;
            m_depthBias = 0;
            m_depthBiasClamp = 0.0f;
            m_slopeScaledDepthBias = 0.0f;
            m_depthClip = true;
            m_fillMode = FillModeType.SOLID;
            m_scissoring = true;
            m_multisample = false;
            m_aaLineEnable = false;
            m_alphaToCoverage = false;
            m_alphaBlend = false;
            m_blendOp = BlendOpType.ADD;
			m_srcBlend = BlendType.SRC_ALPHA;
			m_destBlend = BlendType.INV_SRC_ALPHA;
            m_blendOpAlpha = BlendOpType.ADD;
			m_srcBlendAlpha = BlendType.ONE;
			m_destBlendAlpha = BlendType.ZERO;


			base.setDefaultValues();
		}

		override public void setCurrentValues()
		{
			base.setCurrentValues();

			ShaderName = ShaderName;

			DepthTest = DepthTest;
			DepthWrite = DepthWrite;
			DepthFunc = DepthFunc;
			DepthBias = DepthBias;
			DepthBiasClamp = DepthBiasClamp;
			SlopeScaledDepthBias = SlopeScaledDepthBias;
			DepthClip = DepthClip;
			FillMode = FillMode;
			Scissor = Scissor;
			Multisample = Multisample;
			AALineEnable = AALineEnable;
			AlphaToCoverage = AlphaToCoverage;
			AlphaBlend = AlphaBlend;
			BlendOp = BlendOp;
			SrcBlend = SrcBlend;
			DestBlend = DestBlend;
			BlendOpAlpha = BlendOpAlpha;
			SrcBlendAlpha = SrcBlendAlpha;
			DestBlendAlpha = DestBlendAlpha;
		}
		
		public Material(string name)
			: base(name)
		{
			PluginID = "Material";
			AcceptsControllers = false;
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
			m_contextMenu.MenuItems.Add("Show in Material View", new EventHandler(handleContextMenu));
			m_contextMenu.MenuItems.Add("Save in Material Library", new EventHandler(handleContextMenu));
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
			
			if (item == "Show in Material View")
			{
				TreeNode[] nodes = scene.getMaterialTreeView().Nodes.Find(selectedNode.Name,true);
				if (nodes.Length != 0)
				{
					nodes[0].EnsureVisible();
					scene.getMaterialTreeView().SelectedNode = nodes[0];
					scene.showMaterialTab();
				}
			}
			else if (item == "Save in Material Library")
			{
				SaveFileDialog sfd = new SaveFileDialog();
				sfd.Title = "Save Material...";
				sfd.Filter = "Material Files (*.mat)|*.mat|All files (*.*)|*.*";
				sfd.FileName = "Material.mat";
				sfd.InitialDirectory = Program.g_mainForm.CurrentWorkspace.MaterialLibraryPath;
				sfd.FileName = Name;
				DialogResult result = sfd.ShowDialog();
				if (result == DialogResult.OK)
				{
					FileStream fs = null;
					try
					{
						fs = File.Open(sfd.FileName,FileMode.Create);
					}
					catch (Exception exception)
					{
						MessageBox.Show("Cannot open \"" + sfd.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
						Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
						return;
					}
					BinaryFormatter bf = new BinaryFormatter();
									
					bf.Serialize(fs,this);
					fs.Close();
				}
			}

			base.handleContextMenu(sender,e);

		}

		public virtual TextureNameList getTextureNameList()
		{
			return null;
		}

		public virtual void OnTexDrop(string texName,string texFilename)
		{
		}

		override public void handlePropertyChange(string parentName,string selectedNodeName,string propName,object value)
		{
			//For certain states, we should reapply related states.  For example, when setting
			//depth test, we should set the current value of the depth comparison function
			if (propName == "Depth Test")
				DepthFunc = DepthFunc;
			if (propName == "Alpha Blend")
			{
				BlendOp = BlendOp;
				BlendOpAlpha = BlendOpAlpha;
				SrcBlend = SrcBlend;
				DestBlend = DestBlend;
				SrcBlendAlpha = SrcBlendAlpha;
				DestBlendAlpha = DestBlendAlpha;
			}
		}

		override public void export(BinaryWriter bw)
		{
			//Call base export first
			base.export(bw);

			//Material name
			Utility.writeCString(bw,Name);

			//Shader name
			Utility.writeCString(bw,m_shaderName);

			try
			{
				//Material properties
                bw.Write(m_depthTest);
                bw.Write(m_depthWrite);
                bw.Write((uint)m_depthFunc);
                bw.Write(m_depthBias);
                bw.Write(m_depthBiasClamp);
                bw.Write(m_slopeScaledDepthBias);
                bw.Write(m_depthClip);
                bw.Write((uint)m_fillMode);
                bw.Write(m_scissoring);
                bw.Write(m_multisample);
                bw.Write(m_aaLineEnable);
                bw.Write(m_alphaToCoverage);
                bw.Write(m_alphaBlend);
                bw.Write((uint)m_blendOp);
                bw.Write((uint)m_srcBlend);
                bw.Write((uint)m_destBlend);
                bw.Write((uint)m_blendOpAlpha);
                bw.Write((uint)m_srcBlendAlpha);
                bw.Write((uint)m_destBlendAlpha);

				//If the default shader is applied, then we should indicate
				//that no shader params follow
				if (m_shaderName == "White.fx")
					bw.Write((uint)0);
			}
			catch (NullReferenceException)
			{
				throw new Exception("One or more properties in the node \"" + Name + "\" have no value.  Make sure all properties have a value and then try re-exporting");
			}
		}

		/*public class AnimationTrack
		{

			private string trackName;
			private string filename;
			private int number;
			
			[CategoryAttribute("Anim Track Class Info"),
			DescriptionAttribute("Anim Track Name"),
			Editor(typeof(FileNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
			
			public string TrackName
			{
				get { return trackName; }
				set { trackName = value; }
			}

			[CategoryAttribute("Anim Track Class Info"),
			DescriptionAttribute("Anim Track File Name")]
			public string Filename
			{
				get { return filename; }
				set { filename = value; }
			}

			[CategoryAttribute("Anim Track Class Info"),
			DescriptionAttribute("Anim Track File Name"),
			EditorAttribute(typeof(TrackBarEditor),typeof(System.Drawing.Design.UITypeEditor))]

			public int Number
			{
				get { return number; }
				set { number = value; }
			}
		};

		private string firstname;      
		private string lastname;
		private int age;
		private bool depthwrite;
		private SrcBlendType srcblend;
		private AnimationTrack[] animations;
		private Color matColor;
		private ArrayList people;

		[CategoryAttribute("Contact Information"),
		DescriptionAttribute("First name beyotch")]
		public string FirstName
		{
			get { return firstname; }
			set { firstname = value; }
		}

		[CategoryAttribute("Contact Information"),
		DescriptionAttribute("last name beyotch")]
		public string LastName
		{
			get { return lastname; }
			set
			{
				if (value != "duke")
					return;
				else
					lastname = value;
			}
		}

		[DisplayName("Pretty Age"),
		CategoryAttribute("Age Information"),
		DescriptionAttribute("age beyotch")]
		public int Age
		{
			get { return age; }
			set { age = value; }
		}

		[CategoryAttribute("Render States"),
		DescriptionAttribute("Enable/disable depth writing.")]
		public bool DepthWrite
		{
			get { return depthwrite; }
			set { depthwrite = value; }
		}

		[CategoryAttribute("Render States"),
		DescriptionAttribute("Src blending.")]
		public SrcBlendType SrcBlend
		{
			get { return srcblend; }
			set { srcblend = value; }
		}

		[CategoryAttribute("Animation Tracks"),
		DescriptionAttribute("Animation track list.")]
		public AnimationTrack[] Animations
		{
			get { return animations; }
			set { animations = value; }
		}
		
		[CategoryAttribute("Colors"),
		DescriptionAttribute("Colors."),
		EditorAttribute(typeof(ColorEditor),typeof(System.Drawing.Design.UITypeEditor))]
		public Color MaterialColor 
		{
			get { return matColor; }
			set { matColor = value; }
		}

		[CategoryAttribute("Collection"),
		DescriptionAttribute("Collection.")]
		public ArrayList People
		{
			get { return people; }
			set { people = value; }
		}
	}



	public class ComboEditor : System.Drawing.Design.UITypeEditor
	{
		private IWindowsFormsEditorService edSvc;
		public override System.Drawing.Design.UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
		{
			//If you want the editor to be based on a form (external form that appears when the user presses the ... button), 
			//return UITypeEditorEditStyle.Modal
			return System.Drawing.Design.UITypeEditorEditStyle.DropDown;
		}

		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context,System.IServiceProvider provider,object value)
		{
			edSvc = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
			if (edSvc != null)
			{
				//Create the control that will actually do the editing. This can be any standard Windows.Forms control or
				//a custom made user control
				ListBox lb = new ListBox();
				
				//Here we attch an event handler to the list box to close the combo box containing 
				//it when an item is selected
				lb.SelectedIndexChanged += new EventHandler(selectedIndexChanged);
				for (int h = 0;h < 21;h++) { lb.Items.Add(h); }
				
				//Here, you instruct the propertygrid to show the control in a combo box
				edSvc.DropDownControl(lb);
				return (lb.SelectedItem != null ? ((int)(lb.SelectedItem)) : value);
			}

			return value;
		}

		void selectedIndexChanged(object sender,EventArgs e)
		{
			edSvc.CloseDropDown();
		}

	}*/

/*

	public class ColorEditor : System.Drawing.Design.UITypeEditor
	{
		private IWindowsFormsEditorService edSvc;
		public override System.Drawing.Design.UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
		{
			//If you want the editor to be based on a form (external form that appears when the user presses the ... button), 
			//return UITypeEditorEditStyle.Modal
			return System.Drawing.Design.UITypeEditorEditStyle.DropDown;
		}

		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context,System.IServiceProvider provider,object value)
		{
			edSvc = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
			if (edSvc != null)
			{
				//Create the control that will actually do the editing. This can 
				//be any standard Windows.Forms control or a custom made user control
				ColorDialog cd = new ColorDialog();
				cd.ShowDialog();
								
				return cd.Color;
			}

			return value;
		}
	}
		 */
	}
}