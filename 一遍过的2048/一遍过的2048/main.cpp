// ���õ�ͷ�ļ�
#include <windows.h> // windows���ڳ����̣���Ҫ����ͷ�ļ�windows.h��
#include <stdlib.h>
#include <stdio.h>
#include <ctime> // ����time.h


// LRESULT��һ���������ͣ�ָ�ص�����CALLBACK����ֵ��
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ȫ�ִ�����
int array[4][4] = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0,0,0,0 },{ 0,0,0,0 } };

// ���ھ��
HWND hwnd;
HWND g_hwnd;

// ��ں������൱��main������
int WINAPI WinMain
(
	HINSTANCE hlnstance, // ����ʵ�����
	HINSTANCE hPrevlnstance, // �����̵ĳ���ʵ�����
	PSTR szCmdline, // �����в���
	int iCmdShow // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
	)

{

	/*******************************************************************************
	* ##########      MSG�ṹ�嶨��     ##########
	*
	* typedef struct tagMSG {
	*    HWND hwnd; //��ʾ��Ϣ�����Ĵ��ڡ�����ͨ�������ĳ����Ǵ���Ӧ�ó���һ����Ϣһ�㶼����ĳ������������ġ����磬��ĳ��������а����������������İ�����Ϣ���Ƿ����ô��ڵġ���Windows�����У���HWND���͵ı�������ʶ���ڡ�
	*    UINT message; //ָ������Ϣ�ı�ʶ������Windows�У���Ϣ����һ����ֵ����ʾ�ģ���ͬ����Ϣ��Ӧ��ͬ����ֵ������������ֵ�����ڼ��䣬����Windows����Ϣ��Ӧ����ֵ����ΪWM_XXX�꣨WM��Window Message����д������ʽ��XXX��Ӧĳ����Ϣ��Ӣ��ƴд�Ĵ�д��ʽ�����磬������������Ϣ��WM_LBUTTONDOWN�����̰�����Ϣ��WM_KEYDOWN���ַ���Ϣ��WM_CHAR���ȵȡ��ڳ���������ͨ��������WM_XXX�����ʽ��ʹ����Ϣ�ġ�
	*    WPARAM wParam; //����������������ָ����Ϣ�ĸ�����Ϣ�����磬�������յ�һ���ַ���Ϣ��ʱ��message��Ա������ֵ����WM_CHAR�����û������������ʲô�ַ�����ô����wParam��lParam��˵����wParam��lParam��ʾ����Ϣ����Ϣ�Ĳ�ͬ����ͬ�������֪����������Ա���������ʾ����Ϣ��������MSDN�й���ĳ��������Ϣ��˵���ĵ��鿴�������߿�����VC++�Ŀ���������ͨ��goto definition�鿴һ��WPARAM��LPARAM���������͵Ķ��壬���Է�������������ʵ���Ͼ���unsigned int��long��
	*	  LPARAM lParam;
	*	  DWORD time; //��ʾ��ϢͶ�ݵ���Ϣ�����е�ʱ�䡣
	*	  POINT pt; //��ʾ���ĵ�ǰλ�á�
	* } MSG;
	*
	*******************************************************************************/
	TCHAR *szAppName = TEXT("2048");
	HWND hwnd; // ���ھ��
	MSG msg;
	WNDCLASS wndclass; // WNDCLASS��ע�ᴰ�����API����RegisterClass�Ĳ����������洢һ��������ĸ������ԡ�

	// ������������Ϣ�ĺ����ĺ���ָ��
	wndclass.lpfnWndProc = WndProc;
	// ��ʾû�ж������洢�ռ䡣
	wndclass.cbClsExtra = 0;
	// ��ʾû�ж���Ĵ�����洢�ռ䡣
	wndclass.cbWndExtra = 0;
	// hinstance��instance�ľ����instanceΪռ���ڴ�Ķ���
	wndclass.hInstance = hlnstance;
	// hIcon��Ա����ָ�����ڵ�ͼ�꣬�򵥷����亯��ԭ�ͣ�
	// HICON LoadIcon(HINSTANCE hInstance,LPCTSTR  lpIconName);
	// lpIconName��ָ��NULL�ַ���β���ַ�����ָ�룬������ͼ������
	// ���Ҫʹ��windowsԤ�����ͼ�꣬��ʱhInstance�������ó�NULL��lpIconName�������һЩ�ض�ֵ��
	wndclass.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// �趨�������ʽ��
	wndclass.hCursor = LoadCursor(NULL,
		IDC_ARROW);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ��
	wndclass.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// lpszMenuName��Ӧ����ΪNULL����ΪWindows CE��ֱ��֧���в˵��Ĵ��ڡ� 
	wndclass.lpszMenuName = NULL;
	// ���������������ڴ�������ʱ��Ҫ������֡�
	wndclass.lpszClassName = szAppName;
	// ���崰�������ʽ��
	// CS_HREDRAW��������ڵ�λ�û��ȸı䣬���ػ洰�ڡ�
	// CS_VREDRAW��������ڵ�λ�û�߶ȸı䣬���ػ洰�ڡ�
	// ���˻�����һ���¾�������Ŵ���С���ں���Ϸ��������
	wndclass.style = CS_HREDRAW | CS_VREDRAW;


	// ע�ᴰ��

	if (!RegisterClass(&wndclass))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, TEXT("�޷�ע�ᴰ����"),
			TEXT("ע�����"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ���������С�


	// �������ڡ�

	hwnd = CreateWindow(
		szAppName,		// ���������������Ѿ�ע����ˣ�
		TEXT("�ҵĳ���������2048��Ϸ"),	// ��ʾ�ڴ��ڶ��˵ĳ�������
		// overlapped window���ص��ʹ��ڣ�
		// ��һ�����б߿�ͱ������Ĵ��ڡ�
		// ��������������һ�����Խ������ŵ��ص��Դ��ڣ�
		// ����Ϊ�������Ե�һ�֡�
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
		CW_USEDEFAULT,		// ���ô���ˮƽλ�ã�Ĭ��ֵ��
		CW_USEDEFAULT,		// ���ô��ڴ�ֱλ�ã�Ĭ��ֵ��
		500, 500,		// ���ô��ڵĿ����
		(HWND)NULL,			// �����ھ��
		(HMENU)NULL,		// ���ڲ˵��ľ��
		hlnstance,				// Ӧ�ó���ʵ�����
		(LPVOID)NULL);		// ָ�򸽼����ݵ�ָ��

	if(!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, TEXT("�޷�ע�ᴰ����"),
			TEXT("ע�����"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// ���ڴ����ɹ���������С�

	// ��ʾ����
	ShowWindow(hwnd, iCmdShow);

	// ˢ�´���
	UpdateWindow(hwnd);

	// ��Ϣѭ��
	// ��ȡ����Ϣ�Ĵ��ھ��ȡֵΪNULLʱ��GetMessageΪ�κ����ڵ����̵߳Ĵ��ڼ�����Ϣ��
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg); // ����һ����Ϣ�����ڳ���
		TranslateMessage(&msg); // ���������Ϣת��Ϊ�ַ���Ϣ
	}
	return 0;
}

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
			ExitProcess(0);

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



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		NewNum();
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			//����ϼ�������
		case VK_UP:
		{

			changeArr(0);
			DrawCanvas(hwnd);


		}
		break;
		//����¼�������
		case VK_DOWN:
		{
			changeArr(1);
			DrawCanvas(hwnd);
		}
		break;
		//������������
		case VK_LEFT:
		{
			changeArr(2);
			DrawCanvas(hwnd);
		}
		break;
		//����Ҽ�������
		case VK_RIGHT:
		{
			changeArr(3);
			DrawCanvas(hwnd);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
