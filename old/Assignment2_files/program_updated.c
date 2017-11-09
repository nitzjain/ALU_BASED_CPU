/* Assignment 1 "Best CPU Setup"
Group - SS05*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdint.h>


#define LINE_LEN 15
#define MEM_MAX 2047
#define INS_MAX 1023
#define OS_MAX 511
uint8_t MEM[MEM_MAX + 1];
int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
uint8_t flag;
uint16_t pc, bp, sp;

uint8_t lenben;

//Initializing memory, general purpose registers and special purpose registers.
void cpu_init()
{
	do
	{
		printf("\nMachine selection 1.Little Endian(Default) 2. Big Endian :");
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
	uint16_t ins = 512;
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
			MEM[ins++] = opc;
			MEM[ins++] = opr1;
			MEM[ins++] = opr2;
			MEM[ins++] = opr3;
		}
		else                    //Big endian
		{
			MEM[ins++] = opr3;
			MEM[ins++] = opr2;
			MEM[ins++] = opr1;
			MEM[ins++] = opc;
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

	printf("\n\nContent of instruction Memory:\n\n");
	printf("Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value\n");
	for (i = OS_MAX + 1, j = 1; i <= INS_MAX; i++, j++)
	{
		printf("%03x    ", i);
		printf("%02x    ", MEM[i]);
		if (j % 6 == 0)
			printf("\n");
	}

	printf("\n\nContent of Data Memory:\n\n");
	printf("Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value  Addr  Value\n");
	for (i = INS_MAX + 1, j = 1; i <= MEM_MAX; i++, j++)
	{
		printf("%03X    ", i);
		printf("%02x    ", MEM[i]);
		if (j % 6 == 0)
			printf("\n");
	}

	printf("\n\nContent of General Purpose Register:\n\n");
	printf("R0 = %x\tR1 = %x\tR2 = %x\tR3 = %x\tR4 = %x\tR5 = %x\t R6 = %x\t R7 = %x\n", r0, r1, r2, r3, r4, r5, r6, r7);
	printf("R8 = %x\tR9 = %x\tR10 = %x\tR11 = %x\tR12 = %x\tR13 = %x\tR14 = %x\tR15 = %x\n", r8, r9, r10, r11, r12, r13, r14, r15);

	printf("\nContent of flag register:\t");

	for (bit = 0; bit<8; bit++)  //Printing flags from LSB to MSB
	{
		printf("%i ", flag & 0x01);
		flag = flag >> 1;
	}

	printf("\n\nContent of Program Counter:\t");
	printf("%04x\n", pc);
	printf("\nContent of Stack Pointer:\t");
	printf("%04x\n", sp);
	printf("\nContent of Base Pointer:\t");
	printf("%04x\n", bp);
	getch();
}

//Make some changes to memory and some register to demonstrate Assignment 1.
void assignment2()
{
	//MEM[1024] = 0x11;
	r1 = 0x0C;
	r2 = 0x0D;

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
		MEM[address++] = reg >> 24;
		MEM[address++] = reg >> 16;
		MEM[address++] = reg >> 8;
		MEM[address++] = reg;
	}
	else
	{
		MEM[address++] = reg;
		MEM[address++] = reg >> 8;
		MEM[address++] = reg >> 16;
		MEM[address++] = reg >> 24;
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
		printf("\nError in register read !");
		break;
	}
}
void push(int val)
{
	if (lenben == 1)
	{
		MEM[sp++] = val >> 24;
		MEM[sp++] = val >> 16;
		MEM[sp++] = val >> 8;
		MEM[sp++] = val;
	}
	else
	{
		MEM[sp++] = val;
		MEM[sp++] = val >> 8;
		MEM[sp++] = val >> 16;
		MEM[sp++] = val >> 24;
	}
}
int pop()
{
	int val = 0;
	if (lenben == 1)
	{
		val = MEM[sp--];
		val |= MEM[sp--] << 8;
		val |= MEM[sp--] << 16;
		val |= MEM[sp--] << 24;
	}
	else
	{
		val |= MEM[sp--] << 24;
		val |= MEM[sp--] << 16;
		val |= MEM[sp--] << 8;
		val = MEM[sp--];
	}
	return val;
}
int add(int a, int b)
{
	int na = a, nb = b;
	int carry;
	while (b != 0)
	{
		carry = a&b;
		a = a^b;
		b = carry << 1;
	}

	unsigned int sbita = ((unsigned)na >> 31) & 1, sbitb = ((unsigned)nb >> 31) & 1, sbitc = ((unsigned)a >> 31) & 1;
	//printf("signa = %d signb = %d and signc = %d\n", sbita, sbitb, sbitc);
	if (sbita == sbitb)
	{
		flag |= (sbita^sbitc) << 2;
		//overflow_bit = sbita^sbitc;
		printf("\noverflow flag: %d", sbita^sbitc);
	}
	else
		//overflow_bit = 0;
		flag &= ~(1 << 2);
	//printf("overflow bit for a as %d and b as %d = %d \n", na, nb, overflow_bit);
	return a;
}

int sub(int a, int b)
{
	int minusb = get2scomp(b);
	return(add(a, minusb));
}
int get2scomp(int c)
{
	return(add(~c, 1));
}
int mul(int a, int b)
{
	//printf("a is %d\n", a);
	int tempa = 0, over = 0;
	if (b<0)
	{
		a = get2scomp(a); b = get2scomp(b);
	}
	//printf("a is %d and b is %d\n", a,b);
	int sum = 0, n = 1;
	while (b >= n & over == 0)
	{
		if (b&n)
		{
			sum = add(sum, a);
		}
		//if (overflow_bit == 1)	
		if (flag & (1 << 2))
			break;
		tempa = a;
		a = a << 1;
		over = sub(a >> 31, tempa >> 31);
		//printf("over is %d \n", over);
		n = n << 1;
		if (b >= n & over != 0)
			//overflow_bit = 1;
			flag |= (1 << 2);
		else
			//overflow_bit = 0;
			flag &= ~(1 << 2);
	}
	return sum;
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
	}
	if (a>0)		rem = a;
	else rem = 0;
	printf("dividebyzero bit is %d \n", dividebyzero);
	if (mode)
	{
		if (sign == 1) quo = get2scomp(quo);
		return quo;
	}
	else
	{
		if (rem<0) rem = get2scomp(rem);
		return rem;
	}
}




void check_parity(int result)
{
	int cnt = 0;

	while ((uint8_t)result)
	{
		cnt += result & 1;
		result >>= 1;
	}
	if (cnt % 2)
		flag |= (1 << 2);	//set parity flag
	else
		flag &= ~(1 << 2);	//clear parity flag
}
//This function does the main execution. It reads opcode and operands from instruction memory and perform written steps.
void main_execution()
{

	uint16_t address;
	uint16_t op1, op2, op3;
	pc = 512;
	if (lenben == 2)
	{
		pc = pc + 3;
	}
	while (MEM[pc] != 0XFF)
	{
		if (lenben == 1)
		{
			op1 = pc + 1;
			op2 = pc + 2;
			op3 = pc + 3;
		}
		else
		{
			op1 = pc - 1;
			op2 = pc - 2;
			op3 = pc - 3;
		}
		switch (MEM[pc])
		{
			case 0://Case for loading data from memory to register.
				printf("\nInside load instruction.\n");
				address = make_address(MEM[op2], MEM[op3]);
				*reg_point(MEM[op1]) = load_reg(address);
				break;
			case 1://Case for storing data from register to memory.
				printf("\nInside store instruction.\n");
				address = make_address(MEM[op2], MEM[op3]);
				store_reg(*reg_point(MEM[op1]), address);
				break;
			case 2:
				printf("\nInside add instruction.\n");
				*reg_point(MEM[op1]) = add(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 3:
				printf("\nInside sub instruction.\n");
				*reg_point(MEM[op1]) = sub(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 4:
				printf("\nInside mul instruction.\n");
				*reg_point(MEM[op1]) = mul(*reg_point(MEM[op2]), *reg_point(MEM[op3]));
				break;
			case 5:
				printf("\nInside div instruction.\n");
				*reg_point(MEM[op1]) = div(*reg_point(MEM[op2]), *reg_point(MEM[op3]),1);
				break;
			case 6:
				printf("\nInside mod instruction.\n");
				*reg_point(MEM[op1]) = div(*reg_point(MEM[op2]), *reg_point(MEM[op3]),0);
				break;
			case 7:
				printf("\nInside push instruction.\n");
				push(*reg_point(MEM[op1]));
				break;
			case 8:
				printf("\nInside pop instruction.\n");
				*reg_point(MEM[op1])=pop();
				break;
			default:
				printf("\nWrong instruction.\n");
		}
		pc = pc + 4;
		display();
	}

}


int main()
{
	cpu_init();
	printf("\nCpu state initially");
	display();
	assignment2();  //Presetting values of memory location and register r1 for assignment no. 1
	
	display();
	asm_read();
	printf("\nCpu state after loading instructions into memory");
	display();
	main_execution();
	getch();
	return 0;
}