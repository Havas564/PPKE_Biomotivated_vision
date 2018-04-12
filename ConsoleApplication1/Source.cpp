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
	MidgetSConePathway mscp;
	RedGreenDiscrimination rgd;
	YellowBlueDiscrimination ybd;
	AllConeDiscrimination acd;
	MainDirectionGanglionProcessing mdgp;
	GaborFiltering gf;
	ImageProcessingDataAcquisition ipda;

	string toSave;
	cout << "Do you want to save images? (yes/no) "; cin >> toSave;
	ipda.setSave(toSave);
	
	//base values
	int mainIterator = 1;
	//opening image
	String imageName("C:/Users/matep/testImage.jpg"); // path for the image
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
	//cout << intensityImage.size() << endl; //debug
	Mat redChannel, greenChannel, blueChannel;
	//separating color channels
	ac.colorChannelRetriever(colorImage, "red", redChannel);
	ac.colorChannelRetriever(colorImage, "green", greenChannel);
	ac.colorChannelRetriever(colorImage, "blue", blueChannel);
	ipda.saveImage("red_base", redChannel);
	ipda.saveImage("green_base", greenChannel);
	ipda.saveImage("blue_base", blueChannel);
	// conversion to ratio
	redChannel = ac.conversionToRatio(redChannel);
	greenChannel = ac.conversionToRatio(greenChannel);
	blueChannel = ac.conversionToRatio(blueChannel);
	//cerr << blueChannel(Rect(200, 200, 100, 100)) << endl;
	//imwrite("blue01.jpg", blueChannel);
	blueChannel = ac.conversionToValue(blueChannel);
	//cerr << blueChannel(Rect(200, 200, 100, 100)) << endl;
	//imwrite("blue02.jpg", blueChannel);
	//BIPOLAR PATHWAY
	Mat rodBipolarLevel = rbp.RodBiploarProcessing(intensityImage, mainIterator);
	Mat amacrineAIIlevel = aAIIp.amacrineAIIBipolarProcessing(rodBipolarLevel, mainIterator);
	// CONE PATHWAY
	redChannel.convertTo(redChannel, CV_32F);
	greenChannel.convertTo(greenChannel, CV_32F);
	blueChannel.convertTo(blueChannel, CV_32F);

	vector<Mat> midgetPathway = mscp.midgetBipolarProcessing(blueChannel, mainIterator);
	vector<Mat> redGreenDiscrimination = rgd.redGreenDiscriminationMain(redChannel, greenChannel, mainIterator);

	//vector<Mat> yellowBlueDiscrimination = ybd.yellowBlueDiscriminationMain(redGreenDiscrimination[0], blueChannel, mainIterator);
	vector<Mat> allConeDiscrimination = acd.allConeDiscriminationMain(redChannel, greenChannel, blueChannel, mainIterator);
	vector<Mat> directionGanglionFromRG = mdgp.mainDirectonInformation(redGreenDiscrimination[0], mainIterator);
	//vector<Mat> directionGanglionFromYB = mdgp.mainDirectonInformation(yellowBlueDiscrimination[0], mainIterator);
	vector<Mat> directionGanglionFromAll = mdgp.mainDirectonInformation(allConeDiscrimination[0], mainIterator);
	gf.gaborFiltering(allConeDiscrimination[0]);
	gf.gaborFiltering(allConeDiscrimination[1]);
	return 0;
}

/*
TO ASK: - 
*/

/*
#TODO: - incorporate AII information to AllConeProcessing 
	   - create cin command to initiate saving boolean - in progress
	   - Kernel recreation - done
	   - create rod bipolar output saved image - done
	   - go through AII amacrine cell in the debugger - done
	   - create midget s cone pathway - in progress
	   - go through red-green discrimination in debugger - in progress
	   - memory exception error at synaptic strength modification - debug it!
	   - create red-green output saved image - done
	   - go through yellow-blue discrimination in debugger
	   - yellow-blue discrimination should be merged - the two input matrix will have different size!!!
	   - create yellow-blue output saved image - done
	   - go through all cone discrimination in debugger - done
	   - create all-cone output saved image - done
	   - go through main direction processing in debugger
	   - create main direction output saved image
	   - go through Gabor Filter processing in debugger
	   - create Gabor Filter output saved image
	   - try to lower run time
	   - try to lower run time even further
	   - kernel rect should be one variable for all to save memory - consult it with Tamás before rework
	   - ratio of input center must be corrected - from what value to subtract - done
	   - rewrite all processing function so that in the for cycle it++ changed to it = it + cellInformation[x]
	   - continue to rework the kernel-ratio processes
	   - find bugs cause on case of kernel-ratio
	   - start parallelising the individual processes
	   - might make the code faster if fovae is evaulated in the for cycle of inputmatrix in case of given conditions - PRIORITY!!!
	   
#INSIGHTS:	- Look up SLAM problem and if I need solution for that
			- Create EKF to incorporate all sensory input and to estimate the next ~150 ms, as delay will occure
			- implement PID controller which uses 
			- create motor output which sends wireless information of [pitch ang., roll ang., yaw-rot. speed, z-axis movement]
			- the previous command should be at ~100 Hz
			- maybe the choosen calculation should be made in an if condition set, and direction and edgedetection should be done only on that
			
#CONSULT 2018.03.12.:
			- ROI should be cut out from original picture, and the three colour channel should be seperated after that
			- színcsatornák vizsgálata az elején, egy idõ után csak azt nézhetné hogy mekkora a különbség a mostani és az elõzõ között, azaz van-e értelme taktikát változtatni
			- külön teszt és futtatás üzemmód
			- tesztset képfeldolgozáshoz - élkép, mozásirány
*/
