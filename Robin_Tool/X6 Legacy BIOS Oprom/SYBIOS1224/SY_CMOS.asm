;2005-11-17 luobing	为解决判断是否第一次开机加电，加了一个子程序:clearcmos8dh
;重读程序发现，对CMOS的位，如8d,8e，子程序名称有点混淆，(十进制和十六进制),其实应该是8dh,8eh
;****************************************************;
;*子程序名：	READCMOS8D8EH()												**
;*功能：			读取CMOS8DH的信息，返回是否						**
;*入口参数：	无																		**
;*出口参数：	al=01 允许显示				AL=0 不允许显示	**
;****************************************************;
READCMOS8D8EH	PROC	NEAR
	MOV	DX,072H
	MOV	AL,0DH
	OUT	DX,AL
	MOV	DX,073H
	IN	AL,DX
	TEST	AL,03H				;
	MOV	AL,00H
	JNZ	RCMOS_DIS_NOTAGREE
	MOV	DX,072H
	MOV	AL,0EH
	OUT	DX,AL
	MOV	DX,073H
	IN	AL,DX
	TEST AL,01H
	MOV	AL,00H
	JNZ	RCMOS_DIS_NOTAGREE
	MOV	AL,01H
RCMOS_DIS_NOTAGREE:	
	RET
READCMOS8D8EH	ENDP
;****************************************************;
;*子程序名：	CLEARCMOS8E()													**
;*功能：			清除8EH bit0													**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
CLEARCMOS8E	PROC	NEAR
	MOV	DX,072H
	MOV	AL,0EH
	OUT	DX,AL
	jmp	$+2
	MOV	DX,073H
	IN	AL,DX
	AND	AL,0FEH
	MOV	BL,AL
	;
	MOV	DX,072H
	MOV	AL,0EH
	OUT	DX,AL
	jmp	$+2
	MOV	DX,073H	
	MOV	AL,BL
	OUT	DX,AL
	RET
CLEARCMOS8E	ENDP	
;****************************************************;
;*子程序名：	SETCMOS8E	()													**
;*功能：			设置8EH bit0													**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
SETCMOS8E	PROC	NEAR
	MOV	DX,072H
	MOV	AL,0EH
	OUT	DX,AL
	MOV	DX,073H
	IN	AL,DX
	OR	AL,01H
	MOV	BL,AL
	;
	MOV	DX,072H
	MOV	AL,0EH
	OUT	DX,AL
	MOV	DX,073H	
	MOV	AL,BL
	OUT	DX,AL
	RET
SETCMOS8E	ENDP	
;****************************************************;
;*子程序名：	QDI_SHUTDOWN	()											**
;*功能：			关机 																	**
;*入口参数：																				**
;*出口参数：																				**
;*程序说明：在QDI915主板上，关机,只对这款（系列）有	**
;*					效，非通用程序													**
;****************************************************;
QDI_SHUTDOWN	PROC	NEAR
	CLI
	MOV	DX,0404H
	IN	AX,DX
	OR	AX,03C00H
	MOV	DX,0404H
	OUT	DX,AX
	STI
	RET
QDI_SHUTDOWN	ENDP
;****************************************************;
;*子程序名：	CLEARCMOS8D()													**
;*功能：			清除8DH bit0	bit1										**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
CLEARCMOS8DH	PROC	NEAR
	MOV	DX,072H
	MOV	AL,0DH
	OUT	DX,AL
	jmp	$+2
	MOV	DX,073H
	IN	AL,DX
	AND	AL,NOT 03H
	MOV	BL,AL
	;
	MOV	DX,072H
	MOV	AL,0DH
	OUT	DX,AL
	jmp	$+2
	MOV	DX,073H	
	MOV	AL,BL
	OUT	DX,AL
	RET
CLEARCMOS8DH	ENDP	