#include "gcc-plugin.h"
#include "plugin-version.h"

#if GCCPLUGIN_VERSION >= 4008 || defined(ENABLE_BUILD_WITH_CXX)
#define GCCLUA_USE_CXX
#endif
