// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_EXPOSED_VIDEO_DATA_H_INCLUDED__
#define __S_EXPOSED_VIDEO_DATA_H_INCLUDED__



namespace irr
{
namespace video
{

//! structure for holding data describing a driver and operating system specific data.
/** This data can be retrived by IVideoDriver::getExposedVideoData(). Use this with caution.
This only should be used to make it possible to extend the engine easily without
modification of its source. Note that this structure does not contain any valid data, if
you are using the software or the null device.
*/
struct SExposedVideoData
{
	SExposedVideoData() {}
	explicit SExposedVideoData(void* Window) {SDLData.Window=Window;}

	union
	{
		

		

		

	
        struct
		{
			void* Window;
		} SDLData;
      

	};
};

} // end namespace video
} // end namespace irr


#endif

