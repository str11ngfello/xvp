using System;
using System.Collections.Generic;
using System.Text;
using ICSharpCode.TextEditor;
using DevComponents.DotNetBar;
using ICSharpCode.TextEditor.Document;
using System.Windows.Forms;
using System.IO;
using System.Drawing;

namespace XVPDesigner.Documents
{
	class ScriptDocument : BaseDocument
	{
		TextEditorControl m_textEditor;
		bool m_dirty = false;
		DevComponents.DotNetBar.TabItem m_tabItem;

		private string m_currentScriptPath = null;
		public string ScriptFilename
		{
			get { return m_currentScriptPath; }
		}

		public override bool create(TabItem tab,string name)
		{
			Dispose();

			m_textEditor = new TextEditorControl();
			FileSyntaxModeProvider fsmProvider;
			fsmProvider = new FileSyntaxModeProvider(Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Syntax Highlighting"); // Create new provider with the highlighting directory.
			HighlightingManager.Manager.AddSyntaxModeFileProvider(fsmProvider); // Attach to the text editor.
			m_textEditor.SetHighlighting("lua");
			m_textEditor.ShowEOLMarkers = false;
			m_textEditor.ShowVRuler = false;
			m_textEditor.ShowLineNumbers = false;
			m_textEditor.Dock = DockStyle.Fill;
			tab.AttachedControl.Controls.Add(m_textEditor);
			m_tabItem = tab;
			return true;
		}

		public override bool open(TabItem tab,string filename)
		{
			Dispose();
			m_tabItem = tab;

			try
			{
				m_textEditor = new TextEditorControl();
				FileSyntaxModeProvider fsmProvider;
				fsmProvider = new FileSyntaxModeProvider(Program.g_mainForm.XVPPath + "xvpsdk\\system\\designer\\Syntax Highlighting"); // Create new provider with the highlighting directory.
				HighlightingManager.Manager.AddSyntaxModeFileProvider(fsmProvider); // Attach to the text editor.
				m_textEditor.SetHighlighting("lua");
				m_textEditor.ShowEOLMarkers = false;
				m_textEditor.ShowVRuler = false;
				m_textEditor.ShowLineNumbers = false;
				m_textEditor.Dock = DockStyle.Fill;
				m_textEditor.LoadFile(filename);
				tab.AttachedControl.Controls.Add(m_textEditor);
				m_tabItem = tab;
			}
			catch (Exception exception)
			{
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				MessageBox.Show("Could not open script.  Check the message log for details.","Open Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return false;
			}

			m_dirty = false;
			m_currentScriptPath = filename;
			Program.g_mainForm.addRecentScript(filename);

			return true;
		}
		
		public override bool save()
		{
			if (m_currentScriptPath == null)
			{
				SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
				dlgSaveFileDialog.Title = "Save Script...";
				dlgSaveFileDialog.Filter = "Script Files (*.lua)|*.lua|All files (*.*)|*.*";
				dlgSaveFileDialog.FileName = "Script.lua";
				dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ScriptLibraryPath;
				DialogResult result = dlgSaveFileDialog.ShowDialog();
				if (result != DialogResult.OK)
					return false;

				//Check to make sure the document is not already open under that name 
				if (Program.g_mainForm.m_docMgr.isDocumentOpen(dlgSaveFileDialog.FileName))
				{
					MessageBox.Show("The document " + dlgSaveFileDialog.FileName + " is currently open.  This document cannot be saved using the same filename.","Document Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
					return false;
				}

				m_currentScriptPath = dlgSaveFileDialog.FileName;
				if (m_tabItem != null)
				{
					m_tabItem.Name = dlgSaveFileDialog.FileName;
					m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				}

				dlgSaveFileDialog.Dispose();
			}


			try
			{
				m_textEditor.SaveFile(m_currentScriptPath);
			}
			catch (Exception exception)
			{
				MessageBox.Show("Cannot save \"" + m_currentScriptPath + ".  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
				return false;
			}

			m_dirty = false;

			Program.g_mainForm.addRecentScript(m_currentScriptPath);

			return true;
		}

		public override bool saveAs()
		{
			SaveFileDialog dlgSaveFileDialog = new SaveFileDialog();
			dlgSaveFileDialog.Title = "Save Script As...";
			dlgSaveFileDialog.Filter = "Script Files (*.lua)|*.lua|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Script.lua";
			dlgSaveFileDialog.InitialDirectory = Program.g_mainForm.CurrentWorkspace.ScriptLibraryPath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();

			//Check to make sure the document is not already open under that name 
			if (Program.g_mainForm.m_docMgr.isDocumentOpen(dlgSaveFileDialog.FileName))
			{
				MessageBox.Show("The document " + dlgSaveFileDialog.FileName + " is currently open.  This document cannot be saved using the same filename.","Document Open",MessageBoxButtons.OK,MessageBoxIcon.Information);
				return false;
			}

			if (result == DialogResult.OK)
			{
				try
				{
					m_textEditor.SaveFile(dlgSaveFileDialog.FileName);
				}
				catch (Exception exception)
				{
					MessageBox.Show("Cannot save \"" + dlgSaveFileDialog.FileName + "\".  Ensure the file is not marked read-only or in use by another application.  See message log for details.","File Write Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
					Program.g_mainForm.logMessage(exception.ToString(),Color.Red);
					dlgSaveFileDialog.Dispose();
					return false;
				}
				m_dirty = false;
				m_currentScriptPath = dlgSaveFileDialog.FileName;
				if (m_tabItem != null)
				{
					m_tabItem.Name = dlgSaveFileDialog.FileName;
					m_tabItem.Text = Path.GetFileName(dlgSaveFileDialog.FileName);
				}
				dlgSaveFileDialog.Dispose();

				Program.g_mainForm.addRecentScript(m_currentScriptPath);

				return true;
			}

			return false;
		}

		public override bool close()
		{
			if (m_dirty)
			{
				//Show document
				m_tabItem.Parent.SelectedTab = m_tabItem;

				bool success = true;
				DialogResult dr = MessageBox.Show(m_tabItem.Text + " has been modified.  Would you like to save?","Script Changed",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
				switch (dr)
				{
					case DialogResult.Yes:
						success = save();
						break;
					case DialogResult.Cancel:
						return false;
				}
				if (!success)
					return false;
			}

			Dispose();
			m_currentScriptPath = null;

			base.close();
			return true;
		}

		public void Dispose()
		{
			if (m_textEditor != null)
				m_textEditor.Dispose();
		}
	}
}
