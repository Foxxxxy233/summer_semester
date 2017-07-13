
#include "numbers.h"
#include <windows.h>

HWND g_hwnd;

// ȫ�ִ�����
int array[4][4] = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0,0,0,0 },{ 0,0,0,0 } };


// ��������

// �����麯��
void DrawCanvas(HWND hwnd)
{
	HBRUSH hBrush;
	HDC hdc;
	RECT rt;
	TCHAR szBuffer[10];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (array[i][j] != 0)
			{
				LOGFONT logfont;
				hdc = GetDC(hwnd);
				// �������ֵ������С
				ZeroMemory(&logfont, sizeof(LOGFONT));
				logfont.lfCharSet = GB2312_CHARSET;
				// ��������Ĵ�С
				logfont.lfHeight = -50;
				HFONT hFont = CreateFontIndirect(&logfont);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(200, 200, 0));
				SetBkMode(hdc, TRANSPARENT);
				SelectObject(hdc, hFont);
				// ������ˢ
				hBrush = CreateSolidBrush(RGB((array[i][j] * 10) % 256, (array[i][j] * 40) % 256, 0));
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				FillRect(hdc, &rt, hBrush);
				TextOut(hdc, j * 100, i * 100 + 25, szBuffer, wsprintf(szBuffer, TEXT("%d"), array[i][j]));
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
			else
			{
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				hdc = GetDC(hwnd);
				hBrush = CreateSolidBrush(RGB(200, 250, 0));
				FillRect(hdc, &rt, hBrush);
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
		}
	}
}


void NewNum()
{
	int num = 0; // һ����0��9֮�����
	int xPos = 0, yPos = 0;
	srand(time(0));
	do {
		xPos = rand() % 4;
		yPos = rand() % 4;
	} while (array[xPos][yPos] != 0);
	num = rand() % 10;
	if (num < 7)
		array[xPos][yPos] = 2;
	else
		array[xPos][yPos] = 4;
}

bool isFull()
{
	bool full = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (array[i][j] == 0)
			{
				full = false;
			}
		}
	}
	return full;
}

void changeArr(int direct)
{
	bool xiaoyici = false;
	bool chendi = false;

	switch (direct)
	{
		//�ϼ� done
	case 0:
	{

		//��ʼ�������
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1�ȳ���
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (array[k][i] == 0 && array[k + 1][i] != 0)
							{
								chendi = true;
								array[k][i] = array[k + 1][i];
								array[k + 1][i] = 0;
							}
						}
					}
				}
				//2�����Ƚ�

				for (int j = a; j < 4; ++j)
				{


					if (array[j][i] != 0 && array[j][i] == array[j + 1][i])
					{
						xiaoyici = true;
						array[j][i] <<= 1;
						array[j + 1][i] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//�����������
		 //�ж��Ƿ�����
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("��ҵ���"), TEXT("���"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//�������һ���µ�2��4��������
			if (xiaoyici || chendi) { NewNum(); }
		}

	}
	break;
	//�¼�
	case 1:
	{
		//��ʼ�������
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1�ȳ���
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (array[k][i] == 0 && array[k - 1][i] != 0)
							{
								chendi = true;
								array[k][i] = array[k - 1][i];
								array[k - 1][i] = 0;
							}
						}
					}
				}
				//2�����Ƚ�

				for (int j = 3 - a; j >= 0; --j)
				{
					if (array[j][i] != 0 && array[j][i] == array[j - 1][i])
					{
						xiaoyici = true;
						array[j][i] += array[j][i];
						array[j - 1][i] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//�����������
		//�ж��Ƿ�����
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("��ҵ���"), TEXT("���"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//�������һ���µ�2��4��������
			if (xiaoyici || chendi) { NewNum(); }
		}
	}
	break;
	//���done
	case 2:
	{

		//��ʼ�������
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1�ȳ���
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (array[i][k] == 0 && array[i][k + 1] != 0)
							{
								chendi = true;
								array[i][k] = array[i][k + 1];
								array[i][k + 1] = 0;
							}
						}
					}
				}
				//2�����Ƚ�

				for (int j = a; j < 4; ++j)
				{


					if (array[i][j] != 0 && array[i][j] == array[i][j + 1])
					{
						xiaoyici = true;
						array[i][j] <<= 1;
						array[i][j + 1] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//�����������
		 //�ж��Ƿ�����
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("��ҵ���"), TEXT("���"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//�������һ���µ�2��4��������
			if (xiaoyici || chendi) { NewNum(); }
		}
	}
	break;
	//�Ҽ�
	case 3:
	{
		//��ʼ�������
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1�ȳ���
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (array[i][k] == 0 && array[i][k - 1] != 0)
							{
								chendi = true;
								array[i][k] = array[i][k - 1];
								array[i][k - 1] = 0;
							}
						}
					}
				}
				//2�����Ƚ�

				for (int j = 3 - a; j >= 0; --j)
				{
					if (array[i][j] != 0 && array[i][j] == array[i][j - 1])
					{
						xiaoyici = true;
						array[i][j] += array[i][j];
						array[i][j - 1] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//�����������
		//�ж��Ƿ�����
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("��ҵ���"), TEXT("���"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//�������һ���µ�2��4��������
			if (xiaoyici || chendi) { NewNum(); }
		}
	}
	break;
	default:
		return;
	}
}

