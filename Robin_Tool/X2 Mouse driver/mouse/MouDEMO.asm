;用来调试神郁(BIOS)界面的示例程序
;luobing 2005 - 7 -2  1:52
;常量定义

include MAC.INC
;数据段
.MODEL SMALL
.486
.STACK 200H
.DATA
include datafile.inc
.CODE
START:
	MOV	AX,@DATA
	MOV	DS,AX
	PUSH	CS
	POP	ES
	MOV	BX,OFFSET INVOKER_DATA_SEG
	MOV	ES:[BX],AX
	CALL	INITVIDEO
	
	
	;设置窗口及标题栏
	;CALL	DISP_FRAME
	;
	;设置内外网选择菜单
	
	;MOV	DX,OFFSET STR_NOT_SETUOST
	;CALL	DISP_SETUP
	;CALL	DISP_ERROR
	;CALL	DISP_SETUP
	;显示编辑框
	;MOV	AX,10
	;PUSH	AX
	;PUSH	AX
	;CALL	SHOWLOGO
	;add	sp,4
	;mouse
	CALL	TESTMOUSE
	JC	MOUSE_VALID				;不支持鼠标
	;CALL	FAR PTR MOUSE_HANDLER
;	INT	3
	CALL 	UNINSTALLMOUSE
	CALL	DISP_MAIN_SEL
	CALL	SETUPMOUSE
get_user:	
	mov	al,GL_MOUSE_LBUTTON_UP
	cmp	al,1
	jz	mouse_lbutton_press_up
	jmp mouse_lbutton_no_actup
mouse_lbutton_press_up:
	call	mouse_lbutton_up_fun
	mov	GL_MOUSE_LBUTTON_UP,0
mouse_lbutton_no_actup:	
	mov	al,GL_MOUSE_LBUTTON_DOWN
	cmp	al,1
	jz	mouse_lbutton_press_down
	jmp mouse_lbutton_no_actdown
mouse_lbutton_press_down:
	;lbdebug
	
	call	mouse_lbutton_down_fun
	mov	GL_MOUSE_LBUTTON_DOWN,0
mouse_lbutton_no_actdown:	
	mov	ax,0100h
	int	16h
	jz	get_user		;查询键盘状态，缓冲区没有则继续取
	xor ax,ax
	int 16h 				;清除缓冲区
	;得到键盘的输入，处理
	cmp	al,031h
	jnz	get_user
	
	CALL 	UNINSTALLMOUSE
	JMP	MAIN_EXIT
MOUSE_VALID:
	MOV	DX,OFFSET STR_MOUSE_VALID
	CALL	DISP_ERROR
	XOR	AX,AX
	INT	16H
MAIN_EXIT:
	MOV	AH,0
	MOV	AL,03H
	INT	10H
	MOV	AX,4C00H
	INT	21H
	
;;;;;;;;;;
include mousedrv.asm
include sonfun.asm

END START