#pragma once
#include "Structs.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>

double multiplier{};

bool isNewComboObjectASlider(const string& line) {
	return line.find('B') != string::npos ||
		line.find('C') != string::npos ||
		line.find('L') != string::npos ||
		line.find('P') != string::npos;
}

void buildCircle(Circle& circle, stringstream& ss) {
	std::getline(ss, circle.hitSound, ',');
	std::getline(ss, circle.hitSample, '\r');
	return;
}

void buildSlider(Slider& slider, stringstream& ss) {
	std::getline(ss, slider.hitSound, ',');
	std::getline(ss, slider.curveType, '|');
	std::getline(ss, slider.curvePoints, ',');
	std::getline(ss, slider.slides, ',');
	std::getline(ss, slider.length, ',');
	std::getline(ss, slider.edgeSounds, ',');
	std::getline(ss, slider.edgeSets, ',');
	std::getline(ss, slider.hitSample, '\r');
	return;
}

void parse(
	ifstream& file,
	vector<TimingPoint>& tps,
	vector<Circle>& circles,
	vector<Slider>& sliders,
	vector<Spinner>& spinners
) {
	
	string line;
	bool inTimingPointsSection = false, inHitObjectsSection = false,
		haveMiltiplier = false, parsedTiming = false;

	while (std::getline(file, line)) {
		if (!haveMiltiplier) {
			if (line.find("SliderMultiplier") != string::npos) {
				stringstream ss(line);
				string s{};
				std::getline(ss, s, ':');
				std::getline(ss, s);
				multiplier = stod(s);
				haveMiltiplier = true;
				continue;
			}
		}
		if (line.find("[HitObjects]") != string::npos) {
			inHitObjectsSection = true;
			continue;
		}
		if (line.find("[TimingPoints]") != string::npos) {
			inTimingPointsSection = true;
			continue;
		}
		if (inTimingPointsSection) {
			if (line != "\r") {
				stringstream ss(line);
				TimingPoint tp;
				tp.isInheritedChar = ss.str()[ss.str().length() - 4];
				std::getline(ss, tp.strTime, ',');
				std::getline(ss, tp.strBeatLength, ',');

				tp.time = stoi(tp.strTime);
				tp.beatLength = stod(tp.strBeatLength);
				tp.isInherited = tp.isInheritedChar == '0';

				tps.push_back(tp);
			}
			else {
				inTimingPointsSection = false;
			}
			continue;
		}
		if (inHitObjectsSection) {
			stringstream ss(line);
			hitObject* base = new hitObject;
			string type;
			std::getline(ss, base->x, ',');
			std::getline(ss, base->y, ',');
			std::getline(ss, base->time, ',');
			std::getline(ss, type, ',');
			base->type = stoi(type);
			base->timeInt = stoi(base->time);

			switch (base->type)
			{
			case 1: { // Circle noNewCombo
				Circle circle(*base);
				buildCircle(circle, ss);
				circles.push_back(circle);
				break;
			}
			case 2: { // Slider noNewCombo
				Slider slider(*base);
				buildSlider(slider, ss);
				sliders.push_back(slider);
				break;
			}
			case 3: {
				Spinner spinner(*base);
				std::getline(ss, spinner.hitSound, ',');
				std::getline(ss, spinner.endTime, ',');
				std::getline(ss, spinner.hitSample, '\r');
				spinners.push_back(spinner);
				break;
			}
			default:
				if (isNewComboObjectASlider(line)) {
					Slider slider(*base);
					buildSlider(slider, ss);
					sliders.push_back(slider);
					break;
				}
				else {
					Circle circle(*base);
					buildCircle(circle, ss);
					circles.push_back(circle);
					break;
				}
				break;
			}
			delete base;
		}
	}
	file.close();
}

