;PUBLIC PRESS_PUTBUTTON
;�ӳ���
;****************************************************;
;*�ӳ�������	INITVIDEO															**
;*���ܣ�			��ʼ����ʾ��ģʽ���Ĵ�����������			**
;*��ڲ�����	��																		**
;*���ڲ�����	��																		**
;****************************************************;
INITVIDEO	PROC	NEAR
	PUSHA
	MOV	BX,SYVIDEOMODE
	CALL	SETMODEL
	;
	MOV	AX,04F01H
	MOV	CX,SYVIDEOMODE	
	PUSH	DS
	POP	ES
	MOV	DI,OFFSET SVGA_MODE_INFORMATION
	INT	10H
	;
	MOV	AX,OFFSET	PALETTEDATA
	PUSH	AX
	CALL	INITPALETTE
	POP	CX			;ƽ���ջ
	;
	MOV	BX,DEEPBLUE
	CALL	SETBACKCOLOR
	POPA
	RET
INITVIDEO	ENDP
;****************************************************;
;*�ӳ�������	SETMODEL															**
;*���ܣ�			������ʾģʽ													**
;*��ڲ�����	BX=mode																**
;*���ڲ�����	��																		**
;****************************************************;
SETMODEL	PROC	NEAR
	PUSH	AX
	MOV	AX,04F02H
	INT	10H
	POP	AX
	RET
SETMODEL	ENDP
;****************************************************;
;*�ӳ�������	INITPALETTE														**
;*���ܣ�			��ɫ�Ĵ�������												**
;*��ڲ�����	��ջ��Ϊ���ݵ���ʼ��ַ								**
;*���ڲ�����	��																		**
;****************************************************;
INITPALETTE	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+4]=DATA POINT
	PUSH	SI
	MOV	DX,03C8H
	MOV	AL,0
	OUT	DX,AL
  DB	0EBH					;��ʱ
  DB	00H
  DB	0EBH
  DB	00H						;��ʱ
  ;
  XOR	SI,SI
  JMP	SHORT	INITPALADD
INITPALLOOP:
	MOV	BX,WORD	PTR	[BP+4]
	ADD	BX,SI
	MOV	AL,BYTE	PTR	[BX]
	XOR	AH,AH
	MOV	CL,2
	SAR	AX,CL
	MOV	DX,03C9H
	OUT	DX,AL
	INC	SI
INITPALADD:
	CMP	SI,0300H
	JL	SHORT	INITPALLOOP
  ;	
	POP	SI
	POP	BP
	RET
INITPALETTE	ENDP
;****************************************************;
;*�ӳ�������	SETBACKCOLOR													**
;*���ܣ�			���ñ���															**
;*��ڲ�����	bx=color															**
;*���ڲ�����																				**
;****************************************************;	
SETBACKCOLOR	PROC	NEAR
	PUSH	AX
	PUSH	CX
	PUSH	DX
	PUSH	BP
	;
	MOV	AX,SCREEN_WIDTH
	MOV	CX,SCREEN_HEIGHT
	MUL	CX
	INC	DX						;????
	PUSH	BX					;1 COLOR	[BP+2]
	PUSH	DX					;2 PAGE  			[BP]
	MOV	BP,SP
	PUSH	ES					;
	CLC
	CLD
	XOR	DX,DX
	MOV	DI,0A000H
	MOV	ES,DI
	XOR	DI,DI
SETBACKCOLOR1:
		PUSH	DX
		MOV	AX,04F05H
		MOV	BX,0
		INT	10H
		MOV	AX,[BP+2]
		MOV	AH,AL
		MOV	CX,08000H
		REP	STOSW
		POP	DX
		INC	DX
		CMP	DX,[BP]
		JC	SETBACKCOLOR1
		POP	ES
		ADD	SP,4
		POP	BP
		POP	DX
		POP	CX
		POP	AX
		RET
SETBACKCOLOR	ENDP
;****************************************************;
;*�ӳ�������	VLINE																	**
;*���ܣ�			������																**
;*��ڲ�����	��ջ�дӶ����ף�y1(word),y2,x,				**
;*														color(word)	(X1<X2)		**
;*���ڲ�����																				**
;*����˵�����Ƚ�colorѹջ,���x,y2,y1,���뱣֤y1<y2	**
;*					ע����ú�ƽ���ջ											**
;****************************************************;
VLINE	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	PUSH	SI
	PUSH	DI
	;
	PUSH		ES
	MOV		AX,[BP+4]	
	MOV		CX,SCREEN_WIDTH	
	MUL		CX
	ADD		AX,[BP+8]
	ADC		DX,0
	MOV		DI,0A000H
	MOV		ES,DI
	MOV		DI,AX
	MOV		AX,04F05H
	MOV		BX,0
	INT		010H
	MOV		CX,[BP+6]	
	SUB		CX,[BP+4]
	INC		CX
	MOV		AX,[BP+10]
	CLD	
VLINELOOP:	
	MOV		ES:[DI],AL
	ADD		DI,SCREEN_WIDTH
	JNC	SHORT VLINENOCHANPAGE
	INC		DX
	PUSH		AX
	MOV		AX,04F05H
	MOV		BX,0
	INT		010H
	POP		AX
VLINENOCHANPAGE:
	LOOP	SHORT VLINELOOP
	;
	POP	ES
  POP	DI
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
VLINE	ENDP
;

;****************************************************;
;*�ӳ�������	HLINE																	**
;*���ܣ�			������																**
;*��ڲ�����	��ջ�дӶ����ף�x1(word),x2,y,				**
;*														color(word)	(X1<X2)		**
;*���ڲ�����																				**
;*����˵�����Ƚ�colorѹջ,���y,x2,x1,���뱣֤x1<x2	**
;*					ע����ú�ƽ���ջ											**
;****************************************************;
HLINE	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;Ϊȡ������׼��[BP+4]=x1,[BP+6]=x2,[BP+8]=y,[BP+10]=color...
	PUSH	SI
	PUSH	DI
	;
	PUSH		ES
 	MOV		AX,[BP+8]
	MOV		CX,SCREEN_WIDTH
  MUL		CX
  ADD		AX,[BP+4]
  ADC		DX,0
  MOV		DI,0A000H
 	MOV		ES,DI
  MOV		DI,AX
	MOV		AX,04F05H
	MOV		BX,0
	INT		010H
	MOV		CX,[BP+6]
	SUB		CX,[BP+4]
	INC		CX
	MOV		AX,[BP+10]
	CLD	
HLINELOOP:
	STOSB	
	OR		DI,DI
	JNE	SHORT HLINENOCHANPAGE
	PUSH		AX
	MOV		AX,04F05H
	MOV		BX,0
	INC		DX
	INT		010H
	POP		AX
HLINENOCHANPAGE:
	LOOP	SHORT HLINELOOP
	;
	POP		ES
   ;
	POP	DI
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
HLINE	ENDP

;****************************************************;
;*�ӳ�������	DRAWRECT															**
;*���ܣ�			������																**
;*��ڲ�����	��ջ�дӶ����ף�left(word),top,right	**
;*														bottom	color					**
;*���ڲ�����																				**
;*����˵�����Ƚ�colorѹջ,���BOTTOM,RIGHT,TOP,LEFT	**
;*					��֤left<right,top<bottom								**
;*					ע����ú�ƽ���ջ											**
;****************************************************;
DRAWRECT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;Ϊȡ������׼��[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=RIGHT,[BP+10]=BOTTOM,[BP+12]=COLOR
	PUSH	SI
	PUSH	DI
	;
	MOV	DI,WORD PTR [BP+4]
	MOV	SI,WORD PTR [BP+12]
  PUSH	SI								;		HLINE(LEFT,RIGHT,top,COLOR);
	PUSH	WORD PTR [BP+6]
	PUSH	WORD PTR [BP+8]
	PUSH	DI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;
	PUSH	SI							 ;		HLINE(LEFT,RIGHT,BOTTOM,COLOR);
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+8]
	PUSH	DI
	CALL	NEAR PTR HLINE
	ADD	SP,8							 
	;	
	PUSH	SI							;		VLINE(TOP,BOTTOM,LEFT,COLOR);
	PUSH	DI
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+6]
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;	
 	PUSH	SI								;		VLINE(TOP,BOTTOM,RIGHT,COLOR);
	PUSH	WORD PTR [BP+8]
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+6]
	CALL	NEAR PTR VLINE
	ADD	SP,8
	;
 	POP	DI
	POP	SI
	POP	BP
	RET	
DRAWRECT	ENDP
;****************************************************;
;*�ӳ�������	PUTPIXEL															**
;*���ܣ�			д���ص�															**
;*��ڲ�����	��ջ�дӶ����ף�x(word),y,color				**
;*���ڲ�����	��																		**
;*����˵�����Ƚ�colorѹջ,���y,x										**
;*					ע����ú�ƽ���ջ											**
;****************************************************;
PUTPIXEL	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;Ϊȡ������׼�� [BP+4]=X,[BP+6]=Y,[BP+8]=COLOR
	PUSH	DI
	PUSH		ES			
  ;
  MOV		AX,[BP+6]			
 	MOV		BX,[BP+4]			
  MOV		CX,SCREEN_WIDTH 
  MUL		CX
	ADD		AX,BX	
	ADC		DX,0
	MOV		DI,0A000H
	MOV		ES,DI
	MOV		DI,AX
	MOV		AX,04F05H
	MOV		BX,0
	INT		010H	
	MOV		AX,[BP+8]
	STOSB	
	;
	POP		ES	
	POP	DI
	POP	BP
	RET	
PUTPIXEL	ENDP

;****************************************************;
;*�ӳ�������	PUTASCII															**
;*���ܣ�			��ʾӢ��															**
;*��ڲ�����	��ջ�дӶ����ף�X(word),Y,CODE				**
;*																		color					**
;*���ڲ�����																				**
;*����˵�����Ƚ�colorѹջ,���CODE,Y,X							**
;*	ax,bx,cx,dx��Ӱ��		ע����ú�ƽ���ջ					**
;****************************************************;
PUTASCII	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;Ϊȡ������׼��[BP+4]=X,[BP+6]=Y,[BP+8]=CODE,[BP+10]=COLOR
	SUB	SP,4
	PUSH	SI
   ;
	MOV	WORD PTR [BP-2],0
	JMP	SHORT PUTASCIISCH				;BEGIN:		SERCH FOR CHAR IN ASCII TABLE
PUTASCIICHLOOP:		
	MOV	AX,WORD PTR [BP-2]
	MOV	DX,17										;CODE+ENGLISH	MODEL
	IMUL	DX
	MOV	BX,AX
	MOV	AL,BYTE PTR ASCIITABLE[BX]
	CMP	AL,BYTE PTR [BP+8]
	JNE	SHORT PUTASCIIUNFIT
  JMP	SHORT PUTASCIIFIT
PUTASCIIUNFIT:
	INC	WORD PTR [BP-2]
PUTASCIISCH:
	CMP	WORD PTR [BP-2],MAXASCII
	JL	SHORT PUTASCIICHLOOP
PUTASCIIFIT:										;END:		SERCH FOR CHAR IN ASCII TABLE,SUCCESS
   ;
	MOV	AX,WORD PTR [BP-2]
	MOV	WORD PTR [BP-4],AX
 	XOR	SI,SI
	JMP	SHORT PUTASCIIDISWAIADD
PUTASCIIDISNEI:
	MOV	WORD PTR [BP-2],0
	JMP	SHORT PUTASCIIDISNEIADD
PUTASCIIDIS:									;DISPALY CHAR IS OR NOT
 ;
	MOV	AX,WORD PTR [BP-4]
	MOV	DX,17
	IMUL	DX
	ADD	AX,SI
	MOV	BX,AX
	MOV	AL,BYTE PTR ASCIITABLE[BX+1]
	MOV	AH,0
	MOV	CL,7
	SUB	CL,BYTE PTR [BP-2]
	MOV	DX,1
	SHL	DX,CL
	TEST	AX,DX
	JE	SHORT PUTASCIIDISNO				;CAN NOT DISPLAY THSI PIXEL,JUMP
  PUSH	WORD PTR [BP+10]				;DISPLAY CHAR
	MOV	AX,WORD PTR [BP+6]
	ADD	AX,SI
	PUSH	AX
	MOV	AX,WORD PTR [BP+4]
	ADD	AX,WORD PTR [BP-2]
	PUSH	AX
	CALL	NEAR PTR PUTPIXEL
	ADD	SP,6
PUTASCIIDISNO:
	INC	WORD PTR [BP-2]
PUTASCIIDISNEIADD:
	CMP	WORD PTR [BP-2],8
	JL	SHORT PUTASCIIDIS
	INC	SI
PUTASCIIDISWAIADD:
	CMP	SI,16
	JL	SHORT PUTASCIIDISNEI
   ;
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
PUTASCII	ENDP

;****************************************************; 
;*�ӳ�������	PUTHZ																	** 
;*���ܣ�			��ʾ����															** 
;*��ڲ�����	��ջ�дӶ����ף�X(word),Y,CODE				** 
;*																		color					** 
;*���ڲ�����																				** 
;*����˵�����Ƚ�colorѹջ,���CODE,Y,X							** 
;*	ax,bx,cx,dx��Ӱ��	ע����ú�ƽ���ջ						** 
;****************************************************;
PUTHZ	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+4]=X,[BP+6]=Y,[BP+8]=CODE,[BP+10]=COLOR
	SUB	SP,2
	PUSH	SI
	PUSH	DI
  ;
	MOV	WORD PTR [BP-2],0
	JMP	SHORT PUTHZFCODECOMP
PUTHZFCODE:
  ;	
	MOV	AX,WORD PTR [BP-2]
	MOV	DX,34									;LENGTH OF HZ MODEL
	IMUL	DX
	MOV	BX,AX
	MOV	AX,WORD PTR HZK16TABLE[BX]
	;XCHG	AL,AH;;;������ջ�еĺ��ָߵ��෴�������Է��֣���ֱ�Ӵ��ͺ�������ʱ����������ܱȽ�
	CMP	AX,WORD PTR [BP+8]
	JNE	SHORT PUTHZFCODEADD
   ;	
   ;				BREAK;
   ;	
	JMP	SHORT PUTHZFCODEOK
PUTHZFCODEADD:
	INC	WORD PTR [BP-2]
PUTHZFCODECOMP:
	MOV	AX,WORD PTR [BP-2]
	CMP	AX,WORD PTR HZCOUNT
	JL	SHORT PUTHZFCODE
PUTHZFCODEOK:								;FIND CODE IN HZTABLE OR WENT TO END
   ;	
	MOV	DI,WORD PTR [BP-2]	;INDEX=END OF TABLE
  ;	
	CMP	DI,WORD PTR HZCOUNT
	JNE	SHORT PUTHZDISPLAY		;�ҵ�����ʾ
	JMP	PUTHZFCODEFAIL				;û�ҵ����˳�
PUTHZDISPLAY:
   ;
	XOR	SI,SI
	JMP	PUTHZDISPWAI
PUTHZDISPNEI1:								;DISPLAYNEI1
   ;	
	MOV	WORD PTR [BP-2],0
	JMP	SHORT PUTHZDISPNEICOMP
PUTHZDISPNEI1CODE:
  ;	
	MOV	CL,7
	SUB	CL,BYTE PTR [BP-2]
	MOV	AX,1
	SHL	AX,CL
	PUSH	AX
	MOV	AX,DI
	MOV	DX,34
	IMUL	DX
	MOV	DX,SI
	SHL	DX,1
	ADD	AX,DX
	MOV	BX,AX
	POP	AX
	TEST	AX,WORD PTR HZK16TABLE[BX+2]
	JE	SHORT PUTHZDISPNEI1NO							;NO DISPLAY
   ;	
	PUSH	WORD PTR [BP+10]
	MOV	AX,WORD PTR [BP+6]
	ADD	AX,SI
	PUSH	AX
	MOV	AX,WORD PTR [BP+4]
	ADD	AX,WORD PTR [BP-2]
	PUSH	AX
	CALL	NEAR PTR PUTPIXEL
	ADD	SP,6
PUTHZDISPNEI1NO:
	INC	WORD PTR [BP-2]
PUTHZDISPNEICOMP:
	CMP	WORD PTR [BP-2],8
	JL	SHORT PUTHZDISPNEI1CODE
   ;	
	MOV	WORD PTR [BP-2],0
	JMP	SHORT PUTHZDISPNEI2CNMP						;DISPLAYNEI2
PUTHZDISPNEI2CODE:
   ;	
	MOV	CL,15
	SUB	CL,BYTE PTR [BP-2]
	MOV	AX,1
	SHL	AX,CL
	PUSH	AX
	MOV	AX,DI
	MOV	DX,34
	IMUL	DX
	MOV	DX,SI
	SHL	DX,1
	ADD	AX,DX
	MOV	BX,AX
	POP	AX
	TEST	AX,WORD PTR HZK16TABLE[BX+2]
	JE	SHORT PUTHZDISPNEI2ADD
   ;	
	PUSH	WORD PTR [BP+10]
	MOV	AX,WORD PTR [BP+6]
	ADD	AX,SI
	PUSH	AX
	MOV	AX,WORD PTR [BP+4]
	ADD	AX,WORD PTR [BP-2]
	ADD	AX,8
	PUSH	AX
	CALL	NEAR PTR PUTPIXEL
	ADD	SP,6
PUTHZDISPNEI2ADD:
	INC	WORD PTR [BP-2]
PUTHZDISPNEI2CNMP:
	CMP	WORD PTR [BP-2],8
	JL	SHORT PUTHZDISPNEI2CODE
	INC	SI
PUTHZDISPWAI:
	CMP	SI,16
	JGE	PUTHZDISPEXIT
	JMP	PUTHZDISPNEI1
PUTHZDISPEXIT:
PUTHZFCODEFAIL:
   ;	
	POP	DI
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
PUTHZ	ENDP

;****************************************************; 
;*�ӳ�������	PUTSTR																** 
;*���ܣ�			��ʾ�ַ���														** 
;*��ڲ�����	��ջ�дӶ����ף�X(word),Y,S(�ַ�����ַ** 
;*																		color					** 
;*���ڲ�����																				** 
;*����˵�����Ƚ�colorѹջ,���S,Y,X									** 
;*					ע����ú�ƽ���ջ											** 
;****************************************************;	
PUTSTR	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;Ϊȡ������׼��[BP+4]=X,[BP+6]=Y,[BP+8]=OFFSET S,[BP+10]=COLOR
	PUSH	SI
	PUSH	DI
	MOV	DI,WORD PTR [BP+4]		;[DI]<- X
   ;
	XOR	SI,SI
	JMP	SHORT PUTSTRWHILE
PUTSTRIFASC_HZ:
   ;
	MOV	BX,WORD PTR [BP+8]
	ADD	BX,SI
	CMP	BYTE PTR [BX],128
	JAE	SHORT PUTSTRHZ
   ;	
	PUSH	WORD PTR [BP+10]			;PUTASCII
	MOV	BX,WORD PTR [BP+8]
	ADD	BX,SI
	MOV	AL,BYTE PTR [BX]
	PUSH	AX
	PUSH	WORD PTR [BP+6]
	PUSH	DI
	CALL	NEAR PTR PUTASCII
	ADD	SP,8
   ;	
	INC	SI
   ;	
	ADD	DI,8
   ;	
	JMP	SHORT PUTSTRWHILE
PUTSTRHZ:
   ;	
	PUSH	WORD PTR [BP+10]				;PUTHZ
	MOV	BX,WORD PTR [BP+8]
	ADD	BX,SI
	PUSH	WORD PTR [BX]
	PUSH	WORD PTR [BP+6]
	PUSH	DI
	CALL	NEAR PTR PUTHZ
	ADD	SP,8
  ;	
	INC	SI
   ;	
	INC	SI
   ;	
	ADD	DI,16
PUTSTRWHILE:
	MOV	BX,WORD PTR [BP+8]
	ADD	BX,SI
	CMP	BYTE PTR [BX],0
	JNE	SHORT PUTSTRIFASC_HZ
   ;	
	POP	DI
	POP	SI
	POP	BP
	RET	
PUTSTR	ENDP
;****************************************************; 
;*�ӳ�������	STRLEN																** 
;*���ܣ�			�����ַ�������												** 
;*��ڲ�����	��ջ�дӶ����ף�S(�ַ�����ַ)					** 
;*																		color					** 
;*���ڲ�����	ax=length															** 
;*����˵������Sѹջ																	** 
;*					ע����ú�ƽ���ջ											**
;****************************************************;
STRLEN	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	CX,WORD PTR [BP+4]			;[BP+4]=S (ADDRESS OF STRING)
   ;	
	XOR	DX,DX
	JMP	SHORT STRLENWHILECOMP
STRLENWHILEADD:
   ;	
	INC	DX
STRLENWHILECOMP:
	MOV	BX,CX
	INC	CX
	CMP	BYTE PTR [BX],0
	JNE	SHORT STRLENWHILEADD	
  ;
	MOV	AX,DX									;DX=LENGTH OF STRING (BYTES)
	;													;return ax=length
	POP	BP
	RET	
STRLEN	ENDP

PUTBUTTON	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=PTEXT(POINT STRING),[BP+10]=COLOR
	SUB	SP,6
	PUSH	SI
	PUSH	DI
	MOV	SI,WORD PTR [BP+4]
	MOV	DI,WORD PTR [BP+6]
   ;
	PUSH	WORD PTR [BP+8]
	CALL	NEAR PTR STRLEN
	POP	CX
	MOV	WORD PTR [BP-2],AX					;LENGTH OF STING->[BP-2]
  ;	
	MOV	AX,WORD PTR [BP-2]
	ADD	AX,4
	MOV	CL,3
	SHL	AX,CL
	;XOR	AH,AH								;��Ҫ�޸�,ע��˶�
	;MOV	DL,BYTE PTR CHARWIDTH
	;MUL	DL
	MOV	DX,SI
	ADD	DX,AX
	DEC	DX
	MOV	WORD PTR [BP-4],DX					;[BP-4]=RIGHT
   ;
	MOV	AX,DI
	ADD	AX,DEFAULTBUTTONHEIGHT
	DEC	AX
	MOV	WORD PTR [BP-6],AX					;[BP-6]=BOTTOM
  ;	
	MOV	AX,WHITE
	PUSH	AX
	PUSH	DI
	PUSH	WORD PTR [BP-4]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;	
	MOV	AX,WHITE
	PUSH	AX
	PUSH	SI
	PUSH	WORD PTR [BP-6]
	PUSH	DI
	CALL	NEAR PTR VLINE
	ADD	SP,8
  ;	
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,WORD PTR [BP-6]
	DEC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP-4]
	DEC	AX
	PUSH	AX
	MOV	AX,SI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;
	MOV	AX,DARKBLACK
	PUSH	AX
	PUSH	WORD PTR [BP-6]
	PUSH	WORD PTR [BP-4]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;	
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,WORD PTR [BP-4]
	DEC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP-6]
	DEC	AX
	PUSH	AX
	MOV	AX,DI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR VLINE
	ADD	SP,8
 ;
	MOV	AX,DARKBLACK
	PUSH	AX
	PUSH	WORD PTR [BP-4]
	PUSH	WORD PTR [BP-6]
	PUSH	DI
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;		PUTSTR(LEFT+CHARWIDTH*2,TOP+CHARHEIGHT/2,PTEXT,COLOR);
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+8]
	MOV	AX,CHARHEIGHT
	SHR	AX,1
	ADD	AX,[BP+6]
	PUSH	AX
	MOV	AX,CHARWIDTH
	SHL	AX,1
	ADD	AX,[BP+4]
	PUSH	AX
	CALL	NEAR PTR PUTSTR
	ADD	SP,8
   ;
	POP	DI
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
PUTBUTTON	ENDP
;****************************************************;
;*�ӳ�������	FILLRECT															**
;*���ܣ�			������															**
;*��ڲ�����	��ջ�дӶ����ף�left(word),top,right	**
;*														bottom	color					**
;*���ڲ�����																				**
;*����˵�����Ƚ�colorѹջ,���BOTTOM,RIGHT,TOP,LEFT	**
;*					��֤left<right,top<bottom		ע����ú�ƽ**
;*																						���ջ**
;****************************************************;
FILLRECT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP		;Ϊȡ������׼��[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=RIGHT,[BP+10]=BOTTOM,[BP+12]=COLOR
	PUSH	SI
  ;
	XOR	SI,SI
   ;
	MOV	SI,WORD PTR [BP+6]
	JMP	SHORT FILLRECTCONCOUNT
FILRECTFILL:
    ;	
	PUSH	WORD PTR [BP+12]			;HLINE(LEFT,RIGHT,Y,COLOR);
	PUSH	SI
	PUSH	WORD PTR [BP+8]
	PUSH	WORD PTR [BP+4]
	CALL	NEAR PTR HLINE
	ADD	SP,8
	INC	SI
FILLRECTCONCOUNT:
	CMP	SI,WORD PTR [BP+10]
	JLE	SHORT FILRECTFILL
   ;
	POP	SI
	POP	BP
	RET	
FILLRECT	ENDP
;****************************************************; 
;*�ӳ�������	PUTWINDOW															** 
;*���ܣ�			��ʾWindow����												** 
;*��ڲ�����	��ջ�дӶ����ף�left(word),top,right	** 
;*						bottom,wndcolor,captiontext,capcolor	** 
;*���ڲ�����																				** 
;*����˵�����Ƚ�colorѹջ,���...										** 
;*					ע����ú�ƽ���ջ											** 
;****************************************************;
PUTWINDOW	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+16]=CAPTIONCOLOR,[BP+14]=PCAPTIONTEXT,[BP+12]=WNDCOLOR,
	PUSH	SI					;[BP+10]=BOTTOM,	[BP+8]=RIGHT,	[BP+6]=TOP,	[BP+4]=LEFT
	PUSH	DI
	MOV	DI,WORD PTR [BP+4]
	MOV	SI,WORD PTR [BP+6]
    ;
	PUSH	WORD PTR [BP+12]				;��䴰�屳��
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+8]
	PUSH	SI
	PUSH	DI
	CALL	NEAR PTR FILLRECT
	ADD	SP,10
    ;
 	MOV	AX,WHITE									;������Ӱ:begin
	PUSH	AX
	PUSH	SI
	PUSH	WORD PTR [BP+8]
	PUSH	DI
	CALL	NEAR PTR HLINE
	ADD	SP,8
   ;
	MOV	AX,WHITE
	PUSH	AX
	PUSH	DI
	PUSH	WORD PTR [BP+10]
	PUSH	SI
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,WORD PTR [BP+10]
	DEC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP+8]
	DEC	AX
	PUSH	AX
	MOV	AX,DI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;	
	MOV	AX,DARKBLACK
	PUSH	AX
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+8]
	PUSH	DI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,WORD PTR [BP+8]
	DEC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP+10]
	DEC	AX
	PUSH	AX
	MOV	AX,SI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;
	MOV	AX,DARKBLACK							
	PUSH	AX
	PUSH	WORD PTR [BP+8]
	PUSH	WORD PTR [BP+10]
	PUSH	SI
	CALL	NEAR PTR VLINE
	ADD	SP,8										;������Ӱ:begin
   ;
	MOV	AX,BLUE										;��������ɫ���
	PUSH	AX
	MOV	AX,SI
	ADD	AX,DEFAULTCAPTIONHEIGHT
	INC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP+8]
	ADD	AX,-4
	PUSH	AX
	MOV	AX,SI
	ADD	AX,3
	PUSH	AX
	MOV	AX,DI
	ADD	AX,4
	PUSH	AX
	CALL	NEAR PTR FILLRECT
	ADD	SP,10
  ;
	PUSH	WORD PTR [BP+16]					;������ ��
	PUSH	WORD PTR [BP+14]
	MOV	AX,SI
	ADD	AX,7					;##amend  2005-7-12 22:27
	PUSH	AX
	MOV	AX,DI
	ADD	AX,10+logo_width+charheight/4
	PUSH	AX
	CALL	NEAR PTR PUTSTR
	ADD	SP,8
  ;
  ;showlog
  mov	ax,si
  add	ax,4
  push	ax
  mov	ax,di
	add	ax,8
	push	ax
	call	showlogo
	add	sp,4
	;  
	POP	DI
	POP	SI
	POP	BP
	RET	
PUTWINDOW	ENDP
;****************************************************; 
;*�ӳ�������	PUTEDITBOX														** 
;*���ܣ�			��ʾ�༭����													** 
;*��ڲ�����	��ջ�дӶ����ף�left(word),top,				** 
;*													nrchar(�ַ�����)				** 
;*���ڲ�����	�ı���xcur,ycur(��ǰxy����)						** 
;*����˵�����Ƚ�nrcharѹջ,���top,left							** 
;*					ע����ú�ƽ���ջ											** 
;****************************************************;
PUTEDITBOX	PROC	NEAR
	PUSH	BP
	MOV	BP,SP									;;[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=NRCHAR
	PUSH SI
	PUSH DI
	MOV	SI,WORD PTR [BP+4]		;SI=LEFT
	MOV	DI,WORD PTR [BP+6]		;DI=TOP
   ;	
	MOV	AX,WORD PTR [BP+8]		;���㴰�ڵ��ұ�λ��
	MOV	CL,3									;����[sp-2]�� right
	SHL	AX,CL									;ע�⣬�����ֵĿ��Ϊ8����������λ���㣬�˳��򲻿���ֲ
	MOV	DX,SI
	ADD	DX,AX
	DEC	DX
	MOV	WORD PTR [BP-2],DX
   ;	
  MOV	AX,DI
	ADD	AX,DEFAULTEDITBOXHEIGHT
	DEC	AX
	MOV	WORD PTR [BP-4],AX
   ;	
	MOV	AX,WHITE							;���õ�ɫ ����ɫ��
	PUSH	AX
	PUSH	WORD PTR [BP-4]
	PUSH	WORD PTR [BP-2]
	PUSH	DI
	PUSH	SI
	CALL	NEAR PTR FILLRECT
	ADD	SP,10
  ;	
	MOV	AX,DARKBLACK          ;��ʼ�����
	PUSH	AX
	PUSH	DI
	PUSH	WORD PTR [BP-2]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
   ;	
	MOV	AX,DARKBLACK
	PUSH	AX
	PUSH	SI
	PUSH	WORD PTR [BP-4]
	PUSH	DI
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,DI
	INC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP-2]
	DEC	AX
	PUSH	AX
	MOV	AX,SI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR HLINE
	ADD	SP,8
   ;	
	MOV	AX,LIGHTBLACK
	PUSH	AX
	MOV	AX,SI
	INC	AX
	PUSH	AX
	MOV	AX,WORD PTR [BP-4]
	DEC	AX
	PUSH	AX
	MOV	AX,DI
	INC	AX
	PUSH	AX
	CALL	NEAR PTR VLINE
	ADD	SP,8
   ;
	MOV	AX,WHITE
	PUSH	AX
	PUSH	WORD PTR [BP-4]
	PUSH	WORD PTR [BP-2]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;
	MOV	AX,WHITE
	PUSH	AX
	PUSH	WORD PTR [BP-2]
	PUSH	WORD PTR [BP-4]
	PUSH	DI
	CALL	NEAR PTR VLINE
	ADD	SP,8
  ;
	MOV	AX,SI							;�ı䵱ǰ��������
	ADD	AX,4
	MOV	XCUR,AX
  MOV	AX,DI
	ADD	AX,6
	MOV	YCUR,AX
  ;
	POP	DI
	POP	SI
	MOV	SP,BP
	POP	BP
	RET	
PUTEDITBOX	ENDP
;****************************************************; 
;*�ӳ�������	PUTCURSOR															** 
;*���ܣ�			��ʾ���															** 
;*��ڲ�����	��ջ�дӶ����ף�x,y,color							** 
;*���ڲ�����	�ı���xcur,ycur(��ǰxy����)						** 
;*����˵�����Ƚ�colorѹջ,���y,x										** 
;*					ע����ú�ƽ���ջ											** 
;***************************************************
PUTCURSOR	PROC	NEAR
	PUSH	BP
	MOV	BP,SP								;;[BP+4]=X,[BP+6]=Y,[BP+8]=COLOR
	PUSH	SI
	PUSH	DI
	MOV	SI,WORD PTR [BP+6]			;si=y
   ;	
	MOV	AX,WORD PTR [BP+4]
	MOV	XCUR,AX
   ;
	MOV	YCUR,SI
   ;	
	XOR	DI,DI
	JMP	SHORT PUTCUR_ADD
PUTCUR_CTRL:
   ;	
	PUSH	WORD PTR [BP+8]
	PUSH	WORD PTR [BP+4]
	MOV	AX,SI
	ADD	AX,CHARHEIGHT				;���ĸ߶����ַ��߶�
	DEC	AX									
	PUSH	AX
	PUSH	SI
	CALL	NEAR PTR VLINE
	ADD	SP,8
	INC	DI
PUTCUR_ADD:
	CMP	DI,2							;���Ŀ����2
	JL	SHORT PUTCUR_CTRL
   ;
	POP	DI
	POP	SI
	POP	BP
	RET	
PUTCURSOR	ENDP

;------------------�����ӳ��� ��ʼ------------------------------------
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
;;
;****************************************************;
;*�ӳ�������	SHOWLOGO															**
;*���ܣ�			��ʾ���� LOGO													**
;*��ڲ�����	��ջ�дӶ����ף�x(word),y							**
;*���ڲ�����	��																		**
;*����˵����	y,xѹջ																**
;*					ע����ú�ƽ���ջ		��ǰ�ı���ɫ05bh	**
;*					�ñ�ı���ɫ�޸ĺ�������Ӧ�ĵط�				**
;****************************************************;
SHOWLOGO	PROC	NEAR
	PUSH	BP
	MOV	BP,SP    	 ;Ϊȡ������׼�� [BP+4]=X,[BP+6]=Y
	;
	XOR	CX,CX
	XOR	DX,DX
	MOV	BX,WORD PTR [BP+6];
	MOV	SI,OFFSET logopic
	MOV	CX,LOGO_HEIGHT
SHOWLOGO_LOOPWAI:
	PUSH	CX
	MOV	CX,LOGO_WIDTH	
	MOV	AX,WORD PTR [BP+4];
	DEC AX
SHOWLOGO_LOOPNEI:
	INC	AX
	MOV	DL,BYTE PTR [SI]
	INC	SI
	cmp	dl,011h				;ע�⿴��Щ��ɫ�ǲ�����ʾ�ģ��Ա���logo��������
	jz	showlogo_notdis
	PUSHA
	PUSH	DX
	PUSH	BX
	PUSH	AX
	CALL	PUTPIXEL
	ADD	SP,6
	POPA
showlogo_notdis:
	LOOP	SHOWLOGO_LOOPNEI
	POP	CX
	INC	BX
	LOOP	SHOWLOGO_LOOPWAI
	;
	MOV	SP,BP
	POP	BP
	RET
SHOWLOGO	ENDP
;;;;;;;

;;;
include mousefun.asm
