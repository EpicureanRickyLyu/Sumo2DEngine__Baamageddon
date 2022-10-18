#pragma once
///////////////////////////////////////////////////////////////////////////
//	File		: AABB.h
//  AABB : Axis Aligned Bounding Box
//  Several functions and structures for working with AABB collisions.
//  This implementation is based the following article:
//    https://noonat.github.io/intersect/
///////////////////////////////////////////////////////////////////////////

struct AABB
{
	Point2f pos;
	Point2f halfSize;
};

//-------------------------------------------------------------------------

float Clamp(float f, float lower, float upper);

float SignFloat(float f);

float TolInv(float f);

bool AABBSegmentTest(const AABB& box, const Point2f& a, const Point2f& b, float& tOut);

bool AABBTest(const AABB& boxA, const AABB& boxB, Vector2f& offsetOut);

bool AABBSweepTest(const AABB& boxA, const AABB& boxB, const Vector2f& delta, Vector2f& outPos);

void DrawAABB(const AABB& box, const Play::Colour& colour);

//-------------------------------------------------------------------------