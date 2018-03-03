#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>

// 批次容器，GLTools的一个容器类
GLBatch triangleBatch;

// 着色管理器
GLShaderManager shaderManager;

// 三角形顶点数据
GLfloat halfWidth = 0.1f;
GLfloat vVerts[] = {
    0.0f, 2*halfWidth, 0.0f,
    -halfWidth, 0.0f, 0.0f,
    halfWidth, 0.0f, 0.0f
};


GLfloat xPos = 0.0f, yPos = 0.0f;

#pragma mark - 窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素

void ChangeSize(int w,int h)

{
    
    glViewport(0,0, w, h);
    
}

#pragma mark - 为程序作一次性的设置

void SetupRC()

{
    
    //设置背影颜色
    
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    
    //初始化着色管理器
    
    shaderManager.InitializeStockShaders();
    
    //设置三角形，其中数组vVert包含所有3个顶点的x,y,笛卡尔坐标对。
    
//    GLfloat vVerts[] = {
//
//        -0.5f,0.0f,0.0f,
//
//        0.5f,0.0f,0.0f,
//
//        0.0f,0.5f,0.0f,
//
//    };
    
    //批次处理
    
    triangleBatch.Begin(GL_TRIANGLES,3);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
    triangleBatch.End();
    
}

#pragma mark - 开始渲染

void RenderScene(void)

{
    
    //清除一个或一组特定的缓冲区
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示黑色
    
    GLfloat vBlack[] = {0.0f,0.0f,0.0f,1.0f};
    
    //传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    
//    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vBlack); // 通过坐标方式绘制图形
    
    // 利用矩阵绘制图形，需要用到平面着色器
    M3DMatrix44f mFinalTransform, mTransFormMatrix, mRotationMatrix;
    
    m3dTranslationMatrix44(mTransFormMatrix, xPos, yPos, 0.0f);
    
//    shaderManager.UseStockShader(GLT_SHADER_FLAT,mTransFormMatrix,vBlack);
    //增加难度! 一边移动，一边旋转
    static float yRot = 0.0f;

    /*
     参数1：矩阵
     参数2：弧度
     参数3：X:1，0 1：围绕X轴旋转，0不围绕X轴旋转
     参数4：Y:1，0
     参数5：Z:1，0
     */
    m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(yRot), 0.0f, 0.0f, 1.0f);

    //修改旋转度数
    yRot += 5.0f;



    m3dMatrixMultiply44(mFinalTransform, mTransFormMatrix, mRotationMatrix);

    shaderManager.UseStockShader(GLT_SHADER_FLAT, mFinalTransform, vBlack);
    
    //提交着色器
    
    triangleBatch.Draw();
    
    //将在后台缓冲区进行渲染，然后在结束时交换到前台
    
    glutSwapBuffers();
    
}

#pragma mark - 键盘输入操作(移动图形---通过修改图形坐标)
//void SpeacialKeysRect(int key, int x, int y) {
//    GLfloat stepSize = 0.025f;
//
//    GLfloat originX = vVerts[3];
//    GLfloat originY = vVerts[1];
//    
//    if (key == GLUT_KEY_UP) {
//        originY += stepSize;
//    } else if (key == GLUT_KEY_DOWN) {
//        originY -= stepSize;
//    } else if (key == GLUT_KEY_LEFT) {
//        originX -= stepSize;
//    } else if (key == GLUT_KEY_RIGHT) {
//        originX += stepSize;
//    }
//
//    // 加入边界判断
//    if (originX < -1.0f) {
//        originX = -1.0f;
//    }
//    if (originX > 1.0f - halfWidth*2) {
//        originX = 1.0f - halfWidth*2;
//    }
//    if (originY > 1.0f) {
//        originY = 1.0f;
//    }
//    if (originY < -1.0f + halfWidth*2) {
//        originY = -1.0f + halfWidth*2;
//    }
//
//    vVerts[0] = originX + halfWidth;
//    vVerts[1] = originY;
//    vVerts[3] = originX;
//    vVerts[4] = originY - 2*halfWidth;
//    vVerts[6] = originX + 2*halfWidth;
//    vVerts[7] = vVerts[4];
//
//    triangleBatch.CopyVertexData3f(vVerts);
//    glutPostRedisplay();
//}

#pragma mark - 键盘输入操作(移动图形---通过矩阵方式)
void SpeacialKeysMatrix(int key, int x, int y) {
    GLfloat stepSize = 0.025f;
    
    if (key == GLUT_KEY_UP) {
        yPos += stepSize;
    } else if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    } else if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    } else if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    
    // 加入边界判断
    if (xPos < -1.0f + halfWidth) {
        xPos = -1.0f + halfWidth;
    }
    if (xPos > 1.0f - halfWidth) {
        xPos = 1.0f - halfWidth;
    }
    if (yPos > 1.0f - 2*halfWidth) {
        yPos = 1.0f - 2*halfWidth;
    }
    if (yPos < -1.0f) {
        yPos = -1.0f;
    }
    
    glutPostRedisplay(); // 重新渲染
}

#pragma mark - main 函数，初始化工作
int main(int argc,char* argv[])

{
    
    //设置当前工作目录，针对MAC OS X
    
    gltSetWorkingDirectory(argv[0]);
    
    //初始化GLUT库
    
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    
    glutInitWindowSize(800,600);

    glutCreateWindow("Triangle");
    
    //注册回调函数
    
    glutReshapeFunc(ChangeSize);
    
    glutDisplayFunc(RenderScene);
    
    
//    glutSpecialFunc(SpeacialKeysRect); // 通过坐标改变位置
    glutSpecialFunc(SpeacialKeysMatrix); // 通过矩阵改变位置

    
    //驱动程序的初始化中没有出现任何问题。
    
    GLenum err = glewInit();
    
    if(GLEW_OK != err) {
        
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        
        return 1;
        
    }
    
    //调用SetupRC
    
    SetupRC();
    
    glutMainLoop();
    
    return 0;
    
}



