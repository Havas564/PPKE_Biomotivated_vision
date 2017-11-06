#include "classificator.h"

//constructor and destructor
Classificator::Classificator() {

}

Classificator::~Classificator() {

}

//functions of classificator
Mat Classificator::movementGradientDeterminer(vector<Mat> mainDirectionInformation) {
	Mat placeHolder;
	return placeHolder;
}

Mat Classificator::edgeInformationSummarizer(vector<Mat> gaborFilterInformation) {
	Mat placeHolder;
	return placeHolder;
}

Mat Classificator::contrastInformationSummarizer(vector<Mat> contrastInformation) {
	Mat placeHolder;
	return placeHolder;
}

Mat Classificator::obstructionDetector(Mat movementGradient, Mat edgeInformation, Mat contrastInformaiton) {
	Mat placeHolder;
	return placeHolder;
}

vector<int> Classificator::pathfinder(vector<Mat> prevoiusObstructionMaps) {
	vector<int> placeHolder;
	return placeHolder;
}

// get and set functions
Mat Classificator::getObstructionMap(void) {
	return obstructionMap;
}

vector<Mat> Classificator::getPreviousObstructionMaps(void) {
	return previousObstructionMaps;
}

int Classificator::getMemoryMax(void) {
	return maxMemory;
}

void Classificator::setObstructionMap(Mat newMap) {
	obstructionMap = newMap;
}

void Classificator::setCurrentObstructionMaps(Mat newMap, int currentMemoryPosition) {
	previousObstructionMaps[currentMemoryPosition] = newMap;
}

void Classificator::setMemoryMax(int newValue) {
	maxMemory = newValue;
}

void Classificator::initializeMemory(int iterator) {
	if (iterator < maxMemory) {
		previousObstructionMaps.resize(iterator);
	}else {
		previousObstructionMaps.resize(maxMemory);
	}
}
