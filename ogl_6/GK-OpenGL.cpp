/*********************************************************************
* Multimedia: programowanie grafiki w œrodowisku MS Windows + OpenGL *
*********************************************************************/

#include <windows.h>
#include <math.h>

#include <gl\gl.h>
#include <gl\glu.h>

#define PI 3.1415926

//deklaracja funkcji okna przetwarzaj¹cej meldunki
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL SetWindowPixelFormat(HDC hDC);
BOOL CreateViewGLContext(HDC hDC);
void RenderScene();
void CreateScene();

int g_GLPixelIndex = 0;
HGLRC g_hGLContext = NULL;
HDC g_context = NULL;

double g_angle = 0.0;

enum GLDisplayListNames { Wall=1, Logo=2, Floor=3, Tfront1=4, Tfront2=5, Tback1=6, Tback2=7, Twall=8,Twall1=9,Twall2=10,Twall3=11,Twall4=12,Twall5=13 };

// odpowiednik funkcji main dla MS Windows
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	MSG meldunek;		  // komunikat od systemu operacyjnego
	WNDCLASS nasza_klasa; // klasa g³ównego okna aplikacji
	HWND okno;            // uchwyt g³ównego okna aplikacji
	static char nazwa_klasy[] = "Klasa okna";

	// definicja i rejestracja klasy g³ównego okna aplikacji
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW;
	nasza_klasa.lpfnWndProc   = WndProc;   // adres funkcji okna przetwarzaj¹cej meldunki
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; // uchwyt aktualnego wykonania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;
    RegisterClass(&nasza_klasa);
	
	// utworzenie i uwidocznienie g³ównego okna aplikacji 
 	okno = CreateWindow(nazwa_klasy, "Multimedia",
		                WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
						100, 50, 700, 700, NULL, NULL, hInstance, NULL);
	ShowWindow(okno, nCmdShow);
	UpdateWindow(okno); // odœwie¿enie okna

	// g³ówna pêtla programu
	while (GetMessage(&meldunek, NULL, 0, 0)) // pobranie meldunku z kolejki, FALSE <=> wm_Quit
	{
		TranslateMessage(&meldunek); // wstêpne przetwarzanie meldunku
		DispatchMessage(&meldunek);  // wywo³anie funkcji okna przetwarzaj¹cej meldunek
	}
	return (int)meldunek.wParam;
}

// funkcja okna przetwarzaj¹ca meldunki przekazywane do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
	    	
	switch (kod_meldunku) // rozpoznanie meldunku
	{
	case WM_CREATE:  // utworzenie okna
		{
			mPlik = CreateMenu();	// tworzenie menu
			AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
			mInfo = CreateMenu();
			AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
			mGlowne = CreateMenu();
			AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
			AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
			SetMenu(okno, mGlowne);
			DrawMenuBar(okno);

			g_context = GetDC(okno);	// tworzenie obrazu
			if (SetWindowPixelFormat(g_context)==FALSE)	return FALSE;
			if (CreateViewGLContext(g_context)==FALSE)	return 0;
			CreateScene();
			SetTimer(okno, 1, 20, NULL);
			return 0;
		}

	case WM_COMMAND: // wybór polecenia z menu
		switch (wParam)
		{
			case 101: DestroyWindow(okno); // generacja meldunku WM_DESTROY
        			  break;
			case 200: MessageBox(okno, "Imiê i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: // wciœniêcie lewego przycisku myszy
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			return 0;
		}

	case WM_PAINT:	// odœwie¿enie obrazu (rysowanie)
		{
			PAINTSTRUCT paint;
			HDC kontekst;
			kontekst = BeginPaint(okno, &paint);
			RenderScene();
			SwapBuffers(kontekst);
			EndPaint(okno, &paint);
			return 0;
		}

	case WM_TIMER:	// zegar systemowy
		InvalidateRect(okno, NULL, FALSE);
		g_angle += 0.5;
		if (g_angle > 359.75) g_angle = 0;
		return 0;

	case WM_SIZE:	// zmiana rozmiaru okna
		{
			int cx = LOWORD(lParam);
			int cy = HIWORD(lParam);

			GLsizei width, height;
			GLdouble aspect;
			width = cx;
			height = cy;
			aspect = (cy==0)? (GLdouble)width: (GLdouble)width/(GLdouble)height;
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(55.0, aspect, 1.0, 50.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDrawBuffer(GL_BACK);
			glEnable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			return 0;
		}
  	
	case WM_DESTROY:	// zamkniêcie okna
		if(wglGetCurrentContext()!=NULL)
			wglMakeCurrent(NULL, NULL) ;	// dezaktualizacja kontekstu renderuj¹cego
		if (g_hGLContext!=NULL)
		{
			wglDeleteContext(g_hGLContext);
			g_hGLContext = NULL;
		}
		ReleaseDC(okno, g_context);
		KillTimer(okno, 1);
		PostQuitMessage(0);
		return 0;
    
	default: // standardowa obs³uga pozosta³ych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}

BOOL SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL |PFD_DOUBLEBUFFER |PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	g_GLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
	
	if (g_GLPixelIndex==0) 
	{
		g_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, g_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc)==0)
			return FALSE;
	}
	if (SetPixelFormat( hDC, g_GLPixelIndex, &pixelDesc)==FALSE)
		return FALSE;
	return TRUE;
}

BOOL CreateViewGLContext(HDC hDC)
{
	g_hGLContext = wglCreateContext(hDC);

	if (g_hGLContext == NULL)
		return FALSE;
	if (wglMakeCurrent(hDC, g_hGLContext)==FALSE)
		return FALSE;
	return TRUE;
}

void RenderScene()
{
	GLfloat RedSurface[]  = { 1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat GreenSurface[] = { 0.0f, 0.4f, 0.0f, 1.0f};
	GLfloat WhiteSurface[] = { 1.0f, 1.0f, 1.0f, 1.0f};

	GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat LightDiffuse[] = { 0.8f, 0.8f, 0.8f, 0.8f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
	GLfloat LightPosition[] = { 0.0f, 1.0f, 1.0f, 0.0f };	

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);		//1 sk³adowa: œwiat³o otaczaj¹ce (bezkierunkowe)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);		//2 sk³adowa: œwiat³o rozproszone (kierunkowe)
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	gluLookAt(0.0, 6.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotated(g_angle, 0.0, 1.0, 0.0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, WhiteSurface);
	//glCallList(Wall);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	//glCallList(Logo);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
	glCallList(Floor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	glCallList(Tfront1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WhiteSurface);///////
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);///////
	glCallList(Tfront2);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	glCallList(Tback1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WhiteSurface);/////
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);///////
	glCallList(Tback2);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	glCallList(Twall);
	glCallList(Twall1);
	glCallList(Twall2);
	glPopMatrix();
	glFlush();
}

void CreateScene()
{
	glNewList(Tfront1,GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3d( 0.0, 0.0, 1.0);
			glVertex3d( 0.0, 0.0, 0.5);
			glVertex3d(  1.0, 0.0, 0.5);
			glVertex3d(  1.0, 3.0, 0.5);
			glVertex3d( 0.0, 3.0,  0.5);			
		glEnd();
	glEndList(); 
	
	glNewList(Tfront2,GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
			glNormal3d( 0.0, 0.0, 1.0);
			glVertex3d(  2.0, 3.0, 0.5);
			//glVertex3d(  2.0, 3.5, 0.5);
			//glVertex3d( -1.0, 3.5,  0.5);
			glVertex3d( 1.8, 3.2, 0.5);
			glVertex3d( 0.9, 3.5, 0.5);
			glVertex3d( 0.0, 3.5, 0.5);
			glVertex3d( -0.8, 3.2, 0.5);
			glVertex3d( -1.0, 3.0,  0.5);		
		glEnd();
	glEndList(); 

	glNewList(Tback2,GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
			glNormal3d( 0.0, 0.0, -1.0);
			glVertex3d(  2.0, 3.0, -0.1);
			//glVertex3d(  2.0, 3.5, 0.5);
			//glVertex3d( -1.0, 3.5,  0.5);
			glVertex3d( 1.8, 3.2, -0.1);
			glVertex3d( 0.9, 3.5, -0.1);
			glVertex3d( 0.0, 3.5, -0.1);
			glVertex3d( -0.8, 3.2, -0.1);
			glVertex3d( -1.0, 3.0,  -0.1);		
		glEnd();
	glEndList(); 
	
	glNewList(Tback1,GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3d( 0.0, 0.0, -1.0);
			glVertex3d( 0.0, 0.0, -0.1);
			glVertex3d(  1.0, 0.0, -0.1);
			glVertex3d(  1.0, 3.0, -0.1);
			glVertex3d( 0.0, 3.0,  -0.1);			
		glEnd();
	glEndList(); 

	glNewList(Twall1,GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d( 1.0, 0.0, 0.0);
			glVertex3d(  2.0, 3.0, -0.1);
			glVertex3d(  2.0, 3.0, 0.5);

			//for( long double alpha=0.0; alpha<PI-PI/24; alpha+=PI/12 )
			//{
				//glNormal3d( 0.0*cos(alpha), 1.0*sin(alpha), -1.0);
				glNormal3d( 0.7, 0.7, 0.0);
				glVertex3d( 1.8, 3.2, -0.1);
				glVertex3d( 1.8, 3.2, 0.5);
				glNormal3d( 0.4, 0.8, 0.0);
				glVertex3d( 0.9, 3.5, -0.1);
				glVertex3d( 0.9, 3.5, 0.5);
				glNormal3d( -0.4, 0.8, 0.0);
				glVertex3d( 0.0, 3.5, -0.1);
				glVertex3d( 0.0, 3.5, 0.5);
				glNormal3d( -0.7, 0.7, 0.0);
				glVertex3d( -0.8, 3.2, -0.1);
				glVertex3d( -0.8, 3.2, 0.5);
			//}
			glNormal3d( -1.0, 0.0, 0.0);
			glVertex3d( -1.0, 3.0,  -0.1);	
			glVertex3d( -1.0, 3.0,  0.5);
			//glVertex3d(  2.0, 3.5, -0.1);
			//glVertex3d( -1.0, 3.5,  -0.1);
			
			
		glEnd();
	glEndList(); 

	glNewList(Twall,GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d( 1.0, 0.0, 0.0);

			glVertex3d(  1.0, 0.0, 0.5);
			glVertex3d(  1.0, 0.0, -0.1); // podstawa 
			glVertex3d(  1.0, 3.0, 0.5);
			glVertex3d(  1.0, 3.0, -0.1);	
			glVertex3d(  2.0, 3.0, 0.5);
			glVertex3d(  2.0, 3.0, -0.1); 
		
				
			

		glEnd();
	glEndList();
	
	/*glNewList(Twall1,GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
			for( long double alpha=0.0; alpha<PI-PI/24; alpha+=PI/12 )
			{
				glNormal3d( 1.0*cos(alpha), 1.0*sin(alpha), 0.5);
				glVertex3d( 2.0*cos(alpha), 3.0*sin(alpha), 0.5);
				glVertex3d( 2.0*cos(alpha), 3.0*sin(alpha), 0.5);
			}
			
		
			glNormal3d( 0.0, 1.0, 0.0);
			glVertex3d(  2.0, 3.0, -0.1);
			glVertex3d(  2.0, 3.0, 0.5); 
			glVertex3d(  2.0, 3.5, -0.1);
			glVertex3d(  2.0, 3.5, 0.5);
			glVertex3d( -1.0, 3.5,  -0.1);
			glVertex3d( -1.0, 3.5,  0.5);
			glVertex3d( -1.0, 3.0,  -0.1);
			glVertex3d( -1.0, 3.0,  0.5);
			

	glEnd();
	glEndList();*/
	
	glNewList(Twall2,GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d( -1.0, 0.0, 0.0);

			//glVertex3d( -1.0, 3.5,  0.5); 
			//glVertex3d( -1.0, 3.5,  -0.1);
			glVertex3d( -1.0, 3.0,  0.5);	
			glVertex3d( -1.0, 3.0,  -0.1);	// góra
			glVertex3d( 0.0, 3.0,  0.5);
			glVertex3d( 0.0, 3.0,  -0.1); 
			glVertex3d( 0.0, 0.0,  0.5);
			glVertex3d( 0.0, 0.0,  -0.1);
		
		
	

		glEnd();
	glEndList();
	
	
	glNewList(Floor,GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3d( 0.0, 1.0, 0.0);
			glVertex3d( -15.0, -1.0, -15.0);
			glVertex3d( -15.0, -1.0,  15.0);
			glVertex3d(  15.0, -1.0,  15.0);
			glVertex3d(  15.0, -1.0, -15.0);
		glEnd();
	glEndList();

	
	


/*
	
	glNewList(Wall,GL_COMPILE);	// GL_COMPILE - lista kompilowana, bez wykonania
		glBegin(GL_TRIANGLES);	// mo¿liwe: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
			glNormal3d(  0.0,  0.0,  1.0);	// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
			glVertex3d( -2.0, -0.4, -0.3);	// GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
			glVertex3d(  2.0, -0.4, -0.3);
			glVertex3d(  0.0,  2.0, -0.3);
		glEnd();	
	glEndList();

	glNewList(Logo,GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN );
			glNormal3d( 0.0, 0.0, 1.0);
			glVertex3d( 0.0, 0.0, 0.3);
			for( long double alpha=0.0; alpha<PI-PI/24; alpha+=PI/12 )
				glVertex3d( 1.0*cos(alpha), -1.0*sin(alpha), 0.3);
			glVertex3d( -1.0, 0.0, 0.3);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP );
			glNormal3d( 0.0, 0.0, 1.0);
			for( long double alpha=0.0; alpha<PI-PI/24; alpha+=PI/12 )
			{
				glVertex3d( 1.0*cos(alpha), 1.0*sin(alpha), 0.3);
				glVertex3d( 0.8*cos(alpha), 0.8*sin(alpha), 0.3);
			}
			glVertex3d( -1.0, 0.0, 0.3);
			glVertex3d( -0.8, 0.0, 0.3);
		glEnd();
	glEndList();

	glNewList(Floor,GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3d( 0.0, 1.0, 0.0);
			glVertex3d( -15.0, -1.0, -15.0);
			glVertex3d( -15.0, -1.0,  15.0);
			glVertex3d(  15.0, -1.0,  15.0);
			glVertex3d(  15.0, -1.0, -15.0);
		glEnd();
	glEndList(); */

	
}
