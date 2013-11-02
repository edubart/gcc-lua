#include "gcc-plugin.h"

#if GCC_VERSION >= 4008 || defined(ENABLE_BUILD_WITH_CXX)
#define GCCLUA_USE_CXX
#endif
