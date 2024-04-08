.model small
.386
.data
.code
;为smbus准备底层的I/O访问例程
public _READ_PORT_UCHAR			;read byte form specified register
public _WRITE_PORT_UCHAR    ;write byte to specified register
public _READ_PORT_ULONG			;read 4 bytes form specified register
public _WRITE_PORT_ULONG    ;write 4 bytes to specified register
public _Delay5ms						;delay time function
;	unsigned char READ_PORT_UCHAR(unsigned int port)
_READ_PORT_UCHAR	proc	near
	push	bp
	mov	bp,sp

	mov	 dx,[bp+4]			;port fill
	in	  al,dx
	out		0ebh,al
	out		0ebh,al				

	mov	sp,bp						;return value is in al
	pop	bp
	ret	
_READ_PORT_UCHAR	endp

;	void WRITE_PORT_UCHAR(unsigned int port,unsigned char value)
_WRITE_PORT_UCHAR	proc	near
	push	bp
	mov	bp,sp
	;
	mov		dx,[bp+4]  
	mov		al,[bp+6]
	out	 dx,al
	out		0ebh,al
  out		0ebh,al
   ;	
	pop	bp
	ret	
_WRITE_PORT_UCHAR	endp

;	unsigned long READ_PORT_ULONG(unsigned int port)
;[bp+4]->port    dx:ax->return unsigned long
_READ_PORT_ULONG	proc	near
	push	bp
	mov	bp,sp
	;
	mov	dx,word ptr [bp+4]
	in eax,dx
	out 0ebh,al
	out 0ebh,al
	
	mov	edx,eax
	shr edx,16			;dx:ax=return value
	;
	mov	sp,bp
	pop	bp
	ret	
_READ_PORT_ULONG	endp

;	void WRITE_PORT_ULONG(unsigned int port,unsigned long value)
;[bp+4]->port  [bp+6]:[bp+8]->value(example:dx-<[bp+6],ax<-[bp+8])
_WRITE_PORT_ULONG	proc	near
	push	bp
	mov	bp,sp
	;
	mov	dx,word ptr [bp+4]
	mov	eax,dword ptr [bp+6]
	out dx,eax
	out 0ebh,al
	out 0ebh,al
	;
	mov	sp,bp
	pop	bp
	ret	
_WRITE_PORT_ULONG	endp

;void Delay5ms(void);
_Delay5ms    proc  near
            push  cx

            mov   cx, 1000
      Delay5ms1:
            out	0ebh,al
            loop  short Delay5ms1

            pop   cx
            ret
_Delay5ms    endp
end