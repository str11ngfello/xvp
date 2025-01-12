#include "xvpsdk.h"
#include "ProxyObject.h"
#include "AnimController.h"

namespace XVPManaged
{
	AnimController::AnimController()
	{
		m_pAC = NULL;
		for (XU32 i = 0;i < MAX_ANIM_TRACKS;++i)
			m_animTracks[i] = NULL;
	}

	AnimController::~AnimController()
	{
	}

	XStatus AnimController::init(XScene* pScene)
	{
		XNew(m_pAC,XAnimController(),XOSMgr::getDefaultHeap());
				
		m_pAC->init(1);
		/*XHandle th;
		XAnimTrack* at = NULL;
		XNew(at,XAnimTrack,XOSMgr::getDefaultHeap());
		at->init("c:/scratch/hellcatAnim");
		m_pAC->addTrack(at,th);
	
		m_pAC->playTrack(th,1.0,-1,1.0f);*/
		m_pScene = pScene;

		return X_OK;
	}

	XStatus AnimController::destroy()
	{
		//Free anim tracks
		for (XU32 i = 0;i < MAX_ANIM_TRACKS;++i)
		{
			if (m_animTracks[i] != NULL)
			{
				m_animTracks[i]->destroy();
				XDelete(m_animTracks[i]);
			}
		}

		m_pAC->destroy();
		
		XDelete(m_pAC);

		return X_OK;
	}

	XController* AnimController::getController()
	{
		return m_pAC;
	}

	void AnimController::setNumAnimTracks(XU32 numAnimTracks)
	{
		//Free anim tracks and re initialize the controller
		for (XU32 i = 0;i < MAX_ANIM_TRACKS;++i)
		{
			if (m_animTracks[i] != NULL)
			{
				m_animTracks[i]->destroy();
				XDelete(m_animTracks[i]);
			}
		}

		m_pAC->destroy();

		//If 0 tracks are being created, force to 1
		if (numAnimTracks == 0)
			m_pAC->init(1);
		else
			m_pAC->init(numAnimTracks);
	}

	void AnimController::setAnimTrack(XU32 trackIndex,XS8* pTrackFilename)
	{
		X_ASSERTM(trackIndex < MAX_ANIM_TRACKS,"Max number of tracks has been exceeded");
		X_ASSERTM(pTrackFilename,"Track filename cannot be null");
		X_ASSERTM(m_animTracks[trackIndex] == NULL,"A previous track already exists in this slot.");

		//Allocate a new track, initialize it and add it to the controller
		XHandle trackHandle;
		m_animTracks[trackIndex] = NULL;
		XNew(m_animTracks[trackIndex],XAnimTrack,XOSMgr::getDefaultHeap());
		m_animTracks[trackIndex]->init(NULL,pTrackFilename);
		m_pAC->addTrack(m_animTracks[trackIndex],trackHandle);
	}

	XU32 AnimController::getAnimTrackTime(XU32 trackIndex)
	{
		return m_pAC->getTrackTime(trackIndex);
	}

	XU32 AnimController::getAnimTrackDuration(XU32 trackIndex)
	{
		return m_pAC->getTrackDuration(trackIndex);
	}
	
	void AnimController::setAnimTrackTime(XU32 trackIndex,XU32 milliseconds)
	{
		m_pAC->setTrackTime(trackIndex,milliseconds);
	}

	void AnimController::playAnimTrack(XU32 trackIndex,XF32 speedScale,XS32 playCount,XF32 weight)
	{
		m_pAC->playTrack(trackIndex,speedScale,playCount,weight);
	}

	void AnimController::pauseAnimTrack(XU32 trackIndex)
	{
		m_pAC->pauseTrack(trackIndex);
	}
	
	void AnimController::stopAnimTrack(XU32 trackIndex)
	{
		m_pAC->stopTrack(trackIndex);
	}

	void AnimController::stopAllTracks()
	{
		m_pAC->stopAllTracks();
	}
	
	void AnimController::setAnimTrackSpeed(XU32 trackIndex,XF32 speedScale)
	{
		m_pAC->setTrackSpeed(trackIndex,speedScale);
	}

	void AnimController::setAnimTrackPlayCount(XU32 trackIndex,XS32 playCount)	
	{
		m_pAC->setTrackPlayCount(trackIndex,playCount);
	}

	void AnimController::setAnimTrackWeight(XU32 trackIndex,XF32 weight)
	{
		m_pAC->setTrackWeight(trackIndex,weight);
	}
}
