#include<iostream.h>
#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include<bios.h>

int BK_COLOR =0;
int TEXT_COLOR =15;
int CUR_COLOR =6;
int OUT_COLOR =9;
int SPCL_TXT_COLOR =12;
int TITLE_TXT_COLOR =11;


class Menu
{
	protected:
		int n1,x,y,arrow,poly[10];
		int menuBegin;
		int n;
		char task1[75],title1[75];
	public:
		void Menu:: caption(char tit[],char subTit[]);
		void Menu::clearWindow();
		void writeMenu(int ,char[][20] );
		void boundary();
		int menuActivate(char[][75]);
		void title(char []);
		void taskdisp(char []);
		void taskdisp(char [],char &);
		Menu();
};

Menu::Menu()
{
	strcpy(title1,"fby digital diary");
	strcpy(task1,"fby digital diary");
	x=490;n1=0;
	y=110;
	arrow=0;
	menuBegin=100;
}

void Menu::writeMenu(int n,char menuText[][20])
{
	settextjustify(LEFT_TEXT,TOP_TEXT);
	boundary();
	this->n=n;
	settextstyle(3,0,2);
	setcolor(TEXT_COLOR);
	for(int count=0;count<n;count++)
	{
		outtextxy(400+110,menuBegin,menuText[count]);
		menuBegin+=30;
	}
	menuBegin=100;
}

void Menu:: boundary()
{
	setcolor(OUT_COLOR);
	setbkcolor(BK_COLOR);
	rectangle(5,5,getmaxx()-5,getmaxy()-5);
	line(5,25,getmaxx()-5,25);
	line(5,getmaxy()-25,getmaxx()-5,getmaxy()-25);
	line(getmaxx()-150,26,getmaxx()-150,getmaxy()-25);
}

int Menu::menuActivate(char instruct[][75])
{
	int tn;
	tn=n1;
	do
	{
		setcolor(CUR_COLOR);
		poly[0]=x;
		poly[1]=y;
		poly[2]=x+17;
		poly[3]=y+5;
		poly[4]=x;
		poly[5]=y+10;
		poly[6]=x;
		poly[7]=y;
		setfillstyle(EMPTY_FILL+1,CUR_COLOR );
		fillpoly(4, poly);
		taskdisp(instruct[tn]);
		while (_bios_keybrd(_KEYBRD_READY) == 0);
		arrow = _bios_keybrd(_KEYBRD_READ);
		setfillstyle(EMPTY_FILL+1,0);
		setcolor(0);
		drawpoly(4,poly);
		fillpoly(4,poly);

		if(arrow==0x4800)
		{
			y-=30;
			--tn;
		}
		else if(arrow==0x5000)
		{
			y+=30;
			++tn;
		}
		if(y<110)
		{
			y=(n-1)*30+110;
			tn=n-1;
		}
		else if(y>((n-1)*30+110))
		{
			tn=0;
			y=110;
		}
	}while(arrow!=0x1c0d);
	n1=tn;
	menuBegin=100;
	return((y/30)-2);
}

void Menu::title(char title[])
{
	setcolor(BK_COLOR);
	outtextxy(10,15,title1);
	boundary();
	setcolor(TEXT_COLOR);
	settextstyle(0,0,1);
	outtextxy(10,15,title);
	strcpy(title1,title);
}

void Menu::taskdisp(char task[])
{
	setcolor(BK_COLOR);
	outtextxy(10,getmaxy()-15,task1);
	boundary();
	setcolor(TEXT_COLOR);
	settextstyle(0,0,1);
	outtextxy(10,getmaxy()-15,task);
	strcpy(task1,task);
}

void Menu::taskdisp(char task[],char& ysno)
{
	setcolor(BK_COLOR);
	outtextxy(10,getmaxy()-15,task1);
	boundary();
	setcolor(TEXT_COLOR);
	settextstyle(0,0,1);
	outtextxy(10,getmaxy()-15,task);
	ysno=getch();
	ysno=toupper(ysno);
	strcpy(task1,task);
}

void Menu::clearWindow()
{
	setfillstyle(1,BK_COLOR);
	bar(6,26,getmaxx()-151,getmaxy()-26);
}
void Menu:: caption(char tit[],char subTit[])
{
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(9);
	settextstyle(0,0,2);
	outtextxy(getmaxx()/2-50,getmaxy()/2,tit);
	outtextxy(getmaxx()/2-50,getmaxy()/2+30,subTit);
	settextjustify(LEFT_TEXT,TOP_TEXT);
}
