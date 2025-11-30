#include "BSplineCurveEvaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"


static std::vector<float> constructMatrix(std::vector<float> vals) {
	int freq = 20;
	std::vector<float> points;
	Mat4f bM(
		-1.0, 3.0, -3.0, 1.0,
		3.0, -6.0, 3.0, 0.0,
		-3.0, 3.0, 0, 0.0,
		1.0, 0.0, 0.0, 0.0
	);
	Mat4f M(
		1.0f, 4.0f, 1.0f, 0.0f,
		0.0f, 4.0f, 2.0f, 0.0f,
		0.0f, 2.0f, 4.0f, 0.0f,
		0.0f, 1.0f, 4.0f, 1.0f  
	);

	Vec4f CP(vals[0], vals[1], vals[2], vals[3]);
	Vec4f vec = M * CP;
	vec *= (1.0f / 6.0f);

	for (int j = 0;j <= freq;j++) {
		float t = float(j) / float(freq);
		float t2 = t * t;
		float t3 = t * t * t;

		Vec4f T(t3, t2, t, 1);
		Vec4f intM = T * bM;

		float point = intM[0] * vec[0] + intM[1] * vec[1] + intM[2] * vec[2] + intM[3] * vec[3];

		points.push_back(point);
	}

	return points;


}

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{

	ptvEvaluatedCurvePts.clear();

	std::vector<Point> ptvSimPts;
	ptvSimPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	if (bWrap) {
		ptvSimPts.push_back(Point(ptvSimPts[0].x + fAniLength, ptvSimPts[0].y));
		ptvSimPts.push_back(Point(ptvSimPts[1].x + fAniLength, ptvSimPts[1].y));
		ptvSimPts.push_back(Point(ptvSimPts[2].x + fAniLength, ptvSimPts[2].y));
	}
	else {
		Point fP = ptvCtrlPts[0];
		Point lP = ptvCtrlPts.back();
		ptvSimPts.insert(ptvSimPts.begin(), fP);
		ptvSimPts.insert(ptvSimPts.begin(), fP);
		ptvSimPts.push_back(lP);
		ptvSimPts.push_back(lP);
	}



	for (size_t i = 0;i + 3 < ptvSimPts.size();i++) {
		float inputX[] = { ptvSimPts[i].x,ptvSimPts[i + 1].x,ptvSimPts[i + 2].x,ptvSimPts[i + 3].x };
		std::vector<float> inX(inputX, inputX + 4);
		std::vector<float> outX = constructMatrix(inX);
		float inputY[] = { ptvSimPts[i].y,ptvSimPts[i + 1].y,ptvSimPts[i + 2].y,ptvSimPts[i + 3].y };
		std::vector<float> inY(inputY, inputY + 4);
		std::vector<float> outY = constructMatrix(inY);

		for (int j = 0;j < outX.size();j++) {
			ptvEvaluatedCurvePts.push_back(Point(outX[j], outY[j]));
		}
	}
	if (!bWrap) {
		float yValFirst = ptvEvaluatedCurvePts[0].y;
		float yValLast = ptvEvaluatedCurvePts.back().y;
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point(0.0, yValFirst));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, yValLast));
	}
	else {
		std::vector<Point> extraPoints;
		for (int i = 0;i < ptvEvaluatedCurvePts.size();i++) {
			if (ptvEvaluatedCurvePts[i].x >= 20) {
				extraPoints.push_back(Point(ptvEvaluatedCurvePts[i].x-fAniLength,ptvEvaluatedCurvePts[i].y));
			}
		}
		for (int i = extraPoints.size() - 1;i >= 0;i--) {
			ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), extraPoints[i]);
		}
	}

}