#include "BaseShape.h"
#include "ShaderUtil.h"
#include "Vertex.h"
#include "Fragment.h"
#include "MatrixState.h"
#include "SalLog.h"

namespace yanbo
{

BaseShape::BaseShape()
    : m_vCount(0)
    , m_yAngle(0)
    , m_xAngle(0)
    , m_program(0)
    , m_texId(0)
{
	m_dataArray   = new KVector<float>();
	m_colorArray  = new KVector<float>();
	initShader();
}

BaseShape::~BaseShape()
{
	if (m_dataArray != NULL)
	{
	    delete m_dataArray;
	    m_dataArray = NULL;
	}

	if (m_colorArray != NULL)
	{
		delete m_colorArray;
		m_colorArray = NULL;
	}
}

void BaseShape::setLine(const LPoint& p1, const LPoint& p2)
{
	m_shapeType = EShapeLine;

    float x    = 0.0f;
    float y    = 0.0f;
    float z    = 0.0f;

    ShaderUtil::screenToGlPoint(p1.iX, p1.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);

    ShaderUtil::screenToGlPoint(p2.iX, p2.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);

    m_vCount = 2;
}

void BaseShape::setRect(const LRect& rect)
{
	m_shapeType = EShapeRect;

    float x    = 0.0f;
    float y    = 0.0f;
    float z    = 0.0f;

    ShaderUtil::screenToGlPoint(rect.iTopLeft.iX, rect.iTopLeft.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("gl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(rect.iBottomRight.iX, rect.iTopLeft.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("gl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(rect.iBottomRight.iX, rect.iBottomRight.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("gl x=%f. y=%f", x, y);

    // 第二个三角形开始
    //ShaderUtil::screenToGlPoint(rect.iBottomRight.iX, rect.iBottomRight.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("gl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(rect.iTopLeft.iX, rect.iBottomRight.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("gl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(rect.iTopLeft.iX, rect.iTopLeft.iY, &x, &y);
    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);

    m_vCount = 6;
}

void BaseShape::setImage(void* pixel, const LRect& rect, const LSize& texSize)
{
	m_shapeType = EShapeImage;

	glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA, // RGB 3, RGBA 4
			texSize.iWidth,
			texSize.iHeight,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixel);

	KFORMATLOG("texSize width=%d. height=%d", texSize.iWidth, texSize.iHeight);
	KFORMATLOG("LRect width=%d. height=%d", rect.GetWidth(), rect.GetHeight());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glDisable(GL_TEXTURE_2D);
    float x    = 0.0f;
    float y    = 0.0f;
    float z    = 0.0f;

    // 顶点坐标
    ShaderUtil::screenToGlPoint(
    		rect.iTopLeft.iX,
    		rect.iBottomRight.iY, &x, &y);

    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("texgl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(
    		rect.iBottomRight.iX,
    		rect.iBottomRight.iY, &x, &y);

    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("texgl x=%f. y=%f", x, y);


    ShaderUtil::screenToGlPoint(
    		rect.iTopLeft.iX,
    		rect.iTopLeft.iY, &x, &y);

    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("texgl x=%f. y=%f", x, y);

    ShaderUtil::screenToGlPoint(
    		rect.iBottomRight.iX,
    		rect.iTopLeft.iY, &x, &y);

    m_dataArray->addElement(x);
    m_dataArray->addElement(y);
    m_dataArray->addElement(z);
    KFORMATLOG("texgl x=%f. y=%f", x, y);

    // 纹理坐标
    m_colorArray->addElement(0.0f);
    m_colorArray->addElement(1.0f);

    m_colorArray->addElement(1.0f);
    m_colorArray->addElement(1.0f);

    m_colorArray->addElement(0.0f);
    m_colorArray->addElement(0.0f);

    m_colorArray->addElement(1.0f);
    m_colorArray->addElement(0.0f);

    m_vCount = 4;
}

void BaseShape::setColor(const LRgb& color)
{
	for (int i = 0; i < m_vCount; i++)
	{
	    m_colorArray->addElement((1.0f*color.m_red)/255);
	    m_colorArray->addElement((1.0f*color.m_green)/255);
	    m_colorArray->addElement((1.0f*color.m_blue)/255);
	    m_colorArray->addElement((1.0f*color.m_alpha)/255);
	}
}

void BaseShape::initShader()
{
    m_program = ShaderUtil::createProgram(Vertex::s_plane, Fragment::s_plane);
    //获取程序中顶点位置属性引用id
    m_aPositionHandle = glGetAttribLocation(m_program, "aPosition");
    //获取程序中顶点颜色属性引用id
    m_aColorHandle = glGetAttribLocation(m_program, "aColor");
    //获取程序中总变换矩阵引用id
    m_muMVPMatrixHandle = glGetUniformLocation(m_program, "uMVPMatrix");

    m_uIsImageHandle = glGetUniformLocation(m_program, "uIsImage");

    m_aTextureCoord = glGetAttribLocation(m_program, "aTexCoord");
}

void BaseShape::drawSelf()
{
	KLOG("BaseShape::drawSelf()");
    if (m_program != 0)
    {
		 // 制定使用某套shader程序
		 glUseProgram(m_program);
		 KLOG("BaseShape::drawSelf()1");
		 // 初始化变换矩阵
		 MatrixState::setInitStack();
		 KLOG("BaseShape::drawSelf()2");
		 // 设置沿Z轴正向位移1
		 MatrixState::translate(0.0f, 0.0f, 0.0f);
		 KLOG("BaseShape::drawSelf()3");
		 // 设置绕y轴旋转
		 MatrixState::rotate(m_yAngle, 0, 1, 0);
		 KLOG("BaseShape::drawSelf()4");
		 // 设置绕z轴旋转
		 MatrixState::rotate(m_xAngle, 1, 0, 0);
		 KLOG("BaseShape::drawSelf()5");
		 // 将最终变换矩阵传入shader程序
		 glUniformMatrix4fv(m_muMVPMatrixHandle, 1, GL_FALSE, MatrixState::getFinalMatrix()->getBuffer());
		 // 传入是否为纹理
		 KLOG("BaseShape::drawSelf()6");
		 glUniform1i(m_uIsImageHandle, (m_shapeType != EShapeImage) ? 0 : 1);
		 // 为画笔指定顶点位置数据
		 KLOG("BaseShape::drawSelf()7");
		 glVertexAttribPointer
		 (
				m_aPositionHandle,
				3,
				GL_FLOAT,
				GL_FALSE,
				3*sizeof(float),
				m_dataArray->getBuffer()
		 );

		 // 为画笔指定顶点着色数据
		 int colorPixelSize = (m_shapeType != EShapeImage) ? 4 : 2;
		 GLuint colorHandle = (m_shapeType != EShapeImage) ?
				 m_aColorHandle : m_aTextureCoord;

		 glVertexAttribPointer
		 (
				colorHandle,
				colorPixelSize,
				GL_FLOAT,
				GL_FALSE,
				colorPixelSize*sizeof(float),
				m_colorArray->getBuffer()
		 );
		 // 允许顶点位置数据数组
		 glEnableVertexAttribArray(m_aPositionHandle);
		 glEnableVertexAttribArray(colorHandle);

		 switch (m_shapeType)
		 {
		 case EShapeLine:
		     {
		    	 glDrawArrays(GL_LINES, 0, m_vCount);
		     }
		     break;
		 case EShapeRect:
		     {
		    	 glDrawArrays(GL_TRIANGLES, 0, m_vCount);
		     }
		     break;
		 case EShapeImage:
		     {
		    	 glUniform1i(glGetUniformLocation(m_program, "uSampler2D"), 0);
		         //绑定纹理
		         glActiveTexture(GL_TEXTURE0);
		         glBindTexture(GL_TEXTURE_2D, m_texId);
		         //绘制纹理矩形
		         glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vCount);
		     }
		     break;
		 }

    }
}

}
