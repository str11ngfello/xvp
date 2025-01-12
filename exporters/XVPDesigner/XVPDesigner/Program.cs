using System;
using System.Collections.Generic;
using System.Windows.Forms;
using XVPDesigner.Managers;

namespace XVPDesigner
{
    static public class Program
    {
		//Global instance of main form and other objects
		static public MainForm g_mainForm = null;

		/// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
			g_mainForm = new MainForm();
            Application.Run(g_mainForm);
      
        }
    }
}