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
double AccessoryFunctions::thresholding(double inputElement, double treshold) {
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

Mat AccessoryFunctions::conversionToRatio(Mat inputMatrix) {
	double min, max;
	minMaxLoc(inputMatrix, &min, &max);
	inputMatrix = (inputMatrix - min) / max;
	return inputMatrix;
}

Mat AccessoryFunctions::conversionToValue(Mat inputMatrix) {
	inputMatrix = inputMatrix.mul(255);
	return inputMatrix;
}



//
