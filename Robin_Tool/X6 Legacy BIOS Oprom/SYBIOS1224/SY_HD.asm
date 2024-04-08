;****************************************************;
;*�ӳ�������	RESETDISK															**
;*���ܣ�			���̿���������(FLOP HD)								**
;*��ڲ�����	��																		**
;*���ڲ�����	CF=0	�ɹ�				CF=1  ʧ��					**
;****************************************************;
RESETDISK	PROC	NEAR
	MOV	AH,0
	MOV	DL,80H				;ȱʡ��ΪӲ����IDE1 ����
	INT	13H
	;
	RET
RESETDISK	ENDP
;****************************************************;
;*�ӳ�������	GETDISKATTR														**
;*���ܣ�			��ȡ��������													**
;*��ڲ�����	��																		**
;*���ڲ�����	CF=0	�ɹ�				CF=1  ʧ��					**
;							AL=0  ֧����չ		AL=1  ��֧����չ		**
;****************************************************;
GETDISKATTR	PROC	NEAR
	MOV	AH,41H
	MOV	BX,55AAH
	MOV	DL,80H			;ȱʡ��ΪӲ����IDE1 ����
	INT	13H
	JC	GDA_NOSUPPORT		;����ʧ�ܣ���֧����չ��д
	CMP	BX,0AA55H
	JNZ	GDA_NOSUPPORT		;����ʧ�ܣ���֧����չ��д
	;֧����չ��д�Ļ���ʼ����������
	MOV	AX,4800H
	MOV	DX,0080H
	MOV	SI,OFFSET EDPARA_TABLE		;���ʹ��̲���
	INT	13H
	;����EDD
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
;*�ӳ�������	CALOFFSET()	(##NOTTEST)								**
;*���ܣ�			����offsetֵ����������LBA����չ��LBA	**
;*��ڲ�����																				**
;*���ڲ�����																				**
;*����˵�����������������죬���������Ӳ�̵����ݽṹ**
;*					������俨��EEPROM TABLE.����������ô��,**
;*					���ǵ�����ɶ��Ժͳ���Ĵ�С��Ȩ�����ã�**
;*					������Э�ˣ����������Ѿ�����������������**
;*					��    �ޱ�2005-7-1 20:52								**
;****************************************************;
CALOFFSET	PROC	NEAR
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	;С�ֱ�ʾ�ڳ����βʱ����ӵĴ���
	push	di
	mov	ax,ds
	mov	es,ax
	;	
	MOV	SI,OFFSET	EDPARA_TOTAL
	MOV	AX,WORD	PTR	[SI]
	MOV	DX,WORD	PTR	[SI+2]	;DX:AX EDPARA_TOTAL
	MOV	BX,DX
	MOV	CL,6								;����OFFSETֵ
	SHR BX,CL
	SAL	BX,CL
	MOV	SI,OFFSET MYOFFSETVAL		;����MYOFFSETVAL(GCDZONE[7])
	XOR	AX,AX
	MOV	[SI],AX
	MOV	[SI+2],BX
	;
	mov	di,offset MYZONE_OFFSET	;���EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	push	bx
	sub	bx,2
	mov	di,offset WAI_MAX		;���EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	pop	bx
	;
	MOV	CL,6
	SHR BX,CL		
	MOV	CL,5
	SAL BX,CL								;BX������offset��16λֵ
	XOR	AX,AX
	MOV	SI,OFFSET	OFFSETVAL	;����OFFSETVAL
	MOV	[SI],AX
	MOV	[SI+2],BX
	;
	mov	di,offset WAI_OFFSET	;���EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	SUB	BX,2										;���㵥�����õ����߼�������
	MOV	SI,OFFSET	PART_MAXLBA		;����PART_MAXLBA
	MOV	[SI],AX
	MOV	[SI+2],BX								;bx�洢��part_maxlba�ĸ�16λ
	;
	mov	di,offset NEI_MAX	;���EEPROM table
	mov	[di],ax
	mov	[di+2],bx
	;
	;������չ�����Ĵ�С��(����386�ļĴ���)
	CLC
	SUB	AX,01676H
	SBB	BX,0271H					;��ȥ��ʼ������
	MOV	SI,OFFSET MBR_EXT_MAX
	MOV	[SI],AX
	MOV	[SI+2],BX
	;##AMEND	��չ�����Ľ�������Ӧ�������� 3FH
	;2005 - 7 - 8   18:17
	;��386ָ�������չ�����Ĵ�С
	xor	edx,edx
	mov	dx,bx
	mov	ebx,mbr_ext_max
	mov	cx,03f00h
	div	cx						;������dx��
	mov	ax,dx
	mov	cx,003fh
	div	cl						;������ah��
	xor	edx,edx
	mov	dl,ah
	sub	ebx,edx
	mov	mbr_ext_max,ebx
	;##AMEND	������չ�����Ľ���LBA ��ͷ������
	;2005-7-12 luobing
	mov	eax,mbr_ext_start
	add ebx,eax
	sub	ebx,1
	xor	edx,edx
	mov	eax,ebx    ;׼��64λ���� /3f
	mov	ecx,0000003fh
	div	ecx					;����eax��
	xor	edx,edx     ;����������׼����ģ %ff     
	mov	ecx,000000ffh
	div	ecx					;edx�з������� ��dlΪ��Ҫ��,dh=0
	mov	m_ext_endhead,dl
	
	;
	mov	eax,EDPARA_TOTAL		;;���EEPROM table
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
;*�ӳ�������	RWDISKLBA															**
;*���ܣ�			��չ��дӲ��													**
;*��ڲ�����	EDAP_TABLE,AH(�����Լ�����)					**
;*���ڲ�����	CF=0 �ɹ�    CF=1ʧ��									**
;****************************************************;
RWDISKLBA	PROC	NEAR
	MOV	DL,80H
	MOV	SI,OFFSET	EDAP_TABLE		;DS:SI
	INT	13H
	RET
RWDISKLBA	ENDP
;****************************************************;
;*�ӳ�������	READUSERMBR														**
;*���ܣ�			��Ӳ��ԭ����MBR												**
;*��ڲ�����																				**
;*���ڲ�����	USER_MBR															**
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
;*�ӳ�������	READUSERWAIMBR												**
;*���ܣ�			��Ӳ��ԭ����WAIMBR										**
;*��ڲ�����																				**
;*���ڲ�����	USERWAI_MBR														**
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
;*�ӳ�������	COMPAERMBR														**
;*���ܣ�			�Ƚ��û��ķ�����͹涨�ķ�����,				**
;									��ȥ�û������������ͣ����Ƚϣ�		**
;*��ڲ�����																				**
;*���ڲ�����	ax=0��ʾ���ַ�����ͬ,����ͬ					**
;****************************************************;
COMPAREMBR	PROC	NEAR
	MOV	AX,DS
	MOV	ES,AX
	;
	MOV	CX,4
	MOV	SI,OFFSET MBR_PRI
	MOV	DI,OFFSET USER_PRI
	REPZ	CMPSB			;�Ƚ�
	MOV	AL,[SI-1]
	MOV	BL,ES:[DI-1]
	XOR	AH,AH
	MOV	BH,AH
	SUB	AX,BX
	JNZ	COMPPAR_EXIT
	MOV	CX,61				;�Ƚϵ��ֽ���
	MOV	SI,OFFSET MBR_PRI_CHS
	MOV	DI,OFFSET USER_PRI_CHS
	REPZ	CMPSB			;�Ƚ�,
	MOV	AL,[SI-1]
	MOV	BL,ES:[DI-1]
	XOR	AH,AH
	MOV	BH,AH
	SUB	AX,BX
COMPPAR_EXIT:
	RET
COMPAREMBR	ENDP
;****************************************************;
;*�ӳ�������	FILLMBR																**
;*���ܣ�			дMBR��																**
;*��ڲ�����																				**
;*���ڲ�����																				**
;****************************************************;
FILLMBR	PROC	NEAR
	MOV	AL,USER_PRI_TYPE
	CMP	AL,0
	JZ	FM_NOTYPE
	MOV	MBR_PRI_TYPE,AL
FM_NOTYPE:
	;#AMMEND 2005-7-25 16:03
	;����:�ڰ�װ�����У�win2000���ַ������ڵĻ�������д����������
	mov	eax,dword ptr user_mbr
	cmp	eax,0
	jz	fm_nombr
	;���û���MBRǰ466�ֽ��ƶ���׼��д��Ӳ�̵�MBR
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET USER_MBR		;׼��Դ
	MOV	DI,OFFSET MBR					;׼��Ŀ��
	MOV	CX,223
	REP	MOVSW
fm_nombr:
	;׼����MBR,��ʼд������
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
;*�ӳ�������	FILLWAIMBR														**
;*���ܣ�			д����MBR��														**
;*��ڲ�����																				**
;*���ڲ�����																				**
;****************************************************;
FILLWAIMBR	PROC	NEAR
	MOV	AL,USER_PRI_TYPE
	CMP	AL,0
	JZ	FMWAI_NOTYPE
	MOV	MBR_PRI_TYPE,AL
FMWAI_NOTYPE:
	;#AMMEND 2005-7-25 16:03
	;����:�ڰ�װ�����У�win2000���ַ������ڵĻ�������д����������
	mov	eax,dword ptr user_mbr
	cmp	eax,0
	jz	fmwai_nombr
	;���û���MBRǰ466�ֽ��ƶ���׼��д��Ӳ�̵�MBR
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET USER_MBR		;׼��Դ
	MOV	DI,OFFSET MBR					;׼��Ŀ��
	MOV	CX,223
	REP	MOVSW
	;׼����MBR
	;##AMEND 2005-7-4 20:20
	;��ʼд������
fmwai_nombr:
	;����������446�ֽ�����д��MBR�����ݽṹ
	MOV	AX,DS
	MOV	ES,AX
	MOV	SI,OFFSET MBRBUFFER		;׼��Դ
	MOV	DI,OFFSET MBR					;׼��Ŀ��
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
;*�ӳ�������	CLEARPART23														**
;*���ܣ�			�����3�͵�4������,�������ȥ					**
;*��ڲ�����	�Ѿ���ȡ��USER MBR������							**
;*���ڲ�����	��																		**
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
	;��MBR���������ȥ
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
		
		