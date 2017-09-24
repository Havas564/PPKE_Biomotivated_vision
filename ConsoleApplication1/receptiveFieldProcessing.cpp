#include "receptiveFieldProcessing.h"
#include "memoryHandling.h"
#include "synapticStrength.h"
#include "accessoryFunctions.h"

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
	Mat kernelCenter(fullKernel, Rect(peripheryThickness, peripheryThickness, centerSize, centerSize));
	//periphery creation
	Mat kernelPeriphery = fullKernel.mul(centerEreaser);
	//saving the two kernel to a common variable
	vector<Mat> kernels; //[centre, periphery]
	kernels.push_back(kernelCenter);
	kernels.push_back(kernelPeriphery);
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

//input evaulation in case of one input matrix
vector<Mat> ReceptiveFieldFunctions::receptorFieldEvaluationOneInput(Mat inputMatrix, bool onCell, vector<int> cellInformation, vector<Mat> cellMemory, int mainIterator) {
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
	if (onCell == true) {
		typeModifier = 0;
	}
	else {
		typeModifier = 1;
	}
	matrixSize = af.sizeOfMatrix(inputMatrix);
	//adding the current input to memory
	m.memoryPosition(mainIterator);
	m.pushbackMemory(inputMatrix, m.currentMemoryPosition, cellMemory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(cellMemory, m.currentMemoryPosition);
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
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
vector<Mat> ReceptiveFieldFunctions::receptorFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix, 
	bool onCell, vector<int> cellInformation, vector<Mat> firstCellMemory, vector<Mat> secondCellMemory, int mainIterator) {
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
		secondRatioOfOnInputCenter, secondRatioOfOffInputCenter, secondRatioOfOnInputPeriphery, secondRatioOfOffInputPeriphery, typeModifier;
	double threshold = 0.3;
	if (onCell == true) {
		typeModifier = 0;
	}
	else {
		typeModifier = 1;
	}
	matrixSize = af.sizeOfMatrix(firstInputMatrix);
	//adding the current input to memory
	m.memoryPosition(mainIterator);
	m.pushbackMemory(firstInputMatrix, m.currentMemoryPosition, firstCellMemory);
	m.pushbackMemory(secondInputMatrix, m.currentMemoryPosition, secondCellMemory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	ss.modifierMatrixCalculator(firstCellMemory, m.currentMemoryPosition);
	firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	firstInputMatrix = firstInputMatrix.mul(firstSynapticStrengthMatrix);
	ss.modifierMatrixCalculator(secondCellMemory, m.currentMemoryPosition);
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
