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
		return 0;

	  case WM_COMMAND: // obs³uga wyboru opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(Okno, "Czy wygenerowaæ gong?", "Gong", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(Okno); // wymuszenie meldunku WM_DESTROY
        		  break;
		case 102: MessageBox(Okno, "imiê i nazwisko: Katarzyna Wolszon\nnumer indeksu: 8133", "Student PJWSTK", MB_OK);
		}
		return 0;
	
	  case WM_PAINT: // obs³uga odœwie¿enia okna
		{	PAINTSTRUCT Paint;
			HDC Kontekst = BeginPaint(Okno, &Paint);
			HPEN Pioro = CreatePen(PS_SOLID, 15, RGB(255,0,0)); // czerwone pióro o gruboœci 15
			HBRUSH Pedzel = CreateSolidBrush(RGB(255,0,0)); // czerwony pêdzel
			HPEN Pioro1 = CreatePen(PS_SOLID, 2, RGB(123,200,0)); 
			HBRUSH Pedzel1 = CreateSolidBrush(RGB(0,34,255));

			SelectObject(Kontekst, Pioro);
			// SelectObject(Kontekst, GetStockObject(WHITE_BRUSH)); // standardowy bia³y pêdzel
			Ellipse(Kontekst, 100, 100, 300, 300);	// narysowanie okrêgu za pomoc¹ aktywnych pióra i pêdzla

			SelectObject(Kontekst, Pedzel);
			POINT wielokat[] = {{106, 200}, {118, 248}, {152, 282}, {200, 294}, {248, 282}, {282, 248}, {294, 200}};
			Polygon(Kontekst, wielokat, 7);  // narysowanie wielok¹ta wype³niaj¹cego doln¹ po³owê okrêgu na czerwono

			SetBkMode(Kontekst, TRANSPARENT); // wypisanie tekstu
			SetTextAlign(Kontekst, TA_CENTER | TA_BOTTOM);
			// SetTextColor(Kontekst, RGB(0,0,0));
			TextOut(Kontekst, 200, 200, "P J W S T K", 11); 
			///////////////////////////////////////////////
			SelectObject(Kontekst, Pedzel1);
			SelectObject(Kontekst, Pioro1);
			//MoveToEx(Kontekst,400,100,0);
			//LineTo(Kontekst,400,300);
			//POINT wielokat1[] = {{400, 100}, {410, 100}, {410, 300}, {400, 300}};
			POINT wielokat1[] = {{400, 100}, {420, 100}, {420, 150}, {470, 100},{490, 100}, {420, 170}, {490, 280}, {470, 280},{420, 200}, {420, 280}, {400, 280}};
			Polygon(Kontekst, wielokat1, 11); 
			SelectObject(Kontekst, Pedzel);
			SelectObject(Kontekst, Pioro1);
			POINT wielokat2[] = {{500, 100}, {520, 100}, {520, 260}, {560, 240},{620, 260}, {620, 100}, {640, 100}, {640, 280},{620, 280}, {560, 260}, {520, 280}, {500, 280}};
			Polygon(Kontekst, wielokat2, 12); 


			///////////////////////////////////////////////
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
