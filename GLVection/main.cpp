//
//  main.cpp
//  GLVection
//
//  Created by Kenji Kamimura on 2015/05/15.
//  Copyright (c) 2015年 Kenji Kamimura. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

// glut is now deprecated
//  thus change it to GLFW
#include "GLFW/glfw3.h"
#include <OpenGL/OpenGL.h>
//  To use GLFW: fink, brew or port install glfw.
//  Folowing frameworks are required.
//      CoreVideo, IOKit, Cocoa, OpenGL
//   and include libglfw.dylib to build phase.

// function at exitting program
static void cleanup()
{
    // finalize GLFW
    glfwTerminate();
}

#define PERIODLEN 512

// control variables  (global)
double vArea = 2.0;         // 垂直方向の描画サイズ
double hArea = 2.0;         // 水平方向の描画サイズ
int cycles = 5;             // 描画範囲に何周期置くか
int stop=-1;
double velocity = 0.05;    // 初期速度
double texpos = 0.0;        // 描画位置調整用
//GLboolean exitProg = GL_FALSE;
GLboolean windowed = GL_FALSE;
GLFWwindow *window;
GLdouble rotAngle = 180.0;

// Keyboard CallBack
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            // Space押されたら停止させる
            case GLFW_KEY_SPACE:
                stop *= -1;
                break;
                
            // jで左向きに加速，kで右向きに加速，fで逆向きに進む
            case GLFW_KEY_J:
                velocity+=0.01;
                if (velocity > 1.0)
                {
                    velocity = 1.0;
                }
                fprintf(stderr,"velocity=%f\n",velocity);
                break;
                
            case GLFW_KEY_K:
                velocity-=0.01;
                if (velocity < 0.0)
                {
                    velocity = 0.0;
                }
                fprintf(stderr,"velocity=%f\n",velocity);
                break;
            
            case GLFW_KEY_F:
                rotAngle = rotAngle + 180.0;
                if (rotAngle > 360)
                {
                    rotAngle -= 360;
                }
                fprintf(stderr,"angle=%f\n",rotAngle);
                break;
                
            case GLFW_KEY_R:
                rotAngle += 5.0;
                if (rotAngle > 360.0)
                {
                    rotAngle -= 360.0;
                }
                fprintf(stderr,"angle=%f\n",rotAngle);
                break;

            case GLFW_KEY_V:
                rotAngle -= 5.0;
                if (rotAngle < 0.0)
                {
                    rotAngle += 360.0;
                }
                fprintf(stderr,"angle=%f\n",rotAngle);
                break;
                
            // hで周期を短く，lで長くする
            case GLFW_KEY_H:
                cycles--;
                if (cycles < 0)
                {
                    cycles = 0;
                }
                fprintf(stderr,"cycles=%d\n",cycles);
                break;
                
            case GLFW_KEY_L:
                cycles++;
                fprintf(stderr,"cycles=%d\n",cycles);
                break;
                
//            // ESCで終了
//            case GLFW_KEY_ESCAPE:
//                exitProg = GL_TRUE;
//                break;

            default:
                break;
        }
    }
}

int main(int argc, const char * argv[]) {
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        // Initialization fail
        std::cerr << "Can't initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // 終了時処理の登録
    atexit(cleanup);
    
    // Sin波の作成
    GLubyte sinwave[PERIODLEN][3] = {0};
    for (int indx=0; indx < PERIODLEN; indx++)
    {
        sinwave[indx][0] = ((sin((double)indx/PERIODLEN*2*M_PI)+1)/2)*255;
        sinwave[indx][1] = ((sin((double)indx/PERIODLEN*2*M_PI)+1)/2)*255;
        sinwave[indx][2] = ((sin((double)indx/PERIODLEN*2*M_PI)+1)/2)*255;
    }
    
    // obtaine monitors for later use.
    int m_count;
    GLFWmonitor **monitors = glfwGetMonitors(&m_count);
    
    // test 2015/05/29 get monitor physical size to calculate resolution as visual angle.
    int monHeight, monWidth;
    glfwGetMonitorPhysicalSize(monitors[0], &monWidth, &monHeight);
    
    // create window
    window = glfwCreateWindow(640, 480, "GL Vection", NULL, NULL);
    windowed = GL_TRUE;

    if (window == NULL)
    {
        // fail in window creation
        std::cerr << "Can't create GLFW window." << std::endl;
        return EXIT_FAILURE;
    }

    // gamma ramp
    // object's gamma(input) -> colorsync -> display's gamma(output)
    //  gamma ramp seems adjusting colorsync property.
    
    // ガンマを取得して逆関数をかけようと思ったが，取得されるガンマはほぼ線形
    // 　カラープロファイルを読んでいるわけではなさそう
    {
//        const GLFWgammaramp *gammaO = glfwGetGammaRamp(monitors[0]);
//        GLFWgammaramp gammaI;
//        gammaI.size = gammaO->size;
//        unsigned short red[gammaO->size], green[gammaO->size], blue[gammaO->size];
//        unsigned int gStep = 65535 / gammaI.size, gV, temp[1024];
//        red[0] = 0;
//        green[0] = 0;
//        blue[0] = 0;
//        for (int i=1; i < gammaI.size; i++)
//        {
//            temp[i] = gammaO->red[i];
//            gV = (i * gStep) * (i * gStep);
//            red[i] = gV / gammaO->red[i];
//            green[i] = gV / gammaO->green[i];
//            blue[i] = gV / gammaO->blue[i];
//        }
//        gammaI.red = red;
//        gammaI.green = green;
//        gammaI.blue = blue;
//        
//        glfwSetGammaRamp(monitors[0], &gammaI);
    }   // スコープ作るための括弧

    glfwSetGamma(monitors[0], 1/2.2);    // inverse gamma
    
    // set the window as openGL target
    glfwMakeContextCurrent(window);
    
    // set keyboard callback function
    glfwSetKeyCallback(window, keyCallBack);
    
    GLuint textureS;
    glGenTextures(1, &textureS);
    glBindTexture(GL_TEXTURE_1D, textureS);
    
    // data transfere mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // texture assign
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, PERIODLEN, 0, GL_RGB, GL_UNSIGNED_BYTE, sinwave);
    
    // テクスチャ拡大縮小オプション
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // set clear color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // configure
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 0, 5);
    glMatrixMode(GL_MODELVIEW);
    
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

    // rendering loop
    while (glfwWindowShouldClose(window) == GL_FALSE && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // calc texture pos
        if(stop<0) texpos += velocity;
        // 1枚分以上ずれた時は元の位置に戻す(なくても動くが気持ちの問題)
        if (texpos >= 1.0) texpos -= 1.0;
        if (texpos <= -1.0) texpos += 1.0;
        // テクスチャマッピングはデフォルトでRepeatモード
        
        glRotated(rotAngle, 0, 0, 1.0);
        glEnable(GL_TEXTURE_1D);
        //glNormal3d(0, 0, 1);
        glBegin(GL_QUADS);
        glTexCoord2d(texpos*hArea, 0); glVertex3d(-hArea, -vArea, -1);
        glTexCoord2d(texpos*hArea, 1); glVertex3d(-hArea, vArea, -1);
        glTexCoord2d((texpos+cycles)*hArea, 1); glVertex3d(hArea, vArea, -1);
        glTexCoord2d((texpos+cycles)*hArea, 0); glVertex3d(hArea, -vArea, -1);
        glEnd();
        glDisable(GL_TEXTURE_1D);

        // mask example
        //glColor3d(0, 0, 0);
        //glBegin(GL_QUADS);
        //glVertex3d(-1, -1, -0.5);
        //glVertex3d(-1, 1, -0.5);
        //glVertex3d(-0.3, 1, -0.5);
        //glVertex3d(-0.3, -1, -0.5);
        //glEnd();
        //glColor3d(1, 1, 1);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // toggle Fullscreen
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
             // window廃棄を伴うので，同期処理する(Kyeboardコールバック(非同期)ではやらない)
            if (windowed)
            {
                // とりあえず何も考えずにプライマリモニタを利用
                const GLFWvidmode *mode = glfwGetVideoMode(monitors[0]);
                GLFWwindow *tmpWind = glfwCreateWindow(mode->width, mode->height, "GL Vection", monitors[0], window);
                glfwDestroyWindow(window);
                window = tmpWind;
                glfwMakeContextCurrent(window);
                
                windowed = GL_FALSE;
            }
            else
            {
                GLFWwindow *tmpWind = glfwCreateWindow(640, 480, "GL Vection", NULL, window);
                glfwDestroyWindow(window);
                window = tmpWind;
                glfwMakeContextCurrent(window);
                
                windowed = GL_TRUE;
            }
            // keyboard callback
            glfwSetKeyCallback(window, keyCallBack);
            
            // texture assign
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, PERIODLEN, 0, GL_RGB, GL_UNSIGNED_BYTE, sinwave);
            
            // テクスチャ拡大縮小オプション
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       }
    }

    return EXIT_SUCCESS;
}
