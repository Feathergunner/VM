#include "ArithmeticalLogicalUnit.hpp"

ArithmeticalLogicalUnit::ArithmeticalLogicalUnit()
{
	#ifdef DEBUG
		printf("CONSTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif
}

ArithmeticalLogicalUnit::~ArithmeticalLogicalUnit()
{
	#ifdef DEBUG
		printf("DESTRUCTOR: %s\n", __PRETTY_FUNCTION__);
	#endif
}

/* 
----------------------------------------------------------------------------------------------
	SETTER:
----------------------------------------------------------------------------------------------
*/
void ArithmeticalLogicalUnit::writeA(uint32_t data)
{
	reg_A = data;
}

void ArithmeticalLogicalUnit::writeB(uint32_t data)
{
	reg_B = data;
}

/*
----------------------------------------------------------------------------------------------
	GETTER:
----------------------------------------------------------------------------------------------
*/
uint32_t ArithmeticalLogicalUnit::getC()
{
	return reg_C;
}

uint32_t ArithmeticalLogicalUnit::getA()
{
	return reg_A;
}

uint32_t ArithmeticalLogicalUnit::getB()
{
	return reg_B;
}

bool ArithmeticalLogicalUnit::isZero()
{
	return flag_zero;
}

bool ArithmeticalLogicalUnit::isGreaterZero()
{
	return flag_greaterZero;
}

bool ArithmeticalLogicalUnit::isOverflow()
{
	return flag_overflow;
}

bool ArithmeticalLogicalUnit::isDivideByZero()
{
	return flag_dividebyZero;
}

/*
----------------------------------------------------------------------------------------------
	ARITHMETIC OPERATIONS: 
----------------------------------------------------------------------------------------------
*/
void ArithmeticalLogicalUnit::op_add()
{
	reg_C = reg_A + reg_B;
	setZeroFlags();
	setOverFlowFlag_Add();
}

void ArithmeticalLogicalUnit::op_sub()
{
	reg_C = reg_A - reg_B;
	setZeroFlags();
	setOverFlowFlag_Sub();
}

void ArithmeticalLogicalUnit::op_mul()
{
	reg_C = reg_A * reg_B;
	setZeroFlags();
	
}

void ArithmeticalLogicalUnit:: op_div()
{
	if (reg_B != 0)
	{
		reg_C = reg_A / reg_B;
		setZeroFlags();
		flag_overflow = false;
	} else {
		flag_dividebyZero = true;
	}		
}

void ArithmeticalLogicalUnit::op_and()
{
	reg_C = reg_A & reg_B;
	setZeroFlags();
	flag_overflow = false;
}


void ArithmeticalLogicalUnit::op_or()
{
	reg_C = reg_A | reg_B;
	setZeroFlags();
	flag_overflow = false;
}

void ArithmeticalLogicalUnit::op_shift_l()
{
	reg_C = reg_A<<1;
	setZeroFlags();
	setOverFlowFlag_ShiftL();
}

void ArithmeticalLogicalUnit::op_shift_r()
{
	reg_C = reg_A>>1;
	setZeroFlags();
	flag_overflow = false;
}

/*
----------------------------------------------------------------------------------------------
	INTERN FLAG-SETTER OPERATIONS: 
----------------------------------------------------------------------------------------------
*/

void ArithmeticalLogicalUnit::setZeroFlags()
{
	if (reg_C == 0)
		flag_zero = true;
	else
		flag_zero = false;
	
	if (reg_C > 0)
		flag_greaterZero = true;
	else
		flag_greaterZero = false;
		
	// reset dividebyZero-flag:
	flag_dividebyZero = false;
}

void ArithmeticalLogicalUnit::setOverFlowFlag_Add()
{
	if (reg_C < reg_A || reg_C < reg_B)
		flag_overflow = true;
	else
		flag_overflow = false;
}

void ArithmeticalLogicalUnit::setOverFlowFlag_Sub()
{
	if (reg_B > reg_A)
		flag_overflow = true;
	else
		flag_overflow = false;
}

void ArithmeticalLogicalUnit::setOverFlowFlag_ShiftL()
{
	if (reg_A > reg_C)
		flag_overflow = true;
	else
		flag_overflow = false;
}
