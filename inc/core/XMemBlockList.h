//*****************************************************************************
//*****************************************************************************
//     _  __         _    ______ 
//    | |/ /        | |  / / __ \
//    |   / ______  | | / / /_/ /
//   /   | /_____/  | |/ / ____/ 
//  /_/|_|          |___/_/       
//
//	Extensible Visualization Platform (X-VP) (c) 
//	Copyright 2000. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XMemBlockList.h
//	Author:			Lynn Duke
//	Creation Date:	7-12-06
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MEM_BLOCK_LIST_H__
#define __X_MEM_BLOCK_LIST_H__



//*****************************************************************************
//*****************************************************************************
template<typename T>
class XMemBlockList
{
private:
	
	XPointerContainer*				m_pHead;
	XPointerContainer*				m_pTail;
	XU32							m_numItems;
	XU32							m_peakUsage;
	
public:

	class iterator
	{
	public:
		enum XMarker
		{
			INVALID,
			NONE,
			AFTER_ALL,
			BEFORE_ALL
		};

		XMarker				m_marker;
		XPointerContainer*	m_pContainer;

	public:
		iterator()
		{
			m_pContainer = NULL;
			m_marker = INVALID;
		}

		iterator(XPointerContainer* p)
		{
			m_pContainer = p;
			m_marker = NONE;
		}

		inline void set(XPointerContainer* p)
		{
			m_pContainer = p;
			m_marker = NONE;
		}

		inline XBOOL isValid()
		{
			if (m_pContainer)
				return true;
			else
				return false;
		}

		inline void getNext()
		{
			m_pContainer = m_pContainer->pNext;
			if (!m_pContainer)
				m_marker = AFTER_ALL;
		}

		inline void getPrev()
		{
			m_pContainer = m_pContainer->pPrev;
			if (!m_pContainer)
				m_marker = BEFORE_ALL;
		}

		inline T& getValue()
		{
			X_ASSERTM(m_pContainer,"getValue() had no item to get");
			T* pReturnPointer = (T*)m_pContainer->pUserPointer;
		
			return *pReturnPointer;
		} 
		
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMemBlockList();
	~XMemBlockList();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XPointerContainer* begin();
	XPointerContainer* end();
	T& getFront();
	T& getBack();
	XStatus insertFront(const T& item);
	XStatus insertFront(T& item);
	XStatus insertBack(const T& item);
	XStatus insertBack(T& item);
	XStatus insert(iterator itr,const T& item);
	XStatus insert(iterator itr,T& item);
	XStatus removeFront();
	XStatus removeBack();
	XStatus remove(const T& item);
	XStatus remove(T& item);
	XBOOL contains(const T& item);
	XBOOL contains(T& item);
	XStatus clear();
	XU32 getNumItems();
	XU32 getPeakUsage();
	//XStatus reverse();
};



//*****************************************************************************
//*****************************************************************************
template<typename T>
XMemBlockList<T>::XMemBlockList()
{
	m_numItems = 0;
	m_peakUsage = 0;
	m_pHead = NULL;
	m_pTail = NULL;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XMemBlockList<T>::~XMemBlockList()
{
	//Do I really want this.  It causes a crash to alert me if some memory
	//wasn't freed in time...but it looks like a bug and is annoying.
	//clear();
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
inline XPointerContainer* XMemBlockList<T>::begin()
{
	return m_pHead;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
inline XPointerContainer* XMemBlockList<T>::end()
{
	return m_pTail;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XBOOL XMemBlockList<T>::contains(const T& item)
{
	return contains((T&)item);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XBOOL XMemBlockList<T>::contains(T& item)
{
	XPointerContainer* p = m_pHead;
	while (p)
	{
		if ((*(reinterpret_cast<T*>(p->pUserPointer))) == item)
			return true;
		p = p->pNext;

	}
	
	return false;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::clear()
{
	m_numItems = 0;
	XPointerContainer* p = m_pHead;
	XPointerContainer* pTemp;
	while (p)
	{
		pTemp = p;
		p = p->pNext;
		(*(reinterpret_cast<T*>(pTemp->pUserPointer))).~T();
		XOSMgr::returnMemBlock(sizeof(T),pTemp->pUserPointer);
		XOSMgr::returnMemBlock(sizeof(XPointerContainer),pTemp);
	}
	
	m_pHead = NULL;
	m_pTail = NULL;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
T& XMemBlockList<T>::getFront()
{
	X_ASSERTM(m_pTail,"getFront() found no item to return");
	return *((T*)m_pHead->pUserPointer);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
T& XMemBlockList<T>::getBack()
{
	X_ASSERTM(m_pTail,"getBack() found no item to return");
	return *((T*)m_pTail->pUserPointer);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insert(iterator itr,const T& item)
{
	return insert(itr,(T&)item);
}

	
	
//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insert(iterator itr,T& item)
{
	//First, are we pointed after or before all the items?
	if (itr.m_marker == iterator::INVALID)
		return X_ERR_NOT_READY;
	if (!itr.m_pContainer && itr.m_marker == iterator::AFTER_ALL)
		return insertBack(item);
	if (!itr.m_pContainer && itr.m_marker == iterator::BEFORE_ALL)
		return insertFront(item);

	//First grab a free pointer container
	//XPointerContainer* pNewContainer = XOSMgr::getFreePointerContainer();
	XPointerContainer* pNewContainer = (XPointerContainer*)XOSMgr::getFreeMemBlock(sizeof(XPointerContainer));
	pNewContainer = new(pNewContainer) XPointerContainer;
	void* pNewMemBlock = XOSMgr::getFreeMemBlock(sizeof(T));
	pNewMemBlock = new(pNewMemBlock) T;

	if (!pNewContainer || !pNewMemBlock)
		return X_ERR_LIMIT_REACHED;
	else
	{
		if (!itr.m_pContainer && itr.m_marker == iterator::NONE)
		{
			m_pHead = m_pTail = pNewContainer;
		}
		else
		{
			if (m_pHead == itr.m_pContainer)
			{
				m_pHead = pNewContainer;
				pNewContainer->pNext = itr.m_pContainer;
				pNewContainer->pPrev = itr.m_pContainer->pPrev;
				itr.m_pContainer->pPrev = pNewContainer;
			}
			else
			{
				pNewContainer->pNext = itr.m_pContainer;
				pNewContainer->pPrev = itr.m_pContainer->pPrev;
				itr.m_pContainer->pPrev->pNext = pNewContainer;
				itr.m_pContainer->pPrev = pNewContainer;
			}
		}

		pNewContainer->pUserPointer = pNewMemBlock;
		*(reinterpret_cast<T*>(pNewMemBlock)) = item;
		++m_numItems;
		if (m_numItems > m_peakUsage)
			m_peakUsage = m_numItems;
		return X_OK;
	}
}


//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insertFront(const T& item)
{
	return insertFront((T&)item);
}


	
//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insertFront(T& item)
{
	//First grab a free pointer container
	XPointerContainer* pNewContainer = (XPointerContainer*)XOSMgr::getFreeMemBlock(sizeof(XPointerContainer));
	pNewContainer = new(pNewContainer) XPointerContainer;
	void* pNewMemBlock = XOSMgr::getFreeMemBlock(sizeof(T));
	pNewMemBlock = new(pNewMemBlock) T;

	if (!pNewContainer || !pNewMemBlock)
		return X_ERR_LIMIT_REACHED;
	else
	{
		if (m_numItems == 0)
			m_pHead = m_pTail = pNewContainer;
		else
		{
			m_pHead->pPrev = pNewContainer;
			pNewContainer->pNext = m_pHead;
			m_pHead = pNewContainer;
		}
		pNewContainer->pUserPointer = pNewMemBlock;
		*(reinterpret_cast<T*>(pNewMemBlock)) = item;
		++m_numItems;
		if (m_numItems > m_peakUsage)
			m_peakUsage = m_numItems;
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insertBack(const T& item)
{
	return insertBack((T&)item);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::insertBack(T& item)
{
	//First grab a free pointer container
	XPointerContainer* pNewContainer = (XPointerContainer*)XOSMgr::getFreeMemBlock(sizeof(XPointerContainer));
	pNewContainer = new(pNewContainer) XPointerContainer;
	void* pNewMemBlock = XOSMgr::getFreeMemBlock(sizeof(T));
	pNewMemBlock = new(pNewMemBlock) T;

	if (!pNewContainer || !pNewMemBlock)
		return X_ERR_LIMIT_REACHED;
	else
	{
		if (m_numItems == 0)
			m_pHead = m_pTail = pNewContainer;
		else
		{
			m_pTail->pNext = pNewContainer;
			pNewContainer->pPrev = m_pTail;
			m_pTail = pNewContainer;
		}
		pNewContainer->pUserPointer = pNewMemBlock;
		*(reinterpret_cast<T*>(pNewMemBlock)) = item;
		++m_numItems;
		if (m_numItems > m_peakUsage)
			m_peakUsage = m_numItems;
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::removeFront()
{
	XPointerContainer* pTemp;
	if (m_numItems == 1)
	{
		(*(reinterpret_cast<T*>(m_pHead->pUserPointer))).~T();
		XOSMgr::returnMemBlock(sizeof(T),m_pHead->pUserPointer);
		XOSMgr::returnMemBlock(sizeof(XPointerContainer),m_pHead);
		m_pHead = NULL;
		m_pTail = NULL;
		--m_numItems;
		return X_OK;
	}
	else
	{
		pTemp = m_pHead;
		m_pHead = m_pHead->pNext;
		m_pHead->pPrev = NULL;
		(*(reinterpret_cast<T*>(pTemp->pUserPointer))).~T();
		XOSMgr::returnMemBlock(sizeof(T),pTemp->pUserPointer);
		XOSMgr::returnMemBlock(sizeof(XPointerContainer),pTemp);
		--m_numItems;
		return X_OK;
	}

	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::removeBack()
{
	XPointerContainer* pTemp;
	if (m_numItems == 1)
	{
		(*(reinterpret_cast<T*>(m_pTail->pUserPointer))).~T();
		XOSMgr::returnMemBlock(sizeof(T),m_pTail->pUserPointer);
		XOSMgr::returnMemBlock(sizeof(XPointerContainer),m_pTail);
		m_pHead = NULL;
		m_pTail = NULL;
		--m_numItems;
		return X_OK;
	}
	else
	{
		pTemp = m_pTail;
		m_pTail = m_pTail->pPrev;
		m_pTail->pNext = NULL;
		(*(reinterpret_cast<T*>(pTemp->pUserPointer))).~T();
		XOSMgr::returnMemBlock(sizeof(T),pTemp->pUserPointer);
		XOSMgr::returnMemBlock(sizeof(XPointerContainer),pTemp);
		--m_numItems;
		return X_OK;
	}

	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::remove(const T& item)
{
	return remove((T&)item);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockList<T>::remove(T& item)
{
	//Spin through the list looking for the item to remove. Remove all instances
	//of the item
	XPointerContainer* p = m_pHead;
	XPointerContainer* pTemp;
	while (p)
	{
		if (item == (*(T*)p->pUserPointer))
		{
			pTemp = p;
			p = p->pNext;

			if (pTemp->pPrev)
			{
				pTemp->pPrev->pNext = p;
				if (p)
					p->pPrev = pTemp->pPrev;
			}

			if (pTemp == m_pHead)
			{
				m_pHead = p;
				if (p)
					p->pPrev = NULL;
			}

			if (pTemp == m_pTail)
			{
				m_pTail = pTemp->pPrev;
			}
		
			(*(reinterpret_cast<T*>(pTemp->pUserPointer))).~T();
			XOSMgr::returnMemBlock(sizeof(T),pTemp->pUserPointer);
			XOSMgr::returnMemBlock(sizeof(XPointerContainer),pTemp);
			--m_numItems;
		}
		else
			p = p->pNext;
	}
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XMemBlockList<T>::getNumItems()
{
	return m_numItems;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XMemBlockList<T>::getPeakUsage()
{
	return m_peakUsage;
}



//*****************************************************************************
//*****************************************************************************
/*template<typename T>
XStatus XMemBlockList<T>::reverse()
{
	if (m_numItems > 0)
	{
		XPointerContainer* p = m_pHead;
		XPointerContainer* pTemp;
		while (p)
		{
			pTemp = p;
			p = p->pNext;
			XOSMgr::returnMemBlock(sizeof(T),pTemp->pUserPointer);
			XOSMgr::returnMemBlock(sizeof(XPointerContainer),pTemp);
		}
		
		m_pHead = NULL;
		m_pTail = NULL;
	}

	return X_OK;
}
*/
#endif