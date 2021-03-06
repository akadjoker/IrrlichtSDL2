// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_READ_FILE_H_INCLUDED__
#define __C_READ_FILE_H_INCLUDED__

#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>

#include <stdio.h>
#include "IReadFile.h"
#include "irrString.h"

namespace irr
{

namespace io
{

	/*!
		Class for reading a real file from disk.
	*/
	class CReadFile : public IReadFile
	{
	public:

		CReadFile(const io::path& fileName);

		virtual ~CReadFile();

		//! returns how much was read
		virtual s32 read(void* buffer, u32 sizeToRead) _IRR_OVERRIDE_;

		//! changes position in file, returns true if successful
		virtual bool seek(long finalPos, bool relativeMovement = false) _IRR_OVERRIDE_;

		//! returns size of file
		virtual long getSize() const _IRR_OVERRIDE_;

		//! returns if file is open
		bool isOpen() const
		{
			return File != 0;
		}

		//! returns where in the file we are.
		virtual long getPos() const _IRR_OVERRIDE_;

		//! returns name of file
		virtual const io::path& getFileName() const _IRR_OVERRIDE_;

		//! create read file on disk.
		static IReadFile* createReadFile(const io::path& fileName);

	private:

		//! opens the file
		void openFile();
		SDL_RWops *File;
		//FILE* File;
		long FileSize;
		io::path Filename;
	};

} // end namespace io
} // end namespace irr

#endif

