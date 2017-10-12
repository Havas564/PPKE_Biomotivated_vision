#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include "synapticStrength.h"
#include "memoryHandling.h"
#include "bipolarlevel.h"
#include "accessoryFunctions.h"
#include "receptiveFieldProcessing.h"
#include "ImageProcessingDataAcquisition.h"


using namespace cv;
using namespace std;

bool isFirstIteration(int iterator) {
	bool isFirst = true; // might want to declare in main() - in this case it should be add to input
	if (iterator > 1) {
		isFirst = false;
	}
	else if (iterator == 1) {
		cout << "First iteration." << endl;
	}
	else {
		cout << "Error: Iteration number less than 1" << endl;
	}
	return isFirst;
}

int main(int argc, char** argv){
	//calling headers
	AccessoryFunctions ac;
	Memory m;
	ReceptiveFieldFunctions rfp;
	RodBipolarProcessing rbp;
	RedGreenDiscrimination rgd;
	YellowBlueDiscrimination ybd;
	ImageProcessingDataAcquisition ipda;

	//base values
	int mainIterator = 1;
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
	//imageShow(colorImage);
	waitKey(5); // Wait for a keystroke in the window

	//imageShow(intensityImage);
	cout << intensityImage.size() << endl; //debug
	Mat redChannel, greenChannel, blueChannel;
	//separating color channels
	ac.colorChannelRetriever(colorImage, "red", redChannel);
	ac.colorChannelRetriever(colorImage, "green", greenChannel);
	ac.colorChannelRetriever(colorImage, "blue", blueChannel);
	cout << redChannel.size() << endl; //debug
	cout << greenChannel.size() << endl; //debug
	cout << blueChannel.size() << endl; //debug
	//debug
	ipda.imageShow(redChannel);
	waitKey(100);

	//BIPOLAR PATHWAY
	Mat rodBipolarLevel = rbp.RodBiploarProcessing(intensityImage, m.rodMemory, mainIterator);
	//ipda.imageShow(rodBipolarLevel);
	//waitKey(0);


	//red-green discrimination
	/*Mat redGreenOn, redGreenOff, greenRedOn, greenRedOff;
	vector<Mat> redGreenProcessed = rgd.redGreenDiscriminationMain(redChannel, greenChannel, m.mConeMemory, m.lConeMemory, mainIterator);
	redGreenOn = redGreenProcessed[0];
	redGreenOff = redGreenProcessed[1];
	greenRedOn = redGreenProcessed[2];
	greenRedOff = redGreenProcessed[3];
	//yellow-blue discrimination
	Mat yellowBlueOn, yellowBlueOff, blueYellowOn, blueYellowOff;
	vector<Mat> yellowBlueProcessed = ybd.yellowBlueDiscriminationMain(yellowBlueOn, blueChannel, m.redOpponencyGanglionMemory, m.sConeMemory, mainIterator);
	yellowBlueOn = yellowBlueProcessed[0];
	yellowBlueOff = yellowBlueProcessed[1];
	greenRedOn = yellowBlueProcessed[2];
	greenRedOff = yellowBlueProcessed[3];
	
	namedWindow("test");
	//imshow("test", E);
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	cout << "Time elapsed: " << timePassed << endl;
	waitKey();*/
	return 0;
}

/*
TO ASK: - kell-e külön fovaeKernel változó vagy elég ha mindig újratöltöm az adott helyen?
		- MainDirection kernel creator - jelenleg 3 kernel, kell-e újakat definiálni, vagy csak újratöltöm a kernel és rögtön push_back??
		- 
*/

// Rod Pathway
/*float kernelSize = 3;
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
*/