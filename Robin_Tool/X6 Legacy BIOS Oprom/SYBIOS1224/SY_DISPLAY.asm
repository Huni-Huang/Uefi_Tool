;****************************************************;
;*子程序名：	INITVIDEO															**
;*功能：			初始化显示（模式、寄存器、背景）			**
;*入口参数：	无																		**
;*出口参数：	无																		**
;****************************************************;
INITVIDEO	PROC	NEAR
	MOV	BX,SYVIDEOMODE
	CALL	SETMODEL
	;
	MOV	AX,OFFSET	PALETTEDATA
	PUSH	AX
	CALL	INITPALETTE
	POP	CX			;平衡堆栈
	;
	MOV	BX,DEEPBLUE
	CALL	SETBACKCOLOR
	RET
INITVIDEO	ENDP
;****************************************************;
;*子程序名：	SETMODEL															**
;*功能：			设置显示模式													**
;*入口参数：	BX=mode																**
;*出口参数：	无																		**
;****************************************************;
SETMODEL	PROC	NEAR
	PUSH	AX
	MOV	AX,04F02H
	INT	10H
	POP	AX
	RET
SETMODEL	ENDP
;****************************************************;
;*子程序名：	INITPALETTE														**
;*功能：			颜色寄存器定义												**
;*入口参数：	堆栈中为数据的起始地址								**
;*出口参数：	无																		**
;****************************************************;
INITPALETTE	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+4]=DATA POINT
	PUSH	SI
	MOV	DX,03C8H
	MOV	AL,0
	OUT	DX,AL
  DB	0EBH					;延时
  DB	00H
  DB	0EBH
  DB	00H						;延时
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
;*子程序名：	SETBACKCOLOR													**
;*功能：			设置背景															**
;*入口参数：	bx=color															**
;*出口参数：																				**
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
;*子程序名：	VLINE																	**
;*功能：			画竖线																**
;*入口参数：	堆栈中从顶到底：y1(word),y2,x,				**
;*														color(word)	(X1<X2)		**
;*出口参数：																				**
;*调用说明：先将color压栈,其次x,y2,y1,必须保证y1<y2	**
;*					注意调用后平衡堆栈											**
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
;*子程序名：	HLINE																	**
;*功能：			画横线																**
;*入口参数：	堆栈中从顶到底：x1(word),x2,y,				**
;*														color(word)	(X1<X2)		**
;*出口参数：																				**
;*调用说明：先将color压栈,其次y,x2,x1,必须保证x1<x2	**
;*					注意调用后平衡堆栈											**
;****************************************************;
HLINE	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;为取参数做准备[BP+4]=x1,[BP+6]=x2,[BP+8]=y,[BP+10]=color...
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
;*子程序名：	DRAWRECT															**
;*功能：			画矩形																**
;*入口参数：	堆栈中从顶到底：left(word),top,right	**
;*														bottom	color					**
;*出口参数：																				**
;*调用说明：先将color压栈,其次BOTTOM,RIGHT,TOP,LEFT	**
;*					保证left<right,top<bottom								**
;*					注意调用后平衡堆栈											**
;****************************************************;
DRAWRECT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;为取参数做准备[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=RIGHT,[BP+10]=BOTTOM,[BP+12]=COLOR
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
;*子程序名：	PUTPIXEL															**
;*功能：			写象素点															**
;*入口参数：	堆栈中从顶到底：x(word),y,color				**
;*出口参数：	无																		**
;*调用说明：先将color压栈,其次y,x										**
;*					注意调用后平衡堆栈											**
;****************************************************;
PUTPIXEL	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;为取参数做准备 [BP+4]=X,[BP+6]=Y,[BP+8]=COLOR
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
;*子程序名：	PUTASCII															**
;*功能：			显示英文															**
;*入口参数：	堆栈中从顶到底：X(word),Y,CODE				**
;*																		color					**
;*出口参数：																				**
;*调用说明：先将color压栈,其次CODE,Y,X							**
;*	ax,bx,cx,dx被影响		注意调用后平衡堆栈					**
;****************************************************;
PUTASCII	PROC	NEAR
	PUSH	BP
	MOV	BP,SP				;为取参数做准备[BP+4]=X,[BP+6]=Y,[BP+8]=CODE,[BP+10]=COLOR
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
;*子程序名：	PUTHZ																	** 
;*功能：			显示汉字															** 
;*入口参数：	堆栈中从顶到底：X(word),Y,CODE				** 
;*																		color					** 
;*出口参数：																				** 
;*调用说明：先将color压栈,其次CODE,Y,X							** 
;*	ax,bx,cx,dx被影响	注意调用后平衡堆栈						** 
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
	;XCHG	AL,AH;;;传到堆栈中的汉字高低相反，（调试发现），直接传送汉字内码时必须调整才能比较
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
	JNE	SHORT PUTHZDISPLAY		;找到则显示
	JMP	PUTHZFCODEFAIL				;没找到，退出
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
;*子程序名：	PUTSTR																** 
;*功能：			显示字符串														** 
;*入口参数：	堆栈中从顶到底：X(word),Y,S(字符串地址** 
;*																		color					** 
;*出口参数：																				** 
;*调用说明：先将color压栈,其次S,Y,X									** 
;*					注意调用后平衡堆栈											** 
;****************************************************;	
PUTSTR	PROC	NEAR
	PUSH	BP
	MOV	BP,SP			;为取参数做准备[BP+4]=X,[BP+6]=Y,[BP+8]=OFFSET S,[BP+10]=COLOR
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
;*子程序名：	STRLEN																** 
;*功能：			测量字符串长度												** 
;*入口参数：	堆栈中从顶到底：S(字符串地址)					** 
;*																		color					** 
;*出口参数：	ax=length															** 
;*调用说明：将S压栈																	** 
;*					注意调用后平衡堆栈											**
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
;****************************************************; 
;*子程序名：	PUTBUTTON															** 
;*功能：			显示按钮															** 
;*入口参数：	堆栈中从顶到底：color S(字符串地址)		** 
;*								top,left													** 
;*出口参数：																				** 
;*调用说明：将S压栈																	** 
;*					注意调用后平衡堆栈											**
;*****************************************************
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
	;XOR	AH,AH								;若要修改,注意此段
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
;*子程序名：	PRESS_PUTBUTTON												** 
;*功能：			显示陷下去的按钮											** 
;*入口参数：	堆栈中从顶到底：color S(字符串地址)		** 
;*								top,left													** 
;*出口参数：																				** 
;*调用说明：将S压栈																	** 
;*					注意调用后平衡堆栈											**
;*****************************************************
PRESS_PUTBUTTON	PROC	NEAR
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
	;XOR	AH,AH								;若要修改,注意此段
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
	MOV	AX,DARKBLACK				;顶端的颜色
	PUSH	AX
	PUSH	DI
	PUSH	WORD PTR [BP-4]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;	
	MOV	AX,DARKBLACK			;左边的颜色
	PUSH	AX
	PUSH	SI
	PUSH	WORD PTR [BP-6]
	PUSH	DI
	CALL	NEAR PTR VLINE
	ADD	SP,8
  ;	
	MOV	AX,WHITE	;底端的颜色
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
	MOV	AX,LIGHTBLACK
	PUSH	AX
	PUSH	WORD PTR [BP-6]
	PUSH	WORD PTR [BP-4]
	PUSH	SI
	CALL	NEAR PTR HLINE
	ADD	SP,8
  ;	
	MOV	AX,WHITE
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
	MOV	AX,LIGHTBLACK
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
PRESS_PUTBUTTON	ENDP
;****************************************************;
;*子程序名：	FILLRECT															**
;*功能：			填充矩形															**
;*入口参数：	堆栈中从顶到底：left(word),top,right	**
;*														bottom	color					**
;*出口参数：																				**
;*调用说明：先将color压栈,其次BOTTOM,RIGHT,TOP,LEFT	**
;*					保证left<right,top<bottom		注意调用后平**
;*																						衡堆栈**
;****************************************************;
FILLRECT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP		;为取参数做准备[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=RIGHT,[BP+10]=BOTTOM,[BP+12]=COLOR
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
;*子程序名：	PUTWINDOW															** 
;*功能：			显示Window窗口												** 
;*入口参数：	堆栈中从顶到底：left(word),top,right	** 
;*						bottom,wndcolor,captiontext,capcolor	** 
;*出口参数：																				** 
;*调用说明：先将color压栈,其次...										** 
;*					注意调用后平衡堆栈											** 
;****************************************************;
PUTWINDOW	PROC	NEAR
	PUSH	BP
	MOV	BP,SP					;[BP+16]=CAPTIONCOLOR,[BP+14]=PCAPTIONTEXT,[BP+12]=WNDCOLOR,
	PUSH	SI					;[BP+10]=BOTTOM,	[BP+8]=RIGHT,	[BP+6]=TOP,	[BP+4]=LEFT
	PUSH	DI
	MOV	DI,WORD PTR [BP+4]
	MOV	SI,WORD PTR [BP+6]
    ;
	PUSH	WORD PTR [BP+12]				;填充窗体背景
	PUSH	WORD PTR [BP+10]
	PUSH	WORD PTR [BP+8]
	PUSH	SI
	PUSH	DI
	CALL	NEAR PTR FILLRECT
	ADD	SP,10
    ;
 	MOV	AX,WHITE									;制造阴影:begin
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
	ADD	SP,8										;制造阴影:begin
   ;
	MOV	AX,BLUE										;标题栏颜色填充
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
	PUSH	WORD PTR [BP+16]					;标题栏 字
	PUSH	WORD PTR [BP+14]
	MOV	AX,SI
	ADD	AX,7					;##amend  2005-7-12 22:27,amend for show logo
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
;*子程序名：	PUTEDITBOX														** 
;*功能：			显示编辑窗口													** 
;*入口参数：	堆栈中从顶到底：left(word),top,				** 
;*													nrchar(字符个数)				** 
;*出口参数：	改变了xcur,ycur(当前xy坐标)						** 
;*调用说明：先将nrchar压栈,其次top,left							** 
;*					注意调用后平衡堆栈											** 
;****************************************************;
PUTEDITBOX	PROC	NEAR
	PUSH	BP
	MOV	BP,SP									;;[BP+4]=LEFT,[BP+6]=TOP,[BP+8]=NRCHAR
	PUSH SI
	PUSH DI
	MOV	SI,WORD PTR [BP+4]		;SI=LEFT
	MOV	DI,WORD PTR [BP+6]		;DI=TOP
   ;	
	MOV	AX,WORD PTR [BP+8]		;计算窗口的右边位置
	MOV	CL,3									;放在[sp-2]中 right
	SHL	AX,CL									;注意，由于字的宽度为8，可以以移位计算，此程序不可移植
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
	MOV	AX,WHITE							;设置底色 （白色）
	PUSH	AX
	PUSH	WORD PTR [BP-4]
	PUSH	WORD PTR [BP-2]
	PUSH	DI
	PUSH	SI
	CALL	NEAR PTR FILLRECT
	ADD	SP,10
  ;	
	MOV	AX,DARKBLACK          ;开始画框架
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
	MOV	AX,SI							;改变当前光标的坐标
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
;*子程序名：	SHOWLOGO															**
;*功能：			显示神郁 LOGO													**
;*入口参数：	堆栈中从顶到底：x(word),y							**
;*出口参数：	无																		**
;*调用说明：	y,x压栈																**
;*					注意调用后平衡堆栈		当前的背景色05bh	**
;*					用别的背景色修改函数中相应的地方				**
;****************************************************;
SHOWLOGO	PROC	NEAR
	PUSH	BP
	MOV	BP,SP    	 ;为取参数做准备 [BP+4]=X,[BP+6]=Y
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
	cmp	dl,011h				;注意看那些颜色是不用显示的，以保持logo的完整性
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