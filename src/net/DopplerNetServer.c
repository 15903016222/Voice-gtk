//      DopplerNet.c
//      
//      Copyright 2013 WolfLord <wolflord@wolflord-desktop>


#include "DopplerNetServer.h"
#include "NetResponseFunc.h"
#include <glib.h>

//*****************************************************************
//************    TCP    TCP   TCP
//*****************************************************************

int TcpServerSocketID;//服务器端套接字
int TcpClientSocketID;//客户端套接字


void ServerTcpListenToNet()
{
	/*监听连接请求--监听队列长度为5*/
	listen(TcpServerSocketID,1);
}

int ServerTcpSocketInit()
{
	int i= 0 ; int port;
	struct sockaddr_in TcpLocalAddr;   //服务器网络地址结构体
loop:
	memset(&TcpLocalAddr,0,sizeof(TcpLocalAddr)); //数据初始化--清零
	TcpLocalAddr.sin_family=AF_INET; //设置为IP通信
	TcpLocalAddr.sin_addr.s_addr=INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	port = TCP_PORT + i ;
	TcpLocalAddr.sin_port=htons(port); //服务器端口号
	
	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((TcpServerSocketID=socket(PF_INET,SOCK_STREAM,0))<0)
	{  
		i++ ;
		if(i < 10)  goto loop ;

		perror("socket");
		return 1;
	}
	
	        /*将套接字绑定到服务器的网络地址上*/
	if (bind(TcpServerSocketID,(struct sockaddr *)&TcpLocalAddr,sizeof(struct sockaddr))<0)
	{
		i++ ;
		if(i < 10)  goto loop ;
		perror("bind");
		return 1;
	}
	
	g_debug("PORT %d",port);
	ServerTcpListenToNet();
	
	return 0 ;
}

void ServerTcpCloseClientSocket()
{
    close(TcpClientSocketID);
}

void ServerTcpCloseServerSocket()
{
	close(TcpServerSocketID);
}

struct sockaddr_in TcpRemoteAddr; //客户端网络地址结构体
int ServerTcpAcceptConnect()
{
	socklen_t sin_size;
	sin_size=(socklen_t)sizeof(struct sockaddr_in);
	g_debug("waiting for connect");
	/*等待客户端连接请求到达*/
	if((TcpClientSocketID=accept(TcpServerSocketID,(struct sockaddr*)&TcpRemoteAddr,&sin_size))<0)
	{
		perror("accept");
		return 1;
	}
	else
	{
		g_debug("accept client %s",inet_ntoa(TcpRemoteAddr.sin_addr));
		ServerTcpSendToClient("Welcome to Doppler server", strlen("Welcome to Doppler server") + 1) ;
	}
	return 0;
}

int ServerTcpReadFromeClient(char* pBuf_ , int nSize_)
{
	int ret = 0 ;
	ret = recv(TcpClientSocketID, pBuf_ , nSize_, 0) ;
	return (ret ? 0 : 1) ;
}

int ServerTcpSendToClient(char* pBuf_ , int nSize_)
{
	int ret = 0 ;
	ret = send(TcpClientSocketID,  pBuf_,  nSize_, 0) ;
	if(ret ==  nSize_)  return 0 ;
	return 1;
}

//##########################################################################
//##########################################################################
//*****************************************************************
//************    UDP    UDP   UDP
//*****************************************************************
int    UdpClientSocketID;
struct sockaddr_in UdpRemoteAddr; //服务器端网络地址结构体
    
int InitUdpClient(char* strIP)
{
	memset(&UdpRemoteAddr,0,sizeof(UdpRemoteAddr)); //数据初始化--清零
	UdpRemoteAddr.sin_family=AF_INET; //设置为IP通信
	UdpRemoteAddr.sin_addr.s_addr=inet_addr(strIP);//服务器IP地址
	UdpRemoteAddr.sin_port=htons(UDP_PORT); //服务器端口号
	
	/*创建客户端套接字--IPv4协议，面向无连接通信，UDP协议*/
	if((UdpClientSocketID=socket(PF_INET,SOCK_DGRAM,0))<0)
	{  
		perror("socket");
		return 1;
	}
	return 0;
}

int UdpSendToServer(char* pBuf_ , long int nSize_)
{
	int ret = 0;
	socklen_t _nLength = sizeof(struct sockaddr) ;
	ret = sendto(UdpClientSocketID, pBuf_ , nSize_ ,0 , (struct sockaddr *)&UdpRemoteAddr, _nLength);
	if(ret == nSize_)	return 0;
	return -1 ;
}

#define UDP_BLOCK_SIZE_8K  (8*1024)

int UdpSendBlocksToServer(char* pBuf_ , long int nSize_)
{
	int ret = 0;
	int _nBlocks ;
	static int _nTimes = 0 ;
	g_debug("UdpSendBlocksToServer [%05d][%06ld]" , _nTimes++ , nSize_);

	if(nSize_ % UDP_BLOCK_SIZE_8K)
		_nBlocks = nSize_ / UDP_BLOCK_SIZE_8K  + 1;
	else
		_nBlocks = nSize_ / UDP_BLOCK_SIZE_8K ;

	int i  ; char* _pTmp ;

	_pTmp = pBuf_;
	_pTmp[0] = _nBlocks ;_pTmp[1] = 0 ;
	ret = UdpSendToServer(_pTmp , UDP_BLOCK_SIZE_8K + 1)  ;
	if(ret) return ret ;

	for(i = 1 ; i< _nBlocks ; i++)
	{
		_pTmp = pBuf_ + i * UDP_BLOCK_SIZE_8K ;
		//_pTmp[0] = i  ;
		ret = UdpSendToServer(_pTmp , UDP_BLOCK_SIZE_8K)  ;
		if(ret) return ret ;
	}


	return ret ;
}

void UdpCloseClient()
{
	close(UdpClientSocketID);
}

//#################################################################
//#################################################################
//*****************************************************************
//*****************************************************************

//Reset the CPU.
//Parameter: None.
//Return   : None.
int setup_CMD_NONE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_NONE"  ;
	g_debug("%s" , mes);
	Response_CMD_NONE(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}


int setup_CMD_RESET_ALL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_RESET_ALL"  ;
	g_debug("%s" , mes);
	Response_CMD_RESET_ALL(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

int setup_CMD_RESET_ENCODER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_RESET_ENCODER"  ;
	g_debug("%s" , mes);
	Response_CMD_RESET_ENCODER(Command, ParamSize, Parameter) ;

	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

int setup_CMD_SET_CURRENT_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_SET_CURRENT_GROUP"  ;
	g_debug("%s" , mes);
	
	Response_CMD_SET_CURRENT_GROUP(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1)    ;

	return err;
}


int setup_CMD_ADD_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_ADD_GROUP"  ;
	g_debug("%s" , mes);
	Response_CMD_ADD_GROUP(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err ;
}

int setup_CMD_DEL_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_DEL_GROUP"  ;
	g_debug("%s" , mes);
	Response_CMD_DEL_GROUP(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

int setup_CMD_SET_ENCODER_TYPE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_DEL_GROUP"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_ENCODER_TYPE(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

int setup_CMD_GET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_GET_CONFIG_SCAN"  ;
	g_debug("%s" , mes );
	int* _pData = (int*)Parameter ;
	CONFIG_SCAN _ScanInfor ;
	Response_CMD_GET_CONFIG_SCAN(Command, *_pData, (char*)&_ScanInfor) ;
	err  = ServerTcpSendToClient((void*)(&_ScanInfor) , sizeof(CONFIG_SCAN));

	return err;
}


int setup_CMD_SET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_SCAN"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_SCAN(Command, ParamSize , Parameter ) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

int setup_CMD_GET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  

	char* mes = "setup_CMD_GET_CONFIG_FOCALLAW"  ;

	int* _pData = (int*)Parameter ;
	g_debug("%s ---- %d" , mes , *_pData);
	CONFIG_FOCALLAW _LawInfo ;
	Response_CMD_GET_CONFIG_FOCALLAW(Command,  *_pData ,  (char*)(&_LawInfo) ) ;
	err  = ServerTcpSendToClient((void*)(&_LawInfo) , sizeof(CONFIG_FOCALLAW));
	return err;
}

int setup_CMD_SET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_FOCALLAW"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_FOCALLAW(Command,  ParamSize , Parameter ) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}


int setup_CMD_GET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_PROBE"  ;
	g_debug("%s" , mes);

	CONFIG_PROBE_INFO _ProbeInfo ;
	Response_CMD_GET_CONFIG_PROBE(Command, ParamSize , (char*)&_ProbeInfo ) ;
	err  = ServerTcpSendToClient((void*)(&_ProbeInfo) , sizeof(CONFIG_PROBE_INFO));

	return err;
}

//****************************************

int setup_CMD_SET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_PROBE"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_PROBE(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_WEDGE"  ;
	g_debug("%s" , mes);

	CONFIG_WEDGE_INFO _WedgeInfo ;
	Response_CMD_GET_CONFIG_WEDGE(Command, ParamSize , (char*)&_WedgeInfo ) ;
	err  = ServerTcpSendToClient((void*)(&_WedgeInfo) , sizeof(CONFIG_WEDGE_INFO));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_PROBE"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_WEDGE(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}



//****************************************
int setup_CMD_GET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_PART"  ;
	g_debug("%s" , mes);

	CONFIG_PART_INFO _PartInfo ;
	Response_CMD_GET_CONFIG_PART(Command, ParamSize , (char*)&_PartInfo ) ;
	err  = ServerTcpSendToClient((void*)(&_PartInfo) , sizeof(CONFIG_PART_INFO));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_PART"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_PART(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_UT_GENERAL"  ;
	g_debug("%s" , mes);

	CONFIG_UT_GENERAL _UtGeneral ;
	Response_CMD_GET_CONFIG_UT_GENERAL(Command, ParamSize , (char*)&_UtGeneral ) ;
	err  = ServerTcpSendToClient((void*)(&_UtGeneral) , sizeof(CONFIG_UT_GENERAL));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_UT_GENERAL"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_UT_GENERAL(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_UT_PULSER"  ;
	g_debug("%s" , mes);

	CONFIG_UT_PULSER _UtPulser;
	Response_CMD_GET_CONFIG_UT_PULSER(Command, ParamSize , (char*)&_UtPulser ) ;
	err  = ServerTcpSendToClient((void*)(&_UtPulser) , sizeof(CONFIG_UT_PULSER));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_UT_PULSER"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_UT_PULSER(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_UT_RECEIVER"  ;
	g_debug("%s" , mes);

	CONFIG_UT_RECEIVER _UtReceiver;
	Response_CMD_GET_CONFIG_UT_RECEIVER(Command, ParamSize , (char*)&_UtReceiver ) ;
	err  = ServerTcpSendToClient((void*)(&_UtReceiver) , sizeof(CONFIG_UT_RECEIVER));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_UT_RECEIVER"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_UT_RECEIVER(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_UT_ADVANCED"  ;
	g_debug("%s" , mes);

	CONFIG_UT_ADVANCED _UtAdvanced;
	Response_CMD_GET_CONFIG_UT_ADVANCED(Command, ParamSize , (char*)&_UtAdvanced ) ;
	err  = ServerTcpSendToClient((void*)(&_UtAdvanced) , sizeof(CONFIG_UT_ADVANCED));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_UT_ADVANCED"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_UT_ADVANCED(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}
//****************************************
int setup_CMD_GET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_GATE_INFO"  ;
	g_debug("%s" , mes);
	int* _pGateId  = (int*)Parameter;
	CONFIG_GATE_INFO _GateInfo;
	Response_CMD_GET_CONFIG_GATE_INFO(Command, *_pGateId , (char*)&_GateInfo ) ;
	err  = ServerTcpSendToClient((void*)(&_GateInfo) , sizeof(CONFIG_GATE_INFO));

	return err;
}

//****************************************
int setup_CMD_SET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_GATE_INFO"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_GATE_INFO(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}

//****************************************
int setup_CMD_GET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	char* mes = "setup_CMD_GET_CONFIG_DISPLAY_INFO"  ;
	g_debug("%s" , mes);

	CONFIG_DISPLAY_INFO _DisplayInfo;
	Response_CMD_GET_CONFIG_DISPLAY_INFO(Command, ParamSize , (char*)&_DisplayInfo ) ;
	err  = ServerTcpSendToClient((void*)(&_DisplayInfo) , sizeof(CONFIG_DISPLAY_INFO));

	return err;
}


//****************************************
int setup_CMD_SET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;  
	
	char* mes = "setup_CMD_SET_CONFIG_DISPLAY_INFO"  ;
	g_debug("%s" , mes);
	Response_CMD_SET_CONFIG_DISPLAY_INFO(Command, ParamSize , Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);
	return err;
}


//****************************************
int setup_CMD_GET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_GROUP_BEAMDELAY"  ;
	g_debug("%s" , mes);

	CONFIG_GROUP_BEAMDELAY _BeamDelay;
	Response_CMD_GET_GROUP_BEAMDELAY(Command, ParamSize , (char*)&_BeamDelay ) ;
	err  = ServerTcpSendToClient((void*)(&_BeamDelay) , sizeof(CONFIG_GROUP_BEAMDELAY));

	return err;
}

//****************************************
int setup_CMD_SET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_GROUP_BEAMDELAY"  ;

	Response_CMD_SET_GROUP_BEAMDELAY(Command, ParamSize, Parameter) ;


	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}
//****************************************
int setup_CMD_GET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_GROUP_BEAMDELAY"  ;
	g_debug("%s" , mes);

	CONFIG_GROUP_GAINOFFSET _GainOffset;
	Response_CMD_GET_GROUP_GAINOFFSET(Command, ParamSize , (char*)&_GainOffset ) ;
	err  = ServerTcpSendToClient((void*)(&_GainOffset) , sizeof(CONFIG_GROUP_GAINOFFSET));

	return err;
}
//****************************************
int setup_CMD_SET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_GROUP_GAINOFFSET"  ;
	Response_CMD_SET_GROUP_GAINOFFSET(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}
//****************************************
int setup_CMD_GET_VAL_SIZINECURVE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_SIZINECURVE"  ;
	g_debug("%s" , mes);

	CONFIG_SIZING_CURVE _curve;
	Response_CMD_GET_SIZINE_CURVE(Command, ParamSize , (char*)&_curve ) ;
	err  = ServerTcpSendToClient((void*)(&_curve) , sizeof(CONFIG_SIZING_CURVE));

	return err;
}
//****************************************
int setup_CMD_SET_VAL_SIZINECURVE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_SIZINECURVE"  ;
	Response_CMD_SET_SIZINE_CURVE(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}

//****************************************
int setup_CMD_ENABLE_DATA_TRANSFER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_ENABLE_DATA_TRANSFER"  ;
	int* pVal = (int*)Parameter ;
	if(*pVal)
	{
		UdpCloseClient();
		InitUdpClient(inet_ntoa(TcpRemoteAddr.sin_addr))  ;
	}

	Response_CMD_ENABLE_DATA_TRANSFER(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}
//###########################################
//###########################################
//###########################################
//###########################################
int setup_CMD_GET_VAL_GROUP_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_GET_VAL_GROUP_QTY"  ;
	int _nGroupId = 0 ;
	Response_CMD_GET_VAL_GROUP_QTY(Command, ParamSize, (char*)&_nGroupId) ;
	err  = ServerTcpSendToClient((void*)&_nGroupId , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_TOTAL_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_TOTAL_DATA_SIZE"  ;

	int _nDataSize ;
	Response_CMD_GET_VAL_TOTAL_DATA_SIZE(Command, ParamSize, (char*)&_nDataSize) ;
	err  = ServerTcpSendToClient((void*)&_nDataSize , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_GROUP_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_GROUP_DATA_SIZE"  ;

	int _nDataSize ;
	Response_CMD_GET_VAL_GROUP_DATA_SIZE(Command, ParamSize, (char*)&_nDataSize) ;
	err  = ServerTcpSendToClient((void*)&_nDataSize , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_GROUP_DATA_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_GROUP_DATA_OFFSET"  ;

	int _nOffset ;  int* _pGroup  = (int*)Parameter;
	Response_CMD_GET_VAL_GROUP_DATA_OFFSET(Command, *_pGroup, (char*)&_nOffset) ;
	err  = ServerTcpSendToClient((void*)&_nOffset , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_GROUP_BEAM_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_GROUP_BEAM_OFFSET"  ;

	int _nBeamOff ;
	Response_CMD_GET_VAL_GROUP_BEAM_OFFSET(Command, ParamSize, (char*)&_nBeamOff) ;
	err  = ServerTcpSendToClient((void*)&_nBeamOff , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_GROUP_BEAM_LENGTH(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_GROUP_BEAM_LENGTH"  ;

	int _nBeamLength ;
	Response_CMD_GET_VAL_GROUP_BEAM_LENGTH(Command, ParamSize, (char*)&_nBeamLength) ;
	err  = ServerTcpSendToClient((void*)&_nBeamLength , sizeof(int));

	g_debug("%s" , mes);
	return err;
}


int setup_CMD_GET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_GAIN"  ;

	int _nGain ;
	Response_CMD_GET_VAL_GAIN(Command, ParamSize, (char*)&_nGain) ;
	err  = ServerTcpSendToClient((void*)&_nGain , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_SET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_GAIN"  ;
	Response_CMD_SET_VAL_GAIN(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_START"  ;

	int _nStart ;
	Response_CMD_GET_VAL_START(Command, ParamSize, (char*)&_nStart) ;
	err  = ServerTcpSendToClient((void*)&_nStart , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_SET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_START"  ;
	Response_CMD_SET_VAL_START(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}


int setup_CMD_GET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_RANGE"  ;

	int _nRange ;
	Response_CMD_GET_VAL_RANGE(Command, ParamSize, (char*)&_nRange) ;
	err  = ServerTcpSendToClient((void*)&_nRange , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_SET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_RANGE"  ;
	Response_CMD_SET_VAL_RANGE(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;
	char* mes = "setup_CMD_GET_VAL_POINT_QTY"  ;

	int _nPointQty ;
	Response_CMD_GET_VAL_POINT_QTY(Command, ParamSize, (char*)&_nPointQty) ;
	err  = ServerTcpSendToClient((void*)&_nPointQty , sizeof(int));

	g_debug("%s" , mes);
	return err;
}
int setup_CMD_SET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_POINT_QTY"  ;
	Response_CMD_SET_VAL_POINT_QTY(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)
{

	int err;

	char* mes = "setup_CMD_GET_VAL_VELOCITY"  ;

	int _nVelocity ;
	Response_CMD_GET_VAL_VELOCITY(Command, ParamSize, (char*)&_nVelocity) ;
	err  = ServerTcpSendToClient((void*)&_nVelocity , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_SET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_VELOCITY"  ;
	Response_CMD_SET_VAL_VELOCITY(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}


int setup_CMD_GET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_GET_VAL_WEDGEDELAY"  ;

	int _nWedgeDelay ;
	Response_CMD_GET_VAL_WEDGEDELAY(Command, ParamSize, (char*)&_nWedgeDelay) ;
	err  = ServerTcpSendToClient((void*)&_nWedgeDelay , sizeof(int));

	g_debug("%s" , mes);
	return err;
}
int setup_CMD_SET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_WEDGEDELAY"  ;
	Response_CMD_SET_VAL_WEDGEDELAY(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_GET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_GET_VAL_PRF"  ;

	int _nPrf ;
	Response_CMD_GET_VAL_PRF(Command, ParamSize, (char*)&_nPrf) ;
	err  = ServerTcpSendToClient((void*)&_nPrf , sizeof(int));

	g_debug("%s" , mes);
	return err;
}

int setup_CMD_SET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err;

	char* mes = "setup_CMD_SET_VAL_PRF"  ;
	Response_CMD_SET_VAL_PRF(Command, ParamSize, Parameter) ;
	err  = ServerTcpSendToClient((void*)mes , strlen(mes) + 1);

	g_debug("%s" , mes);
	return err;
}





//****************************************
int CommandTransferBlock1(USLONG Command, USLONG ParamSize, char* data)
{
	int ret = 1 ;
	switch(Command)
	{
	case CMD_NONE:
		ret = setup_CMD_NONE( Command, ParamSize,  data) ;
		break ;
	case CMD_RESET_ALL:
		ret = setup_CMD_RESET_ALL( Command, ParamSize,  data) ;
		break ;
	case CMD_RESET_ENCODER :
		ret = setup_CMD_RESET_ENCODER( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CURRENT_GROUP :
		ret = setup_CMD_SET_CURRENT_GROUP( Command, ParamSize,  data) ;
		break ;
	case CMD_ADD_GROUP :
		ret = setup_CMD_ADD_GROUP( Command, ParamSize,  data) ;
		break ;
	case CMD_DEL_GROUP :
		ret = setup_CMD_DEL_GROUP( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_ENCODER_TYPE :
		ret = setup_CMD_SET_ENCODER_TYPE(Command , ParamSize , data) ;
		break;
	case CMD_GET_CONFIG_SCAN :
		ret = setup_CMD_GET_CONFIG_SCAN( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_SCAN :
		ret = setup_CMD_SET_CONFIG_SCAN( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_FOCALLAW:
		ret = setup_CMD_GET_CONFIG_FOCALLAW( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_FOCALLAW:
		ret = setup_CMD_SET_CONFIG_FOCALLAW( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_PROBE:
		ret = setup_CMD_GET_CONFIG_PROBE( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_PROBE:
		ret = setup_CMD_SET_CONFIG_PROBE( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_WEDGE:
		ret = setup_CMD_GET_CONFIG_WEDGE( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_WEDGE:
		ret = setup_CMD_SET_CONFIG_WEDGE( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_PART:
		ret = setup_CMD_GET_CONFIG_PART( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_PART:
		ret = setup_CMD_SET_CONFIG_PART( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_UT_GENERAL:
		ret = setup_CMD_GET_CONFIG_UT_GENERAL( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_UT_GENERAL:
		ret = setup_CMD_SET_CONFIG_UT_GENERAL( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_UT_PULSER:
		ret = setup_CMD_GET_CONFIG_UT_PULSER( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_UT_PULSER:
		ret = setup_CMD_SET_CONFIG_UT_PULSER( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_UT_RECEIVER:
		ret = setup_CMD_GET_CONFIG_UT_RECEIVER( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_UT_RECEIVER:
		ret = setup_CMD_SET_CONFIG_UT_RECEIVER( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_UT_ADVANCED:
		ret = setup_CMD_GET_CONFIG_UT_ADVANCED( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_UT_ADVANCED:	
		ret = setup_CMD_SET_CONFIG_UT_ADVANCED( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_GATE_INFO:
		ret = setup_CMD_GET_CONFIG_GATE_INFO( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_GATE_INFO:
		ret = setup_CMD_SET_CONFIG_GATE_INFO( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_CONFIG_DISPLAY_INFO :
		ret = setup_CMD_GET_CONFIG_DISPLAY_INFO( Command, ParamSize,  data) ;
		break ;
	case CMD_SET_CONFIG_DISPLAY_INFO :
		ret = setup_CMD_SET_CONFIG_DISPLAY_INFO( Command, ParamSize,  data) ;
		break ;

	case CMD_GET_GROUP_BEAMDELAY:
		ret = setup_CMD_GET_GROUP_BEAMDELAY( Command, ParamSize,  data) ;
		break;
	case CMD_SET_GROUP_BEAMDELAY:
		ret = setup_CMD_SET_GROUP_BEAMDELAY( Command, ParamSize,  data) ;
		break;
	case CMD_GET_GROUP_GAINOFFSET:
		ret = setup_CMD_GET_GROUP_GAINOFFSET( Command, ParamSize,  data) ;
		break;
	case CMD_SET_GROUP_GAINOFFSET:
		ret = setup_CMD_SET_GROUP_GAINOFFSET( Command, ParamSize,  data) ;
		break;
    case CMD_GET_SIZINGCURVE   :
    	ret = setup_CMD_GET_VAL_SIZINECURVE( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_SIZINGCURVE   :
    	ret = setup_CMD_SET_VAL_SIZINECURVE( Command, ParamSize,  data) ;
    	break ;
	default:
		break ;
	}
	
	return ret ;
}

int CommandTransferBlock2(USLONG Command, USLONG ParamSize, char* data)
{
	int ret = 1 ;
	switch(Command)
	{
	//case CMD_MAX_MAX_MAX :
	case CMD_ENABLE_DATA_TRANSFER:
		ret = setup_CMD_ENABLE_DATA_TRANSFER( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_VAL_GROUP_QTY :
		ret = setup_CMD_GET_VAL_GROUP_QTY( Command, ParamSize,  data) ;
		break ;
    case CMD_GET_VAL_TOTAL_DATA_SIZE :
    	ret = setup_CMD_GET_VAL_TOTAL_DATA_SIZE( Command, ParamSize,  data) ;
    	break ;
	case CMD_GET_VAL_GROUP_DATA_SIZE :
		ret = setup_CMD_GET_VAL_GROUP_DATA_SIZE( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_VAL_GROUP_DATA_OFFSET :
		ret = setup_CMD_GET_VAL_GROUP_DATA_OFFSET( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_VAL_GROUP_BEAM_OFFSET :
		ret = setup_CMD_GET_VAL_GROUP_BEAM_OFFSET( Command, ParamSize,  data) ;
		break ;
	case CMD_GET_VAL_GROUP_BEAM_LENGTH :
		ret = setup_CMD_GET_VAL_GROUP_BEAM_LENGTH( Command, ParamSize,  data) ;
		break ;
    case CMD_GET_VAL_GAIN  :
    	ret = setup_CMD_GET_VAL_GAIN( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_GAIN  :
    	ret = setup_CMD_SET_VAL_GAIN( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_START :
    	ret = setup_CMD_GET_VAL_START( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_START :
    	ret = setup_CMD_SET_VAL_START( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_RANGE :
    	ret = setup_CMD_GET_VAL_RANGE( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_RANGE :
    	ret = setup_CMD_SET_VAL_RANGE( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_POINT_QTY :
    	ret = setup_CMD_GET_VAL_POINT_QTY( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_POINT_QTY :
    	ret = setup_CMD_SET_VAL_POINT_QTY( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_WEDGEDELAY :
    	ret = setup_CMD_GET_VAL_WEDGEDELAY( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_WEDGEDELAY :
    	ret = setup_CMD_SET_VAL_WEDGEDELAY( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_VELOCITY   :
    	ret = setup_CMD_GET_VAL_VELOCITY( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_VELOCITY   :
    	ret = setup_CMD_SET_VAL_VELOCITY( Command, ParamSize,  data) ;
    	break ;
    case CMD_GET_VAL_PRF   :
    	ret = setup_CMD_GET_VAL_PRF( Command, ParamSize,  data) ;
    	break ;
    case CMD_SET_VAL_PRF   :
    	ret = setup_CMD_SET_VAL_PRF( Command, ParamSize,  data) ;
    	break ;
    default:
    	break ;
	}

	return ret ;
}

int CommandTransfer(USLONG Command, USLONG ParamSize, char* data)
{
	int ret = 1 ;
	if (Command < CMD_MAX_MAX_MAX)
		ret = CommandTransferBlock1( Command, ParamSize,  data) ;
	else
		ret = CommandTransferBlock2( Command, ParamSize,  data) ;

	return ret ;
}

//**********************************************
//*****	if success 
//*****	return true 
//***** else return false
//**********************************************
int ServerTcpRespondToSocket()
{
	int err;
	char recvBuf[1024 * 256];
	memset(recvBuf , 'A' , 1024 * 256) ;

	USLONG Command;
	USLONG ParamSize;
	char* data;

	if(!TcpClientSocketID)  return 0;
	err = ServerTcpReadFromeClient(recvBuf,1024 * 256);
	if(err)  return 1;
	Command = (USLONG)*recvBuf;
	ParamSize=(USLONG)(*(recvBuf+4));
	data = recvBuf + 8 ;
	err  = CommandTransfer(Command, ParamSize, data);

	return err;
}


