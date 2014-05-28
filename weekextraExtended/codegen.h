/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "symtab.h"
#include "instructions.h"

#define RESERVED_WORDS 4

#define PROCEDURE_PARAM_COUNT(proc) (proc->procAttrs->paramCount)
#define PROCEDURE_SCOPE(proc) (proc->procAttrs->scope)
#define PROCEDURE_FRAME_SIZE(proc) (proc->procAttrs->scope->frameSize)

#define FUNCTION_PARAM_COUNT(func) (func->funcAttrs->paramCount)
#define FUNCTION_SCOPE(func) (func->funcAttrs->scope)
#define FUNCTION_FRAME_SIZE(func) (func->funcAttrs->scope->frameSize)

#define PROGRAM_SCOPE(prog) (prog->progAttrs->scope)
#define PROGRAM_FRAME_SIZE(prog) (prog->progAttrs->scope->frameSize)

#define VARIABLE_OFFSET(var) (var->varAttrs->localOffset)
#define VARIABLE_SCOPE(var) (var->varAttrs->scope)

#define PARAMETER_OFFSET(param) (param->paramAttrs->localOffset)
#define PARAMETER_SCOPE(param) (param->paramAttrs->scope)

#define RETURN_VALUE_OFFSET 0
#define DYNAMIC_LINK_OFFSET 1
#define RETURN_ADDRESS_OFFSET 2
#define STATIC_LINK_OFFSET 3

int computeNestedLevel(Scope* scope);

void genVariableAddress(Object* var);
void genVariableValue(Object* var);

void genParameterAddress(Object* param); 
void genParameterValue(Object* param);

void genReturnValueAddress(Object* func);
void genReturnValueAddress(Object* func);

void genArrayElementAddress(Type* arrayType);
void genArrayElementValue(Type* arrayType);

void genPredefinedProcedureCall(Object* proc);
void genProcedureCall(Object* proc);

void genPredefinedFunctionCall(Object* func);
void genFunctionCall(Object* func);

void genLA(int level, int offset);
void genLV(int level, int offset);
void genLC(WORD constant);
void genLCF(float constant);//
void genLCS(WORD constantCounter);//
void genLI(void);
void genINT(int delta);
void genDCT(int delta);
void genJ(CodeAddress label);
void genFJ(CodeAddress label);
void genHL(void);
void genST(void);
void genCALL(int level, CodeAddress label);
void genENTER(void);
void genEP(void);
void genEF(void);
void genRC(void);
void genRI(void);
void genRF(void);//
void genRS(void);//
void genWRC(void);
void genWRI(void);
void genWRF(void);//
void genWRS(void);//
void genWLN(void);
void genCV(void);

void genAD(void);
void genSB(void);
void genML(void);
void genDV(void);
void genNEG(void);
void genEQ(void);
void genNE(void);
void genGT(void);
void genGE(void);
void genLT(void);
void genLE(void);

void genADF(void);//
void genSBF(void);//
void genMLF(void);//
void genDVF(void);//
void genNEGF(void);//
void genEQF(void);//
void genNEF(void);//
void genGTF(void);//
void genGEF(void);//
void genLTF(void);//
void genLEF(void);//
void genEQS(void);//
void genNES(void);//
void genGTS(void);//
void genGES(void);//
void genLTS(void);//
void genLES(void);//

void genLABEL(CodeAddress label);

CodeAddress newLabel(void);
StringAddress newPreDef(char* string);
int isPredefinedProcedure(Object* proc);
int isPredefinedFunction(Object* func);

void initCodeBuffer(void);
void printCodeBuffer(void);
void cleanCodeBuffer(void);

int translate(char* fileName);

#endif
