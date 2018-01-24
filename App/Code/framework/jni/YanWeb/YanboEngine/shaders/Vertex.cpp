#include "Vertex.h"

namespace yanbo
{

const char* Vertex::s_plane =
		"uniform mat4 uMVPMatrix;\n"
	    "attribute vec3 aPosition;\n"
		"attribute vec4 aColor;\n"
		"attribute vec2 aTexCoord;\n"
		"varying vec4 vColor;\n"
		"varying vec2 vTexCoord;\n"
	    "void main() {\n"
	    "  gl_Position = uMVPMatrix * vec4(aPosition, 1);\n"
		"  vColor = aColor;\n"
		"  vTexCoord = aTexCoord;\n"
	    "}\n";

const char* Vertex::s_tank = NULL;

const char* Vertex::s_soldier = NULL;

const char* Vertex::s_hero = NULL;

const char* Vertex::s_video =
        "uniform mat4 uMVPMatrix;\n"
        "uniform mat4 uSTMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec4 aTextureCoord;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "  gl_Position = uMVPMatrix * aPosition;\n"
        "  vTextureCoord = (uSTMatrix * aTextureCoord).xy;\n"
        "}\n";
}
