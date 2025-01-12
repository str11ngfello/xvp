using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.CodeDom.Compiler;
using System.Reflection;
using XVPDesigner.Managers;
using DevComponents.DotNetBar;
using System.Collections;
using XVPDesigner.Objects;


namespace XVPDesigner.UserControls
{
	public partial class CustomDataBuilder : UserControl
	{
		private string m_cddPath;
		Hashtable m_loadedCDDs = new Hashtable(500);
		bool m_dirty = false;

		//This enum must match the run time side enum exactly
		public enum ParamType
		{
			CDDPT_Decimal,
			CDDPT_DecimalList,
			CDDPT_Integer,
			CDDPT_IntegerList,
			CDDPT_Bool,
			CDDPT_BoolList,
			CDDPT_String,
			CDDPT_StringList,
			CDDPT_File,
			CDDPT_FileList,
			CDDPT_RGBColor,
			CDDPT_RGBColorList
		};

		public CustomDataBuilder()
		{
			InitializeComponent();
			cbDataType.SelectedIndex = 0;
			lblDataTypeExamples.Text = "+/- 1.5";
		}

		public bool init(string cddPath,ButtonItem createButton)
		{
			//Save path
			m_cddPath = cddPath;

			//Setup a file watcher to refresh the list of cdds when
			//something changes externally
			fileSystemWatcher.IncludeSubdirectories = false;
			fileSystemWatcher.Filter = "*.cdd";
			fileSystemWatcher.Path = cddPath;
			fileSystemWatcher.NotifyFilter = NotifyFilters.FileName |
											  NotifyFilters.CreationTime |
											  NotifyFilters.LastWrite |
											  NotifyFilters.DirectoryName;

			//Load the directory of CDDs
			refreshCDDs(createButton);

			return true;
		}

		private void refreshCDDs(ButtonItem createButton)
		{
			//Reset the controls
			cbDataDefinitions.Items.Clear();
			lvDataTypes.Items.Clear();

			Program.g_mainForm.logMessage("Searching for new or updated custom data definitions...",Color.Black);

			//Create compiler and add the common assemblies
			CodeDomProvider codeProvider = CodeDomProvider.CreateProvider("C#");
			CompilerParameters cp = new CompilerParameters();
			cp.ReferencedAssemblies.Add("XVPDesigner.exe");
			cp.ReferencedAssemblies.Add("System.dll");
			cp.ReferencedAssemblies.Add("System.Drawing.dll");
			cp.ReferencedAssemblies.Add("System.Windows.Forms.dll");
			cp.ReferencedAssemblies.Add("System.Design.dll");
			cp.ReferencedAssemblies.Add("DevComponents.DotNetBar2.dll");

			//For each cdd file in the directory, if it's newer than the object file, we need
			//to compile it.
			foreach (string filename in Directory.GetFiles(Program.g_mainForm.CurrentWorkspace.CDDPath))
			{
				//Check filename formatting.  
				if (!filename.EndsWith(".cdd"))
					continue;

				string[] tokens = filename.Split('\\');
				string cddName = tokens[tokens.Length - 1];
				string objFilename = Application.StartupPath + "\\" + Path.GetFileName(filename) + ".dll";
				//Load it into the combo box
				cbDataDefinitions.Items.Add(cddName);
				

				//If the object file for this source file exists, let's check the time stamp to
				//determine if we need to re-compile it.
				if (!File.Exists(objFilename) ||
					(File.GetLastWriteTime(filename) > File.GetLastWriteTime(objFilename)))
				{
                    Program.g_mainForm.logMessage("Found custom data definition \"" + cddName + "\", compiling...", Color.Black);
                    Program.g_mainForm.setSplashStatus("Compiling Custom Data Definition: \"" + cddName + "\"");

					//Generate new C# source for the cdd
					string sourceCode = parseCdd(cddName,filename);
					if (sourceCode == null)
					{
						Program.g_mainForm.logMessage("Code generation failed.",Color.Red);
						continue;
					}

					cp.OutputAssembly = objFilename;
					CompilerResults cr = codeProvider.CompileAssemblyFromSource(cp,sourceCode);

					if (cr.Errors.HasErrors)
					{
						for (int i = 0;i < cr.Errors.Count;++i)
							Program.g_mainForm.logMessage(cr.Errors[i].ToString(),Color.Red);
						Program.g_mainForm.logMessage("Compilation failed.",Color.Red);
					}
					else
					{
						Program.g_mainForm.logMessage("Compilation succeeded.",Color.Green);
					}
				}	
			}

			//Now, for each compiled cdd assembly, attemp to load it.
			foreach (string filename in Directory.GetFiles(Application.StartupPath))
			{
				//Make sure it's at least ending with ".cdd.dll" 
				if (!filename.EndsWith("cdd.dll"))
					continue;
				string[] tokens = filename.Split('\\');
				string cddName = tokens[tokens.Length - 1];

				Program.g_mainForm.logMessage("Found custom data definition assembly \"" + cddName + "\", loading...",Color.Black);
                Program.g_mainForm.setSplashStatus("Loading Custom Data Definition: \"" + cddName + "\"");

				Assembly cddAssembly = Assembly.LoadFile(filename);
				if (cddAssembly == null)
				{
					Program.g_mainForm.logMessage("Could not load assembly for " + cddName,Color.Red);
					continue;
				}

				Type cddDescriptorType = null;
				string imagePath = null;
				foreach (Type type in cddAssembly.GetTypes())
				{
					if (type.IsAbstract) continue;
					if (type.IsDefined(typeof(CDDDescriptor),true))
					{
						//We've found the CDD descriptor type
						cddDescriptorType = type;
					}
					if (type.IsDefined(typeof(IconPath),true))
					{
						object[] o = type.GetCustomAttributes(typeof(IconPath),true);
						imagePath = ((IconPath)o[0]).m_path;
					}
				}

				if (cddDescriptorType == null)
				{
					Program.g_mainForm.logMessage("Could not find cdd descriptor attribute for " + cddName,Color.Red);
					continue;
				}

				//Now, create a new button
				string name = cddName.Remove(cddName.Length - 8);
				ButtonItem newButton = new ButtonItem();

				//Load image for button and add to main icon list
				try
				{
					if (imagePath != "")
					{
						Bitmap image = new Bitmap(imagePath);
						Program.g_mainForm.Icons.Images.Add(cddName,image);
						newButton.Image = image;
						Program.g_mainForm.Icons.Images.Add(cddName,image);
					}
					else
						newButton.Image = Program.g_mainForm.Icons.Images["Unknown"];
				}
				catch (Exception)
				{
					Program.g_mainForm.logMessage("Could not load image " + imagePath,Color.Red);
					newButton.Image = Program.g_mainForm.Icons.Images["Unknown"];
				}

				//Validate the cdd name
				if (!Utility.validateLabel(name))
				{
					MessageBox.Show("The Custom Data name \"" + name + "\" is reserved.  Choose a different name to avoid conflicts.  This CDD will not be loaded.","CDD Name Conflict",MessageBoxButtons.OK,MessageBoxIcon.Warning);
					continue;
				}

				newButton.Name = cddName;
				newButton.Text = name;
				newButton.Click += new System.EventHandler(handleCreateInstance);
				Utility.addButtonSorted(createButton,newButton);
				//createButton.SubItems.Add(newButton);
				newButton.CanCustomize = false;

				//Add the cdd without the dll extension
				m_loadedCDDs.Add(name,cddAssembly);

				//Add the cdd plugin info
				Program.g_mainForm.PluginDialog.addPluginInfo(cddAssembly);
			}


			Program.g_mainForm.logMessage("Search complete.",Color.Black);

		}

		private void handleCreateInstance(object sender,EventArgs e)
		{
			//Get the button that was clicked. The button's text is the type of cdd
			//we want to create an instance of
			ButtonItem buttonClicked = (ButtonItem)sender;
			Assembly assembly = m_loadedCDDs[buttonClicked.Text] as Assembly;
			if (assembly != null)
			{
				foreach (Type type in assembly.GetTypes())
				{
					if (type.IsAbstract) continue;
					if (type.IsDefined(typeof(CDDDescriptor),true))
					{
						//We've found the CDD descriptor type, so create an instance and add
						//it to the actor explorer.  
						object[] parameters = new object[1];
						parameters[0] = buttonClicked.Text;
						SceneNode sceneNode = (SceneNode)assembly.CreateInstance(type.FullName,
									 false,BindingFlags.Default,null,parameters,null,null);

						//Add the node representation to the actor explorer
						TreeNode treeNode = new TreeNode();
						treeNode.Name = System.Guid.NewGuid().ToString();
						if (Program.g_mainForm.Icons.Images[buttonClicked.Name] != null)
							treeNode.ImageKey = treeNode.SelectedImageKey = treeNode.SelectedImageKey = buttonClicked.Name;

						else
							treeNode.ImageKey = treeNode.SelectedImageKey = "Unknown";
						treeNode.Text = buttonClicked.Text;
						Program.g_mainForm.ActiveSceneExplorer.addTree(treeNode,true);
						Program.g_mainForm.ActiveSceneExplorer.addNode(treeNode.Name,sceneNode);
						break;
					}
				}
			}
		}

		private string parseCdd(string cddName,string filename)
		{
			//Lets generate a C# class
			string compilableCDDName = cddName;
			compilableCDDName = compilableCDDName.Replace(' ','_');
			compilableCDDName = compilableCDDName.Replace('.','_');

			FileStream fs = File.Open(filename,FileMode.Open,FileAccess.Read);
			BinaryFormatter bf = new BinaryFormatter();

			string imagePath = (string)bf.Deserialize(fs);		


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
								"using XVPDesigner.UserControls;" +
								"namespace " + compilableCDDName +
								"{" +
								"   [Serializable]" +
								"   [CDDDescriptor]" +
								"	[IconPath(\"" + imagePath.Replace("\\","/") + "\")]" +
								"	public class " + compilableCDDName + " : SceneNode" +
								"	{" +
								"		public " + compilableCDDName + "(string name) : base(name)" +
								"		{ setDefaultValues();" +
								"		}";

			

			//Deserialize list view items
			int numItems = (int)bf.Deserialize(fs);
			
			//Create compilable names for variables
			string[] privateParamVarNames = new string[numItems];
			string[] compilableParamNames = new string[numItems];
			ListViewItem[] lvis = new ListViewItem[numItems];

			for (int i = 0;i < numItems;++i)
			{
				lvis[i] = bf.Deserialize(fs) as ListViewItem;

				//create compilable private and public names for each CDD parameter
				string name = lvis[i].SubItems[0].Text + lvis[i].SubItems[1].Text;
				compilableParamNames[i] = Program.g_mainForm.buildValidCodeString(name);
				privateParamVarNames[i] = "m_" + compilableParamNames[i];

				switch (lvis[i].SubItems[2].Text)
				{
					case "Decimal Number":
						sourceCode += "private float " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(lvis[i].SubItems[4].Text,lvis[i].SubItems[5].Text,lvis[i].SubItems[8].Text)) + "\")]" +
										"public float " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[4].Text != "")
							sourceCode += getMinValueCheckCodeFloat(lvis[i].SubItems[4].Text,privateParamVarNames[i]);
						if (lvis[i].SubItems[5].Text != "")
							sourceCode += getMaxValueCheckCodeFloat(lvis[i].SubItems[5].Text,privateParamVarNames[i]);
										
							sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "Decimal Number List":
						sourceCode += "private float[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(lvis[i].SubItems[4].Text,lvis[i].SubItems[5].Text,lvis[i].SubItems[8].Text)) + "\")]" +
										"public float[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						//Check number of list items first, THEN clamp values if necessary
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						if (lvis[i].SubItems[4].Text != "")
							sourceCode += getMinValueListCheckCodeFloat(lvis[i].SubItems[4].Text,privateParamVarNames[i]);
						if (lvis[i].SubItems[5].Text != "")
							sourceCode += getMaxValueListCheckCodeFloat(lvis[i].SubItems[5].Text,privateParamVarNames[i]);
						sourceCode += privateParamVarNames[i] + " = value; } }";
									
						break;
					case "Integer Number":
						sourceCode += "private int " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(lvis[i].SubItems[4].Text,lvis[i].SubItems[5].Text,lvis[i].SubItems[8].Text)) + "\")]" +
										"public int " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[4].Text != "")
							sourceCode += getMinValueCheckCode(lvis[i].SubItems[4].Text,privateParamVarNames[i]);
						if (lvis[i].SubItems[5].Text != "")
							sourceCode += getMaxValueCheckCode(lvis[i].SubItems[5].Text,privateParamVarNames[i]);

						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "Integer Number List":
						sourceCode += "private int[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(Utility.getDescriptionString(lvis[i].SubItems[4].Text,lvis[i].SubItems[5].Text,lvis[i].SubItems[8].Text)) + "\")]" +
										"public int[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						//Check number of list items first, THEN clamp values if necessary
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						if (lvis[i].SubItems[4].Text != "")
							sourceCode += getMinValueListCheckCode(lvis[i].SubItems[4].Text,privateParamVarNames[i]);
						if (lvis[i].SubItems[5].Text != "")
							sourceCode += getMaxValueListCheckCode(lvis[i].SubItems[5].Text,privateParamVarNames[i]);
						
						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "Boolean":
						sourceCode += "private bool " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public bool " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { " + privateParamVarNames[i] + " = value; }" +
										"}";
						break;
					case "Boolean List":
						sourceCode += "private bool[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public bool[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "String":
						sourceCode += "private string " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text)+ "\")]" +
										"public string " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { " + privateParamVarNames[i] + " = value; }" +
										"}";
						break;
					case "String List":
						sourceCode += "private string[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public string[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "File Path":
						sourceCode += "private string " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")," +
										"Editor(typeof(FileNameEditor),typeof(System.Drawing.Design.UITypeEditor))]" +
										"public string " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { " + privateParamVarNames[i] + " = value; }" +
										"}";
						break;
					case "File Path List":
						sourceCode += "private string[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public string[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
					case "RGB Color":
						sourceCode += "private Color " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public Color " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { " + privateParamVarNames[i] + " = value; }" +
										"}";
						break;
					case "RGB Color List":
						sourceCode += "private Color[] " + privateParamVarNames[i] + ";" +
										"[DisplayName(\"" + lvis[i].SubItems[1].Text + "\")," +
										"CategoryAttribute(\"" + lvis[i].SubItems[0].Text + "\")," +
										"DescriptionAttribute(\"" + Utility.genCompilableString(lvis[i].SubItems[8].Text) + "\")]" +
										"public Color[] " + compilableParamNames[i] +
										"{ get { return " + privateParamVarNames[i] + ";}" +
										" set { ";
						if (lvis[i].SubItems[6].Text != "")
							sourceCode += getMinItemCountCheckCode(lvis[i].SubItems[6].Text);
						if (lvis[i].SubItems[7].Text != "")
							sourceCode += getMaxItemCountCheckCode(lvis[i].SubItems[7].Text);
						sourceCode += privateParamVarNames[i] + " = value; } }";
						break;
				}
			}
			
			//Create the export method
			sourceCode += "override public void export(BinaryWriter bw)" +
						  "{ base.export(bw);";

			sourceCode += "bw.Write(" + numItems + ");";

			for (int i = 0;i < numItems;++i)
			{
				switch (lvis[i].SubItems[2].Text)
				{
					case "Decimal Number":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_Decimal);" +
									  "bw.Write((float)" + privateParamVarNames[i] + ");";
						break;
					case "Decimal Number List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_DecimalList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
											"bw.Write((float)" + privateParamVarNames[i] + "[x]);"; 
						break;
					case "Integer Number":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_Integer);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ");";
						break;
					case "Integer Number List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_IntegerList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
											"bw.Write((float)" + privateParamVarNames[i] + "[x]);"; 
						break;
					case "Boolean":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_Bool);" +
									  "bw.Write((bool)" + privateParamVarNames[i] + ");";
						break;
					case "Boolean List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_BoolList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
											"bw.Write((bool)" + privateParamVarNames[i] + "[x]);"; 
						break;
					case "String":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_String);" +
									  "Utility.writeCString(bw," + privateParamVarNames[i] + ");";
						break;
					case "String List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_StringList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
											"Utility.writeCString(bw," + privateParamVarNames[i] + "[x]);";
						break;
					case "File Path":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_File);" +
									  "Utility.writeCString(bw," + privateParamVarNames[i] + ");";
						break;
					case "File Path List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_FileList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
											"Utility.writeCString(bw," + privateParamVarNames[i] + "[x]);";
						break;
					case "RGB Color":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_RGBColor);" +
									  "bw.Write((float)" + privateParamVarNames[i] + ".R / 255.0f);" +
									  "bw.Write((float)" + privateParamVarNames[i] + ".G / 255.0f);" +
									  "bw.Write((float)" + privateParamVarNames[i] + ".B / 255.0f);";
						break;
					case "RGB Color List":
						sourceCode += "bw.Write((int)CustomDataBuilder.ParamType.CDDPT_RGBColorList);" +
									  "bw.Write((int)" + privateParamVarNames[i] + ".Length);" +
									  "for (int x = 0;x < " + privateParamVarNames[i] + ".Length;++x)" +
									  "{" +
										  "bw.Write((float)" + privateParamVarNames[i] + "[x].R / 255.0f);" +
										  "bw.Write((float)" + privateParamVarNames[i] + "[x].G / 255.0f);" +
										  "bw.Write((float)" + privateParamVarNames[i] + "[x].B / 255.0f);" +
									  "}";
						break;
				}
			}
				
			sourceCode += " }";

			//Create the setDefaultValues() method and deserializing event method
			sourceCode += "[OnDeserializingAttribute()]" +
							"internal void deserializeInit(StreamingContext context)" +
							"{setDefaultValues();}";

			sourceCode += "override public void setDefaultValues() {";
			string[] tokens;
			for (int i = 0;i < numItems;++i)
			{
				switch (lvis[i].SubItems[2].Text)
				{
					case "Decimal Number":
						sourceCode += privateParamVarNames[i] + " = " + lvis[i].SubItems[3].Text + "f;";
						break;
					case "Decimal Number List":
						sourceCode += privateParamVarNames[i] + " =  new float[] { ";
						tokens = lvis[i].SubItems[3].Text.Split(',');
						for (int valueNum = 0;valueNum < tokens.Length;++valueNum)
						{
							if (valueNum == tokens.Length-1)
								sourceCode += tokens[valueNum] + "f";
							else
								sourceCode += tokens[valueNum] + "f,";
						}
						sourceCode += "};";
						
						break;
					case "Integer Number":
						sourceCode += privateParamVarNames[i] + " = " + lvis[i].SubItems[3].Text + ";";
						break;
					case "Integer Number List": 
						sourceCode += privateParamVarNames[i] + " =  new int[] { ";
						tokens = lvis[i].SubItems[3].Text.Split(',');
						for (int valueNum = 0;valueNum < tokens.Length;++valueNum)
						{
							if (valueNum == tokens.Length - 1)
								sourceCode += tokens[valueNum];
							else
								sourceCode += tokens[valueNum] + ",";
						}
						sourceCode += "};";
						break;
					case "Boolean":
						sourceCode += privateParamVarNames[i] + " = " + lvis[i].SubItems[3].Text + ";";
						break;
					case "Boolean List":
						sourceCode += privateParamVarNames[i] + " =  new bool[] { ";
						tokens = lvis[i].SubItems[3].Text.Split(',');
						for (int valueNum = 0;valueNum < tokens.Length;++valueNum)
						{
							if (valueNum == tokens.Length - 1)
								sourceCode += tokens[valueNum];
							else
								sourceCode += tokens[valueNum] + ",";
						}
						sourceCode += "};";
						break;
					case "String":
						sourceCode += privateParamVarNames[i] + " = \"" + lvis[i].SubItems[3].Text.Trim() + "\";";
						break;
					case "String List":
						sourceCode += privateParamVarNames[i] + " =  new string[] { ";
						tokens = lvis[i].SubItems[3].Text.Split(',');
						for (int valueNum = 0;valueNum < tokens.Length;++valueNum)
						{
							if (valueNum == tokens.Length - 1)
								sourceCode += "\"" + tokens[valueNum].Trim() + "\"";
							else
								sourceCode += "\"" + tokens[valueNum].Trim() + "\",";
						}
						sourceCode += "};";
						break;
					case "File Path":
						sourceCode += privateParamVarNames[i] + " = \"\";";
						break;
					case "File Path List":
						sourceCode += privateParamVarNames[i] + " = new string[] { ";
						int numFilePaths = 0;
						if (lvis[i].SubItems[6].Text != "")
							numFilePaths = int.Parse(lvis[i].SubItems[6].Text);
						else if (lvis[i].SubItems[7].Text != "")
							numFilePaths = int.Parse(lvis[i].SubItems[7].Text);
						else
							numFilePaths = 1;
						for (int x = 0;x < numFilePaths;++x)
						{
							if (x == numFilePaths - 1)
								sourceCode += "\"\"";
							else
								sourceCode +=  "\"\",";
						}
						sourceCode += "};";
						break;
					case "RGB Color":
						sourceCode += privateParamVarNames[i] + "= Color.White;";
						break;
					case "RGB Color List":
						sourceCode += privateParamVarNames[i] + " = new Color[] { ";
						int numColors = 0;
						if (lvis[i].SubItems[6].Text != "")
							numColors = int.Parse(lvis[i].SubItems[6].Text);
						else if (lvis[i].SubItems[7].Text != "")
							numColors = int.Parse(lvis[i].SubItems[7].Text);
						else
							numColors = 1;
						for (int x = 0;x < numColors;++x)
						{
							if (x == numColors - 1)
								sourceCode += "Color.White";
							else
								sourceCode += "Color.White,";
						}
						sourceCode += "};";
						break;
				}
			}

			sourceCode += " base.setDefaultValues();}";
			
			sourceCode += " }" +
						"}";

			fs.Close();
			return sourceCode;
		}

		private string getMinItemCountCheckCode(string value)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode = "if (value.Length < " + value + "){ MessageBox.Show(\"The list must contain a minimum of " + value + " entry(s).\",\"Wrong Number of Elements\",MessageBoxButtons.OK,MessageBoxIcon.Error); return;}";
			return sourceCode;
		}

		private string getMaxItemCountCheckCode(string value)
		{
			string sourceCode = "";
			if (value != "")
				sourceCode += "if (value.Length > " + value + "){ MessageBox.Show(\"The list cannot contain more than " + value + " entry(s).\",\"Wrong Number of Elements\",MessageBoxButtons.OK,MessageBoxIcon.Error); return;}";
			return sourceCode;
		}

		private string getMinValueCheckCodeFloat(string value,string variableName)
		{
			/*string sourceCode = "";
			if (value != "")
				sourceCode = "if (value < " + value + "){ MessageBox.Show(\"The value entered is less than the minimum value of " + value + ".\",\"Wrong Value\",MessageBoxButtons.OK,MessageBoxIcon.Error); return;}";
			return sourceCode;*/
			string sourceCode = "";
			if (value != "")
				sourceCode = "if (value < " + value + "){" + variableName + " = " + value + "f; return;}";
			return sourceCode; 
			
		}

		private string getMaxValueCheckCodeFloat(string value,string variableName)
		{
			/*string sourceCode = "";
			if (value != "")
				sourceCode += "if (value > " + value + "){ MessageBox.Show(\"The value entered is more than the maximum value of " + value + ".\",\"Wrong Value\",MessageBoxButtons.OK,MessageBoxIcon.Error); return;}";
			return sourceCode;*/
			string sourceCode = "";
			if (value != "")
				sourceCode += "if (value > " + value + "){" + variableName + " = " + value + "f; return;}";
			return sourceCode;
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

		private string getMinValueListCheckCode(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
			{
				sourceCode += "for (int i = 0;i < value.Length;++i)";
				sourceCode += "if (value[i] < " + value + "){" + variableName + "[i] = " + value + "; return;}";
			}
			return sourceCode;
		}

		private string getMaxValueListCheckCode(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
			{
				sourceCode += "for (int i = 0;i < value.Length;++i)";
				sourceCode += "if (value[i] > " + value + "){" + variableName + "[i] = " + value + "; return;}";
			}
			return sourceCode;
		}

		private string getMinValueListCheckCodeFloat(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
			{
				sourceCode += "for (int i = 0;i < value.Length;++i)";
				sourceCode += "if (value[i] < " + value + "){" + variableName + "[i] = " + value + "f; return;}";
			}
			return sourceCode;
		}

		private string getMaxValueListCheckCodeFloat(string value,string variableName)
		{
			string sourceCode = "";
			if (value != "")
			{
				sourceCode += "for (int i = 0;i < value.Length;++i)";
				sourceCode += "if (value[i] > " + value + "){" + variableName + "[i] = " + value + "f; return;}";
			}
			return sourceCode;
		}

	
		private void cbDataType_SelectedIndexChanged(object sender,EventArgs e)
		{
			if (cbDataType.SelectedItem == null)
				return;

			ComboBox cb = sender as ComboBox;
			switch (cb.SelectedItem.ToString())
			{
				case "Decimal Number":
					lblDataTypeExamples.Text = "1.5 or -1.5";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = true;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = true;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "Decimal Number List":
					lblDataTypeExamples.Text = "1.5,2.0,3.0,-.07";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = true;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = true;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;
				case "Integer Number":
					lblDataTypeExamples.Text = "5 or -5";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = true;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = true;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "Integer Number List":
					lblDataTypeExamples.Text = "5,10,-1000,0";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = true;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = true;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;
				case "Boolean":
					lblDataTypeExamples.Text = "true or false";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "Boolean List":
					lblDataTypeExamples.Text = "true,true,false,true";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;
				case "String":
					lblDataTypeExamples.Text = "The apple is red";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "String List":
					lblDataTypeExamples.Text = "Steve,Bob,Martha";
					lblExamplesText.Visible = true;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = true;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;
				case "File Path":
					lblDataTypeExamples.Text = "";
					lblExamplesText.Visible = false;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = false;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "File Path List":
					lblDataTypeExamples.Text = "";
					lblExamplesText.Visible = false;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = false;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;

				case "RGB Color":
					lblDataTypeExamples.Text = "";
					lblExamplesText.Visible = false;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = false;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "";
					txtMinItemCount.Enabled = false;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = false;
					break;
				case "RGB Color List":
					lblDataTypeExamples.Text = "";
					lblExamplesText.Visible = false;
					txtDefaultValue.Text = "";
					txtDefaultValue.Enabled = false;
					txtMinValue.Text = "";
					txtMinValue.Enabled = false;
					txtMaxValue.Text = "";
					txtMaxValue.Enabled = false;
					txtMinItemCount.Text = "0";
					txtMinItemCount.Enabled = true;
					txtMaxItemCount.Text = "";
					txtMaxItemCount.Enabled = true;
					break;
			}
		}

		private void saveCDD(bool newCDD)
		{
			//There must be at least one data type otherwise the cdd is pointless
			if (lvDataTypes.Items.Count == 0)
			{
				MessageBox.Show("The data definition must contain at least one data type. Definition not saved.","No Data Types",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			//Depending on the editing mode, we are either creating a new cdd or
			//modifying an existing definition
			if (newCDD)
			{
				dlgSaveFileDialog.Title = "Save Custom Data Definition...";
				dlgSaveFileDialog.Filter = "Custom Data Definition Files (*.cdd)|*.cdd|All files (*.*)|*.*";
				dlgSaveFileDialog.FileName = "CustomData.cdd";
				dlgSaveFileDialog.InitialDirectory = m_cddPath;
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result == DialogResult.OK)
				{
					//Save cdd file
					FileStream fs = null;
					try
					{
						fs = File.Open(dlgSaveFileDialog.FileName,FileMode.Create);
					}
					catch (Exception exception)
					{
						MessageBox.Show("Cannot open \"" + dlgSaveFileDialog.FileName + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
						Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
						if (fs != null)
							fs.Close();
						return;
					}
					BinaryFormatter bf = new BinaryFormatter();

					//Serialize list view items
					bf.Serialize(fs,txtImage.Text);
					bf.Serialize(fs,lvDataTypes.Items.Count);
					for (int i = 0;i < lvDataTypes.Items.Count;++i)
						bf.Serialize(fs,lvDataTypes.Items[i]);

					fs.Close();

					//Add the cdd to the combo box
					cbDataDefinitions.Items.Add(Path.GetFileName(dlgSaveFileDialog.FileName));
					MessageBox.Show(Path.GetFileName(dlgSaveFileDialog.FileName) + " saved successfully.","Save Successful",MessageBoxButtons.OK,MessageBoxIcon.Information);
					m_dirty = false;
				}
			}
			else
			{
				//Save cdd file
				FileStream fs = null;
				try
				{
					fs = File.Open(m_cddPath + "\\" + cbDataDefinitions.SelectedItem.ToString(),FileMode.Create);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot open \"" + m_cddPath + "\\" + cbDataDefinitions.SelectedItem.ToString() + "\" for writing.  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					if (fs != null)
						fs.Close();
					return;
				}
				BinaryFormatter bf = new BinaryFormatter();

				//Serialize list view items
				bf.Serialize(fs,txtImage.Text);
				bf.Serialize(fs,lvDataTypes.Items.Count);
				for (int i = 0;i < lvDataTypes.Items.Count;++i)
					bf.Serialize(fs,lvDataTypes.Items[i]);

				fs.Close();
				MessageBox.Show(cbDataDefinitions.SelectedItem.ToString() + " saved successfully.","Save Successful",MessageBoxButtons.OK,MessageBoxIcon.Information);

				m_dirty = false;
			}
		}

		private void btnSave_Click(object sender,EventArgs e)
		{
			if (rbNew.Checked)
				saveCDD(true);
			else
				saveCDD(false);
		}

		private void btnAddDataType_Click(object sender,EventArgs e)
		{
			//Some data is essential
			if (txtDefaultValue.Enabled)
			{
				if (txtCategory.Text == "" ||
					txtDisplayName.Text == "" ||
					cbDataType.SelectedItem == null ||
					txtDefaultValue.Text == "")
				{
					MessageBox.Show("\"Category\", \"Name\", \"Data Type\" and \"Default Value\" fields are mandatory.  Correct the input fields and then re-try.","Missing Input",MessageBoxButtons.OK,MessageBoxIcon.Error);
					return;
				}
			}
			else
			{
				if (txtCategory.Text == "" ||
				txtDisplayName.Text == "" ||
				cbDataType.SelectedItem == null)
				{
					MessageBox.Show("\"Category\", \"Name\" and \"Data Type\" fields are mandatory.  Correct the input fields and then re-try.","Missing Input",MessageBoxButtons.OK,MessageBoxIcon.Error);
					return;
				}

			}

			//First, make sure there are no outstanding errors. If there are, let the user
			//know.  If not, continue the compile and save
			if (errorProvider.GetError(txtCategory) != "" ||
				errorProvider.GetError(txtDisplayName) != "" ||
				errorProvider.GetError(txtDefaultValue) != "" ||
				errorProvider.GetError(txtMinValue) != "" ||
				errorProvider.GetError(txtMaxValue) != "" ||
				errorProvider.GetError(txtMinItemCount) != "" ||
				errorProvider.GetError(txtMaxItemCount) != "" ||
				errorProvider.GetError(txtDescription) != "")
			{
				MessageBox.Show("Some of the input is incorrect.  Correct the input fields that show the red error icon and then re-try.","Incorrect Input",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return;
			}

			//If the name already exists in the list view then we are modifiying
			//the currently selected entry, otherwise create a new entry
			ListViewItem foundItem = null;
			foreach (ListViewItem lvi in lvDataTypes.Items)
			{
				if (txtCategory.Text == lvi.SubItems[0].Text &&
					txtDisplayName.Text == lvi.SubItems[1].Text)
				{
					foundItem = lvi;
					break;
				}

			}

			if (foundItem == null)
			{
				ListViewItem lvi = new ListViewItem(new string[]{txtCategory.Text,
															txtDisplayName.Text,
															cbDataType.SelectedItem.ToString(),
															txtDefaultValue.Text,
															txtMinValue.Text,
															txtMaxValue.Text,
															txtMinItemCount.Text,
															txtMaxItemCount.Text,
															txtDescription.Text});
				lvDataTypes.Items.Add(lvi);
				m_dirty = true;
			}
			else
			{
				foundItem.SubItems[2].Text = cbDataType.SelectedItem.ToString();
				foundItem.SubItems[3].Text = txtDefaultValue.Text;
				foundItem.SubItems[4].Text = txtMinValue.Text;
				foundItem.SubItems[5].Text = txtMaxValue.Text;
				foundItem.SubItems[6].Text = txtMinItemCount.Text;
				foundItem.SubItems[7].Text = txtMaxItemCount.Text;
				foundItem.SubItems[8].Text = txtDescription.Text;
				m_dirty = true;
			}
		}

		private void btnRemoveDataType_Click(object sender,EventArgs e)
		{
			//If no items selected
			if (lvDataTypes.SelectedItems.Count == 0)
				return;

			if (lvDataTypes.SelectedItems.Count != 0)
			{
				//Only if item count changes do we mark the definition dirty
				int itemCount = lvDataTypes.Items.Count;
				foreach (ListViewItem lvi in lvDataTypes.SelectedItems)
					lvDataTypes.Items.Remove(lvi);
				
				if (itemCount != lvDataTypes.Items.Count)
					m_dirty = true;
			}
		}

		private void lvDataTypes_SelectedIndexChanged(object sender,EventArgs e)
		{
			ListView lv = sender as ListView;
			if (lv.SelectedItems.Count != 1)
			{
				txtCategory.Text = "";
				txtDisplayName.Text = "";
				txtDefaultValue.Text = "";
				txtMinValue.Text = "";
				txtMaxValue.Text = "";
				txtMinItemCount.Text = "";
				txtMaxItemCount.Text = "";
				txtDescription.Text = "";
			}
			else
			{
				try
				{
					txtCategory.Text = lv.SelectedItems[0].SubItems[0].Text;
					txtDisplayName.Text = lv.SelectedItems[0].SubItems[1].Text;
					cbDataType.SelectedIndex = cbDataType.FindString(lv.SelectedItems[0].SubItems[2].Text);
					txtDefaultValue.Text = lv.SelectedItems[0].SubItems[3].Text;
					txtMinValue.Text = lv.SelectedItems[0].SubItems[4].Text;
					txtMaxValue.Text = lv.SelectedItems[0].SubItems[5].Text;
					txtMinItemCount.Text = lv.SelectedItems[0].SubItems[6].Text;
					txtMaxItemCount.Text = lv.SelectedItems[0].SubItems[7].Text;
					txtDescription.Text = lv.SelectedItems[0].SubItems[8].Text;
				}
				catch (ArgumentOutOfRangeException exception)
				{
					//Argument was out of range..which means the saved cdd probably
					//was missing some data.  Could be an old version of the cdd.
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				}
			}
		}

		private void loadCDD(string cddPath)
		{
			FileStream fs = null;

			//Load cdd file
			try
			{
				fs = File.Open(cddPath,FileMode.Open,FileAccess.Read);
				BinaryFormatter bf = new BinaryFormatter();

				lvDataTypes.Items.Clear();

				//Deserialize list view items
				string imagePath = (string)bf.Deserialize(fs);
				try
				{
					if (picImagePreview.Image != null)
					{
						picImagePreview.Image.Dispose();
						picImagePreview.Image = null;
					}
					if (imagePath != "")
					{
						Bitmap newImage = new Bitmap(imagePath);
						if (newImage.Width != 16 && newImage.Height != 16)
						{
							MessageBox.Show("Image width and height must be 16x16.","Image Size Not Valid",MessageBoxButtons.OK,MessageBoxIcon.Error);
							picImagePreview.Image = null;
							txtImage.Text = "";
						}
						else
						{
							picImagePreview.Image = new Bitmap(imagePath);
							txtImage.Text = imagePath;
						}
					}
					else
						txtImage.Text = "";
				}
				catch (Exception exception)
				{
					MessageBox.Show("Unable to load image from " + imagePath + "  See the message log for details.","Image Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				}

				int numItems = (int)bf.Deserialize(fs);
				for (int i = 0; i < numItems; ++i)
					lvDataTypes.Items.Add(bf.Deserialize(fs) as ListViewItem);
				if (fs != null)
					fs.Close();
			}
			catch (Exception exception)
			{
				MessageBox.Show("Unable to open Custom Data.  See the message log for details.","CDD Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				if (fs != null)
					fs.Close();
				return;
			}
		}

		private void rbModify_CheckedChanged(object sender,EventArgs e)
		{
			RadioButton rb = sender as RadioButton;
			if (rb.Checked)
			{
				btnDeleteAll.Enabled = true;
				btnDeleteDataDefinition.Enabled = true;
				cbDataDefinitions.Enabled = true;
			}
			else
			{
				btnDeleteAll.Enabled = false;
				btnDeleteDataDefinition.Enabled = false;
				cbDataDefinitions.Enabled = false;
			}
		}

		private void rbNew_CheckedChanged(object sender,EventArgs e)
		{
			RadioButton rb = sender as RadioButton;
			
			//If the new option was selected, reset the fields
			if (rbNew.Checked)
			{
				checkForUnsavedChanges(false);
				btnResetDataType_Click(null,null);
			}
			//Else the modify option is being selected. If a definition
			//is showing in the combo box, then bring up it's data.
			else
			{
				checkForUnsavedChanges(true);
				if (cbDataDefinitions.SelectedItem != null)
					cbDataDefinitions_SelectedIndexChanged(cbDataDefinitions,null);
				else
				{
					btnResetDataType_Click(null,null);
					lvDataTypes.Items.Clear();
				}
			}
		}

		private void btnResetDataType_Click(object sender,EventArgs e)
		{
			txtCategory.Text = "";
			txtDisplayName.Text = "";
			cbDataType.SelectedIndex = 0;
			txtDescription.Text = "";
			lblDataTypeExamples.Text = "+/- 1.5";
			lblExamplesText.Visible = true;
			txtDefaultValue.Text = "";
			txtDefaultValue.Enabled = true;
			txtMinValue.Text = "";
			txtMinValue.Enabled = true;
			txtMaxValue.Text = "";
			txtMaxValue.Enabled = true;
			txtMinItemCount.Text = "";
			txtMinItemCount.Enabled = false;
			txtMaxItemCount.Text = "";
			txtMaxItemCount.Enabled = false;
		}

		private void btnClearDataTypes_Click(object sender,EventArgs e)
		{
			if (lvDataTypes.Items.Count == 0)
				return;

			DialogResult dr = MessageBox.Show("Are you sure you want to remove all data types in the data definition?","Remove All",MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
			if (dr == DialogResult.Yes)
			{
				if (lvDataTypes.Items.Count != 0)
					m_dirty = true;
				lvDataTypes.Items.Clear();
			}
		}

		private void txtCategory_TextChanged(object sender,EventArgs e)
		{
			//Only accept characters and numbers
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			foreach (char c in tb.Text)
			{
				if (!char.IsLetterOrDigit(c) && !char.IsWhiteSpace(c))
				{
					errorProvider.SetError(tb,"Only letters and numbers are allowed.");
					return;
				}
			}
						
			errorProvider.SetError(tb,null);
		}

		private void txtDisplayName_TextChanged(object sender,EventArgs e)
		{
			//Only accept characters and numbers
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			foreach (char c in tb.Text)
			{
				if (!char.IsLetterOrDigit(c) && !char.IsWhiteSpace(c))
				{
					errorProvider.SetError(tb,"Only letters and numbers are allowed.");
					return;
				}
			}

			errorProvider.SetError(tb,null);
		}

		private void txtDefaultValue_TextChanged(object sender,EventArgs e)
		{
			//Depending on what data type is selected, validate the default value
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}
			
			if (!validateDataTypeFormat(tb,tb.Text))
				return;
			if (!validateDefaultValueWithMinMaxValues())
				return;
			if (!validateDefaultValueWithMinMaxItemCounts())
				return;
			
			errorProvider.SetError(tb,null);
			
		}

		private void txtMinValue_TextChanged(object sender,EventArgs e)
		{
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}
			if (!validateDataTypeFormat(tb,tb.Text))
				return;
			if (!validateMinMaxValues())
				return;
			validateDefaultValueWithMinMaxValues();
			errorProvider.SetError(tb,null);
		}

		private bool validateDataTypeFormat(Control control,string value)
		{
			if (cbDataType.SelectedItem == null)
				return true;
	
			float f;
			int i;
			string trimmedString;
			string[] tokens;
			switch (cbDataType.SelectedItem.ToString())
			{
				case "Decimal Number":
					trimmedString = value.Trim();
					if (trimmedString != "")
					{
						if (trimmedString.Split(',').Length > 1)
						{
							errorProvider.SetError(control,"Only one decimal number allowed.");
							return false;
						}
						else if (!float.TryParse(trimmedString,out f))
						{
							errorProvider.SetError(control,"Not a valid decimal number.");
							return false;
						}
					}
					break;
				case "Decimal Number List":
					tokens = value.Split(',');

					foreach (string s in tokens)
					{
						trimmedString = s.Trim();
						if (trimmedString == "")
							continue;
						if (!float.TryParse(trimmedString,out f))
						{
							errorProvider.SetError(control,"Not a valid decimal number list.  Decimal numbers must be seperated by commas and can be positive or negative.");
							return false;
						}
					}
					break;
				case "Integer Number":
					trimmedString = value.Trim();
					if (trimmedString != "")
					{
						if (trimmedString.Split(',').Length > 1)
						{
							errorProvider.SetError(control,"Only one integer number allowed.");
							return false;
						}
						else if (!int.TryParse(trimmedString,out i))
						{
							errorProvider.SetError(control,"Not a valid integer number. Must be a positve or negative whole number without a decimal point.");
							return false;
						}
					}
					break;
				case "Integer Number List":
					tokens = value.Split(',');

					foreach (string s in tokens)
					{
						trimmedString = s.Trim();
						if (trimmedString == "")
							continue;
						if (!int.TryParse(trimmedString,out i))
						{
							errorProvider.SetError(control,"Not a valid integer number list.  Integer numbers must be seperated by commas and can be positive or negative and cannot have a decimal point.");
							return false;
						}
					}
					break;
				case "Boolean":
					trimmedString = value.Trim();
					if (trimmedString != "")
					{
						if (trimmedString.Split(',').Length > 1)
						{
							errorProvider.SetError(control,"Only one boolean allowed.");
							return false;
						}
						else if (trimmedString.ToLower() != "true" && trimmedString.ToLower() != "false")
						{
							errorProvider.SetError(control,"Must be the word \"true\" or \"false\".");
							return false;
						}
					}
					break;
				case "Boolean List":
					tokens = value.Split(',');

					foreach (string s in tokens)
					{
						trimmedString = s.Trim();
						if (trimmedString == "")
							continue;
						if (trimmedString.ToLower() != "true" && trimmedString.ToLower() != "false")
						{
							errorProvider.SetError(control,"Not a valid boolean list.  Must be a list of the words \"true\" and \"false\" seperated by commas.");
							return false;
						}
					}
					break;
				case "String":
					break;
				case "String List":
					break;
				case "File Path":
					break;
				case "File Path List":
					break;
				case "RGB Color":
					break;
				case "RGB Color List":
					break;
			}
		
			return true;
		}

		private void txtMaxValue_TextChanged(object sender,EventArgs e)
		{
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}
			if (!validateDataTypeFormat(tb,tb.Text))
				return;
			if (!validateMinMaxValues())
				return;
			validateDefaultValueWithMinMaxValues();
			errorProvider.SetError(tb,null);
		}

		private void txtMinItemCount_TextChanged(object sender,EventArgs e)
		{
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}

			string trimmedString = tb.Text.Trim();
			int i;
			if (trimmedString != "")
			{
				if (!int.TryParse(trimmedString,out i))
				{
					errorProvider.SetError(tb,"Not a valid integer number. Must be a positve whole number.");
					return;
				}
				else if (i < 0)
				{
					errorProvider.SetError(tb,"Cannot be negative.");
					return;
				}

			}
			if (!validateMinMaxItemCounts())
				return;
			validateDefaultValueWithMinMaxItemCounts();
			errorProvider.SetError(tb,null);
		}

		private void txtMaxItemCount_TextChanged(object sender,EventArgs e)
		{
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}

			string trimmedString = tb.Text.Trim();
			int i;
			if (trimmedString != "")
			{
				if (!int.TryParse(trimmedString,out i))
				{
					errorProvider.SetError(tb,"Not a valid integer number. Must be a positve whole number.");
					return;
				}
				else if (i < 0)
				{
					errorProvider.SetError(tb,"Cannot be negative.");
					return;
				}
			}
			if (!validateMinMaxItemCounts())
				return;
			validateDefaultValueWithMinMaxItemCounts();
			errorProvider.SetError(tb,null);
		}

		private bool validateMinMaxValues()
		{
			if (cbDataType.SelectedItem == null)
			{
				errorProvider.SetError(txtMinValue,null);
				errorProvider.SetError(txtMaxValue,null);
				return true;
			}

			if ((txtMinValue.Text.Trim() == "") || (txtMaxValue.Text.Trim() == ""))
			{
				errorProvider.SetError(txtMinValue,null);
				errorProvider.SetError(txtMaxValue,null);
				return true;
			}
			switch (cbDataType.SelectedItem.ToString())
			{
				case "Decimal Number":
				case "Decimal Number List":
					if (float.Parse(txtMinValue.Text) > float.Parse(txtMaxValue.Text))
					{
						errorProvider.SetError(txtMinValue,"Cannot be greater than the max value.");
						errorProvider.SetError(txtMaxValue,"Cannot be less than the min value.");
						errorProvider.SetError(txtDefaultValue,null);
						return false;
					}
					break;
				case "Integer Number":
				case "Integer Number List":
					if (int.Parse(txtMinValue.Text) > int.Parse(txtMaxValue.Text))
					{
						errorProvider.SetError(txtMinValue,"Cannot be greater than the max value.");
						errorProvider.SetError(txtMaxValue,"Cannot be less than the min value.");
						errorProvider.SetError(txtDefaultValue,null);
						return false;
					}
					break;
			}
			errorProvider.SetError(txtMinValue,null);
			errorProvider.SetError(txtMaxValue,null);

			return true;
		}

		private bool validateMinMaxItemCounts()
		{
			if (cbDataType.SelectedItem == null)
			{
				errorProvider.SetError(txtMinItemCount,null);
				errorProvider.SetError(txtMaxItemCount,null);
				return true;
			}
			if ((txtMinItemCount.Text.Trim() == "") || (txtMaxItemCount.Text.Trim() == ""))
			{
				errorProvider.SetError(txtMinItemCount,null);
				errorProvider.SetError(txtMaxItemCount,null);
				return true;
			}
			if (int.Parse(txtMinItemCount.Text) == 0 && int.Parse(txtMaxItemCount.Text) == 0)
			{
				errorProvider.SetError(txtMinItemCount,"Min item count and max item count cannot both be 0.");
				errorProvider.SetError(txtMaxItemCount,"Min item count and max item count cannot both be 0.");
				return false;
			}

			if (int.Parse(txtMinItemCount.Text) > int.Parse(txtMaxItemCount.Text))
			{
				errorProvider.SetError(txtMinItemCount,"Cannot be greater than the max item count.");
				errorProvider.SetError(txtMaxItemCount,"Cannot be less than the min item count.");
				return false;
			}
			errorProvider.SetError(txtMinItemCount,null);
			errorProvider.SetError(txtMaxItemCount,null);
			return true;
		}

		private bool validateDefaultValueWithMinMaxValues()
		{
			if (cbDataType.SelectedItem == null)
			{
				errorProvider.SetError(txtDefaultValue,null);
				return true;
			}

			if (txtDefaultValue.Text.Trim() == "")
			{
				errorProvider.SetError(txtDefaultValue,null);
				return true;
			}

			string[] tokens;
			string trimmedString;
			switch (cbDataType.SelectedItem.ToString())
			{
				case "Decimal Number":
					if (txtMinValue.Text.Trim() != "")
					{
						if (float.Parse(txtDefaultValue.Text) < float.Parse(txtMinValue.Text))
						{
							errorProvider.SetError(txtDefaultValue,"Default value is not within the range specified by min and max values.");
							return false;
						}
					}
					if (txtMaxValue.Text.Trim() != "")
					{
						if (float.Parse(txtDefaultValue.Text) > float.Parse(txtMaxValue.Text))
						{
							errorProvider.SetError(txtDefaultValue,"Default value is not within the range specified by min and max values.");
							return false;
						}
					}
					break;
				case "Decimal Number List":
					tokens = txtDefaultValue.Text.Trim().Split(',');

					foreach (string s in tokens)
					{
						trimmedString = s.Trim();
						if (trimmedString == "")
							continue;
						if (txtMaxValue.Text.Trim() != "")
						{
							if (float.Parse(trimmedString) > float.Parse(txtMaxValue.Text))
							{
								errorProvider.SetError(txtDefaultValue,"One or more of the values in the default value list is not within the range specified by min and max values.");
								return false;
							}
						}
						if (txtMinValue.Text.Trim() != "")
						{
							if (float.Parse(trimmedString) < float.Parse(txtMinValue.Text))
							{
								errorProvider.SetError(txtDefaultValue,"One or more of the values in the default value list is not within the range specified by min and max values.");
								return false;
							}
						}
					}
					break;
				case "Integer Number":
					if (txtMinValue.Text.Trim() != "")
					{
						if (int.Parse(txtDefaultValue.Text) < int.Parse(txtMinValue.Text))
						{
							errorProvider.SetError(txtDefaultValue,"Default value is not within the range specified by min and max values.");
							return false;
						}
					}
					if (txtMaxValue.Text.Trim() != "")
					{
						if (int.Parse(txtDefaultValue.Text) > int.Parse(txtMaxValue.Text))
						{
							errorProvider.SetError(txtDefaultValue,"Default value is not within the range specified by min and max values.");
							return false;
						}
					}
					break;
				case "Integer Number List":
					tokens = txtDefaultValue.Text.Trim().Split(',');

					foreach (string s in tokens)
					{
						trimmedString = s.Trim();
						if (trimmedString == "")
							continue;
						if (txtMaxValue.Text.Trim() != "")
						{
							if (int.Parse(trimmedString) > int.Parse(txtMaxValue.Text))
							{
								errorProvider.SetError(txtDefaultValue,"One or more of the values in the default value list is not within the range specified by min and max values.");
								return false;
							}
						}
						if (txtMinValue.Text.Trim() != "")
						{
							if (int.Parse(trimmedString) < int.Parse(txtMinValue.Text))
							{
								errorProvider.SetError(txtDefaultValue,"One or more of the values in the default value list is not within the range specified by min and max values.");
								return false;
							}
						}
					}
					break;
			}
			errorProvider.SetError(txtDefaultValue,null);

			return true;
		}

		private bool validateDefaultValueWithMinMaxItemCounts()
		{
			if (cbDataType.SelectedItem == null)
			{
				errorProvider.SetError(txtDefaultValue,null);
				return true;
			}

			if (txtDefaultValue.Text.Trim() == "")
			{
				errorProvider.SetError(txtDefaultValue,null);
				return true;
			}

			string[] tokens;
			string trimmedString;
			tokens = txtDefaultValue.Text.Trim().Split(',');

			foreach (string s in tokens)
			{
				trimmedString = s.Trim();
				if (trimmedString == "")
					continue;
				if (txtMaxItemCount.Text.Trim() != "")
				{
					if (tokens.Length > int.Parse(txtMaxItemCount.Text))
					{
						errorProvider.SetError(txtDefaultValue,"Default value list exceeds max item count.");
						return false;
					}
				}
				if (txtMinItemCount.Text.Trim() != "")
				{
					if (tokens.Length < int.Parse(txtMinItemCount.Text))
					{
						errorProvider.SetError(txtDefaultValue,"Default value list does not meet the minimum item count.");
						return false;
					}
				}
			}

			errorProvider.SetError(txtDefaultValue,null);

			return true;
		}

		private void checkForUnsavedChanges(bool saveNewCDD)
		{
			//If we have unsaved changes, ask use if they want to save
			if (m_dirty)
			{
				DialogResult dr = MessageBox.Show("Changes have been made to the current data definition.  Save changes before switching to the new data definition?","Unsaved Changes",MessageBoxButtons.YesNo,MessageBoxIcon.Question);
				if (dr == DialogResult.No)
					m_dirty = false;
				else
					saveCDD(saveNewCDD);
				m_dirty = false;
			}
		}

		private void cbDataDefinitions_SelectedIndexChanged(object sender,EventArgs e)
		{
			ComboBox cb = sender as ComboBox;
			if (cb.SelectedItem == null)
				return;

			//File should exist if it's in the combo box, but check anyway
			if (File.Exists(m_cddPath + "\\" + cb.SelectedItem.ToString()))
			{
				loadCDD(m_cddPath + "\\" + cb.SelectedItem.ToString());
				txtCategory.Text = "";
				txtDisplayName.Text = "";
				txtDefaultValue.Text = "";
				txtMinValue.Text = "";
				txtMaxValue.Text = "";
				txtMinItemCount.Text = "";
				txtMaxItemCount.Text = "";
				txtDescription.Text = "";
			}
			else
			{
				DialogResult dr = MessageBox.Show("Could not find the file for \"" + cbDataDefinitions.SelectedItem.ToString() + "\".","Missing File",MessageBoxButtons.OK,MessageBoxIcon.Error);
				cbDataDefinitions.Items.Remove(cbDataDefinitions.SelectedItem);
			}
		}

		private void btnDeleteDataDefinition_Click(object sender,EventArgs e)
		{
			if (cbDataDefinitions.SelectedItem == null)
				return;

			//Build path, delete both cdd and compiled assembly
			string path = m_cddPath + "\\" + cbDataDefinitions.SelectedItem.ToString();
			if (File.Exists(path))
			{
				DialogResult dr = MessageBox.Show("Are you sure you want to delete the data definition \"" + cbDataDefinitions.SelectedItem.ToString() + "\"?","Delete Data Definition",MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
				if (dr == DialogResult.Yes)
				{
					File.Delete(path);
					cbDataDefinitions.Items.Remove(cbDataDefinitions.SelectedItem);
					btnResetDataType_Click(null,null);
				}
			}
			else
			{
				DialogResult dr = MessageBox.Show("Could not find the file for \"" + cbDataDefinitions.SelectedItem.ToString() + "\".","Missing File");
				cbDataDefinitions.Items.Remove(cbDataDefinitions.SelectedItem);
			}
		}

		private void btnDeleteAll_Click(object sender,EventArgs e)
		{
			if (cbDataDefinitions.Items.Count == 0)
				return;

			DialogResult dr = MessageBox.Show("Are you sure you want to delete all data definitions?","Delete All Data Definitions",MessageBoxButtons.YesNo,MessageBoxIcon.Warning);
			if (dr == DialogResult.No)
				return;

			foreach (object o in cbDataDefinitions.Items)
			{
				//Build path, delete both cdd and compiled assembly
				string path = m_cddPath + "\\" + o.ToString();
				if (File.Exists(path))
				{
					File.Delete(path);
				}
			}

			cbDataDefinitions.Items.Clear();
		}

		private void fileSystemWatcher_Created(object sender,FileSystemEventArgs e)
		{
			//refreshCDDs();
		}

		private void fileSystemWatcher_Deleted(object sender,FileSystemEventArgs e)
		{
			//refreshCDDs();
		}

		private void fileSystemWatcher_Renamed(object sender,RenamedEventArgs e)
		{
			//refreshCDDs();
		}

		private void txtDescription_TextChanged(object sender,EventArgs e)
		{
			//Only certain symbols are allowed.  We want to exclude escape
			//sequences and other special compiler symbols
			TextBox tb = ((TextBox)sender);
			errorProvider.SetIconPadding(tb,5);

			if (!tb.Enabled)
			{
				errorProvider.SetError(tb,null);
				return;
			}

			/*if (!Utility.validateCString(tb.Text))
			{
				errorProvider.SetError(tb,"At least one character in the string is not allowed in the description");
				return;
			}*/
			
			errorProvider.SetError(tb,null);
		}

		private void cbDataDefinitions_SelectedValueChanged(object sender,EventArgs e)
		{
			
		}

		private void btnBrowseForImage_Click(object sender,EventArgs e)
		{
			dlgFileOpen.Title = "Specify Image (16x16)...";
			dlgFileOpen.Filter = "All files (*.*)|*.*";
			dlgFileOpen.FileName = "";
			dlgFileOpen.InitialDirectory = Application.StartupPath;
			DialogResult result = dlgFileOpen.ShowDialog();
			if (result == DialogResult.OK)
			{
				//Try to load image for preview
				try
				{
					Bitmap newImage = new Bitmap(dlgFileOpen.FileName);
					if (newImage.Width != 16 && newImage.Height != 16)
					{
						MessageBox.Show("Image width and height must be 16x16.","Image Size Not Valid",MessageBoxButtons.OK,MessageBoxIcon.Error);
						return;
					}
					else
					{
						if (picImagePreview.Image != null)
						{
							picImagePreview.Image.Dispose();
							picImagePreview.Image = null;
						}
						picImagePreview.Image = newImage;
						m_dirty = true;
					}
				}
				catch (Exception exception)
				{
					MessageBox.Show("Unable to load image.  See the message log for details.","Image Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					return;
				}

				txtImage.Text = dlgFileOpen.FileName;
			}
		}
	}
}
