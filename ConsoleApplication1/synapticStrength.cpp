#include "synapticStrength.h"
#include "memoryHandling.h"

// constructor and destructor
SynapticStrength::SynapticStrength()
{
}

SynapticStrength::~SynapticStrength()
{
}

// creating modifier matrix
Mat SynapticStrength::modifierMatrixCalculator(vector<Mat> memory) {
	int iterator; //until iterator solved
	//Memory::memoryPosition(iterator);
	return modifierMatrix;
}

Mat SynapticStrength::synapticStrengthMatrixCreator(Mat modifierMatrix, Size memoryItemSize, bool isFirst) {
	Mat synapticStrengthMatrix; // might want to give size and type = Mat(memoryItemSize.height, memoryItemSize.width, CV_64FC1);
	if (isFirst) {
		synapticStrengthMatrix = Mat::ones(1, memoryItemSize.height * memoryItemSize.width, CV_64F);
		//debug
		SynapticStrength::sizeOfMatrix(synapticStrengthMatrix); //debug
		cout << "height: " << s.height << ", width: " << s.width << endl; //debug
	}
	else if(!isFirst){
		synapticStrengthMatrix.create(1, memoryItemSize.height * memoryItemSize.width, CV_64F);
		SynapticStrength::sizeOfMatrix(modifierMatrix);
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
	return modifierMatrix;
}

// assistant functions
Size SynapticStrength::sizeOfMatrix(Mat inputMatrix) {
	Size s = inputMatrix.size();
	return s;
}

