#include "Helper.h"
#include <GL/glew.h>
#include <iostream>

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
 
void glPrintError()
{
	// Get the last error
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
	{
		std::string error = "unknown error";
		std::string description  = "no description";

		// Decode the error code
		switch (errorCode)
		{
			case GL_INVALID_ENUM :
			{
				error = "GL_INVALID_ENUM";
				description = "an unacceptable value has been specified for an enumerated argument";
				break;
			}

			case GL_INVALID_VALUE :
			{
				error = "GL_INVALID_VALUE";
				description = "a numeric argument is out of range";
				break;
			}

			case GL_INVALID_OPERATION :
			{
				error = "GL_INVALID_OPERATION";
				description = "the specified operation is not allowed in the current state";
				break;
			}

			case GL_STACK_OVERFLOW :
			{
				error = "GL_STACK_OVERFLOW";
				description = "this command would cause a stack overflow";
				break;
			}

			case GL_STACK_UNDERFLOW :
			{
				error = "GL_STACK_UNDERFLOW";
				description = "this command would cause a stack underflow";
				break;
			}

			case GL_OUT_OF_MEMORY :
			{
				error = "GL_OUT_OF_MEMORY";
				description = "there is not enough memory left to execute the command";
				break;
			}

			case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :
			{
				error = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
				description = "the object bound to FRAMEBUFFER_BINDING_EXT is not \"framebuffer complete\"";
				break;
			}
		}

		std::cerr << "OpenGL Error: "  << error << ", " << description << std::endl;
	}
}
