;Ϊ��������������д�Ĵ���
;���ܣ�ֱ�Ӹ��߿����л�
;��ʾ������Ϊ���鷳��û��д�����������������ǵ���ͨ����
;luobing 20056-29  16:04
DSEG	SEGMENT
;
WAIINFO	DB	'CURRENT OS IS WAI (1)$'
NEIINFO	DB	'CURRENT OS IS NEI (0)$'
ATTENTION	DB 'PRESS "Y" WILL SWITCH,ARE YOU SURE?  $'
DSEG	ENDS
;
CSEG	SEGMENT	
	ASSUME	CS:CSEG,DS:DSEG
START:
	MOV	AX,DSEG
	MOV DS,AX
	CALL	JUDGEOSSTATUS
	CALL	IDENTIFYCARD
	INT	3H
	MOV	AX,4C00H
	INT	21H
;****************************************************;
;*�ӳ�������	JUDGEOSSTATUS													**
;*���ܣ�			�жϵ�ǰ״̬													**
;*��ڲ�����																				**
;*���ڲ�����	�ж�BL�����λ��1Ϊ�� 0Ϊ��					**
;****************************************************;
JUDGEOSSTATUS	PROC	NEAR
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
	;�жϿ���״̬
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,07H
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
JUDGEOSSTATUS	ENDP
;****************************************************;
;*�ӳ�������	TELLCARDSWITCH												**
;*���ܣ�			���߿��´ιػ����л�									**
;*��ڲ�����																				**
;*���ڲ�����																				**
;****************************************************;
TELLCARDSWITCH	PROC	NEAR
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
	;���ÿ����´ιػ����л�
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,05H
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
TELLCARDSWITCH	ENDP
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
	;
	MOV	SP,BP
	POP	BP
	RET
SENDMYCMD	ENDP
;****************************************************;
;*�ӳ�������	IDENTIFYCARD													**
;*���ܣ�			ȷ�����Ƿ����												**
;*��ڲ�����																				**
;*���ڲ�����	bx=0��������  bx=1������							**
;****************************************************;
IDENTIFYCARD	PROC	NEAR
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
	CMP	CX,00H
	CMP	BX,00H
	MOV	BX,00H					;��־����������
	JNZ	IDENCARD_NOCARD
	;�ж��Ƿ�������
	;cx:bx=4350H:5953H 'SYPC'
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,08H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	CMP	CX,04350H
	CMP	BX,05953H
	MOV	BX,00H			  ;��־����������
	JNZ	IDENCARD_NOCARD
	MOV	BX,01H				;��־��������
IDENCARD_NOCARD:
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
IDENTIFYCARD	ENDP
;;;
CSEG	ENDS
	END	START

