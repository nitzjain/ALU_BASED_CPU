/*
 * bestcpudesign.c
 *
 *  Created on: Sep 17, 2016
 *      Author: Nitesh,Sudheer,Tejaswi,Vishwanath
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
uint32_t instptr=0;
int inst_num,mem_addr,loc2,off;
char userinst[INST_LEN];
char *loc1,*oper,*tempstr;
uint32_t* reg_ptr,base_ptr;;
short reg_num;
uint32_t current_instr = 0x00000000;
uint8_t opcode = 0xFF;//can be changed to avoid default Load operation


/*
Function declaration
 */

void init_memory();
void init_registers();
void display();
int add_mem(int off, int loc2);
void input_cmdline();
int loadreg(char* reg1,uint32_t* reg_num, int memory_addr );
int storereg(char*reg1,uint32_t* reg_num, int memory_addr );
void instruction_calc(uint8_t op);

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
	//Asking user Input
	printf("\nEnter an instruction number 1. Load 2. Store 3. Display 4. Exit\n");
	scanf("%d",&inst_num);
	getchar();
	//Give Further instructions to the user
	switch(inst_num){
	case 1:
		printf("\nYou have selected Load. Instruction should be of the form lw reg,(off)mem\n");
		printf("\nPlease find the acceptable range values:\n\tRegisters: r0-r7\n\tMemory Location:1024-4095\n");
		input_cmdline();
		if (mem_addr == 0)
		break;
		else
		{
		if(strcmp(oper, "lw")==0)
		{
            opcode = 0x00;
            instruction_calc(opcode);
//			loadreg(loc1,reg_ptr,mem_addr);
		}
		}
		break;
	case 2:
		printf("\nYou have selected Store. Instruction should be of the form sw reg,(off)mem\n");
		printf("\nPlease find the acceptable range values:\n\tRegisters: r0-r7\n\tMemory Location:1024-4095\n");
		input_cmdline();
		if (mem_addr == 0)
				break;
				else
				{
		if(strcmp(oper, "sw")==0)
			{
		    opcode = 0x01;
            instruction_calc(opcode);
//			storereg(loc1,reg_ptr,mem_addr);
			}
	}
		break;
	case 3:
		printf("\nYou have selected to display the contents of the memory and registers\n");
		getchar();
		display();
		break;
	case 4:
		printf("\nYou have selected Exit. Exiting the program\n");
		exit(1);
		break;
	}
}
//	getchar();
	return 0;
}

void instruction_calc(uint8_t opcode)
{
	int j,in;
	uint8_t temp;
	current_instr = ((uint32_t) opcode << 24); //8 bit opcode
	    //printf("Instruction + 8 bit Opcode in Instruction memory is: 0x%x\n",current_instr);
	    current_instr += ((uint32_t) reg_num << 20); //4 bit reg_num
	    //printf("Instruction + 4 bit Register Number in Instruction memory is: 0x%x\n",current_instr);
	    current_instr += (off << 12); //8 bit offset
	    //printf("Instruction + 8 bit Offset  in Instruction memory is: 0x%x\n",current_instr);
	    current_instr += loc2; //12 bit memory address
	    //printf("Instruction + 12 bit memory address  in Instruction memory is: 0x%x\n",current_instr);
	    printf("Instruction to be stored in instruction memory: 0x%.8x\n",current_instr);
	    in = 24;
	    for(j=4;j>=1;j--)
	    {
	    	temp = current_instr >> in;
	    	in -=8;
	    	mem[OS_LIMIT+j] = temp ;
	    //	printf("%x,%x,%x",temp,OS_LIMIT+j,mem[OS_LIMIT+j]);
	    }

switch(opcode){
	    case 0x00:
	        printf("Load instruction is being performed. The corresponding opcode is 0x00\n");
	        loadreg(loc1,reg_ptr,mem_addr);
	        break;
	    case 0x01:
	        printf("Store instruction is being performed. The corresponding opcode is 0x01\n");
	        storereg(loc1,reg_ptr,mem_addr);
	        break;
			}
}

void input_cmdline()
{
	fgets(userinst,INST_LEN,stdin); //User Instruction is stored here

	int len = strlen(userinst);
	userinst[len-1]='\0';

	//Breaking the Instruction and storing values for Load/Store
	oper=strtok(userinst," ");
	tempstr= strtok(NULL," ");
	loc1=strtok(tempstr,",(");

	tempstr= strtok(NULL,",(");
	off=atoi(strtok(tempstr,")"));

	tempstr= strtok(NULL,")");
	loc2=atoi(tempstr);

	//Displaying the values from User Instruction
	//printf("Operation:%s\n",oper);
	//printf("Reg:%s Offset:%d Memory:%d\n",loc1,off,loc2);
	mem_addr = add_mem(off,loc2);

	  //=============================================
	  //Selecting the register to pass to Load/Store Method
		if(strcmp(loc1, "r0")==0)
		{
            reg_num = 0x0;
			reg_ptr = &r0;
		}
		else if(strcmp(loc1, "r1")==0)
		{
            reg_num = 0x1;
			reg_ptr = &r1;
		}
		else if(strcmp(loc1, "r2")==0)
		{
            reg_num = 0x2;
			reg_ptr = &r2;
		}
		else if(strcmp(loc1, "r3")==0)
		{
            reg_num = 0x3;
			reg_ptr = &r3;
		}
		else if(strcmp(loc1, "r4")==0)
		{
            reg_num = 0x4;
			reg_ptr = &r4;
		}
		else if(strcmp(loc1, "r5")==0)
		{
            reg_num = 0x5;
			reg_ptr = &r5;
		}
		else if(strcmp(loc1, "r6")==0)
		{
            reg_num = 0x6;
			reg_ptr = &r6;
		}
		else if(strcmp(loc1, "r7")==0)
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
int add_mem(int off, int loc2)
{
	if (off+loc2 > 4095)
		{printf("Memory Overflow\nPlease retry\n");
flagreg=0x0800;
		return 0;}
	else
	return off+loc2;
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
	uint32_t old_mem=0x00000000;

	for(x=4;x>1;x--)
	{
		old_mem +=  mem[memory_addr+x-1];
		old_mem = old_mem << 8;

	}

	temp= *reg_num;
	//printf("%x\n",temp);
	for(x=1;x<=4;x++)
		{
		mem[memory_addr+x-1] =  temp;
		temp = *reg_num >> y;
		y=y+8;
		}

	instptr = instptr + 4;
	printf("Memory Address: 0x%x-0x%x changed from: 0x%x to: 0x%.2x%.2x%.2x%.2x\n",memory_addr,memory_addr+3,old_mem,mem[memory_addr+3],mem[memory_addr+2],mem[memory_addr+1],mem[memory_addr]);
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
	printf("Reg0:0x%x\tReg1:0x%x\tReg2:0x%x\tReg3:0x%x\tReg4:0x%x\tReg5:0x%x\tReg6:0x%x\tReg7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);
	printf("\nThe flag value is:0x%.16x\n",flagreg);

}
