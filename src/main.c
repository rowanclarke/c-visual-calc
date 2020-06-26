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

const float MAXX = 10;
const float MAXY = 10;
const float MAXZ = 10;
const float MAXW = 10;
const float MINX = -10;
const float MINY = -10;
const float MINZ = -10;
const float MINW = -10;
const float SPEED = 180;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
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
    float *function = calloc(R*R*6, sizeof(float));
    float *grid = calloc((MAXX-MINX)*(MAXY-MINY), sizeof(float));
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
	float _i, _j, b1, b2, b3, c1, c2, c3;
	
	b1 = (MAXX-MINX)/2;
	b2 = (MAXY-MINY)/2;
	b3 = (MAXZ-MINZ)/2;
	
	c1 = (MAXX+MINX)/2;
	c2 = (MAXY+MINY)/2;
	c3 = (MAXZ+MINZ)/2;
	
	for (i = 0; i < R; i++) {
		for (j = 0; j < R; j++) {
			_i = (float) i;
			_j = (float) j;
			
			float a[] = {b1*(2*_i-R)/R+c1, b2*(2*_j-R)/R+c2};
			float *b = calloc(2, sizeof(float));
			
			exponentiate(a, b, 2);
			
			function[i*R*6+j*6+3] = (*a-c1)/b1*10;
			function[i*R*6+j*6+4] = (*(a+1)-c2)/b2*10;
			function[i*R*6+j*6+5] = (*b-c3)/b3*10;
			colour(*(b+1), MINW, MAXW, function+i*R*6+j*6);
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
	
    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
            	mx = GET_X_LPARAM(msg.lParam);
            	my = GET_Y_LPARAM(msg.lParam);
			}
			else if (msg.message == WM_LBUTTONUP) {
				rx += (GET_X_LPARAM(msg.lParam)-mx)/(float)W*SPEED;
            	ry += (GET_Y_LPARAM(msg.lParam)-my)/(float)H*SPEED;
			}
            else if (msg.message == WM_MOUSEMOVE && msg.wParam == MK_LBUTTON) {
            	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        	
				glPushMatrix();
				
				glLoadIdentity();
				
				glRotatef(ry+(GET_Y_LPARAM(msg.lParam)-my)/(float)H*SPEED, 1, 0, 0);
				glRotatef(rx+(GET_X_LPARAM(msg.lParam)-mx)/(float)W*SPEED, 0, 0, 1);
				
				for (i = 0; i < R-1; i++) {
					glBegin(GL_QUAD_STRIP);
					for (j = 0; j < R; j++) {
						for (k = 0; k < 2; k++) {
							glColor3f(function[(i+k)*R*6+j*6+0],function[(i+k)*R*6+j*6+1],function[(i+k)*R*6+j*6+2]);
							glVertex3f(function[(i+k)*R*6+j*6+3],function[(i+k)*R*6+j*6+4],function[(i+k)*R*6+j*6+5]);
						}
					}
					glEnd();
				}
				
				glColor3f(1, 1, 1);
				
				for (i = MINX; i <= MAXX; i++) {
					glBegin(GL_LINES);
					glVertex3f(((float)i-c1)*10/b1, -10, 0);
					glVertex3f(((float)i-c1)*10/b1, 10, 0);
					glEnd();
				}
				for (i = MINY; i <= MAXY; i++) {
					glBegin(GL_LINES);
					glVertex3f(-10, ((float)i-c2)*10/b2, 0);
					glVertex3f(10, ((float)i-c2)*10/b2, 0);
					glEnd();
				}
				
				for (i = MINZ; i <= MAXZ; i++) {
					glBegin(GL_LINES);
					glVertex3f(-10, 0, ((float)i-c3)*10/b3);
					glVertex3f(10, 0, ((float)i-c3)*10/b3);
					glEnd();
				}
				for (i = MINX; i <= MAXX; i++) {
					glBegin(GL_LINES);
					glVertex3f(((float)i-c1)*10/b1, 0, -10);
					glVertex3f(((float)i-c1)*10/b1, 0, 10);
					glEnd();
				}
				
				for (i = MINZ; i <= MAXZ; i++) {
					glBegin(GL_LINES);
					glVertex3f(0, -10, ((float)i-c3)*10/b3);
					glVertex3f(0, 10, ((float)i-c3)*10/b3);
					glEnd();
				}
				for (i = MINY; i <= MAXY; i++) {
					glBegin(GL_LINES);
					glVertex3f(0, ((float)i-c2)*10/b2, -10);
					glVertex3f(0, ((float)i-c2)*10/b2, 10);
					glEnd();
				}
				
				glPopMatrix();
	            SwapBuffers(hDC);
	            
	            Sleep(10);
			}
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    free(function);
    free(grid);
    
    DisableOpenGL(hWnd, hDC, hRC);
	
    DestroyWindow(hWnd);

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

	    default:
	        return DefWindowProc(hWnd, message, wParam, lParam);
    	}
}

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
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

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
