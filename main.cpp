#include "MyHeader.h"
#include "resource.h"

RECT g_displayRect;   // 전체화면 창 크기를 저장하기 위한 구조체
int g_xPosOfDroplings[MAX_NUM_OF_DROPLINGS];
double g_yPosOfDroplings[MAX_NUM_OF_DROPLINGS];
double g_dropStartTime[MAX_NUM_OF_DROPLINGS];
int g_amountOfDroplings;
double g_windowStartedTime;
double g_musicStartedTime;
TCHAR g_buffer[50];
int g_playerHealth;
BOOL g_isDroplingHit[MAX_NUM_OF_DROPLINGS];
int g_droplingsState[MAX_NUM_OF_DROPLINGS] = { WAITING };
HFONT g_oldFont, g_droplingsFont, g_infoFont, g_infoTitleFont, g_introFont;
HFONT g_scoreScreenTitleFont, g_scoreScreenFont, g_scoreScreenNumFont, g_scoreScreenExtendFont;
HFONT g_comboFont, g_comboTextFont;
HFONT g_hitEventFont, g_effectFont;
LOGFONT g_logFont;
double g_dropSpeed;
int g_score;
int g_hitEventCount;
int g_comboEventCount;
int g_displayHeight;
int g_displayWidth;
double g_toleranceGood;
double g_toleranceCool;
double g_tolerancePerfect;
RECT g_scoreTextRect, g_comboTextRect, g_speedTextRect, g_scoreText3DRect, g_comboText3DRect, g_speedText3DRect;
RECT g_comboEventRect, g_comboEvent3DRect, g_comboEventTextRect;
RECT g_hitEventRect, g_effectRect, g_syncTextRect, g_syncText3DRect;
RECT g_scoreScreenTitleRect, g_scoreScreenNumRect, g_IntroScreenRect;
int g_comboNum;
int g_maxComboNum;
int g_hitNum;
double g_currentTime;
BOOL g_isDropTimerOn = FALSE;
int g_yPosOfJudgementLine;
int g_xPosOfHitEvent;
int g_yPosOfHitEvent;
int g_xPosOfComboEvent;
int g_yPosOfComboEvent;
HINSTANCE g_hInst;
HBITMAP g_hHealthabarBM, g_hHealthEmptyBM, hMemBM, g_effectBlue, g_effectGreen, g_effectPink;
HBITMAP g_hFrozenBG, g_hGameOver1, g_hGameOver2, g_hFrozenOlaf, g_hIntroBG, g_hOneWayBG;
HBITMAP g_hFrozenCD, g_hOneWayCD, g_hLeftArrow, g_hRightArrow;
HBRUSH g_oldBrush, g_blackBrush;
HPEN g_oldPen, g_judgementLinePen, g_whitePen, g_uiPen, g_erasePen;
BOOL g_bGoodEvent, g_bCoolEvent, g_bPerfectEvent, g_bMissEvent;
BOOL g_bHitEffect[10];
int g_xPosOFHitEffect[10], g_yPosOFHitEffect[10], g_hitID[10];
double g_adjustSynk;
double g_GameOverEventCount;
BOOL g_scoreSoundPlayed, g_scoreScreenOn, g_gotScoreScreenStartTime, g_rankSoundPlayed;
char g_syncMark;
int g_numOfGood, g_numOfCool, g_numOfPerfect, g_numOfMiss;
double g_scoreScreenStartedTime, g_introStartedTime;
int g_textExtendCount[10];
int g_gameState = WAITING;
int g_selectedMusic, g_selectedLevel;
TCHAR g_lyric[304][14];
int g_cdX;
int g_swapDirection;
RECT g_menuSubTitleRect;
HFONT g_subTitleFont;
BOOL g_isBeepSoundPlayed[10];
int i, j;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	LPCTSTR windowClass = TEXT("Rhythm Alphabat");

	g_hInst = hInstance;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = windowClass;
	RegisterClass(&wndClass);

	hWnd = CreateWindow(windowClass, windowClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

////////////////////////////////////////////////////WinProc/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////WinProc/////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:
		g_displayHeight = GetSystemMetrics(SM_CYSCREEN);
		g_displayWidth = GetSystemMetrics(SM_CXSCREEN);
		g_displayRect = { 0, 0, g_displayWidth, g_displayHeight };

		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, NULL, 0, 0,
			g_displayRect.right - g_displayRect.left, g_displayRect.bottom - g_displayRect.top, SWP_SHOWWINDOW);
		FullScreenModeChange(g_displayWidth, g_displayHeight);
		ShowCursor(FALSE);

		hdc = GetDC(hWnd);
		hMemBM = CreateCompatibleBitmap(hdc, g_displayWidth, g_displayHeight);
		ReleaseDC(hWnd, hdc);

		g_windowStartedTime = GetTickCount() / 1000.0;		// 디버그용

		g_yPosOfJudgementLine = g_displayHeight * 0.9;
		g_xPosOfHitEvent = 200;
		g_yPosOfHitEvent = g_displayHeight * 0.55;
		g_xPosOfComboEvent = 100;
		g_yPosOfComboEvent = g_displayHeight * 0.2;

		g_dropSpeed = 4;
		g_adjustSynk = SYCN_DELAY;
		g_playerHealth = 30;
		g_syncMark = '+';
		g_cdX = g_displayWidth *0.5 - 210;
		g_toleranceGood = (6 - g_dropSpeed + 1 + TOLERANCE_GOOD) * TOLERANCE_ADJUST;
		g_toleranceCool = (6 - g_dropSpeed + 1 + TOLERANCE_COOL) * TOLERANCE_ADJUST;
		g_tolerancePerfect = (6 - g_dropSpeed + 1 + TOLERANCE_PERFECT) * TOLERANCE_ADJUST;

		g_hHealthabarBM = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEALTHBAR));
		g_hHealthEmptyBM = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HEALTHBAR_EMPTY));
		g_effectBlue = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HITEFFECT_BLUE));
		g_effectGreen = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HITEFFECT_GREEN));
		g_effectPink = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_HITEFFECT_PINK));
		g_hFrozenBG = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FROZEN));
		g_hGameOver1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GAMEOVER1));
		g_hGameOver2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GAMEOVER2));
		g_hFrozenOlaf = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_OLAF));
		g_hIntroBG = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_INTRO_BG));
		g_hOneWayBG = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ONEWAY));
		g_hFrozenCD = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FROZEN_CD));
		g_hOneWayCD = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ONEWAY_CD));
		g_hRightArrow = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_RIGHT_ARROW));
		g_hLeftArrow = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LEFT_ARROW));

		SetRect(&g_scoreTextRect, DROP_SECTION_WIDTH + 5, 0, DROP_SECTION_WIDTH + 150, 60);
		SetRect(&g_comboTextRect, DROP_SECTION_WIDTH + 5, 50, DROP_SECTION_WIDTH + 150, 110);
		SetRect(&g_speedTextRect, DROP_SECTION_WIDTH + 160, 0, DROP_SECTION_WIDTH + 230, 60);
		SetRect(&g_syncTextRect, DROP_SECTION_WIDTH + 160, 50, DROP_SECTION_WIDTH + 230, 110);
		SetRect(&g_scoreText3DRect, DROP_SECTION_WIDTH + 5 + 2, 0 + 2, DROP_SECTION_WIDTH + 150 + 2, 60 + 2);
		SetRect(&g_comboText3DRect, DROP_SECTION_WIDTH + 5 + 2, 50 + 2, DROP_SECTION_WIDTH + 150 + 2, 110 + 2);
		SetRect(&g_speedText3DRect, DROP_SECTION_WIDTH + 160 + 2, 0 + 2, DROP_SECTION_WIDTH + 230 + 2, 60 + 2);
		SetRect(&g_syncText3DRect, DROP_SECTION_WIDTH + 160 + 2, 50 + 2, DROP_SECTION_WIDTH + 230 + 2, 110 + 2);
		SetRect(&g_hitEventRect, g_xPosOfHitEvent, g_yPosOfHitEvent,
			DROP_SECTION_WIDTH - g_xPosOfHitEvent, g_yPosOfHitEvent + 200);
		SetRect(&g_comboEventRect, g_xPosOfComboEvent, g_yPosOfComboEvent,
			DROP_SECTION_WIDTH - g_xPosOfComboEvent, g_yPosOfComboEvent + 200);
		SetRect(&g_comboEventTextRect, g_xPosOfComboEvent, g_yPosOfComboEvent - 160,
			DROP_SECTION_WIDTH - g_xPosOfComboEvent, g_yPosOfComboEvent + 160 - 160);
		SetRect(&g_comboEvent3DRect, g_xPosOfComboEvent + 2, g_yPosOfComboEvent + 2,
			DROP_SECTION_WIDTH - g_xPosOfComboEvent + 2, g_yPosOfComboEvent + 160 + 2);
		SetRect(&g_scoreScreenTitleRect, 0, 0, g_displayWidth, g_displayHeight * 0.3);
		SetRect(&g_menuSubTitleRect, 0, g_displayHeight * 0.2 + 450, g_displayWidth, g_displayHeight * 0.2 + 550);

		g_logFont.lfWeight = 700;
		g_logFont.lfHeight = 30;
		g_logFont.lfWidth = 30 / 2;
		g_droplingsFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfWidth = 18;
		g_infoFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfWidth = 50;
		g_logFont.lfHeight = 80;
		g_scoreScreenTitleFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfWidth = 50;
		g_logFont.lfHeight = 100;
		g_introFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfWidth = 30;
		g_logFont.lfHeight = 40;
		g_scoreScreenFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfWeight = 500;
		g_logFont.lfHeight = 20;
		g_logFont.lfWidth = 10;
		g_infoTitleFont = CreateFontIndirect(&g_logFont);
		g_logFont.lfHeight = 30;
		g_logFont.lfWidth = 20;
		g_logFont.lfWeight = 700;
		g_subTitleFont = CreateFontIndirect(&g_logFont);

		g_whitePen = (HPEN)GetStockObject(WHITE_PEN);
		g_uiPen = (HPEN)CreatePen(PS_SOLID, 3, RGB(100, 100, 100));
		g_erasePen = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		g_blackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

		PlaySound("Resource/Intro_Music", NULL, SND_ASYNC);
		g_introStartedTime = GetTickCount() / 1000.0 + 1;
		SetTimer(hWnd, INTRO_TIMER, 10, NULL);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (g_gameState == INTRO)
			DrawIntroScreen(hdc);

		else if (g_gameState == SELECTING_MUSIC || g_gameState == SELECTING_LEVEL || g_gameState == SELECTING_SPEED)
			DrawMenu(hdc);

		else if (g_gameState == INGAME)
			DrawGame(hdc);

		else if (g_gameState == SCORE_SCREEN)
			ScoreScreen(hdc);

		if (g_playerHealth <= 0)
			GameOver(hWnd, hdc);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		switch (wParam)
		{
		case RUN_GAME_TIMER:
			RunGame(hWnd);
			break;
		case INTRO_TIMER:
			RunIntro(hWnd);
			break;
		case MENU_TIMER:
			RunMenu(hWnd);
			break;
		case CD_SWAP_TIMER:
			SwapCD(hWnd);
			break;
		}
		return 0;

	case WM_KEYDOWN:
		if (g_gameState == INGAME)
			HitAction(hWnd, wParam);

		if (g_gameState == INTRO && wParam != VK_ESCAPE)
		{
			g_gameState = SELECTING_MUSIC;
			g_selectedMusic = FROZEN;
			KillTimer(hWnd, INTRO_TIMER);
			SetTimer(hWnd, MENU_TIMER, 10, NULL);
			PlaySound("Resource/Frozen_CD", NULL, SND_ASYNC | SND_LOOP);
			return 0;
		}
		if (g_gameState == SCORE_SCREEN || g_gameState == GAMEOVER)
		{
			g_gameState = SELECTING_MUSIC;
			if (g_selectedMusic == FROZEN)
				PlaySound("Resource/Frozen_CD", NULL, SND_ASYNC | SND_LOOP);
			else 
				PlaySound("Resource/OneWay_CD", NULL, SND_ASYNC | SND_LOOP);
			return 0;
		}

		switch (wParam)
		{
		case VK_RIGHT:
			if (g_gameState == SELECTING_MUSIC && g_selectedMusic == FROZEN)
			{
				g_swapDirection = 1;
				SetTimer(hWnd, CD_SWAP_TIMER, 10, NULL);
				PlaySound("Resource/ONEWAY_CD", NULL, SND_ASYNC | SND_LOOP);
			}
			else if (g_gameState == SELECTING_MUSIC && g_selectedMusic == ONEWAY)
			{
				g_swapDirection = 1;
				SetTimer(hWnd, CD_SWAP_TIMER, 10, NULL);
				PlaySound("Resource/FROZEN_CD", NULL, SND_ASYNC | SND_LOOP);
			}
			else if (g_gameState == SELECTING_LEVEL)
			{
				if (g_selectedLevel == EASY)
					g_selectedLevel = HARD;
				else
					g_selectedLevel = EASY;
			}
			else if (g_gameState == SELECTING_SPEED || g_gameState == INGAME)
				IncreaseDropSpeed(hWnd);
			break;
		case VK_LEFT:
			if (g_gameState == SELECTING_MUSIC && g_selectedMusic == FROZEN)
			{
				g_swapDirection = -1;
				SetTimer(hWnd, CD_SWAP_TIMER, 10, NULL);
				PlaySound("Resource/ONEWAY_CD", NULL, SND_ASYNC | SND_LOOP);
			}
			else if (g_gameState == SELECTING_MUSIC && g_selectedMusic == ONEWAY)
			{
				g_swapDirection = -1;
				SetTimer(hWnd, CD_SWAP_TIMER, 10, NULL);
				PlaySound("Resource/FROZEN_CD", NULL, SND_ASYNC | SND_LOOP);
			}
			else if (g_gameState == SELECTING_LEVEL)
			{
				if (g_selectedLevel == EASY)
					g_selectedLevel = HARD;
				else
					g_selectedLevel = EASY;
			}
			else if (g_gameState == SELECTING_SPEED || g_gameState == INGAME)
				DecreaseDropSpeed(hWnd);
			break;
		case VK_UP:
			if (g_gameState == INGAME)
				SyncBackward();
			break;
		case VK_DOWN:
			if (g_gameState == INGAME)
				SyncForward();
			break;
		case VK_RETURN:
			if (g_gameState == SELECTING_MUSIC)
				g_gameState = SELECTING_LEVEL;
			else if (g_gameState == SELECTING_LEVEL)
				g_gameState = SELECTING_SPEED;
			else if (g_gameState == SELECTING_SPEED)
			{
				g_gameState = INGAME;
				Initialize(hWnd);
			}
			break;
		case VK_ESCAPE:
			if (g_gameState == SELECTING_LEVEL)
				g_gameState = SELECTING_MUSIC;
			else if (g_gameState == SELECTING_SPEED)
				g_gameState = SELECTING_LEVEL;
			else
				DestroyWindow(hWnd);
			break;
		}
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, RUN_GAME_TIMER);
		DeleteObject(g_judgementLinePen);
		DeleteObject(g_uiPen);
		DeleteObject(g_erasePen);
		DeleteObject(g_droplingsFont);
		DeleteObject(g_infoFont);
		DeleteObject(g_hitEventFont);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

