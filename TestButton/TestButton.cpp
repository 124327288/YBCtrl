// TestButton.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestButton.h"
#include "button\YBButton.h"
#include "button\YBButtonX.h"
#include "loader\YBImageLoader.h"
#include "frame\YBCtrlFontDepository.h"
#include "YBTopMostBar.h"
#include "YBTestBar.h"
#include "YBIMNotifyDlg.h"
#include "YBPage1.h"
#include "YBPage2.h"
#include "YBPage3.h"
#include "YBPage4.h"
#include "tchar.h"
#include <assert.h>
USING_NAMESPACE_YBCTRL

#define MAX_LOADSTRING 100
#define CTRLID_BTN_LEFT (258)
#define CTRLID_BTNX_RIGHT (259)
#define CTRLID_BTNX2 (260)
#define CTRLID_BTM_BOTTOM (261)
	//new
#define CTRLID_BTN_TEST1 (262)
#define CTRLID_BTN_TEST2 (263)
#define CTRLID_BTN_TEST3 (264)
#define CTRLID_BTN_TEST4 (265)
	//
#define CTRLID_CHILD_TEST1 (266)
#define CTRLID_CHILD_TEST2 (267)
#define CTRLID_CHILD_TEST3 (268)
#define CTRLID_CHILD_TEST4 (269)


	ULONG_PTR g_gdipToken;
::Gdiplus::GdiplusStartupInput g_gdipStartupInput;

//
struct DummyCls4GDIP{
	DummyCls4GDIP() {
		::Gdiplus::GdiplusStartup( &g_gdipToken, &g_gdipStartupInput, NULL );
	}
	~DummyCls4GDIP() {
		::Gdiplus::GdiplusShutdown( g_gdipToken );
	}
};

// 全局变量:
DummyCls4GDIP g_initGdip;
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
YBButtonX g_testBtnX;
YBButtonX g_testBtnX2;
CYBIMNotifyDlg g_testMNotifyBtn;
//YBPopup g_testPopup;
YBTopMostBar g_testTopBar;
YBTopMostBar g_testLeftBar;
YBTopMostBar g_testRightBar;
YBTopMostBar g_testBottomBar;
YBTestBar g_testTestBar;
CYBIMNotifyDlg g_testYBIMNotifyDlg;
unsigned int g_fontIdBtnTxt;
unsigned int g_fontIdBtnTxtTop;
unsigned int g_fontIdBtnTxtMid;
unsigned int g_fontIdBtnTxtUnder;
unsigned int g_fontIdBtnLoad;
unsigned int g_fontIdTime;
unsigned int g_imageIdBtnNormal;
unsigned int g_imageIdBtnHover;
unsigned int g_imageIdBtnPress;
unsigned int g_imageIdBtnDisable;
unsigned int g_imageIdBtnXNormal;
unsigned int g_imageIdBtnXHover;
unsigned int g_imageIdBtnXPress;
unsigned int g_imageIdDownloadBK;
unsigned int g_imageIdInstallBK;
//////////////////////////////////////////////////////////////////////////
YBButtonX g_test1Ctrl;
YBButtonX g_test2Ctrl;
YBButtonX g_test3Ctrl;
YBButtonX g_test4Ctrl;
YBButton g_testBtn;
YBButton g_testBtn2;
YBButton g_testBtn3;
YBButton g_testBtn4;
YBPage1 g_testPage1;
YBPage2 g_testPage2;
YBPage3 g_testPage3;
YBPage4 g_testPage4;
//dialog hwnd;
HWND hWnd1Dlg;
HWND hWnd2Dlg;
HWND hWnd3Dlg;
HWND hWnd4Dlg;
//top button hwnd
HWND hWnd1Ctrl;
HWND hWnd2Ctrl;
HWND hWnd3Ctrl;
HWND hWnd4Ctrl;

//////////////////////////////////////////////////////////////////////////

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//
void loadImage();
void loadFont();
void createTopMostFrame( HINSTANCE hInstance );
void createYBIMNorifyDlg( HINSTANCE hInstance );

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//
	loadImage();
	loadFont();
	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTBUTTON, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTBUTTON));

	// 主消息循环:
 	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			=  CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTBUTTON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_TESTBUTTON);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	//
	int nScreenWidth;
	int nScreenHeight;
	int index = 4;
	int coordinatex;
	nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	coordinatex = nScreenWidth/index;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, nScreenWidth, nScreenHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	//first table button
	hWnd1Ctrl = g_test1Ctrl.Create( 0, WS_CHILD|WS_VISIBLE, coordinatex*0, 0, coordinatex, 30, hWnd, (HMENU)CTRLID_BTN_TEST1, hInstance );
	if( hWnd1Ctrl ){
		hWnd1Dlg = g_testPage1.create( 0, 30, nScreenWidth, nScreenHeight - 30, hWnd );
		// 	   g_testBtn.Create( 0, WS_CHILD|WS_VISIBLE, 80, 80, 60, 25, hWnd1Dlg, (HMENU)CTRLID_CHILD_TEST1, hInst );
		// 	   g_testBtn.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		// 	   g_testBtn.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		// 	   g_testBtn.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		::ShowWindow( hWnd1Dlg,SW_NORMAL );
	}
	g_test1Ctrl.setFixedSpace( 5, 2, 2, 2 );
	g_test1Ctrl.setImage( YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	g_test1Ctrl.setImage( YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	g_test1Ctrl.setImage( YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	g_test1Ctrl.setImage( YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	g_test1Ctrl.setImage( YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	g_test1Ctrl.setImage( YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	g_test1Ctrl.setFontId( g_fontIdBtnTxt );
	g_test1Ctrl.setTextSpace( 5, 5, 5, 5 );
	g_test1Ctrl.setText( _T("电  子  律  师") );
	g_test1Ctrl.enableBtnX( false );
	g_test1Ctrl.pushDown();

	//second table button
	hWnd2Ctrl = g_test2Ctrl.Create( 0, WS_VISIBLE|WS_CHILD, coordinatex*1, 0, coordinatex, 30, hWnd, (HMENU)CTRLID_BTN_TEST2, hInstance  );
	if( hWnd2Ctrl ){
		g_testPage2.m_btnTestModalDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestModalDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestModalDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestMsgBox.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestMsgBox.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestMsgBox.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestPopupPB.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestPopupPB.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestPopupPB.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestFileTPDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestFileTPDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestFileTPDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestFSDownLoader.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestFSDownLoader.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestFSDownLoader.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestWebEditDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestWebEditDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestWebEditDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestInstallFileDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestInstallFileDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestInstallFileDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestUpLoadWarmTipDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestUpLoadWarmTipDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestUpLoadWarmTipDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestYBPopupMB.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestYBPopupMB.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestYBPopupMB.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestDPCDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestDPCDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestDPCDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage2.m_btnTestImageEditor.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage2.m_btnTestImageEditor.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage2.m_btnTestImageEditor.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		
		hWnd2Dlg = g_testPage2.create( 0, 30, nScreenWidth, nScreenHeight - 30, hWnd );
		::ShowWindow( hWnd2Dlg, SW_HIDE );
	}
	g_test2Ctrl.setFixedSpace( 5, 2, 2, 2 );
	g_test2Ctrl.setImage( YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	g_test2Ctrl.setImage( YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	g_test2Ctrl.setImage( YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	g_test2Ctrl.setImage( YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	g_test2Ctrl.setImage( YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	g_test2Ctrl.setImage( YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	g_test2Ctrl.setFontId( g_fontIdBtnTxt );
	g_test2Ctrl.setTextSpace( 5, 5, 5, 5 );
	g_test2Ctrl.setText( _T("风  险  律  师") );
	g_test2Ctrl.enableBtnX( false );
	//    // three table button
	// 
	hWnd3Ctrl = g_test3Ctrl.Create( 0, WS_VISIBLE|WS_CHILD, coordinatex*2, 0, coordinatex, 30, hWnd, (HMENU)CTRLID_BTN_TEST3, hInstance  );
	if( hWnd3Ctrl ){
		g_testPage3.m_btnOpenModalDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage3.m_btnOpenModalDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage3.m_btnOpenModalDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage3.m_btnOpenChildModalDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage3.m_btnOpenChildModalDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage3.m_btnOpenChildModalDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage3.m_btnAddCornerNotifyDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage3.m_btnAddCornerNotifyDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage3.m_btnAddCornerNotifyDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		g_testPage3.m_btnMenuDlg.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testPage3.m_btnMenuDlg.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testPage3.m_btnMenuDlg.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		//
		hWnd3Dlg = g_testPage3.create( 0, 30, nScreenWidth, nScreenHeight - 30, hWnd );
		::ShowWindow( hWnd3Dlg, SW_HIDE );
	}
	g_test3Ctrl.setFixedSpace( 5, 2, 2, 2 );
	g_test3Ctrl.setImage( YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	g_test3Ctrl.setImage( YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	g_test3Ctrl.setImage( YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	g_test3Ctrl.setImage( YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	g_test3Ctrl.setImage( YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	g_test3Ctrl.setImage( YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	g_test3Ctrl.setFontId( g_fontIdBtnTxt );
	g_test3Ctrl.setTextSpace( 5, 5, 5, 5 );
	g_test3Ctrl.setText( _T("判  例  律  师") );
	g_test3Ctrl.enableBtnX( false );

	// four table button
	hWnd4Ctrl = g_test4Ctrl.Create( 0, WS_VISIBLE|WS_CHILD, coordinatex*3, 0, coordinatex, 30, hWnd, (HMENU)CTRLID_BTN_TEST4, hInstance  );
	if( hWnd4Ctrl ){
		hWnd4Dlg = g_testPage4.create( 0, 30, nScreenWidth, nScreenHeight - 30, hWnd );
		g_testBtn4.Create( 0, WS_CHILD|WS_VISIBLE, 80, 80, 60, 25, hWnd4Dlg, (HMENU)CTRLID_CHILD_TEST4, hInst );
		g_testBtn4.setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
		g_testBtn4.setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
		g_testBtn4.setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
		::ShowWindow( hWnd4Dlg, SW_HIDE );
	}
	g_test4Ctrl.setFixedSpace( 5, 2, 2, 2 );
	g_test4Ctrl.setImage( YBButtonX::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
	g_test4Ctrl.setImage( YBButtonX::EBTN_STATUS_HOVER, g_imageIdBtnHover );
	g_test4Ctrl.setImage( YBButtonX::EBTN_STATUS_PRESS, g_imageIdBtnPress );
	g_test4Ctrl.setImage( YBButtonX::EBTNX_STATUS_NORMAL, g_imageIdBtnXNormal );
	g_test4Ctrl.setImage( YBButtonX::EBTNX_STATUS_HOVER, g_imageIdBtnXHover );
	g_test4Ctrl.setImage( YBButtonX::EBTNX_STATUS_PRESS, g_imageIdBtnXPress );
	g_test4Ctrl.setFontId( g_fontIdBtnTxt );
	g_test4Ctrl.setTextSpace( 5, 5, 5, 5 );
	g_test4Ctrl.setText( _T("法  规  律  师") );
	g_test4Ctrl.enableBtnX( false );

	// createTopMostFrame( hInstance );

	// createYBIMNorifyDlg( hInstance );

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case CTRLID_BTN_LEFT:
			{
				int a = 4;
			}
			break;
		case CTRLID_BTN_TEST1:
			{
				if ( !( wmEvent & YBBTNX_CMDEVT_BTNMOVE ) ) {
					g_test2Ctrl.popUp();
					g_test3Ctrl.popUp();
					g_test4Ctrl.popUp();
					::ShowWindow( hWnd1Dlg, SW_SHOWNORMAL );
					::ShowWindow( hWnd2Dlg, SW_HIDE );
					::ShowWindow( hWnd3Dlg, SW_HIDE );
					::ShowWindow( hWnd4Dlg, SW_HIDE );
				}
			}
			break;
		case CTRLID_BTN_TEST2:
			{
				if ( !( wmEvent & YBBTNX_CMDEVT_BTNMOVE ) ) {
					g_test1Ctrl.popUp();
					g_test3Ctrl.popUp();
					g_test4Ctrl.popUp();

					::ShowWindow( hWnd2Dlg, SW_SHOWNORMAL );
					::ShowWindow( hWnd1Dlg, SW_HIDE );
					::ShowWindow( hWnd3Dlg, SW_HIDE );
					::ShowWindow( hWnd4Dlg, SW_HIDE );
				}
			}
			break;
		case CTRLID_BTN_TEST3:
			{
				if ( !( wmEvent & YBBTNX_CMDEVT_BTNMOVE ) ) {
					g_test1Ctrl.popUp();
					g_test2Ctrl.popUp();
					g_test4Ctrl.popUp();

					::ShowWindow( hWnd3Dlg, SW_SHOWNORMAL );
					::ShowWindow( hWnd1Dlg, SW_HIDE );
					::ShowWindow( hWnd2Dlg, SW_HIDE );
					::ShowWindow( hWnd4Dlg, SW_HIDE );
				}
			}
			break;
		case CTRLID_BTN_TEST4:
			{
				if ( !( wmEvent & YBBTNX_CMDEVT_BTNMOVE ) ) {
					g_test2Ctrl.popUp();
					g_test3Ctrl.popUp();
					g_test1Ctrl.popUp();

					::ShowWindow( hWnd4Dlg, SW_SHOWNORMAL );
					::ShowWindow( hWnd1Dlg, SW_HIDE );
					::ShowWindow( hWnd2Dlg, SW_HIDE );
					::ShowWindow( hWnd3Dlg, SW_HIDE );
				}
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			g_test1Ctrl.popUp();
			g_test2Ctrl.popUp();
			g_test3Ctrl.popUp();
			g_test4Ctrl.popUp();
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void loadImage() {
	TCHAR szModulePath[MAX_PATH];
	TCHAR* posTmp = NULL; 
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	std::basic_string< TCHAR > tstrImageDirPath;
	std::basic_string< TCHAR > tstrImageFilePath;
	unsigned int idImage;

	posTmp =  _tcsrchr( szModulePath, _T('\\') );
	if ( !posTmp ) {
		assert( false );
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("Gray.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}

	tstrImageFilePath = tstrImageDirPath + _T("topbar.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}

	tstrImageFilePath = tstrImageDirPath + _T("YBBtnNormal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnNormal = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("YBBtnHover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnHover = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("YBBtnPress.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnPress = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("Disable.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnDisable = idImage;

	//
	tstrImageFilePath = tstrImageDirPath + _T("x_normal.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnXNormal = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("x_hover.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnXHover = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("x_press.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdBtnXPress = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("DownloadBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdDownloadBK = idImage;

	tstrImageFilePath = tstrImageDirPath + _T("InstallBK.png");
	if ( !YBImageLoader::getInstance()->loadImage( tstrImageFilePath, idImage ) ) {
		assert( false );
	}
	g_imageIdInstallBK = idImage;

	//unsigned int ;
	//unsigned int ;
}

void createTopMostFrame( HINSTANCE hInstance ) {
	int nScreenWidth;
	int nScreenHeight;
	int nTopSpace = 30;
	int nLeftSpace = 6;
	int nRightSpace = 6;
	int nBottomSpace = 4;

	nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
	nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );

	// 	g_testTopBar.Create( 0, 0, nScreenWidth, nTopSpace, hInstance );
	// 	g_testTestBar.Create( 0, nTopSpace + 5, nScreenWidth, nTopSpace, hInstance );
	// 	//g_testTopBar.setBkImage( _T("F:\\code\\work\\trunk\\YBCtrl\\Debug\\image\\topbar.png") );
	// 	g_testLeftBar.Create( 0, nTopSpace, nLeftSpace, ( nScreenHeight - nTopSpace - nBottomSpace ), hInstance );
	// 	g_testRightBar.Create( nScreenWidth - nRightSpace, nTopSpace, nRightSpace, ( nScreenHeight - nTopSpace - nBottomSpace ), hInstance );
	// 	g_testBottomBar.Create( 0, nScreenHeight - nBottomSpace, nScreenWidth, nBottomSpace, hInstance );

}

void loadFont() {
	LOGFONT lFont;

	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 18;
	g_fontIdBtnTxt = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 255, 255, 255 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 25;
	lFont.lfWeight = 700;
	g_fontIdBtnTxtTop = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 0, 85, 191 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 600;
	g_fontIdBtnLoad = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 23, 84, 152 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	//lFont.lfWeight = 550;
	g_fontIdBtnTxtMid = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 23, 84, 152 ) );

	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 33;
	g_fontIdBtnTxtUnder = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 136, 176, 225 ) );
	//
	memset( &lFont, 0, sizeof( LOGFONT ) );
	_tcscpy_s( lFont.lfFaceName, _T("微软雅黑") );
	lFont.lfHeight = 20;
	lFont.lfWeight = 600;
	g_fontIdTime = ybctrl::YBCtrlFontDepository::getInstance()->insertLOGFONT( lFont, RGB( 88, 132, 0 ) );
}

// void createYBIMNorifyDlg( HINSTANCE hInstance ) {
// 	ybctrl::YBButton* pBtnInstall = NULL;
// 	ybctrl::YBButton* pBtnCancel = NULL;
// 	//top
// 	g_testYBIMNotifyDlg.Create( 10, 100, hInstance );
// 	g_testYBIMNotifyDlg.setBkImageId( g_imageIdDownloadBK, g_imageIdInstallBK );
// 	g_testYBIMNotifyDlg.setFontIdTop( g_fontIdBtnTxtTop );
// 	//middle
// 	g_testYBIMNotifyDlg.setFontIdMiddle( g_fontIdBtnTxtMid );
// 	g_testYBIMNotifyDlg.setTextMiddle( _T("        尊敬的用户，“搜狗拼音”输入法在登陆设备中没有安装，如果您需要使用本输入法， \
// 										  我们将从本输入法的官方网站“www.wangmawubi.com”中为您自动下载安装。") );
// 	//under
// 	g_testYBIMNotifyDlg.setFontIdUnder( g_fontIdBtnTxtTop );
// 	g_testYBIMNotifyDlg.setTextUnder( _T("下载中...") );
// 	//time
// 	g_testYBIMNotifyDlg.setFontIdTime( g_fontIdTime );
// 	g_testYBIMNotifyDlg.setFontRemTimeId( g_fontIdBtnLoad );
// 
// 	pBtnInstall = g_testYBIMNotifyDlg.getBtnInstall();
// 	pBtnInstall->setFixedSpace( 5, 2, 2, 2 );
// 	pBtnInstall->setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
// 	pBtnInstall->setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
// 	pBtnInstall->setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
// 	pBtnInstall->setFontId( g_fontIdBtnTxt );
// 
// 	//
// 	pBtnCancel = g_testYBIMNotifyDlg.getBtnCancel();
// 	pBtnCancel->setFixedSpace( 5, 2, 2, 2 );
// 	pBtnCancel->setImage( YBButton::EBTN_STATUS_NORMAL, g_imageIdBtnNormal );
// 	pBtnCancel->setImage( YBButton::EBTN_STATUS_HOVER, g_imageIdBtnHover );
// 	pBtnCancel->setImage( YBButton::EBTN_STATUS_PRESS, g_imageIdBtnPress );
// 	pBtnCancel->setFontId( g_fontIdBtnTxt );
//}
