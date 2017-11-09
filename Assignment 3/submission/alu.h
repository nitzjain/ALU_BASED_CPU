/*
 * alu.h
 *
 *  Created on: Nov 1, 2016
 *      Author: Nitesh
 */


uint16_t flagreg=0x0000,imm;

int add_alu(uint32_t r1, uint32_t r2)
{
	uint64_t la=0, lb=0;
	la = r1; lb = r2;
	uint64_t carry=0;
	//printf("\nThe input values are: value1 =%u, value2 =%u \n", la,lb);

	//la= la+lb;

	while (lb != 0)
	{
		carry = la&lb;
		la = la^lb;
		lb = carry << 1;
	}

	int res= (int)la;


	uint32_t carr=la>>32;
	//printf("\nThe carry values after addition is: %u \n", carr);
	if (carr >= 1)
	{
		//printf("\nThere is an overflow and a carry in ADD operation !!!\n");
		flagreg |= (1 << 2); 	//set overflow flag
		flagreg |= (1 << 3);	//set carry flag
	}
	else
	{
		//flag &= ~(1 << 0);
		flagreg &= ~(1 << 2);	//clear overflow flag
		flagreg &= ~(1 << 3);	//clear carry flag
	}

	flagreg &= ~(1 << 1);

	if (res == 0)
	{
		flagreg |= (1 << 0);	//set zero flag
	}
	else
		flagreg &= ~(1 << 0);

	return (la);
}

int negation(int r1)
{
	return add_alu(~r1,1);
}

int sub_alu(int r1,int r2)
{
	// IF the answer goes negative than the sign and overflow flags are set , overflow bec out of unsigned range

	int nr2=negation(r2);
	int64_t result = add_alu(r1, nr2);
	if (r1 < r2)
	{
		flagreg |= (1 << 1);	//set sign flag
		flagreg |= (1 << 2); 	//set overflow flag
	}
	else
	{
		flagreg &= ~(1 << 1);
		flagreg &= ~(1 << 2);	//clear overflow flag
	}

	if (result == 0)
	{
		flagreg |= (1 << 0);	//set zero flag
	}
	else
		flagreg &= ~(1 << 0);

	flagreg &= ~(1 << 3);	//clear carry flag


	return(result);
}


int mul_alu(int r1, int r2)
{
	int i=0;
	uint64_t lres = 0;
	//printf("\nThe input values are: r1=%u, r2=%u \n", r1,r2);

	for(i=0; i<32; i++) {
		if(r1 & 1 << i)
			lres = lres + (r2<<i);
	}

	uint32_t overflow=lres>>32;
	unsigned int res=(int)lres;

	//	printf("\nIn MUL: overflow = %u \n", overflow);

	if (overflow >= 1) {
		//printf("Inside MUL, overflow happened");
		flagreg |= (1 << 2);	//set overflow flag
	}
	else
		flagreg &= ~(1 << 2);	//clear overflow flag

	//Overflow Flag (OF) - set to 1 when there is a signed overflow. For example, when you add bytes 100 + 50 (result is not in range -128...127
	flagreg &= ~(1 << 3);	//clear carry flag, 1*1=1, no carry

	flagreg &= ~(1 << 1);  // clear sign flags since unsigned numbers multiplication

	if (res == 0)
		flagreg |= (1 << 0);	//set zero flag
	else
		flagreg &= ~(1 << 0);

	return res;
}

int div_alu(int a,int b) {
	int quo=0;

	if (b>a) {
		printf("divisor greater than dividend\n");
		flagreg |= (1 << 0);	//set zero flag
		return 0;
	}

	if(b!=0){
		while (a >= b)
		{
			a = sub_alu(a, b);
			quo = add_alu(quo, 1);
		}
	}

	if (quo == 0) {
		flagreg |= (1 << 0);	// set zero flag
	}
	else
		flagreg &= ~(1 << 0);


	flagreg &= ~(1 << 1);  // clear sign flags
	flagreg &= ~(1 << 2);  // clear overflow flag
	flagreg &= ~(1 << 3);  // clear carry flag

	return quo;
}

int mod_alu(int a, int b) {

	int quo=0;

	if (b > a) {
		if (a == 0)
			flagreg |= (1 << 0);	//set zero flag
		else
			flagreg &= ~(1 << 0);

		flagreg &= ~(1 << 1);  // clear sign flags
		flagreg &= ~(1 << 2);	//clear overflow flag
		flagreg &= ~(1 << 3);	//clear carry flag
		printf("divisor greater than dividend\n");
		return a;
	}

	if(b!=0)
	{
		while (a >= b)
		{
			a = sub_alu(a, b);
			quo = add_alu(quo, 1);
		}
	}

	if (a == 0)
		flagreg |= (1 << 0);	//set zero flag
	else
		flagreg &= ~(1 << 0);

	flagreg &= ~(1 << 1);  // clear sign flags
	flagreg &= ~(1 << 2);	//clear overflow flag
	flagreg &= ~(1 << 3);	//clear carry flag

	return a;
}
//=============================================
