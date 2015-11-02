/*
 * NetResponseFunc.h
 *
 *  Created on: 2013-5-2
 *      Author: wolflord
 */

#ifndef NETRESPONSEFUNC_H_
#define NETRESPONSEFUNC_H_


#define USLONG unsigned long
extern int Response_CMD_NONE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_RESET_ALL(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_RESET_ENCODER(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CURRENT_GROUP(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_ADD_GROUP(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_DEL_GROUP(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_ENCODER_TYPE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_UT_BEAM(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_UT_BEAM(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_ALARM_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_ALARM_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_SIZING_CURVE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_SIZING_CURVE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_MEASURE_READING(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_MEASURE_READING(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_MEASURE_CURSOR(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_MEASURE_CURSOR(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_OVERLAY_SWITCH(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_OVERLAY_SWITCH(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_MEASURE_THICKNESS(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_MEASURE_THICKNESS(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_PART_POSITION(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_PART_POSITION(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_CONFIG_PART_INFO(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_CONFIG_PART_INFO(USLONG Command, USLONG ParamSize, char* Parameter);


extern int Response_CMD_GET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_GET_SIZINE_CURVE(USLONG Command, USLONG ParamSize, char* Parameter);
extern int Response_CMD_SET_SIZINE_CURVE(USLONG Command, USLONG ParamSize, char* Parameter);

extern int Response_CMD_ENABLE_DATA_TRANSFER(USLONG Command, USLONG ParamSize, char* Parameter) ;

extern int Response_CMD_ENABLE_DATA_TRANSFER(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GROUP_QTY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_TOTAL_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GROUP_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GROUP_DATA_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GROUP_BEAM_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GROUP_BEAM_LENGTH(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_GET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)  ;
extern int Response_CMD_SET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)  ;
#endif /* NETRESPONSEFUNC_H_ */
