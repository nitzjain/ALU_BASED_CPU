/*

 * cmpe220aludesign.c

 *

 *  Created on: Oct 24, 2016

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
    0x07 : lw
    0x08 : mov
    0x09 : lea
    0x0A : push
    0x0B : pop
    0x0C : slt
    0x0D : jmp
    0x0E : jr
    0x0F : beq
    0x10 : bne
    0x20 : End
    0xFF : Invalid
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "alu.h"
#include "move.h"


/*Defining the memory size and Register count*/


#define REGCNT 8
#define OS_LIMIT 127
#define INST_LIMIT 1023
#define INST_LEN 25




/*Creating arrays to store the values of memory. Basic data type for
 * registers,flags and instruction pointer*/


uint32_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15;
uint32_t instptr=128,instaddr=128;
int inst_num,mem_addr,loc2,off,S,D,tempnum;
char userinst[INST_LEN];
char *loc1,*oper,*tempstr,*rb,*ri,*src,*dest,tempop[3];
uint32_t* reg_ptr,base_ptr;
short reg_num;
uint32_t current_instr = 0x00000000;
uint8_t opcode = 0xFF;//can be changed to avoid default Load operation // 0x20 : end of instruction read..start reading from memory to execute the instructions
short dest_r,index_r,base_r;
uint32_t *base_p,*index_p ,*base_reg;
//uint32_t base_ptr = STACK_MIN, stack_ptr = STACK_MIN;
char *src1, *lblstr;
uint16_t lbl_addr[10] = {0,0,0,0,0,0,0,0,0,0};
short lbl_idx;
FILE *fPointer;


/*

Function declaration

 */

void init_memory();
void init_registers();
void display();
void display_inst();
int add_mem(uint32_t*,uint32_t*,int,int);
void input_cmdline();
void loadreg(uint32_t*, int);
void loadregi(uint32_t*, int);
void storereg(uint32_t*, int);
void instruction_calc(uint8_t);
void start_exe();
void get_reg_num(char *reg);
uint32_t* call_reg_ptr(uint8_t);
int add_alu(uint32_t,uint32_t);
int sub_alu(int,int);
int mul_alu(int,int);
int div_alu(int,int);
int mod_alu(int,int);
int negation(int);
void push(uint32_t);
uint32_t pop();
void assignment3();

//main method
void assignment3()
{
	int num,inp,i;
	uint32_t *dest_p;
	printf("\n*** Binary search ***\nEnter number of elements: ");
	scanf("%d", &num);
	r12 = num;
	printf("\nEnter %d elements in ascending order: ",num);
	for (i = 0; i < num; i++)
	{
		scanf("%d", &inp);
		r9=inp;
		dest_p=call_reg_ptr(9);
		storereg(dest_p,4000+4*i);	//storing the user enetered elements in data memory
	}
	printf("\nEnter 'key' element you need to find: ");
	scanf("%d", &num);
	printf("\nBinary search function will find %d in the array ! \nThe result will be in register r14 !",num);
	r10 = num;							//storing key element in register r10
	r11 = 0;							//storing starting index '0' in register r11
	getchar();
}

int main(void){
	init_memory();
	printf("Memory loaded with initial values.\n");
	// Display initial memory values
	init_registers();
	printf("Registers loaded with initial values.\n");
	printf("\nPress Enter Key to Continue...\n");
	getchar();



	while(1)
	{   instaddr = 128;
	printf("\nEnter an instruction number 1. Input 2. Display 3. Exit\n");
	scanf("%d",&inst_num);
	getchar();


	//Give Further instructions to the user
switch(inst_num){
	case 1:
		printf("\nLoad or Store Instruction can be of the format-lw/sw/lea reg,reg,reg,S,D\n");
		printf("\nLoad Immediate can be of the format- lw reg,$immediate\n");
		printf("\nArithmetic/Move Instruction can be of the format- add/sub/mul/div/mod/mov reg,reg\n");
		printf("\nJump Instruction can be of the format- jmp lbl_name\n");
		printf("\nJump Register Instruction can be of the format- jr reg\n");
		printf("\nStack Operation's Instruction can be of the format- push/pop reg");
		printf("\nSet Less than Operation Instruction can be of the format- slt reg,reg,reg");
		printf("\nBranch Operation's Instruction can be of the format- beq/bne reg,reg,lbl_name");
		printf("\nPlease find the acceptable range values:\n\tReg: r0-r15\n\tS:1,2,4,8\n\tD:0,1,2,3,4\n");
		printf("\nEnter 'End' to start execution\n");
		fPointer = fopen("test.txt","r");
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

			else if(strcmp(oper, "lea")==0)

			{

				opcode = 0x09;

			}

			else if(strcmp(oper, "push")==0)

			{

				opcode = 0x0A;

			}
			else if(strcmp(oper, "pop")==0)

			{

				opcode = 0x0B;

			}
			else if(strcmp(oper, "slt")==0)

			{

				opcode = 0x0C;

			}
			else if(strcmp(oper, "jmp")==0)

			{

				opcode = 0x0D;

			}
			else if(strcmp(oper, "jr")==0)

			{

				opcode = 0x0E;

			}
			else if(strcmp(oper, "beq")==0)

			{

				opcode = 0x0F;

			}
			else if(strcmp(oper, "bne")==0)

			{

				opcode = 0x10;

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
				printf("\nProgram Loaded\n");
				getchar();
				//assignment3();
				start_exe();
				display_inst();

		}



			else
			{
			instruction_calc(opcode);
			}

		}while(!(strncmp(oper,"End",3)==0));
		fclose(fPointer);
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
			flagreg=0x0000;
/*			display_inst();
			getchar();
	*/
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
			flagreg=0x0000;
			/*display_inst();
				getchar();
*/
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

/*
			display_inst();
				getchar();
*/

			break;

		case 0x03:

			printf("Sub instruction is being performed. The corresponding opcode is 0x03\n");
			temp = mem[instptr+2];

			dest = temp & 0xF0;
			dest = dest >> 4;

			//printf("%d\n",dest);
			src1 = temp & 0x0F;

			dest_p = call_reg_ptr(dest);
			src_p1 = call_reg_ptr(src1);
			val1 = *dest_p;
			val2 = *src_p1;
			//printf("%d\t%d\n",val1,val2);


			*dest_p = (*fun_ptr_arr[1])(val1,val2);

/*
			display_inst();
				getchar();
*/

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

/*
			display_inst();
				getchar();
*/

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

/*
			display_inst();
				getchar();
*/

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
/*
			display_inst();
				getchar();
*/
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
			flagreg=0x0000;
/*
			display_inst();
				getchar();
*/


			break;

		case 0x08:
			printf("mov instruction is being performed. The corresponding opcode is 0x08\n");

			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			src1 = temp & 0x0F;
			src_p1 = call_reg_ptr(src1);

			*dest_p = *src_p1;

			flagreg=0x0000;
/*
			display_inst();
							getchar();
*/


			break;

		case 0x09:

			printf("Lea instruction is being performed. The corresponding opcode is 0x09\n");


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

			*dest_p = val2;
			flagreg=0x0000;
/*
			display_inst();
							getchar();
*/



			break;
		case 0x0A:

			printf("Push instruction is being performed. The corresponding opcode is 0x0A\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			val1 = *dest_p;

			push(val1);
			flagreg=0x0000;
/*
			display_inst();
							getchar();
*/

			break;

		case 0x0B:
			printf("Pop instruction is being performed. The corresponding opcode is 0x0B\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);
			val1 = pop(dest_p);
			flagreg=0x0000;
/*
			display_inst();
							getchar();
*/


			break;
		case 0x0C:

			printf("SLT instruction is being performed. The corresponding opcode is 0x0C\n");


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


			val1 = *src_p1;
			val2 = *src_p2;


			val1 = (*fun_ptr_arr[1])(val1,val2);

			if(flagreg & 0x2)
			{
				*dest_p = 1;
			}
			else
			{
				*dest_p = 0;
			}

/*
			display_inst();
							getchar();
*/



			break;

		case 0x0D:
			printf("jump instruction is being performed. The corresponding opcode is 0x0D\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;

			instptr = lbl_addr[dest]-4;
			//*dest_p = val1;
			//printf("\ndest_p : %u, *dest_p : %u, val1 : %u, &r3 : %u",dest_p,*dest_p,val1,&r3);

			flagreg=0x0000;
/*
			display_inst();
							getchar();
*/



			break;


		case 0x0E:
			printf("jr instruction is being performed. The corresponding opcode is 0x0E\n");
			temp = mem[instptr+2];
			dest = temp & 0xF0;
			dest = dest >> 4;
			dest_p = call_reg_ptr(dest);

			instptr = *dest_p - 4;
			flagreg=0x0000;

/*
			display_inst();
							getchar();

*/


			break;

		case 0x0F:
					printf("beq instruction is being performed. The corresponding opcode is 0x0F\n");
					temp = mem[instptr+2];

					dest = temp & 0xF0;
					dest = dest >> 4;
					dest_p = call_reg_ptr(dest);

					src1 = temp & 0x0F;
					src_p1 = call_reg_ptr(src1);
					temp = mem[instptr+1];
								src2 = temp & 0xF0;
								src2 = src2 >> 4;


					val1 = *dest_p;
					val2 = *src_p1;
					*dest_p = (*fun_ptr_arr[1])(val1,val2);

					if(flagreg & 0x1)
					{
						instptr = lbl_addr[src2]-4;
					}


/*

					display_inst();
									getchar();

*/


					break;
		case 0x10:
							printf("bne instruction is being performed. The corresponding opcode is 0x10\n");
							temp = mem[instptr+2];

							dest = temp & 0xF0;
							dest = dest >> 4;
							dest_p = call_reg_ptr(dest);

							src1 = temp & 0x0F;
							src_p1 = call_reg_ptr(src1);
							temp = mem[instptr+1];
										src2 = temp & 0xF0;
										src2 = src2 >> 4;


							val1 = *dest_p;
							val2 = *src_p1;
							*dest_p = (*fun_ptr_arr[1])(val1,val2);

							if(!(flagreg & 0x1))
							{
								instptr = lbl_addr[src2]-4;
							}

/*
							display_inst();
							getchar();
*/



							break;
		}


		instptr +=4;
r9=instptr;
	}while(mem_inst != 0x20);



}



void instruction_calc(uint8_t opcode)

{

	int j,in;

	uint8_t temp;

	if ((opcode == 0x01) || (opcode == 0x00) || (opcode == 0x09))
	{


		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) dest_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) base_r<< 16); //4 bit reg_num2

		current_instr += ((uint32_t) index_r << 12); //4 bit reg_num1

		current_instr += (S << 8); //4 bit scaling factor


		current_instr += D; //8 bit memory address //offset
		//printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);


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


		//printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);


	}

	else if ((opcode == 0x0A)||(opcode == 0x0B)||(opcode == 0x0E))
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) base_r << 20); //4 bit reg_num1

	}
	else if (opcode == 0x0D)
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) lbl_idx << 20); //4 bit reg_num1

	}


	else if (opcode == 0x0C)
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) dest_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) base_r<< 16); //4 bit reg_num2

		current_instr += ((uint32_t) index_r << 12); //4 bit reg_num1

	}
	else if ((opcode == 0x0F)||(opcode == 0x10))
		{

			current_instr = ((uint32_t) opcode << 24); //8 bit opcode

			current_instr += ((uint32_t) dest_r << 20); //4 bit reg_num1

			current_instr += ((uint32_t) base_r<< 16); //4 bit reg_num2

			current_instr += ((uint32_t) lbl_idx << 12); //4 bit reg_num1

		}


	else
	{

		current_instr = ((uint32_t) opcode << 24); //8 bit opcode

		current_instr += ((uint32_t) base_r << 20); //4 bit reg_num1

		current_instr += ((uint32_t) index_r<< 16); //4 bit reg_num2


		//printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);

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



void input_cmdline(){
	char *load_str;
	//fgets(userinst,INST_LEN,stdin);
	fgets(userinst,INST_LEN,fPointer);
	//printf("userinst is %s \n",userinst);
	int len = strlen(userinst);

	userinst[len-1]='\0';

	if(strncmp(userinst,"lb",2)==0 && strchr(userinst,':')!=NULL){
		lblstr=strtok(userinst,":");
		//------------------Stores the InstPtr value for label----------------//
		lbl_idx=(lblstr[2]-'0');
		lbl_addr[lbl_idx]=instaddr;
		//		printf("\nThe Instruction pointer value set for label %d is:0x%x\n",lbl_idx,lbl_addr[lbl_idx]);
		//--------------------------------------------------------------------//
		tempstr= strtok(NULL,":");
		strcpy(userinst,tempstr);
	}

	load_str=strdup(userinst);
	strncpy(tempop,userinst,2);
	tempop[2]='\0';

	//=========================LW,SW,LEA Operations==================================//
	if((strncmp(tempop,"lw",2)==0||strncmp(tempop,"sw",2)==0||strncmp(userinst,"lea",3)==0) && (strchr(userinst,'$')==NULL)){
		tempnum=0;
		while((tempstr=strsep(&load_str," ,"))!=NULL){
			if(tempnum==0){
				oper=tempstr;
				//printf("\nOper:%s\n",oper);
			}
			else if(tempnum==1){
				//printf("\nReg1:%s\n",tempstr);
				get_reg_num(tempstr);
				base_reg = reg_ptr;
				dest_r = reg_num;
			}
			else if(tempnum==2){
				//printf("\nReg2:%s\n",tempstr);
				get_reg_num(tempstr);
				base_p = reg_ptr;
				base_r = reg_num;
			}
			else if(tempnum==3){
				//printf("\nReg3:%s\n",tempstr);
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
				//printf("\nS:%d\n",S);
			}
			else if(tempnum==5){
				if(tempstr==NULL){
					D=0;
				}
				else{
					D= atoi(tempstr);
				}
				//printf("\nD:%d\n",D);
			}
			tempnum++;
		}
		mem_addr = add_mem(base_p,index_p,S,D);
	}
	//=========================LW Immediate Operation==================================//
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
	//=========================End Operation==================================//
	else if(strchr(userinst,'E')!=NULL){
		oper = "End";
	}
	//=========================PUSH,POP Operations==================================//
	else if(strncmp(userinst,"push",4)==0||strncmp(userinst,"pop",3)==0){
		oper=strtok(userinst," ");
		tempstr= strtok(NULL," ");
		if(strcmp(oper,"pop")==0){
			get_reg_num(tempstr);
			base_p = reg_ptr;
			base_r = reg_num;
		}
		else{
			get_reg_num(tempstr);
			base_p = reg_ptr;
			base_r = reg_num;
		}
	}
	//=========================Set Less Than Operations==================================//
	else if(strncmp(userinst,"slt",3)==0){
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
			else{
				get_reg_num(tempstr);
				index_p = reg_ptr;
				index_r = reg_num;
			}
			tempnum++;
		}
	}
	//=========================BEQ/BNE Operations==================================//
	else if(strncmp(userinst,"beq",3)==0||strncmp(userinst,"bne",3)==0){
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
			else{
				lblstr=tempstr;
				//------------------Retrieves the InstPtr value for label----------------//
				lbl_idx=(lblstr[2]-'0');
			}
			tempnum++;
		}
	}
	//=========================JUMP/ JUMP REG Operations==================================//
	else if(strncmp(userinst,"jmp",3)==0||strncmp(userinst,"jr",2)==0){
		oper=strtok(userinst," ");
		tempstr= strtok(NULL," ");

		if(strcmp(oper,"jmp")==0){
			lblstr=tempstr;
			//------------------Retrieves the InstPtr value for label----------------//
			lbl_idx=(lblstr[2]-'0');

		//	printf("\nThe Instruction pointer value for label %d is:0x%x\n",lbl_idx,lbl_addr[lbl_idx]);
			//----------------------------------------------------------------------//
		}
		else if(strcmp(oper,"jr")==0){
			get_reg_num(tempstr);
			//printf("\ntempstr in jr blck is:%s\n",tempstr);
			base_p = reg_ptr;
			base_r = reg_num;
		}
	}

	//=========================Arithmetic Operations==================================//
	else{
		oper = strtok(userinst," ");
	//	printf("\nOper is:%s\n",oper);
		tempstr= strtok(NULL," ");

		dest=strtok(tempstr,",");
	//	printf("\nDest:%s\n",tempstr);
		get_reg_num(dest);
		base_p = reg_ptr;
		base_r = reg_num;

		tempstr= strtok(NULL,",");
		src=tempstr;
	//	printf("\nSrc:%s\n",tempstr);
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
	case 0x08:
		regstr_ptr = &r8;
		break;
	case 0x09:
		regstr_ptr = &r9;
		break;
	case 0x0A:
		regstr_ptr = &r10;
		break;
	case 0x0B:
		regstr_ptr = &r11;
		break;
	case 0x0C:
		regstr_ptr = &r12;
		break;
	case 0x0D:
		regstr_ptr = &r13;
		break;
	case 0x0E:
		regstr_ptr = &r14;
		break;
	case 0x0F:
		regstr_ptr = &r15;
		break;

	}
	return regstr_ptr;
}


//Selecting the register to pass to Load/Store Method

void get_reg_num(char *reg)

{


	if(strncmp(reg, "r0",2)==0)

	{

		reg_num = 0x0;

		reg_ptr = &r0;

	}

	else if(strncmp(reg, "r2",2)==0)

	{

		reg_num = 0x2;

		reg_ptr = &r2;

	}

	else if(strncmp(reg, "r3",2)==0)

	{

		reg_num = 0x3;

		reg_ptr = &r3;

	}

	else if(strncmp(reg, "r4",2)==0)

	{

		reg_num = 0x4;

		reg_ptr = &r4;

	}

	else if(strncmp(reg, "r5",2)==0)

	{

		reg_num = 0x5;

		reg_ptr = &r5;

	}

	else if(strncmp(reg, "r6",2)==0)

	{

		reg_num = 0x6;

		reg_ptr = &r6;

	}

	else if(strncmp(reg, "r7",2)==0)

	{

		reg_num = 0x7;

		reg_ptr = &r7;

	}
	else if(strncmp(reg, "r8",2)==0)

	{

		reg_num = 0x8;

		reg_ptr = &r8;

	}
	else if(strncmp(reg, "r9",2)==0)

	{

		reg_num = 0x9;

		reg_ptr = &r9;

	}
	else if(strncmp(reg, "r10",3)==0)

	{

		reg_num = 0xA;

		reg_ptr = &r10;

	}
	else if(strncmp(reg, "r11",3)==0)

	{

		reg_num = 0xB;

		reg_ptr = &r11;

	}
	else if(strncmp(reg, "r12",3)==0)

	{

		reg_num = 0xC;

		reg_ptr = &r12;

	}

	else if(strncmp(reg, "r13",3)==0)

	{

		reg_num = 0xD;

		reg_ptr = &r13;

	}
	else if(strncmp(reg, "r14",3)==0)

	{

		reg_num = 0xE;

		reg_ptr = &r14;

	}

	else if(strncmp(reg, "r15",3)==0)

	{

		reg_num = 0xF;

		reg_ptr = &r15;

	}

	else if(strncmp(reg, "r1",2)==0)

	{

		reg_num = 0x1;

		reg_ptr = &r1;

	}



	else

	{

		printf("invalid register value, please re-enter\n");



	}
//	printf("\n-------------------------------------------------------------regnum:0x%x\tregptr:%d\n",reg_num,*reg_ptr);

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

	//r0=0xffffffff;//only for assignment 2 purpose
	r0=r1=r2=r3=r4=r5=r6=r7=0x00000000;


}


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



	display_inst();


}


void display_inst()
{
	int x,y;
	printf("\n===========================================================\n");

	printf("\n\nThe register values are as follows:\n\n");

	printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\tr8:0x%x\tr9:0x%x\tr10:0x%x\tr11:0x%x\tr12:0x%x\tr13:0x%x\tr14:0x%x\tr15:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15);

	printf("\nThe flag value is:0x%.4x\n",flagreg);

	printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

	printf("\nThe stack memory is:\n");

	for(x=3800,y=1;x<3840;x=x+4,y++){



			printf("0x%x\t%.2x%.2x%.2x%.2x ",x,mem[x+3],mem[x+2],mem[x+1],mem[x]);

			if(y%4==0){

				printf("\n");

			}

		}



	printf("\n===========================================================\n");

}


