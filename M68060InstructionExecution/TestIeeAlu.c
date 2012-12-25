
#include "M68060IeeAlu.h"
#include "../Assert.h"

typedef struct
{
	const char* description;
	IeeOperation ieeOperation;
	OperationSize operationSize;
	Flags flags;
	uint32_t ieeAValue;
	uint32_t ieeBValue;
	uint32_t expectedIeeResult;
	FlagsModifier expectedFlagsModifier;

} IeeAluTest;

IeeAluTest tests[] =
{
	{ "add.b 0x12,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0, 0x00000012, 0x123456fc, 0x1234560e, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.b 0xfe,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "add.w 0x40fe,0x1234c6fc", IeeOperation_Add, OperationSize_Word, 0, 0x000040fe, 0x1234c6fc, 0x123407fa, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.w 0xfffe,0x123456fc", IeeOperation_Add, OperationSize_Word, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.l 0x10000001,0x123456fc", IeeOperation_Add, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x223456fd, { 0, 0, }, },
	{ "adda.w 0x40fe,0x1234c6fc", IeeOperation_AddA, OperationSize_Word, 0, 0x000040fe, 0x1234c6fc, 0x123507fa, { Flags_All_Mask, 0, }, },
	{ "adda.w 0x8000,0x123456fc", IeeOperation_AddA, OperationSize_Word, 0, 0x00008000, 0x123456fc, 0x1233d6fc, { Flags_All_Mask, 0, }, },
	{ "adda.w 0xfffe,0x123456fc", IeeOperation_AddA, OperationSize_Word, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { Flags_All_Mask, 0, }, },
	{ "adda.l 0x10000001,0x123456fc", IeeOperation_AddA, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x223456fd, { Flags_All_Mask, 0, }, },
	{ "sub.b 0x12,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x00000012, 0x123456fc, 0x123456ea, { 0, Flags_Negative_Mask, }, },
	{ "sub.b 0x7f,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x0000007f, 0x123456fc, 0x1234567d, { 0, Flags_Overflow_Mask, }, },
	{ "sub.b 0xff,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x000000ff, 0x123456fc, 0x123456fd, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "sub.l 0x10000001,0x023456fc", IeeOperation_Sub, OperationSize_Long, 0, 0x10000001, 0x023456fc, 0xf23456fb, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "suba.w 0x8000,0x123456fc", IeeOperation_SubA, OperationSize_Word, 0, 0x00008000, 0x123456fc, 0x1234d6fc, { Flags_All_Mask, 0, }, },
	{ "suba.l 0x10000001,0x123456fc", IeeOperation_SubA, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x023456fb, { Flags_All_Mask, 0, }, },
};

const char* flagsToString(uint flags)
{
	const char* flagStrings[32] = {
		"(none)", "C", "V", "VC", "Z", "ZC", "ZV", "ZVC", "N", "NC", "NV", "NVC", "NZ", "NZC", "NZV", "NZVC",
		"X", "XC", "XV", "XVC", "XZ", "XZC", "XZV", "XZVC", "XN", "XNC", "XNV", "XNVC", "XNZ", "XNZC", "XNZV", "XNZVC",
	};

	M68060_ASSERT(flags < (sizeof flagStrings / sizeof flagStrings[0]), "Unsupported flags value");
	
	return flagStrings[flags];
};

void runTestSuite(const IeeAluTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;
		const IeeAluTest* test = &tests[testId];
		
		uint32_t ieeResult;
		FlagsModifier flagsModifier;
		bool correctIeeResult = true;
		bool correctFlagsModifierAnd = true;
		bool correctFlagsModifierOr = true;
		
		evaluateIeeAluOperation(test->ieeOperation, test->operationSize, test->flags, test->ieeAValue, test->ieeBValue, &ieeResult, &flagsModifier);

		if (ieeResult != test->expectedIeeResult)
			correctIeeResult = false;
		if (flagsModifier.andFlags != test->expectedFlagsModifier.andFlags)
			correctFlagsModifierAnd = false;
		if (flagsModifier.orFlags != test->expectedFlagsModifier.orFlags)
			correctFlagsModifierOr = false;

		success = correctIeeResult && correctFlagsModifierAnd && correctFlagsModifierOr;
			
		if (success && printSuccess)
			printf("success: Invoking %s yields ieeResult %08X, flagsModifer AND %s, OR %s\n", test->description, ieeResult, flagsToString(flagsModifier.andFlags), flagsToString(flagsModifier.orFlags));
		if (!success && printFailure)
		{
			printf("failure: Invoking %s yields aguResult %08X", test->description, ieeResult);
			if (!correctIeeResult)
				printf(" (should be %08X)", test->expectedIeeResult);
			printf(", flagsModifier AND %s", flagsToString(flagsModifier.andFlags));
			if (!correctFlagsModifierAnd)
				printf(" (should be %s)", flagsToString(test->expectedFlagsModifier.andFlags));
			printf(", OR %s", flagsToString(flagsModifier.orFlags));
			if (!correctFlagsModifierOr)
				printf(" (should be %s)", flagsToString(test->expectedFlagsModifier.orFlags));
			printf("\n");
		}

		if (success)
			numSuccessfulTests++;
		numTotalTests++;
	}
	
	*accumulatedSuccessfulTests += numSuccessfulTests;
	*accumulatedTotalTests += numTotalTests;
}

int main(void)
{
	bool printSuccess = true;
	bool printFailure = true;
	
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;

	printf("Testing M68060 IEE-ALU\n");
	printf("\n");

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}