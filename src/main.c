#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#include "nmath.h"
#include "trig2.h"
#include "colour.h"

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);
const int W = 500;
const int H = 500;
const int R = 200;
const float PI = 3.141592653;

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float mx = 0;
    float my = 0;
    float rx = 0;
    float ry = 0;
    float *screen = calloc(R*R*6, sizeof(float));
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "vCalc";
    RegisterClass(&wc);
	
    hWnd = CreateWindow(
      "vCalc", "Visual Calculator", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, W, H,
      NULL, NULL, hInstance, NULL);
	
	int i, j, k;
	float _i, _j;
	for (i = 0; i < R; i++) {
		for (j = 0; j < R; j++) {
			_i = (float) i;
			_j = (float) j;
			
			float a[] = {10*(2*_i-R)/R, 10*(2*_j-R)/R};
			float *b = calloc(2, sizeof(float));
			
			sin2(a, b);
			
			colour(*(b+1), 10, screen+i*R*6+j*6);
			screen[i*R*6+j*6+3] = *a;
			screen[i*R*6+j*6+4] = *(a+1);
			screen[i*R*6+j*6+5] = *b;
			
		}
	}
	
    EnableOpenGL(hWnd, &hDC, &hRC);
    
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	
	glMatrixMode(GL_MODELVIEW);
	
    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
            	mx = GET_X_LPARAM(msg.lParam);
            	my = GET_Y_LPARAM(msg.lParam);
			}
			else if (msg.message == WM_LBUTTONUP) {
				rx += (GET_X_LPARAM(msg.lParam)-mx)/(float)W*360;
            	ry += (GET_Y_LPARAM(msg.lParam)-my)/(float)H*360;
			}
            else if (msg.message == WM_MOUSEMOVE && msg.wParam == MK_LBUTTON) 
			{
            	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        	
				glPushMatrix();
				
				glLoadIdentity();
				
				glRotatef(ry+(GET_Y_LPARAM(msg.lParam)-my)/(float)H*360, 1, 0, 0);
				glRotatef(rx+(GET_X_LPARAM(msg.lParam)-mx)/(float)W*360, 0, 0, 1);
				
				for (i = 0; i < R-1; i++) {
					glBegin(GL_QUAD_STRIP);
					for (j = 0; j < R; j++) {
						for (k = 0; k < 2; k++) {
							glColor3f(screen[(i+k)*R*6+j*6+0],screen[(i+k)*R*6+j*6+1],screen[(i+k)*R*6+j*6+2]);
							glVertex3f(screen[(i+k)*R*6+j*6+3],screen[(i+k)*R*6+j*6+4],screen[(i+k)*R*6+j*6+5]);
						}
					}
					glEnd();
				}
				
				glPopMatrix();
	            SwapBuffers(hDC);
	            
	            Sleep(10);
			}
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    free(screen);
    
    DisableOpenGL(hWnd, hDC, hRC);
	
    DestroyWindow(hWnd);

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;
        
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    
    *hDC = GetDC(hWnd);
	
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);
	
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
