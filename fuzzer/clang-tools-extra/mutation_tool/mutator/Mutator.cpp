#include "Mutator.h"

std::map<int, clang::tooling::FrontendActionFactory *> factoryMap = {
    {1, newFrontendActionFactory<MutatorFrontendAction_1>().release()},
    {2, newFrontendActionFactory<MutatorFrontendAction_2>().release()},
    {3, newFrontendActionFactory<MutatorFrontendAction_3>().release()},
    {4, newFrontendActionFactory<MutatorFrontendAction_4>().release()},
    {5, newFrontendActionFactory<MutatorFrontendAction_5>().release()},
    {6, newFrontendActionFactory<MutatorFrontendAction_6>().release()},
    {7, newFrontendActionFactory<MutatorFrontendAction_7>().release()},
    {8, newFrontendActionFactory<MutatorFrontendAction_8>().release()},
    {9, newFrontendActionFactory<MutatorFrontendAction_9>().release()},
    {10, newFrontendActionFactory<MutatorFrontendAction_10>().release()},
    {11, newFrontendActionFactory<MutatorFrontendAction_11>().release()},
    {12, newFrontendActionFactory<MutatorFrontendAction_12>().release()},
    {13, newFrontendActionFactory<MutatorFrontendAction_13>().release()},
    {14, newFrontendActionFactory<MutatorFrontendAction_14>().release()},
    {15, newFrontendActionFactory<MutatorFrontendAction_15>().release()},
    {16, newFrontendActionFactory<MutatorFrontendAction_16>().release()},
    {17, newFrontendActionFactory<MutatorFrontendAction_17>().release()},
    {18, newFrontendActionFactory<MutatorFrontendAction_18>().release()},
    {19, newFrontendActionFactory<MutatorFrontendAction_19>().release()},
    {20, newFrontendActionFactory<MutatorFrontendAction_20>().release()},
    {21, newFrontendActionFactory<MutatorFrontendAction_21>().release()},
    {22, newFrontendActionFactory<MutatorFrontendAction_22>().release()},
    {23, newFrontendActionFactory<MutatorFrontendAction_23>().release()},
    {24, newFrontendActionFactory<MutatorFrontendAction_24>().release()},
    {25, newFrontendActionFactory<MutatorFrontendAction_25>().release()},
    {26, newFrontendActionFactory<MutatorFrontendAction_26>().release()},
    {27, newFrontendActionFactory<MutatorFrontendAction_27>().release()},
    {28, newFrontendActionFactory<MutatorFrontendAction_28>().release()},
    {29, newFrontendActionFactory<MutatorFrontendAction_29>().release()},
    {30, newFrontendActionFactory<MutatorFrontendAction_30>().release()},
    {31, newFrontendActionFactory<MutatorFrontendAction_31>().release()},
    {32, newFrontendActionFactory<MutatorFrontendAction_32>().release()},
    {33, newFrontendActionFactory<MutatorFrontendAction_33>().release()},
    {34, newFrontendActionFactory<MutatorFrontendAction_34>().release()},
    {35, newFrontendActionFactory<MutatorFrontendAction_35>().release()},
    {36, newFrontendActionFactory<MutatorFrontendAction_36>().release()},
    {37, newFrontendActionFactory<MutatorFrontendAction_37>().release()},
    {38, newFrontendActionFactory<MutatorFrontendAction_38>().release()},
    {39, newFrontendActionFactory<MutatorFrontendAction_39>().release()},
    {40, newFrontendActionFactory<MutatorFrontendAction_40>().release()},
    {41, newFrontendActionFactory<MutatorFrontendAction_41>().release()},
    {42, newFrontendActionFactory<MutatorFrontendAction_42>().release()},
    {43, newFrontendActionFactory<MutatorFrontendAction_43>().release()},
    {44, newFrontendActionFactory<MutatorFrontendAction_44>().release()},
    {45, newFrontendActionFactory<MutatorFrontendAction_45>().release()},
    {46, newFrontendActionFactory<MutatorFrontendAction_46>().release()},
    {47, newFrontendActionFactory<MutatorFrontendAction_47>().release()},
    {48, newFrontendActionFactory<MutatorFrontendAction_48>().release()},
    {49, newFrontendActionFactory<MutatorFrontendAction_49>().release()},
    {50, newFrontendActionFactory<MutatorFrontendAction_50>().release()},
    {51, newFrontendActionFactory<MutatorFrontendAction_51>().release()},
    {52, newFrontendActionFactory<MutatorFrontendAction_52>().release()},
    {53, newFrontendActionFactory<MutatorFrontendAction_53>().release()},
    {54, newFrontendActionFactory<MutatorFrontendAction_54>().release()},


    // preprocess
    {1001, newFrontendActionFactory<MutatorFrontendAction_1001>().release()},
    {1002, newFrontendActionFactory<MutatorFrontendAction_1002>().release()},
    {1003, newFrontendActionFactory<MutatorFrontendAction_1003>().release()},
    {1005, newFrontendActionFactory<MutatorFrontendAction_1005>().release()},
};

clang::tooling::FrontendActionFactory *getMutatorNewFrontendActionFactory(int mutator) {
    auto it = factoryMap.find(mutator);
    if (it != factoryMap.end()) {
        return it->second;
    }
    assert(false && "No mutator matched\n");
    return nullptr; 
}
