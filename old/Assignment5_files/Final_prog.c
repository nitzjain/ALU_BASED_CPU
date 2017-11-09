#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

struct flags
{
	uint8_t sym : 1;
	uint8_t chr : 1;
} flag;
char input[30];	//For storing input expession fom user
int digit = 0;	//Count number digits in single number of expession	
int number = 0;	//Holds number

int i = 0;
int j = 0;	//Holds number of operands
int k = 0;	//Holds level of parse tree
int l = 0;	//Holds temporay copy of level of parse tree

/*Prints parse tree for valid expression*/
void print_parse_tree()
{

	for (i = 0; i < strlen(input);)
	{
		if (isdigit(input[i]))	//if operand is number
		{
			digit++;			//incement digits in oprand
			i++;
		}
		else
		{
			if (digit>0)	//if operand is number
			{

				while (digit>0)
				{
					int power = (int)(pow(10.0, (digit - 1)));
					number += ((input[i - digit] - 48) * power);	//Converts operand into number
					digit--;
				}
				if (j == 1)							//If last operand
				{
					printf("\t\t%d", number);		//Print last operand (number)
					j--;
				}
				else
				{
					if (k>0)							//Adjust indentation of parse tree
						printf("\t\t%c\n\n", input[i]);	
					else
						printf("\t%c\n\n", input[i]);
					l = k;								//Copying level of parse tree into temporary variable
					while (l>0)							//Adjust indentation of parse tree
					{
						printf("\t");
						l--;
					}
					printf("%d", number);			//Print operand (number)
					j--;							//Decrement number of operands left to be printed
					i++;							
					k++;							//Incement level of parse tree
				}
				number = 0;
				digit = 0;
			}
			else if (isalpha(input[i]))
			{
				if (j == 1)							//If last operand
				{
					printf("\t\t%c", input[i]);		//Print last operand (character)
					i++;
					j--;
				}
				else
				{
					if (k>0)							//Adjust indentation of parse tree
						printf("\t\t%c\n\n", input[i + 1]);
					else
						printf("\t%c\n\n", input[i + 1]);
					l = k;								//Copying level of parse tree into temporary variable
					while (l>0)							//Adjust indentation of parse tree
					{
						printf("\t");
						l--;
					}
					printf("%c", input[i]);			//Print operand (character)
					j--;							//Decrement number of operands left to be printed
					i = i + 2;		
					k++;							//Incement level of parse tree
				}
			}
		}
	}
	if (digit > 0)									//If last operand is number
	{

		while (digit > 0)
		{
			int power = (int)(pow(10.0, (digit - 1)));
			number += ((input[i - digit] - 48) * power); //Converts operand into number
			digit--;
		}
		printf("\t\t%d", number);					//Print the operand(number)
	}
}

/*Exits the program*/
void exit_prog()
{
	printf("Expression is not valid !\n");
	getchar();
	getchar();
	exit(0);
}

int main()
{
	printf("\nEnter the expression: ");			
	scanf("%s", &input);					//Input expression from user

	flag.chr = 0;							//Initializing operand(character or number) flag to 0
	flag.sym = 0;							//Initializing symbol flag to 0

	for (i = 0; i < strlen(input); i++)
	{

		if (isdigit(input[i]))				//if operand is number			
			digit++;						//incement digits in oprand

		else
		{
			if (digit>0)	//if operand is number
			{

				/*while (digit>0)
				{
					int power = (int)(pow(10.0, (digit - 1)));
					number += ((input[i - digit] - 48) * power);
					digit--;
				}
				*/
				digit = 0;
				number = 0;
				j++;
				if (flag.chr == 0)		//Checking whether last input was operand or not
				{
					flag.chr = 1;
					flag.sym = 0;
				}
				else
				{
					exit_prog();
				}
				digit = 0;
			}
			if (isalpha(input[i]))		//if input is alphabet
			{
				if (flag.chr == 0)		//Checking whether last input was operand or not
				{
					flag.chr = 1;
					flag.sym = 0;
					j++;				//Increment number of operand count
				}
				else
				{
					exit_prog();
				}
			}
			else
			{
				switch (input[i])				//Handling all operators
				{
				case '+':
				case '-':
				case '/':
				case '*':
				case '%':
					if (flag.sym == 0)		//Checking whether last input was operator or not
					{
						flag.sym = 1;
						flag.chr = 0;
					}
					else
					{
						exit_prog();
					}
					break;

				default:
					exit_prog();
				}
			}
		}
	}
	if (digit>0)									//If last operand is number
	{
		if (flag.chr == 0)
		{
			flag.chr = 1;
			flag.sym = 0;
			j++;
		}
		else
		{
			exit_prog();
		}
	}
	if (flag.chr == 1)
	{
		printf("Expression is valid!\n\n");
		printf("********* Printing parse tree of expression *********\n\n");
		number = 0;
		digit = 0;
		print_parse_tree();
	}
	else
		exit_prog();

	getchar();
	getchar();
	return 0;
}

