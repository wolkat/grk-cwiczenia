/***************************************************************************
* Grafika komputerowa: programowanie grafiki w œrodowisku MS Windows + GDI *
***************************************************************************/
// Zadanie 2

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
 	Okno = CreateWindow(NazwaKlasyOkna, "PJWSTK Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
	static int lPunktow = 0;
	HMENU mGlowne, mPlik, mInfo;
    	
	// obs³uga meldunku w zale¿noœci od kodu meldunku
	switch (KodMeldunku) 
	{ case WM_CREATE:  // obs³uga utworzenia okna - stworzenie menu
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
		case 102: MessageBox(Okno, "imiê i nazwisko:\nnumer indeksu: ", "Student PJWSTK", MB_OK);
		}
		return 0;
	
	  case WM_LBUTTONDOWN: // obs³uga wciœniêcia lewego przycisku myszki
		if(lPunktow >= sizeof(Punkty)/sizeof(POINT)) lPunktow = 0; // przepe³nienie tablicy klikniêæ
		Punkty[lPunktow].x = LOWORD(lParam); // zapisanie wspó³rzêdnych kursora w tablicy klikniêæ
		Punkty[lPunktow].y = HIWORD(lParam);
		lPunktow++;
		InvalidateRect(Okno, NULL, TRUE); // wymuszenie meldunku WM_PAINT
		return 0;
	
	  case WM_PAINT: // obs³uga odœwie¿enia okna
		{	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			HPEN Pioro = CreatePen(PS_SOLID, 15, RGB(255,0,0)); // czerwone pióro o gruboœci 15
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony pêdzel

			for(int i=0; i<lPunktow; i++)
			{	SetViewportOrgEx(Kontekst,Punkty[i].x-200,Punkty[i].y-200,NULL);

				SelectObject(Kontekst, Pioro);
				SelectObject(Kontekst, GetStockObject(WHITE_BRUSH)); // standardowy bia³y pêdzel
				Ellipse(Kontekst, 100, 100, 300, 300);	// narysowanie okrêgu za pomoc¹ aktywnych pióra i pêdzla

				SelectObject(Kontekst, Pedzel);
				POINT wielokat[] = {{106, 200}, {118, 248}, {152, 282}, {200, 294}, {248, 282}, {282, 248}, {294, 200}};
				Polygon(Kontekst, wielokat, 7);  // narysowanie wielok¹ta wype³niaj¹cego doln¹ po³owê okrêgu na czerwono
			}
			DeleteObject(Pioro); // usuniêcie niepotrzebnych przyborów graficznych
			DeleteObject(Pedzel);
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
