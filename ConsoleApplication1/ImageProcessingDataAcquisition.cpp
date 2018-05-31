#include "ImageProcessingDataAcquisition.h"
#include "accessoryFunctions.h"

ImageProcessingDataAcquisition::ImageProcessingDataAcquisition()
{
}

ImageProcessingDataAcquisition::~ImageProcessingDataAcquisition()
{
}

void ImageProcessingDataAcquisition::imageShow(Mat inputMatrix) {
	namedWindow("Current image", WINDOW_AUTOSIZE);
	imshow("Current image", inputMatrix);
	waitKey(5000);
}

void ImageProcessingDataAcquisition::setSaveLocation() {
	string toSave;
	do
	{
		cout << "Do you want to save images? (yes/no) "; cin >> toSave;
	} while (cin.fail()); // check if it works this way
	
	if (toSave == "yes")
	{
		string saveLoc;
		saving = true;
		cout << "Please give full pathway, for saveing location: "; cin >> saveLoc;
		saveLocation = saveLoc;
	}
	else 
	{
		saving = false;
	}
	cin.clear();
}

vector<double> ImageProcessingDataAcquisition::saveTimeInformation(double timePassed, vector<double> timeElapsedVector) {
	timePassed = ((double)getTickCount() - timePassed) / getTickFrequency();
	timeElapsedVector.push_back(timePassed);
	return timeElapsedVector;
}

void ImageProcessingDataAcquisition::saveImage(string fileName, Mat data) {
	ostringstream savingData;
	savingData << saveLocation << fileName << fileExtension;
	string saveInformation;
	saveInformation = savingData.str();
	imwrite(saveInformation, data);
}


void ImageProcessingDataAcquisition::proceedWithSavingIfTrue(Mat fileToSave, string folderName, string fileName, int mainIterator) {
	if (saving)
	{
		AccessoryFunctions af;
		Mat temp;
		temp = af.conversionToValue(fileToSave);
		ostringstream savingInformation;
		stringstream progressMessageStream;
		savingInformation << folderName << fileName << to_string(mainIterator);
		progressMessageStream << "saving " << fileName << fileExtension;
		string fileName;
		string progressMessage;
		fileName = savingInformation.str();
		progressMessage = progressMessageStream.str();
		Mat latestMemorySequence;
		saveImage(fileName, temp);
		cout << progressMessage << endl;
	}
}