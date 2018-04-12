/*
contains code parts, which might be needed as comment

Previous version of synaptic modifier matrix creation:
if (mainIterator > 1) {
ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
}
if (mainIterator < 1) {
modifierMatrix = ss.modifierMatrixCalculator(m.memory, currentMemoryPosition);
}
else {
modifierMatrix = Mat::ones(1, 2, CV_32F);
}


*/

// all cone processing original for cycle start:
/*			/*vector<Mat> firstKernels, secondKernels, thirdKernels;
			Mat firstKernelCenter, secondKernelCenter, ThirdKernelCenter, firstKernelPeriphery, secondKernelPeriphery, thirdKernelPeriphery;
			firstKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			firstKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthFirst);
			secondKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthSecond);
			secondKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthSecond);
			ThirdKernelCenter = kernelCenterReveicer(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthThird);
			thirdKernelPeriphery = kernelPeripheryReceiver(cellInformation[1], cellInformation[0], ij, it, inputModifiedBySynapticStrengthThird);
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
			
*/