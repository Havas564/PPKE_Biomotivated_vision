#include "receptiveFieldProcessing.h"
#include "memoryHandling.h"	
#include "synapticStrength.h"
#include "accessoryFunctions.h"
#include "ImageProcessingDataAcquisition.h"

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
vector<Mat> ReceptiveFieldFunctions::kernelCreator(Mat kernelCenter, Mat kernelPeriphery) {
	vector<Mat> kernels; //[centre, periphery]
	kernels.resize(2);
	kernels[0] = kernelCenter;
	//cerr << kernelCenter << endl;
	//cout << endl;
	kernels[1] = kernelPeriphery;
	//cerr << kernelPeriphery << endl;
	//cout << endl;
	return kernels;
}

Mat ReceptiveFieldFunctions::kernelCenterReveicer(int centerSize, int kernelSize, int iterX, int iterY, Mat inputMatrix) {
	int peripheryThickness = (kernelSize - centerSize) / 2;
	Mat kernelCenter(inputMatrix, Rect(iterX + peripheryThickness, iterY + peripheryThickness, centerSize, centerSize));
	//cerr << kernelCenter << endl; cout << endl;
	return kernelCenter;
}

Mat ReceptiveFieldFunctions::kernelPeripheryReceiver(int centerSize, int kernelSize, int iterX, int iterY, Mat inputMatrix) {
	//Mat fullKernel(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
	Mat fullKernel = oneKernelCreator(inputMatrix, kernelSize, iterX, iterY);
	Mat kernelPeriphery = fullKernel;
	//float data[][] = {{}}
	int peripheryThickness = (kernelSize - centerSize) / 2;
	Mat kernelCenterZeros(fullKernel(Rect(peripheryThickness, peripheryThickness, centerSize, centerSize)));
	//cerr << kernelPeriphery << endl; cout << endl;
	Mat zeroMatrix = kernelCenterZeros - kernelCenterZeros;
	//kernelCenterZeros.setTo(0);
	Rect whereRec(peripheryThickness, peripheryThickness, centerSize, centerSize);
	zeroMatrix.copyTo(kernelPeriphery(whereRec));
	//cerr << kernelPeriphery << endl; cout << endl;
	return kernelPeriphery;
}

// function creating one kernel
Mat ReceptiveFieldFunctions::oneKernelCreator(Mat inputMatrix, int kernelSize, int iterX, int iterY) {
	Mat kernel(inputMatrix, Rect(iterX, iterY, kernelSize, kernelSize));
	//kernel.convertTo(kernel, CV_32F);
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
	//Mat_<float> processedMatrix;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	Mat processedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	//processedMatrix.convertTo(processedMatrix, CV_32F);
	Mat inputModifiedBySynapticStrength;
	bool isFirst = ss.isFirstIteration(mainIterator);
	Mat synapticStrengthMatrix;
	float ratioOfInput, threshold = 0.3;
	float floatPower = 2.0;
	//adding current input to memory
	inputMatrix = af.conversionToRatio(inputMatrix);
	//cerr << inputMatrix(Rect(100, 100, 100, 100)) << endl;
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	if (!isFirst) {
		ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	}
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	
	inputModifiedBySynapticStrength = inputMatrix.mul(synapticStrengthMatrix);
	
	//iterating through the matrix with the kernel
	int iterY = 0;
	for (int it = 0; it < ((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]); it++) {
		int iterX = 0;
		for (int ij = 0; ij < ((matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]); ij++) {
			Mat kernel = oneKernelCreator(inputModifiedBySynapticStrength, cellInformation[0], ij, it);
			//double sumOfKernel = sum(kernel)[0];
			ratioOfInput = 1 - (pow((float)cellInformation[0], floatPower) - sum(kernel)[0]) / pow((float)cellInformation[0], floatPower); // check if it gives the good value
			ratioOfInput = af.thresholding(ratioOfInput, threshold);
			processedMatrix.at<float>(iterY, iterX) = ratioOfInput;
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
	ImageProcessingDataAcquisition ipda;
	//declaring local variables
	vector<Mat> processedMatrices;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	Mat onProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat offProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	bool isFirst;
	Mat synapticStrengthMatrix;
	Mat inputModifiedBySynapticStrengthFirst;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery, typeModifier;
	float threshold = 0.3; 
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	inputMatrix = af.conversionToRatio(inputMatrix);
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	if (!isFirst) {
		ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	}
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	inputModifiedBySynapticStrengthFirst = inputMatrix.mul(synapticStrengthMatrix);
	//iterating through the matrix
	int iterY = 0;
	float floatPower = 2.0;
	for (int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]; ij = ij + cellInformation[3]) {
			vector<Mat> kernels;
			Mat kernelCenter, kernelPeriphery;
			//ratioOfInput = 1 - (pow((float)cellInformation[0], floatPower) - sum(kernel)[0]) / pow((float)cellInformation[0], floatPower); // check if it gives the good value
			kernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			ratioOfOnInputCenter = 1 - (pow((float)cellInformation[1], floatPower) - sum(kernelCenter)[0]) / pow((float)cellInformation[1], floatPower);
			ratioOfOffInputCenter = (pow((float)cellInformation[1], floatPower) - sum(kernelCenter)[0]) / pow((float)cellInformation[1], floatPower);
			kernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			ratioOfOnInputPeriphery = 1 - ((pow((float)cellInformation[0], floatPower) - pow((float)cellInformation[1], floatPower)) - sum(kernelPeriphery)[0]) / (pow((float)cellInformation[0], floatPower) - pow((float)cellInformation[1], floatPower));
			ratioOfOffInputPeriphery = (pow((float)cellInformation[0], floatPower) - pow((float)cellInformation[1], floatPower) - sum(kernelPeriphery)[0]) / (pow((float)cellInformation[0], floatPower) - pow((float)cellInformation[1], floatPower));
			// thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);
			// comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			// creating processed matrix vector
			onProcessedMatrix.at<float>(iterY, iterX) = ratioOfOnInputCenter;
			offProcessedMatrix.at<float>(iterY, iterX) = ratioOfOffInputCenter;
			iterX++;
		}
		iterY++;
	}
	/*cerr << onProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	cerr << offProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	ipda.imageShow(onProcessedMatrix);
	ipda.imageShow(offProcessedMatrix);
	*/
	//loading the processed matrices
	processedMatrices.push_back(onProcessedMatrix);
	processedMatrices.push_back(offProcessedMatrix);
	//fovae processing
	Mat onFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	Mat offFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	int iterYF = 0;
	//int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]
	float ratioOfFovaeOnInputCenter, rationOfFovaeOffInputCenter, rationOfFovaeOnInputPeriphery, rationOfFovaeOffInputPeriphery;
	for (int it = cellInformation[10]; it < cellInformation[10] + (cellInformation[8] / cellInformation[4] * cellInformation[4] - cellInformation[4]); it = it + cellInformation[5]) {
		int iterXF = 0;
		for (int ij = cellInformation[9]; ij < cellInformation[9] + (cellInformation[7] / cellInformation[4] * cellInformation[4] - cellInformation[4]); ij = ij + cellInformation[5]) {
			Mat fovaeKernel = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthFirst); //= kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, fovaeMatrix);
			ratioOfFovaeOnInputCenter = 1 - (pow((float)cellInformation[6], floatPower) - sum(fovaeKernel)[0]) / pow((float)cellInformation[6], floatPower);
			rationOfFovaeOffInputCenter = (pow((float)cellInformation[6], floatPower) - sum(fovaeKernel)[0]) / pow((float)cellInformation[6], floatPower);
			Mat fovaePeriphery = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthFirst); //= kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, fovaeMatrix);
			rationOfFovaeOnInputPeriphery = 1 - ((pow((float)cellInformation[4], floatPower) - pow((float)cellInformation[6], floatPower)) - sum(fovaePeriphery)[0]) / (pow((float)cellInformation[4], floatPower) - pow((float)cellInformation[6], floatPower));
			rationOfFovaeOffInputPeriphery = (pow((float)cellInformation[4], floatPower) - pow((float)cellInformation[6], floatPower) - sum(fovaePeriphery)[0]) / (pow((float)cellInformation[4], floatPower) - pow((float)cellInformation[6], floatPower));
			//thresholding
			ratioOfFovaeOnInputCenter = af.thresholding(ratioOfFovaeOnInputCenter, threshold);
			rationOfFovaeOnInputPeriphery = af.thresholding(rationOfFovaeOnInputPeriphery, threshold);
			rationOfFovaeOffInputCenter = af.thresholding(rationOfFovaeOffInputCenter, threshold);
			rationOfFovaeOffInputPeriphery = af.thresholding(rationOfFovaeOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			ratioOfFovaeOnInputCenter = centerPeripheryComparison(ratioOfFovaeOnInputCenter, rationOfFovaeOnInputPeriphery);
			rationOfFovaeOffInputCenter = centerPeripheryComparison(rationOfFovaeOffInputCenter, rationOfFovaeOffInputPeriphery);
			//creating processed matrix vector
			onFovaeProcessedMatrix.at<float>(iterYF, iterXF) = ratioOfFovaeOnInputCenter;
			offFovaeProcessedMatrix.at<float>(iterYF, iterXF) = rationOfFovaeOffInputCenter;
			iterXF++;
		}
		iterYF++;
	}
	return processedMatrices;
}

//input evaulation in case of two input matrices
vector<Mat> ReceptiveFieldFunctions::receptiveFieldEvaluationTwoInput(Mat firstInputMatrix, Mat secondInputMatrix,
	vector<int> cellInformation, Memory& m, int mainIterator) {
	//calling helper function
	SynapticStrength ss;
	AccessoryFunctions af;
	ImageProcessingDataAcquisition ipda;
	//declaring local variables
	vector<Mat> processedMatrices;
	Size matrixSize = af.sizeOfMatrix(firstInputMatrix);
	Mat firstOnProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat firstOffProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat secondOnProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat secondOffProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat inputModifiedBySynapticStrengthFirst;
	Mat inputModifiedBySynapticStrengthSecond;
	bool isFirst;
	Mat firstSynapticStrengthMatrix;
	Mat secondSynapticStrengthMatrix;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	firstInputMatrix = af.conversionToRatio(firstInputMatrix);
	secondInputMatrix = af.conversionToRatio(secondInputMatrix);
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	if (!isFirst) {
		ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
	}
	firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	inputModifiedBySynapticStrengthFirst = firstInputMatrix.mul(firstSynapticStrengthMatrix);
	if (!isFirst) {
		ss.modifierMatrixCalculator(m.memorySecond, currentMemoryPosition);
	}
	secondSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(ss.modifierMatrix, matrixSize, isFirst);
	inputModifiedBySynapticStrengthSecond = secondInputMatrix.mul(secondSynapticStrengthMatrix);
	//iterating through the matrix
	float firstRatioOfOnInputCenter, firstRatioOfOffInputCenter, firstRatioOfOnInputPeriphery, firstRatioOfOffInputPeriphery,
		secondRatioOfOnInputCenter, secondRatioOfOffInputCenter, secondRatioOfOnInputPeriphery, secondRatioOfOffInputPeriphery;
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0]; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0]; ij = ij + cellInformation[3]) {
			vector<Mat> firstKernels;
			vector<Mat> secondKernels;
			Mat firstKernelCenter, firstKernelPeriphery, secondKernelCenter, secondKernelPeriphery;
			firstKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			//cerr << firstKernelCenter << endl;
			//cout << sum(firstKernelCenter)[0] << endl;
			firstRatioOfOnInputCenter = 1 - (pow((float)cellInformation[1], 2.0) - sum(firstKernelCenter)[0]) / pow((float)cellInformation[1], 2.0);
			firstRatioOfOffInputCenter = (pow((float)cellInformation[1], 2.0) - sum(firstKernelCenter)[0]) / pow((float)cellInformation[1], 2.0);
			//cout << firstRatioOfOnInputCenter << endl;
			firstKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			firstRatioOfOnInputPeriphery = 1 - (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0) - sum(firstKernelPeriphery)[0]) / (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0));
			firstRatioOfOffInputPeriphery = (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0) - sum(firstKernelPeriphery)[0]) / (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0));
			//cerr << firstKernelCenter << endl;
			//cerr << firstKernelPeriphery << endl;
			secondKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthSecond);
			secondRatioOfOnInputCenter = 1 - (pow((float)cellInformation[1], 2.0) - sum(secondKernelCenter)[0]) / pow((float)cellInformation[1], 2.0);
			secondRatioOfOffInputCenter = (pow((float)cellInformation[1], 2.0) - sum(secondKernelCenter)[0]) / pow((float)cellInformation[1], 2.0);
			secondKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthSecond);
			secondRatioOfOnInputPeriphery = 1 - (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0) - sum(secondKernelPeriphery)[0]) / (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0));
			secondRatioOfOffInputPeriphery = (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0) - sum(secondKernelPeriphery)[0]) / (pow((float)cellInformation[0], 2.0) - pow((float)cellInformation[1], 2.0));

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
			firstOnProcessedMatrix.at<float>(iterY, iterX) = firstRatioOfOnInputCenter;
			firstOffProcessedMatrix.at<float>(iterY, iterX) = firstRatioOfOffInputCenter;
			secondOnProcessedMatrix.at<float>(iterY, iterX) = secondRatioOfOnInputCenter;
			secondOffProcessedMatrix.at<float>(iterY, iterX) = secondRatioOfOffInputCenter;
			iterX++;
		}
		iterY++;
	}
	//loading the processed matrices
	/*cerr << firstOnProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	cerr << firstOffProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	cerr << secondOnProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	cerr << secondOffProcessedMatrix(Rect(100, 100, 100, 100)) << endl;
	cout << endl;
	cout << endl;
	ipda.imageShow(firstOnProcessedMatrix);
	ipda.imageShow(firstOffProcessedMatrix);
	ipda.imageShow(secondOnProcessedMatrix);
	ipda.imageShow(secondOffProcessedMatrix);*/
	processedMatrices.push_back(firstOnProcessedMatrix);
	processedMatrices.push_back(firstOffProcessedMatrix);
	processedMatrices.push_back(secondOnProcessedMatrix);
	processedMatrices.push_back(secondOffProcessedMatrix);
	//fovae processing
	Mat firstOnFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	Mat firstOffFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	Mat secondOnFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	Mat secondOffFovaeProcessedMatrix((cellInformation[8] / cellInformation[4]) * cellInformation[4] - cellInformation[4],
		(cellInformation[7] / cellInformation[4]) * cellInformation[4] - cellInformation[4], CV_32F);
	float firstRatioOfFovaeOnInputCenter, firstRatioOfFovaeOffInputCenter, firstRatioOfFovaeOnInputPeriphery, firstRatioOfFovaeOffInputPeriphery,
		secondRatioOfFovaeOnInputCenter, secondRatioOfFovaeOffInputCenter, secondRatioOfFovaeOnInputPeriphery, secondRatioOfFovaeOffInputPeriphery;
	int iterYF = 0;
	for (int it = cellInformation[10]; it < cellInformation[10] + (cellInformation[8] / cellInformation[4] * cellInformation[4] - cellInformation[4]); it = it + cellInformation[5]) {
		int iterXF = 0;
		for (int ij = cellInformation[9]; ij < cellInformation[9] + (cellInformation[7] / cellInformation[4] * cellInformation[4] - cellInformation[4]); ij = ij + cellInformation[5]) {
			Mat firstKernelCenterFovae = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthFirst);
			firstRatioOfFovaeOnInputCenter = 1 - (pow((float)cellInformation[6], 2.0) - sum(firstKernelCenterFovae)[0]) / pow((float)cellInformation[6], 2.0);
			firstRatioOfFovaeOffInputCenter = (pow((float)cellInformation[6], 2.0) - sum(firstKernelCenterFovae)[0]) / pow((float)cellInformation[6], 2.0);
			Mat firstKernelPeripheryFovae = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthFirst);
			firstRatioOfFovaeOnInputPeriphery = 1 - (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0) - sum(firstKernelPeripheryFovae)[0]) / (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0));
			firstRatioOfFovaeOffInputPeriphery = (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0) - sum(firstKernelPeripheryFovae)[0]) / (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0));
			Mat secondKernelCenterFovae = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthSecond);
			secondRatioOfFovaeOnInputCenter = 1 - (pow((float)cellInformation[6], 2.0) - sum(secondKernelCenterFovae)[0]) / pow((float)cellInformation[6], 2.0);
			secondRatioOfFovaeOffInputCenter = (pow((float)cellInformation[6], 2.0) - sum(secondKernelCenterFovae)[0]) / pow((float)cellInformation[6], 2.0);
			Mat secondKernelPeripheryFovae = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, inputModifiedBySynapticStrengthSecond);
			secondRatioOfFovaeOnInputPeriphery = 1 - (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0) - sum(secondKernelPeripheryFovae)[0]) / (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0));
			secondRatioOfFovaeOffInputPeriphery = (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0) - sum(secondKernelPeripheryFovae)[0]) / (pow((float)cellInformation[4], 2.0) - pow((float)cellInformation[6], 2.0));
			
			//thresholding
			firstRatioOfFovaeOnInputCenter = af.thresholding(firstRatioOfFovaeOnInputCenter, threshold);
			firstRatioOfFovaeOnInputPeriphery = af.thresholding(firstRatioOfFovaeOnInputPeriphery, threshold);
			firstRatioOfFovaeOffInputCenter = af.thresholding(firstRatioOfFovaeOffInputCenter, threshold);
			firstRatioOfFovaeOffInputPeriphery = af.thresholding(firstRatioOfFovaeOffInputPeriphery, threshold);
			secondRatioOfFovaeOnInputCenter = af.thresholding(secondRatioOfFovaeOnInputCenter, threshold);
			secondRatioOfFovaeOnInputPeriphery = af.thresholding(secondRatioOfFovaeOnInputPeriphery, threshold);
			secondRatioOfFovaeOffInputCenter = af.thresholding(secondRatioOfFovaeOffInputCenter, threshold);
			secondRatioOfFovaeOffInputPeriphery = af.thresholding(secondRatioOfFovaeOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			firstRatioOfFovaeOnInputCenter = centerPeripheryComparison(firstRatioOfFovaeOnInputCenter, secondRatioOfFovaeOnInputPeriphery);
			firstRatioOfFovaeOffInputCenter = centerPeripheryComparison(firstRatioOfFovaeOffInputCenter, secondRatioOfFovaeOffInputPeriphery);
			secondRatioOfFovaeOnInputCenter = centerPeripheryComparison(secondRatioOfFovaeOnInputCenter, firstRatioOfFovaeOnInputPeriphery);
			secondRatioOfFovaeOffInputCenter = centerPeripheryComparison(secondRatioOfFovaeOffInputCenter, firstRatioOfFovaeOffInputPeriphery);
			//creating processed matrix vector
			firstOnFovaeProcessedMatrix.at<float>(iterYF, iterXF) = firstRatioOfFovaeOnInputCenter;
			firstOffFovaeProcessedMatrix.at<float>(iterYF, iterXF) = firstRatioOfFovaeOffInputCenter;
			secondOnFovaeProcessedMatrix.at<float>(iterYF, iterXF) = secondRatioOfFovaeOnInputCenter;
			secondOffFovaeProcessedMatrix.at<float>(iterYF, iterXF) = secondRatioOfFovaeOffInputCenter;

			iterXF++;
		}
		iterYF++;
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
	temp = fovae.width * 0.2;
	fovaeSize.push_back(temp);
	temp = fovae.height * 0.2;
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
	fovaeCenterCoordinates.push_back((matrixSize.width / 2) - (0.1 * matrixSize.width));
	fovaeCenterCoordinates.push_back((matrixSize.height / 2) - (0.1 * matrixSize.height));
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

vector<int> ReceptiveFieldFunctions::setFovaeXAxisLength(int newValue) {
	cellInformation[7] = newValue;
	return cellInformation;
}

vector<int> ReceptiveFieldFunctions::setFovaeYAxisLength(int newValue) {
	cellInformation[8] = newValue;
	return cellInformation;
}

void ReceptiveFieldFunctions::setFovaeXAxisPosition(int newValue) {
	cellInformation[9] = newValue;
}

void ReceptiveFieldFunctions::setFovaeYAxisPosition(int newValue) {
	cellInformation[10] = newValue;
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

int ReceptiveFieldFunctions::getFovaeXAxisLength() {
	return cellInformation[7];
}

int ReceptiveFieldFunctions::getFovaeYAxisLength() {
	return cellInformation[8];
}

vector<int> ReceptiveFieldFunctions::getAllCellInformation() {
	return cellInformation;
}

int ReceptiveFieldFunctions::getFovaeXAxisPosition() {
	return cellInformation[9];
}

int ReceptiveFieldFunctions::getFovaeYAxisPosition() {
	return cellInformation[10];
}

//ROD bipolar processing
vector<int> RodBipolarProcessing::initializeCellInformation() {
	setKernelSize(5);
	setCenterSize(0);
	setPeripherySize(0);
	setKernelStep(1);
	return cellInformation;
}
vector<int> RodBipolarProcessing::cellInformation(11);

void RodBipolarProcessing::initializeMemory(int iterator) {
	Memory m;
	if (iterator < m.getMemoryMax()) {
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
	// converting AII amacrine output to ratio for all-cone discrimination
	processedMatrix = processedMatrix / 10.0;
	return processedMatrix;
}

//MIDGET Pathway
vector<int> MidgetSConePathway::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(4);
	setCenterSize(2);
	setPeripherySize(2);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxisLength(fovaeSize[0]);
	setFovaeYAxisLength(fovaeSize[1]);
	vector<int> fovaeCoordinates = fovaeCenterCoordinatesAcquirer(inputMatrix);
	setFovaeXAxisPosition(fovaeCoordinates[0]);
	setFovaeYAxisPosition(fovaeCoordinates[1]);
	return cellInformation;
}

void MidgetSConePathway::initializeMemory(int iterator) {
	Memory m;
	if (iterator < 5) {
		sConeMemory.resize(iterator);
	}
	else {
		sConeMemory.resize(m.getMemoryMax());
	}
}

void MidgetSConePathway::loadToMemory(Memory& m) {
	if (sConeMemory.size() != m.memory.size()) {
		m.memory.resize(sConeMemory.size());
	}
	for (int it = 0; it < sConeMemory.size(); it++) {
		m.memory[it] = sConeMemory[it];
	}
}

void MidgetSConePathway::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		sConeMemory[it] = m.memory[it];
	}
}

vector<Mat> MidgetSConePathway::midgetBipolarProcessing(Mat inputMatrix, int mainIterator) {
	Memory m;
	if (mainIterator == 1) {
		initializeCellInformation(inputMatrix);
		initializeMemory(mainIterator);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
	}
	loadToMemory(m);
	//red-green discrimination
	vector<Mat> processedMatrices = receptiveFieldEvaluationOneInput(inputMatrix, cellInformation, m, mainIterator);
	loadToMemory(m);
	return processedMatrices;
}

//REDGREEN discriminator
vector<int> RedGreenDiscrimination::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(4);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxisLength(fovaeSize[0]);
	setFovaeYAxisLength(fovaeSize[1]);
	vector<int> fovaeCoordinates = fovaeCenterCoordinatesAcquirer(inputMatrix);
	setFovaeXAxisPosition(fovaeCoordinates[0]);
	setFovaeYAxisPosition(fovaeCoordinates[1]);
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
		m.memorySecond.resize(mConeMemory.size());
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
	setKernelSize(4);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxisLength(fovaeSize[0]);
	setFovaeYAxisLength(fovaeSize[1]);
	vector<int> fovaeCoordinates = fovaeCenterCoordinatesAcquirer(inputMatrix);
	setFovaeXAxisPosition(fovaeCoordinates[0]);
	setFovaeYAxisPosition(fovaeCoordinates[1]);
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
		m.memorySecond.resize(sConeMemory.size());
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
	setKernelSize(4);
	setCenterSize(2);
	setPeripherySize(cellInformation[0] - cellInformation[1]);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(1);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxisLength(fovaeSize[0]);
	setFovaeYAxisLength(fovaeSize[1]);
	vector<int> fovaeCoordinates = fovaeCenterCoordinatesAcquirer(inputMatrix);
	setFovaeXAxisPosition(fovaeCoordinates[0]);
	setFovaeYAxisPosition(fovaeCoordinates[1]);
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
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	float threshold = 0.3;
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
			Mat firstKernelCenter, secondKernelCenter, ThirdKernelCenter, firstKernelPeriphery, secondKernelPeriphery, thirdKernelPeriphery;
			firstKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, firstInputMatrix);
			firstKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, firstInputMatrix);
			secondKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, secondInputMatrix);
			secondKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, secondInputMatrix);
			ThirdKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, thirdInputMatrix);
			thirdKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, thirdInputMatrix);
			firstKernels = kernelCreator(firstKernelCenter, firstKernelPeriphery);
			secondKernels = kernelCreator(secondKernelCenter, secondKernelPeriphery);
			thirdKernels = kernelCreator(ThirdKernelCenter, thirdKernelPeriphery);
			averagedKernels[0] = (firstKernels[0] + secondKernels[0] + thirdKernels[0]) / 3.0;
			averagedKernels[1] = (firstKernels[1] + secondKernels[1] + thirdKernels[1]) / 3.0;
			// calculating result using the kernels
			ratioOfOnInputCenter =  abs(1 - (pow((float)cellInformation[1], 2.0) - sum(averagedKernels)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((float)cellInformation[1], 2.0) - pow((float)cellInformation[1], 2.0) - sum(averagedKernels)[0]) / pow((float)cellInformation[1], 2.0));;
			ratioOfOffInputCenter = abs((1 - (pow((float)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((float)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((float)cellInformation[1], 2.0) - pow((float)cellInformation[1], 2.0) - sum(firstKernels)[0]) / pow((float)cellInformation[1], 2.0)) - 1);

			//thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);

			//comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			//creating processed matrix vector
			averagedOnProcessedMatrix.at<float>(iterY, iterX) = ratioOfOnInputCenter;
			averagedOffProcessedMatrix.at<float>(iterY, iterX) = ratioOfOffInputCenter;
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
			Mat firstKernelCenterFovae, secondKernelCenterFovae, ThirdKernelCenterFovae,
				firstKernelPeripheryFovae, secondKernelPeripheryFovae, thirdKernelPeripheryFovae;
			firstKernelCenterFovae = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, firstFovaeMatrix);
			firstKernelPeripheryFovae = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, firstFovaeMatrix);
			secondKernelCenterFovae = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, secondFovaeMatrix);
			secondKernelPeripheryFovae = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, secondFovaeMatrix);
			ThirdKernelCenterFovae = kernelCenterReveicer(cellInformation[6], cellInformation[4], ij, it, thirdFovaeMatrix);
			thirdKernelPeripheryFovae = kernelPeripheryReceiver(cellInformation[6], cellInformation[4], ij, it, thirdFovaeMatrix);
			firstFovaeKernel = kernelCreator(firstKernelCenterFovae, firstKernelPeripheryFovae);
			secondFovaeKernel = kernelCreator(secondKernelCenterFovae, secondKernelPeripheryFovae);
			thirdFovaeKernel = kernelCreator(ThirdKernelCenterFovae, thirdKernelPeripheryFovae);
			averagedFovaeKernels[0] = (firstFovaeKernel[0] + secondFovaeKernel[0] + thirdFovaeKernel[0]) / 3.0;
			averagedFovaeKernels[1] = (firstFovaeKernel[1] + secondFovaeKernel[1] + thirdFovaeKernel[1]) / 3.0;
			// calculating result using the kernels
			ratioOfOnInputCenter = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfOnInputPeriphery = abs(1 - (pow((float)cellInformation[1], 2.0) - pow((float)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfOffInputCenter = abs((1 - (pow((float)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((float)cellInformation[1], 2.0)) - 1);
			ratioOfOffInputPeriphery = abs((1 - (pow((float)cellInformation[1], 2.0) - pow((float)cellInformation[1], 2.0) - sum(firstFovaeKernel)[0]) / pow((float)cellInformation[1], 2.0)) - 1);
			//thresholding
			ratioOfOnInputCenter = af.thresholding(ratioOfOnInputCenter, threshold);
			ratioOfOnInputPeriphery = af.thresholding(ratioOfOnInputPeriphery, threshold);
			ratioOfOffInputCenter = af.thresholding(ratioOfOffInputCenter, threshold);
			ratioOfOffInputPeriphery = af.thresholding(ratioOfOffInputPeriphery, threshold);
			//comparing the centre and the periphery
			ratioOfOnInputCenter = centerPeripheryComparison(ratioOfOnInputCenter, ratioOfOnInputPeriphery);
			ratioOfOffInputCenter = centerPeripheryComparison(ratioOfOffInputCenter, ratioOfOffInputPeriphery);
			//creating processed matrix vector
			onFovaeProcessedMatrix.at<float>(it, ij) = ratioOfOnInputCenter;
			offFovaeProcessedMatrix.at<float>(it, ij) = ratioOfOffInputCenter;
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
		m.memorySecond.resize(lConeMemory.size());
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
	setKernelSize(4);
	setCenterSize(0);
	setPeripherySize(0);
	setKernelStep(1);
	setFovaeKernelSize(3);
	setFovaeKernelStep(1);
	setFocaeCenterSize(0);
	vector<int> fovaeSize;
	fovaeSize = fovaeSizeAcquirer(inputMatrix);
	setFovaeXAxisLength(fovaeSize[0]);
	setFovaeYAxisLength(fovaeSize[1]);
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

int MainDirectionGanglionProcessing::movementSensing(float prevois, float firstSide, float secondSide) {
	int movementBool;
	float differenceOfAdjacent, differenceFromPrevious;
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
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	Mat upwardMovementMatrix(matrixSize.height, matrixSize.height, CV_32F);
	Mat downwardMovementMatrix(matrixSize.height, matrixSize.height, CV_32F);
	Mat rightMovementMatrix(matrixSize.height, matrixSize.height, CV_32F);
	Mat leftMovementMatrix(matrixSize.height, matrixSize.height, CV_32F);
	bool isFirst;
	Mat synapticStrengthMatrix;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	inputMatrix = af.conversionToRatio(inputMatrix);
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
			float ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp, ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown,
				ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight, ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft;
			upKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "up");
			downKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "down");
			rightKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "right");
			leftKernels = displacementKernelCreator(inputMatrix, previousInputMatrix, cellInformation[0], ij, it, "left");
			// calculating result using the kernels
			// direction - up
			ratioOfPrevoiusUp = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(upKernels)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfFirstSideUp = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(upKernels)[1]) / pow((float)cellInformation[1], 2.0));;
			ratioOfSecondSideUp = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(upKernels)[2]) / pow((float)cellInformation[1], 2.0));
			int upwardMovement = movementSensing(ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp);
			// direction - down
			ratioOfPrevoiusDown = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(downKernels)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfFirstSideDown = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(downKernels)[1]) / pow((float)cellInformation[1], 2.0));;
			ratioOfSecondSideDown = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(downKernels)[2]) / pow((float)cellInformation[1], 2.0));
			int downwardMovement = movementSensing(ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown);
			// direction - right
			ratioOfPrevoiusRight = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(rightKernels)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfFirstSideRight = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(rightKernels)[1]) / pow((float)cellInformation[1], 2.0));;
			ratioOfSecondSideRight = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(rightKernels)[2]) / pow((float)cellInformation[1], 2.0));
			int rightMovement = movementSensing(ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight);
			// direction - left
			ratioOfPrevoiusLeft = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(leftKernels)[0]) / pow((float)cellInformation[1], 2.0));
			ratioOfFirstSideLeft = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(leftKernels)[1]) / pow((float)cellInformation[1], 2.0));;
			ratioOfSecondSideLeft = abs(1 - (pow((float)cellInformation[1], 2.0) - sum(leftKernels)[2]) / pow((float)cellInformation[1], 2.0));
			int leftMovement = movementSensing(ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft);

			//creating processed matrix vector
			upwardMovementMatrix.at<float>(iterY, iterX) = upwardMovement;
			downwardMovementMatrix.at<float>(iterY, iterX) = downwardMovement;
			rightMovementMatrix.at<float>(iterY, iterX) = rightMovement;
			leftMovementMatrix.at<float>(iterY, iterX) = leftMovement;
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

vector<Mat> MainDirectionGanglionProcessing::mainDirectonInformation(Mat inputMatrix, int mainIterator) {
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
void GaborFiltering::initializeGaborFilterParameters() {
	setTheta();
	setGamma(0.02);
	setLambda(1.0);
	setSigma(1);
	setPsi(0);
}

void GaborFiltering::gaborFiltering(Mat inputMatrix) {
	inputMatrix.convertTo(inputMatrix, CV_32F);
	for (int it = 0; it < theta.size(); it++) {
		Mat currentMatrix;
		Mat kernel = getGaborKernel(Size(getKernelSize(), getKernelSize()), sigma, theta[it], lambda, gamma, psi);
		filter2D(inputMatrix, currentMatrix, CV_32F, kernel);

		cerr << currentMatrix(Rect(30, 30, 10, 10)) << endl; // peek into the data

		Mat viz;
		currentMatrix.convertTo(viz, CV_8U, 1.0 / 255.0);     // move to proper[0..255] range to show it
		//imshow("k", kernel);
		//imshow("d", viz);
		//waitKey();
		currentGaborFilterResult.push_back(currentMatrix);
	}
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

int GaborFiltering::getKernelSize(void) {
	return kernelSize;
}

vector<Mat> GaborFiltering::getCurrentGaborFilterResult() {
	return currentGaborFilterResult;
}


//set functions
void GaborFiltering::setTheta() {
	theta[0] = 0;
	theta[1] = 45;
	theta[2] = 90;
	theta[3] = 135;
}

void GaborFiltering::setGamma(double newValue) {
	gamma = newValue;
}

void GaborFiltering::setLambda(double newValue) {
	lambda = newValue;
}

void GaborFiltering::setSigma(double newValue) {
	sigma = newValue;
}

void GaborFiltering::setPsi(double newValue) {
	psi = newValue;
}

void GaborFiltering::setKernelSize(int newValue) {
	kernelSize = newValue;
}