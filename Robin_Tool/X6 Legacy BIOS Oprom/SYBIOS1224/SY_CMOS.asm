;2005-11-17 luobing	Ϊ����ж��Ƿ��һ�ο����ӵ磬����һ���ӳ���:clearcmos8dh
;�ض������֣���CMOS��λ����8d,8e���ӳ��������е������(ʮ���ƺ�ʮ������),��ʵӦ����8dh,8eh
;****************************************************;
;*�ӳ�������	READCMOS8D8EH()												**
;*���ܣ�			��ȡCMOS8DH����Ϣ�������Ƿ�						**
;*��ڲ�����	��																		**
;*���ڲ�����	al=01 ������ʾ				AL=0 ��������ʾ	**
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
;*�ӳ�������	CLEARCMOS8E()													**
;*���ܣ�			���8EH bit0													**
;*��ڲ�����																				**
;*���ڲ�����																				**
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
;*�ӳ�������	SETCMOS8E	()													**
;*���ܣ�			����8EH bit0													**
;*��ڲ�����																				**
;*���ڲ�����																				**
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
;*�ӳ�������	QDI_SHUTDOWN	()											**
;*���ܣ�			�ػ� 																	**
;*��ڲ�����																				**
;*���ڲ�����																				**
;*����˵������QDI915�����ϣ��ػ�,ֻ����ϵ�У���	**
;*					Ч����ͨ�ó���													**
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
;*�ӳ�������	CLEARCMOS8D()													**
;*���ܣ�			���8DH bit0	bit1										**
;*��ڲ�����																				**
;*���ڲ�����																				**
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