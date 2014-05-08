// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_

#include "CIrrDeviceWin32.h"
#include "IEventReceiver.h"
#include "irrList.h"
#include "os.h"

#include "CTimer.h"
#include "irrString.h"
#include "COSOperator.h"
#include "dimension2d.h"
#include "IGUISpriteBank.h"
#include <winuser.h>
#include "SExposedVideoData.h"
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
#ifdef _IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#ifdef _MSC_VER
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#endif
#else
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif
#endif
#endif
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_)
#include "CEGLManager.h"
#endif
#if defined(_IRR_WINDOWS_API_)
#include "CWGLManager.h"
#endif

namespace irr
{
	namespace video
	{
		#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
		IVideoDriver* createDirectX8Driver(const irr::SIrrlichtCreationParameters& params,
			io::IFileSystem* io, HWND window);
		#endif

		#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
		IVideoDriver* createDirectX9Driver(const irr::SIrrlichtCreationParameters& params,
			io::IFileSystem* io, HWND window);
		#endif

		#ifdef _IRR_COMPILE_WITH_OPENGL_
		IVideoDriver* createOpenGLDriver(const irr::SIrrlichtCreationParameters& params,
			io::IFileSystem* io, IContextManager* contextManager);
		#endif
        
        #ifdef _IRR_COMPILE_WITH_OGLES1_ 	 
        IVideoDriver* createOGLES1Driver(const SIrrlichtCreationParameters& params, io::IFileSystem* io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_)
        , IContextManager* contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
        , CIrrDeviceIPhone* device
#endif
		); 	 
        #endif 	 
        
        #ifdef _IRR_COMPILE_WITH_OGLES2_ 	 
        IVideoDriver* createOGLES2Driver(const SIrrlichtCreationParameters& params, io::IFileSystem* io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_)
        , IContextManager* contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
        , CIrrDeviceIPhone* device
#endif
		); 	 
        #endif
	}
} // end namespace irr

namespace irr
{
struct SJoystickWin32Control
{
	CIrrDeviceWin32* Device;

#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_) && defined(_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_)
	IDirectInput8* DirectInputDevice;
#endif
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
	struct JoystickInfo
	{
		u32 Index;
#ifdef _IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
		core::stringc Name;
		GUID guid;
		LPDIRECTINPUTDEVICE8 lpdijoy;
		DIDEVCAPS devcaps;
		u8 axisValid[8];
#else
		JOYCAPS Caps;
#endif
	};
	core::array<JoystickInfo> ActiveJoysticks;
#endif

	SJoystickWin32Control(CIrrDeviceWin32* dev) : Device(dev)
	{
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_) && defined(_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_)
		DirectInputDevice=0;
		if (DI_OK != (DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInputDevice, NULL)))
		{
			os::Printer::log("Could not create DirectInput8 Object", ELL_WARNING);
			return;
		}
#endif
	}
	~SJoystickWin32Control()
	{
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_) && defined(_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_)
		for(u32 joystick = 0; joystick < ActiveJoysticks.size(); ++joystick)
		{
			LPDIRECTINPUTDEVICE8 dev = ActiveJoysticks[joystick].lpdijoy;
			if (dev)
			{
				dev->Unacquire();
			}
			dev->Release();
		}

		if (DirectInputDevice)
			DirectInputDevice->Release();
#endif
	}

#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_) && defined(_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_)
	static BOOL CALLBACK EnumJoysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID cp)
	{
		SJoystickWin32Control* p=(SJoystickWin32Control*)cp;
		p->directInputAddJoystick(lpddi);
		return DIENUM_CONTINUE;
	}
	void directInputAddJoystick(LPCDIDEVICEINSTANCE lpddi)
	{
		//Get the GUID of the joystuck
		const GUID guid = lpddi->guidInstance;

		JoystickInfo activeJoystick;
		activeJoystick.Index=ActiveJoysticks.size();
		activeJoystick.guid=guid;
		activeJoystick.Name=lpddi->tszProductName;
		if (FAILED(DirectInputDevice->CreateDevice(guid, &activeJoystick.lpdijoy, NULL)))
		{
			os::Printer::log("Could not create DirectInput device", ELL_WARNING);
			return;
		}

		activeJoystick.devcaps.dwSize=sizeof(activeJoystick.devcaps);
		if (FAILED(activeJoystick.lpdijoy->GetCapabilities(&activeJoystick.devcaps)))
		{
			os::Printer::log("Could not create DirectInput device", ELL_WARNING);
			return;
		}

		if (FAILED(activeJoystick.lpdijoy->SetCooperativeLevel(Device->HWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE)))
		{
			os::Printer::log("Could not set DirectInput device cooperative level", ELL_WARNING);
			return;
		}

		if (FAILED(activeJoystick.lpdijoy->SetDataFormat(&c_dfDIJoystick2)))
		{
			os::Printer::log("Could not set DirectInput device data format", ELL_WARNING);
			return;
		}

		if (FAILED(activeJoystick.lpdijoy->Acquire()))
		{
			os::Printer::log("Could not set DirectInput cooperative level", ELL_WARNING);
			return;
		}

		DIJOYSTATE2 info;
		if (FAILED(activeJoystick.lpdijoy->GetDeviceState(sizeof(info),&info)))
		{
			os::Printer::log("Could not read DirectInput device state", ELL_WARNING);
			return;
		}

		ZeroMemory(activeJoystick.axisValid,sizeof(activeJoystick.axisValid));
		activeJoystick.axisValid[0]= (info.lX!=0) ? 1 : 0;
		activeJoystick.axisValid[1]= (info.lY!=0) ? 1 : 0;
		activeJoystick.axisValid[2]= (info.lZ!=0) ? 1 : 0;
		activeJoystick.axisValid[3]= (info.lRx!=0) ? 1 : 0;
		activeJoystick.axisValid[4]= (info.lRy!=0) ? 1 : 0;
		activeJoystick.axisValid[5]= (info.lRz!=0) ? 1 : 0;

		int caxis=0;
		for (u8 i=0; i<6; i++)
		{
			if (activeJoystick.axisValid[i])
				caxis++;
		}

		for (u8 i=0; i<(activeJoystick.devcaps.dwAxes)-caxis; i++)
		{
			if (i+caxis < 8)
				activeJoystick.axisValid[i+caxis]=1;
		}

		ActiveJoysticks.push_back(activeJoystick);
	}
#endif

void pollJoysticks()
{
#if defined _IRR_COMPILE_WITH_JOYSTICK_EVENTS_
#ifdef _IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
	if(0 == ActiveJoysticks.size())
		return;

	u32 joystick;
	DIJOYSTATE2 info;

	for(joystick = 0; joystick < ActiveJoysticks.size(); ++joystick)
	{
		// needs to be reset for each joystick
		// request ALL values and POV as continuous if possible

		const DIDEVCAPS & caps = ActiveJoysticks[joystick].devcaps;
		// if no POV is available don't ask for POV values

		if (!FAILED(ActiveJoysticks[joystick].lpdijoy->GetDeviceState(sizeof(info),&info)))
		{
			SEvent event;

			event.EventType = irr::EET_JOYSTICK_INPUT_EVENT;
			event.JoystickEvent.Joystick = (u8)joystick;

			event.JoystickEvent.POV = (u16)info.rgdwPOV[0];
			// set to undefined if no POV value was returned or the value
			// is out of range
			if ((caps.dwPOVs==0) || (event.JoystickEvent.POV > 35900))
				event.JoystickEvent.POV = 65535;

			for(int axis = 0; axis < SEvent::SJoystickEvent::NUMBER_OF_AXES; ++axis)
				event.JoystickEvent.Axis[axis] = 0;

			u16 dxAxis=0;
			u16 irrAxis=0;

			while (dxAxis < 6 && irrAxis <caps.dwAxes)
			{
				bool axisFound=0;
				s32 axisValue=0;

				switch (dxAxis)
				{
				case 0:
					axisValue=info.lX;
					break;
				case 1:
					axisValue=info.lY;
					break;
				case 2:
					axisValue=info.lZ;
					break;
				case 3:
					axisValue=info.lRx;
					break;
				case 4:
					axisValue=info.lRy;
					break;
				case 5:
					axisValue=info.lRz;
					break;
				case 6:
					axisValue=info.rglSlider[0];
					break;
				case 7:
					axisValue=info.rglSlider[1];
					break;
				default:
					break;
				}

				if (ActiveJoysticks[joystick].axisValid[dxAxis]>0)
					axisFound=1;

				if (axisFound)
				{
					s32 val=axisValue - 32768;

					if (val <-32767) val=-32767;
					if (val > 32767) val=32767;
					event.JoystickEvent.Axis[irrAxis]=(s16)(val);
					irrAxis++;
				}

				dxAxis++;
			}

			u32 buttons=0;
			BYTE* bytebuttons=info.rgbButtons;
			for (u16 i=0; i<32; i++)
			{
				if (bytebuttons[i] >0)
				{
					buttons |= (1 << i);
				}
			}
			event.JoystickEvent.ButtonStates = buttons;

			(void)Device->postEventFromUser(event);
		}
	}
#else
	if (0 == ActiveJoysticks.size())
		return;

	u32 joystick;
	JOYINFOEX info;

	for(joystick = 0; joystick < ActiveJoysticks.size(); ++joystick)
	{
		// needs to be reset for each joystick
		// request ALL values and POV as continuous if possible
		info.dwSize = sizeof(info);
		info.dwFlags = JOY_RETURNALL|JOY_RETURNPOVCTS;
		const JOYCAPS & caps = ActiveJoysticks[joystick].Caps;
		// if no POV is available don't ask for POV values
		if (!(caps.wCaps & JOYCAPS_HASPOV))
			info.dwFlags &= ~(JOY_RETURNPOV|JOY_RETURNPOVCTS);
		if(JOYERR_NOERROR == joyGetPosEx(ActiveJoysticks[joystick].Index, &info))
		{
			SEvent event;

			event.EventType = irr::EET_JOYSTICK_INPUT_EVENT;
			event.JoystickEvent.Joystick = (u8)joystick;

			event.JoystickEvent.POV = (u16)info.dwPOV;
			// set to undefined if no POV value was returned or the value
			// is out of range
			if (!(info.dwFlags & JOY_RETURNPOV) || (event.JoystickEvent.POV > 35900))
				event.JoystickEvent.POV = 65535;

			for(int axis = 0; axis < SEvent::SJoystickEvent::NUMBER_OF_AXES; ++axis)
				event.JoystickEvent.Axis[axis] = 0;

			event.JoystickEvent.ButtonStates = info.dwButtons;

			switch(caps.wNumAxes)
			{
			default:
			case 6:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_V] =
					(s16)((65535 * (info.dwVpos - caps.wVmin)) / (caps.wVmax - caps.wVmin) - 32768);

			case 5:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_U] =
					(s16)((65535 * (info.dwUpos - caps.wUmin)) / (caps.wUmax - caps.wUmin) - 32768);

			case 4:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_R] =
					(s16)((65535 * (info.dwRpos - caps.wRmin)) / (caps.wRmax - caps.wRmin) - 32768);

			case 3:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_Z] =
					(s16)((65535 * (info.dwZpos - caps.wZmin)) / (caps.wZmax - caps.wZmin) - 32768);

			case 2:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_Y] =
					(s16)((65535 * (info.dwYpos - caps.wYmin)) / (caps.wYmax - caps.wYmin) - 32768);

			case 1:
				event.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_X] =
					(s16)((65535 * (info.dwXpos - caps.wXmin)) / (caps.wXmax - caps.wXmin) - 32768);
			}

			(void)Device->postEventFromUser(event);
		}
	}
#endif
#endif // _IRR_COMPILE_WITH_JOYSTICK_EVENTS_
}

bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
{
#if defined _IRR_COMPILE_WITH_JOYSTICK_EVENTS_
#ifdef _IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
	if (!DirectInputDevice || (DirectInputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, SJoystickWin32Control::EnumJoysticks, this, DIEDFL_ATTACHEDONLY )))
	{
		os::Printer::log("Could not enum DirectInput8 controllers", ELL_WARNING);
		return false;
	}

	for(u32 joystick = 0; joystick < ActiveJoysticks.size(); ++joystick)
	{
		JoystickInfo& activeJoystick = ActiveJoysticks[joystick];
		SJoystickInfo info;
		info.Axes=activeJoystick.devcaps.dwAxes;
		info.Buttons=activeJoystick.devcaps.dwButtons;
		info.Name=activeJoystick.Name;
		info.PovHat = (activeJoystick.devcaps.dwPOVs  != 0)
				? SJoystickInfo::POV_HAT_PRESENT : SJoystickInfo::POV_HAT_ABSENT;
		joystickInfo.push_back(info);
	}
	return true;
#else
	joystickInfo.clear();
	ActiveJoysticks.clear();

	const u32 numberOfJoysticks = ::joyGetNumDevs();
	JOYINFOEX info;
	info.dwSize = sizeof(info);
	info.dwFlags = JOY_RETURNALL;

	JoystickInfo activeJoystick;
	SJoystickInfo returnInfo;

	joystickInfo.reallocate(numberOfJoysticks);
	ActiveJoysticks.reallocate(numberOfJoysticks);

	u32 joystick = 0;
	for(; joystick < numberOfJoysticks; ++joystick)
	{
		if(JOYERR_NOERROR == joyGetPosEx(joystick, &info)
			&&
			JOYERR_NOERROR == joyGetDevCaps(joystick,
											&activeJoystick.Caps,
											sizeof(activeJoystick.Caps)))
		{
			activeJoystick.Index = joystick;
			ActiveJoysticks.push_back(activeJoystick);

			returnInfo.Joystick = (u8)joystick;
			returnInfo.Axes = activeJoystick.Caps.wNumAxes;
			returnInfo.Buttons = activeJoystick.Caps.wNumButtons;
			returnInfo.Name = activeJoystick.Caps.szPname;
			returnInfo.PovHat = ((activeJoystick.Caps.wCaps & JOYCAPS_HASPOV) == JOYCAPS_HASPOV)
								? SJoystickInfo::POV_HAT_PRESENT : SJoystickInfo::POV_HAT_ABSENT;

			joystickInfo.push_back(returnInfo);
		}
	}

	for(joystick = 0; joystick < joystickInfo.size(); ++joystick)
	{
		char logString[256];
		(void)sprintf(logString, "Found joystick %d, %d axes, %d buttons '%s'",
			joystick, joystickInfo[joystick].Axes,
			joystickInfo[joystick].Buttons, joystickInfo[joystick].Name.c_str());
		os::Printer::log(logString, ELL_INFORMATION);
	}

	return true;
#endif
#else
	return false;
#endif // _IRR_COMPILE_WITH_JOYSTICK_EVENTS_
}
};
} // end namespace irr

// Get the codepage from the locale language id
// Based on the table from http://www.science.co.il/Language/Locale-Codes.asp?s=decimal
static unsigned int LocaleIdToCodepage(unsigned int lcid)
{
	switch ( lcid )
	{
		case 1098:  // Telugu
		case 1095:  // Gujarati
		case 1094:  // Punjabi
		case 1103:  // Sanskrit
		case 1111:  // Konkani
		case 1114:  // Syriac
		case 1099:  // Kannada
		case 1102:  // Marathi
		case 1125:  // Divehi
		case 1067:  // Armenian
		case 1081:  // Hindi
		case 1079:  // Georgian
		case 1097:  // Tamil
			return 0;
		case 1054:  // Thai
			return 874;
		case 1041:  // Japanese
			return 932;
		case 2052:  // Chinese (PRC)
		case 4100:  // Chinese (Singapore)
			return 936;
		case 1042:  // Korean
			return 949;
		case 5124:  // Chinese (Macau S.A.R.)
		case 3076:  // Chinese (Hong Kong S.A.R.)
		case 1028:  // Chinese (Taiwan)
			return 950;
		case 1048:  // Romanian
		case 1060:  // Slovenian
		case 1038:  // Hungarian
		case 1051:  // Slovak
		case 1045:  // Polish
		case 1052:  // Albanian
		case 2074:  // Serbian (Latin)
		case 1050:  // Croatian
		case 1029:  // Czech
			return 1250;
		case 1104:  // Mongolian (Cyrillic)
		case 1071:  // FYRO Macedonian
		case 2115:  // Uzbek (Cyrillic)
		case 1058:  // Ukrainian
		case 2092:  // Azeri (Cyrillic)
		case 1092:  // Tatar
		case 1087:  // Kazakh
		case 1059:  // Belarusian
		case 1088:  // Kyrgyz (Cyrillic)
		case 1026:  // Bulgarian
		case 3098:  // Serbian (Cyrillic)
		case 1049:  // Russian
			return 1251;
		case 8201:  // English (Jamaica)
		case 3084:  // French (Canada)
		case 1036:  // French (France)
		case 5132:  // French (Luxembourg)
		case 5129:  // English (New Zealand)
		case 6153:  // English (Ireland)
		case 1043:  // Dutch (Netherlands)
		case 9225:  // English (Caribbean)
		case 4108:  // French (Switzerland)
		case 4105:  // English (Canada)
		case 1110:  // Galician
		case 10249:  // English (Belize)
		case 3079:  // German (Austria)
		case 6156:  // French (Monaco)
		case 12297:  // English (Zimbabwe)
		case 1069:  // Basque
		case 2067:  // Dutch (Belgium)
		case 2060:  // French (Belgium)
		case 1035:  // Finnish
		case 1080:  // Faroese
		case 1031:  // German (Germany)
		case 3081:  // English (Australia)
		case 1033:  // English (United States)
		case 2057:  // English (United Kingdom)
		case 1027:  // Catalan
		case 11273:  // English (Trinidad)
		case 7177:  // English (South Africa)
		case 1030:  // Danish
		case 13321:  // English (Philippines)
		case 15370:  // Spanish (Paraguay)
		case 9226:  // Spanish (Colombia)
		case 5130:  // Spanish (Costa Rica)
		case 7178:  // Spanish (Dominican Republic)
		case 12298:  // Spanish (Ecuador)
		case 17418:  // Spanish (El Salvador)
		case 4106:  // Spanish (Guatemala)
		case 18442:  // Spanish (Honduras)
		case 3082:  // Spanish (International Sort)
		case 13322:  // Spanish (Chile)
		case 19466:  // Spanish (Nicaragua)
		case 2058:  // Spanish (Mexico)
		case 10250:  // Spanish (Peru)
		case 20490:  // Spanish (Puerto Rico)
		case 1034:  // Spanish (Traditional Sort)
		case 14346:  // Spanish (Uruguay)
		case 8202:  // Spanish (Venezuela)
		case 1089:  // Swahili
		case 1053:  // Swedish
		case 2077:  // Swedish (Finland)
		case 5127:  // German (Liechtenstein)
		case 1078:  // Afrikaans
		case 6154:  // Spanish (Panama)
		case 4103:  // German (Luxembourg)
		case 16394:  // Spanish (Bolivia)
		case 2055:  // German (Switzerland)
		case 1039:  // Icelandic
		case 1057:  // Indonesian
		case 1040:  // Italian (Italy)
		case 2064:  // Italian (Switzerland)
		case 2068:  // Norwegian (Nynorsk)
		case 11274:  // Spanish (Argentina)
		case 1046:  // Portuguese (Brazil)
		case 1044:  // Norwegian (Bokmal)
		case 1086:  // Malay (Malaysia)
		case 2110:  // Malay (Brunei Darussalam)
		case 2070:  // Portuguese (Portugal)
			return 1252;
		case 1032:  // Greek
			return 1253;
		case 1091:  // Uzbek (Latin)
		case 1068:  // Azeri (Latin)
		case 1055:  // Turkish
			return 1254;
		case 1037:  // Hebrew
			return 1255;
		case 5121:  // Arabic (Algeria)
		case 15361:  // Arabic (Bahrain)
		case 9217:  // Arabic (Yemen)
		case 3073:  // Arabic (Egypt)
		case 2049:  // Arabic (Iraq)
		case 11265:  // Arabic (Jordan)
		case 13313:  // Arabic (Kuwait)
		case 12289:  // Arabic (Lebanon)
		case 4097:  // Arabic (Libya)
		case 6145:  // Arabic (Morocco)
		case 8193:  // Arabic (Oman)
		case 16385:  // Arabic (Qatar)
		case 1025:  // Arabic (Saudi Arabia)
		case 10241:  // Arabic (Syria)
		case 14337:  // Arabic (U.A.E.)
		case 1065:  // Farsi
		case 1056:  // Urdu
		case 7169:  // Arabic (Tunisia)
			return 1256;
		case 1061:  // Estonian
		case 1062:  // Latvian
		case 1063:  // Lithuanian
			return 1257;
		case 1066:  // Vietnamese
			return 1258;
	}
	return 65001;   // utf-8
}

namespace
{
	struct SEnvMapper
	{
		HWND hWnd;
		irr::CIrrDeviceWin32* irrDev;
	};
	irr::core::list<SEnvMapper> EnvMap;

	HKL KEYBOARD_INPUT_HKL=0;
	unsigned int KEYBOARD_INPUT_CODEPAGE = 1252;
}

SEnvMapper* getEnvMapperFromHWnd(HWND hWnd)
{
	

	return 0;
}


irr::CIrrDeviceWin32* getDeviceFromHWnd(HWND hWnd)
{
	
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return DefWindowProc(hWnd, message, wParam, lParam);
}


namespace irr
{

//! constructor
CIrrDeviceWin32::CIrrDeviceWin32(const SIrrlichtCreationParameters& params)
: CIrrDeviceStub(params), ChangedToFullScreen(false), Resized(false),
	ExternalWindow(false), Win32CursorControl(0), JoyControl(0), HWnd(0)
{
	

	// get windows version and create OS operator
	//core::stringc winversion;
	//getWindowsVersion(winversion);
//Operator = new COSOperator(winversion);
	//os::Printer::log(winversion.c_str(), ELL_INFORMATION);

	
	
	MouseMultiClicks.DoubleClickTime = GetDoubleClickTime();

	// create driver

	createDriver();

	if (VideoDriver)
		createGUIAndScene();


}


//! destructor
CIrrDeviceWin32::~CIrrDeviceWin32()
{
	
}


//! create the driver
void CIrrDeviceWin32::createDriver()
{
	
		//	switchToFullScreen();
			ContextManager = new video::CEGLManager();
		//	ContextManager->initialize(CreationParams, video::SExposedVideoData(HWnd));

			VideoDriver = video::createOGLES1Driver(CreationParams, FileSystem, ContextManager);
			if (!VideoDriver)
			{
				os::Printer::log("Could not create OpenGL-ES1 driver.", ELL_ERROR);
			}
		
}


//! runs the device. Returns false if device wants to be deleted
bool CIrrDeviceWin32::run()
{
	os::Timer::tick();

	
	return true;
}


//! Pause the current process for the minimum time allowed only to allow other processes to execute
void CIrrDeviceWin32::yield()
{
	Sleep(1);
}

//! Pause execution and let other processes to run for a specified amount of time.
void CIrrDeviceWin32::sleep(u32 timeMs, bool pauseTimer)
{
	
}


void CIrrDeviceWin32::resizeIfNecessary()
{
	
	Resized = false;
}


//! sets the caption of the window
void CIrrDeviceWin32::setWindowCaption(const wchar_t* text)
{
	
			
}


//! presents a surface in the client area
bool CIrrDeviceWin32::present(video::IImage* image, void* windowId, core::rect<s32>* src)
{
	
	return true;
}


//! notifies the device that it should close itself
void CIrrDeviceWin32::closeDevice()
{
	
	Close=true;
}


//! returns if window is active. if not, nothing needs to be drawn
bool CIrrDeviceWin32::isWindowActive() const
{
	
	return true;
}

//! returns if window has focus
bool CIrrDeviceWin32::isWindowFocused() const
{
	
	return true;
}


//! returns if window is minimized
bool CIrrDeviceWin32::isWindowMinimized() const
{
	
	return false;
}


//! switches to fullscreen
bool CIrrDeviceWin32::switchToFullScreen(bool reset)
{
	
	return true;
}


//! returns the win32 cursor control
CIrrDeviceWin32::CCursorControl* CIrrDeviceWin32::getWin32CursorControl()
{
	return Win32CursorControl;
}


//! \return Returns a pointer to a list with all video modes supported
//! by the gfx adapter.
video::IVideoModeList* CIrrDeviceWin32::getVideoModeList()
{

	return 0;
}


void CIrrDeviceWin32::getWindowsVersion(core::stringc& out)
{
	
}

//! Notifies the device, that it has been resized
void CIrrDeviceWin32::OnResized()
{

}

//! Resize the render window.
void CIrrDeviceWin32::setWindowSize(const irr::core::dimension2d<u32>& size)
{
	

}

//! Sets if the window should be resizable in windowed mode.
void CIrrDeviceWin32::setResizable(bool resize)
{

}


//! Minimizes the window.
void CIrrDeviceWin32::minimizeWindow()
{

}


//! Maximizes the window.
void CIrrDeviceWin32::maximizeWindow()
{
	
}


//! Restores the window to its original size.
void CIrrDeviceWin32::restoreWindow()
{

}

core::position2di CIrrDeviceWin32::getWindowPosition()
{
	
		return core::position2di(-1, -1);
	
}

bool CIrrDeviceWin32::activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
{
	
		return false;
}


//! Set the current Gamma Value for the Display
bool CIrrDeviceWin32::setGammaRamp( f32 red, f32 green, f32 blue, f32 brightness, f32 contrast )
{
	

	return true;
}

//! Get the current Gamma Value for the Display
bool CIrrDeviceWin32::getGammaRamp( f32 &red, f32 &green, f32 &blue, f32 &brightness, f32 &contrast )
{
	

	return true;
}


//! Process system events
void CIrrDeviceWin32::handleSystemMessages()
{
	
}


//! Remove all messages pending in the system message loop
void CIrrDeviceWin32::clearSystemMessages()
{
	
}

// shows last error in a messagebox to help internal debugging.
void CIrrDeviceWin32::ReportLastWinApiError()
{
	
}

// Convert an Irrlicht texture to a Windows cursor
// Based on http://www.codeguru.com/cpp/w-p/win32/cursors/article.php/c4529/
HCURSOR CIrrDeviceWin32::TextureToCursor(HWND hwnd, irr::video::ITexture * tex, const core::rect<s32>& sourceRect, const core::position2d<s32> &hotspot)
{
	
	return 0;
}


CIrrDeviceWin32::CCursorControl::CCursorControl(CIrrDeviceWin32* device, const core::dimension2d<u32>& wsize, HWND hwnd, bool fullscreen)
	: Device(device), WindowSize(wsize), InvWindowSize(0.0f, 0.0f),
		HWnd(hwnd), BorderX(0), BorderY(0),
		UseReferenceRect(false), IsVisible(true)
		, ActiveIcon(gui::ECI_NORMAL), ActiveIconStartTime(0)
{
	
}

CIrrDeviceWin32::CCursorControl::~CCursorControl()
{
	
}


void CIrrDeviceWin32::CCursorControl::initCursors()
{
	
}


void CIrrDeviceWin32::CCursorControl::update()
{
	
}

//! Sets the active cursor icon
void CIrrDeviceWin32::CCursorControl::setActiveIcon(gui::ECURSOR_ICON iconId)
{
	
}


//! Add a custom sprite as cursor icon.
gui::ECURSOR_ICON CIrrDeviceWin32::CCursorControl::addIcon(const gui::SCursorSprite& icon)
{
	
	return gui::ECI_NORMAL;
}


//! replace the given cursor icon.
void CIrrDeviceWin32::CCursorControl::changeIcon(gui::ECURSOR_ICON iconId, const gui::SCursorSprite& icon)
{
	
	
}


//! Return a system-specific size which is supported for cursors. Larger icons will fail, smaller icons might work.
core::dimension2di CIrrDeviceWin32::CCursorControl::getSupportedIconSize() const
{
	core::dimension2di result;

	result.Width = GetSystemMetrics(SM_CXCURSOR);
	result.Height = GetSystemMetrics(SM_CYCURSOR);

	return result;
}



} // end namespace

#endif // _IRR_COMPILE_WITH_WINDOWS_DEVICE_
