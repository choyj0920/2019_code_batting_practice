#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include<windows.h>
#pragma warning(disable: 4996)

void clear_menu(); //박스 생성 함수
void title(void);  // 타이틀 출력
int Choice_List(int );
void InitScreen(void);
void printwordset(int i);
void Run();
void ProcessInput(int);
void CompareWords();
void NextStage();
void DrawWord(int i);
void EraseWord(int i);
void UpdateScore();
void UpdateFailCount();
void StageScreen();
int snakegame();

#define KEY_ESC 27
#define KEY_ENTER '\r'
#define KEY_BS '\b'

#define MAX_WORD_LENGTH 24 //최대 단어 길이
#define MAX_SCREEN_WORD_COUNT 22
#define WORD_COUNT 16 //단어 받아올 갯수

#define COMMAND_SIZE 256  //커맨드 배열칸수

int G2count = 0, G2score[100];

struct ScreenWord  //화면에 표기할 단어 구조체
{
	int index;   //단어 담을 곳
	int x;  //위치값 x,y 
	int y;
};
typedef struct ScreenWord ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// 화면에 나타난 단어들(g_words의 원소 index 저장)
int g_screen_word_count = 0;                    // 화면에 나타난 단어 개수

clock_t g_start_time;                           // 기준 시각
double g_falling_speed = 2.0;

//int g_word_count = 16;
char **g_words;
int g_fail_count = 0;                           // 실패한 개수
int g_score = 0;                                // 정수
char g_input_word[MAX_WORD_LENGTH + 1];         // 입력 단어 저장
int g_input_word_length = 0;                    // 입력 단어 길이
int stage = 1;                                  // 스테이지 단계 저장



void StartGame2(void);

void showScore(void);
#define CMD_LINES 40  //콘솔창 줄 수
#define CMD_COLS 100  //콘솔창 칸 수

int main() {
	int choice = 0, menu;
	char command[COMMAND_SIZE] = { '\0', };
	
	sprintf(command, "mode con: lines=%d cols=%d", CMD_LINES, CMD_COLS); //콘솔창 크기 설정 
	system(command);
	srand(time(NULL)); //시작 srand 설정


	do {
		title();
		menu = Choice_List(choice);
		switch (menu) {
		case 8:
			break;
		case 10:
			StartGame2();  // 타자 게임 실행
			break;
		case 12:
			snakegame();
			break;
		case 14:
			showScore();
			break;
		case 16:
			break;
		case 18:
			break; 

		}
	} while (menu != 18);

}

//박스 생성함수
void clear_menu() { // 타이틀 창 출력 함수
	int i; //반복을 위해 변수선언
	system("cls"); // 화면을 지우고 처음부터 시작
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 모든 글자를 밝은 흰색으로 설정

	for (i = 0; i <= CMD_COLS; i++) { //첫줄 기호 출력
		if (i == 0) {
			printf("%c", 1); //왼쪽 상단 기호 출력
		}
		else if (i == CMD_COLS) {
			printf("%c", 2); // 오른쪽 상단 기호 출력
		}
		else if (i > 1 || i < CMD_COLS) {
			printf("%c", 6); //첫 줄 출력
		}
	}
	GotoXY(0, 1);
	for (i = 0; i < 22; i++) {
		printf("%c\n", 5);// 왼쪽 테두리 출력
	}
	printf("%c", 3); // 왼쪽 하단 기호 출력

	for (i = 1; i < 24; i++) {
		GotoXY(99, i);
		printf("%c", 5); //오른쪽 테두리 출력
	}
	printf("%c", 4); //오른쪽 하단 기호 출력

	GotoXY(1, 23);
	for (i = 0; i < 98; i++) {
		printf("%c", 6); //아랫줄 출력
	}
}

//메뉴 화면 출력 함수
void title(void) {
	int i, j;
	
	while (kbhit()) getch();  //버퍼에 있는 키값을 버림
	clear_menu();
	GotoXY(40, 2); printf("┌"); for (i = 0; i < 18; i++) printf("─"); printf("┐");
	GotoXY(40, 3);  printf("│%18s│"," Batting practice");
	GotoXY(40, 4); printf("└"); for (i = 0; i < 18; i++) printf("─"); printf("┘");

	GotoXY(35, 8); printf("1. Classic Batting Practice ");
	GotoXY(35, 10); printf("2.      Batting Game");
	GotoXY(35, 12); printf("3.       add Game");
	GotoXY(35, 14); printf("4.          Score");
	GotoXY(35, 16); printf("5.          use");
	GotoXY(35, 18); printf("6.          Exit ");

}

// 메뉴 선택 함수 
int Choice_List(int choice)
{
	int i; //
	int y = 8; // 화살표를 1. 에 표시한다

	do { // do-while로 실행하므로서 화살표를 화면에 띄우고 시작한다
		
		for (i = 8; i < 19; i += 2) {
			GotoXY(31, i); printf(" "); //이전 화살표 지우기
		}
		if (y >= 8 || y <= 18) {
			GotoXY(31, y); //선택지 옆에
			printf("▶");  //출력하여 현재 선택 사항을 확인
			SetCursorVisible(0);
		}
		choice = getch(); //화살표를 입력받음

		if (choice == 80) {       //아랫쪽 화살표를 입력받고,
			if (y < 18) {        //현재 화살표의 위치가 가장 아랫쪽이 아니라면
				y += 2;         //화살표를 아래로 내린다
			}
			else if(y == 18)
				y = 8;
			
		}
		else if (choice == 72) { //위쪽 화살표를 입력받고,
			if (y > 8) {       //현재 화살표의 위치가 가장 위쪽이 아니라면
				y -= 2;        //화살표를 위로 올린다
			}
			else if (y = 8)
				y = 18;
			
		}
		
	

		else if (choice == 13 || choice == 32) { //스페이스바나 엔터를 입력받으면
			choice = y;                    //선택한 항목을
			return choice;                 //반환한다
		}

	} while (1);
}

//의미 있는 단어 파일에서 불러오는 함수
void fileword() {
	int i;
	FILE *fp;
	fopen_s(&fp, "g_words.txt", "rt");
	
	g_words = (char **)malloc(sizeof(char *) * (WORD_COUNT + 1));


	for (i = 0; i < WORD_COUNT; i++)
	{
		g_words[i] = (char **)malloc(sizeof(char *) * (MAX_WORD_LENGTH + 1));  //
		fgets(g_words[i], sizeof(char) * MAX_WORD_LENGTH, fp);  //fp에 입력한 단어들 gwords에 입력
		g_words[i][strlen(g_words[i]) - 1] = '\0'; //gword[i] 의 마지막 칸 null입력

	}

	g_words[WORD_COUNT] = " ";

	fclose(fp);
	
}

//타자게임 시작 함수
void StartGame2(void) {
	stage = 1;
	g_falling_speed = 2.0;
	g_fail_count = 0;                           // 실패한 개수


	g_start_time = clock(); //시작 시간 계산
	fileword();     //file에서 단어 가져와서 g_word에 담기 
	InitScreen();  // 게임 UI출력
	Run(); //게임실행 주함수

}

// 타자 게임 틀 출력 함수
void InitScreen(void)
{
	int i;

	clear_menu();

	// 
	GotoXY(0, 24);
	printf("┌────────────┐\n");
	printf("│%12s│\n","Batting Game");
	printf("└────────────┘");

	// 입력칸
	GotoXY(28, 24); printf("┌───────────────┐");
	GotoXY(22, 25); printf("입력칸│%15s│","");
	GotoXY(28, 26); printf("└───────────────┘");

	//score 칸
	GotoXY(50, 24); printf("┌────┐");
	GotoXY(46, 25); printf("점수│%4d│",g_score);
	GotoXY(50, 26); printf("└────┘");

	//fail 칸
	GotoXY(68, 24); printf("┌───┐");
	GotoXY(59, 25); printf("실패 횟수│%3d│", g_fail_count);
	GotoXY(68, 26); printf("└───┘");
}


void Run() {
	int i, count, key; // i는 단어 19개를 만들기위해 , count 는  , key사용자 입력값
	count =  1;
	
	for(i = 0; i< 22;i++)
	{
		printwordset(i);

	}
	while (stage != 0) {
		if (_kbhit()) {
			key = _getch();
			ProcessInput(key);
		}
		if (GetDiffTime(g_start_time) > g_falling_speed) // 지난 시간이 g_falling_speed 보다 컸을때
		{
			for (i = 0; i < count; i++) 
			{
				EraseWord(i);  //i번호 단어 스크린에서 지우기
				g_screen_word[i].y++; //단어 .y값에 1추가
				if (g_screen_word[i].y == 23)  //단어의 y값이 23일때 
				{
					if (g_screen_word[i].index != WORD_COUNT) //그 단어가  값이 없지 않을 때(입력되지 않았을 때) 
					{
						g_fail_count++; //실패횟수 1추가 
						UpdateFailCount(); //실패횟수 출력 
						NextStage(); //
					}
					printwordset(i);  // 새로운 단어 담기 
					g_screen_word[i].y++;// 단어 y 1 증가
					DrawWord(i); //단어 화면 출력

				}
				else
					DrawWord(i);
			}
			if (count < MAX_SCREEN_WORD_COUNT)
				count++; //count
			g_start_time = clock(); //시간 설정
		}
	}
}

//무작위 단어  선택 나올위치[x]값 무작위 선택, 위에서 내려가기위해 y=1
void printwordset(int i) {
	g_screen_word[i].index = rand() % (WORD_COUNT);		//담겨질 단어 인덱스 랜덤 설정 0~(WORD_COUNT-1)
	g_screen_word[i].x = rand() % 77 + 1;               // 단어 출력될 X값 랜덤
	g_screen_word[i].y = 1;						//단어 출력 시작할 위치 Y=1 설정

	if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) > 78) { // 
		while (1) {
			
			g_screen_word[i].x = rand() % 77 + 1;
			if(g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) < 78)
				break;

		
		}
	}
	
}

// 입력 처리 함수
void ProcessInput(int key)
{

	if (key == KEY_ESC) {
		G2score[G2count] = stage;
		G2count++;
		stage = 0;
		StageScreen();
	}
	else if (key == KEY_BS)  //입력값이 백스페이스라면
	{
		if (g_input_word_length != 0)  //여태 입력값이 0이 아니라면 
		{
			GotoXY(30 + g_input_word_length -1 , 25); //입력값 마지막-1 위치로 
			g_input_word[g_input_word_length-1] = NULL; //입력값 저장되있는 것 마지막 글자 제거
			printf(" ");   //입력란 마지막에 쓰여진 것 제거
			g_input_word_length -= 1;
			

		}
	}
	else if (key != KEY_ENTER) //입력값이 엔터,스페이스 가 아니면
	{
		if (g_input_word_length != 24)  //여태 입력한 값이 24자를 넘지 않으면
		{
			GotoXY(30 + g_input_word_length, 25);
			printf("%c", key);  //입력칸 마지막에 출력
			g_input_word[g_input_word_length] = key;
			g_input_word_length++;  //입력한 갯수 +1
		}
	}
	else  //엔터입력
	{
		int count;  
		CompareWords(); 
		GotoXY(30, 25);
		for (count = 0; count < strlen(g_input_word); count++)
		{
			printf(" ");    //여태 입력한 자리수만큼 공백 출력
		}
		for (count = 0; count < g_input_word_length; count++)
		{
			g_input_word[count] = NULL;  //여태 입력한 자리수만큼 input_word 초기화
		}
		g_input_word_length = 0;  // 입력한값 0으로 바꿈
	}
	NextStage();  //실패 성공 횟수에 따라  다음 스테이지로 


}

//입력단어와 스크린단어 비교 함수
void CompareWords() {
	int i;
	int Del = -1;

	for (i = 0; i < 22; i++) //19개의 저장되어있는 단어 탐색 
	{
		if (strcmp(g_input_word, g_words[g_screen_word[i].index]) == 0) // 화면에 뜬 단어 중에 일치하는 것이 있으면
		{
			if (Del != -1) //Del이 -1이 아닌 경우 이번 반복 전에 일치하는게 있었을 때
				if (g_screen_word[i].y > g_screen_word[Del].y) //이 단어가 위치상으로 더 아래있으면
					Del = i;   //del을 이번 i로 
			if (Del == -1)
				Del = i;  //del을 이번 i로 
		}
	}
	if (Del != -1)  //반복문이 끝나고 일치하는 값이 있었을 때
	{
		EraseWord(Del);  //해당 단어 화면에서 제거
		g_screen_word[Del].index = WORD_COUNT;  //인덱스 값 초기화
		g_score++;   //점수 ++
		UpdateScore();  //
	}
	else {
		g_fail_count++;  //잘못 입력시 실패횟수 1추가
		UpdateFailCount(); //실패횟수 출력 
		NextStage(); //
	}
}
//실패 성공 횟수에 따라  처리되는 함수
void NextStage()  
{
	if (g_fail_count == 5) {  //실패한 횟수가 5번 보다 많을 경우 
		system("cls");
		g_score = 0;    //점수 0 초기ㅗ하
		g_fail_count = 0;  //실패 횟수
		GotoXY(0, 10);
		printf("%12s┌────────────┐\n"," ");
		printf("%12s│%12s│\n", "","    실패..");
		printf("%12s└────────────┘"," ");

		Sleep(2000);
		G2score[G2count] = stage;
		G2count++;
		
		stage = 0;
	}
	if (g_score == 8) //점수 가 8점이면
	{
		g_falling_speed *= 0.8;  //다음 단계 난이도 증가
		g_score = 0;    //점수 0 초기ㅗ하
		g_fail_count = 0;  //실패 횟수 
		UpdateScore();     //점수 다시 출력
		UpdateFailCount(); //실횟 다시 출력
		StageScreen();  //다음 스테이지 출력
	}
}


void UpdateScore(void)
{
	GotoXY(54, 25); printf("%d", g_score);
}
void UpdateFailCount(void)
{
	GotoXY(71, 25); printf("%d", g_fail_count);
}
void DrawWord(int i)
{
	GotoXY(g_screen_word[i].x, g_screen_word[i].y);
	printf("%s", g_words[g_screen_word[i].index]);
}
void EraseWord(int i)
{
	int count;
	if (g_screen_word[i].y > 0)
	{
		GotoXY(g_screen_word[i].x, g_screen_word[i].y);
		for (count = 0; count < strlen(g_words[g_screen_word[i].index]); count++)
			printf(" ");
	}
}


void StageScreen(void)
{
	system("cls");

	if (stage > 0)
	{
		GotoXY(40, 10); printf("┌────────────────┐");
		GotoXY(40, 11); printf("│%3d%9s│", stage," Stage Clear!");
		GotoXY(40, 12); printf("│%15s%1d│", "Next Stage >> ",stage + 1);
		GotoXY(40, 13); printf("└────────────────┘");
		stage += 1;
		Sleep(2000);
		InitScreen();
		Run();
	}
	else
	{
		GotoXY(40, 10); printf("┌────────────────────┐\n");
		GotoXY(40, 11); printf("│%20s│\n", "Success. thank you");
		GotoXY(40, 12); printf("└────────────────────┘");
		
		Sleep(2000);
		
	}
}

void showScore(void) {

}