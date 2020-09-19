#pragma once

#include "Win.h"

#define NPE_MAINWINDOW_CLASS_NAME L"MAINWINDOWCLASS"


namespace GUI
{
    template <class DERIVED_TYPE>
    class BaseWindow
    {
    public:
        /**
        * Getter for native HWND
        */
        HWND GetNativeWindow() const { return m_hWnd; }

    protected:
        /**
        * Window procedure which calls the DERIVED_TYPE::HandleMessage function
        * 
        * @param hwnd is the hwnd
        * @param uMsg is the window message
        * @param wParam is the wParam
        * @param lParam is the lParam
        * 
        * @returns handled status
        */
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            DERIVED_TYPE* pThis = nullptr;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->m_hWnd = hwnd;
            }
            else
            {
                pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }
            if (pThis)
            {
                return pThis->HandleMessage(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        /**
        * Contructor for BaseWindow
        */
        BaseWindow() : m_hWnd(NULL) { }

        /**
        * Creates the native window
        * 
        * @param lpWindowName is the title of the window
        * @param dwStyle are style flags for the window
        * @returns TRUE if the window was created successfully, false otherwise
        */
        BOOL CreateNativeWindow(
            PCWSTR lpWindowName,
            DWORD dwStyle,
            DWORD dwExStyle = 0,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int nWidth = CW_USEDEFAULT,
            int nHeight = CW_USEDEFAULT,
            HWND hWndParent = 0,
            HMENU hMenu = 0
        )
        {
            WNDCLASS wc = { 0 };

            wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = NPE_MAINWINDOW_CLASS_NAME;

            RegisterClass(&wc);

            m_hWnd = CreateWindowEx(
                dwExStyle, NPE_MAINWINDOW_CLASS_NAME, lpWindowName, dwStyle, x, y,
                nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
            );

            return (m_hWnd ? TRUE : FALSE);
        }

        /**
        * Handles all window messages
        * 
        * @param uMsg is the window message
        * @param wParam is the wParam
        * @param lParam is the lParam
        */
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    protected:
        HWND m_hWnd;
    };
}

