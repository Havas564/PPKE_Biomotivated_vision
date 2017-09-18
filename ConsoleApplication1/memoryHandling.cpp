#include "memoryHandling.h"

//constructor and destructor
Memory::Memory()
{
}

Memory::~Memory()
{
}

// determine current memory position
int Memory::memoryPosition(int iterator) {
	if (iterator == 1) {
		currentMemoryPosition = 0;
	}
	currentMemoryPosition = iterator % memoryMax;
	if (currentMemoryPosition == 0) {
		currentMemoryPosition = 0;
	}
	return currentMemoryPosition;
}


//INPUT FROM CURRENT ITERATION

// for all types
vector<Mat> Memory::pushbackMemory(Mat currentInput, int currentMemoryPosition, vector<Mat> memoryType) {
	memoryType.insert(memoryType.begin() + currentMemoryPosition, currentInput);
	return memoryType;
}

//CLEAR MEMORY
vector<Mat> Memory::clearMemory(Mat currentInput, vector<Mat> memoryType) {
	memoryType.clear();
	return memoryType;
}

//SIZE OF MEMORY MATRIX
Size Memory::sizeOfMatrixInMemory(vector<Mat> currentMemory) {
	Size s = (currentMemory.front()).size();
	return s;
}

// INPUT FOR SYNAPTIC STRENGTH MATRIX



// PHOTORECEPTOR LEVEL
/*
vector<Mat> Memory::pushbackRodMemory(Mat currentInput, int currentMemoryPosition) {
	rodMemory.insert(rodMemory.begin() + currentMemoryPosition, currentInput);
	return rodMemory;
}

vector<Mat> Memory::pushbackSConeMemory(Mat currentInput, int currentMemoryPosition) {
	sConeMemory.insert(sConeMemory.begin() + currentMemoryPosition, currentInput);
	return sConeMemory;
}

vector<Mat> Memory::pushbackLConeMemory(Mat currentInput, int currentMemoryPosition) {
	lConeMemory.insert(lConeMemory.begin() + currentMemoryPosition, currentInput);
	return lConeMemory;
}

vector<Mat> Memory::pushbackMConeMemory(Mat currentInput, int currentMemoryPosition) {
	mConeMemory.insert(mConeMemory.begin() + currentMemoryPosition, currentInput);
	return mConeMemory;
}

// BIPOLAR LEVEL
vector<Mat> Memory::pushbackRodBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	rodBipolarMemory.insert(rodBipolarMemory.begin() + currentMemoryPosition, currentInput);
	return rodBipolarMemory;
}

vector<Mat> Memory::pushbackSConeBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	sConeBipolarMemory.insert(sConeBipolarMemory.begin() + currentMemoryPosition, currentInput);
	return sConeBipolarMemory;
}

vector<Mat> Memory::pushbackRedOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	redOpponencyBipolarMemory.insert(redOpponencyBipolarMemory.begin() + currentMemoryPosition, currentInput);
	return redOpponencyBipolarMemory;
}

vector<Mat> Memory::pushbackGreenOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	greenOpponencyMemory.insert(greenOpponencyMemory.begin() + currentMemoryPosition, currentInput);
	return greenOpponencyMemory;
}

vector<Mat> Memory::pushbackYellowOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	yellowOpponencyMemory.insert(yellowOpponencyMemory.begin() + currentMemoryPosition, currentInput);
	return yellowOpponencyMemory;
}

vector<Mat> Memory::pushbackBlueOpponencyBipolarMemory(Mat currentInput, int currentMemoryPosition) {
	blueOpponencyMemory.insert(blueOpponencyMemory.begin() + currentMemoryPosition, currentInput);
	return blueOpponencyMemory;
}

vector<Mat> Memory::pushbackAmacrineIIMemory(Mat currentInput, int currentMemoryPosition) {
	amacrineIIMemory.insert(amacrineIIMemory.begin() + currentMemoryPosition, currentInput);
	return amacrineIIMemory;
}
*/
// GANGLION LEVEL




// determine number of block in current memory - UNNECESSARY - SAVED FOR FUTURE USE
/*int Memory::filledMemorySpots(vector<Mat> currentMemory, bool isFilled) {
if (isFilled) {
numberOfMemoryBlock = 5;
}
else if (!isFilled) {

if (numberOfMemoryBlock == 5) {
isFilled = true;
}
}
else {
cout << "Error: isFilled not initialized" << endl;
}
}*/