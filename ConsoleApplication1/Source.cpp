#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
//#include "synapticStrength.h"
//#include "memoryHandling.h"


using namespace cv;
using namespace std;


void imageShow(Mat image) {
	namedWindow("Current image", WINDOW_AUTOSIZE);
	imshow("Current image", image);
}

float convertToRatio(float pixel, float maxNumber) {
	float ratio;
	ratio = maxNumber / 255.0 * pixel;
	return ratio;
}

float convertToPixel(float ratio) {
	float pixel;
	pixel = ratio * 255.0;
	return pixel;
}

int main(int argc, char** argv){
	double timePassed = (double)getTickCount(); // it should be double...
	String imageName("C:/Users/matep/opencv_logo.png"); // path for the image
	if (argc > 1){
		imageName = argv[1];
	}

	Mat colorImage;
	Mat intensityImage;

	colorImage = imread(imageName, IMREAD_COLOR); // Read the file for color for cone pathway
	if (colorImage.empty()){						// Check for invalid input
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	
	//intensity Image
	cvtColor(colorImage, intensityImage, COLOR_BGR2GRAY);

	// debug: showing images before further processing
	imageShow(colorImage);
	waitKey(5); // Wait for a keystroke in the window

	imageShow(intensityImage);
	
	cout << intensityImage.size() << endl; //debug
	
	// Rod Pathway
	float kernelSize = 3;
	Size sizeOfImage = intensityImage.size();
	int rowNumber = sizeOfImage.height;
	int columnNumber = sizeOfImage.width;
	int inputRatio;
	cout << rowNumber << " " << columnNumber << endl;
	int k = 0;
	float inputTreshold = 0.3 * 255;
	Mat rodBipolarOutput = Mat(columnNumber - kernelSize, rowNumber, CV_64FC1);

	float kernelSum = pow(kernelSize, 2) * 255.0;
	float maxSize = pow(kernelSize, 2);
	int posX = 0;
	for (int it = 0; it < rowNumber - kernelSize; it++) {
		int posY = 0;
		for (int ij = 0; ij < columnNumber - kernelSize; ij++) {
			float convertedInput;
			Mat kernel(intensityImage, Rect(ij, it, kernelSize, kernelSize));
			kernelSum = sum(kernel)[0];
			convertedInput = convertToRatio(kernelSum, maxSize);
			inputRatio = 1 - (pow(kernelSize, 2.0) - convertedInput) / pow(kernelSize, 2.0); // think it over, 2nd power may only work in case of 0-1 interval

			if (inputRatio < inputTreshold) {
				inputRatio = 0;
			}
			rodBipolarOutput.at<double>(it, ij);
		}
	}

	/*
	namedWindow("test");
	imshow("test", E);*/
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	cout << "Time elapsed: " << timePassed << endl;
	waitKey();
	return 0;
}