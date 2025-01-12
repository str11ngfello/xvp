using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using IWshRuntimeLibrary;

namespace XVPSetupActions
{
	static class Program
	{
		public static void SetUserVariable(string name,string value,bool isRegExpandSz) 
		{ 
			SetVariable("HKEY_CURRENT_USER\\Environment\\" + name,value,isRegExpandSz);
		}
		
		public static void SetSystemVariable(string name,string value,bool isRegExpandSz) 
		{
			SetVariable("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\" + name,value,isRegExpandSz); }        private static void SetVariable(string fullpath,string value,bool isRegExpandSz) 
			{
				object objValue = value;
				object objType = (isRegExpandSz) ? "REG_EXPAND_SZ" : "REG_SZ";
				WshShell shell = new WshShell(); 
				shell.RegWrite(fullpath,ref objValue,ref objType); 
				int result; 
				SendMessageTimeout((System.IntPtr)HWND_BROADCAST,WM_SETTINGCHANGE,0,"Environment",SMTO_BLOCK | SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG,5000,out result); 
		}
		
		[DllImport("user32.dll",CharSet = CharSet.Auto,SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool SendMessageTimeout(IntPtr hWnd,int Msg,int wParam,string lParam,int fuFlags,int uTimeout,out int lpdwResult); 
		public const int HWND_BROADCAST = 0xffff; 
		public const int WM_SETTINGCHANGE = 0x001A; 
		public const int SMTO_NORMAL = 0x0000; 
		public const int SMTO_BLOCK = 0x0001; 
		public const int SMTO_ABORTIFHUNG = 0x0002; 
		public const int SMTO_NOTIMEOUTIFNOTHUNG = 0x0008;


		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			//Application.Run(new MainForm());

			try
			{
				string[] args = System.Environment.GetCommandLineArgs();
				SetSystemVariable("XVPSDK_DIR",args[1].Replace('\\','/').Trim('\"'),true);
			}
			catch(Exception)
			{
				MessageBox.Show("Unable to set XVPSDK_DIR environment variable.  As a result this variable needs to be set manually and be pointed to the installation root of the XVP","Failed to set Environment Variable",MessageBoxButtons.OK,MessageBoxIcon.Error);
			}
		}
	}
}