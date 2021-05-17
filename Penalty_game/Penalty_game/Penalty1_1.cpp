#include "framework.h"
#include "Penalty1_1.h"
#include <Windows.h>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;
mutex mtx;
MSG lpMsg;

const double BALLSPEED = 25.0;
const double STRIKERSPEED = 18.5;
const double DEFENDERSPEED = 19.2;
const double GOALKEEPERSPEED = 9.0;
const int MATCHTIME = 1000;
const int FRAMES = 50;
const int GOALLINE = 750;
const int GOALKICK = 650;

//position variables
int strikerPositionX = 100;
int strikerPositionY = 300;
int defenderPositionX = 500;
int defenderPositionY = 400;
int goalkeeperPositionX = 800;
int goalkeeperPositionY = 300;
int ballPositionX = strikerPositionX + 40;
int ballPositionY = strikerPositionY;

//additional variables
unsigned int Key;
unsigned int defenderMovementAxisY;
bool kicked = false;

//match end variables
bool ballOwner = true; //default: ball belongs to the striker
bool goal = false;
bool matchOver = false;

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
void checkBallOwner();
void kick();
bool checkForGoal();

//Field block
void DrawField(HDC hdc) {

	//Goal line
	HBRUSH hBrushW = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrushW);
	Rectangle(hdc, GOALLINE, 0, GOALLINE + 4, 600);

	//Goal
	HBRUSH hBrushB = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrushB);
	Rectangle(hdc, GOALLINE, 150, GOALLINE + 100, 154);
	SelectObject(hdc, hBrushB);
	Rectangle(hdc, GOALLINE, 450, GOALLINE + 100, 454);
	SelectObject(hdc, hBrushB);
	Rectangle(hdc, GOALLINE + 100, 150, GOALLINE + 104, 450);
	DeleteObject(hBrushB);

	//Goal line for striker
	SelectObject(hdc, hBrushW);
	Rectangle(hdc, GOALKICK, 100, GOALKICK + 4, 500);
	SelectObject(hdc, hBrushW);
	Rectangle(hdc, GOALKICK, 100, GOALKICK + 100, 104);
	SelectObject(hdc, hBrushW);
	Rectangle(hdc, GOALKICK, 500, GOALKICK + 100, 504);
	DeleteObject(hBrushW);
}

//Striker's block
void strikerMoveControl() {
	for (int i = 0; i < MATCHTIME; i++) {

		kick();

		switch (Key) {
		case VK_LEFT:
			defenderMovementAxisY = 0;
			strikerPositionX -= STRIKERSPEED;
			break;

		case VK_RIGHT:
			defenderMovementAxisY = 0;
			strikerPositionX += STRIKERSPEED;
			break;

		case VK_UP:
			defenderMovementAxisY = 1;
			strikerPositionY -= STRIKERSPEED;
			break;

		case VK_DOWN:
			defenderMovementAxisY = 2;
			strikerPositionY += STRIKERSPEED;
			break;

		default:
			defenderMovementAxisY = 0;
			break;
		}
		Sleep(FRAMES);
	}
}

void renderStriker(HDC hdc) {
	HBRUSH hBrush = CreateSolidBrush(RGB(245, 25, 19));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, strikerPositionX - 30, strikerPositionY - 30, strikerPositionX + 30, strikerPositionY + 30);
	DeleteObject(hBrush);
}

void kick() {
	if (strikerPositionX >= GOALKICK) {
		kicked = true;
	}
}

//Defender's block
void defenderMovement() {
	for (int i = 0; i < MATCHTIME; i++) {
		if (strikerPositionX < defenderPositionX) {
			if (defenderMovementAxisY == 0) {
				defenderPositionX -= DEFENDERSPEED;
				defenderPositionY -= DEFENDERSPEED * sin(i / 2);
			}
			if (defenderMovementAxisY == 1) {
				defenderPositionX -= DEFENDERSPEED;
				defenderPositionY -= DEFENDERSPEED * 1.15;
			}
			if (defenderMovementAxisY == 2) {
				defenderPositionX -= DEFENDERSPEED;
				defenderPositionY += DEFENDERSPEED * 1.15;
			}
		}
		else {
		if (defenderMovementAxisY == 0) {
			defenderPositionX += DEFENDERSPEED;
			defenderPositionY -= DEFENDERSPEED * sin(i / 2);
		}
		if (defenderMovementAxisY == 1) {
			defenderPositionX += DEFENDERSPEED;
			defenderPositionY -= DEFENDERSPEED * 1.15;
		}
		if (defenderMovementAxisY == 2) {
			defenderPositionX += DEFENDERSPEED;
			defenderPositionY += DEFENDERSPEED * 1.15;
		}
		}

		Sleep(FRAMES);
	}
}

void renderDefender(HDC hdc) {
	HBRUSH hBrush = CreateSolidBrush(RGB(100, 223, 227));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, defenderPositionX - 30, defenderPositionY - 30, defenderPositionX + 30, defenderPositionY + 30);
	DeleteObject(hBrush);
}

//Ball's block
void ballMovement() {
	for (int i = 0; i < MATCHTIME; i++) {

		checkBallOwner();

		if (ballOwner) {
			if (kicked) {
				if (defenderMovementAxisY == 1) {
					ballPositionX -= BALLSPEED;
					ballPositionY -= BALLSPEED;

					if (ballPositionX >= GOALLINE) {
						if (checkForGoal()) {
							goal = true;
						}
					}
				}

				if (defenderMovementAxisY == 2) {
					ballPositionX += BALLSPEED;
					ballPositionY += BALLSPEED;

					if (ballPositionX >= GOALLINE) {
						if (checkForGoal()) {
							goal = true;
						}
					}
				}

				if (defenderMovementAxisY == 0) {
					ballPositionX += BALLSPEED;

					if (ballPositionX >= GOALLINE) {
						if (checkForGoal()) {
							goal = true;
						}
					}
				}
			}
			else {
				ballPositionX = strikerPositionX + 40;
				ballPositionY = strikerPositionY;
			}
		}
		else {
			ballPositionX = defenderPositionX + 40;
			ballPositionY = defenderPositionY;
		}

		Sleep(FRAMES);
	}
}

void renderBall(HDC hdc) {
	HBRUSH hBrush = CreateSolidBrush(RGB(245, 242, 217));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, ballPositionX, ballPositionY, ballPositionX + 20, ballPositionY + 20);
	DeleteObject(hBrush);
}

void checkBallOwner() {
	if (((defenderPositionX + 20) == ballPositionX) || ((defenderPositionX - 20) == ballPositionX)) {
		ballOwner = false;
	}
}

bool checkForGoal() {
	if (((ballPositionY) > 150) && (ballPositionY < 450)) {
		if (!(((goalkeeperPositionY + 30) == ballPositionY) || (goalkeeperPositionY - 30) == ballPositionY)) {
			return true;
		}
	}
	else {
		return false;
	}
}

//Goal keeper's block
void goalkeeperMovement() {
	for (int i = 0; i < MATCHTIME; i++) {

		if (goalkeeperPositionY >= 180) {
			if (i % 2 == 0) {
				goalkeeperPositionY += GOALKEEPERSPEED * sin(i);
			}
			else {
				goalkeeperPositionY -= GOALKEEPERSPEED * cos(i);
			}
		}
		else {
			if (i % 2 == 0) {
				goalkeeperPositionY += GOALKEEPERSPEED;
			}
			else {
				goalkeeperPositionY += GOALKEEPERSPEED;
			}
		}
		
		Sleep(FRAMES);
	}
}

void renderGoalkeeper(HDC hdc) {
	HBRUSH hBrush = CreateSolidBrush(RGB(48, 158, 245));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, goalkeeperPositionX - 30, goalkeeperPositionY - 30, goalkeeperPositionX + 30, goalkeeperPositionY + 30);
	DeleteObject(hBrush);
}


void THfunction(HINSTANCE hInstance, WNDCLASS w, const int n_cmd_show) {
	HWND hwnd;
	hwnd = CreateWindow(L"Penalty", L" ", WS_OVERLAPPEDWINDOW, 0, 0, 976, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, n_cmd_show);

	thread strikerThread(strikerMoveControl);
	thread defenderThread(defenderMovement);
	thread ballThread(ballMovement);
	thread goalkeeperThread(goalkeeperMovement);

	while (true) {
		InvalidateRect(hwnd, 0, 1);
		UpdateWindow(hwnd);
		Sleep(FRAMES);

		PeekMessage(&lpMsg, NULL, 0, 0, PM_REMOVE);
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}

	goalkeeperThread.detach();
	ballThread.detach();
	defenderThread.detach();
	strikerThread.detach();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int n_cmd_show) {
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));

	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = reinterpret_cast<WNDPROC>(WndProc);
	w.hInstance = hInstance;
	w.hbrBackground = CreateSolidBrush(0x5BD966);

	w.lpszClassName = L"Penalty";
	RegisterClass(&w);

	THfunction(hInstance, w, n_cmd_show);
}

LONG WINAPI WndProc(HWND hwnd, const UINT message, WPARAM wparam, LPARAM lparam) {
	HDC hdc; PAINTSTRUCT ps; HPEN hpen;

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		DrawField(hdc);

		renderStriker(hdc);
		renderDefender(hdc);
		renderGoalkeeper(hdc);
		renderBall(hdc);

		if (!ballOwner) {
			TextOut(hdc, 500, 300, L"Defenders won", 14);
			Sleep(5000);
			exit(1);
		}

		if (goal) {
			TextOut(hdc, 500, 300, L"Attackers won", 14);
			Sleep(5000);
			exit(1);
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		Key = wparam;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}
