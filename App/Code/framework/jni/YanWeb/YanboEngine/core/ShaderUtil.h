#ifndef ShaderUtil_h
#define ShaderUtil_h

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace yanbo
{
class ShaderUtil
{
public:
	static GLuint loadShader(GLenum shaderType
		, const char* pSource);

    static GLuint createProgram(const char* pVertexSource
		, const char* pFragmentSource);

    static void screenToGlPoint(int androidX, int androidY
    		, float* glX, float* glY);

    static void setScreenSize(int width, int height);

private:
    static int s_width;
    static int s_height;
};

}

#endif
