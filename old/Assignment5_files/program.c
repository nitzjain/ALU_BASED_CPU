#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

struct flags
{
	uint8_t sym : 1;
	uint8_t chr : 1;
	//uint8_t brac;
} flag;
char input[30];
char output[30];
int i = 0;
int j = 0;

void print_parsetree()
{
		
}

void exit_prog()
{
	printf("Not valid expression");
	getchar();
	getchar();
	exit(0);
}
int main()
{
	printf("\nEnter the expression: ");
	scanf("%s", &input);
	int digit = 0;
	int number = 0;
	//flag.brac = 0;
	flag.chr = 0;
	flag.sym = 0;
	//printf("\nYou wrote: %s", input);
	for (i = 0; i < strlen(input); i++)
	{
		if (isdigit(input[i]))
			digit++;
		
		else
		{
			if (digit>0)
			{	
				//printf("digit = %d\n", digit);
				while (digit>0)
				{
					//printf("\ndigit in while = %d", digit);
					//printf("\ninput[i-digit] = %d", (input[i - digit]-48));
					//printf("\n10^digit = %d", (int)(pow(10.0, (digit - 1))));
					int power = (int)(pow(10.0, (digit - 1)));
					number += ((input[i - digit]-48) * power);
					//printf("\nnumber in while is :%d", number);
					digit--;
				}
				printf("number is :%d\n", number);
				number = 0;
				j++;
				if (flag.chr == 0)
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
			if (isalpha(input[i]))
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
			else
			{
				switch (input[i])
				{
				case '+':
				case '-':
				case '/':
				case '*':
					if (flag.sym == 0 )//&& input[i-1]!='(')
					{
						flag.sym = 1;
						flag.chr = 0;
					}
					else
					{
						exit_prog();
					}
					break;
				/*case '(':
							flag.brac++;
						break;
				case ')':
						if (flag.brac != 0 || flag.sym == 0)
							flag.brac--;
						else
						{
							exit_prog();
						}
						break;
						*/
				default:
					exit_prog();
				}
			}
		}
	}
	if (digit>0)
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
	if (flag.chr == 1)// && flag.brac == 0)
	{
		printf("OK !\n");
		printf("j=%d\n",j);
		print_parse_tree();
	}
	else
		exit_prog();

	getchar();
	getchar();
	return 0;
}

