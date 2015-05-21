//
//  main.cpp
//  GLVection
//
//  Created by Kenji Kamimura on 2015/05/15.
//  Copyright (c) 2015年 Kenji Kamimura. All rights reserved.
//

#include <iostream>
#include <cstdlib>

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

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3d(1.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex2d(-0.8,0.0);
  glVertex2d(0.8,0.0);
  glVertex2d(0.0,-0.8);
  glVertex2d(0.0,0.8);
  glEnd();
  glFlush();
}

int main(int argc, const char * argv[]) {
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        // Initialization fail
        std::cerr << "Can't initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // 終了時処理
    atexit(cleanup);
    
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
    // Target window which was created in OpenGL
    glfwMakeContextCurrent(window);
    // Specify background color
    glClearColor(1.0f, 1.0f, 0.5f,1.0f);
   
    while (glfwWindowShouldClose(window)==GL_FALSE)
    {
        // Clear window
        glClear(GL_COLOR_BUFFER_BIT);
       
        display();
        // Swap color buffers
        glfwSwapBuffers(window);
        
        glfwWaitEvents();
    }
    return EXIT_SUCCESS;
}
