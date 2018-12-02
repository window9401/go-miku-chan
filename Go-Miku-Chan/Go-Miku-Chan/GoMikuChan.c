/**
	Title			: ike! Miku chan!
	Developing period	: 2014/11/20 ~ 2014/12/3
	Developed By		: window9401@gmail.com
 */

// include libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <MMSystem.h>

// for use BGM(not used)
#pragma comment(lib,"Winmm.lib")

// define 4 keys for move character
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
// default attack
#define SPACE 32
// skill(throw leek cube)
#define SKILL 97

// for game flow
unsigned int			key;			// identify inputted value(keyboard) 
unsigned int			flow;			// "for" construction's variable be made for map's realization
unsigned int			stagelevel = 1;		// stage level
unsigned int			turn = 0;		// for game's sequential flow

// define HP & MP, location(character, attack, skill), etc setting for character
int				mikuHP = 30;
int				mikuMP = 10;
unsigned int			mikuX = 2;
unsigned int			mikuY = 11;
unsigned int			attackX = 0;
unsigned int			attackY = 0;
unsigned int			skillX = 0;
unsigned int			skillY = 0;
// my character's now attack's number... basic limit 1 bullet, so if now bullet remove, attack possible... 1 = attack imP, 0 = attack P
unsigned int			attacklimit = 0;
// my character's now skill's number (same explanation on)
unsigned int			skilllimit=0;

// define HP & location of attacks for boss(boss1 & boss2)
int						bossX = 70;
int						bossY = 10;
int						boss1HP = 30;
int						bossattackX = 10;
int						bossattackY = 10;
int						boss2HP = 40;
int						bossattack2X=10;
int						bossattack2Y=10;
int						bossattackpattern; // boss's attack pattern setting
int						bossattacklimit=0; // boss's attack limit

// define function
void opening();					// game opening title
void help();					// game guide
void gotoxy();					// for move chracter & boss in console
void removecursor();				// blinking cousor remove in left up
void roundstart();				// paint every game stage
void erase(int, int, int, int);			// remove painted area in console

// for character
// update character, attack, skill's position
void updatemiku();
void updateattack();
void updateskill();
// update HP & MP
void updatemikuHP(int);
void updatemikuMP(int);
// character output in consol function pattern (1,6 array)
void printmiku();
// character's skill (throw leek cube)
void attackskill(char,int);

// for boss
// update boss's location, HP, attacks
void updatebossHP(int);
void updateboss();
void updatebossattack();
void updateboss1attack();
void updateboss2attack();
// print boss in console
void printboss1(); // (4,6 array)
void printboss2(); // (1,6 array)
// boss1's attack pattern(1~4)
void attack11(char,int);
void attack12(char,int);
void attack13(char,int);
void attack14(char,int);
// boss2's attack pattern(1~5)
void attack21(char,int);
void attack21_2(char,int);
void attack22(char,int);
// void attack23(char,int); <-- on testing
void attack24(char,int);
void attack25(char,int);

// game stage
char stage[24][81] = { "|-----------------------------------------------------------------------------|\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n",
		       "|-----------------------------------------------------------------------------|\n",
		       "|                                                                             |\n",
		       "|                                                                             |\n" };

// define character, bosses, attack, skills
char miku[1][6]= {"(^0^)"};						// character (miku)
char boss1[4][6]={"(^o^)",						// stage1 boss (rin+len)
		  "     ",
		  "     ",
		  "(-_-)"};
char boss2[1][6]={"(>_<)"};						// stage2 boss (luka)
// define character & boss name
char mikuname[1][20]  = { "初音ミク" };
char boss1name[1][20] = { "鏡音リン*レン" };				// boss1's name 
char boss2name[1][20] = { "巡音ルカ" };				 // boss2's name
// define attacks, skill
char mikuattack[2]="0";							// default attack (bullet)
char mikuskill[3][4]={"***",						// character's skill form (throw leek's extraction cube)
		      "***",
		      "***"};
char bossattack11[9][2]={  "*",						// boss1's attack1 (5 bullets)
		           " ",
		           "*",
		           " ",
		           "*",
		           " ",
		           "*",
		           " ",
		           "*"};
char bossattack12[7][2]={  "*",						// boss1's attack2 (4 bullets)
			   " ",
			   "*",
			   " ",
			   "*",
			   " ",
			   "*"};
char bossattack13[4][9]={  " **  ** ",					// boss1's attack3 (throw heart)
			   "********",
			   " ****** ",
			   "   **   "}; 
char bossattack14[13][27]={  "      ** **    ** **      ",		// "The Panda doll"...  very strong!
			     "      ** **    ** **      ",
			     "      *****    *****      ",
			     "     ****************     ",
			     "     *  **      **  *     ",
			     "     *              *     ",
			     "    ******************    ",
			     "   ***              ***   ",
			     "  ****              ****  ",
			     " ***  **************  *** ",
			     "        ***    ***        ",
			     "        ***    ***        ",
			     "        ***    ***        "};
char bossattack21[20][2]={				// boss2's attack1
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " "};
char bossattack21_2[20][2]={				// boss2's attack1_2 (attack1_1 + attack1_2)
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*",
			    " ",
			    "*"};
char bossattack22[4][15]={"**************",		// throw SUSI
			  "**************",
                          "*            *",
                          "**************"};
char bossattack24[1][4]={"|||"};			// "TAKO's tentacle"... strong...
char bossattack25[11][16]={"       *       ",		// "THE STAR" summon
			   "      ***      ",
			   "     *****     ",
			   "***************",
			   " ************* ",
			   "  ***********  ",
			   " ************* ",
			   "***************",
			   "     *****     ", 
			   "      ***      ",
			   "       *       "};

/** main function */
int main(void)
{
	srand((unsigned)time(NULL)); // for random value
	opening();
	removecursor();

	// this while loop is used for title & game guide
	while(1)
	{
		if (_kbhit())
			key = _getch();

		if (key == 49)		// start game
			break;
		else if (key == 50)	// game help
		{
			system("cls");
			help();
		}
		else if (key == 51)	// exit game
		{
			exit(0);
		}
		else
			continue;
	}

	// this while loop is used for game
	while (1)
	{
		if (stagelevel == 1) // background color : YELLOW, BOSS : KAGAMINE RIN & LEN
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			roundstart();
			updatemikuHP(mikuHP);
			updatemikuMP(mikuMP);
			updatebossHP(boss1HP);
			gotoxy(3, 24);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("%s", boss1name);
		}

		if (stagelevel == 2) // background color : PULPLE, BOSS : MEGURINE LUKA
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			roundstart();
			updatemikuHP(mikuHP);
			updatemikuMP(mikuMP);
			updatebossHP(boss2HP);
			gotoxy(3, 24);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			printf("%s", boss2name);
		}
		if (stagelevel == 3) break;	// game's end
		gotoxy(mikuX,mikuY);		// character's summon at dafault location
		printmiku(miku,sizeof(miku)/sizeof(miku[0]));	// print character

		while (stagelevel<=3)
		{
			// if character or boss's HP is 0, end game
			if ( (mikuHP<=0) || (stagelevel==1 && boss1HP<=0) || (stagelevel==2 && boss2HP<=0) )
				break;
			
			if (_kbhit()) // move character
			{
				key = _getch();
				updatemiku(key);
			}
			else // update attacks, skills
			{
				updateattack();
				updateskill();
				updatebossattack();
				turn++; // 'turn' value is for update boss's position				
				if (turn == 30) // if 'turn' value is to 30, update boss's location randomly 
					updateboss();
				if (turn == 30) // if 'turn' value is to 30, attack randomly
				{
					if(bossattacklimit!=2) // '2' is boss's attack is already
						bossattacklimit=1; // if this value is not '2', turn to 0 to 1, and attack mode change
					turn=0; // turn reset
				}
				Sleep(20); // for multi threading concept... sleep!
			}
		} // this round's end

		if(mikuHP==0) // if character's HP to 0, game end...
			break; 

		stagelevel++; // if clear target stage, this vaiable's value up for next stage.
	} // end of main while
} // end of main

// game opening
void opening()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf(" ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("                               行け!ミクちゃん!                        ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf("   ■\n");
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                                                                          ■\n");
	// start line
	printf(" ■                                ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" 1. スタㅡト");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf("                              ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                                                                          ■\n");
	// help line
	printf(" ■                                ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" 2. ヘルプ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf("                                ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                                                                          ■\n");
	// end line
	printf(" ■                                ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf(" 3. 終了");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf("                                  ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                                                                          ■\n");
	// plus...
	
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■                         ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	printf("MADE BY window9401@gmail.com");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	printf("                     ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■           (^0^)          (-_-)          (^o^)          (>_<)             ■\n");
	printf(" ■                                                                          ■\n");
	printf(" ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}

// game guide
void help()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("\n");
	printf("                        行け！ミクちゃんについて （^o^）/\n");
	printf("\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("行け！ミクちゃんはシュ一ティングゲ一ムの一種で、\n");
	printf("ボかロイドの初音ミクが活躍するゲ一ムです。\n\n");
	printf("一般敵（部下）はありませんので、さっそくボ一スとたたかうことになります。\n\n");
	printf("ステ一ジは２段階まであり、段階ごとに準備（待機）時間はありません！\n\n");
	printf("詰まらない作品ですが、少しでもおたのしみいただければ大きな力になります！\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	printf("SKILL : A （ネギを三つ投げます）\n\n");
	printf("ATTACK : SPACE （ミドリ色のキュ一ブを投げます）\n\n");
	printf("MOVE : ← ↑ → ↓\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	printf("\n\n\n                    ← を押すとスタ一トメイン一へ行きます。\n");

	while (1)
	{
		if (_kbhit())
			key = _getch();

		if (key == LEFT)
		{
			system("cls");
			key=0;
			opening();
			break;
		}
	}
}

// for remove cursor's blinking
void removecursor() 
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// for print object
void gotoxy(int x, int y) 
{
	COORD pos={x-1,y-1}; // cursor's X value -1 from now value. cursor's Y value -1 from now value.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); // WIN32API's Function
}

// update character's location
void updatemiku(int key)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

	switch (key)
	{
	case UP:
		if (mikuY != 2)
		{
			erase(sizeof(miku)/sizeof(miku[0]),strlen(miku[0]), mikuX, mikuY);
			mikuY -= 1;
			printmiku(miku,sizeof(miku)/sizeof(miku[0]));
		}
		break;

	case LEFT:
		if (mikuX != 2)
		{
			erase(sizeof(miku)/sizeof(miku[0]),strlen(miku[0]), mikuX, mikuY);
			mikuX -= 1;
			printmiku(miku,sizeof(miku)/sizeof(miku[0]));
		}
		break;

	case DOWN:
		if (mikuY != 21)
		{
			erase(sizeof(miku)/sizeof(miku[0]),strlen(miku[0]), mikuX, mikuY);
			mikuY += 1;
			printmiku(miku,sizeof(miku)/sizeof(miku[0]));
		}
		break;

	case RIGHT:
		if (mikuX != 50) // maximum value is 73... character and boss distance (73 is near by)
		{
			erase(sizeof(miku)/sizeof(miku[0]),strlen(miku[0]), mikuX, mikuY);
			mikuX += 1;
			printmiku(miku,sizeof(miku)/sizeof(miku[0]));
		}
		break;

	case SPACE:
		if (attacklimit == 0 && mikuX != 73)
		{
			attacklimit++;
			gotoxy(mikuX+5, mikuY);
			updateattack();
		}
		break;

	case SKILL:
		if(skilllimit==0 && mikuMP>0)
		{
			mikuMP--;
			updatemikuMP(mikuMP);
			skilllimit=1;
			gotoxy(mikuX+5, mikuY-1);
			updateskill();
		}
		break;

	default:
		break;
	}
}

// update character's attack location
void updateattack()
{
	if (attackX != 78)
	{
		if (attacklimit == 1)
		{
			attackX=mikuX+5;
			attackY=mikuY;
			attacklimit+=1;
		}

		if (attacklimit == 2)
		{
			if (attackX== bossX && attackY == bossY)
			{
				if(stagelevel == 1)
				{
					boss1HP--;
					updatebossHP(boss1HP);
				}
				else if(stagelevel == 2)
				{
					boss2HP--;
					updatebossHP(boss2HP);
				}
				attacklimit = 0;
				attackX=0;
			}

			else
			{
				gotoxy(attackX, attackY);
				printf(" ");

				attackX += 1;

				gotoxy(attackX, attackY);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf("%s",mikuattack);
			}
		}

	}	
	else
	{
		gotoxy(attackX, attackY);
		printf(" ");
		attacklimit = 0;
		attackX = 0;
	}
}

// update character's skill location
void updateskill()
{
		if (skillX != 75)
		{
			if (skilllimit == 1)
			{
				skillX=mikuX+5;
				skillY=mikuY-1;
				skilllimit+=1;
			}

			if (skilllimit == 2)
			{
				if ( skillX+2== bossX && (skillY == bossY || skillY+1 == bossY || skillY+2 == bossY) )
				{
					if(stagelevel == 1)
					{
						boss1HP-=4;
						updatebossHP(boss1HP);
					}
					else if(stagelevel == 2)
					{
						boss2HP-=4;
						updatebossHP(boss2HP);
					}
				erase(sizeof(mikuskill)/sizeof(mikuskill[0]),strlen(mikuskill[0]), skillX, skillY);
				skilllimit = 0;
				skillX=0;
				}

			else
			{
				gotoxy(skillX, skillY);
				erase(sizeof(mikuskill)/sizeof(mikuskill[0]),strlen(mikuskill[0]), skillX, skillY);

				skillX += 1;

				gotoxy(skillX, skillY);
				attackskill(mikuskill,sizeof(mikuskill)/sizeof(mikuskill[0]));
			}
		}
		}
	else
	{
		erase(sizeof(mikuskill)/sizeof(mikuskill[0]),strlen(mikuskill[0]), skillX, skillY);
		skilllimit = 0;
		skillX = 0;
	}

}

// update boss's location
void updateboss()
{
	int a = (rand() % 4) + 1;

	if(stagelevel==1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	else if(stagelevel==2)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	else if(stagelevel==3)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	else if(stagelevel==4)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);

	switch (a)
	{
	case 1: // UP
		if (bossY != 7)
		{
			if(stagelevel==1)
			{
				erase(sizeof(boss1)/sizeof(boss1[0]),strlen(boss1[0]), bossX, bossY);
				bossY -= 1;
				printboss1(boss1,sizeof(boss1)/sizeof(boss1[0]));
			}
			else if(stagelevel==2)
			{
				erase(sizeof(boss2)/sizeof(boss2[0]),strlen(boss2[0]), bossX, bossY);
				bossY -= 1;
				printboss2(boss2,sizeof(boss2)/sizeof(boss2[0]));
			}
		}
		break;

	case 2: // LEFT
		if (bossX != 2)
		{
			if(stagelevel==1)
			{
				erase(sizeof(boss1)/sizeof(boss1[0]),strlen(boss1[0]), bossX, bossY);
				bossX -= 1;
				printboss1(boss1,sizeof(boss1)/sizeof(boss1[0]));
			}
			else if(stagelevel==2)
			{
				erase(sizeof(boss2)/sizeof(boss2[0]),strlen(boss2[0]), bossX, bossY);
				bossX -= 1;
				printboss2(boss2,sizeof(boss2)/sizeof(boss2[0]));
			}
		}
		break;

	case 3: // DOWN
		if (bossY != 15)
		{
			if(stagelevel==1)
			{
				erase(sizeof(boss1)/sizeof(boss1[0]),strlen(boss1[0]), bossX, bossY);
				bossY += 1;
				printboss1(boss1,sizeof(boss1)/sizeof(boss1[0]));
			}
			else if(stagelevel==2)
			{
				erase(sizeof(boss2)/sizeof(boss2[0]),strlen(boss2[0]), bossX, bossY);
				bossY += 1;
				printboss2(boss2,sizeof(boss2)/sizeof(boss2[0]));
			}
		}
		break;

	case 4: // RIGHT
		if (bossX != 73)
		{
			if(stagelevel==1)
			{
				erase(sizeof(boss1)/sizeof(boss1[0]),strlen(boss1[0]), bossX, bossY);
				bossX += 1;
				printboss1(boss1,sizeof(boss1)/sizeof(boss1[0]));
			}
			else if(stagelevel==2)
			{
				erase(sizeof(boss2)/sizeof(boss2[0]),strlen(boss2[0]), bossX, bossY);
				bossX += 1;
				printboss2(boss2,sizeof(boss2)/sizeof(boss2[0]));
			}
		}
		break;

	default:
		break;
	}
}

// update boss's attack
void updatebossattack()
{
	if(bossattacklimit==1)
	bossattackpattern=(rand()%5)+1;

	if(stagelevel==1)
		updateboss1attack();
	else if(stagelevel==2)
		updateboss2attack();
}

// boss1's attack pattern definition
void updateboss1attack() 
{
	switch(bossattackpattern)
	{
	case 1: // 5 bullet throwing

	if(bossattackX>2)
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-8;
			bossattackY=bossY-2;
			bossattacklimit++;
		}
		if(bossattacklimit==2)
		{
			if( (bossattackX-1==mikuX && bossattackY==mikuY) || (bossattackX-1==mikuX && bossattackY+2==mikuY) || (bossattackX-1==mikuX && bossattackY+4==mikuY) || (bossattackX-1==mikuX && bossattackY+6==mikuY) || (bossattackX-1==mikuX && bossattackY+8==mikuY) )
			{
				erase(sizeof(bossattack11)/sizeof(bossattack11[0]),strlen(bossattack11[0]), bossattackX, bossattackY);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=1;
				updatemikuHP(mikuHP);

				printmiku(miku,sizeof(miku)/sizeof(miku[0]));
			}

			else
			{
				erase(sizeof(bossattack11)/sizeof(bossattack11[0]),strlen(bossattack11[0]), bossattackX, bossattackY);

				bossattackX--;

				gotoxy(bossattackX,bossattackY);
				attack11(bossattack11,sizeof(bossattack11)/sizeof(bossattack11[0]));
			}
		}
	}
	else
	{
		erase(sizeof(bossattack11)/sizeof(bossattack11[0]),strlen(bossattack11[0]), bossattackX, bossattackY);
		bossattacklimit=0;
		bossattackX=bossX;
	}
	break;


	case 2: // 4 bullet throwing

	if(bossattackX>2 && bossattackY<16) // this if construction for prevention map's break. if not this if construction, case2's bullet will paint over up to game's map
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-8;
			bossattackY=bossY;
			bossattacklimit++;
		}
		if(bossattacklimit==2)
		{
			if( (bossattackX-1==mikuX && bossattackY==mikuY) || (bossattackX-1==mikuX && bossattackY+2==mikuY) || (bossattackX-1==mikuX && bossattackY+4==mikuY) || (bossattackX-1==mikuX && bossattackY+6==mikuY) )
			{
				erase(sizeof(bossattack12)/sizeof(bossattack12[0]),strlen(bossattack12[0]), bossattackX, bossattackY);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=2;
				updatemikuHP(mikuHP);

				printmiku(miku,sizeof(miku)/sizeof(miku[0]));
			}

			else
			{
				erase(sizeof(bossattack12)/sizeof(bossattack12[0]),strlen(bossattack12[0]), bossattackX, bossattackY);

				bossattackX--;

				gotoxy(bossattackX,bossattackY);
				attack12(bossattack12,sizeof(bossattack12)/sizeof(bossattack12[0]));
			}
		}
	}
	else
	{
		erase(sizeof(bossattack12)/sizeof(bossattack12[0]),strlen(bossattack12[0]), bossattackX, bossattackY);
		bossattacklimit=0;
		bossattackX=bossX;
	}

	break;

	case 3: // heart throwing
		
	if(bossattackX >2)
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-8;
			bossattackY=bossY;
			bossattacklimit++;
		}

		if(bossattacklimit==2)
		{
			if( (bossattackX-1==mikuX && bossattackY==mikuY) || (bossattackX-1==mikuX && bossattackY+1==mikuY) || (bossattackX-1==mikuX && bossattackY+2==mikuY) || (bossattackX-1==mikuX && bossattackY+3==mikuY) )
			{
				erase(sizeof(bossattack13)/sizeof(bossattack13[0]),strlen(bossattack13[0]), bossattackX, bossattackY);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=3;
				updatemikuHP(mikuHP);

				printmiku(miku,sizeof(miku)/sizeof(miku[0]));
			}

			else
			{
				erase(sizeof(bossattack13)/sizeof(bossattack13[0]),strlen(bossattack13[0]), bossattackX, bossattackY);

				bossattackX--;

				gotoxy(bossattackX,bossattackY);
				attack13(bossattack13,sizeof(bossattack13)/sizeof(bossattack13[0]));
			}
		}
	}
	else
	{
		erase(sizeof(bossattack13)/sizeof(bossattack13[0]),strlen(bossattack13[0]), bossattackX, bossattackY);
		bossattacklimit=0;
		bossattackX=31;
	}
	break;

	case 4: // "THE PANDA" doll summon, and attack

	if(boss1HP<=15)
	{
		if(bossattackX >2)
		{
			if(bossattacklimit==1)
			{
				if(mikuY<10)
				{
					bossattackX=bossX-27;
					bossattackY=2;
					bossattacklimit++;
				}
				else
				{
					bossattackX=bossX-27;
					bossattackY=9;
					bossattacklimit++;
				}
			}
	
			if(bossattacklimit==2)
			{
				if( ((bossattackX-1<=mikuX && bossattackX+20>=mikuX)&&(bossattackY==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+20>=mikuX)&&(bossattackY+1==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+20>=mikuX)&&(bossattackY+3==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+21>=mikuX)&&(bossattackY+4==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+21>=mikuX)&&(bossattackY+5==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+21>=mikuX)&&(bossattackY+6==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+22>=mikuX)&&(bossattackY+7==mikuY))|| ((bossattackX-1<=mikuX && bossattackX+23>=mikuX)&&(bossattackY+8==mikuY))|| ((bossattackX-1<=mikuX && bossattackX+24>=mikuX)&&(bossattackY+9==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+25>=mikuX)&&(bossattackY+10==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+18>=mikuX)&&(bossattackY+11==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+18>=mikuX)&&(bossattackY+12==mikuY)) || ((bossattackX-1<=mikuX && bossattackX+18>=mikuX)&&(bossattackY+13==mikuY)) )
				{
					erase(sizeof(bossattack14)/sizeof(bossattack14[0]),strlen(bossattack14[0]), bossattackX, bossattackY);
						
					bossattacklimit=0;
					bossattackX=bossX;
	
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					mikuHP-=3;
					updatemikuHP(mikuHP);
	
					printmiku(miku,sizeof(miku)/sizeof(miku[0]));
				}

				else
				{
					erase(sizeof(bossattack14)/sizeof(bossattack14[0]),strlen(bossattack14[0]), bossattackX, bossattackY);

					bossattackX--;

					gotoxy(bossattackX,bossattackY);
					attack14(bossattack14,sizeof(bossattack14)/sizeof(bossattack14[0]));
				}
			}
		}
		else
		{
			erase(sizeof(bossattack14)/sizeof(bossattack14[0]),strlen(bossattack14[0]), bossattackX, bossattackY);
			bossattacklimit=0;
			bossattackX=31;
		}
	}
	break;

	case 5:bossattacklimit=0; break;

	default:
		break;
	}
}

// boss2's attack definition
void updateboss2attack() 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);

	switch(bossattackpattern)
	{
	case 1: // bullet throwing.. char array (attack2_1 + attack2_2) use

	if(bossattackX>2)
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-15;
			bossattackY=2;
			bossattack2X=bossattackX+10; // for consecutively
			bossattack2Y=bossattackY;
			bossattacklimit++;
		}
		if(bossattacklimit==2)
		{
			if( (bossattackX-1==mikuX && bossattackY==mikuY) || (bossattackX-1==mikuX && bossattackY+2==mikuY) || (bossattackX-1==mikuX && bossattackY+4==mikuY) || (bossattackX-1==mikuX && bossattackY+6==mikuY) || (bossattackX-1==mikuX && bossattackY+8==mikuY) || (bossattackX-1==mikuX && bossattackY+10==mikuY) || (bossattackX-1==mikuX && bossattackY+12==mikuY) || (bossattackX-1==mikuX && bossattackY+14==mikuY) || (bossattackX-1==mikuX && bossattackY+16==mikuY) || (bossattackX-1==mikuX && bossattackY+18==mikuY) )
			{
				erase(sizeof(bossattack21)/sizeof(bossattack21[0]),strlen(bossattack21[0]), bossattackX, bossattackY);
				erase(sizeof(bossattack21_2)/sizeof(bossattack21_2[0]),strlen(bossattack21_2[0]), bossattack2X, bossattack2Y);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=1;
				updatemikuHP(mikuHP);
			}
			else if ( ( bossattack2X-1==mikuX ) && ( ( bossattack2Y+1 == mikuY ) || ( bossattack2Y+3 == mikuY ) || ( bossattack2Y+5 == mikuY ) || ( bossattack2Y+7 == mikuY ) || ( bossattack2Y+9 == mikuY ) || ( bossattack2Y+11 == mikuY ) || ( bossattack2Y+13 == mikuY ) || ( bossattack2Y+15 == mikuY ) || ( bossattack2Y+17 == mikuY ) || ( bossattack2Y+19 == mikuY ) ) )
			{
				erase(sizeof(bossattack21)/sizeof(bossattack21[0]),strlen(bossattack21[0]), bossattackX, bossattackY);
				erase(sizeof(bossattack21_2)/sizeof(bossattack21_2[0]),strlen(bossattack21_2[0]), bossattack2X, bossattack2Y);
				
				bossattacklimit=0;
				bossattack2X=bossX;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=1;
				updatemikuHP(mikuHP);
			}

			else
			{
				erase(sizeof(bossattack21)/sizeof(bossattack21[0]),strlen(bossattack21[0]), bossattackX, bossattackY);
				erase(sizeof(bossattack21_2)/sizeof(bossattack21_2[0]),strlen(bossattack21_2[0]), bossattack2X, bossattack2Y);



				bossattackX--;
				bossattack2X--;

				gotoxy(bossattackX,bossattackY);
				attack21(bossattack21,sizeof(bossattack21)/sizeof(bossattack21[0]));
				gotoxy(bossattack2X,bossattack2Y);
				attack21_2(bossattack21_2,sizeof(bossattack21_2)/sizeof(bossattack21_2[0]));

			}
		}
	}
	else
	{
		erase(sizeof(bossattack21)/sizeof(bossattack21[0]),strlen(bossattack21[0]), bossattackX, bossattackY);
		erase(sizeof(bossattack21_2)/sizeof(bossattack21_2[0]),strlen(bossattack21_2[0]), bossattack2X, bossattack2Y);
		bossattacklimit=0;
		bossattackX=bossX;
	}
		break;

	case 2: // throw "SUSI"
		
	if(bossattackX>2)
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-18;
			bossattackY=bossY-1;
			bossattacklimit++;
		}
		if(bossattacklimit==2)
		{
			if( ( bossattackX-1<=mikuX && bossattackX+14>=mikuX ) && ( bossattackY+3>=mikuY ) )
			{
				erase(sizeof(bossattack22)/sizeof(bossattack22[0]),strlen(bossattack22[0]), bossattackX, bossattackY);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=2;
				updatemikuHP(mikuHP);

				printmiku(miku,sizeof(miku)/sizeof(miku[0]));
			}

			else
			{
				erase(sizeof(bossattack22)/sizeof(bossattack22[0]),strlen(bossattack22[0]), bossattackX, bossattackY);

				bossattackX--;

				gotoxy(bossattackX,bossattackY);
				attack22(bossattack22,sizeof(bossattack22)/sizeof(bossattack22[0]));
			}
		}
	}
	else
	{
		erase(sizeof(bossattack22)/sizeof(bossattack22[0]),strlen(bossattack22[0]), bossattackX, bossattackY);
		bossattacklimit=0;
		bossattackX=bossX;
	}
	break;

	case 3: break; // to be continue

	case 4: // "TAKO"s tentacle will character attack
		if(boss2HP<=20)
		{
			if(bossattackY<=20)
			{
			if(bossattacklimit==1)
			{
				bossattackX=mikuX+1;
				bossattackY=2;
				bossattacklimit++;
			}
			if(bossattacklimit==2)
			{
				if( ( bossattackX+10 >= mikuX ) && ( bossY+1==mikuY ) )
				{
					for(flow=2; flow<=21; flow++)
					erase(sizeof(bossattack24)/sizeof(bossattack24[0]),strlen(bossattack24[0]), bossattackX, flow);

					bossattacklimit=0;
					bossattackX=bossX;

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					mikuHP-=1;
					updatemikuHP(mikuHP);

					printmiku(miku,sizeof(miku)/sizeof(miku[0]));
				}
			else
			{

				gotoxy(bossattackX,bossattackY);
				attack24(bossattack24,sizeof(bossattack24)/sizeof(bossattack24[0]));

				bossattackY+=1;
			}
			}
			}

		else
		{
			for(flow=2; flow<=21; flow++)
			erase(sizeof(bossattack24)/sizeof(bossattack24[0]),strlen(bossattack24[0]), bossattackX, flow);
		bossattacklimit=0;
		bossattackX=bossX;
		bossattackY=bossY;
		}
		}

	break;
	

	case 5: // "THE STAR" summon
		
	if(bossattackX>2)
	{
		if(bossattacklimit==1)
		{
			bossattackX=bossX-18;
			if(mikuY<=7) // height control for map's safety...
			bossattackY=2; 
			else if(mikuY>7 && mikuY<=14)
				bossattackY=8;
			else
				bossattackY=11;
			bossattacklimit++;
		}
		if(bossattacklimit==2)
		{
			if( (bossattackX-1==mikuX) && (bossattackY+10>=mikuY) )
			{
				erase(sizeof(bossattack25)/sizeof(bossattack25[0]),strlen(bossattack25[0]), bossattackX, bossattackY);

				bossattacklimit=0;
				bossattackX=bossX;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				mikuHP-=3;
				updatemikuHP(mikuHP);

				printmiku(miku,sizeof(miku)/sizeof(miku[0]));
			}

			else
			{
				erase(sizeof(bossattack25)/sizeof(bossattack25[0]),strlen(bossattack25[0]), bossattackX, bossattackY);

				bossattackX--;

				gotoxy(bossattackX,bossattackY);
				attack25(bossattack25,sizeof(bossattack25)/sizeof(bossattack25[0]));
			}
		}
	}
	else
	{
		erase(sizeof(bossattack25)/sizeof(bossattack25[0]),strlen(bossattack25[0]), bossattackX, bossattackY);
		bossattacklimit=0;
		bossattackX=bossX;
	}
	break;
	}
}

// When target round is started, re-paint map for refresh
void roundstart() 
{
	system("cls");
	bossattacklimit=0; // new boss's attack limiting unbrick
	for (flow = 0; flow<24; flow++)
		printf("%s", stage[flow]);

	gotoxy(3, 23);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("%s", mikuname);
}

// update character's HP
void updatemikuHP(int HP)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

	gotoxy(16,23);
	printf(" | HP:");

	for(flow=0; flow<=30; flow++)
	{	
		if(flow<=30 && flow>=HP)
		{
			gotoxy(22+flow,23);
			printf(" ");
		}
		else
		{
			gotoxy(22+flow,23);
			printf("@");
		}
	}
}

// update character's MP
void updatemikuMP(int MP)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

	gotoxy(52,23);
	printf(" | MP:");

	for(flow=0; flow<=10; flow++)
	{	
		if(flow<=30 && flow>=MP)
		{
			gotoxy(59+flow,23);
			printf(" ");
		}
		else
		{
			gotoxy(59+flow,23);
			printf("@");
		}
	}
}

// update boss's HP
void updatebossHP(int HP)
{
	int MAX_HP; // for boss's HP setting

	if(stagelevel==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		MAX_HP=30;
	}
	else if(stagelevel==2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		MAX_HP=40;
	}
	else if(stagelevel==3)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		MAX_HP=50;
	}

	gotoxy(16,24);
	printf(" | HP:");
	for(flow=0; flow<MAX_HP; flow++)
	{
		if(flow<=MAX_HP && flow>=HP)
		{
			gotoxy(22+flow,24);
			printf(" ");
		}
		else
		{
			gotoxy(22+flow,24);
			printf("@");
		}
	}
}

// for sepecially part removing in consol
void erase(int a, int b, int coordinateX, int coordinateY) // screen's X, screen's Y, for removing X,Y
{
	int first, second;

	for(first=0; first<a; first++)
	{
		for(second=0; second<b; second++)
		{
			gotoxy(coordinateX+second,coordinateY+first);
			printf(" ");
		}
	}
}

// paint character's skill
void attackskill(char attackarray[][4],int size)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);

	for(flow=0; flow<size; flow++)
	{
		gotoxy(skillX,skillY+flow);
		printf("%s",attackarray[flow]);
	}
}

// paint stage boss's skill
// why many function defined is because array's 1 demension argument is must detailed value)
void attack11(char attackarray[][2],int size)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack12(char attackarray[][2],int size)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack13(char attackarray[][9],int size)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack14(char attackarray[][27],int size)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack21(char attackarray[][2],int size)
{
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack21_2(char attackarray[][2],int size)
{
		for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattack2X,bossattack2Y+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack22(char attackarray[][15],int size)
{
		for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack24(char attackarray[][4],int size)
{
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}
void attack25(char attackarray[][16],int size)
{
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossattackX,bossattackY+flow);
		printf("%s",attackarray[flow]);
	}
}

// print character
void printmiku(char charray[][6], int size)
{

	for(flow=0; flow<size; flow++)
	{
		gotoxy(mikuX,mikuY+flow);
		printf("%s",charray[flow]);
	}
}

// print boss1
void printboss1(char charray[][6], int size)
{
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossX,bossY+flow);
		printf("%s",charray[flow]);
	}
}

// print boss2
void printboss2(char charray[][6], int size){
	for(flow=0; flow<size; flow++)
	{
		gotoxy(bossX,bossY+flow);
		printf("%s",charray[flow]);
	}
}
