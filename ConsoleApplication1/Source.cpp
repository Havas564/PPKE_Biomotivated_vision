#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <typeinfo>
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
	amacrineAIIProcessing aAIIp;
	RedGreenDiscrimination rgd;
	YellowBlueDiscrimination ybd;
	AllConeDiscrimination acd;
	MainDirectionGanglionProcessing mdgp;
	GaborFiltering gf;
	ImageProcessingDataAcquisition ipda;
	
	//create save image boolean
	/*bool toSave = false;
	string answer;
	bool errorInput;
	string tmp;
	do {
		cout << "Do you want to save output images? "; cin >> answer;
		errorInput = cin.fail() || cin.peek() != '\n';
		if (errorInput || answer != "yes" || answer != "no") {
			cout << "Wrong input, please give a yer or no answer." << endl;
			cin.clear(); getline(cin, tmp, '\n');
		}
	} while (!errorInput || answer == "yes" || answer == "no");
	if (answer == "yes") {
		toSave = true;
	}
	else if (answer == "no") {
		toSave = false;
	}
	//save location
	string saveLocation;
	if (toSave == true) {
		do {
			cout << "Please provide the save locatin: "; cin >> saveLocation;
			errorInput = cin.peek() != '\n';
			if (errorInput || typeid(saveLocation) != typeid("string")) {
				cout << "Plesase provide the pathway again." << endl;
				cin.clear(); getline(cin, tmp, '\n');
			}
			if (cin.fail()) {
				saveLocation = "C:/Users/matep/Documents/Visual Studio 2015/Projects/Saved_images";
			}
		} while (errorInput);
	}*/
	//base values
	int mainIterator = 1;
	double timePassed = (double)getTickCount();
	//opening image
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
	intensityImage.convertTo(intensityImage, CV_32F);
	cout << intensityImage.size() << endl; //debug
	Mat redChannel, greenChannel, blueChannel;
	/*vector<Mat> RGBChannels = ac.colorChannelRetrieverAlternate(colorImage);
	cout << endl;
	cout << endl;
	cerr << RGBChannels[0](Rect(100, 100, 100, 100)) << endl;
	ipda.imageShow(RGBChannels[0]);
	cout << endl;
	cout << endl;
	cerr << RGBChannels[1](Rect(100, 100, 100, 100)) << endl;
	ipda.imageShow(RGBChannels[1]);
	cout << endl;
	cout << endl;
	cerr << RGBChannels[2](Rect(100, 100, 100, 100)) << endl;
	ipda.imageShow(RGBChannels[2]);*/
	//separating color channels
	ac.colorChannelRetriever(colorImage, "red", redChannel);
	ac.colorChannelRetriever(colorImage, "green", greenChannel);
	ac.colorChannelRetriever(colorImage, "blue", blueChannel);
	/*ipda.imageShow(redChannel);
	ipda.imageShow(greenChannel);
	ipda.imageShow(blueChannel);*/
	cout << redChannel.size() << endl; //debug
	cout << greenChannel.size() << endl; //debug
	cout << blueChannel.size() << endl; //debug
	//debug
	//ipda.imageShow(redChannel);
	waitKey(100);
	
	//BIPOLAR PATHWAY
	Mat rodBipolarLevel = rbp.RodBiploarProcessing(intensityImage, mainIterator);
	/*if (toSave == true) {
		stringstream stringStreamIN;
		stringStreamIN << "RodBipolarLevel_" << mainIterator;
		string imageName = stringStreamIN.str();
		ipda.saveImage(rodBipolarLevel, saveLocation, imageName);
		stringStreamIN.str("");
	}*/
	Mat amacrineAIIlevel = aAIIp.amacrineAIIBipolarProcessing(rodBipolarLevel, mainIterator);
	//cerr << amacrineAIIlevel(Rect(100, 100, 100, 100)) << endl;
	// CONE PATHWAY
	//cout << endl;
	//cout << endl;
	//cerr << redChannel(Rect(100, 100, 100, 100)) << endl;
	redChannel.convertTo(redChannel, CV_32F);
	//cerr << redChannel(Rect(100, 100, 100, 100)) << endl;
	greenChannel.convertTo(greenChannel, CV_32F);
	blueChannel.convertTo(blueChannel, CV_32F);
	//cerr << greenChannel(Rect(100, 100, 100, 100)) << endl;
	/*ipda.imageShow(redChannel);
	ipda.imageShow(greenChannel);
	ipda.imageShow(blueChannel);*/
	vector<Mat> redGreenDiscrimination = rgd.redGreenDiscriminationMain(redChannel, greenChannel, mainIterator);
	/*if (toSave == true) {
		stringstream stringStreamIN;
		stringStreamIN << "RedGreenDiscriminationON_" << mainIterator;
		string imageName = stringStreamIN.str();
		ipda.saveImage(redGreenDiscrimination[0], saveLocation, imageName);
		stringStreamIN.str("");
		stringStreamIN << "RedGreenDiscriminationOFF_" << mainIterator;
		imageName = stringStreamIN.str();
		ipda.saveImage(redGreenDiscrimination[1], saveLocation, imageName);
		stringStreamIN.str("");
	}*/
	vector<Mat> yellowBlueDiscrimination = ybd.yellowBlueDiscriminationMain(redGreenDiscrimination[0], blueChannel, mainIterator);
	/*if (toSave == true) {
	stringstream stringStreamIN;
	stringStreamIN << "YellowBlueDiscriminationON_" << mainIterator;
	string imageName = stringStreamIN.str();
	ipda.saveImage(yellowBlueDiscrimination[0], saveLocation, imageName);
	stringStreamIN.str("");
	stringStreamIN << "YellowBlueDiscriminationOFF_" << mainIterator;
	imageName = stringStreamIN.str();
	ipda.saveImage(yellowBlueDiscrimination[1], saveLocation, imageName);
	stringStreamIN.str("");
	}*/
	vector<Mat> allConeDiscrimination = acd.allConeDiscriminationMain(redChannel, greenChannel, blueChannel, mainIterator);
	/*if (toSave == true) {
	stringstream stringStreamIN;
	stringStreamIN << "AllConeDiscriminationON_" << mainIterator;
	string imageName = stringStreamIN.str();
	ipda.saveImage(allConeDiscrimination[0], saveLocation, imageName);
	stringStreamIN.str("");
	stringStreamIN << "AllConeDiscriminationOFF_" << mainIterator;
	imageName = stringStreamIN.str();
	ipda.saveImage(allConeDiscrimination[1], saveLocation, imageName);
	stringStreamIN.str("");
	}*/
	vector<Mat> directionGanglionFromRG = mdgp.mainDirectonInformation(redGreenDiscrimination[0], mainIterator);
	vector<Mat> directionGanglionFromYB = mdgp.mainDirectonInformation(yellowBlueDiscrimination[0], mainIterator);
	vector<Mat> directionGanglionFromAll = mdgp.mainDirectonInformation(allConeDiscrimination[0], mainIterator);
	gf.gaborFiltering(allConeDiscrimination[0]);
	gf.gaborFiltering(allConeDiscrimination[1]);
	
	/*namedWindow("test");
	//imshow("test", E);
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	cout << "Time elapsed: " << timePassed << endl;
	waitKey();*/
	return 0;
}

/*
TO ASK: - kell-e külön fovaeKernel változó vagy elég ha mindig újratöltöm az adott helyen?
		- MainDirection kernel creator - jelenleg 3 kernel, kell-e újakat definiálni, vagy csak újratöltöm a kernel és rögtön push_back??
		- alternate és original color channel splitter közül melyik legyen?
		- receptiveFieldFunction.cpp-ben a static nem kell a get-set functionok elé?
*/

/*
#TODO: - incorporate AII information to AllConeProcessing 
	   - create cin command to initiate saving boolean - almost done
	   - Kernel recreation - done
	   - create rod bipolar output saved image - done
	   - go through AII amacrine cell in the debugger - done
	   - go through red-green discrimination in debugger - in progress
	   - memory exception error at synaptic strength modification - debug it!
	   - create red-green output saved image - done
	   - go through yellow-blue discrimination in debugger
	   - yellow-blue discrimination should be merged - the two input matrix will have different size!!!
	   - create yellow-blue output saved image - done
	   - go through all cone discrimination in debugger
	   - create all-cone output saved image - done
	   - go through main direction processing in debugger
	   - create main direction output saved image
	   - go through Gabor Filter processing in debugger
	   - create Gabor Filter output saved image
	   - try to lower run time
	   - try to lower run time even further
	   - ratio of input center must be corrected - from what value to subtract - PRIORITY!!!!
	   - rewrite all processing function so that in the for cycle it++ changed to it = it + cellInformation[x]
*/
