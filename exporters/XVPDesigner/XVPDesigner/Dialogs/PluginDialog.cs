using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using XVPDesigner.Managers;

namespace XVPDesigner.Dialogs
{
	public partial class PluginDialog : Form
	{
		public PluginDialog()
		{
			InitializeComponent();
		}

		public void addPluginInfo(Assembly pluginAssembly)
		{
			//Find descriptors for the types we're interesting in
			foreach (Type type in pluginAssembly.GetTypes())
			{
				if (type.IsAbstract) continue;

				//Get the name and dexcription
				string description = null;
				AssemblyName assemblyName = pluginAssembly.GetName();
				object[] attributes = pluginAssembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute),false);
				if (attributes.Length > 0)
					description = ((AssemblyDescriptionAttribute)attributes[0]).Description;
				attributes = pluginAssembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute),false);

				//Create new list view item
				ListViewItem lvi = new ListViewItem(new string[]{type.ToString(),
																assemblyName.Version.ToString(),
																description,
																pluginAssembly.Location});

				//Insert the item into the proper list view based on the type of plugin
				if (type.IsDefined(typeof(SceneNodePluginDescriptor),true))
					lvSceneNodePlugins.Items.Add(lvi);
				else if (type.IsDefined(typeof(ControllerPluginDescriptor),true))
					lvControllerPlugins.Items.Add(lvi);
				else if (type.IsDefined(typeof(ShaderDescriptor),true))
					lvShaderPlugins.Items.Add(lvi);
				else if (type.IsDefined(typeof(PostEffectPluginDescriptor),true))
					lvPostEffectPlugins.Items.Add(lvi);
				else if (type.IsDefined(typeof(CDDDescriptor),true))
					lvCustomDataPlugins.Items.Add(lvi);
				else
					continue;
			}
		}
	}
}