using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using XVPDesigner.UserControls;
using XVPDesigner.Objects;
using XVPDesigner.Managers;
using XVPDesigner.Documents;
using XVPDesigner;
using XVPManaged;
using System.Windows.Forms.Design;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using XVPDesigner.Interfaces;


namespace ParticleSystemPlugin
{
	[SceneNodePluginDescriptor]
	public class ParticleSystemPlugin : BasePlugin
	{
		private ParticleSystemActionBar m_psActionBar;

		public override bool init(MainForm mainForm)
		{
			//Set the plugin's ID first
			PluginID = "Particle System";
						
			//Load images for this plugin
			try
			{
				Bitmap image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\particlesys.png");

				//Set the image to appear in menus and treeviews
				MenuImage = image;
				mainForm.Icons.Images.Add(PluginID,image);

				image = new Bitmap(Program.g_mainForm.XVPPath + "\\xvpsdk\\system\\designer\\Plugin Media\\emitter.png");
				mainForm.Icons.Images.Add("Emitter",image);
			}
			catch (Exception)
			{
				mainForm.logMessage("Could not load images for plugin \"" + PluginID + "\"",Color.Red);
			}

			//Create exactly one instance of the action control
			m_psActionBar = new ParticleSystemActionBar();

			return true;
		}

		public override TreeNode createInstance(uint XVPMHandle,SceneExplorer sceneExplorer)
		{
			string nodeGUID = null;

			//Create a new root node for the particle system
			string psName = "Particle System";
			nodeGUID = System.Guid.NewGuid().ToString();
			TreeNode topLevelNode = new TreeNode(psName);
			topLevelNode.Name = nodeGUID;
			topLevelNode.ImageKey = topLevelNode.SelectedImageKey = PluginID;
			sceneExplorer.addNode(nodeGUID,new ParticleSystem(psName));

			//Create a new emitter container node 
			/*nodeGUID = System.Guid.NewGuid().ToString();
			TreeNode containerNode = topLevelNode.Nodes.Add(nodeGUID,"Emitter Container");
			containerNode.ImageKey = containerNode.SelectedImageKey = "Dummy";
			sceneExplorer.addNode(nodeGUID,new EmitterContainer("Emitter Container"));*/

			//Create particle system with emitters
			nodeGUID = System.Guid.NewGuid().ToString();
			TreeNode emitterNode = topLevelNode.Nodes.Add(nodeGUID,"Emitter");
			emitterNode.ImageKey = emitterNode.SelectedImageKey = "Emitter";
			sceneExplorer.addNode(nodeGUID,new Emitter("Emitter"));

			//Add a material to the emitter
			nodeGUID = System.Guid.NewGuid().ToString();
			TreeNode matNode = emitterNode.Nodes.Add(nodeGUID,"Emitter Material");
			matNode.ImageKey = matNode.SelectedImageKey = "Material";
			sceneExplorer.addNode(nodeGUID,new Material("Emitter Material"));
			
			//Add the new particle system tree representation
			//sceneExplorer.addTree(topLevelNode);

			//Finally, create the graphical instance in the scene
			createProxyInstance(XVPMHandle,sceneExplorer,topLevelNode);

			return topLevelNode;
		}

		public override bool createProxyInstance(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node)
		{
			ParticleSystem ps = sceneExplorer.getNode(node.Name) as ParticleSystem;

			lock (Program.g_mainForm.XVPMLockObject)
			{

				ps.Proxy = XVPM.initParticleSystem(XVPMHandle,(uint)node.Nodes.Count);

				//For each emitter, create a native proxy object
				uint index = 0;
				foreach (TreeNode childNode in node.Nodes)
				{
					Emitter emitter = sceneExplorer.getNode(childNode.Name) as Emitter;
					if (emitter != null)
					{
						//IntPtr  = Marshal.StringToHGlobalAnsi(childNode.Name);
						emitter.Proxy = XVPM.initEmitter(ps.Proxy,index);
						//Marshal.FreeHGlobal(pEmitterName);

						//Grab the material underneathe the emitter
						foreach (TreeNode emitterChild in childNode.Nodes)
						{
							Material material = sceneExplorer.getNode(emitterChild.Name) as Material;
							if (material != null)
							{
								material.Proxy = XVPM.getEmitterMaterial(emitter.Proxy);
							}
						}
					}
					++index;
				}
			}

			return true;
		}

		public override ExportResults exportInstance(string exportDir,SceneExplorer sceneExplorer,TreeNode topLevelNode)
		{
			//Create a new file for the particle system instance we're exporting
			FileStream fs = null;
			try
			{
				fs = File.Open(exportDir + "\\" + topLevelNode.Text + ".xps",FileMode.Create);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot open \"" + exportDir + "\\" + topLevelNode.Text + ".xps" + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return null;
			}
			BinaryWriter bw = new BinaryWriter(fs);
			
			//Output the number of emitters contained within this particle system
			bw.Write(topLevelNode.Nodes.Count);
			
			//Now, output each emitter and it's material.
			try
			{
				foreach (TreeNode node in topLevelNode.Nodes)
				{
					//Get emitter scene node
					Emitter emitter = sceneExplorer.getNode(node.Name) as Emitter;
					emitter.export(bw);

					//Emitter Material
					//Material material = sceneExplorer.getNode(node.Nodes[0].Name) as Material;
					//material.export(bw);

					//Swap out the readable material name for the GUID name
					TreeNode[] matGroupNodes = sceneExplorer.getMaterialTreeView().Nodes.Find(topLevelNode.Name,false);
					foreach (TreeNode matNode in matGroupNodes[0].Nodes)
					{
						ImportMgr.XBSMaterialChunk material = new ImportMgr.XBSMaterialChunk();
						material.name = matNode.Name + ".xma";
						byte[] buf = ImportMgr.RawSerialize(material);
						bw.Write(buf);
					}
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show("Could not export the particle sytem \"" + topLevelNode.Text + "\" for the following reason.  " + exception.ToString(),"Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
			bw.Close();
			fs.Close();

			//Output the particle system name to the manifest
			ExportResults results = new ExportResults();
			results.manifestLines.Add("Name");
			results.manifestLines.Add(topLevelNode.Text + ".xps");

			results.filenames.Add(exportDir + "\\" + topLevelNode.Text + ".xps");
			return results;
		}

		public override ActionExplorerBase getActionExplorer()
		{
			return m_psActionBar;
		}

		/*public override bool setActorContainer(uint XVPMHandle,SceneExplorer sceneExplorer,TreeNode node,XVPDesigner.Objects.ActorContainer actorContainer)
		{
			lock (Program.g_mainForm.XVPMLockObject)
			{
				//Tell proxy it is getting an actor container
				ParticleSystem ps = sceneExplorer.getNode(node.Name) as ParticleSystem;
				XVPM.setParticleSystemActorContainer(ps.Proxy,actorContainer.Proxy);
			}

			return false;
		}*/
	}
}
