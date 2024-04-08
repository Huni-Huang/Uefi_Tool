;为控制神郁卡而编写的代码
;功能：直接告诉卡，切换
;
;luobing 2005-7-1  12:12 调试完毕
;CARD_DATA		;单片机中EPPROM的数据
DSEG	SEGMENT
	NEI_OFFSET						DD	00H			;NEI	OFFSET
												DD	00H			;NEI	FRONT
												DD	00H			;NEI	BEHIND
	NEI_MAX								DD	00H			;NEI	MAX
	WAI_OFFSET						DD	00H			;WAI	OFFSET
												DD	00H			;WAI	FRONT
												DD	00H			;WAI	BEHIND
	WAI_MAX								DD	00H			;WAI	MAX
												DB	80 DUP(0) 
	MYZONE_OFFSET					DD	00H			;MYZONE	OFFSET
												DD	00H			;MYZONE	FRONT
												DD	00H			;MYZONE	BEHIND
	MYZONE_MAX						DD	00H			;MYZONE	MAX
	NRZONE								DB	02H			;OS NUMBER
	NRHEAD								DB	10H			
	CD_SECPERCYL					DB	03FH
	B_SWAP								DB	00H
	CD_RESERVED						DB	122 DUP (0)	;[16*16-8*16-4-2]
	CD_SIG								DW	0A55AH
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
	;CALL	JUDGEOSSTATUS
	;CALL	IDENTIFYCARD
	CALL	READEPPROM
	CALL	WRITEEPPROM
	CALL	READEPPROM
	INT	3H
	MOV	AX,4C00H
	INT	21H
;****************************************************;
;*子程序名：	JUDGEOSSTATUS													**
;*功能：			判断当前状态													**
;*入口参数：																				**
;*出口参数：	判断BL的最高位，1为外 0为内						**
;****************************************************;
;
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
;****************************************************;
;*子程序名：	READEPPROM														**
;*功能：			读EPPROM的数据												**
;*入口参数：																				**
;*出口参数：	存储在CARDDATA的数据结构中						**
;****************************************************;
READEPPROM	PROC	NEAR
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
	;读取EPPROM的数据
	XOR	SI,SI					;用si来控制循环，因为在SENDMYCMD中，大部分的寄存器没有保护
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;准备存储EPPROM结构的指针
REPP_LOOP:
	INC	SI
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,SI
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;读命令
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	MOV	WORD PTR ES:[DI],BX					;低位放在前面1f31f4
	INC	DI
	INC	DI
	MOV	WORD PTR ES:[DI],CX 				;高位在后面 1f51f6
	INC	DI
	INC	DI
	CMP	SI,03FH
	JNZ	REPP_LOOP
	
	;关卡
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	RET
READEPPROM	ENDP
;****************************************************;
;*子程序名：	WRITEEPPROM														**
;*功能：			写EPPROM的数据												**
;*入口参数：	在CARDDATA的数据结构中的数据					**
;*出口参数：	BX=1 写成功	 BX=0 写失败							**
;****************************************************;
WRITEEPPROM	PROC	NEAR
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
	;写入EPPROM
	XOR	SI,SI					;用si来控制循环，因为在SENDMYCMD中，大部分的寄存器没有保护
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;准备存储EPPROM结构的指针
WREPP_LOOP:
	INC	SI
	;传送要写的数据
	MOV	AX,WORD PTR ES:[DI]
	PUSH	AX
	INC	DI
	INC	DI
	MOV	AX,WORD PTR ES:[DI]
	PUSH	AX
	INC	DI
	INC	DI
	;
	MOV	AX,SI
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07EH			;写命令
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;
	CMP	SI,03FH
	JNZ	WREPP_LOOP
	;判断是否写成功，直接比较外网的OFFSET值与EPPROM中相应数相同
	;读
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,04				;外网OFFSET在epprom中的位置
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;读命令
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8								;cx:bx中存了所读数据
	MOV	SI,OFFSET WAI_OFFSET
	MOV	AX,WORD PTR [SI]
	MOV DX,WORD PTR [SI+2]
	CMP	AX,BX
	CMP	DX,CX
	MOV	BX,0
	JNZ	WREPP_FAIL
	MOV	BX,1
WREPP_FAIL:
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
WRITEEPPROM	ENDP
;;;
CSEG	ENDS
	END	START

