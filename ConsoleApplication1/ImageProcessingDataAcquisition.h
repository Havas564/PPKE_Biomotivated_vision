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
	void setSaveLocation();
	vector<double> saveTimeInformation(double timePassed, vector<double> timeElapsedVector);
	void saveImage(string fileName, Mat data);
	void proceedWithSavingIfTrue(Mat fileToSave, string folderName, string fileName, int mainIterator);

	bool saving;

private:
	string saveLocation;
	string fileExtension = ".jpg";
};


#endif // !IMAGEPROCESSINGDATAAQUISITION_H
