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
	static vector<int> setKernelSize(int newValue);
	static vector<int> setCenterSize(int newValue);
	static vector<int> setPeripherySize(int newValue);
	static vector<int> setKernelStep(int newValue);
	static vector<int> setFovaeKernelSize(int newValue);
	static vector<int> setFovaeKernelStep(int newValue);
	static vector<int> setFocaeCenterSize(int newValue);
	static vector<int> setFovaeXAxis(int newValue);
	static vector<int> setFovaeYAxis(int newValue);

	//get functions
	static int getKernelSize();
	static int getCenterSize();
	static int getPeripherySize();
	static int getKernelStep();
	static int getFovaeKernelSize();
	static int getFovaeKernelStep();
	static int getFocaeCenterSize();
	static int getFovaeXAxis();
	static int getFovaeYAxis();
	static vector<int> getAllCellInformation();

protected:
	static vector<int> cellInformation;
private:
	
};

class HasFovae
{
public:

protected:
	static vector<int> fovaeSize;
	static vector<int> fovaeCenterCoordinates;
};

// Derived class
class RodBipolarProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	Mat RodBiploarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator);
private:
	
};

class amacrineAIIProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	Mat amacrineAIIBipolarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator);
private:
	//vector<int> cellInformation;
};

// Derived class
class RedGreenDiscrimination : public ReceptiveFieldFunctions, HasFovae{
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> redGreenDiscriminationMain(Mat firstInputMatrix, Mat seconInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator);

private:

};

// Derived class
class YellowBlueDiscrimination : public ReceptiveFieldFunctions, HasFovae {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator);

private:

};

// Derived class - intensity information from all cones
class AllConeDiscrimination : public ReceptiveFieldFunctions, HasFovae {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory,
		vector<Mat> thirdMemory, vector<int> cellInformation, int mainIterator);
	vector<Mat> allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory,
		vector<Mat> thridMemory, int mainIterator);

private:

};

class MainDirectionGanglionProcessing : public ReceptiveFieldFunctions, HasFovae {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	Mat previousInput(vector<Mat> cellMemory, int currentMemoryPosition);
	int movementSensing(double prevois, double firstSide, double secondSide);
	vector<Mat> directionReceptiveFieldProcessing(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator);
	vector<Mat> mainDirectonInformation(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator);

private:

};

class GaborFiltering : public ReceptiveFieldFunctions {
public:
	vector<double> initializeGaborFilterParameters();

	//get functions
	double getGamma();
	double getLambda();
	double getSigma();
	double getPsi();
	//set functions
	vector<double> setTheta();
	double setGamma(double newValue);
	double setLambda(double newValue);
	double setSigma(double newValue);
	double setPsi(double newValue);
private:
	double sigma, lambda, gamma, psi;
	vector<double> theta;
};

#endif // !RECEPTIVEFIELDPROCESSING_H
