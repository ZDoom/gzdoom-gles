#ifndef __GLES_SYSTEM_H
#define __GLES_SYSTEM_H

#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#if !defined(__APPLE__) && !defined(__FreeBSD__) && !defined(__OpenBSD__) && !defined(__DragonFly__)
#include <malloc.h>
#endif
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define USE_GLES2 0

#if (USE_GLES2)
	#include "glad/glad.h"
#else
	#include "gl_load/gl_load.h"
	#define GL_DEPTH24_STENCIL8_OES GL_DEPTH24_STENCIL8
#endif

#if defined(__APPLE__)
	#include <OpenGL/OpenGL.h>
#endif

// This is the number of vec4s make up the light data
#define LIGHT_VEC4_NUM 4

//#define NO_RENDER_BUFFER

namespace OpenGLESRenderer
{
	struct RenderContextGLES
	{
		unsigned int flags;
		unsigned int maxuniforms;
		unsigned int maxuniformblock;
		unsigned int uniformblockalignment;
		unsigned int maxlights;
		unsigned int numlightvectors;
		bool useMappedBuffers;
		float glslversion;
		int max_texturesize;
		char* vendorstring;
		char* modelstring;
	};

	extern RenderContextGLES gles;

	void InitGLES();
}

#ifdef _MSC_VER
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA

#pragma warning(disable : 4018)     // signed/unsigned mismatch
#pragma warning(disable : 4305)     // truncate from double to float
#endif

#endif //__GL_PCH_H
