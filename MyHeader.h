#ifndef __MyHeader_H
#define __MyHeader_H

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <stdio.h>

#define DROP_SECTION_WIDTH	700		// ���ڵ� �������� �� �ʺ�
#define RUN_GAME_TIMER		1
#define INTRO_TIMER			2
#define MENU_TIMER			3
#define CD_SWAP_TIMER		4
#define TOLERANCE_GOOD		10		// hit �Ǻ� ����
#define TOLERANCE_COOL		6
#define TOLERANCE_PERFECT	2
#define TOLERANCE_ADJUST	4
#define SYCN_DELAY			0.1		// �ʱ� ��ũ
#define MAX_DROP_SPEED			1	// 1�ʸ��� ���ڰ� ������
#define MIN_DROP_SPEED			6	// 6�ʸ��� ���ڰ� ������
#define STANDARD_X_POS_OF_DROPLINGS		30      // �������� ���ڵ��� x��ǥ ���ؼ�
#define SECTION_DISTANCE				70	    // �������� ���ڵ��� x��ǥ ����(���� ����)
#define MAX_NUM_OF_DROPLINGS			400     // �ִ� �������� ���� ��

enum DroplingState { WAITING, DROPING, FINISHED };
enum GameState { LOADING, INTRO, SELECTING_MUSIC, SELECTING_LEVEL, SELECTING_SPEED, INGAME, SCORE_SCREEN, GAMEOVER };
enum MusicState { ONEWAY, FROZEN };
enum LevelState { EASY, HARD };

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);	// WndProc �Լ� ����
void FullScreenModeChange(int nWidth, int nHeight);								// ��üȭ�� �Լ� ����
void DroplingsDrop(HDC hdc, int whichDropling);
DWORD LoadWav(HWND hWnd, LPCTSTR lpszWave);
void IncreaseDropSpeed(HWND hWnd);
void DecreaseDropSpeed(HWND hWnd);
void HitAction(HWND hWnd, WPARAM &wParam);
void DrawGame(HDC hdc);
void RunGame(HWND hWnd);
void Initialize(HWND hWnd);
void GoodEvent(HDC hdc);
void CoolEvent(HDC hdc);
void PerfectEvent(HDC hdc);
void MissEvent(HDC hdc);
void ComboEvent(HDC hdc);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void HealthBarMovement(HDC hdc);
void DrawGameUI(HDC hdc);
void DrawJudgementLine(HDC hdc);
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
void HitEffect(HDC hdc, int whichOne);
void GameOver(HWND hWnd, HDC hdc);
void ScoreScreen(HDC hdc);
void SyncForward();
void SyncBackward();
void TextExtend(HDC hdc, int x, int y, int ID, int outPutNum);
void RankEvent(HDC hdc, int x, int y, int ID, LPTSTR rank);
void PartTransBlt(HDC hdc, int x, int y, int endY, HBITMAP hbitmap, COLORREF clrMask);
void DrawIntroScreen(HDC hdc);
void RunIntro(HWND hWnd);
void DrawMenu(HDC hdc);
void RunMenu(HWND hWnd);
void SwapCD(HWND hWnd);


#endif