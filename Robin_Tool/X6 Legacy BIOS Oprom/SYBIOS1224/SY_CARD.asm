;2005-11-17 luobing	Ϊ����ж��Ƿ��һ�ο����ӵ磬���������ӳ���
;****************************************************;
;*�ӳ�������	JUDGEOSSTATUS													**
;*���ܣ�			�жϵ�ǰ״̬													**
;*��ڲ�����																				**
;*���ڲ�����	�ж�BL�����λ��1Ϊ�� 0Ϊ��						**
;****************************************************;
;
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
	;#AMEND 2005-11-23 12:59 �޸ĳ����ܵļ��
	MOV	CX,BX
	MOV	BX,00H
	JNZ	IDENCARD_NOCARD
	;
	MOV	BX,CX
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
;****************************************************;
;*�ӳ�������	READEEPROM														**
;*���ܣ�			��EEPROM������												**
;*��ڲ�����																				**
;*���ڲ�����	�洢��CARDDATA�����ݽṹ��						**
;****************************************************;
READEEPROM	PROC	NEAR
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
	;��ȡEEPROM������
	XOR	SI,SI					;��si������ѭ������Ϊ��SENDMYCMD�У��󲿷ֵļĴ���û�б���
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;׼���洢EEPROM�ṹ��ָ��
REPP_LOOP:
	INC	SI
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,SI
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;������
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	MOV	WORD PTR ES:[DI],BX					;��λ����ǰ��1f31f4
	INC	DI
	INC	DI
	MOV	WORD PTR ES:[DI],CX 				;��λ�ں��� 1f51f6
	INC	DI
	INC	DI
	CMP	SI,03FH
	JNZ	REPP_LOOP
	
	;�ؿ�
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
;*�ӳ�������	WRITEEEPROM														**
;*���ܣ�			дEEPROM������												**
;*��ڲ�����	��CARDDATA�����ݽṹ�е�����					**
;*���ڲ�����	BX=1 д�ɹ�	 BX=0 дʧ��							**
;****************************************************;
WRITEEEPROM	PROC	NEAR
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
	;д��EEPROM
	XOR	SI,SI					;��si������ѭ������Ϊ��SENDMYCMD�У��󲿷ֵļĴ���û�б���
	MOV	SI,0FFFFH
	MOV	AX,DS
	MOV	ES,AX
	MOV	DI,OFFSET NEI_OFFSET				;׼���洢EEPROM�ṹ��ָ��
WREPP_LOOP:
	INC	SI
	;����Ҫд������
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
	MOV	AX,07EH			;д����
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8
	;
	CMP	SI,03FH
	JNZ	WREPP_LOOP
	;�ж��Ƿ�д�ɹ���ֱ�ӱȽ�������OFFSETֵ��EEPROM����Ӧ����ͬ
	;��
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,04				;����OFFSET��EEPROM�е�λ��
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;������
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8								;cx:bx�д�����������
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
WRITEEEPROM	ENDP
;****************************************************;
;*�ӳ�������	GETCARDSTATUS													**
;*���ܣ�			��ȡ����״̬													**
;*��ڲ�����																				**
;*���ڲ�����	bx																		**
;*����˵����  ���ӳ��������ж��Ƿ��ڰ�װ״̬���Ƿ�	**
;*						�Ѿ���װ������												**
;*						�ж��Ƿ��ڰ�װ״̬����10h�Ƚϣ���λΪ1**
;*							���ڰ�װ״̬��Ϊ0���෴							**
;*						�жϿ��Ƿ�װ������ff00h��һ�£�Ϊ		**
;*							0δ��װ��Ϊ1�Ѱ�װ									**
;****************************************************;
GETCARDSTATUS	PROC	NEAR
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
	;��ȡ״̬����
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
GETCARDSTATUS	ENDP
;****************************************************;
;*�ӳ�������	COMPAREOFFSETVAL()										**
;*���ܣ�			�Ƚϼ���õ���offsetֵ�뿨�������		**
;*						�Ƿ���ͬ
;*��ڲ�����																				**
;*���ڲ�����	BX=0 ����ͬ BX=1 ��ͬ									**
;****************************************************;
COMPAREOFFSETVAL	PROC	NEAR
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
	;��ȡEEPROM�е�����offsetֵ����������õ�offset�Ƚ�
	MOV	AX,00H
	PUSH	AX
	PUSH	AX
	MOV	AX,04				;����OFFSET��EEPROM�е�λ��
	XOR	AH,AH
	PUSH	AX
	MOV	AX,07FH			;������
	PUSH	AX
	CALL	SENDMYCMD
	ADD	SP,8								;cx:bx�д�����������
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
COMPAREOFFSETVAL	ENDP
;���³���Ϊ2005-11-17 10:09����
;****************************************************;
;*�ӳ�������	TESTPOWERSTATE												**
;*���ܣ�			�ж��Ƿ��һ�����										**
;*��ڲ�����																				**
;*���ڲ�����	BL 10H��ʾ��һ�� 1fH���ǵ�һ��				**
;****************************************************;
TESTPOWERSTATE	PROC	NEAR
	PUSH	CX
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
	POP	CX
	RET
TESTPOWERSTATE	ENDP
;****************************************************;
;*�ӳ�������	CLEARPOWERSTATE												**
;*���ܣ�			�����һ�������״̬λ								**
;*��ڲ�����																				**
;*���ڲ�����																				**
;****************************************************;
CLEARPOWERSTATE	PROC	NEAR
	PUSH	CX
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
	POP	CX
	RET
CLEARPOWERSTATE	ENDP