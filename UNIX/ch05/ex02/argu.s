	.file	"argu.c"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -28(%rbp)
	movl	%ecx, -32(%rbp)
	movl	%r8d, -36(%rbp)
	movl	%r9d, -40(%rbp)
	movl	-24(%rbp), %eax
	movl	-20(%rbp), %edx
	addl	%eax, %edx
	movl	-28(%rbp), %eax
	addl	%eax, %edx
	movl	-32(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-40(%rbp), %eax
	movl	-36(%rbp), %edx
	addl	%eax, %edx
	movl	144(%rbp), %eax
	imull	16(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	imull	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	foo, .-foo
	.section	.rodata
.LC0:
	.string	"foo value is:%d\n"
.LC1:
	.string	"%p\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$288, %rsp
	movl	%edi, -132(%rbp)
	movq	%rsi, -144(%rbp)
	movl	$8, -8(%rbp)
	movq	-128(%rbp), %rax
	movq	%rax, 8(%rsp)
	movq	-120(%rbp), %rax
	movq	%rax, 16(%rsp)
	movq	-112(%rbp), %rax
	movq	%rax, 24(%rsp)
	movq	-104(%rbp), %rax
	movq	%rax, 32(%rsp)
	movq	-96(%rbp), %rax
	movq	%rax, 40(%rsp)
	movq	-88(%rbp), %rax
	movq	%rax, 48(%rsp)
	movq	-80(%rbp), %rax
	movq	%rax, 56(%rsp)
	movq	-72(%rbp), %rax
	movq	%rax, 64(%rsp)
	movq	-64(%rbp), %rax
	movq	%rax, 72(%rsp)
	movq	-56(%rbp), %rax
	movq	%rax, 80(%rsp)
	movq	-48(%rbp), %rax
	movq	%rax, 88(%rsp)
	movq	-40(%rbp), %rax
	movq	%rax, 96(%rsp)
	movq	-32(%rbp), %rax
	movq	%rax, 104(%rsp)
	movq	-24(%rbp), %rax
	movq	%rax, 112(%rsp)
	movq	-16(%rbp), %rax
	movq	%rax, 120(%rsp)
	movl	-8(%rbp), %eax
	movl	%eax, 128(%rsp)
	movl	$7, (%rsp)
	movl	$6, %r9d
	movl	$5, %r8d
	movl	$4, %ecx
	movl	$3, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	foo
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	-132(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-144(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-4)"
	.section	.note.GNU-stack,"",@progbits
