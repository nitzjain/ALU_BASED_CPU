/*
 * cmpe220aludesign.c
 *
 *  Created on: Oct 7, 2016
 *      Author: NSTV
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

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
uint16_t flagreg=0x0000;
uint32_t instptr=128;
int inst_num,mem_addr,loc2,off,S,D,tempnum,imm;
char userinst[INST_LEN];
char *loc1,*oper,*tempstr,*rb,*ri,*src,*dest,tempop[2];
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
//int add_mem(int off, int loc2);
int add_mem(uint32_t*,uint32_t*,int,int);

void input_cmdline();
int loadreg(char* reg1,uint32_t* reg_num, int memory_addr );
int storereg(char*reg1,uint32_t* reg_num, int memory_addr );
void instruction_calc(uint8_t op);
void start_exe();
void get_reg_num(char *reg);

//main method
int main(void){
	//Variables for input


	init_memory();
	printf("Memory loaded with initial values.\n");
	// Display initial memory values
	init_registers();
	printf("Registers loaded with initial values.\n");
	printf("\nPress Enter Key to Continue...\n");
	getchar();
	//display();


	while(1)
	{
		printf("\nEnter an instruction number 1. Input 2. Display 3. Exit\n");
		scanf("%d",&inst_num);
		getchar();
		//Give Further instructions to the user


		switch(inst_num){
		case 1:
			printf("\nLoad or Store Instruction can be of the format-lw/sw reg,reg,reg,S,D\n");
			printf("\nArithmetic Instruction can be of the format- add/sub/mul/div/mod reg,reg\n");
			printf("\nPlease find the acceptable range values:\n\tReg: r0-r7\n\tS:1,2,4,8\n\tD:0,1,2,3,4\n");
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
				else if(strcmp(oper, "sl")==0)
				{
					opcode = 0x07;

				}
				else if(strcmp(oper, "srl")==0)
				{
					opcode = 0x08;

				}
				else if(strcmp(oper, "sra")==0)
				{
					opcode = 0x09;

				}
				else if(strcmp(oper, "End")==0)
				{
					opcode = 0x20;
				}
				else
				{
					opcode = 0xFF;  // exit opcode. may have to remove it later. invalid entry
				}

				if (opcode == 0xFF)
				{
					printf("Invalid Instructions. Please start over\n");

				}
				else if(opcode == 0x20)
				{

					instruction_calc(opcode);
					//function execution starts here
					//start_exe();
				}

				else
				{
					instruction_calc(opcode);

				}

			}while(!(strncmp(oper,"End",3)==0));
			break;

		case 2:
			printf("\nYou have selected to display the contents of the memory and registers\n");
			getchar();
			display();
			break;
		case 3:
			printf("\nYou have selected Exit. Exiting the program\n");
			exit(1);
			break;
		}
	}
	//	getchar();
	return 0;
}

// main exe function to read from instruction memory and execute.
//need to include all the functions corresponding to the opcodes defined
void start_exe()
{
	uint8_t mem_inst = 0xFF;
	instptr=128;
	do{
		mem_inst = mem[instptr] << 24;

		switch(mem_inst)
		{
		case 0x00:
			printf("Load instruction is being performed. The corresponding opcode is 0x00\n");
			//loadreg(loc1,base_p,index_P);
			//load(regnum1,effective_addr);
			break;
		case 0x01:
			printf("Store instruction is being performed. The corresponding opcode is 0x01\n");
			//storereg(loc1,reg_ptr,mem_addr);
			break;
		case 0x02:
			printf("Add instruction is being performed. The corresponding opcode is 0x02\n");
			//storereg(loc1,reg_ptr,mem_addr);
			//add(reg_num1,reg_num2);
			break;
		case 0x03:
			printf("Sub instruction is being performed. The corresponding opcode is 0x03\n");
			//loadreg(loc1,reg_ptr,mem_addr);
			break;
		case 0x04:
			printf("Mul instruction is being performed. The corresponding opcode is 0x04\n");
			//storereg(loc1,reg_ptr,mem_addr);
			break;
		case 0x05:
			printf("DIV instruction is being performed. The corresponding opcode is 0x05\n");
			//storereg(loc1,reg_ptr,mem_addr);
			break;
		case 0x06:
			printf("mod instruction is being performed. The corresponding opcode is 0x06\n");
			//storereg(loc1,reg_ptr,mem_addr);
			break;

		}

	}while(mem_inst != 0x20);

}

void instruction_calc(uint8_t opcode)
{
	int j,in;
	uint8_t temp;
	current_instr = ((uint32_t) opcode << 24); //8 bit opcode
	current_instr += ((uint32_t) base_r << 20); //4 bit reg_num1
	current_instr += ((uint32_t) index_r << 16); //4 bit reg_num2
	current_instr += (D << 4); //12 bit offset

	current_instr += S; //4 bit memory address //scaling factor
	printf("\nInstruction to be stored in instruction memory: 0x%.8x\n\n",current_instr);
	in = 24;
	for(j=1;j<=4;j++)
	{
		temp = current_instr >> in;
		in -=8;
		mem[instptr+j-1] = temp ;
		//	printf("%x,%x,%x",temp,instptr+j-1,mem[instptr+j-1]);
	}

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
				//	printf("\noper:%s",oper);

			}
			else if(tempnum==1){
				//	loc1=tempstr;
				//	printf("\nloc1:%s",loc1);

				get_reg_num(tempstr);
				base_reg = reg_ptr;
				dest_r = reg_num;
			}
			else if(tempnum==2){
				//rb=tempstr;

				get_reg_num(tempstr);
				base_p = reg_ptr;
				//	printf("\nrb:%s",rb);
				base_r = reg_num;

			}
			else if(tempnum==3){
				//ri=tempstr;

				get_reg_num(tempstr);
				index_p = reg_ptr;
				index_r = reg_num;

				//	printf("\nri:%s",ri);
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

		printf("\noper:%s\tloc1:%d\trb:%d\tri:%d\tS:%d\tD:%d\n",oper,dest_r,base_r,index_r,S,D);
		//		printf("\noper:%s\tloc1:%s\trb:%s\tri:%s\tS:%d\tD:%d\n",oper,*base_reg,*base_p,*index_p,S,D);
		mem_addr = add_mem(base_p,index_p,S,D);
	}
	else if(strchr(userinst,'$')!=NULL){
		oper=strtok(userinst," ");
		tempstr= strtok(NULL," ");
		loc1=strtok(tempstr,",$");

		tempstr= strtok(NULL,",$");
		imm=atoi(tempstr);
		printf("\nloc1:%s\timm:%d",loc1,imm);
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
		printf("\noper:%s\tsrc:%s\tdest:%s\n",oper,src,dest);
	}


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
	// Only for assignment 1: Allocating memory
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
	if ((temp>4095) || (temp<1024))
	{
		printf("Memory Overflow\nPlease retry\n");
		return 0;
	}
	else
		return temp;
}

int loadreg(char* reg1,uint32_t* reg_num,int memory_addr )
{
	uint32_t old_reg,x=0;
	old_reg = *reg_num;
	*reg_num = 0x00000000;

	for(x=4;x>0;x--)
	{
		*reg_num = *reg_num << 8;
		*reg_num= *reg_num + mem[memory_addr+x-1];
	}

	instptr= instptr +4;
	printf("Register %s changed from: 0x%.8x to: 0x%.8x\n",reg1,old_reg,*reg_num);
	//display();
	return 0;
}

int storereg(char* reg1,uint32_t* reg_num, int memory_addr)
{
	int x,y=8;
	uint8_t temp;
	//	uint32_t old_mem=0x00000000;

	/*for(x=4;x>=1;x--)
	{
		old_mem +=  mem[memory_addr+x-1];
		old_mem = old_mem << 8;
		printf("%x,%x,%x",old_mem,memory_addr+x-1,mem[memory_addr+x-1]);

	}
	 */
	temp= *reg_num;
	//printf("%x\n",temp);
	for(x=1;x<=4;x++)
	{
		mem[memory_addr+x-1] =  temp;
		temp = *reg_num >> y;
		y=y+8;
	}

	instptr = instptr + 4;
	printf("Memory Address: 0x%x-0x%x changed to: 0x%.2x%.2x%.2x%.2x\n",memory_addr,memory_addr+3,mem[memory_addr+3],mem[memory_addr+2],mem[memory_addr+1],mem[memory_addr]);
	//display();
	return 0;
}

//Function to display values at a given time
void display()
{
	int x=0,y=0;
	printf("\n\nValues in Instruction Memory\n\n");
	printf("Loc\tValue\tLoc\tValue\tLoc\tValue\tLoc\tValue\n");

	for(x=OS_LIMIT+1,y=1;x<INST_LIMIT+1;x=x+4,y++)
	{
		printf("0x%x\t%.2x%.2x%.2x%.2x ",x,mem[x],mem[x+1],mem[x+2],mem[x+3]);
		if(y%4==0){
			printf("\n");
		}
	}

	getchar();
	printf("\n\nValues in Main Memory\n\n");
	printf("Loc\tValue\tLoc\tValue\tLoc\tValue\tLoc\tValue\n");


	for(x=INST_LIMIT+1,y=1;x<MEMSIZE+1;x=x+4,y++){

		printf("0x%x\t%.2x%.2x%.2x%.2x ",x,mem[x],mem[x+1],mem[x+2],mem[x+3]);
		if(y%4==0){
			printf("\n");
		}
	}

	printf("\n\nThe register values are as follows:\n\n");
	printf("r0:0x%x\tr1:0x%x\tr2:0x%x\tr3:0x%x\tr4:0x%x\tr5:0x%x\tr6:0x%x\tr7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);
	printf("\nThe flag value is:0x%.4x\n",flagreg);
	printf("\nThe instruction pointer value is:0x%.8x\n",instptr);

}

