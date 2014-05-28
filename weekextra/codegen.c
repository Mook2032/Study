/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include "reader.h"
#include "codegen.h"
#include "tomips.h"

#define CODE_SIZE 10000
extern SymTab* symtab;

extern Object* readiFunction;
extern Object* readcFunction;
extern Object* writeiProcedure;
extern Object* writecProcedure;
extern Object* writelnProcedure;

CodeBlock* codeBlock;

int computeNestedLevel(Scope* scope) {
  // TODO
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
  // TODO
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  genLA(level, VARIABLE_OFFSET(var));
}

void genVariableValue(Object* var) {
  // TODO
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  genLV(level, VARIABLE_OFFSET(var));
}

void genParameterAddress(Object* param) {
  // TODO
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  genLA(level, PARAMETER_OFFSET(param));
}

void genParameterValue(Object* param) {
  // TODO
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  genLV(level, PARAMETER_OFFSET(param));
}

void genReturnValueAddress(Object* func) {
  // TODO
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  genLA(level, 0);
}

void genReturnValueValue(Object* func) {
  // TODO
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  genLV(level, 0);
}

void genPredefinedProcedureCall(Object* proc) {
  if (proc == writeiProcedure)
    genWRI();
  else if (proc == writecProcedure)
    genWRC();
  else if (proc == writelnProcedure)
    genWLN();
}

int computeCallLevel(Scope* scope){  
  // compute current function/produce contain scope
  scope = scope->outer;
  Object* owner2 = scope == NULL? NULL : scope->owner;
  int count = 0;
  while (owner2 != NULL && owner2->kind != OBJ_FUNCTION && owner2->kind != OBJ_PROCEDURE){
    scope = scope->outer;
    if (scope != NULL){
      owner2 = scope->owner;
    } else {
      owner2 = NULL;
    }
  }

// compute current function/produce contain current scope
  Object* owner = symtab->currentScope->owner;
  Scope* tempScope = symtab->currentScope;
  while (owner != NULL && owner != owner2){
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

void genProcedureCall(Object* proc) {
  // TODO
  int level = computeCallLevel(PROCEDURE_SCOPE(proc));
  genCALL(level , proc->procAttrs->codeAddress);
}

void genPredefinedFunctionCall(Object* func) {
  if (func == readiFunction)
    genRI();
  else if (func == readcFunction)
    genRC();
}

void genFunctionCall(Object* func) {
  // TODO
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

void genWRC(void) {
  emitWRC(codeBlock);
}

void genWRI(void) {
  emitWRI(codeBlock);
}

void genWLN(void) {
  emitWLN(codeBlock);
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

void genCV(void) {
  emitCV(codeBlock);
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

int isPredefinedFunction(Object* func) {
  return ((func == readiFunction) || (func == readcFunction));
}

int isPredefinedProcedure(Object* proc) {
  return ((proc == writeiProcedure) || (proc == writecProcedure) || (proc == writelnProcedure));
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
