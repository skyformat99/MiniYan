#include "Shape.h"


namespace yanbo
{
#define UNIT_SIZE 1


Shape::Shape(float r,float R,float z)
    : m_dataArray(108)
    , m_colorArray(144)
    , m_yAngle(0)
    , m_xAngle(0)
    , m_vCount(0)
{
	m_dataArray.clear();
	initVertexData(r, R, z);
	initShader();
}

void Shape::initVertexData(float R,float r,float z)
{
	float tempAngle=360/6;
	for(float angle = 0; angle < 360; angle += tempAngle)
	{
		//第一个三角形
		//第一个中心点
		m_dataArray.addElement(0.0f);
		m_dataArray.addElement(0.0f);
		m_dataArray.addElement(z);
		//第二个点
		m_dataArray.addElement((float) (R*UNIT_SIZE*cos(util::toRadians(angle))));
		m_dataArray.addElement((float) (R*UNIT_SIZE*sin(util::toRadians(angle))));
		m_dataArray.addElement(z);
		//第三个点
		m_dataArray.addElement((float) (r*UNIT_SIZE*cos(util::toRadians(angle+tempAngle/2))));
		m_dataArray.addElement((float) (r*UNIT_SIZE*sin(util::toRadians(angle+tempAngle/2))));
		m_dataArray.addElement(z);

		//第二个三角形
		//第一个中心点
		m_dataArray.addElement(0.0f);
		m_dataArray.addElement(0.0f);
		m_dataArray.addElement(z);
		//第二个点
		m_dataArray.addElement((float) (r*UNIT_SIZE*cos(util::toRadians(angle+tempAngle/2))));
		m_dataArray.addElement((float) (r*UNIT_SIZE*sin(util::toRadians(angle+tempAngle/2))));
		m_dataArray.addElement(z);
		//第三个点
		m_dataArray.addElement((float) (R*UNIT_SIZE*cos(util::toRadians(angle+tempAngle))));
		m_dataArray.addElement((float) (R*UNIT_SIZE*sin(util::toRadians(angle+tempAngle))));
		m_dataArray.addElement(z);
	}

	m_vCount = m_dataArray.size()/3;
	for(int i=0;i<m_vCount;i++)
	{
		if(i%3==0){//中心点为白色
			m_colorArray[i*4]=1;
			m_colorArray[i*4+1]=1;
			m_colorArray[i*4+2]=1;
			m_colorArray[i*4+3]=0;
		}
		else{//边上的点为淡蓝色
			m_colorArray[i*4]=0.45f;
			m_colorArray[i*4+1]=0.75f;
			m_colorArray[i*4+2]=0.75f;
			m_colorArray[i*4+3]=0;
		}
	}
}

void Shape::initShader()
{
    m_program = ShaderUtil::createProgram(Vertex::s_plane, Fragment::s_plane);
    //获取程序中顶点位置属性引用id
    m_aPositionHandle = glGetAttribLocation(m_program, "aPosition");
    //获取程序中顶点颜色属性引用id
    m_aColorHandle = glGetAttribLocation(m_program, "aColor");
    //获取程序中总变换矩阵引用id
    m_muMVPMatrixHandle = glGetUniformLocation(m_program, "uMVPMatrix");
}

void Shape::drawSelf()
{
	 //制定使用某套shader程序
	 glUseProgram(m_program);
	 //初始化变换矩阵
     MatrixState::setInitStack();
     //设置沿Z轴正向位移1
     MatrixState::translate(0.0f,0.0f,1);
     //设置绕y轴旋转
     MatrixState::rotate(m_yAngle,0,1,0);
     //设置绕z轴旋转
     MatrixState::rotate(m_xAngle,1,0,0);
     //将最终变换矩阵传入shader程序
     glUniformMatrix4fv(m_muMVPMatrixHandle, 1, GL_FALSE, MatrixState::getFinalMatrix()->getBuffer());
     //为画笔指定顶点位置数据
     glVertexAttribPointer
     (
    		m_aPositionHandle,
     		3,
     		GL_FLOAT,
     		GL_FALSE,
            3*sizeof(float),
            m_dataArray.getBuffer()
     );
     //为画笔指定顶点着色数据
     glVertexAttribPointer
     (
    		m_aColorHandle,
     		4,
     		GL_FLOAT,
     		GL_FALSE,
            4*sizeof(float),
            m_colorArray.getBuffer()
     );
     //允许顶点位置数据数组
     glEnableVertexAttribArray(m_aPositionHandle);
     glEnableVertexAttribArray(m_aColorHandle);
     //绘制六角星
     glDrawArrays(GL_TRIANGLES, 0, m_vCount);
}

}

