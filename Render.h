#pragma once
#include "SDK.h"
#include <stdlib.h>
#include <stdio.h>


namespace Render {

	int Printf(int X, int Y, cColor Color, bool Center, char* _Input, ...) {
		char buff[1024] = { '\0' };

		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(buff, _Input, Args);
		va_end(Args);

		size_t Size = strlen(buff) + 1;

		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, buff, Size - 1);

		int Width = 0, Height = 0;

		Surface->GetTextSize(5, WideBuffer, Width, Height);

		if (Center)
			Surface->DrawSetTextPos(X - (Width * .5), Y);
		else
			Surface->DrawSetTextPos(X, Y);

		Surface->DrawSetTextFont(5);
		Surface->DrawSetTextColor(Color);
		Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer), 0);
		return Width;
	}



	void DrawFilledRect(int x, int y, int w, int h, cColor Color) {
		Surface->DrawSetColor(Color);
		Surface->DrawFilledRect(x, y, x + w, y + h);
	}


	void DrawLine(int X1, int Y1, int X2, int Y2, cColor Color) {
		Surface->DrawSetColor(Color);
		Surface->DrawLine(X1, Y1, X2, Y2);
	}

	void DrawESPRect(int X, int Y, int W, int H, cColor Color) {
		int SizeW = W / 3;
		int SizeH = H / 4;

		Surface->DrawSetColor(cColor(0, 0, 0, 255));
		Surface->DrawLine(X - 1, Y - 1, (X - 1) + SizeW, Y - 1);
		Surface->DrawLine(X + W - SizeW, Y - 1, (X + W) + 1, Y - 1);
		Surface->DrawLine(X - 1, Y - 1, X - 1, (Y - 1) + SizeH);
		Surface->DrawLine((X + 1) + W, Y - 1, (X + 1) + W, (Y - 1) + SizeH);
		Surface->DrawLine(X - 1, (Y + H) - SizeH, X - 1, (Y + H + 1));
		Surface->DrawLine(X + W + 1, (Y + H) - SizeH, (X + W) + 1, (Y + H + 1));
		Surface->DrawLine(X - 1, (Y + H + 1), (X - 1) + SizeW, (Y + H + 1));
		Surface->DrawLine(X + W + 1, (Y + H + 1), (X + W) - SizeW, (Y + H + 1));

		Surface->DrawSetColor(Color);
		Surface->DrawLine(X, Y, (X)+SizeW, Y);
		Surface->DrawLine(X + W - SizeW, Y, (X + W), Y);
		Surface->DrawLine(X, Y, X, (Y)+SizeH);
		Surface->DrawLine((X)+W, Y, (X)+W, (Y)+SizeH);
		Surface->DrawLine(X, (Y + H) - SizeH, X, (Y + H));
		Surface->DrawLine(X + W, (Y + H) - SizeH, (X + W), (Y + H));
		Surface->DrawLine(X, (Y + H), (X)+SizeW, (Y + H));
		Surface->DrawLine(X + W, (Y + H), (X + W) - SizeW, (Y + H));
	}


	void DrawRect(int X, int Y, int W, int H, cColor Color) {
		Surface->DrawSetColor(Color);
		Surface->DrawOutlinedRect(X, Y, X + W, Y + H);
	}

};