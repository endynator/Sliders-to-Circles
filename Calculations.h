#pragma once
#include "Structs.h"
#include "Parser.h"
#include <vector>
#include <utility>
#include <cmath>
#include <sstream>

std::pair<TimingPoint*, TimingPoint*> findClosestTimingPoints(const std::vector<TimingPoint>& timingPoints, int x) {
	TimingPoint* closestNonInherited = nullptr;
	TimingPoint* closestInherited = nullptr;
	int minNonInheritedDist = std::numeric_limits<int>::max();
	int minInheritedDist = std::numeric_limits<int>::max();

	for (const TimingPoint& tp : timingPoints) {
		if (tp.time <= x) {
			int dist = std::abs(tp.time - x);
			if (tp.isInherited) {
				if (dist < minInheritedDist) {
					minInheritedDist = dist;
					closestInherited = const_cast<TimingPoint*>(&tp);
				}
			}
			else {
				if (dist < minNonInheritedDist) {
					minNonInheritedDist = dist;
					closestNonInherited = const_cast<TimingPoint*>(&tp);
				}
			}
		}
	}

	return std::make_pair(closestNonInherited, closestInherited);
}

int findClosestCircle(const std::vector<Circle>& timingPoints, int x) {
	int minDist = std::numeric_limits<int>::max();

	for (const Circle& tp : timingPoints) {
		if (tp.timeInt <= x) {
			int dist = std::abs(tp.timeInt - x);
			if (dist < minDist) {
				minDist = dist;
			}
		}
	}

	return minDist;
}

int calculateSliderDuration(const Slider& slider,
	const vector<TimingPoint>& timingPoints) {
	auto closestTimingPoints = findClosestTimingPoints(timingPoints, slider.timeInt);

	double SV = (closestTimingPoints.first->time > closestTimingPoints.second->time) ? 1.0 :
		abs(1 / closestTimingPoints.second->beatLength) * 100;

	double sliderDuration = stod(slider.length) / (multiplier * 100 * SV)
		* closestTimingPoints.first->beatLength;

	return round(sliderDuration);
}

void convertSliderToCircles(const Slider& slider,
	const vector<TimingPoint>& timingPoints,
	vector<Circle>& convertedCircles)
{

	int slides = stoi(slider.slides) + 1; // Number of circles to create from the slider

	double beatLength = calculateSliderDuration(slider, timingPoints);

	stringstream curvePointStream(slider.curvePoints);
	string lastCurvePoint;
	while (std::getline(curvePointStream, lastCurvePoint, '|')) {
		// For each curve point, we keep updating lastCurvePoint until we reach the last one
	}

	for (int i = 0; i < slides; ++i) {
		Circle circle;
		if (i % 2 == 0) {
			circle.x = slider.x;
			circle.y = slider.y;
		}
		else {
			stringstream lastCurvePointStream(lastCurvePoint);
			std::getline(lastCurvePointStream, circle.x, ':');
			std::getline(lastCurvePointStream, circle.y, ':');
		}
		circle.time = to_string(slider.timeInt + i * calculateSliderDuration(slider, timingPoints));
		circle.timeInt = stoi(circle.time);
		circle.type = (i == 0) ? (slider.type == 2) ? 1 : 5 : 1;
		circle.hitSound = slider.hitSound;
		circle.hitSample = "0:0:0:0:"; // Default hitSample
		convertedCircles.push_back(circle);
		//convertedCircles.insert(convertedCircles.begin() + findClosestCircle(convertedCircles, circle.timeInt), circle);
	}
}

