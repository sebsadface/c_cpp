	.data
.LC0:	.string	"fib(0) = %ld, expecting 1\n"
.LC1:	.string	"fib(1) = %ld, expecting 1\n"
.LC2:	.string	"fib(2) = %ld, expecting 2\n"
.LC3:	.string	"fib(4) = %ld, expecting 5\n"
.LC4:	.string	"fib(6) = %ld, expecting 13\n"
.LC5:	.string "REGISTER SAVING CONVENTION ERROR DETECTED!\n(this message does not catch all cases)\n"

	.text
fib:
	cmpq	$1, %rdi
	ja	.L8
	movl	$1, %eax
	ret
.L8:
	movq	%rdi, %rbx
	leaq	-2(%rdi), %rdi
	call	fib
	movq	%rax, %rbp
	leaq	-1(%rbx), %rdi
	call	fib
	addq	%rbp, %rax
	ret

# ===============================================================
# you do NOT need to read or understand anything below this point
# ===============================================================

	.globl	main
	.type	main, @function
main:
	subq	$8, %rsp
	movq	$-1, %rbx
	movq	$-1, %rbp
	movl	$0, %edi
	call	fib
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$1, %edi
	call	fib
	movq	%rax, %rdx
	leaq	.LC1(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$2, %edi
	call	fib
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$4, %edi
	call	fib
	movq	%rax, %rdx
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$6, %edi
	call	fib
	movq	%rax, %rdx
	leaq	.LC4(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	cmpq	$-1, %rbx
	jne	.L7
	cmpq	$-1, %rbp
	jne	.L7
.L6:
	movl	$0, %eax
	addq	$8, %rsp
	ret
.L7:
	leaq	.LC5(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	jmp	.L6
