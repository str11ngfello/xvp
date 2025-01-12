using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner
{
	[Serializable]
	public class Workspace
	{
		private bool m_dirty = false;
        public bool Dirty { get { return m_dirty; } set { m_dirty = value; } }

        private string m_targetDevice = "PC";
		public string TargetDevice { get { return m_targetDevice; } set { if (m_targetDevice != value) m_dirty = true; m_targetDevice = value; } }

        private string m_targetIP = "127.0.0.1";
        public string TargetIP { get { return m_targetIP; } set { if (m_targetIP != value) m_dirty = true; m_targetIP = value; } }

        private string m_targetPort = "5000";
        public string TargetPort { get { return m_targetPort; } set { if (m_targetPort != value) m_dirty = true; m_targetPort = value; } }

		private string m_shaderPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl";
		public string ShaderPath { get { return m_shaderPath; } set { if (m_shaderPath != value) m_dirty = true; m_shaderPath = value; } }

		private string m_cddPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Custom Data";
		public string CDDPath { get { return m_cddPath; } set { if (m_cddPath != value) m_dirty = true; m_cddPath = value; } }

		private string m_discPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Disc";
		public string DiscPath { get { return m_discPath; } set { if (m_discPath != value) m_dirty = true; m_discPath = value; } }

		private string m_actorLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Actor Library";
		public string ActorLibraryPath { get { return m_actorLibraryPath; } set { if (m_actorLibraryPath != value) m_dirty = true; m_actorLibraryPath = value; } }

		private string m_actorTemplatePath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Actor Templates";
		public string ActorTemplatePath { get { return m_actorTemplatePath; } set { if (m_actorTemplatePath != value) m_dirty = true; m_actorTemplatePath = value; } }

		private string m_materialLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Material Library";
		public string MaterialLibraryPath { get { return m_materialLibraryPath; } set { if (m_materialLibraryPath != value) m_dirty = true; m_materialLibraryPath = value; } }

		private string m_controllerLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Controller Library";
		public string ControllerLibraryPath { get { return m_controllerLibraryPath; } set { if (m_controllerLibraryPath != value) m_dirty = true; m_controllerLibraryPath = value; } }

		private string m_textureLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Texture Library";
		public string TextureLibraryPath { get { return m_textureLibraryPath; } set { if (m_textureLibraryPath != value) m_dirty = true; m_textureLibraryPath = value; } }
		public string TextureLibraryCachePath { get { return m_textureLibraryPath + TextureLibraryCacheString; } }
		public string TextureLibraryCacheString { get { return "\\" + TextureLibraryCacheDirName + "\\" + TargetDevice + "\\"; } }
		public string TextureLibraryCacheDirName { get { return ".TextureCache"; } }
		public string buildCachedTexturePath(string srcTexPath)
		{
			string temp = null;

			if (!srcTexPath.Contains(TextureLibraryPath))
			{
				//Remove colons from drive letters except for the first drive letter of course
				temp = TextureLibraryCachePath + "\\" + srcTexPath.Replace(':','-');									
			}
			else
			{
				temp = srcTexPath.Insert(TextureLibraryPath.Length,TextureLibraryCacheString);
			}

			return temp;
		}


		private string m_audioLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Audio Library";
		public string AudioLibraryPath { get { return m_audioLibraryPath; } set { if (m_audioLibraryPath != value) m_dirty = true; m_audioLibraryPath = value; } }
		public string AudioLibraryCachePath { get { return m_audioLibraryPath + AudioLibraryCacheString; } }
		public string AudioLibraryCacheString { get { return "\\" + AudioLibraryCacheDirName + "\\" + TargetDevice; } }
		public string AudioLibraryCacheDirName { get { return ".AudioCache"; } }
		public string buildCachedAudioPath(string srcAudioPath)
		{
			string temp = null;

			if (!srcAudioPath.Contains(AudioLibraryPath))
			{
				//Remove colons from drive letters except for the first drive letter of course
				temp = AudioLibraryCachePath + "\\" + srcAudioPath.Replace(':','-');
			}
			else
			{
				temp = srcAudioPath.Insert(AudioLibraryPath.Length,AudioLibraryCacheString);
			}

			return temp;
		}

		private string m_scriptLibraryPath = Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Default Workspace\\Script Library";
		public string ScriptLibraryPath { get { return m_scriptLibraryPath; } set { if (m_scriptLibraryPath != value) m_dirty = true; m_scriptLibraryPath = value; } }
		public string ScriptLibraryCachePath { get { return m_scriptLibraryPath + ScriptLibraryCacheString; } }
		public string ScriptLibraryCacheString { get { return "\\" + ScriptLibraryCacheDirName + "\\" + TargetDevice + "\\"; } }
		public string ScriptLibraryCacheDirName { get { return ".ScriptCache"; } }
		public string buildCachedScriptPath(string srcTexPath)
		{
			string temp = null;

			if (!srcTexPath.Contains(ScriptLibraryPath))
			{
				//Remove colons from drive letters except for the first drive letter of course
				temp = ScriptLibraryCachePath + "\\" + srcTexPath.Replace(':','-');
			}
			else
			{
				temp = srcTexPath.Insert(ScriptLibraryPath.Length,ScriptLibraryCacheString);
			}

			return temp;
		}

		private bool m_showStartPage = true;
        public bool ShowStartPage { get { return m_showStartPage; } set { if (m_showStartPage != value) m_dirty = true; m_showStartPage = value; } }

        private bool m_playEventSounds = true;
        public bool PlayEventSounds { get { return m_playEventSounds; } set { if (m_playEventSounds != value) m_dirty = true; m_playEventSounds = value; } }
		
		private int m_skinColor = 0;
		public int SkinColor { get { return m_skinColor; } set { if (m_skinColor != value) m_dirty = true; m_skinColor = value; } }

		private bool m_showToolTips = true;
		public bool ShowToolTips { get { return m_showToolTips; } set { if (m_showToolTips != value) m_dirty = true; m_showToolTips = value; } }

		public int MaxSamplers
		{
			get
			{
				if (m_targetDevice == "PC")
					return 16;
				else
					throw new Exception("MaxSamplers not defined for target device");
			}
		}
	}
}
