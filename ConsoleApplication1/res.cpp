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