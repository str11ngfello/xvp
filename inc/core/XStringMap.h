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
//	Module Name:	XStringMap.h
//	Author:			Lynn Duke
//	Creation Date:	9-10-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_STRING_MAP_H__
#define __X_STRING_MAP_H__

#include <XOSMgr.h>
#include <XMemBlockList.h>


//*****************************************************************************
//*****************************************************************************
class XHasher
{
public:
	virtual XU32 hash(XS8* pBytes,XU32 numBytes) = 0;
};



//*****************************************************************************
//*****************************************************************************
class XK33Hash : public XHasher
{
public:
	XU32 hash(XS8* pBytes,XU32 numBytes)
	{
		XU32 hash = 5381;
		for (XU32 i = 0;i < numBytes;++i)
			hash = ((hash << 5) + hash) + pBytes[i]; /* hash * 33 + byte */
		return hash;
	}
};



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
class XStringMap
{
public:

	class KeyValuePair
	{
	private:
		XS8* m_pStringKey;
		valueType m_value;

	public:
		KeyValuePair()
		{
			m_pStringKey = NULL;
		}

		KeyValuePair(XS8* pStringKey,valueType& value)
		{
			m_pStringKey = XOSMgr::createString(pStringKey);
			m_value = value;
		}

		~KeyValuePair()
		{
			if (m_pStringKey)
				XOSMgr::destroyString(&m_pStringKey);
		}

		XS8* getStringKey()
		{
			return m_pStringKey;
		}

		valueType* getValue()
		{
			return &m_value;
		}

		bool operator == (KeyValuePair& kvp)
		{
			if (m_pStringKey && kvp.getStringKey())
			{
				if ((strcmp(m_pStringKey,kvp.getStringKey()) == 0) && (m_value == *kvp.getValue()))
					return true;
			}
			else
			{
				if ((m_pStringKey == kvp.getStringKey()) && (m_value == *kvp.getValue()))
					return true;
			}
			
			return false;
		}

		KeyValuePair& operator = (KeyValuePair& kvp)
		{
			if (m_pStringKey)
				XOSMgr::destroyString(&m_pStringKey);
			m_pStringKey = XOSMgr::createString(kvp.getStringKey());
			m_value = *kvp.getValue();
			return *this;
		}

	};

	class iterator
	{
	public:
		typename XMemBlockList<KeyValuePair>::iterator i;
		XU32 startBucketIndex;
		XU32 currentBucketIndex;
		XU32 endBucketIndex;
		XStringMap* pMap;
		
	public:
		iterator()
		{
			startBucketIndex = 0;
			currentBucketIndex = 0;
			endBucketIndex = 0;
			pStringMap = NULL;
		}

		iterator(XStringMap* pStringMap)
		{
			X_ASSERTM(pStringMap,"Invalid StringMap pointer");
			startBucketIndex = 0;
			currentBucketIndex = 0;
			endBucketIndex = pStringMap->getNumBuckets()-1;
			i.set(pStringMap->getBucket(0)->begin());
			pMap = pStringMap;
		}

		inline void set(XStringMap* pStringMap)
		{
			X_ASSERTM(pStringMap,"Invalid StringMap pointer");
			startBucketIndex = 0;
			currentBucketIndex = 0;
			endBucketIndex = pStringMap->getNumBuckets()-1;
			i.set(pStringMap->getBucket(0)->begin());
			pMap = pStringMap;
		}

		inline XBOOL isValid()
		{
			//If we have more items in the current bucket return true
			if (i.isValid())
				return true;
			//else we need to move to the next bucket
			else
			{
				while (currentBucketIndex < endBucketIndex)
				{
					++currentBucketIndex;
					i.set(pMap->getBucket(currentBucketIndex)->begin());
					if (i.isValid())
						return true;
				}
				return false;
			}
		}

		inline void getNext()
		{
			i.getNext();
		}

		inline KeyValuePair& getKVPair()
		{
			return i.getValue(); 			
		} 
	};

	XK33Hash						m_defaultHasher;
	XHasher*						m_pHasher;
	XU32							m_numItems;
	XU32							m_peakUsage;
	XU32							m_numBuckets;
	XMemBlockList<KeyValuePair>*	m_pBuckets;

	
public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XStringMap();
	~XStringMap();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XU32 numBuckets,XHasher* pHasher = NULL);
	XStatus insert(XS8* pStringKey,valueType value);
	XStatus remove(XS8* pStringKey);
	XStatus clear();
	XStatus destroy();
	XU32 getNumItems();
	XU32 getPeakUsage();
	XU32 getNumBuckets();
	XMemBlockList<KeyValuePair>* getBucket(XU32 index);
	valueType* getItem(XS8* pStringKey);
	XF32 getBucketUtilization();
};




//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStringMap<valueType>::XStringMap()
{
	m_numItems = 0;
	m_peakUsage = 0;
	m_numBuckets = 0;
	m_pBuckets = 0;
	m_pHasher = NULL;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStringMap<valueType>::~XStringMap()
{
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStatus XStringMap<valueType>::clear()
{
	for (XU32 i = 0;i < m_numBuckets;++i)
		m_pBuckets[i].clear();
	m_numItems = 0;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStatus XStringMap<valueType>::init(XU32 numBuckets,XHasher* pHasher)
{
	X_ASSERTM(numBuckets != 0,"Number of buckets cannot be 0 for a hash table");

	//Do we have a custom hasher?  If not, use the default hasher
	if (!pHasher)
		m_pHasher = &m_defaultHasher;

	//Allocate buckets
	XNewArray(m_pBuckets,XMemBlockList<KeyValuePair>,numBuckets,XOSMgr::getDefaultHeap());
	m_numBuckets = numBuckets;

	return X_OK;
}


	
//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStatus XStringMap<valueType>::destroy()
{
	//Destroy all buckets
	clear();
	XDeleteArray(m_pBuckets);
	m_numBuckets = 0;
	m_numItems = 0;
	m_peakUsage = 0;
	m_pHasher = NULL;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStatus XStringMap<valueType>::insert(XS8* pStringKey,valueType value)
{
	X_ASSERTM(m_pHasher,"No hash function defined.  Is hash table initialized?");
	X_ASSERTM(pStringKey,"Null string key passed to hash table");

	//First, hash the key and find out which bucket we'll be inserting the
	//value into
	XU32 bucketNum = m_pHasher->hash(pStringKey,(XU32)strlen(pStringKey)) % m_numBuckets;

	//Now make sure the item does not already exist in bucket.  We force string names to
	//be unique. 
	XMemBlockList<KeyValuePair>::iterator i(m_pBuckets[bucketNum].begin());
	XBOOL alreadyExists = false;
	while (i.isValid())
	{
		if (strcmp(pStringKey,i.getValue().getStringKey()) == 0)
		{
			alreadyExists = true;
			break;
		}
		i.getNext();
	}
	if (alreadyExists)
		return X_ERR_REDUNDANT_OPERATION;

	KeyValuePair p(pStringKey,value);
	m_pBuckets[bucketNum].insertBack(p);

	++m_numItems;
	if (m_numItems > m_peakUsage)
		m_peakUsage = m_numItems;

	return X_OK;
}

	

//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XStatus XStringMap<valueType>::remove(XS8* pStringKey)
{
	//First, hash the key and find out which bucket we'll be removing from
	XU32 bucketNum = m_pHasher->hash(pStringKey,(XU32)strlen(pStringKey)) % m_numBuckets;

	XMemBlockList<KeyValuePair>::iterator i(m_pBuckets[bucketNum].begin());
	while (i.isValid())
	{
		if (strcmp(pStringKey,i.getValue().getStringKey()) == 0)
		{
			m_pBuckets[bucketNum].remove(i.getValue());
			--m_numItems;
			break;
		}
		i.getNext();
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XU32 XStringMap<valueType>::getNumItems()
{
	return m_numItems;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XU32 XStringMap<valueType>::getPeakUsage()
{
	return m_peakUsage;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XU32 XStringMap<valueType>::getNumBuckets()
{
	return m_numBuckets;
}


	
//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XMemBlockList<typename XStringMap<valueType>::KeyValuePair>* XStringMap<valueType>::getBucket(XU32 index)
{
	X_ASSERTM(index < m_numBuckets,"Bucket index out of range");
	return &m_pBuckets[index];
} 



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
valueType* XStringMap<valueType>::getItem(XS8* pStringKey)
{
	//First, hash the key and find out which bucket we'll be searching
	XU32 bucketNum = m_pHasher->hash(pStringKey,(XU32)strlen(pStringKey)) % m_numBuckets;

	//Now cycle through the list and see if we can find the string key. 
	XMemBlockList<KeyValuePair>::iterator i(m_pBuckets[bucketNum].begin());
	while (i.isValid())
	{
		if (strcmp(pStringKey,i.getValue().getStringKey()) == 0)
		{
			return i.getValue().getValue();
		}
		i.getNext();
	}

	return NULL;
}



//*****************************************************************************
//*****************************************************************************
template<typename valueType>
XF32 XStringMap<valueType>::getBucketUtilization()
{
	if (m_numBuckets != 0)
	{
		XU32 inUseBuckets = 0;
		for (XU32 i = 0;i < m_numBuckets;++i)
			if (m_pBuckets[i].getNumItems() != 0)
				++inUseBuckets;
		return inUseBuckets/(XF32)m_numBuckets;

	}
	else
		return 0.0f;
}


#endif