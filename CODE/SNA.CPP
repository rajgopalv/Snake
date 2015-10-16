/* FLUTTERBY INTERNATIONAL'S GAME OF SNAKE - 006 */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include"snake\code\menu.cpp"
#include<fstream.h>
#include <conio.h>
#include<dos.h>
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

void boundary()
{
  setcolor(6);
  rectangle(0,0,getmaxx(),getmaxy());
}

class coord{
	public: 
		unsigned  gp,hit;
		int radius;/* CHECK HIT TAIL / CHECK FOOD POSITION */
		void changPosition(int &direct,int &_x,int &_y,int &indicator,int color);
};

class head:virtual public coord
{
	protected:
		int head_move_x,head_move_y,first;
		int direction_head;  /* DIRECTION OF SNAKE */
};

class tail:virtual public coord
{
	protected:
		int tail_move_x,tail_move_y;
		int direction_tail,turn_tail;
};
class foods:virtual public head,virtual public tail
{
	public:
		int foods::put_bonus(int color,int &_x,int&_y);
};

class food:virtual public foods
{
	protected:
		int next_food,x_food,y_food;
};

class bonus:virtual public foods
{
	protected:
		int bonus_point,bonus_x,bonus_y,timer_activate,bonus_rand;
};

static char patterns[8]={ 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F };
class Snake:virtual public food,virtual public bonus,virtual public head,virtual public tail
{
	int i,j,points,level;   /* POINTS IS FOR BONUS / LEVEL-LEVEL */
	int value,eraze_snake[2][200]; /*STORE DATA TO ERAZE */
	char cont,dir[200]; /* DIRECTION OF ERAZE */  
	long int score;
	int high_score,maze,count,elongate;
	int gdriver , gmode, errorcode;
	char hi_score_name[20],draw_head_temp,draw_head,draw_tail;
	fstream hi_score_pointer;
	public:
	Snake();
	void refresh();
	void select_levels(int,int);
	void save_hi_score();
	void finish_bonus();
	int activeSnake();
	void bonus_timer();
	void draw_maze();
};

void Snake::finish_bonus()
{
	setcolor(0);setfillstyle(1,0); /* ERAZE BONUS */
	fillellipse(bonus_x,bonus_y,5,5);
	bonus_x=-1;
	bonus_y=-1;
	gotoxy(1,1);
	printf("   "); /* ERAZE BONUS COUNTDOWN */
	setcolor(70);
	boundary();  /* DRAW BOUNDARY */
}

Snake::Snake()
{
	points=0;
	cont='\0';
	direction_head=direction_tail=0;  /* DIRECTION OF SNAKE */
	score=0;
	bonus_x=bonus_y=-1;timer_activate=600;bonus_rand=1;
	for(i=0;i<9;i++)
	{
		eraze_snake[0][i]=0;
		eraze_snake[1][i]=0;
		dir[i]=0;
	}
	next_food=1;tail_move_x=-5;tail_move_y=-5;
	high_score=0;maze=1;radius = 3;count=0;elongate=21;
	strcpy(hi_score_name,"SYSTEM");
	draw_head_temp='\0';draw_head;draw_tail=RIGHT;
}

void Snake::save_hi_score()
{                                  /* store high score data */
	hi_score_pointer.open("hisna.txt",ios::out);
	high_score=score;
	hi_score_pointer<<score<<" ";
	cleardevice();
	settextstyle(3, HORIZ_DIR,3 );
	outtextxy(getmaxx()/2-220,getmaxy()/2-30,"Congratulations... You Scored The Highest Score..");
	outtextxy(getmaxx()/2-100,getmaxy()/2,"Enter Your Name   ");
	gotoxy(33,22);
	setcolor(70);
	boundary();  /* DRAW BOUNDARY*/
	scanf("%s",hi_score_name);
	hi_score_pointer<<hi_score_name;
	hi_score_pointer.close();
}

/* PLACE BONUS */
int foods::put_bonus(int color,int &_x,int&_y)
{
	int i,j;
	if(color){
		randomize();
		setcolor(2);
		do{
			_x=rand()%(getmaxx()-20);
			_y=rand()%(getmaxy()-20);
			_x+=10;
			_y+=10;
			for(i=_x;i<_x+20;i++)
			{
				for(j=_y;j<_y+20;j++)
				{                                 /*PUT BONUS ONLY IN EMPTY SPACE */
					gp=getpixel(i,j);
					if(gp!=0)
					{ 
						i=0;break;
					}
				}
				if(i==0)
					break;
			}
		}while(gp!=0);
		setcolor(15);
		setfillstyle(1,color);
	}
	if(color==0)
	{
		setcolor(0);
		setfillstyle(1,0);
	}
	fillellipse(_x,_y,4,4);
	return(0);
}

void Snake::select_levels(int lev,int maz)
{
	level=lev;
	maze=maz;
}
void Snake::refresh()
{
	hi_score_pointer.open("hisna.txt",ios::in);
	if(!hi_score_pointer) /*HIGH SCORE FILE */
	{
		hi_score_pointer.close();
		hi_score_pointer.open("hisna.txt",ios::out);
		hi_score_pointer<<hi_score_name<<" ";
		hi_score_pointer<<high_score;
		hi_score_pointer.close();
		cleardevice();
		hi_score_pointer.open("hisna.txt",ios::in);
	}
	next_food=1;tail_move_x=-5;tail_move_y=-5;
	hi_score_pointer>>high_score;
	hi_score_pointer>>hi_score_name;
	hi_score_pointer.close();
}

void Snake::draw_maze()
{
	if(maze==2||maze==3)    /*  DRAW BOX MAZE */
	{
		setcolor(13);
		rectangle(16,16,getmaxx()-16,getmaxy()-16);
		rectangle(17,17,getmaxx()-17,getmaxy()-17);
		rectangle(18,18,getmaxx()-18,getmaxy()-18);
	}
	head_move_x =100;
	head_move_y = getmaxy() / 2;
	if(maze==3)   /* DRAW "Z" MAZE */
	{
		setcolor(13);
		setfillpattern( &patterns[0], 13 );
		bar((getmaxx()/2)-200,(getmaxy()/2)-50,(getmaxy()/2)+250+3,(getmaxy()/2)-50+3);
		bar((getmaxx()/2)-200,(getmaxy()/2)+50,(getmaxy()/2)+250+3,(getmaxy()/2)+50+3);
		setfillstyle(1,0);
		bar(10,50,18,getmaxy()-50);
		bar(getmaxx()-18,50,getmaxx()-10,getmaxy()-50);
		bar(50,1,getmaxx()-50,50);
		bar(50,getmaxy(),getmaxx()-50,getmaxy()-50);
	}
	if(maze==4)
	{
		setcolor(13);
		setfillpattern( &patterns[0], 13 );
		bar(getmaxx()/2-100,getmaxy()/2-210,getmaxx()/2-95,getmaxy()/2+25);
		bar(getmaxx()/2+100,getmaxy()/2+210,getmaxx()/2+95,getmaxy()/2-25);
		bar(getmaxx()/2+190,getmaxy()/2-100,getmaxy()/2+50,getmaxy()/2-95);
		bar(getmaxx()/2-50,getmaxy()/2+100,getmaxy()/2-190,getmaxy()/2+95);
		head_move_x=getmaxx()/2;
		head_move_y=getmaxy()/2-50;
	}
	if(maze==5)
	{
		setcolor(13);
		setfillpattern( &patterns[0], 13 );
		bar(getmaxx()/2-170,30,getmaxx()-100,35);
		bar(10,getmaxy()/2+60,getmaxx()-5,getmaxy()/2+65);
		bar(10,getmaxy()/2,getmaxx()/2-50,getmaxy()/2+5);
		bar(getmaxx()/2-50,getmaxy()/2+5,getmaxx()/2+5-50,30);
		bar(getmaxx()/2+50,getmaxy()/2+5,getmaxx()-10,getmaxy()/2+10);
		bar(getmaxx()/2,getmaxy()/2+60,getmaxx()/2+5,getmaxy()-10);
		bar(30,30,70,35);
		bar(30,30,35,70);
		head_move_x=100;
		head_move_y=getmaxy()/2+35;
	}
}
void Snake::bonus_timer()
{
	gotoxy(1,1);
	printf("%03d",20-(timer_activate)/29);  /* BONUS COUNTDOWN */
}

void coord::changPosition(int &direct,int &_x,int &_y,int &indicator,int color)
{  
	int i;
	if(direct==DOWN)  /* SNAKE MOVES IN DOWN DIRECTION */
	{
		if(color!=0)
			hit=getpixel(_x,_y+10); /* TILL HEAD HIT OWN TAIL */
		_y++;
		if(_y>=getmaxy()-10) /* GO DOWN ... COME FROM UP*/
			_y=11;
		if(indicator==1)
		{
			/* MOVE FAST DURING TURNS */
			for(i=0;i<8;i++)
			{
				setcolor(color);
				circle(_x,_y++,radius);
			}
			indicator=0;
		}
	}
	if(direct==LEFT)
	{ 
		if(color!=0)
			hit=getpixel(_x-10,_y);
		_x--;
		if(_x<=10)
			_x=getmaxx()-11;
		if(indicator==1)
		{
			for(i=0;i<8;i++)
			{
				setcolor(color);
				circle(_x--, _y,radius);
			}
			indicator=0;
		}
	}

	if(direct==RIGHT)
	{
		if(color!=0)
			hit=getpixel(_x+10,_y);
		_x++;
		if(_x>=getmaxx()-10)
			_x=11;
		if(indicator==1)
		{
			for(i=0;i<8;i++)
			{
				setcolor(color);
				circle(_x++,_y,radius);
			}
			indicator=0;
		}
	}

	if(direct==UP)
	{
		if(color!=0)
			hit=getpixel(_x,_y-10);
		_y--;
		if(_y<=10)
			_y=getmaxy()-11;
		if(indicator==1)
		{
			for(i=0;i<8;i++)
			{
				setcolor(color);
				circle(_x, _y--,radius);
			}
			indicator=0;
		}
	}
}

int Snake::activeSnake()
{
	randomize();
	/* INTRODUCTION */
	int gdriver = DETECT, gmode, errorcode;
	initgraph(&gdriver, &gmode, "");
	textcolor(1);
	setbkcolor(BLACK);
	cleardevice();
Newgame:
	count=0;
	draw_head=RIGHT;draw_tail=RIGHT;
	score=0;
	for(i=0;i<=200;i++)
	{
		eraze_snake[0][i]=0;
		eraze_snake[1][i]=0;
		dir[i]=0;
	}
	turn_tail=0;
	direction_tail=0;
	direction_head=0;
	cleardevice();
	settextstyle(0, HORIZ_DIR,1 );
	refresh();
	draw_maze();
	draw_head=RIGHT;  /* SET THE INITIAL DIRECTION OF SNAKE */
	setcolor(70);
	boundary();  /* DRAW BOUNDARY */
	refresh();
	do
	{                   /* TILL THE PLAYER HIT 'E' */
		while(!kbhit())    /* TILL THE USER HIT A DIRECTION */
		{
			delay(23-2*(level+1));  /* THE LEVEL MANAGEMENT */
			elongate++;
			timer_activate++;
			gotoxy(75,1);
			printf("%06ld",score);  /* PRINT SCORE */
			if(count>100)
				count=101;
			if(timer_activate==580)     /* WHEN BONUS TIME FINISHES */
				finish_bonus();

			if(timer_activate<580)
				bonus_timer();

			if(bonus_point==1)
			{
				put_bonus(0,bonus_x,bonus_y);
				bonus_point=put_bonus(1,bonus_x,bonus_y);
				timer_activate=1;
				bonus_point=0;
			}

			if(next_food==1)    /* WHEN PLAYER EATS A FOOD PLACE NEXT FOOD */
			{
				put_bonus(0,x_food,y_food);
				next_food=put_bonus(4,x_food,y_food);
			}
			changPosition(draw_head,head_move_x,head_move_y,first,10);
			if(elongate>20)
				changPosition(draw_tail,tail_move_x,tail_move_y,turn_tail,0);

			for(i=x_food-7;i<=x_food+7;i++)
			{
				for(j=y_food-7;j<=y_food+7;j++)
				{
					if( head_move_x==i&&head_move_y==j)         /* WHEN SNAKE HIT FOOD */
					{
						score+=level;   /* SCORE UPDATE*/
						points++;
						if(points%5==0)
							bonus_point=1;
						next_food=1;
						elongate=0;
						gotoxy(75,1);
						printf("%06ld",score);    /* PRINT SCORE */
						setcolor(10);
						setfillstyle(1,10);
						fillellipse(head_move_x, head_move_y,radius+1,radius+1);
						sound(500);
						delay(10);
						nosound();
						elongate=0;
					}
				}
			}
			for(i=bonus_x-7;i<=bonus_x+7;i++) 
			{
				for(j=bonus_y-7;j<=bonus_y+7;j++)
				{
					if( head_move_x==i&&head_move_y==j) /* WHEN SNAKE HIT BONUS */
					{
						score+=(level*10);
						score+=(580-timer_activate)/29;    /* SCORE UPDATE */
						timer_activate=579;
						gotoxy(75,1);
						printf("%06ld",score);  /* PRINT SCORE */
						setcolor(10);
						setfillstyle(1,10);
						fillellipse(head_move_x, head_move_y,radius+1,radius+1);
						sound(500);
						delay(10);
						nosound();
						break;
					}
					if(timer_activate==579)  /* QUIT COUNTDOWN WHEN PLAYER EATS BONUS */
						break;
				}
			}
			count++;
			if(count==100)    /* START ERAZING TAIL AFTER THE 100`TH MOVE OF HEAD*/
			{
				if(maze==4)
				{
					tail_move_x=getmaxx()/2;
					tail_move_y=getmaxy()/2-50;
				}
				else if(maze==5)
					tail_move_y=getmaxy()/2+35;
				else
				{tail_move_x=100;
					tail_move_y=getmaxy()/2;
				}
			}

			setcolor(10);                          /* DRAW HEAD */
			circle(head_move_x, head_move_y,radius);
			setcolor(0);                          /* DRAW TAIL*/
			circle(tail_move_x,tail_move_y,radius);
			circle(tail_move_x,tail_move_y,radius+1);

			if(tail_move_x==eraze_snake[0][direction_tail] && tail_move_y== eraze_snake[1][direction_tail])
			{
				draw_tail=dir[direction_tail];      /* SCAN TURN DATA TO STAY ON PATH */
				direction_tail++;
				turn_tail=1;
			}
			if(direction_head>198)direction_head=0;
			if(direction_tail>198)direction_tail=0;
			if(hit==10||hit==13)
			{
				sound(100);
				delay(500);
				nosound();
				getch();
				break;
			}
		}

		if(hit==10||hit==13)
			break;

		draw_head_temp=draw_head;
		draw_head='\0';
		draw_head=getch();

		if(draw_head==27)
		{
			setcolor(15);
			settextstyle(3, HORIZ_DIR,1 );
			outtextxy(getmaxx()/2-90,getmaxy()-40,"Quit Game (y/n) ?");
quit:
			draw_head=getch();
			if(draw_head=='y'||draw_head=='Y')
				break;
			else
			{
				setcolor(0);
				settextstyle(3, HORIZ_DIR,1 );
				outtextxy(getmaxx()/2-90,getmaxy()-40,"Quit Game (y/n) ?");
			}
		}
		if((draw_head==DOWN&&draw_head_temp==DOWN)||(draw_head==DOWN&&draw_head_temp==UP)||(draw_head==LEFT&&draw_head_temp==LEFT)||(draw_head==LEFT&&draw_head_temp==RIGHT)||(draw_head==RIGHT&&draw_head_temp==RIGHT)||(draw_head==RIGHT&&draw_head_temp==LEFT)||(draw_head==UP&&draw_head_temp==UP)||(draw_head==UP&&draw_head_temp==DOWN)||(draw_head!=DOWN&&draw_head!=LEFT&&draw_head!=RIGHT&&draw_head!=UP))
		{
			draw_head=draw_head_temp;
			continue;
		}
		first=1;
		eraze_snake[0][direction_head]=head_move_x;   /* STORE THE TURNS DATA */
		eraze_snake[1][direction_head]=head_move_y ;
		dir[direction_head]=draw_head;
		direction_head++;
	}while(draw_head!='e');
	cleardevice();
	setcolor(70);
	boundary();  /* DRAW BOUNDARY */
	setcolor(25);
	settextstyle(3, HORIZ_DIR,4 );
	outtextxy(getmaxx()/2-120,getmaxy()/2,"*** Game Over ***");
	getch();
	if(score>high_score)
	{                                  /* store high score data */
		save_hi_score();
	}
	cleardevice();
	setcolor(70);
	boundary();  /* DRAW BOUNDARY*/
	gotoxy(30,12);
	printf("HIGH SCORE : %s  %d",hi_score_name,high_score);
	gotoxy(30,14);
	printf("YOUR SCORE :  %ld ",score);
	settextstyle(3, HORIZ_DIR,2 );
	outtextxy(getmaxx()/2-100,getmaxy()-100,"Start new game (y/n) ?");
	draw_head=getch();
	if(draw_head=='y'||draw_head=='Y')
		goto Newgame;
	hi_score_pointer.close();
	return 0;
}

main()
{
	int g1=DETECT,g2;
	initgraph(&g1,&g2,"BGI\\");
	int l,m;
	Snake sna;
	Menu level;
	Menu maze;
	char items_l[9][20]={"Slowest","Slower" ,"Slow","Normal slow","Normal "
		,"Normal fast","Fast","Faster","Fastest"};
	char help_l[9][75]={"Slowest","Slower" ,"Slow","Normal slow","Normal "
		,"Normal fast","Fast","Faster","Fastest"};
	char items_m[5][20]={"No maze","Box maze","Tunnel maze","Spiral maze","Twsted maz"};
	char help_m[5][75]={"No maze","Box maze","Tunnel maze","Spiral maze","Twsted maz"};
	cleardevice();
	level.writeMenu(9,items_l);
	settextstyle(3, HORIZ_DIR,5);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	outtextxy(100,230,"FlutterBy Snake! ");
	l=level.menuActivate(help_l);
	cleardevice();
	maze.writeMenu(5,items_m);
	setcolor(9);
	settextstyle(3, HORIZ_DIR,1 );
	outtextxy(20,110,"Instructions  ");
	outtextxy(20,140,"Make the snake grow longer");
	outtextxy(20,170,"by directing it to the food");
	outtextxy(20,200,"Use the direction keys direct the snake");
	outtextxy(20,230,"You cannot stop or move the snake backward ");
	outtextxy(20,260,"Avoid hitting the walls or its own tail");
	outtextxy(20,290,"Get the bonus food before the time gets expired");
	outtextxy(20,320,"Press  'E'  to pause the game");
	outtextxy(20,350,"Press  'esc'  button to end at any part of the game ");
	outtextxy(20,380,"Visit FBY game services for more details ");
	settextstyle(3, HORIZ_DIR,1 );
	settextjustify(LEFT_TEXT,TOP_TEXT);
	m=maze.menuActivate(help_m);
	sna.select_levels(l,m);
	sna.activeSnake();
	closegraph();
}
