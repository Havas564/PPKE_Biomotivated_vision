#pragma once
#ifndef RECEPTIVEFIELDPROCESSING_H
#define RECEPTIVEFIELDPROCESSING_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include "accessoryFunctions.h"
#include "memoryHandling.h"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class ReceptiveFieldFunctions
{
public:
	ReceptiveFieldFunctions();
	~ReceptiveFieldFunctions();
	//functions of receptive field proessing
	vector<Mat> displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize, int iterX, int iterY, string mainDriection);
	vector<Mat> displacementKernelCreatorNew(Mat inputMatrix, int kernelSize, int iterX, int iterY, string mainDirection);
	float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery);
	Mat homogenReceptiveFieldEvaluation(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> receptiveFieldEvaluationOneInput(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, vector<int> cellInformation,
		Memory& m, int mainIterator);
	vector<int> fovaeSizeAcquirer(Mat inputMatrix);
	vector<int> fovaeCenterCoordinatesAcquirer(Mat inputMatrix);
	Mat gaussianKernelCreator(int receptiveFieldSize, float sigma);
	Mat differenceOfGaussiansCreator(int receptiveFieldSize, float sigma1, float sigma2);
	Mat fovaeInformationAcquierer(Mat inputMatrix);
	Mat wholeInformationAcquierer(Mat inputMatrix);

	//set functions
	static vector<int> setKernelSize(int newValue);
	static vector<int> setCenterSize(int newValue);
	static vector<int> setPeripherySize(int newValue);
	static vector<int> setKernelStep(int newValue);
	static vector<int> setFovaeKernelSize(int newValue);
	static vector<int> setFovaeKernelStep(int newValue);
	static vector<int> setFocaeCenterSize(int newValue);
	static vector<int> setFovaeXAxisLength(int newValue);
	static vector<int> setFovaeYAxisLength(int newValue);
	void setFovaeXAxisPosition(int newValue);
	void setFovaeYAxisPosition(int newValue);


	//get functions
	static int getKernelSize();
	static int getCenterSize();
	static int getPeripherySize();
	static int getKernelStep();
	static int getFovaeKernelSize();
	static int getFovaeKernelStep();
	static int getFocaeCenterSize();
	static int getFovaeXAxisLength();
	static int getFovaeYAxisLength();
	static vector<int> getAllCellInformation();
	static int getFovaeXAxisPosition();
	static int getFovaeYAxisPosition();

protected:
	static vector<int> cellInformation;
private:
	const double PI = 3.141592653589793238463;
	
};

// Derived class
class RodBipolarProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	Mat RodBiploarProcessing(Mat inputMatrix, int mainIterator);
private:	
	vector<Mat> rodMemory;
	vector<Mat> rodMemoryDifference;
};

class amacrineAIIProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	Mat amacrineAIIBipolarProcessing(Mat inputMatrix, int mainIterator);
	
private:
	vector<Mat> rodBipolarMemory;
	vector<Mat> rodBipolarMemoryDifference;
};

//Derived class
class MidgetSConePathway : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	vector<Mat> midgetBipolarProcessing(Mat inputMatrix, int mainIterator);
private:
	vector<Mat> sConeMemory;
	vector<Mat> sConeMemoryDifference;
};

// Derived class
class RedGreenDiscrimination : public ReceptiveFieldFunctions{
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	vector<Mat> redGreenDiscriminationMain(Mat firstInputMatrix, Mat seconInputMatrix, int mainIterator);
private:
	vector<Mat> lConeMemory, mConeMemory;
	vector<Mat> lConeMemoryDifference, mConeMemoryDifference;
};

// Derived class
class YellowBlueDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	vector<Mat> yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, int mainIterator);
private:
	vector<Mat> yellowMemory, sConeMemory;
	vector<Mat> yellowMemoryDifference, sConeMemoryDifference;
};

// Derived class - intensity information from all cones
class AllConeDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> receptiveFieldEvaluationThreeInput(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, Memory& m, vector<int> cellInformation, int mainIterator);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	vector<Mat> allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, int mainIterator);

private:
	vector<Mat> allConeMemory;
	vector<Mat> sConeMemoryDifference, lConeMemoryDifference, mConeMemoryDifference;
};

class MainDirectionGanglionProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	//Mat previousInput(vector<Mat> cellMemory, int currentMemoryPosition);
	Mat previousInput(Memory& m, int currentMemoryPosition);
	int movementSensing(float prevois, float firstSide, float secondSide);
	int movementSensingNew(float previousValue, float firstSide, float secondSide, float kernelSize);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	void loadDifferenceToMemory(Memory& m);
	void loadDifferenceFromMemory(Memory& m);
	vector<Mat> directionReceptiveFieldProcessing(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> directionReceptiveFieldProcessingNew(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> mainDirectonInformation(Mat inputMatrix, int mainIterator);

private:
	vector<Mat> redGreenMemory, blueYellowMemory;
	vector<Mat> redGreenMemoryDifference, blueYellowMemoryDifference;
};

class GaborFiltering : public ReceptiveFieldFunctions {
public:
	void initializeGaborFilterParameters();
	void gaborFiltering(Mat inputMatrix);
	//get functions
	double getGamma(void);
	double getLambda(void);
	double getSigma(void);
	double getPsi(void);
	int getKernelSize(void);
	vector<Mat> getCurrentGaborFilterResult();
	//set functions
	void setTheta();
	void setGamma(double newValue);
	void setLambda(double newValue);
	void setSigma(double newValue);
	void setPsi(double newValue);
	void setKernelSize(int newValue);
private:
	double sigma, lambda, gamma, psi;
	vector<double> theta;
	int kernelSize = 31;
	vector<Mat> currentGaborFilterResult;
};

#endif // !RECEPTIVEFIELDPROCESSING_H
