#include "version.h"

#include <stdio.h>

namespace shstrailer {

const char* GetVersionString() {
    static char version_string[19];

    snprintf(version_string, sizeof(version_string), "v%hu.%hu.%hu",
             kMajorVersion, kMinorVersion, kPatchVersion);

    return version_string;
}

}  // namespace shstrailer
