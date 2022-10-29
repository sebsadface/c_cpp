	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 3	sdk_version 12, 3
	.globl	_factorial                      ## -- Begin function factorial
	.p2align	4, 0x90
_factorial:                             ## @factorial
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	testl	%edi, %edi
	js	LBB0_1
## %bb.2:
	addl	$1, %edi
	xorl	%ecx, %ecx
	movl	$1, %eax
	.p2align	4, 0x90
LBB0_3:                                 ## =>This Inner Loop Header: Depth=1
	imull	%ecx, %eax
	addl	$1, %ecx
	cmpl	%ecx, %edi
	jne	LBB0_3
## %bb.4:
	popq	%rbp
	retq
LBB0_1:
	movl	$1, %eax
	popq	%rbp
	retq
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$5, %edi
	callq	_factorial
	leaq	L_.str(%rip), %rdi
	movl	$5, %esi
	movl	%eax, %edx
	xorl	%eax, %eax
	callq	_printf
	xorl	%eax, %eax
	popq	%rbp
	retq
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"factorial(%d) = %d\n"

.subsections_via_symbols
