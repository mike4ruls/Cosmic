#include "Animation.h"



Animation::Animation()
{

}


Animation::~Animation()
{
}

Animation::Animation(AnimationData * animDataIn)
{
}

void Animation::PlayAnimation(int animNum, float startTime, float transitionTime)
{

}

void Animation::SetNextAnimation(int next)
{
}

void Animation::EnableLoopedAnimation()
{
}

void Animation::DisableLoopedAnimation()
{
}

void Animation::PlayNextAnimation()
{
}

void Animation::TransitionIntoNextAnimation()
{
}

void Animation::StopAnimation()
{
}

void Animation::StartAnimation()
{
}

void Animation::SetMsPerFrame(float newRate)
{
}

float Animation::GetMsPerFrame()
{
	return 0.0f;
}

void Animation::CaptureRotationChannel(int which)
{
}

void Animation::CaptureTranslationChannel(int which)
{
}

int Animation::GetCurrentAnimation()
{
	return 0;
}

float Animation::GetAnimationTimeInMs()
{
	return 0.0f;
}

float Animation::GetAnimationTimeInFrames()
{
	return 0.0f;
}

bool Animation::InTransition()
{
	return false;
}

bool Animation::IsAnimationLooped()
{
	return false;
}

int Animation::GetNextAnimation()
{
	return 0;
}

void Animation::GetGlobalPosition(int joint, float & x, float & y, float & z)
{
}

void Animation::GetCurrentVelocity(float & x, float & y, float & z)
{
}

void Animation::SetCurrentVelocity(float x, float y, float z)
{
}

float Animation::GetAnimLengthInMs(int animNum)
{
	return 0.0f;
}
