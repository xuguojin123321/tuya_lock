#include "sys.h"
#include "rc522.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "log.h"
//////////////////////////////////////////////////////////
// M1����Ϊ16��������ÿ���������ĸ��飨��0����1����2����3�����
// ��16��������64���鰴���Ե�ַ���Ϊ��0~63
// ��0�������Ŀ�0�������Ե�ַ0�飩�����ڴ�ų��̴��룬�Ѿ��̻����ɸ���
// ÿ�������Ŀ�0����1����2Ϊ���ݿ飬�����ڴ������
// ÿ�������Ŀ�3Ϊ���ƿ飨���Ե�ַΪ:��3����7����11.....����������A����ȡ���ơ�����B��

/*******************************
*����˵����
*1--SDA  <----->PA4
*2--SCK  <----->PA5
*3--MOSI <----->PA6
*4--MISO <----->PA7
*5--����
*6--GND <----->GND
*7--RST <----->PB0
*8--VCC <----->VCC
************************************/

extern unsigned char my_card_flag;   //�ж��Ƿ����Լ��Ŀ�
extern unsigned char my_card[4];
extern unsigned char my_card1[4];			  	//��1
extern unsigned char my_card2[4];				//��2
extern unsigned char serch_card_flag; //�ж��Ƿ�ˢ���� 
/*ȫ�ֱ���*/
unsigned char CT[2];//������
unsigned char SN[4]; //����
unsigned char RFID[16];			//���RFID
unsigned char lxl_bit=0;
unsigned char card1_bit=0;  //�Ƿ�ʶ�𵽿�1��־λ
unsigned char card2_bit=0;
unsigned char card3_bit=0;
unsigned char card4_bit=0;
unsigned char total=0;
unsigned char lxl[4]= {196,58,104,217};
unsigned char card_1[4]= {83,106,11,1};
unsigned char card_2[4]= {208,121,31,57};
unsigned char card_3[4]= {176,177,143,165};
unsigned char card_4[4]= {5,158,10,136};
u8 KEY[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
u8 AUDIO_OPEN[6] = {0xAA, 0x07, 0x02, 0x00, 0x09, 0xBC};
unsigned char RFID1[16]= {0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
/*��������*/
unsigned char status;
unsigned char s=0x08;

#define   RC522_DELAY()  delay_us( 20 )

//ִ������������ж��Ƿ��п�����ˢ�����ϣ����ҶԱȿ����Ƿ����Լ��Ŀ�
void RC522_Handel(void)
{
    status = PcdRequest(PICC_REQALL,CT);//Ѱ��
    if(status!=MI_OK)
    {
        status = PcdRequest(PICC_REQALL,CT);//�ٴ�Ѱ��
    }
    if(status==MI_OK)// Ѱ���ɹ�
    {
			
	   //serch_card_flag=1;//���ˢ��������־λ��1
        status=MI_ERR;
        status = PcdAnticoll(SN);// ����ײ
    }

    if (status==MI_OK)// ����ײ�ɹ�
    {
        status=MI_ERR;
		LOGD("card id is===== %02x%02x%02x%02x\r\n",SN[0],SN[1],SN[2],SN[3]);	
		/*if((SN[0]==my_card[0])&&(SN[1]==my_card[1])&&(SN[2]==my_card[2])&&(SN[3]==my_card[3]))
		{
			my_card_flag=1;//ʶ���Լ��Ŀ�����־λ��1						
		}
		else if((SN[0]==my_card1[0])&&(SN[1]==my_card1[1])&&(SN[2]==my_card1[2])&&(SN[3]==my_card1[3]))
		{
			my_card_flag=1;//ʶ���Լ��Ŀ�����־λ��1						
		}
		else if((SN[0]==my_card2[0])&&(SN[1]==my_card2[1])&&(SN[2]==my_card2[2])&&(SN[3]==my_card2[3]))
		{
			my_card_flag=1;//ʶ���Լ��Ŀ�����־λ��1						
		}*/
    }
}



static void RC522_SPI_Config ( void )
{
  /* SPI_InitTypeDef  SPI_InitStructure */
  GPIO_InitTypeDef GPIO_InitStructure;
  
	/*!< Configure SPI_RC522_SPI pins: CS */
   RC522_GPIO_CS_CLK_FUN ( RC522_GPIO_CS_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_CS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_CS_Mode;
  GPIO_Init(RC522_GPIO_CS_PORT, &GPIO_InitStructure);
	
  /*!< Configure SPI_RC522_SPI pins: SCK */
	RC522_GPIO_SCK_CLK_FUN ( RC522_GPIO_SCK_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_SCK_Mode;
  GPIO_Init(RC522_GPIO_SCK_PORT, &GPIO_InitStructure);
	
  /*!< Configure SPI_RC522_SPI pins: MOSI */
	RC522_GPIO_MOSI_CLK_FUN ( RC522_GPIO_MOSI_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_MOSI_Mode;
  GPIO_Init(RC522_GPIO_MOSI_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_RC522_SPI pins: MISO */
	RC522_GPIO_MISO_CLK_FUN ( RC522_GPIO_MISO_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_MISO_Mode;
  GPIO_Init(RC522_GPIO_MISO_PORT, &GPIO_InitStructure);	
		
  /*!< Configure SPI_RC522_SPI pins: RST */
	RC522_GPIO_RST_CLK_FUN ( RC522_GPIO_RST_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_RST_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_RST_Mode;
  GPIO_Init(RC522_GPIO_RST_PORT, &GPIO_InitStructure);
	
}
void RC522_Init ( void )
{
	RC522_SPI_Config ();	
	RC522_Reset_Disable();	
	RC522_CS_Disable();
}
/*
 * ��������SPI_RC522_SendByte
 * ����  ����RC522����1 Byte ����
 * ����  ��byte��Ҫ���͵�����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����
 */
void SPI_RC522_SendByte ( uint8_t byte )
{
  uint8_t counter;

  for(counter=0;counter<8;counter++)
  {     
    if ( byte & 0x80 )
      RC522_MOSI_1 ();
    else 
      RC522_MOSI_0 ();
    
    RC522_DELAY();
    RC522_SCK_0 ();
    
    RC522_DELAY();
    RC522_SCK_1();
    
    RC522_DELAY();
    byte <<= 1; 
  } 	
}


/*
 * ��������SPI_RC522_ReadByte
 * ����  ����RC522����1 Byte ����
 * ����  ����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����
 */
uint8_t SPI_RC522_ReadByte ( void )
{
  uint8_t counter;
  uint8_t SPI_Data;

  for(counter=0;counter<8;counter++)
  {
    SPI_Data <<= 1;
    RC522_SCK_0 ();
   
    RC522_DELAY();
    if ( RC522_MISO_GET() == 1)
     SPI_Data |= 0x01;
    
    RC522_DELAY();
    RC522_SCK_1 ();
    
    RC522_DELAY();
  }
  return SPI_Data;
	
}


/*
 * ��������ReadRawRC
 * ����  ����RC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 * ����  : �Ĵ����ĵ�ǰֵ
 * ����  ���ڲ�����
 */
uint8_t ReadRawRC ( uint8_t ucAddress )
{
	uint8_t ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;	
	RC522_CS_Enable();
  
	SPI_RC522_SendByte ( ucAddr );
	ucReturn = SPI_RC522_ReadByte ();
  
	RC522_CS_Disable();
	
	return ucReturn;	
}



/*
 * ��������WriteRawRC
 * ����  ��дRC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 *         ucValue��д��Ĵ�����ֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void WriteRawRC ( uint8_t ucAddress, uint8_t ucValue )
{  
	uint8_t ucAddr;
	
	ucAddr = ( ucAddress << 1 ) & 0x7E;	
	RC522_CS_Enable();
	
	SPI_RC522_SendByte ( ucAddr );	
	SPI_RC522_SendByte ( ucValue );
  
	RC522_CS_Disable();		
}


/*
 * ��������SetBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void SetBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp | ucMask ); // set bit mask
}


/*
 * ��������ClearBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void ClearBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = ReadRawRC ( ucReg );
  WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); // clear bit mask
}


/*
 * ��������PcdAntennaOn
 * ����  ����������
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����
 */
void PcdAntennaOn ( void )
{
  uint8_t uc;

  uc = ReadRawRC ( TxControlReg );
  if ( ! ( uc & 0x03 ) )
   SetBitMask(TxControlReg, 0x03);		
}



/*
 * ��������PcdAntennaOff
 * ����  ����������
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����
 */
void PcdAntennaOff ( void )
{
  ClearBitMask ( TxControlReg, 0x03 );	
}


/*
 * ��������PcdRese
 * ����  ����λRC522
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����
 */
void PcdReset ( void )
{
	RC522_Reset_Disable();
	delay_us ( 1 );
	
	RC522_Reset_Enable();
	delay_us ( 1 );
	
	RC522_Reset_Disable();
	delay_us ( 1 );
	
	WriteRawRC ( CommandReg, 0x0f );
	
	while ( ReadRawRC ( CommandReg ) & 0x10 );

	delay_us ( 1 );
  
	//���巢�ͺͽ��ճ���ģʽ ��Mifare��ͨѶ��CRC��ʼֵ0x6363
  WriteRawRC ( ModeReg, 0x3D );        
	
  WriteRawRC ( TReloadRegL, 30 );      //16λ��ʱ����λ    
	WriteRawRC ( TReloadRegH, 0 );			 //16λ��ʱ����λ
	
  WriteRawRC ( TModeReg, 0x8D );			 //�����ڲ���ʱ��������
	
  WriteRawRC ( TPrescalerReg, 0x3E );	 //���ö�ʱ����Ƶϵ��
	
	WriteRawRC ( TxAutoReg, 0x40 );			 //���Ʒ����ź�Ϊ100%ASK	
	
}

/*
 * ��������M500PcdConfigISOType
 * ����  ������RC522�Ĺ�����ʽ
 * ����  ��ucType��������ʽ
 * ����  : ��
 * ����  ���ⲿ����
 */
void M500PcdConfigISOType ( uint8_t ucType )
{
	if ( ucType == 'A')                     //ISO14443_A
  {
		ClearBitMask ( Status2Reg, 0x08 );
		
    WriteRawRC ( ModeReg, 0x3D );         //3F
		
		WriteRawRC ( RxSelReg, 0x86 );        //84
		
		WriteRawRC( RFCfgReg, 0x7F );         //4F
		
		WriteRawRC( TReloadRegL, 30 );        
		
		WriteRawRC ( TReloadRegH, 0 );
		
		WriteRawRC ( TModeReg, 0x8D );
		
		WriteRawRC ( TPrescalerReg, 0x3E );
		
		delay_us ( 2 );
		
		PcdAntennaOn ();//������
		
   }	 
}


/*
 * ��������PcdComMF522
 * ����  ��ͨ��RC522��ISO14443��ͨѶ
 * ����  ��ucCommand��RC522������
 *         pInData��ͨ��RC522���͵���Ƭ������
 *         ucInLenByte���������ݵ��ֽڳ���
 *         pOutData�����յ��Ŀ�Ƭ��������
 *         pOutLenBit���������ݵ�λ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ڲ�����
 */
char PcdComMF522 ( uint8_t ucCommand,
                   uint8_t * pInData, 
                   uint8_t ucInLenByte, 
                   uint8_t * pOutData,
                   uint32_t * pOutLenBit )		
{
  char cStatus = MI_ERR;
  uint8_t ucIrqEn   = 0x00;
  uint8_t ucWaitFor = 0x00;
  uint8_t ucLastBits;
  uint8_t ucN;
  uint32_t ul;

  switch ( ucCommand )
  {
     case PCD_AUTHENT:		  //Mifare��֤
        ucIrqEn   = 0x12;		//��������ж�����ErrIEn  ��������ж�IdleIEn
        ucWaitFor = 0x10;		//��֤Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ
        break;
     
     case PCD_TRANSCEIVE:		//���շ��� ���ͽ���
        ucIrqEn   = 0x77;		//����TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        ucWaitFor = 0x30;		//Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ�� �����жϱ�־λ
        break;
     
     default:
       break;     
  }
  //IRqInv��λ�ܽ�IRQ��Status1Reg��IRqλ��ֵ�෴ 
  WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );
  //Set1��λ����ʱ��CommIRqReg������λ����
  ClearBitMask ( ComIrqReg, 0x80 );	 
  //д��������
  WriteRawRC ( CommandReg, PCD_IDLE );		 
  
  //��λFlushBuffer����ڲ�FIFO�Ķ���дָ���Լ�ErrReg��BufferOvfl��־λ�����
  SetBitMask ( FIFOLevelReg, 0x80 );			

  for ( ul = 0; ul < ucInLenByte; ul ++ )
    WriteRawRC ( FIFODataReg, pInData [ ul ] ); //д���ݽ�FIFOdata
    
  WriteRawRC ( CommandReg, ucCommand );					//д����


  if ( ucCommand == PCD_TRANSCEIVE )
    
    //StartSend��λ�������ݷ��� ��λ���շ�����ʹ��ʱ����Ч
    SetBitMask(BitFramingReg,0x80);  				  

  ul = 1000;                             //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms

  do 														         //��֤ ��Ѱ���ȴ�ʱ��	
  {
       ucN = ReadRawRC ( ComIrqReg );		 //��ѯ�¼��ж�
       ul --;
  } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );	

  ClearBitMask ( BitFramingReg, 0x80 );	 //��������StartSendλ

  if ( ul != 0 )
  {
    //�������־�Ĵ���BufferOfI CollErr ParityErr ProtocolErr
    if ( ! ( ReadRawRC ( ErrorReg ) & 0x1B ) )	
    {
      cStatus = MI_OK;
      
      if ( ucN & ucIrqEn & 0x01 )				//�Ƿ�����ʱ���ж�
        cStatus = MI_NOTAGERR;   
        
      if ( ucCommand == PCD_TRANSCEIVE )
      {
        //��FIFO�б�����ֽ���
        ucN = ReadRawRC ( FIFOLevelReg );		          
        
        //�����յ����ֽڵ���Чλ��
        ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	
        
        if ( ucLastBits )
          
          //N���ֽ�����ȥ1�����һ���ֽڣ�+���һλ��λ�� ��ȡ����������λ��
          * pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	
        else
          * pOutLenBit = ucN * 8;      //�����յ����ֽ������ֽ���Ч
        
        if ( ucN == 0 )		
          ucN = 1;    
        
        if ( ucN > MAXRLEN )
          ucN = MAXRLEN;   
        
        for ( ul = 0; ul < ucN; ul ++ )
          pOutData [ ul ] = ReadRawRC ( FIFODataReg );   
        
        }        
    }   
    else
      cStatus = MI_ERR;       
  }

  SetBitMask ( ControlReg, 0x80 );           // stop timer now
  WriteRawRC ( CommandReg, PCD_IDLE ); 
   
  return cStatus;
}


/*
 * ��������PcdRequest
 * ����  ��Ѱ��
 * ����  ��ucReq_code��Ѱ����ʽ
 *                     = 0x52��Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 *                     = 0x26��Ѱδ��������״̬�Ŀ�
 *         pTagType����Ƭ���ʹ���
 *                   = 0x4400��Mifare_UltraLight
 *                   = 0x0400��Mifare_One(S50)
 *                   = 0x0200��Mifare_One(S70)
 *                   = 0x0800��Mifare_Pro(X))
 *                   = 0x4403��Mifare_DESFire
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdRequest ( uint8_t ucReq_code, uint8_t * pTagType )
{
  char cStatus;  
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t ulLen;

  //����ָʾMIFARECyptol��Ԫ��ͨ�Լ����п�������ͨ�ű����ܵ����
  ClearBitMask ( Status2Reg, 0x08 );
	//���͵����һ���ֽڵ� ��λ
  WriteRawRC ( BitFramingReg, 0x07 );
  //TX1,TX2�ܽŵ�����źŴ��ݾ����͵��Ƶ�13.56�������ز��ź�
  SetBitMask ( TxControlReg, 0x03 );	

  ucComMF522Buf [ 0 ] = ucReq_code;		//���� ��Ƭ������

  cStatus = PcdComMF522 ( PCD_TRANSCEIVE,	
                          ucComMF522Buf,
                          1, 
                          ucComMF522Buf,
                          & ulLen );	//Ѱ��  

  if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	//Ѱ���ɹ����ؿ����� 
  {    
     * pTagType = ucComMF522Buf [ 0 ];
     * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
  }

  else
   cStatus = MI_ERR;

  return cStatus;	 
}


/*
 * ��������PcdAnticoll
 * ����  ������ײ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdAnticoll ( uint8_t * pSnr )
{
  char cStatus;
  uint8_t uc, ucSnr_check = 0;
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t ulLen;
  
  //��MFCryptol Onλ ֻ�гɹ�ִ��MFAuthent����󣬸�λ������λ
  ClearBitMask ( Status2Reg, 0x08 );
  //����Ĵ��� ֹͣ�շ�
  WriteRawRC ( BitFramingReg, 0x00);	
	//��ValuesAfterColl���н��յ�λ�ڳ�ͻ�����
  ClearBitMask ( CollReg, 0x80 );			  
 
  ucComMF522Buf [ 0 ] = 0x93;	          //��Ƭ����ͻ����
  ucComMF522Buf [ 1 ] = 0x20;
 
  cStatus = PcdComMF522 ( PCD_TRANSCEIVE, 
                          ucComMF522Buf,
                          2, 
                          ucComMF522Buf,
                          & ulLen);      //�뿨Ƭͨ��

  if ( cStatus == MI_OK)		            //ͨ�ųɹ�
  {
    for ( uc = 0; uc < 4; uc ++ )
    {
       * ( pSnr + uc )  = ucComMF522Buf [ uc ]; //����UID
       ucSnr_check ^= ucComMF522Buf [ uc ];
    }
    
    if ( ucSnr_check != ucComMF522Buf [ uc ] )
      cStatus = MI_ERR;    				 
  }
  
  SetBitMask ( CollReg, 0x80 );
      
  return cStatus;		
}


/*
 * ��������CalulateCRC
 * ����  ����RC522����CRC16
 * ����  ��pIndata������CRC16������
 *         ucLen������CRC16�������ֽڳ���
 *         pOutData����ż�������ŵ��׵�ַ
 * ����  : ��
 * ����  ���ڲ�����
 */
void CalulateCRC ( uint8_t * pIndata, 
                 uint8_t ucLen, 
                 uint8_t * pOutData )
{
  uint8_t uc, ucN;

  ClearBitMask(DivIrqReg,0x04);

  WriteRawRC(CommandReg,PCD_IDLE);

  SetBitMask(FIFOLevelReg,0x80);

  for ( uc = 0; uc < ucLen; uc ++)
    WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   

  WriteRawRC ( CommandReg, PCD_CALCCRC );

  uc = 0xFF;

  do 
  {
      ucN = ReadRawRC ( DivIrqReg );
      uc --;
  } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
  
  pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
  pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );		
}


/*
 * ��������PcdSelect
 * ����  ��ѡ����Ƭ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdSelect ( uint8_t * pSnr )
{
  char ucN;
  uint8_t uc;
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t  ulLen;
  
  
  ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
  ucComMF522Buf [ 1 ] = 0x70;
  ucComMF522Buf [ 6 ] = 0;

  for ( uc = 0; uc < 4; uc ++ )
  {
    ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
  }
  
  CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );

  ClearBitMask ( Status2Reg, 0x08 );

  ucN = PcdComMF522 ( PCD_TRANSCEIVE,
                     ucComMF522Buf,
                     9,
                     ucComMF522Buf, 
                     & ulLen );
  
  if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
    ucN = MI_OK;  
  else
    ucN = MI_ERR;    
  
  return ucN;		
}

