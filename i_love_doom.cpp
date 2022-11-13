#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#define WINDOW_TITLE "The Ultimate DOOM - Chocolate Doom 3.0.0"
using namespace std;
using namespace cv;
using namespace sf;

Mat getMat(HWND hWND)
{
	HDC deviceContext = GetDC(hWND);
	HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);

	RECT windowRect;
	GetClientRect(hWND, &windowRect);

	int h = windowRect.bottom;
	int w = windowRect.right;

	HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, w, h);

	SelectObject(memoryDeviceContext, bitmap);

	BitBlt(memoryDeviceContext, 0, 0, w, h, deviceContext, 0, 0, SRCCOPY);

	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = w;
	bi.biHeight = -h;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 1;
	bi.biYPelsPerMeter = 2;
	bi.biClrUsed = 3;
	bi.biClrImportant = 4;

	Mat mat = Mat(h, w, CV_8UC4);

	GetDIBits(memoryDeviceContext, bitmap, 0, h, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(bitmap);
	DeleteObject(memoryDeviceContext);
	ReleaseDC(hWND, deviceContext);

	return mat;
}

int main()
{
	HWND hWND = FindWindow(NULL, WINDOW_TITLE);

	while (!hWND)
	{
		system("cls");
		cout << "Please start the game ..." << endl;
		hWND = FindWindow(NULL, WINDOW_TITLE);
		Sleep(100);
	}

	RenderWindow window(VideoMode(1280, 960), "I LOVE DOOM");

	while (window.isOpen())
	{
		Mat target = getMat(hWND);

		cvtColor(target, target, COLOR_BGR2GRAY);

		//namedWindow("I LOVE DOOM", WINDOW_NORMAL);

		Font font;
		if (!font.loadFromFile("PTMono-Regular.ttf"))
			return MessageBox(0, "r u serios rith nyaw", "I LOVE DOOM", MB_OK | MB_ICONERROR);

		Text text;
		text.setFont(font);
		text.setCharacterSize(8);
		text.setFillColor(Color::White);
		text.setStyle(Text::Bold);

		GaussianBlur(target, target, Size(1, 1), 1, 1);
		threshold(target, target, 100, 255, THRESH_BINARY);

		resize(target, target, Size(80, 80));

		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		wstring rendered_symbols;

		for (int x = 0; x <= target.rows; x++)
			for (int y = 0; y <= target.cols; y++)
			{
				//text.setPosition(x, y);
				//cout << (int)target.at<uchar>(x, y) << endl;

				if (y == target.rows)
					rendered_symbols += L"\n";

				if ((int)target.at<uchar>(x, y) <= 255 && (int)target.at<uchar>(x, y) >= 100)
					rendered_symbols += L"♥";
				else
					rendered_symbols += L" ";
			}

		text.setString(rendered_symbols);

		window.clear();
		window.draw(text);
		window.display();
		//imshow("I LOVE DOOM", target);
		//waitKey(30);
	}

	return 0;
}