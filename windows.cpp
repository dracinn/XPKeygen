﻿//
// Created by Andrew on 10/04/2023.
//

#include <windows.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <objidl.h>
#include <gdiplus.h>

#include "header.h"
#include "resource.h"

HWND hMainWindow;

const WCHAR *pAboutLink = L"https://github.com/Endermanch/XPKeygen",
            *pWebsite = L"https://malwarewatch.org",
            *pVersion = L"2.3",
            *pTitle = L"Windows XP Pro SP3 // Server 2003 SP0 x86 VLK - Enderman[ch]",
            *pGroupTitle = L"Windows XP Pro SP3 // Server 2003 SP0 x86 VLK";

bool bServer = false,
     bMusic = true;

/* Bitmap link processor. */
LRESULT BitmapLinkProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    static TRACKMOUSEEVENT  trackMouse;
    static BOOL             isSet = FALSE;

    // The worst part here was to avoid pointer flickering, as well as spamming the SetCursor function.
    switch (uMsg) {

        // Set the struct up outside of a frequently executed message to speed things up.
        case WM_APP + 0x69: {
            trackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
            trackMouse.dwFlags = TME_LEAVE;
            trackMouse.dwHoverTime = HOVER_DEFAULT;
            trackMouse.hwndTrack = hWindow;

            break;
        }

        // You have to turn off the WM_SETCURSOR, because it spams the SetCursor function with IDC_ARROW in the DefWndProc. Moist garbage.
        case WM_SETCURSOR: break;

        // Set cursor to hand if it's inside of static area, refresh the mouse tracking loop.
        case WM_MOUSEMOVE: {
            if (!isSet) {
                TrackMouseEvent(&trackMouse);
                SetCursor(LoadCursorW(nullptr, IDC_HAND));

                isSet = TRUE;
            }

            break;
        }

        // Set cursor back to normal if it's outside of static area.
        case WM_MOUSELEAVE: {
            if (isSet) {
                SetCursor(LoadCursorW(nullptr, IDC_ARROW));
                isSet = FALSE;
            }

            break;
        }

        // Remove the subclass before window closes.
        case WM_NCDESTROY: {
            RemoveWindowSubclass(hWindow, (SUBCLASSPROC)BitmapLinkProc, uIdSubclass);

            break;
        }

        // Pass everything else to DefWndProc.
        default: return DefSubclassProc(hWindow, uMsg, wParam, lParam);
    }

    return 0;
}

/* Static link processor. */
LRESULT StaticLinkProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    static TRACKMOUSEEVENT  trackMouse;
    static BOOL             isSet = FALSE;

    // The worst part here was to avoid pointer flickering, as well as spamming the SetCursor function.
    switch (uMsg) {

        // Set the struct up outside of a frequently executed message to speed things up.
        case WM_APP + 0x69: {
            trackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
            trackMouse.dwFlags = TME_LEAVE;
            trackMouse.dwHoverTime = HOVER_DEFAULT;
            trackMouse.hwndTrack = hWindow;

            break;
        }

        // You have to turn off the WM_SETCURSOR, because it spams the SetCursor function with IDC_ARROW in the DefWndProc. Moist garbage.
        case WM_SETCURSOR: break;

        // Set cursor to hand if it's inside of static area, refresh the mouse tracking loop.
        case WM_MOUSEMOVE: {
            if (!isSet) {
                TrackMouseEvent(&trackMouse);
                SetCursor(LoadCursorW(nullptr, IDC_HAND));

                isSet = TRUE;
            }

            break;
        }

        // Set cursor back to normal if it's outside of static area.
        case WM_MOUSELEAVE: {
            if (isSet) {
                SetCursor(LoadCursorW(nullptr, IDC_ARROW));
                isSet = FALSE;
            }

            break;
        }

        // Remove the subclass before window closes.
        case WM_NCDESTROY: {
            RemoveWindowSubclass(hWindow, (SUBCLASSPROC)StaticLinkProc, uIdSubclass);

            break;
        }

        // Pass everything else to DefWndProc.
        default: return DefSubclassProc(hWindow, uMsg, wParam, lParam);
    }

    return 0;
}

/* Main window processor. */
LRESULT CALLBACK WNDProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    static HINSTANCE hInstance;

    static HBRUSH   hBGColorPrim, hBGColorSec, hFGColor, hBtnDefault,
                    hBtn1Select, hBtn1Hot,
                    hBtn2Select, hBtn2Hot,
                    hBtn3Select, hBtn3Hot,
                    hBtn4Select, hBtn4Hot;

    static HPEN     hFrameColor, hFramePrim, hBtnDefaultP,
                    hBtn1SelectP, hBtn1HotP,
                    hBtn2SelectP, hBtn2HotP,
                    hBtn3SelectP, hBtn3HotP,
                    hBtn4SelectP, hBtn4HotP;

    static HDC      hMainDC;

    static HBITMAP  hBMusicOn, hBMusicOff;

    switch (uMessage) {
    case WM_CREATE:
        bMusic = true;

        hInstance = ((LPCREATESTRUCT)(lParam))->hInstance;
        hMainDC = GetDC(hWindow);

        playAudio(hInstance, MAKEINTRESOURCEW(IDR_WAVE1), SND_ASYNC | SND_LOOP | SND_NODEFAULT);

        hBMusicOn = (HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, 0);
        hBMusicOff = (HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, 0);

        hFrameColor = CreatePen(PS_SOLID, 1, RGB(240, 240, 240));
        hFramePrim = CreatePen(PS_SOLID, 1, RGB(10, 10, 10));

        hBGColorPrim = CreateSolidBrush(RGB(10, 10, 10));
        hBGColorSec = (HBRUSH)(GetStockObject(BLACK_BRUSH));
        hFGColor = (HBRUSH)GetStockObject(WHITE_BRUSH);

        hBtnDefaultP = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

        hBtn1SelectP = CreatePen(PS_SOLID, 1, RGB(160, 160, 0));
        hBtn1HotP = CreatePen(PS_SOLID, 1, RGB(80, 80, 0));
        
        hBtn2SelectP = CreatePen(PS_SOLID, 1, RGB(0, 0, 160));
        hBtn2HotP = CreatePen(PS_SOLID, 1, RGB(0, 0, 80));

        hBtn3SelectP = CreatePen(PS_SOLID, 1, RGB(160, 0, 0));
        hBtn3HotP = CreatePen(PS_SOLID, 1, RGB(80, 0, 0));

        hBtn4SelectP = CreatePen(PS_SOLID, 1, RGB(0, 160, 0));
        hBtn4HotP = CreatePen(PS_SOLID, 1, RGB(0, 80, 0));

        hBtnDefault = CreateSolidBrush(RGB(30, 30, 30));

        hBtn1Select = CreateSolidBrush(RGB(70, 70, 30));
        hBtn1Hot = CreateSolidBrush(RGB(40, 40, 30));

        hBtn2Select = CreateSolidBrush(RGB(30, 30, 70));
        hBtn2Hot = CreateSolidBrush(RGB(30, 30, 40));

        hBtn3Select = CreateSolidBrush(RGB(70, 30, 30));
        hBtn3Hot = CreateSolidBrush(RGB(40, 30, 30));

        hBtn4Hot = CreateSolidBrush(RGB(30, 40, 30));
        hBtn4Select = CreateSolidBrush(RGB(30, 70, 30));

        break;

    case WM_PAINT: {
        SelectObject(hMainDC, hFrameColor);
        SelectObject(hMainDC, GetStockObject(HOLLOW_BRUSH));

        RoundRect(hMainDC, 10, 165, 589, 430, 12, 12);
        InvalidateRect(GetDlgItem(hWindow, IDC_LABEL1), nullptr, true);

        goto execute;
    }

    case WM_CTLCOLORSTATIC:
        SetBkMode((HDC)wParam, TRANSPARENT);

        if ((HWND)lParam == GetDlgItem(hWindow, IDC_EDIT1)) {
            if (bServer)
                SetTextColor((HDC)wParam, RGB(30, 255, 30));
            else
                SetTextColor((HDC)wParam, RGB(255, 255, 0));

            return (LRESULT)(hBGColorSec);
        }
        else if ((HWND)lParam == GetDlgItem(hWindow, IDC_LABEL4)) {
            SetTextColor((HDC)wParam, RGB(140, 140, 255));
            return (LRESULT)(hBGColorPrim);
        }
        else if ((HWND)lParam == GetDlgItem(hWindow, IDC_LABEL5)) {
            SetTextColor((HDC)wParam, RGB(255, 140, 140));
            return (LRESULT)(hBGColorPrim);
        }
        else {
            SetTextColor((HDC)wParam, RGB(255, 255, 255));
            return (LRESULT)(hBGColorPrim);
        }
        
        break;

    case WM_CTLCOLOREDIT:
        SetBkMode((HDC)wParam, TRANSPARENT);

        if ((HWND)lParam == GetDlgItem(hWindow, IDC_INPUT1)) {
            SetTextColor((HDC)wParam, RGB(255, 140, 140));
        }
        else if ((HWND)lParam == GetDlgItem(hWindow, IDC_INPUT2)) {
            SetTextColor((HDC)wParam, RGB(140, 140, 255));
        }
        else goto execute;

        return (LRESULT)(hBGColorSec);

    case WM_NOTIFY: {
        LPNMHDR nmHeader = (LPNMHDR)lParam;

        if (nmHeader->code == NM_CUSTOMDRAW) {
            LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmHeader;

            HBRUSH hBtnXSelect, hBtnXHot;
            HPEN hBtnXSelectP, hBtnXHotP;
            
            CONST WCHAR *pCaption = L"Unknown";
            
            switch (nmHeader->idFrom) {
                case IDC_BUTTON1:
                    pCaption = L"About";

                    hBtnXSelect = hBtn1Select;
                    hBtnXHot = hBtn1Hot;
                    hBtnXSelectP = hBtn1SelectP;
                    hBtnXHotP = hBtn1HotP;

                    break;
                
                case IDC_BUTTON2:
                    pCaption = L"> Generate <";

                    hBtnXSelect = hBtn2Select;
                    hBtnXHot = hBtn2Hot;
                    hBtnXSelectP = hBtn2SelectP;
                    hBtnXHotP = hBtn2HotP;

                    break;

                case IDC_BUTTON3:
                    pCaption = L"Quit";

                    hBtnXSelect = hBtn3Select;
                    hBtnXHot = hBtn3Hot;
                    hBtnXSelectP = hBtn3SelectP;
                    hBtnXHotP = hBtn3HotP;

                    break;

                case IDC_BUTTON4:
                    pCaption = L"Random";

                    hBtnXSelect = hBtn4Select;
                    hBtnXHot = hBtn4Hot;
                    hBtnXSelectP = hBtn4SelectP;
                    hBtnXHotP = hBtn4HotP;

                    break;

                default:
                    return CDRF_DODEFAULT;
            }

            SetBkMode(item->hdc, TRANSPARENT);
            SetTextColor(item->hdc, RGB(255, 255, 255));

            SelectObject(item->hdc, hFramePrim);
            SelectObject(item->hdc, hBGColorPrim);

            Rectangle(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom);

            if (item->uItemState & CDIS_SELECTED) {
                SelectObject(item->hdc, hBtnXSelectP);
                SelectObject(item->hdc, hBtnXSelect);
            }
            else if (item->uItemState & CDIS_HOT) {
                SelectObject(item->hdc, hBtnXHotP);
                SelectObject(item->hdc, hBtnXHot);
            }
            else {
                SelectObject(item->hdc, hBtnDefaultP);
                SelectObject(item->hdc, hBtnDefault);
            }

            RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 12, 12);
            DrawTextW(item->hdc, pCaption, -1, &item->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            return CDRF_SKIPDEFAULT;
        }
        
        return CDRF_DODEFAULT;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case IDC_IMAGE2: {
                switch (HIWORD(wParam)) {
                    case STN_CLICKED:
                        if (bMusic) {
                            SendMessageW((HWND)lParam, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMusicOff);
                            stopAudio();

                            bMusic = false;
                        }
                        else {
                            SendMessageW((HWND)lParam, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMusicOn);
                            playAudio(hInstance, MAKEINTRESOURCEW(IDR_WAVE1), SND_ASYNC | SND_LOOP | SND_NODEFAULT);

                            bMusic = true;
                        }
                        break;
                    }

                break;
            }

            case IDC_LABEL4: {
                switch (HIWORD(wParam)) {
                    case STN_CLICKED:
                        ShellExecuteW(hWindow, L"open", pWebsite, nullptr, nullptr, SW_SHOWNORMAL);

                        break;
                }

                break;
            }

            case IDC_LABEL2: {
                switch (HIWORD(wParam)) {
                    case STN_CLICKED:
                        CheckRadioButton(hWindow, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

                        break;
                }

                __fallthrough;
            }

            case IDC_RADIO1: 
                switch (HIWORD(wParam)) {
                    case BN_CLICKED:
                        EnableWindow(GetDlgItem(hMainWindow, IDC_BUTTON4), true);

                        EnableWindow(GetDlgItem(hMainWindow, IDC_INPUT1), true);
                        EnableWindow(GetDlgItem(hMainWindow, IDC_INPUT2), true);

                        bServer = false;

                        break;
                }

                break;

            case IDC_LABEL3: {
                switch (HIWORD(wParam)) {
                    case STN_CLICKED:
                        CheckRadioButton(hWindow, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);

                        break;
                }

                __fallthrough;
            }

            case IDC_RADIO2:
                switch (HIWORD(wParam)) {
                case BN_CLICKED:
                    EnableWindow(GetDlgItem(hMainWindow, IDC_BUTTON4), false);

                    EnableWindow(GetDlgItem(hMainWindow, IDC_INPUT1), false);
                    EnableWindow(GetDlgItem(hMainWindow, IDC_INPUT2), false);

                    bServer = true;

                    break;
                }

                break;

            case IDC_BUTTON1: {
                ShellExecuteW(hWindow, L"open", pAboutLink, nullptr, nullptr, SW_SHOWNORMAL); 
                
                break;
            }

            case IDC_BUTTON2: {
                WCHAR   *pText = (WCHAR *)calloc(512, sizeof(WCHAR));
                HWND    hEdit = GetDlgItem(hMainWindow, IDC_EDIT1);

                if (bServer) {
                    formatServer(pText);
                }
                else {
                    WCHAR   pBSection[4]{}, pCSection[8]{};
                    HWND    hInput1 = GetDlgItem(hMainWindow, IDC_INPUT1),
                            hInput2 = GetDlgItem(hMainWindow, IDC_INPUT2);

                    SendMessageW(hInput1, WM_GETTEXT, 3 + NULL_TERMINATOR, (LPARAM)pBSection);
                    SendMessageW(hInput2, WM_GETTEXT, 6 + NULL_TERMINATOR, (LPARAM)pCSection);

                    formatXP(pBSection, pCSection, pText);
                }

                SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)pText);
                free(pText);

                return 0;
            }

            case IDC_BUTTON3: {
                DestroyWindow(hWindow);

                return 0;
            }

            case IDC_BUTTON4: {
                ul32 msDigits = randomRange(0, 999),
                     lsDigits = randomRange(0, 999'999);

                WCHAR pBSection[4]{}, pCSection[8]{};

                wsprintfW(pBSection, L"%03d", msDigits);
                wsprintfW(pCSection, L"%06d", lsDigits);

                SendMessageW(GetDlgItem(hMainWindow, IDC_INPUT1), WM_SETTEXT, 0, (LPARAM)pBSection);
                SendMessageW(GetDlgItem(hMainWindow, IDC_INPUT2), WM_SETTEXT, 0, (LPARAM)pCSection);

                break;
            }
        }
        
        
        break;

    case WM_CLOSE:
        DestroyWindow(hWindow);

        break;

    case WM_DESTROY:
        DeleteObject(hBGColorPrim);

        DeleteObject(hBtn1Select);
        DeleteObject(hBtn1Hot);
        DeleteObject(hBtn2Select);
        DeleteObject(hBtn2Hot);
        DeleteObject(hBtn3Select);
        DeleteObject(hBtn3Hot);
        DeleteObject(hBtn4Select);
        DeleteObject(hBtn4Hot);

        DeleteObject(hBtnDefault);

        DeleteObject(hBtn1SelectP);
        DeleteObject(hBtn1HotP);
        DeleteObject(hBtn2SelectP);
        DeleteObject(hBtn2HotP);
        DeleteObject(hBtn3SelectP);
        DeleteObject(hBtn3HotP);
        DeleteObject(hBtn4SelectP);
        DeleteObject(hBtn4HotP);

        DeleteObject(hBtnDefaultP);

        DeleteObject(hFrameColor);
        DeleteObject(hFramePrim);

        ReleaseDC(hWindow, hMainDC);

        PostQuitMessage(0);
        break;

    execute:
    default:
        return DefWindowProc(hWindow, uMessage, wParam, lParam);
    }

    return 0;
}

/* Initialize system fonts. */
void InitializeFonts(HFONT *hLabelFont, HFONT *hSmolFont, HFONT *hBoldFont, HFONT *hCaptionFont) {
    NONCLIENTMETRICSW nonClientMetrics;

    // Get information about the default system font.
    nonClientMetrics.cbSize = sizeof(NONCLIENTMETRICSW);
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &nonClientMetrics, 0);

    ULONG defaultHeight = nonClientMetrics.lfMessageFont.lfHeight;

    // Create default font.
    *hLabelFont = CreateFontIndirectW(&nonClientMetrics.lfMessageFont);

    // Create smol font.
    nonClientMetrics.lfMessageFont.lfHeight = 12;
    *hSmolFont = CreateFontIndirectW(&nonClientMetrics.lfMessageFont);

    // Create bold font.
    nonClientMetrics.lfMessageFont.lfWeight = 700;
    nonClientMetrics.lfMessageFont.lfHeight = defaultHeight;
    *hBoldFont = CreateFontIndirectW(&nonClientMetrics.lfMessageFont);

    // Create caption font.
    nonClientMetrics.lfMessageFont.lfHeight = 30;
    *hCaptionFont = CreateFontIndirectW(&nonClientMetrics.lfMessageFont);
}

/* Initialize main window. */
bool InitializeWindow(HINSTANCE hInstance) {
    HFONT   hLabelFont,
            hSmolFont,
            hCaptionFont,
            hBoldFont;

    WNDCLASSEX wndClass{};

    // Register the main window class.
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = (WNDPROC)WNDProc;
    wndClass.lpszClassName = L"XPKeygen";
    wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(10, 10, 10));
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.hIcon = LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_ICON1));
    wndClass.hIconSm = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON1));
    wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    if (!RegisterClassExW(&wndClass))
        return false;

    InitializeFonts(&hLabelFont, &hSmolFont, &hBoldFont, &hCaptionFont);

    const int   w = 615,
                h = 495,
                x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2,
                y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    hMainWindow = CreateWindowExW(
            0,
            L"XPKeygen",
            pTitle,
            WS_SYSMENU,
            x, y,
            w, h,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    HDC hMainDC = GetDC(hMainWindow);

    // Select the default font.
    SelectObject(hMainDC, hLabelFont);

    HBITMAP hBLogo = (HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, 0);

    HWND hLogo = CreateWindowExW(
        0,
        L"Static", nullptr,
        WS_CHILD | WS_VISIBLE |
        SS_BITMAP | SS_REALSIZEIMAGE,
        0, 0,
        600, 0,
        hMainWindow, (HMENU)IDC_IMAGE1,
        hInstance, nullptr
    );

    SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBLogo);

    HBITMAP hBMusicOn = (HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, 0),
            hBMusicOff = (HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, 0);

    HWND hMusic = CreateWindowExW(
        0,
        L"Static", nullptr,
        WS_CHILD | WS_VISIBLE |
        SS_BITMAP | SS_REALSIZECONTROL |
        SS_NOTIFY,
        w - 67, 180,
        32, 32,
        hMainWindow, (HMENU)IDC_IMAGE2,
        hInstance, nullptr
    );

    SendMessageW(hMusic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMusicOn);

    SetWindowSubclass(hMusic, (SUBCLASSPROC)BitmapLinkProc, IDC_IMAGE2, 0);
    SendMessageW(hMusic, WM_APP + 0x69, 0, 0);

    HWND hGroupBox = CreateWindowExW(
        0,
        L"Static", pGroupTitle,
        WS_CHILD | WS_VISIBLE |
        SS_CENTER,
        42, 150,
        515, 32,
        hMainWindow, (HMENU)IDC_LABEL1,
        hInstance, nullptr
    );

    SendMessageW(hGroupBox, WM_SETFONT, (WPARAM)hCaptionFont, 0);

    HWND hRPKLabel = CreateWindowExW(
        0,
        L"Static", L"Raw Product Key:",
        WS_CHILD | WS_VISIBLE,
        20, 190,
        100, 16,
        hMainWindow, nullptr,
        hInstance, nullptr
    );

    SendMessageW(hRPKLabel, WM_SETFONT, (WPARAM)hBoldFont, 0);

    HWND hInput1 = CreateWindowExW(
        0,
        L"Edit",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP |
        ES_LEFT | ES_NUMBER,
        130, 189,
        40, 20,
        hMainWindow,
        (HMENU)IDC_INPUT1,
        hInstance,
        nullptr
    );

    SendMessageW(hInput1, EM_SETCUEBANNER, 0, (LPARAM)L"BBB");
    SendMessageW(hInput1, WM_SETTEXT, 0, (LPARAM)L"640");
    SendMessageW(hInput1, WM_SETFONT, (WPARAM)hLabelFont, 0);

    SendMessageW(hInput1, EM_SETLIMITTEXT, (WPARAM)3, 0);

    HWND hRPKDash = CreateWindowExW(
        0,
        L"Static", L"-",
        WS_CHILD | WS_VISIBLE,
        173, 190,
        10, 16,
        hMainWindow, nullptr,
        hInstance, nullptr
    );

    SendMessageW(hRPKDash, WM_SETFONT, (WPARAM)hBoldFont, 0);

    HWND hInput2 = CreateWindowExW(
        0,
        L"Edit",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP |
        ES_LEFT | ES_NUMBER,
        181, 189,
        70, 20,
        hMainWindow,
        (HMENU)IDC_INPUT2,
        hInstance,
        nullptr
    );

    SendMessageW(hInput2, EM_SETCUEBANNER, 0, (LPARAM)L"CCCCCC");
    SendMessageW(hInput2, WM_SETTEXT, 0, (LPARAM)L"883400");
    SendMessageW(hInput2, WM_SETFONT, (WPARAM)hLabelFont, 0);

    SendMessageW(hInput2, EM_SETLIMITTEXT, (WPARAM)6, 0);

    HWND hRandomize = CreateWindowExW(
        0,
        L"Button",
        L"Randomize",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        260, 188,
        75, 22,
        hMainWindow,
        (HMENU)IDC_BUTTON4,
        hInstance,
        nullptr
    );

    SendMessageW(hRandomize, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hVersionLabel = CreateWindowExW(
        0,
        L"Static", L"Version:",
        WS_CHILD | WS_VISIBLE,
        20, 220,
        100, 16,
        hMainWindow, nullptr,
        hInstance, nullptr
    );

    SendMessageW(hVersionLabel, WM_SETFONT, (WPARAM)hBoldFont, 0);

    HWND hRadio1 = CreateWindowExW(
        WS_EX_WINDOWEDGE,
        L"Button",
        L"",
        WS_VISIBLE | WS_CHILD | WS_GROUP | WS_TABSTOP |
        BS_AUTORADIOBUTTON,
        70, 219,
        17, 20,
        hMainWindow,
        (HMENU)IDC_RADIO1,
        hInstance, nullptr
    );

    SendMessageW(hRadio1, BM_SETCHECK, 1, 0);
    SendMessageW(hRadio1, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hRadioLabel1 = CreateWindowExW(
        0,
        L"Static",
        L"Windows XP VLK",
        WS_CHILD | WS_VISIBLE |
        SS_NOTIFY,
        89, 221,
        90, 16,
        hMainWindow, (HMENU)IDC_LABEL2,
        hInstance, nullptr
    );

    SendMessageW(hRadioLabel1, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hRadio2 = CreateWindowExW(
        WS_EX_WINDOWEDGE,
        L"Button",
        L"",
        WS_VISIBLE | WS_CHILD | WS_TABSTOP |
        BS_AUTORADIOBUTTON,
        200, 219,
        17, 20,
        hMainWindow,
        (HMENU)IDC_RADIO2,
        hInstance, nullptr);

    SendMessageW(hRadio2, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hRadioLabel2 = CreateWindowExW(
        0,
        L"Static",
        L"Windows Server 2003 VLK",
        WS_CHILD | WS_VISIBLE |
        SS_NOTIFY,
        218, 221,
        142, 16,
        hMainWindow, (HMENU)IDC_LABEL3,
        hInstance, nullptr
    );

    SendMessageW(hRadioLabel2, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hEdit = CreateWindowExW(
        0,
        L"Edit",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | 
        ES_MULTILINE | ES_READONLY |
        ES_LEFT | ES_UPPERCASE,
        20, 250,
        w - 57, h - 360,
        hMainWindow,
        (HMENU)IDC_EDIT1,
        hInstance,
        nullptr
    );

    SendMessageW(hEdit, WM_SETFONT, (WPARAM)hBoldFont, 0);

    HWND hInfo = CreateWindowExW(
        0,
        L"Button",
        L"About",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP |
        BS_PUSHBUTTON,
        44, h - 102,
        100, 27,
        hMainWindow,
        (HMENU)IDC_BUTTON1,
        hInstance,
        nullptr
    );

    SendMessageW(hInfo, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hGenerate = CreateWindowExW(
        0,
        L"Button",
        L"Generate",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP |
        BS_PUSHBUTTON,
        250, h - 102,
        100, 27,
        hMainWindow,
        (HMENU)IDC_BUTTON2,
        hInstance,
        nullptr
    );

    SendMessageW(hGenerate, WM_SETFONT, (WPARAM)hLabelFont, 0);

    HWND hQuit = CreateWindowExW(
        0,
        L"Button",
        L"Quit",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP |
        BS_PUSHBUTTON,
        w - 160, h - 102,
        100, 27,
        hMainWindow,
        (HMENU)IDC_BUTTON3,
        hInstance,
        nullptr
    );

    SendMessageW(hQuit, WM_SETFONT, (WPARAM)hLabelFont, 0);

    WCHAR pVersionString[256]{};

    wsprintfW(pVersionString, L"v%s • %s", pVersion, pWebsite);

    HWND hVersion = CreateWindowExW(
        0,
        L"Static",
        pVersionString,
        WS_CHILD | WS_VISIBLE |
        SS_NOTIFY,
        10, 436,
        170, 16,
        hMainWindow, (HMENU)IDC_LABEL4,
        hInstance, nullptr
    );

    SetWindowSubclass(hVersion, (SUBCLASSPROC)StaticLinkProc, IDC_LABEL4, 0);

    SendMessageW(hVersion, WM_SETFONT, (WPARAM)hSmolFont, 0);
    SendMessageW(hVersion, WM_APP + 0x69, 0, 0);

    HWND hBRText = CreateWindowExW(
        0,
        L"Static",
        L"z22 / mskey / Endermanch ◄ 16/04/2023",
        WS_CHILD | WS_VISIBLE,
        w - (170 + 20), 436,
        170, 16,
        hMainWindow, (HMENU)IDC_LABEL5,
        hInstance, nullptr
    );

    SendMessageW(hBRText, WM_SETFONT, (WPARAM)hSmolFont, 0);

    ShowWindow(hMainWindow, SW_SHOW);
    UpdateWindow(hMainWindow);

    MSG uMessage;

    while(GetMessageW(&uMessage, nullptr, 0, 0)) {
        if (!IsDialogMessageW(hMainWindow, &uMessage)) {
            TranslateMessage(&uMessage);
            DispatchMessageW(&uMessage);
        }
    }

    ReleaseDC(hMainWindow, hMainDC);

    return true;
}