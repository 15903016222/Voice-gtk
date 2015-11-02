/*
 * calibration.h
 *
 *  Created on: 2012-6-8
 *      Author: shensheng
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_





extern void   CalibrationCallback021();
extern int    CalibrationCallback023();
extern void   CalibrationCallback024();
extern int    CalibrationCallback025();
extern void   esc_calibration()       ;
extern int    GetUtWedgeDelayCalicbration(int nGroupId_ , double nVelocity_) ;
extern int    cba_ultrasound_TCG() ;
extern int    cba_ultrasound_sensitivity() ;
extern int    cba_ultrasound_wedgedelay()  ;
extern double cba_ultrasound_velocity()  ;
extern double cba_encoder();
extern void   ClearCalibrationWedgeDelay()  ;
extern void   ClearCalibrationSensation ()  ;
extern void   ClearCalibrateEnvWedgeDlay()  ;
extern void   ClearCalibrateEnvSensitivity()  ;
extern int    LookupTabelAWS(int nAngleType_ , double nValue_) ;
extern int    CalibrationCallback022()   ;
extern void GetCalibrationStartAndStepLaw(int grp , int* nStart_ , int* nStep_) ;
typedef struct _awsRange
{
   double up   ;
   double down ;
}AWS_RANGE;


#endif /* CALIBRATION_H_ */
