#include <graphics.h>
#include <conio.h> 
#include<math.h>
#include<tchar.h>
#include<stdio.h>
#include<string.h>
#define PI 3.14
#define MAX_IMAGE 15  
#define MAX_ITEMS 15   
#define MAX_ANGLE 58 
#define GOLD_VALUE 60 
#define STONE_VALUE 20
#define DIAMOND_VALUE 80
#define LEVEL_ONE_TIMELIMIT 5
#define LEVEL_TWO_TIMELIMIT 5
#define LEVEL_THREE_TIMELIMIT 5
#define LEVEL_ONE_TARGET 0
#define LEVEL_TWO_TARGET 0
#define LEVEL_THREE_TARGET 0
#define TIME_uNdFiNe 0
#define BAKUDAN_VALUE 200
#define MAX_RANKING_LIST_NUM 10
#define CLOCK_VALUE 250
struct Hook {
	int x;      //始//////位//
	int y;      //////////////
	int endx;   //////////////
	int endy;   //末//////置//
	int angle;  
	int len;
	int direction;//摆动方向
	int speed_x;//速度x方向分量
	int speed_y;//速度y方向分量
	int action;//状态(正常、伸长、缩短)
	int index; //抓到物品的下标
}hook,gouzi;
typedef struct ITEM //物品
{
	int x;
	int y;
	int size; //边界判断
	bool flag;//是否存在
	int type;//物品类型，石头，金块，钱袋
	int value;//物品价值
	bool find;

	struct ITEM* next;
}item;
enum TYPE //物品类型枚举
{
	GOLD,
	DIAMOND,
	STONE,
	//方向
	LEFT,
	RIGHT,
	//状态，正常，伸长，缩短
	M_NOMAL,
	M_LONG,
	M_SHORT,
};
typedef struct info
{
	char name[20];
	int points;
}INFO;

/***************************自定义函数初始化区域***************************/
int initialize_enterscreen(ITEM * pHead);
ITEM * create(void);
void textinput(int x6, int xfw, int yfw, char* q); //实现输入输出功能
void textsettings(void);
void draw_items(ITEM * pHead);
void shop(void);                                //商店界面,要新建画布，存储信息到文件，再进入下一关（再次新建）
void settings(void);
void pass(void);
void write_in_txt(void);
void show_ranking_list(void);
void use_prop(void);
char name[100];
TCHAR value[20];
TCHAR T_ime[20];
TCHAR LEVEL_NUM[20];
TCHAR TARGET[20];
TCHAR COINS[20];
TCHAR BAKUDAN_NUM[20];
TCHAR CLOCK_NUM[20];
TCHAR show_points[20];
int value_trans;
bool click_result;
int key_judge(void);                              //判断是什么按键
int state_judge(int x1, int x2, int y1, int y2);//判断鼠标位置是否符合要求 x1-x2,y1-y2
int distant(Hook hook);
void hook_wave(void);
void hook_grab(int speed);
void grab_judge(ITEM * pHead);
void dead(void);
bool PAUSE;
bool button_judge(int x1, int y1, int x2, int y2);
MOUSEMSG biege;
int LEVEL;
int CONDITION;
int TIME;
int REAL_TIME;
void time(void);
int total_value;
int com_value;
int target[3] = { LEVEL_ONE_TARGET,LEVEL_TWO_TARGET,LEVEL_THREE_TARGET };
int bakudan_value;
int clock_value;
int coins;
int bakuda_num;
int clock_num;
int data_num;
ITEM* pNow_1;
ITEM* pNow_2;
ITEM* pReal = (ITEM*)malloc(sizeof(ITEM));;
/**************************************************************************/
/**********************************主函数**********************************/
int main()
{
	ITEM* pHead = create();
	DWORD t1, t2;
	t1 = t2 = GetTickCount(); 
	initgraph(960, 540, SHOWCONSOLE);
	
	textsettings();              //字体样式设置
	printf("测试：程序正常启动\n");
	if (initialize_enterscreen(pHead))    //开始界面
		draw_items(pHead);                    //进入主界面
	else
	{
		TCHAR s[] = _T("程序出错，请联系我！");
		outtextxy(200, 240, s);
	}
	/**************************************第一关*************************************/
	
	BeginBatchDraw();
	while (1)
	{
		printf("进入循环了啊啊啊啊啊啊啊啊啊啊啊啊\n");
		if (TIME == 0)
		{
			com_value = value_trans;
			total_value += value_trans;
			coins = total_value;
			value_trans = 0;
			TIME = 100 * LEVEL_TWO_TIMELIMIT;
			EndBatchDraw();
			break;
		}
		if (t2 - t1 > 15)
		{
			hook_wave();
			t1 = t2;
		}
		t2 = GetTickCount();
		use_prop();
		time();
		printf("%d\n", TIME);
		draw_items(pHead);
		hook_grab(3);
		printf("grab\n");
		grab_judge(pHead);
		
		FlushBatchDraw();
	}
	if (com_value >= LEVEL_ONE_TARGET)
	{
		printf("关卡结束判定测试：【判定成功】已过关\n");
		LEVEL++;
		com_value = 0;
		shop();
	}
	else
	{
		printf("关卡结束判定测试：【判定成功】未过关\n");
		dead();
		return 0;
	}
	/**************************************第二关*************************************/
	initgraph(960, 540,SHOWCONSOLE);
	textsettings();
	BeginBatchDraw();
	while (1)
	{
		if (TIME == 0)
		{
			com_value = value_trans;
			total_value += value_trans;
			coins = coins + total_value;
			value_trans = 0;
			TIME = 100 * LEVEL_THREE_TIMELIMIT;
			EndBatchDraw();
			break;
		}
		if (t2 - t1 > 15)
		{
			hook_wave();
			t1 = t2;
		}
		t2 = GetTickCount();
		use_prop();
		time();
		draw_items(pHead);
		hook_grab(3);
		grab_judge(pHead);
		FlushBatchDraw();
	}
	if (com_value >= LEVEL_TWO_TARGET)
	{
		LEVEL++;
		com_value = 0;
		shop();
	}
	else
	{
		dead();
		return 0;
	}
	/**************************************第三关*************************************/
	initgraph(960, 540, SHOWCONSOLE);
	textsettings();
	BeginBatchDraw();
	while (1)
	{
		if (TIME == 0)
		{
			com_value = value_trans;
			total_value += value_trans;
			coins = coins + total_value;
			value_trans = 0;
			TIME = TIME_uNdFiNe;
			EndBatchDraw();
			break;
		}
		if (t2 - t1 > 15)
		{
			hook_wave();
			t1 = t2;
		}
		t2 = GetTickCount();
		use_prop();
		time();
		draw_items(pHead);
		hook_grab(3);
		grab_judge(pHead);
		FlushBatchDraw();
	}
	if (com_value >= LEVEL_THREE_TARGET)
	{
		//com_value = 0;
		pass();//恭喜过关 end
	}
	else
		dead();
	/**************************************结束*************************************/
	_getch();
	closegraph();
	return 0;
}
/**************************************************************************/
/*******************************自定义函数原型******************************/
int initialize_enterscreen(ITEM * pHead)
{
	
	IMAGE start,startbottom2,login_pic;
	int i = 0;
	loadimage(&start, _T("..//picture//pictures//start.png"));
	loadimage(&startbottom2, _T("..//picture//pictures//startbottom2.png"));
	putimage(0, 0, &start);
	putimage(120, 430, &startbottom2);
	srand(GetTickCount());
	bakuda_num = 0;
	clock_num = 0;
	coins = 0;
	data_num = 0;
	printf("测试：程序加载资源成功\n");
	//初始化关卡编号
	LEVEL = 1;
	//初始化时间
	TIME = 100 * LEVEL_ONE_TIMELIMIT;
	//初始化PAUSE按钮判断
	PAUSE = FALSE;
	click_result = FALSE;
	//初始化分数
	value_trans = 0;
	total_value = 0;
	com_value = 0;

	//初始化物品
	ITEM* pNow = (ITEM*)malloc(sizeof(ITEM));
	pNow = pHead->next;
	printf("初始化测试：\n");
	while (NULL != pNow)
	{
		pNow->flag = true;
		pNow->size = 60;
		pNow->type = rand() % 3;
		pNow->x = rand() % 800 + 50;
		pNow->y = rand() % 400 + 120;
		pNow->value = 0;
		pNow->find = false;
		if (pNow->type == GOLD)
			pNow->value = GOLD_VALUE;
		else if (pNow->type == STONE)
			pNow->value = STONE_VALUE;
		else
			pNow->value = DIAMOND_VALUE;
		printf("type=%d\tx=%d\ty=%d\tvalue=%d\n", pNow->type, pNow->x, pNow->y, pNow->value);
		pNow = pNow->next;
	}
	
	//初始化物品
	bakudan_value = BAKUDAN_VALUE;
	clock_value = CLOCK_VALUE;
	
	//初始化钩子
	hook.x = 455;
	hook.endx = hook.x;
	hook.y = 50;
	hook.endy = hook.y + hook.len;
	hook.len = 50;
	hook.angle = 0;
	hook.direction = RIGHT;
	hook.speed_x = 0;
	hook.speed_y = 0;
	hook.action = M_NOMAL;
	hook.index = -1;
	//钩头
	gouzi.x = hook.x - 18;
	gouzi.endx = 438;
	gouzi.y = 100;
	gouzi.endy = 50;
	gouzi.len = 50;
	gouzi.angle = 0;
	gouzi.direction = RIGHT;
	gouzi.speed_x = 0;
	gouzi.speed_y = 0;
	gouzi.action = M_NOMAL;
	gouzi.index = -1;
	//开始界面
	if (state_judge(120, 260, 430, 520))
	{
		printf("测试：点击开始按钮，进入信息登陆界面正常\n");
		cleardevice();
		loadimage(&login_pic, _T("..//picture//pictures//login_pic.png"));
		putimage(140, 80, &login_pic);
		textinput(300, 240, 250, name);
		i++;
	}
	else;
	
	if (i == 1)   
	{
		cleardevice();
		return 1;
	}
	else
		return -1;
}
void textinput(int x6, int xfw, int yfw, char * q)
{
	int m;
	m  = 0;
	printf("当前已获取的字母:\n");
	while ((*q = _getch()) != 13)          //空格13
	{
		printf("%c\t", *q);
		if (*q == VK_BACK && x6 != xfw)
		{
			*q = ' ';
			setbkcolor(WHITE);             //set-word-background-color
			setbkmode(OPAQUE);             //background-mode: TRANSPARENT\OPAQUE
			outtextxy(x6, yfw, q);
			x6 -= 22;                      //删除时 间距
			--q;                          //指针向前移动
		}
		else if (*q == VK_BACK)
			continue;
		else
		{
			outtextxy(x6 += 22, yfw, q);              //显示为字母，即用户名    添加时 间距
			++q;                                        //指针向后移动
		}
	}
	*q = '\0';
	printf("\n%s\n", q);
}
void textsettings(void)
{
	//抗锯齿输出
	LOGFONT f;
	gettextstyle(&f);						// 获取当前字体设置
	f.lfHeight = 48;						// 设置字体高度为 48
	_tcscpy_s(f.lfFaceName, _T("黑体"));		// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	return;
}
void draw_items(ITEM * pHead)//绘制总函数
{
	
	IMAGE background, brick, char1, char2, char1_mask, char2_mask;
	IMAGE gold, money, stone;
	IMAGE gold_bk, money_bk, stone_bk;//还未加载
	IMAGE gouzitou,gouzitou_mask;
	
	int x, y;
	loadimage(&background, _T("..//picture//pictures//level-background-0.jpg"));
	loadimage(&brick, _T("..//picture//pictures//brick.png"));
	loadimage(&char1, _T("..//picture//pictures//char1.jpg"));
	loadimage(&char2, _T("..//picture//pictures//char2.jpg"));
	loadimage(&gouzitou, _T("..//picture//pictures//hook.bmp"));
	loadimage(&gouzitou_mask, _T("..//picture//pictures//hook_mask.bmp"));
	loadimage(&gold_bk, _T("..//picture//pictures//big_gold_mask.png"));
	loadimage(&char1_mask, _T("..//picture//pictures//char1_mask.jpg"));
	//加载金子石头钱袋的图片
	loadimage(&gold, _T("..//picture//pictures//big_gold.png"));
	loadimage(&money, _T("..//picture//pictures//diamond.png"));
	loadimage(&stone, _T("..//picture//pictures//small_stone.jpg"));
	loadimage(&money_bk, _T("..//picture//pictures//diamond_mask.png"));
	loadimage(&stone_bk, _T("..//picture//pictures//small_stone_mask.jpg"));
	
	
	putimage(0, 0, &background);
	putimage(437, 0, &char1_mask, SRCPAINT);
	putimage(437, 0, &char1, SRCAND);
	
	//绘制边界（上）
	for (x = 0, y = 69; x <= 880; x += 80)
		putimage(x, y, &brick);

	//绘制商店按钮 
	setfillcolor(BROWN);
	setlinecolor(BROWN);
	settextcolor(WHITE);
	settextstyle(40, 0, _T("楷体"));
	fillrectangle(730, 10, 820, 66);
	outtextxy(735, 20, "商店");

	//绘制暂停按钮
	fillrectangle(840, 10, 930, 66);
	outtextxy(845, 20, "暂停");

	//绘制得分目标、得分、分数
	settextstyle(30, 0, _T("楷体"));
	outtextxy(150, 20, "得分");
	outtextxy(10, 20, "目标");
	outtextxy(290, 20, "时间");
	_stprintf_s(TARGET, _T("%d"), target[LEVEL - 1]);
	outtextxy(80, 20, TARGET);
	//绘制用户名
	settextcolor(RED);
	settextstyle(20, 0, "楷体");
	outtextxy(550, 45, "加油,");
	outtextxy(600, 45, name);
	//绘制关卡编号
	_stprintf_s(LEVEL_NUM, _T("%d"), LEVEL);
	outtextxy(580, 20, LEVEL_NUM);
	outtextxy(550, 20, "第   关");
	//绘制物品
	pNow_1 = (ITEM*)malloc(sizeof(ITEM));
	pNow_1 = pHead->next;
	while (NULL != pNow_1)
	{
		printf("进入循环\n");
		if (pNow_1->flag == true)
		{
			switch (pNow_1->type)
			{
			case GOLD:
				printf("进入循环_1\n");
				printf("%d\t%d\n", pNow_1->x, pNow_1->y);
				//putimage(100, 100, &gold); printf("金子图片\n");
				putimage(pNow_1->x, pNow_1->y, &gold_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &gold, SRCAND);
				break;
			case DIAMOND:
				printf("进入循环_2\n");
				putimage(pNow_1->x, pNow_1->y, &money_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &money, SRCAND);
				break;
			case STONE:
				printf("进入循环_3\n");
				putimage(pNow_1->x, pNow_1->y, &stone_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &stone, SRCAND);
				break;
			}
		}
		pNow_1 = pNow_1->next;
	}
	
	//绘制钩子
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	line(hook.x, hook.y, hook.endx, hook.endy);
	printf("绘制狗子\n");
	//绘制钩子头
	putimage(gouzi.x, gouzi.y, &gouzitou_mask, SRCPAINT);
	putimage(gouzi.x, gouzi.y, &gouzitou,SRCAND);

	//绘制分数
	settextstyle(30, 0, _T("楷体"));
	_stprintf_s(value, _T("%d"), value_trans);
	outtextxy(220, 20, value);
	//倒计时
	REAL_TIME = TIME / 100;
	_stprintf_s(T_ime, _T("%d"), REAL_TIME);
	outtextxy(355, 20, T_ime);
	//Sleep(1000);
	printf("循环\n");
	//Sleep(1000);
	//按钮点击判断（还未实现）
	/*
	if (state_judge(840, 930, 19, 69) == 1)//如果点设置按钮state_judge(840, 930, 19, 69)
		settings();
	if (state_judge(730, 820, 19, 69) == 1)//如果点商店按钮state_judge(730, 820, 19, 69)
		shop();                      //进入商店界面
	*/
	}
int key_judge(void)
{
	char key;

	key = _getch();
	if (key == 13)    //回车返回13
		return 13;
	else if (key == 27)    //esc返回27
		return 27;
	else
		return -1;
}
void shop(void)
{
	initgraph(798, 580, SHOWCONSOLE);
	IMAGE bk_shop, bakudan;
	IMAGE clock, clock_mask;
	loadimage(&bk_shop, _T("..//picture//pictures//shop.png"));
	loadimage(&bakudan, _T("..//picture//pictures//bakudan.bmp"));
	loadimage(&clock, _T("..//picture//pictures//clock.jpeg"));
	loadimage(&clock_mask, _T("..//picture//pictures//clock_mask.png"));
	putimage(0, 0, &bk_shop);
	putimage(360, 355, &bakudan);
	putimage(420, 355, &clock);
	setbkmode(TRANSPARENT);
	while (1)
	{
		biege = GetMouseMsg();
		//显示提示
		outtextxy(360, 440, "这个可以帮你炸掉石头以获得更快的下一次抓取");
		//显示现有金币
		outtextxy(430, 120, "金币");
		_stprintf_s(COINS, _T("%d"), coins);
		outtextxy(500, 120, COINS);
		//显示已有道具数量
		outtextxy(200, 500, "背包中道具数量：");
		outtextxy(380, 500, "bakudan:");
		outtextxy(400, 550, "clock:");
		_stprintf_s(BAKUDAN_NUM, _T("%d"), bakuda_num);
		outtextxy(450, 500, BAKUDAN_NUM);
		_stprintf_s(CLOCK_NUM, _T("%d"), clock_num);
		outtextxy(450, 550, CLOCK_NUM);
		//显示道具价格
		outtextxy(378, 345, "200");
		outtextxy(460, 345, "250");

		if (button_judge(360, 355, 397, 428)) //炸弹图标判断
		{
			if (coins >= bakudan_value)
			{
				coins = coins - bakudan_value;
				bakuda_num++;
				printf("购买测试（信息更新测试）-【正常】coins-%d total_num+%d\n", bakudan_value, bakuda_num);
			}
			else
				outtextxy(160, 550, "金币不足，无法购买！");
		}
		if ((button_judge(420, 355, 500, 435))) //钟表图标判断
		{
			if (coins >= clock_value)
			{
				coins = coins - clock_value;
				clock_num++;
			}
			else
				outtextxy(160, 550, "金币不足，无法购买！");
		}
		if (button_judge(610, 90, 730, 150))
		{
			printf("按钮测试：【正常】-用户点击按钮，进入下一关\n");
			break;
		}
	}
	return;
}
int state_judge(int x1, int x2, int y1, int y2)
{
	MOUSEMSG state;
	bool jdg = false;
	int flag = 0;
	
	while (1)
	{
		state = GetMouseMsg();
		switch (state.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((state.x >= x1 && state.x <= x2) && (state.y >= y1 && state.y <= y2))
			{
				jdg = true;
				flag++;
			}
			break;
		}
		if(flag == 1)
			break;
	}
	if (jdg == true)
		return 1;
	else
		return -1;
}
void settings(void)
{
	initgraph(798, 580);
}
int distant(Hook hook)
{
	int dis = sqrt((long)(hook.x - hook.endx) * (hook.x - hook.endx) + (hook.y - hook.endy) * (hook.y - hook.endy));
	return dis <= hook.len;
}
void hook_wave(void)
{
	/*************************************************判断暂停*************************************************/
	
	if(MouseHit())
		biege = GetMouseMsg();
	if (button_judge(840, 10, 930, 66) == TRUE && PAUSE == TRUE && biege.mkLButton == TRUE && click_result == FALSE)
	{
		PAUSE = FALSE;
		click_result = TRUE;
		
	}
	
	if (button_judge(840, 10, 930, 66) == TRUE && biege.mkLButton == TRUE && click_result == FALSE)
	{
		PAUSE = TRUE;
		click_result = TRUE;
	}
	if (biege.mkLButton == FALSE)
		click_result = FALSE;

	if (PAUSE == TRUE)
		return;
	/******************************************************判断暂停********************************************************/
	
	if (hook.action == M_NOMAL)
	{
		if (hook.direction == RIGHT)
		{
			hook.angle++;
			gouzi.angle++;
		}
		else
		{
			hook.angle--;
			gouzi.angle--;
		}
		if (hook.angle > MAX_ANGLE)
		{
			hook.direction = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE)
		{
			hook.direction = RIGHT;
		}
		
		hook.endx = hook.x + sin(PI / 180 * hook.angle) * hook.len;
		hook.endy = hook.y + cos(PI / 180 * hook.angle) * hook.len;
		gouzi.x = gouzi.endx + sin(PI / 180 * gouzi.angle) * gouzi.len;
		gouzi.y = gouzi.endy + cos(PI / 180 * gouzi.angle) * gouzi.len;
		printf("钩子摆动测试-正在摆动\t【钩子末位置】endx=%d\tendy=%d\n", hook.endx, hook.endy);
	}
	
}
void hook_grab(int speed)
{

	if (GetAsyncKeyState(VK_SPACE) && hook.action == M_NOMAL)// (GetAsyncKeyState(VK_LBUTTON) && hook.action == M_NOMAL))
	{
		
		hook.speed_x = sin(PI / 180 * hook.angle) * speed;
		hook.speed_y = cos(PI / 180 * hook.angle) * speed;
		hook.action = M_LONG;
		gouzi.speed_x = sin(PI / 180 * gouzi.angle) * speed;
		gouzi.speed_y = cos(PI / 180 * gouzi.angle) * speed;
		printf("钩子伸缩测试-【钩子速度】speedx=%d\tspeedy=%d\n", hook.speed_x, hook.speed_y);
	}
	//边界判断
	if (hook.endx<0 || hook.endx> 960 || hook.endy > 540)
	{
		hook.action = M_SHORT;
	}
	if (hook.action == M_LONG)
	{
		hook.endx += hook.speed_x;
		hook.endy += hook.speed_y;
		gouzi.x += gouzi.speed_x;
		gouzi.y += gouzi.speed_y;
		printf("钩子伸缩测试-【钩子状态】-伸长 endx=%d\tendy=%d\n", hook.endx, hook.endy);
	}
	else if (hook.action == M_SHORT)
	{
		hook.endx -= hook.speed_x;
		hook.endy -= hook.speed_y;
		gouzi.x -= gouzi.speed_x;
		gouzi.y -= gouzi.speed_y;
		printf("钩子伸缩测试-【钩子状态】-缩短 endx=%d\tendy=%d\n", hook.endx, hook.endy);
		if (distant(hook))
		{
			hook.action = M_NOMAL;
		}
	}
	
}
void grab_judge(ITEM * pHead)
{
	int i;
	printf("judge\n");
	pNow_2 = (ITEM*)malloc(sizeof(ITEM));
	pNow_2 = pHead->next;
	while (NULL != pNow_2)
	{

		if (pNow_2->flag == true && hook.endx > pNow_2->x && hook.endx < pNow_2->x + pNow_2->size && hook.endy > pNow_2->y && hook.endy < pNow_2->y + pNow_2->size)
		{
			printf("钩子判定功能测试：【已抓到】\n");
			
			pReal = pNow_2;
			pReal->find = true;
			//Sleep(2000);
			break;
		}
		pNow_2 = pNow_2->next;
	}
	if (pReal->find == true)
	{
		hook.action = M_SHORT;
		pReal->x = hook.endx;
		pReal->y = hook.endy;
		printf("正在缩短啊啊啊啊啊啊啊啊啊啊啊啊啊啊\n");
		if (distant(hook))
		{
			value_trans += pReal->value;
			pReal->flag = false;
			pReal->find = false;
			hook.action = M_NOMAL;
			printf("缩短完成啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊\n");
			printf("得分系统测试：【已抓到宝物type=%d，获取得分points=%d】\n", pReal->type, pReal->value);
		}
	}
	
	printf("judge_finish\n");
	
}
bool button_judge(int x1, int y1, int x2, int y2)
{
	//MOUSEMSG biege;
	//if (MouseHit())
	//{
		
		//biege = GetMouseMsg();
		if (biege.mkLButton && biege.x >= x1 && biege.x <= x2 && biege.y >= y1 && biege.y <= y2)
			return true;
	//}
	return false;
}
void time(void)
{
	TIME--;	
}
void dead(void)
{
	initgraph(800, 576,SHOWCONSOLE);
	IMAGE dead;
	loadimage(&dead, _T("..//picture//pictures//dead.png"));
	putimage(0, 0, &dead);
	printf("程序结束判断测试：【程序结束】-正常-未过关\n");
	Sleep(10000);
	write_in_txt();
	Sleep(2000);
	show_ranking_list();
	_getch();
	return;
}
void pass(void)
{
	initgraph(800, 576, SHOWCONSOLE);
	IMAGE clear;
	loadimage(&clear, _T("..//picture//pictures//clear.png"));
	putimage(0, 0, &clear);
	printf("程序结束判断测试：【程序结束】-正常-通关\n");
	//把本局游戏的最高分写入文件
	write_in_txt();
	Sleep(2000);
	show_ranking_list();
	_getch();
	return;
}
void show_ranking_list(void)
{
	initgraph(800, 576, SHOWCONSOLE);
	IMAGE paihangbang;
	loadimage(&paihangbang, _T("..//picture//pictures//paihangbang.png"));
	putimage(0, 0, &paihangbang);

	int i = 0;
	/*读取到结构体，循环输出*/;
	printf("读取数据测试：\t");
	FILE* read = fopen("data.txt", "r");
	if (read == NULL)
	{
		outtextxy(100, 100, "游戏信息保存失败");
		Sleep(1000);
		return;
	}
	INFO* read_data = (INFO*)malloc(sizeof(INFO));
	if (read_data)
	{
		while (fscanf(read, "%s\t%d\n", read_data->name, &read_data->points) != EOF)
			data_num++;
	}
	else
	{
		outtextxy(100, 100, "游戏信息保存失败");
		return;
	}
	INFO* data_role = (INFO*)malloc(sizeof(INFO) * data_num);
	rewind(read);
	while (fscanf(read, "%s\t%d\n", read_data->name, &read_data->points) != EOF)
	{
		strcpy(data_role[i].name, read_data->name);
		data_role[i].points = read_data->points;
		i++;
	}
	printf("【读取数据成功】-文件关闭\n");
	fclose(read);
	printf("排序测试：\t");
	i = 0;
	//排序
	int j;
	int max;
	INFO temp;
	printf("【开始排序】\n");
	for (i = 0; i < data_num; i++)
	{
		max = i;
		for (j = i + 1; j < data_num; j++)
		{
			if (data_role[j].points > data_role[max].points)
				max = j;
		}
		if (max != j)
		{
			temp = data_role[i];
			data_role[i] = data_role[max];
			data_role[max] = temp;
		}
		printf("【正在排序%d/%d】", i+1, data_num);
	}
	printf("\n【排序完成】\n");
	i = 0;
	//显示排名
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("楷体"));
	for (; i < MAX_RANKING_LIST_NUM; i++)
	{
		outtextxy(280, 210+(20*i), data_role[i].name);
		//fenshu
		//outtextxy(200, 100 + (20 * i)), data_role[i].points);
		_stprintf_s(show_points, _T("%d"), data_role[i].points);
		outtextxy(500, 210+(20*i), show_points);
	}
	Sleep(5000);
	return ;
}
void write_in_txt(void)
{
	printf("文件写入测试：\t");
	FILE* write = fopen("data.txt", "a+");
	if (write == NULL)
	{
		outtextxy(100, 100, "游戏信息保存失败");
		Sleep(1000);
		return ;
	}
	INFO * data = (INFO*)malloc(sizeof(INFO));
	if (data)
	{
		printf("【成功创建文件指针】\n");
		strcpy(data->name, name);
		data->points = total_value;
		fprintf(write, "%s\t%d\n", data->name, data->points);
		printf("【成功写入】-成功创建文件指针并打开文件-进行格式化写入操作\n");
	}
	else
	{
		outtextxy(100, 100, "游戏保存失败");
		Sleep(1000);
		return;
	}
	fclose(write);
	return;
}
ITEM* create(void)
	{
		int i;

		ITEM* pHead_0;
		pHead_0 = (ITEM*)malloc(sizeof(ITEM));
		if (NULL == pHead_0)
		{
			outtextxy(100,100,"分配内存失败");
			exit(-1);
		}

		ITEM* pTail;
		pTail = pHead_0;
		pHead_0->next = NULL;

		for (i = 0; i < MAX_ITEMS; i++)
		{
			ITEM * pNew = (ITEM*)malloc(sizeof(ITEM));
			if (NULL == pNew)
			{
				printf("分配内存失败");
				exit(-1);
			}
			//pNew->size = 1;
			pTail->next = pNew;
			pNew->next = NULL;
			pTail = pNew;
		}

		return pHead_0;
	}
void use_prop(void) //z是时间卡(0x5A) a是过关卡(0x41)
{
	IMAGE add_clock, add_clock_mask;
	loadimage(&add_clock, _T("..//picture//pictures//addclock.png"));
	loadimage(&add_clock_mask, _T("..//picture//pictures//add_clock_mask.png"));

	if ((GetAsyncKeyState(0x5A)&1)==1)
	{
		if (clock_num > 0)
		{
			clock_num = clock_num - 1;
			TIME = TIME + 500;
			putimage(100, 100, &add_clock_mask);
			putimage(100, 100, &add_clock);
			Sleep(1000);

		}
	}
}