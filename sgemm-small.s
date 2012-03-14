	.file	"sgemm-small.c"
	.text
	.p2align 4,,15
	.globl	square_sgemm
	.type	square_sgemm, @function
square_sgemm:
.LFB531:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$376, %rsp
	movq	%rdx, -304(%rbp)
	movq	%rcx, -216(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	movl	%edi, %eax
	imull	%edi, %eax
	cltq
	leaq	30(,%rax,4), %rax
	andq	$-16, %rax
	subq	%rax, %rsp
	leaq	15(%rsp), %rdx
	andq	$-16, %rdx
	testl	%edi, %edi
	jle	.L1
	.cfi_offset 3, -56
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
	movslq	%edi, %rbx
	movl	%edi, %ecx
	leal	0(,%rdi,4), %r9d
	andl	$-4, %ecx
	movq	%rbx, -296(%rbp)
	leal	(%rdi,%rdi), %r8d
	salq	$2, %rbx
	movl	%ecx, -308(%rbp)
	movq	%rsi, %r12
	movq	%rbx, -320(%rbp)
	xorl	%r13d, %r13d
	xorl	%r10d, %r10d
	movl	%ecx, %r14d
	movl	%ecx, %r11d
	movq	%rsi, -136(%rbp)
.L3:
	testl	%r11d, %r11d
	je	.L40
	leal	(%rdi,%r10), %ecx
	movq	%r12, %rax
	xorl	%esi, %esi
	.p2align 4,,10
	.p2align 3
.L4:
	movl	(%rax), %r15d
	movl	%ecx, %ebx
	addl	$4, %esi
	subl	%edi, %ebx
	movslq	%ebx, %rbx
	movl	%r15d, (%rdx,%rbx,4)
	movl	4(%rax), %r15d
	movslq	%ecx, %rbx
	movl	%r15d, (%rdx,%rbx,4)
	leal	(%rdi,%rcx), %ebx
	movl	8(%rax), %r15d
	movslq	%ebx, %rbx
	movl	%r15d, (%rdx,%rbx,4)
	leal	(%rcx,%r8), %ebx
	movl	12(%rax), %r15d
	addl	%r9d, %ecx
	addq	$16, %rax
	cmpl	%r11d, %esi
	movslq	%ebx, %rbx
	movl	%r15d, (%rdx,%rbx,4)
	jl	.L4
	cmpl	%r14d, %edi
	movl	%r14d, %eax
	jle	.L6
.L5:
	movq	-136(%rbp), %rbx
	movslq	%eax, %rcx
	addq	%r13, %rcx
	leaq	(%rbx,%rcx,4), %rsi
	movl	%edi, %ecx
	imull	%eax, %ecx
	addl	%r10d, %ecx
	.p2align 4,,10
	.p2align 3
.L7:
	movl	(%rsi), %r15d
	addl	$1, %eax
	movslq	%ecx, %rbx
	addq	$4, %rsi
	addl	%edi, %ecx
	cmpl	%eax, %edi
	movl	%r15d, (%rdx,%rbx,4)
	jg	.L7
.L6:
	addl	$1, %r10d
	addq	-320(%rbp), %r12
	addq	-296(%rbp), %r13
	cmpl	%edi, %r10d
	jne	.L3
	leal	(%rdi,%rdi), %ecx
	movq	-216(%rbp), %rax
	movl	%edi, %esi
	movq	-216(%rbp), %r8
	addq	-320(%rbp), %r8
	andl	$-8, %esi
	movslq	%ecx, %r9
	movl	%esi, -312(%rbp)
	movq	-216(%rbp), %rsi
	leaq	(%rax,%r9,4), %rax
	movq	%r9, -368(%rbp)
	movq	-216(%rbp), %r9
	addl	%edi, %ecx
	movq	$0, -280(%rbp)
	movl	$0, -344(%rbp)
	movq	%rax, -248(%rbp)
	leal	0(,%rdi,4), %eax
	movslq	%ecx, %rbx
	movq	%r8, -240(%rbp)
	leaq	(%rsi,%rbx,4), %rsi
	movq	%rbx, -376(%rbp)
	movslq	%eax, %r8
	movq	-216(%rbp), %rbx
	addl	%edi, %eax
	leaq	(%r9,%r8,4), %r9
	cltq
	movq	%r8, -384(%rbp)
	movq	%rax, -392(%rbp)
	movq	-216(%rbp), %r8
	addl	%ecx, %ecx
	movq	%r9, -264(%rbp)
	movl	-308(%rbp), %r9d
	leaq	(%rbx,%rax,4), %rbx
	leal	0(,%rdi,8), %eax
	movq	%rsi, -256(%rbp)
	movq	-216(%rbp), %rsi
	movslq	%ecx, %rcx
	movq	%rbx, -224(%rbp)
	subl	%edi, %eax
	subl	$1, %r9d
	movq	%rcx, -408(%rbp)
	cltq
	shrl	$2, %r9d
	leaq	(%rsi,%rcx,4), %rsi
	leaq	(%r8,%rax,4), %r8
	movq	%rax, -400(%rbp)
	mov	%r9d, %eax
	leaq	4(,%rax,4), %rax
	movq	%rsi, -232(%rbp)
	movl	%r9d, -348(%rbp)
	movq	%r8, -272(%rbp)
	xorl	%r8d, %r8d
	movq	%rax, -360(%rbp)
	movl	-312(%rbp), %eax
	movl	%eax, -352(%rbp)
.L8:
	movl	-312(%rbp), %eax
	testl	%eax, %eax
	je	.L41
	leal	0(,%rdi,8), %eax
	movq	-392(%rbp), %r9
	movq	-304(%rbp), %rcx
	movq	-400(%rbp), %rbx
	movq	-408(%rbp), %rsi
	cltq
	movl	$0, -196(%rbp)
	movq	%rax, -136(%rbp)
	salq	$2, %rax
	movq	%r9, -176(%rbp)
	movq	%rax, -328(%rbp)
	movq	-384(%rbp), %rax
	movq	-360(%rbp), %r9
	addq	-280(%rbp), %r9
	movq	%rcx, -208(%rbp)
	movq	%rbx, -192(%rbp)
	movq	%rax, -168(%rbp)
	movl	-348(%rbp), %eax
	movq	%rsi, -184(%rbp)
	movq	-376(%rbp), %rcx
	movq	-296(%rbp), %rsi
	movq	-368(%rbp), %rbx
	leal	4(,%rax,4), %eax
	movq	%r9, -336(%rbp)
	movq	%rcx, -160(%rbp)
	movq	%rsi, -144(%rbp)
	movq	%rbx, -152(%rbp)
	xorl	%esi, %esi
	movl	%eax, -340(%rbp)
	.p2align 4,,10
	.p2align 3
.L16:
	movq	-248(%rbp), %rcx
	movq	-216(%rbp), %r9
	movq	-240(%rbp), %rax
	movq	-256(%rbp), %rbx
	movss	(%rcx,%rsi,4), %xmm8
	movq	-232(%rbp), %rcx
	movss	(%r9,%rsi,4), %xmm10
	movq	-264(%rbp), %r9
	movss	(%rax,%rsi,4), %xmm9
	movq	-224(%rbp), %rax
	movss	(%rcx,%rsi,4), %xmm4
	movl	-308(%rbp), %ecx
	movss	(%rbx,%rsi,4), %xmm7
	movq	-272(%rbp), %rbx
	movss	(%r9,%rsi,4), %xmm6
	testl	%ecx, %ecx
	movss	(%rax,%rsi,4), %xmm5
	movss	(%rbx,%rsi,4), %xmm3
	je	.L27
	xorps	%xmm11, %xmm11
	movq	-144(%rbp), %r14
	movq	-152(%rbp), %r13
	movq	-160(%rbp), %r12
	movss	%xmm10, -200(%rbp)
	movq	-168(%rbp), %rbx
	movq	-176(%rbp), %r11
	movss	%xmm9, -284(%rbp)
	movq	-184(%rbp), %r10
	subq	%rsi, %r14
	movss	%xmm8, -288(%rbp)
	movq	-192(%rbp), %r9
	subq	%rsi, %r13
	subq	%rsi, %r12
	movq	-208(%rbp), %rax
	subq	%rsi, %rbx
	subq	%rsi, %r11
	movq	-280(%rbp), %rcx
	movq	-336(%rbp), %r15
	subq	%rsi, %r10
	subq	%rsi, %r9
	movaps	%xmm11, %xmm12
	movaps	%xmm11, %xmm13
	salq	$2, %r14
	movaps	%xmm11, %xmm14
	salq	$2, %r13
	movaps	%xmm11, %xmm15
	salq	$2, %r12
	movaps	%xmm11, %xmm10
	salq	$2, %rbx
	movaps	%xmm11, %xmm9
	salq	$2, %r11
	movaps	%xmm11, %xmm8
	salq	$2, %r10
	salq	$2, %r9
	.p2align 4,,10
	.p2align 3
.L11:
	movups	(%rdx,%rcx,4), %xmm0
	addq	$4, %rcx
	movups	(%rax), %xmm2
	movups	(%rax,%r14), %xmm1
	mulps	%xmm0, %xmm2
	mulps	%xmm0, %xmm1
	addps	%xmm2, %xmm15
	movups	(%rax,%r13), %xmm2
	addps	%xmm1, %xmm14
	movups	(%rax,%r12), %xmm1
	mulps	%xmm0, %xmm2
	mulps	%xmm0, %xmm1
	addps	%xmm2, %xmm13
	movups	(%rax,%rbx), %xmm2
	addps	%xmm1, %xmm12
	movups	(%rax,%r11), %xmm1
	mulps	%xmm0, %xmm2
	mulps	%xmm0, %xmm1
	addps	%xmm2, %xmm8
	movups	(%rax,%r10), %xmm2
	addps	%xmm1, %xmm9
	movups	(%rax,%r9), %xmm1
	mulps	%xmm0, %xmm2
	addq	$16, %rax
	cmpq	%r15, %rcx
	mulps	%xmm0, %xmm1
	addps	%xmm2, %xmm10
	addps	%xmm1, %xmm11
	jne	.L11
	movl	-340(%rbp), %ecx
	movaps	%xmm8, %xmm2
	movaps	%xmm9, %xmm1
	movaps	%xmm10, %xmm0
	movss	-284(%rbp), %xmm9
	movss	-200(%rbp), %xmm10
	movss	-288(%rbp), %xmm8
.L10:
	movups	%xmm15, -128(%rbp)
	xorl	%eax, %eax
	movups	%xmm14, -112(%rbp)
	movups	%xmm13, -96(%rbp)
	movups	%xmm12, -80(%rbp)
.L12:
	addss	-128(%rbp,%rax), %xmm10
	addss	-112(%rbp,%rax), %xmm9
	addss	-96(%rbp,%rax), %xmm8
	addss	-80(%rbp,%rax), %xmm7
	addq	$4, %rax
	cmpq	$16, %rax
	jne	.L12
	movups	%xmm2, -128(%rbp)
	xorb	%al, %al
	movups	%xmm1, -112(%rbp)
	movups	%xmm0, -96(%rbp)
	movups	%xmm11, -80(%rbp)
.L13:
	addss	-128(%rbp,%rax), %xmm6
	addss	-112(%rbp,%rax), %xmm5
	addss	-96(%rbp,%rax), %xmm4
	addss	-80(%rbp,%rax), %xmm3
	addq	$4, %rax
	cmpq	$16, %rax
	jne	.L13
	cmpl	%ecx, %edi
	jle	.L14
	movq	-304(%rbp), %r9
	movslq	%ecx, %rax
	movq	-144(%rbp), %r15
	movq	-152(%rbp), %r14
	movq	-160(%rbp), %r13
	addq	%rsi, %rax
	movq	-168(%rbp), %r12
	movq	-176(%rbp), %rbx
	movq	-184(%rbp), %r11
	movq	-192(%rbp), %r10
	leaq	(%r9,%rax,4), %rax
	subq	%rsi, %r15
	subq	%rsi, %r14
	subq	%rsi, %r13
	subq	%rsi, %r12
	subq	%rsi, %rbx
	subq	%rsi, %r11
	subq	%rsi, %r10
	.p2align 4,,10
	.p2align 3
.L15:
	leal	(%r8,%rcx), %r9d
	movss	(%rax), %xmm1
	addl	$1, %ecx
	movslq	%r9d, %r9
	movss	(%rdx,%r9,4), %xmm0
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm10
	movss	(%rax,%r15,4), %xmm1
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm9
	movss	(%rax,%r14,4), %xmm1
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm8
	movss	(%rax,%r13,4), %xmm1
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm7
	movss	(%rax,%r12,4), %xmm1
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm6
	movss	(%rax,%rbx,4), %xmm1
	mulss	%xmm0, %xmm1
	addss	%xmm1, %xmm5
	movss	(%rax,%r11,4), %xmm1
	mulss	%xmm0, %xmm1
	mulss	(%rax,%r10,4), %xmm0
	addq	$4, %rax
	cmpl	%edi, %ecx
	addss	%xmm1, %xmm4
	addss	%xmm0, %xmm3
	jne	.L15
.L14:
	movq	-216(%rbp), %rax
	movq	-240(%rbp), %rcx
	movq	-248(%rbp), %rbx
	movq	-256(%rbp), %r9
	addl	$8, -196(%rbp)
	movss	%xmm10, (%rax,%rsi,4)
	movq	-264(%rbp), %rax
	movss	%xmm9, (%rcx,%rsi,4)
	movq	-224(%rbp), %rcx
	movss	%xmm8, (%rbx,%rsi,4)
	movss	%xmm7, (%r9,%rsi,4)
	movq	-232(%rbp), %rbx
	movss	%xmm6, (%rax,%rsi,4)
	movq	-272(%rbp), %r9
	movss	%xmm5, (%rcx,%rsi,4)
	movq	-136(%rbp), %rcx
	movss	%xmm4, (%rbx,%rsi,4)
	movq	-328(%rbp), %rax
	movss	%xmm3, (%r9,%rsi,4)
	addq	%rax, -208(%rbp)
	addq	%rcx, -144(%rbp)
	addq	%rcx, %rsi
	addq	%rcx, -152(%rbp)
	addq	%rcx, -160(%rbp)
	addq	%rcx, -168(%rbp)
	addq	%rcx, -176(%rbp)
	addq	%rcx, -184(%rbp)
	addq	%rcx, -192(%rbp)
	movl	-312(%rbp), %ebx
	cmpl	%ebx, -196(%rbp)
	jl	.L16
	movl	-352(%rbp), %r11d
	cmpl	%r11d, %edi
	jle	.L18
.L17:
	movl	%edi, %r10d
	movq	-304(%rbp), %r9
	movl	-348(%rbp), %eax
	imull	%r11d, %r10d
	movq	-216(%rbp), %r13
	movl	-308(%rbp), %r14d
	movq	-304(%rbp), %r15
	leal	4(,%rax,4), %r12d
	movslq	%r10d, %r10
	leaq	(%r9,%r10,4), %rbx
	movq	-360(%rbp), %r9
	addq	-280(%rbp), %r9
	.p2align 4,,10
	.p2align 3
.L24:
	testl	%r14d, %r14d
	movss	0(%r13,%r10,4), %xmm1
	je	.L19
	movq	-280(%rbp), %rax
	xorps	%xmm2, %xmm2
	movq	%rbx, %rcx
	.p2align 4,,10
	.p2align 3
.L20:
	movups	(%rdx,%rax,4), %xmm0
	addq	$4, %rax
	movups	(%rcx), %xmm3
	addq	$16, %rcx
	cmpq	%r9, %rax
	mulps	%xmm3, %xmm0
	addps	%xmm0, %xmm2
	jne	.L20
	movups	%xmm2, -128(%rbp)
	cmpl	%r12d, %edi
	movl	%r12d, %eax
	addss	-128(%rbp), %xmm1
	addss	-124(%rbp), %xmm1
	addss	-120(%rbp), %xmm1
	addss	-116(%rbp), %xmm1
	jle	.L22
.L21:
	movslq	%eax, %rcx
	addq	%r10, %rcx
	leaq	(%r15,%rcx,4), %rcx
	.p2align 4,,10
	.p2align 3
.L23:
	leal	(%rax,%r8), %esi
	addl	$1, %eax
	movslq	%esi, %rsi
	movss	(%rdx,%rsi,4), %xmm0
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpl	%eax, %edi
	addss	%xmm0, %xmm1
	jg	.L23
.L22:
	addl	$1, %r11d
	movss	%xmm1, 0(%r13,%r10,4)
	addq	-320(%rbp), %rbx
	addq	-296(%rbp), %r10
	cmpl	%r11d, %edi
	jg	.L24
.L18:
	addl	$1, -344(%rbp)
	addq	$4, -216(%rbp)
	addl	%edi, %r8d
	movq	-296(%rbp), %rsi
	addq	$4, -240(%rbp)
	addq	$4, -248(%rbp)
	addq	$4, -256(%rbp)
	addq	$4, -264(%rbp)
	addq	$4, -224(%rbp)
	addq	$4, -232(%rbp)
	addq	$4, -272(%rbp)
	addq	%rsi, -280(%rbp)
	cmpl	%edi, -344(%rbp)
	jne	.L8
.L1:
	movq	-56(%rbp), %r8
	xorq	%fs:40, %r8
	jne	.L42
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L27:
	.cfi_restore_state
	xorps	%xmm11, %xmm11
	xorl	%ecx, %ecx
	movaps	%xmm11, %xmm0
	movaps	%xmm11, %xmm1
	movaps	%xmm11, %xmm2
	movaps	%xmm11, %xmm12
	movaps	%xmm11, %xmm13
	movaps	%xmm11, %xmm14
	movaps	%xmm11, %xmm15
	jmp	.L10
.L19:
	xorps	%xmm0, %xmm0
	xorl	%eax, %eax
	movups	%xmm0, -128(%rbp)
	addss	-128(%rbp), %xmm1
	addss	-124(%rbp), %xmm1
	addss	-120(%rbp), %xmm1
	addss	-116(%rbp), %xmm1
	jmp	.L21
.L41:
	xorl	%r11d, %r11d
	jmp	.L17
.L40:
	xorl	%eax, %eax
	jmp	.L5
.L42:
	.p2align 4,,6
	call	__stack_chk_fail
	.cfi_endproc
.LFE531:
	.size	square_sgemm, .-square_sgemm
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
