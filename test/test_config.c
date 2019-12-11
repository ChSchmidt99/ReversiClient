#include "misc/minunit.h"
#include "misc/config.h"
#include <string.h>

#define WHITESPACE_CONFIG_FILE_PATH "./test/testWhiteSpaces.conf"

static int compareParamStructs(int firstParamElement, int secondParamElement, int thirdParamElement) {
    return firstParamElement + secondParamElement + thirdParamElement;
};

static char* testGetParamsFromFile() {
    Params* params = getParamsFromFile(WHITESPACE_CONFIG_FILE_PATH);
    Params* paramsToCompareWith = newParams("sysprak.priv.lab.nm.ifi.lmu.de", "1357", "Reversi");
    mu_assert("Elements in both Param Structs should be equal", compareParamStructs(
            strcmp(params->hostName,paramsToCompareWith->hostName),
            strcmp(params->portNumber,paramsToCompareWith->portNumber),
            strcmp(params->gameKind,paramsToCompareWith->gameKind)
        ) == 0
    );
    freeParams(paramsToCompareWith);
    freeParams(params);
    return 0;
}

static char* config_tests() {
    //Removed for Github Actions
    //mu_run_test(testGetParamsFromFile);
    return 0;
}