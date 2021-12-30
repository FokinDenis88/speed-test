#include "TesterSpeed.h"
#include "TestSpeed.h"


// Tests
#include "OmpVsSTL.h"
#include "MatrixParallel.h"
#include "MatrixMult.h"
#include "SummationFotNeuralNetworks.h"
#include "StructVsLocal.h"
#include "MLP.h"
#include "MeanSquare.h"
#include "VecOps.h"
#include "Switch.h"
#include "AccumulateVsSum.h"
#include "VariablesOutsideForLoop.h"


constexpr size_t test_count_p = 30000000;

void OldTests() {
    OmpVsSTL::Main();
    MatrixParallel::Main();
}

int main() {
    //OldTests();
    //MatrixMult::Main();
    //SummationFotNeuralNetworks::Main();
    //StructVsLocal::Main();
    //mean_square::Main();
    //MLP::Main();
    //OmpVsSTL::Main();
    //Switch::Main();
    //VecOps::Main();
    //AccumulateVsSum::Main();
    VariablesOutsideForLoop::Main();
}