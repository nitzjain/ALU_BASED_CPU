/*

 * cmpe220aludesign.c

 *

 *  Created on: Oct 7, 2016

 *      Author: NSTV

 */


/*
Flags : 0 bit : Zero Flag
1 bit : Sign Flag
2 bit : OverFlow Flag
3 bit : Carry Flag
 */
/*
 opcodes: 0x00 : Load
   0x01 : Store
    0x02 : add
    0x03 : sub
    0x04 : mul
    0x05 : div
    0x06 : mod
    0x07 : lwi
    0x08 : mov
    0x20 : End
    0xFF : Invalid
 */

/*
 Location 1024-1027 is reserved
 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdint.h>

#include <stddef.h>

//#include <conio.h>



/*Defining the memory size and Register count*/

#define MEMSIZE 4095

#define REGCNT 8

#define OS_LIMIT 127

#define INST_LIMIT 1023

#define INST_LEN 20



/*Creating arrays to store the values of memory. Basic data type for

 * registers,flags and instruction pointer*/

uint8_t mem[MEMSIZE+1];

uint32_t r0,r1,r2,r3,r4,r5,r6,r7;

uint16_t flagreg=0x0000,imm;

uint32_t instptr=128,instaddr=128;

int inst_num,mem_addr,loc2,off,S,D,tempnum;

char userinst[INST_LEN];

char *loc1,*oper,*tempstr,*rb,*ri,*src,*dest,tempop[3];

uint32_t* reg_ptr,base_ptr;;

short reg_num;

uint32_t current_instr = 0x00000000;

uint8_t opcode = 0xFF;//can be changed to avoid default Load operation // 0x20 : end of instruction read..start reading from memory to execute the instructions

short dest_r,index_r,base_r;

uint32_t *base_p,*index_p ,*base_reg;





/*

Function declaration

 */



void init_memory();

void init_registers();

void display();


int add_mem(uint32_t*,uint32_t*,int,int);



void input_cmdline();

void loadreg(uint32_t*, int);
void loadregi(uint32_t*, int);

void storereg(uint32_t*, int);

void instruction_calc(uint8_t);

void start_exe();

void get_reg_num(char *reg);

uint32_t* call_reg_ptr(uint8_t);
int add_alu(int,int);
int sub_alu(int,int);
int mul_alu(int,int);
int div_alu(int,int);
int mod_alu(int,int);
int negation(int);
void flags(uint64_t);

//main method

int main(void){



	init_memory();

	printf("Memory loaded with initial values.\n");

	// Display initial memory values

	init_registers();

	printf("Registers loaded with initial values.\n");

	printf("\nPress Enter Key to Continue...\n");

	getchar();

	//display();





	while(1)

	{   instaddr = 128;

	printf("\nEnter an instruction number 1. Input 2. Display 3. Exit\n");

	scanf("%d",&inst_num);

	getchar();

	//Give Further instructions to the user





	switch(inst_num){

	case 1:

		printf("\nLoad or Store Instruction can be of the format-lw/sw reg,reg,reg,S,D\n");

		printf("\nArithmetic Instruction can be of the format- add/sub/mul/div/mod reg,reg\n");

		printf("\nLoad Immediate can be of the format- lw reg,$immediate\n");
		printf("\nPlease find the acceptable range values:\n\tReg: r0-r7\n\tS:1,2,4,8\n\tD:0,1,2,3,4\n");
		printf("\nEnter 'End' to start execution\n");


		do{

			input_cmdline();

			if(strcmp(oper, "lw")==0)

			{

				opcode = 0x00;



			}

			else if(strcmp(oper, "sw")==0)

			{

				opcode = 0x01;



			}

			else if(strcmp(oper, "add")==0)

			{

				opcode = 0x02;



			}

			else if(strcmp(oper, "sub")==0)

			{

				opcode = 0x03;



			}

			else if(strcmp(oper, "mul")==0)

			{

				opcode = 0x04;



			}

			else if(strcmp(oper, "div")==0)

			{

				opcode = 0x05;



			}

			else if(strcmp(oper, "mod")==0)

			{

				opcode = 0x06;



			}

			else if(strcmp(oper, "lwi")==0)

			{

				opcode = 0x07;



			}

			else if(strcmp(oper, "mov")==0)

			{

				opcode = 0x08;



			}


			else if(strcmp(oper, "End")==0)

			{

				opcode = 0x20;

			}

			else

			{

				opcode = 0xFF;

			}



			if (opcode == 0xFF)

			{

				printf("Invalid Instructions. Please start over\n");



			}

			else if(opcode == 0x20)

			{



				instruction_calc(opcode);

				//function execution starts here

				start_exe();

			}



			else

			{

				instruction_calc(opcode);



			}



		}while(!(strncmp(oper,"End",3)==0));

		break;



	case 2:

		printf("\nYou have selected to display the contents of the memory and registers\n");
		printf("Press Enter to continue\n");
		getchar();

		display();

		break;

	case 3:

		printf("\nYou have selected Exit. Exiting the program\n");

		exit(1);

		break;

	}

	}


	return 0;

}



// main exe function to read from instruction memory and execute.

//need to include all the functions corresponding to the opcodes defined

void start_exe()

{

	uint8_t mem_inst = 0xFF,temp, src1,src2,src3,src4,dest;
	uint16_t *imm_temp;
	uint32_t *dest_p,*src_p1,*src_p2,*src_p3,*src_p4;
	instptr=128;
	int (*fun_ptr_arr[])(int, int) = {add_alu, sub_alu, mul_alu,div_alu,mod_alu};

	uint32_t val1,val2;
	uint16_t imm_t;

	do{

		mem_inst = mem[instptr+3];

		switch(mem_inst)

		{

		case 0x00:

			printf("Load instruction is being performed. The corresponding opcode is 0x00\n");


			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			src1 = temp & 0x0F;
			src_p1 = call_reg_ptr(src1);

			temp = mem[instptr+1];
			src2 = temp & 0xF0;
			src2 = src2 >> 4;
			src_p2 = call_reg_ptr(src2);

			src3 = temp & 0x0F;

			src4 = mem[instptr];

			val2 = *src_p1+*src_p2*src3+src4;


			loadreg(dest_p,val2);

			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);


			break;

		case 0x01:

			printf("Store instruction is being performed. The corresponding opcode is 0x01\n");

			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			src1 = temp & 0x0F;
			src_p1 = call_reg_ptr(src1);

			temp = mem[instptr+1];
			src2 = temp & 0xF0;
			src2 = src2 >> 4;
			src_p2 = call_reg_ptr(src2);

			src3 = temp & 0x0F;

			src4 = mem[instptr];

			val2 = *src_p1+*src_p2*src3+src4;


			storereg(dest_p,val2);
			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x02:

			printf("Add instruction is being performed. The corresponding opcode is 0x02\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;



			*dest_p = (*fun_ptr_arr[0])(val1,val2);
			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);


			break;

		case 0x03:

			printf("Sub instruction is being performed. The corresponding opcode is 0x03\n");
			temp = mem[instptr+2];
			printf("%d\n",temp);

			dest = temp & 0xF0;
			dest = dest >> 4;

			printf("%d\n",dest);
			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;
			printf("%d\t%d\n",val1,val2);


			*dest_p = (*fun_ptr_arr[1])(val1,val2);

			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x04:

			printf("Mul instruction is being performed. The corresponding opcode is 0x04\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;

			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;


			*dest_p = (*fun_ptr_arr[2])(val1,val2);

			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x05:

			printf("DIV instruction is being performed. The corresponding opcode is 0x05\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;

			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;


			*dest_p = (*fun_ptr_arr[3])(val1,val2);

			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x06:

			printf("mod instruction is being performed. The corresponding opcode is 0x06\n");

			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;

			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;


			*dest_p = (*fun_ptr_arr[4])(val1,val2);
			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x07:


			printf("load Immediate instruction is being performed. The corresponding opcode is 0x07\n");


			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);


			src1 = temp & 0x0F ;
			temp = mem[instptr+1];

			imm_t = src1 << 8;
			imm_t |= temp;
			loadregi(dest_p,imm_t);

			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

			break;

		case 0x08:

			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			src1 = temp & 0x0F;
			src_p1 = call_reg_ptr(src1);


			val1 = *dest_p;
			val2 = *src_p1;

			storereg(src_p1,1024);
			loadreg(dest_p,1024);
			printf("\n\nThe register values are as follows:\n\n");

			printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

			printf("\nThe flag value is:0x%.4x\n",flagreg);

			printf("\nThe instruction pointer value is:0x%.8x\n",instptr);



			break;


		}


		instptr +=4;

	}while(mem_inst != 0x20);



}



void instruction_calc(uint8_t opcode)

{

	int j,in;

	uint8_t temp;

	if ((opcode == 0x01) || (opcode == 0x00))
	{


		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) dest_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) base_r<< 16); //4 bit reg_num2

		current_instr += ((uint32_t) index_r << 12); //4 bit reg_num1

		current_instr += (S << 8); //4 bit scaling factor


		current_instr += D; //8 bit memory address //offset
		printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);


	}

	else if (opcode == 0x20)
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode
		//printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);


	}
	else if ((opcode == 0x07))
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) base_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) imm << 8); //12 bit immediate value


		printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);


	}

	else
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) base_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) index_r<< 16); //4 bit reg_num2


		printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);

	}

	in = 0;

	for(j=1;j<=4;j++)

	{

		temp = current_instr >> in;

		in +=8;

		mem[instaddr+j-1] = temp ;



	}

	instaddr += 4;

}



void input_cmdline()

{

	char *load_str;



	fgets(userinst,INST_LEN,stdin);

	int len = strlen(userinst);

	load_str=strdup(userinst);



	userinst[len-1]='\0';

	strncpy(tempop,userinst,2);

	tempop[2]='\0';



	if((strncmp(tempop,"lw",2)==0||strncmp(tempop,"sw",2)==0) && (strchr(userinst,'$')==NULL)){



		tempnum=0;



		while((tempstr=strsep(&load_str," ,"))!=NULL){

			if(tempnum==0){

				oper=tempstr;



			}

			else if(tempnum==1){




				get_reg_num(tempstr);

				base_reg = reg_ptr;

				dest_r = reg_num;

			}

			else if(tempnum==2){




				get_reg_num(tempstr);

				base_p = reg_ptr;


				base_r = reg_num;



			}

			else if(tempnum==3){


				get_reg_num(tempstr);

				index_p = reg_ptr;

				index_r = reg_num;


			}

			else if(tempnum==4){

				if(tempstr==NULL){

					S=0;

				}

				else{

					S= atoi(tempstr);

				}

			}

			else if(tempnum==5){

				if(tempstr==NULL){

					D=0;

				}

				else{

					D= atoi(tempstr);

				}

			}

			tempnum++;



		}



		mem_addr = add_mem(base_p,index_p,S,D);

	}

	else if(strchr(userinst,'$')!=NULL){

		oper=strtok(userinst," ");
		tempstr= strtok(NULL," ");

		if(strncmp(tempop,"lw",2)==0)
			oper="lwi";
		if(strncmp(tempop,"sw",2)==0)
			oper="swi";

		loc1=strtok(tempstr,",$");
		get_reg_num(loc1);
		base_p = reg_ptr;
		base_r = reg_num;
		tempstr= strtok(NULL,",$");

		imm=atoi(tempstr);

	}

	else if(strchr(userinst,'E')!=NULL)

	{

		oper = "End";

		//printf("")

	}

	else{

		oper = strtok(userinst," ");

		tempstr= strtok(NULL," ");



		dest=strtok(tempstr,",");

		get_reg_num(dest);

		base_p = reg_ptr;

		base_r = reg_num;



		tempstr= strtok(NULL,",");

		src=tempstr;

		get_reg_num(src);

		index_p = reg_ptr;

		index_r = reg_num;



		S=0;

		D=0;


	}
}


uint32_t *call_reg_ptr(uint8_t tem)
{
	uint32_t *regstr_ptr;
	switch(tem)
	{
	case 0x00:
		regstr_ptr = &r0;
		break;
	case 0x01:
		regstr_ptr = &r1;
		break;
	case 0x02:
		regstr_ptr = &r2;
		break;
	case 0x03:
		regstr_ptr = &r3;
		break;
	case 0x04:
		regstr_ptr = &r4;
		break;
	case 0x05:
		regstr_ptr = &r5;
		break;
	case 0x06:
		regstr_ptr = &r6;
		break;
	case 0x07:
		regstr_ptr = &r7;
		break;


	}
	return regstr_ptr;
}


void loadreg(uint32_t* reg_num,int memory_addr )

{

	uint32_t old_reg,x=0;

	old_reg = *reg_num;

	*reg_num = 0x00000000;



	for(x=4;x>0;x--)

	{

		*reg_num = *reg_num << 8;

		*reg_num= *reg_num + mem[memory_addr+x-1];

	}




	printf("Register changed from: 0x%.8x to: 0x%.8x\n",old_reg,*reg_num);


}

void loadregi(uint32_t* reg_num,int memory_addr )

{

	uint32_t old_reg,x=0;

	old_reg = *reg_num;

	*reg_num = 0x00000000;

	*reg_num = memory_addr;




	printf("Register changed from: 0x%.8x to: 0x%.8x\n",old_reg,*reg_num);


}


void storereg(uint32_t* reg_num, int memory_addr)

{

	int x,y=8;

	uint8_t temp;


	temp= *reg_num;


	for(x=1;x<=4;x++)

	{

		mem[memory_addr+x-1] =  temp;

		temp = *reg_num >> y;

		y=y+8;

	}




	printf("Memory Address: 0x%x-0x%x changed to: 0x%.2x%.2x%.2x%.2x\n",memory_addr,memory_addr+3,mem[memory_addr+3],mem[memory_addr+2],mem[memory_addr+1],mem[memory_addr]);

}

int add_alu(int r1, int r2)

{


	uint64_t la = 0,lb=0;

	la = r1; lb = r2;



	uint64_t carry=0;

	while (lb != 0)

	{

		carry = la&lb;

		la = la^lb;

		lb = carry << 1;
		//printf("\n\nCarry is%d",carry);

	}


	int res= (int)la;

	//1. carry flag

	carry = la>>32;
	carry &= 1;
	if (carry==1)

	{
		flagreg |= (1 << 3);	//set carry flag

		printf("carry Flag is set\n");}

	else

		flagreg &= ~(1 << 3);




	uint32_t overflow=la>>32;

	if (overflow >= 1)

	{

		flagreg |= (1 << 2);	//set overflow flag


	}

	else

	{

		flagreg &= ~(1 << 2);	//clear overflow flag

	}



	if (res == 0)

	{flagreg |= (1 << 0);	//set zero flag


	}

	else

		flagreg &= ~(1 << 0);



	printf("the flag register is %d\n ", flagreg);





	return (la);



}

void flags(uint64_t lresult)



{

	int result= (int)lresult;

	uint32_t overflow=lresult>>32;

	if (overflow >= 1)

	{

		flagreg |= (1 << 2);	//set overflow flag

		//printf("overflow Flag is set\n");

	}

	else

	{

		flagreg &= ~(1 << 2);	//clear overflow flag

	}



	if (result<0)

	{flagreg |= (1 << 1);	//set sign flag

	}

	else

		flagreg &= ~(1 << 1);



	if (result == 0)

	{flagreg |= (1 << 0);	//set zero flag


	}

	else

		flagreg &= ~(1 << 0);




}



//Subtraction: A - B = A + (-B) = A + ~B + 1

int negation(int r1){



	return add_alu(~r1,1);



}



int sub_alu(int r1,int r2){



	int nr2=negation(r2);

	uint64_t result = add_alu(r1, nr2);

	flags(result);


	return(result);


}





//overflow = For unsigned addition, overflow occurs when the carry out is 1





int mul_alu(int r1, int r2)

{



	int i=0;

	uint64_t lres = 0;


	for(i=0;i< 32; i++)

	{

		if(r1 & 1 << i)

			lres= lres + (r2<<i);

	}


	flags(lres);




	int res=(int)lres;
	printf("the flag register is %d\n ", res);

	return res;

}



//Binary multiplication is the same as repeated binary addition; add the multicand to itself the multiplier number of times.

//In multiplication no carry bit as 1*1=1



//Binary division is the repeated process of subtraction, just as in decimal division.



int div_alu(int a,int b)

{

	int quo=0;

	if (b>a) {

		printf("divisor greater than dividend\n");

		flags(a);

		return 0;}

	if(b!=0){

		while (a >= b)

		{

			a = sub_alu(a, b);

			quo = add_alu(quo, 1);


		}

	}


	flags(quo);

	return quo;

}

int mod_alu(int a, int b)

{

	int quo=0;

	if (b>a) {

		printf("divisor greater than dividend\n");

		flags(a);

		return a;

	}

	if(b!=0){

		while (a >= b){

			a = sub_alu(a, b);

			quo = add_alu(quo, 1);

		}

	}




	flags(a);

	return a;





}

//=============================================

//Selecting the register to pass to Load/Store Method

void get_reg_num(char *reg)

{



	if(strcmp(reg, "r0")==0)

	{

		reg_num = 0x0;

		reg_ptr = &r0;

	}

	else if(strcmp(reg, "r1")==0)

	{

		reg_num = 0x1;

		reg_ptr = &r1;

	}

	else if(strcmp(reg, "r2")==0)

	{

		reg_num = 0x2;

		reg_ptr = &r2;

	}

	else if(strcmp(reg, "r3")==0)

	{

		reg_num = 0x3;

		reg_ptr = &r3;

	}

	else if(strcmp(reg, "r4")==0)

	{

		reg_num = 0x4;

		reg_ptr = &r4;

	}

	else if(strcmp(reg, "r5")==0)

	{

		reg_num = 0x5;

		reg_ptr = &r5;

	}

	else if(strcmp(reg, "r6")==0)

	{

		reg_num = 0x6;

		reg_ptr = &r6;

	}

	else if(strcmp(reg, "r7")==0)

	{

		reg_num = 0x7;

		reg_ptr = &r7;

	}

	else

	{

		printf("invalid register value, please re-enter\n");



	}

}





//Function to initialize memory locations to 1

void init_memory(){

	int i,temp;

	for(i=0;i<=OS_LIMIT;i++){

		//mem[i]=0x00000000;

		temp = rand();

		mem[i]=temp;



	}



	for(i=OS_LIMIT+1;i<MEMSIZE+1;i++){

		temp = rand();

		mem[i]=temp;

	}

	// Only for assignment: Allocating memory

	mem[2300]=0xab;

	mem[2301]=0xcd;

	mem[2302]=0xef;

	mem[2303]=0xff;



}



//Function to initialize register values to 0

void init_registers(){

	r0=r1=r2=r3=r4=r5=r6=r7=0x00000000;


}



//Add Offset and Memory to get Final Memory Address

// Incase instruction evaluated < 1024 . then it is a problem.

int add_mem(uint32_t *rb, uint32_t *ri,int S, int D)

{

	int temp;

	temp= *rb+*ri*S+D;

	if (temp>4095)

	{

		printf("Memory Overflow\nPlease retry\n");

		return 0;

	}

	//	else if(temp<1024)
	//	{
	//		printf("Protected memory cannot be modified.\nPlease retry\n");

	//		return 0;

	//}

	return temp;

}





//Function to display values at a given time

void display()

{

	int x=0,y=0;

	printf("\n\nValues in Instruction Memory\n\n");

	printf("Loc\tValue\tLoc\tValue\tLoc\tValue\tLoc\tValue\n");



	for(x=OS_LIMIT+1,y=1;x<INST_LIMIT+1;x=x+4,y++)

	{

		printf("0x%x\t%.2x%.2x%.2x%.2x ",x,mem[x+3],mem[x+2],mem[x+1],mem[x]);

		if(y%4==0){

			printf("\n");

		}

	}



	getchar();

	printf("\n\nValues in Main Memory\n\n");

	printf("Loc\tValue\tLoc\tValue\tLoc\tValue\tLoc\tValue\n");





	for(x=INST_LIMIT+1,y=1;x<MEMSIZE+1;x=x+4,y++){



		printf("0x%x\t%.2x%.2x%.2x%.2x ",x,mem[x+3],mem[x+2],mem[x+1],mem[x]);

		if(y%4==0){

			printf("\n");

		}

	}



	printf("\n\nThe register values are as follows:\n\n");

	printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);

	printf("\nThe flag value is:0x%.4x\n",flagreg);

	printf("\nThe instruction pointer value is:0x%.8x\n",instptr);



}
