#pragma once
#ifndef ACCESSORYFUNCTIONS_H
#define ACCESSORYFUNCTIONS_H

#include <iostream>
#include <opencv2\core.hpp>

using namespace std;
using namespace cv;

class AccessoryFunctions {

public:
	AccessoryFunctions();
	~AccessoryFunctions();

	//variables
	Mat neededChannel;
	Size s;
	//functions - short descriptions in the .cpp file
	vector<Mat> kernelCreator(int centerSize, int peripherySize, int iterX, int iterY, Mat inputMatrix);
	Size sizeOfMatrix(Mat inputMatrix);
	double tressholding(double inputElement, int treshold);
	Mat colorChannelRetriever(Mat inputMatrix, string channelRequired); // function cutting out given color channel from rgb Mat


private:
};

#endif // !ACCESSORYFUNCTIONS_H
