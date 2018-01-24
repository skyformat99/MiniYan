#include "Fragment.h"

namespace yanbo
{

const char* Fragment::s_plane =
	    "precision mediump float;\n"
		"varying vec2 vTexCoord;\n"
		"varying vec4 vColor;\n"
		"uniform sampler2D uSampler2D;\n"
		"uniform int uIsImage;\n"
	    "void main() {\n"
		"    if (uIsImage == 1) {\n"
		"        gl_FragColor = texture2D(uSampler2D, vTexCoord); \n"
		"    } else {\n"
		"        gl_FragColor = vColor;\n"
		"    }\n"
	    "}\n";

const char* Fragment::s_tank = NULL;

const char* Fragment::s_soldier = NULL;

const char* Fragment::s_hero = NULL;

const char* Fragment::s_video =
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
