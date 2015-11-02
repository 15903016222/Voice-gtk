/*
 * menu.h
 *
 *  Created on: 2012-5-4
 *      Author: gener
 */

#ifndef MENUHANDLER_H_
#define MENUHANDLER_H_

#include "menuDefine.h"
void wizardGroupHandler_setStep(int step);
int wizardGroupHandler_back(void* pPara);
int wizardGroupHandler_group_select(void* pPara);
int wizardGroupHandler_addGroup(void* pPara);
int wizardGroupHandler_DeleteGroup(void* pPara);
int wizardGroupHandler_finish(void* pPara);
int wizardGroupHandler_next(void* pPara);
int wizardHandlerGroup_group_mode(void* p_para);
int wizardHandlerGroup_geometry(void* p_para);
int wizardHandlerGroup_thickness(void* p_para);
int wizardHandlerGroup_diameter(void* p_para);
int wizardHandlerGroup_pulse(void* p_para);
int wizardHandlerGroup_receive(void* p_para);
int wizardHandlerGroup_tx_rx_mode(void* p_para);
int wizardHandlerGroup_select_probe(void* p_para);
int wizardHandlerGroup_probe_auto_detect(void* p_para);
#define wizardHandlerGroup_select_wedge wizardHandlerGroup_select_probe
int wizardHandlerGroup_scan_off(void* p_para);
int wizardHandlerGroup_skew(void* p_para);
int wizardHandlerGroup_index_offset(void* p_para);
int wizardHandlerGroup_define_probe(void* p_para);
int wizardHandlerGroup_define_wedge(void* p_para);
int wizardHandlerGroup_material(void* p_para);
int wizardHandlerGroup_continue(void* p_para);

void wizardFocallawHandler_setStep(int step);
int wizardFocallawHandler_back(void* pPara);
int wizardFocallawHandler_group_select(void* pPara);
int wizardFocallawHandler_generate(void* pPara);
int wizardFocallawHandler_next(void* pPara);
int wizardFocallawHandler_law_type(void* p_para);
int wizardFocallawHandler_focal_type(void* p_para);
int wizardFocallawHandler_position_start(void* p_para);
int wizardFocallawHandler_position_end(void* p_para);
int wizardFocallawHandler_position_step(void* p_para);
int wizardFocallawHandler_offset_start(void* p_para);
int wizardFocallawHandler_offset_end(void* p_para);
int wizardFocallawHandler_depth_start(void* p_para);
int wizardFocallawHandler_depth_end(void* p_para);
int wizardFocallawHandler_wave_type(void* p_para);
int wizardFocallawHandler_material(void* p_para);
int wizardFocallawHandler_element_qty(void* p_para);
int wizardFocallawHandler_first_element(void* p_para);
int wizardFocallawHandler_last_element(void* p_para);
int wizardFocallawHandler_element_step(void* p_para);
int wizardFocallawHandler_min_angle(void* p_para);
int wizardFocallawHandler_max_angle(void* p_para);
int wizardFocallawHandler_angle_step(void* p_para);
int wizardFocallawHandler_focus_depth(void* p_para);


int outputHandler_alarmSelect(void* p_para);
int outputHandler_alarmGroupSelect(void* p_para);
int outputHandler_alarmCondition1(void* p_para);
int outputHandler_alarmOperator(void* p_para);
int outputHandler_alarmCondition2(void* p_para);
int outputHandler_outputSelect(void* p_para);
int outputHandler_switch(void* p_para);
int outputHandler_alarmCount(void* p_para);
int outputHandler_analogGroup(void* p_para);
int outputHandler_alarmBeep(void* p_para);
int outputHandler_analogDataSource(void* p_para);
int outputHandler_alarmDelay(void* p_para);
int outputHandler_alarmHoleTime(void* p_para);

int UpdateHandler_ResetCFG(void* p_para);
#endif /* MENUHANDLER_H_ */
