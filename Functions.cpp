#define _CRT_SECURE_NO_WARNINGS
#include "MyHeader.h"
#include "Frozen.h"
#include "Frozen_Easy.h"
#include "Oneway.h"
#include "Oneway_Easy.h"

extern RECT g_displayRect;   // 전체화면 창 크기를 저장하기 위한 구조체
extern int g_xPosOfDroplings[MAX_NUM_OF_DROPLINGS];
extern double g_yPosOfDroplings[MAX_NUM_OF_DROPLINGS];
extern double g_dropStartTime[MAX_NUM_OF_DROPLINGS];
extern int g_amountOfDroplings;
extern double g_windowStartedTime;
extern double g_musicStartedTime;
extern TCHAR g_buffer[50];
extern int g_playerHealth;
extern BOOL g_isDroplingHit[MAX_NUM_OF_DROPLINGS];
extern int g_droplingsState[MAX_NUM_OF_DROPLINGS];
extern HFONT g_oldFont, g_droplingsFont, g_infoFont, g_infoTitleFont, g_introFont;
extern HFONT g_scoreScreenTitleFont, g_scoreScreenFont, g_scoreScreenNumFont, g_scoreScreenExtendFont;
extern HFONT g_comboFont, g_comboTextFont;
extern HFONT g_hitEventFont, g_effectFont;
extern LOGFONT g_logFont;
extern double g_dropSpeed;
extern int g_score;
extern int g_hitEventCount;
extern int g_comboEventCount;
extern int g_displayHeight;
extern int g_displayWidth;
extern double g_toleranceGood;
extern double g_toleranceCool;
extern double g_tolerancePerfect;
extern RECT g_scoreTextRect, g_comboTextRect, g_speedTextRect, g_scoreText3DRect, g_comboText3DRect, g_speedText3DRect;
extern RECT g_comboEventRect, g_comboEvent3DRect, g_comboEventTextRect;
extern RECT g_hitEventRect, g_effectRect, g_syncTextRect, g_syncText3DRect;
extern RECT g_scoreScreenTitleRect, g_scoreScreenNumRect, g_IntroScreenRect;
extern int g_comboNum;
extern int g_maxComboNum;
extern int g_hitNum;
extern double g_currentTime;
extern BOOL g_isDropTimerOn;
extern int g_yPosOfJudgementLine;
extern int g_xPosOfHitEvent;
extern int g_yPosOfHitEvent;
extern int g_xPosOfComboEvent;
extern int g_yPosOfComboEvent;
extern HINSTANCE g_hInst;
extern HBITMAP g_hHealthabarBM, g_hHealthEmptyBM, hMemBM, g_effectBlue, g_effectGreen, g_effectPink;
extern HBITMAP g_hFrozenBG, g_hGameOver1, g_hGameOver2, g_hFrozenOlaf, g_hIntroBG, g_hOneWayBG;
extern HBITMAP g_hFrozenCD, g_hOneWayCD, g_hLeftArrow, g_hRightArrow;
extern HBRUSH g_oldBrush, g_blackBrush;
extern HPEN g_oldPen, g_judgementLinePen, g_whitePen, g_uiPen, g_erasePen;
extern BOOL g_bGoodEvent, g_bCoolEvent, g_bPerfectEvent, g_bMissEvent;
extern BOOL g_bHitEffect[10];
extern int g_xPosOFHitEffect[10], g_yPosOFHitEffect[10], g_hitID[10];
extern double g_adjustSynk;
extern double g_GameOverEventCount;
extern BOOL g_scoreSoundPlayed, g_scoreScreenOn, g_gotScoreScreenStartTime, g_rankSoundPlayed;
extern char g_syncMark;
extern int g_numOfGood, g_numOfCool, g_numOfPerfect, g_numOfMiss;
extern double g_scoreScreenStartedTime, g_introStartedTime;
extern int g_textExtendCount[10];
extern int g_gameState;
extern int g_selectedMusic, g_selectedLevel;
extern TCHAR g_lyric[304][14];
extern int g_cdX;
extern int g_swapDirection;
extern RECT g_menuSubTitleRect;
extern HFONT g_subTitleFont;
extern BOOL g_isBeepSoundPlayed[10];
extern int i, j;

////////////////////////////////////////////////////////////////// 함수 정의 ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// 함수 정의 ///////////////////////////////////////////////////////
// 전체화면 함수 정의
void FullScreenModeChange(int nWidth, int nHeight)
{
	DEVMODE sDevMode;
	sDevMode.dmSize = sizeof(DEVMODE);
	sDevMode.dmPelsWidth = nWidth;
	sDevMode.dmPelsHeight = nHeight;
	sDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&sDevMode, CDS_FULLSCREEN);
}

void RunGame(HWND hWnd)
{
	double currentTime = GetTickCount() / 1000.0;

	for (i = 0; i < g_amountOfDroplings; i++)
	{
		if (g_dropStartTime[i] < currentTime - g_musicStartedTime && g_droplingsState[i] == WAITING)
			g_droplingsState[i] = DROPING;
	}

	if (currentTime - g_musicStartedTime > g_dropStartTime[g_amountOfDroplings - 1] + 11 && g_gameState == INGAME)
		g_gameState = SCORE_SCREEN;

	InvalidateRect(hWnd, NULL, FALSE);
}

// 글자들 떨어지게하는 함수
void DroplingsDrop(HDC hdc, int whichDropling)
{
	double currentTime;
	g_oldFont = (HFONT)SelectObject(hdc, g_droplingsFont);

	if (g_yPosOfDroplings[whichDropling] > g_yPosOfJudgementLine + g_toleranceGood &&
		g_isDroplingHit[whichDropling] != TRUE)	// 글자를 맞추지 못하면 발생하는 일
	{
		g_droplingsState[i] = FINISHED;
		g_comboNum = 0;
		g_hitNum = 0;
		g_playerHealth--;
		g_bGoodEvent = FALSE;
		g_bCoolEvent = FALSE;
		g_bPerfectEvent = FALSE;
		g_bMissEvent = TRUE;
		g_numOfMiss++;
		g_hitEventCount = 0;
	}
	else
	{
		currentTime = GetTickCount() / 1000.0;
		g_yPosOfDroplings[whichDropling] = (currentTime - g_musicStartedTime - g_dropStartTime[whichDropling])
			* g_yPosOfJudgementLine / g_dropSpeed;		// 글자들의 y값을 증가시킴 (by Time)

		if (g_isDroplingHit[whichDropling] != TRUE)	// hit를 한 글자는 출력 안하게
		{
			SetTextColor(hdc, RGB(70, 70, 70));		// 단어 3D 효과
			TextOut(hdc, g_xPosOfDroplings[whichDropling] + 2, g_yPosOfDroplings[whichDropling] + 2,
				g_lyric[whichDropling], lstrlen(g_lyric[whichDropling]));
			SetTextColor(hdc, RGB(255, 255, 255));		//  단어 전체 출력
			TextOut(hdc, g_xPosOfDroplings[whichDropling], g_yPosOfDroplings[whichDropling],
				g_lyric[whichDropling], lstrlen(g_lyric[whichDropling]));

			SetTextColor(hdc, RGB(255, 20, 255));		// 단어의 첫글자 Highlight 출력
			TextOut(hdc, g_xPosOfDroplings[whichDropling], g_yPosOfDroplings[whichDropling],
				g_lyric[whichDropling], 1);
		}
	}
	SelectObject(hdc, g_oldFont);
}

void IncreaseDropSpeed(HWND hWnd)
{
	if (g_dropSpeed - 1 >= MAX_DROP_SPEED)
	{
		g_dropSpeed--;
		g_toleranceGood = (6 - g_dropSpeed + 1 + TOLERANCE_GOOD) * TOLERANCE_ADJUST;
		g_toleranceCool = (6 - g_dropSpeed + 1 + TOLERANCE_COOL) * TOLERANCE_ADJUST;
		g_tolerancePerfect = (6 - g_dropSpeed + 1 + TOLERANCE_PERFECT) * TOLERANCE_ADJUST;

		for (i = 0; i < g_amountOfDroplings; i++)
		{
			if (g_selectedMusic == ONEWAY && g_selectedLevel == HARD)
				g_dropStartTime[i] = g_oneWayDropTime[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == FROZEN && g_selectedLevel == HARD)
				g_dropStartTime[i] = g_forzenDropTime[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == ONEWAY && g_selectedLevel == EASY)
				g_dropStartTime[i] = g_oneWayDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == FROZEN && g_selectedLevel == EASY)
				g_dropStartTime[i] = g_forzenDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		}
	}
}

void DecreaseDropSpeed(HWND hWnd)
{
	if (g_dropSpeed + 1 <= MIN_DROP_SPEED)
	{
		g_dropSpeed++;
		g_toleranceGood = (6 - g_dropSpeed + 1 + TOLERANCE_GOOD) * TOLERANCE_ADJUST;
		g_toleranceCool = (6 - g_dropSpeed + 1 + TOLERANCE_COOL) * TOLERANCE_ADJUST;
		g_tolerancePerfect = (6 - g_dropSpeed + 1 + TOLERANCE_PERFECT) * TOLERANCE_ADJUST;

		for (i = 0; i < g_amountOfDroplings; i++)
		{
			if (g_selectedMusic == ONEWAY && g_selectedLevel == HARD)
				g_dropStartTime[i] = g_oneWayDropTime[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == FROZEN && g_selectedLevel == HARD)
				g_dropStartTime[i] = g_forzenDropTime[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == ONEWAY && g_selectedLevel == EASY)
				g_dropStartTime[i] = g_oneWayDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
			else if (g_selectedMusic == FROZEN && g_selectedLevel == EASY)
				g_dropStartTime[i] = g_forzenDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		}
	}
}

void HitAction(HWND hWnd, WPARAM &wParam)
{
	for (i = 0; i < g_amountOfDroplings; i++)  // 키보드를 타이밍에 맞게, 그리고 알맞은 알파벳을 눌렸는지 판단 
	{
		if (g_droplingsState[i] == DROPING &&
			g_yPosOfDroplings[i] >= g_yPosOfJudgementLine - g_toleranceGood &&
			g_yPosOfDroplings[i] <= g_yPosOfJudgementLine + g_toleranceGood &&
			wParam == g_lyric[i][0] && g_isDroplingHit[i] == FALSE)
		{
			// 맞았으면 할 것들
			g_bGoodEvent = FALSE;
			g_bCoolEvent = FALSE;
			g_bPerfectEvent = FALSE;
			g_bMissEvent = FALSE;
			if (g_yPosOfDroplings[i] >= g_yPosOfJudgementLine - g_tolerancePerfect &&
				g_yPosOfDroplings[i] <= g_yPosOfJudgementLine + g_tolerancePerfect)
			{
				g_bPerfectEvent = TRUE;
				g_numOfPerfect++;
				if (g_comboNum >= 60)
					g_score += 3000;
				else if (g_comboNum >= 30)
					g_score += 2000;
				else if (g_comboNum >= 10)
					g_score += 1500;
				else
					g_score += 1000;
			}
			else if (g_yPosOfDroplings[i] >= g_yPosOfJudgementLine - g_toleranceCool &&
				g_yPosOfDroplings[i] <= g_yPosOfJudgementLine + g_toleranceCool)
			{
				g_bCoolEvent = TRUE;
				g_numOfCool++;
				if (g_comboNum >= 60)
					g_score += 2000;
				else if (g_comboNum >= 30)
					g_score += 1500;
				else if (g_comboNum >= 10)
					g_score += 1000;
				else
					g_score += 700;
			}
			else
			{
				g_bGoodEvent = TRUE;
				g_numOfGood++;
				if (g_comboNum >= 60)
					g_score += 1500;
				else if (g_comboNum >= 30)
					g_score += 1000;
				else if (g_comboNum >= 10)
					g_score += 700;
				else
					g_score += 500;
			}

			for (j = 0; j < 10; j++)
			{
				if (g_bHitEffect[j] == FALSE)
				{
					g_xPosOFHitEffect[j] = g_xPosOfDroplings[i];
					g_yPosOFHitEffect[j] = g_yPosOfDroplings[i];
					g_hitID[j] = i;
					g_bHitEffect[j] = TRUE;
					break;
				}
			}
			g_isDroplingHit[i] = TRUE;
			g_hitEventCount = 0;
			g_comboEventCount = 0;
			g_hitNum++;

			if (g_hitNum > 1)
			{
				g_comboNum++;
				if (g_maxComboNum < g_comboNum)
					g_maxComboNum = g_comboNum;
			}

			if (g_hitNum % 3 == 0)
			{
				if (g_playerHealth < 30)
					g_playerHealth++;
			}

			break;
		}
	}
}

void DrawGame(HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hMemBM);
	SetBkMode(memDC, TRANSPARENT);
	g_oldBrush = (HBRUSH)SelectObject(memDC, g_blackBrush);
	g_oldPen = (HPEN)SelectObject(memDC, g_uiPen);

	if (g_selectedMusic == FROZEN)
		DrawBitmap(memDC, -300, 0, g_hFrozenBG);
	else
		DrawBitmap(memDC, 100, -100, g_hOneWayBG);

	Rectangle(memDC, 0, 0, DROP_SECTION_WIDTH, g_displayHeight);	// Drop 영역을 검은색으로 지움

	// 판단 선
	DrawJudgementLine(memDC);

	// 윗트임, 밑트임
	SelectObject(memDC, g_erasePen);
	MoveToEx(memDC, 2, 0, NULL);
	LineTo(memDC, DROP_SECTION_WIDTH - 2, 0);
	MoveToEx(memDC, 2, g_displayHeight - 1, NULL);
	LineTo(memDC, DROP_SECTION_WIDTH - 2, g_displayHeight - 1);
	SelectObject(memDC, g_uiPen);

	// 체력바
	HealthBarMovement(memDC);

	// 점수, 콤보수, 속도
	DrawGameUI(memDC);


	// 콤보 이벤트
	ComboEvent(memDC);

	// Hit 이벤트
	if (g_bGoodEvent == TRUE)
		GoodEvent(memDC);
	if (g_bCoolEvent == TRUE)
		CoolEvent(memDC);
	if (g_bPerfectEvent == TRUE)
		PerfectEvent(memDC);
	if (g_bMissEvent == TRUE)
		MissEvent(memDC);

	// 글자 Drop
	for (i = 0; i < g_amountOfDroplings; i++)
	{
		if (g_droplingsState[i] == DROPING)
			DroplingsDrop(memDC, i);
	}

	// HIT 이펙트
	for (i = 0; i < 5; i++)
	{
		if (g_bHitEffect[i] == TRUE)
			HitEffect(memDC, i);
	}

	BitBlt(hdc, 0, 0, g_displayWidth, g_displayHeight, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, g_oldPen);
	SelectObject(memDC, oldBitmap);
	SelectObject(memDC, g_oldBrush);
	DeleteObject(memDC);
}

void Initialize(HWND hWnd)
{
	for (i = 0; i < 304; i++)
	{
		for (j = 0; j < 14; j++)
			g_lyric[i][j] = '\0';
	}

	if (g_selectedMusic == ONEWAY && g_selectedLevel == HARD)
	{
		g_amountOfDroplings = sizeof(g_oneWayDropTime) / sizeof(double);
		for (i = 0; i < g_amountOfDroplings; i++)
		{
			for (j = 0; j < lstrlen(g_oneWayLyric[i]); j++)
				g_lyric[i][j] = g_oneWayLyric[i][j];

			g_xPosOfDroplings[i] = STANDARD_X_POS_OF_DROPLINGS + g_xPosOfDroplings_ONEWAY[i] * SECTION_DISTANCE;
			g_dropStartTime[i] = g_oneWayDropTime[i] - g_dropSpeed + g_adjustSynk;
		}

		PlaySound(TEXT("Resource/Hillsong - One Way.wav"), NULL, SND_ASYNC);
	}
	else if (g_selectedMusic == FROZEN && g_selectedLevel == HARD)
	{
		g_amountOfDroplings = sizeof(g_forzenDropTime) / sizeof(double);

		for (i = 0; i < g_amountOfDroplings; i++)
		{
			for (j = 0; j < lstrlen(g_frozenLyric[i]); j++)
				g_lyric[i][j] = g_frozenLyric[i][j];

			g_xPosOfDroplings[i] = STANDARD_X_POS_OF_DROPLINGS + g_xPosOfDroplings_FROZEN[i] * SECTION_DISTANCE;
			g_dropStartTime[i] = g_forzenDropTime[i] - g_dropSpeed + g_adjustSynk;
		}

		PlaySound(TEXT("Resource/Frozen - Let it Go.wav"), NULL, SND_ASYNC);
	}
	else if (g_selectedMusic == ONEWAY && g_selectedLevel == EASY)
	{
		g_amountOfDroplings = sizeof(g_oneWayDropTime_easy) / sizeof(double);

		for (i = 0; i < g_amountOfDroplings; i++)
		{
			for (j = 0; j < lstrlen(g_oneWayLyric_easy[i]); j++)
				g_lyric[i][j] = g_oneWayLyric_easy[i][j];

			g_xPosOfDroplings[i] = STANDARD_X_POS_OF_DROPLINGS + g_xPosOfDroplings_ONEWAY_easy[i] * SECTION_DISTANCE;
			g_dropStartTime[i] = g_oneWayDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		}

		PlaySound(TEXT("Resource/Hillsong - One Way.wav"), NULL, SND_ASYNC);
	}

	else if (g_selectedMusic == FROZEN && g_selectedLevel == EASY)
	{
		g_amountOfDroplings = sizeof(g_forzenDropTime_easy) / sizeof(double);

		for (i = 0; i < g_amountOfDroplings; i++)
		{
			for (j = 0; j < lstrlen(g_frozenLyric_easy[i]); j++)
				g_lyric[i][j] = g_frozenLyric_easy[i][j];

			g_xPosOfDroplings[i] = STANDARD_X_POS_OF_DROPLINGS + g_xPosOfDroplings_FROZEN_easy[i] * SECTION_DISTANCE;
			g_dropStartTime[i] = g_forzenDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		}

		PlaySound(TEXT("Resource/Frozen - Let it Go.wav"), NULL, SND_ASYNC);
	}

	for (i = 0; i < g_amountOfDroplings; i++)
	{
		if (g_droplingsState[i] == WAITING) break;
		g_droplingsState[i] = WAITING;
		g_yPosOfDroplings[i] = 0;
		g_isDroplingHit[i] = FALSE;
	}

	g_currentTime = 0;
	g_playerHealth = 30;
	g_comboNum = 0;
	g_maxComboNum = 0;
	g_score = 0;
	g_hitNum = 0;
	g_bGoodEvent = FALSE;
	g_bCoolEvent = FALSE;
	g_bPerfectEvent = FALSE;
	g_bMissEvent = FALSE;
	g_GameOverEventCount = 0;
	g_scoreScreenOn = FALSE;
	g_scoreSoundPlayed = FALSE;
	g_gotScoreScreenStartTime = FALSE;
	g_rankSoundPlayed = FALSE;
	g_numOfMiss = g_numOfGood = g_numOfCool = g_numOfPerfect = 0;
	for (i = 0; i < 10; i++)
	{
		g_textExtendCount[i] = 0;
		g_isBeepSoundPlayed[i] = FALSE;
	}
	g_musicStartedTime = GetTickCount() / 1000.0;
	KillTimer(hWnd, MENU_TIMER);
	SetTimer(hWnd, RUN_GAME_TIMER, 10, NULL);
	g_gameState = INGAME;
}

void ComboEvent(HDC hdc)
{
	if (g_comboNum != 0)
	{
		if (g_comboEventCount < 10)
		{
			g_logFont.lfHeight = 200 - g_comboEventCount * 14;
			g_logFont.lfWidth = 120 - g_comboEventCount * 6;
			g_comboFont = CreateFontIndirect(&g_logFont);
			g_logFont.lfHeight = 160 - g_comboEventCount * 13;
			g_logFont.lfWidth = 60 - g_comboEventCount * 4;
			g_comboTextFont = CreateFontIndirect(&g_logFont);
		}
		else
		{
			g_logFont.lfHeight = 60;
			g_logFont.lfWidth = 60;
			g_comboFont = CreateFontIndirect(&g_logFont);
			g_logFont.lfHeight = 30;
			g_logFont.lfWidth = 20;
			g_comboTextFont = CreateFontIndirect(&g_logFont);
		}

		g_oldFont = (HFONT)SelectObject(hdc, g_comboTextFont);
		SetTextColor(hdc, RGB(150 + 60 * (rand() % 4), 150 + 60 * (rand() % 4), 150 + 60 * (rand() % 4)));
		DrawText(hdc, "COMBO", 5, &g_comboEventTextRect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);

		SelectObject(hdc, g_comboFont);
		SetTextColor(hdc, RGB(100, 100, 100));
		sprintf(g_buffer, TEXT("%d"), g_comboNum);
		DrawText(hdc, g_buffer, lstrlen(g_buffer), &g_comboEvent3DRect, DT_SINGLELINE | DT_CENTER | DT_TOP);
		SetTextColor(hdc, RGB(200, 200, 200));
		DrawText(hdc, g_buffer, lstrlen(g_buffer), &g_comboEventRect, DT_SINGLELINE | DT_CENTER | DT_TOP);

		g_comboEventCount++;

		(HFONT)SelectObject(hdc, g_oldFont);
		DeleteObject(g_comboFont);
		DeleteObject(g_comboTextFont);
	}
}

void GoodEvent(HDC hdc)
{
	if (g_hitEventCount >= 45)
		g_bGoodEvent = FALSE;

	if (g_hitEventCount > 40)
	{
		g_logFont.lfHeight = 40 - (g_hitEventCount - 40) * 8;
		g_logFont.lfWidth = 20;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else if (g_hitEventCount < 5)
	{
		g_logFont.lfHeight = 50 - g_hitEventCount * 2;
		g_logFont.lfWidth = 25 - g_hitEventCount * 1;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else
	{
		g_logFont.lfHeight = 40;
		g_logFont.lfWidth = 20;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	g_oldFont = (HFONT)SelectObject(hdc, g_hitEventFont);
	SetTextColor(hdc, RGB(255 - 5 * (g_hitEventCount % 10), 0, 255 - 5 * (g_hitEventCount % 10)));
	DrawText(hdc, "GOOD", 4, &g_hitEventRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	g_hitEventCount++;

	(HFONT)SelectObject(hdc, g_oldFont);
	DeleteObject(g_hitEventFont);
}

void CoolEvent(HDC hdc)
{
	if (g_hitEventCount >= 45)
		g_bCoolEvent = FALSE;
	if (g_hitEventCount > 40)
	{
		g_logFont.lfHeight = 40 - (g_hitEventCount - 40) * 8;
		g_logFont.lfWidth = 20;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else if (g_hitEventCount < 5)
	{
		g_logFont.lfHeight = 70 - g_hitEventCount * 6;
		g_logFont.lfWidth = 40 - g_hitEventCount * 4;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else
	{
		g_logFont.lfHeight = 40;
		g_logFont.lfWidth = 20;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	g_oldFont = (HFONT)SelectObject(hdc, g_hitEventFont);
	SetTextColor(hdc, RGB(50, 255 - 10 * (g_hitEventCount % 10), 50));
	DrawText(hdc, "COOL", 4, &g_hitEventRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	g_hitEventCount++;

	(HFONT)SelectObject(hdc, g_oldFont);
	DeleteObject(g_hitEventFont);
}

void PerfectEvent(HDC hdc)
{
	if (g_hitEventCount >= 45)
		g_bPerfectEvent = FALSE;
	if (g_hitEventCount > 40)
	{
		g_logFont.lfHeight = 40 - (g_hitEventCount - 40) * 8;
		g_logFont.lfWidth = 20;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else if (g_hitEventCount < 5)
	{
		g_logFont.lfHeight = 100 - g_hitEventCount * 12;
		g_logFont.lfWidth = 25 - g_hitEventCount * 2;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	else
	{
		g_logFont.lfHeight = 40;
		g_logFont.lfWidth = 15;
		g_hitEventFont = CreateFontIndirect(&g_logFont);
	}
	g_oldFont = (HFONT)SelectObject(hdc, g_hitEventFont);
	SetTextColor(hdc, RGB(0, 255 - 15 * (g_hitEventCount % 10), 255));
	DrawText(hdc, "PERFECT!", 8, &g_hitEventRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	g_hitEventCount++;

	(HFONT)SelectObject(hdc, g_oldFont);
	DeleteObject(g_hitEventFont);
}

void MissEvent(HDC hdc)
{
	g_logFont.lfHeight = 40;
	g_logFont.lfWidth = 20;
	g_hitEventFont = CreateFontIndirect(&g_logFont);
	g_oldFont = (HFONT)SelectObject(hdc, g_hitEventFont);
	SetTextColor(hdc, RGB(50, 100, 255));
	if (g_hitEventCount >= 40)
		g_bMissEvent = FALSE;
	if (g_hitEventCount < 30)
	{
		SetRect(&g_hitEventRect, g_xPosOfHitEvent, g_yPosOfHitEvent + 20 - g_hitEventCount * 0.5,
			DROP_SECTION_WIDTH - g_xPosOfHitEvent, g_yPosOfHitEvent + 200 + 20 - g_hitEventCount * 0.5);
	}
	DrawText(hdc, "miss", 4, &g_hitEventRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	g_hitEventCount++;

	(HFONT)SelectObject(hdc, g_oldFont);
	DeleteObject(g_hitEventFont);
}

void HealthBarMovement(HDC hdc)
{
	static int healthMovementLine = 0;
	static int mark = 0;

	PartTransBlt(hdc, DROP_SECTION_WIDTH, g_displayHeight - 600, 600, g_hHealthabarBM, RGB(0, 0, 0));
	PartTransBlt(hdc, DROP_SECTION_WIDTH, g_displayHeight - 600,
		healthMovementLine + 5 + 595 - 595 * g_playerHealth / 30, g_hHealthEmptyBM, RGB(0, 0, 0));
	if (healthMovementLine > 10)
		mark = -5;
	if (healthMovementLine < 1)
		mark = 5;
	healthMovementLine += mark;
}

void DrawGameUI(HDC hdc)
{
	g_oldBrush = (HBRUSH)SelectObject(hdc, g_blackBrush);

	// 테두리 상자들
	Rectangle(hdc, DROP_SECTION_WIDTH, 0, DROP_SECTION_WIDTH + 155, 50);
	Rectangle(hdc, DROP_SECTION_WIDTH, 50, DROP_SECTION_WIDTH + 155, 100);
	Rectangle(hdc, DROP_SECTION_WIDTH + 155, 0, DROP_SECTION_WIDTH + 240, 50);
	Rectangle(hdc, DROP_SECTION_WIDTH + 155, 50, DROP_SECTION_WIDTH + 240, 100);

	// 제목
	g_oldFont = (HFONT)SelectObject(hdc, g_infoTitleFont);
	SetTextColor(hdc, RGB(150, 150, 200));
	DrawText(hdc, "Score", 5, &g_scoreTextRect, DT_LEFT | DT_TOP);
	DrawText(hdc, "Max Combo", 9, &g_comboTextRect, DT_LEFT | DT_TOP);
	DrawText(hdc, "Speed", 5, &g_speedTextRect, DT_LEFT | DT_TOP);
	DrawText(hdc, "Sync", 5, &g_syncTextRect, DT_LEFT | DT_TOP);

	// 실제 숫자 출력
	SelectObject(hdc, g_infoFont);
	SetTextColor(hdc, RGB(100, 100, 100));
	sprintf(g_buffer, TEXT("%07d"), g_score);
	DrawText(hdc, g_buffer, 7, &g_scoreText3DRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	sprintf(g_buffer, TEXT("%04d"), g_maxComboNum);
	DrawText(hdc, g_buffer, 4, &g_comboText3DRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	sprintf(g_buffer, TEXT("x%f"), MIN_DROP_SPEED - g_dropSpeed + 1);
	DrawText(hdc, g_buffer, 2, &g_speedText3DRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	if (g_adjustSynk < 0)
		sprintf(g_buffer, TEXT("%c%f"), g_syncMark, g_adjustSynk * -1);
	else
		sprintf(g_buffer, TEXT("%c%f"), g_syncMark, g_adjustSynk);
	DrawText(hdc, g_buffer, 4, &g_syncText3DRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SetTextColor(hdc, RGB(230, 230, 230));
	sprintf(g_buffer, TEXT("%07d"), g_score);
	DrawText(hdc, g_buffer, 7, &g_scoreTextRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	sprintf(g_buffer, TEXT("%04d"), g_maxComboNum);
	DrawText(hdc, g_buffer, 4, &g_comboTextRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	sprintf(g_buffer, TEXT("x%f"), MIN_DROP_SPEED - g_dropSpeed + 1);
	DrawText(hdc, g_buffer, 2, &g_speedTextRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	if (g_adjustSynk < 0)
		sprintf(g_buffer, TEXT("%c%f"), g_syncMark, g_adjustSynk * -1);
	else
		sprintf(g_buffer, TEXT("%c%f"), g_syncMark, g_adjustSynk);
	DrawText(hdc, g_buffer, 4, &g_syncTextRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, g_oldFont);
	SelectObject(hdc, g_oldBrush);
}

void DrawJudgementLine(HDC hdc)
{
	static int color = 0;
	static int mark = 0;

	if (color > 100)
		mark = -5;
	if (color < 1)
		mark = 5;
	color += mark;
	g_judgementLinePen = (HPEN)CreatePen(PS_SOLID, 15, RGB(150 + color, 150 + color, 150 + color));

	// 판단 선
	g_oldPen = (HPEN)SelectObject(hdc, g_judgementLinePen);
	MoveToEx(hdc, 0, g_yPosOfJudgementLine + 20, NULL);
	LineTo(hdc, 700, g_yPosOfJudgementLine + 20);
	SelectObject(hdc, g_oldPen);
	DeleteObject(g_judgementLinePen);
}

void HitEffect(HDC hdc, int whichOne)
{
	static int hitEffectCount[10];
	g_logFont.lfWeight = 700;
	g_logFont.lfHeight = 30 + hitEffectCount[whichOne] * 2;
	g_logFont.lfWidth = (30 + hitEffectCount[whichOne] * 2) / 2;
	g_effectFont = CreateFontIndirect(&g_logFont);
	g_oldFont = (HFONT)SelectObject(hdc, g_effectFont);
	SetTextColor(hdc, RGB(255, 255, 255));


	if (hitEffectCount[whichOne] > 10)
	{
		g_bHitEffect[whichOne] = FALSE;
		hitEffectCount[whichOne] = 0;
	}
	if (hitEffectCount[whichOne] < 3)
		TransBlt(hdc, g_xPosOFHitEffect[whichOne] + 20, g_yPosOFHitEffect[whichOne] + 10, g_effectPink, RGB(0, 0, 0));
	else if (hitEffectCount[whichOne] < 6)
		TransBlt(hdc, g_xPosOFHitEffect[whichOne] + 20, g_yPosOFHitEffect[whichOne] + 10, g_effectGreen, RGB(0, 0, 0));
	else
		TransBlt(hdc, g_xPosOFHitEffect[whichOne] + 20, g_yPosOFHitEffect[whichOne] + 10, g_effectBlue, RGB(0, 0, 0));

	SetRect(&g_effectRect, g_xPosOFHitEffect[whichOne] - 5, g_yPosOFHitEffect[whichOne] - 7,
		g_xPosOFHitEffect[whichOne] + 50 - 5, g_yPosOFHitEffect[whichOne] + 50 - 7);
	DrawText(hdc, g_lyric[g_hitID[whichOne]], 1, &g_effectRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	hitEffectCount[whichOne]++;
	SelectObject(hdc, g_oldFont);
	DeleteObject(g_effectFont);
}

void SyncForward()
{
	if (g_adjustSynk - 0.1 <= 0)
	{
		g_adjustSynk = 0.0;
		g_syncMark = '\0';
	}
	else
		g_adjustSynk -= 0.1;

	for (i = 0; i < g_amountOfDroplings; i++)
	{
		if (g_selectedMusic == ONEWAY && g_selectedLevel == HARD)
			g_dropStartTime[i] = g_oneWayDropTime[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == FROZEN && g_selectedLevel == HARD)
			g_dropStartTime[i] = g_forzenDropTime[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == ONEWAY && g_selectedLevel == EASY)
			g_dropStartTime[i] = g_oneWayDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == FROZEN && g_selectedLevel == EASY)
			g_dropStartTime[i] = g_forzenDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
	}
}

void SyncBackward()
{
	if (g_adjustSynk >= 0)
		g_syncMark = '+';
	g_adjustSynk += 0.1;

	for (i = 0; i < g_amountOfDroplings; i++)
	{
		if (g_selectedMusic == ONEWAY && g_selectedLevel == HARD)
			g_dropStartTime[i] = g_oneWayDropTime[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == FROZEN && g_selectedLevel == HARD)
			g_dropStartTime[i] = g_forzenDropTime[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == ONEWAY && g_selectedLevel == EASY)
			g_dropStartTime[i] = g_oneWayDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
		else if (g_selectedMusic == FROZEN && g_selectedLevel == EASY)
			g_dropStartTime[i] = g_forzenDropTime_easy[i] - g_dropSpeed + g_adjustSynk;
	}
}

void GameOver(HWND hWnd, HDC hdc)
{
	if (g_gameState == INGAME)
	{
		PlaySound(TEXT("Resource/GameOver.wav"), NULL, SND_ASYNC);
		g_gameState = GAMEOVER;
		g_GameOverEventCount = 0;
	}

	if (g_GameOverEventCount < 1)
	{
		g_GameOverEventCount += 0.05;
		DrawBitmap(hdc, 0, g_GameOverEventCount * (g_displayHeight / 2) - 514, g_hGameOver1);
		DrawBitmap(hdc, 0, g_displayHeight - g_GameOverEventCount * (g_displayHeight / 2), g_hGameOver2);
	}
}

void ScoreScreen(HDC hdc)
{
	int scoreCount;
	double currentTime;
	currentTime = GetTickCount() / 1000.0;
	if (g_gotScoreScreenStartTime == FALSE)
	{
		g_scoreScreenStartedTime = GetTickCount() / 1000.0;
		g_gotScoreScreenStartTime = TRUE;
		PlaySound(NULL, NULL, SND_ASYNC);
	}
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hMemBM);
	SetBkMode(memDC, TRANSPARENT);
	g_oldBrush = (HBRUSH)SelectObject(memDC, g_blackBrush);

	Rectangle(memDC, 0, 0, g_displayWidth, g_displayHeight);
	DrawBitmap(memDC, 1000, 500, g_hFrozenOlaf);

	g_oldPen = (HPEN)SelectObject(memDC, g_scoreScreenTitleFont);
	SetTextColor(memDC, RGB(255, 255, 0));
	DrawText(memDC, "Your Score!", 11, &g_scoreScreenTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(memDC, g_scoreScreenFont);
	SetTextColor(memDC, RGB(100, 100, 100));
	TextOut(memDC, 305 + 2, 200 + 2, "GOOD", 4);
	TextOut(memDC, 305 + 2, 280 + 2, "COOL", 4);
	TextOut(memDC, 200 + 2, 360 + 2, "PERFECT", 7);
	TextOut(memDC, 325 + 2, 440 + 2, "MISS", 4);
	TextOut(memDC, 90 + 2, 520 + 2, "MAX COMBO", 9);
	TextOut(memDC, 260 + 2, 600 + 2, "SCORE", 5);
	SetTextColor(memDC, RGB(220, 220, 220));
	TextOut(memDC, 305, 200, "GOOD", 4);
	TextOut(memDC, 305, 280, "COOL", 4);
	TextOut(memDC, 200, 360, "PERFECT", 7);
	TextOut(memDC, 325, 440, "MISS", 4);
	TextOut(memDC, 90, 520, "MAX COMBO", 9);
	TextOut(memDC, 260, 600, "SCORE", 5);

	if (currentTime - g_scoreScreenStartedTime > 1)
		TextExtend(memDC, 570, 190, 1, g_numOfGood);
	if (currentTime - g_scoreScreenStartedTime > 1.5)
		TextExtend(memDC, 570, 270, 2, g_numOfCool);
	if (currentTime - g_scoreScreenStartedTime > 2)
		TextExtend(memDC, 570, 350, 3, g_numOfPerfect);
	if (currentTime - g_scoreScreenStartedTime > 2.5)
		TextExtend(memDC, 570, 430, 4, g_numOfMiss);
	if (currentTime - g_scoreScreenStartedTime > 3)
		TextExtend(memDC, 570, 510, 5, g_maxComboNum);
	if (currentTime - g_scoreScreenStartedTime > 3.5 && currentTime - g_scoreScreenStartedTime <= 6.5)
	{
		if (g_scoreSoundPlayed == FALSE)
		{
			PlaySound(TEXT("Resource/ScoreSound.wav"), NULL, SND_ASYNC);
			g_scoreSoundPlayed = TRUE;
		}
		scoreCount = g_score * ((currentTime - g_scoreScreenStartedTime - 3.5) / 3);
		g_logFont.lfWeight = 700;
		g_logFont.lfHeight = 60;
		g_logFont.lfWidth = 30;
		g_scoreScreenExtendFont = CreateFontIndirect(&g_logFont);
		SelectObject(memDC, g_scoreScreenExtendFont);
		sprintf(g_buffer, "%d", scoreCount);
		SetTextColor(memDC, RGB(100, 100, 100));
		TextOut(memDC, 570 + 2, 590 + 2, g_buffer, lstrlen(g_buffer));
		SetTextColor(memDC, RGB(0, 200, 255));
		TextOut(memDC, 570, 590, g_buffer, lstrlen(g_buffer));
	}

	if (currentTime - g_scoreScreenStartedTime > 6.5)
	{
		g_logFont.lfWeight = 700;
		g_logFont.lfHeight = 60;
		g_logFont.lfWidth = 30;
		g_scoreScreenExtendFont = CreateFontIndirect(&g_logFont);
		SelectObject(memDC, g_scoreScreenExtendFont);
		sprintf(g_buffer, "%d", g_score);
		SetTextColor(memDC, RGB(100, 100, 100));
		TextOut(memDC, 570 + 2, 590 + 2, g_buffer, lstrlen(g_buffer));
		SetTextColor(memDC, RGB(0, 200, 255));
		TextOut(memDC, 570, 590, g_buffer, lstrlen(g_buffer));

		if (g_rankSoundPlayed == FALSE)
		{
			PlaySound(TEXT("Resource/Score_Rank.wav"), NULL, SND_ASYNC);
			g_rankSoundPlayed = TRUE;
		}
		if (g_selectedLevel == HARD)
		{
			if (g_score > 550000)
				RankEvent(memDC, 500, -100, 6, "S");
			else if (g_score > 450000)
				RankEvent(memDC, 500, -100, 6, "A");
			else if (g_score > 350000)
				RankEvent(memDC, 500, -100, 6, "B");
			else if (g_score > 250000)
				RankEvent(memDC, 500, -100, 6, "C");
			else if (g_score > 200000)
				RankEvent(memDC, 500, -100, 6, "D");
			else
				RankEvent(memDC, 500, -100, 6, "F");
		}
		else
		{
			if (g_score > 450000)
				RankEvent(memDC, 500, -100, 6, "S");
			else if (g_score > 400000)
				RankEvent(memDC, 500, -100, 6, "A");
			else if (g_score > 300000)
				RankEvent(memDC, 500, -100, 6, "B");
			else if (g_score > 200000)
				RankEvent(memDC, 500, -100, 6, "C");
			else if (g_score > 150000)
				RankEvent(memDC, 500, -100, 6, "D");
			else
				RankEvent(memDC, 500, -100, 6, "F");
		}
	}

	BitBlt(hdc, 0, 0, g_displayWidth, g_displayHeight, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, g_oldPen);
	SelectObject(memDC, oldBitmap);
	SelectObject(memDC, g_oldBrush);
	DeleteObject(memDC);
	DeleteObject(g_scoreScreenExtendFont);
}

void TextExtend(HDC hdc, int x, int y, int ID, int outPutNum)
{
	if (g_isBeepSoundPlayed[ID] == FALSE)
	{
		PlaySound(TEXT("Resource/BeepSound.wav"), NULL, SND_ASYNC);
		g_isBeepSoundPlayed[ID] = TRUE;
	}
	g_logFont.lfWeight = 700;
	g_logFont.lfHeight = g_textExtendCount[ID] * 3;
	g_logFont.lfWidth = (g_textExtendCount[ID] * 3) / 2;
	g_scoreScreenExtendFont = CreateFontIndirect(&g_logFont);
	g_oldFont = (HFONT)SelectObject(hdc, g_scoreScreenExtendFont);

	sprintf(g_buffer, "%d", outPutNum);
	SetRect(&g_scoreScreenNumRect, x + 2, y + 2, x + 100 + 2, y + 100 + 2);
	SetTextColor(hdc, RGB(100, 100, 100));
	DrawText(hdc, g_buffer, lstrlen(g_buffer), &g_scoreScreenNumRect, DT_SINGLELINE | DT_LEFT);
	SetRect(&g_scoreScreenNumRect, x, y, x + 100, y + 100);
	SetTextColor(hdc, RGB(255, 255, 255));
	DrawText(hdc, g_buffer, lstrlen(g_buffer), &g_scoreScreenNumRect, DT_SINGLELINE | DT_LEFT);

	if (g_textExtendCount[ID] < 20)
		g_textExtendCount[ID]++;
	SelectObject(hdc, g_oldFont);
	DeleteObject(g_scoreScreenExtendFont);
}

void RankEvent(HDC hdc, int x, int y, int ID, LPTSTR rank)
{
	g_logFont.lfWeight = 700;
	g_logFont.lfHeight = 800 - g_textExtendCount[ID] * 20;
	g_logFont.lfWidth = 400 - (g_textExtendCount[ID] * 20) / 2;
	g_scoreScreenExtendFont = CreateFontIndirect(&g_logFont);
	g_oldFont = (HFONT)SelectObject(hdc, g_scoreScreenExtendFont);

	SetRect(&g_scoreScreenNumRect, x + 2, y + 2, x + 1000 + 2, y + 1000 + 2);
	SetTextColor(hdc, RGB(100, 100, 100));
	DrawText(hdc, rank, 1, &g_scoreScreenNumRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	SetRect(&g_scoreScreenNumRect, x, y, x + 1000, y + 1000);
	SetTextColor(hdc, RGB(255, 50, 50));
	DrawText(hdc, rank, 1, &g_scoreScreenNumRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	if (g_textExtendCount[ID] < 20)
		g_textExtendCount[ID]++;
	SelectObject(hdc, g_oldFont);
	DeleteObject(g_scoreScreenExtendFont);
}

void RunIntro(HWND hWnd)
{
	double currentTime;
	currentTime = GetTickCount() / 1000.0;
	if (currentTime - g_introStartedTime > 0.1)
		g_gameState = INTRO;
	if (currentTime - g_introStartedTime <= 11)
		SetRect(&g_IntroScreenRect, g_displayWidth / 2 - ((g_displayWidth / 2) / 10) * (currentTime - g_introStartedTime),
			g_displayHeight / 2 - ((g_displayHeight / 2) / 10) * (currentTime - g_introStartedTime),
			g_displayWidth / 2 + ((g_displayWidth / 2) / 10) * (currentTime - g_introStartedTime),
			g_displayHeight / 2 + ((g_displayHeight / 2) / 10) * (currentTime - g_introStartedTime));

	InvalidateRect(hWnd, &g_IntroScreenRect, FALSE);
}

void DrawIntroScreen(HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hMemBM);
	static int x, y = 1, q = 0, p = 0;
	SetBkMode(memDC, TRANSPARENT);
	g_oldFont = (HFONT)SelectObject(memDC, g_introFont);

	DrawBitmap(memDC, -350, -300, g_hIntroBG);
	SetTextColor(memDC, RGB(50 + x + p, 50 + x, 200 + x));
	DrawText(memDC, "RYHTHM  ALPHABET", lstrlen("RYHTHM  ALPHABET"), &g_scoreScreenTitleRect,
		DT_SINGLELINE | DT_CENTER | DT_BOTTOM);

	SelectObject(memDC, g_scoreScreenFont);
	SetTextColor(memDC, RGB(255 - x, 255 - x, 255 - x));
	DrawText(memDC, "Press Any Key", lstrlen("Press Any Key"), &g_menuSubTitleRect,
		DT_SINGLELINE | DT_CENTER);

	BitBlt(hdc, 0, 0, g_displayWidth, g_displayHeight, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, g_oldPen);
	SelectObject(memDC, oldBitmap);
	DeleteObject(memDC);
	x += y;
	q++;
	if (x > 55)
		y = -1;
	if (x < 1)
		y = 1;
	if (q > 200)
	{
		q = 0;
		p--;
	}
	else if (q > 100)
		p++;
}

void RunMenu(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, FALSE);
}

void DrawMenu(HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hMemBM);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, g_blackBrush);
	SetBkMode(memDC, TRANSPARENT);
	SetTextColor(memDC, RGB(150, 150, 200));

	Rectangle(memDC, 0, 0, g_displayWidth, g_displayHeight);

	g_oldFont = (HFONT)SelectObject(memDC, g_scoreScreenTitleFont);
	DrawText(memDC, "Select Music", 12, &g_scoreScreenTitleRect, DT_SINGLELINE | DT_CENTER);

	DrawBitmap(memDC, g_cdX, g_displayHeight * 0.2, g_hFrozenCD);
	DrawBitmap(memDC, g_cdX + g_displayWidth / 2 + 210, g_displayHeight * 0.2, g_hOneWayCD);
	DrawBitmap(memDC, g_cdX - (g_displayWidth / 2 + 210), g_displayHeight * 0.2, g_hOneWayCD);

	SelectObject(memDC, g_subTitleFont);

	if (g_gameState == SELECTING_MUSIC)
	{
		TransBlt(memDC, g_displayWidth * 0.2, g_displayHeight * 0.5, g_hLeftArrow, RGB(0, 0, 0));
		TransBlt(memDC, g_displayWidth * 0.8, g_displayHeight * 0.5, g_hRightArrow, RGB(0, 0, 0));
	}
	else if (g_gameState == SELECTING_LEVEL)
	{
		SetTextColor(memDC, RGB(150, 150, 200));
		DrawText(memDC, "DIFFICULTY", 10, &g_menuSubTitleRect, DT_SINGLELINE | DT_CENTER | DT_TOP);
		SetTextColor(memDC, RGB(255, 255, 255));
		if (g_selectedLevel == HARD)
			DrawText(memDC, "HARD", 4, &g_menuSubTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		else
			DrawText(memDC, "EASY", 4, &g_menuSubTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		TransBlt(memDC, g_displayWidth * 0.4, g_displayHeight * 0.85, g_hLeftArrow, RGB(0, 0, 0));
		TransBlt(memDC, g_displayWidth * 0.6, g_displayHeight * 0.85, g_hRightArrow, RGB(0, 0, 0));
	}
	else if (g_gameState == SELECTING_SPEED)
	{
		SetTextColor(memDC, RGB(150, 150, 200));
		DrawText(memDC, "SPEED", 5, &g_menuSubTitleRect, DT_SINGLELINE | DT_CENTER | DT_TOP);
		sprintf(g_buffer, TEXT("x%f"), MIN_DROP_SPEED - g_dropSpeed + 1);
		SetTextColor(memDC, RGB(255, 255, 255));
		DrawText(memDC, g_buffer, 2, &g_menuSubTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		TransBlt(memDC, g_displayWidth * 0.4, g_displayHeight * 0.85, g_hLeftArrow, RGB(0, 0, 0));
		TransBlt(memDC, g_displayWidth * 0.6, g_displayHeight * 0.85, g_hRightArrow, RGB(0, 0, 0));
	}

	BitBlt(hdc, 0, 0, g_displayWidth, g_displayHeight, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, g_oldFont);
	SelectObject(memDC, g_oldBrush);
	SelectObject(memDC, oldBitmap);
	DeleteObject(memDC);
}

void SwapCD(HWND hWnd)
{
	static int x = 0;
	g_cdX += (g_displayWidth / 2 + 210) * 0.1 * g_swapDirection;
	x++;
	if (x >= 10)
	{
		x = 0;
		KillTimer(hWnd, CD_SWAP_TIMER);
		if (g_selectedMusic == ONEWAY)
		{
			g_selectedMusic = FROZEN;
			g_cdX = g_displayWidth * 0.5 - 210;
		}
		else
		{
			g_selectedMusic = ONEWAY;
			g_cdX = g_displayWidth;
		}
	}
	if (g_cdX > g_displayWidth)
		g_cdX = -419;
	if (g_cdX < -419)
		g_cdX = g_displayWidth;
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC memDC;
	HBITMAP oldBitmap;
	int bx, by;
	BITMAP bit;

	memDC = CreateCompatibleDC(hdc);
	oldBitmap = (HBITMAP)SelectObject(memDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
}

void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize, 1);

	x -= bm.bmWidth / 2;
	y -= bm.bmHeight / 2;

	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);
	hdcSave = CreateCompatibleDC(hdc);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
	BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

void PartTransBlt(HDC hdc, int x, int y, int endY, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = endY;
	DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);
	hdcSave = CreateCompatibleDC(hdc);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
	BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}