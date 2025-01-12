using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace XVPDesigner
{
	[Serializable]
	public class ViewportConfig
	{
		//This constant should match VIEWPORT_COLOR in XVPManaged
		static Color VIEWPORT_COLOR = Color.FromArgb((int)(.38f * 255),(int)(.38f * 255),(int)(.38f * 255));

		private bool m_dirty = false;
		public bool Dirty { get { return m_dirty; } set { m_dirty = value; } }

		private string m_actorTemplate = "No Template";
		public string ActorTemplate { get { return m_actorTemplate; } set { if (m_actorTemplate != value) m_dirty = true; m_actorTemplate = value; } }

		private uint m_layout = 0;
		public uint Layout { get { return m_layout; } set { if (m_layout != value) m_dirty = true; m_layout = value; } }

		private Color m_backgroundColor = VIEWPORT_COLOR;
		public Color BackgroundColor { get { return m_backgroundColor; } set { if (m_backgroundColor != value) m_dirty = true; m_backgroundColor = value; } }

		private float m_unitSize = 1;
		public float UnitSize { get { return m_unitSize; } set { if (m_unitSize != value) m_dirty = true; m_unitSize = value; } }

		private uint m_gridSize = 20;
		public uint GridSize { get { return m_gridSize; } set { if (m_gridSize != value) m_dirty = true; m_gridSize = value; } }

		private uint m_majorLine = 10;
		public uint MajorLine { get { return m_majorLine; } set { if (m_majorLine != value) m_dirty = true; m_majorLine = value; } }
				
		private uint m_lightingMode = 0;
		public uint LightingMode { get { return m_lightingMode; } set { if (m_lightingMode != value) m_dirty = true; m_lightingMode = value; } }

		private int[] m_lightIntensity = new int[3] { 500,500,500 };
		public void setLightIntensity(uint lightIndex,int intensity)
		{
			if (m_lightIntensity[lightIndex] != intensity)
				m_dirty = true;
			m_lightIntensity[lightIndex] = intensity;

		}
		public int getLightIntensity(int lightIndex)
		{
			return m_lightIntensity[lightIndex];
		}

		private Color[] m_lightColor = new Color[3] { Color.Transparent,Color.Transparent,Color.Transparent };
		public void setLightColor(uint lightIndex,Color color)
		{
			if (m_lightColor[lightIndex] != color)
				m_dirty = true;
			m_lightColor[lightIndex] = color;

		}
		public Color getLightColor(int lightIndex)
		{
			return m_lightColor[lightIndex];
		}

		private string m_lightingScenario = "btnDirTW";
		public string LightingScenario { get { return m_lightingScenario; } set { if (m_lightingScenario != value) m_dirty = true; m_lightingScenario = value; } }

		private string m_lightAnimSpeed = "No Animation";
		public string LightAnimSpeed { get { return m_lightAnimSpeed; } set { if (m_lightAnimSpeed != value) m_dirty = true; m_lightAnimSpeed = value; } }

		private int m_lightingAngle = 0;
		public int LightingAngle { get { return m_lightingAngle; } set { if (m_lightingAngle != value) m_dirty = true; m_lightingAngle = value; } }

		private bool m_enableShadows = false;
		public bool EnableShadows { get { return m_enableShadows; } set { if (m_enableShadows != value) m_dirty = true; m_enableShadows = value; } }

		private bool[] m_showGrid = new bool[4] {true,true,true,true};
		public void setShowGrid(int viewportIndex,bool show)
		{
			if (m_showGrid[viewportIndex] != show)
				m_dirty = true;
			m_showGrid[viewportIndex] = show;

		}
		public bool getShowGrid(int viewportIndex)
		{
			return m_showGrid[viewportIndex];
		}

		private string[] m_cameraView = new string[4] { "Perspective","Top","Left","Front" };
		public void setCameraView(int viewportIndex,string cameraView)
		{
			if (m_cameraView[viewportIndex] != cameraView)
				m_dirty = true;
			m_cameraView[viewportIndex] = cameraView;
			
		}
		public string getCameraView(int viewportIndex)
		{
			return m_cameraView[viewportIndex];
		}
		
		/*private Color m_gridColor1;
		public Color GridColor1 { get { return m_gridColor1; } set { if (m_gridColor1 != value) m_dirty = true; m_gridColor1 = value; } }

		private Color m_gridColor2;
		public Color GridColor2 { get { return m_gridColor2; } set { if (m_gridColor2 != value) m_dirty = true; m_gridColor2 = value; } }

		private Color m_gridColor3;
		public Color GridColor3 { get { return m_gridColor3; } set { if (m_gridColor3 != value) m_dirty = true; m_gridColor3 = value; } }

		private Color m_gridColor4;
		public Color GridColor4 { get { return m_gridColor4; } set { if (m_gridColor4 != value) m_dirty = true; m_gridColor4 = value; } }
		*/
		private float[] m_cameraZoom = new float[4] {40,40,40,40};
		public void setCameraZoom(int viewportIndex,float distance)
		{
			if (m_cameraZoom[viewportIndex] != distance)
				m_dirty = true;	
			m_cameraZoom[viewportIndex] = distance;

		}
		public float getCameraZoom(int viewportIndex)
		{
			return m_cameraZoom[viewportIndex];
		}

		private Utility.Vector2D[] m_cameraRotation = new Utility.Vector2D[4] { new Utility.Vector2D(-.785f,.5f),new Utility.Vector2D(-.785f,.5f),new Utility.Vector2D(-.785f,.5f),new Utility.Vector2D(-.785f,.5f) };
		public void setCameraRotation(int viewportIndex,Utility.Vector2D rotation)
		{
			if (m_cameraRotation[viewportIndex] != rotation)
				m_dirty = true;
			m_cameraRotation[viewportIndex] = rotation;
		}
		public Utility.Vector2D getCameraRotation(int viewportIndex)
		{
			return m_cameraRotation[viewportIndex];
		}

		private Utility.Vector3D[] m_cameraPan = new Utility.Vector3D[4] { new Utility.Vector3D(),new Utility.Vector3D(),new Utility.Vector3D(),new Utility.Vector3D() };
		public void setCameraPan(int viewportIndex,Utility.Vector3D pan)
		{
			if (m_cameraPan[viewportIndex] != pan)
				m_dirty = true;
			m_cameraPan[viewportIndex] = pan;
		}
		public Utility.Vector3D getCameraPan(int viewportIndex)
		{
			return m_cameraPan[viewportIndex];
		}

		private string[] m_selectedCameraGUID = new string[4] { null,null,null,null };
		public void setSelectedCameraGUID(int viewportIndex,string GUID)
		{
			if (m_selectedCameraGUID[viewportIndex] != GUID)
				m_dirty = true;
			m_selectedCameraGUID[viewportIndex] = GUID;

		}
		public string getSelectedCameraGUID(int viewportIndex)
		{
			return m_selectedCameraGUID[viewportIndex];
		}
	}
}
