;2005-11-17 luobing	为解决判断是否第一次开机加电，加了两个子程序
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
	XOR	BX,BX
	XOR	CX,CX
	TEST	AL,01H				;看看是否命令有错
	JNZ	SMCMD_EXIT
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
SMCMD_EXIT:
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
	;#AMEND 2005-11-23 12:59 修改成严密的检查
	MOV	CX,BX
	MOV	BX,00H
	JNZ	IDENCARD_NOCARD
	;
	MOV	BX,CX
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
;*子程序名：	READEEPROM														**
;*功能：			读EEPROM的数据												**
;*入口参数：																				**
;*出口参数：	存储在CARDDATA的数据结构中						**
;****************************************************;
READEEPROM	PROC	NEAR
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
	;读取EEPROM的数据
	XOR	SI,SI					;用si来控制循环，因为在SENDMYCMD中，大部分的寄存器没有保护
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;准备存储EEPROM结构的指针
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
READEEPROM	ENDP
;****************************************************;
;*子程序名：	WRITEEEPROM														**
;*功能：			写EEPROM的数据												**
;*入口参数：	在CARDDATA的数据结构中的数据					**
;*出口参数：	BX=1 写成功	 BX=0 写失败							**
;****************************************************;
WRITEEEPROM	PROC	NEAR
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
	;写入EEPROM
	XOR	SI,SI					;用si来控制循环，因为在SENDMYCMD中，大部分的寄存器没有保护
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;准备存储EEPROM结构的指针
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
	;判断是否写成功，直接比较外网的OFFSET值与EEPROM中相应数相同
	;读
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,04				;外网OFFSET在EEPROM中的位置
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
	MOV	BX,0
	JNZ	WREPP_FAIL
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
WRITEEEPROM	ENDP
;****************************************************;
;*子程序名：	GETCARDSTATUS													**
;*功能：			读取卡的状态													**
;*入口参数：																				**
;*出口参数：	bx																		**
;*函数说明：  此子程序用来判断是否在安装状态，是否	**
;*						已经安装过卡。												**
;*						判断是否在安装状态，与10h比较，此位为1**
;*							不在安装状态，为0则相反							**
;*						判断卡是否安装过：与ff00h与一下，为		**
;*							0未安装，为1已安装									**
;****************************************************;
GETCARDSTATUS	PROC	NEAR
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
	;读取状态数据
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,07H
	PUSH	AX
	MOV	AX,07DH
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;
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
GETCARDSTATUS	ENDP
;****************************************************;
;*子程序名：	COMPAREOFFSETVAL()										**
;*功能：			比较计算得到的offset值与卡中所存的		**
;*						是否相同
;*入口参数：																				**
;*出口参数：	BX=0 不相同 BX=1 相同									**
;****************************************************;
COMPAREOFFSETVAL	PROC	NEAR
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
	;读取EEPROM中的外网offset值，与计算所得的offset比较
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,04				;外网OFFSET在EEPROM中的位置
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;读命令
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8								;cx:bx中存了所读数据
	MOV	SI,OFFSET OFFSETVAL
	MOV	AX,WORD PTR [SI]
	MOV DX,WORD PTR [SI+2]
	CMP	AX,BX
	MOV	BX,0
	JNZ	COMP_OFFVAL_NOTMATCH
	CMP	DX,CX
	MOV	BX,0
	JNZ	COMP_OFFVAL_NOTMATCH
	MOV	BX,1
COMP_OFFVAL_NOTMATCH:
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
COMPAREOFFSETVAL	ENDP
;以下程序为2005-11-17 10:09所加
;****************************************************;
;*子程序名：	TESTPOWERSTATE												**
;*功能：			判断是否第一次起机										**
;*入口参数：																				**
;*出口参数：	BL 10H表示第一次 1fH不是第一次				**
;****************************************************;
TESTPOWERSTATE	PROC	NEAR
	PUSH	CX
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
	;判断是否第一次起机
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
	POP	CX
	RET
TESTPOWERSTATE	ENDP
;****************************************************;
;*子程序名：	CLEARPOWERSTATE												**
;*功能：			清除第一次起机的状态位								**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
CLEARPOWERSTATE	PROC	NEAR
	PUSH	CX
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
	;清除第一次起机的状态位
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
	POP	CX
	RET
CLEARPOWERSTATE	ENDP