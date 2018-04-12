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
	waitKey(5000);
}

void ImageProcessingDataAcquisition::saveImage(Mat inputMatrix, string saveLocation, string imageName) {
	stringstream locationAndName;
	locationAndName << saveLocation << imageName;
	string saveInformation = locationAndName.str();
	imwrite(saveInformation, inputMatrix);
}

void ImageProcessingDataAcquisition::setSaveLocation() {
	if (saving == true) {
		cout << "Please provide the save location for the images: " << endl;
		cin >> saveLocation;
	}
}

vector<double> ImageProcessingDataAcquisition::saveTimeInformation(double timePassed, vector<double> timeElapsedVector) {
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	timeElapsedVector.push_back(timePassed);
	return timeElapsedVector;
}

bool ImageProcessingDataAcquisition::setSave(string toSave) {
	if (toSave == "yes") {
		saving = true;
	}
	else {
		saving = false;
	}
	return saving;
}

void ImageProcessingDataAcquisition::saveImage(string fileName, Mat data) {
	if (saving == true) {
		ostringstream savingData;
		savingData << saveLocation << fileName;
		string saveInformation;
		saveInformation = savingData.str();
		imwrite(saveInformation, data);
	}
}