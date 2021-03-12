/*
** win32video.cpp
** Code to let ZDoom draw to the screen
**
**---------------------------------------------------------------------------
** Copyright 1998-2006 Randy Heit
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/

#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include "wglext.h"
#include <vector>

#include "gl_sysfb.h"
#include "hardware.h"
#include "templates.h"
#include "version.h"
#include "c_console.h"
#include "v_video.h"
#include "i_input.h"
#include "i_system.h"
#include "v_text.h"
#include "m_argv.h"
#include "printf.h"
#include "engineerrors.h"
#include "win32glesvideo.h"
#include "win32glvideo.h"

#include "gles_framebuffer.h"
//#include "Mali_OpenGL_ES_Emulator\include\EGL\egl.h"
//#include "glad/glad_egl.h"
#include "EGL/egl.h"

EXTERN_CVAR(Int, gl_pipeline_depth);


//#define USE_EGL

// Add to class!
EGLDisplay	sEGLDisplay;
EGLContext	sEGLContext;
EGLSurface	sEGLSurface;

//==========================================================================
//
// 
//
//==========================================================================

Win32GLESVideo::Win32GLESVideo()
{
}

//==========================================================================
//
// 
//
//==========================================================================

DFrameBuffer * Win32GLESVideo::CreateFrameBuffer()
{
	SystemGLFrameBuffer *fb;

	fb = new OpenGLESRenderer::OpenGLFrameBuffer(m_hMonitor, vid_fullscreen);

	fb->mPipelineNbr = gl_pipeline_depth;
	fb->mPipelineNbr = 4;
	return fb;
}




//==========================================================================
//
// 
//
//==========================================================================


void SystemGLFrameBuffer::SwapBuffers()
{
	::SwapBuffers(static_cast<Win32GLVideo*>(Video)->m_hDC);
	//eglSwapBuffers(sEGLDisplay, sEGLSurface));
}

bool Win32GLESVideo::InitHardware(HWND Window, int multisample)
{
#ifdef USE_EGL
	EGLint eglError;

	m_Window = Window;
	m_hDC = GetDC(Window);

	EGLint aEGLAttributes[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	EGLint aEGLContextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLConfig	aEGLConfigs[1];
	EGLint		cEGLConfigs;

	//HDC   hDisplay = EGL_DEFAULT_DISPLAY;

	sEGLDisplay = eglGetDisplay(m_hDC);
	
	eglError = eglGetError(); 
	if (eglError != EGL_SUCCESS) {
		exit(1); 
	} 

	eglInitialize(sEGLDisplay, NULL, NULL);
	eglError = eglGetError();
	if (eglError != EGL_SUCCESS) {
		exit(1);
	}

	eglChooseConfig(sEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs);
	eglError = eglGetError();
	if (eglError != EGL_SUCCESS) {
		exit(1);
	}

	if (cEGLConfigs == 0) {
		printf("No EGL configurations were returned.\n");
		exit(-1);
	}

	sEGLSurface = eglCreateWindowSurface(sEGLDisplay,	aEGLConfigs[0], Window, NULL);

	if (sEGLSurface == EGL_NO_SURFACE) {
		printf("Failed to create EGL surface.\n");
		exit(-1);
	}

	sEGLContext = eglCreateContext(sEGLDisplay,
		aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);

	if (sEGLContext == EGL_NO_CONTEXT) {
		printf("Failed to create EGL context.\n");
		exit(-1);
	}

	eglMakeCurrent(sEGLDisplay, sEGLSurface, sEGLSurface, sEGLContext);
	return true;

#endif	

	// We get here if the driver doesn't support the modern context creation API which always means an old driver.
	I_Error("R_OPENGL: Unable to create an OpenGL render context. Insufficient driver support for context creation\n");
	return false;
}

//==========================================================================
//
// 
//
//==========================================================================

void Win32GLESVideo::Shutdown()
{
	if (m_hDC) ReleaseDC(m_Window, m_hDC);
}





