;��������˵����
;									1 ���޲���ϵͳ�����������PS/2��꣬��Ȼ��BIOS����֧��int 15h��c2h����
;									2 һ������£�2001��������Ҳ��֧��USB����꣬����������֧�֣������������
;									3 �ṩ��101h 103h 105h������ʾģʽ�Ľӿڣ�����ģʽ��֧�֣���Ҫ�Ļ���ϵ��^^
;																									                   --ESC luobing 2005-12-23
;ʹ�ñ�����Ҫ�㣺
;                 1 �����������ݶκʹ������ͬһ�����ڣ����������Ҫ�����Լ����ӳ���ʱ������
;										����ds es��ֵ��Ȼ��涨���Լ������ݶΣ�ȱʡ��Ϊ��Ĵ���κͱ������Ĵ������ͬ��
;									2 ���ֻ��Ҫ֪��������Ҽ��Ƿ��ɿ����ǰ��£������ṩ�˽ӿ�
;										����ı������嵽��ĳ����У�����������Ҫ���ṩ���������ݶΣ�INVOKER_DATA_SEG��ֵ��������ĳ�������д
;										;������õ�ȫ�ֱ���
;										GL_MOUSE_LBUTTON_DOWN				DB		0H			;Ϊ1�������������� 0 û�а���,�˱����������ı䣬�����������Ҫ���Ϊ��ʼֵ0h
;										GL_MOUSE_LBUTTON_UP					DB		0H			;Ϊ1���������ɿ��� 0 û���ɿ�,�˱����������ı䣬�����������Ҫ���Ϊ��ʼֵ0h
;										GL_MOUSE_RBUTTON_DOWN				DB		0H			;����Ҽ�������ͬ��
;										GL_MOUSE_RBUTTON_UP					DB		0H			
;										GL_MOUSE_XCORD							DW		?				;��������
;										GL_MOUSE_YCORD							DW		?
;									ע�⣺����ı����Ƿ�����ĳ����е����ݶ��У�������ڳ���ʼ��ʱ����дINVOKER_DATA_SEG
;												���ڱ���������������ӳ�����У�����CS��ֵ����ͬ�ģ���Ȼ����ĳ���Ӧ����64K���ڣ�̫���
;												�Ļ���Ҳ����Ϊ����^^���߿��Ժ�����ϵ���ֻ���:220, �ֻ���13645168631  -���- ^_^
;											  ��дINVOKER_DATA_SEG��������:
;															PUSH	CS
;															POP	ES
;															MOV	BX,OFFSET INVOKER_DATA_SEG
;															PUSH	DS
;															POP	AX
;															MOV	ES:[BX],AX
;                 3 �����Ŀ��Բ���DEMO����
;luobing	2005-12-23 13:55  ������ƽ��ҹ��
;luobing	2005-12-23 15:02  ��ɳ������ܣ����г�����103��ģʽ�µ��Գɹ�
;luobing	2006-2-8	 20:32	�������AMI��BIOS���޷�ʹ�õ����⣬
;------��������-------------------------------------------------------------
;****************************************************;
;*�ӳ�������	TESTMOUSE															**
;*���ܣ�			�������������MOUSE_HANDLER_TEST��		**
;*��ڲ�����																				**
;*���ڲ�����	cf=1 �����������޷��������						**
;*������־��	101��ģʽ����ͨ�� 2005-12-20	luobing	**
;****************************************************;
TESTMOUSE		PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;;�涨��ǰ�����ݶκ͸��Ӷ�
	PUSH	CS
	POP	DS
	PUSH	CS
	POP	ES
	;
	MOV	AX,0C205H
	MOV	BX,100H
	INT	15H
	MOV	AX,0C207H 				;��װ��� INT15H
	MOV	BX,OFFSET MOUSE_HANDLER_TEST
	INT	15H
	MOV	AX,0C200H					;��PS/2���
	MOV	BH,1
	INT	15H
	;
	POPA
	POP	ES
	POP	DS
	RET
TESTMOUSE	ENDP
;****************************************************;
;*�ӳ�������	SETUPMOUSE														**
;*���ܣ�			��װ���������MOUSE_HANDLER��					**
;*��ڲ�����																				**
;*���ڲ�����																				**
;*������־��	101��ģʽ����ͨ�� 2005-12-20	luobing	**
;****************************************************;	
SETUPMOUSE	PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;
	;�涨��ǰ�����ݶκ͸��Ӷ�
	PUSH	CS
	POP	DS
	PUSH	CS
	POP	ES
	;
	MOV	AX,0C205H					;initialize mouse
	MOV	BX,100H
	INT	15H
	;
	MOV	AX,0C207H 				;��װ��� INT15H
	MOV	BX,OFFSET MOUSE_HANDLER
	INT	15H
	MOV	AX,0C200H					;��PS/2���
	MOV	BH,1
	INT	15H
	CALL	SAVE_MOUSE
	;
	POPA
	POP	ES
	POP	DS
	RET
SETUPMOUSE	ENDP
;****************************************************;
;*�ӳ�������	UNINSTALLMOUSE												**
;*���ܣ�			ж�����������MOUSE_HANDLER��					**
;*��ڲ�����																				**
;*���ڲ�����																				**
;*������־��	101��ģʽ����ͨ�� 2005-12-20	luobing	**
;****************************************************;
UNINSTALLMOUSE	PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;�ر�PS/2���
	MOV	AX,0C200H
	MOV	BH,0
	INT	15H
	;����װ��HANDLER(XXXX:XXXX)
	MOV	AX,0
	MOV	ES,AX
	MOV	BX,0
	MOV	AX,0C207H
	INT	15H
	;
	POPA
	POP	ES
	POP	DS
	RET
UNINSTALLMOUSE	ENDP

;------------------------------------------------------
;ϵͳ���õĹ���,
;�˺���������FAR���ã�������õĻ����������Ԥ֪������
;��Ҫ����ջ��������������úܲ�^^
MOUSE_HANDLER	PROC	FAR
	PUSHA
	PUSH	ES
	PUSH	DS
	;ѹջ16+2+2=20
JMP	MOUSE_HANDLER_CODE
;��������������õı���������
INVOKER_DATA_SEG					DW		?
;SVGA����Ƶģʽ��Ϣ��
MOUSE_SVGA_MODE_INFORMATION LABEL BYTE
MOUSE_ModeAttributes			 dw 	? 				; mode attributes
MOUSE_WinAAttributes 			 db 	? 				; window A attributes
MOUSE_WinBAttributes 			 db 	? 				; window B attributes
MOUSE_WinGranularity 			 dw 	? 				; window granularity
MOUSE_WinSize 						 dw 	? 				; window size
MOUSE_WinASegment					 dw 	? 				; window A start segment
MOUSE_WinBSegment					 dw 	? 				; window B start segment
MOUSE_WinFuncPtr					 dd 	? 				; real mode pointer to window function
MOUSE_BytesPerScanLine		 dw 	? 				; bytes per scan line
MOUSE_SVGA_CARE_DATA_LENGTH	EQU	$-MOUSE_SVGA_MODE_INFORMATION
MOUSE_SVGA_NOT_CARE_DATA	 db   (256-MOUSE_SVGA_CARE_DATA_LENGTH) DUP (0);
;
MOUSE_FLAG					DB		0
CURRENT_MODE				DW		?									;��Ļ�ֱ���״̬
SEGINDEXOLD					DW    0FFFFH						;��һ�����Դ��ҳ�κ�
SEGINDEX						DW		0									;�Դ��ҳ�κ�
SEGOFFSET						DW		?									;�Դ��ҳƫ�Ƶ�ַ
;��������״�Ķ��壬�������
;    X����Ϊ����Y����Ϊ����
MOUSETYPE		DW		0000H,0001H,0002H,0003H,0004H,0005H,0006H
						DW		0007H,0008H,0009H,000AH,000BH,000CH
						DW		0101H,0202H,0303H,0404H,0505H,0606H,0707H,0808H,0909H
						DW		010BH,020AH,0309H,0409H,0509H,0609H,0709H,0809H
						DW		030BH,030CH,040DH,040EH,050FH
						DW		060AH,060BH,070CH,070DH,080EH
						DW		0610H,0710H,0810H
;		���ͼ����ڲ�����
						DW		0102H,0103H,0104H,0105H,0106H,0107H,0108H,0109H,010AH
						DW					0203H,0204H,0205H,0206H,0207H,0208H,0209H
						DW								0304H,0305H,0306H,0307H,0308H
						DW											0405H,0406H,0407H,0408H
						DW														0506H,0507H,0508H
						DW																	0607H,0608H
						DW																				0708H
						DW		030AH,040AH,050AH,040BH,050BH,040CH,050CH,060CH,050DH
						DW		060DH,050EH,060EH,070EH,060FH,070FH,080FH
MOUSETYPELEN	EQU	$-MOUSETYPE
MOUSETYPECOUNTER		DW	?
;ע��˴���
SAVEOLD			DB		MOUSETYPELEN  DUP (0H)			;�������λ�õ���Ļͼ�������
SAVENEW			DB		MOUSETYPELEN  DUP (0H)			;���ں�SAVEOLD���齻��ʹ��
MOUSECOLOR	DB		43*2 DUP (BLACK)							;���������ⲿ��ɫ
						DB		47*2 DUP (WHITE)								;���������ڲ���ɫ
;ȫ�ֱ���
MOUSE_STATE				DB		0							;���״̬�ֽ�
MOUSE_STATE_OLD		DB		?							;�ϴ����״̬
MOUSE_COUNT				DB		0							;����FLAG
MOUSE_XCORD				DW		0							;������Ͻǵ�X���꣬��ʼ��0
MOUSE_YCORD				DW		0							;������Ͻǵ�Y���꣬��ʼ��0
MOUSE_XCORDOLD		DW		0							;ǰ�ε����Xλ��
MOUSE_YCORDOLD		DW		0							;ǰ�ε����Yλ��
;����ο�ʼ
MOUSE_HANDLER_CODE:
	PUSH	CS			;DS=CS
	POP	DS
	;��ȡ��ǰ��ʾģʽ����Ϣ
	MOV	AX,04F01H
	MOV	CX,CURRENT_MODE
	PUSH	CS
	POP	ES
	MOV	DI,OFFSET MOUSE_SVGA_MODE_INFORMATION
	INT	10H
	MOV	AX,04F03H
	INT	10H
	MOV	CURRENT_MODE,BX ;��ǰ����ʾģʽ
	MOV	AX,04F05H
	MOV	BX,0100H
	INT	10H
	MOV	SEGINDEXOLD,DX ;��仭���ǰ���Դ��ҳ
	MOV	AX,SS
	MOV	ES,AX
	MOV	BX,SP
	ADD	BX,30				;���ϵͳ������ֵ����λ����SP+10��10+20
	MOV	DX,ES:[BX]			;SAVE IN DX
	;Ϊ���������δ�������ʹ��count(ȫ�ֱ���)
	CMP	MOUSE_COUNT,0
	JZ	MOUSE_H_FIRST								;0:��һ������  mouse��״̬
	CMP	MOUSE_COUNT,1
	JZ	MOUSE_H_SECOND							;1:�ڶ�������  mouse x��������
	CMP	MOUSE_COUNT,2
	JZ	MOUSE_H_THIRD								;2:����������  mouse y��������
;;
MOUSE_H_FIRST:
	MOV	AL,MOUSE_STATE
	MOV	MOUSE_STATE_OLD,AL 							;����һ�ε����״̬����
	ADD	MOUSE_COUNT,1
	MOV	MOUSE_STATE,DL 						;����״̬������MOUSE_STATE��
	MOV	AX,MOUSE_XCORD
	MOV	MOUSE_XCORDOLD,AX
	MOV	AX,MOUSE_YCORD
	MOV	MOUSE_YCORDOLD,AX
	TEST	DX,0001H										;���λΪ1 ��ʾ�������
	JNZ	MOUSE_LBUTTON_DOWN						;Ϊ1ʱ��ʾ����������״̬����������갴����������Ľӿ�
	MOV	AL,MOUSE_STATE_OLD
	TEST	AL,0001H										;�ϴ��ǲ��ǰ�����������?
	JZ		PRE_MOUSE_LBUTTON_NOT
	;�˴���ʾ�������ɿ���
	PUSHA
	PUSH	DS
	PUSH	ES
	MOV	AX,INVOKER_DATA_SEG
	MOV	DS,AX
	MOV	GL_MOUSE_LBUTTON_UP,1
	PUSH	CS
	POP	ES
	MOV	BX,OFFSET MOUSE_XCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_XCORD	
	MOV WORD PTR DS:[BX],AX
	MOV	BX,OFFSET MOUSE_YCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_YCORD	
	MOV WORD PTR DS:[BX],AX
	POP	ES
	POP	DS
	POPA
PRE_MOUSE_LBUTTON_NOT:
	JMP	MOUSE_RBUTTON_DOWN
MOUSE_LBUTTON_DOWN:
;���������µĴ���ӿ�
	PUSHA
	PUSH	DS
	PUSH	ES
	MOV	AX,INVOKER_DATA_SEG
	MOV	DS,AX
	MOV	GL_MOUSE_LBUTTON_DOWN,1
	PUSH	CS
	POP	ES
	MOV	BX,OFFSET MOUSE_XCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_XCORD	
	MOV WORD PTR DS:[BX],AX
	MOV	BX,OFFSET MOUSE_YCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_YCORD	
	MOV WORD PTR DS:[BX],AX
	POP	ES
	POP	DS
	POPA

	JMP	MOUSE_H_OVER
MOUSE_RBUTTON_DOWN:
	TEST	DX,0002H		;�����ж�����Ҽ��Ƿ���
	JNZ	MOUSE_RBUTTON_DOWN_YES	;����Ҽ�����
	MOV	AL,MOUSE_STATE_OLD
	TEST	AL,0002H										;�ϴ��ǲ��ǰ���������Ҽ�?
	JZ		PRE_MOUSE_RBUTTON_NOT
	;�˴���ʾ����Ҽ��ɿ���
	PUSHA
	PUSH	DS
	PUSH	ES
	MOV	AX,INVOKER_DATA_SEG
	MOV	DS,AX
	PUSH	CS
	POP	ES
	MOV	DS:GL_MOUSE_RBUTTON_UP,1
	MOV	BX,OFFSET MOUSE_XCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_XCORD	
	MOV WORD PTR DS:[BX],AX
	MOV	BX,OFFSET MOUSE_YCORD
	MOV	AX,ES:[BX]
	MOV	BX,OFFSET GL_MOUSE_YCORD	
	MOV WORD PTR DS:[BX],AX
	POP	ES
	POP	DS
	POPA
PRE_MOUSE_RBUTTON_NOT:
	JMP	MOUSE_H_OVER
MOUSE_RBUTTON_DOWN_YES:
	;����Ҽ����µĴ���ӿ�
	PUSHA
	PUSH	DS
	MOV	AX,INVOKER_DATA_SEG
	MOV	DS,AX
	MOV	GL_MOUSE_RBUTTON_DOWN,1
	POP	DS
	POPA
	JMP	MOUSE_H_OVER
;Xλ�ƴ�������ֻ�ı�MOUSE_XCORD��ֵ�������������״���ȵ�MOUSE_YCORD�����ʱ���ٻ�
MOUSE_H_SECOND:
	ADD	MOUSE_COUNT,1
	CMP	DX,0
	JNZ	MOUSE_H_MOVED_X					;���x�������ƶ�
	JMP	MOUSE_H_OVER						;û���ƶ�����ת
MOUSE_H_MOVED_X:
	TEST	MOUSE_STATE,10H			;00010000��ʾ�������ƶ�
	JNZ	XNEGATIVE
	ADD	MOUSE_XCORD,DX
	;�ж���Ļ��ʾģʽ���Ӷ��õ���Ļ���ޣ���ֹ����Ƴ���Ļ
	;800x600����xmax��799  1024x768����xmax��1023  640x480����xmax��639
	CMP	CURRENT_MODE,105H
	JZ	MOUSE_H_M1024
	CMP	CURRENT_MODE,103H
	JNZ	MOUSE_H_M640
	CMP	MOUSE_XCORD,799
	JNB	MOUSE_H_BIG800
	JMP	MOUSE_H_OVER
MOUSE_H_BIG800:
	MOV	MOUSE_XCORD,799
	JMP	MOUSE_H_OVER
MOUSE_H_M1024:
	CMP	MOUSE_XCORD,1023
	JNB	MOUSE_H_BIG1024
	JMP	MOUSE_H_OVER
MOUSE_H_BIG1024:
	MOV	MOUSE_XCORD,1023
	JMP	MOUSE_H_OVER
MOUSE_H_M640:
	CMP	MOUSE_XCORD,639
	JNB	MOUSE_H_BIG640
	JMP	MOUSE_H_OVER
MOUSE_H_BIG640:
	MOV	MOUSE_XCORD,639
	JMP	MOUSE_H_OVER
XNEGATIVE:
	NEG	DL							;�������ƶ�����
	SUB	MOUSE_XCORD,DX
	CMP	MOUSE_XCORD,0
	JL	MOUSE_H_XLESS0
	JMP	MOUSE_H_OVER
MOUSE_H_XLESS0:
	MOV	MOUSE_XCORD,0
	JMP	MOUSE_H_OVER
;
MOUSE_H_THIRD:
	MOV	MOUSE_COUNT,0				;���y������
	CMP	DX,0
	JNZ	MOUSE_H_MOVEDY
	JMP	MOUSE_H_COMPLETE				;��ʹy����û��λ��Ҳ������괦����
;�������ԭ�������Ͻǵĵѿ������꣬���������Ϊ��
;���y�᷽��ͬ�ˣ�����Ϊ��
MOUSE_H_MOVEDY:
	TEST	MOUSE_STATE,20H
	JNZ	YNEGATIVE
	;����Ǹ�����ֱ�Ӽ�ȥ
	SUB	MOUSE_YCORD,DX
	CMP	MOUSE_YCORD,0
	JL	MOUSE_H_YLESS0	;�з������Ƚ�,С��ת��
	JMP	MOUSE_H_COMPLETE
MOUSE_H_YLESS0:
	MOV	MOUSE_YCORD,0
	JMP	MOUSE_H_COMPLETE
;���Ϊ����ȡ�������
YNEGATIVE:
	NEG	DL
	ADD	MOUSE_YCORD,DX
	CMP	CURRENT_MODE,105H
	JZ	MOUSE_H_M768
	CMP	CURRENT_MODE,103H
	JNZ	MOUSE_H_M480
	CMP	MOUSE_YCORD,599
	JNB	MOUSE_H_BIG600
	JMP	MOUSE_H_COMPLETE
	;�Ƚ��Ƿ񳬳��˸�����ʾģʽ�Ľ���ֵ
MOUSE_H_BIG600:
	MOV	MOUSE_YCORD,599
	JMP	MOUSE_H_COMPLETE
MOUSE_H_M768:
	CMP	MOUSE_YCORD,767
	JNB	MOUSE_H_BIG768
	JMP	MOUSE_H_COMPLETE
MOUSE_H_BIG768:
	MOV	MOUSE_YCORD,767
	JMP	MOUSE_H_COMPLETE
MOUSE_H_M480:
	CMP	MOUSE_YCORD,479
	JNB	BIG480
	JMP	MOUSE_H_COMPLETE
BIG480:
	MOV	MOUSE_YCORD,479
	JMP	MOUSE_H_COMPLETE
MOUSE_H_COMPLETE:
	;����һ�ε�SAVENEW���Ƶ�SAVEOLD,SAVENEW������ֵ
	PUSH	CS
	POP	AX
	MOV	ES,AX
	MOV	DS,AX
	MOV	SI,OFFSET SAVENEW
	MOV	DI,OFFSET SAVEOLD
	MOV	CX,MOUSETYPELEN
	CLD
	REP MOVSB
	;�ָ���Ļ����SAVEOLDֵ
	CALL	RESTORE_MOUSE
	;���������λ�ô�����Ļ��SAVENEW��
	CALL	SAVE_MOUSE
SHOWMS:
	CALL	SHOW_MOUSE
	JMP	MOUSE_H_OVER
MOUSE_H_OVER:
	
	POP	DS
	POP	ES
	POPA
	RETF
MOUSE_HANDLER	ENDP
	
;----------------------------------------
;****************************************************;
;*�ӳ�������	MOUSE_MAPMEMORY												**
;*���ܣ�			������Ҫ��ʾ�����ʾҳ��ƫ�Ƶ�ַ			**
;*��ڲ�����	EDX:	Y����     EBX:  X����						**
;*���ڲ�����	SEGINDEX:�Դ��ַҳ�ţ�     					**
;*						SEGOFFSET:�Դ��ַƫ��								**
;*����ע��	�ڲ�ͬ��ʾģʽ�£��в�ͬ��ӳ�䷽ʽ		**	
;*						���ӳ����Ѿ�ʵ�ֻ�ҳ��								**
;*������־��	101��ģʽ����ͨ�� 2005-12-20	luobing	**
;****************************************************;
MOUSE_MAPMEMORY	PROC	
	MOV	EAX,EDX 								;EAX->Y
	XOR	ECX,ECX
	MOV	CX,WORD PTR MOUSE_BytesPerScanLine
	;JMP	MOUSE_MM_GGG
;MOUSE_MM_GGG:
;	MOV	ECX,832
	MUL	ECX				;ECX�з���һ�еĵ�����EAX*ECX=Y*һ�еĵ���
	ADD	EAX,EBX 	;ADDRESS��X+Y*һ�еĵ���
	MOV	CL,16
	MOV	EBX,EAX
	SHR	EBX,CL  	;��ַ�е�ҳ�ţ�����16λ
	MOV	SEGINDEX,BX
	AND	EAX,0FFFFH	;��ַƫ��OFFSET
	MOV	SEGOFFSET,AX
;�Ƚ�����һ�����Ƿ���ͬһ��ҳ���ϣ����򲻻�ҳ
	CMP	SEGINDEXOLD,BX
	JZ	INSAMEPAGE
	MOV	SEGINDEXOLD,BX
	MOV	BX,0     ;������û�ҳ�ж�
	MOV	DX,SEGINDEX
	MOV	AX,04F05H
	INT	10H
INSAMEPAGE:
	RET
MOUSE_MAPMEMORY	ENDP
;-------------------------------------------
;****************************************************;
;*�ӳ�������	RESTORE_MOUSE													**
;*���ܣ�			�ָ������λ����Ļ										**
;*��ڲ�����	SAVEOLD�����е�����										**
;*���ڲ�����	�ָ�mouse_xcordold��mouse_ycordλ��		**
;*						������																**						
;****************************************************;
RESTORE_MOUSE	PROC	NEAR
	PUSHAD
	MOV	EBX,0
	MOV	EDX,0
	MOV	ECX,0
	MOV	EAX,0
	MOV	AX,MOUSETYPELEN
	SHR	AX,1
	MOV	MOUSETYPECOUNTER,AX 			;�������ͼ��Ĵ�С
	MOV	DI,0
	MOV	SI,0
RESTORE_MOUSEPIXEL:
	MOV	BX,MOUSE_XCORDOLD							;��ǰһ�����λ��X����ŵ�BX��
	MOV	AX,MOUSETYPE[DI]				;������е�һ����ŵ�AX��
	PUSH	AX
	MOV	CL,8
	SHR	AX,CL
	AND	AX,0FFH									;AX=00XXH��øõ�����ڻ�׼�������ƫ��
	ADD	BX,AX										;ʵ������
	POP	AX
	;
	MOV	DX,MOUSE_YCORDOLD
	AND	AX,0FFH									;AX=00YYH
	ADD	DX,AX 									;ʵ������
	CALL	MOUSE_MAPMEMORY							;���е�ַӳ�����PAGE/OFFET
	MOV	AX,0A000H
	MOV	ES,AX
	MOV	BX,SEGOFFSET
	MOV	AL,SAVEOLD[DI]				;�ָ���Ļԭ����
	MOV	ES:[BX],AL
	INC	DI
	INC	DI						;DI+2ָ����һ��WORD
	DEC	MOUSETYPECOUNTER
	JNZ	RESTORE_MOUSEPIXEL
	POPAD
	RET
RESTORE_MOUSE	ENDP
;------------------------------------------------------
;****************************************************;
;*�ӳ�������	SAVE_MOUSE														**
;*���ܣ�			���浱ǰ���λ�õ���Ļ����						**
;*��ڲ�����	��ǰ�����λ��												**
;*���ڲ�����	SAVENEW�����е�����										**
;****************************************************;	
SAVE_MOUSE	PROC	NEAR
	PUSHAD
	MOV	EBX,0
	MOV	EDX,0
	MOV	ECX,0
	MOV	EAX,0
	MOV	AX,MOUSETYPELEN
	SHR	AX,1
	MOV	MOUSETYPECOUNTER,AX 			;�������ͼ������Ĵ�С
	XOR	DI,DI
	XOR	SI,SI
SAVE_M_SAVEPIXEL:
	MOV	BX,MOUSE_XCORD
	MOV	AX,MOUSETYPE[DI]					;ȡ������������
	PUSH	AX
	MOV	CL,8
	SHR	AX,CL
	AND	AX,0FFH
	ADD	BX,AX 							;��ȡx����
	POP	AX
	MOV	DX,MOUSE_YCORD
	AND	AX,0FFH
	ADD	DX,AX 							;��ȡy����
	CALL	MOUSE_MAPMEMORY		;���ҳ�ź�ƫ��
	MOV	AX,0A000H
	MOV	ES,AX
	MOV	BX,SEGOFFSET
	MOV	AL,BYTE PTR ES:[BX]
	MOV	BYTE PTR SAVENEW[DI],AL
SAVE_M_CT1:
	INC	DI
	INC	DI
	DEC	MOUSETYPECOUNTER
	JNZ	SAVE_M_SAVEPIXEL
	POPAD
	RET
SAVE_MOUSE	ENDP
;-----------------------------------------------------
;****************************************************;
;*�ӳ�������	SHOW_MOUSE														**
;*���ܣ�			�������															**
;*��ڲ�����	�����������ͼ������									**
;*���ڲ�����																				**
;*������־��	101��ģʽ����ͨ�� 2005-12-20	luobing	**
;****************************************************;	
SHOW_MOUSE	PROC	NEAR
	PUSHAD
	MOV	EAX,0
	MOV	EBX,0
	MOV	ECX,0
	MOV	EDX,0
	MOV	AX,MOUSETYPELEN
	SHR	AX,1
	MOV	MOUSETYPECOUNTER,AX
	MOV	DI,0
	MOV	SI,0
LODRMOS:
	MOV	BX,MOUSE_XCORD
	MOV	AX,MOUSETYPE[DI]
	PUSH	AX
	MOV	CL,8
	SHR	AX,CL
	AND	AX,0FFH
	ADD	BX,AX
	POP	AX
	MOV	DX,MOUSE_YCORD
	AND	AX,0FFH
	ADD	DX,AX
	CMP	CURRENT_MODE,105H
	JZ	SHOW_M_E1024
	CMP	CURRENT_MODE,103H
	JNZ	SHOW_M_E640
	CMP	BX,799			;>799,����ʾ
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
SHOW_M_E1024:
	CMP	BX,1023 		;>1023����ʾ
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
SHOW_M_E640:
	CMP	BX,639
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
NOTEXCEED:								;û��Խ��
	CALL	MOUSE_MAPMEMORY							;�������ͼ�ε��ҳ�ź�ƫ��
	MOV	AX,0A000H
	MOV	ES,AX
	MOV	BX,SEGOFFSET
	MOV	AL,BYTE PTR MOUSECOLOR[DI]		;ȡ��ɫ
	MOV BYTE PTR ES:[BX],AL
	JMP	SHOW_M_CONT
SHOW_M_CONT:
	INC	DI
	INC	DI					;DI+2ָ����һ��WORD
	DEC	MOUSETYPECOUNTER	;������һ����
	JNZ	LODRMOS
	POPAD
	RET
SHOW_MOUSE	ENDP
;
MOUSE_HANDLER_TEST	PROC	FAR

	RETF
MOUSE_HANDLER_TEST	ENDP