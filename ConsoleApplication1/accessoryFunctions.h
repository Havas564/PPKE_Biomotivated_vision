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
	float thresholding(float inputElement, float treshold);
	Mat colorChannelRetriever(Mat inputMatrix, string channelRequired, Mat& neededChannel); // function cutting out given color channel from rgb Mat
	vector<Mat> colorChannelRetrieverAlternate(Mat inputMatrix);
	Mat conversionToRatio(Mat inputMatrix);
	Mat conversionToValue(Mat inputMatrix);

private:

};



#endif // !ACCESSORYFUNCTIONS_H
