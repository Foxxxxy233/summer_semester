// 引用的头文件
#include <windows.h> // windows窗口程序编程，需要引用头文件windows.h。
#include <stdlib.h>
#include <stdio.h>
#include <ctime> // 包含time.h


// LRESULT是一个数据类型，指回调函数CALLBACK返回值。
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 全局大数组
int array[4][4] = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0,0,0,0 },{ 0,0,0,0 } };

// 窗口句柄
HWND hwnd;
HWND g_hwnd;

// 入口函数（相当于main函数）
int WINAPI WinMain
(
	HINSTANCE hlnstance, // 程序实例句柄
	HINSTANCE hPrevlnstance, // 父进程的程序实例句柄
	PSTR szCmdline, // 命令行参数
	int iCmdShow // 用于指明窗口是否需要显示的参数。
	)

{

	/*******************************************************************************
	* ##########      MSG结构体定义     ##########
	*
	* typedef struct tagMSG {
	*    HWND hwnd; //表示消息所属的窗口。我们通常开发的程序都是窗口应用程序，一个消息一般都是与某个窗口相关联的。例如，在某个活动窗口中按下鼠标左键，产生的按键消息就是发给该窗口的。在Windows程序中，用HWND类型的变量来标识窗口。
	*    UINT message; //指定了消息的标识符。在Windows中，消息是由一个数值来表示的，不同的消息对应不同的数值。但是由于数值不便于记忆，所以Windows将消息对应的数值定义为WM_XXX宏（WM是Window Message的缩写）的形式，XXX对应某种消息的英文拼写的大写形式。例如，鼠标左键按下消息是WM_LBUTTONDOWN，键盘按下消息是WM_KEYDOWN，字符消息是WM_CHAR，等等。在程序中我们通常都是以WM_XXX宏的形式来使用消息的。
	*    WPARAM wParam; //以下两个变量用于指定消息的附加信息。例如，当我们收到一个字符消息的时候，message成员变量的值就是WM_CHAR，但用户到底输入的是什么字符，那么就由wParam和lParam来说明。wParam、lParam表示的信息随消息的不同而不同。如果想知道这两个成员变量具体表示的信息，可以在MSDN中关于某个具体消息的说明文档查看到。读者可以在VC++的开发环境中通过goto definition查看一下WPARAM和LPARAM这两种类型的定义，可以发现这两种类型实际上就是unsigned int和long。
	*	  LPARAM lParam;
	*	  DWORD time; //表示消息投递到消息队列中的时间。
	*	  POINT pt; //表示鼠标的当前位置。
	* } MSG;
	*
	*******************************************************************************/
	TCHAR *szAppName = TEXT("2048");
	HWND hwnd; // 窗口句柄
	MSG msg;
	WNDCLASS wndclass; // WNDCLASS是注册窗口类的API函数RegisterClass的参数，用来存储一个窗口类的各种属性。

	// 用来处理窗口消息的函数的函数指针
	wndclass.lpfnWndProc = WndProc;
	// 表示没有多余的类存储空间。
	wndclass.cbClsExtra = 0;
	// 表示没有多余的窗口类存储空间。
	wndclass.cbWndExtra = 0;
	// hinstance是instance的句柄；instance为占用内存的对象。
	wndclass.hInstance = hlnstance;
	// hIcon成员用来指定窗口的图标，简单分析其函数原型：
	// HICON LoadIcon(HINSTANCE hInstance,LPCTSTR  lpIconName);
	// lpIconName是指向NULL字符结尾的字符串的指针，它包含图标名。
	// 如果要使用windows预定义的图标，这时hInstance必须设置成NULL，lpIconName则可以是一些特定值。
	wndclass.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// 设定鼠标光标样式。
	wndclass.hCursor = LoadCursor(NULL,
		IDC_ARROW);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象。
	// 这里加载的是一个白色的画刷。
	wndclass.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// lpszMenuName域应设置为NULL，因为Windows CE不直接支持有菜单的窗口。 
	wndclass.lpszMenuName = NULL;
	// 给窗口类起名，在创建窗口时需要这个名字。
	wndclass.lpszClassName = szAppName;
	// 定义窗口类的样式。
	// CS_HREDRAW：如果窗口的位置或宽度改变，将重绘窗口。
	// CS_VREDRAW：如果窗口的位置或高度改变，将重绘窗口。
	// 用人话解释一下下就是如果放大、缩小窗口后游戏会重启。
	wndclass.style = CS_HREDRAW | CS_VREDRAW;


	// 注册窗口

	if (!RegisterClass(&wndclass))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, TEXT("无法注册窗口类"),
			TEXT("注册错误"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功则继续运行。


	// 创建窗口。

	hwnd = CreateWindow(
		szAppName,		// 窗口类名（上面已经注册过了）
		TEXT("我的超级棒棒的2048游戏"),	// 显示在窗口顶端的程序名字
		// overlapped window：重叠型窗口，
		// 即一个带有边框和标题栏的窗口。
		// 接下来产生的是一个可以进行缩放的重叠性窗口，
		// 后者为窗口属性的一种。
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
		CW_USEDEFAULT,		// 设置窗口水平位置（默认值）
		CW_USEDEFAULT,		// 设置窗口垂直位置（默认值）
		500, 500,		// 设置窗口的宽与高
		(HWND)NULL,			// 父窗口句柄
		(HMENU)NULL,		// 窗口菜单的句柄
		hlnstance,				// 应用程序实例句柄
		(LPVOID)NULL);		// 指向附加数据的指针

	if(!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, TEXT("无法注册窗口类"),
			TEXT("注册错误"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口创建成功则继续运行。

	// 显示窗口
	ShowWindow(hwnd, iCmdShow);

	// 刷新窗口
	UpdateWindow(hwnd);

	// 消息循环
	// 当取得消息的窗口句柄取值为NULL时，GetMessage为任何属于调用线程的窗口检索消息。
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg); // 调度一个消息给窗口程序
		TranslateMessage(&msg); // 将虚拟键消息转化为字符消息
	}
	return 0;
}

// 画数组函数
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
				// 设置数字的字体大小
				ZeroMemory(&logfont, sizeof(LOGFONT));
				logfont.lfCharSet = GB2312_CHARSET;
				// 设置字体的大小
				logfont.lfHeight = -50;
				HFONT hFont = CreateFontIndirect(&logfont);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(200, 200, 0));
				SetBkMode(hdc, TRANSPARENT);
				SelectObject(hdc, hFont);
				// 创建画刷
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
	int num = 0; // 一个在0至9之间的数
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
		//上键 done
	case 0:
	{

		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
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
				//2两两比较

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


		}//结束数组操作
		 //判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { NewNum(); }
		}

	}
	break;
	//下键
	case 1:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
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
				//2两两比较

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
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { NewNum(); }
		}
	}
	break;
	//左键done
	case 2:
	{

		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
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
				//2两两比较

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


		}//结束数组操作
		 //判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { NewNum(); }
		}
	}
	break;
	//右键
	case 3:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
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
				//2两两比较

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
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
			ExitProcess(0);

		}
		else {
			//随机生成一个新的2或4放入数组
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
			//如果上键被按下
		case VK_UP:
		{

			changeArr(0);
			DrawCanvas(hwnd);


		}
		break;
		//如果下键被按下
		case VK_DOWN:
		{
			changeArr(1);
			DrawCanvas(hwnd);
		}
		break;
		//如果左键被按下
		case VK_LEFT:
		{
			changeArr(2);
			DrawCanvas(hwnd);
		}
		break;
		//如果右键被按下
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
