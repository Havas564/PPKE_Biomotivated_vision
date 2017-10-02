#include "synapticStrength.h"
#include "memoryHandling.h"
#include "accessoryFunctions.h"

// constructor and destructor
SynapticStrength::SynapticStrength()
{
}

SynapticStrength::~SynapticStrength()
{
}


//bool checking if it is the first iteration of the program
bool SynapticStrength::isFirstIteration(int mainIterator) {
	bool isFirst;
	if (mainIterator == 1) {
		isFirst = true;
	}
	else {
		isFirst = false;
	}
	return isFirst;
}
// 
Mat SynapticStrength::synapticStrengthMatrixCreator(Mat modifierMatrix, Size memoryItemSize, bool isFirst) {
	AccessoryFunctions af;
	Mat synapticStrengthMatrix; // might want to give size and type = Mat(memoryItemSize.height, memoryItemSize.width, CV_64FC1);
	if (isFirst) {
		synapticStrengthMatrix = Mat::ones(1, memoryItemSize.height * memoryItemSize.width, CV_64F); // check, if CV_64F is the correct one
		//debug
		af.sizeOfMatrix(synapticStrengthMatrix); //debug
		cout << "height: " << s.height << ", width: " << s.width << endl; //debug
	}
	else if(!isFirst){
		synapticStrengthMatrix.create(1, memoryItemSize.height * memoryItemSize.width, CV_64F);
		af.sizeOfMatrix(modifierMatrix);
		for (int it = 0; it < s.height; it++) {
			for (int ij = 0; ij < s.width; ij++) {
				synapticStrengthMatrix.push_back(synapticStrengthMatrix.at<float>(1, it * ij) * 1 / (1 + exp(-1 * modifierMatrix.at<float>(it, ij))));
				//synapticStrengthMatrix.at<double>(it, ij);
			}
		}
	}
	else {
		cout << "Error: bool isFirst not initiated!" << endl;
	}
	return synapticStrengthMatrix;
}

// function creating the modifier matrix from memory
Mat SynapticStrength::modifierMatrixCalculator(vector<Mat> currentMemory, int currentMemoryPosition) {
	Memory m;
	Mat modifierMatrix;
	Size sizeOfMatrix = m.sizeOfMatrixInMemory(currentMemory);
	bool isFilled;
	if (sizeOfMatrix.width > 4)
	{
		isFilled = true;
	}
	if (isFilled == false) {
		vector<Mat> differenceToPrevious;
		int iter = 1;
		double sumOfExp;
		for (int it = currentMemory.size() - 1; it > 0; it--) {
			Mat temp;
			temp = currentMemory[it] - currentMemory[it + 1];
			differenceToPrevious.push_back(temp.clone()); // .clone() not sure if needed, not sure if this Mat needed at all, for now keeping for DEBUG purpuses
			if (it == currentMemory.size() - 1) {
				modifierMatrix = temp;
				sumOfExp = 1;
			}
			else {
				modifierMatrix = modifierMatrix + (temp*exp(-iter));
				sumOfExp = sumOfExp + exp(-iter);
			}
			iter++;
		}
		modifierMatrix = modifierMatrix / sumOfExp; // .divide() for array/array or scalar/array in theory
		return modifierMatrix;
	}
	else if (isFilled == true) {
		int iter = 1;
		double sumOfExp;
		//loading to straight memory - may need alternative like deque
		for (int it = 0; it < m.getMemoryMax() - 1; it++) {
			Mat temp;
			temp = currentMemory[(currentMemoryPosition - it - 1) % m.getMemoryMax()] - currentMemory[(currentMemoryPosition - it) % m.getMemoryMax()];
			if (it == 0) {
				modifierMatrix = temp;
				sumOfExp = 1;
			}
			else {
				modifierMatrix = modifierMatrix + (temp*exp(-iter));
				sumOfExp = sumOfExp + exp(-iter);
			}
			iter++;

		}
		modifierMatrix = modifierMatrix / sumOfExp; // may make sumOfExp a declared const - although it would fix max memory
		return modifierMatrix;
		//for (int it = currentMemoryPosition % 5; it < currentMemoryPosition + m.memoryMax; it = (it + 1) % 5)
	}
	else {
		cout << "Error: isFilled bool not initialized." << endl;
	}

	return modifierMatrix;
}

//set values
bool SynapticStrength::setIsFirst(bool newValue) {
	isFirst = newValue;
	return isFirst;
}
//get values
bool SynapticStrength::getIsFirst() {
	return isFirst;
}

