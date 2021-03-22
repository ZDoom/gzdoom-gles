

#include "gles_system.h"

#if USE_GLES2

PFNGLMAPBUFFERRANGEEXTPROC glMapBufferRange = NULL;
PFNGLUNMAPBUFFEROESPROC glUnmapBuffer = NULL;
#ifdef __ANDROID__
#include <dlfcn.h>

static void* LoadGLES2Proc(const char* name)
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

static void* LoadGLES2Proc(const char* name)
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

#endif // USE_GLES2

namespace OpenGLESRenderer
{
	RenderContextGLES gles;

	void InitGLES()
	{

#if USE_GLES2

		if (!gladLoadGLES2Loader(&LoadGLES2Proc))
		{
			exit(-1);
		}

		glMapBufferRange = (PFNGLMAPBUFFERRANGEEXTPROC)LoadGLES2Proc("glMapBufferRange");
		glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)LoadGLES2Proc("glUnmapBuffer");

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
		gles.useMappedBuffers = true;
		gles.depthStencilAvailable = true;
		gles.maxuniforms = 1024 * 16;
		gles.max_texturesize = 1024 * 4;
		gles.modelstring = (char*)glGetString(GL_RENDERER);
		gles.vendorstring = (char*)glGetString(GL_VENDOR);
		gles.maxlights = 32; // TODO, calcualte this
		gles.numlightvectors = (gles.maxlights * LIGHT_VEC4_NUM);
	}

}
