;sy son for C 2009-11-9 18:17 Q45
;2010-1-28 15:26:09 luobing 将获取smbus base address改为自动获取
;使用lb_spd中调试的结果改写的
;CFG_ADDR_PORT		EQU	0CF8h		; PCI address port
;CFG_DATA_PORT		EQU	0CFCh		; PCI data port

;READ_BLOCK		EQU	00h
;WRITE_BLOCK		EQU	01h

;PCI_FUN_3		EQU	03h		; PCI function 3
;PCI_DEV_1F		EQU	01Fh		; PCI device 1F
;PCI_BUS_00		EQU	08000h		; PCI bus 0
;-------------------------------------------------------------------------
;
; SMbus values
;
ICH_SMB_IOREG00		EQU	00h		; SMbus Host Status
ICH_SMB_IOREG02		EQU	02h		; SMbus Host Control
ICH_SMB_IOREG03		EQU	03h		; SMbus Host Command
ICH_SMB_IOREG04		EQU	04h		; SMbus Xmit Slave Address
ICH_SMB_IOREG05		EQU	05h		; SMbus Host Data 0
;ICH_SMB_IOREG06		EQU	06h		; SMbus Host Data 1
;ICH_SMB_IOREG07		EQU	07h		; SMbus Block Data Byte
;ICH_SMB_IOREG09		EQU	09h		; SMbus Rcv Slave Address
;ICH_SMB_IOREG0A		EQU	0Ah		; SMbus Slave Data
;ICH_SMB_IOREG0D		EQU	0Dh		; SMbus Aux Control


;SMB_BASE_REG		EQU	20h		; SMbus base address
;  SMB_BASE_MASK		EQU	0FFF0h		;   masks out LS nibble
;SMB_DEV_STATUS		EQU	06H		; SMbus device status reg
;  SMB_TARGET_ABORT	EQU	0000100000000000b;  Target-abort flag
;SMB_CMD_REG		EQU	04h		; SMbus Command register
;  SMB_IO_ENABLE		EQU	0001h		;   Enables the SMbus
;SMB_HOST_REG		EQU	040h		; SMbus host config reg
;  SMB_HOST_ENABLE	EQU	00000001b	;   Enable host flag
;  SMB_HOST_RESET	EQU	00001000b	;   Reset host
  
  
;  SMB_BUSY_MASK		EQU	00000001b	; SMbus busy
SMB_INT_MASK		EQU	00000010b	; SMbus int mask
SMB_ERROR_MASK		EQU	00011100b	; SMbus error mask
;SMB_ALERT_MASK		EQU	00100000b	; SMbus SMALERT mask
SMB_START		EQU	01000000b	; SMbus control - start
SMB_READ		EQU	00001000b	; SMbus control - read
;SMB_BLOCK		EQU	00010100b	; SMbus control - block
;SMB_ADDR_READ_MASK	EQU	00000001b	; SMbus sets read bit
SMB_ERROR_INT_MASK	EQU	SMB_ERROR_MASK + SMB_INT_MASK
;SMB_READ_START		EQU	SMB_READ + SMB_START
SMB_CLEAR_MASK		EQU	10111110b	; SMbus clear status flags

;DIMM0_SPD_ADDRESS	EQU	0A0h		; Channel A DIMM 0
;DIMM1_SPD_ADDRESS	EQU	0A2h		; Channel A DIMM 1
;DIMM2_SPD_ADDRESS	EQU	0A4h		; Channel B DIMM 0
;DIMM3_SPD_ADDRESS	EQU	0A6h		; Channel B DIMM 1

;SMB_BASE_ADDR = 1c20h  ;Q45上的smbus base addr
.model small
.386
.data
.code
public _I2C_Read
public _I2C_Write
public _Delay10Ms
public _ASC2HEX
;[]==============================================================[]
;Input      none
;Output : 	none
;[]==============================================================[]
get_SMBAddr	proc	near
	pusha
	jmp	get_SMBAddr_next
	SMB_BASE_ADDR	DW	01C20H
get_SMBAddr_next:
;	int 3
;	mov	ax,0b109h
;	mov	bx,31
;	shl	bx,3							;bl7-3 Device number   bl2-0 Function number
;	or	bx,03h						;D31:F3 smbus
;	mov	di,020h
;	int 1ah
;	jc get_SMBAddr_err
;	and	cx,0fffeh
;	mov	word ptr cs:[SMB_BASE_ADDR],cx
  mov	ax, 8000h 
  shl	eax, 16 
  mov	ah, 31	;SMBUS_DEV                 ; pci dev number of south bridge 
  shl	ah, 3 
  or	ah, 3; SMBUS_FUNC                ; pci function number of smbus dev 
  mov	al, 20h; ICH_SMB_REG20        ; smbus base address regiser 
  mov	dx, 0CF8h 
  out	dx, eax 
  mov	dx, 0CFCh 
  in	ax, dx 
  and	ax, 0FFF0h;        SMB_ADDR_MASK 
  mov	dx, ax 
	mov	word ptr cs:[SMB_BASE_ADDR],dx
;get_SMBAddr_err:				;如果出错，则直接使用原始值
	popa
	ret
get_SMBAddr	endp
;unsigned char I2C_Read(unsigned char DeviceID,unsigned char RegIndex)
;[bp+4]=DeviceID,[bp+6]=RegIndex,
_I2C_Read	proc	near
	push	bp
	mov	bp,sp
	push    bx
	push    cx
	push    dx
	call get_SMBAddr
	mov	ah,byte ptr [bp+4]
	mov	al,byte ptr [bp+6]
	
	call	smbClearStatus		; clear the SMBus status register
	call	smbSetCommand		; writes to the SMBus command reg
					;  starting at byte AL
	mov	bl, ah
	or	bl, 1			; [0] set to read from address
	call	smbSetSlaveAddr		; sets the SMBus slave address
	mov	cl, SMB_READ
	call	smbSendCommand		; sends the command and 1 byte

smbNoDataWaiting:
	call	smbReadStatus		; Check the SMBus status
	jnc	smbNoDataWaiting	; loop if nothing there

	mov	bh, ICH_SMB_IOREG05
	call	smbIoRegRead
	clc

exit:
        pop     dx
        pop     cx
        pop     bx
        pop	bp
        ret
_I2C_Read	endp
;	void I2C_Write(unsigned char DeviceID,unsigned char RegIndex,unsigned char value)
;[bp+4]=DeviceID,[bp+6]=RegIndex,[bp+8]=value
_I2C_Write	proc	near
	push	bp
	mov	bp,sp
	push    bx
	push    cx
	push    dx
	call get_SMBAddr
	mov	ah,byte ptr [bp+4]
	mov	al,byte ptr [bp+6]
	mov	cl,byte ptr [bp+8]
	
	call	smbClearStatus		; clear the SMBus status register
	call	smbSetCommand		; writes to the SMBus command reg
					;  starting at byte AL
	mov	bl, ah
	;or	bl, 1			; [0] set to read from address
	call	smbSetSlaveAddr		; sets the SMBus slave address
	
	;write //luobing
	mov	bh, ICH_SMB_IOREG05
	push	ax
	mov	al,cl
	call smbIoRegWrite
	pop	ax
	mov	cl, SMB_READ
	call	smbSendCommand		; sends the command and 1 byte

smbNoDataWaiting1:
	call	smbReadStatus		; Check the SMBus status
	jnc	smbNoDataWaiting1	; loop if nothing there

	;mov	bh, ICH_SMB_IOREG05
	;call	smbIoRegRead
	clc

exit1:
        pop     dx
        pop     cx
        pop     bx
        pop	bp
        ret
_I2C_Write	endp
;-------------------------------------------------------------------------
; 	smbClearStatus - Clears the SMBus status register
; 	Entry:		None
; 	Exit:		None
; 	Modifies:		None
; 	Processing:
;		Clears the SMBus status register
smbClearStatus PROC NEAR
	push	ax
	push	bx

	mov	bh, ICH_SMB_IOREG00
	mov	al, SMB_CLEAR_MASK
	call	smbIoRegWrite

	pop	bx
	pop	ax
	ret
smbClearStatus ENDP
;-------------------------------------------------------------------------
; 	smbSetCommand - Sets the SMBus command register
; 	Entry:  AL	- byte to write to the command register
; 	Exit:   AL	- byte to write to the command register
; 	Modifies:	None
; 	Processing:		Sets the SMBus command register with AL.
smbSetCommand PROC NEAR
	push	bx

	mov	bh, ICH_SMB_IOREG03
	call	smbIoRegWrite

	pop	bx
	ret
smbSetCommand ENDP

;-------------------------------------------------------------------------
; 	smbIoRegRead - read a register in I/O space
; 	Entry: BH - offset of register to be read
; 	Exit:  AL - register value
; 	Modifies:	None
;; 	Processing:
;	  Get the I/O base address and add it to the register offset to
;	  get the register address in I/O space.  Read a byte from that
;	  location.
smbIoRegRead PROC NEAR
	push	bx
	push	dx

	mov	bl, bh
	xor	bh, bh				; Clear result register

	mov	dx, word ptr cs:SMB_BASE_ADDR		; Get SMB base address
	add	dx, bx				; Point to real address

	in	al, dx				; Read data from port

	pop	dx
	pop	bx
	ret
smbIoRegRead ENDP
;-------------------------------------------------------------------------
; 	smbIoRegWrite - write to a register in I/O space
; 	Entry:       BH - offset of register to be written
;                AL - value to be written
; 	Exit:       BH - offset of register to be written
;                AL - value to be written
;
; 	Modifies:		None
; 	Processing:
;	  Get the I/O base address and add it to the register offset to
;	  get the register address in I/O space.  Write a byte at that
;	  location.
smbIoRegWrite PROC NEAR
	push	bx
	push	dx

	mov	bl, bh
	xor	bh, bh				; Clear result register
	mov	dx, word ptr cs:SMB_BASE_ADDR		; Get SMB base address
	add	dx, bx				; Point to real address

	out	dx, al				; Write data to the port

	pop	dx
	pop	bx
	ret
smbIoRegWrite ENDP

;-------------------------------------------------------------------------
; 	smbSetSlaveAddr - Sets the SMBus slave address
; 	Entry:	     BL	- SMBus slave address
; 	Exit:	     BL	- SMBus slave address
; 	Modifies:		None
; 	Processing:		Sets the SMBus slave address from BL.
smbSetSlaveAddr PROC NEAR
	push	ax
	push	bx

	mov	al, bl
	mov	bh, ICH_SMB_IOREG04
	call	smbIoRegWrite

	pop	bx
	pop	ax
	ret
smbSetSlaveAddr ENDP

;-------------------------------------------------------------------------
; 	smbSendCommand - tells the SMBus to send the packet
; 	Entry:  CL - command bits
; 	Exit:		None
; 	Modifies:		None
; 	Processing:		Tells the SMBus to send the packet.
smbSendCommand PROC NEAR
	push	ax
	push	bx

	and	cl, (111b SHL 2)	; only command bits
	mov	bh, ICH_SMB_IOREG02
	call	smbIoRegRead
	and	al, NOT(111b SHL 2)	; clear command bits
	or	al, SMB_START
	or	al, cl			; Add our command
	call	smbIoRegWrite

	pop	bx
	pop	ax
	ret
smbSendCommand ENDP
;-------------------------------------------------------------------------
; 	smbReadStatus - Reads the SMBus status register
; 	Entry:		None
; 	Exit:     DL	- Status flags
;	    STC - an error or interrupt occured
;	    CLC - nothing happened
; 	Modifies:		None
; 	Processing:
;		Reads the SMBus status register and sets the carry flag
;		if there is an error or interrupt.
smbReadStatus PROC NEAR
	push	ax
	push	bx

	mov	bh, ICH_SMB_IOREG00
	call	smbIoRegRead
	test	al, SMB_ERROR_INT_MASK	; did an error/interrupt occur?
	jnz	smbExitError		; yep
	clc
	jmp	smbExit

smbExitError:
	stc

smbExit:
	mov	dl, al
	pop	bx
	pop	ax
	ret
smbReadStatus ENDP

Delay5ms    proc  near
            push  cx

            mov   cx, 1000
      Delay5ms1:
            out	0ebh,al
            loop  short Delay5ms1

            pop   cx
            ret
Delay5ms    endp
;void Delay10Ms(void) 
_Delay10Ms  PROC NEAR
	push	bp
	mov	bp,sp
  PUSHA
  ;MOV  DX,04240H         ;1S ;CX:DX 规定延时的微秒数
  ;MOV  CX,0FH
  MOV DX,02710H         ;10MS  ;CX:DX 规定延时的微秒数
  MOV CX,0H
  MOV AH,86H
  INT 15H
  POPA
  ;
  pop	bp
	ret	
_Delay10Ms  ENDP
;****************************************************;
;*子程序名：	ASCTOHEX															**
;*功能：			ASCII码转换为十六进制									**
;*入口参数：	AL=所需转换字符												**
;*出口参数：	AL=转换字符														**
;****************************************************;	
;unsigned char ASC2HEX(unsigned char ch);
_ASC2HEX	PROC	NEAR
	push	bp
	mov	bp,sp
	mov	al,[bp+4]
	OR	AL,20H						;大写转小写
	SUB	AL,'0'
	CMP	AL,9
	JBE	_ATHOVER
	SUB	AL,'a'-'0'-10
_ATHOVER:	
		pop	bp
		RET
_ASC2HEX	ENDP
end