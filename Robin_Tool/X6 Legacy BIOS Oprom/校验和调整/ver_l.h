/*�Ͼ���˼���з��� �汾�����ļ� ver_m.h��Ϊ�ײ�C�������*/
/*2008-8-13 12:59  luobing*/

#ifndef __Ver_L_H__
#define __Ver_L_H__
//�����Ҫ�޸İ汾��Ϣ����ֱ���޸ĺ궨�壬��Ҫȥ�Ķ�����ı���
#define MainVer     0x01                          //���汾��(0~3)
#define SubVer      0x01                          //�ΰ汾��(0~63)
#define CodeType    0x0F                          //��Ʒ���������(0x0:�������� 0x1:OEM�ص� 0xF:ESC�ڲ�����)
#define ReleaseVerH 0x01                          //�����İ汾�ŵĸ�λ(0~15)
#define ReleaseVerL 0x02                          //�����İ汾�ŵĵ�λ(0~255)
#define OEM_ID      0x00                          //(0~65535)OEM����ID�������OEM�ص��Ĵ��룬��ID��Ч��������Ч
#define PString     "Chks.exe"       //��Ʒ�����ַ���


/*----------------------------------����汾���� begin-------------------------*/
/*Ϊ��֤���������޹��ԣ�����Ϊ���µ�������ʽ*/
/*��һ������Ϊ��־�����ø���λ�ü�����*/
/*�ڶ����������������Ϊ: ��־���ڲ��汾�š�ExtVer1�ĸ��ֽڡ�ExtVer1�ĵ��ֽڡ�ExtVer2*/
/*
  MyVersion[12]='2': �ײ�����̶���־����Ҫ����
    MyVersion[13]=InsideVer:  �з����ڲ��汾�ţ�����λΪ���汾�ţ�����λΪ�ΰ汾��
  MyVersion[14]=ExtVer1�ĸ��ֽ�: �����λΪ�������ͣ�����λ�ǳ���Release�汾�ĸ���λ
    �������ͣ�  0x00:�������룬��MyVersion[14]=0x0? (?����������)
                0x01:OEM�ص��� ��MyVersion[14]=0x1? (?����������)
                0x0F:ESC�ڲ����߼� ��MyVersion[14]=0xF? (?����������)
  MyVersion[15]=ExtVer1�ĵ��ֽ�: ����Release�汾�ĵͰ�λ
  MyVersion[16]=ExtVer2�ĸ��ֽ�--|
                                 |--���̱��:ֻ�е�OEM�ص�ʱ�˱�Ų�������
  MyVersion[17]=ExtVer2�ĵ��ֽ�--|
*/    
unsigned char MyVersion[]={'N','a','n','j','i','n','g',0x20,'E','S','C',0x00,  
                           '2',((MainVer<<6)+SubVer),((CodeType<<4)+(ReleaseVerH)),
                               ReleaseVerL,(OEM_ID>>8),(OEM_ID&0xff)};
unsigned char Prog_ver[]=PString;
/*----------------------------------����汾���� end-------------------------*/
#endif









