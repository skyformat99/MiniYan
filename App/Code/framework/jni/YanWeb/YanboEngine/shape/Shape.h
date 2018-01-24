#ifndef Shape_h
#define Shape_h
#include "KVector.h"
#include "ShaderUtil.h"
#include "Vertex.h"
#include "Fragment.h"
#include "MatrixState.h"
#include <math.h>

namespace yanbo
{

class Shape
{
public:
	Shape(float r,float R,float z);
	~Shape();

	void initVertexData(float R,float r,float z);
	void initShader();

	void drawSelf();

    float m_yAngle;//绕y轴旋转的角度
    float m_xAngle;//绕z轴旋转的角度

private:
	KVector<float> m_dataArray;
	KVector<float> m_colorArray;
	GLuint m_program;
	GLuint m_aPositionHandle;
	GLuint m_aColorHandle;
	GLuint m_muMVPMatrixHandle;

    int m_vCount;
};

}

#endif
