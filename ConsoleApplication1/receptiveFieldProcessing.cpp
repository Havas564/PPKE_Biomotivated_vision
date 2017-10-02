#include "receptiveFieldProcessing.h"
#include "memoryHandling.h"
#include "synapticStrength.h"
#include "accessoryFunctions.h"

//RECEPTIVEFIELDFUNCTIONS functions

ReceptiveFieldFunctions::ReceptiveFieldFunctions()
{
}

ReceptiveFieldFunctions::~ReceptiveFieldFunctions()
{
}

// ON/OFF declaration
bool ReceptiveFieldFunctions::onOff(string cellType) {
	bool onCell;
	if (cellType == "ON") {
		onCell = true;
	}
	else if(cellType == "OFF"){
		onCell = false;
	}
	else {
		cout << "Error: Wrong celltype given." << endl;
	}
	return onCell;
}

// function creating equilateral kernel with centrum and periphery
vector<Mat> ReceptiveFieldFunctions::kernelCreator(int centerSize, int peripherySize, int iterX, int iterY, Mat inputMatrix) {
	//Mat centerEreaser = Mat::zeros(centerSize, centerSize, CV_64FC1);
	Mat centerEreaser = (Mat_<double>(peripherySize, peripherySize) << \
		1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1);
	Mat fullKernel(inputMatrix, Rect(iterX, iterY, peripherySize, peripherySize));
	//center creation
	int peripheryThickness = (peripherySize - centerSize) / 2;
	Mat kernelCenter(fullKernel, Rect(iterX + peripheryThickness, iterY + peripheryThickness, centerSize, centerSize));
	//periphery creation
	Mat kernelPeriphery = fullKernel.mul(centerEreaser); // it holds the zero values - keep that in mind for further calculations
	//saving the two kernel to a common variable
	vector<Mat> kernels; //[centre, periphery]
	kernels.push_back(kernelCenter);
	kernels.push_back(kernelPeriphery);
	return kernels;
}

// function creating one kernel
Mat ReceptiveFieldFunctions::oneKernelCreator(Mat inputMatrix, int kernelSize, int iterX, int iterY) {
	Mat kernel(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
	return kernel;
}

// function creating kernel with different side size (e.g. for movement ganglion cells)
vector<Mat> ReceptiveFieldFunctions::displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize, int iterX, int iterY, string mainDirection) {
	vector<Mat> kernels;
	if (mainDirection == "up") {
		Mat kernel1(previousInputMatrix, Rect(iterX, iterY + kernelSize, kernelSize, kernelSize));
		Mat kernel2(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		Mat kernel3(inputMatrix, Rect(iterX, iterY + kernelSize, kernelSize, kernelSize));
		kernels.push_back(kernel1);
		kernels.push_back(kernel2);
		kernels.push_back(kernel3);
	}
	else if (mainDirection == "down") {
		Mat kernel1(previousInputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		Mat kernel2(inputMatrix, Rect(iterX, iterY + kernelSize, kernelSize, kernelSize));
		Mat kernel3(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		kernels.push_back(kernel1);
		kernels.push_back(kernel2);
		kernels.push_back(kernel3);
	}
	else if (mainDirection == "right") {
		Mat kernel1(previousInputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		Mat kernel2(inputMatrix, Rect(iterX + kernelSize, iterY, kernelSize, kernelSize));
		Mat kernel3(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		kernels.push_back(kernel1);
		kernels.push_back(kernel2);
		kernels.push_back(kernel3);
	}
	else if (mainDirection == "left") {
		Mat kernel1(previousInputMatrix, Rect(iterX + kernelSize, iterY, kernelSize, kernelSize));
		Mat kernel2(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
		Mat kernel3(inputMatrix, Rect(iterX + kernelSize, iterY, kernelSize, kernelSize));
		kernels.push_back(kernel1);
		kernels.push_back(kernel2);
		kernels.push_back(kernel3);
	}
	else {
		cout << "ERROR: Wrong direction given" << endl;
	}
	return kernels;
}

// center-periphery comparison
float ReceptiveFieldFunctions::centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery) {
	if (ratioOfPeriphery > ratioOfCenter + 0.5) {
		ratioOfCenter = 0;
	}
	else if (ratioOfPeriphery + 0.5 < ratioOfCenter) {
		ratioOfCenter = 1;
	}
	else if (ratioOfPeriphery > ratioOfCenter) {
		ratioOfCenter = ratioOfCenter * (1 - (ratioOfPeriphery - ratioOfCenter));
	}
	else if (ratioOfPeriphery < ratioOfCenter) {
		ratioOfCenter = ratioOfCenter * (1 + (ratioOfCenter - ratioOfPeriphery));
	}
	else if (ratioOfCenter == ratioOfPeriphery) {
		ratioOfCenter = ratioOfCenter;
	}
	else {
		cout << "Error: comparison of center and periphery malfunctioning." << endl;
	}
	return ratioOfCenter;
}

//homogen receptive field input evaluation
Mat ReceptiveFieldFunctions::homogenReceptiveFieldEvaluation(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator) {
	//Calling helper functions
	Memory m;
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variables
	int currentMemoryPosition = m.memoryPosition(mainIterator); // placeholder - must be initiated by m.memoryPosition() func.
	Mat processedMatrix;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	bool isFirst = ss.isFirstIteration(mainIterator);
	Mat synapticStrengthMatrix;
	double ratioOfInput, threshold = 0.3;
	//adding current input to memory
	m.pushbackMemory(inputMatrix, currentMemoryPosition, cellMemory);
	//modifying input with synapticStrength function
	ss.modifierMatrixCalculator(cellMemory, currentMemoryPosition);
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	inputMatrix = inputMatrix.mul(synapticStrengthMatrix);
	//iterating through the matrix with the kernel
	int iterY = 0;
	for (int it = 0; it < ((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]); it++) {
		int iterX = 0;
		for (int ij = 0; ij < ((matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]); ij++) {
			Mat kernel = oneKernelCreator(inputMatrix, cellInformation[0], ij, it);
			ratioOfInput = 1 - (pow(cellInformation[0], 2.0) - cellInformation[0]) / pow(cellInformation[0], 2.0); // check if it gives the good value
			ratioOfInput = af.thresholding(ratioOfInput, threshold);
			processedMatrix.at<double>(iterX, iterY) = ratioOfInput;
			iterX++;
		}
		iterY++;
	}
	return processedMatrix;
}

//input evaulation in case of one input matrix
vector<Mat> ReceptiveFieldFunctions::receptiveFieldEvaluationOneInput(Mat inputMatrix, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator) {
	//calling helper function
	Memory m;
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variables
	vector<Mat> processedMatrices;
	Mat onProcessedMatrix;
	Mat offProcessedMatrix;
	Size matrixSize;
	bool isFirst;
	Mat synapticStrengthMatrix;
	double ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery, typeModifier;
	double threshold = 0.3;
	matrixSize = af.sizeOfMatrix(inputMatrix);
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(inputMatrix, currentMemoryPosition, cellMemory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	Mat modifierMatrix;
	if (mainIterator < 1) {
		modifierMatrix = ss.modifierMatrixCalculator(cellMemory, currentMemoryPosition);
	}
	else {
		modifierMatrix = Mat::ones(1, 2, CV_64F);
	}
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
	inputMatrix = inputMatrix.mul(synapticStrengthMatrix);
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]; ij = ij + cellInformation[3]) {
			vector<Mat> kernels;
			kernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, inputMatrix);
			// calculating result using the kernels
			ratioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1],	2.0) - sum(kernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			//thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			//creating processed matrix vector
			onProcessedMatrix.at<double>(iterX, iterY) = ratioOfOnInputCenter;
			offProcessedMatrix.at<double>(iterX, iterY) = ratioOfOffInputCenter;
			iterX++;
		}
		iterY++;
	}
	//loading the processed matrices
	processedMatrices.push_back(onProcessedMatrix);
	processedMatrices.push_back(offProcessedMatrix);
	//fovae processing
	vector<Mat> fovaeKernel;
	Mat onFovaeProcessedMatrix;
	Mat offFovaeProcessedMatrix;
	Mat fovaeMatrix(inputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Size areaOfFovae = af.sizeOfMatrix(fovaeMatrix);
	for (int it = 0; it < areaOfFovae.height; it++) {
		for (int ij = 0; ij < areaOfFovae.width; ij++) {
			fovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, fovaeMatrix);
			// calculating result using the kernels
			ratioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(fovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(fovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(fovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(fovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			//thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			//creating processed matrix vector
			onFovaeProcessedMatrix.at<double>(ij, it) = ratioOfOnInputCenter;
			offFovaeProcessedMatrix.at<double>(ij, it) = ratioOfOffInputCenter;
		}
	}
	return processedMatrices;
}

//input evaulation in case of two input matrices
vector<Mat> ReceptiveFieldFunctions::receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix,
	vector<int> cellInformation, vector<Mat> firstCellMemory, vector<Mat> secondCellMemory, int mainIterator) {
	//calling helper function
	Memory m;
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variables
	vector<Mat> processedMatrices;
	Mat firstOnProcessedMatrix;
	Mat firstOffProcessedMatrix;
	Mat secondOnProcessedMatrix;
	Mat secondOffProcessedMatrix;
	Size matrixSize;
	bool isFirst;
	Mat firstSynapticStrengthMatrix;
	Mat secondSynapticStrengthMatrix;
	double firstRatioOfOnInputCenter, firstRatioOfOffInputCenter, firstRatioOfOnInputPeriphery, firstRatioOfOffInputPeriphery,
		secondRatioOfOnInputCenter, secondRatioOfOffInputCenter, secondRatioOfOnInputPeriphery, secondRatioOfOffInputPeriphery;
	double threshold = 0.3;
	matrixSize = af.sizeOfMatrix(firstInputMatrix);
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition, firstCellMemory);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition, secondCellMemory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(firstCellMemory, currentMemoryPosition);
	firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	firstInputMatrix = firstInputMatrix.mul(firstSynapticStrengthMatrix);
	ss.modifierMatrixCalculator(secondCellMemory, currentMemoryPosition);
	secondSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	secondInputMatrix = secondInputMatrix.mul(secondSynapticStrengthMatrix);
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]; ij = ij + cellInformation[3]) {
			vector<Mat> firstKernels;
			vector<Mat> secondKernels;
			firstKernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, firstInputMatrix);
			secondKernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, secondInputMatrix);
			// calculating result using the kernels
			firstRatioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0));
			firstRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0));
			firstRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			firstRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			//thresholding
			firstRatioOfOnInputCenter = af.thresholding(firstRatioOfOnInputCenter, threshold);
			firstRatioOfOnInputPeriphery = af.thresholding(firstRatioOfOnInputPeriphery, threshold);
			firstRatioOfOffInputCenter = af.thresholding(firstRatioOfOffInputCenter, threshold);
			firstRatioOfOffInputPeriphery = af.thresholding(firstRatioOfOffInputPeriphery, threshold);
			secondRatioOfOnInputCenter = af.thresholding(secondRatioOfOnInputCenter, threshold);
			secondRatioOfOnInputPeriphery = af.thresholding(secondRatioOfOnInputPeriphery, threshold);
			secondRatioOfOffInputCenter = af.thresholding(secondRatioOfOffInputCenter, threshold);
			secondRatioOfOffInputPeriphery = af.thresholding(secondRatioOfOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			firstRatioOfOnInputCenter = centerPeripheryComparison(firstRatioOfOnInputCenter, secondRatioOfOnInputPeriphery);
			firstRatioOfOffInputCenter = centerPeripheryComparison(firstRatioOfOffInputCenter, secondRatioOfOffInputPeriphery);
			secondRatioOfOnInputCenter = centerPeripheryComparison(secondRatioOfOnInputCenter, firstRatioOfOnInputPeriphery);
			secondRatioOfOffInputCenter = centerPeripheryComparison(secondRatioOfOffInputCenter, firstRatioOfOffInputPeriphery);
			//creating processed matrix vector
			firstOnProcessedMatrix.at<double>(iterX, iterY) = firstRatioOfOnInputCenter;
			firstOffProcessedMatrix.at<double>(iterX, iterY) = firstRatioOfOffInputCenter;
			secondOnProcessedMatrix.at<double>(iterX, iterY) = secondRatioOfOnInputCenter;
			secondOffProcessedMatrix.at<double>(iterX, iterY) = secondRatioOfOffInputCenter;
			iterX++;
		}
		iterY++;
	}
	//loading the processed matrices
	processedMatrices.push_back(firstOnProcessedMatrix);
	processedMatrices.push_back(firstOffProcessedMatrix);
	processedMatrices.push_back(secondOnProcessedMatrix);
	processedMatrices.push_back(secondOffProcessedMatrix);
	//fovae processing
	vector<Mat> firstFovaeKernel;
	vector<Mat> secondFovaeKernel;
	Mat firstOnFovaeProcessedMatrix;
	Mat firstOffFovaeProcessedMatrix;
	Mat secondOnFovaeProcessedMatrix;
	Mat secondOffFovaeProcessedMatrix;
	Mat firstFovaeMatrix(firstInputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Mat secondFovaeMatrix(secondInputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Size areaOfFovae = af.sizeOfMatrix(firstFovaeMatrix);
	for (int it = 0; it < areaOfFovae.height; it++) {
		for (int ij = 0; ij < areaOfFovae.width; ij++) {
			firstFovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, firstFovaeMatrix);
			secondFovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, secondFovaeMatrix);
			// calculating result using the kernels
			firstRatioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			firstRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			firstRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			firstRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			//thresholding
			firstRatioOfOnInputCenter = af.thresholding(firstRatioOfOnInputCenter, threshold);
			firstRatioOfOnInputPeriphery = af.thresholding(firstRatioOfOnInputPeriphery, threshold);
			firstRatioOfOffInputCenter = af.thresholding(firstRatioOfOffInputCenter, threshold);
			firstRatioOfOffInputPeriphery = af.thresholding(firstRatioOfOffInputPeriphery, threshold);
			secondRatioOfOnInputCenter = af.thresholding(secondRatioOfOnInputCenter, threshold);
			secondRatioOfOnInputPeriphery = af.thresholding(secondRatioOfOnInputPeriphery, threshold);
			secondRatioOfOffInputCenter = af.thresholding(secondRatioOfOffInputCenter, threshold);
			secondRatioOfOffInputPeriphery = af.thresholding(secondRatioOfOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			firstRatioOfOnInputCenter = centerPeripheryComparison(firstRatioOfOnInputCenter, secondRatioOfOnInputPeriphery);
			firstRatioOfOffInputCenter = centerPeripheryComparison(firstRatioOfOffInputCenter, secondRatioOfOffInputPeriphery);
			secondRatioOfOnInputCenter = centerPeripheryComparison(secondRatioOfOnInputCenter, firstRatioOfOnInputPeriphery);
			secondRatioOfOffInputCenter = centerPeripheryComparison(secondRatioOfOffInputCenter, firstRatioOfOffInputPeriphery);
			//creating processed matrix vector
			firstOnFovaeProcessedMatrix.at<double>(ij, it) = firstRatioOfOnInputCenter;
			firstOffFovaeProcessedMatrix.at<double>(ij, it) = firstRatioOfOffInputCenter;
			secondOnFovaeProcessedMatrix.at<double>(ij, it) = secondRatioOfOnInputCenter;
			secondOffFovaeProcessedMatrix.at<double>(ij, it) = secondRatioOfOffInputCenter;
		}
	}
	processedMatrices.push_back(firstOnFovaeProcessedMatrix);
	processedMatrices.push_back(firstOffFovaeProcessedMatrix);
	processedMatrices.push_back(secondOnFovaeProcessedMatrix);
	processedMatrices.push_back(secondOffFovaeProcessedMatrix);
	return processedMatrices;
}


//Acquiring fovae size
vector<int> ReceptiveFieldFunctions::fovaeSizeAcquirer(Mat inputMatrix) {
	AccessoryFunctions af;
	vector<int> fovaeSize; // [x-axis, y-axis]
	Size fovae = af.sizeOfMatrix(inputMatrix);
	int temp;
	temp = fovae.width;
	fovaeSize.push_back(temp);
	temp = fovae.height;
	fovaeSize.push_back(temp);
	return fovaeSize;
}

//Acquiring fovae's cetral location
vector<int> ReceptiveFieldFunctions::fovaeCenterCoordinatesAcquirer(Mat inputMatrix) { // + gps data considering the direction of the drones self-movement
	//functionpart which calculates fovae based on the direction of self-movement
	//placeholder: calculateing the center of the input image
	AccessoryFunctions af;
	vector<int> fovaeCenterCoordinates;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	fovaeCenterCoordinates[0] = matrixSize.width;
	fovaeCenterCoordinates[1] = matrixSize.height;
	return fovaeCenterCoordinates;
}

//ROD bipolar processing
vector<int> RodBipolarProcessing::initializeCellInformation() {
	cellInformation[0] = 5;
	cellInformation[1] = 1;
	return cellInformation;
}
vector<int> RodBipolarProcessing::cellInformation = {5,1};

Mat RodBipolarProcessing::RodBiploarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator) {
	Mat processedMatrix;
	if (mainIterator == 1) {
		initializeCellInformation();
	}
	processedMatrix = homogenReceptiveFieldEvaluation(inputMatrix, cellInformation, cellMemory, mainIterator);
	return processedMatrix;
}
//set functions
vector<int> RodBipolarProcessing::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

//get functions
int RodBipolarProcessing::getCellInformation(int position) {
	return cellInformation.at(position);
}

vector<int> RodBipolarProcessing::getAllCellInformation() {
	return cellInformation;
}

//AMACRINE AII processing
vector<int> amacrineAIIProcessing::initializeCellInformation() {
	cellInformation[0] = 5;
	cellInformation[1] = 1;
	return cellInformation;
}

Mat amacrineAIIProcessing::amacrineAIIBipolarProcessing(Mat inputMatrix, vector<Mat> cellMemory, int mainIterator){
	Mat processedMatrix;
	if (mainIterator == 1) {
		initializeCellInformation();
	}
	processedMatrix = homogenReceptiveFieldEvaluation(inputMatrix, cellInformation, cellMemory, mainIterator);
	return processedMatrix;
}

//set functions
vector<int> amacrineAIIProcessing::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

//get functions
int amacrineAIIProcessing::getCellInformation(int position) {
	return cellInformation.at(position);
}

vector<int> amacrineAIIProcessing::getAllCellInformation(){
	return cellInformation;
}

//REDGREEN discriminator
vector<int> RedGreenDiscrimination::initializeCellInformation(Mat inputMatrix) {
	cellInformation[0] = 4; // kernelSize
	cellInformation[1] = 2; // centre size
	cellInformation[2] = cellInformation[0] - cellInformation[1];
	cellInformation[3] = 1;
	cellInformation[4] = 3; // fovae kernel Size
	cellInformation[5] = 1; // focae step size
	cellInformation[6] = 1; // fovae center size
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	cellInformation[7] = fovaeSize[0]; // fovae x-axis size
	cellInformation[8] = fovaeSize[1]; // fovae y-axis size
	return cellInformation;
}

vector<Mat> RedGreenDiscrimination::redGreenDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator) {
	initializeCellInformation(firstInputMatrix);
	//red-green discrimination
	vector<Mat> processedMatrices = receptiveFieldEvaluationTwoInput(firstInputMatrix, secondInputMatrix, cellInformation, firstMemory, secondMemory, mainIterator);
	return processedMatrices;
}
//set functions
vector<int> RedGreenDiscrimination::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

vector<int> RedGreenDiscrimination::setFovaeSize(int newValue, int position) {
	fovaeSize.at(position) = newValue;
	return fovaeSize;
}

//get functions
int RedGreenDiscrimination::getCellInformation(int position) {
	return cellInformation.at(position);
}

int RedGreenDiscrimination::getFovaeSize(int position) {
	return fovaeSize.at(position);
}

vector<int> RedGreenDiscrimination::getAllCellInformation() {
	return cellInformation;
}

vector<int> RedGreenDiscrimination::getAllFovaeSize() {
	return fovaeSize;
}

//YELLOWBLUE discriminator
vector<int> YellowBlueDiscrimination::initializeCellInformation(Mat inputMatrix) {
	cellInformation[0] = 4; // kernelSize
	cellInformation[1] = 2; //centre size
	cellInformation[2] = cellInformation[0] - cellInformation[1]; //perihpery size
	cellInformation[3] = 1; // step size
	cellInformation[4] = 3; // fovae kernel Size
	cellInformation[5] = 1; // focae step size
	cellInformation[6] = 1; // fovae center size
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	cellInformation[7] = fovaeSize[0]; // fovae x-axis size
	cellInformation[8] = fovaeSize[1]; // fovae y-axis size
	return cellInformation;

}

vector<Mat> YellowBlueDiscrimination::yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory, int mainIterator) {
	//yellow-blue discrimination
	if (mainIterator == 1) {
		initializeCellInformation(firstInputMatrix);
	}
	vector<Mat> processedMatrices = receptiveFieldEvaluationTwoInput(firstInputMatrix, secondInputMatrix, cellInformation, firstMemory, secondMemory, mainIterator);
	return processedMatrices;
}
//set functions
vector<int> YellowBlueDiscrimination::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

vector<int> YellowBlueDiscrimination::setFovaeSize(int newValue, int position) {
	fovaeSize.at(position) = newValue;
	return fovaeSize;
}

//get functions
int YellowBlueDiscrimination::getCellInformation(int position) {
	return cellInformation.at(position);
}

int YellowBlueDiscrimination::getFovaeSize(int position) {
	return fovaeSize.at(position);
}

vector<int> YellowBlueDiscrimination::getAllCellInformation() {
	return cellInformation;
}

vector<int> YellowBlueDiscrimination::getAllFovaeSize() {
	return fovaeSize;
}

//ALLCONE discriminator
vector<int> AllConeDiscrimination::initializeCellInformation(Mat inputMatrix) {
	cellInformation[0] = 4;
	cellInformation[1] = 2;
	cellInformation[2] = cellInformation[0] - cellInformation[1];
	cellInformation[3] = 1;
	cellInformation[4] = 3; // fovae kernel Size
	cellInformation[5] = 1; // focae step size
	cellInformation[6] = 1; // fovae center size
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	cellInformation[7] = fovaeSize[0]; // fovae x-axis size
	cellInformation[8] = fovaeSize[1]; // fovae y-axis size
	return cellInformation;
}

//vector<Mat> AllConeDiscrimination::

vector<Mat> AllConeDiscrimination::allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, vector<Mat> firstMemory, vector<Mat> secondMemory,
	vector<Mat> thridMemory, int mainIterator) {
	initializeCellInformation(firstInputMatrix);
	//all-three cone discrimination

	//vector<Mat> processedMatrices = receptorFieldEvaluationOneInput(inputMatrix, cellInformation, cellMemory, mainIterator);
	vector<Mat> processedMatrices;
	return processedMatrices;
}

//set functions
vector<int> AllConeDiscrimination::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

vector<int> AllConeDiscrimination::setFovaeSize(int newValue, int position) {
	fovaeSize.at(position) = newValue;
	return fovaeSize;
}

//get functions
int AllConeDiscrimination::getCellInformation(int position) {
	return cellInformation.at(position);
}

int AllConeDiscrimination::getFovaeSize(int position) {
	return fovaeSize.at(position);
}

vector<int> AllConeDiscrimination::getAllCellInformation() {
	return cellInformation;
}

vector<int> AllConeDiscrimination::getAllFovaeSize() {
	return fovaeSize;
}

// MAIN DIRECTION GANGLION processing
vector<int> MainDirectionGanglionProcessing::initializeCellInformation(Mat inputMatrix) {
	cellInformation[0] = 4; //kernelSize
	cellInformation[1] = 2; //kernelStep
	cellInformation[2] = 3; // fovae kernel size
	cellInformation[3] = 1; // fovae step size
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	cellInformation[4] = fovaeSize[0];
	cellInformation[5] = fovaeSize[1];
	return cellInformation;
}

vector<Mat> MainDirectionGanglionProcessing::mainDirectonInformation() {
	vector<Mat> placeHolder;
	return placeHolder;
}

//set functions
vector<int> MainDirectionGanglionProcessing::setCellInformation(int newValue, int position) {
	cellInformation.at(position) = newValue;
	return cellInformation;
}

vector<int> MainDirectionGanglionProcessing::setFovaeSize(int newValue, int position) {
	fovaeSize.at(position) = newValue;
	return fovaeSize;
}

//get functions
int MainDirectionGanglionProcessing::getCellInformation(int position) {
	return cellInformation.at(position);
}

int MainDirectionGanglionProcessing::getFovaeInformation(int position) {
	return fovaeSize.at(position);
}

vector<int> MainDirectionGanglionProcessing::getAllCellInformation() {
	return cellInformation;
}

vector<int> MainDirectionGanglionProcessing::getAllFovaeInformation() {
	return fovaeSize;
}