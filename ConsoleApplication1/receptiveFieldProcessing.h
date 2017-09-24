#pragma once
#ifndef RECEPTIVEFIELDPROCESSING_H
#define RECEPTIVEFIELDPROCESSING_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <opencv2\core.hpp>
#include "accessoryFunctions.h"

using namespace std;
using namespace cv;

class ReceptiveFieldFunctions
{
public:
	ReceptiveFieldFunctions();
	~ReceptiveFieldFunctions();
	//functions of receptive field proessing
	bool onOff(string cellType);
	vector<Mat> kernelCreator(int centerSize, int peripherySize, int iterX, int iterY, Mat inputMatrix);
	float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery);
	vector<Mat> receptorFieldEvaluationOneInput(Mat inputMatrix, bool onCell, vector<int> cellInformation, 
		vector<Mat> cellMemory, int mainIterator); // think over cellinformation vector
	vector<Mat> receptorFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, bool onCell, 
		vector<int> cellInformation, vector<Mat> firstCellMemory, vector<Mat> secondCellMemory, int mainIterator);
	vector<int> fovaeSizeAcquirer(Mat inputMatrix);

private:
	vector<int> cellInformation; //[kernelSize, centerSize, peripherySize, kernelStep, fovae_x_axis, fovae_y_axis]
	vector<int> fovaeSize;
};


#endif // !RECEPTIVEFIELDPROCESSING_H
