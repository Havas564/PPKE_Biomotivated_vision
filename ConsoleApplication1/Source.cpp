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
//#include "cvconfig.h"

#ifdef HAVE_IPP_A
#include "opencv2/core/ippasync.hpp"
#define CHECK_STATUS(STATUS, NAME)\
    if(STATUS!=HPP_STATUS_NO_ERROR){ printf("%s error %d\n", NAME, STATUS);\
    if (virtMatrix) {hppStatus delSts = hppiDeleteVirtualMatrices(accel, virtMatrix); CHECK_DEL_STATUS(delSts,"hppiDeleteVirtualMatrices");}\
    if (accel)      {hppStatus delSts = hppDeleteInstance(accel); CHECK_DEL_STATUS(delSts, "hppDeleteInstance");}\
    return -1;}
#define CHECK_DEL_STATUS(STATUS, NAME)\
    if(STATUS!=HPP_STATUS_NO_ERROR){ printf("%s error %d\n", NAME, STATUS); return -1;}
#endif

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

	//initiating program, setting save variable
	ipda.setSaveLocation();
	
	//base values
	int mainIterator = 1;
	int maxIteration;
	string fileName;
	string inputExtension = ".jpg";

/*#ifdef HAVE_IPP_A
	hppiMatrix* src, *dst;
	hppAccel accel = 0;
	hppAccelType accelType;
	hppStatus sts;
	hppiVirtualMatrix * virtMatrix;*/
	//initiate camera
	//VideoCapture cap;
	//Creating accelerator instance
	/*accelType = sAccel == "cpu" ? HPP_ACCEL_TYPE_CPU :
				sAccel == "gpu" ? HPP_ACCEL_TYPE_GPU :
								  HPP_ACCEL_TYPE_ANY;
	
	sts = hppCreateInstance(accelType, 0, &accel);
	CHECK_STATUS(sts, "hppCreateInstance");
	virtMatrix = hppiCreateVirtualMatrices(accel, 1);*/
	// Starting iteration
	do 
	{
		cout << "Give the number of pictures to process: "; cin >> maxIteration; cout << endl;
	} while (cin.fail());
	if (maxIteration == 1)
	{
		//opening image
		String imageName("C:/Users/matep/testImage.jpg"); // path for the image
		if (argc > 1) {
			imageName = argv[1];
		}
		Mat colorImage;
		Mat intensityImage;
		colorImage = imread(imageName, IMREAD_COLOR); // Read the file for color for cone pathway
		if (colorImage.empty()) {						// Check for invalid input
			cout << "Could not open or find the image" << endl;
			return -1;
		}
		//intensity Image
		cvtColor(colorImage, intensityImage, COLOR_BGR2GRAY);
		intensityImage.convertTo(intensityImage, CV_32F);
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
		//BIPOLAR PATHWAY
		clock_t start, end;
		start = clock();
		Mat rodBipolarLevel = rbp.RodBiploarProcessing(intensityImage, mainIterator);
		ipda.proceedWithSavingIfTrue(rodBipolarLevel, "RodData/", "rodProcessing", mainIterator);
		Mat amacrineAIIlevel = aAIIp.amacrineAIIBipolarProcessing(rodBipolarLevel, mainIterator);
		ipda.proceedWithSavingIfTrue(amacrineAIIlevel, "AmacrineData/", "amacrineProcessing", mainIterator);
		// CONE PATHWAY
		redChannel.convertTo(redChannel, CV_32F);
		greenChannel.convertTo(greenChannel, CV_32F);
		blueChannel.convertTo(blueChannel, CV_32F);

		vector<Mat> midgetPathway = mscp.midgetBipolarProcessing(blueChannel, mainIterator);
		ipda.proceedWithSavingIfTrue(midgetPathway[0], "MidgetPathway/", "midgetProcessing", mainIterator);
		ipda.proceedWithSavingIfTrue(midgetPathway[1], "MidgetPathway/", "midgetProcessingFovae", mainIterator);
		vector<Mat> redGreenDiscrimination = rgd.redGreenDiscriminationMain(redChannel, greenChannel, mainIterator);
		ipda.proceedWithSavingIfTrue(redGreenDiscrimination[0], "RedGreenDiscrimation/", "redGreenProcessing", mainIterator);
		ipda.proceedWithSavingIfTrue(redGreenDiscrimination[1], "RedGreenDiscrimation/", "redGreenProcessingFovae", mainIterator);
		vector<Mat> yellowBlueDiscrimination = ybd.yellowBlueDiscriminationMain(redGreenDiscrimination[0], blueChannel, mainIterator);
		ipda.proceedWithSavingIfTrue(yellowBlueDiscrimination[0], "YellowBlue/", "yellowBlueProcessing", mainIterator);
		ipda.proceedWithSavingIfTrue(yellowBlueDiscrimination[1], "YellowBlue/", "yellowBlueProcessingFovae", mainIterator);
		vector<Mat> allConeDiscrimination = acd.allConeDiscriminationMain(redChannel, greenChannel, blueChannel, mainIterator);
		ipda.proceedWithSavingIfTrue(allConeDiscrimination[0], "AllThreeChannel/", "allThreeProcessing", mainIterator);
		ipda.proceedWithSavingIfTrue(allConeDiscrimination[1], "AllThreeChannel/", "allThreeProcessingFovae", mainIterator);
		//vector<Mat> directionGanglionFromRG = mdgp.mainDirectonInformation(redGreenDiscrimination[0], mainIterator);
		//ipda.proceedWithSavingIfTrue(directionGanglionFromRG[0], "DirectionInformation/", "direction", mainIterator);
		//vector<Mat> directionGanglionFromYB = mdgp.mainDirectonInformation(yellowBlueDiscrimination[0], mainIterator);
		//vector<Mat> directionGanglionFromAll = mdgp.mainDirectonInformation(allConeDiscrimination[0], mainIterator);
		gf.gaborFiltering(allConeDiscrimination[0]);
		//gf.gaborFiltering(allConeDiscrimination[1]);
		end = clock();
		cout << "Elapsed time in seconds: " << (static_cast<double>(end) - start) / CLK_TCK << endl;
	}
	else {
		do
		{
			cout << "Provide filename (without extension): " << endl;
			cin >> fileName;
		} while (cin.fail());
		//iterating through the testpictures
		for (int mainIt = 0; mainIt < maxIteration; mainIt++)
		{
			stringstream fileNameContents;
			fileNameContents <<"C:/Users/matep/" << fileName << mainIt << inputExtension;
			string imageName;
			imageName = fileNameContents.str();
			if (argc < 1)
			{
				imageName = argv[1]; // questionable if it is right this way
			}
			Mat colorImage;
			Mat intensityImage;
			colorImage = imread(imageName, IMREAD_COLOR); // Read the file for color for cone pathway
			if (colorImage.empty())
			{						// Check for invalid input
				cout << "Could not open or find the image" << endl;
				return -1;
			}
			//intensity Image
			cvtColor(colorImage, intensityImage, COLOR_BGR2GRAY);
			intensityImage.convertTo(intensityImage, CV_32F);
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
			//BIPOLAR PATHWAY
			clock_t start, end;
			start = clock();
			Mat rodBipolarLevel = rbp.RodBiploarProcessing(intensityImage, mainIterator);
			ipda.proceedWithSavingIfTrue(rodBipolarLevel, "RodData/", "rodProcessing", mainIterator);
			Mat amacrineAIIlevel = aAIIp.amacrineAIIBipolarProcessing(rodBipolarLevel, mainIterator);
			ipda.proceedWithSavingIfTrue(amacrineAIIlevel, "AmacrineData/", "amacrineProcessing", mainIterator);
			// CONE PATHWAY
			redChannel.convertTo(redChannel, CV_32F);
			greenChannel.convertTo(greenChannel, CV_32F);
			blueChannel.convertTo(blueChannel, CV_32F);

			vector<Mat> midgetPathway = mscp.midgetBipolarProcessing(blueChannel, mainIterator);
			ipda.proceedWithSavingIfTrue(midgetPathway[0], "MidgetPathway/", "midgetProcessing", mainIterator);
			ipda.proceedWithSavingIfTrue(midgetPathway[1], "MidgetPathway/", "midgetProcessingFovae", mainIterator);
			vector<Mat> redGreenDiscrimination = rgd.redGreenDiscriminationMain(redChannel, greenChannel, mainIterator);
			ipda.proceedWithSavingIfTrue(redGreenDiscrimination[0], "RedGreenDiscrimation/", "redGreenProcessing", mainIterator);
			ipda.proceedWithSavingIfTrue(redGreenDiscrimination[1], "RedGreenDiscrimation/", "redGreenProcessingFovae", mainIterator);
			vector<Mat> yellowBlueDiscrimination = ybd.yellowBlueDiscriminationMain(redGreenDiscrimination[0], blueChannel, mainIterator);
			ipda.proceedWithSavingIfTrue(yellowBlueDiscrimination[0], "YellowBlue/", "yellowBlueProcessing", mainIterator);
			ipda.proceedWithSavingIfTrue(yellowBlueDiscrimination[1], "YellowBlue/", "yellowBlueProcessingFovae", mainIterator);
			vector<Mat> allConeDiscrimination = acd.allConeDiscriminationMain(redChannel, greenChannel, blueChannel, mainIterator);
			ipda.proceedWithSavingIfTrue(allConeDiscrimination[0], "AllThreeChannel/", "allThreeProcessing", mainIterator);
			ipda.proceedWithSavingIfTrue(allConeDiscrimination[1], "AllThreeChannel/", "allThreeProcessingFovae", mainIterator);
			//vector<Mat> directionGanglionFromRG = mdgp.mainDirectonInformation(redGreenDiscrimination[0], mainIterator);
			//ipda.proceedWithSavingIfTrue(directionGanglionFromRG[0], "DirectionInformation/", "direction", mainIterator);
			//vector<Mat> directionGanglionFromYB = mdgp.mainDirectonInformation(yellowBlueDiscrimination[0], mainIterator);
			//vector<Mat> directionGanglionFromAll = mdgp.mainDirectonInformation(allConeDiscrimination[0], mainIterator);
			/*if (mainIterator > 1)
			{
				ipda.proceedWithSavingIfTrue(directionGanglionFromAll[0], "DirectionInformation/", "drectionInformation", mainIterator);
			}*/
			gf.gaborFiltering(allConeDiscrimination[0]);
			//gf.gaborFiltering(allConeDiscrimination[1]);
			end = clock();
			cout << "Elapsed time in seconds: " << (static_cast<double>(end) - start) / CLK_TCK << endl;
			mainIterator++;
		}
	}
	/*#else
	printf("IPP Async not supported\n");
	#endif*/
	
	return 0;
}

/*
TO ASK: - probléma: ugyan a function-on belül elvileg rendben elvégzi a DoG-ot, a kimeneti kép amit kiküld a main-be egy fekete semmi
		- 
*/

/*
#TODO: - incorporate AII information to AllConeProcessing 
	   - create cin command to initiate saving boolean - done
	   - create rod bipolar output saved image - done
	   - go through AII amacrine cell in the debugger - done
	   - create midget s cone pathway - done
	   - go through red-green discrimination in debugger - done
	   - create red-green output saved image - done
	   - go through yellow-blue discrimination in debugger - 
	   - create yellow-blue output saved image - done
	   - go through all cone discrimination in debugger - done
	   - create all-cone output saved image - done
	   - go through main direction processing in debugger
	   - create main direction output saved image
	   - go through Gabor Filter processing in debugger - done
	   - create Gabor Filter output saved image
	   - try to lower run time
	   - try to lower run time even further
	   - rewrite all processing function so that in the for cycle it++ changed to it = it + cellInformation[x]
	   - continue to rework the kernel-ratio processes
	   - start parallelising the individual processes
	   - might make the code faster if fovae is evaulated in the for cycle of inputmatrix in case of given conditions - PRIORITY!!!
	   
#INSIGHTS:	- Look up SLAM problem and if I need solution for that
			- Create EKF to incorporate all sensory input and to estimate the next ~150 ms, as delay will occure
			- implement PID controller which uses 
			- create motor output which sends wireless information of [pitch ang., roll ang., yaw-rot. speed, z-axis movement]
			- the previous command should be at ~100 Hz
			- maybe the choosen calculation should be made in an if condition set, and direction and edgedetection should be done only on that
			
#CONSULT 2018.03.12.:
			- színcsatornák vizsgálata az elején, egy idõ után csak azt nézhetné hogy mekkora a különbség a mostani és az elõzõ között, azaz van-e értelme taktikát változtatni
			- külön teszt és futtatás üzemmód
			- tesztset képfeldolgozáshoz - élkép, mozásirány
*/

// ipda-t bemenetként beadni a függvénynek!