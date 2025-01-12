
using System;
using System.Reflection;

namespace XVPDesigner.TypeEditors
{
	[AttributeUsage(AttributeTargets.Property)]
	public class EditorRange : System.Attribute
	{
		private string m_minValue = "0";
		public string MinValue
		{
			get { return m_minValue; }
		}

		private string m_maxValue = "100";
		public string MaxValue
		{
			get { return m_maxValue; }
		}

		private string m_step = "1";
		public string Step
		{
			get { return m_step; }
		}

		public EditorRange(string minValue,string maxValue,string step)
		{
			m_minValue = minValue;
			m_maxValue = maxValue;
			m_step = step;
		}

		public EditorRange(string minValue,string maxValue)
		{
			m_minValue = minValue;
			m_maxValue = maxValue;
			m_step = "1";
		}
	}
}
