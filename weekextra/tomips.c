/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "tomips.h"
#include "instructions.h"

  //s0 is the current base.
  //s1 is supposed to be the result of address calculation.
  //s2 is used to retrieve base of upper level.
  //By the way, s7 was, like, the ultimate base. We need it, from time to time
  
void transBase(FILE *f, int p)
{
  int i;
  fprintf(f,"move $s2, $s0\n");
  for (i=0;i<p;i++)
    fprintf(f,"\
addi $s1, $s2, -12\n\
lw   $s2, 0($s1)\n");
}

void transAddress(FILE *f, int p, int q)
{
  transBase(f,p);
  fprintf(f,"addi $s1, $s2, -%d\n", q*4);
}

void transLA(FILE *f, WORD p, WORD q)
{
  transAddress(f,p,q);
  fprintf(f,"\
addi $sp, $sp, -4\n\
sw   $s1, 0($sp) \n");
}

void transLV(FILE *f, WORD p, WORD q)
{
  transAddress(f,p,q);
  fprintf(f,"\
addi $sp, $sp, -4\n\
lw $t8, 0($s1)   \n\
sw $t8, 0($sp)   \n");
}

void transLC(FILE *f, WORD q)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
li   $t8, %d\n\
sw   $t8, 0($sp)\n", q);
}

void transLI(FILE *f)
{
  fprintf(f,"\
lw   $t8, 0($sp)\n\
lw   $t9, 0($t8)\n\
sw   $t9, 0($sp)\n");
}

void transINT(FILE *f, WORD q)
{
  fprintf(f,"addi $sp, $sp, -%d\n", q*4);
}

void transDCT(FILE *f, WORD q)
{
  fprintf(f,"addi $sp, $sp, %d\n", q*4);
}

void transJ(FILE *f, WORD q)
{
  fprintf(f,"j label%d\n",q);
}

void transFJ(FILE *f, WORD q)
{
  fprintf(f,"\
lw   $t8, 0($sp)\n\
addi $sp, $sp, 4\n\
beq  $t8, $zero, label%d\n",q);
}

void transHL(FILE *f)
{
  fprintf(f,"\
move $sp, $s7\n\
li   $v0, 10\n\
syscall\n");
}

void transST(FILE *f)
{
  fprintf(f,"\
lw  $t8, 4($sp)\n\
lw  $t9, 0($sp)\n\
sw  $t9, 0($t8)\n\
addi $sp, $sp, 8\n");
}

void transENTER(FILE *f)
{
   fprintf(f,"\
addi $sp, $sp, -12\n\
sw   $ra, 0($sp)\n\
addi $sp, $sp, 12\n");
}

void transCALL(FILE *f, WORD p, WORD q)
{
   transBase(f,p);
   fprintf(f,"\
addi $sp, $sp, -16\n\
sw   $s2, 0($sp)\n\
sw   $s0, 8($sp)\n\
addi $s0, $sp, 12\n\
addi $sp, $sp, 16\n\
jal  label%d\n",q);
}

void transEP(FILE *f)
{
   fprintf(f,"\
move $s2, $s0\n\
addi $s1, $s2, -8\n\
lw   $ra, 0($s1)\n\
lw   $s0, 4($s1)\n\
addi $sp, $s2, 4\n\
jr   $ra\n");
}

void transEF(FILE *f)
{
   fprintf(f,"\
move $s2, $s0\n\
addi $s1, $s2, -8\n\
lw   $ra, 0($s1)\n\
lw   $s0, 4($s1)\n\
move $sp, $s2\n\
jr   $ra\n");
}

void transRC(FILE *f)
{
      fprintf(f,"\
addi $sp, $sp, -4\n\
li   $v0, 12\n\
syscall\n\
sw   $v0, 0($sp)\n");
}

void transRI(FILE *f)
{
      fprintf(f,"\
addi $sp, $sp, -4\n\
li   $v0, 5\n\
syscall\n\
sw   $v0, 0($sp)\n");
}

void transWRC(FILE *f)
{
  fprintf(f,"\
lw   $a0, 0($sp)\n\
li   $v0, 11\n\
syscall\n\
addi $sp, $sp, 4\n");
}

void transWRI(FILE *f)
{
  fprintf(f,"\
lw   $a0, 0($sp)\n\
li   $v0, 1\n\
syscall\n\
addi $sp, $sp, 4\n");
}

void transWLN(FILE *f)
{
  fprintf(f,"\
li   $a0, '\\n'\n\
li   $v0, 11\n\
syscall\n");
}

void transAD(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
add  $t8, $t8, $t9\n\
sw   $t8, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transSB(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
sub  $t8, $t8, $t9\n\
sw   $t8, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transML(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
mult $t8, $t9\n\
mflo $t8\n\
sw   $t8, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transDV(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
div  $t8, $t9\n\
mflo $t8\n\
sw   $t8, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transNEG(FILE *f)
{
  fprintf(f,"\
lw   $t8, 0($sp)\n\
sub  $t8, $zero, $t8\n\
sw   $t8, 0($sp)\n");
}

void transCV(FILE *f)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
lw   $t8, 4($sp)\n\
sw   $t8, 0($sp)\n");
}

void transEQ(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t1, $t8, $t9\n\
slt  $t2, $t9, $t8\n\
or   $t0, $t1, $t2\n\
sub  $t0, $zero, $t0\n\
addi $t0, $t0, 1\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transNE(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t1, $t8, $t9\n\
slt  $t2, $t9, $t8\n\
or   $t0, $t1, $t2\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGT(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t0, $t9, $t8\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLT(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t0, $t8, $t9\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGE(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t0, $t8, $t9\n\
sub  $t0, $zero, $t0\n\
addi $t0, $t0, 1\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLE(FILE *f)
{
  fprintf(f,"\
lw   $t8, 4($sp)\n\
lw   $t9, 0($sp)\n\
slt  $t0, $t9, $t8\n\
sub  $t0, $zero, $t0\n\
addi $t0, $t0, 1\n\
sw   $t0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLABEL(FILE *f, WORD q)
{
  fprintf(f,"label%d:\n",q);
}

void toMipsTranslator(CodeBlock *codeBlock, FILE *f) {
  Instruction* code = codeBlock->code;
  int codeSize = codeBlock->codeSize;
  int codePos = 0;

  fprintf(f,"\
main:\n\
move $s7, $sp\n\
addi $s0, $sp, -4\n");
  
  while (codePos < codeSize) {

    switch (code[codePos].op) {
    case OP_LA:
      transLA(f,code[codePos].p,code[codePos].q);
      break;
    case OP_LV: 
      transLV(f,code[codePos].p,code[codePos].q);
      break;
    case OP_LC: 
      transLC(f,code[codePos].q);
      break;
    case OP_LI:
      transLI(f);
      break;
    case OP_INT:
      transINT(f,code[codePos].q);
      break;
    case OP_DCT: 
      transDCT(f,code[codePos].q);
      break;
    case OP_J: 
      transJ(f,code[codePos].q);
      break;
    case OP_FJ: 
      transFJ(f,code[codePos].q);
      break;
    case OP_HL: 
      transHL(f);
      break;
    case OP_ST: 
      transST(f);
      break;
    case OP_ENTER:
      transENTER(f);
      break;
    case OP_CALL: 
      transCALL(f,code[codePos].p,code[codePos].q);       
      break;
    case OP_EP: 
      transEP(f);
      break;
    case OP_EF:
      transEF(f);
      break;
    case OP_RC: 
      transRC(f);
      break;
    case OP_RI:
      transRI(f);
      break;
    case OP_WRC: 
      transWRC(f);
      break;     
    case OP_WRI: 
      transWRI(f);
      break;
    case OP_WLN:
      transWLN(f);
      break;
    case OP_AD:
      transAD(f);
      break;
    case OP_SB:
      transSB(f);
      break;
    case OP_ML:
      transML(f);
      break;
    case OP_DV: 
      transDV(f);
      break;
    case OP_NEG:
      transNEG(f);
      break;
    case OP_CV: 
      transCV(f);
      break;
    case OP_EQ:
      transEQ(f);
      break;
    case OP_NE:
      transNE(f);
      break;
    case OP_GT:
      transGT(f);
      break;
    case OP_LT:
      transLT(f);
      break;
    case OP_GE:
      transGE(f);
      break;
    case OP_LE:
      transLE(f);
      break;
    case OP_LABEL:
	  transLABEL(f,code[codePos].q);
      break;
    default: break;
    }
    codePos++;
  }
}
