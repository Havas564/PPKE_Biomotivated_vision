#pragma once
#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H

#include <iostream>
#include <stdlib.h>
#include <opencv2\core.hpp>

using namespace std;
using namespace cv;

class Classificator {
public:
	Classificator();
	~Classificator();

	Mat movementGradientDeterminer(vector<Mat> mainDirectionInformation);
	Mat edgeInformationSummarizer(vector<Mat> gaborFilterInformation);
	Mat contrastInformationSummarizer(vector<Mat> contrastInformation);
	Mat obstructionDetector(Mat movementGradient, Mat edgeInformation, Mat contrastInformaiton);
	vector<int> pathfinder(vector<Mat> prevoiusObstructionMaps);

	Mat getObstructionMap(void);
	vector<Mat> getPreviousObstructionMaps(void);
	int getMemoryMax(void);
	void setObstructionMap(Mat newMap);
	void setCurrentObstructionMaps(Mat newMap, int currentMemoryPosition);
	void setMemoryMax(int newValue);
	void initializeMemory(int iterator);

private:
	Mat obstructionMap;
	vector<Mat> previousObstructionMaps;
	int maxMemory;
};

#endif // !CLASSIFICATOR_H
