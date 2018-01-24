#ifndef BaseShape_h
#define BaseShape_h

#include "kvector.h"
#include "LGdi.h"
#include "KRef.h"

#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

using namespace util;

namespace yanbo
{

class BaseShape : public KRef
{
public:
	enum ShapeType
	{
		EShapeLine,
		EShapeRect,
		EShapeString,
		EShapeImage,
		EShapeVideo,
	};

public:
	BaseShape();
	virtual ~BaseShape();
public:
	void setColor(const LRgb& color);

	void setRect(const LRect& rect);
	void setLine(const LPoint& p1, const LPoint& p2);
	void setImage(void* pixel, const LRect& rect, const LSize& texSize);

	void initShader();
	void drawSelf();

private:
	KVector<float>* m_dataArray;
	KVector<float>* m_colorArray;

	GLuint m_program;
	GLuint m_aPositionHandle;
	GLuint m_aColorHandle;
	GLuint m_muMVPMatrixHandle;
	GLuint m_uIsImageHandle;
	GLuint m_aTextureCoord;

    float m_yAngle;//绕y轴旋转的角度
    float m_xAngle;//绕z轴旋转的角度

    int m_vCount;

    int m_shapeType;

    GLuint m_texId;
};
}
#endif
