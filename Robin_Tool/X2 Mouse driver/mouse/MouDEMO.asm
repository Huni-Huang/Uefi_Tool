;������������(BIOS)�����ʾ������
;luobing 2005 - 7 -2  1:52
;��������

include MAC.INC
;���ݶ�
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
	
	
	;���ô��ڼ�������
	;CALL	DISP_FRAME
	;
	;����������ѡ��˵�
	
	;MOV	DX,OFFSET STR_NOT_SETUOST
	;CALL	DISP_SETUP
	;CALL	DISP_ERROR
	;CALL	DISP_SETUP
	;��ʾ�༭��
	;MOV	AX,10
	;PUSH	AX
	;PUSH	AX
	;CALL	SHOWLOGO
	;add	sp,4
	;mouse
	CALL	TESTMOUSE
	JC	MOUSE_VALID				;��֧�����
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
	jz	get_user		;��ѯ����״̬��������û�������ȡ
	xor ax,ax
	int 16h 				;���������
	;�õ����̵����룬����
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