#include "Calculations.h"
#include <algorithm>

int main() {

	string inputFileName{};
	cout << "Input file name: " << endl;
	std::getline(cin, inputFileName);
	string outputFileName{ inputFileName };
	outputFileName.insert(outputFileName.length() - 4, "[noSliders]");
	string content{}, delimiter{ "[HitObjects]" }, copyStr{ "Version" };


	ifstream file{ inputFileName, std::ios::binary };
	ofstream outputFile{ outputFileName, std::ios::binary };

	while (std::getline(file, content)) {
		if (content.find(delimiter) != std::string::npos) {
			outputFile << content;
			break;
		}
		if (content.find(copyStr) != std::string::npos) {
			stringstream ss{ content };
			string version{};
			std::getline(ss, version, ':');
			std::getline(ss, version, '\r');
			outputFile << copyStr + ':' << version << "[noSliders]\n";
			continue;
		}
		outputFile << content;
	}	
	file.close();

	ifstream inputFile{ inputFileName, std::ios::binary };

	vector<TimingPoint> timingPoints;

	vector<Circle> circles;
	vector<Slider> sliders;
	vector<Spinner> spinners;

	parse(inputFile, timingPoints, circles, sliders, spinners);

	for (const auto& slider : sliders) {
		convertSliderToCircles(slider, timingPoints, circles);
	}

	sort(circles.begin(), circles.end(), [](const Circle& other1, const Circle& other2)->bool {
		return other1.timeInt < other2.timeInt;
		});

	for (const auto& circle : circles) {
		for (const auto& spinner : spinners) {
			if (spinner.timeInt < circle.timeInt) {
				outputFile << spinner.x << ',' << spinner.y << ',' << spinner.type << spinner.hitSound
					<< spinner.endTime << spinner.hitSample << endl;
			}
		}
		outputFile << circle.x << "," << circle.y << "," << circle.time << "," << ((circle.type == 1) ? '1' : '5') << ","
			<< circle.hitSound << "," << circle.hitSample << endl;
	}
	
	cout << "Conversion completed. Please check the output file." << endl;

	outputFile.close();
}