#ifndef  ____DOPPLER_NET_SERVER_H_______
#define  ____DOPPLER_NET_SERVER_H_______

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define USLONG unsigned long

#define   TCP_PORT               8000
#define   UDP_PORT               6000
#define   IP_ADDR                "192.168.1.195"
//*************************** TCP
extern int  ServerTcpSocketInit() ;
extern void ServerTcpCloseClientSocket();
extern void ServerTcpCloseServerSocket();
extern int  ServerTcpAcceptConnect();
extern int  ServerTcpReadFromeClient(char* pBuf_ , int nSize_);
extern int  ServerTcpSendToClient(char* pBuf_ , int nSize_);
extern int  ServerTcpRespondToSocket() ;

//*************************** UDP
extern int InitUdpClient(char* pIP);
extern int UdpSendToServer(char* pBuf_ , long int nSize_);
extern void UdpCloseClient();
extern int UdpSendBlocksToServer(char* pBuf_ , long int nSize_) ;
//**************************************

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
typedef struct
{
	
	unsigned int uiStart;       // unit  : ns
	unsigned int uiRange;       // unit  : ns
	unsigned int uiWedgeDelay ;  // unit : ns
	__attribute__((aligned(8)))
    double       fGain ;        // range : 0 - 80.0 dB or 0 - 110.0 dB
	double       fVelocity   ;  // unit : m/s

} CONFIG_UT_GENERAL;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_TX_RX_MODE
{
	setup_TX_RX_MODE_PC = 0 ,
	setup_TX_RX_MODE_PE ,
	setup_TX_RX_MODE_TT ,
	setup_TX_RX_MODE_TOFD
};

enum setup_VOLTAGE_MODE
{
	setup_VOLTAGE_50  = 0 ,
	setup_VOLTAGE_100     ,
	setup_VOLTAGE_200 
} ;


typedef struct
{
	unsigned int uiPulserFir ;  // first trige element 1-128
	unsigned int eTRMode     ;  // setup_TX_RX_MODE
	unsigned int eVoltage    ;  // setup_VOLTAGE_MODE
	unsigned int uiPRF       ;  // pulser repeat frequency
	unsigned int uiPulserWidth ; // unit ns  (align with 2.5)
	__attribute__((aligned(8)))
	double fFequency  ;  // tirge frequency, unit: MHz
	
} CONFIG_UT_PULSER;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_FILTER_MODE
{
    setup_FILTER_NONE = 0 ,
    setup_FILTER_AUTO     ,
    setup_FILTER_1_MHZ    ,
    setup_FILTER_2_5MHZ   ,
    setup_FILTER_5_MHZ    ,
    setup_FILTER_7_5MHZ   ,
    setup_FILTER_10_MHZ
} ;

enum setup_RECTIFIER_MODE
{
    setup_RECTIFIER_RF = 0          ,
    setup_RECTIFIER_HW_POSITIVE     ,
    setup_RECTIFIER_HW_NEGTIVE      ,
    setup_RECTIFIER_FW              
} ;

enum setup_EVERAGE_MODE
{
    setup_EVERAGE_1 = 0  ,
    setup_EVERAGE_2      , 
    setup_EVERAGE_4      , 
    setup_EVERAGE_8      ,
    setup_EVERAGE_16 
} ;

typedef struct
{
	unsigned int uiReceiveFir   ;   // first receive element 1-128
	unsigned int eFilterMode    ;	// setup_FILTER_MODE
	unsigned int eRectifier     ;   // setup_RECTIFIER_MODE
	unsigned int bVideoFilter   ;	// video filter : on / off
	unsigned int eAverage       ;   // setup_EVERAGE_MODE
	unsigned int uiReject       ;   // rejection 0 - 100
}CONFIG_UT_RECEIVER;


//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//

typedef struct
{
	double       afGainOffset[256]    ;  // unit : db
}CONFIG_GROUP_GAINOFFSET;

typedef struct
{
	unsigned int auiBeamDelay[256]    ;  // unit : ns
}CONFIG_GROUP_BEAMDELAY ;
//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//

typedef struct
{   
	unsigned int uiPointQty     ;  //  sample point quantity per law
	__attribute__((aligned(8)))
	double       fSumGain       ;  //  sum gain , unit : db
} CONFIG_UT_ADVANCED;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
typedef struct
{
	int             	 eCurType ;		/* NONE  ,DAC, LINEAR DAC, TCG */
	int                  nPointQty;     // DAC or TCG setting Point quantity
	int                  nCurveStep ;   // unit 0.01 DB
	int               anRefAmp[256] ;   // unit 0.1 DB
	unsigned int      anAmp[256][16];   // unit 0.01 DB
	unsigned int      anPos[256][16];   // unit 10ns
} CONFIG_SIZING_CURVE;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//


enum setup_GATE_NAME
{
    setup_GATE_A = 0 ,
    setup_GATE_B     ,
    setup_GATE_I 
};

enum setup_GATE_SYN_MODE
{
	setup_GATE_SYN_PULSE = 0 ,
	setup_GATE_SYN_I , 
	setup_GATE_SYN_A ,  
} ;

enum setup_GATE_MEASURE_MODE
{
	setup_GATE_MEASURE_PEAK = 0 ,
	setup_GATE_MEASURE_EDGE ,   
} ;

typedef struct
{
	unsigned int eGate    ;     // setup_Gate_NAME
	unsigned int uiStart  ;     // gate start position
	unsigned int uiWidth  ;     // gate width
	unsigned int uiThreshold  ; // gate height
	unsigned int eSynChro     ; // setup_GATE_SYN_MODE
	unsigned int eMeasure     ; // setup_GATE_MEASURE_MODE
} CONFIG_GATE_INFO;
//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_DISPLAY_SELECTION_MODE
{
    Setup_DISPLAY_MODE_A = 0 ,
    Setup_DISPLAY_MODE_B  ,
    Setup_DISPLAY_MODE_C  ,
    Setup_DISPLAY_MODE_S  ,
    Setup_DISPLAY_MODE_A_B ,
    Setup_DISPLAY_MODE_A_B_C ,
    Setup_DISPLAY_MODE_A_B_S ,
    Setup_DISPLAY_MODE_A_C ,
    Setup_DISPLAY_MODE_A_S ,
    Setup_DISPLAY_MODE_PA_TOFD 
    
} ;

enum setup_DISPLAY_CSCAN_SOURCE
{
	setup_DISPLAY_CSCAN_A  = 0    ,
	setup_DISPLAY_CSCAN_B         ,
	setup_DISPLAY_CSCAN_THICKNESS ,
	setup_DISPLAY_CSCAN_I         ,
	setup_DISPLAY_CSCAN_OFF       ,
}  ;

enum setup_DISPLAY_UTUNIT
{
	setup_DISPLAY_UTUNIT_SOUNDPATH = 0 ,
	setup_DISPLAY_UTUNIT_TIME ,
	setup_DISPLAY_UTUNIT_TRUEDEPTH
} ;

typedef struct
{
	unsigned int eDisplay    ;   // setup_DISPLAY_SELECTION_MODE
	unsigned int bDisplayAll ;   // false : display all group , true : display current group
	unsigned int eCScan1     ;   // setup_DISPLAY_CSCAN_SOURCE
	unsigned int eCScan2     ;   // setup_DISPLAY_CSCAN_SOURCE
	unsigned int aeUtUnit[8] ;   // setup_DISPLAY_UTUNIT
}  CONFIG_DISPLAY_INFO;
//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//

typedef struct
{
    char            strSerial[20]  ;
    char            strMode[20]  ;
	unsigned int	uiElemQty    ;	
	__attribute__((aligned(8)))
	double      	fPitch       ;			
	double      	fFrequency   ;		
	double      	fReferencePoint; 
} CONFIG_PROBE_INFO;
//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_WEDGE_TYPE
{
	setup_WEDGE_TYPE_PA  = 0 ,
	setup_WEDGE_TYPE_UT   
} ;

enum setup_WEDGE_ORIENTATION
{
	setup_WEDGE_ORIENTATION_REVERSE = 0 ,
	setup_WEDGE_ORIENTATION_NORMAL 
} ;

typedef struct
{
    char            strSerial[20] ;
    char            strMode[20]  ;
	unsigned int    eType ;			// 0: PA wedge  1: UT Wedge
	unsigned int	eOrientation;	// 1 Normal 0 reversal//
	double      	fAngle;			// wedge angle
	double      	fHeight;	    // first element height
	double   	    fProbeDelay; 	// UT wedge , unit : ns
	double 	        fRefPoint;		// UT wedge , unit : mm
	double      	fVelocity;		// velocity   m/s

}  CONFIG_WEDGE_INFO;
//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_PROBE_PART_SKEW_ANGLE
{
	setup_PROBE_PART_SKEW_0 = 0 ,
	setup_PROBE_PART_SKEW_90  ,
	setup_PROBE_PART_SKEW_180 ,
	setup_PROBE_PART_SKEW_270 
};

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_MATERIAL_TYPE
{
                                     //   LW        SW
	setup_MATERIAL_ALUMINUM  = 0   , //{630000, 310000, "铝"},/* 0 */
	setup_MATERIAL_STEEL           , //{589000, 324000, "普通钢"},/* 1 */
	setup_MATERIAL_STEEL_STRAINLESS, //{580000, 310000, "不锈钢"},/* 2 */
	setup_MATERIAL_BRASS           , //{430000, 200000, "黄铜"},/* 3 */
	setup_MATERIAL_COPPER          , //{470000, 230000, "铜"},/* 4 */
	setup_MATERIAL_IRON            , //{590000, 320000, "铁"},/* 5 */
	setup_MATERIAL_LEAD            , //{220000, 220000, "铅"},/* 6 */
	setup_MATERIAL_NYLON           , //{260000, 260000, "尼龙"},/* 7 */
	setup_MATERIAL_SILVER          , //{360000, 160000, "银"},/* 8 */
	setup_MATERIAL_GOLD  		   , //{320000, 320000, "金"},/* 9 */
	setup_MATERIAL_ZINC            , //{420000, 240000, "锌"},/* 10 */
	setup_MATERIAL_TITANIUM        , //{610000, 310000, "钛"},/* 11 */
	setup_MATERIAL_TIN             , //{330000, 170000, "锡"},/* 12 */
	setup_MATERIAL_EXPOXYRESIN     , //{254000, 110000, "环氧树脂"},/* 13 */
	setup_MATERIAL_ICE             , //{400000, 200000, "冰"},/* 14 */
	setup_MATERIAL_NICKEL          , //{560000, 300000, "镍"},/* 15 */
	setup_MATERIAL_PLEXIGLASS      , //{270000, 146000, "树脂玻璃"},/* 16 */
	setup_MATERIAL_POLYSTYRENE     , //{240000, 115000, "聚苯乙烯"},/* 17 */
	setup_MATERIAL_PORCELAIN       , //{584200, 584200, "瓷器"},/* 18 */
	setup_MATERIAL_PVC             , //{238800, 238800, "聚氯乙烯"},/* 19 */
	setup_MATERIAL_QUARTZGLASS     , //{563900, 563900, "石英玻璃"},/* 20 */
	setup_MATERIAL_RUBBERVULCANIZED, //{231100, 231100, "硫化橡胶"},/* 21 */
	setup_MATERIAL_TEFLON          , //{140000, 140000, "聚四氟乙烯"},/* 22 */
	setup_MATERIAL_WATER           , //{148000, 148000, "水"}	/* 23 */
} ;



typedef struct
{
	unsigned int  eSkew        ;   //   setup_PROBE_PART_SKEW_ANGLE
	unsigned int   eMaterial    ;  // setup_MATERIAL_TYPE
	double        fThickness  ;  // PART thickness , unit : mm
	double        fScanOffset  ;   //   scan axis offset
	double        fIndexOffset ;   //   index axis offset


} CONFIG_PART_INFO;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//
enum setup_LAW_TYPE
{
	setup_LAW_TYPE_AZIMUTHAL = 0 , 
	setup_LAW_TYPE_LINEAR   
} ;

enum setup_FOCAL_TYPE
{
	setup_FOCAL_TYPE_HALF_PATH = 0 , 
	setup_FOCAL_TYPE_TRUE_DEPTH  ,
	setup_FOCAL_TYPE_PROJECTION  ,
	setup_FOCAL_TYPE_FOCAL_PLANE  
} ;

typedef struct
{
	unsigned int eLawType   ;        //  setup_LAW_TYPE
	unsigned int eFocalType ;		 //  setup_FOCAL_TYPE

	unsigned int uiAngleStart;		    // 0.1 degree
	unsigned int uiAngleStop ;			// focal law  angle setting
	unsigned int uiAngleStep ;			//
	
	unsigned int	uiElem_qty;			    // aperture setting
	unsigned int	uiFirst_tx_elem;		// first trigger element
	unsigned int	uiFirst_rx_elem;		// first receive element
	unsigned int	uiLast_tx_elem;		    // last  trigger element
	unsigned int	uiLast_rx_elem;		    // last  receive element
	unsigned int	uiElem_step;			// element step (linear scan)
	__attribute__((aligned(8)))
	//###################################################
	double	fPositionStart;		            // focal depth
	//#######################               //
	double	fPositionEnd;		            // not available currently
	double	fPositionStep;		            // not available currently
	//###################################################
	double	fOffsetStart;		            // focal plane
	double	fDepthStart;		            //
	double	fOffsetEnd;			            //
	double	fDepthEnd;			            //

}CONFIG_FOCALLAW ;

//#######Guangzhou Doppler Electronic Technologies Co. Ltd########
//***************************************************************//

/*encoder definition.*/
enum setup_ENCODER_TYPE
{
	setup_ENCODER_TYPE_TIMER = 0,
	setup_ENCODER_TYPE_ENCODER_1,
	setup_ENCODER_TYPE_ENCODER_2
};

/*Different counter types for encoder input*/
enum setup_ENCODER_MODE
{
    setup_ENC_MODE_DOWN  = 1   ,
    setup_ENC_MODE_UP          ,
    setup_ENC_MODE_QUADRATURE  ,
} ;

enum setup_ENCODER_POLARITY
{
    setup_ENC_NORNAL = 0,
    setup_ENC_REVERSE   
} ;

typedef struct
{
	unsigned int eEncoderType  ;// setup_ENCODER_TYPE
	unsigned int eEncoderMode  ;// setup_ENCODER_MODE
	unsigned int ePolarity     ;// setup_ENCODER_POLARITY
	__attribute__((aligned(8)))

	double fMaxScanSpeed;   
	double fResulotion  ;
	double fOrigin      ;
	
	double fScanStart   ;   
	double fScanStop    ;	 
	double fScanStep    ;   
} CONFIG_SCAN ; //__attribute__((aligned(8)));


enum COMMAND
{
	CMD_NONE =  0 ,
	CMD_RESET_ALL  ,
	CMD_RESET_ENCODER  ,
	CMD_SET_CURRENT_GROUP ,
	CMD_GET_STATUS_INFO ,
	CMD_ADD_GROUP ,
	CMD_DEL_GROUP ,
	CMD_SET_ENCODER_TYPE,
	CMD_GET_CONFIG_SCAN ,
	CMD_SET_CONFIG_SCAN ,
	CMD_GET_CONFIG_FOCALLAW ,
	CMD_SET_CONFIG_FOCALLAW ,
	CMD_GET_CONFIG_PROBE ,
	CMD_SET_CONFIG_PROBE ,
	CMD_GET_CONFIG_WEDGE ,
	CMD_SET_CONFIG_WEDGE ,
	CMD_GET_CONFIG_PART  ,
	CMD_SET_CONFIG_PART  ,
	CMD_GET_CONFIG_UT_GENERAL ,
	CMD_SET_CONFIG_UT_GENERAL ,
	CMD_GET_CONFIG_UT_PULSER  ,
	CMD_SET_CONFIG_UT_PULSER  ,
	CMD_GET_CONFIG_UT_RECEIVER,
	CMD_SET_CONFIG_UT_RECEIVER,
	CMD_GET_CONFIG_UT_ADVANCED ,
	CMD_SET_CONFIG_UT_ADVANCED ,
	CMD_GET_CONFIG_GATE_INFO   ,
	CMD_SET_CONFIG_GATE_INFO   ,
	CMD_GET_CONFIG_DISPLAY_INFO  ,
	CMD_SET_CONFIG_DISPLAY_INFO  ,
	CMD_GET_GROUP_BEAMDELAY    ,
	CMD_SET_GROUP_BEAMDELAY    ,
	CMD_GET_GROUP_GAINOFFSET   ,
	CMD_SET_GROUP_GAINOFFSET   ,
    CMD_GET_SIZINGCURVE ,
    CMD_SET_SIZINGCURVE ,
    
	CMD_MAX_MAX_MAX  ,

    CMD_ENABLE_DATA_TRANSFER ,

	CMD_GET_VAL_GROUP_QTY ,
    CMD_GET_VAL_TOTAL_DATA_SIZE ,
	CMD_GET_VAL_GROUP_DATA_SIZE ,
	CMD_GET_VAL_GROUP_DATA_OFFSET ,
	CMD_GET_VAL_GROUP_BEAM_OFFSET ,
	CMD_GET_VAL_GROUP_BEAM_LENGTH ,

    CMD_GET_VAL_GAIN  ,
    CMD_SET_VAL_GAIN  ,

    CMD_GET_VAL_START ,
    CMD_SET_VAL_START ,

    CMD_GET_VAL_RANGE ,
    CMD_SET_VAL_RANGE ,


    CMD_GET_VAL_POINT_QTY ,
    CMD_SET_VAL_POINT_QTY ,

    CMD_GET_VAL_WEDGEDELAY ,
    CMD_SET_VAL_WEDGEDELAY ,

    CMD_GET_VAL_VELOCITY   ,
    CMD_SET_VAL_VELOCITY   ,

    CMD_GET_VAL_PRF   ,
    CMD_SET_VAL_PRF   ,

} ;

#endif   // ____DOPPLER_NET_SERVER_H_______
