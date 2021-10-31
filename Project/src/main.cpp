//=======================================================================================
//
// main.cpp
// Copyright Greg King 2021
//
// Distributed under the MIT licence
//
//    MIT License
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//
//=======================================================================================

#include <GLFW/glfw3.h>

#include "G64.h"
#include "GVICII.h"
#include "GColour.h"

#define SCREEN_FRAME_ASPECT (320.f / 200.f)
#define SCREEN_FRAME_PIXEL_WIDTH (640)
#define SCREEN_FRAME_PIXEL_HEIGHT (SCREEN_FRAME_PIXEL_WIDTH / SCREEN_FRAME_ASPECT)

#define SCREEN_BORDER_FRACTION_X (0.12f)
#define SCREEN_BORDER_FRACTION_Y (0.12f)

//========================================================================
//
// App globals
//
//========================================================================
GLFWwindow* window;
G64* g64;
GLuint texture;

//========================================================================
//
// Initialise glfw
//
//========================================================================
int initGL() {
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
	}

	int windowWidth = SCREEN_FRAME_PIXEL_WIDTH/(1-2*SCREEN_BORDER_FRACTION_X);
	int windowHeight = SCREEN_FRAME_PIXEL_HEIGHT/(1-2*SCREEN_BORDER_FRACTION_Y);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "G64", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return 0;
}


//========================================================================
//
// Initialise the G64
//
//========================================================================
int initG64(const char* const prgPath) {
    g64 = new G64();

    if(g64 == NULL) {
        return -1;
    }

    if(g64->loadPrg(prgPath) != 0) {
		return -2;
    }

    return 0;
}


//========================================================================
//
// Shutdown and dispose of the G64
//
//========================================================================
void disposeG64() {
    if(g64 != NULL) {
        delete g64;
        g64 = NULL;
    }
}


//========================================================================
//
// Shutdown openGL
//
//========================================================================
void disposeGL() {
    glfwTerminate();
}


//========================================================================
//
// Render the G64 framebuffer to OpenGL
//
//========================================================================
void renderG64Video() {
    GVICII* gpu = g64->getVideo();

    const GColour clearCol = gpu->getBorderColour();
    glClearColor(   clearCol.getRedFloat(),
                    clearCol.getGreenFloat(),
                    clearCol.getBlueFloat(),
                    1.0f
                );

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					gpu->getFrameBufferWidth(),
					gpu->getFrameBufferHeight(),
					0,
					GL_RGBA,
					GL_UNSIGNED_SHORT_5_5_5_1,
					gpu->getFrameBuffer()
				);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height); // need to do something better with this when aspect is all wrong

	float left = width*SCREEN_BORDER_FRACTION_X;
	float right = width - left;
	float bottom = height*SCREEN_BORDER_FRACTION_Y;
	float top = height - bottom;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.f, 1.f);	glVertex3f(left, top, 0);
		glTexCoord2f(1.f, 1.f);	glVertex3f(right, top, 0);
		glTexCoord2f(1.f, 0.f);	glVertex3f(right, bottom, 0);
		glTexCoord2f(0.f, 0.f);	glVertex3f(left, bottom, 0);
	}
	glEnd();
}

//========================================================================
//
// Entry point
//
//========================================================================
int main(void)
{
	int errorCode;

	errorCode = initGL();
    if(errorCode != 0) {
		printf("Failed to initialise OpenGL.  Error code: %d\n", errorCode);
        return -1;
	}

	errorCode = initG64("HelloWorld.prg");
    if(errorCode != 0) {
		printf("Failed to initialise G64.  Error code: %d\n", errorCode);
        return -2;
	}

	g64->start();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderG64Video();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    g64->stop();

    disposeG64();
    disposeGL();

    return 0;
}
