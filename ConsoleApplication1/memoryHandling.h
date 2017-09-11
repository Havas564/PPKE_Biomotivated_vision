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
	//int iterator;
	int currentMemoryPosition;
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

	//SYNAPTIC STRENGTH MEMORY

	//CURRENT POSITION
	int memoryPosition(int iterator);

	//copyto-t hasznélni a MAT-ok betöltésénél
	// MEMORY INPUT FROM CURRENT ITERATION - körbuffert megnézni - kiválhatja a számolást
	vector<Mat> pushbackMemory(Mat currentInput, int currentMemoryPosition, vector<Mat> memoryType);
	vector<Mat> clearMemory(Mat currentInput, vector<Mat> memoryType);
	vector<Mat> pushbackRodMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackSConeMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackLConeMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackMConeMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackRodBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackSConeBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackRedOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackGreenOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackYellowOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackBlueOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition);
	vector<Mat> pushbackAmacrineIIMemory(Mat currentInput, int currentMemoryPosition);


private:
	int memoryMax = 5;
};

Memory::Memory()
{
}

Memory::~Memory()
{
}

#endif // MEMORYOFEVENTS_H
#pragma once
