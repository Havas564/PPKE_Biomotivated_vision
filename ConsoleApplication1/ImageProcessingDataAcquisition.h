#pragma once
#ifndef IMAGEPROCESSINGDATAAQUISITION_H
#define IMAGEPROCESSINGDATAAQUISITION_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include "opencv2\core.hpp"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class ImageProcessingDataAcquisition {

public:
	ImageProcessingDataAcquisition();
	~ImageProcessingDataAcquisition();

	void imageShow(Mat inputMatrix);
	void saveImage(Mat inputMatrix, string saveLocation, string imageName);
	void setSaveLocation();
	vector<double> saveTimeInformation(double timePassed, vector<double> timeElapsedVector);
	bool setSave(string toSave);
	void saveImage(string fileName, Mat data);

private:
	string saveLocation;
	bool saving;
};


#endif // !IMAGEPROCESSINGDATAAQUISITION_H
