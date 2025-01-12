
#include <xvpsdk.h>



#define HEAP_FACTORY_ROOT_HEAP_NAME			"Root"
#define HEAP_FACTORY_DEFAULT_HEAP_NAME		"Default"

XHeap * XHeapFactory::s_pRootHeap    = NULL;
XHeap * XHeapFactory::s_pDefaultHeap = NULL;
XHeap XHeapFactory::s_heaps[];


XStatus XHeapFactory::init()
{   
    for (XU32 i = 0;i < X_MAX_HEAPS;++i)
        s_heaps[i].Initialize ();

    s_pRootHeap = CreateNewHeap(HEAP_FACTORY_ROOT_HEAP_NAME);
    s_pDefaultHeap = CreateHeap(HEAP_FACTORY_DEFAULT_HEAP_NAME);

	return X_OK;
}



XHeap * XHeapFactory::GetRootHeap ()
{
	//We may need to create the heap before XVP has initialized.  For example, 
	//a global or static variable may need memory before XVP has initialized.
    if (s_pRootHeap == NULL)
        init();

    return s_pRootHeap;
}


XHeap * XHeapFactory::GetDefaultHeap ()
{
	//We may need to create the heap before XVP has initialized.  For example, 
	//a global or static variable may need memory before XVP has initialized.
    if (s_pRootHeap == NULL)
        init();

    return s_pDefaultHeap;
}



XHeap * XHeapFactory::CreateHeap (char * name, char * parent)
{
	//We may need to create the heap before XVP has initialized.  For example, 
	//a global or static variable may need memory before XVP has initialized.
    if (s_pRootHeap == NULL)
        init();

    XHeap * pParent = FindHeap(parent);
    if (pParent == NULL)
    {
        pParent = CreateNewHeap(parent);
        pParent->AttachTo (s_pRootHeap);
    }

    XHeap * pHeap = FindHeap(name);
    if (pHeap == NULL)
        pHeap = CreateNewHeap(name);

    pHeap->AttachTo (pParent);
    return pHeap;
}



XHeap * XHeapFactory::CreateHeap (char * name)
{
    return CreateHeap(name, HEAP_FACTORY_ROOT_HEAP_NAME);
}


XHeap * XHeapFactory::FindHeap (char * name)
{
    for (int i=0; i < X_MAX_HEAPS; ++i)
    {
        XHeap * pHeap = &s_heaps[i];
        if (pHeap->IsActive() && !_stricmp(name, pHeap->GetName()))
            return pHeap;
    }

    return NULL;
}


XHeap * XHeapFactory::CreateNewHeap (char * name)
{
    for (int i=0; i < X_MAX_HEAPS; ++i)
    {
        XHeap * pHeap = &s_heaps[i];
        if (!pHeap->IsActive())
        {
            pHeap->Activate (name);
            return pHeap;
        }
    }

    return NULL;
}



void XHeapFactory::PrintInfo ()
{
	char buf[512];

	sprintf(buf,"\n\nMEMORY STATISTICS");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
    sprintf (buf,"                                   Local        Local        Local         Total        Total        Total");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
    sprintf (buf,"Name                             Current         Peak       Allocs       Current         Peak       Allocs");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
    sprintf (buf,"----------------------------------------------------------------------------------------------------------");
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
    GetRootHeap()->PrintTreeInfo(0);
	X_DEBUG_LOG("\n\n",X_DLL_NONE);
}



int XHeapFactory::GetMemoryBookmark () 
{
    return XHeap::GetMemoryBookmark();
}


/*void XHeapFactory::ReportMemoryLeaks (int nBookmark) 
{
    ReportMemoryLeaks (/*nBookmark, GetMemoryBookmark ());
}*/


#ifndef XVP_FINAL
void XHeapFactory::dumpAllocations() 
{
	X_DEBUG_LOG_LN("\n*** BEGIN MEMORY ALLOCATION DUMP ***",X_DLL_NONE);
    int numAllocations = 0;
    for (int i=0; i < X_MAX_HEAPS; ++i)
        if (s_heaps[i].IsActive())
            numAllocations += s_heaps[i].dumpAllocations();

	
    if (numAllocations > 0)
	{
		XS8 buf[256];
		sprintf(buf,"%d total memory allocations active.", numAllocations);
        X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	}
    else
	{
        X_DEBUG_LOG_LN("No memory allocations.",X_DLL_NONE);
	}
	X_DEBUG_LOG_LN("*** END MEMORY ALLOCATION DUMP ***",X_DLL_NONE);
}
#endif