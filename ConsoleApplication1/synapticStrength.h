#pragma once
#ifndef SYNAPTICSTRENGTH_H
#define SYNAPTICSTRENGTH_H

#include <iostream>
#include <math.h>
#include <opencv2\core.hpp>

using namespace std;
using namespace cv;

class SynapticStrength {

public:
	SynapticStrength();
	~SynapticStrength();
	//declaring variables
	Mat modifierMatrix;
	Size s;
	//main functions
	Mat synapticStrengthMatrixCreator(Mat modifierMatrix, Size memoryItemSize, bool isFirst);
	Mat modifierMatrixCalculator(vector<Mat> memory, int currentMemoryPosition);
	//assisting functions
	

private:

};





#endif
#pragma once
