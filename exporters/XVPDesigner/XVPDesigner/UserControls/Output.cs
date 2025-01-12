using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace XVPDesigner.UserControls
{
	public partial class Output : UserControl
	{
		private int m_commandStartIndex = 0;
		private string m_commandString = "";

		private class MessageInfo
		{
			public string message;
			public Color color;
		}
		private List<MessageInfo> m_messageQueue = new List<MessageInfo>(100);
	
		public Output()
		{
			InitializeComponent();
		}

		public void logMessage(string message,Color color)
		{
			lock (m_messageQueue)
			{
				MessageInfo mi = new MessageInfo();
				mi.message = message;
				mi.color = color;
				m_messageQueue.Add(mi);
			}
			/*lock (rtxtOutput)
			{
				rtxtOutput.AppendText(message);
				rtxtOutput.AppendText("\n");

				//Count line breaks to subtract during highlighting
				int numLineBreaks = 0;
				for (int i = 0; i < message.Length; ++i)
					if (message[i] == '\n')
						numLineBreaks++;

				//Highlight the text
				rtxtOutput.SuspendLayout();
				rtxtOutput.Select((rtxtOutput.Text.Length - 1) - message.Length + numLineBreaks,message.Length);
				rtxtOutput.SelectionColor = color;
				rtxtOutput.DeselectAll();
				rtxtOutput.ResumeLayout();
				rtxtOutput.Refresh();
			}*/
		}

		private void rtxtOutput_TextChanged(object sender,EventArgs e)
		{
			rtxtOutput.ScrollToCaret();
		}

		private void resetCommandStart()
		{
			m_commandStartIndex = rtxtConsole.Text.Length;
		}

		private void rtxtConsole_KeyPress(object sender,KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13)
			{
				if (rtxtConsole.Text[rtxtConsole.Text.Length - 1] != '>')
				{
					rtxtConsole.Select(m_commandStartIndex,rtxtConsole.Text.Length - m_commandStartIndex);
					m_commandString = rtxtConsole.SelectedText;
					Program.g_mainForm.sendTargetCommand(m_commandString);
					rtxtConsole.AppendText("X-VP>");
				}

				rtxtConsole.SuspendLayout();
				rtxtConsole.SelectAll();
				rtxtConsole.SelectionProtected = true;
				rtxtConsole.Select(rtxtConsole.Text.Length,0);
				rtxtConsole.DeselectAll();
				rtxtConsole.ResumeLayout();
				resetCommandStart();
			}
		}

		private void tsbSaveMessageLog_Click(object sender,EventArgs e)
		{
			dlgSaveFileDialog.Title = "Save Message Log...";
			dlgSaveFileDialog.Filter = "Rich Text Files (*.rtf)|*.rtf|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "MessageLog.rtf";
			if (dlgSaveFileDialog.InitialDirectory == "")
				dlgSaveFileDialog.InitialDirectory = Application.StartupPath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				rtxtOutput.SaveFile(dlgSaveFileDialog.FileName);
		}

		private void tsbClearMessageLog_Click(object sender,EventArgs e)
		{
			rtxtOutput.Clear();
		}

		private void tsbSaveConsole_Click(object sender,EventArgs e)
		{
			dlgSaveFileDialog.Title = "Save Console...";
			dlgSaveFileDialog.Filter = "Rich Text Files (*.rtf)|*.rtf|All files (*.*)|*.*";
			dlgSaveFileDialog.FileName = "Console.rtf";
			if (dlgSaveFileDialog.InitialDirectory == "")
				dlgSaveFileDialog.InitialDirectory = Application.StartupPath;
			DialogResult result = dlgSaveFileDialog.ShowDialog();
			if (result == DialogResult.OK)
				rtxtConsole.SaveFile(dlgSaveFileDialog.FileName);
		}

		private void tsbClearConsole_Click(object sender,EventArgs e)
		{
			rtxtConsole.Clear();
		}

		private void timerMessageQueue_Tick(object sender,EventArgs e)
		{
			lock (m_messageQueue)
			{
				if (m_messageQueue.Count == 0)
					return;

				foreach (MessageInfo mi in m_messageQueue)
				{
					rtxtOutput.AppendText(mi.message);
					rtxtOutput.AppendText("\n");

					//Count line breaks to subtract during highlighting
					int numLineBreaks = 0;
					for (int i = 0; i < mi.message.Length; ++i)
						if (mi.message[i] == '\n')
							numLineBreaks++;

					//Highlight the text
					rtxtOutput.SuspendLayout();
					rtxtOutput.Select((rtxtOutput.Text.Length - 1) - mi.message.Length + numLineBreaks,mi.message.Length);
					rtxtOutput.SelectionColor = mi.color;
					rtxtOutput.DeselectAll();
					rtxtOutput.ResumeLayout();
				}
				rtxtOutput.Refresh();
				m_messageQueue.Clear();
			}
		}
	}
}
