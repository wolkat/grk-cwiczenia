/******************************************************************
* Multimedia: programowanie grafiki w �rodowisku MS Windows + GDI *
******************************************************************/

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
 	Okno = CreateWindow(NazwaKlasyOkna, "PJWSTK Multimedia", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
	static int rozmiary[100];
	static int lPunktow = 0;
	static int ii=0;
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

		}
		return 0;
	
	  case WM_LBUTTONDOWN: // obs�uga wci�ni�cia lewego przycisku myszki
		if(lPunktow >= sizeof(Punkty)/sizeof(POINT)) lPunktow = 0; // przepe�nienie tablicy klikni��
		Punkty[lPunktow].x = LOWORD(lParam); // zapisanie wsp�rz�dnych kursora w tablicy klikni��
		Punkty[lPunktow].y = HIWORD(lParam);
		rozmiary[lPunktow] = ii;
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;
	
	  case WM_PAINT: // obs�uga od�wie�enia okna
		{	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			HPEN Pioro = CreatePen(PS_SOLID, 15, RGB(255,0,0)); // czerwone pi�ro o grubo�ci 15
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony p�dzel
			HPEN Pioro1 = CreatePen(PS_SOLID, 2, RGB(123,200,0)); 
			HBRUSH Pedzel1 = CreateSolidBrush(RGB(0,34,255));
			
			for(int k=0; k<lPunktow; k++)
			{	
				SetViewportOrgEx(Kontekst,Punkty[k].x-200,Punkty[k].y-200,NULL);
				
				int i = rozmiary[k];
				SelectObject(Kontekst, Pedzel1);
				SelectObject(Kontekst, Pioro1);
				POINT wielokat1[] = {{100, 100}, {120+i, 100}, {120+i, 150+i}, {170+i, 100},{190+i, 100}, {120+i, 170+i}, {190+i, 280+i}, {170+i, 280+i},{120+i, 200+i}, {120+i, 280+i}, {100, 280+i}};
				Polygon(Kontekst, wielokat1, 11); 
				SelectObject(Kontekst, Pedzel);
				SelectObject(Kontekst, Pioro1);
				POINT wielokat2[] = {{200, 100}, {220, 100}, {220, 260}, {260, 240},{320, 260}, {320, 100}, {340, 100}, {340, 280},{320, 280}, {260, 260}, {220, 280}, {200, 280}};
				Polygon(Kontekst, wielokat2, 12); 


			///////////////////////////////////////////////
			
				

			}
			DeleteObject(Pioro); // usuni�cie niepotrzebnych przybor�w graficznych
			DeleteObject(Pedzel);
			DeleteObject(Pioro1); 
			DeleteObject(Pedzel1);
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
