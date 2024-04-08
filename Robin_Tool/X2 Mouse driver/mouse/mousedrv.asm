;驱动功能说明：
;									1 在无操作系统的情况下驱动PS/2鼠标，当然，BIOS必须支持int 15h的c2h功能
;									2 一般情况下，2001年后的主板也能支持USB的鼠标，本驱动可以支持，看你的运气了
;									3 提供了101h 103h 105h三种显示模式的接口，其他模式不支持，需要的话联系我^^
;																									                   --ESC luobing 2005-12-23
;使用本驱动要点：
;                 1 本驱动的数据段和代码段在同一个段内，所以如果需要调用自己的子程序时，必须
;										保存ds es的值，然后规定你自己的数据段（缺省认为你的代码段和本驱动的代码段相同）
;									2 如果只需要知道鼠标左右键是否松开还是按下，下面提供了接口
;										下面的变量定义到你的程序中，本驱动还需要你提供你程序的数据段，INVOKER_DATA_SEG的值必须由你的程序来填写
;										;鼠标所用的全局变量
;										GL_MOUSE_LBUTTON_DOWN				DB		0H			;为1，鼠标左键按下了 0 没有按下,此变量由驱动改变，主程序用完后要清除为初始值0h
;										GL_MOUSE_LBUTTON_UP					DB		0H			;为1，鼠标左键松开了 0 没有松开,此变量由驱动改变，主程序用完后要清除为初始值0h
;										GL_MOUSE_RBUTTON_DOWN				DB		0H			;鼠标右键，解释同上
;										GL_MOUSE_RBUTTON_UP					DB		0H			
;										GL_MOUSE_XCORD							DW		?				;鼠标的坐标
;										GL_MOUSE_YCORD							DW		?
;									注意：上面的变量是放在你的程序中的数据段中，你必须在程序开始的时候填写INVOKER_DATA_SEG
;												由于本程序必须放在你的子程序库中，所以CS的值是相同的，当然，你的程序应该在64K以内，太大的
;												的话我也无能为力了^^或者可以和我联系：分机号:220, 手机：13645168631  -◎◎- ^_^
;											  填写INVOKER_DATA_SEG方法如下:
;															PUSH	CS
;															POP	ES
;															MOV	BX,OFFSET INVOKER_DATA_SEG
;															PUSH	DS
;															POP	AX
;															MOV	ES:[BX],AX
;                 3 其他的可以参照DEMO程序
;luobing	2005-12-23 13:55  明天是平安夜了
;luobing	2005-12-23 15:02  完成初步功能，所有程序在103号模式下调试成功
;luobing	2006-2-8	 20:32	解决了在AMI的BIOS上无法使用的问题，
;------鼠标的驱动-------------------------------------------------------------
;****************************************************;
;*子程序名：	TESTMOUSE															**
;*功能：			测试鼠标驱动（MOUSE_HANDLER_TEST）		**
;*入口参数：																				**
;*出口参数：	cf=1 此驱动程序无法驱动鼠标						**
;*程序日志：	101号模式调试通过 2005-12-20	luobing	**
;****************************************************;
TESTMOUSE		PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;;规定当前的数据段和附加段
	PUSH	CS
	POP	DS
	PUSH	CS
	POP	ES
	;
	MOV	AX,0C205H
	MOV	BX,100H
	INT	15H
	MOV	AX,0C207H 				;安装句柄 INT15H
	MOV	BX,OFFSET MOUSE_HANDLER_TEST
	INT	15H
	MOV	AX,0C200H					;打开PS/2鼠标
	MOV	BH,1
	INT	15H
	;
	POPA
	POP	ES
	POP	DS
	RET
TESTMOUSE	ENDP
;****************************************************;
;*子程序名：	SETUPMOUSE														**
;*功能：			安装鼠标驱动（MOUSE_HANDLER）					**
;*入口参数：																				**
;*出口参数：																				**
;*程序日志：	101号模式调试通过 2005-12-20	luobing	**
;****************************************************;	
SETUPMOUSE	PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;
	;规定当前的数据段和附加段
	PUSH	CS
	POP	DS
	PUSH	CS
	POP	ES
	;
	MOV	AX,0C205H					;initialize mouse
	MOV	BX,100H
	INT	15H
	;
	MOV	AX,0C207H 				;安装句柄 INT15H
	MOV	BX,OFFSET MOUSE_HANDLER
	INT	15H
	MOV	AX,0C200H					;打开PS/2鼠标
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
;*子程序名：	UNINSTALLMOUSE												**
;*功能：			卸载鼠标驱动（MOUSE_HANDLER）					**
;*入口参数：																				**
;*出口参数：																				**
;*程序日志：	101号模式调试通过 2005-12-20	luobing	**
;****************************************************;
UNINSTALLMOUSE	PROC	NEAR
	PUSH	DS
	PUSH	ES
	PUSHA
	;关闭PS/2鼠标
	MOV	AX,0C200H
	MOV	BH,0
	INT	15H
	;清理安装的HANDLER(XXXX:XXXX)
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
;系统调用的功能,
;此函数必须是FAR调用，错误调用的话会产生不可预知的现象
;不要将堆栈弹出，否则会死得很惨^^
MOUSE_HANDLER	PROC	FAR
	PUSHA
	PUSH	ES
	PUSH	DS
	;压栈16+2+2=20
JMP	MOUSE_HANDLER_CODE
;数据区，存放所用的变量和数据
INVOKER_DATA_SEG					DW		?
;SVGA的视频模式信息表
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
CURRENT_MODE				DW		?									;屏幕分辨率状态
SEGINDEXOLD					DW    0FFFFH						;上一个点显存分页段号
SEGINDEX						DW		0									;显存分页段号
SEGOFFSET						DW		?									;显存分页偏移地址
;◎◎鼠标形状的定义，相对坐标
;    X左向为正向，Y下向为正向
MOUSETYPE		DW		0000H,0001H,0002H,0003H,0004H,0005H,0006H
						DW		0007H,0008H,0009H,000AH,000BH,000CH
						DW		0101H,0202H,0303H,0404H,0505H,0606H,0707H,0808H,0909H
						DW		010BH,020AH,0309H,0409H,0509H,0609H,0709H,0809H
						DW		030BH,030CH,040DH,040EH,050FH
						DW		060AH,060BH,070CH,070DH,080EH
						DW		0610H,0710H,0810H
;		鼠标图像的内部点阵
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
;注意此处，
SAVEOLD			DB		MOUSETYPELEN  DUP (0H)			;保存鼠标位置的屏幕图像的数组
SAVENEW			DB		MOUSETYPELEN  DUP (0H)			;用于和SAVEOLD数组交替使用
MOUSECOLOR	DB		43*2 DUP (BLACK)							;定义鼠标的外部颜色
						DB		47*2 DUP (WHITE)								;定义鼠标的内部颜色
;全局变量
MOUSE_STATE				DB		0							;鼠标状态字节
MOUSE_STATE_OLD		DB		?							;上次鼠标状态
MOUSE_COUNT				DB		0							;计数FLAG
MOUSE_XCORD				DW		0							;鼠标左上角的X坐标，初始＝0
MOUSE_YCORD				DW		0							;鼠标左上角的Y坐标，初始＝0
MOUSE_XCORDOLD		DW		0							;前次的鼠标X位置
MOUSE_YCORDOLD		DW		0							;前次的鼠标Y位置
;代码段开始
MOUSE_HANDLER_CODE:
	PUSH	CS			;DS=CS
	POP	DS
	;获取当前显示模式的信息
	MOV	AX,04F01H
	MOV	CX,CURRENT_MODE
	PUSH	CS
	POP	ES
	MOV	DI,OFFSET MOUSE_SVGA_MODE_INFORMATION
	INT	10H
	MOV	AX,04F03H
	INT	10H
	MOV	CURRENT_MODE,BX ;当前的显示模式
	MOV	AX,04F05H
	MOV	BX,0100H
	INT	10H
	MOV	SEGINDEXOLD,DX ;填充画鼠标前的显存的页
	MOV	AX,SS
	MOV	ES,AX
	MOV	BX,SP
	ADD	BX,30				;获得系统传来的值，其位置在SP+10处10+20
	MOV	DX,ES:[BX]			;SAVE IN DX
	;为了区分依次传进来的使用count(全局变量)
	CMP	MOUSE_COUNT,0
	JZ	MOUSE_H_FIRST								;0:第一个参数  mouse的状态
	CMP	MOUSE_COUNT,1
	JZ	MOUSE_H_SECOND							;1:第二个参数  mouse x方向数据
	CMP	MOUSE_COUNT,2
	JZ	MOUSE_H_THIRD								;2:第三个参数  mouse y方向数据
;;
MOUSE_H_FIRST:
	MOV	AL,MOUSE_STATE
	MOV	MOUSE_STATE_OLD,AL 							;将上一次的鼠标状态保存
	ADD	MOUSE_COUNT,1
	MOV	MOUSE_STATE,DL 						;鼠标的状态保存在MOUSE_STATE中
	MOV	AX,MOUSE_XCORD
	MOV	MOUSE_XCORDOLD,AX
	MOV	AX,MOUSE_YCORD
	MOV	MOUSE_YCORDOLD,AX
	TEST	DX,0001H										;最低位为1 表示左键按下
	JNZ	MOUSE_LBUTTON_DOWN						;为1时表示鼠标左键按下状态，留下了鼠标按键动作处理的接口
	MOV	AL,MOUSE_STATE_OLD
	TEST	AL,0001H										;上次是不是按下了鼠标左键?
	JZ		PRE_MOUSE_LBUTTON_NOT
	;此处表示鼠标左键松开了
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
;鼠标左键按下的处理接口
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
	TEST	DX,0002H		;继续判断鼠标右键是否按下
	JNZ	MOUSE_RBUTTON_DOWN_YES	;鼠标右键处理
	MOV	AL,MOUSE_STATE_OLD
	TEST	AL,0002H										;上次是不是按下了鼠标右键?
	JZ		PRE_MOUSE_RBUTTON_NOT
	;此处表示鼠标右键松开了
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
	;鼠标右键按下的处理接口
	PUSHA
	PUSH	DS
	MOV	AX,INVOKER_DATA_SEG
	MOV	DS,AX
	MOV	GL_MOUSE_RBUTTON_DOWN,1
	POP	DS
	POPA
	JMP	MOUSE_H_OVER
;X位移处理函数，只改变MOUSE_XCORD的值，不绘制鼠标形状；等到MOUSE_YCORD处理的时候再画
MOUSE_H_SECOND:
	ADD	MOUSE_COUNT,1
	CMP	DX,0
	JNZ	MOUSE_H_MOVED_X					;鼠标x方向有移动
	JMP	MOUSE_H_OVER						;没有移动则跳转
MOUSE_H_MOVED_X:
	TEST	MOUSE_STATE,10H			;00010000表示负方向移动
	JNZ	XNEGATIVE
	ADD	MOUSE_XCORD,DX
	;判断屏幕显示模式，从而得到屏幕界限，防止鼠标移出屏幕
	;800x600－－xmax＝799  1024x768－－xmax＝1023  640x480－－xmax＝639
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
	NEG	DL							;负方向移动处理
	SUB	MOUSE_XCORD,DX
	CMP	MOUSE_XCORD,0
	JL	MOUSE_H_XLESS0
	JMP	MOUSE_H_OVER
MOUSE_H_XLESS0:
	MOV	MOUSE_XCORD,0
	JMP	MOUSE_H_OVER
;
MOUSE_H_THIRD:
	MOV	MOUSE_COUNT,0				;鼠标y方向处理
	CMP	DX,0
	JNZ	MOUSE_H_MOVEDY
	JMP	MOUSE_H_COMPLETE				;即使y方向没有位移也调用鼠标处理函数
;坐标采用原点在左上角的笛卡儿坐标，右向和下向为正
;鼠标y轴方向同此，向下为正
MOUSE_H_MOVEDY:
	TEST	MOUSE_STATE,20H
	JNZ	YNEGATIVE
	;如果非负，则直接减去
	SUB	MOUSE_YCORD,DX
	CMP	MOUSE_YCORD,0
	JL	MOUSE_H_YLESS0	;有符号数比较,小于转移
	JMP	MOUSE_H_COMPLETE
MOUSE_H_YLESS0:
	MOV	MOUSE_YCORD,0
	JMP	MOUSE_H_COMPLETE
;如果为负，取补后相加
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
	;比较是否超出了各种显示模式的界限值
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
	;将上一次的SAVENEW复制到SAVEOLD,SAVENEW接受新值
	PUSH	CS
	POP	AX
	MOV	ES,AX
	MOV	DS,AX
	MOV	SI,OFFSET SAVENEW
	MOV	DI,OFFSET SAVEOLD
	MOV	CX,MOUSETYPELEN
	CLD
	REP MOVSB
	;恢复屏幕，用SAVEOLD值
	CALL	RESTORE_MOUSE
	;保存新鼠标位置处的屏幕到SAVENEW处
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
;*子程序名：	MOUSE_MAPMEMORY												**
;*功能：			计算所要显示点的显示页和偏移地址			**
;*入口参数：	EDX:	Y坐标     EBX:  X坐标						**
;*出口参数：	SEGINDEX:显存地址页号，     					**
;*						SEGOFFSET:显存地址偏移								**
;*程序备注：	在不同显示模式下，有不同的映射方式		**	
;*						本子程序已经实现换页，								**
;*程序日志：	101号模式调试通过 2005-12-20	luobing	**
;****************************************************;
MOUSE_MAPMEMORY	PROC	
	MOV	EAX,EDX 								;EAX->Y
	XOR	ECX,ECX
	MOV	CX,WORD PTR MOUSE_BytesPerScanLine
	;JMP	MOUSE_MM_GGG
;MOUSE_MM_GGG:
;	MOV	ECX,832
	MUL	ECX				;ECX中放置一行的点数，EAX*ECX=Y*一行的点数
	ADD	EAX,EBX 	;ADDRESS＝X+Y*一行的点数
	MOV	CL,16
	MOV	EBX,EAX
	SHR	EBX,CL  	;地址中的页号：右移16位
	MOV	SEGINDEX,BX
	AND	EAX,0FFFFH	;地址偏移OFFSET
	MOV	SEGOFFSET,AX
;比较与上一个点是否在同一个页面上，是则不换页
	CMP	SEGINDEXOLD,BX
	JZ	INSAMEPAGE
	MOV	SEGINDEXOLD,BX
	MOV	BX,0     ;否则调用换页中断
	MOV	DX,SEGINDEX
	MOV	AX,04F05H
	INT	10H
INSAMEPAGE:
	RET
MOUSE_MAPMEMORY	ENDP
;-------------------------------------------
;****************************************************;
;*子程序名：	RESTORE_MOUSE													**
;*功能：			恢复老鼠标位置屏幕										**
;*入口参数：	SAVEOLD数组中的内容										**
;*出口参数：	恢复mouse_xcordold和mouse_ycord位置		**
;*						的内容																**						
;****************************************************;
RESTORE_MOUSE	PROC	NEAR
	PUSHAD
	MOV	EBX,0
	MOV	EDX,0
	MOV	ECX,0
	MOV	EAX,0
	MOV	AX,MOUSETYPELEN
	SHR	AX,1
	MOV	MOUSETYPECOUNTER,AX 			;计算鼠标图像的大小
	MOV	DI,0
	MOV	SI,0
RESTORE_MOUSEPIXEL:
	MOV	BX,MOUSE_XCORDOLD							;将前一个鼠标位置X坐标放到BX中
	MOV	AX,MOUSETYPE[DI]				;将鼠标中的一个点放到AX中
	PUSH	AX
	MOV	CL,8
	SHR	AX,CL
	AND	AX,0FFH									;AX=00XXH获得该点相对于基准点的坐标偏移
	ADD	BX,AX										;实际坐标
	POP	AX
	;
	MOV	DX,MOUSE_YCORDOLD
	AND	AX,0FFH									;AX=00YYH
	ADD	DX,AX 									;实际坐标
	CALL	MOUSE_MAPMEMORY							;进行地址映射计算PAGE/OFFET
	MOV	AX,0A000H
	MOV	ES,AX
	MOV	BX,SEGOFFSET
	MOV	AL,SAVEOLD[DI]				;恢复屏幕原内容
	MOV	ES:[BX],AL
	INC	DI
	INC	DI						;DI+2指向下一个WORD
	DEC	MOUSETYPECOUNTER
	JNZ	RESTORE_MOUSEPIXEL
	POPAD
	RET
RESTORE_MOUSE	ENDP
;------------------------------------------------------
;****************************************************;
;*子程序名：	SAVE_MOUSE														**
;*功能：			保存当前鼠标位置的屏幕内容						**
;*入口参数：	当前的鼠标位置												**
;*出口参数：	SAVENEW数组中的内容										**
;****************************************************;	
SAVE_MOUSE	PROC	NEAR
	PUSHAD
	MOV	EBX,0
	MOV	EDX,0
	MOV	ECX,0
	MOV	EAX,0
	MOV	AX,MOUSETYPELEN
	SHR	AX,1
	MOV	MOUSETYPECOUNTER,AX 			;计算鼠标图形数组的大小
	XOR	DI,DI
	XOR	SI,SI
SAVE_M_SAVEPIXEL:
	MOV	BX,MOUSE_XCORD
	MOV	AX,MOUSETYPE[DI]					;取得鼠标相对坐标
	PUSH	AX
	MOV	CL,8
	SHR	AX,CL
	AND	AX,0FFH
	ADD	BX,AX 							;获取x坐标
	POP	AX
	MOV	DX,MOUSE_YCORD
	AND	AX,0FFH
	ADD	DX,AX 							;获取y坐标
	CALL	MOUSE_MAPMEMORY		;获得页号和偏移
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
;*子程序名：	SHOW_MOUSE														**
;*功能：			绘制鼠标															**
;*入口参数：	鼠标数据区的图像数组									**
;*出口参数：																				**
;*程序日志：	101号模式调试通过 2005-12-20	luobing	**
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
	CMP	BX,799			;>799,不显示
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
SHOW_M_E1024:
	CMP	BX,1023 		;>1023不显示
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
SHOW_M_E640:
	CMP	BX,639
	JNBE	SHOW_M_CONT
	JMP	NOTEXCEED
NOTEXCEED:								;没有越界
	CALL	MOUSE_MAPMEMORY							;计算鼠标图形点的页号和偏移
	MOV	AX,0A000H
	MOV	ES,AX
	MOV	BX,SEGOFFSET
	MOV	AL,BYTE PTR MOUSECOLOR[DI]		;取颜色
	MOV BYTE PTR ES:[BX],AL
	JMP	SHOW_M_CONT
SHOW_M_CONT:
	INC	DI
	INC	DI					;DI+2指向下一个WORD
	DEC	MOUSETYPECOUNTER	;绘制下一个点
	JNZ	LODRMOS
	POPAD
	RET
SHOW_MOUSE	ENDP
;
MOUSE_HANDLER_TEST	PROC	FAR

	RETF
MOUSE_HANDLER_TEST	ENDP