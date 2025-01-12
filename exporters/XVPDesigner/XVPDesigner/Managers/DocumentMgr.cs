using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using XVPDesigner.Documents;

namespace XVPDesigner.Managers
{
	public class DocumentMgr
	{
		private Hashtable m_documents = new Hashtable();
		public Hashtable Documents
		{
			get { return m_documents; }
		}

		public void init()
		{
		}

		public void addDocument(string docName,BaseDocument doc)
		{
			//See if the document is already open, if so we can't open it again.
			if (m_documents.Contains(docName))
				throw new Exception("Document " + docName + " already exists in the Document Manager.  Only one copy of a document can exist in the manager.");
			
			//Document is not already open so we can add it to the manager
			m_documents.Add(docName,doc);
		}

		public bool isDocumentOpen(string docName)
		{
			return m_documents.Contains(docName);
		}
	}
}
