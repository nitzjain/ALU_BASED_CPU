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
uint32_t mem[MEMSIZE+1];
uint32_t r0,r1,r2,r3,r4,r5,r6,r7;
uint32_t flagreg=0x00000000;
uint32_t instptr=0;
int inst_num,mem_addr,loc2,off;
char userinst[INST_LEN];
char *loc1,*oper,*tempstr;
uint32_t* reg_ptr;


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
	printf("Enter an instruction number 1. Load 2. Store 3. Display 4. Exit\n");
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
			loadreg(loc1,reg_ptr,mem_addr);
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
				storereg(loc1,reg_ptr,mem_addr);
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
			reg_ptr = &r0;
		}
		else if(strcmp(loc1, "r1")==0)
		{
			reg_ptr = &r1;
		}
		else if(strcmp(loc1, "r2")==0)
		{
			reg_ptr = &r2;
		}
		else if(strcmp(loc1, "r3")==0)
		{
			reg_ptr = &r3;
		}
		else if(strcmp(loc1, "r4")==0)
		{
			reg_ptr = &r4;
		}
		else if(strcmp(loc1, "r5")==0)
		{
			reg_ptr = &r5;
		}
		else if(strcmp(loc1, "r6")==0)
		{
			reg_ptr = &r6;
		}
		else if(strcmp(loc1, "r7")==0)
		{
			reg_ptr = &r7;
		}
		else
		{
			printf("invalid register value, please re-enter");

		}

}




//Function to initialize memory locations to 1
void init_memory(){
	int i;
	for(i=0;i<=OS_LIMIT;i++){
		mem[i]=0x00000000;
	}

	for(i=OS_LIMIT+1;i<MEMSIZE+1;i++){
		mem[i]=0x00000001;
	}

	//mem[2300]=0xabcdefff;

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
	return 0;}
	else
	return off+loc2;
}

int loadreg(char* reg1,uint32_t* reg_num,int memory_addr )
{
	//display();
	uint32_t temp;
	printf("Fetching Memory location %d i.e. %x in Hex\n",memory_addr,memory_addr);
	temp = *reg_num;
	*reg_num= mem[memory_addr];
	instptr= instptr +4;
	printf("Register %s changed from: %x to: %x\n",reg1,temp,*reg_num);
	//display();
	return 0;
}

int storereg(char* reg1,uint32_t* reg_num, int memory_addr)
{
	//display();
	uint32_t temp;
	printf("Storing into Memory location %d i.e. %x in Hex\n",memory_addr,memory_addr);
	temp = mem[memory_addr];
	mem[memory_addr] =  *reg_num;
	instptr = instptr + 4;
	printf("Memory Address: 0x%x changed from: %x to: %x\n",memory_addr,temp,mem[memory_addr]);
	//display();
	return 0;
}

//Function to display values at a given time
void display()
{
	int x=0,y=0;
	printf("\n\nValues in Instruction Memory\n\n");
	printf("Loc  Value  Loc  Value  Loc  Value  Loc  Value  Loc  Value  Loc  Value\n");

	for(x=OS_LIMIT+1,y=1;x<=INST_LIMIT;x++,y++)
		{
		printf("0x%x\t%x ",x,mem[x]);
		if(y%6==0){
			printf("\n");
				}
		}

	getchar();
	printf("\n\nValues in Main Memory\n\n");
	printf("Loc  Value  Loc  Value  Loc  Value  Loc  Value  Loc  Value  Loc  Value\n");

	for(x=INST_LIMIT+1,y=1;x<MEMSIZE+1;x++,y++){
		printf("0x%x\t%x ",x,mem[x]);
		if(y%6==0){
			printf("\n");
		}
	}

	printf("\n\nThe register values are as follows:\n\n");
	printf("Reg0:0x%x\tReg1:0x%x\tReg2:0x%x\tReg3:0x%x\tReg4:0x%x\tReg5:0x%x\tReg6:0x%x\tReg7:0x%x\t\n",r0,r1,r2,r3,r4,r5,r6,r7);
	printf("\nThe flag value is:0x%x\n",flagreg);

	//getchar();
}
