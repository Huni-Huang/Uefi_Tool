;****************************************************;
;*子程序名：	RESETDISK															**
;*功能：			磁盘控制器重启(FLOP HD)								**
;*入口参数：	无																		**
;*出口参数：	CF=0	成功				CF=1  失败					**
;****************************************************;
RESETDISK	PROC	NEAR
	MOV	AH,0
	MOV	DL,80H				;缺省认为硬盘在IDE1 主盘
	INT	13H
	;
	RET
RESETDISK	ENDP
;****************************************************;
;*子程序名：	GETDISKATTR														**
;*功能：			获取磁盘属性													**
;*入口参数：	无																		**
;*出口参数：	CF=0	成功				CF=1  失败					**
;							AL=0  支持扩展		AL=1  不支持扩展		**
;****************************************************;
GETDISKATTR	PROC	NEAR
	MOV	AH,41H
	MOV	BX,55AAH
	MOV	DL,80H			;缺省认为硬盘在IDE1 主盘
	INT	13H
	JC	GDA_NOSUPPORT		;访问失败，不支持扩展读写
	CMP	BX,0AA55H
	JNZ	GDA_NOSUPPORT		;访问失败，不支持扩展读写
	;支持扩展读写的话开始填充磁盘属性
	MOV	AX,4800H
	MOV	DX,0080H
	MOV	SI,OFFSET EDPARA_TABLE		;传送磁盘参数
	INT	13H
	;传送EDD
	MOV AX,EDD_SEG
	MOV	ES,AX
	MOV	DI,EDD_OFFSET
	MOV	SI,OFFSET EDD_BASEPORT			;SI->EDD
	MOV	CX,8
GDA_FILLEDD:
	MOV	AX,ES:WORD PTR [DI]
	MOV	[SI],AX
	INC	SI
	INC	SI
	INC	DI
	INC	DI
	LOOP	GDA_FILLEDD
	MOV	AL,0
	JMP	GDA_EXIT
GDA_NOSUPPORT:	
	MOV	AL,1
GDA_EXIT:
	RET
GETDISKATTR	ENDP
;****************************************************;
;*子程序名：	CALOFFSET()	(##NOTTEST)								**
;*功能：			计算offset值，单网可用LBA，扩展的LBA	**
;*入口参数：																				**
;*出口参数：																				**
;*函数说明：本函数经过改造，不仅仅填充硬盘的数据结构**
;*					而且填充卡的EEPROM TABLE.本来不想这么做,**
;*					考虑到程序可读性和程序的大小，权衡良久，**
;*					还是妥协了，从字面上已经看不出函数的作用**
;*					了    罗冰2005-7-1 20:52								**
;****************************************************;
CALOFFSET	PROC	NEAR
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	;小字表示在程序结尾时候添加的代码
	push	di
	mov	ax,ds
	mov	es,ax
	;	
	MOV	SI,OFFSET	EDPARA_TOTAL
	MOV	AX,WORD	PTR	[SI]
	MOV	DX,WORD	PTR	[SI+2]	;DX:AX EDPARA_TOTAL
	MOV	BX,DX
	MOV	CL,6								;计算OFFSET值
	SHR BX,CL
	SAL	BX,CL
	MOV	SI,OFFSET MYOFFSETVAL		;存入MYOFFSETVAL(GCDZONE[7])
	XOR	AX,AX
	MOV	[SI],AX
	MOV	[SI+2],BX
	;
	mov	di,offset MYZONE_OFFSET	;填充EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	push	bx
	sub	bx,2
	mov	di,offset WAI_MAX		;填充EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	pop	bx
	;
	MOV	CL,6
	SHR BX,CL		
	MOV	CL,5
	SAL BX,CL								;BX存贮了offset高16位值
	XOR	AX,AX
	MOV	SI,OFFSET	OFFSETVAL	;存入OFFSETVAL
	MOV	[SI],AX
	MOV	[SI+2],BX
	;
	mov	di,offset WAI_OFFSET	;填充EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	SUB	BX,2										;计算单网可用的总逻辑扇区数
	MOV	SI,OFFSET	PART_MAXLBA		;存入PART_MAXLBA
	MOV	[SI],AX
	MOV	[SI+2],BX								;bx存储了part_maxlba的高16位
	;
	mov	di,offset NEI_MAX	;填充EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	;计算扩展分区的大小，(可用386的寄存器)
	CLC
	SUB	AX,01676H
	SBB	BX,0271H					;减去起始的扇区
	MOV	SI,OFFSET MBR_EXT_MAX
	MOV	[SI],AX
	MOV	[SI+2],BX
	;##AMEND	扩展分区的结束扇区应该能整除 3FH
	;2005 - 7 - 8   18:17
	;用386指令计算扩展扇区的大小
	xor	edx,edx
	mov	dx,bx
	mov	ebx,mbr_ext_max
	mov	cx,03f00h
	div	cx						;余数在dx中
	mov	ax,dx
	mov	cx,003fh
	div	cl						;余数在ah中
	xor	edx,edx
	mov	dl,ah
	sub	ebx,edx
	mov	mbr_ext_max,ebx
	;##AMEND	计算扩展分区的结束LBA 的头，返填
	;2005-7-12 luobing
	mov	eax,mbr_ext_start
	add ebx,eax
	sub	ebx,1
	xor	edx,edx
	mov	eax,ebx    ;准备64位除法 /3f
	mov	ecx,0000003fh
	div	ecx					;商在eax中
	xor	edx,edx     ;余数丢弃，准备求模 %ff     
	mov	ecx,000000ffh
	div	ecx					;edx中放有余数 ，dl为所要数,dh=0
	mov	m_ext_endhead,dl
	
	;
	mov	eax,EDPARA_TOTAL		;;填充EEPROM table
	sub	eax,1
	mov	MYZONE_MAX,eax
	pop	di
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
	RET
CALOFFSET	ENDP
;****************************************************;
;*子程序名：	RWDISKLBA															**
;*功能：			扩展读写硬盘													**
;*入口参数：	EDAP_TABLE,AH(必须自己填充表)					**
;*出口参数：	CF=0 成功    CF=1失败									**
;****************************************************;
RWDISKLBA	PROC	NEAR
	MOV	DL,80H
	MOV	SI,OFFSET	EDAP_TABLE		;DS:SI
	INT	13H
	RET
RWDISKLBA	ENDP
;****************************************************;
;*子程序名：	READUSERMBR														**
;*功能：			读硬盘原来的MBR												**
;*入口参数：																				**
;*出口参数：	USER_MBR															**
;****************************************************;	
READUSERMBR	PROC	NEAR
	MOV	EDAP_RWCOUNT,01H
	MOV	AX,OFFSET USER_MBR
	MOV	EDAP_OFFSET,AX
	MOV	AX,DS
	MOV	EDAP_SEG,AX
	MOV	EDAP_RWLBAL,00H
	MOV	EDAP_RWLBAH,00H
	MOV	AH,42H
	CALL	RWDISKLBA
	RET
READUSERMBR	ENDP
;****************************************************;
;*子程序名：	READUSERWAIMBR												**
;*功能：			读硬盘原来的WAIMBR										**
;*入口参数：																				**
;*出口参数：	USERWAI_MBR														**
;****************************************************;	
READUSERWAIMBR	PROC	NEAR
	MOV	EDAP_RWCOUNT,01H
	MOV	AX,OFFSET USER_MBR
	MOV	EDAP_OFFSET,AX
	MOV	AX,DS
	MOV	EDAP_SEG,AX
	;
	MOV	SI,OFFSET OFFSETVAL
	MOV	AX,WORD PTR [SI]
	MOV	BX,WORD PTR [SI+2]
	MOV	EDAP_RWLBAL,AX
	MOV	EDAP_RWLBAH,BX
	MOV	AH,42H
	CALL	RWDISKLBA
	RET
READUSERWAIMBR	ENDP
;****************************************************;
;*子程序名：	COMPAERMBR														**
;*功能：			比较用户的分区表和规定的分区表,				**
;									除去用户的主分区类型（不比较）		**
;*入口参数：																				**
;*出口参数：	ax=0表示两字符串相同,否则不同					**
;****************************************************;
COMPAREMBR	PROC	NEAR
	MOV	AX,DS
	MOV	ES,AX
	;
	MOV	CX,4
	MOV	SI,OFFSET MBR_PRI
	MOV	DI,OFFSET USER_PRI
	REPZ	CMPSB			;比较
	MOV	AL,[SI-1]
	MOV	BL,ES:[DI-1]
	XOR	AH,AH
	MOV	BH,AH
	SUB	AX,BX
	JNZ	COMPPAR_EXIT
	MOV	CX,61				;比较的字节数
	MOV	SI,OFFSET MBR_PRI_CHS
	MOV	DI,OFFSET USER_PRI_CHS
	REPZ	CMPSB			;比较,
	MOV	AL,[SI-1]
	MOV	BL,ES:[DI-1]
	XOR	AH,AH
	MOV	BH,AH
	SUB	AX,BX
COMPPAR_EXIT:
	RET
COMPAREMBR	ENDP
;****************************************************;
;*子程序名：	FILLMBR																**
;*功能：			写MBR区																**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
FILLMBR	PROC	NEAR
	MOV	AL,USER_PRI_TYPE
	CMP	AL,0
	JZ	FM_NOTYPE
	MOV	MBR_PRI_TYPE,AL
FM_NOTYPE:
	;#AMMEND 2005-7-25 16:03
	;问题:在安装过程中，win2000发现分区存在的话，不会写主引导扇区
	mov	eax,dword ptr user_mbr
	cmp	eax,0
	jz	fm_nombr
	;将用户的MBR前466字节移动到准备写入硬盘的MBR
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET USER_MBR		;准备源
	MOV	DI,OFFSET MBR					;准备目的
	MOV	CX,223
	REP	MOVSW
fm_nombr:
	;准备好MBR,开始写入内网
	MOV	EDAP_RWCOUNT,01H
	MOV	AX,OFFSET MBR
	MOV	EDAP_OFFSET,AX
	MOV	AX,DS
	MOV	EDAP_SEG,AX
	MOV	EDAP_RWLBAL,00H
	MOV	EDAP_RWLBAH,00H
	MOV	AH,43H
	CALL	RWDISKLBA
	RET
FILLMBR	ENDP
;****************************************************;
;*子程序名：	FILLWAIMBR														**
;*功能：			写外网MBR区														**
;*入口参数：																				**
;*出口参数：																				**
;****************************************************;
FILLWAIMBR	PROC	NEAR
	MOV	AL,USER_PRI_TYPE
	CMP	AL,0
	JZ	FMWAI_NOTYPE
	MOV	MBR_PRI_TYPE,AL
FMWAI_NOTYPE:
	;#AMMEND 2005-7-25 16:03
	;问题:在安装过程中，win2000发现分区存在的话，不会写主引导扇区
	mov	eax,dword ptr user_mbr
	cmp	eax,0
	jz	fmwai_nombr
	;将用户的MBR前466字节移动到准备写入硬盘的MBR
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET USER_MBR		;准备源
	MOV	DI,OFFSET MBR					;准备目的
	MOV	CX,223
	REP	MOVSW
	;准备好MBR
	;##AMEND 2005-7-4 20:20
	;开始写入外网
fmwai_nombr:
	;将缓冲区的446字节重新写回MBR的数据结构
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET MBRBUFFER		;准备源
	MOV	DI,OFFSET MBR					;准备目的
	MOV	CX,223
	REP	MOVSW
	;
	mov	edap_rwcount,01h
	mov	ax,offset mbr
	mov	edap_offset,ax
	mov	ax,ds
	mov	edap_seg,ax
	mov	si,offset offsetval
	mov	ax,word ptr [si]
	mov	bx,word ptr [si+2]
	mov	edap_rwlbal,ax
	mov	edap_rwlbah,bx
	mov	ah,43h
	call	rwdisklba
	;
	RET
FILLWAIMBR	ENDP
;****************************************************;
;*子程序名：	CLEARPART23														**
;*功能：			清除第3和第4分区表,并且填回去					**
;*入口参数：	已经读取到USER MBR的内容							**
;*出口参数：	无																		**
;****************************************************;
CLEARPART23	PROC	NEAR
	PUSH	AX
	PUSH	CX
	PUSH	SI
	MOV	SI,OFFSET USER_NON_PART
	MOV	CX,32
CLEAR_N_PART23:
	MOV BYTE PTR [SI],00H
	INC	SI
	LOOP	CLEAR_N_PART23
	;将MBR区重新填回去
	MOV	EDAP_RWCOUNT,01H
	MOV	AX,OFFSET USER_MBR
	MOV	EDAP_OFFSET,AX
	MOV	AX,DS
	MOV	EDAP_SEG,AX
	MOV	EDAP_RWLBAL,00H
	MOV	EDAP_RWLBAH,00H
	MOV	AH,43H
	CALL	RWDISKLBA
	POP	SI
	POP	CX
	POP	AX
	RET
CLEARPART23	ENDP
		
		