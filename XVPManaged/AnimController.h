
#pragma once

using namespace System;

#define MAX_ANIM_TRACKS	64

namespace XVPManaged
{
	public class AnimController : public BaseController
	{
	protected:
		XAnimController* m_pAC;
		XAnimTrack* m_animTracks[MAX_ANIM_TRACKS];

	public:
		AnimController();
		~AnimController();
		XStatus init(XScene* pScene);
		XStatus destroy();
		XController* getController();
		void setNumAnimTracks(XU32 numAnimTracks);
		void setAnimTrack(XU32 trackIndex,XS8* pTrackFilename);
		XU32 getAnimTrackTime(XU32 trackIndex);
		XU32 getAnimTrackDuration(XU32 trackIndex);
		void setAnimTrackTime(XU32 trackIndex,XU32 milliseconds);
		void playAnimTrack(XU32 trackIndex,XF32 speedScale,XS32 playCount,XF32 weight);
		void pauseAnimTrack(XU32 trackIndex);
		void stopAnimTrack(XU32 trackIndex);
		void stopAllTracks();
		void setAnimTrackSpeed(XU32 trackIndex,XF32 speedScale);
		void setAnimTrackPlayCount(XU32 trackIndex,XS32 playCount);
		void setAnimTrackWeight(XU32 trackIndex,XF32 weight);

	};
}