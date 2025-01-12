using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Windows.Forms;
using Microsoft.Win32;

namespace XVPDesigner
{
	[RunInstaller(true)]
	public partial class Installer1 : Installer
	{
		public Installer1()
		{
			InitializeComponent();
		}
	}
}