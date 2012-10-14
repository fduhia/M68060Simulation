
#include "M68060PairabilityTests.h"
#include "../Assert.h"

const char* PairabilityTestResultToString(PairabilityTestResult pairabilityTestResult)
{
	static const char* pairabilityTestResultStrings[] =
	{
		"Success",
		"Test2Failure_FirstInstructionIs_pOEPOnly",
		"Test2Failure_SecondInstructionIsNot_pOEPOrsOEP",
		"Test3Failure_SecondInstructionUsesPCRelativeAddressing",
		"Test4Failure_BothInstructionsReferenceMemory",
		"Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionResult",
		"Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionResult",
	};

	M68060_ASSERT((size_t) pairabilityTestResult < (sizeof pairabilityTestResultStrings / sizeof pairabilityTestResultStrings[0]), "Invalid pairabilityTestResult");
	
	return pairabilityTestResultStrings[(int) pairabilityTestResult];
}

PairabilityTestResult checkPairability_Test2_InstructionClassification(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (opWord0->pairability == Pairability_pOEP_Only)
		return PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly;

	if (opWord1->pairability != Pairability_pOEP_Or_sOEP)
		return PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP;
		
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test3_AllowableAddressingModesInsOEP(DecodedOpWord* opWord1)
{
	if (opWord1->aguBase == ExecutionResource_PC)
		return PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing;

	// TODO: return failure if using {(bd, An, Xi*SF)} addressing mode
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test4_AllowableOperandDataMemoryReference(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (opWord0->hasMemoryReference && opWord1->hasMemoryReference)
		return PairabilityTestResult_Test4Failure_BothInstructionsReferenceMemory;

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test5_RegisterConflictsOnAguResources(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (isRegister(opWord1->aguBase) && (opWord1->aguBase == opWord0->aguResult || opWord1->aguBase == opWord0->ieeResult))
		return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionResult;
	if (isRegister(opWord1->aguIndex) && (opWord1->aguIndex == opWord0->aguResult || opWord1->aguIndex == opWord0->ieeResult))
		return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionResult;

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	PairabilityTestResult test2Result = checkPairability_Test2_InstructionClassification(opWord0, opWord1);
	PairabilityTestResult test3Result = checkPairability_Test3_AllowableAddressingModesInsOEP(opWord1);
	PairabilityTestResult test4Result = checkPairability_Test4_AllowableOperandDataMemoryReference(opWord0, opWord1);
	PairabilityTestResult test5Result = checkPairability_Test5_RegisterConflictsOnAguResources(opWord0, opWord1);

	if (test2Result != PairabilityTestResult_Success)
		return test2Result;

	if (test3Result != PairabilityTestResult_Success)
		return test3Result;

	if (test4Result != PairabilityTestResult_Success)
		return test4Result;

	if (test5Result != PairabilityTestResult_Success)
		return test5Result;

	return PairabilityTestResult_Success;
}
