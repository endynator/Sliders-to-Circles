#pragma once
#include <string>

using namespace std;

struct TimingPoint {
	int time;
	double beatLength;
	bool isInherited;

	string strTime;
	string strBeatLength;
	char isInheritedChar;
};


struct hitObject {
	hitObject(string x_, string y_, string time_, int type_) :
		x{ x_ }, y{ y_ }, time{ time_ }, type{ type_ } {}
	hitObject(const hitObject& other) :
		x{ other.x }, y{ other.y }, time{ other.time }, type{ other.type }, timeInt( other.timeInt ) {}
	hitObject() {}

	string x{};
	string y{};
	string time{};
	int type{};

	int timeInt{};
};

struct Circle : public hitObject {
	Circle(hitObject& base) :
		hitObject(base) {}
	Circle() {}

	string hitSound{};
	string hitSample{};

};

struct Slider : public hitObject {
	Slider(hitObject& base) :
		hitObject(base) {}
	Slider() {}

	string hitSound{};
	string curveType{};
	string curvePoints{};
	string slides{};
	string length{};
	string edgeSounds{};
	string edgeSets{};
	string hitSample{};
};

struct Spinner : public hitObject {
	Spinner(hitObject& base) :
		hitObject(base) {}
	Spinner() {}

	string hitSound{};
	string endTime{};
	string hitSample{};

};