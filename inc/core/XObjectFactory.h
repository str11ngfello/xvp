


#ifndef __X_OBJECT_FACTORY_H__
#define __X_OBJECT_FACTORY_H__


//Object creation function 
template<typename BaseClassType, typename ClassType>
BaseClassType *CreateObject()
{
	ClassType* pNewObject = NULL;
	XNew(pNewObject,ClassType(),XOSMgr::getDefaultHeap());
	return pNewObject;
}

template<typename BaseClassType>
class XObjectFactory
{
protected:
	typedef BaseClassType *(*CreateObjectFunc)();
	XStringMap<CreateObjectFunc> m_stringMap;
	XBOOL m_inited;

public:
	   //typedef typename XStringMap<CreateObjectFunc>::const_iterator ConstIterator;
	   //typedef typename XStringMap<CreateObjectFunc>::iterator Iterator;

	XObjectFactory()
	{
		m_inited = false;
	}

	XStatus init(XU32 numBuckets)
	{
		m_inited = true;
		return m_stringMap.init(numBuckets);
	}
	
	XStatus destroy()
	{
		m_inited = false;
		return m_stringMap.destroy();
	}

	template<typename ClassType>
	XStatus registerObject(XS8* stringKey)
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		X_ASSERTM(stringKey,"String key cannot be null");
		X_ASSERTM(m_stringMap.getItem(stringKey) == NULL,"Object type already registered in Object Factory");

		return m_stringMap.insert(stringKey,&CreateObject<BaseClassType, ClassType>);
	}

	XStatus unregisterObject(XS8* stringKey)
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		X_ASSERTM(stringKey,"String key cannot be null");
		return m_stringMap.remove(stringKey);
	}

	XStatus unregisterAllObjects()
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		return m_stringMap.clear();
	}

	BaseClassType *createObject(XS8* stringKey)
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		X_ASSERTM(stringKey,"String key cannot be null");
		X_ASSERTM(m_stringMap.getItem(stringKey) != NULL,"Object creation failed because the object factory could not find a corresponding registered object type");
		return (*m_stringMap.getItem(stringKey))() ;
	}
	
	XU32 getNumRegisteredObjects()
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		return m_stringMap.getNumItems();
	}

	XBOOL isObjectRegistered(XS8* stringKey)
	{
		X_ASSERTM(m_inited,"Object factory not initialized");
		X_ASSERTM(stringKey,"String key cannot be null");
		if (m_stringMap.getItem(stringKey) != NULL)
			return true;
		else
			return false;
	}

};


#endif