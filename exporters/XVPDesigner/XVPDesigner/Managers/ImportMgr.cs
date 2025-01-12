using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using XVPDesigner.Objects;
using XVPDesigner.Dialogs;

namespace XVPDesigner.Managers
{
	public class ImportMgr
	{
		public enum XBS_CHUNK_ID
		{
			XBS_INVALID,
			XBS_MATERIAL,
			XBS_MODEL,
			XBS_MODEL_OBJECT,
			XBS_SKIN_MESH,
			XBS_BONE,
			XBS_LIGHT,
			XBS_PARTICLE_SYSTEM,
			XBS_ANIM_TRACK,
			XBS_NUM_CHUNK_IDS
		};

		[StructLayout(LayoutKind.Sequential)]
		public struct XBSChunkHeader
		{
			public int chunkID;
			public int sizeInBytes;
			public int version;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct XColor
		{
			public float r;
			public float g;
			public float b;
			public float a;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct XVector3D
		{
			public float x;
			public float y;
			public float z;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct XVertexDesc
		{
			public byte numColors;
			public byte numNormals;
			public byte numUVs;
			public byte numTangents;
			public byte numBinormals;
			public byte numBoneIndices4Tuplets;
			public byte numBoneWeights4Tuplets;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct XBSMaterialChunk
		{
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst = 64)]
			public string name;
		}


		[StructLayout(LayoutKind.Sequential)]
		public struct XMatrix4x4
		{
			public float _11,_12,_13,_14;
			public float _21,_22,_23,_24;
			public float _31,_32,_33,_34;
			public float _41,_42,_43,_44;
		};

		[StructLayout(LayoutKind.Sequential)]
		public struct XBSModelObjectChunk
		{
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst = 64)]
			public string name;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst = 64)]
			public string parentName;
			public XMatrix4x4 modelTransform;
			public XMatrix4x4 invBoneTransform;
			public uint numVertexBuffers;
			public byte isBone;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct XBSSkinChunk
		{
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=64)]
			public string name;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=64)]
			public string parentName;
			public XMatrix4x4 modelTransform;
			public uint numVertexBuffers;
		}

		public static byte[] RawSerialize(object anything)
		{
			int rawsize = Marshal.SizeOf(anything);
			IntPtr buffer = Marshal.AllocHGlobal(rawsize);
			Marshal.StructureToPtr(anything,buffer,false);
			byte[] rawdatas = new byte[rawsize];
			Marshal.Copy(buffer,rawdatas,0,rawsize);
			Marshal.FreeHGlobal(buffer);
			return rawdatas;
		}

		public static object RawDeserialize(byte[] rawdatas,Type anytype)
		{
			int rawsize = Marshal.SizeOf(anytype);
			if (rawsize > rawdatas.Length)
				return null;
			IntPtr buffer = Marshal.AllocHGlobal(rawsize);
			Marshal.Copy(rawdatas,0,buffer,rawsize);
			object retobj = Marshal.PtrToStructure(buffer,anytype);
			Marshal.FreeHGlobal(buffer);
			return retobj;
		}

#if false
		static public void importModelNode(TreeNode topLevelNode,Hashtable localHashTable,LODGatherer lodDialog)
		{
			string cmd = "";
			int d = 0;
			foreach (ListViewItem i in lodDialog.lvLODGatherer.Items) 
			{
				TreeNode newNode = new TreeNode();
				newNode.Text = "LOD_" + i.Text;
				newNode.Name = "LOD_" + i.Text;
				newNode.SelectedImageIndex = 3;
				newNode.ImageIndex = 3;
				topLevelNode.Nodes.Add(newNode); 

				if (i.Text == "0")
				{
					ImportMgr.importModel(newNode,localHashTable,i.SubItems[1].Text);
					cmd += "Model.init(\"" + i.SubItems[1].Text + "\")";
				}
				else
				{
					ImportMgr.importModel(newNode,localHashTable,i.SubItems[1].Text);
					cmd += "Model.mergeLOD(\"" + i.SubItems[1].Text + "\"," + d + ")";
				}
				
				d += 100;
			}
			cmd += "Model.generate()";
			cmd = cmd.Replace("\\","/");
			Program.g_mainForm.sendTargetCommand(cmd);
		}

			
		static public void importModel(TreeNode parentNode,Hashtable localHashTable,string fileName)
		{
			//TreeNode mainTreeNode = new TreeNode("Model");
			//mainTreeNode.SelectedImageIndex = 3;
			//mainTreeNode.ImageIndex = 3;
			
			FileStream fs = File.Open(fileName,FileMode.Open);
			BinaryReader br = new BinaryReader(fs);

			//m_tvSceneNodes.Nodes.Add(fileName,fileName,3).SelectedImageIndex = 3;
			//m_tvSceneMaterials.Nodes.Add(fileName,fileName,3).SelectedImageIndex = 3;

			while (fs.Position < fs.Length)
			{
				XBSChunkHeader header = new XBSChunkHeader();
				byte[] buf = new byte[Marshal.SizeOf(header)];
				buf = br.ReadBytes(Marshal.SizeOf(header));
				header = (XBSChunkHeader)RawDeserialize(buf,header.GetType());

				switch (header.chunkID)
				{
					case (3):
						uint numNodes = br.ReadUInt32();
						while (fs.Position < fs.Length)
						{
							XBSChunkHeader header2 = new XBSChunkHeader();
							byte[] buf2 = new byte[Marshal.SizeOf(header2)];
							buf2 = br.ReadBytes(Marshal.SizeOf(header2));
							header2 = (XBSChunkHeader)RawDeserialize(buf2,header2.GetType());
							TreeNode treeNode;

							switch (header2.chunkID)
							{
								case (1):
									XBSMaterialChunk material = new XBSMaterialChunk();
									byte[] buf3 = new byte[Marshal.SizeOf(material)];
									buf3 = br.ReadBytes(Marshal.SizeOf(material));
									material = (XBSMaterialChunk)RawDeserialize(buf3,material.GetType());

									//Make an entry into the materials tree view
									/*TreeNode[] matTreeNodes = m_tvSceneMaterials.Nodes.Find(fileName,true);
									treeNode = new Material();
									treeNode.Name = material.name;
									treeNode.Text = material.name;
									treeNode.SelectedImageIndex = 2;

									matTreeNodes[0].Nodes.Add(treeNode);//material.name, material.name, 2);
									 */

									//Only add the node to the hash table once
									if (localHashTable[material.name] == null)
										localHashTable.Add(material.name,new Material(material.name));
									//Program.g_mainForm.addNode(material.name,new Material(material.name));

									break;
								case (4):
									XBSModelObjectChunk modelObject = new XBSModelObjectChunk();
									byte[] buf4 = new byte[Marshal.SizeOf(modelObject)];
									buf4 = br.ReadBytes(Marshal.SizeOf(modelObject));
									modelObject = (XBSModelObjectChunk)RawDeserialize(buf4,modelObject.GetType());

									//Make an entry into the hierarchy tree view
									string parentName;
									if (modelObject.parentName == "")
										parentName = fileName;
									else
										parentName = modelObject.parentName;


									TreeNode[] treeNodes = parentNode.Nodes.Find(parentName,true);

									int imageNum = 0;
									if (modelObject.numVertexBuffers > 0)
										imageNum = 1;

									if (treeNodes.GetLength(0) != 0)
										treeNode = treeNodes[0].Nodes.Add(modelObject.name,modelObject.name,imageNum);
									else
										treeNode = parentNode.Nodes.Add(modelObject.name,modelObject.name,imageNum);
									treeNode.SelectedImageIndex = imageNum;

									if (localHashTable[modelObject.name] == null)
										localHashTable.Add(modelObject.name,new ModelObject(modelObject.name));
									//Program.g_mainForm.addNode(modelObject.name,new ModelObject(modelObject.name));



									for (int i = 0;i < modelObject.numVertexBuffers;++i)
									{
										uint pt = br.ReadUInt32();

										XVertexDesc vertDesc = new XVertexDesc();
										byte[] vertDescBuf = new byte[Marshal.SizeOf(vertDesc)];
										vertDescBuf = br.ReadBytes(Marshal.SizeOf(vertDesc));
										vertDesc = (XVertexDesc)RawDeserialize(vertDescBuf,vertDesc.GetType());
										//fread(&desc,sizeof(desc),1,inFile);


										//System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
										string materialName = ASCIIEncoding.ASCII.GetString(br.ReadBytes(64));
										materialName = materialName.Remove(materialName.IndexOf("\0"));
										TreeNode materialTreeNode = treeNode.Nodes.Add(materialName,materialName,2);
										materialTreeNode.SelectedImageIndex = 2;


										// string materialName = br.ReadBytes(64);
										//XS8 materialName[XBS_MAX_NAME_LEN];
										//fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

										uint numVerts = br.ReadUInt32();
										//fread(&numVerts,sizeof(numVerts),1,inFile);

										//Position
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadSingle();
											br.ReadSingle();
											br.ReadSingle();
											//fread(&tempV3d,sizeof(tempV3d),1,inFile);
											//pVB->setPosition(j,0,tempV3d);
										}
										//Normals
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadSingle();
											br.ReadSingle();
											br.ReadSingle();
											//fread(&tempV3d,sizeof(tempV3d),1,inFile);
											//pVB->setNormal(j,0,tempV3d);
										}
										//Colors
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadUInt32();

											//fread(&tempXU32,sizeof(tempXU32),1,inFile);
											//pVB->setColor(j,0,tempXU32);
										}
										//UVs
										for (int j = 0;j < vertDesc.numUVs;++j)
										{
											for (int k = 0;k < numVerts;++k)
											{
												br.ReadSingle();
												br.ReadSingle();
												//fread(&tempV2d,sizeof(tempV2d),1,inFile);
												//pVB->setUV(k,j,tempV2d);
											}
										}

										uint numIndicies = br.ReadUInt32();
										//fread(&numIndicies,sizeof(numIndicies),1,inFile);

										for (int j = 0;j < numIndicies;++j)
										{
											br.ReadUInt16();
											//XU16 i;
											//fread(&i,sizeof(i),1,inFile);

											//pIB[j] =  i;
										}
									}

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

			string str = fileName.Replace("\\","/"); ;
			string command = "Model.init(\"" + str + "\") Model.generate()";

			//m_targetMgr.sendCommand(command);

			//return mainTreeNode;
		}
	

		/*static public void importModel(bool LOD,string fileName)
		{
			FileStream fs = File.Open(fileName,FileMode.Open);
			BinaryReader br = new BinaryReader(fs);

			m_tvSceneNodes.Nodes.Add(fileName,fileName,3).SelectedImageIndex = 3;
			m_tvSceneMaterials.Nodes.Add(fileName,fileName,3).SelectedImageIndex = 3;

			while (fs.Position < fs.Length)
			{
				XBSChunkHeader header = new XBSChunkHeader();
				byte[] buf = new byte[Marshal.SizeOf(header)];
				buf = br.ReadBytes(Marshal.SizeOf(header));
				header = (XBSChunkHeader)RawDeserialize(buf,header.GetType());

				switch (header.chunkID)
				{
					case (3):
						uint numNodes = br.ReadUInt32();
						while (fs.Position < fs.Length)
						{
							XBSChunkHeader header2 = new XBSChunkHeader();
							byte[] buf2 = new byte[Marshal.SizeOf(header2)];
							buf2 = br.ReadBytes(Marshal.SizeOf(header2));
							header2 = (XBSChunkHeader)RawDeserialize(buf2,header2.GetType());
							TreeNode treeNode;

							switch (header2.chunkID)
							{
								case (1):
									XBSMaterialChunk material = new XBSMaterialChunk();
									byte[] buf3 = new byte[Marshal.SizeOf(material)];
									buf3 = br.ReadBytes(Marshal.SizeOf(material));
									material = (XBSMaterialChunk)RawDeserialize(buf3,material.GetType());

									//Make an entry into the materials tree view
									TreeNode[] matTreeNodes = m_tvSceneMaterials.Nodes.Find(fileName,true);
									treeNode = new Material();
									treeNode.Name = material.name;
									treeNode.Text = material.name;
									treeNode.SelectedImageIndex = 2;

									matTreeNodes[0].Nodes.Add(treeNode);//material.name, material.name, 2);

									break;
								case (4):
									XBSModelObjectChunk modelObject = new XBSModelObjectChunk();
									byte[] buf4 = new byte[Marshal.SizeOf(modelObject)];
									buf4 = br.ReadBytes(Marshal.SizeOf(modelObject));
									modelObject = (XBSModelObjectChunk)RawDeserialize(buf4,modelObject.GetType());

									//Make an entry into the hierarchy tree view
									string parentName;
									if (modelObject.parentName == "")
										parentName = fileName;
									else
										parentName = modelObject.parentName;


									TreeNode[] treeNodes = m_tvSceneNodes.Nodes.Find(parentName,true);

									int imageNum = 0;
									if (modelObject.numVertexBuffers > 0)
										imageNum = 1;

									if (treeNodes.GetLength(0) != 0)
										treeNode = treeNodes[0].Nodes.Add(modelObject.name,modelObject.name,imageNum);
									else
										treeNode = m_tvSceneNodes.Nodes.Add(modelObject.name,modelObject.name,imageNum);
									treeNode.SelectedImageIndex = imageNum;

									for (int i = 0;i < modelObject.numVertexBuffers;++i)
									{
										uint pt = br.ReadUInt32();

										XVertexDesc vertDesc = new XVertexDesc();
										byte[] vertDescBuf = new byte[Marshal.SizeOf(vertDesc)];
										vertDescBuf = br.ReadBytes(Marshal.SizeOf(vertDesc));
										vertDesc = (XVertexDesc)RawDeserialize(vertDescBuf,vertDesc.GetType());
										//fread(&desc,sizeof(desc),1,inFile);


										//System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
										string materialName = ASCIIEncoding.ASCII.GetString(br.ReadBytes(64));
										TreeNode materialTreeNode = treeNode.Nodes.Add(materialName,materialName,2);
										materialTreeNode.SelectedImageIndex = 2;


										// string materialName = br.ReadBytes(64);
										//XS8 materialName[XBS_MAX_NAME_LEN];
										//fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

										uint numVerts = br.ReadUInt32();
										//fread(&numVerts,sizeof(numVerts),1,inFile);

										//Position
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadSingle();
											br.ReadSingle();
											br.ReadSingle();
											//fread(&tempV3d,sizeof(tempV3d),1,inFile);
											//pVB->setPosition(j,0,tempV3d);
										}
										//Normals
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadSingle();
											br.ReadSingle();
											br.ReadSingle();
											//fread(&tempV3d,sizeof(tempV3d),1,inFile);
											//pVB->setNormal(j,0,tempV3d);
										}
										//Colors
										for (int j = 0;j < numVerts;++j)
										{
											br.ReadUInt32();

											//fread(&tempXU32,sizeof(tempXU32),1,inFile);
											//pVB->setColor(j,0,tempXU32);
										}
										//UVs
										for (int j = 0;j < vertDesc.numUVs;++j)
										{
											for (int k = 0;k < numVerts;++k)
											{
												br.ReadSingle();
												br.ReadSingle();
												//fread(&tempV2d,sizeof(tempV2d),1,inFile);
												//pVB->setUV(k,j,tempV2d);
											}
										}

										uint numIndicies = br.ReadUInt32();
										//fread(&numIndicies,sizeof(numIndicies),1,inFile);

										for (int j = 0;j < numIndicies;++j)
										{
											br.ReadUInt16();
											//XU16 i;
											//fread(&i,sizeof(i),1,inFile);

											//pIB[j] =  i;
										}
									}

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

			string str = fileName.Replace("\\","/"); ;
			string command = "Model.init(\"" + str + "\") Model.generate()";

			m_targetMgr.sendCommand(command);
		}*/
#endif
	}
}
