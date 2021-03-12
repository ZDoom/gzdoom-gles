#pragma once

#include "win32basevideo.h"

//==========================================================================
//
// 
//
//==========================================================================

class Win32GLESVideo : public Win32BaseVideo
{
public:
	Win32GLESVideo();

	DFrameBuffer *CreateFrameBuffer() override;
	bool InitHardware(HWND Window, int multisample);
	void Shutdown();

protected:
	HGLRC m_hRC;
};
