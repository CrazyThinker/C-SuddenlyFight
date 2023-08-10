//Made by 배근준
//Copyrights ⓒ 2009.12.24
#include<turboc.h>
#include<io.h>
#include "Resource.h"
#include "Class.h"
int Map[MAX_X+1][MAX_Y+1],nMapStartLocation,nCharacter,nItem,nUser,AttackTimerStep[MAX_PLAYER+1][MAX_ITEM+1],AttackTimerGo[MAX_PLAYER+1][MAX_ITEM+1],nMove,nPlayer,nReplay=1,TempHpUp[MAX_PLAYER+1],MapStartLocationIndex[MAX_PLAYER+1];
char Character[MAX_CHARACTER+1][3],MapName[1000];
bool AttackTimerCheck[MAX_PLAYER+1][MAX_ITEM+1];
long StartTime,ExitTime;
CPoint MapSize,MapStartLocation[MAX_STARTLOCATION+1],AttackTimerPoint[MAX_PLAYER+1][MAX_ITEM+1],Move[MAX_MOVE][2];
CPlayer User[MAX_PLAYER+1];
CItem Item[MAX_ITEM+1];
CUserKey UserKey[MAX_PLAYER+1];
CTimer AttackTimer[MAX_PLAYER+1][MAX_ITEM+1];
Direction AttackTimerVision[MAX_PLAYER+1][MAX_ITEM+1];
CReplay Replay[MAX_REPLAY+1];
#define CompareKeyboard(Keyboard1,Keyboard2) Keyboard1.Key==Keyboard2.Key && Keyboard1.Type==Keyboard2.Type
#define CharacterGotoxy(CHARACTER_POINT) gotoxy(CHARACTER_POINT.y*2+2,CHARACTER_POINT.x+1)
class CCode
{
private:
	int LastIndex;
public:
	char IF[MAX_CODE+1][20],THEN[MAX_CODE+1][20];
	int GetValue(int i,int Index)
	{
		int Index1,Index2;
		for(;;i++)
		{
			if(IF[Index][i]==0) break;
			else if(strncmp(IF[Index]+i,"USER",4)==0)
			{
				i+=4;
				if(IF[Index][i]!='[') return -1;
				i++;
				Index1=GetValue(i,Index);
				i=LastIndex;
				if(IF[Index][i]!=']') return -1;
				i++;
				if(IF[Index][i]!='.') return -1;
				if(strncmp(IF[Index]+i,"POINT",5)==0)
				{
					i+=5;
					if(IF[Index][i]!='.') return -1;
					if(IF[Index][i]=='X') return User[Index1].Point.x;
					else if(IF[Index][i]=='Y') return User[Index2].Point.y;
					else return -1;
				}
				else if(strncmp(IF[Index]+i,"HP",2)==0)
				{
					i+=2,LastIndex=i+3;
					if(strncmp(IF[Index]+i,"POS",3)==0) return User[Index1].Hp.Pos;
					else return -1;
				}
				else if(strncmp(IF[Index]+i,"ITEM",4)==0)
				{
					i+=4;
					if(IF[Index][i]!='.') return -1;
					i++;
					if(IF[Index][i]!='[') return -1;
					Index2=GetValue(i+1,Index);
					i=LastIndex;
				}
				else if(strncmp(IF[Index]+i,"VISION",6)==0)
				{
					LastIndex=i+6;
					return User[Index1].Vision;
				}
				else if(strncmp(IF[Index]+i,"ITEMFOCUS",9)==0)
				{
					LastIndex=i+9;
					return User[Index1].Focus;
				}
			}
		}
	}
};
void SetTextColor(char color[3])
{
	WORD attr,previous_background_color;
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	previous_background_color=csbi.wAttributes >> 0x4;
	previous_background_color=previous_background_color << 0x4;
	/* Foreground Color */
	switch(color[0])
	{
	case '0': // Black
		attr=0;
		break;
	case '1': // Blue
		attr=FOREGROUND_BLUE;
		break;
	case '2': // Green
		attr=FOREGROUND_GREEN;
		break;
	case '3': // Aqua (Cyan)
		attr=FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case '4': // Red
		attr=FOREGROUND_RED;
		break;
	case '5': // Purple (Magenta)
		attr=FOREGROUND_RED | FOREGROUND_BLUE;
		break;
	case '6': // Yellow
		attr=FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case '7': // White (Gray: System Default)
		attr=FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case '8': // Gray (Light Black)
		attr=FOREGROUND_INTENSITY;
		break;
	case '9': // Light Blue
		attr=FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case 'A': // Light Green
		attr=FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case 'B': // Light Aqua (Cyan)
		attr=FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case 'C': // Light Red
		attr=FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case 'D': // Light Purple (Magenta)
		attr=FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case 'E': // Light Yellow
		attr=FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case 'F': // Bright White
		attr=FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	}
	/* Background Color */
	switch(color[1])
	{
	case '\0': // 배경색이 지정돼 있지 않으면 기존의 배경색을 사용.
		// 전경색만 지정하면 배경색이 항상 검정으로 되어 버림.
		attr=attr | previous_background_color;
		break;
	case '0': // Black
		attr=attr | 0;
		break;
	case '1': // Blue
		attr=attr | BACKGROUND_BLUE;
		break;
	case '2': // Green
		attr=attr | BACKGROUND_GREEN;
		break;
	case '3': // Aqua (Cyan)
		attr=attr | BACKGROUND_GREEN | BACKGROUND_BLUE;
		break;
	case '4': // Red
		attr=attr | BACKGROUND_RED;
		break;
	case '5': // Purple (Magenta)
		attr=attr | BACKGROUND_RED | BACKGROUND_BLUE;
		break;
	case '6': // Yellow
		attr=attr | BACKGROUND_RED | BACKGROUND_GREEN;
		break;
	case '7': // White (Gray: System Default)
		attr=attr | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
		break;
	case '8': // Gray (Light Black)
		attr=attr | BACKGROUND_INTENSITY;
		break;
	case '9': // Light Blue
		attr=attr | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	case 'A': // Light Green
		attr=attr | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case 'B': // Light Aqua (Cyan)
		attr=attr | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	case 'C': // Light Red
		attr=attr | BACKGROUND_RED | BACKGROUND_INTENSITY;
		break;
	case 'D': // Light Purple (Magenta)
		attr=attr | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	case 'E': // Light Yellow
		attr=attr | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case 'F': // Bright White
		attr=attr | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),attr);
}
char *GetMapCharacter(const CPoint Point)
{
	if(Point.x<0 || Point.x>=MapSize.x || Point.y<0 || Point.y>=MapSize.y) return CHARACTER_MAP_BACKGROUND;
	int i,j;
	for(i=1;i<=nMove;i++) for(j=0;j<2;j++) if(Move[i][j].x==Point.x && Move[i][j].y==Point.y) return CHARACTER_MOVE;
	switch(Map[Point.x][Point.y])
	{
	case 0 : return CHARACTER_MAP0;
	case 1 : return CHARACTER_MAP1;
	case 2 : return CHARACTER_MAP0;
	case BOX : return CHARACTER_BOX;
	}
	return NULL;
}
int CheckMapPlayer(const CPoint Point)
{
	int i;
	for(i=1;i<=nUser;i++) if(User[i].Point.x==Point.x && User[i].Point.y==Point.y && User[i].Die==false) return i;
	return -1;
}
void ShowUserHp(const int UserIndex)
{
	int i,Temp=(int)((float)User[UserIndex].Hp.Pos/User[UserIndex].Hp.Max*10);
	gotoxy(26,MapSize.x+UserIndex+1);
	printf("%3d",User[UserIndex].Hp.Pos);
	if(Temp!=TempHpUp[UserIndex])
	{
		TempHpUp[UserIndex]=Temp;
		gotoxy(4,MapSize.x+UserIndex+1);
		SetTextColor("9");
		for(i=1;i<=Temp;i++) printf("■");
		for(;i<=10;i++) printf("□");
		SetTextColor("0");
	}
}
void ShowUserShot(const int UserIndex)
{
	gotoxy(37+strlen(Item[User[UserIndex].Focus].ItemCharacter),MapSize.x+UserIndex+1);
	if(Item[User[UserIndex].Focus].Shot==INF) printf("∞");
	else printf("%d",User[UserIndex].Item[User[UserIndex].Focus].Shot);
	printf("          ");
}
void SetCharacterLocation(void)
{
	int i;
	bool o[MAX_PLAYER+1]={0};
	for(i=1;i<=nUser;i++)
	{
		do MapStartLocationIndex[i]=random(nMapStartLocation)+1;
		while(o[MapStartLocationIndex[i]]==true);
		o[MapStartLocationIndex[i]]=true;
	}
	for(i=1;i<=nUser;i++) User[i].Point=MapStartLocation[MapStartLocationIndex[i]];
}
bool LoadCharacter(void)
{
	int i;
	FILE *fi=fopen("Character.dat","r");
	if(fi==NULL) return false;
	fscanf(fi,"%d",&nCharacter);
	for(i=0;i<nCharacter;i++) fscanf(fi,"%s",Character[i]);
	fclose(fi);
	return true;
}
bool LoadMapFile(const char *MapFilename)
{
	int i,j;
	FILE *fi;
	fi=fopen(MapFilename,"r");
	if(fi==NULL) return false;
	fscanf(fi,"%d %d\n",&MapSize.x,&MapSize.y);
	for(i=0;i<MapSize.x;i++) for(j=0;j<MapSize.y;j++) fscanf(fi,"%d",&Map[i][j]);
	fscanf(fi,"\n%d",&nMapStartLocation);
	for(i=1;i<=nMapStartLocation;i++) fscanf(fi,"\n%d %d",&MapStartLocation[i].x,&MapStartLocation[i].y),MapStartLocation[i].x--,MapStartLocation[i].y--;
	fscanf(fi,"\n%d",&nMove);
	for(i=1;i<=nMove;i++) fscanf(fi,"\n%d %d %d %d",&Move[i][0].x,&Move[i][0].y,&Move[i][1].x,&Move[i][1].y),Move[i][0].x--,Move[i][0].y--,Move[i][1].x--,Move[i][1].y--;
	fclose(fi);
	return true;
}
bool LoadItem(void)
{
	int i,len;
	FILE *fi=fopen("Item.dat","r");
	if(fi==NULL) return false;
	fscanf(fi,"%d",&nItem);
	for(i=1;i<=nItem;i++)
	{
		fscanf(fi,"\n%d ",&len);
		fgets(Item[i].ItemCharacter,len+1,fi);
		fscanf(fi," %d",&Item[i].Shot);
		fscanf(fi," %d",&Item[i].Attack);
		fscanf(fi," %d",&Item[i].Range);
		fscanf(fi," %d",&Item[i].Elapse);
	}
	fclose(fi);
	return true;
}
bool LoadKey(void)
{
	int i;
	FILE *fi=fopen("Key.dat","r");
	if(fi==NULL) return false;
	fscanf(fi,"%d",&nPlayer);
	for(i=1;i<=nPlayer;i++)
	{
		fscanf(fi,"%d %d",&UserKey[i].Up.Key,&UserKey[i].Up.Type);
		fscanf(fi," %d %d",&UserKey[i].Down.Key,&UserKey[i].Down.Type);
		fscanf(fi," %d %d",&UserKey[i].Left.Key,&UserKey[i].Left.Type);
		fscanf(fi," %d %d",&UserKey[i].Right.Key,&UserKey[i].Right.Type);
		fscanf(fi," %d %d",&UserKey[i].Command.Key,&UserKey[i].Command.Type);
		fscanf(fi," %d %d",&UserKey[i].Prev.Key,&UserKey[i].Prev.Type);
		fscanf(fi," %d %d",&UserKey[i].Next.Key,&UserKey[i].Next.Type);
	}
	fclose(fi);
	return true;
}
void GetKey(CKeyboard &Key)
{
	Key.Type=1;
	Key.Key=_getch();
	if(Key.Key==0xE0 || Key.Key==0)
	{
		Key.Key=_getch();
		Key.Type=0;
	}
}
void ShowMap(void)
{
	int i,j;
	gotoxy(0,0);
	for(i=0;i<=MapSize.y+1;i++) printf("%s",CHARACTER_MAP_BACKGROUND);
	for(i=0;i<MapSize.x;i++)
	{
		putch('\n');
		printf("%s",CHARACTER_MAP_BACKGROUND);
		for(j=0;j<MapSize.y;j++)
		{
			switch(Map[i][j])
			{
			case 0 : printf("%s",CHARACTER_MAP0); break;
			case 1 : printf("%s",CHARACTER_MAP1); break;
			case 2 : printf("%s",CHARACTER_MAP0); break;
			}
		}
		printf("%s",CHARACTER_MAP_BACKGROUND);
	}
	putch('\n');
	for(i=0;i<=MapSize.y+1;i++) printf("%s",CHARACTER_MAP_BACKGROUND);
	for(i=1;i<=nMove;i++)
	{
		for(j=0;j<2;j++)
		{
			CharacterGotoxy(Move[i][j]);
			printf("%s",CHARACTER_MOVE);
		}
	}
}
void MovePlayer(int UserIndex,const Direction D)
{
	int i;
	CPoint Before,After;
	Before=After=User[UserIndex].Point;
	switch(D)
	{
	case Up : After.x--;
		break;
	case Down : After.x++;
		break;
	case Left : After.y--;
		break;
	case Right : After.y++;
		break;
	}
	if(GetMapCharacter(After)!=CHARACTER_MAP0)
	{
		if(GetMapCharacter(After)==CHARACTER_MOVE)
		{
			for(i=1;i<=nMove;i++)
			{
				if(Move[i][0].x==After.x && Move[i][0].y==After.y) After=Move[i][1];
				else if(Move[i][1].x==After.x && Move[i][1].y==After.y) After=Move[i][0];
			}
		}
		else return;
	}
	if(Map[After.x][After.y]==2)
	{
		Map[After.x][After.y]=0;
		User[UserIndex].Hp.Pos-=Item[4].Attack;
		if(User[UserIndex].Hp.Pos<=0) User[UserIndex].Die=true,CharacterGotoxy(User[UserIndex].Point),printf("%s",GetMapCharacter(User[UserIndex].Point));
		ShowUserHp(UserIndex);
	}
	CharacterGotoxy(Before);
	printf("%s",GetMapCharacter(Before));
	CharacterGotoxy(After);
	SetTextColor(User[UserIndex].CharacterColor);
	printf("%s",User[UserIndex].Character);
	SetTextColor("0");
	User[UserIndex].Point=After;
}
void SaveReplay(const char *MapFileName)
{
	int i;
	char szText[1000];
	FILE *fi=fopen(MapName,"r"),*fo=fopen(MapFileName,"w");
	while(fgets(szText,1000,fi)) fprintf(fo,"%s",szText);
	fclose(fi);
	fprintf(fo,"\n%d\n%d",nUser,MapStartLocationIndex[1]);
	for(i=2;i<=nUser;i++) fprintf(fo," %d",MapStartLocationIndex[i]);
	fprintf(fo,"\n%s",User[1].Character);
	for(i=2;i<=nUser;i++) fprintf(fo," %s",User[i].Character);
	fprintf(fo,"\n%s",User[1].CharacterColor);
	for(i=2;i<=nUser;i++) fprintf(fo," %s",User[i].CharacterColor);
	fprintf(fo,"\n%d\n",nReplay-1);
	if(nReplay!=1)
	{
		fprintf(fo,"%d %d %d",Replay[1].Time,Replay[1].UserIndex,Replay[1].UserDirection);
		for(i=2;i<nReplay;i++) fprintf(fo," %d %d %d",Replay[i].Time,Replay[i].UserIndex,Replay[i].UserDirection);
	}
	fprintf(fo,"\n%d",ExitTime-StartTime);
	fclose(fo);
}
int ProcessKeyboard(const CKeyboard Key)
{
	int i,j,k,Temp;
	char ItemCharacter[3];
	CPoint Point;
	if(CompareKeyboard(Key,EXITGAME)==true)
	{
		CKeyboard Key;
		ExitTime=clock();
		clrscr();
		printf("Replay 파일을 저장하실 것입니까(Y/N)?");
		while(1)
		{
			GetKey(Key);
			if(Key.Key=='Y' || Key.Key=='y')
			{
				char MapFileName[1000];
				FILE *fp;
				clrscr();
				printf("파일 이름 : ");
				while(1)
				{
					fflush(stdin);
					gets(MapFileName);
					if(strcmp(MapFileName+strlen(MapFileName)-4,".rep")!=0) strcat(MapFileName,".rep");
					if(strcmp(MapFileName,".rep")==0)
					{
						gotoxy(12,0);
						continue;
					}
					fp=fopen(MapFileName,"r");
					if(fp==NULL)
					{
						SaveReplay(MapFileName);
						return 1;
					}
					else
					{
						fclose(fp);
						printf("같은 경로에 파일이 존재합니다.\n덮어쓰시겠습니까(Y/N)?");
						while(1)
						{
							GetKey(Key);
							if(Key.Key=='Y' || Key.Key=='y')
							{
								SaveReplay(MapFileName);
								return 1;
							}
							else if(Key.Key=='N' || Key.Key=='n') break;
						}
						clrscr();
						printf("파일 이름 : ");
					}
				}
			}
			else if(Key.Key=='N' || Key.Key=='n') break;
			else if(CompareKeyboard(Key,EXITGAME)==true) return 1;
		}
		return 1;
	}
	for(i=1;i<=nUser;i++)
	{
		if(User[i].Die==true) continue;
		if(CompareKeyboard(Key,UserKey[i].Up)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=1;
			nReplay++;
			User[i].Vision=Up;
			if(User[i].MoveCheck==true)
			{
				CPoint Point=User[i].Point;
				Point.x--;
				if(CheckMapPlayer(Point)==-1)
				{
					if(User[i].MoveSpeed.IsElapse()==false) return 0;
					User[i].MoveSpeed.SetTime();
					MovePlayer(i,Up);
				}
			}
		}
		else if(CompareKeyboard(Key,UserKey[i].Down)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=2;
			nReplay++;
			User[i].Vision=Down;
			if(User[i].MoveCheck==true)
			{
				CPoint Point=User[i].Point;
				Point.x++;
				if(CheckMapPlayer(Point)==-1)
				{
					if(User[i].MoveSpeed.IsElapse()==false) return 0;
					User[i].MoveSpeed.SetTime();
					MovePlayer(i,Down);
				}
			}
		}
		else if(CompareKeyboard(Key,UserKey[i].Left)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=3;
			nReplay++;
			User[i].Vision=Left;
			if(User[i].MoveCheck==true)
			{
				CPoint Point=User[i].Point;
				Point.y--;
				if(CheckMapPlayer(Point)==-1)
				{
					if(User[i].MoveSpeed.IsElapse()==false) return 0;
					User[i].MoveSpeed.SetTime();
					MovePlayer(i,Left);
				}
			}
		}
		else if(CompareKeyboard(Key,UserKey[i].Right)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=4;
			nReplay++;
			User[i].Vision=Right;
			if(User[i].MoveCheck==true)
			{
				CPoint Point=User[i].Point;
				Point.y++;
				if(CheckMapPlayer(Point)==-1)
				{
					if(User[i].MoveSpeed.IsElapse()==false) return 0;
					User[i].MoveSpeed.SetTime();
					MovePlayer(i,Right);
				}
			}
		}
		else if(CompareKeyboard(Key,UserKey[i].Command)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=5;
			nReplay++;
			if(AttackTimerCheck[i][User[i].Focus]==true) break;
			else if(Item[User[i].Focus].AttackSpeed.IsElapse()==false) break;
			Item[User[i].Focus].AttackSpeed.SetTime();
			if(User[i].Item[User[i].Focus].Shot!=INF)
			{
				if(User[i].Item[User[i].Focus].Shot<=0) break;
				User[i].Item[User[i].Focus].Shot--;
				gotoxy(0,24);
				ShowUserShot(i);
			}
			switch(User[i].Focus)
			{
			case 1 :
				Point=User[i].Point;
				switch(User[i].Vision)
				{
				case Up :
					strcpy(ItemCharacter,"↑");
					Point.x--;
					break;
				case Down :
					strcpy(ItemCharacter,"↓");
					Point.x++;
					break;
				case Left :
					strcpy(ItemCharacter,"←");
					Point.y--;
					break;
				case Right :
					strcpy(ItemCharacter,"→");
					Point.y++;
					break;
				}
				CharacterGotoxy(Point);
				SetTextColor("8");
				printf("%s",ItemCharacter);
				SetTextColor("0");
				AttackTimer[i][User[i].Focus].SetTimer(User[i].Item[User[i].Focus].Elapse);
				AttackTimerPoint[i][User[i].Focus]=Point;
				AttackTimerCheck[i][User[i].Focus]=true;
				AttackTimerStep[i][User[i].Focus]=1;
				User[i].MoveCheck=false;
				Temp=CheckMapPlayer(Point);
				if(Temp!=-1)
				{
					User[Temp].Hp.Pos-=User[i].Item[User[i].Focus].Attack,ShowUserHp(Temp);
					if(User[Temp].Hp.Pos<=0)
					{
						User[Temp].Die=true;
						CharacterGotoxy(User[Temp].Point);
						printf("%s",GetMapCharacter(User[Temp].Point));
					}
				}
				break;
			case 2 :
				Point=User[i].Point;
				AttackTimerGo[i][User[i].Focus]=1;
				switch(User[i].Vision)
				{
				case Up :
					Point.x--;
					break;
				case Down :
					Point.x++;
					break;
				case Left :
					Point.y--;
					break;
				case Right :
					Point.y++;
					break;
				}
				Temp=CheckMapPlayer(Point);
				if(Temp==-1)
				{
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(User[i].Item[User[i].Focus].Elapse);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=1;
				}
				else
				{
					User[Temp].Hp.Pos-=User[i].Item[User[i].Focus].Attack,ShowUserHp(Temp);
					if(User[Temp].Hp.Pos<=0)
					{
						User[Temp].Die=true;
						CharacterGotoxy(User[Temp].Point);
						printf("%s",GetMapCharacter(User[Temp].Point));
					}
				}
				break;
			case 3 :
				Point=User[i].Point;
				AttackTimerGo[i][User[i].Focus]=1;
				switch(User[i].Vision)
				{
				case Up :
					Point.x--;
					break;
				case Down :
					Point.x++;
					break;
				case Left :
					Point.y--;
					break;
				case Right :
					Point.y++;
					break;
				}
				Temp=CheckMapPlayer(Point);
				if(Temp==-1)
				{
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(User[i].Item[User[i].Focus].Elapse);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=1;
				}
				else
				{
					User[Temp].Hp.Pos-=User[i].Item[User[i].Focus].Attack,ShowUserHp(Temp);
					if(User[Temp].Hp.Pos<=0)
					{
						User[Temp].Die=true;
						CharacterGotoxy(User[Temp].Point);
						printf("%s",GetMapCharacter(User[Temp].Point));
					}
				}
				break;
			case 4 :
				if(Map[User[i].Point.x][User[i].Point.y]==0) Map[User[i].Point.x][User[i].Point.y]=2;
				else User[i].Item[User[i].Focus].Shot++,ShowUserShot(i);
				break;
			case 5 :
				Point=User[i].Point;
				AttackTimerGo[i][User[i].Focus]=1;
				switch(User[i].Vision)
				{
				case Up :
					Point.x--;
					break;
				case Down :
					Point.x++;
					break;
				case Left :
					Point.y--;
					break;
				case Right :
					Point.y++;
					break;
				}
				Temp=CheckMapPlayer(Point);
				if(Temp==-1)
				{
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(User[i].Item[User[i].Focus].Elapse);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=0;
				}
				else
				{
					AttackTimerStep[i][User[i].Focus]=2;
					CPoint Points[5];
					for(j=0;j<=4;j++) Points[j]=User[i].Point;
					Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
					for(j=0;j<=4;j++)
					{
						if(Points[j].x<0 || Points[j].x>=MapSize.x || Points[j].y<0 || Points[j].y>=MapSize.y) continue;
						Temp=CheckMapPlayer(Points[j]);
						CharacterGotoxy(Points[j]);
						SetTextColor("8"),printf("▒"),SetTextColor("0");
						if(Temp!=-1)
						{
							float Times;
							switch(j)
							{
							case 0 :
								Times=(float)1;
								break;
							case 1 :
							case 2 :
							case 3 :
							case 4 :
								Times=(float)1.2;
								break;
							}
							User[Temp].Hp.Pos-=(int)(User[i].Item[User[i].Focus].Attack*Times),ShowUserHp(Temp);
							if(User[Temp].Hp.Pos<=0)
							{
								User[Temp].Die=true;
								CharacterGotoxy(User[Temp].Point);
								printf("%s",GetMapCharacter(User[Temp].Point));
							}
						}
					}
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(1000);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=1;
				}
				break;
			case 6 :
				User[i].MoveCheck=false;
				Point=User[i].Point;
				AttackTimerGo[i][User[i].Focus]=1;
				switch(User[i].Vision)
				{
				case Up :
					Point.x--;
					break;
				case Down :
					Point.x++;
					break;
				case Left :
					Point.y--;
					break;
				case Right :
					Point.y++;
					break;
				}
				Temp=CheckMapPlayer(Point);
				if(Temp==-1)
				{
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(User[i].Item[User[i].Focus].Elapse);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=0;
				}
				else
				{
					AttackTimerStep[i][User[i].Focus]=2;
					CPoint Points[5];
					for(j=0;j<=4;j++) Points[j]=User[i].Point;
					Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
					for(j=0;j<=4;j++)
					{
						if(Points[j].x<0 || Points[j].x>=MapSize.x || Points[j].y<0 || Points[j].y>=MapSize.y) continue;
						Temp=CheckMapPlayer(Points[j]);
						CharacterGotoxy(Points[j]);
						SetTextColor("8"),printf("▒"),SetTextColor("0");
						if(Temp!=-1)
						{
							float Times;
							switch(j)
							{
							case 0 :
								Times=(float)1;
								break;
							case 1 :
							case 2 :
							case 3 :
							case 4 :
								Times=(float)1.2;
								break;
							}
							User[Temp].Hp.Pos-=(int)(User[i].Item[User[i].Focus].Attack*Times),ShowUserHp(Temp);
							if(User[Temp].Hp.Pos<=0)
							{
								User[Temp].Die=true;
								CharacterGotoxy(User[Temp].Point);
								printf("%s",GetMapCharacter(User[Temp].Point));
							}
						}
					}
					AttackTimerVision[i][User[i].Focus]=User[i].Vision;
					AttackTimer[i][User[i].Focus].SetTimer(1000);
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=1;
				}
				break;
			case 7 :
				if(AttackTimerStep[i][User[i].Focus]==0)
				{
					AttackTimerStep[i][User[i].Focus]=1;
					AttackTimerPoint[i][User[i].Focus]=User[i].Point;
				}
				else
				{
					User[i].Item[User[i].Focus].Shot++,ShowUserShot(i);
					AttackTimerCheck[i][User[i].Focus]=true;
					AttackTimerStep[i][User[i].Focus]=2;
					CPoint Points[9];
					for(k=0;k<=8;k++) Points[k]=AttackTimerPoint[i][User[i].Focus];
					Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++,Points[5].x--,Points[5].y--,Points[6].x--,Points[6].y++,Points[7].x++,Points[7].y--,Points[8].x++,Points[8].y++;
					for(k=0;k<=8;k++)
					{
						if(Points[k].x<0 || Points[k].x>=MapSize.x || Points[k].y<0 || Points[k].y>=MapSize.y) continue;
						Temp=CheckMapPlayer(Points[k]);
						CharacterGotoxy(Points[k]);
						SetTextColor("8"),printf("▒"),SetTextColor("0");
						if(Temp!=-1)
						{
							float Times;
							switch(k)
							{
							case 0 :
								Times=(float)1.2;
								break;
							case 1 :
							case 2 :
							case 3 :
							case 4 :
								Times=(float)1;
								break;
							case 5 :
							case 6 :
							case 7 :
							case 8 :
								Times=(float)0.8;
							}
							User[Temp].Hp.Pos-=(int)(User[i].Item[User[i].Focus].Attack*Times),ShowUserHp(Temp);
							if(User[Temp].Hp.Pos<=0)
							{
								User[Temp].Die=true;
								CharacterGotoxy(User[Temp].Point);
								printf("%s",GetMapCharacter(User[Temp].Point));
							}
						}
					}
					AttackTimer[i][User[i].Focus].SetTimer(1000);
				}
				break;
			case 8 :
				if(Map[User[i].Point.x][User[i].Point.y]!=1 && GetMapCharacter(User[i].Point)!=CHARACTER_MOVE) Map[User[i].Point.x][User[i].Point.y]=1;
				else User[i].Item[User[i].Focus].Shot++,ShowUserShot(i);
				break;
			}
		}
		else if(CompareKeyboard(Key,UserKey[i].Prev)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=6;
			nReplay++;
			User[i].Focus--;
			if(User[i].Focus==0) User[i].Focus=nItem;
			gotoxy(36,MapSize.x+i+1);
			printf("%s     ",User[i].Item[User[i].Focus].ItemCharacter);
			ShowUserShot(i);
		}
		else if(CompareKeyboard(Key,UserKey[i].Next)==true)
		{
			Replay[nReplay].Time=clock()-StartTime;
			Replay[nReplay].UserIndex=i;
			Replay[nReplay].UserDirection=7;
			nReplay++;
			if(User[i].Focus==nItem) User[i].Focus=0;
			User[i].Focus++;
			gotoxy(36,MapSize.x+i+1);
			printf("%s     ",User[i].Item[User[i].Focus].ItemCharacter);
			ShowUserShot(i);
		}
	}
	return 0;
}
void GameHelp(void)
{
	clrscr();
}
void GameSetting(void)
{
	clrscr();
}
int main(void)
{
	delay(0);
	randomize();
	int i,j,k,UserCharacter[MAX_PLAYER+1],Temp,nMap=0,Cursor=1;
	char ItemCharacter[3],MapFileName[80][1000];
	bool Command[MAX_PLAYER+1],Break=false;
	long handle;
	FILE *fi;
	_finddata_t finddata;
	CPoint Before,After;
	CKeyboard Key;
	setcursortype(NOCURSOR);
	system("color F0");
	printf("%s %s\n만든이 : %s\n오류 신고는 bgjun0204@naver.com",PROGRAM_NAME,PROGRAM_VERSION,PROGRAM_AUTHOR);
	delay(2000);
	clrscr();
	printf("캐릭터 로딩중...");
	if(LoadCharacter()==false)
	{
		clrscr();
		printf("Character.dat 파일이 없습니다!!!");
		delay(1000);
		return 0;
	}
	printf("완료\n");
	printf("아이템 로딩중...");
	if(LoadItem()==false)
	{
		clrscr();
		printf("Item.dat 파일이 없습니다!!!");
		delay(1000);
		return 0;
	}
	printf("완료\n");
	printf("키 로딩중...");
	if(LoadKey()==false)
	{
		clrscr();
		printf("Key.dat 파일이 없습니다!!!");
		delay(1000);
		return 0;
	}
	printf("완료\n");
	clrscr();
	printf("%s %s\n▶｜게임 시작\n▷｜게임 방법\n▷｜게임 설정",PROGRAM_NAME,PROGRAM_VERSION);
	Cursor=1;
	while(1)
	{
		GetKey(Key);
		if(CompareKeyboard(Key,EXITGAME)==true) return 0;
		else if(CompareKeyboard(Key,KeyUpArrow)==true)
		{
			if(Cursor>1)
			{
				gotoxy(0,Cursor);
				printf("▷");
				Cursor--;
				gotoxy(0,Cursor);
				printf("▶");
			}
		}
		else if(CompareKeyboard(Key,KeyDownArrow)==true)
		{
			if(Cursor<3)
			{
				gotoxy(0,Cursor);
				printf("▷");
				Cursor++;
				gotoxy(0,Cursor);
				printf("▶");
			}
		}
		else if(CompareKeyboard(Key,KeyCommand)==true)
		{
			if(Cursor==1) break;
			else if(Cursor==2) GameHelp();
			else if(Cursor==3) GameSetting();
			clrscr();
			printf("%s %s\n▶｜게임 시작\n▷｜게임 방법\n▷｜게임 설정",PROGRAM_NAME,PROGRAM_VERSION);
			Cursor=1;
		}
	}
	clrscr();
	handle=_findfirst("*.map",&finddata);
	if(handle!=-1)
	{
		do
		{
			nMap++;
			fi=fopen(finddata.name,"r");
			if(fi!=NULL)
			{
				fclose(fi);
				for(i=strlen(finddata.name)-1;i>=0;i--) if(finddata.name[i]=='\\') break;
				i++;
				strcpy(MapFileName[nMap],&finddata.name[i]);
				printf("▷｜%s\n",MapFileName[nMap]);
			}
		}while(_findnext(handle,&finddata)!=-1);
		_findclose(handle);
	}
	else
	{
		printf("맵 파일이 없습니다!!!");
		delay(1000);
		return 0;
	}
	gotoxy(0,0);
	printf("▶");
	while(1)
	{
		GetKey(Key);
		if(CompareKeyboard(Key,EXITGAME)==true) return 0;
		else if(CompareKeyboard(Key,MapRandom)==true)
		{
			Cursor=random(nMap)+1;
			goto MapSelect;
		}
		for(i=1;i<=nPlayer;i++)
		{
			if(CompareKeyboard(Key,UserKey[i].Up)==true)
			{
				if(Cursor>1)
				{
					gotoxy(0,Cursor-1);
					printf("▷");
					Cursor--;
					gotoxy(0,Cursor-1);
					printf("▶");
				}
			}
			else if(CompareKeyboard(Key,UserKey[i].Down)==true)
			{
				if(Cursor<nMap)
				{
					gotoxy(0,Cursor-1);
					printf("▷");
					Cursor++;
					gotoxy(0,Cursor-1);
					printf("▶");
				}
			}
			else if(CompareKeyboard(Key,UserKey[i].Command)==true)
			{
MapSelect:
				if(LoadMapFile(MapFileName[Cursor])==true)
				{
					strcpy(MapName,MapFileName[Cursor]);
					Break=true;
					break;
				}
			}
		}
		if(Break==true) break;
	}
	Break=false;
	nUser=2;
	if(nPlayer>nMapStartLocation) nPlayer=nMapStartLocation;
	clrscr();
	printf("플레이어 수(2~%d) : 2",nPlayer);
	while(1)
	{
		GetKey(Key);
		if(CompareKeyboard(Key,EXITGAME)==true) return 0;
		for(i=1;i<=nPlayer;i++)
		{
			if(CompareKeyboard(Key,UserKey[i].Left)==true)
			{
				if(nUser>2)
				{
					nUser--;
					gotoxy(19,0);
					printf("%d",nUser);
				}
			}
			else if(CompareKeyboard(Key,UserKey[i].Right)==true)
			{
				if(nUser<nPlayer)
				{
					nUser++;
					gotoxy(19,0);
					printf("%d",nUser);
				}
			}
			else if(CompareKeyboard(Key,UserKey[i].Command)==true)
			{
				Break=true;
				break;
			}
		}
		if(Break==true) break;
	}
	clrscr();
	for(i=1;i<=nUser;i++) UserCharacter[i]=0,Command[i]=false;
	printf("캐릭터 고르기\n");
	for(i=1;i<=nUser;i++) printf("Player%d ",i);
	putch('\n');
	for(i=0;i<nUser;i++) printf("%s      ",Character[0]);
	putch('\n');
	while(1)
	{
		GetKey(Key);
		if(CompareKeyboard(Key,EXITGAME)==true) return 0;
		for(i=1;i<=nUser;i++)
		{
			if(CompareKeyboard(Key,UserKey[i].Left)==true)
			{
				if(UserCharacter[i]==0) UserCharacter[i]=nCharacter;
				UserCharacter[i]--;
				gotoxy(i*8-8,2);
				printf("%s",Character[UserCharacter[i]]);
				break;
			}
			else if(CompareKeyboard(Key,UserKey[i].Right)==true)
			{
				UserCharacter[i]++;
				if(UserCharacter[i]==nCharacter) UserCharacter[i]=0;
				gotoxy(i*8-8,2);
				printf("%s",Character[UserCharacter[i]]);
				break;
			}
			else if(CompareKeyboard(Key,UserKey[i].Command)==true)
			{
				gotoxy(i*8-8,3);
				printf("결정");
				Command[i]=true;
				for(j=1;j<=nUser;j++) if(Command[j]==false) break;
				break;
			}
		}
		if(j==nUser+1) break;
	}
	clrscr();
	j=0;
	for(i=1;i<=nUser;i++) Command[i]=false,User[i].CharacterColor[0]='0',User[i].CharacterColor[1]=0;
	printf("캐릭터 색\n");
	for(i=1;i<=nUser;i++) printf("Player%d ",i);
	putch('\n');
	for(i=1;i<=nUser;i++) printf("%s      ",Character[UserCharacter[i]]);
	putch('\n');
	while(1)
	{
		GetKey(Key);
		if(CompareKeyboard(Key,EXITGAME)==true) return 0;
		for(i=1;i<=nUser;i++)
		{
			if(CompareKeyboard(Key,UserKey[i].Left)==true)
			{
LeftAgain:
				if(User[i].CharacterColor[0]=='0') User[i].CharacterColor[0]='F'+1;
				if(User[i].CharacterColor[0]=='A') User[i].CharacterColor[0]='9'+1;
				User[i].CharacterColor[0]--;
				if(User[i].CharacterColor[0]=='F') goto LeftAgain;
				gotoxy(i*8-8,2);
				SetTextColor(User[i].CharacterColor);
				printf("%s",Character[UserCharacter[i]]);
				SetTextColor("0");
				break;
			}
			else if(CompareKeyboard(Key,UserKey[i].Right)==true)
			{
RightAgain:
				if(User[i].CharacterColor[0]=='9') User[i].CharacterColor[0]='A'-1;
				if(User[i].CharacterColor[0]=='F') User[i].CharacterColor[0]='0'-1;
				User[i].CharacterColor[0]++;
				if(User[i].CharacterColor[0]=='F') goto RightAgain;
				gotoxy(i*8-8,2);
				SetTextColor(User[i].CharacterColor);
				printf("%s",Character[UserCharacter[i]]);
				SetTextColor("0");
				break;
			}
			else if(CompareKeyboard(Key,UserKey[i].Command)==true)
			{
				gotoxy(i*8-8,3);
				printf("결정");
				Command[i]=true;
				for(j=1;j<=nUser;j++) if(Command[j]==false) break;
				break;
			}
		}
		if(j==nUser+1) break;
	}
	clrscr();
	gotoxy(0,MapSize.x+2);
	for(i=1;i<=nUser;i++)
	{
		for(j=1;j<=nItem;j++) User[i].Item[j]=Item[j];
		User[i].MoveSpeed.SetTimer(200);
		User[i].HpUp.SetTimer(500);
		User[i].Focus=1;
		User[i].Hp.Max=900;
		User[i].Hp.Pos=800;
		User[i].MoveCheck=true;
		strcpy(User[i].Character,Character[UserCharacter[i]]);
		printf("P%d [                    ] %3d / %3d %s\n",i,User[i].Hp.Pos,User[i].Hp.Max,Item[1].ItemCharacter);
	}
	for(i=1;i<=nUser;i++) ShowUserHp(i),ShowUserShot(i);
	ShowMap();
	SetCharacterLocation();
	for(i=1;i<=nUser;i++)
	{
		gotoxy(User[i].Point.y*2+2,User[i].Point.x+1);
		SetTextColor(User[i].CharacterColor);
		printf("%s",User[i].Character);
		SetTextColor("0");
	}
	StartTime=clock();
	while(1)
	{
		if(_kbhit())
		{
			GetKey(Key);
			if(ProcessKeyboard(Key)==1) return 0;
		}
		for(i=1;i<=nUser;i++)
		{
			if(User[i].Hp.Max!=User[i].Hp.Pos && User[i].HpUp.IsElapse()==true && User[i].Die==false)
			{
				User[i].HpUp.SetTime();
				User[i].Hp.Pos+=1;
				ShowUserHp(i);
			}
			for(j=1;j<=nItem;j++)
			{
				if(AttackTimerCheck[i][j]==true)
				{
					switch(j)
					{
					case 1 :
						if(AttackTimer[i][j].IsElapse()==false) break;
						AttackTimer[i][j].SetTime();
						if(AttackTimerStep[i][j]==1)
						{
							CharacterGotoxy(AttackTimerPoint[i][j]);
							Temp=CheckMapPlayer(AttackTimerPoint[i][j]);
							if(Temp==-1) printf("%s",GetMapCharacter(AttackTimerPoint[i][j]));
							else
							{
								SetTextColor(User[Temp].CharacterColor);
								printf("%s",User[Temp].Character);
								SetTextColor("0");
							}
							AttackTimer[i][j].SetTimer(500);
							AttackTimerStep[i][j]=2;
							User[i].MoveCheck=true;
						}
						else if(AttackTimerStep[i][j]==2) AttackTimerCheck[i][j]=false;
						break;
					case 2 :
						if(AttackTimer[i][j].IsElapse()==false) break;
						AttackTimer[i][j].SetTime();
						Before=After=AttackTimerPoint[i][j];
						switch(AttackTimerVision[i][j])
						{
						case Up :
							After.x--;
							break;
						case Down :
							After.x++;
							break;
						case Left :
							After.y--;
							break;
						case Right :
							After.y++;
							break;
						}
						CharacterGotoxy(Before);
						Temp=CheckMapPlayer(Before);
						if(Temp!=-1)
						{
							SetTextColor(User[Temp].CharacterColor);
							printf("%s",User[Temp].Character);
							SetTextColor("0");
						}
						else printf("%s",GetMapCharacter(Before));
						Temp=CheckMapPlayer(After);
						if(Temp!=-1)
						{
							User[Temp].Hp.Pos-=User[i].Item[j].Attack-AttackTimerGo[i][j],ShowUserHp(Temp),AttackTimerCheck[i][j]=false;
							if(User[Temp].Hp.Pos<=0)
							{
								User[Temp].Die=true;
								CharacterGotoxy(User[Temp].Point);
								printf("%s",GetMapCharacter(User[Temp].Point));
							}
						}
						else if(GetMapCharacter(After)==CHARACTER_MAP0 || GetMapCharacter(After)==CHARACTER_MOVE)
						{
							if(AttackTimerGo[i][j]>Item[j].Range) AttackTimerCheck[i][j]=false;
							else
							{
								CharacterGotoxy(After);
								printf("·");
								AttackTimerPoint[i][j]=After;
								AttackTimerGo[i][j]++;
							}
						}
						else AttackTimerCheck[i][j]=false;
						break;
					case 3 :
						if(AttackTimer[i][j].IsElapse()==false) break;
						AttackTimer[i][j].SetTime();
						Before=After=AttackTimerPoint[i][j];
						switch(AttackTimerVision[i][j])
						{
						case Up :
							strcpy(ItemCharacter,"｜");
							After.x--;
							break;
						case Down :
							strcpy(ItemCharacter,"｜");
							After.x++;
							break;
						case Left :
							strcpy(ItemCharacter,"―");
							After.y--;
							break;
						case Right :
							strcpy(ItemCharacter,"―");
							After.y++;
							break;
						}
						CharacterGotoxy(Before);
						Temp=CheckMapPlayer(Before);
						if(Temp!=-1)
						{
							SetTextColor(User[Temp].CharacterColor);
							printf("%s",User[Temp].Character);
							SetTextColor("0");
						}
						else printf("%s",GetMapCharacter(Before));
						Temp=CheckMapPlayer(After);
						if(Temp!=-1)
						{
							User[Temp].Hp.Pos-=User[i].Item[j].Attack-AttackTimerGo[i][j],ShowUserHp(Temp),AttackTimerCheck[i][j]=false;
							if(User[Temp].Hp.Pos<=0)
							{
								User[Temp].Die=true;
								CharacterGotoxy(User[Temp].Point);
								printf("%s",GetMapCharacter(User[Temp].Point));
							}
						}
						else if(GetMapCharacter(After)==CHARACTER_MAP0 || GetMapCharacter(After)==CHARACTER_MOVE)
						{
							if(AttackTimerGo[i][j]>Item[j].Range) AttackTimerCheck[i][j]=false;
							else
							{
								CharacterGotoxy(After);
								printf("%s",ItemCharacter);
								AttackTimerPoint[i][j]=After;
								AttackTimerGo[i][j]++;
							}
						}
						else AttackTimerCheck[i][j]=false;
						break;
					case 5 :
						if(AttackTimerStep[i][j]==0)
						{
							if(AttackTimer[i][j].IsElapse()==false) break;
							AttackTimer[i][j].SetTime();
							Before=After=AttackTimerPoint[i][j];
							switch(AttackTimerVision[i][j])
							{
							case Up :
								After.x--;
								break;
							case Down :
								After.x++;
								break;
							case Left :
								After.y--;
								break;
							case Right :
								After.y++;
								break;
							}
							strcpy(ItemCharacter,"◎");
							CharacterGotoxy(Before);
							Temp=CheckMapPlayer(Before);
							if(Temp!=-1)
							{
								SetTextColor(User[Temp].CharacterColor);
								printf("%s",User[Temp].Character);
								SetTextColor("0");
							}
							else printf("%s",GetMapCharacter(Before));
							Temp=CheckMapPlayer(After);
							if(Temp!=-1)
							{
								AttackTimerCheck[i][j]=false;
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=After;
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									SetTextColor("8"),printf("▒"),SetTextColor("0");
									AttackTimerStep[i][j]=1;
									if(Temp!=-1)
									{
										User[Temp].Hp.Pos-=User[i].Item[j].Attack,ShowUserHp(Temp);
										if(User[Temp].Hp.Pos<=0)
										{
											User[Temp].Die=true;
											CharacterGotoxy(User[Temp].Point);
											printf("%s",GetMapCharacter(User[Temp].Point));
										}
									}
								}
								AttackTimerStep[i][j]=1;
								AttackTimer[i][j].SetTimer(1000);
								AttackTimerCheck[i][j]=true;
								AttackTimerPoint[i][j]=After;
							}
							else if(GetMapCharacter(After)==CHARACTER_MAP0 || GetMapCharacter(After)==CHARACTER_MOVE)
							{
								CharacterGotoxy(After);
								printf("%s",ItemCharacter);
								AttackTimerPoint[i][j]=After;
								AttackTimerGo[i][j]++;
							}
							else
							{
								AttackTimerPoint[i][j]=Before;
								AttackTimerCheck[i][j]=false;
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=Before;
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									SetTextColor("8"),printf("▒"),SetTextColor("0");
									AttackTimerStep[i][j]=1;
									if(Temp!=-1)
									{
										User[Temp].Hp.Pos-=User[i].Item[j].Attack,ShowUserHp(Temp);
										if(User[Temp].Hp.Pos<=0)
										{
											User[Temp].Die=true;
											CharacterGotoxy(User[Temp].Point);
											printf("%s",GetMapCharacter(User[Temp].Point));
										}
									}
								}
								AttackTimerStep[i][j]=1;
								AttackTimer[i][j].SetTimer(1000);
								AttackTimerCheck[i][j]=true;
							}
						}
						else if(AttackTimerStep[i][j]==1)
						{
							if(AttackTimer[i][j].IsElapse()==true)
							{
								AttackTimer[i][j].SetTime();
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=AttackTimerPoint[i][j];
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									if(Temp!=-1)
									{
										SetTextColor(User[Temp].CharacterColor);
										printf("%s",User[Temp].Character);
										SetTextColor("0");
									}
									else printf("%s",GetMapCharacter(Points[k]));
								}
								AttackTimerCheck[i][j]=false;
							}
						}
						break;
					case 6 :
						if(AttackTimerStep[i][j]==0)
						{
							if(AttackTimer[i][j].IsElapse()==false) break;
							AttackTimer[i][j].SetTime();
							Before=After=AttackTimerPoint[i][j];
							switch(AttackTimerVision[i][j])
							{
							case Up :
								strcpy(ItemCharacter,"▲");
								After.x--;
								break;
							case Down :
								strcpy(ItemCharacter,"▼");
								After.x++;
								break;
							case Left :
								strcpy(ItemCharacter,"◀");
								After.y--;
								break;
							case Right :
								strcpy(ItemCharacter,"▶");
								After.y++;
								break;
							}
							CharacterGotoxy(Before);
							Temp=CheckMapPlayer(Before);
							if(Temp!=-1)
							{
								SetTextColor(User[Temp].CharacterColor);
								printf("%s",User[Temp].Character);
								SetTextColor("0");
							}
							else printf("%s",GetMapCharacter(Before));
							Temp=CheckMapPlayer(After);
							if(Temp!=-1)
							{
								AttackTimerCheck[i][j]=false;
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=After;
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									SetTextColor("8"),printf("▒"),SetTextColor("0");
									AttackTimerStep[i][j]=1;
									if(Temp!=-1)
									{
										User[Temp].Hp.Pos-=User[i].Item[j].Attack-AttackTimerGo[i][j]*10,ShowUserHp(Temp);
										if(User[Temp].Hp.Pos<=0)
										{
											User[Temp].Die=true;
											CharacterGotoxy(User[Temp].Point);
											printf("%s",GetMapCharacter(User[Temp].Point));
										}
									}
								}
								AttackTimerStep[i][j]=1;
								AttackTimer[i][j].SetTimer(1000);
								AttackTimerCheck[i][j]=true;
								AttackTimerPoint[i][j]=After;
							}
							else if(After.x<0 || After.x>=MapSize.x || After.y<0 || After.y>=MapSize.y)
							{
								AttackTimerPoint[i][j]=Before;
								AttackTimerCheck[i][j]=false;
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=Before;
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									SetTextColor("8"),printf("▒"),SetTextColor("0");
									AttackTimerStep[i][j]=1;
									if(Temp!=-1)
									{
										User[Temp].Hp.Pos-=User[i].Item[j].Attack-AttackTimerGo[i][j]*10,ShowUserHp(Temp);
										if(User[Temp].Hp.Pos<=0)
										{
											User[Temp].Die=true;
											CharacterGotoxy(User[Temp].Point);
											printf("%s",GetMapCharacter(User[Temp].Point));
										}
									}
								}
								AttackTimerStep[i][j]=1;
								AttackTimer[i][j].SetTimer(1000);
								AttackTimerCheck[i][j]=true;
							}
							else if(GetMapCharacter(After)==CHARACTER_MAP0)
							{
								CharacterGotoxy(After);
								printf("%s",ItemCharacter);
								AttackTimerPoint[i][j]=After;
								AttackTimerGo[i][j]++;
							}
							else
							{
								CharacterGotoxy(After);
								printf("%s",ItemCharacter);
								AttackTimerPoint[i][j]=After;
							}
						}
						else if(AttackTimerStep[i][j]==1)
						{
							if(AttackTimer[i][j].IsElapse()==true)
							{
								AttackTimer[i][j].SetTime();
								CPoint Points[5];
								for(k=0;k<=4;k++) Points[k]=AttackTimerPoint[i][j];
								Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++;
								for(k=0;k<=4;k++)
								{
									Temp=CheckMapPlayer(Points[k]);
									CharacterGotoxy(Points[k]);
									if(Temp!=-1)
									{
										SetTextColor(User[Temp].CharacterColor);
										printf("%s",User[Temp].Character);
										SetTextColor("0");
									}
									else printf("%s",GetMapCharacter(Points[k]));
								}
								AttackTimerCheck[i][j]=false;
								User[i].MoveCheck=true;
							}
						}
						break;
					case 7 :
						if(AttackTimer[i][j].IsElapse()==true)
						{
							AttackTimer[i][j].SetTime();
							CPoint Points[5];
							for(k=0;k<=8;k++) Points[k]=AttackTimerPoint[i][j];
							Points[1].x--,Points[2].x++,Points[3].y--,Points[4].y++,Points[5].x--,Points[5].y--,Points[6].x--,Points[6].y++,Points[7].x++,Points[7].y--,Points[8].x++,Points[8].y++;
							for(k=0;k<=8;k++)
							{
								Temp=CheckMapPlayer(Points[k]);
								CharacterGotoxy(Points[k]);
								if(Temp!=-1)
								{
									SetTextColor(User[Temp].CharacterColor);
									printf("%s",User[Temp].Character);
									SetTextColor("0");
								}
								else printf("%s",GetMapCharacter(Points[k]));
							}
							AttackTimerCheck[i][j]=false;
							AttackTimerStep[i][j]=0;
						}
						break;
					}
				}
			}
		}
	}
	return 0;
}