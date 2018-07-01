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
Mat SynapticStrength::synapticStrengthMatrixCreator(Mat modifierMatrix, Size memoryItemSize, bool isFirst)
{
	AccessoryFunctions af;
	 // might want to give size and type = Mat(memoryItemSize.height, memoryItemSize.width, CV_64FC1);
	if (isFirst)
	{
		//synapticStrengthMatrix = Mat::ones(1, memoryItemSize.height * memoryItemSize.width, CV_64F); // check, if CV_64F is the correct one
		Mat synapticStrengthMatrix;
		synapticStrengthMatrix = Mat::ones(memoryItemSize.height, memoryItemSize.width, CV_32F);
		return synapticStrengthMatrix;
	}
	else
	{
		Mat synapticStrengthMatrix(memoryItemSize.height, memoryItemSize.width, CV_32F);
		Size s = af.sizeOfMatrix(modifierMatrix);
		Mat testMatrix;
		float testModifier;
		//testModifier = 1 / (1 + exp(-1 * ))
		for (int it = 0; it < s.height; it++)
		{
			for (int ij = 0; ij < s.width; ij++)
			{
				synapticStrengthMatrix.at<float>(it, ij) = 1 / (1 + exp(-1 * modifierMatrix.at<float>(it, ij)));
				//synapticStrengthMatrix.push_back(synapticStrengthMatrix.at<float>(it, ij) * 1 / (1 + exp(-1 * modifierMatrix.at<float>(it, ij))));
				//synapticStrengthMatrix.push_back(synapticStrengthMatrix.at<float>(1, it * ij) * 1 / (1 + exp(-1 * modifierMatrix.at<float>(it, ij))));
				//synapticStrengthMatrix.at<double>(it, ij);
				
			}
		}
		return synapticStrengthMatrix;
	}
	
}

Mat SynapticStrength::synapticStrengthMatrixCreatorNew(Mat modifierMatrix, Size memoryItemSize, bool isFirst)
{
	AccessoryFunctions af;
	Mat synapticStrengthMatrix(memoryItemSize.height, memoryItemSize.width, CV_32F);
	Size s = af.sizeOfMatrix(modifierMatrix);
	Mat testMatrix;
	float testModifier;
	//Mat multiplier = Mat::ones(memoryItemSize.height, memoryItemSize.width, CV_32F);
	for (int it = 0; it < s.height; it++)
	{
		for (int ij = 0; ij < s.width; ij++)
		{
			synapticStrengthMatrix.at<float>(it, ij) = 1 / (1 + exp(-1 * modifierMatrix.at<float>(it, ij)));
		}
	}
	return synapticStrengthMatrix;
}

// function creating the modifier matrix from memory
Mat SynapticStrength::modifierMatrixCalculator(vector<Mat> currentMemory, int mainIterator) {
	Memory m;
	Mat modifierMatrix;
	Size sizeOfMatrix = m.sizeOfMatrixInMemory(currentMemory);
	bool isFilled = m.getIsFilled();
	if (currentMemory.size() > 4)
	{
		isFilled = true;
	}
	if (isFilled == false)
	{
		vector<Mat> differenceToPrevious;
		int iter = 1;
		double sumOfExp;
		for (int it = currentMemory.size() - 1; it > 0; it--)
		{
			Mat temp;
			temp = currentMemory[it] - currentMemory[it - 1];
			differenceToPrevious.push_back(temp.clone()); // .clone() not sure if needed, not sure if this Mat needed at all, for now keeping for DEBUG purpuses
			if (it == currentMemory.size() - 1)
			{
				modifierMatrix = temp;
				sumOfExp = 1;
			}
			else
			{
				modifierMatrix = modifierMatrix + (temp*exp(-iter));
				sumOfExp = sumOfExp + exp(-iter);
			}
			iter++;
		}
		modifierMatrix = modifierMatrix / sumOfExp; // .divide() for array/array or scalar/array in theory
		return modifierMatrix;
	}
	else if (isFilled == true)
	{
		int iter = 1;
		double sumOfExp;
		//loading to straight memory - may need alternative like deque
		for (int it = 0; it < m.getMemoryMax() - 1; it++)
		{
			Mat temp;
			// recreate the line below - it calculates wrong result
			temp = abs(currentMemory[(mainIterator - it - 1) % m.getMemoryMax()] - currentMemory[(mainIterator - it) % m.getMemoryMax()]);
			if (it == 0)
			{
				modifierMatrix = temp;
				sumOfExp = 1;
			}
			else
			{
				modifierMatrix = modifierMatrix + (temp*exp(-iter));
				sumOfExp = sumOfExp + exp(-iter);
			}
			iter++;
		}
		modifierMatrix = modifierMatrix / sumOfExp; // may make sumOfExp a declared const - although it would fix max memory
		return modifierMatrix;
		//for (int it = currentMemoryPosition % 5; it < currentMemoryPosition + m.memoryMax; it = (it + 1) % 5)
	}
	else
	{
		cout << "Error: isFilled bool not initialized." << endl;
	}

	return modifierMatrix;
}


Mat SynapticStrength::modifierMatrixCalculatorNew(vector<Mat> memory, int currentMemoryPosition)
{
	Memory m;
	Mat modifierMatrix;
	Size sizeOfMatrix = m.sizeOfMatrixInMemory(memory);

	int iter = 1;
	double sumOfExp;
	for (int it = 0; it < m.getMemoryMax() - 1; it++)
	{
		Mat temp;
		temp = memory[(currentMemoryPosition - it - 1) % m.getMemoryMax()] - memory[(currentMemoryPosition - it) % m.getMemoryMax()];
		if (it == 0)
		{
			modifierMatrix = temp;
			sumOfExp = 1;
		}
		else
		{
			modifierMatrix = modifierMatrix + (temp*exp(-iter));
			sumOfExp = sumOfExp + exp(-1 * iter);
		}
		iter++;
	}
	modifierMatrix = modifierMatrix / sumOfExp; // may make sumOfExp a declared const - although it would fix max memory
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

