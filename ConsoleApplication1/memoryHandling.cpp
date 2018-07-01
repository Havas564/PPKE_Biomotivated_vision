#include "memoryHandling.h"

//constructor and destructor
Memory::Memory()
{
	//memory.resize(memoryMax); //create the vector sized as the memoryMax variable
}

Memory::~Memory()
{
}

// determine current memory position
int Memory::memoryPosition(int iterator) {
	currentMemoryPosition = iterator % memoryMax - 1;
	if (currentMemoryPosition == -1) {
		currentMemoryPosition = 4;
	}
	return currentMemoryPosition;
}


//INPUT FROM CURRENT ITERATION

// for all types
void Memory::pushbackMemory(Mat currentInput, int currentMemoryPosition, vector<Mat> memory) {
	memory[currentMemoryPosition] = currentInput;
	//memoryType.insert(memoryType.begin() + currentMemoryPosition, currentInput);
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

void Memory::setNumberOfMemoryBlocks(int newValue) {
	numberOfMemoryBlock = newValue;
}

void Memory::setMemoryMax(int newValue) {
	memoryMax = newValue;
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