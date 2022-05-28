#include "tester-speed.hpp"
#include "test-speed.hpp"


// Tests
#include "OmpVsSTL.h"
#include "MatrixParallel.h"
#include "MatrixMult.h"
#include "summation-for-neural-networks.hpp"
#include "StructVsLocal.h"
#include "MLP.h"
#include "MeanSquare.h"
#include "vec-ops.hpp"
#include "switch.hpp"
#include "AccumulateVsSum.h"
#include "variables-outside-for-loop.hpp"


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