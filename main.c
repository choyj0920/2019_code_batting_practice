#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include<windows.h>		
#pragma warning(disable: 4996) // ���� ���� ����

void clear_menu(); //�ڽ� ���� �Լ�
void title(void);  // Ÿ��Ʋ ���
int Choice_List(int);
void InitScreen1(void);
void InitScreen2(void);
void printwordset(int i);
void Run1();
void Run2();
void DrawWord1(int i);
void DrawWord2(int i);
void EraseWord(int i);
void UpdateScore();
void UpdateFailCount();
void StageScreen1(int);
void StageScreen2();
int snakegame();
int Choice_Game1(int choice);

void ProcessInput1(int, int *);
void ProcessInput2(int);
void CompareWords1(int *i);
void CompareWords2();
void NextStage1();
void NextStage2();
void StartCountDown();
void check_success();

void TP_print();
void rand_TP_answer();
void battinggame();

void Run_TP();
void Process_TP(int key);



#define KEY_ESC 27
#define KEY_ENTER '\r'
#define KEY_BS '\b'

#define MAX_WORD_LENGTH 24 //�ִ� �ܾ� ����
#define MAX_SCREEN_WORD_COUNT 22
#define WORD_COUNT 119 //�ܾ� �޾ƿ� ����

#define COMMAND_SIZE 256  //Ŀ�ǵ� �迭ĭ��

int G2count = 0, G2score[100];

struct ScreenWord  //ȭ�鿡 ǥ���� �ܾ� ����ü
{
	int index;   //�ܾ� ���� ��
	int x;  //��ġ�� x,y 
	int y;
	int length;
	char *str;
};
typedef struct ScreenWord ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// ȭ�鿡 ��Ÿ�� �ܾ��(g_words�� ���� index ����)
int g_screen_word_count = 0;                    // ȭ�鿡 ��Ÿ�� �ܾ� ����

clock_t g_start_time;                           // ���� �ð�
double g_falling_speed = 2.0;
double g_change_time = 9.0;

char **g_words;
int g_fail_count = 0;                           // ������ ����
int g_score = 0;                                // ����
int *pg = &g_score;
char g_input_word[MAX_WORD_LENGTH + 1];         // �Է� �ܾ� ����
int g_input_word_length = 0;                    // �Է� �ܾ� ����
int stage = 1;                                  // �������� �ܰ� ����
int menu;

//�� �� ����
char typing_answer[200];  //������ �������� ����
int typ_count; //�Է��� ����
char typing_user[200]; //������ �Է°��� ���� 
int finish_TP;

void StartGame1(void);
void StartGame2(void);
void calculatespeed();

void showScore(void);
#define CMD_LINES 40  //�ܼ�â �� ��
#define CMD_COLS 100  //�ܼ�â ĭ ��

int main() {
	int choice = 0;
	char command[COMMAND_SIZE] = { '\0', };

	sprintf(command, "mode con: lines=%d cols=%d", CMD_LINES, CMD_COLS); //�ܼ�â ũ�� ���� 
	system(command);
	srand((unsigned)time(NULL)); //���� srand ����


	do {
		title();
		menu = Choice_List(choice);
		switch (menu) {
		case 8:
			StartGame1();
			break;
		case 10:
			StartGame2();  // Ÿ�� ���� ����
			break;
		case 12:
			battinggame();

			break;
		case 14:
			snakegame();

			break;
		case 16:
		
			break;

		}
	} while (menu != 16);

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
	int i;

	while (kbhit()) getch();  //���ۿ� �ִ� Ű���� ����
	clear_menu();
	GotoXY(40, 2); printf("��"); for (i = 0; i < 18; i++) printf("��"); printf("��");
	GotoXY(40, 3);  printf("��%18s��", " Batting practice");
	GotoXY(40, 4); printf("��"); for (i = 0; i < 18; i++) printf("��"); printf("��");

	GotoXY(35, 8); printf("1.    Batting Practice ");
	GotoXY(35, 10); printf("2.      Batting Game");
	GotoXY(35, 12); printf("3.       add Game");
	GotoXY(35, 14); printf("4.         use");
	GotoXY(35, 16); printf("5.         Exit ");

}

// �޴� ���� �Լ� 
int Choice_List(int choice)
{

	int i; //
	int y = 8; // ȭ��ǥ�� 1. �� ǥ���Ѵ�

	do { // do-while�� �����ϹǷμ� ȭ��ǥ�� ȭ�鿡 ���� �����Ѵ�
		switch (y) {
		case 8:
			GotoXY(40, 3);  printf("��%18s��", "  Batting practice");
			break;

		case 10:
			GotoXY(40, 3);  printf("��%18s��", "   Batting Game ");
			break;
		case 12:
			GotoXY(40, 3);  printf("��%18s��", "   add Game    ");
			break;
		case 14:
			GotoXY(40, 3);  printf("��%18s��", "    use      ");
			break;
		case 16:
			GotoXY(40, 3);  printf("��%18s��", "    EXIT       ");
			break;
		

		}

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
			else if (y == 16)
				y = 8;

		}
		else if (choice == 72) { //���� ȭ��ǥ�� �Է¹ް�,
			if (y > 8) {       //���� ȭ��ǥ�� ��ġ�� ���� ������ �ƴ϶��
				y -= 2;        //ȭ��ǥ�� ���� �ø���
			}
			else if (y = 8)
				y = 16;

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

void StartGame1(void) {
	int choice = 0;
	stage = 1;
	clear_menu();

	GotoXY(35, 12); printf("3.         Hard");
	GotoXY(35, 10); printf("2.     Intermediate");
	GotoXY(35, 8); printf("1.         Easy ");

	while (kbhit()) getch();  //���ۿ� �ִ� Ű���� ����

	int i = Choice_Game1(choice);
	switch (i) {
	case 8:
		g_change_time = 9.0;
		break;
	case 10:
		g_change_time = 6.0;
	case 12:
		g_change_time = 3.0;
		break;
	}
	StartCountDown();
	stage = 1;
	g_fail_count = 0;


	srand((unsigned)time(NULL));
	fileword();
	InitScreen1();
	Run1();

}

int Choice_Game1(int choice)
{

	int i; //
	int y = 8; // ȭ��ǥ�� 1. �� ǥ���Ѵ�

	do { // do-while�� �����ϹǷμ� ȭ��ǥ�� ȭ�鿡 ���� �����Ѵ�
		switch (y) {
		case 8:
			GotoXY(40, 3);  printf("��%18s��", "  Easy");
			break;

		case 10:
			GotoXY(40, 3);  printf("��%18s��", "   Intermediate ");
			break;
		case 12:
			GotoXY(40, 3);  printf("��%18s��", "   Hard    ");
			break;
	
		}

		for (i = 8; i < 19; i += 2) {
			GotoXY(31, i); printf(" "); //���� ȭ��ǥ �����
		}
		if (y >= 8 || y <= 12) {
			GotoXY(31, y); //������ ����
			printf("��");  //����Ͽ� ���� ���� ������ Ȯ��
			SetCursorVisible(0);
		}
		choice = getch(); //ȭ��ǥ�� �Է¹���

		if (choice == 80) {       //�Ʒ��� ȭ��ǥ�� �Է¹ް�,
			if (y < 12) {        //���� ȭ��ǥ�� ��ġ�� ���� �Ʒ����� �ƴ϶��
				y += 2;         //ȭ��ǥ�� �Ʒ��� ������
			}
			else if (y == 12)
				y = 8;

		}
		else if (choice == 72) { //���� ȭ��ǥ�� �Է¹ް�,
			if (y > 8) {       //���� ȭ��ǥ�� ��ġ�� ���� ������ �ƴ϶��
				y -= 2;        //ȭ��ǥ�� ���� �ø���
			}
			else if (y = 8)
				y = 12;

		}


		else if (choice == 13 || choice == 32) { //�����̽��ٳ� ���͸� �Է¹�����
			choice = y;                    //������ �׸���
			return choice;                 //��ȯ�Ѵ�
		}

	} while (1);
}

void InitScreen1(void) {

	clear_menu();

	// 
	GotoXY(0, 24);
	printf("��������������������������������������\n");
	printf("��%12s��\n", " Batting Practice");
	printf("��������������������������������������");

	GotoXY(28, 24); printf("����������������������������������");
	GotoXY(22, 25); printf("�Է�ĭ��%15s��", "");
	GotoXY(28, 26); printf("����������������������������������");

	//score ĭ
	GotoXY(50, 24); printf("������������");
	GotoXY(46, 25); printf("������%4d��", g_score);
	GotoXY(50, 26); printf("������������");

	//fail ĭ
	GotoXY(68, 24); printf("����������");
	GotoXY(59, 25); printf("���� Ƚ����%3d��", g_fail_count);
	GotoXY(68, 26); printf("����������");
}

void Run1() {
	int i, count, key;
	count = 1;
	for (i = 0; i < WORD_COUNT; i++) {
		g_screen_word[i].index = rand() % (WORD_COUNT);
	}
	i = 0;

	g_start_time = (unsigned)time(0);
	while (g_change_time != 0) {
		if (stage == 0)
			break;
		DrawWord1(i);
		*pc = 0;
		while (time(0) < g_start_time + g_change_time) {
			if (kbhit()) {
				key = getch();
				ProcessInput1(key, &i);
			}
			if (correct == 1) break;
		}
		while (kbhit()) getch();
		g_start_time = (unsigned)time(0); //�ð� ����
		i++;
		check_success();
		NextStage1();
		if (i > 119) break;
	}

}

void ProcessInput1(int key, int *i) {


	if (key == KEY_ESC) {
		G2score[G2count] = g_score; //�̱���
		G2count++;
		stage = 0;
	}
	else if (key == KEY_BS)  //�Է°��� �齺���̽����
	{
		if (g_input_word_length != 0)  //���� �Է°��� 0�� �ƴ϶�� 
		{
			GotoXY(30 + g_input_word_length - 1, 25); //�Է°� ������-1 ��ġ�� 
			g_input_word[g_input_word_length - 1] = NULL; //�Է°� ������ִ� �� ������ ���� ����
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
	else
	{
		int count;
		CompareWords1(i);
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
		g_start_time = (unsigned)time(0);
	}
}

void CompareWords1(int *i) {

	if (strcmp(g_input_word, g_words[g_screen_word[*i].index]) == 0) // ȭ�鿡 �� �ܾ� �߿� ��ġ�ϴ� ���� ������
	{

		(*pg)++;   //���� ++
		UpdateScore();  //
		(*i)++;
		*pc = 1;

	}
	else {
		g_fail_count++;  //�߸� �Է½� ����Ƚ�� 1�߰�
		UpdateFailCount(); //����Ƚ�� ��� 
		NextStage1(); //
	}
}

void NextStage1()
{
	if (g_fail_count == 5) {  //������ Ƚ���� 5�� ���� ���� ��� 
		system("cls");		
		GotoXY(0, 10);
		printf("%12s����������������������������\n", " ");
		printf("%12s��%12s��\n", "", "    ����..");
		printf("%12s����������������������������", " ");

		Sleep(2000);
		G2score[G2count] = g_score;
		G2count++;
		g_score = 0;    //���� 0 �ʱ�ȭ"
		g_fail_count = 0;  //���� Ƚ��

		stage = 0;
	}
	if (g_score == 8) //���� �� 8���̸�
	{
		g_change_time *= 0.8;  //���� �ܰ� ���̵� ����
		G2score[G2count] = g_score;
		G2count++;
		g_score = 0;    //���� 0 �ʱ�ȭ
		g_fail_count = 0;  //���� Ƚ�� 
		UpdateScore();     //���� �ٽ� ���
		UpdateFailCount(); //��Ƚ �ٽ� ���
		StageScreen1(menu);  //���� �������� ���
	}
	if (g_score == 0) return;
}

//Ÿ�ڰ��� ���� �Լ�
void StartGame2(void) {
	
	void StartCountDown();
	stage = 1;
	g_falling_speed = 2.0;
	g_fail_count = 0;                           // ������ ����


	g_start_time = clock(); //���� �ð� ���
	fileword();     //file���� �ܾ� �����ͼ� g_word�� ��� 
	InitScreen2();  // ���� UI���
	Run2(); //���ӽ��� ���Լ�

}

// Ÿ�� ���� Ʋ ��� �Լ�
void InitScreen2(void)
{	

	clear_menu();

	// 
	GotoXY(0, 24);
	printf("����������������������������\n");
	printf("��%12s��\n", "Batting Game");
	printf("����������������������������");

	// �Է�ĭ
	GotoXY(28, 24); printf("����������������������������������");
	GotoXY(22, 25); printf("�Է�ĭ��%15s��", "");
	GotoXY(28, 26); printf("����������������������������������");

	//score ĭ
	GotoXY(50, 24); printf("������������");
	GotoXY(46, 25); printf("������%4d��", g_score);
	GotoXY(50, 26); printf("������������");

	//fail ĭ
	GotoXY(68, 24); printf("����������");
	GotoXY(59, 25); printf("���� Ƚ����%3d��", g_fail_count);
	GotoXY(68, 26); printf("����������");
}

//�� ���� �Լ�
void Run2() {
	int i, count, key; // i�� �ܾ� 19���� ��������� , count ��  , key����� �Է°�
	count = 1;

	for (i = 0; i < 22; i++)
	{
		printwordset(i);

	}
	while (stage != 0) {
		if (_kbhit()) {
			key = _getch();
			ProcessInput2(key);
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
						NextStage2(); //
					}
					printwordset(i);  // ���ο� �ܾ� ��� 
					g_screen_word[i].y++;// �ܾ� y 1 ����
					DrawWord2(i); //�ܾ� ȭ�� ���

				}
				else
					DrawWord2(i);
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
			if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) < 78)
				break;


		}
	}

}

// �Է� ó�� �Լ�
void ProcessInput2(int key)
{

	if (key == KEY_ESC) {
		G2score[G2count] = stage;
		G2count++;
		stage = 0;
		StageScreen2();
	}
	else if (key == KEY_BS)  //�Է°��� �齺���̽����
	{
		if (g_input_word_length != 0)  //���� �Է°��� 0�� �ƴ϶�� 
		{
			GotoXY(30 + g_input_word_length - 1, 25); //�Է°� ������-1 ��ġ�� 
			g_input_word[g_input_word_length - 1] = NULL; //�Է°� ������ִ� �� ������ ���� ����
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
		CompareWords2();
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
	NextStage2();  //���� ���� Ƚ���� ����  ���� ���������� 


}

//�Է´ܾ�� ��ũ���ܾ� �� �Լ�
void CompareWords2() {
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
		UpdateScore();
	}
	else {
		g_fail_count++;  //�߸� �Է½� ����Ƚ�� 1�߰�
		UpdateFailCount(); //����Ƚ�� ��� 
		NextStage2();
	}
}
//���� ���� Ƚ���� ����  ó���Ǵ� �Լ�
void NextStage2()
{
	if (g_fail_count == 5) {  //������ Ƚ���� 5�� ���� ���� ��� 
		system("cls");
		g_score = 0;    //���� 0 �ʱ�ȭ
		g_fail_count = 0;  //���� Ƚ��
		GotoXY(0, 10);
		printf("%12s����������������������������\n", " ");
		printf("%12s��%12s��\n", "", "    ����..");
		printf("%12s����������������������������", " ");

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
		StageScreen2();  //���� �������� ���
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

void DrawWord1(int i)
{	
	if (i != 0) {
		GotoXY(40, 8);
		printf("%20s", " ");
	}
	GotoXY(40, 8);
	printf("%s", g_words[g_screen_word[i].index]);
}

void DrawWord2(int i)
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

void StageScreen1(int menu)
{
	system("cls");


	if (stage > 0)
	{
		GotoXY(40, 10); printf("������������������������������������");
		GotoXY(40, 11); printf("��%3d%9s��", stage, " Stage Clear!");
		GotoXY(40, 12); printf("��%15s%1d��", "Next Stage >> ", stage + 1);
		GotoXY(40, 13); printf("������������������������������������");
		stage += 1;
		Sleep(2000);
		system("cls");
		clear_menu();
		InitScreen1();
		Run1();
	}
}

//���� �������� ȭ�� ���,���� ȭ�� 
void StageScreen2(void)
{
	system("cls");

	if (stage > 0)
	{
		GotoXY(40, 10); printf("������������������������������������");
		GotoXY(40, 11); printf("��%3d%9s��", stage, " Stage Clear!");
		GotoXY(40, 12); printf("��%15s%1d��", "Next Stage >> ", stage + 1);
		GotoXY(40, 13); printf("������������������������������������");
		stage += 1;
		Sleep(2000);
		InitScreen2();
		Run2();
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

// TP ���� x`���� �����Լ�
void battinggame() {
	StartCountDown();
	finish_TP = 0;
	g3_count = 0;
	rand_TP_answer();
	typ_count = 0; //�Է��� ����
	g_start_time = clock(); //���� �ð� ���
	clear_menu();
	start = (unsigned)time(0);
	Run_TP(); 
	calculatespeed();
	Sleep(3000);

}

//�������� ����  ���
void rand_TP_answer() {
	int i;
	for (i = 0; i < 100; i++) {
		typing_answer[i] = rand() % ('z' - 'a' + 1) + 'a';
	}
}

// TP_��� �Լ�
void TP_print() {
	int i = 0, j;
	for (i; i < 100; i++) {
		j = i / 50;
		GotoXY(25 + i % 50, 3 * j + 2);
		SetCursorVisible(0);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ��� ���ڸ� ���� ������� ����
		if (typ_count > i && typing_answer[i] != typing_user[i]) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // ���ڸ� ������ ����;
			printf("\a");
		}
		printf("%c", typing_answer[i]);
		if (i < typ_count) {
			GotoXY(25 + i % 50, 3 * j + 3);
			printf("%c", typing_user[i]);


		}
	}
}
//TP���Լ�
void Run_TP() {
	int count = 0, key;

	while (finish_TP == 0) {
		if (_kbhit()) {
			key = getch();
			Process_TP(key);

		};
		TP_print();
		if (typ_count == 100)
			finish_TP = 2;
		
		calculatespeed();


	}

	
}

//TP�Է� ó��
void Process_TP(int key)
{

	if (key == KEY_ESC) {
		finish_TP = 1;

	}
	else if (key == KEY_BS)  //�Է°��� �齺���̽����
	{
		if (typ_count != 0)  //���� �Է°��� 0�� �ƴ϶�� 
		{
			GotoXY(25 + (typ_count - 1) % 50, 3 * ((typ_count - 1) / 50) + 3); //�Է°� ������-1 ��ġ�� 
			typing_user[typ_count - 1] = NULL; //�Է°� ������ִ� �� ������ ���� ����
			printf("  ");   //�Է¶� �������� ������ �� ����
			typ_count -= 1;


		}
	}
	else if (key != KEY_ENTER) //�Է°��� ����,�齺���̽� �� �ƴϸ�
	{
		GotoXY(26 + (typ_count - 1) % 50, 3 * ((typ_count - 1) / 50) + 3);

		typing_user[typ_count] = key;
		typ_count++;  //�Է��� ���� +1

	}
	else  //�����Է�
	{

	}


}


void calculatespeed()
{
	end = (unsigned)time(0);

	if (typ_count==0)
		return 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ��� ���ڸ� ���� ������� ����
	g3_count=0;
	for (int i = 0; i < typ_count; i++) {
		if (typing_answer[i] == typing_user[i])
			g3_count++;
	}
	timespent = end - start;
	GotoXY(28, 24);
	printf("\n  ��Ȯ��: %d %% \n", g3_count * 100 / typ_count);
	GotoXY(28, 25);
	printf("Ÿ��: %.0lf", typ_count * 60 / timespent);
	GotoXY(48, 25);
	printf("��ȿŸ��: %.0lf\n", g3_count * 60 / timespent);
}

void StartCountDown() {

	for (int i = 3; i > 0; i--) {
		system("cls");
		GotoXY(40, 8);
		printf("%d�� �� ���۵˴ϴ�!", i);
		Sleep(1000);
	}
}

void check_success()
{
	if (g_change_time < 2)
	{
		GotoXY(40, 10); printf("��������������������������������������������\n");
		GotoXY(40, 11); printf("��%20s��\n", "Success. thank you");
		GotoXY(40, 12); printf("��������������������������������������������");

		Sleep(2000);

	}
}
