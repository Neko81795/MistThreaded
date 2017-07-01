#pragma once
#pragma warning(push, 0)
#include <atlbase.h>
#include <atlwin.h>
#pragma warning(pop)

namespace Graphics
{
	//message, wParam, lParam, handled
	typedef LRESULT(*WinFunc)(UINT, WPARAM, LPARAM, BOOL&);

	class Window : public CWindowImpl<Window, CWindow, CWinTraits<WS_OVERLAPPEDWINDOW | WS_VISIBLE>>
	{
	public:
		DECLARE_WND_CLASS_EX(L"PCGraphicsWindow", CS_VREDRAW | CS_HREDRAW, -1)
		unsigned int Width;
		unsigned int Height;
		HWND Handle;
		bool PostQuitWhenDestroyed;
		WinFunc OnKeyDown;
		WinFunc OnKeyUp;
		WinFunc OnSystemKeyDown;
		WinFunc OnSystemKeyUp;
		WinFunc OnMouseScroll;
		WinFunc OnMouseButton;
		WinFunc OnMouseMove;

		Window(const wchar_t* name, bool postQuitWhenDestroyed = true);
		~Window();

	private:
		BEGIN_MSG_MAP(Window)
			MESSAGE_HANDLER(WM_PAINT, Paint)
			MESSAGE_HANDLER(WM_DESTROY, Destroy)
			MESSAGE_HANDLER(WM_SIZE, Resize)
			MESSAGE_HANDLER(WM_EXITSIZEMOVE, ResizeExit)
			MESSAGE_HANDLER(WM_DISPLAYCHANGE, DisplayChange)
			MESSAGE_HANDLER(WM_KEYDOWN, KeyDown)
			MESSAGE_HANDLER(WM_KEYUP, KeyUp)
			MESSAGE_HANDLER(WM_SYSKEYDOWN, SysKeyDown)
			MESSAGE_HANDLER(WM_SYSKEYUP, SysKeyUp)
			MESSAGE_HANDLER(WM_MOUSEWHEEL, Scroll)
			MESSAGE_HANDLER(WM_RBUTTONDOWN, MouseButton)
			MESSAGE_HANDLER(WM_LBUTTONDOWN, MouseButton)
			MESSAGE_HANDLER(WM_MBUTTONDOWN, MouseButton)
			MESSAGE_HANDLER(WM_XBUTTONDOWN, MouseButton)
			MESSAGE_HANDLER(WM_RBUTTONUP, MouseButton)
			MESSAGE_HANDLER(WM_LBUTTONUP, MouseButton)
			MESSAGE_HANDLER(WM_MBUTTONUP, MouseButton)
			MESSAGE_HANDLER(WM_XBUTTONUP, MouseButton)
			MESSAGE_HANDLER(WM_MOUSEMOVE, MouseMove)
		END_MSG_MAP()

		void(*onResize)();

		LRESULT Paint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT Destroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT Resize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT DisplayChange(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT ResizeExit(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

		LRESULT KeyDown(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT KeyUp(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT SysKeyDown(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT SysKeyUp(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT Scroll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT MouseButton(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
		LRESULT MouseMove(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
	};
}