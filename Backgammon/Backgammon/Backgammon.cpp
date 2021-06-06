#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <time.h>
#include <random>
#pragma warning(disable : 4996).


void MenuPrint(void);
void ClearScreen(void);
void MenuProcess(void);
void NewGame(void);
void ContinueGame(void);
void GamePrint(void);
void FirstDice(void);
void PrintOrderOfPlay(void);
void WriteDiceToFile(int, int);
void AutoDice(void);
void GameProcess(void);
int GameControls(char[4]);
void GameSave(char[4], int, char, char);
void ChangeOrderOfPlay(char, int, int);
void ChangeSourceRow(int, int, int, char);
void ChangeDestinationRow(int, int, int, char, char);
void WriteBrokenToFile(char);

int main()
{
	MenuPrint();
	MenuProcess();

	system("pause");
}

void MenuPrint(void)
{
	ClearScreen();
	printf("Please enter a number. Press the Enter button.\n");
	printf("1. New game\n");
	printf("2. Continue game\n");
	printf("3. End game\n");
}

void ClearScreen(void)
{
	fprintf(stdout, "\033[2J"); // clean screen
	fprintf(stdout, "\033[1;1H"); // move cursor to the first line
}

void MenuProcess(void)
{
	int menuSelected;
	scanf("%d", &menuSelected);

	switch (menuSelected)
	{
	case 1:
		printf("1. New game\n");
		NewGame();
		break;
	case 2:
		printf("2. Continue game\n");
		ContinueGame();
		break;
	case 3:
		printf("3. End game\n");
		system("Exit");
		break;
	default:
		printf("Wrong choice\n");
		MenuProcess();
		break;
	}
}

void NewGame(void)
{
	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "w+");
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 24; j++)
		{
			if (i == 1 && j == 1)
			{
				fprintf(fpGameFile, "5Y");
				j += 1;
			}
			else if (i == 1 && j == 9)
			{
				fprintf(fpGameFile, "3X");
				j += 1;
			}
			else if (i == 1 && j == 13)
			{
				fprintf(fpGameFile, "5X");
				j += 1;
			}
			else if (i == 1 && j == 23)
			{
				fprintf(fpGameFile, "2Y");
				j += 1;
			}
			else if (i == 3 && j == 9)
			{
				fprintf(fpGameFile, "0X");
				j += 1;
			}
			else if (i == 3 && j == 11)
			{
				fprintf(fpGameFile, "0 ");
				j += 1;
			}
			else if (i == 3 && j == 13)
			{
				fprintf(fpGameFile, "0 ");
				j += 1;
			}
			else if (i == 3 && j == 15)
			{
				fprintf(fpGameFile, "0Y");
				j += 1;
			}
			else if (i == 5 && j == 1)
			{
				fprintf(fpGameFile, "5X");
				j += 1;
			}
			else if (i == 5 && j == 9)
			{
				fprintf(fpGameFile, "3Y");
				j += 1;
			}
			else if (i == 5 && j == 13)
			{
				fprintf(fpGameFile, "5Y");
				j += 1;
			}
			else if (i == 5 && j == 23)
			{
				fprintf(fpGameFile, "2X");
				j += 1;
			}
			else
			{
				fprintf(fpGameFile, " ");
			}

		}
		fprintf(fpGameFile, "\n");
	}

	fclose(fpGameFile);

	FILE* fpDiceHistoryFile;
	fpDiceHistoryFile = fopen("DiceHistory.dat", "w+");
	fclose(fpDiceHistoryFile);

	AutoDice();
	GamePrint();
	FirstDice();
	PrintOrderOfPlay();
	GameProcess();
}

void ContinueGame(void)
{
	GamePrint();
	PrintOrderOfPlay();
	GameProcess();
}

void GamePrint(void)
{
	ClearScreen();
	FILE* fpGameFile;

	fpGameFile = fopen("Table.dat", "r");
	fseek(fpGameFile, 0, SEEK_SET);
	char readGameRow[25];

	for (int i = 1; i <= 5; i++)
	{
		printf("%d ", i);
		fgets(readGameRow, 25, fpGameFile);
		puts(readGameRow);
		fseek(fpGameFile, sizeof(char), SEEK_CUR);
		fseek(fpGameFile, sizeof(char), SEEK_CUR);
	}
	printf("  ");
	for (char j = 'A'; j < 'M'; j++)
	{
		printf("%c ", j);
	}
	printf("\n");

	fclose(fpGameFile);

	printf("Dice rolled automatically\n");


}

void FirstDice(void)
{
	srand((unsigned)time(0));
	int XDice = (rand() % 6) + 1;
	srand((unsigned)time(0));
	int YDice = (rand() % 6) + 1;

	while (XDice == YDice)
	{
		srand((unsigned)time(0));
		YDice = (rand() % 6) + 1;
	}
	FILE* fpDiceHistoryFile;
	fpDiceHistoryFile = fopen("DiceHistory.dat", "a+");
	fprintf(fpDiceHistoryFile, "X %d", XDice);
	fprintf(fpDiceHistoryFile, "\n");
	fprintf(fpDiceHistoryFile, "Y %d", YDice);
	fprintf(fpDiceHistoryFile, "\n");
	fclose(fpDiceHistoryFile);

	FILE* fpOrderOfPlayFile;
	fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "w+");
	if (XDice > YDice)
	{
		//player|dice|NumberOfGame 
		fprintf(fpOrderOfPlayFile, "X11");
		printf("First player X\n");
	}
	else
	{
		//player|dice|NumberOfGame 
		fprintf(fpOrderOfPlayFile, "Y11");
		printf("First player Y\n");
	}
	fprintf(fpOrderOfPlayFile, "\n");
	fclose(fpOrderOfPlayFile);

}

void PrintOrderOfPlay(void)
{
	FILE* fpOrderOfPlayFile;
	fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "r+");
	char readOrderOfPlay[5];
	fgets(readOrderOfPlay, 5, fpOrderOfPlayFile);
	printf("Player %c plays for the number %c dice. Number of game : %c \n", readOrderOfPlay[0], readOrderOfPlay[1], readOrderOfPlay[2]);
	fclose(fpOrderOfPlayFile);
	printf("Please enter your command. For Example E5A1 \n");
}

void WriteDiceToFile(int firstDice, int secondDice)
{
	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "r+");

	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, 62, SEEK_CUR);
	fprintf(fpGameFile, "%d ", firstDice);
	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, 64, SEEK_CUR);
	fprintf(fpGameFile, "%d ", secondDice);
	fclose(fpGameFile);

	FILE* fpOrderOfPlayFile;
	fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "r+");
	char readOrderOfPlay[5];
	fgets(readOrderOfPlay, 5, fpOrderOfPlayFile);
	fclose(fpOrderOfPlayFile);

	FILE* fpDiceHistoryFile;
	fpDiceHistoryFile = fopen("DiceHistory.dat", "a+");
	fprintf(fpDiceHistoryFile, "%c %d %d", readOrderOfPlay[0], firstDice, secondDice);
	fprintf(fpDiceHistoryFile, "\n");
	fclose(fpDiceHistoryFile);

}

void AutoDice()
{
	srand((unsigned)time(0));
	int firstDice = (rand() % 6) + 1;
	int secondDice = (rand() % 6) + 1;
	WriteDiceToFile(firstDice, secondDice);
}

void GameProcess(void)
{
	char enteredValue[4];
	scanf("%s", enteredValue);
	int isSuccessGameControls = GameControls(enteredValue);

	if (isSuccessGameControls == 0)
	{
		printf("Continue press enter\n");
		system("pause");
		GamePrint();
		PrintOrderOfPlay();
		GameProcess();

	}
	else
	{

		int firstDice;
		int secondDice;
		char counter = '1';

		FILE* fpGameFile;
		fpGameFile = fopen("Table.dat", "r+");
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 62, SEEK_CUR);
		fscanf(fpGameFile, "%d", &firstDice);
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 64, SEEK_CUR);
		fscanf(fpGameFile, "%d", &secondDice);
		fclose(fpGameFile);

		// zar çift ise 2 oynama hakkı ver.
		if (firstDice == secondDice)
		{
			counter = '2';
		}

		//player|dice|NumberOfGame 
		FILE* fpOrderOfPlayFile;
		fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "r+");
		char readOrderOfPlay[5];
		fgets(readOrderOfPlay, 5, fpOrderOfPlayFile);
		fclose(fpOrderOfPlayFile);

		char player = (char)readOrderOfPlay[0];
		char playerDice = readOrderOfPlay[1];
		char numberOfGame = readOrderOfPlay[2];
		char otherPlayer = ' ';

		switch (player)
		{
		case 'X':
			otherPlayer = 'Y';
			break;
		case 'Y':
			otherPlayer = 'X';
			break;
		}



		// oyuncu zarı 1 oyun 1 ise; 
		if (playerDice == '1' && numberOfGame == '1')
		{
			//| Counter ile NumberOfGame eşit ise playerDice arttır |
			if (counter == numberOfGame)
			{
				GameSave(enteredValue, firstDice, player, otherPlayer);
				ChangeOrderOfPlay(player, 2, 1);
			}
			else //| Counter ile NumberOfGame eşit değil ise NumberOfGame arttır |
			{
				GameSave(enteredValue, firstDice, player, otherPlayer);
				ChangeOrderOfPlay(player, 1, 2);
			}

		}
		// oyuncu zarı 1 oyun 2 ise; 
		if (playerDice == '1' && numberOfGame == '2')
		{
			//| Counter ile NumberOfGame eşit ise playerDice arttır ve NumberOfGame değerini 1 yap |
			if (counter == numberOfGame)
			{
				GameSave(enteredValue, secondDice, player, otherPlayer);
				ChangeOrderOfPlay(player, 2, 1);
			}
			else //| Counter ile NumberOfGame eşit değil ise hata vardır |
			{
				printf("ERROR");
			}
		}
		// oyuncu zarı 2 oyun 1 ise; 
		if (playerDice == '2' && numberOfGame == '1')
		{
			//| Counter ile NumberOfGame eşit ise oyuncuyu değiştir ve zar at | 
			if (counter == numberOfGame)
			{
				GameSave(enteredValue, secondDice, player, otherPlayer);
				ChangeOrderOfPlay(otherPlayer, 1, 1);
				AutoDice();
			}
			else //| Counter ile NumberOfGame eşit değil ise NumberOfGame arttır |
			{
				GameSave(enteredValue, secondDice, player, otherPlayer);
				ChangeOrderOfPlay(player, 2, 2);
			}
		}
		// oyuncu zarı 2 oyun 2 ise; 
		if (playerDice == '2' && numberOfGame == '2')
		{
			//| Counter ile NumberOfGame eşit ise oyuncuyu değiştir ve zar at |
			if (counter == numberOfGame)
			{
				GameSave(enteredValue, secondDice, player, otherPlayer);
				ChangeOrderOfPlay(otherPlayer, 1, 1);
				AutoDice();
			}
			else //| Counter ile NumberOfGame eşit değil ise hata vardır |
			{
				printf("error");
			}
		}

		GamePrint();
		PrintOrderOfPlay();
		GameProcess();
	}

}

int GameControls(char enteredValue[4])
{
	int isSuccess = 1;
	size_t lenteredValueLength = strlen(enteredValue);
	if (lenteredValueLength != 4)
	{
		printf("Wrong: Only 4 character\n");
		isSuccess = 0;
	}

	if (enteredValue[0] == enteredValue[2] && enteredValue[1] == enteredValue[3])
	{
		printf("Wrong : Same character\n");
		isSuccess = 0;
	}
	if (enteredValue[0] != 'A' && enteredValue[0] != 'B' && enteredValue[0] != 'C' && enteredValue[0] != 'D' && enteredValue[0] != 'E' && enteredValue[0] != 'F' && enteredValue[0] != 'G' && enteredValue[0] != 'H' && enteredValue[0] != 'I' && enteredValue[0] != 'J' && enteredValue[0] != 'K' && enteredValue[0] != 'L')
	{
		printf("Wrong : First character\n");
		isSuccess = 0;
	}

	if (enteredValue[1] != '1' && enteredValue[1] != '2' && enteredValue[1] != '3' && enteredValue[1] != '4' && enteredValue[1] != '5')
	{
		printf("Wrong : Second character\n");
		isSuccess = 0;
	}

	if (enteredValue[2] != 'A' && enteredValue[2] != 'B' && enteredValue[2] != 'C' && enteredValue[2] != 'D' && enteredValue[2] != 'E' && enteredValue[2] != 'F' && enteredValue[2] != 'G' && enteredValue[2] != 'H' && enteredValue[2] != 'I' && enteredValue[2] != 'J' && enteredValue[2] != 'K' && enteredValue[2] != 'L')
	{
		printf("Wrong : Third character\n");
		isSuccess = 0;
	}

	if (enteredValue[3] != '1' && enteredValue[3] != '2' && enteredValue[3] != '3' && enteredValue[3] != '4' && enteredValue[3] != '5')
	{
		printf("Wrong : Fourth character\n");
		isSuccess = 0;
	}

	int firstDice;
	int secondDice;
	char tempSourcePlayer = ' ';
	char tempTargetPlayer = ' ';
	int tempTargetCount = 0;

	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "r+");
	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, 62, SEEK_CUR);
	fscanf(fpGameFile, "%d", &firstDice);
	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, 64, SEEK_CUR);
	fscanf(fpGameFile, "%d", &secondDice);

	if (enteredValue[1] == '1')
	{
		int tempFirstPoint = 0;
		switch (enteredValue[0])
		{
		case 'A':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 1), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'B':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 3), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'C':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 5), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'D':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 7), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'E':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 9), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'F':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 11), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'G':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 13), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'H':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 15), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'I':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 17), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'J':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 19), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'K':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 21), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'L':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 23), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;

		}

	}

	if (enteredValue[1] == '5')
	{
		int tempFirstPoint = 104;
		switch (enteredValue[0])
		{
		case 'A':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 1), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'B':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 3), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'C':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 5), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'D':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 7), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'E':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 9), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'F':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 11), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'G':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 13), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'H':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 15), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'I':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 17), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'J':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 19), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'K':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 21), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'L':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 23), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;

		}

	}

	if (enteredValue[1] == '3')
	{
		int tempFirstPoint = 52;
		switch (enteredValue[0])
		{
		case 'E':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 9), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;
		case 'H':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 15), SEEK_CUR);
			fscanf(fpGameFile, "%c", &tempSourcePlayer);
			break;

		}

	}

	if (enteredValue[3] == '1')
	{
		int tempFirstPoint = 0;
		switch (enteredValue[2])
		{
		case 'A':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'B':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 1), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'C':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 3), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'D':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 5), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'E':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 7), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'F':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 9), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'G':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 11), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'H':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 13), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'I':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 15), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'J':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 17), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'K':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 19), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'L':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 21), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;

		}

	}

	if (enteredValue[3] == '5')
	{
		int tempFirstPoint = 104;
		switch (enteredValue[2])
		{
		case 'A':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'B':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 1), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'C':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 3), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'D':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 5), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'E':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 7), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'F':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 9), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'G':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 11), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'H':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 13), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'I':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 15), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'J':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 17), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'K':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 19), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;
		case 'L':
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, (tempFirstPoint + 21), SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempTargetCount);
			fscanf(fpGameFile, "%c", &tempTargetPlayer);
			break;

		}

	}



	fclose(fpGameFile);

	if (enteredValue[3] == '3')
	{
		int tempFirstPoint = 52;
		switch (enteredValue[2])
		{
		case 'E':
			printf("Wrong : Access denied");
			isSuccess = 0;
			break;
		case 'H':
			printf("Wrong : Access denied");
			isSuccess = 0;;
			break;

		}

	}

	//player|dice|NumberOfGame 
	FILE* fpOrderOfPlayFile;
	fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "r+");
	char readOrderOfPlay[5];
	fgets(readOrderOfPlay, 5, fpOrderOfPlayFile);
	fclose(fpOrderOfPlayFile);

	char player = (char)readOrderOfPlay[0];
	char playerDice = readOrderOfPlay[1];
	char numberOfGame = readOrderOfPlay[2];
	char otherPlayer = ' ';

	switch (player)
	{
	case 'X':
		otherPlayer = 'Y';
		break;
	case 'Y':
		otherPlayer = 'X';
		break;
	}

	if (tempSourcePlayer != player)
	{
		printf("Wrong : Source player is not equal");
		isSuccess = 0;
	}

	if (tempTargetPlayer != player && tempTargetCount > 1)
	{
		printf("Wrong : Source player is not equal");
		isSuccess = 0;
	}

	if (player == 'X')
	{

		int firstCount = 0;
		if (enteredValue[0] == 'E' && enteredValue[1] == '3')
		{
			firstCount = 1;
		}
		else if (enteredValue[0] == 'L' && enteredValue[1] == '5')
		{
			firstCount = 2;
		}
		else if (enteredValue[0] == 'K' && enteredValue[1] == '5')
		{
			firstCount = 3;
		}
		else if (enteredValue[0] == 'J' && enteredValue[1] == '5')
		{
			firstCount = 4;
		}
		else if (enteredValue[0] == 'I' && enteredValue[1] == '5')
		{
			firstCount = 5;
		}
		else if (enteredValue[0] == 'H' && enteredValue[1] == '5')
		{
			firstCount = 6;
		}
		else if (enteredValue[0] == 'G' && enteredValue[1] == '5')
		{
			firstCount = 7;
		}
		else if (enteredValue[0] == 'F' && enteredValue[1] == '5')
		{
			firstCount = 8;
		}
		else if (enteredValue[0] == 'E' && enteredValue[1] == '5')
		{
			firstCount = 9;
		}
		else if (enteredValue[0] == 'D' && enteredValue[1] == '5')
		{
			firstCount = 10;
		}
		else if (enteredValue[0] == 'C' && enteredValue[1] == '5')
		{
			firstCount = 11;
		}
		else if (enteredValue[0] == 'B' && enteredValue[1] == '5')
		{
			firstCount = 12;
		}
		else if (enteredValue[0] == 'A' && enteredValue[1] == '5')
		{
			firstCount = 13;
		}
		else if (enteredValue[0] == 'A' && enteredValue[1] == '1')
		{
			firstCount = 14;
		}
		else if (enteredValue[0] == 'B' && enteredValue[1] == '1')
		{
			firstCount = 15;
		}
		else if (enteredValue[0] == 'C' && enteredValue[1] == '1')
		{
			firstCount = 16;
		}
		else if (enteredValue[0] == 'D' && enteredValue[1] == '1')
		{
			firstCount = 17;
		}
		else if (enteredValue[0] == 'E' && enteredValue[1] == '1')
		{
			firstCount = 18;
		}
		else if (enteredValue[0] == 'F' && enteredValue[1] == '1')
		{
			firstCount = 19;
		}
		else if (enteredValue[0] == 'G' && enteredValue[1] == '1')
		{
			firstCount = 20;
		}
		else if (enteredValue[0] == 'H' && enteredValue[1] == '1')
		{
			firstCount = 21;
		}
		else if (enteredValue[0] == 'I' && enteredValue[1] == '1')
		{
			firstCount = 22;
		}
		else if (enteredValue[0] == 'J' && enteredValue[1] == '1')
		{
			firstCount = 23;
		}
		else if (enteredValue[0] == 'K' && enteredValue[1] == '1')
		{
			firstCount = 24;
		}
		else if (enteredValue[0] == 'L' && enteredValue[1] == '1')
		{
			firstCount = 25;
		}

		int secondCount = 0;
		if (enteredValue[2] == 'E' && enteredValue[3] == '3')
		{
			secondCount = 1;
		}
		else if (enteredValue[2] == 'L' && enteredValue[3] == '5')
		{
			secondCount = 2;
		}
		else if (enteredValue[2] == 'K' && enteredValue[3] == '5')
		{
			secondCount = 3;
		}
		else if (enteredValue[2] == 'J' && enteredValue[3] == '5')
		{
			secondCount = 4;
		}
		else if (enteredValue[2] == 'I' && enteredValue[3] == '5')
		{
			secondCount = 5;
		}
		else if (enteredValue[2] == 'H' && enteredValue[3] == '5')
		{
			secondCount = 6;
		}
		else if (enteredValue[2] == 'G' && enteredValue[3] == '5')
		{
			secondCount = 7;
		}
		else if (enteredValue[2] == 'F' && enteredValue[3] == '5')
		{
			secondCount = 8;
		}
		else if (enteredValue[2] == 'E' && enteredValue[3] == '5')
		{
			secondCount = 9;
		}
		else if (enteredValue[2] == 'D' && enteredValue[3] == '5')
		{
			secondCount = 10;
		}
		else if (enteredValue[2] == 'C' && enteredValue[3] == '5')
		{
			secondCount = 11;
		}
		else if (enteredValue[2] == 'B' && enteredValue[3] == '5')
		{
			secondCount = 12;
		}
		else if (enteredValue[2] == 'A' && enteredValue[3] == '5')
		{
			secondCount = 13;
		}
		else if (enteredValue[2] == 'A' && enteredValue[3] == '1')
		{
			secondCount = 14;
		}
		else if (enteredValue[2] == 'B' && enteredValue[3] == '1')
		{
			secondCount = 15;
		}
		else if (enteredValue[2] == 'C' && enteredValue[3] == '1')
		{
			secondCount = 16;
		}
		else if (enteredValue[2] == 'D' && enteredValue[3] == '1')
		{
			secondCount = 17;
		}
		else if (enteredValue[2] == 'E' && enteredValue[3] == '1')
		{
			secondCount = 18;
		}
		else if (enteredValue[2] == 'F' && enteredValue[3] == '1')
		{
			secondCount = 19;
		}
		else if (enteredValue[2] == 'G' && enteredValue[3] == '1')
		{
			secondCount = 20;
		}
		else if (enteredValue[2] == 'H' && enteredValue[3] == '1')
		{
			secondCount = 21;
		}
		else if (enteredValue[2] == 'I' && enteredValue[3] == '1')
		{
			secondCount = 22;
		}
		else if (enteredValue[2] == 'J' && enteredValue[3] == '1')
		{
			secondCount = 23;
		}
		else if (enteredValue[2] == 'K' && enteredValue[3] == '1')
		{
			secondCount = 24;
		}
		else if (enteredValue[2] == 'L' && enteredValue[3] == '1')
		{
			secondCount = 25;
		}

		if (firstCount >= secondCount)
		{
			printf("Wrong : Player X’s play direction is clockwise \n");
			isSuccess = 0;
		}

		if ((secondCount - firstCount) > 6)
		{
			printf("Wrong : move must be less than 7\n");
			isSuccess = 0;
		}

		if (secondCount == 0 || firstCount == 0)
		{
			printf("Wrong : undefined move\n");
			isSuccess = 0;
		}

		if ((secondCount - firstCount) == 0)
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

		if (playerDice == '1' && firstDice < (secondCount - firstCount))
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

		if (playerDice == '2' && secondDice < (secondCount - firstCount))
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

	}

	if (player == 'Y')
	{
		int firstCount = 0;
		if (enteredValue[0] == 'H' && enteredValue[1] == '3')
		{
			firstCount = 1;
		}
		else if (enteredValue[0] == 'L' && enteredValue[1] == '1')
		{
			firstCount = 2;
		}
		else if (enteredValue[0] == 'K' && enteredValue[1] == '1')
		{
			firstCount = 3;
		}
		else if (enteredValue[0] == 'J' && enteredValue[1] == '1')
		{
			firstCount = 4;
		}
		else if (enteredValue[0] == 'I' && enteredValue[1] == '1')
		{
			firstCount = 5;
		}
		else if (enteredValue[0] == 'H' && enteredValue[1] == '1')
		{
			firstCount = 6;
		}
		else if (enteredValue[0] == 'G' && enteredValue[1] == '1')
		{
			firstCount = 7;
		}
		else if (enteredValue[0] == 'F' && enteredValue[1] == '1')
		{
			firstCount = 8;
		}
		else if (enteredValue[0] == 'E' && enteredValue[1] == '1')
		{
			firstCount = 9;
		}
		else if (enteredValue[0] == 'D' && enteredValue[1] == '1')
		{
			firstCount = 10;
		}
		else if (enteredValue[0] == 'C' && enteredValue[1] == '1')
		{
			firstCount = 11;
		}
		else if (enteredValue[0] == 'B' && enteredValue[1] == '1')
		{
			firstCount = 12;
		}
		else if (enteredValue[0] == 'A' && enteredValue[1] == '1')
		{
			firstCount = 13;
		}
		else if (enteredValue[0] == 'A' && enteredValue[1] == '5')
		{
			firstCount = 14;
		}
		else if (enteredValue[0] == 'B' && enteredValue[1] == '5')
		{
			firstCount = 15;
		}
		else if (enteredValue[0] == 'C' && enteredValue[1] == '5')
		{
			firstCount = 16;
		}
		else if (enteredValue[0] == 'D' && enteredValue[1] == '5')
		{
			firstCount = 17;
		}
		else if (enteredValue[0] == 'E' && enteredValue[1] == '5')
		{
			firstCount = 18;
		}
		else if (enteredValue[0] == 'F' && enteredValue[1] == '5')
		{
			firstCount = 19;
		}
		else if (enteredValue[0] == 'G' && enteredValue[1] == '5')
		{
			firstCount = 20;
		}
		else if (enteredValue[0] == 'H' && enteredValue[1] == '5')
		{
			firstCount = 21;
		}
		else if (enteredValue[0] == 'I' && enteredValue[1] == '5')
		{
			firstCount = 22;
		}
		else if (enteredValue[0] == 'J' && enteredValue[1] == '5')
		{
			firstCount = 23;
		}
		else if (enteredValue[0] == 'K' && enteredValue[1] == '5')
		{
			firstCount = 24;
		}
		else if (enteredValue[0] == 'L' && enteredValue[1] == '5')
		{
			firstCount = 25;
		}

		int secondCount = 0;
		if (enteredValue[2] == 'H' && enteredValue[3] == '3')
		{
			secondCount = 1;
		}
		else if (enteredValue[2] == 'L' && enteredValue[3] == '1')
		{
			secondCount = 2;
		}
		else if (enteredValue[2] == 'K' && enteredValue[3] == '1')
		{
			secondCount = 3;
		}
		else if (enteredValue[2] == 'J' && enteredValue[3] == '1')
		{
			secondCount = 4;
		}
		else if (enteredValue[2] == 'I' && enteredValue[3] == '1')
		{
			secondCount = 5;
		}
		else if (enteredValue[2] == 'H' && enteredValue[3] == '1')
		{
			secondCount = 6;
		}
		else if (enteredValue[2] == 'G' && enteredValue[3] == '1')
		{
			secondCount = 7;
		}
		else if (enteredValue[2] == 'F' && enteredValue[3] == '1')
		{
			secondCount = 8;
		}
		else if (enteredValue[2] == 'E' && enteredValue[3] == '1')
		{
			secondCount = 9;
		}
		else if (enteredValue[2] == 'D' && enteredValue[3] == '1')
		{
			secondCount = 10;
		}
		else if (enteredValue[2] == 'C' && enteredValue[3] == '1')
		{
			secondCount = 11;
		}
		else if (enteredValue[2] == 'B' && enteredValue[3] == '1')
		{
			secondCount = 12;
		}
		else if (enteredValue[2] == 'A' && enteredValue[3] == '1')
		{
			secondCount = 13;
		}
		else if (enteredValue[2] == 'A' && enteredValue[3] == '5')
		{
			secondCount = 14;
		}
		else if (enteredValue[2] == 'B' && enteredValue[3] == '5')
		{
			secondCount = 15;
		}
		else if (enteredValue[2] == 'C' && enteredValue[3] == '5')
		{
			secondCount = 16;
		}
		else if (enteredValue[2] == 'D' && enteredValue[3] == '5')
		{
			secondCount = 17;
		}
		else if (enteredValue[2] == 'E' && enteredValue[3] == '5')
		{
			secondCount = 18;
		}
		else if (enteredValue[2] == 'F' && enteredValue[3] == '5')
		{
			secondCount = 19;
		}
		else if (enteredValue[2] == 'G' && enteredValue[3] == '5')
		{
			secondCount = 20;
		}
		else if (enteredValue[2] == 'H' && enteredValue[3] == '5')
		{
			secondCount = 21;
		}
		else if (enteredValue[2] == 'I' && enteredValue[3] == '5')
		{
			secondCount = 22;
		}
		else if (enteredValue[2] == 'J' && enteredValue[3] == '5')
		{
			secondCount = 23;
		}
		else if (enteredValue[2] == 'K' && enteredValue[3] == '5')
		{
			secondCount = 24;
		}
		else if (enteredValue[2] == 'L' && enteredValue[3] == '5')
		{
			secondCount = 25;
		}

		if (firstCount >= secondCount)
		{
			printf("Wrong : Player Y’s play direction is counter-clockwise \n");
			isSuccess = 0;
		}

		if ((secondCount - firstCount) > 6)
		{
			printf("Wrong : move must be less than 7\n");
			isSuccess = 0;
		}

		if (secondCount == 0 || firstCount == 0)
		{
			printf("Wrong : undefined move\n");
			isSuccess = 0;
		}

		if ((secondCount - firstCount) == 0)
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

		if (playerDice == '1' && firstDice < (secondCount - firstCount))
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

		if (playerDice == '2' && secondDice < (secondCount - firstCount))
		{
			printf("Wrong : move must be grater than 0\n");
			isSuccess = 0;
		}

	}


	return isSuccess;
}
void GameSave(char enteredValue[4], int dice, char player, char otherPlayer)
{
	enteredValue[0] = toupper(enteredValue[0]);
	enteredValue[2] = toupper(enteredValue[2]);

	if (enteredValue[1] == '1')
	{
		switch (enteredValue[0])
		{
		case 'A':
			ChangeSourceRow(0, 0, 0, player);
			break;
		case 'B':
			ChangeSourceRow(0, 2, 2, player);
			break;
		case 'C':
			ChangeSourceRow(0, 4, 4, player);
			break;
		case 'D':
			ChangeSourceRow(0, 6, 6, player);
			break;
		case 'E':
			ChangeSourceRow(0, 8, 8, player);
			break;
		case 'F':
			ChangeSourceRow(0, 10, 10, player);
			break;
		case 'G':
			ChangeSourceRow(0, 12, 12, player);
			break;
		case 'H':
			ChangeSourceRow(0, 14, 14, player);
			break;
		case 'I':
			ChangeSourceRow(0, 16, 16, player);
			break;
		case 'J':
			ChangeSourceRow(0, 18, 18, player);
			break;
		case 'K':
			ChangeSourceRow(0, 20, 20, player);
			break;
		case 'L':
			ChangeSourceRow(0, 22, 22, player);
			break;
		}
	}

	if (enteredValue[1] == '5')
	{
		int tempStartCell = 104;
		switch (enteredValue[0])
		{
		case 'A':
			ChangeSourceRow(104, 0, tempStartCell, player);
			break;
		case 'B':
			ChangeSourceRow(104, 2, (tempStartCell + 2), player);
			break;
		case 'C':
			ChangeSourceRow(104, 4, (tempStartCell + 4), player);
			break;
		case 'D':
			ChangeSourceRow(104, 6, (tempStartCell + 6), player);
			break;
		case 'E':
			ChangeSourceRow(104, 8, (tempStartCell + 8), player);
			break;
		case 'F':
			ChangeSourceRow(104, 10, (tempStartCell + 10), player);
			break;
		case 'G':
			ChangeSourceRow(104, 12, (tempStartCell + 12), player);
			break;
		case 'H':
			ChangeSourceRow(104, 14, (tempStartCell + 14), player);
			break;
		case 'I':
			ChangeSourceRow(104, 16, (tempStartCell + 16), player);
			break;
		case 'J':
			ChangeSourceRow(104, 18, (tempStartCell + 18), player);
			break;
		case 'K':
			ChangeSourceRow(104, 20, (tempStartCell + 20), player);
			break;
		case 'L':
			ChangeSourceRow(104, 22, (tempStartCell + 22), player);
			break;
		}
	}

	if (enteredValue[1] == '3')
	{
		int tempStartCell = 52;
		int tempStartRow = 52;
		switch (enteredValue[0])
		{
		case 'E':
			ChangeSourceRow(tempStartRow, 8, (tempStartCell + 8), player);
			break;
		case 'H':
			ChangeSourceRow(tempStartRow, 14, (tempStartCell + 14), player);
			break;
		}
	}

	////******


	if (enteredValue[3] == '1')
	{
		switch (enteredValue[2])
		{
		case 'A':
			ChangeDestinationRow(0, 0, 0, player, otherPlayer);
			break;
		case 'B':
			ChangeDestinationRow(0, 2, 2, player, otherPlayer);
			break;
		case 'C':
			ChangeDestinationRow(0, 4, 4, player, otherPlayer);
			break;
		case 'D':
			ChangeDestinationRow(0, 6, 6, player, otherPlayer);
			break;
		case 'E':
			ChangeDestinationRow(0, 8, 8, player, otherPlayer);
			break;
		case 'F':
			ChangeDestinationRow(0, 10, 10, player, otherPlayer);
			break;
		case 'G':
			ChangeDestinationRow(0, 12, 12, player, otherPlayer);
			break;
		case 'H':
			ChangeDestinationRow(0, 14, 14, player, otherPlayer);
			break;
		case 'I':
			ChangeDestinationRow(0, 16, 16, player, otherPlayer);
			break;
		case 'J':
			ChangeDestinationRow(0, 18, 18, player, otherPlayer);
			break;
		case 'K':
			ChangeDestinationRow(0, 20, 20, player, otherPlayer);
			break;
		case 'L':
			ChangeDestinationRow(0, 22, 22, player, otherPlayer);
			break;
		}
	}

	if (enteredValue[3] == '5')
	{
		int tempStartCell = 104;
		switch (enteredValue[2])
		{
		case 'A':
			ChangeDestinationRow(104, 0, tempStartCell, player, otherPlayer);
			break;
		case 'B':
			ChangeDestinationRow(104, 2, (tempStartCell + 2), player, otherPlayer);
			break;
		case 'C':
			ChangeDestinationRow(104, 4, (tempStartCell + 4), player, otherPlayer);
			break;
		case 'D':
			ChangeDestinationRow(104, 6, (tempStartCell + 6), player, otherPlayer);
			break;
		case 'E':
			ChangeDestinationRow(104, 8, (tempStartCell + 8), player, otherPlayer);
			break;
		case 'F':
			ChangeDestinationRow(104, 10, (tempStartCell + 10), player, otherPlayer);
			break;
		case 'G':
			ChangeDestinationRow(104, 12, (tempStartCell + 12), player, otherPlayer);
			break;
		case 'H':
			ChangeDestinationRow(104, 14, (tempStartCell + 14), player, otherPlayer);
			break;
		case 'I':
			ChangeDestinationRow(104, 16, (tempStartCell + 16), player, otherPlayer);
			break;
		case 'J':
			ChangeDestinationRow(104, 18, (tempStartCell + 18), player, otherPlayer);
			break;
		case 'K':
			ChangeDestinationRow(104, 20, (tempStartCell + 20), player, otherPlayer);
			break;
		case 'L':
			ChangeDestinationRow(104, 22, (tempStartCell + 22), player, otherPlayer);
			break;
		}
	}

	if (enteredValue[3] == '3')
	{
		int tempStartCell = 52;
		int tempStartRow = 52;
		switch (enteredValue[2])
		{
		case 'E':
			ChangeDestinationRow(tempStartRow, 8, (tempStartCell + 8), player, otherPlayer);
			break;
		case 'H':
			ChangeDestinationRow(tempStartRow, 14, (tempStartCell + 14), player, otherPlayer);
			break;
		}
	}

}

void ChangeOrderOfPlay(char player, int playerDice, int numberOfGame)
{
	FILE* fpOrderOfPlayFile;
	fpOrderOfPlayFile = fopen("OrderOfPlay.dat", "w+");
	//player|dice|NumberOfGame 
	fprintf(fpOrderOfPlayFile, "%c%d%d", player, playerDice, numberOfGame);
	fprintf(fpOrderOfPlayFile, "\n");
	fclose(fpOrderOfPlayFile);
}

void ChangeSourceRow(int startRow, int startRowCell, int startCell, char player)
{
	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "r+");

	char tempRow1[25];
	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, startRow, SEEK_CUR);
	fgets(tempRow1, 25, fpGameFile);

	int tempNumber = 0;
	int newNumber = 0;
	char tempPlayer = ' ';
	char tempNumberChar = ' ';

	if (tempRow1[startRowCell] != ' ')
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, startCell, SEEK_CUR);
		fscanf(fpGameFile, "%d", &tempNumber);
	}

	if (tempNumber > 1)
	{
		newNumber = tempNumber - 1;
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, startCell, SEEK_CUR);
		fprintf(fpGameFile, "%d%c", newNumber, player);
	}
	else if (startRow == 52)
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, startCell, SEEK_CUR);
		fprintf(fpGameFile, "%d%c", newNumber, player);
	}
	else
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, startCell, SEEK_CUR);
		fprintf(fpGameFile, "  ");
	}
	fclose(fpGameFile);
}

void ChangeDestinationRow(int startRow, int startRowCell, int startCell, char player, char otherPlayer)
{
	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "r+");

	char tempRow1[25];
	fseek(fpGameFile, 0, SEEK_SET);
	fseek(fpGameFile, startRow, SEEK_CUR);
	fgets(tempRow1, 25, fpGameFile);

	int tempNumber = 0;
	int newNumber = 0;
	char tempPlayer = ' ';
	char tempTargetCellPlayer = ' ';

	if (tempRow1[(startRowCell + 1)] == ' ' || (tempRow1[(startRowCell + 1)] != ' ' && tempRow1[(startRowCell + 1)] == player))
	{
		if (tempRow1[startRowCell] != ' ')
		{
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, startCell, SEEK_CUR);
			fscanf(fpGameFile, "%d", &tempNumber);
		}

		if (tempNumber >= 1)
		{
			newNumber = tempNumber + 1;
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, startCell, SEEK_CUR);
			fprintf(fpGameFile, "%d%c", newNumber, player);
		}
		else
		{
			fseek(fpGameFile, 0, SEEK_SET);
			fseek(fpGameFile, startCell, SEEK_CUR);
			fprintf(fpGameFile, "%d%c", 1, player);
		}

	}

	if ((tempRow1[(startRowCell + 1)] != ' ' && tempRow1[(startRowCell + 1)] == otherPlayer))
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, startCell, SEEK_CUR);
		fprintf(fpGameFile, "%d%c", 1, player);


	}

	fclose(fpGameFile);

	if ((tempRow1[(startRowCell + 1)] != ' ' && tempRow1[(startRowCell + 1)] == otherPlayer))
	{

		WriteBrokenToFile(otherPlayer);

	}

}


void WriteBrokenToFile(char brokenPlayer)
{
	FILE* fpGameFile;
	fpGameFile = fopen("Table.dat", "r+");
	int tempNumber = 0;
	int newNumber = 0;

	if (brokenPlayer == 'X')
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 60, SEEK_CUR);
		fscanf(fpGameFile, "%d", &tempNumber);

		newNumber = tempNumber + 1;
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 60, SEEK_CUR);
		fprintf(fpGameFile, "%dX", newNumber);
	}

	if (brokenPlayer == 'Y')
	{
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 66, SEEK_CUR);
		fscanf(fpGameFile, "%d", &tempNumber);

		newNumber = tempNumber + 1;
		fseek(fpGameFile, 0, SEEK_SET);
		fseek(fpGameFile, 66, SEEK_CUR);
		fprintf(fpGameFile, "%dY", newNumber);
	}


	fclose(fpGameFile);


}

