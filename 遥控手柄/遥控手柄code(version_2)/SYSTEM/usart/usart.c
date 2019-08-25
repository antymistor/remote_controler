#include "sys.h"
#include "usart.h"	  

#include "act.h"
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���         
#define ifacceptcapitalinstruction  1 //�趨�Ƿ�Ҫ���ܴ�дָ�����ָ��Ŀ��������ָ�
 
#define IDt 'A'
#define IDr d
u8 cnum=0;
u8 dectr[6]={0};

u8 tempstr2[15];
int speed;



struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 

void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)                	//??1??????
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
		Res =USART_ReceiveData(USART1);	
		#if ifacceptcapitalinstruction
			  if ( ((Res<='c')&&(Res>='a'))|| ((Res<='Z')&&(Res>='A')))
		#else
		    if ( (Res<='c')&&(Res>='a') )
		#endif
		{
			cnum=1;
			dectr[0]=Res;
		}
		else if(cnum>0)
		{
			if ( ( (Res>='0')&& (Res<='9') )  )
			{dectr[cnum]=Res;
			cnum++;
			}
			else
			cnum=0;
		}
		
		if(cnum==5)
		{
			cnum=0;
			if(dectr[0]=='c')
			{
				speed=(dectr[1]-'0')*1000+(dectr[2]-'0')*100+(dectr[3]-'0')*10+(dectr[4]-'0')-5000;
				sprintf(tempstr2,"%s%d%s","Coverd: ",speed,"mm");
			  writefeedback(tempstr2);}
//			if(dectr[0]=='A')
//			{
//				if(dectr[1]=='0')
//				{
//				speed=(dectr[2]-'0')*10+(dectr[3]-'0');
//				sprintf(tempstr2,"%s%d%s","UP_P:",speed,"%");
//			  writefeedback(tempstr2);}
//				else if(dectr[1]=='1')
//				{
//				speed=(dectr[2]-'0')*10+(dectr[3]-'0');
//				sprintf(tempstr2,"%s%d%s","DOWN_P:",speed,"%");
//			  writefeedback(tempstr2);
//				}
//			}
			
		}
	}
		
} 



