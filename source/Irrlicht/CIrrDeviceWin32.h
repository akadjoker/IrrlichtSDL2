// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_IRR_DEVICE_WIN32_H_INCLUDED__
#define __C_IRR_DEVICE_WIN32_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_

#include "CIrrDeviceStub.h"
#include "IrrlichtDevice.h"
#include "IImagePresenter.h"

#define WIN32_LEAN_AND_MEAN
#if !defined(_IRR_XBOX_PLATFORM_)
	#include <windows.h>
	#include <mmsystem.h> // For JOYCAPS
	#include <windowsx.h>
#endif
#if !defined(GET_X_LPARAM)
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

namespace irr
{
	struct SJoystickWin32Control;

	class CIrrDeviceWin32 : public CIrrDeviceStub, video::IImagePresenter
	{
	friend struct SJoystickWin32Control;
	public:

		//! constructor
		CIrrDeviceWin32(const SIrrlichtCreationParameters& params);

		//! destructor
		virtual ~CIrrDeviceWin32();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run() _IRR_OVERRIDE_;

		//! Cause the device to temporarily pause execution and let other processes to run
		// This should bring down processor usage without major performance loss for Irrlicht
		virtual void yield() _IRR_OVERRIDE_;

		//! Pause execution and let other processes to run for a specified amount of time.
		virtual void sleep(u32 timeMs, bool pauseTimer) _IRR_OVERRIDE_;

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text) _IRR_OVERRIDE_;

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive() const _IRR_OVERRIDE_;

		//! returns if window has focus
		virtual bool isWindowFocused() const _IRR_OVERRIDE_;

		//! returns if window is minimized
		virtual bool isWindowMinimized() const _IRR_OVERRIDE_;

		//! presents a surface in the client area
		virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0) _IRR_OVERRIDE_;

		//! notifies the device that it should close itself
		virtual void closeDevice() _IRR_OVERRIDE_;

		//! \return Returns a pointer to a list with all video modes
		//! supported by the gfx adapter.
		virtual video::IVideoModeList* getVideoModeList() _IRR_OVERRIDE_;

		//! Notifies the device, that it has been resized
		/** Must be publis as it is called from free function (event handler) */
		void OnResized();

		//! Sets if the window should be resizable in windowed mode.
		virtual void setResizable(bool resize=false) _IRR_OVERRIDE_;

		//! Resize the render window.
		virtual void setWindowSize(const irr::core::dimension2d<u32>& size) _IRR_OVERRIDE_;

		//! Minimizes the window.
		virtual void minimizeWindow() _IRR_OVERRIDE_;

		//! Maximizes the window.
		virtual void maximizeWindow() _IRR_OVERRIDE_;

		//! Restores the window size.
		virtual void restoreWindow() _IRR_OVERRIDE_;

		//! Get the position of the window on screen
		virtual core::position2di getWindowPosition() _IRR_OVERRIDE_;

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo) _IRR_OVERRIDE_;

		//! Set the current Gamma Value for the Display
		virtual bool setGammaRamp( f32 red, f32 green, f32 blue, f32 brightness, f32 contrast ) _IRR_OVERRIDE_;

		//! Get the current Gamma Value for the Display
		virtual bool getGammaRamp( f32 &red, f32 &green, f32 &blue, f32 &brightness, f32 &contrast ) _IRR_OVERRIDE_;

		//! Remove all messages pending in the system message loop
		virtual void clearSystemMessages() _IRR_OVERRIDE_;

		//! Get the device type
		virtual E_DEVICE_TYPE getType() const _IRR_OVERRIDE_
		{
				return EIDT_WIN32;
		}

		//! Compares to the last call of this function to return double and triple clicks.
		//! \return Returns only 1,2 or 3. A 4th click will start with 1 again.
		virtual u32 checkSuccessiveClicks(s32 mouseX, s32 mouseY, EMOUSE_INPUT_EVENT inputEvent ) _IRR_OVERRIDE_
		{
			// we just have to make it public
			return CIrrDeviceStub::checkSuccessiveClicks(mouseX, mouseY, inputEvent );
		}

		//! switchs to fullscreen
		bool switchToFullScreen(bool reset=false);

		//! Check for and show last Windows API error to help internal debugging.
		//! Does call GetLastError and on errors formats the errortext and displays it in a messagebox.
		static void ReportLastWinApiError();
	//! Implementation of the win32 cursor control
		class CCursorControl : public gui::ICursorControl
		{
		public:

			CCursorControl(CIrrDeviceWin32* device, const core::dimension2d<u32>& wsize, HWND hwnd, bool fullscreen);
			~CCursorControl();

			//! Changes the visible state of the mouse cursor.
			virtual void setVisible(bool visible) _IRR_OVERRIDE_
			{
				
				IsVisible = visible;
			}

			//! Returns if the cursor is currently visible.
			virtual bool isVisible() const _IRR_OVERRIDE_
			{
				_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
				return IsVisible;
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(const core::position2d<f32> &pos) _IRR_OVERRIDE_
			{
				setPosition(pos.X, pos.Y);
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(f32 x, f32 y) _IRR_OVERRIDE_
			{
							}

			//! Sets the new position of the cursor.
			virtual void setPosition(const core::position2d<s32> &pos) _IRR_OVERRIDE_
			{
				setPosition(pos.X, pos.Y);
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(s32 x, s32 y) _IRR_OVERRIDE_
			{
				
				CursorPos.X = x;
				CursorPos.Y = y;
			}

			//! Returns the current position of the mouse cursor.
			virtual const core::position2d<s32>& getPosition() _IRR_OVERRIDE_
			{
				
				return CursorPos;
			}

			//! Returns the current position of the mouse cursor.
			virtual core::position2d<f32> getRelativePosition() _IRR_OVERRIDE_
			{
				
				return core::position2d<f32>(CursorPos.X / (f32)ReferenceRect.getWidth(),
						CursorPos.Y / (f32)ReferenceRect.getHeight());
			}

			//! Sets an absolute reference rect for calculating the cursor position.
			virtual void setReferenceRect(core::rect<s32>* rect=0) _IRR_OVERRIDE_
			{
			
			}

			/** Used to notify the cursor that the window was resized. */
			void OnResize(const core::dimension2d<u32>& size)
			{
				
			}

			/** Used to notify the cursor that the window resizable settings changed. */
			void updateBorderSize(bool fullscreen, bool resizable)
			{
				
			}


			//! Sets the active cursor icon
			virtual void setActiveIcon(gui::ECURSOR_ICON iconId) _IRR_OVERRIDE_;

			//! Gets the currently active icon
			virtual gui::ECURSOR_ICON getActiveIcon() const _IRR_OVERRIDE_
			{
				return ActiveIcon;
			}

			//! Add a custom sprite as cursor icon.
			virtual gui::ECURSOR_ICON addIcon(const gui::SCursorSprite& icon) _IRR_OVERRIDE_;

			//! replace the given cursor icon.
			virtual void changeIcon(gui::ECURSOR_ICON iconId, const gui::SCursorSprite& icon) _IRR_OVERRIDE_;

			//! Return a system-specific size which is supported for cursors. Larger icons will fail, smaller icons might work.
			virtual core::dimension2di getSupportedIconSize() const _IRR_OVERRIDE_;

			void update();

		private:

			//! Updates the internal cursor position
			void updateInternalCursorPosition()
			{
				
			}

			CIrrDeviceWin32* Device;
			core::position2d<s32> CursorPos;
			core::dimension2d<u32> WindowSize;
			core::dimension2d<f32> InvWindowSize;
			HWND HWnd;

			s32 BorderX, BorderY;
			core::rect<s32> ReferenceRect;
			bool UseReferenceRect;
			bool IsVisible;


			struct CursorFrameW32
			{
				CursorFrameW32() : IconHW(0) {}
				CursorFrameW32(HCURSOR icon) : IconHW(icon) {}

				HCURSOR IconHW;	// hardware cursor
			};

			struct CursorW32
			{
				CursorW32() {}
				explicit CursorW32(HCURSOR iconHw, u32 frameTime=0) : FrameTime(frameTime)
				{
					Frames.push_back( CursorFrameW32(iconHw) );
				}
				core::array<CursorFrameW32> Frames;
				u32 FrameTime;
			};

			core::array<CursorW32> Cursors;
			gui::ECURSOR_ICON ActiveIcon;
			u32 ActiveIconStartTime;

			void initCursors();
		};

		//! returns the win32 cursor control
		CCursorControl* getWin32CursorControl();

	private:

		//! create the driver
		void createDriver();

		//! Process system events
		void handleSystemMessages();

		void getWindowsVersion(core::stringc& version);

		void resizeIfNecessary();

		bool ChangedToFullScreen;
		bool Resized;
		bool ExternalWindow;
		CCursorControl* Win32CursorControl;
		SJoystickWin32Control* JoyControl;
		DEVMODE DesktopMode;
		HWND HWnd;
	};

} // end namespace irr

#endif // _IRR_COMPILE_WITH_WINDOWS_DEVICE_
#endif // __C_IRR_DEVICE_WIN32_H_INCLUDED__
