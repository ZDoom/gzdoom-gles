

#include "gles_system.h"


namespace OpenGLESRenderer
{

	RenderContextGLES gles;


	void InitGLES()
	{
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