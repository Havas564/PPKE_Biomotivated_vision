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


// GANGLION LEVEL

//get functions
bool Memory::getIsFilled() {
	return isFilled;
}

int Memory::getNumberOfMemoryBlocks() {
	return numberOfMemoryBlock;
}

int Memory::getMemoryMax() {
	return memoryMax;
}

//set functions
bool Memory::setIsFilled(bool newValue) {
	isFilled = newValue;
	return isFilled;
}

int Memory::setNumberOfMemoryBlocks(int newValue) {
	numberOfMemoryBlock = newValue;
	return numberOfMemoryBlock;
}

int Memory::setMemoryMax(int newValue) {
	memoryMax = newValue;
	return memoryMax;
}



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