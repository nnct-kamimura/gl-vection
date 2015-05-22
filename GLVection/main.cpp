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
double vArea = 1.0;         // 垂直方向の描画サイズ
double hArea = 1.0;         // 水平方向の描画サイズ
int cycles = 5;             // 描画範囲に何周期置くか
int stop=-1;
double velocity = -0.05;    // 運動右向きが負
double texpos = 0.0;        // 描画位置調整用
GLboolean exitProg = GL_FALSE;


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
                
            // jで速さを小さく，kで大きく．(右向き正)
            case GLFW_KEY_J:
                velocity+=0.01;
                fprintf(stderr,"velocity=%f\n",velocity);
                break;
                
            case GLFW_KEY_K:
                velocity-=0.01;
                fprintf(stderr,"velocity=%f\n",velocity);
                break;
            
            // ESCで終了
            case GLFW_KEY_ESCAPE:
                exitProg = GL_TRUE;
                break;

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
    
    // とりあえず何も考えずにプライマリモニタを利用
    const GLFWvidmode *mode = glfwGetVideoMode(monitors[0]);
    
    // create window
    //GLFWwindow *const window(glfwCreateWindow(mode->width, mode->height, "GL Vection", monitors[0], NULL));
    GLFWwindow *const window(glfwCreateWindow(640, 480, "GL Vection", NULL, NULL));
    
    if (window == NULL)
    {
        // fail in window creation
        std::cerr << "Can't create GLFW window." << std::endl;
        return EXIT_FAILURE;
    }
    
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
        
        glEnable(GL_TEXTURE_1D);
        //glNormal3d(0, 0, 1);
        glBegin(GL_QUADS);
        glTexCoord2d(texpos, 0); glVertex3d(-hArea, -vArea, -1);
        glTexCoord2d(texpos, 1); glVertex3d(-hArea, vArea, -1);
        glTexCoord2d(texpos+cycles, 1); glVertex3d(hArea, vArea, -1);
        glTexCoord2d(texpos+cycles, 0); glVertex3d(hArea, -vArea, -1);
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
    }
    

    return EXIT_SUCCESS;
}
