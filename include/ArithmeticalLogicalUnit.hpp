#ifndef ArithmeticalLogicalUnit_h
#define ArithmeticalLogicalUnit_h

#include <stdint.h>

class ArithmeticalLogicalUnit
{
private:
	// input registers to CPU:
	uint32_t reg_A;
	uint32_t reg_B;
	// output register from CPU:
	uint32_t reg_C;

	// output flags:
	bool flag_zero;
	bool flag_greaterZero;
	bool flag_overflow;
	bool flag_dividebyZero;
	
	void setZeroFlags();
	void setOverFlowFlag_Add();
	void setOverFlowFlag_Sub();
	void setOverFlowFlag_ShiftL();
	
public:	
	// constructor:
	ArithmeticalLogicalUnit(){};
	// destructor:
	~ArithmeticalLogicalUnit(){}
	
	// register operations:
	void writeA(uint32_t data);
	void writeB(uint32_t data);
	uint32_t getC();
	
	// read flags:
	bool isZero();
	bool isGreaterZero();
	bool isOverflow();
	bool isDivideByZero();
	
	// arithmetical and logical operations:
	// S-RISC:
	void op_add();
	void op_sub();
	void op_and();
	void op_or();
	void op_shift_l();
	void op_shift_r();
	
	// RISC:
	void op_mul();
	void op_div();
	
	// DEBUG:
	uint32_t getA();
	uint32_t getB();
};

#endif
