///////////////////////////////////////////////////////////////////////////
//	File		: AABB.h
//  AABB : Axis Aligned Bounding Box
//  Several functions and structures for working with AABB collisions.
//  This implementation is based the following article:
//    https://noonat.github.io/intersect/
///////////////////////////////////////////////////////////////////////////

#include "Play.h"
#include "AABB.h"

//-------------------------------------------------------------------------

float Clamp(float f, float lower, float upper)
{
	if (f < lower) return lower;
	if (f > upper) return upper;
	return f;
}

//-------------------------------------------------------------------------

float SignFloat(float f)
{
	return f >= 0.f ? 1.f : -1.f;
}

//-------------------------------------------------------------------------

float TolInv(float f)
{
	return abs(f) > 0.00001 ? 1.0f / f : 99999.9f;
}

//-------------------------------------------------------------------------

bool AABBSegmentTest(const AABB& box, const Point2f& a, const Point2f& b, float& tOut)
{
	if (a.x == b.x && a.y == b.y)
	{
		return false;
	}

	Vector2f delta = b - a;
	Vector2f scale = { TolInv(delta.x), TolInv(delta.y) };
	Vector2f sign = { SignFloat(scale.x), SignFloat(scale.y) };

	Vector2f vT0 = (box.pos - box.halfSize * sign - a) * scale;
	Vector2f vT1 = (box.pos + box.halfSize * sign - a) * scale;

	if (vT0.x > vT1.y || vT0.y > vT1.x)
	{
		return false;
	}

	float t0 = vT0.x > vT0.y ? vT0.x : vT0.y;
	float t1 = vT1.x < vT1.y ? vT1.x : vT1.y;

	if (t0 >= 1.f || t1 <= 0.f)
	{
		return false;
	}

	tOut = Clamp(t0, 0.f, 1.0f);
	return true;
}

//-------------------------------------------------------------------------

bool AABBTest(const AABB& boxA, const AABB& boxB, Vector2f& offsetOut)
{
	float dx = boxA.pos.x - boxB.pos.x;
	float px = (boxA.halfSize.x + boxB.halfSize.x) - abs(dx);
	if (px <= 0)
	{
		return false;
	}

	float dy = boxA.pos.y - boxB.pos.y;
	float py = (boxA.halfSize.y + boxB.halfSize.y) - abs(dy);
	if (py <= 0)
	{
		return false;
	}

	if (px < py)
	{
		offsetOut.x = dx >= 0.f ? px : -px;
		offsetOut.y = 0.f;
	}
	else
	{
		offsetOut.x = 0.f;
		offsetOut.y = dy >= 0.f ? -py : py;
	}
	return true;
}

//-------------------------------------------------------------------------

bool AABBSweepTest(const AABB& boxA, const AABB& boxB, const Vector2f& delta, Vector2f& outPos)
{
	if (delta.x == 0.f && delta.y == 0.f) {
		Vector2f offset;
		if (AABBTest(boxA, boxB, offset))
		{
			outPos = boxB.pos + delta;
			return true;
		}
	}

	AABB box = { boxA.pos, boxA.halfSize + boxB.halfSize };

	float time = 0.f;
	if (AABBSegmentTest(box, boxB.pos, boxB.pos + delta, time))
	{
		outPos = boxB.pos + delta * time;
		return true;
	}

	outPos = boxB.pos + delta;

	return false;
}

//-------------------------------------------------------------------------

void DrawAABB(const AABB& box, const Play::Colour& colour)
{
	Point2f bl = box.pos - box.halfSize;
	Point2f br = box.pos + box.halfSize;
	Play::DrawRect(bl, br, colour);
}

//-------------------------------------------------------------------------
