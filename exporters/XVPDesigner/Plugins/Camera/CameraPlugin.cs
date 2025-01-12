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
using XVPDesigner.TypeEditors;
using XVPDesigner.Documents;
using XVPManaged;

namespace CameraPlugin
{
	//This enum must match XVP
	public enum ProjectionType
	{
		Perspective,
		Orthographic
	};

	[SceneNodePluginDescriptor]
	public class CameraPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Camera";

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


			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//Create a top level node
			TreeNode topLevelNode = new TreeNode(PluginID);
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;
			Camera cameraNode = new Camera(PluginID);
			sceneExplorer.addNode(topLevelNode.Name,cameraNode);

			TreeNode postEffectsTreeNode = new TreeNode();
			postEffectsTreeNode.Text = "Post Effects";
			postEffectsTreeNode.Name = System.Guid.NewGuid().ToString();
			postEffectsTreeNode.ImageKey = postEffectsTreeNode.SelectedImageKey = "PostEffect";
			PostEffectContainer containerNode = new PostEffectContainer("Post Effects");
			sceneExplorer.addNode(postEffectsTreeNode.Name,containerNode);
			topLevelNode.Nodes.Add(postEffectsTreeNode);

			TreeNode hdrTreeNode = new TreeNode();
			hdrTreeNode.Text = "High Dynamic Range";
			hdrTreeNode.Name = System.Guid.NewGuid().ToString();
			hdrTreeNode.ImageKey = hdrTreeNode.SelectedImageKey = "HDR";
			HDRPostEffect hdrNode = new HDRPostEffect("HDR");
			sceneExplorer.addNode(hdrTreeNode.Name,hdrNode);
			postEffectsTreeNode.Nodes.Add(hdrTreeNode);
		
			//sceneExplorer.addTree(topLevelNode);

			//Finally, create the proxy instance
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				Camera camera = sceneExplorer.getNode(node.Name) as Camera;
				camera.Proxy = XVPM.initCamera(XVPMHandle);
				camera.XVPMHandle = XVPMHandle;

				PostEffectContainer containerNode = null;
				HDRPostEffect hdrNode = null;
			
				//Find post effects container node and all post effects node
				foreach (TreeNode childNode in node.Nodes)
				{
					containerNode = sceneExplorer.getNode(childNode.Name) as PostEffectContainer;
					if (containerNode == null)
						continue;

					foreach (TreeNode postEffectNode in childNode.Nodes)
					{
						hdrNode = sceneExplorer.getNode(postEffectNode.Name) as HDRPostEffect;
						if (hdrNode == null)
						{
							//Get the node's type ID but make sure it's a scene node
							SceneNode sceneNode = (SceneNode)sceneExplorer.Hashtable[postEffectNode.Name];
							if (sceneNode == null)
								continue;

							Program.g_mainForm.PluginMgr.createProxyInstance(sceneNode.PluginID,sceneExplorer,postEffectNode);
						}
						else
						{
							//Create HDR proxy
							hdrNode.Proxy = XVPM.initHDRPostEffect(camera.Proxy);
						}
					}

				}
			}

			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			//ActorStudioDocument doc = Program.g_mainForm.getActiveActorStudioDocument();

			lock (Program.g_mainForm.XVPMLockObject)
			{
				Camera camera = sceneExplorer.getNode(node.Name) as Camera;

				PostEffectContainer containerNode = null;
				HDRPostEffect hdrNode = null;

				//Find post effects container node and all post effects node
				foreach (TreeNode childNode in node.Nodes)
				{
					containerNode = sceneExplorer.getNode(childNode.Name) as PostEffectContainer;
					if (containerNode == null)
						continue;

					foreach (TreeNode postEffectNode in childNode.Nodes)
					{
						hdrNode = sceneExplorer.getNode(postEffectNode.Name) as HDRPostEffect;
						if (hdrNode == null)
						{
							//Get the node's type ID but make sure it's a scene node
							SceneNode sceneNode = (SceneNode)sceneExplorer.Hashtable[postEffectNode.Name];
							if (sceneNode == null)
								continue;

							Program.g_mainForm.PluginMgr.destroyProxyInstance(sceneNode.PluginID,sceneExplorer,postEffectNode);
						}
						else
						{
							//Destroy HDR proxy
							XVPM.destroyHDRPostEffect(hdrNode.Proxy);
						}
					}

				}

				XVPM.destroyCamera(XVPMHandle,camera.Proxy);

			}

			return true;

			/*ActorStudioDocument doc = Program.g_mainForm.getActiveActorStudioDocument();

			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy proxy
				Camera camera = sceneExplorer.getNode(node.Name) as Camera;
				XVPM.destroyCamera(doc.XVPMHandle,camera.Proxy);
			}
			
			return true;*/
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Create a camera file (xcf) to stick in the wad
			results.manifestLines.Add("CameraName");
			results.manifestLines.Add(topLevelNode.Text);
			results.manifestLines.Add("CameraFile");
			results.manifestLines.Add(topLevelNode.Name + ".xcf");

			//Create a new file for the camera instance we're exporting
			FileStream fsout = null;
			try
			{
				fsout = File.Open(exportDir + "\\" + topLevelNode.Name + ".xcf",FileMode.Create);
				results.filenames.Add(topLevelNode.Name + ".xcf");
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + exportDir + "\\" + topLevelNode.Name + ".xcf" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (fsout != null)
					fsout.Close();
				return null;
			}
			BinaryWriter bw = new BinaryWriter(fsout);

			//Ouput camera data
			Camera camera = sceneExplorer.getNode(topLevelNode.Name) as Camera;
			if (camera == null)
				throw new Exception("Expected a camera node");
			camera.export(bw);

			bw.Close();
			fsout.Close();

			/*
			//Stick the number of post effects in the manifest
			results.manifestLines.Add("NumPostFX");

			int pfxCount = 0;
			TreeNode postEffectNode = topLevelNode.Nodes["Post Effects"];
			if (postEffectNode == null)
				throw new Exception("Expected a post effect node");
			foreach (TreeNode node in postEffectNode.Nodes)
				pfxCount++;
			results.manifestLines.Add(pfxCount.ToString());



			//For each post effect
			foreach (TreeNode lodNode in topLevelNode.Nodes)
			{
				//If this node is a controller, skip it
				if (lodNode.ImageKey == "Controller")
					continue;

				//Get the LOD node
				ModelLOD lod = sceneExplorer.getNode(lodNode.Name) as ModelLOD;
				if (lod == null)
					continue;

				//Create a new file for the model instance we're exporting
				FileStream fsout = null;
				try
				{
					fsout = File.Open(exportDir + "\\" + lodNode.Name + ".xmf",FileMode.Create);
					results.filenames.Add(lodNode.Name + ".xmf");
					results.manifestLines.Add(lodNode.Text);
					results.manifestLines.Add(lodNode.Name + ".xmf");

					//results.manifestLines.Add(lodNode.Text + "Dist=" + lod.LODDistance);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + exportDir + "\\" + lodNode.Name + ".xmf" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					return null;
				}
				BinaryWriter bw = new BinaryWriter(fsout);

				//Now open the source model file
				FileStream fsin = File.Open(lod.Filename,FileMode.Open,FileAccess.Read);
				BinaryReader br = new BinaryReader(fsin);

				//Export LOD information
				while (fsin.Position < fsin.Length)
				{
					TreeNode[] matGroupNodes;

					ImportMgr.XBSChunkHeader header = new ImportMgr.XBSChunkHeader();
					byte[] buf = new byte[Marshal.SizeOf(header)];
					buf = br.ReadBytes(Marshal.SizeOf(header));
					bw.Write(buf);
					header = (ImportMgr.XBSChunkHeader)ImportMgr.RawDeserialize(buf,header.GetType());

					ImportMgr.XBS_CHUNK_ID chunkID = (ImportMgr.XBS_CHUNK_ID)header.chunkID;
					switch (chunkID)
					{
						case (ImportMgr.XBS_CHUNK_ID.XBS_MODEL):
							uint numNodes = br.ReadUInt32();
							bw.Write(numNodes);
							while (fsin.Position < fsin.Length)
							{
								ImportMgr.XBSChunkHeader header2 = new ImportMgr.XBSChunkHeader();
								byte[] buf2 = new byte[Marshal.SizeOf(header2)];
								buf2 = br.ReadBytes(Marshal.SizeOf(header2));
								bw.Write(buf2);
								header2 = (ImportMgr.XBSChunkHeader)ImportMgr.RawDeserialize(buf2,header2.GetType());

								switch (header2.chunkID)
								{
									case ((int)ImportMgr.XBS_CHUNK_ID.XBS_MATERIAL):
										ImportMgr.XBSMaterialChunk material = new ImportMgr.XBSMaterialChunk();
										byte[] buf3 = new byte[Marshal.SizeOf(material)];
										buf3 = br.ReadBytes(Marshal.SizeOf(material));

										//Swap out the readable material name for the GUID name
										matGroupNodes = sceneExplorer.getMaterialTreeView().Nodes.Find(topLevelNode.Name,false);
										foreach (TreeNode matNode in matGroupNodes[0].Nodes)
										{
											string matName = ASCIIEncoding.ASCII.GetString(buf3);
											matName = matName.Remove(matName.IndexOf("\0"));
											if (matNode.Text == matName)
											{
												ASCIIEncoding.ASCII.GetBytes(matNode.Name,0,matNode.Name.Length,buf3,0);
												buf3[matNode.Name.Length] = (byte)'.';
												buf3[matNode.Name.Length+1] = (byte)'x';
												buf3[matNode.Name.Length+2] = (byte)'m';
												buf3[matNode.Name.Length+3] = (byte)'a';
												buf3[matNode.Name.Length+4] = (byte)'\0';
												break;
											}
										}
										bw.Write(buf3);

										material = (ImportMgr.XBSMaterialChunk)ImportMgr.RawDeserialize(buf3,material.GetType());
										break;
									case ((int)ImportMgr.XBS_CHUNK_ID.XBS_MODEL_OBJECT):
										ImportMgr.XBSModelObjectChunk modelObject = new ImportMgr.XBSModelObjectChunk();
										byte[] buf4 = new byte[Marshal.SizeOf(modelObject)];
										buf4 = br.ReadBytes(Marshal.SizeOf(modelObject));
										bw.Write(buf4);
										modelObject = (ImportMgr.XBSModelObjectChunk)ImportMgr.RawDeserialize(buf4,modelObject.GetType());

										for (int i = 0; i < modelObject.numVertexBuffers; ++i)
										{
											uint pt = br.ReadUInt32();
											bw.Write(pt);

											ImportMgr.XVertexDesc vertDesc = new ImportMgr.XVertexDesc();
											byte[] vertDescBuf = new byte[Marshal.SizeOf(vertDesc)];
											vertDescBuf = br.ReadBytes(Marshal.SizeOf(vertDesc));
											bw.Write(vertDescBuf);
											vertDesc = (ImportMgr.XVertexDesc)ImportMgr.RawDeserialize(vertDescBuf,vertDesc.GetType());

											byte[] matNameBytes = new byte[64];
											matNameBytes = br.ReadBytes(64);


											//Swap out the readable material name for the GUID name
											matGroupNodes = sceneExplorer.getMaterialTreeView().Nodes.Find(topLevelNode.Name,false);
											foreach (TreeNode matNode in matGroupNodes[0].Nodes)
											{
												string matName = ASCIIEncoding.ASCII.GetString(matNameBytes);
												matName = matName.Remove(matName.IndexOf("\0"));
												if (matNode.Text == matName)
												{
													ASCIIEncoding.ASCII.GetBytes(matNode.Name,0,matNode.Name.Length,matNameBytes,0);
													matNameBytes[matNode.Name.Length] = (byte)'.';
													matNameBytes[matNode.Name.Length + 1] = (byte)'x';
													matNameBytes[matNode.Name.Length + 2] = (byte)'m';
													matNameBytes[matNode.Name.Length + 3] = (byte)'a';
													matNameBytes[matNode.Name.Length + 4] = (byte)'\0';
													break;
												}
											}
											bw.Write(matNameBytes);
											string materialName = ASCIIEncoding.ASCII.GetString(matNameBytes);

											uint numVerts = br.ReadUInt32();
											bw.Write(numVerts);

											//Position
											for (int j = 0; j < numVerts; ++j)
											{
												bw.Write(br.ReadSingle());
												bw.Write(br.ReadSingle());
												bw.Write(br.ReadSingle());
											}
											//Normals
											for (int j = 0; j < numVerts; ++j)
											{
												bw.Write(br.ReadSingle());
												bw.Write(br.ReadSingle());
												bw.Write(br.ReadSingle());
											}
											//Colors
											for (int j = 0; j < numVerts; ++j)
											{
												bw.Write(br.ReadUInt32());
											}
											//UVs
											for (int j = 0; j < vertDesc.numUVs; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													bw.Write(br.ReadSingle());
													bw.Write(br.ReadSingle());
												}
											}

											for (int j = 0; j < vertDesc.numTangents; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													bw.Write(br.ReadSingle());
													bw.Write(br.ReadSingle());
													bw.Write(br.ReadSingle());
												}
											}

											for (int j = 0; j < vertDesc.numBinormals; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													bw.Write(br.ReadSingle());
													bw.Write(br.ReadSingle());
													bw.Write(br.ReadSingle());
												}
											}

											uint numIndicies = br.ReadUInt32();
											bw.Write(numIndicies);

											for (int j = 0; j < numIndicies; ++j)
											{
												bw.Write(br.ReadUInt16());
											}
										}

										//Bounding sphere center and radius
										bw.Write(br.ReadSingle());
										bw.Write(br.ReadSingle());
										bw.Write(br.ReadSingle());
										bw.Write(br.ReadSingle());

										break;
									default:
										fsin.Seek(header2.sizeInBytes,SeekOrigin.Current);
										break;
								}



							}

							break;
						default:
							break;
					}

					fsin.Seek(header.sizeInBytes,SeekOrigin.Current);
				}

				br.Close();
				bw.Close();
				fsin.Close();
				fsout.Close();
			}
			*/
			return results;
		}
	}

	[Serializable]
	public class Camera : SceneNode , CameraProvider
	{
		[NonSerialized]
		private uint m_XVPMHandle = 0xFFFFFFFF;
		[Browsable(false)]
		public uint XVPMHandle
		{
			get { return m_XVPMHandle; }
			set { m_XVPMHandle = value; }
		}

		private float m_horizontalFOV = 3.14159f/4.0f;
		[DisplayName("Horizontal FOV"),
		CategoryAttribute("Field of View"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","3.0",".00001")]
		public float HorizontalFOV
		{
			get { return m_horizontalFOV; }
			set
			{
				m_horizontalFOV = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"HorizontalFOV",value);
			}
		}
		
		private float m_nearClipPlane = .001f;
		[DisplayName("Near Clip Plane"),
		CategoryAttribute("Clip Planes"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.001","10000.0",".001")]
		public float NearClipPlane
		{
			get { return m_nearClipPlane; }
			set
			{
				m_nearClipPlane = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"NearClipPlane",value);
			}
		}
		
		private float m_farClipPlane = 10000.0f;
		[DisplayName("Far Clip Plane"),
		CategoryAttribute("Clip Planes"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.001","10000.0",".001")]
		public float FarClipPlane
		{
			get { return m_farClipPlane; }
			set
			{
				m_farClipPlane = value;
				Program.g_mainForm.setFloatProperty(m_proxy,"FarClipPlane",value);
			}
		}
		
		private ProjectionType m_projectionType = ProjectionType.Perspective;
		[DisplayName("Projection Type"),
		CategoryAttribute("Projection"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public ProjectionType Projection
		{
			get { return m_projectionType; }
			set 
			{ 
				m_projectionType = value;
				Program.g_mainForm.setEnumProperty(m_proxy,"ProjectionType",Convert.ToInt32(value));
			}
		}

		public Camera(string name)
			: base(name)
		{
			PluginID = "Camera";
			AcceptsControllers = true;
			Renameable = true;
		}

		public override void setDefaultValues()
		{
			HorizontalFOV = 3.14159f/4.0f;
			NearClipPlane = .001f;
			FarClipPlane = 10000.0f;
			Projection = ProjectionType.Perspective;
		}

		public override void setCurrentValues()
		{
			HorizontalFOV = HorizontalFOV;
			NearClipPlane = NearClipPlane;
			FarClipPlane = FarClipPlane;
			Projection = Projection;
		}

		override public void export(BinaryWriter bw)
		{
			//Call base export first
			base.export(bw);

			try
			{
				//Camera properties
				bw.Write(m_nearClipPlane);
				bw.Write(m_farClipPlane);
				bw.Write(m_horizontalFOV);
				bw.Write((uint)m_projectionType);
			}
			catch (NullReferenceException)
			{
				throw new Exception("One or more properties in the node \"" + Name + "\" have no value.  Make sure all properties have a value and then try re-exporting");
			}
		}
		
		public string setAsCamera(uint viewportIndex)
		{
			bool success = false;
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//ActorStudioDocument doc = Program.g_mainForm.getActiveActorStudioDocument();
				success = XVPM.setUserCamera(XVPMHandle,Proxy,viewportIndex);
			}

			if (!success)
			{
				MessageBox.Show("This camera is already assigned to a viewport.  A camera can only be assigned to one viewport at a time. ","Camera Already Assigned To Viewport",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return null;
			}
			else
				return Name;
		}

		public void removeAllPostEffects()
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.removeAllPostEffects(Proxy);
			}
		}

		public void addPostEffect(XVPDesigner.Objects.BasePostEffect postEffect)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				XVPM.addPostEffect(Proxy,postEffect.Proxy);
			}
		}
	}

	[Serializable]
	public class PostEffectContainer : SceneNode
	{
		public PostEffectContainer(string name)
			: base(name)
		{
			PluginID = "PostEffectContainer";
			AcceptsControllers = false;
			Renameable = false;
		}
	}

	[Serializable]
	public class HDRPostEffect : XVPDesigner.Objects.BasePostEffect
	{
		private float m_middleGrayKey = 0.7f;
		[DisplayName("Middle Gray Key"),
		CategoryAttribute("Tonemapping"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","5.0",".00001")]
		public float MiddleGrayKey
		{
			get { return m_middleGrayKey; }
			set
			{
				m_middleGrayKey = value;
				Program.g_mainForm.setFloatProperty(Proxy,"MiddleGrayKey",value);
			}
		}

		private float m_brightPassThreshold = 2.1f;
		[DisplayName("Bright Pass Threshold"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5.0",".00001")]
		public float BrightPassThreshold
		{
			get { return m_brightPassThreshold; }
			set
			{
				m_brightPassThreshold = value;
				Program.g_mainForm.setFloatProperty(Proxy,"BrightPassThreshold",value);
			}
		}

		private float m_brightPassOffset = 2.2f;
		[DisplayName("Bright Pass Offset"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5.0",".00001")]
		public float BrightPassOffset
		{
			get { return m_brightPassOffset; }
			set
			{
				m_brightPassOffset = value;
				Program.g_mainForm.setFloatProperty(Proxy,"BrightPassOffset",value);
			}
		}

		private float m_luminanceWhite = 2.0f;
		[DisplayName("Luminance White"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5.0",".00001")]
		public float LuminanceWhite
		{
			get { return m_luminanceWhite; }
			set
			{
				m_luminanceWhite = value;
				Program.g_mainForm.setFloatProperty(Proxy,"LuminanceWhite",value);
			}
		}

		private float m_bloomScale = .25f;
		[DisplayName("Bloom Scale"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
	    EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","5.0",".00001")]
		public float BloomScale
		{
			get { return m_bloomScale; }
			set
			{
				m_bloomScale = value;
				Program.g_mainForm.setFloatProperty(Proxy,"BloomScale",value);
			}
		}

		private float m_exposureClamp = 0.0f;
		[DisplayName("Exposure Clamp"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0","1.0",".00001")]
		public float ExposureClamp
		{
			get { return m_exposureClamp; }
			set
			{
				m_exposureClamp = value;
				Program.g_mainForm.setFloatProperty(Proxy,"ExposureClamp",value);
			}
		}

		private float m_adaptationSpeed = 0.0f;
		[DisplayName("Adaptation Speed"),
		CategoryAttribute("Tonemapping"),
	    DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
	    EditorRange("0.0",".5",".00001")]
		public float AdaptationSpeed
		{
			get { return m_adaptationSpeed; }
			set
			{
				m_adaptationSpeed = value;
				Program.g_mainForm.setFloatProperty(Proxy,"AdaptationSpeed",value);
			}
		}
		
		private bool m_instantAdapt = false;
		[DisplayName("Instant Adapt"),
		CategoryAttribute("Tonemapping"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission.")]
		public bool InstantAdapt
		{
			get { return m_instantAdapt; }
			set
			{
				m_instantAdapt = value;
				Program.g_mainForm.setBoolProperty(Proxy,"InstantAdapt",value);
			}
		}

		private float m_brightness = 0.0f;
		[DisplayName("Brightness"),
		CategoryAttribute("Final Conditioning"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("-1.0","1.0",".00001")]
		public float Brightness
		{
			get { return m_brightness; }
			set
			{
				m_brightness = value;
				Program.g_mainForm.setFloatProperty(Proxy,"Brightness",value);
			}
		}
		
		private float m_contrast = 1.0f;
		[DisplayName("Contrast"),
	    CategoryAttribute("Final Conditioning"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","10.0",".00001")]
		public float Contrast
		{
			get { return m_contrast; }
			set
			{
				m_contrast = value;
				Program.g_mainForm.setFloatProperty(Proxy,"Contrast",value);
			}
		}

		private float m_desaturation = 0.0f;
		[DisplayName("Desaturation"),
	    CategoryAttribute("Final Conditioning"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","1.0",".00001")]
		public float Desaturation
		{
			get { return m_desaturation; }
			set
			{
				m_desaturation = value;
				Program.g_mainForm.setFloatProperty(Proxy,"Desaturation",value);
			}
		}

		private float m_colorBalanceR = 0.0f;
		[DisplayName("Red"),
		CategoryAttribute("Color Balance"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("-1.0","1.0",".00001")]
		public float ColorBalanceRed
		{
			get { return m_colorBalanceR; }
			set
			{
				m_colorBalanceR = value;
				Program.g_mainForm.setFloatProperty(Proxy,"ColorBalanceRed",value);
			}
		}

		private float m_colorBalanceG = 0.0f;
		[DisplayName("Green"),
		CategoryAttribute("Color Balance"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("-1.0","1.0",".00001")]
		public float ColorBalanceGreen
		{
			get { return m_colorBalanceG; }
			set
			{
				m_colorBalanceG = value;
				Program.g_mainForm.setFloatProperty(Proxy,"ColorBalanceGreen",value);
			}
		}

		private float m_colorBalanceB = 0.0f;
		[DisplayName("Blue"),
		CategoryAttribute("Color Balance"),
		DescriptionAttribute("Type of emitter that describes the shape for the volume of emission."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("-1.0","1.0",".00001")]
		public float ColorBalanceBlue
		{
			get { return m_colorBalanceB; }
			set
			{
				m_colorBalanceB = value;
				Program.g_mainForm.setFloatProperty(Proxy,"ColorBalanceBlue",value);
			}
		}
		
		public HDRPostEffect(string name)
			: base(name)
		{
			PluginID = "High Dynamic Range";
			AcceptsControllers = false;
			Renameable = false;
			Moveable = false;
		}

		public override void setDefaultValues()
		{
			MiddleGrayKey = 0.7f;
			BrightPassThreshold = 2.1f;
			BrightPassOffset = 2.2f;
			LuminanceWhite = 2.0f;
			BloomScale = .25f;
			ExposureClamp = 0.0f;
			AdaptationSpeed = 0.0f;
			InstantAdapt = false;
			Brightness = 0.0f;
			Contrast = 1.0f;
			Desaturation = 0.0f;
		}

		public override void setCurrentValues()
		{
			MiddleGrayKey = MiddleGrayKey;
			BrightPassThreshold = BrightPassThreshold;
			BrightPassOffset = BrightPassOffset;
			LuminanceWhite = LuminanceWhite;
			BloomScale = BloomScale;
			ExposureClamp = ExposureClamp;
			AdaptationSpeed = AdaptationSpeed;
			InstantAdapt = InstantAdapt;
			Brightness = Brightness;
			Contrast = Contrast;
			Desaturation = Desaturation;
		}
	}

}
