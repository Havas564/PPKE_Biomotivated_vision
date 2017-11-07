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
	bool onOff(string cellType);
	vector<Mat> kernelCreator(int centerSize, int peripherySize, int iterX, int iterY, Mat inputMatrix);
	Mat oneKernelCreator(Mat inputMatrix, int kernelSize, int iterX, int iterY);
	vector<Mat> displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize, int iterX, int iterY, string mainDriection);
	float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery);
	Mat homogenReceptiveFieldEvaluation(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> receptiveFieldEvaluationOneInput(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, vector<int> cellInformation,
		Memory& m, int mainIterator);
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
	
};

// Derived class
class RodBipolarProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	Mat RodBiploarProcessing(Mat inputMatrix, int mainIterator);
private:	
	vector<Mat> rodMemory;
};

class amacrineAIIProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation();
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	Mat amacrineAIIBipolarProcessing(Mat inputMatrix, int mainIterator);
	
private:
	vector<Mat> rodBipolarMemory;
};

// Derived class
class RedGreenDiscrimination : public ReceptiveFieldFunctions{
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	vector<Mat> redGreenDiscriminationMain(Mat firstInputMatrix, Mat seconInputMatrix, int mainIterator);
private:
	vector<Mat> lConeMemory, mConeMemory;
};

// Derived class
class YellowBlueDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	vector<Mat> yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, int mainIterator);
private:
	vector<Mat> yellowMemory, sConeMemory;
};

// Derived class - intensity information from all cones
class AllConeDiscrimination : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	vector<Mat> receptiveFieldEvaluationThreeInput(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, Memory& m, vector<int> cellInformation, int mainIterator);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	vector<Mat> allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, int mainIterator);

private:
	vector<Mat> sConeMemory, lConeMemory, mConeMemory;
};

class MainDirectionGanglionProcessing : public ReceptiveFieldFunctions {
public:
	vector<int> initializeCellInformation(Mat inputMatrix);
	Mat previousInput(vector<Mat> cellMemory, int currentMemoryPosition);
	int movementSensing(float prevois, float firstSide, float secondSide);
	void initializeMemory(int iterator);
	void loadToMemory(Memory& m);
	void loadFromMemory(Memory& m);
	vector<Mat> directionReceptiveFieldProcessing(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator);
	vector<Mat> mainDirectonInformation(Mat inputMatrix, vector<int> cellInformation, int mainIterator);

private:
	vector<Mat> redGreenMemory, blueYellowMemory;
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
