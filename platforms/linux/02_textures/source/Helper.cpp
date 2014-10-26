#include "Helper.h"

std::string GetProcessPath() {
#if defined( PLATFORM_OSX )
	char exe_file[PATH_MAX + 1];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle) {
		CFURLRef mainURL = CFBundleCopyBundleURL(mainBundle);

		if (mainURL) {
			int ok = CFURLGetFileSystemRepresentation ( mainURL, (Boolean) true, (UInt8*)exe_file, PATH_MAX );

			if (ok) {
				return std::string(exe_file) + "/";
			}
		}
	}

	return "./";
#elif defined( PLATFORM_LINUX )
	char exe_file[PATH_MAX + 1];
	int size;
	size = readlink("/proc/self/exe", exe_file, PATH_MAX);
	if (size < 0) {
		return "./";
	} else {
		exe_file[size] = '\0';
		return std::string(dirname(exe_file)) + "/";
	}
#elif defined( PLATFORM_WIN32 )
	// Get path to executable:
	TCHAR szDllName[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFilename[_MAX_DIR];
	TCHAR szExt[_MAX_DIR];
	GetModuleFileName(0, szDllName, _MAX_PATH);

	_splitpath(szDllName, szDrive, szDir, szFilename, szExt);

	return std::string(szDrive) + std::string(szDir);
#elif defined( PLATFORM_BSD )
	int mib[4];
	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_PATHNAME;
	mib[3] = -1;
	char buf[1024];
	size_t cb = sizeof(buf);
	sysctl(mib, 4, buf, &cb, NULL, 0);

	return FileRemoveFileName( std::string( buf ) );
#elif defined( PLATFORM_SOLARIS )
	return FileRemoveFileName( std::string( getexecname() ) );
#elif defined( PLATFORM_HAIKU )
	image_info info;
	int32 cookie = 0;

	while ( B_OK == get_next_image_info( 0, &cookie, &info ) ) {
		if ( info.type == B_APP_IMAGE )
			break;
	}

	return FileRemoveFileName( std::string( info.name ) );
#else
	#warning GetProcessPath() not implemented on this platform. ( will return "./" )
	return "./";
#endif
}
 
