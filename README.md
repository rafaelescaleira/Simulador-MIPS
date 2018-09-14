Simulador MIPS
==================================
Desenvolvimento de um programa simulador da arquitetura do MIPS, para executar programas em linguagem de máquina MIPS.
• Recebe como entrada um programa em linguagem de máquina MIPS
(instruções e dados)
• Executa instruções do programa, uma após a outra

Instruções de Máquina Simuladas
-------------------------------
• Lógico - aritméticas: add, sub, addi, and, or, xor, nor, andi, ori, sll, srl, slt, slti;
• Multiplicação e divisão: mult, div;
• Acesso à memória: lw, sw, lb, sb;
• Desvios condicionais: beq, bne;
• Desvios incondicionais: j, jr, jal;
• Chamada ao sistema operacional: syscall;
• Serviços simulados: 
  • 1 (print integer);
  • 4 (print string);
  • 5 (read integer);
  • 8 (read string);
  • 10 (exit);
• Outras: mfhi, mflo, lui;

Arquivos de Entrada para Simulador
----------------------------------
• Gerados usando MARS:
  • Abrir programa <programa>.asm no MARS
  • Ir no menu Settings, opção Memory Configuration:
    • Selecionar Configuration: Compact, Data at address 0 
    • Dar Apply
  • Montar programa
  • Ir no menu File, opção Dump Memory: 
    • Selecionar Memory Segment: .text
    • Selecionar Dump Format: Binary
    • Dar Dump To File: <programa>.text
  • Ir no menu File, opção Dump Memory: 
    • Selecionar Memory Segment: .data
    • Selecionar Dump Format: Binary
    • Dar Dump To File: <programa>.data
  
Instruções para Compilação
--------------------------

 `` ~$ gcc simmips. -o simmips -Wall -std=c99 -pedantic ``

Instruções para Execução
------------------------

`` ~$ ./simmips <programa>.text <programa>.data <programa>.reg ``

