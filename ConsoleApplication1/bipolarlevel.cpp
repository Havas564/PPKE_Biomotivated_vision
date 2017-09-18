#include "bipolarlevel.h"


BipolarLevel::BipolarLevel()
{
}

BipolarLevel::~BipolarLevel()
{
}


// center-periphery comparison
float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery) {
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