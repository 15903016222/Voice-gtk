/*
 * sizingcurve.h
 *
 *  Created on: 2012-10-12
 *      Author: wolflord
 */

#ifndef SIZINGCURVE_H_
#define SIZINGCURVE_H_
extern void   InitSizingCurveParameters(int nGroupId_) ;
extern void   SynocDACParameters(int nGroupId_ , int nBeamNo_ ) ;
extern void   refresh_linear_dac_pointer_info() ;
extern double CurrentPointTcgGainLimit(int nGroupId_) ;
extern void   AddDacPoint(int nGroupId_) ;
extern double DacAmptitudeMinimumLimit(int nGroupId_);
extern void   RefreshDacPointInfo(int nGroupId_);
extern void   SynocDACParametersOfCurrentPoint(int nGroupId_) ;
extern void   ClearTCGCalibrationCurrentPoint(int nGroupId_) ;
extern double GetTcgCurrentPointValue(int nGroupId_) ;
#endif /* SIZINGCURVE_H_ */
