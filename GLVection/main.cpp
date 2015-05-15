//
//  main.cpp
//  GLVection
//
//  Created by Kenji Kamimura on 2015/05/15.
//  Copyright (c) 2015年 Kenji Kamimura. All rights reserved.
//

#include <iostream>

// glut is now deprecated
//  thus change it to GLFW
#include "GLFW/glfw3.h"
#include <OpenGL/OpenGL.h>

//  To use GLFW: fink, brew or port install glfw.
//  Folowing frameworks are required.
//      CoreVideo, IOKit, Cocoa, OpenGL
//   and include libglfw.dylib to build phase.
int main(int argc, const char * argv[]) {
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
