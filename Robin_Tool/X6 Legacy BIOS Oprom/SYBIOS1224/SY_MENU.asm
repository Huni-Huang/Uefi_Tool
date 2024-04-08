;****************************************************;
;*子程序名：	DISP_ERROR														**
;*功能：			显示无效硬盘													**
;*入口参数：	DX=错误信息的地址											**
;*出口参数：																				**
;*使用说明：  																			**
;****************************************************;
DISP_ERROR	PROC	NEAR
	PUSH	DX
	PUSH	BP
	MOV	BP,SP
	CALL	DISP_FRAME
	MOV	AX,RED						;显示字的颜色
	PUSH	AX
	;取字符串的地址
	MOV	AX,WORD PTR [BP+2]	;显示字
	PUSH	AX
	MOV	BX,FRAME_BOTTOM
	MOV	AX,FRAME_TOP
	SUB	BX,AX
	SHR	BX,1
	ADD	AX,BX
	SUB	AX,8								;字符高度是16，取一半
	PUSH	AX								;计算字符串的坐标 Y 值
	;取字符串地址
	MOV	AX,WORD PTR [BP+2]					;计算字符串的坐标 X 值
	PUSH	AX
	CALL	NEAR PTR STRLEN
	ADD	SP,2
	MOV	CL,3
	SHL	AX,CL								;字符的宽度为8，可以这样做
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
;*子程序名：	DISP_MAIN_SEL													**
;*功能：			显示内外网选择界面										**
;*入口参数：	无																		**
;*出口参数：																				**
;*使用说明：  																			**
;****************************************************;
DISP_MAIN_SEL	PROC	NEAR
	CALL	DISP_FRAME
	;设置内外网选择菜单
	;外网
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
	;内网
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
;*子程序名：	DISP_FRAME														**
;*功能：			显示主框架														**
;*入口参数：	无																		**
;*出口参数：																				**
;*使用说明：  框架的位置是自由调整的，看PUTWINDOW的	**
;*						函数说明															**
;****************************************************;
DISP_FRAME	PROC	NEAR
	;设置窗口及标题栏
	MOV	AX,WHITE
	PUSH	AX
	MOV	DX,OFFSET	STR_CAP			;设置标题
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
;*子程序名：	DISP_SETUP														**
;*功能：			显示安装界面													**
;*入口参数：	DX=显示的第一行字符串的地址						**
;*出口参数：																				**
;*使用说明：  																			**
;****************************************************;
DISP_SETUP	PROC	NEAR
	PUSH	DX
	PUSH	BP
	MOV	BP,SP 						;[BP+2]=显示第一行的字符串地址
	CALL	DISP_FRAME
	MOV	AX,YELLOW						;显示字的颜色
	PUSH	AX
	;取字符串的地址
	MOV	AX,WORD PTR [BP+2]	;显示字
	PUSH	AX
	MOV	BX,FRAME_BOTTOM
	MOV	AX,FRAME_TOP
	SUB	BX,AX
	SHR	BX,1
	ADD	AX,BX
	SUB	AX,24								;字符高度是16，取一个半
	PUSH	AX								;计算字符串的坐标 Y 值
	;取字符串地址
	MOV	AX,WORD PTR [BP+2]					;计算字符串的坐标 X 值
	PUSH	AX
	CALL	NEAR PTR STRLEN
	ADD	SP,2
	MOV	CL,3
	SHL	AX,CL								;字符的宽度为8，可以这样做
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
	SHL	AX,CL								;字符的宽度为8，可以这样做
	MOV	DX,XCUR
	ADD	DX,AX
	MOV	XCUR,DX
	MOV	BX,YCUR
	;显示编辑框
	MOV	AX,2
	PUSH	AX
	PUSH	BX
	PUSH	DX
	CALL	PUTEDITBOX
	ADD	SP,6
	;接受用户输入
	CALL	GETUSERIN
	MOV	SP,BP
	POP	BP
	POP	DX
	RET
DISP_SETUP	ENDP
;****************************************************;
;*子程序名：	GETUSERIN															**
;*功能：			获取用户的输入,显示										**
;*入口参数：																				**
;*出口参数：	al='Y' 																**
;****************************************************;	 	
GETUSERIN	PROC	NEAR
	XOR	BX,BX
GUI_LOOP:
	XOR	AX,AX
	INT	16H
	CMP	AL,40H
	JNC	GUI_DISPLAY
	CMP	BL,'Y'				;bl中存放的是上一次的输入
	JNZ	GUI_LOOP
	CMP	AL,0DH
	JZ	GUI_EXIT
GUI_DISPLAY:
	CMP	AL,60H
	JC	GUI_2CAP			;转换为大写
	SUB	AL,20H
GUI_2CAP:
	CMP	AL,'Y'
	JZ	GUI_DISINPUT
	;清除上次的显示
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
	MOV	AL,00H;在下面运行的程序中，什么都不显示
GUI_DISINPUT:
	MOV	BL,AL				;将本次输入放入bl
	;在当前光标处显示输入的值
	PUSH	BX
	;显示输入的值
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
;*子程序名：	GETSELECT															**
;*功能：			获取用户的选择												**
;*入口参数：																				**
;*出口参数：	AL=选择																**
;****************************************************;	
GETSELECT	PROC	NEAR             ;从键盘上取得用户选择
	XOR   AX,AX
	INT  16H
	RET                    
GETSELECT	ENDP
;****************************************************;
;*子程序名：	MOUSE_LBUTTON_DOWN_FUN								**
;*功能：			鼠标左键按下的图像										**
;*入口参数：																				**
;*出口参数：																				**
;程序说明：		画出鼠标按下时按钮陷入的画面					**
;****************************************************;	
MOUSE_LBUTTON_DOWN_FUN	PROC	NEAR
	PUSHA
	PUSH ES
	;比较鼠标的按键区域是否落在了button内
	CLC
	MOV	AX,GL_MOUSE_XCORD
	CMP	AX,BUTTON_LEFT_WAI		;内外网的按钮是左对齐的，所以只要比较一次
	JAE	HOT_POINT_MORE_LEFT
	JMP	EXCEED_HOT_POINT
HOT_POINT_MORE_LEFT:
	CMP	AX,	BUTTON_LEFT_WAI+BUTTON_WIDTH
	JAE	EXCEED_HOT_POINT			;x超出范围
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_WAI
	JB	EXCEED_HOT_POINT			;y超出范围
	CMP	AX,BUTTON_TOP_WAI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT_WAI	;比较是否落在外网的按钮区域内
		;外网
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
	MOV	GL_MOUSE_SELECT,1		;说明鼠标选择的是外网	
	JMP	HOT_POINT_DONE
EXCEED_HOT_POINT_WAI:			;接着比较是否落在内网的按钮区域内
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_NEI
	JB	EXCEED_HOT_POINT			;y没有落在热点内
	CMP	AX,BUTTON_TOP_NEI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT			;y没有落在热点内
		;内网
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
	MOV	GL_MOUSE_SELECT,2			;说明鼠标选择的是内网	
EXCEED_HOT_POINT:
HOT_POINT_DONE:
	POP ES
	POPA
	RET
MOUSE_LBUTTON_DOWN_FUN	ENDP

;****************************************************;
;*子程序名：	MOUSE_LBUTTON_UP_FUN									**
;*功能：			鼠标左键松开的图像										**
;*入口参数：																				**
;*出口参数：																				**
;程序说明：		画出鼠标松开时按钮的式样							**
;****************************************************;	
MOUSE_LBUTTON_UP_FUN	PROC	NEAR
	PUSHA
	PUSH ES
	;比较鼠标的按键区域是否落在了button内
	CLC
	MOV	AX,GL_MOUSE_XCORD
	CMP	AX,BUTTON_LEFT_WAI		;内外网的按钮是左对齐的，所以只要比较一次
	JAE	HOT_POINT_MORE_LEFT1
	JMP	EXCEED_HOT_POINT1
HOT_POINT_MORE_LEFT1:
	CMP	AX,	BUTTON_LEFT_WAI+BUTTON_WIDTH
	JAE	EXCEED_HOT_POINT1			;x超出范围
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_WAI
	JB	EXCEED_HOT_POINT1		;y超出范围
	CMP	AX,BUTTON_TOP_WAI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT_WAI1	;比较是否落在外网的按钮区域内
		MOV	GL_MOUSE_TARGET,1			;落在了按钮区域内
		;
	JMP	HOT_POINT_DONE1
EXCEED_HOT_POINT_WAI1:			;接着比较是否落在内网的按钮区域内
	MOV	AX,GL_MOUSE_YCORD
	CMP	AX,BUTTON_TOP_NEI
	JB	EXCEED_HOT_POINT1			;y没有落在热点内
	CMP	AX,BUTTON_TOP_NEI+DEFAULTBUTTONHEIGHT
	JA	EXCEED_HOT_POINT1			;y没有落在热点内
		MOV	GL_MOUSE_TARGET,1		;落在了按钮区域内
	JMP	HOT_POINT_DONE1
EXCEED_HOT_POINT1:					;超出按钮的范围，处理
	MOV	GL_MOUSE_SELECT,0FFH	;留下鼠标左键松开的接口，供程序用
	MOV	GL_MOUSE_TARGET,0
HOT_POINT_DONE1:
	;不管鼠标停在何处，都重新绘制
		;外网
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
		;内网
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
	;处理完毕
	POP ES
	POPA
	RET
MOUSE_LBUTTON_UP_FUN	ENDP