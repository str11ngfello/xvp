using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.CodeDom.Compiler;
using System.CodeDom;
using Microsoft.CSharp;
using System.Reflection;
using DevComponents.DotNetBar;
using XVPDesigner.Objects;
using System.Collections;
using XVPManaged;

namespace XVPDesigner.Managers
{
	public sealed class ShaderDescriptor : Attribute { }
	public sealed class CDDDescriptor : Attribute { }
	public sealed class IconPath : Attribute 
	{
		public string m_path;
        public IconPath(string path)
        {
			m_path = path;
        }
	}
	
	public class ShaderMgr
	{
		public struct ShaderParam
		{	
			public string name;
			public string encoding;
			public string type;
			public string description;
			public string defaultValue;
			public string includeDefine;
			public string[] defaultVecValue;
			public string[] rangeValues;
			public string[] minVecRangeValues;
			public string[] maxVecRangeValues;
			public string compilableParamName;
			public string privateParamVarName; 
		};
		
		//This enum must match the run time side enum exactly
		public enum ShaderParamType
		{
			SPT_Tex2D,
			SPT_Tex3D,
			SPT_TexCube,
			SPT_Bool,
			SPT_Enum,
			SPT_Int,
			SPT_Uint,
			SPT_Float,
			SPT_Vec2,
			SPT_Vec3,
			SPT_Vec4,
			SPT_Color
		};
		
		Hashtable m_loadedShaders = new Hashtable(500);
		private string m_shaderPath;

		public bool init(string shaderPath)
		{
			m_shaderPath = shaderPath;
			return updateShaders();
		}

		public bool updateShaders()
		{
			Program.g_mainForm.logMessage("Searching for shaders...",Color.Black);

			//Create compiler and add the common assemblies
			CodeDomProvider codeProvider = CodeDomProvider.CreateProvider("C#");
			CompilerParameters cp = new CompilerParameters();
			cp.ReferencedAssemblies.Add("XVPDesigner.exe");
			cp.ReferencedAssemblies.Add("System.dll");
			cp.ReferencedAssemblies.Add("System.Drawing.dll");
			cp.ReferencedAssemblies.Add("System.Windows.Forms.dll");
			cp.ReferencedAssemblies.Add("System.Design.dll");
			cp.ReferencedAssemblies.Add("DevComponents.DotNetBar2.dll");

			//Keep the shader plugin directory clean.  If a plugin is found that does not have a matching shader
			//then delete it.  Otherwise, we could run into problems with loading actors created with
			//older shaders.  Do this first!!
			try
			{
				foreach (string filename in Directory.GetFiles(Application.StartupPath))
				{
					//Make sure it's at least ending with ".fx.dll" 
					if (!filename.EndsWith("fx.dll"))
						continue;
					if (!File.Exists(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + Path.GetFileNameWithoutExtension(filename)))
					{
						Program.g_mainForm.logMessage("Deleting dangling plugin " + Path.GetFileName(filename),Color.Black);
						File.Delete(filename);
					}
				}
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage("Could not delete fx.dll's for which no shaders were found.  This could result in problems loading files that were created with dangling plugins. " + exception.ToString(),Color.Red);
			}
				

			//For each file in the directory, if it's newer than the object file, we need
			//to compile it.
			foreach (string filename in Directory.GetFiles(m_shaderPath))
			{
				//Check filename formatting.  
				if (!filename.EndsWith(".fx"))
					continue;

				string[] tokens = filename.Split('\\');
				string shaderName = tokens[tokens.Length - 1];
				string objFilename = Application.StartupPath + "\\" + Path.GetFileName(filename) + ".dll";


				//If the object file for this source file exists, let's check the time stamp to
				//determine if we need to re-compile it.
				if (!File.Exists(objFilename) ||
					(File.GetLastWriteTime(filename) > File.GetLastWriteTime(objFilename)))
				{
                    Program.g_mainForm.logMessage("Found shader \"" + shaderName + "\", compiling...", Color.Black);
                    Program.g_mainForm.setSplashStatus("Compiling Shader: \"" + shaderName + "\"");

					//Generate new C# source for the updated shader
					string sourceCode = parseShader(shaderName,filename);
					if (sourceCode == null)
					{
						Program.g_mainForm.logMessage("Failed to generate source code for shader \"" + shaderName + "\".",Color.Red);
						continue;
					}

					cp.OutputAssembly = objFilename;
					CompilerResults cr = codeProvider.CompileAssemblyFromSource(cp,sourceCode);
					
					if (cr.Errors.HasErrors)
					{
						for (int i = 0;i < cr.Errors.Count;++i)
							Program.g_mainForm.logMessage(cr.Errors[i].ToString(),Color.Red);
						Program.g_mainForm.logMessage("Compilation failed, dumping source code string - ",Color.Red);
						Program.g_mainForm.logMessage(sourceCode,Color.Red);
					}
					else
					{
						Program.g_mainForm.logMessage("Compilation succeeded.",Color.Green);
					}
				}
			}

			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));

			//Now, for each compiled shader assembly, attemp to load it.
			foreach (string filename in Directory.GetFiles(Application.StartupPath))
			{
				//Make sure it's at least ending with ".fx.dll" 
				if (!filename.EndsWith("fx.dll"))
					continue;

				string[] tokens = filename.Split('\\');
				string shaderName = tokens[tokens.Length - 1];
				
				//Make sure there is a matching shader or skip loading this dll and warn
				if (!File.Exists(Program.g_mainForm.XVPPath + "xvpsdk\\system\\shaders\\hlsl\\" + Path.GetFileNameWithoutExtension(shaderName)))
				{
					Program.g_mainForm.logMessage("Could not find shader for assembly \"" + shaderName + "\", skipping...",Color.Blue);
					Program.g_mainForm.setSplashStatus("Skipping Shader: \"" + shaderName + "\"");
					continue;
				}

                Program.g_mainForm.logMessage("Found shader assembly \"" + shaderName + "\", loading...", Color.Black);
                Program.g_mainForm.setSplashStatus("Loading Shader: \"" + shaderName + "\"");

				Assembly shaderAssembly = Assembly.LoadFile(filename);
				if (shaderAssembly == null)
				{
					Program.g_mainForm.logMessage("Could not load assembly for " + shaderName,Color.Red);
					continue;
				}

				Type shaderDescriptorType = null;
				foreach (Type type in shaderAssembly.GetTypes())
				{
					if (type.IsAbstract) continue;
					if (type.IsDefined(typeof(ShaderDescriptor),true))
					{
						//We've found the shader descriptor type, so we'll stop here.  
						shaderDescriptorType = type;

						break;
					}
				}

				if (shaderDescriptorType == null)
				{
					Program.g_mainForm.logMessage("Could not find shader descriptor attribute for " + shaderName,Color.Red);
					continue;
				}

				//Add the shader without the dll extension
				m_loadedShaders.Add(shaderName.Remove(shaderName.Length-4),shaderAssembly);

				//Add the shader info to the plugin dialog
				Program.g_mainForm.PluginDialog.addPluginInfo(shaderAssembly);
			}

			Program.g_mainForm.logMessage("Search complete.",Color.Black);

			return true;
		}

		private string parseShader(string shaderName,string filename)
		{
			string description,defines;
			if (!Utility.getShaderMetaData(filename,out description,out defines))
			{
				Program.g_mainForm.logMessage("Could not get shader meta data for \"" + filename + "\".",Color.Red);
				return null;
			}

			FileStream fs = File.Open(filename, FileMode.Open, FileAccess.Read);
			TextReader tr = new StreamReader(fs);
			ArrayList shaderParams = new ArrayList();

			string line;
			//while there are still lines of text to parse in the shader...
			while ((line = tr.ReadLine()) != null)
			{
				//Create a new shader param structure 
				ShaderParam p = new ShaderParam();

				//Is this a special mark-up line?
				line.Trim();
				string markupSignal = "//$$|";
				if (line.Contains(markupSignal))
				{
					//We've found a line of markup, so process its tokens, if there aren't
					//any tokens then ignore this incomplete line
					line = line.Remove(0,markupSignal.Length);
					string[] lineTokens = line.Split(new char[] {'|'},StringSplitOptions.RemoveEmptyEntries);
					if (lineTokens.Length <= 1)
						continue;

					//Trim whitespace on all the tokens 
					for (int i = 0;i < lineTokens.Length;++i)
						lineTokens[i] = lineTokens[i].Trim();

					//Token 1 should contain the parameter type.  Once we know the type,
					//we know how to process the rest of the line
					if (lineTokens[0].ToLower() == "2dtexture")
					{
						//2DTexture must have either 3 or 4 total tokens. The
						//description is optional.
						if (lineTokens.Length != 3 && lineTokens.Length != 4)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type 2DTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "2DTexture";

						//Encoding should follow next
						p.encoding = null;
						lineTokens[1] = lineTokens[1].Trim();
						if (lineTokens[1] == "NONE") p.encoding = "NONE";
						if (lineTokens[1] == "MIPMAP") p.encoding = "MIPMAP";
						if (lineTokens[1] == "RGBA") p.encoding = "RGBA";
						if (lineTokens[1] == "RGBa") p.encoding = "RGBa";
						if (lineTokens[1] == "XYZ") p.encoding = "XYZ";
						if (p.encoding == null)
						{
							Program.g_mainForm.logMessage("Unrecognized texture encoding \"" + lineTokens[1] + "\" for 2DTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Get map name next
						p.name = lineTokens[2].Trim();

						//Finally, if there is a description, store it
						if (lineTokens.Length > 3)
							p.description = lineTokens[3].Trim();
					}
					else if (lineTokens[0].ToLower() == "3dtexture")
					{
						//3DTexture must have either 3 or 4 total tokens. The
						//description is optional.
						if (lineTokens.Length != 3 && lineTokens.Length != 4)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type 3DTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "3DTexture";

						//Encoding should follow next
						p.encoding = null;
						lineTokens[1] = lineTokens[1].Trim();
						if (lineTokens[1] == "NONE") p.encoding = "NONE";
						if (lineTokens[1] == "MIPMAP") p.encoding = "MIPMAP";
						if (lineTokens[1] == "RGBA") p.encoding = "RGBA";
						if (lineTokens[1] == "RGBa") p.encoding = "RGBa";
						if (lineTokens[1] == "XYZ") p.encoding = "XYZ";
						if (p.encoding == null)
						{
							Program.g_mainForm.logMessage("Unrecognized texture encoding \"" + lineTokens[1] + "\" for 3DTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Get map name next
						p.name = lineTokens[2].Trim();

						//Finally, if there is a description, store it
						if (lineTokens.Length > 3)
							p.description = lineTokens[3].Trim();
					}
					else if (lineTokens[0].ToLower() == "cubetexture")
					{
						//CubeTexture must have either 3 or 4 total tokens. The
						//description is optional.
						if (lineTokens.Length != 3 && lineTokens.Length != 4)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type CubeTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "CubeTexture";

						//Encoding should follow next
						p.encoding = null;
						lineTokens[1] = lineTokens[1].Trim();
						if (lineTokens[1] == "NONE") p.encoding = "NONE";
						if (lineTokens[1] == "MIPMAP") p.encoding = "MIPMAP";
						if (lineTokens[1] == "RGBA") p.encoding = "RGBA";
						if (lineTokens[1] == "RGBa") p.encoding = "RGBa";
						if (lineTokens[1] == "XYZ") p.encoding = "XYZ";
						if (p.encoding == null)
						{
							Program.g_mainForm.logMessage("Unrecognized texture encoding \"" + lineTokens[1] + "\" for CubeTexture in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Get map name next
						p.name = lineTokens[2].Trim();

						//Finally, if there is a description, store it
						if (lineTokens.Length > 3)
							p.description = lineTokens[3].Trim();
					}
					else if (lineTokens[0].ToLower() == "bool")
					{
						//bool must have either 3 or 4 total tokens. The
						//description is optional.
						if (lineTokens.Length != 3 && lineTokens.Length != 4)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type bool in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "bool";

						//Get the bool's name next
						p.name = lineTokens[1].Trim();

						//Next comes the default value for the bool, true or false in parenthesis
						if (lineTokens[2].ToLower().Contains("true"))
							p.defaultValue = "true";
						else if (lineTokens[2].ToLower().Contains("false"))
							p.defaultValue = "false";
						else
						{
							Program.g_mainForm.logMessage("Invalid default value \"" + lineTokens[2] + "\" for bool \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes an optional description
						if (lineTokens.Length > 3)
							p.description = lineTokens[3].Trim();
					}
					else if (lineTokens[0].ToLower() == "enum")
					{
						//enum must have either 4 or 5 total tokens. The
						//description is optional.
						if (lineTokens.Length != 4 && lineTokens.Length != 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type enum in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "enum";

						//Get the enum's name next
						p.name = lineTokens[1].Trim();

						//Next comes the values of the enumeration
						string enumString = lineTokens[2].Trim();
						enumString = enumString.TrimStart('[');
						enumString = enumString.TrimEnd(']');
						p.rangeValues = enumString.Split(new char[] {'#'},StringSplitOptions.RemoveEmptyEntries);
						if (p.rangeValues.Length != 0)
							for (int i = 0; i < p.rangeValues.Length; ++i)
								p.rangeValues[i] = p.rangeValues[i].Trim();
						if (p.rangeValues == null)
						{
							Program.g_mainForm.logMessage("Invalid enum values \"" + enumString + "\" for enum \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes the default value for the enumeration.  Make sure the default value
						//exists in the enumeration values
						p.defaultValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						bool defaultValueOK = false;
						foreach (string s in p.rangeValues)
						{
							if (p.defaultValue == s)
							{
								defaultValueOK = true;
								break;
							}
						}
						if (!defaultValueOK)
						{
							Program.g_mainForm.logMessage("Default value \"" + p.defaultValue + "\" for enum \"" + p.name + "\" does not exist in specified enum values for enum in \"" + filename + "\".",Color.Red);
							return null;
						}
											
						//Next comes an optional description
						if (lineTokens.Length > 4)
							p.description = lineTokens[4].Trim();
					}
					else if (lineTokens[0].ToLower() == "int")
					{
						//int must have a minimum of 5 total tokens. 
						if (lineTokens.Length < 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type int in \"" + filename + "\". Expected at least 5 tokens.",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "int";

						//Get the int's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the int
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');
						p.rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
						if (p.rangeValues.Length != 0)
							for (int i = 0; i < p.rangeValues.Length; ++i)
								p.rangeValues[i] = p.rangeValues[i].Trim();

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							//Should be exactly 2 range values
							if (p.rangeValues == null)
							{
								Program.g_mainForm.logMessage("Invalid range values \"" + rangeString + "\" for int \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
							if (p.rangeValues.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for int \"" + p.name + "\" in \"" + filename + "\".  Int range must consist of 2 values - a min and a max.",Color.Red);
								return null;
							}
							try
							{
								if (int.Parse(p.rangeValues[0]) > int.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Minimum range is greater than maximum range for int \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range for int \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
						}
						else
						{
							p.rangeValues = null;
						}

						//Next comes the default value for the int.  Make sure the default value
						//exists in the specified range for the int
						p.defaultValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							int value = int.Parse(p.defaultValue);

							if (p.rangeValues != null)
							{
								if (value < int.Parse(p.rangeValues[0]) || value > int.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Default value \"" + p.defaultValue + "\" does not exist in the specified range \"" + rangeString + "\" for int \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + p.defaultValue + "\" for int \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes the description
						p.description = lineTokens[4].Trim();

						if (lineTokens.Length >= 6)
							p.includeDefine = lineTokens[5].Trim();
						else
							p.includeDefine = null;
					}
					else if (lineTokens[0].ToLower() == "uint")
					{
						//uint must have at least 5 total tokens. 
						if (lineTokens.Length < 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type uint in \"" + filename + "\".  Expected at least 5 tokens",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "uint";

						//Get the uint's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the uint
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');
						p.rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
						if (p.rangeValues.Length != 0)
							for (int i = 0; i < p.rangeValues.Length; ++i)
								p.rangeValues[i] = p.rangeValues[i].Trim();

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							//Should be exactly 2 range values
							if (p.rangeValues == null)
							{
								Program.g_mainForm.logMessage("Invalid range values \"" + rangeString + "\" for uint \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
							if (p.rangeValues.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for uint \"" + p.name + "\" in \"" + filename + "\".  Uint range must consist of 2 values - a min and a max.",Color.Red);
								return null;
							}
							try
							{
								if (uint.Parse(p.rangeValues[0]) > uint.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Minimum range is greater than maximum range for uint \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range for uint \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
						}
						else
						{
							p.rangeValues = null;
						}

						//Next comes the default value for the uint.  Make sure the default value
						//exists in the specified range for the uint
						p.defaultValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							uint value = uint.Parse(p.defaultValue);

							if (p.rangeValues != null)
							{
								if (value < uint.Parse(p.rangeValues[0]) || value > uint.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Default value \"" + p.defaultValue + "\" does not exist in the specified range \"" + rangeString + "\" for uint \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + p.defaultValue + "\" for uint \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes the description
						p.description = lineTokens[4].Trim();

						if (lineTokens.Length >= 6)
							p.includeDefine = lineTokens[5].Trim();
						else
							p.includeDefine = null;
					}
					else if (lineTokens[0].ToLower() == "float")
					{
						//float must have at least 5 total tokens. 
						if (lineTokens.Length < 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type float in \"" + filename + "\". Minimum number of tokens not found.",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "float";

						//Get the float's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the float
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');
						p.rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
						if (p.rangeValues.Length != 0)
							for (int i = 0; i < p.rangeValues.Length; ++i)
								p.rangeValues[i] = p.rangeValues[i].Trim();

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							//Should be exactly 3 range values
							if (p.rangeValues == null)
							{
								Program.g_mainForm.logMessage("Invalid range values \"" + rangeString + "\" for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
							if (p.rangeValues.Length != 3)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for float \"" + p.name + "\" in \"" + filename + "\".  Float range must consist of 2 values - a min, a max and a step size.",Color.Red);
								return null;
							}
							try
							{
								if (float.Parse(p.rangeValues[0]) > float.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Minimum range is greater than maximum range for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(p.rangeValues[2]) < 0.0f)
								{
									Program.g_mainForm.logMessage("Step size cannot be negative for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
								return null;
							}
						}
						else
						{
							p.rangeValues = null;
						}

						//Next comes the default value for the float.  Make sure the default value
						//exists in the specified range for the float
						p.defaultValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							float value = float.Parse(p.defaultValue);

							if (p.rangeValues != null)
							{
								if (value < float.Parse(p.rangeValues[0]) || value > float.Parse(p.rangeValues[1]))
								{
									Program.g_mainForm.logMessage("Default value \"" + p.defaultValue + "\" does not exist in the specified range \"" + rangeString + "\" for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + p.defaultValue + "\" for float \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes the description
						p.description = lineTokens[4].Trim();

						if (lineTokens.Length >= 6)
							p.includeDefine = lineTokens[5].Trim();
						else
							p.includeDefine = null;
					}
					else if (lineTokens[0].ToLower() == "vector2")
					{
						//vector2 must have either 4 or 5 total tokens. The
						//description is optional.
						if (lineTokens.Length != 4 && lineTokens.Length != 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type vector2 in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "vector2";

						//Get the vector2's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the vector2
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							string[] rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
							if (rangeValues.Length != 0)
								for (int i = 0; i < rangeValues.Length; ++i)
									rangeValues[i] = rangeValues[i].Trim();

							//Range must specify min and max for x and y components
							if (rangeValues.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector2 \"" + p.name + "\" in \"" + filename + "\".  Vector2 range must consist of 4 values - a min and a max for x and y components.  For example, [x1 y1 : x2 y2], where x1 and y1 specify the minimum for each component and x2 and y2 specify the maximum for each component",Color.Red);
								return null;
							}

							string[] minString = rangeValues[0].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							string[] maxString = rangeValues[1].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);

							//Check for 2 components each
							if (minString.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of min range values for vector2 \"" + p.name + "\" in \"" + filename + "\".  Vector2 range must consist of 4 values - a min and a max for x and y components.  For example, [x1 y1 : x2 y2], where x1 and y1 specify the minimum for each component and x2 and y2 specify the maximum for each component",Color.Red);
								return null;
							}
							//Check for 2 components each
							if (maxString.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of max range values for vector2 \"" + p.name + "\" in \"" + filename + "\".  Vector2 range must consist of 4 values - a min and a max for x and y components.  For example, [x1 y1 : x2 y2], where x1 and y1 specify the minimum for each component and x2 and y2 specify the maximum for each component",Color.Red);
								return null;
							}

							try
							{
								if (float.Parse(minString[0]) > float.Parse(maxString[0]))
								{
									Program.g_mainForm.logMessage("Minimum range x is greater than maximum range x for vector2 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[1]) > float.Parse(maxString[1]))
								{
									Program.g_mainForm.logMessage("Minimum range y is greater than maximum range y for vector2 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								p.minVecRangeValues = minString;
								p.maxVecRangeValues = maxString;
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range values for vector2 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}
						}
						else
						{
							p.minVecRangeValues = null;
							p.maxVecRangeValues = null;
						}

						//Next comes the default value for the vec2.  Make sure the default value
						//exists in the specified range for the vec2 and is the right number of components
						string defVecValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							string[] defComponents = defVecValue.Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							if (defComponents.Length != 2)
							{
								Program.g_mainForm.logMessage("Could not parse default value for vector2 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}

							if (p.minVecRangeValues != null)
							{
								if ((float.Parse(defComponents[0]) < float.Parse(p.minVecRangeValues[0]) || float.Parse(defComponents[0]) > float.Parse(p.maxVecRangeValues[0])) ||
									(float.Parse(defComponents[1]) < float.Parse(p.minVecRangeValues[1]) || float.Parse(defComponents[1]) > float.Parse(p.maxVecRangeValues[1])))
								{
									Program.g_mainForm.logMessage("Default value \"" + defVecValue + "\" does not exist in the specified range for vector2 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							p.defaultVecValue = defComponents;
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + defVecValue + "\" for vector2 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes an optional description
						if (lineTokens.Length > 4)
							p.description = lineTokens[4].Trim();
					}
					else if (lineTokens[0].ToLower() == "vector3")
					{
						//vector3 must have either 4 or 5 total tokens. The
						//description is optional.
						if (lineTokens.Length != 4 && lineTokens.Length != 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type vector3 in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "vector3";

						//Get the vector3's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the vector3
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							string[] rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
							if (rangeValues.Length != 0)
								for (int i = 0; i < rangeValues.Length; ++i)
									rangeValues[i] = rangeValues[i].Trim();

							//Range must specify min and max for x, y and z components
							if (rangeValues.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector3 \"" + p.name + "\" in \"" + filename + "\".  vector3 range must consist of 6 values - a min and a max for x, y and z components.  For example, [x1 y1 z1 : x2 y2 z2], where x1,y1 and z1 specify the minimum for each component and x2,y2 and z2 specify the maximum for each component",Color.Red);
								return null;
							}

							string[] minString = rangeValues[0].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							string[] maxString = rangeValues[1].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);

							//Check for 3 components each
							if (minString.Length != 3)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector3 \"" + p.name + "\" in \"" + filename + "\".  vector3 range must consist of 6 values - a min and a max for x, y and z components.  For example, [x1 y1 z1 : x2 y2 z2], where x1,y1 and z1 specify the minimum for each component and x2,y2 and z2 specify the maximum for each component",Color.Red);
								return null;
							}
							//Check for 3 components each
							if (maxString.Length != 3)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector3 \"" + p.name + "\" in \"" + filename + "\".  vector3 range must consist of 6 values - a min and a max for x, y and z components.  For example, [x1 y1 z1 : x2 y2 z2], where x1,y1 and z1 specify the minimum for each component and x2,y2 and z2 specify the maximum for each component",Color.Red);
								return null;
							}

							try
							{
								if (float.Parse(minString[0]) > float.Parse(maxString[0]))
								{
									Program.g_mainForm.logMessage("Minimum range x is greater than maximum range x for vector3 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[1]) > float.Parse(maxString[1]))
								{
									Program.g_mainForm.logMessage("Minimum range y is greater than maximum range y for vector3 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[2]) > float.Parse(maxString[2]))
								{
									Program.g_mainForm.logMessage("Minimum range z is greater than maximum range z for vector3 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								p.minVecRangeValues = minString;
								p.maxVecRangeValues = maxString;
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range values for vector3 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}
						}
						else
						{
							p.minVecRangeValues = null;
							p.maxVecRangeValues = null;
						}

						//Next comes the default value for the vector3.  Make sure the default value
						//exists in the specified range for the vector3 and is the right number of components
						string defVecValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							string[] defComponents = defVecValue.Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							if (defComponents.Length != 3)
							{
								Program.g_mainForm.logMessage("Could not parse default value for vector3 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}

							if (p.minVecRangeValues != null)
							{
								if ((float.Parse(defComponents[0]) < float.Parse(p.minVecRangeValues[0]) || float.Parse(defComponents[0]) > float.Parse(p.maxVecRangeValues[0])) ||
									(float.Parse(defComponents[1]) < float.Parse(p.minVecRangeValues[1]) || float.Parse(defComponents[1]) > float.Parse(p.maxVecRangeValues[1])) ||
									(float.Parse(defComponents[2]) < float.Parse(p.minVecRangeValues[2]) || float.Parse(defComponents[2]) > float.Parse(p.maxVecRangeValues[2])))
								{
									Program.g_mainForm.logMessage("Default value \"" + defVecValue + "\" does not exist in the specified range for vector3 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							p.defaultVecValue = defComponents;
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + defVecValue + "\" for vector3 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes an optional description
						if (lineTokens.Length > 4)
							p.description = lineTokens[4].Trim();
					}
					else if (lineTokens[0].ToLower() == "vector4")
					{
						//vector4 must have either 4 or 5 total tokens. The
						//description is optional.
						if (lineTokens.Length != 4 && lineTokens.Length != 5)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type vector4 in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "vector4";

						//Get the vector4's name next
						p.name = lineTokens[1].Trim();

						//Next comes the range of the vector4
						string rangeString = lineTokens[2].Trim();
						rangeString = rangeString.TrimStart('[');
						rangeString = rangeString.TrimEnd(']');

						//If range was specified...
						if (rangeString.Trim() != "")
						{
							string[] rangeValues = rangeString.Split(new char[] { '#' },StringSplitOptions.RemoveEmptyEntries);
							if (rangeValues.Length != 0) 
								for (int i = 0; i < rangeValues.Length; ++i)
									rangeValues[i] = rangeValues[i].Trim();

							//Range must specify min and max for x,y,z and w components
							if (rangeValues.Length != 2)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector4 \"" + p.name + "\" in \"" + filename + "\".  vector4 range must consist of 8 values - a min and a max for x,y,z and w components.  For example, [x1 y1 z1 w1 : x2 y2 z2 w2], where x1,y1,z1 and w1 specify the minimum for each component and x2,y2,z2 and w2 specify the maximum for each component",Color.Red);
								return null;
							}

							string[] minString = rangeValues[0].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							string[] maxString = rangeValues[1].Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);

							//Check for 4 components each
							if (minString.Length != 4)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector4 \"" + p.name + "\" in \"" + filename + "\".  vector4 range must consist of 8 values - a min and a max for x,y,z and w components.  For example, [x1 y1 z1 w1 : x2 y2 z2 w2], where x1,y1,z1 and w1 specify the minimum for each component and x2,y2,z2 and w2 specify the maximum for each component",Color.Red);
								return null;
							}
							//Check for 4 components each
							if (maxString.Length != 4)
							{
								Program.g_mainForm.logMessage("Wrong number of range values for vector4 \"" + p.name + "\" in \"" + filename + "\".  vector4 range must consist of 8 values - a min and a max for x,y,z and w components.  For example, [x1 y1 z1 w1 : x2 y2 z2 w2], where x1,y1,z1 and w1 specify the minimum for each component and x2,y2,z2 and w2 specify the maximum for each component",Color.Red);
								return null;
							}

							try
							{
								if (float.Parse(minString[0]) > float.Parse(maxString[0]))
								{
									Program.g_mainForm.logMessage("Minimum range x is greater than maximum range x for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[1]) > float.Parse(maxString[1]))
								{
									Program.g_mainForm.logMessage("Minimum range y is greater than maximum range y for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[2]) > float.Parse(maxString[2]))
								{
									Program.g_mainForm.logMessage("Minimum range z is greater than maximum range z for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								if (float.Parse(minString[3]) > float.Parse(maxString[3]))
								{
									Program.g_mainForm.logMessage("Minimum range w is greater than maximum range 2 for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
								p.minVecRangeValues = minString;
								p.maxVecRangeValues = maxString;
							}
							catch (Exception)
							{
								Program.g_mainForm.logMessage("Could not parse range values for vector4 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}
						}
						else
						{
							p.minVecRangeValues = null;
							p.maxVecRangeValues = null;
						}

						//Next comes the default value for the vector4.  Make sure the default value
						//exists in the specified range for the vector4 and is the right number of components
						string defVecValue = lineTokens[3].Trim().TrimStart('(').TrimEnd(')');
						try
						{
							string[] defComponents = defVecValue.Split(new char[] { ' ','\t' },StringSplitOptions.RemoveEmptyEntries);
							if (defComponents.Length != 4)
							{
								Program.g_mainForm.logMessage("Could not parse default value for vector4 \"" + p.name + "\" in \"" + filename + "\".  Vector values must be specified in floating point format.",Color.Red);
								return null;
							}

							if (p.minVecRangeValues != null)
							{
								if ((float.Parse(defComponents[0]) < float.Parse(p.minVecRangeValues[0]) || float.Parse(defComponents[0]) > float.Parse(p.maxVecRangeValues[0])) ||
									(float.Parse(defComponents[1]) < float.Parse(p.minVecRangeValues[1]) || float.Parse(defComponents[1]) > float.Parse(p.maxVecRangeValues[1])) ||
									(float.Parse(defComponents[2]) < float.Parse(p.minVecRangeValues[2]) || float.Parse(defComponents[2]) > float.Parse(p.maxVecRangeValues[2])) ||
									(float.Parse(defComponents[3]) < float.Parse(p.minVecRangeValues[3]) || float.Parse(defComponents[3]) > float.Parse(p.maxVecRangeValues[3])))
								{
									Program.g_mainForm.logMessage("Default value \"" + defVecValue + "\" does not exist in the specified range for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
									return null;
								}
							}
							p.defaultVecValue = defComponents;
						}
						catch (Exception)
						{
							Program.g_mainForm.logMessage("Could not parse default value \"" + defVecValue + "\" for vector4 \"" + p.name + "\" in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Next comes an optional description
						if (lineTokens.Length > 4)
							p.description = lineTokens[4].Trim();
					}
					else if (lineTokens[0].ToLower() == "color")
					{
						//color must have either 2 or 3 total tokens. The
						//description is optional.
						if (lineTokens.Length != 2 && lineTokens.Length != 3)
						{
							Program.g_mainForm.logMessage("Malformed shader parameter of type color in \"" + filename + "\".",Color.Red);
							return null;
						}

						//Store the parameter type
						p.type = "color";

						//Get the color's name next
						p.name = lineTokens[1].Trim();

						//Next comes an optional description
						if (lineTokens.Length > 2)
							p.description = lineTokens[2].Trim();
					}
					//else we didn't recognize the parameter type
					else
					{
						Program.g_mainForm.logMessage("Unrecognized shader parameter type \"" + lineTokens[0] + "\" in \"" + filename + "\".",Color.Red);
						return null;
					}

					shaderParams.Add(p);
				}
			}

			tr.Close();
			fs.Close();

			//Before we continue and generate C# code let's validate some conditions.
			//Make sure parameter names are unique
			foreach (ShaderParam param in shaderParams)
			{
				if (param.name == null)
					continue;

				int total = 0;
				foreach (ShaderParam param2 in shaderParams)
				{
					if (param.name == param2.name)
						total++;
				}
				if (total > 1)
				{
					Program.g_mainForm.logMessage("Mulitple shader parameters share the name \"" + param.name + "\" in " + filename + ".  Parameter names must be unique.",Color.Red);
					return null;
				}
			}

			//Name's cannot match any standard material strings either
			foreach (ShaderParam param in shaderParams)
			{
				if (param.name == null)
					continue;

				int total = 0;
				foreach (ShaderParam param2 in shaderParams)
				{
					if ((param.name == "DepthTest") ||
						(param.name == "DepthWrite") ||
						(param.name == "DepthFunc") ||
						(param.name == "DepthBias") ||
						(param.name == "DepthBiasClamp") ||
						(param.name == "SlopeScaledDepthBias") ||
						(param.name == "DepthClip") ||
						(param.name == "FillMode") ||
						(param.name == "Scissor") ||
						(param.name == "Multisample") ||
						(param.name == "AntialiasedLine") ||
						(param.name == "AlphaToCoverage") ||
						(param.name == "AlphaBlend") ||
						(param.name == "BlendOpType") ||
						(param.name == "SrcBlend") ||
						(param.name == "DestBlend") ||
						(param.name == "BlendOpAlpha") ||
						(param.name == "SrcBlendAlpha") ||
						(param.name == "DestBlendAlpha"))
						total++;
				}
				if (total > 1)
				{
					Program.g_mainForm.logMessage("Shader parameter \"" + param.name + "\" in " + filename + " is using a reserved name.  Use a different name for this shader parameter.",Color.Red);
					return null;
				}
			}


			//Now we have a list of shader parameters.  Lets generate a C# class that
			//derives from Material and compile it.  Also, build a shader name
			//suitable for compilation
			string compilableShaderName = shaderName;
			compilableShaderName = compilableShaderName.Replace(' ','_');
			compilableShaderName = compilableShaderName.Replace('.','_');
			compilableShaderName = compilableShaderName.Replace('#','_');
			compilableShaderName = compilableShaderName.Replace('-','_');

			string sourceCode = "using System; " +
								"using System.IO;" +
								"using System.Text;" +
								"using System.Runtime.Serialization;" +
								"using System.Collections.Generic;" +
								"using System.ComponentModel;" +
								"using System.Windows.Forms.Design;" +
								"using System.Windows.Forms;" +
								"using System.Drawing;" +
								"using System.Collections;" +
								"using XVPDesigner;" +
								"using XVPDesigner.Objects;" +
								"using XVPDesigner.Managers;" +
								"using XVPDesigner.TypeEditors;" +
								"namespace " + compilableShaderName +
								"{" +
								"   [Serializable]" +
								"   [ShaderDescriptor]" +
								"	public class " + compilableShaderName + " : Material" +
								"	{" +
								"		public " + compilableShaderName + "(string name) : base(name)" +
								"		{ setDefaultValues();" +
								"		}";
				
			//create compilable private and public names for each shader parameter
			for (int i = 0;i < shaderParams.Count;++i) 
			{
				//Generate a compilable name for property and variable from the display name
				ShaderParam p = (ShaderParam)shaderParams[i];
				p.compilableParamName = Program.g_mainForm.buildValidCodeString(p.name);
				p.privateParamVarName = "m_" + p.compilableParamName;
				shaderParams[i] = p;
			}

			//create data members for shader parameters 
			uint samplerIndex = 0;
			foreach (ShaderParam param in shaderParams)
			{
				//Validate parameters and fill in defaults if some are null
				if (param.name == null)
					throw new Exception("Shader parameters must have a name.  At least one parameter in " + filename + " does not have a name.");
				
				//Build browsable string
				string browsableString = "Browsable(false),";
				if (param.includeDefine == null || (param.includeDefine != null && defines.Contains(param.includeDefine+"="+param.includeDefine)))
					browsableString = "Browsable(true),";
				
				switch (param.type)
				{
					case "2DTexture":
						if (param.encoding == "XYZ")
							sourceCode += "private string " + param.privateParamVarName + "= Program.g_mainForm.XVPPath + \"xvpsdk/system/textures/defaultNormalMap.png\";";
						else
							sourceCode += "private string " + param.privateParamVarName + "= Program.g_mainForm.XVPPath + \"xvpsdk/system/textures/defaultTexture.png\";";

						sourceCode += "[DisplayName(\"File\")," +
									"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")," +
									browsableString + 
									"Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]" +
									"public string " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + getTextureFormatCheckCode() + param.privateParamVarName + " = value; " +
									" Program.g_mainForm.refreshTextureBrowser(getTextureNameList());" +
									" Program.g_mainForm.setTexture(m_proxy," + samplerIndex + ",value);}" +
									"}";
						sourceCode = addSamplerMembers(sourceCode,param,samplerIndex);
						samplerIndex++;
						break;
					case "3DTexture":
						sourceCode += "private string " + param.privateParamVarName + "= Program.g_mainForm.XVPPath + \"xvpsdk/system/textures/defaultTexture.png\";" +
									"[DisplayName(\"File\")," +
									"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")," +
									browsableString + 
									"Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]" +
									"public string " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + getTextureFormatCheckCode() + param.privateParamVarName + " = value; " +
									" Program.g_mainForm.refreshTextureBrowser(getTextureNameList());" +
									" Program.g_mainForm.setTexture(m_proxy," + samplerIndex + ",value);}" +
									"}";
						sourceCode = addSamplerMembers(sourceCode,param,samplerIndex);
						samplerIndex++;
						break;
					case "CubeTexture":
						sourceCode += "private string " + param.privateParamVarName + "= Program.g_mainForm.XVPPath + \"xvpsdk/system/textures/defaultCubeMap.dds\";" +
									"[DisplayName(\"File\")," +
									"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")," +
									browsableString + 
									"Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]" +
									"public string " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + getTextureFormatCheckCode() + param.privateParamVarName + " = value; " +
									" Program.g_mainForm.refreshTextureBrowser(getTextureNameList());" +
									" Program.g_mainForm.setTexture(m_proxy," + samplerIndex + ",value);}" +
									"}";
						sourceCode = addSamplerMembers(sourceCode,param,samplerIndex);
						samplerIndex++;
						break;
					case "bool":
						sourceCode += "private bool " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									browsableString + 
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")]" +
									"public bool " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + param.privateParamVarName + " = value; Program.g_mainForm.setBoolProperty(m_proxy,\"" + param.name + "\",value);}" +
									"}";
						break;
					case "enum":
						sourceCode += "public enum " + param.compilableParamName + "_ENUM {";
						foreach (string s in param.rangeValues)
						{
							sourceCode += s + ",";
						}
						sourceCode += "};";

						sourceCode += "private " + param.compilableParamName + "_ENUM " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									browsableString + 
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")]" +
									"public " + param.compilableParamName + "_ENUM " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + param.privateParamVarName + " = value; Program.g_mainForm.setEnumProperty(m_proxy,\"" + param.name + "\",Convert.ToInt32(value));}" +
									"}";
						break;
					case "int":
						sourceCode += "private int " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									browsableString + 
									"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.rangeValues[0],param.rangeValues[1],param.description)) + "\")," +
									"EditorAttribute(typeof(IntSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor))," + 
									"EditorRange(\""+param.rangeValues[0]+"\",\""+param.rangeValues[1]+"\")]" +
									"public int " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";
						if (param.rangeValues != null)
						{
							sourceCode += getMinValueCheckCode(param.rangeValues[0],param.privateParamVarName);
							sourceCode += getMaxValueCheckCode(param.rangeValues[1],param.privateParamVarName);
						}
						sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setIntProperty(m_proxy,\"" + param.name + "\",value);} }";
						break;
					case "uint":
						sourceCode += "private uint " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									browsableString + 
									"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.rangeValues[0],param.rangeValues[1],param.description)) + "\")," +
									"EditorAttribute(typeof(UIntSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor))," + 
									"EditorRange(\""+param.rangeValues[0]+"\",\""+param.rangeValues[1]+"\")]" +
									"public uint " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";

						if (param.rangeValues != null)
						{
							sourceCode += getMinValueCheckCode(param.rangeValues[0],param.privateParamVarName);
							sourceCode += getMaxValueCheckCode(param.rangeValues[1],param.privateParamVarName);
						}
						sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setUIntProperty(m_proxy,\"" + param.name + "\",value);} }";
						break;
					case "float":
						sourceCode += "private float " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.rangeValues[0],param.rangeValues[1],param.description)) + "\")," +
									browsableString + 
									"EditorAttribute(typeof(FloatSliderTypeEditor),typeof(System.Drawing.Design.UITypeEditor))," + 
									"EditorRange(\""+param.rangeValues[0]+"\",\""+param.rangeValues[1]+"\",\""+param.rangeValues[2]+"\")]" +
									"public float " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";

						if (param.rangeValues != null)
						{
							sourceCode += getMinValueCheckCodeFloat(param.rangeValues[0],param.privateParamVarName);
							sourceCode += getMaxValueCheckCodeFloat(param.rangeValues[1],param.privateParamVarName);
						}
						sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setFloatProperty(m_proxy,\"" + param.name + "\",value);} }";
						break;
					case "vector2":
						sourceCode += "private float[] " + param.privateParamVarName + " = new float[2]{0,0};" +
									"[DisplayName(\"" + param.name + "\")," +
									browsableString + 
									"CategoryAttribute(\"Shader Parameters\"),";
									if (param.minVecRangeValues != null)
										sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.minVecRangeValues[0] + " " + param.minVecRangeValues[1],param.maxVecRangeValues[0] + " " + param.maxVecRangeValues[1],param.description)) + "\")]";
									else
										sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString("","",param.description)) + "\")]";
									sourceCode += "public float[] " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";
						if (param.minVecRangeValues != null)
						{
							sourceCode += getValueCheckCodeFloatVec(0,param.minVecRangeValues[0],param.maxVecRangeValues[0],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(1,param.minVecRangeValues[1],param.maxVecRangeValues[1],param.privateParamVarName);
						}
						else
							sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setVector2DProperty(m_proxy,\"" + param.name + "\",value[0],value[1]);";
						sourceCode += "} }";
						break;
					case "vector3":
						sourceCode += "private float[] " + param.privateParamVarName + " = new float[3]{0,0,0};" +
									"[DisplayName(\"" + param.name + "\")," +
									browsableString + 
									"CategoryAttribute(\"Shader Parameters\"),";
									if (param.minVecRangeValues != null)
										sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.minVecRangeValues[0] + " " + param.minVecRangeValues[1] + " " + param.minVecRangeValues[2],param.maxVecRangeValues[0] + " " + param.maxVecRangeValues[1] + " " + param.maxVecRangeValues[2],param.description)) + "\")]";
									else
										sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString("","",param.description)) + "\")]";
									sourceCode += "public float[] " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";
						if (param.minVecRangeValues != null)
						{
							sourceCode += getValueCheckCodeFloatVec(0,param.minVecRangeValues[0],param.maxVecRangeValues[0],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(1,param.minVecRangeValues[1],param.maxVecRangeValues[1],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(2,param.minVecRangeValues[2],param.maxVecRangeValues[2],param.privateParamVarName);
						}
						else
							sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setVector3DProperty(m_proxy,\"" + param.name + "\",value[0],value[1],value[2]);";
						sourceCode += "} }";
						break;
					case "vector4":
						sourceCode += "private float[] " + param.privateParamVarName + " = new float[4]{0,0,0,0};" +
									"[DisplayName(\"" + param.name + "\")," +
									browsableString + 
									"CategoryAttribute(\"Shader Parameters\"),";
						if (param.minVecRangeValues != null)
							sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(param.minVecRangeValues[0] + " " + param.minVecRangeValues[1] + " " + param.minVecRangeValues[2] + " " + param.minVecRangeValues[3],param.maxVecRangeValues[0] + " " + param.maxVecRangeValues[1]+ " " + param.maxVecRangeValues[2] + " " + param.maxVecRangeValues[3],param.description) ) + "\")]";
						else
							sourceCode += "DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString("","",param.description)) + "\")]";
							sourceCode += "public float[] " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { ";
						if (param.minVecRangeValues != null)
						{
							sourceCode += getValueCheckCodeFloatVec(0,param.minVecRangeValues[0],param.maxVecRangeValues[0],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(1,param.minVecRangeValues[1],param.maxVecRangeValues[1],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(2,param.minVecRangeValues[2],param.maxVecRangeValues[2],param.privateParamVarName);
							sourceCode += getValueCheckCodeFloatVec(3,param.minVecRangeValues[3],param.maxVecRangeValues[3],param.privateParamVarName);
						}
						else
							sourceCode += param.privateParamVarName + " = value; Program.g_mainForm.setVector4DProperty(m_proxy,\"" + param.name + "\",value[0],value[1],value[2],value[3]);";
						sourceCode += "} }";
						break;
					case "color":
						sourceCode += "private Color " + param.privateParamVarName + ";" +
									"[DisplayName(\"" + param.name + "\")," +
									"CategoryAttribute(\"Shader Parameters\")," +
									browsableString + 
									"DescriptionAttribute(\"" + Utility.genCompilableString(param.description) + "\")]" +
									"public Color " + param.compilableParamName +
									"{ get { return " + param.privateParamVarName + ";}" +
									" set { " + param.privateParamVarName + " = value; Program.g_mainForm.setColorProperty(m_proxy,\"" + param.name + "\"," + param.privateParamVarName + ".R/255.0f," +param.privateParamVarName + ".G/255.0f,"+param.privateParamVarName + ".B/255.0f,1.0f); }" +
									"}";
						break;
					default:
						throw new Exception("Unrecognized shader param \"" + param.type + "\" found in shader param list.");
				}
			}

			//Create the postDeserializeRangeCheck method
			//This method checks ranges in case data was saved with old range limits.  For example, if
			//a property called "Speed" was saved with a value of 50, but later the valid 
			//range for "Speed" becomes 0 - 25, the old value will be clamped to the new range.
			sourceCode += "public override void postDeserializeRangeCheck()" +
							"{";
								
					foreach (ShaderParam param in shaderParams)
					{		
						//Only numeric quantities need this check. 
						switch (param.type)
						{
							case "int":
							case "uint":
							case "float":
							case "vector2":
							case "vector3":
							case "vector4":
								sourceCode += param.compilableParamName + " = " + param.compilableParamName + ";";
								break;
						}
					}
			sourceCode += " base.postDeserializeRangeCheck();}";

			//Create the OnTexDrop handler
			sourceCode += "public override void OnTexDrop(string texName, string texFilename)" +
							"{" +
								"switch (texName)" +
								"{";
								
								foreach (ShaderParam param in shaderParams)
								{
									switch (param.type)
									{
										case "2DTexture":
										case "3DTexture":
										case "CubeTexture":
											sourceCode += "case \"" + param.name + "\":" +
														  param.compilableParamName + " = texFilename;" +
														  "break;";
											break;
									}
								}
								sourceCode += "default: break;" + //MessageBox.Show(\"Material does not have a texture named \" + texName); break;" +
								"}" +
							"}";

			//Create the refreshTextureBrowser method
			sourceCode += "public override TextureNameList getTextureNameList()" +
						  "{" +
						  "		TextureNameList textureNameList = new TextureNameList();" +
						  "		textureNameList.ownerMaterial = this;";
						  		
			foreach (ShaderParam param in shaderParams)
			{
				switch (param.type)
				{
					case "2DTexture":
					case "3DTexture":
					case "CubeTexture":
						sourceCode += "textureNameList.names.Add(\"" + param.name + "\");" +
									  "textureNameList.filenames.Add(" + param.privateParamVarName + ");" +
									  "textureNameList.encoding.Add(\"" + param.encoding + "\");" +
									  "textureNameList.description.Add(\"" + param.description + "\");";
						break;
				}
			}
			sourceCode += " return textureNameList;}";


			//Create the export method
			sourceCode += "override public void export(BinaryWriter bw)" +
						  "{ base.export(bw);";

			//Output num textures
			int numTextures = 0;
			foreach (ShaderParam param in shaderParams)
			{
				if ((param.type == "2DTexture") ||
					(param.type == "3DTexture") ||
					(param.type == "CubeTexture"))
					numTextures++;
			}
			sourceCode += "bw.Write(" + numTextures + ");";

			foreach (ShaderParam param in shaderParams)
			{
				switch (param.type)
				{
					case "2DTexture":
					case "3DTexture":
					case "CubeTexture":
						sourceCode += "Utility.writeCString(bw,Path.ChangeExtension(Path.GetFileName(" + param.privateParamVarName + "),\".xif\"));" +
									  "bw.Write((int)" + param.privateParamVarName + "AddressUMode); " +
									  "bw.Write((int)" + param.privateParamVarName + "AddressVMode); " +
                                      "bw.Write((int)" + param.privateParamVarName + "AddressWMode); " +
                                      "bw.Write((float)" + param.privateParamVarName + "MipLODBias); " +
                                      "bw.Write((uint)" + param.privateParamVarName + "MaxAnisotropy); " +
                                      "bw.Write((int)" + param.privateParamVarName + "ComparisonFunc); " +
                                      "bw.Write((float)" + param.privateParamVarName + "MinLOD); " +
                                      "bw.Write((float)" + param.privateParamVarName + "MaxLOD); " +
                                      "bw.Write((float)" + param.privateParamVarName + "BorderColor.R / 255.0f);" +
                                      "bw.Write((float)" + param.privateParamVarName + "BorderColor.G / 255.0f);" +
                                      "bw.Write((float)" + param.privateParamVarName + "BorderColor.B / 255.0f);" +
                                      "bw.Write((float)" + param.privateParamVarName + "BorderColorAlpha / 255.0f);" +
                                      "bw.Write((int)" + param.privateParamVarName + "Filter); ";
						break;
				}
			}

			//Now output the shader param block
			foreach (ShaderParam param in shaderParams)
			{
				switch (param.type)
				{
					case "bool":
						sourceCode += "bw.Write((bool)" + param.privateParamVarName + "); ";
						break;
					case "enum":
						sourceCode += "bw.Write((int)" + param.privateParamVarName + "); ";
						break;
					case "int":
						sourceCode += "bw.Write((int)" + param.privateParamVarName + "); ";
						break;
					case "uint":
						sourceCode += "bw.Write((uint)" + param.privateParamVarName + "); ";
						break;
					case "float":
						sourceCode += "bw.Write((float)" + param.privateParamVarName + "); ";
						break;
					case "vector2":
						sourceCode += "bw.Write((float)" + param.privateParamVarName + "[0]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[1]);";
						break;
					case "vector3":
						sourceCode += "bw.Write((float)" + param.privateParamVarName + "[0]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[1]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[2]);";
						break;
					case "vector4":
						sourceCode += "bw.Write((float)" + param.privateParamVarName + "[0]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[1]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[2]);" +
									  "bw.Write((float)" + param.privateParamVarName + "[3]);";
						break;
					case "color":
						sourceCode += "bw.Write((float)" + param.privateParamVarName + ".R / 255.0f);" +
									  "bw.Write((float)" + param.privateParamVarName + ".G / 255.0f);" +
									  "bw.Write((float)" + param.privateParamVarName + ".B / 255.0f);";
						break;
				}
			}
			sourceCode += " }";
			
			//Create the setDefaultValues() method and deserializing event method
			sourceCode += "[OnDeserializingAttribute()]" +
							"internal void deserializeInit(StreamingContext context)" +
							"{m_proxy = IntPtr.Zero; setDefaultValues();}";

			sourceCode += "override public void setDefaultValues() {";
			foreach (ShaderParam param in shaderParams)
			{
				switch (param.type)
				{
					case "2DTexture":
					case "3DTexture":
					case "CubeTexture":
						sourceCode += param.privateParamVarName + "AddressUMode = TextureAddressType.WRAP;" +
									  param.privateParamVarName + "AddressVMode = TextureAddressType.WRAP;" +
                                      param.privateParamVarName + "AddressWMode = TextureAddressType.WRAP;" +
                                      param.privateParamVarName + "MipLODBias = 0.0f; " +
                                      param.privateParamVarName + "MaxAnisotropy = 1; " +
                                      param.privateParamVarName + "ComparisonFunc = CompareFuncType.NEVER; " +
                                      param.privateParamVarName + "MinLOD = 0.0f; " +
                                      param.privateParamVarName + "MaxLOD = 10000.0f; " +
                                      param.privateParamVarName + "BorderColor = Color.White;" +
                                      param.privateParamVarName + "BorderColorAlpha = 255;" +
                                      param.privateParamVarName + "Filter = TextureFilterType.MIN_MAG_MIP_LINEAR;";
						break;
					case "bool":
						sourceCode += param.privateParamVarName + " = " + param.defaultValue + ";";
						break;
					case "enum":
						sourceCode += param.privateParamVarName + " = " + param.compilableParamName + "_ENUM." + param.defaultValue + ";";
						break;
					case "int":
						sourceCode += param.privateParamVarName + " = " + param.defaultValue + ";";
						break;
					case "uint":
						sourceCode += param.privateParamVarName + " = " + param.defaultValue + ";";
						break;
					case "float":
						sourceCode += param.privateParamVarName + " = " + param.defaultValue + "f;";
						break;
					case "vector2":
						sourceCode += param.privateParamVarName + " = new float[2];";
						sourceCode += param.privateParamVarName + "[0] = " + param.defaultVecValue[0] + "f;";
						sourceCode += param.privateParamVarName + "[1] = " + param.defaultVecValue[1] + "f;";
						break;
					case "vector3":
						sourceCode += param.privateParamVarName + " = new float[3];";
						sourceCode += param.privateParamVarName + "[0] = " + param.defaultVecValue[0] + "f;";
						sourceCode += param.privateParamVarName + "[1] = " + param.defaultVecValue[1] + "f;";
						sourceCode += param.privateParamVarName + "[2] = " + param.defaultVecValue[2] + "f;";
						break;
					case "vector4":
						sourceCode += param.privateParamVarName + " = new float[4];";
						sourceCode += param.privateParamVarName + "[0] = " + param.defaultVecValue[0] + "f;";
						sourceCode += param.privateParamVarName + "[1] = " + param.defaultVecValue[1] + "f;";
						sourceCode += param.privateParamVarName + "[2] = " + param.defaultVecValue[2] + "f;";
						sourceCode += param.privateParamVarName + "[3] = " + param.defaultVecValue[3] + "f;";
						break;
					case "color":
						sourceCode += param.privateParamVarName + " = Color.White;";
						break;
				}
			}
			sourceCode += " base.setDefaultValues();}";

			//Create the setCurrentValues() method
			sourceCode += "override public void setCurrentValues() {";

			sourceCode += " base.setCurrentValues();";

			foreach (ShaderParam param in shaderParams)
			{
				switch (param.type)
				{
					case "2DTexture":
					case "3DTexture":
					case "CubeTexture":
						sourceCode += param.compilableParamName + "AddressUMode = " + param.compilableParamName + "AddressUMode;" + 
									  param.compilableParamName + "AddressVMode = " + param.compilableParamName + "AddressVMode;" + 
									  param.compilableParamName + "AddressWMode = " + param.compilableParamName + "AddressWMode;" + 
									  param.compilableParamName + "MipLODBias = " + param.compilableParamName + "MipLODBias;" + 
									  param.compilableParamName + "MaxAnisotropy = " + param.compilableParamName + "MaxAnisotropy;" + 
									  param.compilableParamName + "ComparisonFunc = " + param.compilableParamName + "ComparisonFunc;" + 
									  param.compilableParamName + "MinLOD = " + param.compilableParamName + "MinLOD;" + 
									  param.compilableParamName + "MaxLOD = " + param.compilableParamName + "MaxLOD;" + 
									  param.compilableParamName + "BorderColor = " + param.compilableParamName + "BorderColor;" + 
									  param.compilableParamName + "BorderColorAlpha = " + param.compilableParamName + "BorderColorAlpha;" +
									  param.compilableParamName + "Filter = " + param.compilableParamName + "Filter;";
						sourceCode += param.compilableParamName + " = " + param.compilableParamName + ";";
						break;
					case "bool":
					case "enum":
					case "int":
					case "uint":
					case "float":
					case "vector2":
					case "vector3":
					case "vector4":
					case "color":
						sourceCode += param.compilableParamName + " = " + param.compilableParamName + ";";
						break;
				}
			}
			sourceCode += " }";


			sourceCode += " }" +
						"}";




			return sourceCode;
		}

		public string addSamplerMembers(string sourceCode,ShaderParam param,uint samplerIndex)
		{
			return sourceCode +
			"private TextureAddressType " + param.privateParamVarName + "AddressUMode;" +
			"[DisplayName(\"U Address Mode\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Method to use for resolving a u texture coordinate that is outside the 0 to 1 range .\")]" +
			"public TextureAddressType " + param.compilableParamName + "AddressUMode" +
			"{ get { return " + param.privateParamVarName + "AddressUMode;}" +
			" set { " + param.privateParamVarName + "AddressUMode = value; Program.g_mainForm.setSamplerAddressU(m_proxy,"+samplerIndex+",value);}}" +

			"private TextureAddressType " + param.privateParamVarName + "AddressVMode;" +
			"[DisplayName(\"V Address Mode\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Method to use for resolving a v texture coordinate that is outside the 0 to 1 range .\")]" +
			"public TextureAddressType " + param.compilableParamName + "AddressVMode" +
			"{ get { return " + param.privateParamVarName + "AddressVMode;}" +
			" set { " + param.privateParamVarName + "AddressVMode = value; Program.g_mainForm.setSamplerAddressV(m_proxy," + samplerIndex + ",value);}}" +

			"private TextureAddressType " + param.privateParamVarName + "AddressWMode;" +
			"[DisplayName(\"W Address Mode\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Method to use for resolving a w texture coordinate that is outside the 0 to 1 range .\")]" +
			"public TextureAddressType " + param.compilableParamName + "AddressWMode" +
			"{ get { return " + param.privateParamVarName + "AddressWMode;}" +
			" set { " + param.privateParamVarName + "AddressWMode = value; Program.g_mainForm.setSamplerAddressW(m_proxy," + samplerIndex + ",value);}}" +

			"private float " + param.privateParamVarName + "MipLODBias;" +
			"[DisplayName(\"Mip LOD Bias\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Offset from the calculated mipmap level. For example, if a texture should be sampled at mipmap level 3 and MipLODBias is 2, then the texture will be sampled at mipmap level 5.\")]" +
			"public float " + param.compilableParamName + "MipLODBias" +
			"{ get { return " + param.privateParamVarName + "MipLODBias;}" +
			" set { " + param.privateParamVarName + "MipLODBias = value; Program.g_mainForm.setSamplerMipLODBias(m_proxy," + samplerIndex + ",value);}}" +

			"private uint " + param.privateParamVarName + "MaxAnisotropy;" +
			"[DisplayName(\"Max Anisotropy\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Maximum Anisotropy.\")]" +
			"public uint " + param.compilableParamName + "MaxAnisotropy" +
			"{ get { return " + param.privateParamVarName + "MaxAnisotropy;}" +
			" set { " + param.privateParamVarName + "MaxAnisotropy = value; Program.g_mainForm.setSamplerMaxAnisotropy(m_proxy," + samplerIndex + ",value);}}" +

			"private CompareFuncType " + param.privateParamVarName + "ComparisonFunc;" +
			"[DisplayName(\"Comparison Function\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"A function that compares sampled data against existing sampled data.\")]" +
			"public CompareFuncType " + param.compilableParamName + "ComparisonFunc" +
			"{ get { return " + param.privateParamVarName + "ComparisonFunc;}" +
			" set { " + param.privateParamVarName + "ComparisonFunc = value; Program.g_mainForm.setSamplerFunc(m_proxy," + samplerIndex + ",value);}}" +

			"private float " + param.privateParamVarName + "MinLOD;" +
			"[DisplayName(\"Min LOD\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Lower end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed.\")]" +
			"public float " + param.compilableParamName + "MinLOD" +
			"{ get { return " + param.privateParamVarName + "MinLOD;}" +
			" set { " + param.privateParamVarName + "MinLOD = value; Program.g_mainForm.setSamplerMinLOD(m_proxy," + samplerIndex + ",value);}}" +

			"private float " + param.privateParamVarName + "MaxLOD;" +
			"[DisplayName(\"Max LOD\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level and any level higher than that is less detailed. This value must be greater than or equal to MinLOD.\")]" +
			"public float " + param.compilableParamName + "MaxLOD" +
			"{ get { return " + param.privateParamVarName + "MaxLOD;}" +
			" set { " + param.privateParamVarName + "MaxLOD = value; Program.g_mainForm.setSamplerMaxLOD(m_proxy," + samplerIndex + ",value);}}" +

			"private Color " + param.privateParamVarName + "BorderColor;" +
			"[DisplayName(\"Border Color\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Border color to use if border addressing mode is specified for AddressU, AddressV, or AddressW.\")]" +
			"public Color " + param.compilableParamName + "BorderColor" +
			"{ get { return " + param.privateParamVarName + "BorderColor;}" +
			" set { " + param.privateParamVarName + "BorderColor = value; Program.g_mainForm.setSamplerBorderColor(m_proxy," + samplerIndex + ",value.R/255.0f,value.G/255.0f,value.B/255.0f," + param.privateParamVarName + "BorderColorAlpha/255.0f);}}" +

			"private byte " + param.privateParamVarName + "BorderColorAlpha;" +
			"[DisplayName(\"Border Color Alpha\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Border color alpha to use if border addressing mode is specified for AddressU, AddressV, or AddressW.\")]" +
			"public byte " + param.compilableParamName + "BorderColorAlpha" +
			"{ get { return " + param.privateParamVarName + "BorderColorAlpha;}" +
			" set { " + param.privateParamVarName + "BorderColorAlpha = value; Program.g_mainForm.setSamplerBorderColor(m_proxy," + samplerIndex + "," + param.privateParamVarName + "BorderColor.R/255.0f," + param.privateParamVarName + "BorderColor.G/255.0f," + param.privateParamVarName + "BorderColor.B/255.0f," + param.privateParamVarName + "BorderColorAlpha/255.0f);}}" +

			"private TextureFilterType " + param.privateParamVarName + "Filter;" +
			"[DisplayName(\"Filter\")," +
			"CategoryAttribute(\"Texture: " + param.name + " (" + param.encoding + ")\")," +
			"DescriptionAttribute(\"Filtering method to use when sampling a texture.\")]" +
			"public TextureFilterType " + param.compilableParamName + "Filter" +
			"{ get { return " + param.privateParamVarName + "Filter;}" +
			" set { " + param.privateParamVarName + "Filter = value; Program.g_mainForm.setSamplerFilter(m_proxy," + samplerIndex + ",value);}}";
		}

		public Material createMaterial(string materialName,string shaderName)
		{
			Assembly shaderAssembly = (Assembly)m_loadedShaders[shaderName];
			Material shaderInstance = null;
			if (shaderAssembly != null)
			{
				try
				{
					Type shaderDescriptorType = null;
					foreach (Type type in shaderAssembly.GetTypes())
					{
						if (type.IsAbstract) continue;
						if (type.IsDefined(typeof(ShaderDescriptor),true))
						{
							//We've found the shader descriptor type, so we'll stop here.  
							shaderDescriptorType = type;

							break;
						}
					}

					object[] parameters = new object[1];
					parameters[0] = materialName;
					shaderInstance = (Material)shaderAssembly.CreateInstance(shaderDescriptorType.FullName,
									 false,BindingFlags.Default,null,parameters,null,null);
				}
				catch (Exception e)
				{
					string s = e.ToString();
					Program.g_mainForm.logMessage("Could not create an instance of " + shaderName,Color.Red);
					Program.g_mainForm.logMessage(s,Color.Red);
				}
			}
			else
				Program.g_mainForm.logMessage("Could not find shader " + shaderName,Color.Red);

			return shaderInstance;
		}

		public ArrayList getShaderNames()
		{
			ArrayList shaderNames = new ArrayList();
			foreach (string s in m_loadedShaders.Keys)
				shaderNames.Add(s);

			return shaderNames;
		}

		private string getMinValueCheckCode(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode = "if (value < " + value + "){" + variableName + " = " + value + "; return;}";
			return sourceCode;
		}

		private string getMaxValueCheckCode(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode += "if (value > " + value + "){" + variableName + " = " + value + "; return;}";
			return sourceCode;
		}

		private string getMinValueCheckCodeFloat(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode = "if (value < " + value + "){" + variableName + " = " + value + "f; return;}";
			return sourceCode;
		}

		private string getMaxValueCheckCodeFloat(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode += "if (value > " + value + "){" + variableName + " = " + value + "f; return;}";
			return sourceCode;
		}

		private string getValueCheckCodeFloatVec(int index,string minValue,string maxValue,string variableName)
		{
			string sourceCode = "";
			sourceCode += "if (value[" + index + "] < " + minValue + "){" + variableName + "[" + index + "] = " + minValue + "f;}";
			sourceCode += "else if (value[" + index + "] > " + maxValue + "){" + variableName + "[" + index + "] = " + maxValue + "f;}";
			sourceCode += "else {" + variableName + "[" + index + "] = value[" + index + "];}";
			return sourceCode;
		}

		private string getTextureFormatCheckCode()
		{
			return  "if (value != null)" +
					"{" +
						"string ext = Path.GetExtension(value);" +
						"ext = ext.ToLower();" +
						"if ((ext != \".bmp\") && (ext != \".dds\") && (ext != \".jpg\") && (ext != \".gif\") && (ext != \".png\") && (ext != \".tif\")  )" +
						"{ " +
							"MessageBox.Show(\"The texture format supplied is not supported. Supported textures formats include .DDS, .PNG, .BMP, .JPG, .TIF, .GIF\",\"Unsupported Texture Format\",MessageBoxButtons.OK,MessageBoxIcon.Error);" +
							"return;" +
						"} " +
					"}";
		}

	}
}
