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
//	Module Name:	XAnimConroller.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-6-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the animation controller.  
*/
//*****************************************************************************
XAnimController::XAnimController()
{
	m_numTracks = 0;
}



//*****************************************************************************
/**
Destructor for the animation controller.  
*/
//*****************************************************************************
XAnimController::~XAnimController()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimController::init(XU32 maxTracks)
{
	m_trackIndexArray.init(maxTracks,NULL);
	m_currentTrackList.init(maxTracks,NULL);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimController::destroy()
{
	flushTracks();
	m_trackIndexArray.destroy();
	m_currentTrackList.destroy();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XAnimController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the animation controller.  This is the
description that appears in log files and other debug output.

@return A string description of the animation controller.
*/
//*****************************************************************************
XS8* XAnimController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XAnimController";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
bool XAnimController::isCompatible(XSceneNode* pNode)
{
	X_ASSERTM(pNode,"Invalid node pointer");
	
	X_SCENE_NODE_TYPE snt = pNode->getType();
	XBOOL typeOK = (X_SNT_GET_BASE_SNID(snt) == X_SNID_MODEL_OBJECT);
				/*(snt == X_SNT_MODEL_OBJECT) || 
				(snt == X_SNT_BONE) ||
				(snt == X_SNT_CAMERA_OBJECT) ||
				(snt == X_SNT_PARTICLE_EMITTER));*/
		
	if (typeOK)
		return true;
	else
		return false;



	//Is this node compatible with the controller?
	/*if (typeOK)
	{
		//Make sure that we have track information for this node.  If we don't
		//then it makes no sense to attach the animation controller to the node
		//as it won't affect it.  Spin through each track and check to see if the
		//nodeID exists for that track.  If, at least 1 track doesn not reference
		//the node, then return false to indicate that this node should not
		//recieve the attachment.
		XU32 nodeID = ((XModelObject*)pNode)->getHierarchyID();
		for (XU32 i = 0;i < m_numTracks;i++)
		{	
			if (nodeID < (*m_trackIndexArray[i])->m_keysList.size())
			{
				if ((*m_trackIndexArray[i])->m_keysList[nodeID])
					return true;
			}
		}	
	}*/

	//If we make it here, we couldn't find any animation info from
	//any of the tracks so this attachment is not compatible.
	//return false;
}



//*****************************************************************************
/**
Allows the controller to influence a scene node.  Normally, the user does
not call this method manually.  Instead, once this controller has been 
attached to a node and the node has been added to the scene graph, this method
is called by the node whenever the update pass occurs in the main scene graph.

@param pNode Pointer to the current scene node which is requesting an update.
			 This parameter will never be NULL.  
@param pParentNode Pointer to the parent scene node of pNode.  If the node
				   does not have a parent, this parameter will be NULL.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
					This parameter will never be NULL.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
										  XFreeArray<XMatrix4x4>* pMatrixStack)
{
	XQuaternion				fromQuat;
	XQuaternion				toQuat;
	XQuaternion				newQuat;
	XVector3D				fromTransVec;
	XVector3D				toTransVec;
	XVector3D				fromScaleVec(1,1,1);
	XVector3D				toScaleVec(1,1,1);
	XF32					quatSlerp = 0;
	XF32					transLerp = 0;
	XF32					scaleLerp = 0;
	XMatrix4x4				objectTransform;
	XU32					nodeID = ((XModelObject*)pNode)->getHierarchyID();
	XVector3D				newTransVec;
	XVector3D				newScaleVec;
	XVector3D				translationVec;
	XAnimTrackInfo*			atPtr;
	accumulator				acc;
	

	for (XU32 i = 0;i < m_numTracks;i++)
	{	
		//If animation is not playing just continue...
		atPtr = m_currentTrackList[i]; 
		if ((!(atPtr->flags & X_AT_PLAYING)) || (atPtr->weight <= 0))
			continue;
 
		XU32 currentPlayTime = atPtr->currentPlayTime;

		//Make sure that we have a valid nodeID for this particular track
		if (nodeID > (*m_trackIndexArray[i])->m_keysList.size()-1)
			return X_ERR_NO_EFFECT;
		if (!(*m_trackIndexArray[i])->m_keysList[nodeID])
			return X_ERR_NO_EFFECT;

		XFreeArray<XRotationKey>* rotKeyListPtr = &(*m_trackIndexArray[i])->m_keysList[nodeID]->rotKeyList;
		XFreeArray<XPositionKey>* posKeyListPtr = &(*m_trackIndexArray[i])->m_keysList[nodeID]->posKeyList;
		XFreeArray<XScaleKey>* scaleKeyListPtr = &(*m_trackIndexArray[i])->m_keysList[nodeID]->scaleKeyList;
		XU32 lastPosKey = (*m_trackIndexArray[i])->m_keysList[nodeID]->lastPosIndex;
		XU32 lastRotKey = (*m_trackIndexArray[i])->m_keysList[nodeID]->lastRotIndex;
		XU32 lastScaleKey = (*m_trackIndexArray[i])->m_keysList[nodeID]->lastScaleIndex;

		//Search forwards or backwards?
		if (currentPlayTime >= (*rotKeyListPtr)[lastRotKey]->time)
		{ 
			//Find the rotation key.
			for (XU32 j = lastRotKey;j < rotKeyListPtr->getNumItems()-1;++j)
			{	
				if ((currentPlayTime >= (*rotKeyListPtr)[j]->time) &&
						(currentPlayTime <= (*rotKeyListPtr)[j+1]->time))
				{
					fromQuat = (*rotKeyListPtr)[j]->quat;
					toQuat = (*rotKeyListPtr)[j+1]->quat;
					quatSlerp = (XF32)(currentPlayTime - (*rotKeyListPtr)[j]->time) /
							((*rotKeyListPtr)[j+1]->time - (*rotKeyListPtr)[j]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastRotIndex = j;
					break;
				}					
			}
		}
		else
		{
			//Find the rotation key by searching backwards.
			for (XU32 j = lastRotKey;j > 0;--j)
			{	
				if ((currentPlayTime >= (*rotKeyListPtr)[j-1]->time) &&
						(currentPlayTime <= (*rotKeyListPtr)[j]->time))
				{
					fromQuat = (*rotKeyListPtr)[j-1]->quat;
					toQuat = (*rotKeyListPtr)[j]->quat;
					quatSlerp = (XF32)(currentPlayTime - (*rotKeyListPtr)[j-1]->time) /
							((*rotKeyListPtr)[j]->time - (*rotKeyListPtr)[j-1]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastRotIndex = j-1;
					break;
				}					
			}
		}

		//Search forwards or backwards?
		if (currentPlayTime >= (*posKeyListPtr)[lastPosKey]->time)
		{ 
			//Find the position key.
			for (XU32 j = lastPosKey;j < posKeyListPtr->getNumItems()-1;++j)
			{
				if ((currentPlayTime >= (*posKeyListPtr)[j]->time) &&
					(currentPlayTime <= (*posKeyListPtr)[j+1]->time))
				{
					fromTransVec = (*posKeyListPtr)[j]->vec;
					toTransVec = (*posKeyListPtr)[j+1]->vec;
					transLerp = (XF32)(currentPlayTime - (*posKeyListPtr)[j]->time) /
							((*posKeyListPtr)[j+1]->time - (*posKeyListPtr)[j]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastPosIndex = j;
					break;
				}		
			}
		}
		else
		{
			//Find the position key by searching backwards.
			for (XU32 j = lastPosKey;j > 0;--j)
			{
				if ((currentPlayTime >= (*posKeyListPtr)[j-1]->time) &&
					(currentPlayTime <= (*posKeyListPtr)[j]->time))
				{
					fromTransVec = (*posKeyListPtr)[j-1]->vec;
					toTransVec = (*posKeyListPtr)[j]->vec;
					transLerp = (XF32)(currentPlayTime - (*posKeyListPtr)[j-1]->time) /
							((*posKeyListPtr)[j]->time - (*posKeyListPtr)[j-1]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastPosIndex = j-1;
					break;
				}		
			}
		}

		//Search forwards or backwards?
		if (currentPlayTime >= (*scaleKeyListPtr)[lastScaleKey]->time)
		{ 
			//Find the scale key.
			for (XU32 j = lastScaleKey;j < scaleKeyListPtr->getNumItems()-1;++j)
			{
				if ((currentPlayTime >= (*scaleKeyListPtr)[j]->time) &&
					(currentPlayTime <= (*scaleKeyListPtr)[j+1]->time))
				{
					fromScaleVec = (*scaleKeyListPtr)[j]->vec;
					toScaleVec = (*scaleKeyListPtr)[j+1]->vec;
					scaleLerp = (XF32)(currentPlayTime - (*scaleKeyListPtr)[j]->time) /
							((*scaleKeyListPtr)[j+1]->time - (*scaleKeyListPtr)[j]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastScaleIndex = j;
					break;
				}		
			}
		}
		else
		{
			//Find the scale key by searching backwards
			for (XU32 j = lastScaleKey;j > 0;--j)
			{
				if ((currentPlayTime >= (*scaleKeyListPtr)[j-1]->time) &&
					(currentPlayTime <= (*scaleKeyListPtr)[j]->time))
				{
					fromScaleVec = (*scaleKeyListPtr)[j-1]->vec;
					toScaleVec = (*scaleKeyListPtr)[j]->vec;
					scaleLerp = (XF32)(currentPlayTime - (*scaleKeyListPtr)[j-1]->time) /
							((*scaleKeyListPtr)[j]->time - (*scaleKeyListPtr)[j-1]->time);
					(*m_trackIndexArray[i])->m_keysList[nodeID]->lastScaleIndex = j-1;
					break;
				}		
			}
		}


		//Lerp the position.
		newTransVec = fromTransVec.lerp(toTransVec,transLerp);

		//Lerp the scale.
		newScaleVec = fromScaleVec.lerp(toScaleVec,scaleLerp);

		//Slerp the rotation.
		toQuat.normalize();
		newQuat = fromQuat.slerp(toQuat,quatSlerp);

		//Insert info into the accumulator
		XF32 factor = atPtr->weight;	
		acc.rotation = acc.rotation.slerp(newQuat,factor);
		acc.translation = acc.translation.lerp(newTransVec,factor);
		acc.scale = acc.scale.lerp(newScaleVec,factor);
	}

	//Create scale matrix.
	XMatrix4x4 scale;
	scale._11 = acc.scale.x;
	scale._22 = acc.scale.y;
	scale._33 = acc.scale.z;
	
	//Create new transform with rotation and scale.   		
	objectTransform = acc.rotation.toMatrix4x4();

	XMatrix4x4 trans;
	objectTransform._41 += acc.translation.x;
	objectTransform._42 += acc.translation.y;
	objectTransform._43 += acc.translation.z;

	XMatrix4x4 final =  objectTransform * scale; 

	//Finally, set the new transform.
	pNode->setModelTransform(final);
	
	return X_OK;
}
	  


//*****************************************************************************
/**
Allows the animation to update any time dependant internal variables.  X-VP
calls this method once per frame.

@param timeElapsed	The amount of time in seconds that has passed since the last
					frame update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::update(XF32 timeElapsed)
{
	//Update all the currently playing animations. The "* 1000" is for milliseconds
	XU32 tempNewTime = 0;

	for (XU32 i = 0;i < m_numTracks;i++)
	{
		if (X_TEST_ULONG_BIT(m_currentTrackList[i]->flags,X_AT_PLAYING))
		{		 
			//Bail if this animation is paused.
			if (X_TEST_ULONG_BIT(m_currentTrackList[i]->flags,X_AT_PAUSED))
				continue;

			tempNewTime = (XU32)((m_currentTrackList[i]->currentPlayTime) +
						  ((timeElapsed * 1000) * m_currentTrackList[i]->speedScale));
			  
			//If the new time is greater than the length of the animation, then
			//we need to see if this animation is set to loop.
			if (tempNewTime > (*m_trackIndexArray[i])->m_timeLength)
			{
				//If this is not an infinite loop animation then decrement
				//the playCount.
				if (m_currentTrackList[i]->playCount > 0)
					m_currentTrackList[i]->playCount--;
				
				//We need to respond to all events that we may miss by looping or
				//finishing the animation.
				flushTimeEvents(i);

				switch (m_currentTrackList[i]->playCount)
				{
				//Done looping...
				case 0:
					m_currentTrackList[i]->currentPlayTime = (*m_trackIndexArray[i])->m_timeLength;
					X_CLEAR_ULONG_BIT(m_currentTrackList[i]->flags,X_AT_PLAYING);
					break;
				//Looping forever (-1) or x number of times.
				default:
					//Temporarily set the current play time to 0 so we can sync all
					//time events.  Then we'll adjust the overlap and account
					//for any time events
					m_currentTrackList[i]->currentPlayTime = 0;
					syncTimeEvents(i);

					m_currentTrackList[i]->currentPlayTime = tempNewTime - (*m_trackIndexArray[i])->m_timeLength;
					handleTimeEvents(i);
					break;
				};	
			}
			else
			{
				m_currentTrackList[i]->currentPlayTime = tempNewTime;
				handleTimeEvents(i);
			}
		}
	}

	return X_OK;
}



//*****************************************************************************
/**
Flushes any remaining time events for an animation track that have not been
triggered yet.  All event callbacks remaining will be called.

@param trackHandle	The handle of the track for which to flush all time events.
@return A return code of type XStatus.
*/
//*****************************************************************************
void XAnimController::flushTimeEvents(XHandle trackHandle)
{
	//Make sure all events for the entire animation have played and
	//mark them as consumed.
	XMemBlockList<XAnimTimeEvent>::iterator i(m_currentTrackList[(XU32)trackHandle]->timeEvents.begin());
	while (i.isValid())
	{
		if (i.getValue().consumed != 1)
		{
			m_currentTrackList[(XU32)trackHandle]->pEventCallback(X_AET_TIME,i.getValue().time,
							m_currentTrackList[(XU32)trackHandle]->pEventCallbackData);
			i.getValue().consumed = 1;
		}
		i.getNext();
	}
}



//*****************************************************************************
/**
Syncs all time events for an animation track.  What this means is that events
occuring before the current play time will be marked as consumed and all events
that haven't triggered yet will be marked as unconsumed.  This method does
not call event callbacks when marking events as consumed.

@param trackHandle	The handle of the track for which to sync all time events.
@return A return code of type XStatus.
*/
//*****************************************************************************
void XAnimController::syncTimeEvents(XHandle trackHandle)
{
	//Reset the consumed status of all events based on the current play time
	XMemBlockList<XAnimTimeEvent>::iterator i(m_currentTrackList[(XU32)trackHandle]->timeEvents.begin());
	while (i.isValid())
	{
		if (m_currentTrackList[(XU32)trackHandle]->currentPlayTime > i.getValue().time)
			i.getValue().consumed = 1;
		else
			i.getValue().consumed = 0;
		i.getNext();
	}
}



//*****************************************************************************
/**
Handles all time events for a particular track that have triggered due to the
current play time of the track.  Events that trigger will have their corresponding
event callback called.

@param trackHandle	The handle of the track for which to handle time events.
@param A return code of type XStatus.
*/
//*****************************************************************************
void XAnimController::handleTimeEvents(XHandle trackHandle)
{
	//Callback all expired events if there is a registered event callback
	if (!m_currentTrackList[(XU32)trackHandle]->pEventCallback)
		return;

	XMemBlockList<XAnimTimeEvent>::iterator i(m_currentTrackList[(XU32)trackHandle]->timeEvents.begin());
	while (i.isValid())
	{
		if ((m_currentTrackList[(XU32)trackHandle]->currentPlayTime > i.getValue().time) &&
			(i.getValue().consumed != 1))
		{
			m_currentTrackList[(XU32)trackHandle]->pEventCallback(X_AET_TIME,i.getValue().time,
				m_currentTrackList[(XU32)trackHandle]->pEventCallbackData);
			i.getValue().consumed = 1;
		}
		i.getNext();
	}
}



//*****************************************************************************
/**
Adds an animation track to the animation controller.  Once a track is added,
it is available for playback and manipulation.

@param pAnimationTrack	Pointer to an animation track that has been previously
						initialized.
@param trackHandle	A variable in which to store the handle for the newly
					added track.  
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::addTrack(XAnimTrack* pAnimationTrack,XHandle& trackHandle)
{
	//Don't go over max
	if (m_trackIndexArray.getNumItems() == m_trackIndexArray.getCapacity())
		return X_ERR_LIMIT_REACHED;

	if (pAnimationTrack)
	{
		if (pAnimationTrack->m_keysList.size() == 0)
			return X_ERR_INVALID_PARAMS;

		XAnimTrack** ppAnimationTrack = m_trackIndexArray.insertBack();
		(*ppAnimationTrack) = pAnimationTrack;

		trackHandle = m_numTracks;

		XAnimTrackInfo* pTrackInfo = m_currentTrackList.insertBack();
		pTrackInfo->currentPlayTime = 0;
		pTrackInfo->flags = 0;
		pTrackInfo->playCount = 0;
		pTrackInfo->speedScale = 0;
		
		++m_numTracks;

		return X_OK;
	}
	else 
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
/**
Removes all animation tracks from the animation controller.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::flushTracks()
{
	//Flush out all tracks.
	m_trackIndexArray.clear();
	m_currentTrackList.clear();
	m_numTracks = 0;

	return X_OK;
}



//*****************************************************************************
/**
Begins playback of a specific animation track.

@param trackHandle The handle of the animation to operate on.  This is the handle that
				   was assigned when the animation track was added to the animation
				   controller.
@param speedScale	The speed of the animation.  A value of 1 plays the animation
					at it's original recorded rate.  A value of .5 would play the
					animation at half its original rate.  A value of 10 would
					play the animation 10 times its normal playback rate.  Values
					less than 0 are undefined.
@param playCount	How many times to loop the animation. For example, a value
					of -1 causes the animation to loop indefinitely, while a 
					value of 3 causes the animation to loop until it has played
					3 times.  
@param weight	The weight to use when mixing this animation with other animations.  
				If other animation tracks are playing at the same time, you can specify a
				weight that the controller will use when mixing in the specified 
				animation.  For example a value of 1 would cause the animation to
				be played out to its full weight.  If this value is .5, the animation
				controller will mix the animation with other playing animations
				at half weight.  Values less than 0 are invalid.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::playTrack(XHandle trackHandle,XF32 speedScale,int playCount,
								   XF32 weight)
{
	//Validate parameters
	if ((trackHandle < 0) || (trackHandle >= m_numTracks) || (speedScale < 0) 
		|| (weight < 0) || (playCount == 0))
		return X_ERR_INVALID_PARAMS;

	//Now that we know the trackHandle is valid let's set the animation properties
	//for the corresponding track handle passed in.

	m_currentTrackList[(XU32)trackHandle]->currentPlayTime = 0;
	m_currentTrackList[(XU32)trackHandle]->playCount = playCount;
	m_currentTrackList[(XU32)trackHandle]->speedScale = speedScale;
	m_currentTrackList[(XU32)trackHandle]->weight = weight;

	syncTimeEvents(trackHandle);
	
	X_CLEAR_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PAUSED);
	X_SET_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PLAYING);

	return X_OK;
}



//*****************************************************************************
/**
Stops an animation track.  

@param trackHandle	The handle of the animation track to operate on.  This is
					the handle that was assigned when the animation track was
					added to the animation controller.  
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::stopTrack(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;

	X_CLEAR_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PLAYING);

	return X_OK;
}



//*****************************************************************************
/**
Stops all animation tracks.  

@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::stopAllTracks()
{
	for (XU32 i = 0;i < m_currentTrackList.getNumItems();++i)
		X_CLEAR_ULONG_BIT(m_currentTrackList[i]->flags,X_AT_PLAYING);

	return X_OK;
}



//*****************************************************************************
/**
Pauses an animation track.

@param trackHandle	The handle of the animation to operate on.  This is the handle
					that was assigned when the animation track was added to the
					animation controller.		
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::pauseTrack(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;

	X_SET_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PAUSED);

	return X_OK;
}



//*****************************************************************************
/**
Resumes an animation track.  

@param trackHandle	The handle of the animation track to operate on.  This is
					the handle that was assigned when the animation track was
					added to the animation controller.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::resumeTrack(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;

	X_CLEAR_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PAUSED);

	return X_OK;
}



//*****************************************************************************
/**
Sets the playback speed of an animation track.

@param trackHandle	The handle of the animation track to operate on.  This is
					the ID that was assigned when the animation track was added
					to the animation controller.
@param speedScale	The speed of the animation.  A value of 1 plays the animation
					at it's original recorded rate.  A value of .5 would play the
					animation at half its original rate.  A value of 10 would
					play the animation 10 times its normal playback rate.  Values
					less than 0 are invalid.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::setTrackSpeed(XHandle trackHandle,XF32 speedScale)
{
	//Validate parameters.
	if ((trackHandle < 0) || (trackHandle >= m_numTracks) || (speedScale < 0))
		return X_ERR_INVALID_PARAMS;

	//Set the speed of the animation
	m_currentTrackList[(XU32)trackHandle]->speedScale = speedScale;

	return X_OK;
}



//*****************************************************************************
/**
Sets the number of times to loop an animation track.  

@param trackHandle	The handle of the animation track to operate on.  This is
					the handle that was assigned when the animation track was
					added to the animation controller.
@param playCount	How many times to loop the animation. For example, a value
					of -1 causes the animation to loop indefinitely, while a 
					value of 3 causes the animation to loop until it has played
					3 times.  
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::setTrackPlayCount(XHandle trackHandle,int playCount)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_INVALID_PARAMS;

	//Set the loop count of the animation
	m_currentTrackList[(XU32)trackHandle]->playCount = playCount;
	
	return X_OK;
}



//*****************************************************************************
/**
Sets the weight of an animation track.  

@param trackHandle	The handle of the animation to operate on.  This is the 
					handle that was assigned when the animation track was added
					to the animation controller.
@param weight	The weight to use when mixing this animation with other animations.  
				If other animation tracks are currently playing, you can specify a
				weight that the controller will use when mixing in the specified 
				animation.  For example a value of 1 would cause the animation to
				be mixed in to its full strength.  If this value is .5, the animation
				controller will mix the animation with other playing animations
				at half strength.  Values less than 0 are invalid.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::setTrackWeight(XHandle trackHandle,XF32 weight)
{
	//Validate parameters.
	if ((trackHandle < 0) || (trackHandle >= m_numTracks) || (weight < 0))
		return X_ERR_INVALID_PARAMS;

	//Set the animation flags
	m_currentTrackList[(XU32)trackHandle]->weight = weight;
	
	return X_OK;
}



//*****************************************************************************
/**
Gets the playback speed of an animation track.

@param trackHandle	The handle of the track to query.  This is the handle that
					was assigned when the animation track was added to the 
					animation controller.
@return The current playback speed of the animation.
*/
//*****************************************************************************
XF32 XAnimController::getTrackSpeed(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_INVALID_PARAMS;

	return m_currentTrackList[(XU32)trackHandle]->speedScale;
}



//*****************************************************************************
/**
Gets the number of times to loop an animation.  

@param trackHandle	The handle of the track to query.  This is the handle that
					was assigned when the animation track was added to the 
					animation controller.
@return The loop count of the currently playing animation.
*/
//*****************************************************************************
XU8 XAnimController::getTrackPlayCount(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_INVALID_PARAMS;

	return m_currentTrackList[(XU32)trackHandle]->playCount;
}



//*****************************************************************************
/**
Determines if an animation is currently playing.

@param trackHandle	The handle of the track to query.  This is the handle that
					was assigned when the animation track was added to the
					animation controller.
@return True if the animation is playing, false if it isn't.
*/
//*****************************************************************************
XBOOL XAnimController::isTrackPlaying(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return false;

	if (X_TEST_ULONG_BIT(m_currentTrackList[(XU32)trackHandle]->flags,X_AT_PLAYING))
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Gets the current time of an animation in milliseconds.  For example, if 
the animation is 5000 milliseconds long, this method can be called to query
exactly where in the 5000 milliseconds the animation is.

@param trackHandle	The handle of the track to query.  This is the handle that
					was assigned when the animation track was added to the
					animation controller.
@return The current time of the animation in milliseconds.
*/
//*****************************************************************************
XU32 XAnimController::getTrackTime(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return 0;

	return m_currentTrackList[(XU32)trackHandle]->currentPlayTime;
}



//*****************************************************************************
/**
Sets the current time of an animation in milliseconds.  This method can be
used to seek to a particular playback time within the animation.

@param trackHandle	The handle of the track for which to set the track time.
					This is the handle that	was assigned when the animation 
					track was added to the animation controller.
@param milliseconds	The seek time in milliseconds.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::setTrackTime(XHandle trackHandle,XU32 milliseconds)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;

	if (milliseconds <= (*m_trackIndexArray[(XU32)trackHandle])->m_timeLength)
	{
		m_currentTrackList[(XU32)trackHandle]->currentPlayTime = milliseconds;
		syncTimeEvents(trackHandle);
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Sets the current time of an animation in milliseconds.  This method can be
used to seek to a particular playback time within the animation.

@param trackHandle	The handle of the track for which to set the track time.
					This is the handle that	was assigned when the animation 
					track was added to the animation controller.
@param milliseconds	The seek time in milliseconds.
@return A status code of type XStatus.
*/
//*****************************************************************************
XU32 XAnimController::getTrackDuration(XHandle trackHandle)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return 0;

	return (*m_trackIndexArray[(XU32)trackHandle])->m_timeLength;
}



//*****************************************************************************
/**
Registers a user callback with an animation track to be called whenever time
events for that track occur.

@param trackHandle	The handle of the track to register a callback with.  This
					is the handle that was assigned when the animation track
					was added to the animation controller.
@param callback	The user callback method to call when the events trigger.  Pass
				NULL to remove the current registration.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::registerEventCallback(XHandle trackHandle,void (*callback) 
											   (X_ANIM_EVENT_TYPE type,XU32 eventTime,
											   void* pUserData))
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;
	else
	{
		m_currentTrackList[(XU32)trackHandle]->pEventCallback = callback;
		return X_OK;
	}
}



//*****************************************************************************
/**
Sets user callback data for an animation track that will be passed to
the registered callback when any time events occur.

@param trackHandle	The handle of the track for which to set the callback data.
					This is the handle that was assigned when the animation track
					was added to the animation controller.
@param pData	The user callback data to be passed to a registered user callback.
@return A return code of type XStatus.
//*****************************************************************************
XStatus XAnimController::setEventCallbackData(XHandle trackHandle,void* pData)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;
	else
	{
		m_currentTrackList[(XU32)trackHandle].pEventCallbackData = pData;
		return X_OK;
	}	
}



//*****************************************************************************
/**
Registers a time event to occur at a specific time within an animation track.  
For example, if the animation is 5000 milliseconds long, you can register an event
to occur 1 second into the animation's playback.  When 1 second is reached, if a
callback has been registered with the track it will be called.  Note that "time"
here means the pre-recorded time.  This means that if you change the speed scale
of the animation track, you also change the scale of the event time.  You can
register an many events as needed.  If an event is added at the same time as
a previous event, it will be discarded.

@param trackHandle	The handle of the track for which to set a time event.
					This is the handle that was assigned when the animation track
					was added to the animation controller.
@param eventTime	The time, in milliseconds, at which the event should trigger.
					This value cannot exceed the length of the animation.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::registerEvent(XHandle trackHandle,XU32 eventTime)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;
	else
	{
		//Filter out multiple events at the same time.
		XMemBlockList<XAnimTimeEvent>::iterator i(m_currentTrackList[(XU32)trackHandle]->timeEvents.begin());
		while (i.isValid())
		{
			if (i.getValue().time == eventTime)
				return X_ERR_REDUNDANT_OPERATION;
			i.getNext();
		}

		if (eventTime <= (*m_trackIndexArray[(XU32)trackHandle])->m_timeLength)
		{
			XAnimTimeEvent event;
			event.time = eventTime;

			//If the current play time is after the time at which this
			//event should occur, add it as consumed so it doesn't
			//play.
			if (m_currentTrackList[(XU32)trackHandle]->currentPlayTime > eventTime)
				event.consumed = 1;
			else
				event.consumed = 0;
			m_currentTrackList[(XU32)trackHandle]->timeEvents.insertBack(event);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//*****************************************************************************
/**
Unregisters a time event from a specific time within an animation track.

@param trackHandle	The handle of the track for which to remove a time event.
					This is the handle that was assigned when the animation track
					was added to the animation controller.
@param eventTime	The time, in milliseconds, at which the event should be removed.
					This value cannot exceed the length of the animation.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimController::unregisterEvent(XHandle trackHandle,XU32 eventTime)
{
	//Make sure that the track handle is valid
	if ((trackHandle < 0) || (trackHandle >= m_numTracks))
		return X_ERR_NO_SUCH_OBJECT;
	else
	{
		if (eventTime <= (*m_trackIndexArray[(XU32)trackHandle])->m_timeLength)
		{
			XAnimTimeEvent event;
			event.time = eventTime;
			m_currentTrackList[(XU32)trackHandle]->timeEvents.remove(event);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
}

