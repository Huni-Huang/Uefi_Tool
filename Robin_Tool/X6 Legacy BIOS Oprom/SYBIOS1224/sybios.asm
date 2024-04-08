;程序说明：为写入bios编写的神郁软件，在dos下执行，调试用
;
;2005-7-4      luobing
;各个子程序基本调试完毕，读出来的数据与预想的基本相符
;对于写硬盘那块，还要做一些调整，外网（offset之后）的分区表没有写到硬盘上;(已写)
;从今天开始，程序不准做大的修改，修改的部分都会标上日期
;看程序可用的一些字符 ##NOTTEST(未测试)  ##DEBUG(为调试所加)  ##ERROR(严重错误)
;											##AMEND(date:y-m-d hh:mm)修改及日期
;罗冰  2005-7-4  20:08
;考虑以下情况，用户在外网发现自己的分区表不对，软件会提示，要求安装，再提示不在安装状态
;							用户插上开关，此时到了内网状态，很不巧，内网的分区是符合的，软件不会再提示了，怎么办?
;					解决办法：在检查分区的模块中，先判断是否在安装状态，在的话就检查外网分区(之所以这样做，是因为offset的关系)
;问题：开始的时候必须填充单片机中的可读区域，否则安装的时候不能读写整个硬盘，
;2005-7-5   10:53  luobing
;
;
;
;这是第一个通过测试的程序，可以写在QDI(915)的bios中运行
;2005-7-6  19:07  luobing
;今天发现两个程序的漏洞：1 在SENDMYCMD函数中，如果硬盘没有访问成功，(=50H?),会一直重启硬盘控制器。
;													 但对于某些硬盘来说，遇到不认识的命令，会在1f7的bit0中返回1,所以其实会一直重启，进入死循环
;												 2 对SPFDISK，分区(LBA)的大小必须是3FH的倍数，否则会出错，必须修改CALOFFSET
;修改后的CALOFFSET还未测试
;2005-7-8		20:36		luobing	
;为了显示logo，增加了函数showlogo,修改了putwindow,以及相关显示logo用的颜色和信息(logopic)
;配合软件distill，提取所需的logo信息
;2005-7-13 luobing 15:50
;2005-8-5  基本定版
;2005-8-29 在使用过程中，如果对内(外)网的未划分区域进行分区，在开始的时候将进行比较，此时将提示安装
;						为了解决这种情况，在进入之前，将分区表的后两个填充为0，（程序结构更乱了，没办法）
;2005-9-1		为了提醒用户在安装完以后将开关去掉，加了一个错误检测
;2005-9-15  修改了版本号
;2005-11-17	为了判断是否第一次加电起机，直接显示界面，并且清除内外网标志位(8dH)
;2005-12-24	20:18 为了增加两个功能：1 判断是否有USB设备影响安全，要求用户取下
;															2 增加鼠标功能，使用户能在无OS的情况下使用鼠标
;           			另外修改显示模式为103H，800x600x256
;									注意：鼠标驱动非标准的，经过了我的修改，在鼠标左键松开的时候，也会传送x y 的坐标
;2005-12-26 10:56	调试成功鼠标功能
;常量定义:主要是界面显示所用到常量
;#############################################  常量定义开始  #####################################################
include SY_EQU.inc
;#############################################  常量定义结束  #####################################################
;
.Model Tiny,C
.486
.code
org 0h
start:
        DW 0AA55h                           	;扩展BIOS标志
        DB 40h                              	;本程序大于32K值为80H,否则为40H
       ;保存各寄存器
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
        call main          						;入口标志
        cli
        mov	ax,9000h
        mov	ds,ax
        xor	si,si
        ;mov	cs,[si]					;cs 不回送 错误1：不能用寄存器传送cs
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
        retf 																	;远程返回
;#############################################  数据段开始  #####################################################
;数据段：显示部分所用数据，硬盘控制所用数据(结构)，提示信息所用字符串
include SY_DATA.ASM
;#############################################  数据段结束  #####################################################
;
;#############################################  代码段开始  #####################################################
;+++++++++++++++++++++++++++++++	主程序开始	++++++++++++++++++++++++++++++++++++++++++
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
	;##AMEND 2005-12-24 20:58 为鼠标所加
	CALL	UNINSTALLMOUSE			;卸载鼠标驱动
	;
	POPFD
	POPA
	RET
MAIN	ENDP
	
;+++++++++++++++++++++++++++++++	主程序结束	++++++++++++++++++++++++++++++++++++++++++
;
;;+++++++++++++++++++++++++++++++	子程序开始	++++++++++++++++++++++++++++++++++++++++++
;子程序有六个部分：1 显示部分子程序  2 硬盘控制子程序  3 控制卡的子程序 
;									 4 读取CMOS信息(为了以后扩展，后加的子程序也放在这个部分)
;									 5 为主程序直接调用的界面子程序
;									 6 主干模块
;------------------主干模块 开始------------------------------------
MAIN_MOUDLE	PROC	NEAR
	;显示的初始化工作
	CALL	INITVIDEO				;显示模式及颜色寄存器准备完毕
	;;;;;;;;;;;;;;;
	;读硬盘的基本属性
	CALL	RESETDISK
	CALL	GETDISKATTR
	;JC	HD_ERROR_NOPRIHD	;错误，没有找到硬盘或者不在主盘 error#1 
	JC	MERROR1
	CMP	AL,1
	;JZ	HD_ERROR_NOEXTSUPPORT	;错误，不支持扩展，版本太低 error#2
	JZ	MERROR2
	MOV	AX,EDD_BASEPORT
	CMP	AX,01F0H
	;JNZ	HD_ERROR_NOPRIHD	;错误，没有找到硬盘或者不在主盘 error#1
	JNZ	MERROR1
	MOV	SI,OFFSET EDPARA_TOTAL
	MOV	AX,[SI+2]
	MOV	CL,5
	SHR	AX,CL	;计算硬盘大小(G)
	CMP	AX,032H ;与50G比
	;JB	HD_ERROR_LESS60G	;错误，不支持小于60G的硬盘 error#3
	JB	MERROR3
	;MOV	SI,OFFSET EDPARA_HEADS
	;MOV	AL,BYTE PTR [SI]
	;CMP	AL,0FH
	;JZ	HD_ERROR_15HEADS	;错误，不支持磁头为15的硬盘 error#4
	;JZ	MERROR4
	;;;;;;;;;;;;;;;
	;确定卡的存在
	CALL	IDENTIFYCARD
	CMP	BL,0
	;JZ	CARD_ERROR_NOCARD	;错误，卡不存在或非神郁卡 error#5
	JZ	MERROR5
	;;;
	;;;
	;卡和硬盘都存在，开始填充各项数据
	;##AMEND	2005-11-17 LUOBING
	;         为判断是否第一次起机所加。此时必须出现界面，并且将CMOS 8DH的表示内外网标志位清除(bit0、bit1)
	CALL	TESTPOWERSTATE
	CMP	BL,10H
	JNZ	NOT_FIRST_POWER
	;mov	dx,offset str_lb_1
	;call	disp_error
	;xor	ax,ax
	;int	16h
	;
	CALL	CLEARCMOS8DH		;第一次加电必须清除内外网标志
	CALL	CLEARPOWERSTATE	;清除上电复位的状态位(即第一次加电的标志位，在单片机内)
	;lbdebug	2005-11-18	9:43
	mov	cx,11h
loop_test_pw_state:
	dec	cx
	call	testpowerstate
	cmp	cx,0
	jz	merror9			;如果无法清除的话，报错
	cmp	bl,01fh
	jnz	loop_test_pw_state
	;此处注意程序结构
NOT_FIRST_POWER:
	;;;
	;;
	CALL	CALOFFSET	;计算OFFSET 和MYOFFSET的值，填充数据结构(卡和硬盘的)
	CALL	READUSERMBR ;读取用户的MBR信息，保存到USER_MBR
	;##AMEND 2005-8-29 为了将第3和第四个分区清除所加
	CALL	CLEARPART23	;
	;
	CALL	READUSERMBR ;读取用户的MBR信息，保存到USER_MBR
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;舞台搭建完毕，演员开始登场;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	CALL	GETCARDSTATUS		;判断卡是否装过(即判断单片机有没有安装的标志)
	AND	BX,0FF00H
	;JZ	NOT_SETUP					;未安装，跳转到未安装处理模块 moudel#1
	JZ	MMOUDEL1
	CALL	COMPAREMBR			;比较用户的分区表和规定的是否相同
	CMP	AX,0
	;JNZ	MBR_NOTMATCH			;分区表不相同 moudel#2
	JNZ	MMOUDEL2
	;##AMEND 2005-7-5 7:51 在安装状态下，比较外网的分区表
	call	getcardstatus		;确定是否在安装状态
	test	bx,0010h				;不在安装状态 不用比较
	jnz	comparenext
	;
	mov	eax,edpara_total		;放开读写
	sub	eax,1
	mov	nei_max,eax
	;xor	eax,eax
	;mov	wai_offset,eax
	call	writeEEPROM
	;
	;
	call readuserwaimbr		;读外网的分区表
	;读完，写回单片机，读写重新抓回
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
	call	comparembr			;比较外网的分区表，不同则跳转
	cmp	ax,0
	jnz	mmoudel2
	;
comparenext:
	CALL	COMPAREOFFSETVAL	;比较EEPROM中的offsetwai的值和计算的是否相同
	CMP	BX,0
	;JZ	OFFSETVAL_NOTMATCH	;OFFSET值不同跳转处理moudel#3
	JZ	MMOUDEL3
	;;主角一号，显示判断开始
	;##AMEND 判断是否在安装状态，是的话提示将开关取下
	call	getcardstatus		;确定是否在安装状态
	test	bx,0010h				;不在安装状态 不用比较
	jz	MERROR8						;为0在安装状态，提示错误
	;判断CMOS8DH处bit0 bit1是否全为0[即第一次开机，（此时只能通过上层软件是否对8dh置位来判断
	;   并不是绝对准确的。）或者当前没有一个系统处在挂起]
	;并且CMOS8EH的bit0 为0（这位由本软件所用，用来表示以下情况发生：向卡发送了切换的消息，且软件使PC关机而后自动唤醒）
	;    在这种情况下，界面不需要再出来.
	CALL	READCMOS8D8EH
	CMP	AL,00H
	;JNZ	MAIN_MOUDEL_DNEIWAI		;需要显示，跳转
	JNZ	DISNW
	CALL	CLEARCMOS8E		;清cmos8eh bit0
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
	;显示
;MAIN_MOUDEL_DNEIWAI:
DISNW:
	;##AMEND 2005-12-24 20:56 为鼠标所加
	;提供鼠标所需要的变量:DS
	PUSH	ES
	PUSH	AX
	PUSH	CS
	POP	ES
	MOV	BX,OFFSET INVOKER_DATA_SEG
	MOV	AX,8000H
	MOV	ES:[BX],AX
	POP	AX
	POP	ES
	CALL	SETUPMOUSE			;安装鼠标驱动
	;
	CALL	DISP_MAIN_SEL			;内外网选择界面
MAIN_MOUDEL_GETUSERIN:
;##AMEND 2005-12-24 luobing 
;为显示鼠标，修改了循环条件
	;
	mov	al,gl_mouse_lbutton_down
	cmp	al,1
	jz	mouse_lbutton_press_down
	jmp mouse_lbutton_no_actdown
mouse_lbutton_press_down:			;鼠标左键按下了，做相应的动作
	call	mouse_lbutton_down_fun
	mov	gl_mouse_lbutton_down,0
mouse_lbutton_no_actdown:	
	mov	al,gl_mouse_lbutton_up
	cmp	al,1	
	jz	mouse_lbutton_press_up
	jmp mouse_lbutton_no_act
mouse_lbutton_press_up:				;鼠标左键松开了，做相应的动作
	call	mouse_lbutton_up_fun
	mov	gl_mouse_lbutton_up,0
;
mouse_lbutton_no_act:					;鼠标左键没有动作
	;判断鼠标选择的是哪个按钮：内 2   外 1
	mov	al,gl_mouse_target			;判断是否在松开后还在按钮的区域内
	cmp	al,1
	jnz	mouse_not_hit_target
	mov	al,gl_mouse_select			;鼠标选择的是外网
	cmp	al,1
	jz	MAIN_MOUDEL_WAISEL
	cmp	al,2
	jz	MAIN_MOUDEL_NEISEL			;鼠标选择的是内网
mouse_not_hit_target:	
	mov	ax,0100h
	int	16h
	jz	main_moudel_getuserin
	;
	CALL	GETSELECT
	CMP	AL,'1'						;选择外网
	JZ	MAIN_MOUDEL_WAISEL
	CMP	AL,'2'						;选择内网
	JZ	MAIN_MOUDEL_NEISEL
	CMP	AX,2F00H
	JZ	DISP_VERSION				;显示版本 ##version
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
	;JZ	MAIN_MOUDEL_EXIT	;所选网与当前的网状态相同，交出控制权
	JZ	MAINEXIT
	;;;;主角二号，所选网与当前的网状态不同，处理如下
	CALL	SETCMOS8E					;设置cmos8e bit0，下次启动的时候不显示界面
	CALL	TELLCARDSWITCH	;下次掉电后切换
	CALL	RESETDISK				;在关机前重启控制器，使之复位
	CALL	QDI_SHUTDOWN	;关机然后自动唤醒，只对QDI的915主板有用
	JMP	$											;等待关机 自动唤醒
;主角三号：moudel#1~3的处理
;NOT_SETUP:				;moudel#1 第一次安装
DISP_VERSION:
	MOV	DX,OFFSET STR_VERSION
	CALL	DISP_ERROR
	CALL	GETSELECT
	JMP	DISNW
MMOUDEL1:
	MOV	DX,OFFSET STR_SETUP
	CALL	DISP_SETUP
	JMP	SETUP_SYBIOS
;MBR_NOTMATCH:			;moudel#2 分区表不符合，安装
MMOUDEL2:
	MOV	DX,OFFSET STR_SETUP1
	CALL	DISP_SETUP
	JMP	SETUP_SYBIOS
;OFFSETVAL_NOTMATCH:		;moudel#3 卡中OFFSET不对，安装
MMOUDEL3:
	MOV	DX,OFFSET STR_UNMATCHOFF
	CALL	DISP_SETUP
SETUP_SYBIOS:				;准备安装
	CALL	GETCARDSTATUS		;确定是否在安装状态
	TEST	BX,0010H
	;JNZ	CARD_ERROR_NOT_SETUP			;错误，不在安装状态 error#6
	JNZ	MERROR6
	;安装：填分区表，写EEPROM
	;放开读写，写单片机的第一区域
	;##AMEND	2005-7-5 8:34
	mov	eax,edpara_total		;放开读写
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
	;JNZ	EEPROM_ERROR_WR		;错误，写失败 error#7
	JNZ	MERROR7
	;JMP	MAIN_MOUDEL_DNEIWAI
	JMP	DISNW
	;;;;;;;;;;;;;看看配角的表演;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;错误的显示及处理 error#1~7
;HD_ERROR_NOPRIHD:							
MERROR1:											;error#1,硬盘不是主盘或不存在
	MOV	DX,OFFSET STR_NOPRIHD
	CALL	DISP_ERROR	
	CALL	GETSELECT							;按任意键退出，交出控制权利
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
;HD_ERROR_NOEXTSUPPORT:				;error#2,不支持扩展读写
MERROR2:
	MOV	DX,OFFSET STR_NOSUPEXTHD
	CALL	DISP_ERROR	
	JMP	$											;不交出控制权利
;HD_ERROR_LESS60G:							;error#3,不支持60G以下硬盘
MERROR3:
	MOV	DX,OFFSET STR_NOSUPPORTHD
	CALL	DISP_ERROR	
	JMP	$					;不交出控制权利
;HD_ERROR_15HEADS:							;error#4,不支持磁头为15的硬盘 
;MERROR4:
	;MOV	DX,OFFSET STR_NOSUP15
	;CALL	DISP_ERROR	
	;JMP	$					;不交出控制权利
;CARD_ERROR_NOCARD:						;error#5,卡不存在或非神郁卡 	
MERROR5:
	MOV	DX,OFFSET STR_NOCARD
	CALL	DISP_ERROR
	CALL	GETSELECT							;按任意键退出，交出控制权利
	;JMP	MAIN_MOUDEL_EXIT
	JMP	MAINEXIT
;CARD_ERROR_NOT_SETUP:					;error#6，不在安装状态 
MERROR6:
	MOV	DX,OFFSET STR_NOT_SETUOST
	CALL	DISP_ERROR	
	JMP	$					;不交出控制权利
;EEPROM_ERROR_WR:					;error#7，写失败 
MERROR7:	
	MOV	DX,OFFSET STR_WEPP_ERROR
	CALL	DISP_ERROR	
	JMP	$					;不交出控制权利
MERROR8:
	MOV	DX,OFFSET STR_NOTICE_SETUP	;error#8，安装完成后，应该将开关取下
	CALL	DISP_ERROR	
	JMP	$					;不交出控制权利
;lbdebug	2005-11-18	9:53
merror9:													;error#9,清除第一次起机状态位不成功
	MOV	DX,OFFSET STR_CLEAR_PW_ST
	CALL	DISP_ERROR
	JMP	$
;;;;;
;;;;谢幕;;;;;
;MAIN_MOUDEL_EXIT:
MAINEXIT:
	;返回到3号显示模式
	MOV	AX,0003H
	INT	10H
	RET
MAIN_MOUDLE ENDP
;------------------主干模块 结束------------------------------------

;------------------界面子程序 开始------------------------------------
include SY_MENU.asm
;------------------界面子程序 结束------------------------------------
;------------------读取CMOS信息 开始------------------------------------
include SY_CMOS.asm
;------------------读取CMOS信息 结束------------------------------------
;------------------控制卡的子程序 开始------------------------------------
include SY_CARD.asm
;------------------控制卡的子程序 结束-----------------------------------------------------------
;------------------硬盘控制子程序 开始------------------------------------------------------------
include SY_HD.asm
;------------------硬盘控制子程序 结束------------------------------------------------------------
;------------------显示部分子程序 开始------------------------------------------------------------
include SY_DISPLAY.asm
;------------------显示部分子程序 结束------------------------------------------------------------
;------------------鼠标驱动子程序 开始------------------------------------------------------------
include mousedrv.asm
;------------------鼠标驱动子程序 结束------------------------------------------------------------
;
;;+++++++++++++++++++++++++++++++	子程序结束	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
END START