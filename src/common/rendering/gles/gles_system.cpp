

#include "gles_system.h"


#ifdef __ANDROID__
#include <dlfcn.h>

static void* LinuxProcAddress(const char* name)
{
	static void *glesLib = NULL;

	if(!glesLib)
	{
		int flags = RTLD_LOCAL | RTLD_NOW;

		glesLib = dlopen("libGLESv2_CM.so", flags);
		if(!glesLib)
		{
			glesLib = dlopen("libGLESv2.so", flags);
		}
	}

	void * ret = NULL;
	ret =  dlsym(glesLib, name);

	if(!ret)
	{
		//LOGI("Failed to load: %s", name);
	}
	else
	{
		//LOGI("Loaded %s func OK", name);
	}

	return ret;
}

#elif defined _WIN32

#include <windows.h>

static HMODULE opengl32dll;
static PROC(WINAPI* getprocaddress)(LPCSTR name);

static void* WinGetProcAddressGLES(const char* name)
{

	HINSTANCE hGetProcIDDLL = LoadLibraryA("libGLESv2.dll");
	
	int error =	GetLastError();

	void* addr = GetProcAddress(hGetProcIDDLL, name);
	if (!addr)
	{
		//exit(1);
	}
	else
	{
		return addr;
	}
}

#endif

namespace OpenGLESRenderer
{
	RenderContextGLES gles;

	void InitGLES()
	{
#if USE_GLES2
#ifdef __ANDROID__
		if (!gladLoadGLES2Loader(&LinuxProcAddress))
		{
			exit(-1);
		}
#else
		if (!gladLoadGLES2Loader(&WinGetProcAddressGLES))
		{
			exit(-1);
		}
#endif
#else
		static bool first = true;

		if (first)
		{
			if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
			{
				//I_FatalError("Failed to load OpenGL functions.");
			}
		}
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
#endif
	
		gles.flags = 0;
		gles.useMappedBuffers = false;
		gles.glslversion = 3.31;
		gles.maxuniforms = 1024 * 16;
		gles.maxuniformblock = 1024 * 16;
		gles.uniformblockalignment = 256;
		gles.max_texturesize = 1024 * 4;
		gles.modelstring = (char*)"MODEL";
		gles.vendorstring = (char*)"EMILES";
		gles.maxlights = 32;
		gles.numlightvectors = (gles.maxlights * 4) + 1; // maxlights lights, + size vector at the beginning
	}

}
