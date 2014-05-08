// Copyright (C) 2013 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CEGLManager.h"

#ifdef _IRR_COMPILE_WITH_EGL_MANAGER_

#include "irrString.h"
#include "os.h"

namespace irr
{
namespace video
{

CEGLManager::CEGLManager() : IContextManager(),   MajorVersion(0), MinorVersion(0)
{
	#ifdef _DEBUG
	setDebugName("CEGLManager");
	#endif
}

CEGLManager::~CEGLManager()
{
    destroyContext();
    destroySurface();
    terminate();
}

bool CEGLManager::initialize(const SIrrlichtCreationParameters& params, const SExposedVideoData& data)
{
	Params=params;
	Data=data;

	

	gWindow = (SDL_Window*)Data.SDLData.Window;

    return true;
}

void CEGLManager::terminate()
{
 
    MajorVersion = 0;
    MinorVersion = 0;
}

bool CEGLManager::generateSurface()
{
   

    return true;
}

void CEGLManager::destroySurface()
{
   
}

bool CEGLManager::generateContext()
{
   
	//	OpenGLESVersion = 1;
	
    return true;
}

void CEGLManager::destroyContext()
{
   
}

bool CEGLManager::activateContext(const SExposedVideoData& videoData)
{
	
	return true;
}

const SExposedVideoData& CEGLManager::getContext() const
{
	return Data;
}

bool CEGLManager::swapBuffers()
{
	SDL_GL_SwapWindow( gWindow );
    return true;
}

bool CEGLManager::testEGLError()
{

	return true;

}

}
}

#endif
