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
	int x;      //ʼ//////λ//
	int y;      //////////////
	int endx;   //////////////
	int endy;   //ĩ//////��//
	int angle;  
	int len;
	int direction;//�ڶ�����
	int speed_x;//�ٶ�x�������
	int speed_y;//�ٶ�y�������
	int action;//״̬(�������쳤������)
	int index; //ץ����Ʒ���±�
}hook,gouzi;
typedef struct ITEM //��Ʒ
{
	int x;
	int y;
	int size; //�߽��ж�
	bool flag;//�Ƿ����
	int type;//��Ʒ���ͣ�ʯͷ����飬Ǯ��
	int value;//��Ʒ��ֵ
	bool find;

	struct ITEM* next;
}item;
enum TYPE //��Ʒ����ö��
{
	GOLD,
	DIAMOND,
	STONE,
	//����
	LEFT,
	RIGHT,
	//״̬���������쳤������
	M_NOMAL,
	M_LONG,
	M_SHORT,
};
typedef struct info
{
	char name[20];
	int points;
}INFO;

/***************************�Զ��庯����ʼ������***************************/
int initialize_enterscreen(ITEM * pHead);
ITEM * create(void);
void textinput(int x6, int xfw, int yfw, char* q); //ʵ�������������
void textsettings(void);
void draw_items(ITEM * pHead);
void shop(void);                                //�̵����,Ҫ�½��������洢��Ϣ���ļ����ٽ�����һ�أ��ٴ��½���
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
int key_judge(void);                              //�ж���ʲô����
int state_judge(int x1, int x2, int y1, int y2);//�ж����λ���Ƿ����Ҫ�� x1-x2,y1-y2
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
/**********************************������**********************************/
int main()
{
	ITEM* pHead = create();
	DWORD t1, t2;
	t1 = t2 = GetTickCount(); 
	initgraph(960, 540, SHOWCONSOLE);
	
	textsettings();              //������ʽ����
	printf("���ԣ�������������\n");
	if (initialize_enterscreen(pHead))    //��ʼ����
		draw_items(pHead);                    //����������
	else
	{
		TCHAR s[] = _T("�����������ϵ�ң�");
		outtextxy(200, 240, s);
	}
	/**************************************��һ��*************************************/
	
	BeginBatchDraw();
	while (1)
	{
		printf("����ѭ���˰�����������������������\n");
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
		printf("�ؿ������ж����ԣ����ж��ɹ����ѹ���\n");
		LEVEL++;
		com_value = 0;
		shop();
	}
	else
	{
		printf("�ؿ������ж����ԣ����ж��ɹ���δ����\n");
		dead();
		return 0;
	}
	/**************************************�ڶ���*************************************/
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
	/**************************************������*************************************/
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
		pass();//��ϲ���� end
	}
	else
		dead();
	/**************************************����*************************************/
	_getch();
	closegraph();
	return 0;
}
/**************************************************************************/
/*******************************�Զ��庯��ԭ��******************************/
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
	printf("���ԣ����������Դ�ɹ�\n");
	//��ʼ���ؿ����
	LEVEL = 1;
	//��ʼ��ʱ��
	TIME = 100 * LEVEL_ONE_TIMELIMIT;
	//��ʼ��PAUSE��ť�ж�
	PAUSE = FALSE;
	click_result = FALSE;
	//��ʼ������
	value_trans = 0;
	total_value = 0;
	com_value = 0;

	//��ʼ����Ʒ
	ITEM* pNow = (ITEM*)malloc(sizeof(ITEM));
	pNow = pHead->next;
	printf("��ʼ�����ԣ�\n");
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
	
	//��ʼ����Ʒ
	bakudan_value = BAKUDAN_VALUE;
	clock_value = CLOCK_VALUE;
	
	//��ʼ������
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
	//��ͷ
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
	//��ʼ����
	if (state_judge(120, 260, 430, 520))
	{
		printf("���ԣ������ʼ��ť��������Ϣ��½��������\n");
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
	printf("��ǰ�ѻ�ȡ����ĸ:\n");
	while ((*q = _getch()) != 13)          //�ո�13
	{
		printf("%c\t", *q);
		if (*q == VK_BACK && x6 != xfw)
		{
			*q = ' ';
			setbkcolor(WHITE);             //set-word-background-color
			setbkmode(OPAQUE);             //background-mode: TRANSPARENT\OPAQUE
			outtextxy(x6, yfw, q);
			x6 -= 22;                      //ɾ��ʱ ���
			--q;                          //ָ����ǰ�ƶ�
		}
		else if (*q == VK_BACK)
			continue;
		else
		{
			outtextxy(x6 += 22, yfw, q);              //��ʾΪ��ĸ�����û���    ���ʱ ���
			++q;                                        //ָ������ƶ�
		}
	}
	*q = '\0';
	printf("\n%s\n", q);
}
void textsettings(void)
{
	//��������
	LOGFONT f;
	gettextstyle(&f);						// ��ȡ��ǰ��������
	f.lfHeight = 48;						// ��������߶�Ϊ 48
	_tcscpy_s(f.lfFaceName, _T("����"));		// ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	return;
}
void draw_items(ITEM * pHead)//�����ܺ���
{
	
	IMAGE background, brick, char1, char2, char1_mask, char2_mask;
	IMAGE gold, money, stone;
	IMAGE gold_bk, money_bk, stone_bk;//��δ����
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
	//���ؽ���ʯͷǮ����ͼƬ
	loadimage(&gold, _T("..//picture//pictures//big_gold.png"));
	loadimage(&money, _T("..//picture//pictures//diamond.png"));
	loadimage(&stone, _T("..//picture//pictures//small_stone.jpg"));
	loadimage(&money_bk, _T("..//picture//pictures//diamond_mask.png"));
	loadimage(&stone_bk, _T("..//picture//pictures//small_stone_mask.jpg"));
	
	
	putimage(0, 0, &background);
	putimage(437, 0, &char1_mask, SRCPAINT);
	putimage(437, 0, &char1, SRCAND);
	
	//���Ʊ߽磨�ϣ�
	for (x = 0, y = 69; x <= 880; x += 80)
		putimage(x, y, &brick);

	//�����̵갴ť 
	setfillcolor(BROWN);
	setlinecolor(BROWN);
	settextcolor(WHITE);
	settextstyle(40, 0, _T("����"));
	fillrectangle(730, 10, 820, 66);
	outtextxy(735, 20, "�̵�");

	//������ͣ��ť
	fillrectangle(840, 10, 930, 66);
	outtextxy(845, 20, "��ͣ");

	//���Ƶ÷�Ŀ�ꡢ�÷֡�����
	settextstyle(30, 0, _T("����"));
	outtextxy(150, 20, "�÷�");
	outtextxy(10, 20, "Ŀ��");
	outtextxy(290, 20, "ʱ��");
	_stprintf_s(TARGET, _T("%d"), target[LEVEL - 1]);
	outtextxy(80, 20, TARGET);
	//�����û���
	settextcolor(RED);
	settextstyle(20, 0, "����");
	outtextxy(550, 45, "����,");
	outtextxy(600, 45, name);
	//���ƹؿ����
	_stprintf_s(LEVEL_NUM, _T("%d"), LEVEL);
	outtextxy(580, 20, LEVEL_NUM);
	outtextxy(550, 20, "��   ��");
	//������Ʒ
	pNow_1 = (ITEM*)malloc(sizeof(ITEM));
	pNow_1 = pHead->next;
	while (NULL != pNow_1)
	{
		printf("����ѭ��\n");
		if (pNow_1->flag == true)
		{
			switch (pNow_1->type)
			{
			case GOLD:
				printf("����ѭ��_1\n");
				printf("%d\t%d\n", pNow_1->x, pNow_1->y);
				//putimage(100, 100, &gold); printf("����ͼƬ\n");
				putimage(pNow_1->x, pNow_1->y, &gold_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &gold, SRCAND);
				break;
			case DIAMOND:
				printf("����ѭ��_2\n");
				putimage(pNow_1->x, pNow_1->y, &money_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &money, SRCAND);
				break;
			case STONE:
				printf("����ѭ��_3\n");
				putimage(pNow_1->x, pNow_1->y, &stone_bk, SRCPAINT);
				putimage(pNow_1->x, pNow_1->y, &stone, SRCAND);
				break;
			}
		}
		pNow_1 = pNow_1->next;
	}
	
	//���ƹ���
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	line(hook.x, hook.y, hook.endx, hook.endy);
	printf("���ƹ���\n");
	//���ƹ���ͷ
	putimage(gouzi.x, gouzi.y, &gouzitou_mask, SRCPAINT);
	putimage(gouzi.x, gouzi.y, &gouzitou,SRCAND);

	//���Ʒ���
	settextstyle(30, 0, _T("����"));
	_stprintf_s(value, _T("%d"), value_trans);
	outtextxy(220, 20, value);
	//����ʱ
	REAL_TIME = TIME / 100;
	_stprintf_s(T_ime, _T("%d"), REAL_TIME);
	outtextxy(355, 20, T_ime);
	//Sleep(1000);
	printf("ѭ��\n");
	//Sleep(1000);
	//��ť����жϣ���δʵ�֣�
	/*
	if (state_judge(840, 930, 19, 69) == 1)//��������ð�ťstate_judge(840, 930, 19, 69)
		settings();
	if (state_judge(730, 820, 19, 69) == 1)//������̵갴ťstate_judge(730, 820, 19, 69)
		shop();                      //�����̵����
	*/
	}
int key_judge(void)
{
	char key;

	key = _getch();
	if (key == 13)    //�س�����13
		return 13;
	else if (key == 27)    //esc����27
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
		//��ʾ��ʾ
		outtextxy(360, 440, "������԰���ը��ʯͷ�Ի�ø������һ��ץȡ");
		//��ʾ���н��
		outtextxy(430, 120, "���");
		_stprintf_s(COINS, _T("%d"), coins);
		outtextxy(500, 120, COINS);
		//��ʾ���е�������
		outtextxy(200, 500, "�����е���������");
		outtextxy(380, 500, "bakudan:");
		outtextxy(400, 550, "clock:");
		_stprintf_s(BAKUDAN_NUM, _T("%d"), bakuda_num);
		outtextxy(450, 500, BAKUDAN_NUM);
		_stprintf_s(CLOCK_NUM, _T("%d"), clock_num);
		outtextxy(450, 550, CLOCK_NUM);
		//��ʾ���߼۸�
		outtextxy(378, 345, "200");
		outtextxy(460, 345, "250");

		if (button_judge(360, 355, 397, 428)) //ը��ͼ���ж�
		{
			if (coins >= bakudan_value)
			{
				coins = coins - bakudan_value;
				bakuda_num++;
				printf("������ԣ���Ϣ���²��ԣ�-��������coins-%d total_num+%d\n", bakudan_value, bakuda_num);
			}
			else
				outtextxy(160, 550, "��Ҳ��㣬�޷�����");
		}
		if ((button_judge(420, 355, 500, 435))) //�ӱ�ͼ���ж�
		{
			if (coins >= clock_value)
			{
				coins = coins - clock_value;
				clock_num++;
			}
			else
				outtextxy(160, 550, "��Ҳ��㣬�޷�����");
		}
		if (button_judge(610, 90, 730, 150))
		{
			printf("��ť���ԣ���������-�û������ť��������һ��\n");
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
	/*************************************************�ж���ͣ*************************************************/
	
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
	/******************************************************�ж���ͣ********************************************************/
	
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
		printf("���Ӱڶ�����-���ڰڶ�\t������ĩλ�á�endx=%d\tendy=%d\n", hook.endx, hook.endy);
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
		printf("������������-�������ٶȡ�speedx=%d\tspeedy=%d\n", hook.speed_x, hook.speed_y);
	}
	//�߽��ж�
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
		printf("������������-������״̬��-�쳤 endx=%d\tendy=%d\n", hook.endx, hook.endy);
	}
	else if (hook.action == M_SHORT)
	{
		hook.endx -= hook.speed_x;
		hook.endy -= hook.speed_y;
		gouzi.x -= gouzi.speed_x;
		gouzi.y -= gouzi.speed_y;
		printf("������������-������״̬��-���� endx=%d\tendy=%d\n", hook.endx, hook.endy);
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
			printf("�����ж����ܲ��ԣ�����ץ����\n");
			
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
		printf("�������̰���������������������������\n");
		if (distant(hook))
		{
			value_trans += pReal->value;
			pReal->flag = false;
			pReal->find = false;
			hook.action = M_NOMAL;
			printf("������ɰ�������������������������������������\n");
			printf("�÷�ϵͳ���ԣ�����ץ������type=%d����ȡ�÷�points=%d��\n", pReal->type, pReal->value);
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
	printf("��������жϲ��ԣ������������-����-δ����\n");
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
	printf("��������жϲ��ԣ������������-����-ͨ��\n");
	//�ѱ�����Ϸ����߷�д���ļ�
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
	/*��ȡ���ṹ�壬ѭ�����*/;
	printf("��ȡ���ݲ��ԣ�\t");
	FILE* read = fopen("data.txt", "r");
	if (read == NULL)
	{
		outtextxy(100, 100, "��Ϸ��Ϣ����ʧ��");
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
		outtextxy(100, 100, "��Ϸ��Ϣ����ʧ��");
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
	printf("����ȡ���ݳɹ���-�ļ��ر�\n");
	fclose(read);
	printf("������ԣ�\t");
	i = 0;
	//����
	int j;
	int max;
	INFO temp;
	printf("����ʼ����\n");
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
		printf("����������%d/%d��", i+1, data_num);
	}
	printf("\n��������ɡ�\n");
	i = 0;
	//��ʾ����
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("����"));
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
	printf("�ļ�д����ԣ�\t");
	FILE* write = fopen("data.txt", "a+");
	if (write == NULL)
	{
		outtextxy(100, 100, "��Ϸ��Ϣ����ʧ��");
		Sleep(1000);
		return ;
	}
	INFO * data = (INFO*)malloc(sizeof(INFO));
	if (data)
	{
		printf("���ɹ������ļ�ָ�롿\n");
		strcpy(data->name, name);
		data->points = total_value;
		fprintf(write, "%s\t%d\n", data->name, data->points);
		printf("���ɹ�д�롿-�ɹ������ļ�ָ�벢���ļ�-���и�ʽ��д�����\n");
	}
	else
	{
		outtextxy(100, 100, "��Ϸ����ʧ��");
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
			outtextxy(100,100,"�����ڴ�ʧ��");
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
				printf("�����ڴ�ʧ��");
				exit(-1);
			}
			//pNew->size = 1;
			pTail->next = pNew;
			pNew->next = NULL;
			pTail = pNew;
		}

		return pHead_0;
	}
void use_prop(void) //z��ʱ�俨(0x5A) a�ǹ��ؿ�(0x41)
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