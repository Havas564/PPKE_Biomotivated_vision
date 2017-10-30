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
vector<Mat> ReceptiveFieldFunctions::displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize,
	int iterX, int iterY, string mainDirection) {
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
Mat ReceptiveFieldFunctions::homogenReceptiveFieldEvaluation(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator) {
	//Calling helper functions
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variables
	int currentMemoryPosition = m.memoryPosition(mainIterator); // placeholder - must be initiated by m.memoryPosition() func.
	Mat processedMatrix, inputModifiedBySynapticStrength;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	bool isFirst = ss.isFirstIteration(mainIterator);
	Mat synapticStrengthMatrix;
	double ratioOfInput, threshold = 0.3;
	//adding current input to memory
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	if (mainIterator > 1) {
		ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	}
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	//withfor
	/*for (int i = 0; i < matrixSize.height; i++) {
		for (int j = 0; j < matrixSize.width; j++) {
			inputModifiedBySynapticStrength.at<float>(i, j) = inputMatrix.at<float>(i, j) *
				synapticStrengthMatrix.at<float>(i, j);
		}
	}*/
	inputModifiedBySynapticStrength = inputMatrix.mul(synapticStrengthMatrix);
	//iterating through the matrix with the kernel
	int iterY = 0;
	for (int it = 0; it < ((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]); it++) {
		int iterX = 0;
		for (int ij = 0; ij < ((matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]); ij++) {
			Mat kernel = oneKernelCreator(inputModifiedBySynapticStrength, cellInformation[0], ij, it);
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
vector<Mat> ReceptiveFieldFunctions::receptiveFieldEvaluationOneInput(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator) {
	//calling helper function
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
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	Mat modifierMatrix;
	if (mainIterator < 1) {
		modifierMatrix = ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
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
			ratioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[1]) / pow((double)cellInformation[1], 2.0));
			ratioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(kernels)[1]) / pow((double)cellInformation[1], 2.0)) - 1);
			// thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);
			// comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			// creating processed matrix vector
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
	vector<int> cellInformation, Memory& m, int mainIterator) {
	//calling helper function
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
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	firstInputMatrix = firstInputMatrix.mul(firstSynapticStrengthMatrix);
	ss.modifierMatrixCalculator(m.memorySecond, currentMemoryPosition);
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
			firstRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[1]) / pow((double)cellInformation[1], 2.0));
			firstRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			firstRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[1]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[1]) / pow((double)cellInformation[1], 2.0));
			secondRatioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			secondRatioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(secondKernels)[1]) / pow((double)cellInformation[1], 2.0)) - 1);
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

vector<Mat> initiateMemory(vector<Mat> memoryType) {
	Memory m;
	memoryType = m.memory;
	return memoryType;
}

//set functions
vector<int> ReceptiveFieldFunctions::setKernelSize(int newValue) {
	cellInformation[0] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setCenterSize(int newValue) {
	cellInformation[1] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setPeripherySize(int newValue) {
	cellInformation[2] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setKernelStep(int newValue) {
	cellInformation[3] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFovaeKernelSize(int newValue) {
	cellInformation[4] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFovaeKernelStep(int newValue) {
	cellInformation[5] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFocaeCenterSize(int newValue) {
	cellInformation[6] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFovaeXAxis(int newValue) {
	cellInformation[7] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFovaeYAxis(int newValue) {
	cellInformation[8] = newValue;
	return cellInformation;
}

//get functions
int ReceptiveFieldFunctions::getKernelSize() {
	return cellInformation[0];
}

int ReceptiveFieldFunctions::getCenterSize() {
	return cellInformation[1];
}

int ReceptiveFieldFunctions::getPeripherySize() {
	return cellInformation[2];
}

int ReceptiveFieldFunctions::getKernelStep() {
	return cellInformation[3];
}

int ReceptiveFieldFunctions::getFovaeKernelSize() {
	return cellInformation[4];
}

int ReceptiveFieldFunctions::getFovaeKernelStep() {
	return cellInformation[5];
}

int ReceptiveFieldFunctions::getFocaeCenterSize() {
	return cellInformation[6];
}

int ReceptiveFieldFunctions::getFovaeXAxis() {
	return cellInformation[7];
}

int ReceptiveFieldFunctions::getFovaeYAxis() {
	return cellInformation[8];
}

vector<int> ReceptiveFieldFunctions::getAllCellInformation() {
	return cellInformation;
}

//ROD bipolar processing
vector<int> RodBipolarProcessing::initializeCellInformation() {
	setKernelSize(5);
	setCenterSize(0);
	setPeripherySize(0);
	setKernelStep(1);
	return cellInformation;
}
vector<int> RodBipolarProcessing::cellInformation(9);

void RodBipolarProcessing::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		rodMemory.resize(iterator);
	}
	else {
		rodMemory.resize(m.getMemoryMax());
	}
}

void RodBipolarProcessing::loadToMemory(Memory& m) {
	if (rodMemory.size() != m.memory.size()) {
		m.memory.resize(rodMemory.size());
	}
	for (int it = 0; it < rodMemory.size(); it++) {
		m.memory[it] = rodMemory[it];
	}
}

void RodBipolarProcessing::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		rodMemory[it] = m.memory[it];
	}
}

Mat RodBipolarProcessing::RodBiploarProcessing(Mat inputMatrix, int mainIterator) {
	Memory m;
	Mat processedMatrix;
	if (mainIterator == 1) {
		initializeCellInformation();
		initializeMemory(mainIterator);
	}
	else if(mainIterator < 6){
		initializeMemory(mainIterator);
	}
	loadToMemory(m);
	processedMatrix = homogenReceptiveFieldEvaluation(inputMatrix, cellInformation, m, mainIterator);
	loadFromMemory(m);
	return processedMatrix;
}

//AMACRINE AII processing
vector<int> amacrineAIIProcessing::initializeCellInformation() {
	setKernelSize(5);
	setCenterSize(0);
	setPeripherySize(0);
	setKernelStep(1);
	return cellInformation;
}
//vector<int> amacrineAIIProcessing::cellInformation(9);

void amacrineAIIProcessing::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		rodBipolarMemory.resize(iterator);
	}
	else {
		rodBipolarMemory.resize(m.getMemoryMax());
	}
}

	void amacrineAIIProcessing::loadToMemory(Memory& m) {
		if (rodBipolarMemory.size() != m.memory.size()) {
			m.memory.resize(rodBipolarMemory.size());
		}
		for (int it = 0; it < rodBipolarMemory.size(); it++) {
			m.memory[it] = rodBipolarMemory[it];
		}
	}

	void amacrineAIIProcessing::loadFromMemory(Memory& m) {
		for (int it = 0; it < m.memory.size(); it++) {
			rodBipolarMemory[it] = m.memory[it];
		}
	}

Mat amacrineAIIProcessing::amacrineAIIBipolarProcessing(Mat inputMatrix, int mainIterator){
	Memory m;
	Mat processedMatrix;
	if (mainIterator == 1) {
		initializeCellInformation();
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
	}
	loadToMemory(m);
	processedMatrix = homogenReceptiveFieldEvaluation(inputMatrix, cellInformation, m, mainIterator);
	loadFromMemory(m);
	return processedMatrix;
}


//REDGREEN discriminator
vector<int> RedGreenDiscrimination::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(5);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxis(fovaeSize[0]);
	setFovaeYAxis(fovaeSize[1]);
	return cellInformation;
}

void RedGreenDiscrimination::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		lConeMemory.resize(iterator);
		mConeMemory.resize(iterator);
	}
	else {
		lConeMemory.resize(m.getMemoryMax());
		mConeMemory.resize(m.getMemoryMax());
	}
}

void RedGreenDiscrimination::loadToMemory(Memory& m) {
	if (lConeMemory.size() != m.memory.size()) {
		m.memory.resize(lConeMemory.size());
	}
	if (mConeMemory.size() != m.memorySecond.size()) {
		m.memory.resize(mConeMemory.size());
	}
	for (int it = 0; it < lConeMemory.size(); it++) {
		m.memory[it] = lConeMemory[it];
		m.memorySecond[it] = mConeMemory[it];
	}
}

void RedGreenDiscrimination::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		lConeMemory[it] = m.memory[it];
		mConeMemory[it] = m.memorySecond[it];

	}
}

//vector<int> RedGreenDiscrimination::cellInformation(9);

vector<Mat> RedGreenDiscrimination::redGreenDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, int mainIterator) {
	Memory m;
	if (mainIterator == 1) {
		initializeCellInformation(firstInputMatrix);
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
	}
	loadToMemory(m);
	//red-green discrimination
	vector<Mat> processedMatrices = receptiveFieldEvaluationTwoInput(firstInputMatrix, secondInputMatrix, cellInformation, m, mainIterator);
	loadToMemory(m);
	return processedMatrices;
}

//YELLOWBLUE discriminator
vector<int> YellowBlueDiscrimination::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(5);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxis(fovaeSize[0]);
	setFovaeYAxis(fovaeSize[1]);
	return cellInformation;

}
//vector<int> YellowBlueDiscrimination::cellInformation(9);

void YellowBlueDiscrimination::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		yellowMemory.resize(iterator);
		sConeMemory.resize(iterator);
	}
	else {
		yellowMemory.resize(m.getMemoryMax());
		sConeMemory.resize(m.getMemoryMax());
	}
}

void YellowBlueDiscrimination::loadToMemory(Memory& m) {
	if (yellowMemory.size() != m.memory.size()) {
		m.memory.resize(yellowMemory.size());
	}
	if (sConeMemory.size() != m.memorySecond.size()) {
		m.memory.resize(sConeMemory.size());
	}
	for (int it = 0; it < yellowMemory.size(); it++) {
		m.memory[it] = yellowMemory[it];
		m.memorySecond[it] = sConeMemory[it];
	}
}

void YellowBlueDiscrimination::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		yellowMemory[it] = m.memory[it];
		sConeMemory[it] = m.memorySecond[it];

	}
}

vector<Mat> YellowBlueDiscrimination::yellowBlueDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, int mainIterator) {
	Memory m;
	//yellow-blue discrimination
	if (mainIterator == 1) {
		initializeCellInformation(firstInputMatrix);
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
	}
	loadToMemory(m);
	vector<Mat> processedMatrices = receptiveFieldEvaluationTwoInput(firstInputMatrix, secondInputMatrix, cellInformation, m, mainIterator);
	return processedMatrices;
}

//ALLCONE discriminator
vector<int> AllConeDiscrimination::initializeCellInformation(Mat inputMatrix) {
	//initialize cell information
	setKernelSize(5);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxis(fovaeSize[0]);
	setFovaeYAxis(fovaeSize[1]);
	return cellInformation;
}
//vector<int> AllConeDiscrimination::cellInformation(9);

//evaluation of the three input receptive fields
vector<Mat> AllConeDiscrimination::receptiveFieldEvaluationThreeInput(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, Memory& m,
	vector<int> cellInformation, int mainIterator) {
	//calling helper function
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variables
	vector<Mat> processedMatrices;
	vector<Mat> averagedKernels;
	Mat averagedOnProcessedMatrix;
	Mat averagedOffProcessedMatrix;
	Size matrixSize;
	bool isFirst;
	Mat firstSynapticStrengthMatrix;
	Mat secondSynapticStrengthMatrix;
	Mat thirdSynapticStrengthMatrix;
	double ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	double threshold = 0.3;
	matrixSize = af.sizeOfMatrix(firstInputMatrix);
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition);
	m.pushbackMemory(thirdInputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	firstInputMatrix = firstInputMatrix.mul(firstSynapticStrengthMatrix);
	ss.modifierMatrixCalculator(m.memorySecond, currentMemoryPosition);
	secondSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	secondInputMatrix = secondInputMatrix.mul(secondSynapticStrengthMatrix);
	ss.modifierMatrixCalculator(m.memoryThird, currentMemoryPosition);
	thirdSynapticStrengthMatrix == ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	thirdInputMatrix = thirdInputMatrix.mul(thirdSynapticStrengthMatrix);
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]; ij = ij + cellInformation[3]) {
			vector<Mat> firstKernels;
			vector<Mat> secondKernels;
			vector<Mat> thirdKernels;
			firstKernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, firstInputMatrix);
			secondKernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, secondInputMatrix);
			thirdKernels = kernelCreator(cellInformation[1], cellInformation[2], ij, it, thirdInputMatrix);
			averagedKernels[0] = (firstKernels[0] + secondKernels[0] + thirdKernels[0]) / 3.0;
			averagedKernels[1] = (firstKernels[1] + secondKernels[1] + thirdKernels[1]) / 3.0;
			// calculating result using the kernels
			ratioOfOnInputCenter =  abs(1 - (pow((double)cellInformation[1], 2.0) - sum(averagedKernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(averagedKernels)[0]) / pow((double)cellInformation[1], 2.0));;
			ratioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((double)cellInformation[1], 2.0)) - 1);

			//thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);

			//comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			//creating processed matrix vector
			averagedOnProcessedMatrix.at<double>(iterX, iterY) = ratioOfOnInputCenter;
			averagedOffProcessedMatrix.at<double>(iterX, iterY) = ratioOfOffInputCenter;
			iterX++;
		}
		iterY++;
	}
	//loading the processed matrices
	processedMatrices.push_back(averagedOnProcessedMatrix);
	processedMatrices.push_back(averagedOffProcessedMatrix);
	//fovae processing
	vector<Mat> firstFovaeKernel;
	vector<Mat> secondFovaeKernel;
	vector<Mat> thirdFovaeKernel;
	vector<Mat> averagedFovaeKernels;
	Mat onFovaeProcessedMatrix;
	Mat offFovaeProcessedMatrix;
	Mat secondOnFovaeProcessedMatrix;
	Mat secondOffFovaeProcessedMatrix;
	Mat firstFovaeMatrix(firstInputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Mat secondFovaeMatrix(secondInputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Mat thirdFovaeMatrix(thirdInputMatrix, Rect((int)(0.4 * matrixSize.width), (int)(0.4 * matrixSize.height), cellInformation[4], cellInformation[5]));
	Size areaOfFovae = af.sizeOfMatrix(firstFovaeMatrix);
	for (int it = 0; it < areaOfFovae.height; it++) {
		for (int ij = 0; ij < areaOfFovae.width; ij++) {
			firstFovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, firstFovaeMatrix);
			secondFovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, secondFovaeMatrix);
			thirdFovaeKernel = kernelCreator(cellInformation[1], cellInformation[2], ij, it, secondFovaeMatrix);
			averagedFovaeKernels[0] = (firstFovaeKernel[0] + secondFovaeKernel[0] + thirdFovaeKernel[0]) / 3.0;
			averagedFovaeKernels[1] = (firstFovaeKernel[1] + secondFovaeKernel[1] + thirdFovaeKernel[1]) / 3.0;
			// calculating result using the kernels
			ratioOfOnInputCenter = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfOffInputCenter = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((double)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((double)cellInformation[1], 2.0)) - 1);
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
	processedMatrices.push_back(onFovaeProcessedMatrix);
	processedMatrices.push_back(offFovaeProcessedMatrix);
	return processedMatrices;
}

void AllConeDiscrimination::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		sConeMemory.resize(iterator);
		lConeMemory.resize(iterator);
		mConeMemory.resize(iterator);
	}
	else {
		sConeMemory.resize(m.getMemoryMax());
		lConeMemory.resize(m.getMemoryMax());
		mConeMemory.resize(m.getMemoryMax());
	}
}

void AllConeDiscrimination::loadToMemory(Memory& m) {
	if (sConeMemory.size() != m.memory.size()) {
		m.memory.resize(sConeMemory.size());
	}
	if (lConeMemory.size() != m.memorySecond.size()) {
		m.memory.resize(lConeMemory.size());
	}
	if (mConeMemory.size() != m.memoryThird.size()) {
		m.memoryThird.resize(mConeMemory.size());
	}
	for (int it = 0; it < sConeMemory.size(); it++) {
		m.memory[it] = sConeMemory[it];
		m.memorySecond[it] = lConeMemory[it];
		m.memoryThird[it] = mConeMemory[it];
	}
}

void AllConeDiscrimination::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		lConeMemory[it] = m.memory[it];
		sConeMemory[it] = m.memorySecond[it];
		mConeMemory[it] = m.memoryThird[it];
	}
}

//vector<Mat> AllConeDiscrimination::
vector<Mat> AllConeDiscrimination::allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, int mainIterator) {
	Memory m;
	vector<Mat> processedMatrices;
	if (mainIterator == 1) {
		initializeCellInformation(firstInputMatrix);
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
	}
	//all-three cone discrimination
	loadToMemory(m);
	processedMatrices = receptiveFieldEvaluationThreeInput(firstInputMatrix, secondInputMatrix, thirdInputMatrix, m, cellInformation, mainIterator);
	//vector<Mat> processedMatrices = receptorFieldEvaluationOneInput(inputMatrix, cellInformation, cellMemory, mainIterator);
	loadFromMemory(m);
	return processedMatrices;
}

// MAIN DIRECTION GANGLION processing
vector<int> MainDirectionGanglionProcessing::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(5);
	setCenterSize(0);
	setPeripherySize(0);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(0);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxis(fovaeSize[0]);
	setFovaeYAxis(fovaeSize[1]);
	return cellInformation;
}
//vector<int> MainDirectionGanglionProcessing::cellInformation(9);

Mat MainDirectionGanglionProcessing::previousInput(vector<Mat> cellMemory, int currentMemoryPosition) {
	Mat previousInputMatrix;
	int previousPosOfMemory = currentMemoryPosition - 1;
	if (previousPosOfMemory == -1) {
		previousPosOfMemory = 4;
	}
	previousInputMatrix = cellMemory[previousPosOfMemory];
	return previousInputMatrix;
}

int MainDirectionGanglionProcessing::movementSensing(double prevois, double firstSide, double secondSide) {
	int movementBool;
	double differenceOfAdjacent, differenceFromPrevious;
	differenceOfAdjacent = abs(secondSide - firstSide);
	differenceFromPrevious = abs(prevois - firstSide);
	// deciding the existance of edge movement
	if (differenceOfAdjacent < 0.05 && differenceFromPrevious > 0.1) { //should play with the values here - might convert them to adjustable variables
		movementBool = 1;
	}
	else {
		movementBool = 0;
	}
	return movementBool;
}

vector<Mat> MainDirectionGanglionProcessing::directionReceptiveFieldProcessing(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator) {
	//calling helper functions
	SynapticStrength ss;
	AccessoryFunctions af;
	//declaring local variable types
	Mat previousInputMatrix;
	vector<Mat> processedMatrices;
	Mat upwardMovementMatrix;
	Mat downwardMovementMatrix;
	Mat rightMovementMatrix;
	Mat leftMovementMatrix;
	Size matrixSize;
	bool isFirst;
	Mat synapticStrengthMatrix;
	double ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	double threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(inputMatrix, currentMemoryPosition);
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	inputMatrix = inputMatrix.mul(synapticStrengthMatrix);
	//loading previous input matirx
	previousInputMatrix = previousInput(m.memory, currentMemoryPosition);
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; ij = ij + cellInformation[3]) {
			vector<Mat> upKernels;
			vector<Mat> downKernels;
			vector<Mat> rightKernels;
			vector<Mat> leftKernels;
			int movementBool;
			double ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp, ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown,
				ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight, ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft;
			upKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "up");
			downKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "down");
			rightKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "right");
			leftKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "left");
			// calculating result using the kernels
			// direction - up
			ratioOfPrevoiusUp = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(upKernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfFirstSideUp = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(upKernels)[1]) / pow((double)cellInformation[1], 2.0));;
			ratioOfSecondSideUp = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(upKernels)[2]) / pow((double)cellInformation[1], 2.0));
			int upwardMovement = movementSensing(ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp);
			// direction - down
			ratioOfPrevoiusDown = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(downKernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfFirstSideDown = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(downKernels)[1]) / pow((double)cellInformation[1], 2.0));;
			ratioOfSecondSideDown = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(downKernels)[2]) / pow((double)cellInformation[1], 2.0));
			int downwardMovement = movementSensing(ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown);
			// direction - right
			ratioOfPrevoiusRight = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(rightKernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfFirstSideRight = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(rightKernels)[1]) / pow((double)cellInformation[1], 2.0));;
			ratioOfSecondSideRight = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(rightKernels)[2]) / pow((double)cellInformation[1], 2.0));
			int rightMovement = movementSensing(ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight);
			// direction - left
			ratioOfPrevoiusLeft = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(leftKernels)[0]) / pow((double)cellInformation[1], 2.0));
			ratioOfFirstSideLeft = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(leftKernels)[1]) / pow((double)cellInformation[1], 2.0));;
			ratioOfSecondSideLeft = abs(1 - (pow((double)cellInformation[1], 2.0) - sum(leftKernels)[2]) / pow((double)cellInformation[1], 2.0));
			int leftMovement = movementSensing(ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft);

			//creating processed matrix vector
			upwardMovementMatrix.at<double>(iterX, iterY) = upwardMovement;
			downwardMovementMatrix.at<double>(iterX, iterY) = downwardMovement;
			rightMovementMatrix.at<double>(iterX, iterY) = rightMovement;
			leftMovementMatrix.at<double>(iterX, iterY) = leftMovement;
			iterX++;
		}
		iterY++;
	}
	processedMatrices.push_back(upwardMovementMatrix);
	processedMatrices.push_back(downwardMovementMatrix);
	processedMatrices.push_back(rightMovementMatrix);
	processedMatrices.push_back(leftMovementMatrix);
	return processedMatrices;
}

void MainDirectionGanglionProcessing::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		redGreenMemory.resize(iterator);
	}
	else {
		redGreenMemory.resize(m.getMemoryMax());
	}
}

void MainDirectionGanglionProcessing::loadToMemory(Memory& m) {
	if (redGreenMemory.size() != m.memory.size()) {
		m.memory.resize(redGreenMemory.size());
	}
	for (int it = 0; it < redGreenMemory.size(); it++) {
		m.memory[it] = redGreenMemory[it];
	}
}

void MainDirectionGanglionProcessing::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		redGreenMemory[it] = m.memory[it];
	}
}

vector<Mat> MainDirectionGanglionProcessing::mainDirectonInformation(Mat inputMatrix, vector<int> cellInformation, int mainIterator) {
	vector<Mat> mainDirectionInformation;
	Memory m;
	if (mainIterator == 1) {
		initializeCellInformation(inputMatrix);
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
		loadToMemory(m);
		directionReceptiveFieldProcessing(inputMatrix, cellInformation, m, mainIterator);
		loadFromMemory(m);
	}else {
		loadToMemory(m);
		directionReceptiveFieldProcessing(inputMatrix, cellInformation, m, mainIterator);
		loadFromMemory(m);
	}  //m.initializeMemory(blueYellowMemory); //adding later if it works
	return mainDirectionInformation;
}

//Gabor filtering
vector<double> GaborFiltering::initializeGaborFilterParameters() {
	vector<double> placeHolder;
	return placeHolder;
}

//get functions

double GaborFiltering::getGamma() {
	return gamma;
}

double GaborFiltering::getLambda() {
	return lambda;
}

double GaborFiltering::getSigma() {
	return sigma;
}

double GaborFiltering::getPsi() {
	return psi;
}

//set functions
vector<double> GaborFiltering::setTheta() {
	theta[0] = 0;
	theta[1] = 45;
	theta[2] = 90;
	theta[3] = 135;
	return theta;
}

double GaborFiltering::setGamma(double newValue) {
	gamma = newValue;
	return gamma;
}

double GaborFiltering::setLambda(double newValue) {
	lambda = newValue;
	return lambda;
}

double GaborFiltering::setSigma(double newValue) {
	sigma = newValue;
	return sigma;
}

double GaborFiltering::setPsi(double newValue) {
	psi = newValue;
	return psi;
}