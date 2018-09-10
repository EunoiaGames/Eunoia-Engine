#include "Win32Display.h"


namespace Eunoia { namespace Rendering {

	Win32Display* Win32Display::s_pActiveDisplay = NULL;

	LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	void handle_input(WPARAM wparam, LPARAM lparam);

	Win32Display::Win32Display(const String & title, uint32 width, uint32 height) :
		m_title(title),
		m_width(width),
		m_height(height),
		m_shouldClose(false)
	{
		WNDCLASS wnd_class = { 0 };
		wnd_class.style = CS_OWNDC;
		wnd_class.lpfnWndProc = WinProc;
		wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd_class.lpszClassName = "eunoia";
		RegisterClass(&wnd_class);

		uint32 monitorWidth = GetSystemMetrics(SM_CXSCREEN);
		uint32 monitorHeight = GetSystemMetrics(SM_CYSCREEN);

		uint32 xpos = (monitorWidth - m_width) / 2;
		uint32 ypos = (monitorHeight - m_height) / 2;

		int flags = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
		m_windowHandle = CreateWindow(wnd_class.lpszClassName, m_title.C_Str(), flags, xpos, ypos, m_width, m_height, NULL, NULL, NULL, NULL);

		InitRawInputs();

		PIXELFORMATDESCRIPTOR pixel_format {};

		pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

#ifdef EU_RENDER_API_OPENGL
		pixel_format.dwFlags |= PFD_SUPPORT_OPENGL;
#endif

		pixel_format.nVersion = 1;
		pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixel_format.cColorBits = 32;
		pixel_format.cRedBits = 8;
		pixel_format.cGreenBits = 8;
		pixel_format.cBlueBits = 8;
		pixel_format.cAlphaBits = 8;
		pixel_format.cDepthBits = 24;
		pixel_format.cStencilBits = 8;
		pixel_format.iPixelType = PFD_TYPE_RGBA;
		pixel_format.cAuxBuffers = 0;
		pixel_format.iLayerType = PFD_MAIN_PLANE;

		HDC hdc = GetDC(m_windowHandle);
		int pixelFormat = ChoosePixelFormat(hdc, &pixel_format);

		if (pixelFormat)
		{
			if (!SetPixelFormat(hdc, pixelFormat, &pixel_format))
			{
				std::cerr << "Could not set pixel format for win32" << std::endl;
				return;
			}
		}
		else
		{
			std::cerr << "Could not set pixel format for win32" << std::endl;
			return;
		}

		s_pActiveDisplay = this;
		//Display Input
	}

	void Win32Display::Update() const
	{
		MSG msg = { 0 };
		while (PeekMessage(&msg, m_windowHandle, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool Win32Display::ShouldClose() const
	{
		return m_shouldClose;
	}

	uint32 Win32Display::GetWidth() const
	{
		return m_width;
	}

	uint32 Win32Display::GetHeight() const
	{
		return m_height;
	}

	String Win32Display::GetTitle() const
	{
		return m_title;
	}

	void Win32Display::SetClose()
	{
		m_shouldClose = true;
	}

	HWND Win32Display::GetWindowHandle() const
	{
		return m_windowHandle;
	}

	Win32Display * Win32Display::GetDisplay()
	{
		return s_pActiveDisplay;
	}

	bool Win32Display::InitRawInputs() const
	{
		RAWINPUTDEVICE raw_input_keyboard_device{};

		raw_input_keyboard_device.dwFlags = RIDEV_NOLEGACY;
		raw_input_keyboard_device.usUsagePage = 0x01;
		raw_input_keyboard_device.usUsage = 0x06;
		raw_input_keyboard_device.hwndTarget = m_windowHandle;

		RAWINPUTDEVICE raw_input_mouse_device{};

		raw_input_mouse_device.dwFlags = 0;
		raw_input_mouse_device.usUsagePage = 0x01;
		raw_input_mouse_device.usUsage = 0x02;
		raw_input_mouse_device.hwndTarget = m_windowHandle;

		RAWINPUTDEVICE raw_input_device_list[] = { raw_input_mouse_device, raw_input_keyboard_device };

		return RegisterRawInputDevices(raw_input_device_list, 2, sizeof(RAWINPUTDEVICE)) == TRUE;
	}

	void handle_input(WPARAM wparam, LPARAM lparam)
	{
		UINT size;
		LPBYTE bytes;

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

		bytes = new BYTE[size];
		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, bytes, &size, sizeof(RAWINPUTHEADER));

		RAWINPUT* pInput = (RAWINPUT*)bytes;

		if (pInput->header.dwType == RIM_TYPEKEYBOARD)
		{
			if (pInput->data.keyboard.Message == WM_KEYDOWN)
			{
				USHORT key = pInput->data.keyboard.MakeCode;
				//Display::GetDisplay()->GetInput()->SetKeyState(key, true);
			}
			else if (pInput->data.keyboard.Message == WM_KEYUP)
			{
				USHORT key = pInput->data.keyboard.MakeCode;
				//Display::GetDisplay()->GetInput()->SetKeyState(key, false);
			}
		}
		else if (pInput->header.dwType == RIM_TYPEMOUSE)
		{
			const RAWMOUSE& mouse = pInput->data.mouse;

			USHORT wheel = mouse.usButtonData;
			ULONG button = mouse.ulButtons;

			unsigned int engineButton = 0;
			int buttonStatus = -1;

			switch (button)
			{
			case RI_MOUSE_BUTTON_1_DOWN:	engineButton = 0; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_1_UP:		engineButton = 0; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_2_DOWN:	engineButton = 1; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_2_UP:		engineButton = 1; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_3_DOWN:	engineButton = 2; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_3_UP:		engineButton = 2; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_4_DOWN:	engineButton = 3; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_4_UP:		engineButton = 3; buttonStatus = false; break;
			case RI_MOUSE_BUTTON_5_DOWN:	engineButton = 4; buttonStatus = true; break;
			case RI_MOUSE_BUTTON_5_UP:		engineButton = 4; buttonStatus = false; break;
			case RI_MOUSE_WHEEL:			engineButton = 5; buttonStatus = true; break;
			}

			if (buttonStatus != -1)
			{
				//Display::GetDisplay()->GetInput()->SetMouseButtonState(engineButton, buttonStatus);
			}
		}

		delete[] bytes;
	}

	LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_DESTROY: Win32Display::GetDisplay()->SetClose(); break;
		case WM_CLOSE: Win32Display::GetDisplay()->SetClose(); break;
		case WM_INPUT: handle_input(wparam, lparam); break;
		}

		return DefWindowProc(handle, msg, wparam, lparam);
	}

} }
