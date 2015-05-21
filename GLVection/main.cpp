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
    GLFWwindow *const window(glfwCreateWindow(mode->width, mode->height, "GL Vection", monitors[0], NULL));
    //GLFWwindow *const window(glfwCreateWindow(640, 480, "GL Vection", NULL, NULL));
    
    if (window == NULL)
    {
        // fail in window creation
        std::cerr << "Can't create GLFW window." << std::endl;
        return EXIT_FAILURE;
    }
    
    // set the window as openGL target
    glfwMakeContextCurrent(window);
    
    // set clear color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // rendering loop
    while (glfwWindowShouldClose(window) == GL_FALSE && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return EXIT_SUCCESS;
}
