/*
BSD 2-Clause License

Copyright (c) 2019, xujinpeng1117
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef CAN_ISO_TP_H
#define CAN_ISO_TP_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
//Support CAN_FD need more ram
#define SUPPORT_CAN_FD

#define OP_OK 0
#define OP_NOK 1
typedef struct can_iso_tp_link_t* can_iso_tp_link_t_p;
struct CAN_msg_id {
	uint32_t id:29;   //frame id
	uint32_t isExt:1;//0:std frame�� 1��ext frame
};
struct CAN_msg {
	struct CAN_msg_id id;
	uint8_t dlc;
#ifdef SUPPORT_CAN_FD
	uint8_t data[64];
#else
	uint8_t data[8];
#endif
};
typedef enum {
	N_OK
	, N_TIMEOUT_A
	, N_TIMEOUT_BS
	, N_TIMEOUT_CR
	, N_WRONG_SN
	, N_INVALID_FS
	, N_UNEXP_PDU
	, N_WFT_OVRN
	, N_BUFFER_OVFLW
	, N_ERROR
}CAN_ISO_TP_RESAULT;
struct can_iso_tp_init_t {
	void* usr_arg;  //used by user
	struct CAN_msg_id tx_id; //tx id when tx tp frame
	struct CAN_msg_id rx_id; //rx id when rx tp frame
	struct CAN_msg_id funtion_id; //tx id when tx tp function frame
	int (*L_Data_request)(can_iso_tp_link_t_p link, const struct CAN_msg* msg); //���ڵ��õײ���������CAN����
	void (*N_USData_indication)(can_iso_tp_link_t_p link, const uint8_t payload[], uint32_t size, CAN_ISO_TP_RESAULT error);//�������ϲ����㱨�յ�TP����
	void (*N_USData_confirm)(can_iso_tp_link_t_p link, const uint8_t payload[], uint32_t size, CAN_ISO_TP_RESAULT error);//�������ϲ����㱨TP���ݷ�����ɽ��
	uint8_t *rx_buff;  //���ڶ�֡����
	uint32_t rx_buff_len;  //���ڶ�֡����
	//��ʱ����
	uint16_t N_As;//���Ͷ˽����ݴ��͵����ն˵����ʱ��,Ĭ��1000
	uint16_t N_Ar;//���ն˽������ƴ��͵����Ͷ˵����ʱ��,Ĭ��1000
	uint16_t N_Bs;//���Ͷ��ڳɹ�������֡�󵽽��յ�����֡�����ʱ��,Ĭ��1000
	//uint16_t N_Br;//���ն��ڽ��յ���֡�󵽷��������Ƶ����ʱ��
	//uint16_t N_Cs;//���ն��ڷ��������Ƶ����ն˵����ʱ��
	uint16_t N_Cr;//���ն��ڷ��ͳɹ������ƺ��յ�����֡�����ʱ��,Ĭ��1000
	uint8_t N_WFTmax;//���͹�������ȴ�����
	uint8_t FC_BS;//���չ����з������ص�BS����
	uint8_t STmin;//���չ����з������ص�BS����
	uint8_t TX_DLC;//���ͱ��ĵ�DLCֵ
	uint8_t frame_pad;
	//��ѡ:��ӡ������Ϣ
	void(*print_debug)(const char *str);
};


/*
*********************************************************************************************************
*                                        ���������ͨ��
* Description : ���������ͨ��
* Arguments   : �����ͨ������
* Returns    : >=0 TPͨ����ţ���0��ʼ����
*              <0 ����ʧ��
*********************************************************************************************************
*/
int iso_can_tp_create(can_iso_tp_link_t_p link, struct can_iso_tp_init_t* init);

/*
*********************************************************************************************************
*                                        ѭ������CAN TPģ��
* Description : ѭ������CAN TPģ��,���ڱ��Ĵ�����ʱ����
* Arguments   : user_ms����ǰϵͳ���뼶ʱ��
* Returns    : ��
*********************************************************************************************************
*/
void iso_can_tp_poll(can_iso_tp_link_t_p link, unsigned int user_ms);

/*
*********************************************************************************************************
*                                        �յ�CAN����
* Description : �յ�CAN����
* Arguments   : channel: ͨ�����
*               CAN_msg���յ��ı���
* Returns    : 0:�յ�������ģ���Ӧͨ���й�
               ����:�յ�������ģ���Ӧͨ���޹�
*********************************************************************************************************
*/
int iso_can_tp_L_Data_indication(can_iso_tp_link_t_p link, const struct CAN_msg* msg);
/*
*********************************************************************************************************
*                                        ����CAN����
* Description : ����CAN����
* Arguments   : channel��ͨ�����
                CAN_msg�����͵ı���
* Returns    : 0���ɹ�������
*              ������������ʧ��
*********************************************************************************************************
*/
//can_iso_tp_link_t�ṹ����int(*L_Data_request)(can_iso_tp_link_t_p link, const struct CAN_msg* msg);
/*
*********************************************************************************************************
*                                        ����CAN�������ȷ��
* Description : ȷ�Ϸ���CAN�������
* Arguments   : channel��ͨ�����
				CAN_msg�����͵ı��ģ����ص���ָ�룬��ֻ��֤���ݺ�L_Data_request�ж�Ӧָ��ָ��������ͬ�����ñ�֤��ͬһָ��
				error:
					0: ���ͳɹ�
					����:����ʧ��
* Returns    : 0����ȷȷ��
*              �������ñ��Ĳ��Ǳ�ģ�����豨��
*********************************************************************************************************
*/
int iso_can_tp_L_Data_confirm(can_iso_tp_link_t_p link, const struct CAN_msg* msg, int8_t error);

/*
*********************************************************************************************************
*                                        �յ���������ݰ�
* Description : �յ���������ݰ�
* Arguments   : channel��ͨ�����
                payload���յ�������
*               size���յ������ݳ���
* Returns    : 0���ɹ�������
*              ������������ʧ��
*********************************************************************************************************
*/
//can_iso_tp_link_t�ṹ����void (*N_USData_indication)(can_iso_tp_link_t_p link,  const uint8_t payload[], uint32_t size);
/*
*********************************************************************************************************
*                                        ������·��������
* Description : ������·��������
* Arguments   : channel: ͨ�����
*               isFunction���Ƿ�Ϊ����Ѱַ��0���ǣ�������
				payload����������
				size���������ݳ���
* Returns    : 0������ɹ�
*               ����������ʧ��
*********************************************************************************************************
*/
int iso_can_tp_N_USData_request(can_iso_tp_link_t_p link, uint8_t isFunction, const uint8_t payload[], uint32_t size);

/*
*********************************************************************************************************
*                                        ��·�������������ȷ��
* Description : ��·�������������ȷ��
* Arguments   : channel: ͨ�����
*               isFunction����һ�������Ƿ�Ϊ����Ѱַ
				payload����������ָ�룬ָ����һ��iso_can_tp_N_USData_request�������������
				size����һ�����������ݳ���
				error���������ȷ�Ͻ��
					0����ȷ����
					���������ʹ���
* Returns    : 
*********************************************************************************************************
*/
//can_iso_tp_link_t�ṹ����void(*N_USData_confirm)(can_iso_tp_link_t_p link, const uint8_t payload[], uint32_t size, ISOTP_PROTOCOL_RESAULT error);

#ifdef __cplusplus
}
#endif

#endif
