/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <string.h>

#include "tomips.h"
#include "instructions.h"

  //s0 is the current base.
  //s1 is supposed to be the result of address calculation.
  //s2 is used to retrieve base of upper level.
  //By the way, s7 was, like, the storage for sp.

void printmipsString(FILE *f, char* string)
{
	int i, n = strlen(string);
	for (i=0;i<n;i++)
	{
		switch (string[i])
		{
			case '"':
				fputc('\\',f);
				fputc('"',f);
				break;
			case '\t':
				fputc('\\',f);
				fputc('t',f);
				break;
			case '\n':
				fputc('\\',f);
				fputc('n',f);
				break;
			case '\'':
				fputc('\\',f);
				fputc('\'',f);
				break;
			case '\\':
				fputc('\\',f);
				fputc('\\',f);
				break;
			case '\0':
				fputc('\\',f);
				fputc('0',f);
				break;
			case '\b':
				fputc('\\',f);
				fputc('b',f);
				break;
			case '\r':
				fputc('\\',f);
				fputc('r',f);
				break;
			case '\v':
				fputc('\\',f);
				fputc('v',f);
				break;
			case '\a':
				fputc('\\',f);
				fputc('a',f);
				break;
			case '\f':
				fputc('\\',f);
				fputc('f',f);
				break;
			default:
				fputc(string[i],f);
				break;
		}
	}
}
  
void prepareData(FILE *f, CodeBlock* codeBlock)
{
  int i;
  int asciizCount = codeBlock->asciizCount;
  fprintf(f,".data\n");
  for (i=0; i< asciizCount; i++)
  {
	fprintf(f,"string%d: .asciiz \"",i);
	printmipsString(f, codeBlock -> stringList[i]);
	fprintf(f,"\"\n");
  }
  
}
  
void preparedCode(FILE *f)
{

  fprintf(f,"\
main:\n\
move $s7, $sp\n\
addi $s0, $sp, -4\n\
j label1\n\
ltf:\n\
c.lt.s $f2, $f4\n\
bc1t ltft\n\
li $v0, 0\n\
jr $ra\n\
ltft:\n\
li $v0, 1\n\
jr $ra\n\
lef:\n\
c.le.s $f2, $f4\n\
bc1t left\n\
li $v0, 0\n\
jr $ra\n\
left:\n\
li $v0, 1\n\
jr $ra\n\
gtf:\n\
c.le.s $f2, $f4\n\
bc1f gtft\n\
li $v0, 0\n\
jr $ra\n\
gtft:\n\
li $v0, 1\n\
jr $ra\n\
gef:\n\
c.lt.s $f2, $f4\n\
bc1f geft\n\
li $v0, 0\n\
jr $ra\n\
geft:\n\
li $v0, 1\n\
jr $ra\n\
eqf:\n\
c.eq.s $f2, $f4\n\
bc1t eqft\n\
li $v0, 0\n\
jr $ra\n\
eqft:\n\
li $v0, 1\n\
jr $ra\n\
nef:\n\
c.eq.s $f2, $f4\n\
bc1f neft\n\
li $v0, 0\n\
jr $ra\n\
neft:\n\
li $v0, 1\n\
jr $ra\n\
compareS:\n\
compareSLoop:\n\
lb $t4, 0($a0)\n\
lb $t5, 0($a1)\n\
sub $v0, $t4, $t5\n\
bne $v0, $zero, compareSEnd\n\
or $t7, $t4, $t5\n\
beq $t7, $zero, compareSEnd\n\
addi $a0, $a0, 1\n\
addi $a1, $a1, 1\n\
j compareSLoop\n\
compareSEnd:\n\
jr $ra\n\
eqs:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
beq $v0, $zero, eqst\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
eqst:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
nes:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
bne $v0, $zero, nest\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
nest:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
lts:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
slt $t7, $v0, $zero\n\
bne $t7, $zero, ltst\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
ltst:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
les:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
slt $t7, $zero, $v0\n\
beq $t7, $zero, lest\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
lest:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
gts:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
slt $t7, $zero, $v0\n\
bne $t7, $zero, gtst\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
gtst:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
ges:\n\
addi $sp, $sp, -4\n\
sw $ra, 0($sp)\n\
jal compareS\n\
slt $t7, $v0, $zero\n\
beq $t7, $zero, gest\n\
li $v0, 0\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
gest:\n\
li $v0, 1\n\
lw $ra, 0($sp)\n\
addi $sp, $sp, 4\n\
jr $ra\n\
trimEnter:\n\
trimEnterLoop:\n\
lb $t4, 0($a0)\n\
beq $t4, $zero, trimReal\n\
addi $a0, $a0, 1\n\
j  trimEnterLoop\n\
trimReal:\n\
addi $a0, $a0, -1\n\
li $t4, 0\n\
sb $t4, 0($a0)\n\
jr $ra\n");
}
  
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

void transLCF(FILE *f, float qFloat)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
li.s $f2, %f\n\
s.s $f2, 0($sp)\n", qFloat);
}

void transLCS(FILE *f, WORD q)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
la   $t8, string%d\n\
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

void transRF(FILE *f)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
li   $v0, 6\n\
syscall\n\
s.s $f0, 0($sp)\n");	
}

void transRS(FILE *f)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
li   $a0, 32\n\
li   $v0, 9\n\
syscall\n\
move $s4, $v0\n\
move $a0, $s4\n\
li   $a1, 31\n\
li   $v0, 8\n\
syscall\n\
move $a0, $s4\n\
jal  trimEnter\n\
sw   $s4, 0($sp)\n");
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

void transWRF(FILE *f)
{
  fprintf(f,"\
l.s $f12, 0($sp)\n\
li   $v0, 2\n\
syscall\n\
addi $sp, $sp, 4\n");
}

void transWRS(FILE *f)
{
  fprintf(f,"\
lw   $a0, 0($sp)\n\
li   $v0, 4\n\
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

void transCV(FILE *f)
{
  fprintf(f,"\
addi $sp, $sp, -4\n\
lw   $t8, 4($sp)\n\
sw   $t8, 0($sp)\n");
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

void transADF(FILE *f)
{
  fprintf(f,"\
l.s  $f2, 4($sp)\n\
l.s  $f4, 0($sp)\n\
add.s $f2, $f2, $f4\n\
s.s  $f2, 4($sp)\n\
addi  $sp, $sp, 4\n");
}

void transSBF(FILE *f)
{
  fprintf(f,"\
l.s  $f2, 4($sp)\n\
l.s  $f4, 0($sp)\n\
sub.s $f2, $f2, $f4\n\
s.s  $f2, 4($sp)\n\
addi  $sp, $sp, 4\n");
}

void transMLF(FILE *f)
{
  fprintf(f,"\
l.s  $f2, 4($sp)\n\
l.s  $f4, 0($sp)\n\
mul.s $f2, $f2, $f4\n\
s.s  $f2, 4($sp)\n\
addi  $sp, $sp, 4\n");
}

void transDVF(FILE *f)
{
  fprintf(f,"\
l.s  $f2, 4($sp)\n\
l.s  $f4, 0($sp)\n\
div.s $f2, $f2, $f4\n\
s.s  $f2, 4($sp)\n\
addi  $sp, $sp, 4\n");
}

void transNEGF(FILE *f)
{
  fprintf(f,"\
l.s  $f2, 0($sp)\n\
neg.s $f2, $f2\n\
s.s  $f2, 0($sp)\n");
}

void transEQF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  eqf\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transNEF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  nef\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLTF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  ltf\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLEF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  lef\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGTF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  gtf\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGEF(FILE *f)
{
  fprintf(f,"\
l.s $f2, 4($sp)\n\
l.s $f4, 0($sp)\n\
jal  gef\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}


void transEQS(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  eqs\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transNES(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  nes\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLTS(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  lts\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transLES(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  les\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGTS(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  gts\n\
sw   $v0, 4($sp)\n\
addi $sp, $sp, 4\n");
}

void transGES(FILE *f)
{
  fprintf(f,"\
lw   $a0, 4($sp)\n\
lw   $a1, 0($sp)\n\
jal  ges\n\
sw   $v0, 4($sp)\n\
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

  preparedCode(f);
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
	case OP_LCF:
	  transLCF(f,code[codePos].qFloat);
	  break;
	case OP_LCS:
	  transLCS(f,code[codePos].q);
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
    case OP_RF:
      transRF(f);
      break;
    case OP_RS:
      transRS(f);
      break;
    case OP_WRC: 
      transWRC(f);
      break;     
    case OP_WRI: 
      transWRI(f);
      break;
    case OP_WRF: 
      transWRF(f);
      break;
    case OP_WRS: 
      transWRS(f);
      break;
    case OP_WLN:
      transWLN(f);
      break;
    case OP_CV: 
      transCV(f);
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
    case OP_ADF:
      transADF(f);
      break;
    case OP_SBF:
      transSBF(f);
      break;
    case OP_MLF:
      transMLF(f);
      break;
    case OP_DVF: 
      transDVF(f);
      break;
    case OP_NEGF:
      transNEGF(f);
      break;
    case OP_EQF:
      transEQF(f);
      break;
    case OP_NEF:
      transNEF(f);
      break;
    case OP_GTF:
      transGTF(f);
      break;
    case OP_LTF:
      transLTF(f);
      break;
    case OP_GEF:
      transGEF(f);
      break;
    case OP_LEF:
      transLEF(f);
      break;
	  
    case OP_EQS:
      transEQS(f);
      break;
    case OP_NES:
      transNES(f);
      break;
    case OP_GTS:
      transGTS(f);
      break;
    case OP_LTS:
      transLTS(f);
      break;
    case OP_GES:
      transGES(f);
      break;
    case OP_LES:
      transLES(f);
      break;

    case OP_LABEL:
	  transLABEL(f,code[codePos].q);
      break;
    default: break;
    }
    codePos++;
  }
  
    prepareData(f, codeBlock);
}
