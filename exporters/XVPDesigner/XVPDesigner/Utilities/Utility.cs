using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using XVPDesigner.Objects;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Net;
using DevComponents.DotNetBar;
using System.Runtime.InteropServices;
	

namespace XVPDesigner
{

	public class Utility
	{
		static private string[] m_reservedLabels = new string[] {"High Dynamic Range"};

		static public void showNullExportError(SceneNode sceneNode,string propertyName)
		{
			PropertyInfo pi = sceneNode.GetType().GetProperty(propertyName);
			if (pi != null)
			{
				object[] attributes = pi.GetCustomAttributes(typeof(DisplayNameAttribute),false);
				if (attributes != null && attributes.Length > 0)
				{
					DisplayNameAttribute att = (DisplayNameAttribute)attributes[0];
					MessageBox.Show("The \"" + att.DisplayName + "\" property for the node \"" + sceneNode.Name + "\" has no value.  Give this property a value and then try re-exporting.","Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				}
			}
			else
			{
				MessageBox.Show("One of the properties for node \"" + sceneNode.Name + "\" has no value.  Make sure all properties have a value and then try re-exporting.","Export Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}

		static public void writeCString(BinaryWriter bw,string s)
		{
			//Check for null or empty strings
			if (s == null || s == "") 
			{
				bw.Write((uint)0);
			}
			else
			{
				byte[] byteBuffer = Encoding.ASCII.GetBytes(s);
				bw.Write(byteBuffer.Length + 1);
				bw.Write(byteBuffer);
				bw.Write('\0');
			}
		}

		static public void writeCString(StreamWriter sw,string s)
		{
			//Check for null or empty strings
			if (s == null || s == "")
			{
				sw.Write('\0');
			}
			else
			{
				sw.Write(s.Length + 1);
				sw.Write(s);
				sw.Write('\0');
			}
		}


		/*static public bool validateCString(string s)
		{
			foreach (char c in s)
			{
				//if ((!char.IsLetter(c) && !char.IsWhiteSpace(c) && !char.IsNumber(c) && !char.IsPunctuation(c)) ||
				//	(c == '\\' || c == '/' || c == '\"' || c == '\'' || c == '`'))
				if (c == '\\' || c == '\"')
				{
					return false;
				}
			}
			return true;
		}*/

		static public string getDescriptionString(string minValueString,string maxValueString,string description)
		{
			string minString,maxString;
			if (minValueString == "")
				minString = "None";
			else
				minString = minValueString;
			if (maxValueString == "")
				maxString = "None";
			else
				maxString = maxValueString;

			string sourceCode = description + "  [Min: " + minString + " | Max: " + maxString + "]";
			return sourceCode;
		}

		static public string genCompilableString(string s)
		{
			//Make sure to replace backslashes first!
			string compilableString = null;
			if (s != null)
			{
				compilableString = s.Replace("\\","\\\\");
				compilableString = compilableString.Replace("\"","\\\"");
			}
			return compilableString;
		}

        static public string checkIPAddress(string s)
        {
            if (s != "")
            {
                IPAddress tempAddr;
                if (IPAddress.TryParse(s, out tempAddr) == true)
                    return null;
            }

            return "IP Addresses must follow the xxx.xxx.xxx.xxx format where xxx is a number in the range of 0-255.";
        }

        static public string checkPort(string s)
        {
            if (s != "")
            {
                foreach (char c in s)
                {
                    if (!char.IsNumber(c))
                    {
                        return "Port number must be a positive numeric quantity.";
                    }
                }

                if (int.Parse(s) > 65535)
                {
                    return "Maximum value for port number is 65,535.";
                }

                return null;
            }
            else
            {
                return "Port number is blank.";
            }
        }

		static public string checkPath(string s)
		{
			if (s != "")
			{
				foreach (char c in s)
				{
					if ((!char.IsLetter(c) && !char.IsWhiteSpace(c) && !char.IsNumber(c) && !char.IsPunctuation(c)) ||
						(c == '\"' || c == '`'))
					{
						return "Invalid characters in path.";
					}
				}
				if (!Directory.Exists(s))
					return "Path does not exist.";
				return null;
			}

			return "Path is blank.";
		}
		
		static public void destroyDirectory(string directory)
		{
			DirectoryInfo di = new DirectoryInfo(directory);
			DirectoryInfo[] subDirs = di.GetDirectories();
			FileInfo[] files = di.GetFiles();

			foreach (DirectoryInfo dir in subDirs)
				destroyDirectory(dir.FullName);

			foreach (FileInfo file in files)
				File.Delete(file.FullName);

			di.Delete();
		}

		static public void addButtonSorted(ButtonItem createButton,ButtonItem newButton)
		{
			int index;
			for (index = 0; index < createButton.SubItems.Count; ++index)
			{
				if (createButton.SubItems[index] as ButtonItem == null)
					continue;
				if (createButton.SubItems[index].Text.CompareTo(newButton.Text) > 0)
					break;
			}

			createButton.SubItems.Insert(index,newButton);
		}

		static public bool validateLabel(string s)
		{
			//Check string against reserved strings
			for (int i = 0;i < m_reservedLabels.Length;++i)
				if (s == m_reservedLabels[i])
					return false;

			return true;
		}

		static public string FormatFileSize(long Bytes)
		{
			if (Bytes >= 1073741824)
			{
				Decimal size = Decimal.Divide(Bytes,1073741824);
				return String.Format("{0:##.##} GB",size);
			}
			else if (Bytes >= 1048576)
			{
				Decimal size = Decimal.Divide(Bytes,1048576);
				return String.Format("{0:##.##} MB",size);
			}
			else if (Bytes >= 1024)
			{
				Decimal size = Decimal.Divide(Bytes,1024);
				return String.Format("{0:##.##} KB",size);
			}
			else if (Bytes > 0 & Bytes < 1024)
			{
				Decimal size = Bytes;
				return String.Format("{0:##.##} Bytes",size);
			}
			else
			{
				return "0 Bytes";
			}
		}

		static public bool isCompoundShaderName(string shaderName)
		{
			if (shaderName.Contains("#"))
				return true;
			else
				return false;
		}

		static public string convertToShaderDisplayName(string original)
		{
			try
			{
				string[] parts;
				parts = original.Split('#');
				if (parts.Length > 1)
				{
					return parts[0] + " (" + parts[1] + ")";
				}
				
				throw new Exception("Input string is not the proper format");
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Problem parsing templated shader " + original + ". " + exception.ToString(),Color.Red);
				return null;
			}
		}

		static public void getCompoundShaderNameParts(string compoundName,out string shaderName,out string templateName)
		{
			try
			{
				string[] parts;
				parts = compoundName.Split('#');
				if (parts.Length > 1)
				{
					shaderName = parts[0];
					templateName = parts[1];
					return;
				}
				
				throw new Exception("Input string is not the proper format");
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Problem parsing templated shader " + compoundName + ". " + exception.ToString(),Color.Red);
				shaderName = null;
				templateName = null;
				return;
			}
		}

		static public string getShaderNameFromCompoundName(string compoundName)
		{
			try
			{
				string[] parts;
				parts = compoundName.Split('#');
				if (parts.Length > 1)
				{
					return parts[0];

				}

				throw new Exception("Input string is not the proper format");
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Problem parsing templated shader " + compoundName + ". " + exception.ToString(),Color.Red);
				return null;
			}
		}

		static public string getTemplateNameFromCompoundName(string compoundName)
		{
			try
			{
				string[] parts;
				parts = compoundName.Split('#');
				if (parts.Length > 1)
				{
					return parts[1];
				}

				throw new Exception("Input string is not the proper format");
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Problem parsing templated shader " + compoundName + ". " + exception.ToString(),Color.Red);
				return null;
			}
		}
		
		static public bool getShaderMetaData(string shaderFilename,out string desc,out string defines)
		{
			StreamReader sr = null;
			desc = defines = null;
			try
			{
				sr = File.OpenText(shaderFilename);
				defines = "";
				desc = null;
				string input = null;
				while ((input = sr.ReadLine()) != null)
				{
					if (input.Contains("//!!|"))
					{
						string[] parts = input.Split(new char[] { '|' },StringSplitOptions.RemoveEmptyEntries);
						if (parts.Length == 2)
						{
							desc = parts[1].Trim();
						}
					}
					else if (input.Contains("//##|"))
					{
						string[] featureParts = input.Split(new char[] { '|' },StringSplitOptions.RemoveEmptyEntries);

						//We assume the following this string was build correctly since it was generated by us
						//and not hand edited	
						defines += "-D" + featureParts[1].Trim() + "=" + featureParts[1].Trim() + " ";
					}
				}
				sr.Close();

				return true;
			}
			catch (Exception exception)
			{
				if (sr != null)
					sr.Close();
				MessageBox.Show("There was a problem encountered while trying to obtain metadata from the shader " + shaderFilename + ". See the message log for more information.","Shader Meta Data",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return false;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		[Serializable]
		public class Vector2D
		{
			public float x;
			public float y;

			public Vector2D()
			{
				x = 0.0f;
				y = 0.0f;
			}
			
			public Vector2D(float ix,float iy)
			{
				x = ix;
				y = iy;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		[Serializable]
		public class Vector3D
		{
			public float x;
			public float y;
			public float z;

			public Vector3D()
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
			}

			public Vector3D(float ix,float iy,float iz)
			{
				x = ix;
				y = iy;
				z = iz;
			}
		}
	}
}
