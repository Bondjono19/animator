#pragma once

#include "CurveEvaluator.h"
#include "mat.h"
#include "vec.h"
//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};
static Vec4f multiplyRowVectorByMat4(const Vec4f& T, const Mat4f& M)
{
    Vec4f result;

    result[0] = T[0] * M[0][0] + T[1] * M[1][0] + T[2] * M[2][0] + T[3] * M[3][0];
    result[1] = T[0] * M[0][1] + T[1] * M[1][1] + T[2] * M[2][1] + T[3] * M[3][1];
    result[2] = T[0] * M[0][2] + T[1] * M[1][2] + T[2] * M[2][2] + T[3] * M[3][2];
    result[3] = T[0] * M[0][3] + T[1] * M[1][3] + T[2] * M[2][3] + T[3] * M[3][3];

    return result;
}