
	.data
firstNumberPrompt: .asciiz "Informe o primeiro numero: "
secondNumberPrompt:	.asciiz "Informe o segundo numero: "
invalidInput: .asciiz "Desculpe\nEntrada Invalida !\n"
resultPrompt: .asciiz "Resultado: "
additionPrompt: .asciiz "\n* * * * SOMAR * * * *\n"
subtractionPrompt: .asciiz "\n* * * SUBTRAIR * * *\n"
encerraPrograma: .asciiz "PROGRAMA ENCERRADO\n"
newLine: .asciiz "\n"
menuPrompt:
	.ascii "\n* * * * MENU * * * *\n"
	.ascii "*                  *\n"
	.ascii "*    SOMAR    (1)  *\n"
	.ascii "*    SUBTRAIR (2)  *\n"
	.ascii "*    SAIR     (3)  *\n"
	.ascii "*                  *\n"
	.ascii "* * * * *  * * * * *\n\n"
	.ascii "Informe a Operacao: "
	.byte 0
	
	.globl main
	.text
main:
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	
mainLoop:
	
	li $s0, 0
	
	la $a0, menuPrompt
	addi $v0, $zero, 4
	syscall
	
	addi $v0, $zero, 5
	syscall
	addi $s0, $v0, 0

	beq $s0, 1, numberPrompt 
	beq $s0, 2, numberPrompt 
	beq $s0, 3, exitLoop 

	bgt $s0, 3, invalidInputLoop 
	bltz $s0, invalidInputLoop

exitLoop: 
	la $a0, encerraPrograma
	addi $v0, $zero, 4
	syscall
	addi $v0, $zero, 10
	syscall

invalidInputLoop: 
	la $a0, invalidInput
	addi $v0, $zero, 4
	syscall
	
	j mainLoop
	
numberPrompt:
	addi $sp, $sp, -4
	sw 	$s1, 0($sp)
	
	beq $s0, 1, doesEqualOne
	beq $s0, 2, doesEqualTwo	
doesEqualOne:
	jal printAdditionLabel
	j endIfStatement
doesEqualTwo:
	jal printSubtractionLabel
	j endIfStatement
endIfStatement:
	j mainLoop
	
printAdditionLabel: 
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	la $a0, additionPrompt
	jal inputNumber
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
	
printSubtractionLabel:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	la $a0, subtractionPrompt
	jal inputNumber
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
	
inputNumber:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	addi $v0, $zero, 4
	syscall 

	la $a0, firstNumberPrompt
	addi $v0, $zero, 4
	syscall
	addi $v0, $zero, 5
	syscall
	addi $t0, $v0, 0
	
	la $a0, secondNumberPrompt
	addi $v0, $zero, 4
	syscall
	addi $v0, $zero, 5
	syscall
	addi $t1, $v0, 0
	
	addi $a0, $t0, 0
	addi $a1, $t1, 0
	beq $s0, 1, addition
	beq $s0, 2, subtraction
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
addition:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	add $a0, $a0, $a1	
	jal result
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
	
subtraction:	
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	sub $a0, $a0, $a1	
	jal result
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
	
result:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	addi $t0, $a0, 0
	
	la $a0, resultPrompt
	addi $v0, $zero, 4
	syscall
	addi $a0, $t0, 0
	addi $v0, $zero, 1
	syscall	
	
	la $a0, newLine
	addi $v0, $zero, 4
	syscall
	
	lw $s1, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 8
	jr $ra
