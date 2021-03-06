#pragma once
#include "AnimationData.h"

// Copypasta from Gamasutra https://www.gamasutra.com/view/feature/131694/architecting_a_3d_animation_engine.php

//Listing 1

// An interface to an animated object. Most comments are
// left out for brevity. See accompanying article for
// full descriptions of everything here.

// StandardTransitionTime is 1/10 second, 100 milliseconds


static const float StandardTransitionTime = 100.0f;


// DefaultMsPerFrame is 1/30 second


static const float DefaultMsPerFrame = 33.333333f;


// The intent is to provide an interface only, but some of the data
// mentioned in the article is presented below. Obviously, in a full
// animation engine implementation, there will be much more data
// in this object.

class Animation
{
	int currentAnimation; // ID of animation playing now
	float currentAnimationTime; // current frame of current animation
	int nextAnimation; // ID of next animation to play, -1 if none set
	float msPerFrame; // current playback rate setting
	bool loopCurrentAnim; // true if current animation should loop


						  // Every animated object has to have a pointer to the animation data
						  // it is capable of playing. Actual implementation is up to you.

	AnimationData *animationData;
public:
	Animation();
	~Animation();

			Animation(AnimationData *animDataIn); // constructor
			void PlayAnimation(int animNum, float startTime = 0.0f, float transitionTime = 0.0f);

			void SetNextAnimation(int next);

			void EnableLoopedAnimation();
			void DisableLoopedAnimation();
			bool IsAnimationLooped();

			void PlayNextAnimation();
			void TransitionIntoNextAnimation();

			void StopAnimation();
			void StartAnimation();

			void SetMsPerFrame(float newRate = DefaultMsPerFrame);
			float GetMsPerFrame();

			void CaptureRotationChannel(int which);
			void CaptureTranslationChannel(int which);

			int GetCurrentAnimation();
			float GetAnimationTimeInMs();
			float GetAnimationTimeInFrames();
			bool InTransition();

			int GetNextAnimation();
			void GetGlobalPosition(int joint, float &x, float &y, float &z);
			void GetCurrentVelocity(float &x, float &y, float &z);
			void SetCurrentVelocity(float x, float y, float z);
			float GetAnimLengthInMs(int animNum);
	};
////Listing 2
//
//inline void PlayAnimationAtTime(int animNum, float startTime)
//{
//	PlayAnimation(animNum, startTime);
//}
//
//
////Listing 3
//
//inline void TransitionIntoAnimation(int animNum)
//{
//	PlayAnimation(animNum, 0.0f, StandardTransitionTime);
//}
//
//
////Listing 4
//
//inline void TransitionIntoAnimationAtTime(int animNum, float startTime)
//{
//	PlayAnimation(animNum, startTime, StandardTransitionTime);
//}
//
//
////Listing 5
//
//void EnableLoopedAnimation();
//void DisableLoopedAnimation();
//bool IsAnimationLooped();
//
//
////Listing 6
//
//int GetNextAnimation();
//void GetGlobalPosition(int joint, float &x, float &y, float &z);
//void GetCurrentVelocity(float &x, float &y, float &z);
//void SetCurrentVelocity(float x, float y, float z);
//float GetAnimLengthInMs(int animNum);
//bool IsAnimationLooped();


