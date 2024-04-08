;为控制神郁卡而编写的代码
;功能：直接告诉卡，切换
;显示部分因为嫌麻烦，没有写，不过三个函数都是调试通过的
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
;*子程序名：	JUDGEOSSTATUS													**
;*功能：			判断当前状态													**
;*入口参数：																				**
;*出口参数：	判断BL的最高位，1为外 0为内					**
;****************************************************;
JUDGEOSSTATUS	PROC	NEAR
	;开卡
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,01H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;判断卡的状态
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
	;关卡
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
;*子程序名：	TELLCARDSWITCH												**
;*功能：			告诉卡下次关机后切换									**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
TELLCARDSWITCH	PROC	NEAR
	;开卡
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,01H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;设置卡在下次关机后切换
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
	;关卡
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
;*子程序名：	SENDMYCMD															**
;*功能：			发送访问卡的命令											**
;*入口参数：	堆栈中从顶到底：CMD,SUBCMD,DWVALH,DWVALL**
;*																									**
;*出口参数：	CX:BX		1F6..1F3											**
;*调用说明：先将DWVALL,DWVALH压栈,其次SUBCMD,CMD		**
;*					注意调用后平衡堆栈(ADD	SP,8)						**
;****************************************************;
SENDMYCMD	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;为取参数做准备[BP+4]=CMD,[BP+6]=SUBCMD,[BP+8]=DWVALH,[BP+10]=DWVALL
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
	JZ	SMC_HDREADY			;硬盘是否忙，忙的话重启控制器(可直接调用resetdisk)
	MOV	DX,01F6H				;开始重启控制器
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
	OUT	DX,AL					;重启完毕
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
;*子程序名：	IDENTIFYCARD													**
;*功能：			确定卡是否存在												**
;*入口参数：																				**
;*出口参数：	bx=0卡不存在  bx=1卡存在							**
;****************************************************;
IDENTIFYCARD	PROC	NEAR
	;开卡
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
	MOV	BX,00H					;标志：卡不存在
	JNZ	IDENCARD_NOCARD
	;判断是否神郁卡
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
	MOV	BX,00H			  ;标志：卡不存在
	JNZ	IDENCARD_NOCARD
	MOV	BX,01H				;标志：卡存在
IDENCARD_NOCARD:
	PUSH	BX
	;关卡
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

