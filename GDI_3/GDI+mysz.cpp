/***************************************************************************
* Grafika komputerowa: programowanie grafiki w �rodowisku MS Windows + GDI *
***************************************************************************/
// Zadanie 2

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
	static int lPunktow = 0;
	HMENU mGlowne, mPlik, mInfo;
    	
	// obs�uga meldunku w zale�no�ci od kodu meldunku
	switch (KodMeldunku) 
	{ case WM_CREATE:  // obs�uga utworzenia okna - stworzenie menu
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Gong...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 102, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
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
		case 102: MessageBox(Okno, "imi� i nazwisko:\nnumer indeksu: ", "Student PJWSTK", MB_OK);
		}
		return 0;
	
	  case WM_LBUTTONDOWN: // obs�uga wci�ni�cia lewego przycisku myszki
		if(lPunktow >= sizeof(Punkty)/sizeof(POINT)) lPunktow = 0; // przepe�nienie tablicy klikni��
		Punkty[lPunktow].x = LOWORD(lParam); // zapisanie wsp�rz�dnych kursora w tablicy klikni��
		Punkty[lPunktow].y = HIWORD(lParam);
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;
	
	  case WM_PAINT: // obs�uga od�wie�enia okna
		{	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			HPEN Pioro = CreatePen(PS_SOLID, 15, RGB(255,0,0)); // czerwone pi�ro o grubo�ci 15
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony p�dzel

			for(int i=0; i<lPunktow; i++)
			{	SetViewportOrgEx(Kontekst,Punkty[i].x-200,Punkty[i].y-200,NULL);

				SelectObject(Kontekst, Pioro);
				SelectObject(Kontekst, GetStockObject(WHITE_BRUSH)); // standardowy bia�y p�dzel
				Ellipse(Kontekst, 100, 100, 300, 300);	// narysowanie okr�gu za pomoc� aktywnych pi�ra i p�dzla

				SelectObject(Kontekst, Pedzel);
				POINT wielokat[] = {{106, 200}, {118, 248}, {152, 282}, {200, 294}, {248, 282}, {282, 248}, {294, 200}};
				Polygon(Kontekst, wielokat, 7);  // narysowanie wielok�ta wype�niaj�cego doln� po�ow� okr�gu na czerwono
			}
			DeleteObject(Pioro); // usuni�cie niepotrzebnych przybor�w graficznych
			DeleteObject(Pedzel);
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
