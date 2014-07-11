//==================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "OpenGLWindow.h"

namespace t3d
{
	OpenGLWindow::OpenGLWindow()
	{
		mWidth = gDefaultWindowWidth;
		mHeight = gDefaultWindowHeight;
		mTitle = gWindowTitle;
		mWindow = NULL;
		mAspectScale = Vec2f(1.0f, 1.0f);
		mZoom = Vec2f(1.0f, 1.0f);
		mShouldTerminate = false;
	}

	
	bool OpenGLWindow::init(int width, int height, const String &title)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;

		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!mWindow)
		{
			terminate();
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(mWindow);
		gl3wInit();
		printContextInformation();

		//if (gl3wIsSupported(4, 4) == false)
		//	printf("Bad OpenGL version.\n");
		GLFWInput::instance();
		glfwSetKeyCallback(mWindow, reinterpret_cast<GLFWkeyfun>(GLFWInput::keyboardCallback));
		glfwSetWindowSizeCallback(mWindow, GLFWInput::windowResizeCallback);
		GLFWInput::instance().addWindow(this);
		_onResize(mWidth, mHeight);

		onStartup();

		sf::Clock clock;

		while (!glfwWindowShouldClose(mWindow)  &&  !mShouldTerminate)
		{
			onUpdate((double)clock.getElapsedTime().asSeconds());

			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}

		onTerminate();
		glfwTerminate();

		return true;
	}


	void OpenGLWindow::printContextInformation() const
	{
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	}


	void OpenGLWindow::setZoom(Vec2f zoom)
	{
		mZoom = zoom;

		if (mZoom.x < 0.0)
			mZoom.x = 0.0;
		if (mZoom.y < 0.0)
			mZoom.y = 0.0;

		_onResize(mWidth, mHeight);
	}


	void OpenGLWindow::addZoom(Vec2f zoom)
	{
		mZoom += zoom;

		if (mZoom.x < 0.0)
			mZoom.x = 0.0;
		if (mZoom.y < 0.0)
			mZoom.y = 0.0;

		_onResize(mWidth, mHeight);
	}

	
	void OpenGLWindow::_onResize(int width, int height)
	{
		mAspectScale = Vec2f((500.0f/width)*mZoom.x, (500.0f/height)*mZoom.y);
		mWidth = width;
		mHeight = height;

		onResize(width, height);
	}
};