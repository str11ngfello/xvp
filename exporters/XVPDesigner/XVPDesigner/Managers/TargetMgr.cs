using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Drawing;

namespace XVPDesigner.Managers
{
	public class TargetMgr
	{
		private string m_device = "";
		private string m_IPAddress = "";
		private string m_port = "";
		private Socket m_socket;

		//***************************************************************************
		//Properties
		//***************************************************************************
		public bool connected
		{
			get { return m_socket.Connected; }
		}

		public string ipAddress
		{
			get { return m_IPAddress; }
			set { m_IPAddress = value; }
		}

		public string port
		{
			get { return m_port; }
			set { m_port = value; }
		}

		public string device
		{
			get { return m_device; }
			set { m_device = value; }
		}


		//***************************************************************************
		//Methods
		//***************************************************************************
		public bool connect()
		{
			//Check parameters
			if (m_device == "" || m_IPAddress == "" || m_port == "")
			{
				MessageBox.Show("Network settings have not been fully specified.  Specify IP Address, Port #, etc, then re-connect.","Invalid Network Settings",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return false;
			}	

			//Create socket and try to connect
			m_socket = new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);

			IPAddress hostadd;
			IPEndPoint EPhost;
			try
			{
				hostadd = IPAddress.Parse(m_IPAddress);
				int portnum = Int16.Parse(m_port);
				EPhost = new IPEndPoint(hostadd,portnum);
			}
			catch (Exception)
			{
				MessageBox.Show("Target could not be reached due to invalid network settings","Invalid Network Settings",MessageBoxButtons.OK,MessageBoxIcon.Error);
				return false;
			}

			try
			{
				m_socket.Connect(EPhost);
			}
			catch (Exception)
			{
				string message = "Could not connect to " + m_device + " at " +m_IPAddress + ":" + m_port + ".  Verify that the target is connected to the network.";
				MessageBox.Show(message,"Connection Attempt Failed",MessageBoxButtons.OK,MessageBoxIcon.Exclamation);
				return false;
			}

			//Connection succeeded
			return true;
		}


		public void disconnect()
		{
			if (m_socket != null && m_socket.Connected)
				m_socket.Disconnect(true);
		}


		public bool sendCommand(string cmdString)
		{
			//If we aren't connected we can't send the string
			if (m_socket != null && m_socket.Connected)
			{
				//Convert to ascii bytes and send the string to the target
				Byte[] commandBuf = ASCIIEncoding.ASCII.GetBytes(cmdString);

				try
				{
					m_socket.Send(commandBuf);
				}
				catch (Exception e)
				{
					Program.g_mainForm.logMessage(e.ToString(),Color.Red);
					Program.g_mainForm.logMessage("Closing network connection.",Color.Red);
					m_socket = null;
				}
				return true;
			}
			else
				return false;
		}

	}






}
