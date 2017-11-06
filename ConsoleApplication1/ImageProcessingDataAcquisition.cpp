#include "ImageProcessingDataAcquisition.h"

ImageProcessingDataAcquisition::ImageProcessingDataAcquisition()
{
}

ImageProcessingDataAcquisition::~ImageProcessingDataAcquisition()
{
}

void ImageProcessingDataAcquisition::imageShow(Mat inputMatrix) {
	namedWindow("Current image", WINDOW_AUTOSIZE);
	imshow("Current image", inputMatrix);
	waitKey(0);
}

void ImageProcessingDataAcquisition::saveImage(Mat inputMatrix, string saveLocation, string imageName) {
	stringstream locationAndName;
	locationAndName << saveLocation << imageName;
	string saveInformation = locationAndName.str();
	imwrite(saveInformation, inputMatrix);
}

void ImageProcessingDataAcquisition::setSaveLocation() {
	cout << "Please provide the save location for the images: " << endl;
	cin >> saveLocation;
}

vector<double> ImageProcessingDataAcquisition::saveTimeInformation(double timePassed, vector<double> timeElapsedVector) {
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	timeElapsedVector.push_back(timePassed);
	return timeElapsedVector;
}