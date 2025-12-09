# Execute all test files
path="/home/ryan/ML-Library/build/"

echo "---------- Test Activation Functions ----------"
${path}testActivation

echo "---------- Test Data Manipulation ----------"
${path}testDataManip

echo "---------- Test Dot Product Function ----------"
${path}testDot

echo "---------- Test Identity Function ----------"
${path}testIden

echo "---------- Test Matrix Vector Multiplication ----------"
${path}testMatVect

echo "---------- Test Matrix Math Functions ----------"
${path}testMatOps

echo "---------- Test Random Permutation Function ----------"
${path}testRandPerm

echo "---------- Test Transpose Functions ----------"
${path}testTrans

echo "---------- Test Vector Math Functions ----------"
${path}testVectOps
