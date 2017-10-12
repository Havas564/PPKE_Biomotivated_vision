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

void ImageProcessingDataAcquisition::saveTimeInformation() {

}