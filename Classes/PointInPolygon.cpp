#include "PointInPolygon.h"
#include "cocos2d.h"
#include <cmath>
#include <algorithm>

using namespace std;

//判断点在线段上
bool IsPointOnLine(FLOAT_TYPE px0, FLOAT_TYPE py0, FLOAT_TYPE px1, FLOAT_TYPE py1, FLOAT_TYPE px2, FLOAT_TYPE py2)
{
	bool flag = false;
	FLOAT_TYPE d1 = (px1 - px0) * (py2 - py0) - (px2 - px0) * (py1 - py0);
	if ((abs(d1) < FLT_EPSILON) && ((px0 - px1) * (px0 - px2) <= 0) && ((py0 - py1) * (py0 - py2) <= 0))
	{
		flag = true;
	}
	return flag;
}

//判断两线段相交
bool IsIntersect(FLOAT_TYPE px1, FLOAT_TYPE py1, FLOAT_TYPE px2, FLOAT_TYPE py2, FLOAT_TYPE px3, FLOAT_TYPE py3, FLOAT_TYPE px4, FLOAT_TYPE py4)
{
	bool flag = false;
	FLOAT_TYPE d = (px2 - px1) * (py4 - py3) - (py2 - py1) * (px4 - px3);
	if (d != 0)
	{
		FLOAT_TYPE r = ((py1 - py3) * (px4 - px3) - (px1 - px3) * (py4 - py3)) / d;
		FLOAT_TYPE s = ((py1 - py3) * (px2 - px1) - (px1 - px3) * (py2 - py1)) / d;
		if ((r >= 0) && (r <= 1) && (s >= 0) && (s <= 1))
		{
			flag = true;
		}
	}
	return flag;
}

//判断点在多边形内
bool Point_In_Polygon_2D(FLOAT_TYPE x, FLOAT_TYPE y, const std::vector<cocos2d::Point>& POL)
{
	bool isInside = false;
	int count = 0;

	FLOAT_TYPE minX = FLT_MAX;
	for (int i = 0; i < POL.size(); i++)
	{
		minX = std::min(minX, POL[i].x);
	}

	FLOAT_TYPE px = x;
	FLOAT_TYPE py = y;
	FLOAT_TYPE linePoint1x = x;
	FLOAT_TYPE linePoint1y = y;
	FLOAT_TYPE linePoint2x = minX - 10;			//取最小的X值还小的值作为射线的终点
	FLOAT_TYPE linePoint2y = y;

	FLOAT_TYPE cx1, cy1, cx2, cy2;

	//遍历每一条边
	for (int i = 0; i < POL.size(); i++)
	{
		if (i == 0) {
			cx1 = POL.rbegin()->x;
			cy1 = POL.rbegin()->y;
		}
		else {
			cx1 = POL[i - 1].x;
			cy1 = POL[i - 1].y;
		}
		cx2 = POL[i].x;
		cy2 = POL[i].y;

		if (IsPointOnLine(px, py, cx1, cy1, cx2, cy2))
		{
			return true;
		}

		if (fabs(cy2 - cy1) < FLT_EPSILON)   //平行则不相交
		{
			continue;
		}

		if (cy1 > cy2 && IsPointOnLine(cx1, cy1, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
		{
			count++;
		}
		else if (cy2 > cy1 && IsPointOnLine(cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
		{
			count++;
		}
		else if (IsIntersect(cx1, cy1, cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
		{
			count++;
		}
	}

	if (count % 2 == 1)
	{
		isInside = true;
	}

	return isInside;
}