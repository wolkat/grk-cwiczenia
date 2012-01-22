/******************************************************************
* Multimedia: programowanie grafiki w œrodowisku MS Windows + GDI *
******************************************************************/

#include <windows.h>

// deklaracja funkcji okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// g³ówna funkcja programu
 int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// deklaracja zmiennych
	MSG Meldunek;
	WNDCLASS KlasaOkna;
	HWND Okno;
	static char NazwaKlasyOkna[] = "Okno";

	// definicja w³aœciwoœci definiowanej klasy okna i jej rejestracja
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
	
	// utworzenie i wyœwietlenie okna
 	Okno = CreateWindow(NazwaKlasyOkna, "PJWSTK Multimedia", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
   	ShowWindow (Okno, nCmdShow) ;
	UpdateWindow (Okno) ;

	// pêtla obs³ugi meldunków (g³ówna pêtla programu)
	while (GetMessage(&Meldunek, NULL, 0, 0))	// FALSE tylko dla komunikatu WM_QUIT
	{
		TranslateMessage(&Meldunek); // wstêpna obróbka meldunku
		DispatchMessage(&Meldunek);  // wywolanie funkcji okna na podstawie KlasaOkna.lpfnWndProc
	}
	return (int)Meldunek.wParam;
}

// funkcja okna zawieraj¹ca obs³ugê meldunków przesy³anych do okna
LRESULT CALLBACK WndProc (HWND Okno, UINT KodMeldunku, WPARAM wParam, LPARAM lParam)
{
	// deklaracja zmiennych
	static POINT Punkty[100]; // tablica klikniêæ
	static int rozmiary[100];
	static int lPunktow = 0;
	static int ii=0;
	HMENU mGlowne, mPlik, mRozmiar, mKolor, mInfo;
    	
	// obs³uga meldunku w zale¿noœci od kodu meldunku
	switch (KodMeldunku) 
	{ case WM_CREATE:  // obs³uga utworzenia okna - stworzenie menu
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Gong...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mRozmiar = CreateMenu();
		AppendMenu(mRozmiar, MF_STRING, 103, "&Du¿e");
		AppendMenu(mRozmiar, MF_STRING, 104, "&Ma³e");
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

		Punkty[lPunktow].x=200; // wype³nienie pierwszej pozycji tablicy klikniêæ
		Punkty[lPunktow].y=200;
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;

	  case WM_COMMAND: // obs³uga wyboru opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(Okno, "Czy wygenerowaæ gong?", "Gong", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(Okno); // wymuszenie meldunku WM_DESTROY
        		  break;
		case 102: MessageBox(Okno, "imiê i nazwisko: Katarzyna Wolszon\nnumer indeksu: s8133", "Student PJWSTK", MB_OK);
				  break;
		case 103: ii = 50;
				  break;
		case 104: ii = 0;
				  break;

		}
		return 0;
	
	  case WM_LBUTTONDOWN: // obs³uga wciœniêcia lewego przycisku myszki
		if(lPunktow >= sizeof(Punkty)/sizeof(POINT)) lPunktow = 0; // przepe³nienie tablicy klikniêæ
		Punkty[lPunktow].x = LOWORD(lParam); // zapisanie wspó³rzêdnych kursora w tablicy klikniêæ
		Punkty[lPunktow].y = HIWORD(lParam);
		rozmiary[lPunktow] = ii;
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;
	
	  case WM_PAINT: // obs³uga odœwie¿enia okna
		{	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			HPEN Pioro = CreatePen(PS_SOLID, 15, RGB(255,0,0)); // czerwone pióro o gruboœci 15
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony pêdzel
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
			DeleteObject(Pioro); // usuniêcie niepotrzebnych przyborów graficznych
			DeleteObject(Pedzel);
			DeleteObject(Pioro1); 
			DeleteObject(Pedzel1);
			EndPaint(Okno, &Paint);
		}
		return 0;
  	
	  case WM_DESTROY: // obs³uga zamkniêcia okna - wygenerowanie meldunku WM_QUIT
		PostQuitMessage (0) ;
		return 0;
    
	  default: // standardowa obs³uga wszystkich pozosta³ych meldunków
		return DefWindowProc(Okno, KodMeldunku, wParam, lParam);
	}
}
