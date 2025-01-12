using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using XVPDesigner.Objects;
using System.Media;
using System.Diagnostics;
using TextureBrowserLibrary;
using System.Threading;
using System.Collections;
using XVPDesigner.Documents;
using ICSharpCode.TextEditor.Document;

namespace XVPDesigner.UserControls
{
	public partial class LibraryExplorer : UserControl
	{
		//Paths
		private string m_actorLibraryPath;
		public string ActorLibraryPath
		{
			get { return m_actorLibraryPath; }
			set
			{
				m_actorLibraryPath = value;
				if (Directory.Exists(m_actorLibraryPath))
				{
					actorDirWatcher.Path = m_actorLibraryPath;
					lock (tvActorLibrary) { ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text); }
					tvActorPreview.Nodes.Clear();
				}
			}
		}

		private string m_materialLibraryPath;
		public string MaterialLibraryPath
		{
			get { return m_materialLibraryPath; }
			set
			{
				m_materialLibraryPath = value;
				if (Directory.Exists(m_materialLibraryPath))
				{
					materialDirWatcher.Path = m_materialLibraryPath;
					lock (tvMaterialLibrary) { MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text); }
					pgridMaterials.SelectedObject = null;
				}
			}
		}

		private string m_controllerLibraryPath;
		public string ControllerLibraryPath
		{
			get { return m_controllerLibraryPath; }
			set
			{
				m_controllerLibraryPath = value;
				if (Directory.Exists(m_controllerLibraryPath))
				{
					controllerDirWatcher.Path = m_controllerLibraryPath;
					lock (tvControllerLibrary) { ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text); }
					pgridControllers.SelectedObject = null;
				}
			}
		}

		private string m_audioLibraryPath;
		public string AudioLibraryPath
		{
			get { return m_audioLibraryPath; }
			set
			{
				m_audioLibraryPath = value;
				if (Directory.Exists(m_audioLibraryPath))
				{
					audioDirWatcher.Path = m_audioLibraryPath;
					lock (tvAudioLibrary) { AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text); }
				}
			}
		}

		private string m_textureLibraryPath;
		public string TextureLibraryPath
		{
			get { return m_textureLibraryPath; }
			set
			{
				m_textureLibraryPath = value;
				if (Directory.Exists(m_textureLibraryPath))
				{
					textureDirWatcher.Path = m_textureLibraryPath;
					lock (tvTextureLibrary) { TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text); }
					clearTextureBrowser();
				}
			}
		}

		private string m_scriptLibraryPath;
		public string ScriptLibraryPath
		{
			get { return m_scriptLibraryPath; }
			set
			{
				m_scriptLibraryPath = value;
				if (Directory.Exists(m_scriptLibraryPath))
				{
					scriptDirWatcher.Path = m_scriptLibraryPath;
					lock (tvScriptLibrary) { ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text); }
				}
			}
		}

		Direct3DLoader m_d3dLoader = null;
		Material.TextureNameList m_queuedTexNameList = null;
		string m_queuedActorFilename = null;
		string m_queuedMaterialFilename = null;
		string m_queuedControllerFilename = null;

		//Delegates
		private delegate void addActorNodeDelegate(TreeNode treeNode);
		private addActorNodeDelegate m_addActorNodeInvoker;
		private void addActorNodeHandler(TreeNode treeNode)
		{
			tvActorPreview.Nodes.Add(treeNode);
		}

		private delegate void clearActorPreviewDelegate();
		private clearActorPreviewDelegate m_clearActorPreviewInvoker;
		private void clearActorPreviewHandler()
		{
			tvActorPreview.Nodes.Clear();
		}

		private bool m_oneClickAdd = false;
		public bool OneClickAdd
		{
			get { return m_oneClickAdd; }
			set { m_oneClickAdd = value; }
		}

		public LibraryExplorer()
		{
			InitializeComponent();
		}

		public void init(string actorLibraryPath,string materialLibraryPath,
						 string controllerLibraryPath,string audioLibraryPath,
						 string textureLibraryPath,string scriptLibraryPath)
		{
			//Verify paths
			if (!Directory.Exists(actorLibraryPath))
				throw new Exception("Invalid actor library path \"" + actorLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");
			if (!Directory.Exists(materialLibraryPath))
				throw new Exception("Invalid material library path \"" + materialLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");
			if (!Directory.Exists(audioLibraryPath))
				throw new Exception("Invalid audio library path \"" + audioLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");
			if (!Directory.Exists(controllerLibraryPath))
				throw new Exception("Invalid controller library path \"" + controllerLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");
			if (!Directory.Exists(textureLibraryPath))
				throw new Exception("Invalid texture library path \"" + textureLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");
			if (!Directory.Exists(scriptLibraryPath))
				throw new Exception("Invalid script library path \"" + scriptLibraryPath + "\" used to initialize Library Explorer.  Make sure the path exists.");

			if (m_d3dLoader == null)
				m_d3dLoader = new Direct3DLoader(this.Handle);
			
			tvActorPreview.ImageList = Program.g_mainForm.Icons;

			FileSyntaxModeProvider fsmProvider;
			fsmProvider = new FileSyntaxModeProvider(Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Syntax Highlighting"); // Create new provider with the highlighting directory.
			HighlightingManager.Manager.AddSyntaxModeFileProvider(fsmProvider); // Attach to the text editor.
			txtScriptPreview.SetHighlighting("lua");

			//Setup delegates for control marshalling across threads
			m_addActorNodeInvoker = new addActorNodeDelegate(addActorNodeHandler);
			m_clearActorPreviewInvoker = new clearActorPreviewDelegate(clearActorPreviewHandler);

			//Setup a file watchers to refresh the list of templates when
			//something changes
			actorDirWatcher.IncludeSubdirectories = true;
			actorDirWatcher.Filter = "*";
			actorDirWatcher.Path = actorLibraryPath; 
			actorDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;
			
			materialDirWatcher.IncludeSubdirectories = true;
			materialDirWatcher.Filter = "*";
			materialDirWatcher.Path = materialLibraryPath;
			materialDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			controllerDirWatcher.IncludeSubdirectories = true;
			controllerDirWatcher.Filter = "*";
			controllerDirWatcher.Path = controllerLibraryPath;
			controllerDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			audioDirWatcher.IncludeSubdirectories = true;
			audioDirWatcher.Filter = "*";
			audioDirWatcher.Path = audioLibraryPath;
			audioDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			textureDirWatcher.IncludeSubdirectories = true;
			textureDirWatcher.Filter = "*";
			textureDirWatcher.Path = textureLibraryPath;
			textureDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			scriptDirWatcher.IncludeSubdirectories = true;
			scriptDirWatcher.Filter = "*";
			scriptDirWatcher.Path = scriptLibraryPath;
			scriptDirWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			m_actorLibraryPath = actorLibraryPath;
			m_materialLibraryPath = materialLibraryPath;
			m_controllerLibraryPath = controllerLibraryPath;
			m_audioLibraryPath = audioLibraryPath;
			m_textureLibraryPath = textureLibraryPath;
			m_scriptLibraryPath = scriptLibraryPath;

			//Search for templates
			lock (tvActorLibrary) {	ActorDirSearch(actorLibraryPath,txtActorFilter.Text); }
			lock (tvMaterialLibrary) { MaterialDirSearch(materialLibraryPath,txtMaterialFilter.Text); }
			lock (tvControllerLibrary) { ControllerDirSearch(controllerLibraryPath,txtControllerFilter.Text); }
			lock (tvAudioLibrary) { AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text); }
			lock (tvTextureLibrary) { TextureDirSearch(textureLibraryPath,txtTextureFilter.Text); }
			lock (tvScriptLibrary) { ScriptDirSearch(scriptLibraryPath,txtScriptFilter.Text); }

		}

		private void ActorDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvActorLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				//Skip actor conversion directories
				if (!d.EndsWith("_obj"))
				{
					TreeNode dirNode = tvActorLibrary.Nodes.Add(d,Path.GetFileName(d));
					dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
					ActorDirSearchRecursive(d,dirNode,filterStringLC);
				}
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory,"*.act"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterStringLC))
				{
					TreeNode templateNode = tvActorLibrary.Nodes.Add(f,Path.GetFileNameWithoutExtension(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Actor";
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvActorLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}

			//Add a text editor for the script preview window
		}

		private void ActorDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (!d.EndsWith("_obj"))
				{
					TreeNode dirNode = node.Nodes.Add(d,Path.GetFileName(d));
					dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
					ActorDirSearchRecursive(d,dirNode,filterString);
				}
			}
			
			foreach (string f in Directory.GetFiles(directory,"*.act"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterString))
				{
					TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Actor";
				}
			}
		}


		private void MaterialDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvMaterialLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				TreeNode dirNode = tvMaterialLibrary.Nodes.Add(d, Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				MaterialDirSearchRecursive(d,dirNode,filterStringLC);
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory, "*.mat"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterStringLC))
				{
					TreeNode templateNode = tvMaterialLibrary.Nodes.Add(f,Path.GetFileNameWithoutExtension(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Material";
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvMaterialLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}
		}

		private void MaterialDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				TreeNode dirNode = node.Nodes.Add(d, Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				MaterialDirSearchRecursive(d,dirNode,filterString);
			}

			foreach (string f in Directory.GetFiles(directory, "*.mat"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterString))
				{
					TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Material";
				}
			}
		}

		private void ControllerDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvControllerLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				TreeNode dirNode = tvControllerLibrary.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				ControllerDirSearchRecursive(d,dirNode,filterStringLC);
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory,"*.con"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterStringLC))
				{
					TreeNode templateNode = tvControllerLibrary.Nodes.Add(f,Path.GetFileNameWithoutExtension(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Controller";
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvControllerLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}

		}

		private void ControllerDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				TreeNode dirNode = node.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				ControllerDirSearchRecursive(d,dirNode,filterString);
			}

			foreach (string f in Directory.GetFiles(directory,"*.con"))
			{
				if (Path.GetFileNameWithoutExtension(f).ToLower().Contains(filterString))
				{
					TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Controller";
				}
			}
		}
		
		private void AudioDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvAudioLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".AudioCache"))
					continue;
				TreeNode dirNode = tvAudioLibrary.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				AudioDirSearchRecursive(d,dirNode,filterStringLC);
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory,"*"))
			{
				if (Path.GetFileName(f).ToLower().Contains(filterStringLC))
				{
					TreeNode templateNode = tvAudioLibrary.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Audio";
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvAudioLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}

		}

		private void AudioDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".AudioCache"))
					continue;
				TreeNode dirNode = node.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				AudioDirSearchRecursive(d,dirNode,filterString);
			}

			foreach (string f in Directory.GetFiles(directory,"*"))
			{
				if (filterString != null)
				{
					if (Path.GetFileName(f).ToLower().Contains(filterString))
					{
						TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
						templateNode.ImageKey = templateNode.SelectedImageKey = "Audio";
					}
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvAudioLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}
		}

		private void TextureDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvTextureLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".TextureCache"))
					continue;
				//Ignore cache directory
				if (d.Contains(Program.g_mainForm.CurrentWorkspace.TextureLibraryPath + "\\" +
								Program.g_mainForm.CurrentWorkspace.TextureLibraryCacheDirName))
					continue;

				TreeNode dirNode = null;
				if (!chkEnableTextureFilter.Checked)
				{
					dirNode = tvTextureLibrary.Nodes.Add(d,Path.GetFileName(d));
					dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				}
										
				TextureDirSearchRecursive(d,dirNode,filterStringLC);
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory,"*"))
			{
				if (chkEnableTextureFilter.Checked)
				{
					if (Path.GetFileName(f).ToLower().Contains(filterStringLC))
					{
						TreeNode templateNode = tvTextureLibrary.Nodes.Add(f,Path.GetFileName(f));
						templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
					}
				}
				else
				{
					TreeNode templateNode = tvTextureLibrary.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
				}

			}

			//Remove empty folders
			/*foreach (TreeNode folderNode in tvTextureLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}*/

		}

		private void TextureDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".TextureCache"))
					continue; 
				TreeNode dirNode = null;
				if (node != null)
				{
					dirNode = node.Nodes.Add(d,Path.GetFileName(d));
					dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				}
				TextureDirSearchRecursive(d,dirNode,filterString);
			}

			foreach (string f in Directory.GetFiles(directory,"*"))
			{
				if (chkEnableTextureFilter.Checked)
				{
					if (Path.GetFileName(f).ToLower().Contains(filterString))
					{
						if (node != null)
						{
							TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
							templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
						}
						else
						{
							TreeNode templateNode = tvTextureLibrary.Nodes.Add(f,Path.GetFileName(f));
							templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
						}
					}
				}
				else
				{
					if (node != null)
					{
						TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
						templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
					}
					else
					{
						TreeNode templateNode = tvTextureLibrary.Nodes.Add(f,Path.GetFileName(f));
						templateNode.ImageKey = templateNode.SelectedImageKey = "Texture";
					}
				}
			}
		}

		private void ScriptDirSearch(string directory,string filterString)
		{
			string filterStringLC = filterString.ToLower();

			//First, clear out the contents of the tree view
			tvScriptLibrary.Nodes.Clear();

			//For each directory in root directory, recurse it and find templates
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".ScriptCache"))
					continue;
				TreeNode dirNode = tvScriptLibrary.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				ScriptDirSearchRecursive(d,dirNode,filterStringLC);
			}

			//For each file in root directory, add it to the explorer
			foreach (string f in Directory.GetFiles(directory,"*.lua"))
			{
				if (Path.GetFileName(f).ToLower().Contains(filterStringLC))
				{
					TreeNode templateNode = tvScriptLibrary.Nodes.Add(f,Path.GetFileName(f));
					templateNode.ImageKey = templateNode.SelectedImageKey = "Script";
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvScriptLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}

		}

		private void ScriptDirSearchRecursive(string directory,TreeNode node,string filterString)
		{
			foreach (string d in Directory.GetDirectories(directory))
			{
				if (d.Contains(".ScriptCache"))
					continue;
				TreeNode dirNode = node.Nodes.Add(d,Path.GetFileName(d));
				dirNode.ImageKey = dirNode.SelectedImageKey = "Folder";
				ScriptDirSearchRecursive(d,dirNode,filterString);
			}

			foreach (string f in Directory.GetFiles(directory,"*.lua"))
			{
				if (filterString != null)
				{
					if (Path.GetFileName(f).ToLower().Contains(filterString))
					{
						TreeNode templateNode = node.Nodes.Add(f,Path.GetFileName(f));
						templateNode.ImageKey = templateNode.SelectedImageKey = "Script";
					}
				}
			}

			//Remove empty folders
			foreach (TreeNode folderNode in tvScriptLibrary.Nodes)
			{
				if (folderNode.ImageKey == "Folder")
				{
					if (folderNode.Nodes.Count == 0)
						folderNode.Remove();
				}
			}
		}
		
		private void tvActorLibrary_MouseDown(object sender,MouseEventArgs e)
		{
			TreeNode selectedNode = tvActorLibrary.GetNodeAt(e.X,e.Y);

			if (selectedNode != null)
			{
				//Can't dragdrop folders
				if (selectedNode.ImageKey == "Folder")
					return;

				tvActorLibrary.SelectedNode = selectedNode;
				tvActorLibrary.DoDragDrop(selectedNode,DragDropEffects.Copy);

				/*m_actorSceneExplorer.postDeserializeFixup();
				m_actorSceneExplorer.init(m_shaderMgr.getShaderNames());
				m_actorSceneExplorer.Dock = DockStyle.Fill;
				m_activeSceneExplorer = m_actorSceneExplorer;
	
				SceneDockContainer.Controls.Clear();
				SceneDockContainer.Controls.Add(m_actorSceneExplorer);*/


				//m_currentActorPath = filename;
				//m_actorSceneExplorer.Dirty = false;
				//m_actorPropertyExplorer.Dirty = false;

				//addRecentActor(m_currentActorPath);

			}
		}

		private void tvMaterialLibrary_MouseDown(object sender, MouseEventArgs e)
		{
			TreeNode selectedNode = tvMaterialLibrary.GetNodeAt(e.X, e.Y);

			if (selectedNode != null)
			{
				//Can't dragdrop folders
				if (selectedNode.ImageKey == "Folder")
					return;

				tvMaterialLibrary.SelectedNode = selectedNode;
				tvMaterialLibrary.DoDragDrop(selectedNode, DragDropEffects.Copy);
			}
		}

		private void tvControllerLibrary_MouseDown(object sender,MouseEventArgs e)
		{
			TreeNode selectedNode = tvControllerLibrary.GetNodeAt(e.X,e.Y);

			if (selectedNode != null)
			{
				//Can't dragdrop folders
				if (selectedNode.ImageKey == "Folder")
					return;

				tvControllerLibrary.SelectedNode = selectedNode;
				tvControllerLibrary.DoDragDrop(selectedNode,DragDropEffects.Copy);
			}
		}

		private void tvAudioLibrary_MouseDown(object sender,MouseEventArgs e)
		{
			TreeNode selectedNode = tvAudioLibrary.GetNodeAt(e.X,e.Y);

			if (selectedNode != null)
			{
				//Can't dragdrop folders
				if (selectedNode.ImageKey == "Folder")
					return;

				tvAudioLibrary.SelectedNode = selectedNode;
				tvAudioLibrary.DoDragDrop(selectedNode,DragDropEffects.Copy);
			}
		}

		private void actorDirWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvActorLibrary)
			{
				ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text);
			}
		}

		private void actorDirWatcher_Deleted(object sender, FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvActorLibrary)
			{
				ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text);
			}
		}

		private void actorDirWatcher_Renamed(object sender, RenamedEventArgs e)
		{
			//Search for templates. 
			lock (tvActorLibrary)
			{
				ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text);
			}
		}

		private void materialDirWatcher_Created(object sender, FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvMaterialLibrary)
			{
				MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text);
			}
		}

		private void materialDirWatcher_Deleted(object sender, FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvMaterialLibrary)
			{
				MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text);
			}
		}

		private void materialDirWatcher_Renamed(object sender, RenamedEventArgs e)
		{
			//Search for templates. 
			lock (tvMaterialLibrary)
			{
				MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text);
			}
		}

		private void controllerDirWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvControllerLibrary)
			{
				ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text);
			}
		}

		private void controllerDirWatcher_Deleted(object sender,FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvControllerLibrary)
			{
				ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text);
			}
		}

		private void controllerDirWatcher_Renamed(object sender,RenamedEventArgs e)
		{
			//Search for templates. 
			lock (tvControllerLibrary)
			{
				ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text);
			}
		}
		
		private void audioDirWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvAudioLibrary)
			{
				AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text);
			}
		}

		private void audioDirWatcher_Deleted(object sender,FileSystemEventArgs e)
		{
			//Search for templates
			lock (tvAudioLibrary)
			{
				AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text);
			}
		}

		private void audioDirWatcher_Renamed(object sender,RenamedEventArgs e)
		{
			//Search for templates
			lock (tvAudioLibrary)
			{
				AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text);
			}
		}

		private void textureDirWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//Don't care about cache
			if (e.FullPath.Contains(Program.g_mainForm.CurrentWorkspace.TextureLibraryPath + "\\" +
									Program.g_mainForm.CurrentWorkspace.TextureLibraryCacheDirName))
				return;
			
			//Search for templates. 
			lock (tvTextureLibrary)
			{
				TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text);
			}
		}

		private void textureDirWatcher_Deleted(object sender,FileSystemEventArgs e)
		{
			//Don't care about cache
			if (e.FullPath.Contains(Program.g_mainForm.CurrentWorkspace.TextureLibraryPath + "\\" +
									Program.g_mainForm.CurrentWorkspace.TextureLibraryCacheDirName))
				return;

			//Search for templates. 
			lock (tvTextureLibrary)
			{
				TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text);
			}
		}

		private void textureDirWatcher_Renamed(object sender,RenamedEventArgs e)
		{
			//Don't care about cache
			if (e.FullPath.Contains(Program.g_mainForm.CurrentWorkspace.TextureLibraryPath + "\\" +
									Program.g_mainForm.CurrentWorkspace.TextureLibraryCacheDirName))
				return;
			
			//Search for templates. 
			lock (tvTextureLibrary)
			{
				TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text);
			}
		}

		private void scriptDirWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//Search for templates. 
			lock (tvScriptLibrary)
			{
				ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text);
			}
		}

		private void scriptDirWatcher_Deleted(object sender,FileSystemEventArgs e)
		{
			//Search for templates
			lock (tvScriptLibrary)
			{
				ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text);
			}
		}

		private void scriptDirWatcher_Renamed(object sender,RenamedEventArgs e)
		{
			//Search for templates
			lock (tvScriptLibrary)
			{
				ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text);
			}
		}
		
		private void tvMaterialLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			refreshMaterialPreview(tv.SelectedNode.Name);
			/*
			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			try
			{
				fs = File.Open(tv.SelectedNode.Name,FileMode.Open,FileAccess.Read);
			}
			catch (Exception ex)
			{
				Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}

			try
			{
				Material newMaterial = bf.Deserialize(fs) as Material;
				pgridMaterials.SelectedObject = newMaterial;
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error occurred while loading material template for preview.  See the message log for details.","Material Template Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
			fs.Close();
			 */
		}

		private void pgridMaterials_PropertyValueChanged(object s,PropertyValueChangedEventArgs e)
		{
			MessageBox.Show("Properties in this property grid are for read-only purposes.  Property value not changed.","Read-only Properties",MessageBoxButtons.OK,MessageBoxIcon.Information);
		}

		private void pgridControllers_PropertyValueChanged(object s,PropertyValueChangedEventArgs e)
		{
			MessageBox.Show("Properties in this property grid are for read-only purposes.  Property value not changed.","Read-only Properties",MessageBoxButtons.OK,MessageBoxIcon.Information);
		}
		
		private void tvControllerLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			refreshControllerPreview(tv.SelectedNode.Name);
			/*
			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			try
			{
				fs = File.Open(tv.SelectedNode.Name,FileMode.Open,FileAccess.Read);
			}
			catch (Exception ex)
			{
				Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}

			try
			{
				Controller newController = bf.Deserialize(fs) as Controller;
				pgridControllers.SelectedObject = newController;
			}
			catch (Exception exception)
			{
				MessageBox.Show("Error occurred while loading controller template for preview.  See the message log for details.","Controller Template Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
			fs.Close();
			 * */
		}

	
		private void tvAudioLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			if (tvAudioLibrary.SelectedNode != null)
			{
				if (tvAudioLibrary.SelectedNode.ImageKey == "Folder")
					return;

				axWindowsMediaPlayer.settings.autoStart = false;
				axWindowsMediaPlayer.URL = tvAudioLibrary.SelectedNode.Name;
			}
		}

		private void tvAudioLibrary_DoubleClick(object sender,EventArgs e)
		{
			if (tvAudioLibrary.SelectedNode != null)
			{
				if (tvAudioLibrary.SelectedNode.ImageKey == "Folder")
					return;

				axWindowsMediaPlayer.settings.autoStart = true;
				axWindowsMediaPlayer.URL = tvAudioLibrary.SelectedNode.Name;
			}
		}

		private void tvTextureLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			if (tvTextureLibrary.SelectedNode != null)
			{
				if (tvTextureLibrary.SelectedNode.ImageKey == "Folder")
				{
					clearTextureBrowser();
					return;
				}

				//Add the source image and the cached image if one exists
				Material.TextureNameList tnl = new Material.TextureNameList();
				tnl.names.Add(tvTextureLibrary.SelectedNode.Text);
				tnl.filenames.Add(tvTextureLibrary.SelectedNode.Name);
				tnl.description.Add("");

				string cachedTex = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(tvTextureLibrary.SelectedNode.Name);
				cachedTex = Path.ChangeExtension(cachedTex,".xif");
				if (File.Exists(cachedTex))
				{
					tnl.names.Add(Path.GetFileName(cachedTex));
					tnl.filenames.Add(cachedTex);
					tnl.description.Add("");
				}

				refreshTextureBrowser(tnl);
				//picTexturePreview.Image = new Bitmap(tvTextureLibrary.SelectedNode.Name);
				//axWindowsMediaPlayer.settings.autoStart = false;
				//axWindowsMediaPlayer.URL = tvTextureLibrary.SelectedNode.Name;
			}
		}

		private void tvTextureLibrary_DoubleClick(object sender,EventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			Process p = new Process();
			p.StartInfo.FileName = tv.SelectedNode.Name;
			p.StartInfo.CreateNoWindow = true;
			p.Start();
		}
		
		private void tvActorLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			refreshActorPreview(tv.SelectedNode.Name);

			/*TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			//Setup preview of actor by loading in the actor and taking
			//only the tree view information.  Don't even bother loading
			//the hash table or doing post fix up.
			FileStream fs = null;
			SceneExplorer actorSceneExplorer;

			try
			{
				fs = File.Open(tv.SelectedNode.Name,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				actorSceneExplorer = bf.Deserialize(fs) as SceneExplorer;
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not preview actor.  Check the message log for details.","Preview Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (fs != null)
					fs.Close();
				return;
			}

			if (fs != null)
				fs.Close();

			tvActorPreview.Nodes.Clear();
			foreach (TreeNode node in actorSceneExplorer.getHierarchyTreeView().Nodes)
				tvActorPreview.Nodes.Add((TreeNode)node.Clone());
			 */
		}


		public void clearTextureBrowser()
		{
			if (bitmapLoadWorker.IsBusy)
				bitmapLoadWorker.CancelAsync();

			texturePreview.Clear();
		}

		public void refreshTextureBrowser(Material.TextureNameList texNameList)
		{
			//First, check to see if we are processing a previous refresh.  If so,
			//cancel the job so we can start a new one
			if (bitmapLoadWorker.IsBusy)
			{
				bitmapLoadWorker.CancelAsync();
				m_queuedTexNameList = texNameList;
			}
			else
			{
				texturePreview.Clear();
				if (texNameList != null)
					bitmapLoadWorker.RunWorkerAsync(texNameList);
			}
		}

		public void refreshActorPreview(string actorFilename)
		{
			//First, check to see if we are processing a previous refresh.  If so,
			//cancel the job so we can start a new one
			if (actorPreviewWorker.IsBusy)
			{
				actorPreviewWorker.CancelAsync();
				m_queuedActorFilename = actorFilename;
			}
			else
			{
				tvActorPreview.Nodes.Clear();
				if (actorFilename != null)
					actorPreviewWorker.RunWorkerAsync(actorFilename);
			}
		}

		public void refreshMaterialPreview(string materialFilename)
		{
			//First, check to see if we are processing a previous refresh.  If so,
			//cancel the job so we can start a new one
			if (materialPreviewWorker.IsBusy)
			{
				materialPreviewWorker.CancelAsync();
				m_queuedMaterialFilename = materialFilename;
			}
			else
			{
				pgridMaterials.SelectedObject = null;
				if (materialFilename != null)
					materialPreviewWorker.RunWorkerAsync(materialFilename);
			}
		}

		public void refreshControllerPreview(string controllerFilename)
		{
			//First, check to see if we are processing a previous refresh.  If so,
			//cancel the job so we can start a new one
			if (controllerPreviewWorker.IsBusy)
			{
				controllerPreviewWorker.CancelAsync();
				m_queuedControllerFilename = controllerFilename;
			}
			else
			{
				pgridControllers.SelectedObject = null;
				if (controllerFilename != null)
					controllerPreviewWorker.RunWorkerAsync(controllerFilename);
			}
		}
		
		private void bitmapLoadWorker_DoWork(object sender,DoWorkEventArgs e)
		{
			BackgroundWorker bgWorker = (BackgroundWorker)sender;
			Material.TextureNameList texNameList = e.Argument as Material.TextureNameList;
			for (int i = 0; i < texNameList.names.Count; ++i)
			{
				//Pause for while to keep the UI responsive
				Thread.Sleep(33);
				
				//Check for cancel event
				if (bgWorker.CancellationPending)
				{
					e.Cancel = true;
					return;
				}

				if (texNameList.filenames[i] == null)
				{
					//Add a null bitmap and a blank image info structure.
					texNameList.bitmaps.Add(null);
                    texNameList.imageInfos.Add(new Microsoft.Xna.Framework.Graphics.TextureInformation());
				}
				else
				{
					//Try to load the bitmap in...if it fails for some reason,
					//then the preview will not be available.  Most likely a format
					//may not be supported if this happens.  E.g. tga.  Or,
					//the image does not exist.
					Bitmap bitmap = null;
					try
					{
                        Microsoft.Xna.Framework.Graphics.TextureInformation imageInfo;	
						bitmap = m_d3dLoader.Load((string)texNameList.filenames[i],out imageInfo);// new Bitmap((string)texNameList.filenames[i]);
						texNameList.bitmaps.Add(bitmap);
						texNameList.imageInfos.Add(imageInfo);
					}
					catch (Exception) { }
				}
			}
			e.Result = texNameList;
		}
		
		private void actorPreviewWorker_DoWork(object sender,DoWorkEventArgs e)
		{
			BackgroundWorker bgWorker = (BackgroundWorker)sender;
			string filename = e.Argument as string;

			//Setup preview of actor by loading in the actor and taking
			//only what information is necessary.  Don't even bother loading
			//the hash table or doing post fix up.
			FileStream fs = null;
			ViewportConfig viewportConfig = null;
			ArrayList nodes = null; 
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();
				viewportConfig = bf.Deserialize(fs) as ViewportConfig;
				nodes = bf.Deserialize(fs) as ArrayList;
			}
			catch (Exception exception)
			{
				Console.WriteLine(exception.ToString());
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not preview actor.  Check the message log for details.","Preview Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				if (fs != null)
					fs.Close();
				return;
			}

			if (fs != null)
				fs.Close();

			tvActorPreview.Invoke(m_clearActorPreviewInvoker,null);
			foreach (TreeNode node in nodes)
			{
				//Check for cancel event
				Thread.Sleep(11);
				if (bgWorker.CancellationPending)
				{
					e.Cancel = true;
					return;
				}
				tvActorPreview.Invoke(m_addActorNodeInvoker,(TreeNode)node);
			}
		}

		private void materialPreviewWorker_DoWork(object sender,DoWorkEventArgs e)
		{
			BackgroundWorker bgWorker = (BackgroundWorker)sender;
			string filename = e.Argument as string;

			//Check for cancel event
			if (bgWorker.CancellationPending)
			{
				e.Cancel = true;
				return;
			}

			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
			}
			catch (Exception ex)
			{
				Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}

			Material newMaterial = null;
			try
			{
				newMaterial = bf.Deserialize(fs) as Material;
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Error occurred while loading material template for preview.  Exception follows.",Color.Red);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
			if (fs != null)
				fs.Close();

			e.Result = newMaterial;		
		}

		private void controllerPreviewWorker_DoWork(object sender,DoWorkEventArgs e)
		{
			BackgroundWorker bgWorker = (BackgroundWorker)sender;
			string filename = e.Argument as string;

			//Check for cancel event
			if (bgWorker.CancellationPending)
			{
				e.Cancel = true;
				return;
			}

			FileStream fs = null;
			BinaryFormatter bf = new BinaryFormatter();
			try
			{
				fs = File.Open(filename,FileMode.Open,FileAccess.Read);
			}
			catch (Exception ex)
			{
				Program.g_mainForm.logMessage(ex.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}

			Controller newController = null;
			try
			{
				newController = bf.Deserialize(fs) as Controller;
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Error occurred while loading controller template for preview.  Exception follows.",Color.Red);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
			if (fs != null)
				fs.Close();

			e.Result = newController;
		}
		
		private void bitmapLoadWorker_RunWorkerCompleted(object sender,RunWorkerCompletedEventArgs e)
		{
			//If we were cancelled, then stop the thread.  Also, check to see if there is
			//a queued texture name list to process.  If so, process it.
			if (e.Cancelled)
			{
				if (m_queuedTexNameList != null)
				{
					lock (m_queuedTexNameList)
					{
						bitmapLoadWorker.RunWorkerAsync(m_queuedTexNameList);
						m_queuedTexNameList = null;
					}
				}
			}
			else
			{
				Material.TextureNameList texNameList = e.Result as Material.TextureNameList;
				for (int i = 0; i < texNameList.bitmaps.Count; ++i)
					texturePreview.AddTexture(new TextureHandle((string)texNameList.names[i],
														(string)texNameList.description[i],
													 (string)texNameList.filenames[i],
													(Bitmap)texNameList.bitmaps[i],
                                                    (Microsoft.Xna.Framework.Graphics.TextureInformation)texNameList.imageInfos[i],
													null));
				texturePreview.DisplayThumbnails();
			}
		}

		private void actorPreviewWorker_RunWorkerCompleted(object sender,RunWorkerCompletedEventArgs e)
		{
			//If we were cancelled, then stop the thread.  Also, check to see if there is
			//a queued texture name list to process.  If so, process it.
			if (e.Cancelled)
			{
				if (m_queuedActorFilename != null)
				{
					lock (m_queuedActorFilename)
					{
						actorPreviewWorker.RunWorkerAsync(m_queuedActorFilename);
						m_queuedActorFilename = null;
					}
				}
			}
			else
			{
				//tvActorPreview.Nodes.Clear();
				//ArrayList nodes = e.Result as ArrayList;
				//for (int i = 0;i < nodes.Count;++i)
				//	tvActorPreview.Nodes.Add((TreeNode)nodes[i]);
			}
		}

		private void materialPreviewWorker_RunWorkerCompleted(object sender,RunWorkerCompletedEventArgs e)
		{
			//If we were cancelled, then stop the thread.  Also, check to see if there is
			//a queued material to process.  If so, process it.
			if (e.Cancelled)
			{
				if (m_queuedMaterialFilename != null)
				{
					lock (m_queuedMaterialFilename)
					{
						materialPreviewWorker.RunWorkerAsync(m_queuedMaterialFilename);
						m_queuedMaterialFilename = null;
					}
				}
			}
			else
			{
				if (e.Result != null)
					pgridMaterials.SelectedObject = e.Result as Material;
			}
		}

		private void controllerPreviewWorker_RunWorkerCompleted(object sender,RunWorkerCompletedEventArgs e)
		{
			//If we were cancelled, then stop the thread.  Also, check to see if there is
			//a queued controller to process.  If so, process it.
			if (e.Cancelled)
			{
				if (m_queuedControllerFilename != null)
				{
					lock (m_queuedControllerFilename)
					{
						controllerPreviewWorker.RunWorkerAsync(m_queuedControllerFilename);
						m_queuedControllerFilename = null;
					}
				}
			}
			else
			{
				if (e.Result != null)
					pgridControllers.SelectedObject = e.Result as Controller;
			}
		}
		
		private void chkEnableAudioFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableAudioFilter.Checked)
			{
				txtAudioFilter.Enabled = true;
				lock (tvAudioLibrary)
				{
					AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text);
				}
			}
			else
			{
				txtAudioFilter.Enabled = false;
				lock (tvAudioLibrary)
				{
					AudioDirSearch(m_audioLibraryPath,"");
				}
			}
			
		}

		private void txtAudioFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvAudioLibrary)
			{
				AudioDirSearch(m_audioLibraryPath,txtAudioFilter.Text);
			}
		}

		private void chkEnableActorFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableActorFilter.Checked)
			{
				txtActorFilter.Enabled = true;
				lock (tvActorLibrary)
				{
					ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text);
				}
			}
			else
			{
				txtActorFilter.Enabled = false;
				lock (tvActorLibrary)
				{
					ActorDirSearch(m_actorLibraryPath,"");
				}
			}
		}

		private void chkEnableMaterialFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableMaterialFilter.Checked)
			{
				txtMaterialFilter.Enabled = true;
				lock (tvMaterialLibrary)
				{
					MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text);
				}
			}
			else
			{
				txtMaterialFilter.Enabled = false;
				lock (tvMaterialLibrary)
				{
					MaterialDirSearch(m_materialLibraryPath,"");
				}
			}
		}

		private void chkEnableControllerFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableControllerFilter.Checked)
			{
				txtControllerFilter.Enabled = true;
				lock (tvControllerLibrary)
				{
					ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text);
				}
			}
			else
			{
				txtControllerFilter.Enabled = false;
				lock (tvControllerLibrary)
				{
					ControllerDirSearch(m_controllerLibraryPath,"");
				}
			}
		}

		private void chkEnableTextureFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableTextureFilter.Checked)
			{
				txtTextureFilter.Enabled = true;
				lock (tvTextureLibrary)
				{
					TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text);
				}
			}
			else
			{
				txtTextureFilter.Enabled = false;
				lock (tvTextureLibrary)
				{
					TextureDirSearch(m_textureLibraryPath,"");
				}
			}
		}
		
		private void txtActorFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvActorLibrary)
			{
				ActorDirSearch(m_actorLibraryPath,txtActorFilter.Text);
			}
		}

		private void txtMaterialFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvMaterialLibrary)
			{
				MaterialDirSearch(m_materialLibraryPath,txtMaterialFilter.Text);
			}
		}

		private void txtControllerFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvControllerLibrary)
			{
				ControllerDirSearch(m_controllerLibraryPath,txtControllerFilter.Text);
			}
		}

		private void txtTextureFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvTextureLibrary)
			{
				TextureDirSearch(m_textureLibraryPath,txtTextureFilter.Text);
			}
		}

		private void chkEnableScriptFilter_CheckedChanged(object sender,EventArgs e)
		{
			if (chkEnableScriptFilter.Checked)
			{
				txtScriptFilter.Enabled = true;
				lock (tvScriptLibrary)
				{
					ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text);
				}
			}
			else
			{
				txtScriptFilter.Enabled = false;
				lock (tvScriptLibrary)
				{
					ScriptDirSearch(m_scriptLibraryPath,"");
				}
			}

		}

		private void txtScriptFilter_TextChanged(object sender,EventArgs e)
		{
			lock (tvScriptLibrary)
			{
				ScriptDirSearch(m_scriptLibraryPath,txtScriptFilter.Text);
			}
		}
		
		private void tsbExpandAllActorFolders_Click(object sender,EventArgs e)
		{
			tvActorLibrary.ExpandAll();
		}

		private void tsbCollaspeAllActorFolders_Click(object sender,EventArgs e)
		{
			tvActorLibrary.CollapseAll();
		}

		private void tsbExpandAllMaterialFolders_Click(object sender,EventArgs e)
		{
			tvMaterialLibrary.ExpandAll();
		}

		private void tsbCollapseAllMaterialFolders_Click(object sender,EventArgs e)
		{
			tvMaterialLibrary.CollapseAll();
		}

		private void tsbExpandAllControllerFolders_Click(object sender,EventArgs e)
		{
			tvControllerLibrary.ExpandAll();
		}

		private void tsbCollapseAllControllerFolders_Click(object sender,EventArgs e)
		{
			tvControllerLibrary.CollapseAll();
		}

		private void tsbExpandAllTextureFolders_Click(object sender,EventArgs e)
		{
			tvTextureLibrary.ExpandAll();
		}

		private void tsbCollapseAllTextureFolders_Click(object sender,EventArgs e)
		{
			tvTextureLibrary.CollapseAll();
		}

		private void tsbExpandAllAudioFolders_Click(object sender,EventArgs e)
		{
			tvAudioLibrary.ExpandAll();
		}

		private void tsbCollapseAllAudioFolders_Click(object sender,EventArgs e)
		{
			tvAudioLibrary.CollapseAll();
		}

		private void tvTextureLibrary_MouseDown(object sender,MouseEventArgs e)
		{
			TreeNode selectedNode = tvTextureLibrary.GetNodeAt(e.X,e.Y);

			if (selectedNode != null)
			{
				//Can't dragdrop folders
				if (selectedNode.ImageKey == "Folder")
					return;

				tvTextureLibrary.SelectedNode = selectedNode;
				tvTextureLibrary.DoDragDrop(selectedNode,DragDropEffects.Copy);
			}
		}

		private void tsbDeleteCompressedVersion_Click(object sender,EventArgs e)
		{
			if (tvTextureLibrary.SelectedNode == null)
				return;

			string cachedTex = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(tvTextureLibrary.SelectedNode.Name);
			cachedTex = Path.ChangeExtension(cachedTex,".xif");
			if (File.Exists(cachedTex))
			{
				File.Delete(cachedTex);
				tvTextureLibrary_AfterSelect(tvTextureLibrary,null);
			}
		}

		private void tsbDeleteAllCompressedVersions_Click(object sender,EventArgs e)
		{
			DialogResult dr = MessageBox.Show("All compressed versions of all textures in the library will be deleted.  This operation cannot be undone and will cause textures to be recompressed the next time they are needed.  Do you want to continue?","Delete Compressed Textures",MessageBoxButtons.YesNo,MessageBoxIcon.Warning,MessageBoxDefaultButton.Button2);
			if (dr == DialogResult.No)
				return;

			//Blow away the entiry cache
			try 
			{
				if (Directory.Exists(Program.g_mainForm.CurrentWorkspace.TextureLibraryCachePath))
					Utility.destroyDirectory(Program.g_mainForm.CurrentWorkspace.TextureLibraryCachePath);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Could not empty texture cache directory.  Try manually deleting the directory contents in " + Program.g_mainForm.CurrentWorkspace.TextureLibraryCachePath +".  See the message log for details","Delete Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.Message,Color.Red);
				return;
			}
/*
			foreach (TreeNode node in tvTextureLibrary.Nodes)
				DeleteCompressedTextureRecursive(node);*/

			tvTextureLibrary_AfterSelect(tvTextureLibrary,null);
		}

		private void tsbOpenExplorerLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_actorLibraryPath))
				System.Diagnostics.Process.Start(m_actorLibraryPath);
			else
				MessageBox.Show("The current path " + m_actorLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 
		}

		private void tsbMaterialLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_materialLibraryPath))
				System.Diagnostics.Process.Start(m_materialLibraryPath);
			else
				MessageBox.Show("The current path " + m_materialLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 
		}

		private void tsbAudioLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_audioLibraryPath))
				System.Diagnostics.Process.Start(m_audioLibraryPath);
			else
				MessageBox.Show("The current path " + m_audioLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 
		}

		private void tsbTextureLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_textureLibraryPath))
				System.Diagnostics.Process.Start(m_textureLibraryPath);
			else
				MessageBox.Show("The current path " + m_textureLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 
		}

		private void tsbControllerLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_controllerLibraryPath))
				System.Diagnostics.Process.Start(m_controllerLibraryPath);
			else
				MessageBox.Show("The current path " + m_controllerLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 
		}

		private void tsbOnceClickAdd_Click(object sender,EventArgs e)
		{
			Color color = Color.Yellow;
			if (btnOneClickAdd.Checked)
				color = Color.Red;

			//Run through all the documents in the document manager and change
			//the highlight color for all XVP Controls
			foreach (string docName in Program.g_mainForm.m_docMgr.Documents.Keys)
			{
				XVPMDocument doc = Program.g_mainForm.m_docMgr.Documents[docName] as XVPMDocument;
				doc.setActiveViewportColor(color);
			}

			m_oneClickAdd = btnOneClickAdd.Checked;
		}

		/*private void DeleteCompressedTextureRecursive(TreeNode node)
		{
			string cachedTex = Program.g_mainForm.CurrentWorkspace.buildCachedTexturePath(node.Name);
			cachedTex = Path.ChangeExtension(cachedTex,".xif");
			if (File.Exists(cachedTex))
				File.Delete(cachedTex);
			
			foreach (TreeNode childNode in node.Nodes)
				DeleteCompressedTextureRecursive(childNode);
		}*/

		public TreeNode getSelectedActorNode()
		{
			if (tvActorLibrary.SelectedNode != null)
			{
				if (tvActorLibrary.SelectedNode.ImageKey == "Actor")
					return tvActorLibrary.SelectedNode;
				else
					return null;
			}
			else
				MessageBox.Show("One-Click Actor Creation is enabled in the Actor Library but no actor is selected in the library.  Select an actor in the library.","No Actor Selected in the Library",MessageBoxButtons.OK,MessageBoxIcon.Information);

			return null;
		}

		private void btnExpandAllScriptFolders_Click(object sender,EventArgs e)
		{
			tvScriptLibrary.ExpandAll();
		}

		private void btnCollapseAllScriptFolders_Click(object sender,EventArgs e)
		{
			tvScriptLibrary.CollapseAll();
		}

		private void btnOpenScriptLocation_Click(object sender,EventArgs e)
		{
			if (Directory.Exists(m_scriptLibraryPath))
				System.Diagnostics.Process.Start(m_scriptLibraryPath);
			else
				MessageBox.Show("The current path " + m_scriptLibraryPath + " is invalid.  Please check the path settings for the workspace.","Invalid Path",MessageBoxButtons.OK,MessageBoxIcon.Information); 

		}

		private void tvScriptLibrary_AfterSelect(object sender,TreeViewEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			try
			{
				txtScriptPreview.LoadFile(tv.SelectedNode.Name);
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
			}
		}

		private void tvActorLibrary_MouseDoubleClick(object sender,MouseEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			Program.g_mainForm.openActorFromFile(tv.SelectedNode.Name);
		}

		private void tvScriptLibrary_MouseDoubleClick(object sender,MouseEventArgs e)
		{
			TreeView tv = (TreeView)sender;
			if (tv.SelectedNode == null)
				return;

			if (tv.SelectedNode.ImageKey == "Folder")
				return;

			Program.g_mainForm.openScriptFromFile(tv.SelectedNode.Name);
		}
	}

	
}
