

#include "gles_system.h"

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

static int TestPointer(const PROC pTest)
{
	ptrdiff_t iTest;
	if (!pTest) return 0;
	iTest = (ptrdiff_t)pTest;

	if (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;

	return 1;
}


extern "C" PROC zd_wglGetProcAddress(LPCSTR name);

static void* WinGetProcAddress(const char* name)
{
	HMODULE glMod = NULL;
	PROC pFunc = zd_wglGetProcAddress((LPCSTR)name);
	if (TestPointer(pFunc))
	{
		return pFunc;
	}
	glMod = GetModuleHandleA("OpenGL32.dll");
	return GetProcAddress(glMod, (LPCSTR)name);
}

namespace OpenGLESRenderer
{

	RenderContextGLES gles;


	void InitGLES()
	{
#if USE_GLES2
		if (!gladLoadGLES2Loader(&WinGetProcAddressGLES))
		{
			exit(-1);
		}
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
		gles.glslversion = 3.31;
		gles.maxuniforms = 1024 * 16;
		gles.maxuniformblock = 1024 * 16;
		gles.uniformblockalignment = 256;
		gles.max_texturesize = 1024 * 4;
		gles.modelstring = (char*)"MODEL";
		gles.vendorstring = (char*)"EMILES";
	}

}