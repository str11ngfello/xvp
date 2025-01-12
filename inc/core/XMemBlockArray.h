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
//	Module Name:	XMemBlockArray.h
//	Author:			Lynn Duke
//	Creation Date:	7-16-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MEM_BLOCK_ARRAY_H__
#define __X_MEM_BLOCK_ARRAY_H__






//*****************************************************************************
//*****************************************************************************
template<typename T>
class XMemBlockArray
{
private:
	//XFreeList<T>	m_freeList;
	T**				m_pUsedList;
	XU32			m_numItems;
	XU32			m_maxCapacity;
	XBOOL			m_inited;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMemBlockArray();
	~XMemBlockArray();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	//XStatus init(XU32 maxCapacity,XS8* pName,XHeap* pHeap = NULL);
	//XStatus destroy();
	T* getBack();
	T* insertBack();
	T* insert(XU32 index);
	XStatus removeBack();
	XStatus remove(XU32 index);
	XStatus remove(T item);
	XStatus remove(T* pItem);
	XStatus clear();
	XU32 getNumItems();
	XU32 getCapacity();
	XU32 getPeakUsage();
	T* operator[] (XU32 index);
};



//*****************************************************************************
//*****************************************************************************
template<typename T>
XMemBlockArray<T>::XMemBlockArray()
{
	m_maxCapacity = 0;
	m_numItems = 0;
	m_pUsedList = NULL;
	m_inited = false;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XMemBlockArray<T>::~XMemBlockArray()
{
	destroy();
}



//*****************************************************************************
//*****************************************************************************
/*template<typename T>
XStatus XMemBlockArray<T>::init(XU32 maxCapacity,XS8* pName,XHeap* pHeap = NULL)
{
	destroy();

	X_ASSERTM(maxCapacity,"initial items cannot be 0");
	X_ASSERTM(!m_inited,"already initialized");
	
	//Allocate the free list and pointer list
	m_freeList.init(maxCapacity,pName,pHeap);
	XNewArray(m_pUsedList,T*,maxCpacity,XOSMgr::getDefaultHeap());// = new T*[maxCapacity];
	X_ASSERTM(m_pUsedList,"allocation failed");

	m_numItems = 0;
	m_maxCapacity = maxCapacity;
	m_inited = true;

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::destroy()
{
	//Return all used items
	clear();

	m_freeList.destroy();
	XDeleteArray(m_pUsedList);	
	m_inited = false;

	return X_OK;
}
*/


//*****************************************************************************
//*****************************************************************************
template<typename T>
T* XMemBlockArray<T>::getBack()
{
	if (m_numItems == 0)
		return NULL;
	else 
		return m_pUsedList[m_numItems-1];
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
T* XMemBlockArray<T>::insertBack()
{
	//Try to allocate free item
	T* pNewItem = m_freeList.allocateItem();
	if (!pNewItem)
	{
	/*	X_DEBUG
		(	
			XS8 buf[256];
			if (m_freeList.getName())
				sprintf(buf,"insert into XMemBlockArray \"%s\" failed because capacity has been exhausted",m_freeList.getName());
			else
				sprintf(buf,"insert into XMemBlockArray <noname> failed because capacity has been exhausted");

			X_WARNING(buf);
		)*/
		
		return NULL;
	}
	else
	{
		m_pUsedList[m_numItems++] = pNewItem;
		return pNewItem;
	}
}


//*****************************************************************************
//*****************************************************************************
template<typename T>
T* XMemBlockArray<T>::insert(XU32 index)
{
	XS32 intIndex = index;
	if ((((XS32)index)-1 < (XS32)m_numItems) && (m_numItems != m_maxCapacity))
	{
		//Try to allocate free item
		T* pNewItem = m_freeList.allocateItem();
		if (!pNewItem)
		{
			/*X_DEBUG
			(	
				XS8 buf[256];
				if (m_freeList.getName())
					sprintf(buf,"insert into XMemBlockArray \"%s\" failed because capacity has been exhausted",m_freeList.getName());
				else
					sprintf(buf,"insert into XMemBlockArray <noname> failed because capacity has been exhausted");

				X_WARNING(buf);
			)*/
			return NULL;
		}
		else
		{
			//Now shift all the items down one to create a new hole
			for (XU32 i = m_numItems;i > index;--i)
				m_pUsedList[i] = m_pUsedList[i-1];

			//Now fill the hole with the new item
			m_pUsedList[index] = pNewItem;
			++m_numItems;
			return pNewItem;
		}
	}
	else
		return NULL;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::removeBack()
{
	if (m_numItems)
		m_freeList.freeItem(m_pUsedList[--m_numItems]);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::remove(XU32 index)
{
	if (index < m_numItems)
	{
		//Remove the item
		m_freeList.freeItem(m_pUsedList[--m_numItems]);
		
		//Now shift all the remaining items to fill the hole
		for (XU32 i = index+1;i <= m_numItems;++i)
			m_pUsedList[i-1] = m_pUsedList[i];
		
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::remove(T item)
{
	//Remove all occurences of the item
	XBOOL lookAgain = false;
	do
	{
		lookAgain = false;
		for (XU32 i = 0;i < m_numItems;++i)
		{
			if (item == (*m_pUsedList[i]))
			{
				remove(i);
				lookAgain = true;
				break;
			}
		}
	}
	while (lookAgain);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::remove(T* pItem)
{
	for (XU32 i = 0;i < m_numItems;++i)
	{
		if (pItem == m_pUsedList[i])
		{
			remove(i);
			break;
		}
	}
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XMemBlockArray<T>::clear()
{
	//Return all used items
	for (XU32 i = 0;i < m_numItems;++i)
		m_freeList.freeItem(m_pUsedList[i]);
	m_numItems = 0;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XMemBlockArray<T>::getNumItems()
{
	return m_numItems;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XMemBlockArray<T>::getCapacity()
{
	return m_maxCapacity;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
T* XMemBlockArray<T>::operator[](XU32 index)
{
	if (index < m_numItems)
		return m_pUsedList[index];
	else
		return NULL;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XMemBlockArray<T>::getPeakUsage()
{
	return m_freeList.getPeakBlocksInUse();
}




#endif