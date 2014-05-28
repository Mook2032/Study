/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "codegen.h"
#include "tomips.h"

#define CODE_SIZE 10000
extern SymTab* symtab;

extern Object* readiFunction;
extern Object* readcFunction;
extern Object* readfFunction;
extern Object* readsFunction;
extern Object* writeiProcedure;
extern Object* writecProcedure;
extern Object* writefProcedure;
extern Object* writesProcedure;
extern Object* writelnProcedure;

CodeBlock* codeBlock;

int computeNestedLevel(Scope* scope) {
  int count = 0;
  
  Scope* tempScope = symtab->currentScope;
  while (scope != tempScope){
    if (tempScope == NULL){
      count = 0;
      break;
    }
    tempScope = tempScope->outer;
    count++;
  } 

  return count;
}

void genVariableAddress(Object* var) {
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  genLA(level, VARIABLE_OFFSET(var));
}

void genVariableValue(Object* var) {
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  genLV(level, VARIABLE_OFFSET(var));
}

void genParameterAddress(Object* param) {
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  genLA(level, PARAMETER_OFFSET(param));
}

void genParameterValue(Object* param) {
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  genLV(level, PARAMETER_OFFSET(param));
}

void genReturnValueAddress(Object* func) {
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  genLA(level, 0);
}

void genReturnValueValue(Object* func) {
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  genLV(level, 0);
}

int computeCallLevel(Scope* scope){  
  // find the function that contains this scope, for safe, really
  scope = scope->outer;
  Object* ownerCall = scope == NULL? NULL : scope->owner;
  int count = 0;
  while (ownerCall != NULL && ownerCall->kind != OBJ_FUNCTION && ownerCall->kind != OBJ_PROCEDURE){
    scope = scope->outer;
    if (scope != NULL){
      ownerCall = scope->owner;
    } else {
      ownerCall = NULL;
    }
  }
  //where the real calculation happen
  Object* owner = symtab->currentScope->owner;
  Scope* tempScope = symtab->currentScope;
  while (owner != NULL && owner != ownerCall){
    if ( owner->kind == OBJ_FUNCTION || owner->kind == OBJ_PROCEDURE){
      count++;
    }
    tempScope = tempScope->outer;
    if (tempScope != NULL){
      owner = tempScope->owner;
    } else {
      owner = NULL;
    }
  }

  return count;
}

void genPredefinedProcedureCall(Object* proc) {
  if (proc == writeiProcedure)
    genWRI();
  else if (proc == writecProcedure)
    genWRC();
  else if (proc == writefProcedure)
    genWRF();
  else if (proc == writesProcedure)
    genWRS();
  else if (proc == writelnProcedure)
    genWLN();
}

void genProcedureCall(Object* proc) {
  int level = computeCallLevel(PROCEDURE_SCOPE(proc));
  genCALL(level , proc->procAttrs->codeAddress);
}

void genPredefinedFunctionCall(Object* func) {
  if (func == readiFunction)
    genRI();
  else if (func == readcFunction)
    genRC();
  else if (func == readfFunction)
    genRF();
  else if (func == readsFunction)
    genRS();
}

void genFunctionCall(Object* func) {
  int level = computeCallLevel(FUNCTION_SCOPE(func));
  genCALL(level, func->funcAttrs->codeAddress);
}

void genLA(int level, int offset) {
  emitLA(codeBlock, level, offset);
}

void genLV(int level, int offset) {
  emitLV(codeBlock, level, offset);
}

void genLC(WORD constant) {
  emitLC(codeBlock, constant);
}

void genLCF(float constant) {
  emitLCF(codeBlock, constant);
}

void genLCS(WORD constantCounter){
  emitLCS(codeBlock, constantCounter);
}

void genLI(void) {
  emitLI(codeBlock);
}

void genINT(int delta) {
  emitINT(codeBlock,delta);
}

void genDCT(int delta) {
  emitDCT(codeBlock,delta);
}

void genJ(CodeAddress label) {
  emitJ(codeBlock,label);
}

void genFJ(CodeAddress label) {
  emitFJ(codeBlock, label);
}

void genHL(void) {
  emitHL(codeBlock);
}

void genST(void) {
  emitST(codeBlock);
}

void genCALL(int level, CodeAddress label) {
  emitCALL(codeBlock, level, label);
}

void genENTER(){
  emitENTER(codeBlock);
}

void genEP(void) {
  emitEP(codeBlock);
}

void genEF(void) {
  emitEF(codeBlock);
}

void genRC(void) {
  emitRC(codeBlock);
}

void genRI(void) {
  emitRI(codeBlock);
}

void genRF(void) {
  emitRF(codeBlock);
}

void genRS(void) {
  emitRS(codeBlock);
}

void genWRC(void) {
  emitWRC(codeBlock);
}

void genWRI(void) {
  emitWRI(codeBlock);
}

void genWRF(void) {
  emitWRF(codeBlock);
}

void genWRS(void) {
  emitWRS(codeBlock);
}

void genWLN(void) {
  emitWLN(codeBlock);
}

void genCV(void) {
  emitCV(codeBlock);
}

void genAD(void) {
  emitAD(codeBlock);
}

void genSB(void) {
  emitSB(codeBlock);
}

void genML(void) {
  emitML(codeBlock);
}

void genDV(void) {
  emitDV(codeBlock);
}

void genNEG(void) {
  emitNEG(codeBlock);
}

void genEQ(void) {
  emitEQ(codeBlock);
}

void genNE(void) {
  emitNE(codeBlock);
}

void genGT(void) {
  emitGT(codeBlock);
}

void genGE(void) {
  emitGE(codeBlock);
}

void genLT(void) {
  emitLT(codeBlock);
}

void genLE(void) {
  emitLE(codeBlock);
}

void genADF(void) {
  emitADF(codeBlock);
}

void genSBF(void) {
  emitSBF(codeBlock);
}

void genMLF(void) {
  emitMLF(codeBlock);
}

void genDVF(void) {
  emitDVF(codeBlock);
}

void genNEGF(void) {
  emitNEGF(codeBlock);
}

void genEQF(void) {
  emitEQF(codeBlock);
}

void genNEF(void) {
  emitNEF(codeBlock);
}

void genGTF(void) {
  emitGTF(codeBlock);
}

void genGEF(void) {
  emitGEF(codeBlock);
}

void genLTF(void) {
  emitLTF(codeBlock);
}

void genLEF(void) {
  emitLEF(codeBlock);
}

void genEQS(void) {
  emitEQS(codeBlock);
}

void genNES(void) {
  emitNES(codeBlock);
}

void genGTS(void) {
  emitGTS(codeBlock);
}

void genGES(void) {
  emitGES(codeBlock);
}

void genLTS(void) {
  emitLTS(codeBlock);
}

void genLES(void) {
  emitLES(codeBlock);
}

void genLABEL(CodeAddress label) {
  emitLABEL(codeBlock, label);
}

void updateJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

void updateFJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

CodeAddress newLabel(void){
  codeBlock->labelCount++;
  return codeBlock->labelCount;
}

StringAddress newPreDef(char* string)
{
   codeBlock->stringList[codeBlock->asciizCount] = (char*)malloc(sizeof(char)*(strlen(string)+1));
   strcpy(codeBlock->stringList[codeBlock->asciizCount], string);
   codeBlock->asciizCount++;
   return (codeBlock->asciizCount - 1);
}

int isPredefinedFunction(Object* func) {
  return ((func == readiFunction) || (func == readcFunction) || (func == readfFunction) || (func == readsFunction));
}

int isPredefinedProcedure(Object* proc) {
  return ((proc == writeiProcedure) || (proc == writecProcedure) || (proc == writefProcedure) || (proc == writesProcedure) || (proc == writelnProcedure));
}

void initCodeBuffer(void) {
  codeBlock = createCodeBlock(CODE_SIZE);
}

void printCodeBuffer(void) {
  printCodeBlock(codeBlock);
}

void cleanCodeBuffer(void) {
  freeCodeBlock(codeBlock);
}

int translate(char* fileName) {
  FILE* f;

  f = fopen(fileName, "wb");
  if (f == NULL) return IO_ERROR;
  toMipsTranslator(codeBlock, f);
  fclose(f);
  return IO_SUCCESS;
}
