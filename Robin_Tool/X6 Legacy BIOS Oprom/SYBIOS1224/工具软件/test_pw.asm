;test prog	luobing	2005-11-16
;�����Ƿ��һ�������
dseg	segment
	dis1	db	'first power!$'
	dis2	db	'not first power!$'
dseg	ends
;
cseg	segment
	assume	cs:cseg,ds:dseg
start:
	mov	ax,dseg
	mov	ds,ax
	;
	;�ж��Ƿ��һ�����
	call	testpowerstate
	cmp	bl,10h
	jnz	not_fir_pwr
	mov	dx,offset dis1
	mov	ah,9
	int	21h
	call	clearpowerstate
not_fir_pwr:
	mov	dx,offset dis2
	mov	ah,9
	int	21h
	;
	;
	mov	ax,4c00h
	int	21h
	
;****************************************************;
;*�ӳ�������	TESTPOWERSTATE												**
;*���ܣ�			�ж��Ƿ��һ�����										**
;*��ڲ�����																				**
;*���ڲ�����	BL 10H��ʾ��һ�� 1fH���ǵ�һ��				**
;****************************************************;
TESTPOWERSTATE	PROC	NEAR
	;����
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,01H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;�ж��Ƿ��һ�����
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,0BH		; 
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	PUSH	BX
	;�ؿ�
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	POP	BX
	RET
TESTPOWERSTATE	ENDP
;****************************************************;
;*�ӳ�������	CLEARPOWERSTATE												**
;*���ܣ�			�����һ�������״̬λ								**
;*��ڲ�����																				**
;*���ڲ�����																				**
;****************************************************;
CLEARPOWERSTATE	PROC	NEAR
	;����
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,01H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;�����һ�������״̬λ
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,0AH		; 
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	PUSH	BX
	;�ؿ�
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	POP	BX
	RET
CLEARPOWERSTATE	ENDP
;****************************************************;
;*�ӳ�������	SENDMYCMD															**
;*���ܣ�			���ͷ��ʿ�������											**
;*��ڲ�����	��ջ�дӶ����ף�CMD,SUBCMD,DWVALH,DWVALL**
;*																									**
;*���ڲ�����	CX:BX		1F6..1F3											**
;*����˵�����Ƚ�DWVALL,DWVALHѹջ,���SUBCMD,CMD		**
;*					ע����ú�ƽ���ջ(ADD	SP,8)						**
;****************************************************;
SENDMYCMD	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;Ϊȡ������׼��[BP+4]=CMD,[BP+6]=SUBCMD,[BP+8]=DWVALH,[BP+10]=DWVALL
	;
	;MOV	DX,EDD_BASEPORT+6
	MOV	DX,01F6H
	MOV	AL,0E0H
	OUT	DX,AL
	MOV	CX,1000H
	LOOP	$				;DELAY TIME
SMC_TESTHDIDLE:
	MOV	DX,01F7H
	IN	AL,DX			;GET DATA FORM 1F7
	CMP	AL,050H
	JZ	SMC_HDREADY			;Ӳ���Ƿ�æ��æ�Ļ�����������(��ֱ�ӵ���resetdisk)
	XOR	BX,BX
	XOR	CX,CX
	TEST	AL,01H				;�����Ƿ������д�
	JNZ	SMCMD_EXIT
	MOV	DX,01F6H				;��ʼ����������
	MOV	AL,0E0H
	OUT	DX,AL
	MOV	DX,03F6H
	MOV	AL,04H
	OUT	DX,AL
	MOV	CX,1000H
	LOOP	$					;DELAY TIME
	MOV	DX,01F6H
	MOV	AL,0E0H
	OUT	DX,AL
	MOV	DX,03F6H
	MOV	AL,08H
	OUT	DX,AL					;�������
	MOV	CX,1000H
	LOOP	$					;DELAY TIME
	JMP	SMC_TESTHDIDLE
SMC_HDREADY:
	MOV	DX,01F2H					;SEND SUBCMD
	MOV	AL,BYTE PTR [BP+6]
	OUT	DX,AL
	;;;;									;SEND	DWVAL,CX:BX DWVAL
	MOV	CX,WORD PTR [BP+8]		;DWVALH
	MOV	BX,WORD PTR [BP+10]		;DWVALL
	;
	MOV	DX,01F3H
	MOV	AL,BL
	OUT	DX,AL
	MOV	DX,01F4H
	MOV	AL,BH
	OUT	DX,AL
	MOV	DX,01F5H
	MOV	AL,CL
	OUT	DX,AL
	MOV	DX,01F6H
	MOV	AL,CH
	OUT	DX,AL
	;
	MOV	DX,01F7H
	MOV	AL,BYTE PTR [BP+4]
	OUT	DX,AL
	;SEND DWVAL OVER
SMC_SVALOVER:
	MOV	CX,1000H					;DELAY TIME
	LOOP	$
	MOV	DX,01F7H
	IN	AL,DX
	TEST	AL,080H
	JNZ	SMC_SVALOVER
	;READ RETURN	DATA
	MOV	DX,01F6H							;RETURN CX:BX
	IN	AL,DX
	MOV	CH,AL
	MOV	DX,01F5H
	IN	AL,DX
	MOV	CL,AL
	MOV	DX,01F4H
	IN	AL,DX
	MOV	BH,AL
	MOV	DX,01F3H
	IN	AL,DX
	MOV	BL,AL
	;
SMCMD_EXIT:
	;
	MOV	SP,BP
	POP	BP
	RET
SENDMYCMD	ENDP
;;
cseg	ends
	end	start