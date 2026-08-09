#include "version.h"

namespace shstrailer {

const char* GetVersionString() {
    static char version_string[16];

    snprintf(version_string, sizeof(version_string), "v%u.%u.%u", kMajorVersion,
             kMinorVersion, kPatchVersion);

    return version_string;
}

}  // namespace shstrailer
