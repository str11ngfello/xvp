using System;
using System.IO;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using XVPDesigner.Objects;
using XVPDesigner.Managers;
using XVPDesigner.Dialogs;
using XVPDesigner;
using System.Windows.Forms.Design;
using System.Drawing;
using System.Runtime.InteropServices;
using Dialogs;
using XVPDesigner.Documents;
using XVPManaged;
using XVPDesigner.TypeEditors;

namespace ModelPlugin
{
	[SceneNodePluginDescriptor]
	public class ModelPlugin : BasePlugin
	{
		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Model";
			
			//Load image for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\model.png");
				Bitmap image2 = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\LOD.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);
				mainForm.Icons.Images.Add("LOD",image2);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}

			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			//Display the LOD gatherer dialog to let the user specify LODs
			LODGatherer lodDialog = new LODGatherer();
			DialogResult result = lodDialog.ShowDialog();

			if (result != DialogResult.OK) 
				return null;

			//First, check to make sure another top level node doesn't already
			//have this name.  If it does, ask the user to rename the node they're
			//adding.
			/*bool checkName = true;
			string safeName = lodDialog.txtModelName.Text;
			do
			{
				if (sceneExplorer.nodeNameExists(safeName))
				{
					DuplicateNameDialog dialog = new DuplicateNameDialog();
					dialog.txtNewName.Text = safeName;
					dialog.txtNewName.Name = safeName;
					dialog.txtNewName.SelectAll();
					if (dialog.ShowDialog() == DialogResult.Cancel)
						return false;
					else
					{
						safeName = dialog.txtNewName.Text;
						continue;
					}
				}

				checkName = false;
			} while (checkName);
			*/
			//Create a top level node named after the model
			TreeNode topLevelNode = new TreeNode(lodDialog.txtModelName.Text);
		
			topLevelNode.Name = System.Guid.NewGuid().ToString();
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;
			Model model = new Model(lodDialog.txtModelName.Text);
			model.NumLODs = (uint)lodDialog.lvLODGatherer.Items.Count;
			sceneExplorer.addNode(topLevelNode.Name,model);

			

			string cmd = "";
			int d = 0;
			Hashtable GUIDTable = new Hashtable(50);
			Hashtable matGUIDTable = new Hashtable(50);
			foreach (ListViewItem i in lodDialog.lvLODGatherer.Items)
			{
				//Create LOD node
				TreeNode newNode = new TreeNode();
				newNode.Text = "LOD " + i.Text;
				newNode.Name = System.Guid.NewGuid().ToString();
				newNode.ImageKey = newNode.SelectedImageKey = "LOD";
				topLevelNode.Nodes.Add(newNode);

				ModelLOD modelLOD = new ModelLOD(newNode.Text);
				modelLOD.Filename = i.SubItems[1].Text;
				sceneExplorer.addNode(newNode.Name,modelLOD);

				if (i.Text == "0")
				{
					importModel(newNode, GUIDTable, matGUIDTable, sceneExplorer, i.SubItems[1].Text);
					cmd += "Model.init(\"" + i.SubItems[1].Text + "\")";
				}
				else
				{
					importModel(newNode,GUIDTable,matGUIDTable,sceneExplorer,i.SubItems[1].Text);
					cmd += "Model.mergeLOD(\"" + i.SubItems[1].Text + "\"," + d + ")";
				}

				modelLOD.LODDistance = d;
				d += 100;
			}

			//foreach (SceneNode sceneNode in sceneNodes)
			//{
			//	sceneExplorer.addNode(System.Guid.NewGuid().ToString(),sceneNode);
			//}

			//sceneExplorer.addTree(topLevelNode);

			cmd += "Model.generate()";
			cmd = cmd.Replace("\\","/");
			Program.g_mainForm.sendTargetCommand(cmd);

			//Finally, create the graphical instance in the scene
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		private void importModel(TreeNode parentNode,Hashtable GUIDTable,Hashtable matGUIDTable,SceneExplorer sceneExplorer,string fileName)
		{
			FileStream fs = File.Open(fileName, FileMode.Open, FileAccess.Read);
			BinaryReader br = new BinaryReader(fs);

			while (fs.Position < fs.Length)
			{
				ImportMgr.XBSChunkHeader header = new ImportMgr.XBSChunkHeader();
				byte[] buf = new byte[Marshal.SizeOf(header)];
				buf = br.ReadBytes(Marshal.SizeOf(header));
				header = (ImportMgr.XBSChunkHeader)ImportMgr.RawDeserialize(buf,header.GetType());

				ImportMgr.XBS_CHUNK_ID chunkID = (ImportMgr.XBS_CHUNK_ID)header.chunkID;
				switch (chunkID)
				{
					case (ImportMgr.XBS_CHUNK_ID.XBS_MODEL):
						uint numNodes = br.ReadUInt32();
						while (fs.Position < fs.Length)
						{
							ImportMgr.XBSChunkHeader header2 = new ImportMgr.XBSChunkHeader();
							byte[] buf2 = new byte[Marshal.SizeOf(header2)];
							buf2 = br.ReadBytes(Marshal.SizeOf(header2));
							header2 = (ImportMgr.XBSChunkHeader)ImportMgr.RawDeserialize(buf2,header2.GetType());
							TreeNode treeNode;

							switch (header2.chunkID)
							{
								case ((int)ImportMgr.XBS_CHUNK_ID.XBS_MATERIAL):
									ImportMgr.XBSMaterialChunk material = new ImportMgr.XBSMaterialChunk();
									byte[] buf3 = new byte[Marshal.SizeOf(material)];
									buf3 = br.ReadBytes(Marshal.SizeOf(material));
									material = (ImportMgr.XBSMaterialChunk)ImportMgr.RawDeserialize(buf3,material.GetType());
									break;
								case ((int)ImportMgr.XBS_CHUNK_ID.XBS_MODEL_OBJECT):
									ImportMgr.XBSModelObjectChunk modelObject = new ImportMgr.XBSModelObjectChunk();
									byte[] buf4 = new byte[Marshal.SizeOf(modelObject)];
									buf4 = br.ReadBytes(Marshal.SizeOf(modelObject));
									modelObject = (ImportMgr.XBSModelObjectChunk)ImportMgr.RawDeserialize(buf4,modelObject.GetType());

									//If we have already seen this model object name, then don't create a new node.
									//Instead, point this tree node to the same scene node. 
									string modelObjectGUID = null;
									if (GUIDTable[modelObject.name] != null)
										modelObjectGUID = (string)GUIDTable[modelObject.name];
									else
									{
										modelObjectGUID = Guid.NewGuid().ToString();
										GUIDTable.Add(modelObject.name, modelObjectGUID);
										sceneExplorer.addNode(modelObjectGUID,new ModelObject(modelObject.name));
									}

									//Make an entry into the hierarchy tree view
									string parentName;
									if (modelObject.parentName == "")
										parentName = fileName;
									else
										parentName = modelObject.parentName;

									//Get the parent's GUID.  We make the assumption that the parent
									//has already been seen and therefore the GUID exists.
									TreeNode[] treeNodes = parentNode.Nodes.Find((string)GUIDTable[parentName],true);

									string imageKey = "Dummy";
									if (modelObject.isBone != 0)
										imageKey = "Bone";
									else if (modelObject.numVertexBuffers > 0)
										imageKey = "Geometry";

									if (treeNodes.GetLength(0) != 0)
									{
										treeNode = treeNodes[0].Nodes.Add(modelObjectGUID,modelObject.name);
										treeNode.ImageKey = treeNode.SelectedImageKey = imageKey;
									}
									else
										treeNode = parentNode.Nodes.Add(modelObjectGUID,modelObject.name);
									treeNode.ImageKey = treeNode.SelectedImageKey = imageKey;

									if (modelObject.isBone == 0)
									{
										for (int i = 0; i < modelObject.numVertexBuffers; ++i)
										{
											uint pt = br.ReadUInt32();

											ImportMgr.XVertexDesc vertDesc = new ImportMgr.XVertexDesc();
											byte[] vertDescBuf = new byte[Marshal.SizeOf(vertDesc)];
											vertDescBuf = br.ReadBytes(Marshal.SizeOf(vertDesc));
											vertDesc = (ImportMgr.XVertexDesc)ImportMgr.RawDeserialize(vertDescBuf,vertDesc.GetType());

											string materialName = ASCIIEncoding.ASCII.GetString(br.ReadBytes(64));
											materialName = materialName.Remove(materialName.IndexOf("\0"));

											//Only add the material scene node once 
											string materialGUID = null;
											if (matGUIDTable[materialName] != null)
												materialGUID = (string)matGUIDTable[materialName];
											else
											{
												materialGUID = Guid.NewGuid().ToString();
												matGUIDTable.Add(materialName,materialGUID);
												sceneExplorer.addNode(materialGUID,new Material(materialName));
											}

											TreeNode materialTreeNode = treeNode.Nodes.Add(materialGUID,materialName);
											materialTreeNode.ImageKey = materialTreeNode.SelectedImageKey = "Material";

											uint numVerts = br.ReadUInt32();

											//Position
											for (int j = 0; j < numVerts; ++j)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}
											//Normals
											for (int j = 0; j < numVerts; ++j)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}
											//Colors
											for (int j = 0; j < vertDesc.numColors; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadUInt32();
												}
											}
											//UVs
											for (int j = 0; j < vertDesc.numUVs; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											for (int j = 0; j < vertDesc.numTangents; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											for (int j = 0; j < vertDesc.numBinormals; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											uint numIndicies = br.ReadUInt32();

											for (int j = 0; j < numIndicies; ++j)
											{
												br.ReadUInt16();
											}
										}
									}

									//Bounding sphere center and radius
									br.ReadSingle();
									br.ReadSingle();
									br.ReadSingle();
									br.ReadSingle();


									break;
								case ((int)ImportMgr.XBS_CHUNK_ID.XBS_SKIN_MESH):
									ImportMgr.XBSSkinChunk skinObject = new ImportMgr.XBSSkinChunk();
									buf4 = new byte[Marshal.SizeOf(skinObject)];
									buf4 = br.ReadBytes(Marshal.SizeOf(skinObject));
									skinObject = (ImportMgr.XBSSkinChunk)ImportMgr.RawDeserialize(buf4,skinObject.GetType());

									//If we have already seen this model object name, then don't create a new node.
									//Instead, point this tree node to the same scene node. 
									string skinObjectGUID = null;
									if (GUIDTable[skinObject.name] != null)
										skinObjectGUID = (string)GUIDTable[skinObject.name];
									else
									{
										skinObjectGUID = Guid.NewGuid().ToString();
										GUIDTable.Add(skinObject.name,skinObjectGUID);
										sceneExplorer.addNode(skinObjectGUID,new SkinObject(skinObject.name));
									}

									//Make an entry into the hierarchy tree view
									if (skinObject.parentName == "")
										parentName = fileName;
									else
										parentName = skinObject.parentName;

									//Get the parent's GUID.  We make the assumption that the parent
									//has already been seen and therefore the GUID exists.
									treeNodes = parentNode.Nodes.Find((string)GUIDTable[parentName],true);

									imageKey = "Skin";

									if (treeNodes.GetLength(0) != 0)
									{
										treeNode = treeNodes[0].Nodes.Add(skinObjectGUID,skinObject.name);
										treeNode.ImageKey = treeNode.SelectedImageKey = imageKey;
									}
									else
										treeNode = parentNode.Nodes.Add(skinObjectGUID,skinObject.name);
									treeNode.ImageKey = treeNode.SelectedImageKey = imageKey;

									
										for (int i = 0; i < skinObject.numVertexBuffers; ++i)
										{
											uint pt = br.ReadUInt32();

											ImportMgr.XVertexDesc vertDesc = new ImportMgr.XVertexDesc();
											byte[] vertDescBuf = new byte[Marshal.SizeOf(vertDesc)];
											vertDescBuf = br.ReadBytes(Marshal.SizeOf(vertDesc));
											vertDesc = (ImportMgr.XVertexDesc)ImportMgr.RawDeserialize(vertDescBuf,vertDesc.GetType());

											string materialName = ASCIIEncoding.ASCII.GetString(br.ReadBytes(64));
											materialName = materialName.Remove(materialName.IndexOf("\0"));

											//Only add the material scene node once 
											string materialGUID = null;
											if (matGUIDTable[materialName] != null)
												materialGUID = (string)matGUIDTable[materialName];
											else
											{
												materialGUID = Guid.NewGuid().ToString();
												matGUIDTable.Add(materialName,materialGUID);
												sceneExplorer.addNode(materialGUID,new Material(materialName));
											}

											TreeNode materialTreeNode = treeNode.Nodes.Add(materialGUID,materialName);
											materialTreeNode.ImageKey = materialTreeNode.SelectedImageKey = "Material";

											uint numVerts = br.ReadUInt32();

											//Position
											for (int j = 0; j < numVerts; ++j)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}
											//Normals
											for (int j = 0; j < numVerts; ++j)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}
											//Colors
											for (int j = 0; j < vertDesc.numColors; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadUInt32();
												}
											}
											//UVs
											for (int j = 0; j < vertDesc.numUVs; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											for (int j = 0; j < vertDesc.numTangents; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											for (int j = 0; j < vertDesc.numBinormals; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													br.ReadSingle();
													br.ReadSingle();
													br.ReadSingle();
												}
											}

											//numBones
											for (int j = 0; j < numVerts; ++j)
											{
												br.ReadUInt32();
											}

											//bone weights
											for (int k = 0; k < numVerts; ++k)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}

											//bone indices
											for (int k = 0; k < numVerts; ++k)
											{
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
												br.ReadSingle();
											}

											uint numIndicies = br.ReadUInt32();

											for (int j = 0; j < numIndicies; ++j)
											{
												br.ReadUInt16();
											}
										}
									

									//Bounding sphere center and radius
									br.ReadSingle();
									br.ReadSingle();
									br.ReadSingle();
									br.ReadSingle();


									break;
								default:
									fs.Seek(header2.sizeInBytes,SeekOrigin.Current);
									break;
							}

						}

						break;
					default:
						break;
				}

				fs.Seek(header.sizeInBytes,SeekOrigin.Current);
			}

			br.Close();
			fs.Close();

			//string str = fileName.Replace("\\","/"); ;
			//string command = "Model.init(\"" + str + "\") Model.generate()";
		}


		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			//ActorStudioDocument doc = Program.g_mainForm.getActiveActorStudioDocument();

			lock (Program.g_mainForm.XVPMLockObject)
			{
				Model model = sceneExplorer.getNode(node.Name) as Model;

				//First verify all the required assets are present or we have to fail
				foreach (TreeNode childNode in node.Nodes)
				{
					ModelLOD modelLOD = sceneExplorer.getNode(childNode.Name) as ModelLOD;
					if (modelLOD == null)
						continue;
					if (!File.Exists(modelLOD.Filename))
					{
						MessageBox.Show("Could not find model file \"" + modelLOD.Filename + "\".","Missing Asset",MessageBoxButtons.OK,MessageBoxIcon.Error);
						return false;
					}
				}

				//Create model LOD proxies
				int ctr = 0;
				foreach (TreeNode childNode in node.Nodes)
				{
					ModelLOD modelLOD = sceneExplorer.getNode(childNode.Name) as ModelLOD;
					if (modelLOD == null)
						continue;
					
					IntPtr pFilename = Marshal.StringToHGlobalAnsi(modelLOD.Filename);
					if (ctr == 0)
						model.Proxy = XVPM.initModel(XVPMHandle,pFilename,model.NumLODs);
					else
						XVPM.mergeModelLOD(model.Proxy,pFilename,modelLOD.LODDistance);			
					Marshal.FreeHGlobal(pFilename);

					//Store model lod information in the model lod node and give it a
					//reference to it's parent proxy.  Set the proxy first!
					modelLOD.ParentModelProxy = model.Proxy;
					modelLOD.LODDistance = modelLOD.LODDistance;
					

					ctr++;
				}

				XVPM.postInitModel(model.Proxy);

				//Create material proxies
				foreach (TreeNode childNode in node.Nodes)
					getMaterialProxyRecursive(childNode,sceneExplorer,model.Proxy);

				//Set the scene 
				setScene(XVPMHandle,model.SceneName,sceneExplorer,node);
			}
			
			return true;
		}

		public override bool destroyProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Destroy model proxy
				Model model = sceneExplorer.getNode(node.Name) as Model;
				if (model.Proxy != IntPtr.Zero)
					XVPM.destroyModel(model.Proxy,XVPMHandle);

				//Destroy material proxies
				foreach (TreeNode childNode in node.Nodes)
					destroyMaterialProxyRecursive(childNode,sceneExplorer,model.Proxy);
			}

			return true;
		}

		/*public override bool setActorContainer(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node,XVPDesigner.Objects.ActorContainer actorContainer)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Tell model proxy it is getting an actor container
				Model model = sceneExplorer.getNode(node.Name) as Model;
				XVPM.setModelActorContainer(model.Proxy,actorContainer.Proxy);
			}

			return false;
		}*/

		private void getMaterialProxyRecursive(TreeNode node,SceneExplorer sceneExplorer,IntPtr modelProxy)
		{ 
			Material material = sceneExplorer.getNode(node.Name) as Material;
			if (material != null)
			{
				if (material.Proxy == IntPtr.Zero)
				{
					IntPtr pMaterialName = Marshal.StringToHGlobalAnsi(material.Name);
					material.Proxy = XVPM.getModelMaterial(modelProxy,pMaterialName);
					Marshal.FreeHGlobal(pMaterialName);
				}
			}

			foreach (TreeNode childNode in node.Nodes)
				getMaterialProxyRecursive(childNode,sceneExplorer,modelProxy);
		}

		private void destroyMaterialProxyRecursive(TreeNode node,SceneExplorer sceneExplorer,IntPtr modelProxy)
		{
			Material material = sceneExplorer.getNode(node.Name) as Material;
			if (material != null)
			{
				if (material.Proxy != IntPtr.Zero)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.destroyModelMaterial(material.Proxy);
					}
					material.Proxy = IntPtr.Zero;
				}
			}
	
			foreach (TreeNode childNode in node.Nodes)
				destroyMaterialProxyRecursive(childNode,sceneExplorer,modelProxy);
		}

		private void setLightmapToMaterialRecursive(TreeNode node,SceneExplorer sceneExplorer,string lightmapPath)
		{
			Material material = sceneExplorer.getNode(node.Name) as Material;
			if (material != null)
			{
				material.OnTexDrop("Light Map",lightmapPath);
			}

			foreach (TreeNode childNode in node.Nodes)
				setLightmapToMaterialRecursive(childNode,sceneExplorer,lightmapPath);
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			ExportResults results = new ExportResults();

			//Stick the number of lods in the manifest
			results.manifestLines.Add("NumLODs");

			int lodCount = 0;
			foreach (TreeNode node in topLevelNode.Nodes)
				if (node.ImageKey != "Controller")
					lodCount++;
			results.manifestLines.Add(lodCount.ToString());

			//For each LOD
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
					if (fsout != null)
						fsout.Close();

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
											for (int j = 0; j < vertDesc.numColors; ++j)
											{
												for (int k = 0; k < numVerts; ++k)
												{
													bw.Write(br.ReadUInt32());
												}
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

			return results;
		}

		public override void setScene(uint XVPMHandle,string sceneName,SceneExplorer sceneExplorer,TreeNode treeNode)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				string finalSceneName = sceneName;
				
				//An empty string means that the scene hasn't ever been set.  That's fine,
				//just set the default scene	
				if (finalSceneName == "" || finalSceneName == null)
					finalSceneName = "Main";

				//Set scene information
				Model model = sceneExplorer.getNode(treeNode.Name) as Model;
				model.SceneName = finalSceneName;
				
				//If the set fails, then return this scene node to the default scene
				IntPtr pSceneName = Marshal.StringToHGlobalAnsi(finalSceneName);
				if (!XVPM.setScene(XVPMHandle,model.Proxy,pSceneName))
					model.SceneName = "Main";
				Marshal.FreeHGlobal(pSceneName);
			}
		}

		public override float[] getStageTransform()
		{
			return new float[16] {1.0f,0.0f,0.0f,0.0f,
								  0.0f,1.0f,0.0f,0.0f,
								  0.0f,0.0f,1.0f,0.0f,
								  0.0f,0.0f,0.0f,1.0f};
		}

		public override bool setLightmap(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node,string lightmapPath)
		{
			//Make sure the lightmap actually exists
			if (File.Exists(lightmapPath))
			{
				//Apply the light map to all the model materials
				foreach (TreeNode childNode in node.Nodes)
					setLightmapToMaterialRecursive(childNode,sceneExplorer,lightmapPath);

				return true;
			}
			else
				return false;
		}
	}

	[Serializable]
	public class Model : SceneNode
	{
		private uint m_numLODs = 0;
		[ReadOnlyAttribute(true)]
		public uint NumLODs
		{
			get { return m_numLODs; }
			set { m_numLODs = value; }
		}

		public Model(string name)
			: base(name)
		{
			PluginID = "Model";
			AcceptsControllers = false;
			Renameable = true;
		}

		public override void attachController(IntPtr controllerProxy,TreeNode nodeToAttachTo)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//If we're attaching to the top level model node, pass null as node name
				if (nodeToAttachTo.Parent == null || nodeToAttachTo.Parent.ImageKey == "Actor")
					XVPM.attachControllerToModel(Proxy,controllerProxy,IntPtr.Zero);
				else
				{
					IntPtr pNodeName = Marshal.StringToHGlobalAnsi(nodeToAttachTo.Text);
					XVPM.attachControllerToModel(Proxy,controllerProxy,pNodeName);
					Marshal.FreeHGlobal(pNodeName);
				}

			}
		}

		public override void detachController(IntPtr controllerProxy,TreeNode nodeToDetachFrom)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//If we're detaching from the top level model node, pass null as node name
				if (nodeToDetachFrom.Parent == null)
					XVPM.detachControllerFromModel(Proxy,controllerProxy,IntPtr.Zero);
				else
				{
					IntPtr pNodeName = Marshal.StringToHGlobalAnsi(nodeToDetachFrom.Text);
					XVPM.detachControllerFromModel(Proxy,controllerProxy,pNodeName);
					Marshal.FreeHGlobal(pNodeName);
				}
			}
		}
	}

	[Serializable]
	public class ModelLOD : SceneNode
	{
		//Pointer to parent model proxy
		[NonSerialized]
		IntPtr m_parentModelProxy;
		[Browsable(false)]
		public IntPtr ParentModelProxy
		{
			get { return m_parentModelProxy; }
			set { m_parentModelProxy = value; }
		}

		public string m_filename;
		[ReadOnlyAttribute(true)]
		public string Filename
		{
			get { return m_filename; }
			set { m_filename = value; }
		}

		private float m_lodDist = 0;
		[DisplayName("LOD Distance"),
		CategoryAttribute("Distances"),
		DescriptionAttribute("Distance at which to activate LOD."),
		EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor)),
		EditorRange("0.0","2500.0","1")]
		public float LODDistance
		{
			get { return m_lodDist; }
			set 
			{
				//We cannot set the 0th LOD because it translates to the highest
				//LOD.  It's distance is always 0
				uint LODIndex = uint.Parse(Name.Split(' ')[1]);
				if (LODIndex == 0)
				{
					//MessageBox.Show("The LOD distance for the highest LOD (LOD 0) cannot be changed.  It must always be 0.","Highest LOD Distance",MessageBoxButtons.OK,MessageBoxIcon.Information);
					return;
				}

				//No negative values allowed
				if (value < 0)
					return;

				m_lodDist = value;
				if (m_parentModelProxy != IntPtr.Zero)
				{
					lock (Program.g_mainForm.XVPMLockObject)
					{
						XVPM.setModelLODDist(m_parentModelProxy,LODIndex-1,value);
					}
				}
			}
		}

		public ModelLOD(string name)
			: base(name)
		{
			PluginID = "ModelLOD";
			AcceptsControllers = false;
			Renameable = false;
		}
	}

	[Serializable]
	public class ModelObject : SceneNode
	{
		public ModelObject(string name)
			: base(name)
		{
			PluginID = "ModelObject";
			AcceptsControllers = true;
			Renameable = false;
		}
	}

	[Serializable]
	public class SkinObject : SceneNode
	{
		public SkinObject(string name)
			: base(name)
		{
			PluginID = "SkinObject";
			AcceptsControllers = false;
			Renameable = false;
		}
	}
}
