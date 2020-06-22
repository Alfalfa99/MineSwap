#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

int FLAG = 100;									//如果插上旗将会使该格子无法被点击 
int	hang, lie, minenum;							//定义初始的行列数和雷数 
int iwidth , iheight;							//定义图片的宽高 
int map[999][999];								//创建地图! 
int dec, sign;									//转换时间函数中的参数 
double start_time, finish_time;					//开始和结束的时间 
int cost_time;									//记录花费的全部时间
char timetime[20];								//将花费的时间转为字符串类型 

struct best_miner{
	char name[100];				//新纪录的创造者 
	char n_hard[5];				//新纪录的难度 
	char time[10];				//新纪录所花的时间
	struct best_miner *next; 
};

struct best_miner *h;
void gameover(int hard_switch);										//判断游戏是否已经结束
void ini_system();													//初始化系统的各项数据 
void hardmode(int hard_switch);										//设置难度函数 
void setmine();														//布雷函数
void printmine();													//输出一个格子附近的雷数 
void setmap();														//取得格子附近的雷数
void show_space(int x,int y);										//展示空地 
void show_num(int x, int y);										//展示图片 
void print_score();													//读取扫雷英雄榜
void create(int hard_switch);										//创建链表 
void scan_score(int hard_switch);									//写入扫雷英雄榜 

int main(void)
{	

	int i, j, x, y;
	int hard_switch = 1;			//设置初始难度等级为1(简单) 
	char winword[] = "你赢了!";
	MOUSEMSG m;
	IMAGE flag, block, enmined, mine, quote, space, face;

startgame:	ini_system();			//初始化系统数据 
	create(hard_switch);	//创建链表,导入数据 
	hardmode(hard_switch);	//设置难度 
	setmine();						//布雷函数
	setmap();						//取得格子附近的雷数
	//------------------------------------------------------导入图片以及绘制界面 
	initgraph(hang * iwidth, lie * iheight + 2 * iheight);

    setfillstyle(SOLID_FILL);		//顶部白框框的实现
    setfillcolor(LIGHTGRAY);
    floodfill(0,2 * iheight,WHITE);
    
    
	loadimage(&flag, _T("flag.bmp"), 25, 25);
	loadimage(&block, _T("block.bmp"), 25, 25);
	loadimage(&enmined, _T("enmined.bmp"), 25, 25);
	loadimage(&mine, _T("mine.bmp"), 25, 25);
	loadimage(&quote, _T("quote.bmp"), 25, 25);
	loadimage(&space, _T("space.bmp"), 25, 25);
	loadimage(&face, _T("face.bmp"), hang * iwidth / 4, 2 * iheight);

	for (i = 0; i < hang; i++)
	{
		for (j = 0; j < lie; j++)
		{
			/*	if (map[i][j] != -1)
				{
					putimage(i * iwidth, j * iheight + 2 * iheight, &block);	//填上白块 
				}
				else		//调试模式 
				{
					putimage(i * iwidth, j * iheight + 2 * iheight, &mine);		//填上雷块 
				}*/
				
				putimage(i * iwidth, j * iheight + 2 * iheight, &block);	//填上白块 
		}
	}
	
	putimage(hang * iwidth / 2.75,  0.075 * iheight, &face);	//滑稽脸重置
	start_time = clock();	//开始计时 
	while (1)
	{
		m=GetMouseMsg();
		gameover(hard_switch);	//使用gameover函数判断是否游戏结束

		x=m.x/25;				//获取鼠标点击的雷的数组行下标 
        y=(m.y-2*iheight)/25;	//获取鼠标点击的雷的数组列下标 
		switch(m.uMsg)
			   {
			   		case WM_LBUTTONDOWN:	//鼠标左键的事件 
			   			{
						   if (m.x <= hang * iwidth && m.x >= 0 && m.y <= lie * iheight + 2*iheight && m.y > 2*iheight)	//鼠标点击在地图区域
      					 	{
      					 		if (map[x][y] == -1)	//如果点到雷
								{
									putimage(x * iwidth, y * iheight + 2 * iheight, &enmined);
									start_time = 0;					//失败后重置开始时间 
								/*	for (i = 0; i < hang; i++)		//失败后锁定全部格子不再能点击
									{
										for (j = 0; j < lie; j++)
										{
											map[i][j] = FLAG;
										}
									}
									goto startgame;*/
								} 
								else if (map[x][y] == 0)
								{
									show_space(x, y);			//连锁打开
								}
								else if (map[x][y] >=1 && map[x][y] <= 8)
								{
									show_num(x, y);
								}
       						} 
       						else if (m.x <= hang * iwidth / 2 && m.x >= 0 && m.y <= 2 * iheight && m.y>= 0)//鼠标点击在笑脸上 
							{
								goto startgame;
							}
       						gameover(hard_switch);	//使用gameover函数判断是否游戏结束 
       					}break;
       				case WM_RBUTTONDOWN:	//鼠标右键的事件 
       					{
       						if (m.x <= hang * iwidth && m.x >= 0 && m.y <= lie * iheight + 2*iheight && m.y>= 0)	//鼠标点击在地图区域
							{
								if (map[x][y] > -50 && map[x][y] < 10)	//如果该格还没插旗子 
								{
									map[x][y] -= FLAG;
									putimage(x * iwidth, y * iheight + 2 * iheight, &flag);
									gameover(hard_switch);	//使用gameover函数判断是否游戏结束 
								}
								else if(map[x][y] < -50)		//如果已经插旗子 
								{
									map[x][y] += FLAG;
									putimage(x * iwidth, y * iheight + 2 * iheight, &block);
									gameover(hard_switch);	//使用gameover函数判断是否游戏结束 
								}
							}
						} break;
			   } 
        
        FlushMouseMsgBuffer();
	}


	return 0;
} 

void ini_system()	//初始化系统的各项数据 
{
	int i, j;
	
	for (i = 0; i < 999; i++)
	{
		for(j = 0; j < 999; j++)
		{
			map[i][j] = 0;			//初始化数组 
		}
	}
	
	iwidth = 25;		//初始化图片尺寸 
	iheight = 25;
	
	start_time = 0;	//初始化开始时间 
	finish_time = 0;	//初始化结束时间 
	hang = 15;		//初始化行数 
	lie = 15;		//初始化列数 
	minenum = 30;	//初始化雷数 
}
 
void hardmode(int hard_switch)//设置难度函数 
{
	char str[64];
	
start:	char *p = "请选择数字选项\n1.简单\n2.普通\n3.困难\n4.自定义(无法记录排名)\n5.排行榜\n6.退出";
	for(bool b = false; !b; )
	{
		b = !!InputBox(str, 64, p, "准备初始化游戏", "1" , 620);
		sscanf(str, "%d", &hard_switch);
		if(hard_switch != 1 && hard_switch != 2 && hard_switch != 3 && hard_switch != 4 && hard_switch != 5 && hard_switch != 6)
		{
			b = false;
			p = "请选择数字选项\n1.简单\n2.普通\n3.困难\n4.自定义(无法记录排名)\n5.排行榜\n6.退出";
		}
	}
	if (hard_switch == 1)
	{
		lie = hang = 10;
		minenum = 20;
	}
	else if (hard_switch == 2)
	{
		lie = hang = 15;
		minenum = 35;
	}
	else if (hard_switch == 3)
	{
		lie = hang = 20;
		minenum = 50;
	}
	else if (hard_switch == 4)
	{
		char *p = "请输入所需的列数(width)、行数(height)以及地雷数，用空格隔开\n警告!该难度下不会记录排名!";
		for(bool b = false; !b; )
		{
			b = !!InputBox(str, 64, p, "准备初始化游戏", "15 15 30" , 620);
			sscanf(str, "%d%d%d", &lie, &hang, &minenum);
			if(lie < 1 || hang < 1 || minenum < 0 || minenum >= hang*lie - 1)
			{
				b = false;
				p = "输入的列数不合法!请重新输入!\n请输入所需的列数(width)、行数(height)以及地雷数，用空格隔开\n警告,该难度下不会记录排名!";
			}
		}
	}
	else if (hard_switch == 5)
	{
		print_score();
		system("pause");
		goto start;
	}
	else if (hard_switch == 6)
	{
		exit(0);		//退出程序
	}
}

void setmine()		//布雷函数
{
	int i, j, k, num = 0;
	
	srand((unsigned)time(NULL));	//初始化随机器
	while(num < minenum)	//随机布雷 
    {
        k=rand() % (hang * lie);		//布好一个雷后布下一个
        j = k % lie;
		i = k / hang;
		if (map[i][j] == 0)
       	{
        	map[i][j] = -1;
        	num++;
        }
    } 
} 

void setmap()	//取得格子附近的雷数
{
	int i,j;
    for (i=0;i<hang;i++)
	{
        for (j=0;j<lie;j++)
        {
            if (j < lie-1 && i<hang-1 && map[i][j]==-1 && map[i+1][j+1]!=-1)
                map[i+1][j+1]++;
            if (i<hang && map[i][j]==-1 && map[i+1][j]!=-1)
                map[i+1][j]++;
            if (i<hang-1 && j>0 && map[i][j]==-1 && map[i+1][j-1]!=-1)
                map[i+1][j-1]++;
            if (j>0 && map[i][j]==-1 && map[i][j-1]!=-1)
                map[i][j-1]++;
            if (i>0 && j>0 && map[i][j]==-1 && map[i-1][j-1]!=-1)
                map[i-1][j-1]++;
            if (i>0 && map[i][j]==-1 && map[i-1][j]!=-1)
                map[i-1][j]++;
            if (i>0 && j<lie-1 && map[i][j]==-1 && map[i-1][j+1]!=-1)
                map[i-1][j+1]++;
            if (j<lie-1 && map[i][j]==-1 && map[i][j+1]!=-1)
                map[i][j+1]++;
        }
	}
}

void show_space(int x,int y)	//展示空地 
{
	IMAGE space;
	
	loadimage(&space, _T("space.bmp"), 25, 25);
	
    if(x>=0 && x<hang && y>=0 && y<lie && map[x][y]==0)
    {
        putimage(x * iwidth, y * iheight + 2 * iheight, &space);
        map[x][y] += FLAG * 5;	//标记该点已被打开过 
        show_space(x-1,y);
        show_space(x+1,y);
        show_space(x,y-1);
        show_space(x,y+1);
    }
    if(x>=0 && x < hang && y>=0 && y<lie && map[x][y]>0 && map[x][y]<=8)
    {
        show_num(x,y);
    }
}

void show_num(int x, int y)		//展示图片 
{
	IMAGE p1, p2, p3, p4, p5, p6, p7, p8;
	
	loadimage(&p1, _T("1.bmp"), 25, 25);
	loadimage(&p2, _T("2.bmp"), 25, 25);
	loadimage(&p3, _T("3.bmp"), 25, 25);
	loadimage(&p4, _T("4.bmp"), 25, 25);
	loadimage(&p5, _T("5.bmp"), 25, 25);
	loadimage(&p6, _T("6.bmp"), 25, 25);
	loadimage(&p7, _T("7.bmp"), 25, 25);
	loadimage(&p8, _T("8.bmp"), 25, 25);
	
    if (map[x][y]==1)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p1);
    if (map[x][y]==2)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p2);
    if (map[x][y]==3)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p3);
    if (map[x][y]==4)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p4);
    if (map[x][y]==5)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p5);
    if (map[x][y]==6)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p6);
    if (map[x][y]==7)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p7);
    if (map[x][y]==8)
        putimage(x * iwidth, y * iheight + 2 * iheight, &p8);
	map[x][y]+=FLAG * 5;			//标记该点已被打开过 
}

void gameover(int hard_switch)		//判断游戏是否已经结束
{
	int i, j;
	int opennum = 0, F_mine = 0, err_flag = 0;	//打开的格子数和插上旗子的雷数和错误插旗的格子数
	
	for (i = 0; i < hang; i++)
	{
		for (j = 0; j < lie; j++)
		{
			if (map[i][j] > 400)	//判断被插旗的格子数和被打开的格子数 
			{
				opennum++;
			}
			if (map[i][j] == -101)	//判断被插旗子的格子是不是雷
			{
				F_mine++;
			}
			if (map[i][j] < -50 &&map[i][j] != -101)	//判断格子是否被错误插旗
			{
				err_flag += 1;
			}
		}
	}
	if ((opennum == (hang * lie - minenum) || F_mine == minenum) && err_flag == 0)
	{
		finish_time = clock();	//游戏结束,结束计时 
		cost_time = (int)(finish_time - start_time) / CLOCKS_PER_SEC;
		itoa(cost_time, timetime, 10);
       	outtextxy((hang * iwidth) - 50,  2 * iheight / 2, timetime);
       							
       	RECT r = {0, 0, hang * iwidth, lie * iheight + 2 * iheight};
		drawtext(_T("你赢了!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		for (i = 0; i < hang; i++)		//游戏结束后锁定全部格子不再能点击
		{
			for (j = 0; j < lie; j++)
			{
				map[i][j] = FLAG;
			}
		}
		scan_score(hard_switch);
	}
}

void create(int hard_switch)		//创建链表 
{
		FILE *fp;
		struct best_miner *p1, *p2;
		
		if ((fp=fopen("score.txt","r")) == NULL)
		{
			h = NULL;
		}
		else
		{
			h = p2 = (struct best_miner *)malloc(sizeof(struct best_miner));
			if (h == NULL)
			{
				h = p2;
			}
			while (fscanf(fp,"%s%s%s",p2->name, p2->n_hard, p2->time) != EOF) //将文件数据导入链表 
   			{
   				p1 = (struct best_miner *)malloc(sizeof(struct best_miner));
      			p2->next = p1;
				p2 = p1;
   			}
			p2->next = NULL;
  		}

}

void print_score()				//读取扫雷英雄榜 
{
	int i = 0, j = 0;

	FILE *fp;
	struct best_miner * p1;
	
	initgraph(800,600);
	RECT r = {0, 0, 800, 600};
	if ((fp=fopen("score.txt","r")) == NULL)
	{
		drawtext(_T("尚无记录!点击回车键继续"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
  		p1 = h;
		while (p1->next != NULL)	//将链表数据输出 
		{
			outtextxy(i, j, p1->name);
			outtextxy(i+100, j, p1->n_hard);
			outtextxy(i+200, j, p1->time);
			p1 = p1->next;
			j+=20;
		}
		fclose(fp);
	}

}

void scan_score(int hard_switch)		//写入扫雷英雄榜 
{
	char str[64];
	char *pt = "请输入您的大名!";
	FILE*fs;
	struct best_miner *p, *p0;
	
	fs=fopen("score.txt","w+");
	p = p0 = h;
	if (p == NULL)
	{
		p = (struct best_miner *)malloc(sizeof(struct best_miner));
		InputBox(str, 64, pt, "新纪录!", "匿名" , 620);
		sscanf(str, "%s", p->name);
		if(strcmp(p->name, "") == 0)
		{
			strcpy(p->name, "匿名"); 
		}
		if (hard_switch == 1)
		{
			strcpy(p->n_hard, "简单"); 
		}
		else if (hard_switch == 2)
		{
			strcpy(p->n_hard, "普通"); 
		}
		else if (hard_switch == 3)
		{
			strcpy(p->n_hard, "困难"); 
		}
		fprintf(fs,"%s %s %s\n",p->name, p->n_hard, timetime);
		fclose(fs);
	} 
	else if (hard_switch != 4 && p != NULL)
	{
		while(p->next != NULL && p != NULL)
		{
			if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "简单") == 0))
			{
				InputBox(str, 64, pt, "新纪录!", "匿名" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "匿名"); 
				}
				strcpy(p->n_hard, "简单"); 
				strcpy(p->time, timetime); 
				break; 
			}
			else if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "普通") == 0))
			{
				InputBox(str, 64, pt, "新纪录!", "匿名" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "匿名"); 
				}
				strcpy(p->n_hard, "普通"); 
				strcpy(p->time, timetime); 
				break;
			}
			else if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "困难") == 0))
			{
				InputBox(str, 64, pt, "新纪录!", "匿名" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "匿名"); 
				}
				strcpy(p->n_hard, "困难"); 
				strcpy(p->time, timetime); 
				break;
			}
			p = p->next;
		}
		p = h;
		while (p->next != NULL)
		{
			fprintf(fs,"%s %s %s\n",p->name, p->n_hard, p->time);
			p = p->next;
		}
		fclose(fs);
	}
}
