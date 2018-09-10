#pragma once

#include "../../DataStructures/String.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eunoia { namespace Rendering {

	class Win32Display
	{
	public:
		Win32Display(const String& title, uint32 width, uint32 height);

		void Update() const;

		bool ShouldClose() const;
		uint32 GetWidth() const;
		uint32 GetHeight() const;
		String GetTitle() const;

		void SetClose();

		HWND GetWindowHandle() const;

		static Win32Display* GetDisplay();
	private:
		bool InitRawInputs() const;
	private:
		static Win32Display* s_pActiveDisplay;

		HWND m_windowHandle;

		String m_title;
		uint32 m_width;
		uint32 m_height;

		bool m_shouldClose;
	};

} }
