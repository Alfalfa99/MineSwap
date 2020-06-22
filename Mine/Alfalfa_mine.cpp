#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

int FLAG = 100;									//��������콫��ʹ�ø����޷������ 
int	hang, lie, minenum;							//�����ʼ�������������� 
int iwidth , iheight;							//����ͼƬ�Ŀ�� 
int map[999][999];								//������ͼ! 
int dec, sign;									//ת��ʱ�亯���еĲ��� 
double start_time, finish_time;					//��ʼ�ͽ�����ʱ�� 
int cost_time;									//��¼���ѵ�ȫ��ʱ��
char timetime[20];								//�����ѵ�ʱ��תΪ�ַ������� 

struct best_miner{
	char name[100];				//�¼�¼�Ĵ����� 
	char n_hard[5];				//�¼�¼���Ѷ� 
	char time[10];				//�¼�¼������ʱ��
	struct best_miner *next; 
};

struct best_miner *h;
void gameover(int hard_switch);										//�ж���Ϸ�Ƿ��Ѿ�����
void ini_system();													//��ʼ��ϵͳ�ĸ������� 
void hardmode(int hard_switch);										//�����ѶȺ��� 
void setmine();														//���׺���
void printmine();													//���һ�����Ӹ��������� 
void setmap();														//ȡ�ø��Ӹ���������
void show_space(int x,int y);										//չʾ�յ� 
void show_num(int x, int y);										//չʾͼƬ 
void print_score();													//��ȡɨ��Ӣ�۰�
void create(int hard_switch);										//�������� 
void scan_score(int hard_switch);									//д��ɨ��Ӣ�۰� 

int main(void)
{	

	int i, j, x, y;
	int hard_switch = 1;			//���ó�ʼ�Ѷȵȼ�Ϊ1(��) 
	char winword[] = "��Ӯ��!";
	MOUSEMSG m;
	IMAGE flag, block, enmined, mine, quote, space, face;

startgame:	ini_system();			//��ʼ��ϵͳ���� 
	create(hard_switch);	//��������,�������� 
	hardmode(hard_switch);	//�����Ѷ� 
	setmine();						//���׺���
	setmap();						//ȡ�ø��Ӹ���������
	//------------------------------------------------------����ͼƬ�Լ����ƽ��� 
	initgraph(hang * iwidth, lie * iheight + 2 * iheight);

    setfillstyle(SOLID_FILL);		//�����׿���ʵ��
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
					putimage(i * iwidth, j * iheight + 2 * iheight, &block);	//���ϰ׿� 
				}
				else		//����ģʽ 
				{
					putimage(i * iwidth, j * iheight + 2 * iheight, &mine);		//�����׿� 
				}*/
				
				putimage(i * iwidth, j * iheight + 2 * iheight, &block);	//���ϰ׿� 
		}
	}
	
	putimage(hang * iwidth / 2.75,  0.075 * iheight, &face);	//����������
	start_time = clock();	//��ʼ��ʱ 
	while (1)
	{
		m=GetMouseMsg();
		gameover(hard_switch);	//ʹ��gameover�����ж��Ƿ���Ϸ����

		x=m.x/25;				//��ȡ��������׵��������±� 
        y=(m.y-2*iheight)/25;	//��ȡ��������׵��������±� 
		switch(m.uMsg)
			   {
			   		case WM_LBUTTONDOWN:	//���������¼� 
			   			{
						   if (m.x <= hang * iwidth && m.x >= 0 && m.y <= lie * iheight + 2*iheight && m.y > 2*iheight)	//������ڵ�ͼ����
      					 	{
      					 		if (map[x][y] == -1)	//����㵽��
								{
									putimage(x * iwidth, y * iheight + 2 * iheight, &enmined);
									start_time = 0;					//ʧ�ܺ����ÿ�ʼʱ�� 
								/*	for (i = 0; i < hang; i++)		//ʧ�ܺ�����ȫ�����Ӳ����ܵ��
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
									show_space(x, y);			//������
								}
								else if (map[x][y] >=1 && map[x][y] <= 8)
								{
									show_num(x, y);
								}
       						} 
       						else if (m.x <= hang * iwidth / 2 && m.x >= 0 && m.y <= 2 * iheight && m.y>= 0)//�������Ц���� 
							{
								goto startgame;
							}
       						gameover(hard_switch);	//ʹ��gameover�����ж��Ƿ���Ϸ���� 
       					}break;
       				case WM_RBUTTONDOWN:	//����Ҽ����¼� 
       					{
       						if (m.x <= hang * iwidth && m.x >= 0 && m.y <= lie * iheight + 2*iheight && m.y>= 0)	//������ڵ�ͼ����
							{
								if (map[x][y] > -50 && map[x][y] < 10)	//����ø�û������ 
								{
									map[x][y] -= FLAG;
									putimage(x * iwidth, y * iheight + 2 * iheight, &flag);
									gameover(hard_switch);	//ʹ��gameover�����ж��Ƿ���Ϸ���� 
								}
								else if(map[x][y] < -50)		//����Ѿ������� 
								{
									map[x][y] += FLAG;
									putimage(x * iwidth, y * iheight + 2 * iheight, &block);
									gameover(hard_switch);	//ʹ��gameover�����ж��Ƿ���Ϸ���� 
								}
							}
						} break;
			   } 
        
        FlushMouseMsgBuffer();
	}


	return 0;
} 

void ini_system()	//��ʼ��ϵͳ�ĸ������� 
{
	int i, j;
	
	for (i = 0; i < 999; i++)
	{
		for(j = 0; j < 999; j++)
		{
			map[i][j] = 0;			//��ʼ������ 
		}
	}
	
	iwidth = 25;		//��ʼ��ͼƬ�ߴ� 
	iheight = 25;
	
	start_time = 0;	//��ʼ����ʼʱ�� 
	finish_time = 0;	//��ʼ������ʱ�� 
	hang = 15;		//��ʼ������ 
	lie = 15;		//��ʼ������ 
	minenum = 30;	//��ʼ������ 
}
 
void hardmode(int hard_switch)//�����ѶȺ��� 
{
	char str[64];
	
start:	char *p = "��ѡ������ѡ��\n1.��\n2.��ͨ\n3.����\n4.�Զ���(�޷���¼����)\n5.���а�\n6.�˳�";
	for(bool b = false; !b; )
	{
		b = !!InputBox(str, 64, p, "׼����ʼ����Ϸ", "1" , 620);
		sscanf(str, "%d", &hard_switch);
		if(hard_switch != 1 && hard_switch != 2 && hard_switch != 3 && hard_switch != 4 && hard_switch != 5 && hard_switch != 6)
		{
			b = false;
			p = "��ѡ������ѡ��\n1.��\n2.��ͨ\n3.����\n4.�Զ���(�޷���¼����)\n5.���а�\n6.�˳�";
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
		char *p = "���������������(width)������(height)�Լ����������ÿո����\n����!���Ѷ��²����¼����!";
		for(bool b = false; !b; )
		{
			b = !!InputBox(str, 64, p, "׼����ʼ����Ϸ", "15 15 30" , 620);
			sscanf(str, "%d%d%d", &lie, &hang, &minenum);
			if(lie < 1 || hang < 1 || minenum < 0 || minenum >= hang*lie - 1)
			{
				b = false;
				p = "������������Ϸ�!����������!\n���������������(width)������(height)�Լ����������ÿո����\n����,���Ѷ��²����¼����!";
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
		exit(0);		//�˳�����
	}
}

void setmine()		//���׺���
{
	int i, j, k, num = 0;
	
	srand((unsigned)time(NULL));	//��ʼ�������
	while(num < minenum)	//������� 
    {
        k=rand() % (hang * lie);		//����һ���׺���һ��
        j = k % lie;
		i = k / hang;
		if (map[i][j] == 0)
       	{
        	map[i][j] = -1;
        	num++;
        }
    } 
} 

void setmap()	//ȡ�ø��Ӹ���������
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

void show_space(int x,int y)	//չʾ�յ� 
{
	IMAGE space;
	
	loadimage(&space, _T("space.bmp"), 25, 25);
	
    if(x>=0 && x<hang && y>=0 && y<lie && map[x][y]==0)
    {
        putimage(x * iwidth, y * iheight + 2 * iheight, &space);
        map[x][y] += FLAG * 5;	//��Ǹõ��ѱ��򿪹� 
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

void show_num(int x, int y)		//չʾͼƬ 
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
	map[x][y]+=FLAG * 5;			//��Ǹõ��ѱ��򿪹� 
}

void gameover(int hard_switch)		//�ж���Ϸ�Ƿ��Ѿ�����
{
	int i, j;
	int opennum = 0, F_mine = 0, err_flag = 0;	//�򿪵ĸ������Ͳ������ӵ������ʹ������ĸ�����
	
	for (i = 0; i < hang; i++)
	{
		for (j = 0; j < lie; j++)
		{
			if (map[i][j] > 400)	//�жϱ�����ĸ������ͱ��򿪵ĸ����� 
			{
				opennum++;
			}
			if (map[i][j] == -101)	//�жϱ������ӵĸ����ǲ�����
			{
				F_mine++;
			}
			if (map[i][j] < -50 &&map[i][j] != -101)	//�жϸ����Ƿ񱻴������
			{
				err_flag += 1;
			}
		}
	}
	if ((opennum == (hang * lie - minenum) || F_mine == minenum) && err_flag == 0)
	{
		finish_time = clock();	//��Ϸ����,������ʱ 
		cost_time = (int)(finish_time - start_time) / CLOCKS_PER_SEC;
		itoa(cost_time, timetime, 10);
       	outtextxy((hang * iwidth) - 50,  2 * iheight / 2, timetime);
       							
       	RECT r = {0, 0, hang * iwidth, lie * iheight + 2 * iheight};
		drawtext(_T("��Ӯ��!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		for (i = 0; i < hang; i++)		//��Ϸ����������ȫ�����Ӳ����ܵ��
		{
			for (j = 0; j < lie; j++)
			{
				map[i][j] = FLAG;
			}
		}
		scan_score(hard_switch);
	}
}

void create(int hard_switch)		//�������� 
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
			while (fscanf(fp,"%s%s%s",p2->name, p2->n_hard, p2->time) != EOF) //���ļ����ݵ������� 
   			{
   				p1 = (struct best_miner *)malloc(sizeof(struct best_miner));
      			p2->next = p1;
				p2 = p1;
   			}
			p2->next = NULL;
  		}

}

void print_score()				//��ȡɨ��Ӣ�۰� 
{
	int i = 0, j = 0;

	FILE *fp;
	struct best_miner * p1;
	
	initgraph(800,600);
	RECT r = {0, 0, 800, 600};
	if ((fp=fopen("score.txt","r")) == NULL)
	{
		drawtext(_T("���޼�¼!����س�������"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
  		p1 = h;
		while (p1->next != NULL)	//������������� 
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

void scan_score(int hard_switch)		//д��ɨ��Ӣ�۰� 
{
	char str[64];
	char *pt = "���������Ĵ���!";
	FILE*fs;
	struct best_miner *p, *p0;
	
	fs=fopen("score.txt","w+");
	p = p0 = h;
	if (p == NULL)
	{
		p = (struct best_miner *)malloc(sizeof(struct best_miner));
		InputBox(str, 64, pt, "�¼�¼!", "����" , 620);
		sscanf(str, "%s", p->name);
		if(strcmp(p->name, "") == 0)
		{
			strcpy(p->name, "����"); 
		}
		if (hard_switch == 1)
		{
			strcpy(p->n_hard, "��"); 
		}
		else if (hard_switch == 2)
		{
			strcpy(p->n_hard, "��ͨ"); 
		}
		else if (hard_switch == 3)
		{
			strcpy(p->n_hard, "����"); 
		}
		fprintf(fs,"%s %s %s\n",p->name, p->n_hard, timetime);
		fclose(fs);
	} 
	else if (hard_switch != 4 && p != NULL)
	{
		while(p->next != NULL && p != NULL)
		{
			if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "��") == 0))
			{
				InputBox(str, 64, pt, "�¼�¼!", "����" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "����"); 
				}
				strcpy(p->n_hard, "��"); 
				strcpy(p->time, timetime); 
				break; 
			}
			else if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "��ͨ") == 0))
			{
				InputBox(str, 64, pt, "�¼�¼!", "����" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "����"); 
				}
				strcpy(p->n_hard, "��ͨ"); 
				strcpy(p->time, timetime); 
				break;
			}
			else if ((strcmp(timetime, p->time) < 0) && (strcmp(p->n_hard, "����") == 0))
			{
				InputBox(str, 64, pt, "�¼�¼!", "����" , 620);
				sscanf(str, "%s", p->name);
				if(strcmp(p->name, "") == 0)
				{
					strcpy(p->name, "����"); 
				}
				strcpy(p->n_hard, "����"); 
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
