using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace XVPLuaGen
{
	class Program
	{
		//Source code directories
		static string[] m_directories = new string[] {"c:/",
													  "c:/",
													  "c:/"};

		static int Main(string[] args)
		{
			//Parse all code looking for function to be exported to scripts
			for (int i = 0; i < m_directories.Length; ++i)
			{
				foreach (string s in Directory.GetFiles(m_directories[i],"*.h"))
				{
					//Open the file for reading
					FileStream fs = null;
					try
					{
						fs = File.Open(s,FileMode.Open,FileAccess.Read);
					}
					catch (Exception exception)
					{
						Console.WriteLine(exception.ToString());
						if (fs != null)
							fs.Close();
						return -1;
					}

					TextReader tr = new StreamReader(fs);
					string line = null;
					
					//while there are still lines of text to parse ...
					while ((line = tr.ReadLine()) != null)
					{
						if (line.Contains("XVP_LUA"))
						{
							//Is this just a global
						}
					}
				}
			}

			return 0;
		}
	}
}
