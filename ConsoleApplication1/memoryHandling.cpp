#include "memoryHandling.h"

// determine current memory position
int Memory::memoryPosition(int iterator) {
	currentMemoryPosition = iterator % memoryMax;
	if (currentMemoryPosition == 0) {
		currentMemoryPosition = memoryMax;
	}
	return currentMemoryPosition;
}


//INPUT FROM CURRENT ITERATION

// for all types
vector<Mat> Memory::pushbackMemory(Mat currentInput, int currentMemoryPosition, vector<Mat> memoryType) {
	memoryType.insert(memoryType.begin() + currentMemoryPosition, currentInput);
	return memoryType;
}

//CLEAN MEMORY
vector<Mat> Memory::clearMemory(Mat currentInput, vector<Mat> memoryType) {
	memoryType.clear();
	return memoryType;
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
