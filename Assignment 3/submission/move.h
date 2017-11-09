/*
 * move.h
 *
 *  Created on: Nov 1, 2016
 *      Author: Nitesh
 */

#define STACK_MAX 3999
#define STACK_MIN 3800
#define MEMSIZE 4095
uint32_t base_ptr = STACK_MIN, stack_ptr = STACK_MIN;

uint8_t mem[MEMSIZE+1];

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




//	printf("Register changed from: 0x%.8x to: 0x%.8x\n",old_reg,*reg_num);


}

void loadregi(uint32_t* reg_num,int memory_addr )

{

	uint32_t old_reg;

	old_reg = *reg_num;

	*reg_num = 0x00000000;

	*reg_num = memory_addr;




//	printf("Register changed from: 0x%.8x to: 0x%.8x\n",old_reg,*reg_num);


}


void storereg(uint32_t* reg_num, int memory_addr)

{
	//printf("Memory Address: 0x%.2x%.2x%.2x%.2x\n",mem[memory_addr+3],mem[memory_addr+2],mem[memory_addr+1],mem[memory_addr]);

	int x,y=8;

	uint8_t temp;


	temp= *reg_num;


	for(x=1;x<=4;x++)

	{

		mem[memory_addr+x-1] =  temp;

		temp = *reg_num >> y;

		y=y+8;

	}




	//printf("Memory Address: 0x%x-0x%x changed to: 0x%.2x%.2x%.2x%.2x\n",memory_addr,memory_addr+3,mem[memory_addr+3],mem[memory_addr+2],mem[memory_addr+1],mem[memory_addr]);

}



void push(uint32_t data)
{
	if(stack_ptr >= STACK_MAX)
	{
		printf("\nCRITICAL ERROR:  Stack Overflow.\n");
		exit(1);
	}
	else
	{
		int x,y=8;
		uint8_t temp;
		temp = data;
		stack_ptr = stack_ptr + 4;
		//printf("\nstack_ptr: %u\n",stack_ptr);
		for(x=1;x<=4;x++)
		{
			mem[stack_ptr+x-1] =  temp;
			//printf("mem[stack_ptr+x-1] : %d, stack_ptr+x-1 : %d, ",mem[stack_ptr+x-1],stack_ptr+x-1);
			temp = data >> y;
			y=y+8;
		}
		//printf("Push operation is performed. %u is pushed into stack.\n",data);
	}
}



uint32_t pop(uint32_t* reg_num)
{
	//uint32_t data = 0;
	if(stack_ptr <= base_ptr)
	{
		printf("CRITICAL ERROR: Stack Underflow.\n");
		exit(1);
	}
	else
	{
		uint32_t old_reg,x=0;
		old_reg = *reg_num;
		*reg_num = 0x00000000;

		for(x=4;x>0;x--)
		{
			*reg_num = *reg_num << 8;
			*reg_num= *reg_num + mem[stack_ptr+x-1];
			mem[stack_ptr+x-1] = 0x0;
		}

		stack_ptr = stack_ptr -4;
		//printf("Register changed from: 0x%.8x to: 0x%.8x\n",old_reg,*reg_num);

		//printf("Pop operation is performed. %u is popped from stack.\n",*reg_num);
	}
	return *reg_num;
}
