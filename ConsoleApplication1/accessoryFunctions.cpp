#include "accessoryFunctions.h"

//constructor and destructor
AccessoryFunctions::AccessoryFunctions()
{
}

AccessoryFunctions::~AccessoryFunctions()
{
}


// calculating the size of the matrix
Size AccessoryFunctions::sizeOfMatrix(Mat inputMatrix) {
	Size s = inputMatrix.size();
	return s;
}

// zeroing below treshold
float AccessoryFunctions::thresholding(float inputElement, float treshold) {
	if (inputElement < treshold) {
		inputElement = 0;
	}
	return inputElement;
}


// Function separating the color channels - Note: OpenCV uses BGR color order! For channel required give on of the wanted colors with lowercase letters.
Mat AccessoryFunctions::colorChannelRetriever(Mat inputMatrix, string channelRequired, Mat& neededChannel) {
	Mat channels[3];
	
	split(inputMatrix, channels);
	if (channelRequired == "red") {
		neededChannel = channels[2];
		return neededChannel;
	}
	else if (channelRequired == "green") {
		neededChannel = channels[1];
		return neededChannel;
	}
	else if (channelRequired == "blue") {
		neededChannel = channels[0];
		return neededChannel;
	}
	else {
		cout << "Error: Wrong color type added for input." << endl;
	}
}

vector<Mat> AccessoryFunctions::colorChannelRetrieverAlternate(Mat inputMatrix) {
	vector<Mat> indiviualChannels;
	Mat channels[3];
	split(inputMatrix, channels);
	Mat tmp;
	tmp = channels[2]; //red
	indiviualChannels.push_back(tmp);
	tmp = channels[1]; //green
	indiviualChannels.push_back(tmp);
	tmp = channels[0]; //blue
	indiviualChannels.push_back(tmp);
	return indiviualChannels;
}

Mat AccessoryFunctions::conversionToRatio(Mat inputMatrix) {
	Mat outputMatrix;
	inputMatrix.convertTo(outputMatrix, CV_32F, 1.0 / 255, 0);
	return outputMatrix;
}

Mat AccessoryFunctions::conversionToValue(Mat inputMatrix) {
	Mat outputMatrix;
	inputMatrix.convertTo(outputMatrix, CV_32SC1, 255, 0);
	return outputMatrix;
}

//
