#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM, LPARAM); 

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("Windows API Material Design");
	MSG msg;
	WNDCLASSEX wc;
	HWND hwnd;
	
	memset(&wc,0,sizeof(wc));
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = szAppName;
	
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL,TEXT("很抱歉，注册窗口类失败!"),TEXT("警告"),MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_DLGMODALFRAME,szAppName,TEXT("Material Design Button"),WS_OVERLAPPEDWINDOW^WS_THICKFRAME,CW_USEDEFAULT,CW_USEDEFAULT,500,250,NULL,NULL,hInstance,NULL);
	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	ShowWindow(hwnd,nCmdShow);
	
	UpdateWindow(hwnd);
	
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam; 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HPEN hPen;
	static HBRUSH hBrush;
	static int DecideToDraw,DecideToDrawSpeed;				/*判断如何绘制*/
	static int x,y;					/*鼠标位置坐标*/
	static int Radii;				/*圆形半径*/
	PAINTSTRUCT ps;
	
	static int cxClient,cyClient;
	
	switch(Message)
	{
		case WM_TIMER:
		{
			InvalidateRect(hwnd,NULL,TRUE);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			DecideToDraw = TRUE;
			DecideToDrawSpeed = FALSE;
			Radii = 0;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			SetTimer(hwnd,1,5,NULL);
			InvalidateRect(hwnd,NULL,TRUE);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			DecideToDraw = FALSE;
			DecideToDrawSpeed = TRUE;
			InvalidateRect(hwnd,NULL,TRUE);
			KillTimer(hwnd,1);
			return 0;
		}
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			return 0;
		}
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd,&ps);
			SetBkMode(hdc,TRANSPARENT);
			hBrush = CreateSolidBrush(RGB(190,190,190));
			SelectObject(hdc,GetStockObject(NULL_PEN));
			SelectObject(hdc,hBrush);
			if(DecideToDraw)
			{
				Ellipse(hdc,x-Radii,y-Radii,x+Radii,y+Radii);
				Radii++;
				if(Radii >= max(cxClient,cyClient))
				{
					DecideToDraw = FALSE;
				}
			}
			else if(DecideToDrawSpeed)
			{
				Ellipse(hdc,x-Radii,y-Radii,x+Radii,y+Radii);
				Radii += 2;							//半径每次增长2 
				if(Radii >= max(cxClient,cyClient))
				{
					DecideToDrawSpeed = FALSE;
				}
				Sleep(2);
				InvalidateRect(hwnd,NULL,TRUE);
			}
			DeleteObject(hBrush);
			EndPaint(hwnd,&ps); 
			return 0;
		}
		case WM_DESTROY:
		{
			DeleteObject(hPen);
			DeleteObject(hBrush);
			PostQuitMessage(0);
			return 0;
		}
		default:
			return DefWindowProc(hwnd,Message,wParam,lParam);
	}
}
