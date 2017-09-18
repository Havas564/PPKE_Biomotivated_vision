#include "accessoryFunctions.h"

//constructor and destructor
AccessoryFunctions::AccessoryFunctions()
{
}

AccessoryFunctions::~AccessoryFunctions()
{
}

// function creating equilateral kernel with centrum and periphery
vector<Mat> AccessoryFunctions::kernelCreator(int centerSize, int peripherySize, int iterX, int iterY, Mat inputMatrix) {
	//Mat centerEreaser = Mat::zeros(centerSize, centerSize, CV_64FC1);
	Mat centerEreaser = (Mat_<double>(peripherySize, peripherySize) << \
		1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1);
	Mat fullKernel(inputMatrix, Rect(iterX, iterY, peripherySize, peripherySize));
	//center creation
	int peripheryThickness = (peripherySize - centerSize) / 2;
	Mat kernelCenter(fullKernel, Rect(peripheryThickness, peripheryThickness, centerSize, centerSize));
	//periphery creation
	Mat kernelPeriphery = fullKernel.mul(centerEreaser);
	//saving the two kernel to a common variable
	vector<Mat> kernels;
	kernels.push_back(kernelCenter);
	kernels.push_back(kernelPeriphery);
	return kernels;
}

// calculating the size of the matrix
Size AccessoryFunctions::sizeOfMatrix(Mat inputMatrix) {
	Size s = inputMatrix.size();
	return s;
}

// zeroing below treshold
double AccessoryFunctions::tressholding(double inputElement, int treshold) {
	if (inputElement < treshold) {
		inputElement = 0;
	}
	return inputElement;
}


// Function separating the color channels - Note: OpenCV uses BGR color order! For channel required give on of the wanted colors with lowercase letters.
Mat AccessoryFunctions::colorChannelRetriever(Mat inputMatrix, string channelRequired) {
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

//
