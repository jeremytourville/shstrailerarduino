#include "version.h"

#include <stdint.h>
#include <stdio.h>

namespace shstrailer {

const char* GetVersionString() {
    static char buffer[kVersionBufferSize];

    const int status = snprintf(buffer, kVersionBufferSize, "v%hu.%hu.%hu",
                                kMajorVersion, kMinorVersion, kPatchVersion);

    if (status < 0) {
        snprintf(buffer, kVersionBufferSize, "fail");
    }

    return buffer;
}

}  // namespace shstrailer
