#include "BezierCurveEvaluator.h"
#include <assert.h>
#include "mat.h"
#include "vec.h"


void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int n = ptvCtrlPts.size();
	std::vector<Point> simCtlPts(ptvCtrlPts);
	ptvEvaluatedCurvePts.clear();
	if (n == 0) {
		return;
	}
	else if (n == 1) {
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[0]);
	}
	else if (n < 4) {
		ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	}

	//bernstein matrix
	Mat4f bM(
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	);
	//frequency of points per curve
	int freq = 100;

	if (bWrap) {
		if (ptvCtrlPts.size() >= 6 && ptvCtrlPts.size() % 3 == 0) {
			simCtlPts.push_back(Point(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y));
		}
	}
	int ctlPtsIterated = 0;
	for (int i = 0;i + 3 < simCtlPts.size();i += 3) {
		ctlPtsIterated+=3;
		for (int j = 0;j <= freq;j++) {
			float t = float(j) / float(freq);
			float t2 = t * t;
			float t3 = t * t * t;

			Vec4f intM = multiplyRowVectorByMat4(Vec4f(t3, t2, t, 1), bM);

			float x = intM[0] * simCtlPts[i].x + intM[1] * simCtlPts[i + 1].x + intM[2] * simCtlPts[i + 2].x + intM[3] * simCtlPts[i + 3].x;
			float y = intM[0] * simCtlPts[i].y + intM[1] * simCtlPts[i + 1].y + intM[2] * simCtlPts[i + 2].y + intM[3] * simCtlPts[i + 3].y;
			ptvEvaluatedCurvePts.push_back(Point(x, y));
		}
	}
	for (int i = ctlPtsIterated;i < ptvCtrlPts.size();i++) {
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
	}
	if (bWrap) {
		if (ptvCtrlPts.size() >= 6 && ptvCtrlPts.size() % 3 == 0) {
			std::vector<Point> offsetPoints;
			for (Point p : ptvEvaluatedCurvePts) {
				if (p.x > fAniLength) {
					Point nP = p;
					nP.x = p.x - fAniLength;
					offsetPoints.push_back(nP);
				}
			}
			for (int i = offsetPoints.size();i > 0;i--) {
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), offsetPoints[i - 1]);
			}

		}
		else {
			if (ptvEvaluatedCurvePts.size() == 0) {
				return;
			}
			Point lastP = ptvEvaluatedCurvePts.back();
			Point offsetP(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y);
			float slope = (offsetP.y - lastP.y) / (offsetP.x - lastP.x);
			float intercept = lastP.y - slope * lastP.x;
			float yVal = slope * fAniLength + intercept;
			ptvEvaluatedCurvePts.push_back(offsetP);
			ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point(0.0, yVal));
		}


	}
	else {
		Point adjustFirstP(0.0, ptvEvaluatedCurvePts[0].y);
		Point adjustLastP(fAniLength, ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size() - 1].y);
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), adjustFirstP);
		ptvEvaluatedCurvePts.push_back(adjustLastP);


	}
}
