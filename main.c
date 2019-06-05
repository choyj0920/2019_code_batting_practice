#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include<windows.h>
#pragma warning(disable: 4996)

void clear_menu(); //�ڽ� ���� �Լ�
void title(void);  // Ÿ��Ʋ ���
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

#define MAX_WORD_LENGTH 24 //�ִ� �ܾ� ����
#define MAX_SCREEN_WORD_COUNT 22
#define WORD_COUNT 16 //�ܾ� �޾ƿ� ����

#define COMMAND_SIZE 256  //Ŀ�ǵ� �迭ĭ��

int G2count = 0, G2score[100];

struct ScreenWord  //ȭ�鿡 ǥ���� �ܾ� ����ü
{
	int index;   //�ܾ� ���� ��
	int x;  //��ġ�� x,y 
	int y;
};
typedef struct ScreenWord ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// ȭ�鿡 ��Ÿ�� �ܾ��(g_words�� ���� index ����)
int g_screen_word_count = 0;                    // ȭ�鿡 ��Ÿ�� �ܾ� ����

clock_t g_start_time;                           // ���� �ð�
double g_falling_speed = 2.0;

//int g_word_count = 16;
char **g_words;
int g_fail_count = 0;                           // ������ ����
int g_score = 0;                                // ����
char g_input_word[MAX_WORD_LENGTH + 1];         // �Է� �ܾ� ����
int g_input_word_length = 0;                    // �Է� �ܾ� ����
int stage = 1;                                  // �������� �ܰ� ����



void StartGame2(void);

void showScore(void);
#define CMD_LINES 40  //�ܼ�â �� ��
#define CMD_COLS 100  //�ܼ�â ĭ ��

int main() {
	int choice = 0, menu;
	char command[COMMAND_SIZE] = { '\0', };
	
	sprintf(command, "mode con: lines=%d cols=%d", CMD_LINES, CMD_COLS); //�ܼ�â ũ�� ���� 
	system(command);
	srand(time(NULL)); //���� srand ����


	do {
		title();
		menu = Choice_List(choice);
		switch (menu) {
		case 8:
			break;
		case 10:
			StartGame2();  // Ÿ�� ���� ����
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

//�ڽ� �����Լ�
void clear_menu() { // Ÿ��Ʋ â ��� �Լ�
	int i; //�ݺ��� ���� ��������
	system("cls"); // ȭ���� ����� ó������ ����
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ��� ���ڸ� ���� ������� ����

	for (i = 0; i <= CMD_COLS; i++) { //ù�� ��ȣ ���
		if (i == 0) {
			printf("%c", 1); //���� ��� ��ȣ ���
		}
		else if (i == CMD_COLS) {
			printf("%c", 2); // ������ ��� ��ȣ ���
		}
		else if (i > 1 || i < CMD_COLS) {
			printf("%c", 6); //ù �� ���
		}
	}
	GotoXY(0, 1);
	for (i = 0; i < 22; i++) {
		printf("%c\n", 5);// ���� �׵θ� ���
	}
	printf("%c", 3); // ���� �ϴ� ��ȣ ���

	for (i = 1; i < 24; i++) {
		GotoXY(99, i);
		printf("%c", 5); //������ �׵θ� ���
	}
	printf("%c", 4); //������ �ϴ� ��ȣ ���

	GotoXY(1, 23);
	for (i = 0; i < 98; i++) {
		printf("%c", 6); //�Ʒ��� ���
	}
}

//�޴� ȭ�� ��� �Լ�
void title(void) {
	int i, j;
	
	while (kbhit()) getch();  //���ۿ� �ִ� Ű���� ����
	clear_menu();
	GotoXY(40, 2); printf("��"); for (i = 0; i < 18; i++) printf("��"); printf("��");
	GotoXY(40, 3);  printf("��%18s��"," Batting practice");
	GotoXY(40, 4); printf("��"); for (i = 0; i < 18; i++) printf("��"); printf("��");

	GotoXY(35, 8); printf("1. Classic Batting Practice ");
	GotoXY(35, 10); printf("2.      Batting Game");
	GotoXY(35, 12); printf("3.       add Game");
	GotoXY(35, 14); printf("4.          Score");
	GotoXY(35, 16); printf("5.          use");
	GotoXY(35, 18); printf("6.          Exit ");

}

// �޴� ���� �Լ� 
int Choice_List(int choice)
{
	int i; //
	int y = 8; // ȭ��ǥ�� 1. �� ǥ���Ѵ�

	do { // do-while�� �����ϹǷμ� ȭ��ǥ�� ȭ�鿡 ���� �����Ѵ�
		
		for (i = 8; i < 19; i += 2) {
			GotoXY(31, i); printf(" "); //���� ȭ��ǥ �����
		}
		if (y >= 8 || y <= 18) {
			GotoXY(31, y); //������ ����
			printf("��");  //����Ͽ� ���� ���� ������ Ȯ��
			SetCursorVisible(0);
		}
		choice = getch(); //ȭ��ǥ�� �Է¹���

		if (choice == 80) {       //�Ʒ��� ȭ��ǥ�� �Է¹ް�,
			if (y < 18) {        //���� ȭ��ǥ�� ��ġ�� ���� �Ʒ����� �ƴ϶��
				y += 2;         //ȭ��ǥ�� �Ʒ��� ������
			}
			else if(y == 18)
				y = 8;
			
		}
		else if (choice == 72) { //���� ȭ��ǥ�� �Է¹ް�,
			if (y > 8) {       //���� ȭ��ǥ�� ��ġ�� ���� ������ �ƴ϶��
				y -= 2;        //ȭ��ǥ�� ���� �ø���
			}
			else if (y = 8)
				y = 18;
			
		}
		
	

		else if (choice == 13 || choice == 32) { //�����̽��ٳ� ���͸� �Է¹�����
			choice = y;                    //������ �׸���
			return choice;                 //��ȯ�Ѵ�
		}

	} while (1);
}

//�ǹ� �ִ� �ܾ� ���Ͽ��� �ҷ����� �Լ�
void fileword() {
	int i;
	FILE *fp;
	fopen_s(&fp, "g_words.txt", "rt");
	
	g_words = (char **)malloc(sizeof(char *) * (WORD_COUNT + 1));


	for (i = 0; i < WORD_COUNT; i++)
	{
		g_words[i] = (char **)malloc(sizeof(char *) * (MAX_WORD_LENGTH + 1));  //
		fgets(g_words[i], sizeof(char) * MAX_WORD_LENGTH, fp);  //fp�� �Է��� �ܾ�� gwords�� �Է�
		g_words[i][strlen(g_words[i]) - 1] = '\0'; //gword[i] �� ������ ĭ null�Է�

	}

	g_words[WORD_COUNT] = " ";

	fclose(fp);
	
}

//Ÿ�ڰ��� ���� �Լ�
void StartGame2(void) {
	stage = 1;
	g_falling_speed = 2.0;
	g_fail_count = 0;                           // ������ ����


	g_start_time = clock(); //���� �ð� ���
	fileword();     //file���� �ܾ� �����ͼ� g_word�� ��� 
	InitScreen();  // ���� UI���
	Run(); //���ӽ��� ���Լ�

}

// Ÿ�� ���� Ʋ ��� �Լ�
void InitScreen(void)
{
	int i;

	clear_menu();

	// 
	GotoXY(0, 24);
	printf("����������������������������\n");
	printf("��%12s��\n","Batting Game");
	printf("����������������������������");

	// �Է�ĭ
	GotoXY(28, 24); printf("����������������������������������");
	GotoXY(22, 25); printf("�Է�ĭ��%15s��","");
	GotoXY(28, 26); printf("����������������������������������");

	//score ĭ
	GotoXY(50, 24); printf("������������");
	GotoXY(46, 25); printf("������%4d��",g_score);
	GotoXY(50, 26); printf("������������");

	//fail ĭ
	GotoXY(68, 24); printf("����������");
	GotoXY(59, 25); printf("���� Ƚ����%3d��", g_fail_count);
	GotoXY(68, 26); printf("����������");
}


void Run() {
	int i, count, key; // i�� �ܾ� 19���� ��������� , count ��  , key����� �Է°�
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
		if (GetDiffTime(g_start_time) > g_falling_speed) // ���� �ð��� g_falling_speed ���� ������
		{
			for (i = 0; i < count; i++) 
			{
				EraseWord(i);  //i��ȣ �ܾ� ��ũ������ �����
				g_screen_word[i].y++; //�ܾ� .y���� 1�߰�
				if (g_screen_word[i].y == 23)  //�ܾ��� y���� 23�϶� 
				{
					if (g_screen_word[i].index != WORD_COUNT) //�� �ܾ  ���� ���� ���� ��(�Էµ��� �ʾ��� ��) 
					{
						g_fail_count++; //����Ƚ�� 1�߰� 
						UpdateFailCount(); //����Ƚ�� ��� 
						NextStage(); //
					}
					printwordset(i);  // ���ο� �ܾ� ��� 
					g_screen_word[i].y++;// �ܾ� y 1 ����
					DrawWord(i); //�ܾ� ȭ�� ���

				}
				else
					DrawWord(i);
			}
			if (count < MAX_SCREEN_WORD_COUNT)
				count++; //count
			g_start_time = clock(); //�ð� ����
		}
	}
}

//������ �ܾ�  ���� ������ġ[x]�� ������ ����, ������ ������������ y=1
void printwordset(int i) {
	g_screen_word[i].index = rand() % (WORD_COUNT);		//����� �ܾ� �ε��� ���� ���� 0~(WORD_COUNT-1)
	g_screen_word[i].x = rand() % 77 + 1;               // �ܾ� ��µ� X�� ����
	g_screen_word[i].y = 1;						//�ܾ� ��� ������ ��ġ Y=1 ����

	if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) > 78) { // 
		while (1) {
			
			g_screen_word[i].x = rand() % 77 + 1;
			if(g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) < 78)
				break;

		
		}
	}
	
}

// �Է� ó�� �Լ�
void ProcessInput(int key)
{

	if (key == KEY_ESC) {
		G2score[G2count] = stage;
		G2count++;
		stage = 0;
		StageScreen();
	}
	else if (key == KEY_BS)  //�Է°��� �齺���̽����
	{
		if (g_input_word_length != 0)  //���� �Է°��� 0�� �ƴ϶�� 
		{
			GotoXY(30 + g_input_word_length -1 , 25); //�Է°� ������-1 ��ġ�� 
			g_input_word[g_input_word_length-1] = NULL; //�Է°� ������ִ� �� ������ ���� ����
			printf(" ");   //�Է¶� �������� ������ �� ����
			g_input_word_length -= 1;
			

		}
	}
	else if (key != KEY_ENTER) //�Է°��� ����,�����̽� �� �ƴϸ�
	{
		if (g_input_word_length != 24)  //���� �Է��� ���� 24�ڸ� ���� ������
		{
			GotoXY(30 + g_input_word_length, 25);
			printf("%c", key);  //�Է�ĭ �������� ���
			g_input_word[g_input_word_length] = key;
			g_input_word_length++;  //�Է��� ���� +1
		}
	}
	else  //�����Է�
	{
		int count;  
		CompareWords(); 
		GotoXY(30, 25);
		for (count = 0; count < strlen(g_input_word); count++)
		{
			printf(" ");    //���� �Է��� �ڸ�����ŭ ���� ���
		}
		for (count = 0; count < g_input_word_length; count++)
		{
			g_input_word[count] = NULL;  //���� �Է��� �ڸ�����ŭ input_word �ʱ�ȭ
		}
		g_input_word_length = 0;  // �Է��Ѱ� 0���� �ٲ�
	}
	NextStage();  //���� ���� Ƚ���� ����  ���� ���������� 


}

//�Է´ܾ�� ��ũ���ܾ� �� �Լ�
void CompareWords() {
	int i;
	int Del = -1;

	for (i = 0; i < 22; i++) //19���� ����Ǿ��ִ� �ܾ� Ž�� 
	{
		if (strcmp(g_input_word, g_words[g_screen_word[i].index]) == 0) // ȭ�鿡 �� �ܾ� �߿� ��ġ�ϴ� ���� ������
		{
			if (Del != -1) //Del�� -1�� �ƴ� ��� �̹� �ݺ� ���� ��ġ�ϴ°� �־��� ��
				if (g_screen_word[i].y > g_screen_word[Del].y) //�� �ܾ ��ġ������ �� �Ʒ�������
					Del = i;   //del�� �̹� i�� 
			if (Del == -1)
				Del = i;  //del�� �̹� i�� 
		}
	}
	if (Del != -1)  //�ݺ����� ������ ��ġ�ϴ� ���� �־��� ��
	{
		EraseWord(Del);  //�ش� �ܾ� ȭ�鿡�� ����
		g_screen_word[Del].index = WORD_COUNT;  //�ε��� �� �ʱ�ȭ
		g_score++;   //���� ++
		UpdateScore();  //
	}
	else {
		g_fail_count++;  //�߸� �Է½� ����Ƚ�� 1�߰�
		UpdateFailCount(); //����Ƚ�� ��� 
		NextStage(); //
	}
}
//���� ���� Ƚ���� ����  ó���Ǵ� �Լ�
void NextStage()  
{
	if (g_fail_count == 5) {  //������ Ƚ���� 5�� ���� ���� ��� 
		system("cls");
		g_score = 0;    //���� 0 �ʱ����
		g_fail_count = 0;  //���� Ƚ��
		GotoXY(0, 10);
		printf("%12s����������������������������\n"," ");
		printf("%12s��%12s��\n", "","    ����..");
		printf("%12s����������������������������"," ");

		Sleep(2000);
		G2score[G2count] = stage;
		G2count++;
		
		stage = 0;
	}
	if (g_score == 8) //���� �� 8���̸�
	{
		g_falling_speed *= 0.8;  //���� �ܰ� ���̵� ����
		g_score = 0;    //���� 0 �ʱ����
		g_fail_count = 0;  //���� Ƚ�� 
		UpdateScore();     //���� �ٽ� ���
		UpdateFailCount(); //��Ƚ �ٽ� ���
		StageScreen();  //���� �������� ���
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
		GotoXY(40, 10); printf("������������������������������������");
		GotoXY(40, 11); printf("��%3d%9s��", stage," Stage Clear!");
		GotoXY(40, 12); printf("��%15s%1d��", "Next Stage >> ",stage + 1);
		GotoXY(40, 13); printf("������������������������������������");
		stage += 1;
		Sleep(2000);
		InitScreen();
		Run();
	}
	else
	{
		GotoXY(40, 10); printf("��������������������������������������������\n");
		GotoXY(40, 11); printf("��%20s��\n", "Success. thank you");
		GotoXY(40, 12); printf("��������������������������������������������");
		
		Sleep(2000);
		
	}
}

void showScore(void) {

}