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

// function creating kernel with different side size (e.g. for movement ganglion cells)
vector<Mat> ReceptiveFieldFunctions::displacementKernelCreator(Mat inputMatrix, Mat previousInputMatrix, int kernelSize,
	int iterX, int iterY, string mainDirection) {
	AccessoryFunctions af; // for debugging purpose
	vector<Mat> kernels;
	if (mainDirection == "up") {
		Mat kernel1(kernelSize, kernelSize, inputMatrix.type());
		Mat kernel2(kernelSize, kernelSize, inputMatrix.type());
		Mat kernel3(kernelSize, kernelSize, inputMatrix.type());
		Mat temp(kernelSize, kernelSize, inputMatrix.type());
		//Mat temp(previousInputMatrix, Rect(iterX, iterY + kernelSize, kernelSize, kernelSize));
		//vector<Rect> receptiveFields;
		Rect receptiveField1(iterX, iterY + kernelSize, kernelSize, kernelSize);
		//receptiveFields.push_back(receptiveField1);
		temp = previousInputMatrix(receptiveField1);
		//temp.copyTo(kernel1); // bugs out
		kernel1 = temp.clone();
		kernels.push_back(kernel1);
		temp.release();
		Rect receptiveField2(iterX, iterY, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField2);
		//temp.copyTo(kernel2);
		kernel2 = temp.clone();
		kernels.push_back(kernel2);
		temp.release();
		Rect receptiveField3(iterX, iterY + kernelSize, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField3);
		//temp.copyTo(kernel3);
		kernel3 = temp.clone();
		kernels.push_back(kernel3);
		temp.release();
		/*Mat tempTest;
		tempTest = af.conversionToValue(kernel1);
		imwrite("E:/TestFolder/debugDirection1.jpg", tempTest);
		tempTest.release();
		tempTest = af.conversionToValue(kernel2);
		imwrite("E:/TestFolder/debugDirection2.jpg", tempTest);
		tempTest.release();
		tempTest = af.conversionToValue(kernel3);
		imwrite("E:/TestFolder/debugDiretion3.jpg", tempTest);*/
	}
	else if (mainDirection == "down") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat kernel3(kernelSize, kernelSize, CV_32F);
		Mat temp;
		
		Rect receptiveField1(iterX, iterY, kernelSize, kernelSize);
		Rect receptiveField2(iterX, iterY + kernelSize, kernelSize, kernelSize);
		Rect receptiveField3(iterX, iterY, kernelSize, kernelSize);
		temp = previousInputMatrix(receptiveField1);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		kernels.push_back(kernel2);
		temp.release();
		temp = inputMatrix(receptiveField3);
		temp.copyTo(kernel3);
		kernels.push_back(kernel3);
		kernels.push_back(kernel3);
		temp.release();
	}
	else if (mainDirection == "right") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat kernel3(kernelSize, kernelSize, CV_32F);
		Mat temp;

		Rect receptiveField1(iterX, iterY, kernelSize, kernelSize);
		Rect receptiveField2(iterX + kernelSize, iterY, kernelSize, kernelSize);
		Rect receptiveField3(iterX, iterY, kernelSize, kernelSize);
		temp = previousInputMatrix(receptiveField1);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		kernels.push_back(kernel2);
		temp.release();
		temp = inputMatrix(receptiveField3);
		temp.copyTo(kernel3);
		kernels.push_back(kernel3);
		kernels.push_back(kernel3);
		temp.release();
	}
	else if (mainDirection == "left") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat kernel3(kernelSize, kernelSize, CV_32F);
		Mat temp;

		Rect receptiveField1(iterX + kernelSize, iterY, kernelSize, kernelSize);
		Rect receptiveField2(iterX, iterY, kernelSize, kernelSize);
		Rect receptiveField3(iterX + kernelSize, iterY, kernelSize, kernelSize);
		temp = previousInputMatrix(receptiveField1);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		kernels.push_back(kernel2);
		temp.release();
		temp = inputMatrix(receptiveField3);
		temp.copyTo(kernel3);
		kernels.push_back(kernel3);
		kernels.push_back(kernel3);
		temp.release();
	}
	else {
		cout << "ERROR: Wrong direction given" << endl;
	}
	return kernels;
}

vector<Mat> ReceptiveFieldFunctions::displacementKernelCreatorNew(Mat inputMatrix, int kernelSize, int iterX, int iterY, string mainDirection)
{
	AccessoryFunctions af; // for debugging purpose
	vector<Mat> kernels;
	if (mainDirection == "up") {
		Mat kernel1(kernelSize, kernelSize, inputMatrix.type());
		Mat kernel2(kernelSize, kernelSize, inputMatrix.type());
		Mat temp(kernelSize, kernelSize, inputMatrix.type());
		Rect receptiveField1(iterX, iterY, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField1);
		kernel1 = temp.clone();
		kernels.push_back(kernel1);
		temp.release();
		Rect receptiveField2(iterX, iterY + kernelSize, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField2);
		kernel2 = temp.clone();
		kernels.push_back(kernel2);
		temp.release();
		/*Mat tempTest;
		tempTest = af.conversionToValue(kernel1);
		imwrite("E:/TestFolder/debugDirection1.jpg", tempTest);
		tempTest.release();
		tempTest = af.conversionToValue(kernel2);
		imwrite("E:/TestFolder/debugDirection2.jpg", tempTest);
		tempTest.release();*/
	}
	else if (mainDirection == "down") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat temp(kernelSize, kernelSize, inputMatrix.type());
;
		Rect receptiveField1(iterX, iterY + kernelSize, kernelSize, kernelSize);
		Rect receptiveField2(iterX, iterY, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		temp.release();
	}
	else if (mainDirection == "right") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat temp;

		Rect receptiveField1(iterX + kernelSize, iterY, kernelSize, kernelSize);
		Rect receptiveField2(iterX, iterY, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField1);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		temp.release();
	}
	else if (mainDirection == "left") {
		Mat kernel1(kernelSize, kernelSize, CV_32F);
		Mat kernel2(kernelSize, kernelSize, CV_32F);
		Mat temp;

		Rect receptiveField1(iterX, iterY, kernelSize, kernelSize);
		Rect receptiveField2(iterX + kernelSize, iterY, kernelSize, kernelSize);
		temp = inputMatrix(receptiveField1);
		temp.copyTo(kernel1);
		kernels.push_back(kernel1);
		temp.release();
		temp = inputMatrix(receptiveField2);
		temp.copyTo(kernel2);
		kernels.push_back(kernel2);
		temp.release();
	}
	else {
		cout << "ERROR: Wrong direction given" << endl;
	}
	return kernels;
}

// center-periphery comparison - sanszos hogy nem kell már
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
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	Mat processedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat inputModifiedBySynapticStrength;
	bool isFirst = ss.isFirstIteration(mainIterator);
	Mat synapticStrengthMatrix;
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	float ratioOfInput, threshold = 0.3;
	m.pushbackMemory(inputMatrix, currentMemoryPosition, m.memory);
	//modifying input with synapticStrength function
	if (mainIterator > 5)
	{
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		}
		synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);

		inputModifiedBySynapticStrength = inputMatrix.mul(synapticStrengthMatrix);
	}
	else
	{
		inputModifiedBySynapticStrength = inputMatrix;
	}
	
	// CONVOLVING WITH MEXICAN HAT FUNCTION
	processedMatrix = wholeInformationAcquierer(inputModifiedBySynapticStrength);
	Mat debugRod;
	debugRod = af.conversionToValue(processedMatrix);
	imwrite("E:/TestFolder/debugPicRod.jpg", debugRod);
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
	Mat processedMatrix, fovaeMatrix;
	Size matrixSize = af.sizeOfMatrix(inputMatrix);
	Mat onProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat offProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	bool isFirst;
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	Mat synapticStrengthMatrix;
	Mat inputModifiedBySynapticStrengthFirst;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery, typeModifier;
	float threshold = 0.3; 
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(inputMatrix, currentMemoryPosition, m.memory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	if (mainIterator > 5)
	{
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		}
		synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthFirst = inputMatrix.mul(synapticStrengthMatrix);
	}
	else
	{
		inputModifiedBySynapticStrengthFirst = inputMatrix;
	}
	
	Mat tempTest;
	tempTest = af.conversionToValue(inputModifiedBySynapticStrengthFirst);
	imwrite("E:/TestFolder/debugPic1.jpg", tempTest);
	// CONVOLVING WITH MEXICAN HAT FUNCTION
	processedMatrix = wholeInformationAcquierer(inputModifiedBySynapticStrengthFirst);
	processedMatrices.push_back(processedMatrix);
	Mat temp2;
	temp2 = af.conversionToValue(processedMatrix);
	imwrite("E:/TestFolder/debugPic2.jpg", temp2);
	fovaeMatrix = fovaeInformationAcquierer(inputModifiedBySynapticStrengthFirst);
	processedMatrices.push_back(fovaeMatrix);
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
	Mat differenceOfInput((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);
	Mat inputModifiedBySynapticStrengthFirst;
	Mat inputModifiedBySynapticStrengthSecond;
	bool isFirst;
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	Mat firstSynapticStrengthMatrix;
	Mat secondSynapticStrengthMatrix;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition, m.memory);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition, m.memorySecond);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	if (mainIterator > 5)
	{
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		}
		firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthFirst = firstInputMatrix.mul(firstSynapticStrengthMatrix);
		modifierMatrix.release(); // might need redefinition
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memorySecond, mainIterator);
		}
		secondSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthSecond = secondInputMatrix.mul(secondSynapticStrengthMatrix);
	}
	else
	{
		inputModifiedBySynapticStrengthFirst = firstInputMatrix;
		inputModifiedBySynapticStrengthSecond = secondInputMatrix;
	}
	//CONVOLVING WITH MAXICNA HAT FUNCTION
	Mat processedMatrixOn, fovaeMatrix;
	subtract(inputModifiedBySynapticStrengthFirst, inputModifiedBySynapticStrengthSecond, differenceOfInput);
	
	processedMatrixOn = wholeInformationAcquierer(inputModifiedBySynapticStrengthFirst);
	processedMatrices.push_back(processedMatrixOn);
	
	//fovae processing
	fovaeMatrix = fovaeInformationAcquierer(differenceOfInput);
	processedMatrices.push_back(fovaeMatrix);
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
	fovaeCenterCoordinates.push_back((matrixSize.width / 2) - (0.2 * matrixSize.width));
	fovaeCenterCoordinates.push_back((matrixSize.height / 2) - (0.2 * matrixSize.height));
	return fovaeCenterCoordinates;
}

vector<Mat> initiateMemory(vector<Mat> memoryType) {
	Memory m;
	memoryType = m.memory;
	return memoryType;
}

Mat ReceptiveFieldFunctions::gaussianKernelCreator(int receptiveFieldSize, float sigma) {
	Mat gaussianKernel(receptiveFieldSize, receptiveFieldSize, CV_32F);
	int x, y;
	for (int it = 0; it < receptiveFieldSize; it++) {
		for (int ij = 0; ij < receptiveFieldSize; ij++) {
			x = it - receptiveFieldSize / 2;
			y = ij - receptiveFieldSize / 2;
			gaussianKernel.at<float>(it, ij) = (1.0 / (1.0 * PI * pow(sigma, 2.0))) * exp(-(1.0 / (2.0 * pow(sigma, 2.0))) * (pow(x, 2.0) + pow(y, 2.0)));
		}
	}
	return gaussianKernel;
	//(1.0/(1.0 * pi * (sigma^2))) * exp(-(1.0 / (2*(sigma^2))) * (x^2 + y^2));
}

Mat ReceptiveFieldFunctions::differenceOfGaussiansCreator(int receptiveFieldSize, float sigma1, float sigma2) {
	Mat gaussianKernel(receptiveFieldSize, receptiveFieldSize, CV_32F);
	float temp1, temp2;
	float x, y;
	for (int it = 0; it < receptiveFieldSize; it++) {
		for (int ij = 0; ij < receptiveFieldSize; ij++) {
			x = it - receptiveFieldSize / 2;
			y = ij - receptiveFieldSize / 2;
			temp1 = (1.0 / (1.0 * PI * pow(sigma1, 2.0))) * exp(-(1.0 / (2.0 * pow(sigma1, 2.0))) * (pow(x, 2.0) + pow(y, 2.0)));
			temp2 = (1.0 / (1.0 * PI * pow(sigma2, 2.0))) * exp(-(1.0 / (2.0 * pow(sigma2, 2.0))) * (pow(x, 2.0) + pow(y, 2.0)));
			gaussianKernel.at<float>(it, ij) = temp1 - temp2;
		}
	}
	return gaussianKernel;
};

// fovea output matrix creation with Difference of Gaussians
Mat ReceptiveFieldFunctions::fovaeInformationAcquierer(Mat inputMatrix) 
{
	Mat kernel, processedMatrix;
	int sigma1 = 20;
	int sigma2 = 30;
	Mat fovaeMatrix(inputMatrix(Rect(cellInformation[9], cellInformation[10], cellInformation[7], cellInformation[7])));
	kernel = differenceOfGaussiansCreator(cellInformation[0], sigma1, sigma2);
	filter2D(fovaeMatrix, processedMatrix, fovaeMatrix.depth(), kernel);
	return fovaeMatrix;

}

Mat ReceptiveFieldFunctions::wholeInformationAcquierer(Mat inputMatrix) {
	AccessoryFunctions af;
	Mat kernel, temp, processedMatrix;
	temp = inputMatrix;
	Mat tempTest;
	tempTest = af.conversionToValue(temp);
	imwrite("E:/TestFolder/debugPic3.jpg", tempTest);
	int sigma1 = 2;
	int sigma2 = 4;
	kernel = differenceOfGaussiansCreator(cellInformation[0], sigma1, sigma2);
	filter2D(temp, processedMatrix, temp.depth(), kernel);
	Mat tempTest2;
	tempTest2 = af.conversionToValue(temp);
	imwrite("E:/TestFolder/debugPic4.jpg", tempTest2);
	return processedMatrix;
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

void RodBipolarProcessing::loadDifferenceToMemory(Memory& m)
{
	if (rodMemoryDifference.size() != m.memoryDifference.size()) {
		m.memoryDifference.resize(rodMemoryDifference.size());
	}
	for (int it = 0; it < rodMemoryDifference.size(); it++) {
		m.memoryDifference[it] = rodMemoryDifference[it];
	}
}

void RodBipolarProcessing::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		rodMemory[it] = m.memory[it];
	}
}

void RodBipolarProcessing::loadDifferenceFromMemory(Memory& m)
{
	for (int it = 0; it < m.memoryDifference.size(); it++) {
		rodMemoryDifference[it] = m.memoryDifference[it];
	}
}

Mat RodBipolarProcessing::RodBiploarProcessing(Mat inputMatrix, int mainIterator) {
	Memory m;
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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
	processedMatrix = processedMatrix / 10.0;
	loadFromMemory(m);
	// converting AII amacrine output to ratio for all-cone discrimination
	return processedMatrix;
}

//MIDGET Pathway
vector<int> MidgetSConePathway::initializeCellInformation(Mat inputMatrix) {
	setKernelSize(6); // changes to 6 from 4
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
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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
	loadFromMemory(m);
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
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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
	loadFromMemory(m);
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
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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
	loadFromMemory(m);
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
	ImageProcessingDataAcquisition ipda;
	//declaring local variables
	vector<Mat> processedMatrices;
	Size matrixSize = af.sizeOfMatrix(firstInputMatrix);
	Mat averagedOnProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);;
	Mat averagedOffProcessedMatrix((matrixSize.height / cellInformation[0]) * cellInformation[0] - cellInformation[0],
		(matrixSize.width / cellInformation[0]) * cellInformation[0] - cellInformation[0], CV_32F);;
	bool isFirst;
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	Mat firstSynapticStrengthMatrix, secondSynapticStrengthMatrix, thirdSynapticStrengthMatrix;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	/*
	m.pushbackMemory(firstInputMatrix, currentMemoryPosition);
	m.pushbackMemory(secondInputMatrix, currentMemoryPosition);
	m.pushbackMemory(thirdInputMatrix, currentMemoryPosition);
	*/
	Mat allConeInput;
	allConeInput = (firstInputMatrix + secondInputMatrix + thirdInputMatrix) / 3.0;
	//modifying input with synapticStrength function
	/*Mat inputModifiedBySynapticStrengthFirst, inputModifiedBySynapticStrengthSecond, inputModifiedBySynapticStrengthThird;
	isFirst = ss.isFirstIteration(mainIterator);
	if (mainIterator > 5)
	{
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		}
		firstSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthFirst = firstInputMatrix.mul(firstSynapticStrengthMatrix);
		modifierMatrix.release();
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memorySecond, mainIterator);
		}
		secondSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthSecond = secondInputMatrix.mul(secondSynapticStrengthMatrix);
		modifierMatrix.release();
		if (!isFirst) {
			modifierMatrix = ss.modifierMatrixCalculator(m.memoryThird, mainIterator);
		}
		thirdSynapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		inputModifiedBySynapticStrengthThird = thirdInputMatrix.mul(thirdSynapticStrengthMatrix);
	}
	else
	{
		inputModifiedBySynapticStrengthFirst = firstInputMatrix;
		inputModifiedBySynapticStrengthSecond = secondInputMatrix;
		inputModifiedBySynapticStrengthThird = thirdInputMatrix;
	}*/
	
	//iterating through the matrix
	isFirst = ss.isFirstIteration(mainIterator);
	m.pushbackMemory(allConeInput, currentMemoryPosition, m.memory);
	Mat allConeModifiedInput;
	Mat synapticStrengthMatrix;
	if (mainIterator > 5)
	{
		modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		allConeModifiedInput = allConeInput.mul(synapticStrengthMatrix);

	}
	else
	{
		allConeModifiedInput = allConeInput;
	}

	// CONVOLVING WITH MEXICAN HAT FUNCTION
	Mat onProcessedMatrix, offProcessedMatrix, fovaeMatrix;
	onProcessedMatrix = wholeInformationAcquierer(allConeModifiedInput);
	fovaeMatrix = fovaeInformationAcquierer(allConeModifiedInput);
	//loading processed matrices
	processedMatrices.push_back(onProcessedMatrix);
	processedMatrices.push_back(fovaeMatrix);

	return processedMatrices;
}

/*void AllConeDiscrimination::initializeMemory(int iterator) {
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
}*/
void AllConeDiscrimination::initializeMemory(int iterator)
{
	Memory m;
	if (iterator < 5)
	{
		allConeMemory.resize(iterator);
	}
	else
	{
		allConeMemory.resize(m.getMemoryMax());
	}
}

/*void AllConeDiscrimination::loadToMemory(Memory& m) {
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
}*/
void AllConeDiscrimination::loadToMemory(Memory& m)
{
	if (allConeMemory.size() != m.memory.size())
	{
		m.memory.resize(allConeMemory.size());
	}
	for (int it = 0; it < allConeMemory.size(); it++) {
		m.memory[it] = allConeMemory[it];
	}
}

/*void AllConeDiscrimination::loadFromMemory(Memory& m) {
	for (int it = 0; it < m.memory.size(); it++) {
		lConeMemory[it] = m.memory[it];
		sConeMemory[it] = m.memorySecond[it];
		mConeMemory[it] = m.memoryThird[it];
	}
}*/
void AllConeDiscrimination::loadFromMemory(Memory& m)
{
	for (int it = 0; it < m.memory.size(); it++)
	{
		allConeMemory[it] = m.memory[it];
	}
}

//vector<Mat> AllConeDiscrimination::
vector<Mat> AllConeDiscrimination::allConeDiscriminationMain(Mat firstInputMatrix, Mat secondInputMatrix, Mat thirdInputMatrix, int mainIterator) {
	Memory m;
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
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

//vector<Mat> cellMemory
Mat MainDirectionGanglionProcessing::previousInput(Memory& m, int currentMemoryPosition) {
	Mat previousInputMatrix;
	int previousPosOfMemory = currentMemoryPosition - 1;
	if (previousPosOfMemory == -1) {
		previousPosOfMemory = 4;
	}
	previousInputMatrix = m.memory[previousPosOfMemory];
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

int movementSensingNew(Memory& m, float firstSide, float secondSide, float kernelSize)
{
	int movementBool;
	float placeHolder = 1.0;
	float differenceOfAdjacent, differenceFromPrevious;
	differenceOfAdjacent = abs(secondSide - firstSide);
	differenceFromPrevious = abs(placeHolder - firstSide);
	// deciding the existance of edge movement
	if (differenceOfAdjacent < 0.05 * kernelSize && differenceFromPrevious > 0.1 * kernelSize) { //should play with the values here - might convert them to adjustable variables
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
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	Mat synapticStrengthMatrix;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	//inputMatrix = af.conversionToRatio(inputMatrix);
	m.pushbackMemory(inputMatrix, currentMemoryPosition, m.memory);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	Mat modifiedInputMatrix;
	if (mainIterator > 5)
	{
		modifierMatrix = ss.modifierMatrixCalculator(m.memory, mainIterator);
		synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
		modifiedInputMatrix = inputMatrix.mul(synapticStrengthMatrix);
	}
	else
	{
		modifiedInputMatrix = inputMatrix;
	}
	//loading previous input matirx
	previousInputMatrix = previousInput(m, currentMemoryPosition);
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; ij = ij + cellInformation[3]) {
			vector<Mat> upKernels, downKernels, rightKernels, leftKernels;
			int movementBool;
			float ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp, ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown,
				ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight, ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft;
			upKernels = displacementKernelCreator(modifiedInputMatrix, previousInputMatrix, cellInformation[0], ij, it, "up");
			//downKernels = displacementKernelCreator(modifiedInputMatrix, previousInputMatrix, cellInformation[0], ij, it, "down");
			//rightKernels = displacementKernelCreator(modifiedInputMatrix, previousInputMatrix, cellInformation[0], ij, it, "right");
			//leftKernels = displacementKernelCreator(modifiedInputMatrix, previousInputMatrix, cellInformation[0], ij, it, "left");
			// calculating result using the kernels
			// direction - up
			ratioOfPrevoiusUp = sum(upKernels[0])[0];
			ratioOfFirstSideUp = sum(upKernels[1])[0];
			ratioOfSecondSideUp = sum(upKernels[2])[0];
			int upwardMovement = movementSensing(ratioOfPrevoiusUp, ratioOfFirstSideUp, ratioOfSecondSideUp);
			// direction - down
			/*ratioOfPrevoiusDown = sum(downKernels[0])[0];
			ratioOfFirstSideDown = sum(downKernels[1])[0];
			ratioOfSecondSideDown = sum(downKernels[2])[0];
			int downwardMovement = movementSensing(ratioOfPrevoiusDown, ratioOfFirstSideDown, ratioOfSecondSideDown);
			// direction - right
			ratioOfPrevoiusRight = sum(rightKernels[0])[0];
			ratioOfFirstSideRight = sum(rightKernels[1])[0];
			ratioOfSecondSideRight = sum(rightKernels[2])[0];
			int rightMovement = movementSensing(ratioOfPrevoiusRight, ratioOfFirstSideRight, ratioOfSecondSideRight);
			// direction - left
			ratioOfPrevoiusLeft = sum(leftKernels[0])[0];
			ratioOfFirstSideLeft = sum(leftKernels[1])[0];
			ratioOfSecondSideLeft = sum(leftKernels[2])[0];
			int leftMovement = movementSensing(ratioOfPrevoiusLeft, ratioOfFirstSideLeft, ratioOfSecondSideLeft);*/

			//creating processed matrix vector
			upwardMovementMatrix.at<float>(iterY, iterX) = upwardMovement;
			//downwardMovementMatrix.at<float>(iterY, iterX) = downwardMovement;
			//rightMovementMatrix.at<float>(iterY, iterX) = rightMovement;
			//leftMovementMatrix.at<float>(iterY, iterX) = leftMovement;
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

/*vector<Mat> MainDirectionGanglionProcessing::directionReceptiveFieldProcessingNew(Mat inputMatrix, vector<int> cellInformation, Memory& m, int mainIterator)
{
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
	Mat modifierMatrix(matrixSize.height, matrixSize.width, CV_32F);
	Mat synapticStrengthMatrix;
	float ratioOfOnInputCenter, ratioOfOffInputCenter, ratioOfOnInputPeriphery, ratioOfOffInputPeriphery;
	float threshold = 0.3;
	//adding the current input to memory
	int currentMemoryPosition = m.memoryPosition(mainIterator);
	//inputMatrix = af.conversionToRatio(inputMatrix);
	m.pushbackMemory(inputMatrix, currentMemoryPosition);
	//modifying input with synapticStrength function
	isFirst = ss.isFirstIteration(mainIterator);
	if (mainIterator > 1)
	{
		modifierMatrix = ss.modifierMatrixCalculator(inputMatrix, currentMemoryPosition);
	}
	synapticStrengthMatrix = ss.synapticStrengthMatrixCreator(modifierMatrix, matrixSize, isFirst);
	inputMatrix = inputMatrix.mul(synapticStrengthMatrix);
	//loading previous input matirx
	previousInputMatrix = previousInput(m, currentMemoryPosition);
	float kernelElementSize;
	kernelElementSize = pow(cellInformation[0], 2.0);
	Mat previousMatrix;
	previousMatrix = m.directionMemory[currentMemoryPosition];
	//iterating through the matrix
	int iterY = 0;
	for (int it = 0; it < (matrixSize.height / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; it = it + cellInformation[3]) {
		int iterX = 0;
		for (int ij = 0; ij < (matrixSize.width / (cellInformation[0] * 2)) * (cellInformation[0] * 2) - cellInformation[0] * 2; ij = ij + cellInformation[3]) {
			vector<Mat> upKernels, downKernels, rightKernels, leftKernels;
			int movementBool;
			float ratioOfFirstSideUp, ratioOfSecondSideUp, ratioOfFirstSideDown, ratioOfSecondSideDown,
				ratioOfFirstSideRight, ratioOfSecondSideRight, ratioOfFirstSideLeft, ratioOfSecondSideLeft;
			upKernels = displacementKernelCreatorNew(inputMatrix, cellInformation[0], ij, it, "up");
			downKernels = displacementKernelCreatorNew(inputMatrix, cellInformation[0], ij, it, "down");
			rightKernels = displacementKernelCreatorNew(inputMatrix, cellInformation[0], ij, it, "right");
			leftKernels = displacementKernelCreatorNew(inputMatrix, cellInformation[0], ij, it, "left");
			// calculating result using the kernels
			// direction - up
			ratioOfFirstSideUp = sum(upKernels[0])[0];
			ratioOfSecondSideUp = sum(upKernels[1])[0];
			int upwardMovement = movementSensingNew(previousMatrix, ratioOfFirstSideUp, ratioOfSecondSideUp, kernelElementSize);
			// direction - down
			ratioOfFirstSideDown = sum(downKernels[0])[0];
			ratioOfSecondSideDown = sum(downKernels[1])[0];
			int downwardMovement = movementSensingNew(m, ratioOfFirstSideDown, ratioOfSecondSideDown, kernelElementSize);
			// direction - right
			ratioOfFirstSideRight = sum(rightKernels[0])[0];
			ratioOfSecondSideRight = sum(rightKernels[1])[0];
			int rightMovement = movementSensingNew(m, ratioOfFirstSideRight, ratioOfSecondSideRight, kernelElementSize);
			// direction - left
			ratioOfFirstSideLeft = sum(leftKernels[0])[0];
			ratioOfSecondSideLeft = sum(leftKernels[1])[0];
			int leftMovement = movementSensingNew(m, ratioOfFirstSideLeft, ratioOfSecondSideLeft, kernelElementSize);
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
}*/

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
	ImageProcessingDataAcquisition ipda;
	AccessoryFunctions ac;
	if (mainIterator == 1) {
		initializeCellInformation(inputMatrix);
		initializeMemory(mainIterator);
		redGreenMemory[0] = inputMatrix;
		//loadToMemory(m);
		//m.memory[0] = inputMatrix;
		//loadFromMemory(m);
	}
	else if (mainIterator < 6) {
		initializeMemory(mainIterator);
		loadToMemory(m);
		mainDirectionInformation = directionReceptiveFieldProcessing(inputMatrix, cellInformation, m, mainIterator);
		loadFromMemory(m);
	}else {
		loadToMemory(m);
		mainDirectionInformation = directionReceptiveFieldProcessing(inputMatrix, cellInformation, m, mainIterator);
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