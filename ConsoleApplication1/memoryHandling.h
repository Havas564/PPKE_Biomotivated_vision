#pragma once
#ifndef MEMORYHANDLING_H
#define MEMORYHANDLING_H

#include <iostream>
#include <vector>
#include <opencv2\core.hpp>

using namespace std;
using namespace cv;

class Memory
{
public:
	//constructor & destructor
	Memory();
	~Memory();
	// INPUT MEMORY
	//photoreceptor level
	vector<Mat> rodMemory;
	vector<Mat> sConeMemory;
	vector<Mat> lConeMemory;
	vector<Mat> mConeMemory;
	//bipolar level
	vector<Mat> rodBipolarMemory;
	vector<Mat> sConeBipolarMemory;
	vector<Mat> redOpponencyBipolarMemory;
	vector<Mat> greenOpponencyMemory;
	vector<Mat> yellowOpponencyMemory;
	vector<Mat> blueOpponencyMemory;
	vector<Mat> amacrineIIMemory;
	//ganglion level
	vector<Mat> redOpponencyGanglionMemory;
	vector<Mat> greenOpponencyGanglionMemory;
	vector<Mat> intensityGanglionMemory;
	vector<Mat>	yellowOpponencyGanglionMemory;
	vector<Mat> blueOpponencyGanglionMemory;
	vector<Mat> onGreenRedMidgetOutputMemory;
	vector<Mat> rightMovementGanglionMemory;
	vector<Mat> leftMovementGanglionMemory;
	vector<Mat> upMovementGanglionMemory;
	vector<Mat> downMovementGanglionMemory;
	//high order processing
	vector<Mat> shapeDetectorLevel1;
	vector<Mat> shapeDetectorLevel2;
	vector<Mat> shapeDetectorLevel3;
	//SYNAPTIC STRENGTH MEMORY

	//copyto-t hasznélni a MAT-ok betöltésénél
	// MEMORY INPUT FROM CURRENT ITERATION - körbuffert megnézni - kiválhatja a számolást
	vector<Mat> pushbackMemory(Mat currentInput, int currentMemoryPosition, vector<Mat> memoryType);
	vector<Mat> clearMemory(Mat currentInput, vector<Mat> memoryType);
	Size sizeOfMatrixInMemory(vector<Mat> currentMemory);
	int filledMemorySpots(vector<Mat> currentMemory, bool isFilled);
	
	//get functions
	bool getIsFilled();
	int getNumberOfMemoryBlocks();
	int getMemoryMax();
	int memoryPosition(int iterator);
	//set functions
	bool setIsFilled(bool newValue);
	int setNumberOfMemoryBlocks(int newValue);
	int setMemoryMax(int newValue);

private:
	bool isFilled = false;
	int numberOfMemoryBlock;
	int memoryMax = 5;
	int currentMemoryPosition;
};


#endif // MEMORYOFEVENTS_H
#pragma once
