#pragma once

#include <Arduino.h>

namespace shstrailer {

constexpr uint16_t kMajorVersion = 1;
constexpr uint16_t kMinorVersion = 0;
constexpr uint16_t kPatchVersion = 0;

// Returns the version string in the format "v<major>.<minor>.<patch>".
// no need to free the returned string, it is statically allocated.
[[nodiscard]] const char* GetVersionString();

}  // namespace shstrailer
