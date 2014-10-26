#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <cmath>
#include <climits>
#include <limits>

#if defined( __APPLE_CC__ ) || defined ( __APPLE__ )
	#define PLATFORM_OSX
	#include <CoreFoundation/CoreFoundation.h>
#elif defined( __WIN32__ ) || defined( _WIN32 ) || defined( _WIN64 )
	#define PLATFORM_WIN32
	#include <windows.h>
#elif defined ( linux ) || defined( __linux__ )
	#define PLATFORM_LINUX
	#include <libgen.h>
	#include <unistd.h>
#elif defined( __HAIKU__ ) || defined( __BEOS__ )
	#define PLATFORM_HAIKU
	#include <kernel/OS.h>
	#include <kernel/image.h>
#elif defined( __SVR4 )
	#define PLATFORM_SOLARIS
	#include <stdlib.h>
#elif defined( __FreeBSD__ ) || defined(__OpenBSD__) || defined( __NetBSD__ ) || defined( __DragonFly__ )
	#define PLATFORM_BSD
	#include <sys/sysctl.h>
#endif

extern std::string GetProcessPath();

extern void glPrintError();

#endif
