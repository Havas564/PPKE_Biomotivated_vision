#pragma once
#ifndef BIPOLARLEVEL_H
#define BIPOLARLEVEL_H

#include <iostream>
#include <stdlib.h>
#include <opencv2\core.hpp>
#include "accessoryFunctions.h"

using namespace std;
using namespace cv;

class BipolarLevel
{
public:
	BipolarLevel();
	~BipolarLevel();
	// basic values

	
	//center - periphery comparison
	float centerPeripheryComparison(float ratioOfCenter, float ratioOfPeriphery);

private:

};



#endif // !BIPOLARLEVEL_H
