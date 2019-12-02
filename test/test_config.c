#include "minunit.h"
#include "config.h"
#include <string.h>

static int compareParamStructs(int firstParamElement, int secondParamElement, int thirdParamElement) {
    return firstParamElement + secondParamElement + thirdParamElement;
};

static char * testGetParamsFromFile() {
    char* testConfigFilePath = "./test/testWhiteSpaces.conf";
    Params* params = getParamsFromFile(testConfigFilePath);
    Params* paramsToCompareWith = newParams("sysprak.priv.lab.nm.ifi.lmu.de", "1357", "Reversi");
    mu_assert("Elements in both Param Structs should be equal", compareParamStructs(
            strcmp(params->hostName,paramsToCompareWith->hostName),
            strcmp(params->portNumber,paramsToCompareWith->portNumber),
            strcmp(params->gameKind,paramsToCompareWith->gameKind)
        ) ==0
    );
    return 0;
}

static char * config_tests() {
    mu_run_test(testGetParamsFromFile);
    return 0;
}