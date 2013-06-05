/***************************************************************************
* Grafika komputerowa: programowanie grafiki w �rodowisku MS Windows + GDI *
***************************************************************************/
// Zadanie 3

#include <windows.h>

// deklaracja funkcji okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// g��wna funkcja programu
 int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// deklaracja zmiennych
	MSG Meldunek;
	WNDCLASS KlasaOkna;
	HWND Okno;
	static char NazwaKlasyOkna[] = "Okno";

	// definicja w�a�ciwo�ci definiowanej klasy okna i jej rejestracja
	KlasaOkna.style         = CS_HREDRAW | CS_VREDRAW;
	KlasaOkna.lpfnWndProc   = WndProc; 
 	KlasaOkna.cbClsExtra    = 0 ;
	KlasaOkna.cbWndExtra    = 0 ;
	KlasaOkna.hInstance     = hInstance;
	KlasaOkna.hIcon         = 0;
	KlasaOkna.hCursor       = LoadCursor(0, IDC_ARROW);
	KlasaOkna.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	KlasaOkna.lpszMenuName  = "Menu" ;
	KlasaOkna.lpszClassName = NazwaKlasyOkna;
    RegisterClass (&KlasaOkna);
	
	// utworzenie i wy�wietlenie okna
 	Okno = CreateWindow(NazwaKlasyOkna, "PJWSTK Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
   	ShowWindow (Okno, nCmdShow) ;
	UpdateWindow (Okno) ;

	// p�tla obs�ugi meldunk�w (g��wna p�tla programu)
	while (GetMessage(&Meldunek, NULL, 0, 0))	// FALSE tylko dla komunikatu WM_QUIT
	{
		TranslateMessage(&Meldunek); // wst�pna obr�bka meldunku
		DispatchMessage(&Meldunek);  // wywolanie funkcji okna na podstawie KlasaOkna.lpfnWndProc
	}
	return (int)Meldunek.wParam;
}

// funkcja okna zawieraj�ca obs�ug� meldunk�w przesy�anych do okna
LRESULT CALLBACK WndProc (HWND Okno, UINT KodMeldunku, WPARAM wParam, LPARAM lParam)
{
	// deklaracja zmiennych
	static POINT Punkty[100]; // tablica klikni��
	static POINT Punkty2[100]; // tablica klikni��
    static int rozmiary[100], regiony[100];
	static int lPunktow = 0;
    static int lPunktow2 = 0;
	static int i = 50;
	static int ii=0;
	static bool leftMouse;
    static bool rightMouse;
	
	static bool negatyw = false;
	HMENU mGlowne, mPlik, mRozmiar, mKolor, mInfo;

	// obs�uga meldunku w zale�no�ci od kodu meldunku
	switch (KodMeldunku) 
	{ case WM_CREATE:  // obs�uga utworzenia okna - stworzenie menu
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Gong...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mRozmiar = CreateMenu();
		AppendMenu(mRozmiar, MF_STRING, 103, "&Du�e");
		AppendMenu(mRozmiar, MF_STRING, 104, "&Ma�e");
		mKolor = CreateMenu();
		AppendMenu(mKolor, MF_STRING, 105, "&Podstawowy");
		AppendMenu(mKolor, MF_STRING, 106, "&Negatyw");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 102, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mRozmiar, "&Rozmiar");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mKolor, "&Kolor");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Info");
		SetMenu(Okno, mGlowne);
		DrawMenuBar(Okno);

		Punkty[lPunktow].x=200; // wype�nienie pierwszej pozycji tablicy klikni��
		Punkty[lPunktow].y=200;
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;

	  case WM_COMMAND: // obs�uga wyboru opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(Okno, "Czy wygenerowa� gong?", "Gong", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(Okno); // wymuszenie meldunku WM_DESTROY
        		  break;
		case 102: MessageBox(Okno, "imi� i nazwisko: Katarzyna Wolszon\nnumer indeksu: s8133", "Student PJWSTK", MB_OK);
				  break;
		case 103: ii = 50;
				  break;
		case 104: ii = 0;
				  break;
		case 105: negatyw = false; //kolor podstawowy
				  break;
		case 106: negatyw = true; //negatyw
				  break;
		}
		return 0;
	
	  case WM_LBUTTONDOWN: // obs�uga wci�ni�cia lewego przycisku myszki
		leftMouse=true;
        rightMouse=false;
		if(lPunktow >= sizeof(Punkty)/sizeof(POINT)) lPunktow = 0; // przepe�nienie tablicy klikni��
		Punkty[lPunktow].x = LOWORD(lParam); // zapisanie wsp�rz�dnych kursora w tablicy klikni��
		Punkty[lPunktow].y = HIWORD(lParam);
		rozmiary[lPunktow] = ii;
		//if (negatyw) negatywT[lPunktow] = true;
		//else negatywT[lPunktow] = false;

		lPunktow++;
		InvalidateRect(Okno, NULL, leftMouse); // wymuszenie meldunku WM_PAINT
		return 0;

	case WM_RBUTTONDOWN: // obs�uga wci�ni�cia prawego przycisku myszki
        leftMouse=false;
        rightMouse=true;
        if(lPunktow2 >= sizeof(Punkty2)/sizeof(POINT)) lPunktow2 = 0; // przepe�nienie tablicy klikni��
        Punkty2[lPunktow2].x = LOWORD(lParam); // zapisanie wsp�rz�dnych kursora w tablicy klikni��
        Punkty2[lPunktow2].y = HIWORD(lParam);
        lPunktow2++;
        InvalidateRect(Okno, NULL, rightMouse); // wymuszenie meldunku WM_PAINT
        return 0;

	  case WM_PAINT: // obs�uga od�wie�enia okna
		  {	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			char Tekst[] = "Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon Katarzyna Wolszon";
			POINT Wielokat1[] = {{100, 100}, {120+i, 100}, {120+i, 150+i/2}, {170+i, 100},{190+i*2, 100}, {120+i, 170+i}, {190+i*2, 280+i}, {170+i, 280+i},{120+i, 200+i+i/2}, {120+i, 280+i}, {100, 280+i}};	
			POINT Wielokat2[] = {{200+i*2, 100}, {220+i*3, 100}, {220+i*3, 260}, {260+i*3, 240},{320+i*3, 260}, {320+i*3, 100}, {340+i*4, 100}, {340+i*4, 280+i},{320+i*3, 280+i}, {260+i*3, 260+i}, {220+i*3, 280+i}, {200+i*2, 280+i}};
			POINT Wielokat1m[] = {{100, 100}, {120, 100}, {120, 150}, {170, 100},{190, 100}, {120, 170}, {190, 280}, {170, 280},{120, 200}, {120, 280}, {100, 280}};
			POINT Wielokat2m[] = {{200, 100}, {220, 100}, {220, 260}, {260, 240},{320, 260}, {320, 100}, {340, 100}, {340, 280},{320, 280}, {260, 260}, {220, 280}, {200, 280}};
				
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony p�dzel
			HBRUSH Pedzel1 = CreateSolidBrush(RGB(0,0,255));
			HBRUSH Pedzel2 = CreateSolidBrush(RGB(0,255,255));
			HBRUSH Pedzel3 = CreateSolidBrush(RGB(255,255,0));
			HPEN Pioro2 = CreatePen(PS_SOLID, 2, RGB(0,255,255));
			HPEN Pioro3 = CreatePen(PS_SOLID, 2, RGB(255,255,0));
			SelectObject(Kontekst, GetStockObject(NULL_PEN));
			SetBkMode(Kontekst, TRANSPARENT);
			
			HRGN Region0 = CreatePolygonRgn(Wielokat1, 11, WINDING);
			HRGN Region1 =  CreatePolygonRgn(Wielokat2, 12, WINDING);
			HRGN Region2 = CreateRectRgn(0, 0, 0, 0); // pusty region
			//HRGN Region3 = CreatePolygonRgn(Wielokat1m, 11, WINDING);
			//HRGN Region4 =  CreatePolygonRgn(Wielokat2m, 12, WINDING);
			//HRGN RegionT = CreateRectRgn(0, 0, 0, 0); // pusty region
			
			for(int n=0; n<lPunktow; n++)
			{	
				
				OffsetRgn(Region0, Punkty[n].x-200, Punkty[n].y-200);
				OffsetRgn(Region1, Punkty[n].x-200, Punkty[n].y-200);
				CombineRgn(Region2, Region1, Region0, RGN_OR);
				//SelectClipRgn(Kontekst, Region2);
				//SelectObject(Kontekst, GetStockObject(WHITE_BRUSH)); // standardowy bia�y p�dzel
				//PaintRgn(Kontekst, Region2);
				//FrameRgn(Kontekst, Region2, Pedzel, 15, 15);
				SelectClipRgn(Kontekst, Region0);
				SelectObject(Kontekst, Pedzel);
				PaintRgn(Kontekst, Region0);
				SelectClipRgn(Kontekst, Region1);
				SelectObject(Kontekst, Pedzel1);
				PaintRgn(Kontekst, Region1);
				//CombineRgn(RegionT, RegionT, Region2, RGN_OR);
				SelectClipRgn(Kontekst, Region2);
				WORD Odstep = HIWORD(GetDialogBaseUnits());
				for(int y = 0; y < 850; y = y+Odstep) TextOut(Kontekst, 0, y, Tekst, strlen(Tekst));
				OffsetRgn(Region0, -Punkty[n].x+200, -Punkty[n].y+200);
				OffsetRgn(Region1, -Punkty[n].x+200, -Punkty[n].y+200);
			}

			 for(int j=0; j<lPunktow2; j++){    
				SetViewportOrgEx(Kontekst,Punkty2[j].x-500,Punkty2[j].y-400,NULL);
				SelectObject(Kontekst, Pioro2);
				Polygon(Kontekst, Wielokat1m, 11);
				SelectObject(Kontekst, Pioro3);
				Polygon(Kontekst, Wielokat2m, 12);
                

			 }

			DeleteObject(Pedzel);
			DeleteObject(Pedzel1);
			DeleteObject(Pedzel2);
			DeleteObject(Pedzel3);
			DeleteObject(Pioro2);
			DeleteObject(Pioro3);
			EndPaint(Okno, &Paint);
		}
		return 0;
  	
	  case WM_DESTROY: // obs�uga zamkni�cia okna - wygenerowanie meldunku WM_QUIT
		PostQuitMessage (0) ;
		return 0;
    
	  default: // standardowa obs�uga wszystkich pozosta�ych meldunk�w
		return DefWindowProc(Okno, KodMeldunku, wParam, lParam);
	}
}
