#include <stdio.h>
#include "led.h"
#include "button.h"
#include "buzzer.h"

#include <sys/msg.h>
#include <string.h>

//C,D,E,F,G,A,B: 계이름
//C+, C-: 옥타브
//C+#, C-@: 임시 조표
//C$, : 내추랄. 퍼마조표를 뗀다.
//C+#1, C+#2, C+#4, C+#8, C+#6: 16분음표
//C+#1., C+#2.: 점음표
//P: 쉼표
//P1: 1분쉼표
//P1.: 점 1분쉼표
//[#]: 상시조표
//[#F]: 라 장조
//[@BE]: 내림마장조

const char song_note [] = 
"[@B]" //바장조

"E6 D C# D 	F8 P G6 F E F	A8 P B6 A G# A"
"E+ D+ C+# D+ 	E+ D+	C+# D+		F+4 D+8 F+	E+ D+ C+ D+	E+D+C+D+E+D+C+B$"
"A4 E6 D C# D F8 P G6 F E F A8 P B6 A G# A"
"E+ D+ C+# D+   E+ D+ C+# D+    F+4 D+8 F+ E+D+C+D+E+D+C+D+E+D+C+B$"

"A4 A8 B C+ C+ D+6 C+ B A G4 A8 B C+ C+ D+6 C+ B A G4 F8 G"
"A A B6 A G F E4 F8 G A A B6 A G F E4 E6 D C# D"
"F8 P G6 F E F A8 P8 B6 A G# A E+ D+ C+# D+ E+D+C+# D+ F+4 D+8 E+"
"F+8 E+D+C+# D+ABGF4 E8D6ED4 D+8 E+ F+#4 D+8 E+8"

"F+8# E+ D+ C+# B$ C+# D+ E+ C+# A D+ E+ F+#4 D+8 E+ F+# E+ D+ C+#"
"B$ E+ C+# A D+4 D+8 E+ F+#4 D+8 E+8 F+#4 E+8 D+ C+# B$ C+# D+ E+"
"C+# A D+ E+ F+#4 D+8 E+ F+# E+ D+ C+# B$ E+ C+# A D+4 A6 B A G F G F E D F E D C# D E C# A B$ C+# A"

"D C# D E F E F G A G# A G# A B A G"
"F G F E D F E D C D E C A B$ C+ A B$ C+ D+ B$ G# A B$ G$"
"A4"
;


enum
{
	PARSE_TYPE_INITIAL,
	PARSE_TYPE_NOTE,		//음표/쉼표
	PARSE_TYPE_SPEED,	//속도
	PARSE_TYPE_MAJOR,	//장단조/
};
enum
{
	PARSE_STAGE_UNKNOWN,
	PARSE_STAGE_NOTE,		//계이름
	PARSE_STAGE_OCT,
	PARSE_STAGE_TEMP,		//임시조표
	PARSE_STAGE_SUSTAIN,	//지속시간
	PARSE_STAGE_POINT,		//점
	
	PARSE_STAGE_MAJOR_MINOR,
	PARSE_STAGE_MAJOR_NOTE,
	PARSE_STAGE_MAJOR_CLOSE,
};

#define DEFAULT_SUS		8			//32분음표가 기본.

int main (void)
{
	ledLibInit(); 
	buttonInit();
	buzzerInit();
	
	
	printf ("JUKEBOX by J.Min\r\n");
	printf ("\tHome: Play a song.\r\n");
	printf ("\tBack: Pause the song.\r\n");
	printf ("\tStop: Stop the song.\r\n");
	printf ("\tVol Dn: Quit the Jukebox.\r\n");
	
	printf ("\r\nEmbedded system is fun!\r\n");
	
	int songPtr = 0;
	int oneTickPerMs = DEFAULT_SUS;	//1분음표=1.024초, 2분음표=512ms, 4분음표 = 256ms, 8분음표 = 128ms, 16분음표 = 16ms; 
	int msCnt = 0;
	int waitToNextTick = DEFAULT_SUS;
	int prevWaitToNextTick = waitToNextTick;
	int parseType = PARSE_TYPE_INITIAL;
	int parseStage = PARSE_STAGE_UNKNOWN;;
	int nearByScale = 0;	//영구조표때문.
	
	int permaMajor[8] = {0,0,0,0,0,0,0,0,};	//영구조표 기록
	int majorMinor = 0;
	
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	BUTTON_MSG_T recvdMsg;
	
	int ifPlay = 0;
	while ( 1 )	
	{
		if ( msgrcv (msgID, &recvdMsg, sizeof(BUTTON_MSG_T) - sizeof(long int), 0, IPC_NOWAIT) >= 0	)
		{
			//Button!
			if (recvdMsg.keyInput == 114) //제일 오른쪽거.
				break;	//프로그램 종료.
			else if (recvdMsg.keyInput == 102) //제일 왼쪽거 Play
				ifPlay = 1;
			else if (recvdMsg.keyInput == 158) //두번째 거. Pause
				ifPlay = 0;
			else if (recvdMsg.keyInput == 217) //세번째 거. Stop
			{
				ifPlay 	= 0;
				songPtr	= 0;
			}
		}
		usleep(1000);	//Every 1ms.
		msCnt++;
		if (msCnt >= oneTickPerMs)
		{	//매 틱마다.
			msCnt = 0;
			//oneTick Reached.
			//16분음표 하나 추가.
			if (waitToNextTick == 0)
			{
				if (songPtr == strlen(song_note) )
				{
					buzzerStopSong(); //끝.
				}
				//뭔가 노트 하나를 처리해야함.
				int newNote = 0;
				if (ifPlay == 0)	//현재 플레이상태가 아니면.
				{
					//소리끄고 기둘리기.
					ledLibRaw(0);
					buzzerStopSong();
					waitToNextTick = DEFAULT_SUS;	//그냥 기달리기.
				}
				else 
				{
					while ( songPtr <= strlen(song_note) ) //while of Parse.
					{
						if (parseType == PARSE_TYPE_INITIAL)	//이것이 뭔가?!
						{
							//if ( (song_note[songPtr] == 0) || (song_note[songPtr] == ' ')	songPtr++;	//이건 공백은 해석하지 않기.
	
							switch (song_note[songPtr])
							{
								case 'A': newNote = 21;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'A'; songPtr++; break;
								case 'B': newNote = 23;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT; nearByScale = 'B'; songPtr++; break;
								case 'C': newNote = 12;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'C'; songPtr++; break;
								case 'D': newNote = 14;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'D'; songPtr++; break;
								case 'E': newNote = 16;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'E'; songPtr++; break;
								case 'F': newNote = 17;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'F'; songPtr++; break;
								case 'G': newNote = 19;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_OCT;	nearByScale = 'G'; songPtr++; break;
								case 'P': newNote = -1;	parseType = PARSE_TYPE_NOTE;	parseStage = PARSE_STAGE_SUSTAIN;	nearByScale = '-'; songPtr++; break;
								case '[': parseType = PARSE_TYPE_MAJOR; 	parseStage = PARSE_STAGE_MAJOR_MINOR; songPtr++; memset(permaMajor, 0, sizeof(permaMajor));		break;
								default: songPtr++;	break;	//공백등은 개무시.
								//case ' ':
								//default:
							}
						}
						
						else if (parseType == PARSE_TYPE_NOTE)
						{
							//waitToNextTick = DEFAULT_SUS;
							waitToNextTick = prevWaitToNextTick;
							if (parseStage == PARSE_STAGE_OCT)
							{
								if (song_note[songPtr] == '+')		//높은 옥타브...
								{
									//printf ("HighOct!\r\n");
									newNote+=12;
									songPtr++;
								}
								else if (song_note[songPtr] == '-')	//낮은 옥타브...
								{
									newNote-=12;
									songPtr++;
								}
								parseStage = PARSE_STAGE_TEMP;	//Try Again.
							}
							else if (parseStage == PARSE_STAGE_TEMP)
							{
								if (newNote != -1)
								{	//쉼표는 적용하면 안됨.
									newNote = newNote + permaMajor[nearByScale-'A'] ;	//고정조표 반영
									//if (permaMajor[nearByScale-'A'] != 0) printf ("nearByScale:%c\r\n",nearByScale);
									if (song_note[songPtr] == '#') 
									{
										songPtr++;
										if (permaMajor[nearByScale-'A']!=1)			//PermaMajor == 1:Sharp이 붙은것.
											newNote+=1;
									}
									else if (song_note[songPtr] == '@') 
									{
										songPtr++;
										if (permaMajor[nearByScale-'A']!=-1)	//PermaMajor == -1:Flat이 붙은것.
											newNote-=1;
									}
									else if (song_note[songPtr] == '$')
									{
										//......네추랄...
										songPtr++;
										newNote = newNote - permaMajor[nearByScale-'A'];
									}
								}
								//제자리표도 해야하는데 귀찮다.
								parseStage = PARSE_STAGE_SUSTAIN;
							}
							else if (parseStage == PARSE_STAGE_SUSTAIN)
							{
								//printf ("SustainStg...\r\n");
								
								if (song_note[songPtr] == '1')
								{
									waitToNextTick = DEFAULT_SUS*32;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '2')
								{
									waitToNextTick = DEFAULT_SUS*16;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '4')
								{				
									waitToNextTick = DEFAULT_SUS*8;	//32가 minimum Resolution.
									songPtr++;
								}
								else if (song_note[songPtr] == '8')
								{
									waitToNextTick = DEFAULT_SUS*4;
									songPtr++;
								}
								else if (song_note[songPtr] == '6')
								{
									waitToNextTick = DEFAULT_SUS*2;
									songPtr++;
								}
								prevWaitToNextTick = waitToNextTick;
								parseStage = PARSE_STAGE_POINT;
							}	//Sustain.
							else if ( parseStage == PARSE_STAGE_POINT )
							{
								if (song_note[songPtr] == '.')
								{
									waitToNextTick = waitToNextTick*3/2;	//1.5
									songPtr++;
								}
								//끝. Oshimai.
								//발음.
								//printf ("Ohimai.... Note:%d, SUstain:%d\r\n",newNote,waitToNextTick);
								
								//LED켜기/끄기
								if (newNote == -1)
								{	//쉼표는 다 끄기.
									ledLibRaw(0);
									buzzerStopSong();
								}
								else
								{
									ledLibRaw(0);
									ledLibOnOff((nearByScale-'A'-2+8)%8,1);	//도를 첫번째 LED에 매치시키기 위해.
									buzzerPlaySong(newNote);
								}
								parseType = PARSE_TYPE_INITIAL;
								break;	//이번 parsing 완료.
							}
						}	//End of ParseType_note
						
						else if (parseType == PARSE_TYPE_MAJOR)
						{
							//printf("ParseTypeMajor!\r\n");
							if ( parseStage == PARSE_STAGE_MAJOR_MINOR )
							{
								if ( song_note[songPtr] == '#' )
								{
									majorMinor = 1;
									songPtr++;
									parseStage = PARSE_STAGE_MAJOR_NOTE;
								}
								else if (song_note[songPtr] == '@' )
								{
									majorMinor = -1;
									songPtr++;
									parseStage = PARSE_STAGE_MAJOR_NOTE;
								}
								else
								{
									parseStage = PARSE_STAGE_MAJOR_CLOSE;
								}
							}
							else if (parseStage == PARSE_STAGE_MAJOR_NOTE)
							{
								//printf ("%c!\r\n",song_note[songPtr]);
								if ( song_note[songPtr] >= 'A' && song_note[songPtr] <= 'G' )
								{
									permaMajor[song_note[songPtr] - 'A'] = majorMinor;
									songPtr++;
								}
								else
								{
									//printf ("This should be Here!\r\n");
									parseStage = PARSE_STAGE_MAJOR_CLOSE;
								}
							}
							else if (parseStage == PARSE_STAGE_MAJOR_CLOSE)
							{
								if ( song_note[songPtr] != ']' )
									//printf ("Major Minor Open/Close err!\r\n");
								songPtr++;
								//printf ("%d,%d,%d,%d,%d,%d,%d,%d\r\n",permaMajor[0],permaMajor[1],permaMajor[2],permaMajor[3],permaMajor[4],permaMajor[5],permaMajor[6],permaMajor[7]);
								parseType = PARSE_TYPE_INITIAL;
							}
						}
					}
				}	//Play 끝.
				
				if ( songPtr > strlen(song_note) )
				{
					printf ("Song is ended, successfully!\r\n");
					//Song is ended.
					ifPlay = 0;
					songPtr = 0;	//초기화.
				}
			}
			else
			{
				//이번틱은 넘기기.
				waitToNextTick --;
			}
		}
	}
	
	
	//End of while.
	ledLibExit();
	buttonExit();
	buzzerExit();
}
