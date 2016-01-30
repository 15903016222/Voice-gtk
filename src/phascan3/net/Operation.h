/*
 * Operation.h
 *
 *  Created on: 2013-5-3
 *      Author: wolflord
 */

#ifndef OPERATION_H_
#define OPERATION_H_

extern void RedrawAllUserInterface() ;
extern void ResetFpgaAll() ;
extern void RestartSCAN() ;
extern void GenerateFocallawOfAllGroup(int bRefreshCalibration) ;
//extern void RefreshGainMark(int nGroupId_)  ;
extern void MainMenuRefresh(int nId_)  ;
extern void SetCurrentGroup(int nId_)  ;

extern void NetAddOneGroup(int nGroupId_) ;
extern void NetDeleteOneGroup(int nGroupId_) ;
extern void NetSetEncoderType(int nType_)  ;
extern void NetGetScanInformation(void* pData_ , int id) ;
extern void NetSetScanInformation(void* pData_ ) ;

extern void NetGetFocallawInfo(void* pData_ , int nGroupId_ ) ;
extern void NetSetFocallawInfo(void* pData_ , int nGroupId_ ) ;
extern void NetGetProbeInfo(char* pData_ , int nGroupId_) ;
extern void NetSetProbeInfo(char* pData_ , int nGroupId_) ;
extern void NetGetWedgeInfo(char* pData_ , int nGroupId_) ;
extern void NetSetWedgeInfo(char* pData_ , int nGroupId_) ;

extern void NetGetPartInfo(void* pData_ , int nGroupId_)  ;
extern void NetSetPartInfo(void* pData_ , int nGroupId_)  ;

extern void NetGetUtGeneral(void* pData_ , int grp)  ;
extern void NetSetUtGeneral(void* pData_ , int grp)  ;
extern void NetGetUtPulser(void* pData_ , int nGroupId_)   ;
extern void NetSetUtPulser(void* pData_ , int nGroupId_)   ;
extern void NetGetUtReceive(void* pData_ , int nGroupId_)  ;
extern void NetSetUtReceive(void* pData_ , int nGroupId_)  ;

extern void NetGetGroupBeamDelay(void* pData_ , int nGroupId_) ;
extern void NetSetGroupBeamDelay(void* pData_ , int nGroupId_) ;
extern void NetGetGroupGainOffset(void* pData_ , int nGroupId_);
extern void NetSetGroupGainOffset(void* pData_ , int nGroupId_);
extern void NetGetGroupSizingCurve(void* pData_ , int nGroupId_);
extern void NetSetGroupSizingCurve(void* pData_ , int nGroupId_);
extern void NetGetUtAdvanced(void* pData_ , int nGroupId_)  ;
extern void NetSetUtAdvanced(void* pData_ , int nGroupId_)  ;

extern void NetGetGateInfo(void* pData_ , int nGroupId_ , int nGatePos_) ;
extern void NetSetGateInfo(void* pData_ , int nGroupId_) ;
extern void NetGetDisplayInfo(void* pData_) ;
extern void NetSetDisplayInfo(void* pData_) ;


extern void NetSetGainValue(int grp , double nVal_) ;

extern void NetSetGroupValueStart(int grp , int val) ;
extern void NetSetGroupValueRange(int nGroupId_ , int nVal_) ;
extern void NetSetGroupValuePointQty(int nGroupId_ , int nVal_) ;
extern void NetSetGroupValueWedgeDelay(int nGroupId_ , int nVal_) ;
extern void NetSetGroupValueVelocity(int nGroupId_ , int nVal_) ;
extern void NetSetGroupValuePRF(int nGroupId_ , int nVal_) ;
#endif /* OPERATION_H_ */
