;****************************************************;
;*�ӳ�������	DISP_ERROR														**
;*���ܣ�			��ʾ��ЧӲ��													**
;*��ڲ�����	DX=������Ϣ�ĵ�ַ											**
;*���ڲ�����																				**
;*ʹ��˵����  																			**
;****************************************************;
DISP_ERROR	PROC	NEAR
	PUSH	DX
	PUSH	BP
	MOV	BP,SP
	CALL	DISP_FRAME
	MOV	AX,RED						;��ʾ�ֵ���ɫ
	PUSH	AX
	;ȡ�ַ����ĵ�ַ
	MOV	AX,WORD PTR [BP+2]	;��ʾ��
	PUSH	AX
	MOV	BX,FRAME_BOTTOM
	MOV	AX,FRAME_TOP
	SUB	BX,AX
	SHR	BX,1
	ADD	AX,BX
	SUB	AX,8								;�ַ��߶���16��ȡһ��
	PUSH	AX								;�����ַ��������� Y ֵ
	;ȡ�ַ�����ַ
	MOV	AX,WORD PTR [BP+2]					;�����ַ��������� X ֵ
	PUSH	AX
	CALL	NEAR PTR STRLEN
	ADD	SP,2
	MOV	CL,3
	SHL	AX,CL								;�ַ��Ŀ��Ϊ8������������
	MOV	CX,FRAME_LEFT
	MOV	DX,FRAME_RIGHT
	SUB	DX,CX
	SUB	DX,AX
	SHR	DX,1
	ADD	DX,CX
	MOV	AX,DX			
	PUSH	AX
	CALL	PUTSTR
	ADD	SP,8	
	;
	MOV	SP,BP
	POP	BP
	POP	DX
	RET
DISP_ERROR	ENDP
;****************************************************;
;*�ӳ�������	DISP_MAIN_SEL													**
;*���ܣ�			��ʾ������ѡ�����										**
;*��ڲ�����	��																		**
;*���ڲ�����																				**
;*ʹ��˵����  																			**
;****************************************************;
DISP_MAIN_SEL	PROC	NEAR
	CALL	DISP_FRAME
	;����������ѡ��˵�
	;����
	MOV	AX,RED
	PUSH	AX
	MOV	DX,OFFSET	STR_SYWAI
	PUSH	DX
	MOV	AX,BUTTON_TOP_WAI
	PUSH	AX
	MOV	AX,BUTTON_LEFT_WAI
	PUSH	AX
	CALL	PUTBUTTON
	ADD	SP,8
	;����
	MOV	AX,GREEN
	PUSH	AX
	MOV	DX,OFFSET	STR_SYNEI
	PUSH	DX
	MOV	AX,BUTTON_TOP_NEI
	PUSH	AX
	MOV	AX,BUTTON_LEFT_NEI
	PUSH	AX
	CALL	PUTBUTTON
	ADD	SP,8
	RET
DISP_MAIN_SEL	ENDP
;****************************************************;
;*�ӳ�������	DISP_FRAME														**
;*���ܣ�			��ʾ�����														**
;*��ڲ�����	��																		**
;*���ڲ�����																				**
;*ʹ��˵����  ��ܵ�λ�������ɵ����ģ���PUTWINDOW��	**
;*						����˵��															**
;****************************************************;
DISP_FRAME	PROC	NEAR
	;���ô��ڼ�������
	MOV	AX,WHITE
	PUSH	AX
	MOV	DX,OFFSET	STR_CAP			;���ñ���
	PUSH	DX
	MOV	AX,LIGHTGRAY
	PUSH	AX
	MOV	AX,FRAME_BOTTOM						;BOTTOM
	PUSH	AX
	MOV	AX,FRAME_RIGHT						;RIGHT
	PUSH	AX
	MOV	AX,FRAME_TOP						;TOP
	PUSH	AX
	MOV	AX,FRAME_LEFT						;LEFT
	PUSH	AX
	CALL	PUTWINDOW
	ADD	SP,14
	RET
DISP_FRAME	ENDP
;****************************************************;
;*�ӳ�������	DISP_SETUP														**
;*���ܣ�			��ʾ��װ����													**
;*��ڲ�����	DX=��ʾ�ĵ�һ���ַ����ĵ�ַ						**
;*���ڲ�����																				**
;*ʹ��˵����  																			**
;****************************************************;
DISP_SETUP	PROC	NEAR
	PUSH	DX
	PUSH	BP
	MOV	BP,SP 						;[BP+2]=��ʾ��һ�е��ַ�����ַ
	CALL	DISP_FRAME
	MOV	AX,YELLOW						;��ʾ�ֵ���ɫ
	PUSH	AX
	;ȡ�ַ����ĵ�ַ
	MOV	AX,WORD PTR [BP+2]	;��ʾ��
	PUSH	AX
	MOV	BX,FRAME_BOTTOM
	MOV	AX,FRAME_TOP
	SUB	BX,AX
	SHR	BX,1
	ADD	AX,BX
	SUB	AX,24								;�ַ��߶���16��ȡһ����
	PUSH	AX								;�����ַ��������� Y ֵ
	;ȡ�ַ�����ַ
	MOV	AX,WORD PTR [BP+2]					;�����ַ��������� X ֵ
	PUSH	AX
	CALL	NEAR PTR STRLEN
	ADD	SP,2
	MOV	CL,3
	SHL	AX,CL								;�ַ��Ŀ��Ϊ8������������
	MOV	CX,FRAME_LEFT
	MOV	DX,FRAME_RIGHT
	SUB	DX,CX
	SUB	DX,AX
	SHR	DX,1
	ADD	DX,CX
	MOV	AX,DX			
	PUSH	AX
	CALL	PUTSTR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+2]					;X
	MOV	BX,[BP+4]					;Y
	POP	BP
	ADD	SP,8
	;
	MOV	AX,YELLOW
	PUSH	AX
	MOV	AX,OFFSET STR_SETUP2
	PUSH	AX
	ADD	BX,32
	PUSH	BX					;Y
	MOV	YCUR,BX
	PUSH	DX					;X
	MOV	XCUR,DX
	CALL	PUTSTR
	ADD	SP,8
	;
	MOV	AX,OFFSET STR_SETUP2
	PUSH	AX
	CALL	NEAR PTR STRLEN
	ADD	SP,2
	MOV	CL,3
	SHL	AX,CL								;�ַ��Ŀ��Ϊ8������������
	MOV	DX,XCUR
	ADD	DX,AX
	MOV	XCUR,DX
	MOV	BX,YCUR
	;��ʾ�༭��
	MOV	AX,2
	PUSH	AX
	PUSH	BX
	PUSH	DX
	CALL	PUTEDITBOX
	ADD	SP,6
	;�����û�����
	CALL	GETUSERIN
	MOV	SP,BP
	POP	BP
	POP	DX
	RET
DISP_SETUP	ENDP
;****************************************************;
;*�ӳ�������	GETUSERIN															**
;*���ܣ�			��ȡ�û�������,��ʾ										**
;*��ڲ�����																				**
;*���ڲ�����	al='Y' 																**
;****************************************************;	 	
GETUSERIN	PROC	NEAR
	XOR	BX,BX
GUI_LOOP:
	XOR	AX,AX
	INT	16H
	CMP	AL,40H
	JNC	GUI_DISPLAY
	CMP	BL,'Y'				;bl�д�ŵ�����һ�ε�����
	JNZ	GUI_LOOP
	CMP	AL,0DH
	JZ	GUI_EXIT
GUI_DISPLAY:
	CMP	AL,60H
	JC	GUI_2CAP			;ת��Ϊ��д
	SUB	AL,20H
GUI_2CAP:
	CMP	AL,'Y'
	JZ	GUI_DISINPUT
	;����ϴε���ʾ
	MOV	CX,WHITE
	PUSH	CX
	MOV	CX,0FFH
	PUSH	CX
	MOV	CX,YCUR
	PUSH	CX
	MOV	CX,XCUR
	PUSH	CX
	CALL	PUTASCII
	ADD	SP,8
	MOV	AL,00H;���������еĳ����У�ʲô������ʾ
GUI_DISINPUT:
	MOV	BL,AL				;�������������bl
	;�ڵ�ǰ��괦��ʾ�����ֵ
	PUSH	BX
	;��ʾ�����ֵ
	MOV	CX,RED
	PUSH	CX
	XOR	AH,AH
	PUSH	AX
	MOV	AX,YCUR
	PUSH	AX
	MOV	AX,XCUR
	PUSH	AX
	CALL	PUTASCII
	ADD	SP,8
	POP	BX
	JMP	GUI_LOOP
GUI_EXIT:
	MOV	AL,BL
	RET
GETUSERIN	ENDP
;****************************************************;
;*�ӳ�������	GETSELECT															**
;*���ܣ�			��ȡ�û���ѡ��												**
;*��ڲ�����																				**
;*���ڲ�����	AL=ѡ��																**
;****************************************************;	
GETSELECT	PROC	NEAR             ;�Ӽ�����ȡ���û�ѡ��
	XOR   AX,AX
	INT  16H
	RET                    
GETSELECT	ENDP
;****************************************************;
;*�ӳ�������	MOUSE_LBUTTON_DOWN_FUN								**
;*���ܣ�			���������µ�ͼ��										**
;*��ڲ�����																				**
;*���ڲ�����																				**
;����˵����		������갴��ʱ��ť����Ļ���					**
;****************************************************;	
MOUSE_LBUTTON_DOWN_FUN	PROC	NEAR
	PUSHA
	PUSH ES
	;�Ƚ����İ��������Ƿ�������button��
	CLC
	MOV	AX,GL_MOUSE_XCORD
	CMP	AX,BUTTON_LEFT_WAI		;�������İ�ť�������ģ�����ֻҪ�Ƚ�һ��
	JAE	HOT_POINT_MORE_LEFT
	JMP	EXCEED_HOT_POINT
HOT_POINT_MORE_LEFT:
	CMP	AX,	BUTTON_LEFT_WAI+BUTTON_WIDTH
	JAE	EXCEED_HOT_POINT			;x������Χ
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_WAI
	JB	EXCEED_HOT_POINT			;y������Χ
	CMP	AX,BUTTON_TOP_WAI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT_WAI	;�Ƚ��Ƿ����������İ�ť������
		;����
		MOV	AX,RED
		PUSH	AX
		MOV	DX,OFFSET	STR_SYWAI
		PUSH	DX
		MOV	AX,BUTTON_TOP_WAI
		PUSH	AX
		MOV	AX,BUTTON_LEFT_WAI
		PUSH	AX
		CALL	PRESS_PUTBUTTON
		ADD	SP,8
		;
	MOV	GL_MOUSE_SELECT,1		;˵�����ѡ���������	
	JMP	HOT_POINT_DONE
EXCEED_HOT_POINT_WAI:			;���űȽ��Ƿ����������İ�ť������
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_NEI
	JB	EXCEED_HOT_POINT			;yû�������ȵ���
	CMP	AX,BUTTON_TOP_NEI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT			;yû�������ȵ���
		;����
		MOV	AX,GREEN
		PUSH	AX
		MOV	DX,OFFSET	STR_SYNEI
		PUSH	DX
		MOV	AX,BUTTON_TOP_NEI
		PUSH	AX
		MOV	AX,BUTTON_LEFT_NEI
		PUSH	AX
		CALL	PRESS_PUTBUTTON
		ADD	SP,8	
	MOV	GL_MOUSE_SELECT,2			;˵�����ѡ���������	
EXCEED_HOT_POINT:
HOT_POINT_DONE:
	POP ES
	POPA
	RET
MOUSE_LBUTTON_DOWN_FUN	ENDP

;****************************************************;
;*�ӳ�������	MOUSE_LBUTTON_UP_FUN									**
;*���ܣ�			�������ɿ���ͼ��										**
;*��ڲ�����																				**
;*���ڲ�����																				**
;����˵����		��������ɿ�ʱ��ť��ʽ��							**
;****************************************************;	
MOUSE_LBUTTON_UP_FUN	PROC	NEAR
	PUSHA
	PUSH ES
	;�Ƚ����İ��������Ƿ�������button��
	CLC
	MOV	AX,GL_MOUSE_XCORD
	CMP	AX,BUTTON_LEFT_WAI		;�������İ�ť�������ģ�����ֻҪ�Ƚ�һ��
	JAE	HOT_POINT_MORE_LEFT1
	JMP	EXCEED_HOT_POINT1
HOT_POINT_MORE_LEFT1:
	CMP	AX,	BUTTON_LEFT_WAI+BUTTON_WIDTH
	JAE	EXCEED_HOT_POINT1			;x������Χ
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_WAI
	JB	EXCEED_HOT_POINT1		;y������Χ
	CMP	AX,BUTTON_TOP_WAI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT_WAI1	;�Ƚ��Ƿ����������İ�ť������
		MOV	GL_MOUSE_TARGET,1			;�����˰�ť������
		;
	JMP	HOT_POINT_DONE1
EXCEED_HOT_POINT_WAI1:			;���űȽ��Ƿ����������İ�ť������
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_NEI
	JB	EXCEED_HOT_POINT1			;yû�������ȵ���
	CMP	AX,BUTTON_TOP_NEI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT1			;yû�������ȵ���
		MOV	GL_MOUSE_TARGET,1		;�����˰�ť������
	JMP	HOT_POINT_DONE1
EXCEED_HOT_POINT1:					;������ť�ķ�Χ������
	MOV	GL_MOUSE_SELECT,0FFH	;�����������ɿ��Ľӿڣ���������
	MOV	GL_MOUSE_TARGET,0
HOT_POINT_DONE1:
	;�������ͣ�ںδ��������»���
		;����
		MOV	AX,RED
		PUSH	AX
		MOV	DX,OFFSET	STR_SYWAI
		PUSH	DX
		MOV	AX,BUTTON_TOP_WAI
		PUSH	AX
		MOV	AX,BUTTON_LEFT_WAI
		PUSH	AX
		CALL	PUTBUTTON
		ADD	SP,8
		;
		;����
		MOV	AX,GREEN
		PUSH	AX
		MOV	DX,OFFSET	STR_SYNEI
		PUSH	DX
		MOV	AX,BUTTON_TOP_NEI
		PUSH	AX
		MOV	AX,BUTTON_LEFT_NEI
		PUSH	AX
		CALL	PUTBUTTON
		ADD	SP,8	
		;
	;�������
	POP ES
	POPA
	RET
MOUSE_LBUTTON_UP_FUN	ENDP