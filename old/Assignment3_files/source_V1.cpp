/* Assignment 2 "Adding an ALU and a Stack"
Group - SS05*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<conio.h>
#include<ctype.h>
#include<cmath>


#define LINE_LEN 15
#define MEM_MAX 2047
#define STACK_MAX 1152
#define INS_MAX 1023
#define OS_MAX 511
uint8_t MEM[MEM_MAX + 1];
int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
uint8_t flag;
uint32_t pc, bp, sp;

uint8_t lenben;	//lenben=1 (Little Endian Machine)
				//lenben=2 (Big Endian Machine)

//Initializing memory, general purpose registers and special purpose registers.
void cpu_init()
{
	do
	{
		printf("\nMachine selection 1.Little Endian 2. Big Endian : ");
		scanf("%d", &lenben);
	} while ((lenben != 1) && (lenben != 2));

	for (int i = OS_MAX + 1; i <= MEM_MAX; i++)
	{
		MEM[i] = 0xff;
	}
	r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = r8 = r9 = r10 = r11 = r12 = r13 = r14 = r15 = 0x00;
	flag = 0;
	pc = 0;
	bp = INS_MAX + 1;
	sp = INS_MAX + 1;
}

//Converting register name to the operand value which CPU understands.
uint8_t get_reg_code(char *ins)
{
	uint8_t code = 0x00;
	ins++;  //GO TO NEXT POSITION BYPASS 'r'

	switch (atoi(ins))
	{
	case 0:
		code = 0x00;
		break;
	case 1:
		code = 0x01;
		break;
	case 2:
		code = 0x02;
		break;
	case 3:
		code = 0x03;
		break;
	case 4:
		code = 0x04;
		break;
	case 5:
		code = 0x05;
		break;
	case 6:
		code = 0x06;
		break;
	case 7:
		code = 0x07;
		break;
	case 8:
		code = 0x08;
		break;
	case 9:
		code = 0x09;
		break;
	case 10:
		code = 0x0A;
		break;
	case 11:
		code = 0x0B;
		break;
	case 12:
		code = 0x0C;
		break;
	case 13:
		code = 0x0D;
		break;
	case 14:
		code = 0x0E;
		break;
	case 15:
		code = 0x0F;
		break;
	default:
		printf("\nError in register read !");
		break;
	}
	return code;
}

//Converts hex to decimal.
int hex_to_dec(char ch)
{
	if (isdigit(ch))
		return (ch - '0');
	else if (islower(ch))
		return (ch - 'a' + 10);
	else if (isupper(ch))
		return (ch - 'A' + 10);
}

//Read assembly language from text file and convert it to opcode and operands and store it in instruction memory.
void asm_read()
{
	uint16_t ins = OS_MAX+1;
	char line[LINE_LEN];
	uint8_t lines = 0;
	char *opc_s, *opr1_s, *opr2_s, *opr3_s;
	uint8_t opc, opr1, opr2, opr3;

	FILE *fp;
	fp = fopen("assembly.txt", "r");
	if (fp == NULL)
		printf("\nError opening assembly file !");

	while (fgets(line, LINE_LEN, fp) != NULL)
	{
		lines++;
	}
	
	fp = fopen("assembly.txt", "r");
	while (lines != 0)
	{
		fgets(line, LINE_LEN, fp);
		opc_s = strtok(line, " ");
		opr1_s = strtok(NULL, ",");
		opr2_s = strtok(NULL, ",");
		opr3_s = strtok(NULL, " ");
		if (!strcmp(opc_s, "lw"))
			opc = 0x00;
		else if (!strcmp(opc_s, "sw"))
			opc = 0x01;
		else if (!strcmp(opc_s, "add"))
			opc = 0x02;
		else if (!strcmp(opc_s, "sub"))
			opc = 0x03;
		else if (!strcmp(opc_s, "mul"))
			opc = 0x04;
		else if (!strcmp(opc_s, "div"))
			opc = 0x05;
		else if (!strcmp(opc_s, "mod"))
			opc = 0x06;
		else if (!strcmp(opc_s, "push"))
			opc = 0x07;
		else if (!strcmp(opc_s, "pop"))
			opc = 0x08;

		opr1 = get_reg_code(opr1_s);
		if (opc == 0x00 || opc == 0x01)			//for load and store operation
		{
			opr2 = (hex_to_dec(opr2_s[0]) << 4) | (hex_to_dec(opr2_s[1]));
			opr3 = (hex_to_dec(opr2_s[2]) << 4) | (hex_to_dec(opr2_s[3]));
		}
		else if (opc == 0x07 || opc == 0x08)	//for push and pop operation
		{
			opr2 = 0;
			opr3 = 0;
		}
		else                                   //for all other operations
		{
			opr2 = get_reg_code(opr2_s);
			opr3 = get_reg_code(opr3_s);
		}

		if (lenben == 1)		//Little endian
		{
			MEM[ins++] = opr3;
			MEM[ins++] = opr2;
			MEM[ins++] = opr1;
			MEM[ins++] = opc;
		}
		else                    //Big endian
		{
			MEM[ins++] = opc;
			MEM[ins++] = opr1;
			MEM[ins++] = opr2;
			MEM[ins++] = opr3;
		}

		printf("\n");
		lines--;
	}
	fclose(fp);
}

//Display contents of memory, general purpose and special purpose registers.
void display()
{
	int i, bit, j;
	uint8_t temp_flag = flag;	//copying status register in temporary
	printf("\n\nContent of instruction Memory:\n\n");
	printf("Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value\n");
	//for (i = OS_MAX + 1, j = 1; i <= INS_MAX; i++, j++)
	for (i = OS_MAX + 1, j = 1; i <= OS_MAX+32; i++, j++)
	{
		printf("%03x    ", i);
		printf("%02x    ", MEM[i]);
		if (j % 6 == 0)
			printf("\n");
	}

	printf("\n\nContent of Stack Memory:\n\n");
	printf("Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value\n");
	for (i = INS_MAX + 1, j = 1; i <= INS_MAX+32; i++, j++)
	{
		printf("%03X    ", i);
		printf("%02x    ", MEM[i]);
		if (j % 6 == 0)
			printf("\n");
	}

	printf("\n\nContent of Data Memory:\n\n");
	printf("Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value\n");
	//for (i = INS_MAX + 1, j = 1; i <= MEM_MAX; i++, j++)
	for (i = STACK_MAX + 1, j = 1; i <= STACK_MAX+32; i++, j++)
	{
		printf("%03X    ", i);
		printf("%02x    ", MEM[i]);
		if (j % 6 == 0)
			printf("\n");
	}

	printf("\n\nContent of General Purpose Register:\n\n");
	printf("R0 = %08x R1 = %08x  R2 = %08x  R3 = %08x  R4 = %08x  R5 = %08x  R6 = %08x  R7 = %08x\n", r0, r1, r2, r3, r4, r5, r6, r7);
	printf("R8 = %08x R9 = %08x R10 = %08x R11 = %08x R12 = %08x R13 = %08x R14 = %08x R15 = %08x\n", r8, r9, r10, r11, r12, r13, r14, r15);

	printf("\nContents of Status register:\t");

	for (bit = 0; bit<8; bit++)  //Printing flags from MSB to LSB
	{
		printf("%i ", (temp_flag & 0x80)>>7);
		temp_flag = temp_flag<<1;
	}

	printf("\n\nContent of Program Counter (PC): ");
	printf("%08x\n", pc);
	printf("\nContent of Stack Pointer (SP):\t");
	printf("%08x\n", sp);
	printf("\nContent of Base Pointer (BP):\t");
	printf("%08x\n", bp);
	getch();
}

//Stores predifined values in registers to demonstrate Assignment 2.
void assignment2()
{
	r1 = 0x00000FFF;
	r2 = 0x00000002;
}

//Checks for flags and sets/resets accordingly
void check_flags(uint64_t lresult)
{
	int result = (int)lresult;
	if (result == 0)
		flag |= (1 << 3);	//set zero flag
	else
		flag &= ~(1 << 3);	//clear zero flag

	if (lresult<0)
		flag |= (1 << 4);	//set sign flag
	else
		flag &= ~(1 << 4);	//clear sign flag

	int cnt = 0;
	uint8_t res;
	res=result;
	while (res)
	{
		cnt += res & 1;
		res >>= 1;
	}
	if (cnt % 2)
		flag |= (1 << 1);	//set parity flag
	else
		flag &= ~(1 << 1);	//clear parity flag

	uint32_t overflow=lresult>>32;
	
	if (overflow >= 1)
	{
		flag |= (1 << 0);
		flag |= (1 << 5);	//set overflow flag
	}
	else
	{
		flag &= ~(1 << 0);
		flag &= ~(1 << 5);	//clear overflow flag
	}
}

//This function fetch data from memory and store it to general purpose register.
int load_reg(uint16_t address)
{
	int val=0;
	if (lenben == 1)
	{
		val = MEM[address++];
		val |= MEM[address++] << 8;
		val |= MEM[address++] << 16;
		val |= MEM[address++] << 24;
	}
	else
	{
		val |= MEM[address++] << 24;
		val |= MEM[address++] << 16;
		val |= MEM[address++] << 8;
		val = MEM[address++];
	}
	return val;
}

//This function store general purpose register value to some memory location.
void store_reg(int reg, uint16_t address)
{
	if (lenben == 1)
	{
		MEM[address++] = reg;
		MEM[address++] = reg >> 8;
		MEM[address++] = reg >> 16;
		MEM[address++] = reg >> 24;
	}
	else
	{
		MEM[address++] = reg >> 24;
		MEM[address++] = reg >> 16;
		MEM[address++] = reg >> 8;
		MEM[address++] = reg;
	}
}

//This function uses two 8 bit numbers and convert them to some memory location which can be used to store and load data from it.
int make_address(uint8_t addr1, uint8_t addr2)
{

	uint16_t addr;
	addr = (addr1 << 8) + addr2;
	printf("\nAddress value = %d.\n", addr);
	return addr;

}

int* reg_point(uint8_t op)
{
	switch (op)
	{
	case 0:
		return &r0;
	case 1:
		return &r1;
	case 2:
		return &r2;
	case 3:
		return &r3;
	case 4:
		return &r4;
	case 5:
		return &r5;
	case 6:
		return &r6;
	case 7:
		return &r7;
	case 8:
		return &r8;
	case 9:
		return &r9;
	case 10:
		return &r10;
	case 11:
		return &r11;
	case 12:
		return &r12;
	case 13:
		return &r13;
	case 14:
		return &r14;
	case 15:
		return &r15;
	default:
		printf("\nError reading register address !");
		break;
	}
}
void push(int val)
{
	if (sp >= STACK_MAX)
	{
		printf("\nError: Stack is full ! \nExiting program !");
		getchar();
		exit(1);
	}
	if (lenben == 1)
	{
		MEM[sp++] = val;
		MEM[sp++] = val >> 8;
		MEM[sp++] = val >> 16;
		MEM[sp++] = val >> 24;
	}
	else
	{
		MEM[sp++] = val >> 24;
		MEM[sp++] = val >> 16;
		MEM[sp++] = val >> 8;
		MEM[sp++] = val;
	}
}

int pop()
{
	int val = 0;
	if (sp <= INS_MAX + 4)
	{
		printf("\nError: Stack is empty ! \nExiting program !");
		getchar();
		exit(1);				//Exits program
	}
		
	if (lenben == 1)
	{
		val |= MEM[--sp] << 24;
		MEM[sp] = 0xff;			//Clear the stack
		val |= MEM[--sp] << 16;
		MEM[sp] = 0xff;
		val |= MEM[--sp] << 8;
		MEM[sp] = 0xff;
		val |= MEM[--sp];
		MEM[sp] = 0xff;
	}
	else
	{
		val |= MEM[--sp];
		MEM[sp] = 0xff;			//Clear the stack
		val |= MEM[--sp] << 8;
		MEM[sp] = 0xff;
		val |= MEM[--sp] << 16;
		MEM[sp] = 0xff;
		val |= MEM[--sp] << 24;
		MEM[sp] = 0xff;
	}
	return val;
}
int get2scomp(int c)
{
	return(add(~c, 1));
}
int add(int a, int b)
{
	uint64_t na = 0,nb=0;
	na = a; nb = b;
	uint64_t carry;
	while (nb != 0)
	{
		carry = na&nb;
		na = na^nb;
		nb = carry << 1;
	}
	check_flags(na);
	return ((int)na);
}

int sub(int a, int b)
{
	int result;
	int minusb = get2scomp(b);
	result = add(a, minusb);
	return(result);
}


int mul(int a, int b)
{
	uint64_t na = a, nb = b;
	uint64_t lresult = 0;
	if (nb<0 && na>0)
	{
		na = na + nb;
		nb = na - nb;
		na = na - nb;	//swapping of the numbers to get nb to positive
	}
	if (nb < 0 && na < 0)
	{
		na = abs(na);
		nb = abs(nb);
	}
	while (nb != 0)
	{
		if (nb & 0x01)
		{
			lresult = add(lresult, na);
		}
		na <<= 1;
		nb >>= 1;
	}
	check_flags(lresult);
	return ((int)lresult);
}

int div_mod(int a, int b, int mode)  //mode 1 for div and 0 for mod
{
	int dividebyzero = 0;
	int sign = 0, quo = 0, rem = 0;
	if (a<0)
	{
		a = get2scomp(a);
		sign = sign ^ 1;
	}
	if (b<0)
	{
		b = get2scomp(b);
		sign = sign ^ 1;
	}
	if (b>0)
	{
		while (a >= b)
		{
			a = sub(a, b);
			quo = add(quo, 1);
		}
	}
	else
	{
		dividebyzero = 1;
		printf("\nDivision by zero exception !");
		getchar();
		exit(1);
	}
	if (a>0)		
		rem = a;
	else 
		rem = 0;
	
	if (mode)
	{
		if (sign == 1) 
			quo = get2scomp(quo);
		check_flags(quo);
		return quo;
	}
	else
	{
		if (rem<0) 
			rem = get2scomp(rem);
		check_flags(rem);
		return rem;
	}
}

bool branchEqual(int a , int b)
{
	if( a == b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//This function does the main execution. It reads opcode and operands from instruction memory and perform written steps.
void main_execution()
{
	int flag=0;
	bool equal;
	uint16_t address;
	uint16_t op1, op2, op3;
	pc = 512;
	if (lenben == 1)
	{
		pc = pc + 3;
	}
	while (MEM[pc] != 0XFF)
	{
		if (lenben == 1)
		{
			op1 = pc - 1;
			op2 = pc - 2;
			op3 = pc - 3;
		}
		else
		{
			op1 = pc + 1;
			op2 = pc + 2;
			op3 = pc + 3;
		}
		switch (MEM[pc])
		{
			case 0://Case for loading data from memory to register.
				printf("\n************** Inside load instruction. **************\n");
				address = make_address(MEM[op2], MEM[op3]);
				*reg_point(MEM[op1]) = load_reg(address);
				break;
			case 1://Case for storing data from register to memory.
				printf("\n************** Inside store instruction. **************\n");
				address = make_address(MEM[op2], MEM[op3]);
				store_reg(*reg_point(MEM[op1]), address);
				break;
			case 2:
				printf("\n************** Inside add instruction. **************\n");
				*reg_point(MEM[op1]) = add(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 3:
				printf("\n************** Inside sub instruction. **************\n");
				*reg_point(MEM[op1]) = sub(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 4:
				printf("\n************** Inside mul instruction. **************\n");
				*reg_point(MEM[op1]) = mul(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 5:
				printf("\n************** Inside div instruction. **************\n");
				printf("\n************** It might take more time depending on input **************\n");
				*reg_point(MEM[op1]) = div_mod(*reg_point(MEM[op2]), *reg_point(MEM[op3]),1);
				break;
			case 6:
				printf("\n************** Inside mod instruction. **************\n");
				printf("\n************** It might take more time depending on input **************\n");
				*reg_point(MEM[op1]) = div_mod(*reg_point(MEM[op2]), *reg_point(MEM[op3]),0);
				break;
			case 7:
				printf("\n **************Inside push instruction. **************\n");
				push(*reg_point(MEM[op1]));
				break;
			case 8:
				printf("\n************** Inside pop instruction. **************\n");
				*reg_point(MEM[op1])=pop();
				break;
			case 9:
				printf("\n************** Inside jump instruction. **************\n");
				address = make_address(MEM[op2], MEM[op3]);
				pc = address;
				flag=1;
				break;
			case 10:
				printf("\n************** Inside jump register instruction. **************\n");
				pc=*reg_point(MEM[op1]);
				flag=1;
				break;
			case 11:
				printf("\n************** Inside branch on equal instruction. **************\n");
				
				equal = branchEqual(*reg_point(MEM[op1]), *reg_point(MEM[op2]));
				if(equal)
				{
					pc= pc + op3;
					flag=1;
				}
				break;
			case 12:
				printf("\n************** Inside branch on not equal instruction. **************\n");

				equal = branchEqual(*reg_point(MEM[op1]), *reg_point(MEM[op2]));		
				if(!equal)
				{
					pc= pc + op3;
					flag=1;
				}
				break;

			case 13:
				printf("\n************** Inside set on less then instruction. **************\n");

				if(*reg_point(MEM[op2]) << *reg_point(MEM[op3]))
				{
					*reg_point(MEM[op1])=1;
				}
				else
				{
					*reg_point(MEM[op1])=0;
				}

				break;

			default:
				printf("\nWrong instruction.\n");
		}
		display();
		if(flag==0)
		{
			pc = pc + 4;
		}
		else
		{
			flag = 0;
		}
	}
}


int main()
{
	cpu_init();
	printf("\n************** Cpu state initially **************");
	display();
	assignment2();  //Stores predifined values in registers to demonstrate Assignment 2.
	printf("\n************** Storing predifined values in registers to demonstrate Assignment 2 **************");
	display();
	asm_read();
	printf("\n************** Cpu state after loading instructions into memory **************");
	display();
	main_execution();
	return 0;
}