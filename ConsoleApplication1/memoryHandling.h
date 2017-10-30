#pragma once
#ifndef MEMORYHANDLING_H
#define MEMORYHANDLING_H

#include <iostream>
#include <vector>
#include <opencv2\core.hpp>
//#include "receptiveFieldProcessing.h"

using namespace std;
using namespace cv;

class Memory
{
public:
	//constructor & destructor
	Memory();
	~Memory();
	// INPUT MEMORY
	vector<Mat> memory; // main memory modifier
	vector<Mat> memorySecond; // second memory modifier in case of two input memories
	vector<Mat> memoryThird; // third memory modifier in case of three input memories

	//SYNAPTIC STRENGTH MEMORY

	//copyto-t hasznélni a MAT-ok betöltésénél
	// MEMORY INPUT FROM CURRENT ITERATION - körbuffert megnézni - kiválhatja a számolást
	void pushbackMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> clearMemory(Mat currentInput, vector<Mat> memoryType);
	Size sizeOfMatrixInMemory(vector<Mat> currentMemory);
	
	//get functions
	bool getIsFilled();
	int getNumberOfMemoryBlocks();
	int getMemoryMax();
	int memoryPosition(int iterator);
	//set functions
	bool setIsFilled(bool newValue);
	void setNumberOfMemoryBlocks(int newValue);
	void setMemoryMax(int newValue);

private:
	bool isFilled = false;
	int numberOfMemoryBlock;
	int memoryMax = 5;
	int currentMemoryPosition;
};


#endif // MEMORYOFEVENTS_H
#pragma once
