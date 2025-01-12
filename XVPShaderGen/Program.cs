using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace XVPShaderGen
{
    class Program
    {
        static int Main(string[] args)
        {
			Int32 numPermutations = 0;
			const Int32 maxFeatures = 64;
			List<string> featureDefines = new List<string>();
			bool hasVS = false;
			bool hasPS = false;
			bool hasGS = false;
			bool buildAll = false;

			for (int i = 0;i < args.Length;++i)
			{
				if (args[i] == "-buildAll")
					buildAll = true;
			}

			string startupPath = Process.GetCurrentProcess().MainModule.FileName;
			startupPath = Path.GetDirectoryName(startupPath) + "\\";

			//Build a string path to the hlsl compiler..
			string fxcString = Environment.GetEnvironmentVariable("DXSDK_DIR");
			if (fxcString == null)
			{
				Console.WriteLine("Unable to get DXSDK_DIR to find hlsl compiler");
				return -1;
			}
			fxcString += "\\Utilities\\bin\\x86\\fxc.exe";

			Process p = new Process();
			ProcessStartInfo psi = new ProcessStartInfo();
			psi.FileName = fxcString;
			psi.CreateNoWindow = true;
			psi.WindowStyle = ProcessWindowStyle.Hidden;
			p.StartInfo = psi;
			
            //We expect a shader filename and a platform ID at a minimum
            if (args.Length < 2)
            {
                Console.WriteLine("Expected at least 2 parameters - a shader filename followed by a platform ID string");
                return -1;
            }
            if (!File.Exists(args[0]))
            {
                Console.WriteLine("File " + args[0] + " does not exist");
                return -1;
            }

            //Open the shader file and look for features and other information
          	StreamReader sr;
			try
			{
				sr = File.OpenText(args[0]);
			}
			catch (Exception exception)
			{
				Console.WriteLine("Unable to open file - " + exception);
				
				return -1;
			}

			string input = null;
			while ((input = sr.ReadLine()) != null)
			{
				if (input.Contains("//@@|"))
				{
					string[] parts = input.Split(new char[] { '|' },StringSplitOptions.RemoveEmptyEntries);
					if (parts.Length == 2)
					{
						if (parts[1].Contains("VS"))
							hasVS = true;
						else if (parts[1].Contains("GS"))
							hasGS = true;
						else if (parts[1].Contains("PS"))
							hasPS = true;
						else
						{
							Console.WriteLine("Unrecognized shader string - " + input + " Expected VS,PS or GS.");
							return -1;
						}
					}
					else
					{
						Console.WriteLine("Malformed shader string - " + input + " Expected VS,PS or GS.");
						return -1;
					}
				}
				else if (input.Contains("//##|"))
				{
					string[] featureParts = input.Split(new char[] {'|'}, StringSplitOptions.RemoveEmptyEntries);
					if (featureParts.Length >= 4)
					{
						//We assume the following
						//1. the first part is the required special symbol "//##|"
						//2. is the required feature symbol used as a #define during compilation
						//3. is the required feature name 
						//4. is the required feature description 
						//5. the reset of the parts are optional dependencies of the feature
						featureDefines.Add(featureParts[1].Trim());
					}
					else
					{
						Console.WriteLine("Malformed feature string - " + input + " Expected at least 4 tokens.");
						return -1;
					}
				}
			}

			sr.Close();

			//Max feature check
			if (featureDefines.Count > maxFeatures)
			{
				Console.WriteLine("Too many features for this shader.  Maximum number of feature defines is " + maxFeatures);
				return -1;
			}
			
			if (!buildAll)
			{
				string configString = "";
				for (int i = 2; i < args.Length; ++i)
					configString += "-" + args[i];
				
				//Build the shader permutation
				string objectFilename = Path.GetDirectoryName(args[0]);
				if (objectFilename != "")
					objectFilename += "\\";
				objectFilename += Path.GetFileNameWithoutExtension(args[0]);
				Console.WriteLine("Building " + objectFilename + configString);

				try
				{
					//Build the define string to pass to the command line
					string[] defines = configString.Split('-');
					string commandLine = "";
					for(int k = 1;k < defines.Length;++k)
						commandLine += " /D" + defines[k] + "=1";

					if(hasVS)
					{
						psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainVS /Tvs_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".vso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
						//Console.WriteLine(psi.Arguments);
						p.Start();
						p.WaitForExit();
						string errors = CheckForCompilationErrors();
						if((p.ExitCode != 0) || (errors != null))
						{
							Console.WriteLine("Vertex shader compilation failed!\n" + errors);
							return -1;
						}
					}

					if(hasGS)
					{
						psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainGS /Tgs_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".gso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
						//Console.WriteLine(psi.Arguments);
						p.Start();
						p.WaitForExit();
						string errors = CheckForCompilationErrors();
						if((p.ExitCode != 0) || (errors != null))
						{
							Console.WriteLine("Geometry shader compilation failed!\n" + errors);
							return -1;
						}
					}

					if(hasPS)
					{
						psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainPS /Tps_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".pso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
						//Console.WriteLine(psi.Arguments);
						p.Start();
						p.WaitForExit();
						string errors = CheckForCompilationErrors();
						if((p.ExitCode != 0) || (errors != null))
						{
							Console.WriteLine("Pixel shader compilation failed!\n" + errors);
							return -1;
						}
					}

				}
				catch(Exception exception)
				{
					Console.WriteLine("Exception occurred while running " + psi.FileName + " Exception -> " + exception.ToString());
					return -1;
				}
			}
			else
			{
				//Now that we have the feature defines we can calculate the number
				//of permutations and generate all the possible shaders
				numPermutations = (Int32)Math.Pow(2, featureDefines.Count());
				for (Int32 i = 0; i < numPermutations; ++i)
				{
					Int64 configBits = i;
					string configString = "";
					for (Int32 j = 0; j < maxFeatures; ++j)
					{
						if ((configBits & 0x1) == 1)
						{
							if (j < featureDefines.Count())
								configString += "-" + featureDefines[j];
						}
						configBits = configBits >> 1;
					}

					//Build the shader permutation
					string objectFilename = Path.GetDirectoryName(args[0]);
					if (objectFilename != "")
						objectFilename += "\\";
					objectFilename += Path.GetFileNameWithoutExtension(args[0]);
					Console.WriteLine("Building " + objectFilename + configString);

					try
					{
						//Build the define string to pass to the command line
						string[] defines = configString.Split('-');
						string commandLine = "";
						for (int k = 1; k < defines.Length; ++k)
							commandLine += " /D" + defines[k] + "=1";

						if (hasVS)
						{
							psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainVS /Tvs_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".vso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
							//Console.WriteLine(psi.Arguments);
							p.Start();
							p.WaitForExit();
							string errors = CheckForCompilationErrors();
							if((p.ExitCode != 0) || (errors != null))
							{
								Console.WriteLine("Vertex shader compilation failed!\n" + errors);
								return -1;
							}
						}

						if (hasGS)
						{
							psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainGS /Tgs_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".gso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
							//Console.WriteLine(psi.Arguments);
							p.Start();
							p.WaitForExit();
							string errors = CheckForCompilationErrors();
							if((p.ExitCode != 0) || (errors != null))
							{
								Console.WriteLine("Geometry shader compilation failed!\n" + errors);
								return -1;
							}
						}

						if (hasPS)
						{
							psi.Arguments = "\"" + args[0] + "\" /Zpr /EmainPS /Tps_4_0 " + commandLine + " /Fo\"" + objectFilename + configString + ".pso\" " + "/Fe\""+startupPath+"ShaderErrors.txt\"";
							//Console.WriteLine(psi.Arguments);
							p.Start();
							p.WaitForExit();
							string errors = CheckForCompilationErrors();
							if((p.ExitCode != 0) || (errors != null))
							{
								Console.WriteLine("Pixel shader compilation failed!\n" + errors);
								return -1;
							}
						}

					}
					catch (Exception exception)
					{
						Console.WriteLine("Exception occurred while running " + psi.FileName + " Exception -> " + exception.ToString());
						return -1;
					}

				}
			}

			p.Dispose();
			
            return 0;
        }

		static public string CheckForCompilationErrors()
		{
			try
			{
				string startupPath = Process.GetCurrentProcess().MainModule.FileName;
				startupPath = Path.GetDirectoryName(startupPath) + "\\";

				if (File.Exists(startupPath + "ShaderErrors.txt"))
				{
					FileStream f = File.Open(startupPath + "ShaderErrors.txt", FileMode.Open);
					TextReader tr = new StreamReader(f);
					string errors = tr.ReadToEnd();
					tr.Close();
					f.Close();
					if (errors == "")
						return null;
					else
						return errors;					
				}

				return null;
			}
			catch (Exception exception)
			{
				Console.WriteLine(exception.ToString());
				return null;
			}
		}
    }
}
