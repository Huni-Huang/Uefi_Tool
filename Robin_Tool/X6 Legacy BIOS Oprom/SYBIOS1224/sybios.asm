;����˵����Ϊд��bios��д�������������dos��ִ�У�������
;
;2005-7-4      luobing
;�����ӳ������������ϣ���������������Ԥ��Ļ������
;����дӲ���ǿ飬��Ҫ��һЩ������������offset֮�󣩵ķ�����û��д��Ӳ����;(��д)
;�ӽ��쿪ʼ������׼������޸ģ��޸ĵĲ��ֶ����������
;��������õ�һЩ�ַ� ##NOTTEST(δ����)  ##DEBUG(Ϊ��������)  ##ERROR(���ش���)
;											##AMEND(date:y-m-d hh:mm)�޸ļ�����
;�ޱ�  2005-7-4  20:08
;��������������û������������Լ��ķ������ԣ��������ʾ��Ҫ��װ������ʾ���ڰ�װ״̬
;							�û����Ͽ��أ���ʱ��������״̬���ܲ��ɣ������ķ����Ƿ��ϵģ������������ʾ�ˣ���ô��?
;					����취���ڼ�������ģ���У����ж��Ƿ��ڰ�װ״̬���ڵĻ��ͼ����������(֮����������������Ϊoffset�Ĺ�ϵ)
;���⣺��ʼ��ʱ�������䵥Ƭ���еĿɶ����򣬷���װ��ʱ���ܶ�д����Ӳ�̣�
;2005-7-5   10:53  luobing
;
;
;
;���ǵ�һ��ͨ�����Եĳ��򣬿���д��QDI(915)��bios������
;2005-7-6  19:07  luobing
;���췢�����������©����1 ��SENDMYCMD�����У����Ӳ��û�з��ʳɹ���(=50H?),��һֱ����Ӳ�̿�������
;													 ������ĳЩӲ����˵����������ʶ���������1f7��bit0�з���1,������ʵ��һֱ������������ѭ��
;												 2 ��SPFDISK������(LBA)�Ĵ�С������3FH�ı������������������޸�CALOFFSET
;�޸ĺ��CALOFFSET��δ����
;2005-7-8		20:36		luobing	
;Ϊ����ʾlogo�������˺���showlogo,�޸���putwindow,�Լ������ʾlogo�õ���ɫ����Ϣ(logopic)
;������distill����ȡ�����logo��Ϣ
;2005-7-13 luobing 15:50
;2005-8-5  ��������
;2005-8-29 ��ʹ�ù����У��������(��)����δ����������з������ڿ�ʼ��ʱ�򽫽��бȽϣ���ʱ����ʾ��װ
;						Ϊ�˽������������ڽ���֮ǰ����������ĺ��������Ϊ0��������ṹ�����ˣ�û�취��
;2005-9-1		Ϊ�������û��ڰ�װ���Ժ󽫿���ȥ��������һ��������
;2005-9-15  �޸��˰汾��
;2005-11-17	Ϊ���ж��Ƿ��һ�μӵ������ֱ����ʾ���棬���������������־λ(8dH)
;2005-12-24	20:18 Ϊ�������������ܣ�1 �ж��Ƿ���USB�豸Ӱ�찲ȫ��Ҫ���û�ȡ��
;															2 ������깦�ܣ�ʹ�û�������OS�������ʹ�����
;           			�����޸���ʾģʽΪ103H��800x600x256
;									ע�⣺��������Ǳ�׼�ģ��������ҵ��޸ģ����������ɿ���ʱ��Ҳ�ᴫ��x y ������
;2005-12-26 10:56	���Գɹ���깦��
;��������:��Ҫ�ǽ�����ʾ���õ�����
;#############################################  �������忪ʼ  #####################################################
include SY_EQU.inc
;#############################################  �����������  #####################################################
;
.Model Tiny,C
.486
.code
org 0h
start:
        DW 0AA55h                           	;��չBIOS��־
        DB 40h                              	;���������32KֵΪ80H,����Ϊ40H
       ;������Ĵ���
        cli
        mov	bx,ds
        mov	ax,9000h
        mov	ds,ax
        xor	si,si
        mov	ax,cs
        mov	[si],ax					;store cs	[9000:0]
        add	si,2
        mov	[si],bx					;store ds	[9000:2]
        add	si,2
        mov	ax,es						;store es	[9000:4]
        mov	[si],ax					
        add	si,2
        mov	ax,ss						;store ss	[9000:6]
        mov	[si],ax
        add	si,2
        mov	ax,sp						;store sp	[9000:8]
        mov	[si],ax	
        mov	ax,8000h				;ss:sp   8000:mystack
        mov	ss,ax
        mov	ax,offset mystack
        mov	sp,ax
        mov	ds,bx
        sti
        call main          						;��ڱ�־
        cli
        mov	ax,9000h
        mov	ds,ax
        xor	si,si
        ;mov	cs,[si]					;cs ������ ����1�������üĴ�������cs
        add	si,4
        mov	es,[si]
        add	si,2
        mov	ss,[si]
        add	si,2
        mov	sp,[si]
        mov	si,2
        mov	ax,[si]
        mov	ds,ax
        sti
        retf 																	;Զ�̷���
;#############################################  ���ݶο�ʼ  #####################################################
;���ݶΣ���ʾ�����������ݣ�Ӳ�̿�����������(�ṹ)����ʾ��Ϣ�����ַ���
include SY_DATA.ASM
;#############################################  ���ݶν���  #####################################################
;
;#############################################  ����ο�ʼ  #####################################################
;+++++++++++++++++++++++++++++++	������ʼ	++++++++++++++++++++++++++++++++++++++++++
MAIN	PROC	NEAR
	PUSHA					;.386 CODE
	PUSHFD
	MOV	AX,CS
	MOV	DS,AX
	
	;
	MOV	AX,8000H
	MOV	ES,AX
	MOV	SI,OFFSET DATABEGIN
	MOV	DI,OFFSET DATABEGIN
	MOV	CX,OFFSET ENDDATA
	MOV	AX,OFFSET DATABEGIN
	SUB	CX,AX
	REP	MOVSB
	MOV	AX,8000H
	MOV	DS,AX
	;
	;
	CALL	MAIN_MOUDLE
	;
	;##AMEND 2005-12-24 20:58 Ϊ�������
	CALL	UNINSTALLMOUSE			;ж���������
	;
	POPFD
	POPA
	RET
MAIN	ENDP
	
;+++++++++++++++++++++++++++++++	���������	++++++++++++++++++++++++++++++++++++++++++
;
;;+++++++++++++++++++++++++++++++	�ӳ���ʼ	++++++++++++++++++++++++++++++++++++++++++
;�ӳ������������֣�1 ��ʾ�����ӳ���  2 Ӳ�̿����ӳ���  3 ���ƿ����ӳ��� 
;									 4 ��ȡCMOS��Ϣ(Ϊ���Ժ���չ����ӵ��ӳ���Ҳ�����������)
;									 5 Ϊ������ֱ�ӵ��õĽ����ӳ���
;									 6 ����ģ��
;------------------����ģ�� ��ʼ------------------------------------
MAIN_MOUDLE	PROC	NEAR
	;��ʾ�ĳ�ʼ������
	CALL	INITVIDEO				;��ʾģʽ����ɫ�Ĵ���׼�����
	;;;;;;;;;;;;;;;
	;��Ӳ�̵Ļ�������
	CALL	RESETDISK
	CALL	GETDISKATTR
	;JC	HD_ERROR_NOPRIHD	;����û���ҵ�Ӳ�̻��߲������� error#1 
	JC	MERROR1
	CMP	AL,1
	;JZ	HD_ERROR_NOEXTSUPPORT	;���󣬲�֧����չ���汾̫�� error#2
	JZ	MERROR2
	MOV	AX,EDD_BASEPORT
	CMP	AX,01F0H
	;JNZ	HD_ERROR_NOPRIHD	;����û���ҵ�Ӳ�̻��߲������� error#1
	JNZ	MERROR1
	MOV	SI,OFFSET EDPARA_TOTAL
	MOV	AX,[SI+2]
	MOV	CL,5
	SHR	AX,CL	;����Ӳ�̴�С(G)
	CMP	AX,032H ;��50G��
	;JB	HD_ERROR_LESS60G	;���󣬲�֧��С��60G��Ӳ�� error#3
	JB	MERROR3
	;MOV	SI,OFFSET EDPARA_HEADS
	;MOV	AL,BYTE PTR [SI]
	;CMP	AL,0FH
	;JZ	HD_ERROR_15HEADS	;���󣬲�֧�ִ�ͷΪ15��Ӳ�� error#4
	;JZ	MERROR4
	;;;;;;;;;;;;;;;
	;ȷ�����Ĵ���
	CALL	IDENTIFYCARD
	CMP	BL,0
	;JZ	CARD_ERROR_NOCARD	;���󣬿������ڻ�������� error#5
	JZ	MERROR5
	;;;
	;;;
	;����Ӳ�̶����ڣ���ʼ����������
	;##AMEND	2005-11-17 LUOBING
	;         Ϊ�ж��Ƿ��һ��������ӡ���ʱ������ֽ��棬���ҽ�CMOS 8DH�ı�ʾ��������־λ���(bit0��bit1)
	CALL	TESTPOWERSTATE
	CMP	BL,10H
	JNZ	NOT_FIRST_POWER
	;mov	dx,offset str_lb_1
	;call	disp_error
	;xor	ax,ax
	;int	16h
	;
	CALL	CLEARCMOS8DH		;��һ�μӵ���������������־
	CALL	CLEARPOWERSTATE	;����ϵ縴λ��״̬λ(����һ�μӵ�ı�־λ���ڵ�Ƭ����)
	;lbdebug	2005-11-18	9:43
	mov	cx,11h
loop_test_pw_state:
	dec	cx
	call	testpowerstate
	cmp	cx,0
	jz	merror9			;����޷�����Ļ�������
	cmp	bl,01fh
	jnz	loop_test_pw_state
	;�˴�ע�����ṹ
NOT_FIRST_POWER:
	;;;
	;;
	CALL	CALOFFSET	;����OFFSET ��MYOFFSET��ֵ��������ݽṹ(����Ӳ�̵�)
	CALL	READUSERMBR ;��ȡ�û���MBR��Ϣ�����浽USER_MBR
	;##AMEND 2005-8-29 Ϊ�˽���3�͵��ĸ������������
	CALL	CLEARPART23	;
	;
	CALL	READUSERMBR ;��ȡ�û���MBR��Ϣ�����浽USER_MBR
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;��̨���ϣ���Ա��ʼ�ǳ�;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	CALL	GETCARDSTATUS		;�жϿ��Ƿ�װ��(���жϵ�Ƭ����û�а�װ�ı�־)
	AND	BX,0FF00H
	;JZ	NOT_SETUP					;δ��װ����ת��δ��װ����ģ�� moudel#1
	JZ	MMOUDEL1
	CALL	COMPAREMBR			;�Ƚ��û��ķ�����͹涨���Ƿ���ͬ
	CMP	AX,0
	;JNZ	MBR_NOTMATCH			;��������ͬ moudel#2
	JNZ	MMOUDEL2
	;##AMEND 2005-7-5 7:51 �ڰ�װ״̬�£��Ƚ������ķ�����
	call	getcardstatus		;ȷ���Ƿ��ڰ�װ״̬
	test	bx,0010h				;���ڰ�װ״̬ ���ñȽ�
	jnz	comparenext
	;
	mov	eax,edpara_total		;�ſ���д
	sub	eax,1
	mov	nei_max,eax
	;xor	eax,eax
	;mov	wai_offset,eax
	call	writeEEPROM
	;
	;
	call readuserwaimbr		;�������ķ�����
	;���꣬д�ص�Ƭ������д����ץ��
	mov	si,offset offsetval
	mov	ax,word ptr [si]
	mov	bx,word ptr [si+2]
	sub	bx,2
	mov	si,offset nei_max
	mov	[si],ax
	mov	[si+2],bx
	;mov	eax,offsetval
	;mov	wai_offset,eax
	call	writeEEPROM
	;
	call	comparembr			;�Ƚ������ķ�������ͬ����ת
	cmp	ax,0
	jnz	mmoudel2
	;
comparenext:
	CALL	COMPAREOFFSETVAL	;�Ƚ�EEPROM�е�offsetwai��ֵ�ͼ�����Ƿ���ͬ
	CMP	BX,0
	;JZ	OFFSETVAL_NOTMATCH	;OFFSETֵ��ͬ��ת����moudel#3
	JZ	MMOUDEL3
	;;����һ�ţ���ʾ�жϿ�ʼ
	;##AMEND �ж��Ƿ��ڰ�װ״̬���ǵĻ���ʾ������ȡ��
	call	getcardstatus		;ȷ���Ƿ��ڰ�װ״̬
	test	bx,0010h				;���ڰ�װ״̬ ���ñȽ�
	jz	MERROR8						;Ϊ0�ڰ�װ״̬����ʾ����
	;�ж�CMOS8DH��bit0 bit1�Ƿ�ȫΪ0[����һ�ο���������ʱֻ��ͨ���ϲ�����Ƿ��8dh��λ���ж�
	;   �����Ǿ���׼ȷ�ġ������ߵ�ǰû��һ��ϵͳ���ڹ���]
	;����CMOS8EH��bit0 Ϊ0����λ�ɱ�������ã�������ʾ��������������򿨷������л�����Ϣ�������ʹPC�ػ������Զ����ѣ�
	;    ����������£����治��Ҫ�ٳ���.
	CALL	READCMOS8D8EH
	CMP	AL,00H
	;JNZ	MAIN_MOUDEL_DNEIWAI		;��Ҫ��ʾ����ת
	JNZ	DISNW
	CALL	CLEARCMOS8E		;��cmos8eh bit0
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
	;��ʾ
;MAIN_MOUDEL_DNEIWAI:
DISNW:
	;##AMEND 2005-12-24 20:56 Ϊ�������
	;�ṩ�������Ҫ�ı���:DS
	PUSH	ES
	PUSH	AX
	PUSH	CS
	POP	ES
	MOV	BX,OFFSET INVOKER_DATA_SEG
	MOV	AX,8000H
	MOV	ES:[BX],AX
	POP	AX
	POP	ES
	CALL	SETUPMOUSE			;��װ�������
	;
	CALL	DISP_MAIN_SEL			;������ѡ�����
MAIN_MOUDEL_GETUSERIN:
;##AMEND 2005-12-24 luobing 
;Ϊ��ʾ��꣬�޸���ѭ������
	;
	mov	al,gl_mouse_lbutton_down
	cmp	al,1
	jz	mouse_lbutton_press_down
	jmp mouse_lbutton_no_actdown
mouse_lbutton_press_down:			;�����������ˣ�����Ӧ�Ķ���
	call	mouse_lbutton_down_fun
	mov	gl_mouse_lbutton_down,0
mouse_lbutton_no_actdown:	
	mov	al,gl_mouse_lbutton_up
	cmp	al,1	
	jz	mouse_lbutton_press_up
	jmp mouse_lbutton_no_act
mouse_lbutton_press_up:				;�������ɿ��ˣ�����Ӧ�Ķ���
	call	mouse_lbutton_up_fun
	mov	gl_mouse_lbutton_up,0
;
mouse_lbutton_no_act:					;������û�ж���
	;�ж����ѡ������ĸ���ť���� 2   �� 1
	mov	al,gl_mouse_target			;�ж��Ƿ����ɿ����ڰ�ť��������
	cmp	al,1
	jnz	mouse_not_hit_target
	mov	al,gl_mouse_select			;���ѡ���������
	cmp	al,1
	jz	MAIN_MOUDEL_WAISEL
	cmp	al,2
	jz	MAIN_MOUDEL_NEISEL			;���ѡ���������
mouse_not_hit_target:	
	mov	ax,0100h
	int	16h
	jz	main_moudel_getuserin
	;
	CALL	GETSELECT
	CMP	AL,'1'						;ѡ������
	JZ	MAIN_MOUDEL_WAISEL
	CMP	AL,'2'						;ѡ������
	JZ	MAIN_MOUDEL_NEISEL
	CMP	AX,2F00H
	JZ	DISP_VERSION				;��ʾ�汾 ##version
	JMP	MAIN_MOUDEL_GETUSERIN
MAIN_MOUDEL_WAISEL:
	MOV	AL,080H
	JMP	MAIN_SELECT_DEAL
MAIN_MOUDEL_NEISEL:
	MOV	AL,00H
MAIN_SELECT_DEAL:
	PUSH	AX
	CALL	JUDGEOSSTATUS
	POP	AX
	AND	BL,080H
	XOR	AL,BL
	;JZ	MAIN_MOUDEL_EXIT	;��ѡ���뵱ǰ����״̬��ͬ����������Ȩ
	JZ	MAINEXIT
	;;;;���Ƕ��ţ���ѡ���뵱ǰ����״̬��ͬ����������
	CALL	SETCMOS8E					;����cmos8e bit0���´�������ʱ����ʾ����
	CALL	TELLCARDSWITCH	;�´ε�����л�
	CALL	RESETDISK				;�ڹػ�ǰ������������ʹ֮��λ
	CALL	QDI_SHUTDOWN	;�ػ�Ȼ���Զ����ѣ�ֻ��QDI��915��������
	JMP	$											;�ȴ��ػ� �Զ�����
;�������ţ�moudel#1~3�Ĵ���
;NOT_SETUP:				;moudel#1 ��һ�ΰ�װ
DISP_VERSION:
	MOV	DX,OFFSET STR_VERSION
	CALL	DISP_ERROR
	CALL	GETSELECT
	JMP	DISNW
MMOUDEL1:
	MOV	DX,OFFSET STR_SETUP
	CALL	DISP_SETUP
	JMP	SETUP_SYBIOS
;MBR_NOTMATCH:			;moudel#2 ���������ϣ���װ
MMOUDEL2:
	MOV	DX,OFFSET STR_SETUP1
	CALL	DISP_SETUP
	JMP	SETUP_SYBIOS
;OFFSETVAL_NOTMATCH:		;moudel#3 ����OFFSET���ԣ���װ
MMOUDEL3:
	MOV	DX,OFFSET STR_UNMATCHOFF
	CALL	DISP_SETUP
SETUP_SYBIOS:				;׼����װ
	CALL	GETCARDSTATUS		;ȷ���Ƿ��ڰ�װ״̬
	TEST	BX,0010H
	;JNZ	CARD_ERROR_NOT_SETUP			;���󣬲��ڰ�װ״̬ error#6
	JNZ	MERROR6
	;��װ���������дEEPROM
	;�ſ���д��д��Ƭ���ĵ�һ����
	;##AMEND	2005-7-5 8:34
	mov	eax,edpara_total		;�ſ���д
	sub	eax,1
	mov	nei_max,eax
	;xor	eax,eax
	;mov	wai_offset,eax
	call	writeEEPROM
	;
	CALL	READUSERMBR
	CALL	FILLMBR
	CALL	READUSERWAIMBR
	CALL	FILLWAIMBR
	;
	mov	si,offset offsetval
	mov	ax,word ptr [si]
	mov	bx,word ptr [si+2]
	sub	bx,2
	mov	si,offset nei_max
	mov	[si],ax
	mov	[si+2],bx
	;mov	eax,offsetval
	;mov	wai_offset,eax
	;
	CALL	WRITEEEPROM
	CMP	BX,1
	;JNZ	EEPROM_ERROR_WR		;����дʧ�� error#7
	JNZ	MERROR7
	;JMP	MAIN_MOUDEL_DNEIWAI
	JMP	DISNW
	;;;;;;;;;;;;;������ǵı���;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;�������ʾ������ error#1~7
;HD_ERROR_NOPRIHD:							
MERROR1:											;error#1,Ӳ�̲������̻򲻴���
	MOV	DX,OFFSET STR_NOPRIHD
	CALL	DISP_ERROR	
	CALL	GETSELECT							;��������˳�����������Ȩ��
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
;HD_ERROR_NOEXTSUPPORT:				;error#2,��֧����չ��д
MERROR2:
	MOV	DX,OFFSET STR_NOSUPEXTHD
	CALL	DISP_ERROR	
	JMP	$											;����������Ȩ��
;HD_ERROR_LESS60G:							;error#3,��֧��60G����Ӳ��
MERROR3:
	MOV	DX,OFFSET STR_NOSUPPORTHD
	CALL	DISP_ERROR	
	JMP	$					;����������Ȩ��
;HD_ERROR_15HEADS:							;error#4,��֧�ִ�ͷΪ15��Ӳ�� 
;MERROR4:
	;MOV	DX,OFFSET STR_NOSUP15
	;CALL	DISP_ERROR	
	;JMP	$					;����������Ȩ��
;CARD_ERROR_NOCARD:						;error#5,�������ڻ�������� 	
MERROR5:
	MOV	DX,OFFSET STR_NOCARD
	CALL	DISP_ERROR
	CALL	GETSELECT							;��������˳�����������Ȩ��
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
;CARD_ERROR_NOT_SETUP:					;error#6�����ڰ�װ״̬ 
MERROR6:
	MOV	DX,OFFSET STR_NOT_SETUOST
	CALL	DISP_ERROR	
	JMP	$					;����������Ȩ��
;EEPROM_ERROR_WR:					;error#7��дʧ�� 
MERROR7:	
	MOV	DX,OFFSET STR_WEPP_ERROR
	CALL	DISP_ERROR	
	JMP	$					;����������Ȩ��
MERROR8:
	MOV	DX,OFFSET STR_NOTICE_SETUP	;error#8����װ��ɺ�Ӧ�ý�����ȡ��
	CALL	DISP_ERROR	
	JMP	$					;����������Ȩ��
;lbdebug	2005-11-18	9:53
merror9:													;error#9,�����һ�����״̬λ���ɹ�
	MOV	DX,OFFSET STR_CLEAR_PW_ST
	CALL	DISP_ERROR
	JMP	$
;;;;;
;;;;лĻ;;;;;
;MAIN_MOUDEL_EXIT:
MAINEXIT:
	;���ص�3����ʾģʽ
	MOV	AX,0003H
	INT	10H
	RET
MAIN_MOUDLE ENDP
;------------------����ģ�� ����------------------------------------

;------------------�����ӳ��� ��ʼ------------------------------------
include SY_MENU.asm
;------------------�����ӳ��� ����------------------------------------
;------------------��ȡCMOS��Ϣ ��ʼ------------------------------------
include SY_CMOS.asm
;------------------��ȡCMOS��Ϣ ����------------------------------------
;------------------���ƿ����ӳ��� ��ʼ------------------------------------
include SY_CARD.asm
;------------------���ƿ����ӳ��� ����-----------------------------------------------------------
;------------------Ӳ�̿����ӳ��� ��ʼ------------------------------------------------------------
include SY_HD.asm
;------------------Ӳ�̿����ӳ��� ����------------------------------------------------------------
;------------------��ʾ�����ӳ��� ��ʼ------------------------------------------------------------
include SY_DISPLAY.asm
;------------------��ʾ�����ӳ��� ����------------------------------------------------------------
;------------------��������ӳ��� ��ʼ------------------------------------------------------------
include mousedrv.asm
;------------------��������ӳ��� ����------------------------------------------------------------
;
;;+++++++++++++++++++++++++++++++	�ӳ������	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
END START