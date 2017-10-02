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
	Mat oneKernelCreator(Mat inputMatrix, int kernelSize, int iterX, int iterY);
	vector<Mat> displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize, int iterX, int iterY, string mainDriection);
	float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery);
	Mat homogenReceptiveFieldEvaluation(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator);
	vector<Mat> receptiveFieldEvaluationOneInput(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator);
	vector<Mat> receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, vector<int> cellInformation,
		vector<Mat> firstCellMemory, vector<Mat> secondCellMemory, int mainIterator);
	vector<int> fovaeSizeAcquirer(Mat inputMatrix);
	vector<int> fovaeCenterCoordinatesAcquirer(Mat inputMatrix);
	
	//set functions
	//vector<int> setFovaeSize(int newValue, int position);
	//vector<int> getAllCellInformation();
	//vector<int> getAllFovaeSize();
private:
	
};

// Derived class
class RodBipolarProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	Mat RodBiploarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator);
	//set functions
	vector<int> setCellInformation(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	vector<int> getAllCellInformation();
private:
	vector<int> cellInformation;
};

class amacrineAIIProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	Mat amacrineAIIBipolarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator);
	//set functions
	vector<int> setCellInformation(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	vector<int> getAllCellInformation();
private:
	vector<int> cellInformation;
};

// Derived class
class RedGreenDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> redGreenDiscriminationMain(Mat firstInputMatrix, Mat seconInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator);
	//set functions
	vector<int> setCellInformation(int newValue, int position);
	vector<int> setFovaeSize(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	int getFovaeSize(int position);
	vector<int> getAllCellInformation();
	vector<int> getAllFovaeSize();


private:
	vector<int> cellInformation; //[kernelSize, centerSize, peripherySize, kernelStep, fovae_x_axis, fovae_y_axis]
	vector<int> fovaeSize;
	vector<int> fovaeCenterCoordinates;
};

// Derived class
class YellowBlueDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator);
	//set functions
	vector<int> setCellInformation(int newValue, int position);
	vector<int> setFovaeSize(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	int getFovaeSize(int position);
	vector<int> getAllCellInformation();
	vector<int> getAllFovaeSize();

private:
	vector<int> cellInformation; //[kernelSize, centerSize, peripherySize, kernelStep, fovae_x_axis, fovae_y_axis]
	vector<int> fovaeSize;
	vector<int> fovaeCenterCoordinates;
};

// Derived class
class AllConeDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory,
		vector<Mat> thridMemory, int mainIterator);
	//set functions
	vector<int> setCellInformation(int newValue, int position);
	vector<int> setFovaeSize(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	int getFovaeSize(int position);
	vector<int> getAllCellInformation();
	vector<int> getAllFovaeSize();

private:
	vector<int> cellInformation; //[kernelSize, centerSize, peripherySize, kernelStep, fovae_x_axis, fovae_y_axis]
	vector<int> fovaeSize;
	vector<int> fovaeCenterCoordinates;
};

class MainDirectionGanglionProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> mainDirectonInformation();
	//set functions
	vector<int>setCellInformation(int newValue, int position);
	vector<int>setFovaeSize(int newValue, int position);
	//get functions
	int getCellInformation(int position);
	int getFovaeInformation(int position);
	vector<int> getAllCellInformation();
	vector<int> getAllFovaeInformation();
private:
	vector<int> cellInformation;
	vector<int> fovaeSize;
	vector<int> fovaeCenterCoordinates;
};

#endif // !RECEPTIVEFIELDPROCESSING_H
