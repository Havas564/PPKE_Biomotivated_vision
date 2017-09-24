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
	Size s;
	//functions - short descriptions in the .cpp file
	Size sizeOfMatrix(Mat inputMatrix);
	double thresholding(double inputElement, double treshold);
	Mat colorChannelRetriever(Mat inputMatrix, string channelRequired, Mat& neededChannel); // function cutting out given color channel from rgb Mat
	Mat conversionToRatio(Mat inputMatrix);
	Mat conversionToValue(Mat inputMatrix);

private:

};

#endif // !ACCESSORYFUNCTIONS_H
