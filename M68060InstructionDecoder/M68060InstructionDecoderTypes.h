
#ifndef M68060INSTRUCTIONDECODERTYPES_H
#define M68060INSTRUCTIONDECODERTYPES_H

#include "../Types.h"

typedef enum
{
	ExecutionResource_None,

	ExecutionResource_D0,	// Contents of integer register file
	ExecutionResource_D1,
	ExecutionResource_D2,
	ExecutionResource_D3,
	ExecutionResource_D4,
	ExecutionResource_D5,
	ExecutionResource_D6,
	ExecutionResource_D7,
	ExecutionResource_A0,
	ExecutionResource_A1,
	ExecutionResource_A2,
	ExecutionResource_A3,
	ExecutionResource_A4,
	ExecutionResource_A5,
	ExecutionResource_A6,
	ExecutionResource_A7,

	ExecutionResource_AguResult, // Output from AGU

	ExecutionResource_MemoryOperand, // Result from OC stage
	ExecutionResource_PC, // Contents of PC register

	ExecutionResource_uOpByte0, // Lower byte of first UOp extension word
	ExecutionResource_uOpWord0, // First UOp extension word
	ExecutionResource_uOpWord1, // Second UOp extension word
	ExecutionResource_uOpLong, // Both UOp extension words, seen as a 32-bit value

	ExecutionResource_AguTemp, // Temporary register used when decomposing complex addressing modes into multiple UOps
	ExecutionResource_IeeTemp, // Temporary register used when decomposing instructions with both immediates and displacements into multiple UOps

	ExecutionResource_ImmOpWord3Bit, // Special 3-bit immediate operand that is embedded in the OpWord
	ExecutionResource_ImmOpWord8Bit, // Special 8-bit immediate operand that is embedded in the OpWord

	ExecutionResource_Constant1, // Constant value "1"

	ExecutionResource_CCR,
	
} ExecutionResource;

const char* ExecutionResourceToString(ExecutionResource executionResource);

bool isDnRegister(ExecutionResource executionResource);
bool isAnRegister(ExecutionResource executionResource);
bool isRegister(ExecutionResource executionResource);

typedef enum
{
	Pairability_pOEP_Or_sOEP,
	Pairability_pOEP_But_Allows_sOEP,
	Pairability_pOEP_Only,

} Pairability;

const char* PairabilityToString(Pairability pairability);

typedef enum
{
	AguOperation_None,
	AguOperation_PostIncrement,
	AguOperation_PreDecrement,
	AguOperation_OffsetBaseIndexScale,
	AguOperation_PostIncrementSP,
	AguOperation_PreDecrementSP,

} AguOperation;

const char* AguOperationToString(AguOperation aguOperation);

typedef enum
{
	AguIndexSize_None,
	AguIndexSize_Word,
	AguIndexSize_Long,

} AguIndexSize;

const char* AguIndexSizeToString(AguIndexSize aguIndexSize);

typedef enum
{
	AguDisplacementSize_None,
	AguDisplacementSize_S8,
	AguDisplacementSize_S16,
	AguDisplacementSize_S32,

} AguDisplacementSize;

const char* AguDisplacementSizeToString(AguDisplacementSize aguDisplacementSize);

typedef enum
{
	OperationSize_None,
	OperationSize_Byte,
	OperationSize_Word,
	OperationSize_Long,

} OperationSize;

const char* OperationSizeToString(OperationSize operationSize);

typedef enum
{
	IeeOperation_None,
	IeeOperation_ForwardIeeA,
	IeeOperation_Add,
	IeeOperation_AddA,
	IeeOperation_AddX,
	IeeOperation_Cmp,
	IeeOperation_CmpA,
	IeeOperation_Sub,
	IeeOperation_SubA,
	IeeOperation_SubX,
	IeeOperation_Move,
	IeeOperation_MoveA,
	IeeOperation_Neg,
	IeeOperation_NegX,
	IeeOperation_Tst,
	IeeOperation_Asl,
	IeeOperation_Asr,
	IeeOperation_Lsl,
	IeeOperation_Lsr,
	IeeOperation_Rol,
	IeeOperation_Ror,
	IeeOperation_Roxl,
	IeeOperation_Roxr,
	IeeOperation_And,
	IeeOperation_Or,
	IeeOperation_Eor,
	IeeOperation_Not,
	IeeOperation_Clr,
	IeeOperation_Ext,
	IeeOperation_BChg,
	IeeOperation_BClr,
	IeeOperation_BSet,
	IeeOperation_BTst,
	IeeOperation_Abcd,
	IeeOperation_Sbcd,
	IeeOperation_Nbcd,
	IeeOperation_Pack,
	IeeOperation_Unpk,
	IeeOperation_AndToCcr,
	IeeOperation_EorToCcr,
	IeeOperation_OrToCcr,
	IeeOperation_MoveToCcr,
	IeeOperation_MoveFromCcr,
	IeeOperation_Scc,
	IeeOperation_Swap,
	IeeOperation_Bcc,

} IeeOperation;

const char* IeeOperationToString(IeeOperation ieeOperation);

#endif
