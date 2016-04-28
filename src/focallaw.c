/************************************************* 
               聚焦法则 
 ************************************************* */

#include "focallaw.h"
#define PI M_PI
#define TRUE 1
#define FALSE 0
#define IF_DEBUG 0

#define SearchGate 0.001
#define SearchPathGate      0.00001

void focal_law(PARAMETER_P data);/*回调函数*/
void focal_law_type(PARAMETER_P data, double RefractAngle, double ScrewAnlge, double FocusLocation) ;
void current_focal_law(PARAMETER_P data);
void clear_focal_law(PARAMETER_P data); //聚焦法则数据清零 
void LinearFocusLaw(PARAMETER_P p, double RefractAngle, double ScrewAngle);
void DepthFocusLaw(PARAMETER_P data, double RefractAngle, double ScrewAngle);
/*聚焦法则算法实现*/
void cid_1d_eaglet(PARAMETER_P data, double angle1, double depth);
void cod_1d_eaglet(PARAMETER_P data, double angle1, double depth);
void flat_1d_roof(PARAMETER_P data, double angle1, double depth);
void flat_2d_roof(PARAMETER_P data, double angle1, double angle2, double depth);
void flat_1d_pr(PARAMETER_P data, double angle1, double depth);
void flat_1d_wedge_pr(PARAMETER_P data, double angle1, double depth);
void flat_1d_roof_pr(PARAMETER_P data, double angle1, double depth);
void flat_2d_roof_pr(PARAMETER_P data, double angle1, double angle2, double depth);
void cylinder_2d_roof_od (PARAMETER_P data, double angle1, double angle2, double depth);
void cylinder_2d_roof_id(PARAMETER_P data, double angle1, double angle2 , double depth);
void cylinder_2d_roof_od_pr(PARAMETER_P data, double angle1, double angle2, double depth);
void cylinder_2d_roof_id_pr(PARAMETER_P data, double angle1, double angle2, double depth);
void cylinder_1d_od_zero (PARAMETER_P data, double angle1, double depth);
void cylinder_1d_id_zero (PARAMETER_P data, double angle1, double depth);
/*  angle and depth transfer  */
int ProjectionToDepth_1d_roof (PARAMETER_P data, double RefractAngle, double Projection, double* FocusDepth);
int PlaneToDepth_1d_roof (PARAMETER_P data, double RefractAngle, double* FocusDepth);
int AngleSteerToRefract_1d_roof( PARAMETER_P data , double beam_steer_angle, double* RefractAngle);
int BeamSkewToRefract_1d_roof( PARAMETER_P data , double beam_skew_angle, double* RefractAngle);
int PlaneToDepth_2d_roof (PARAMETER_P data, double angle1, double angle2, double* FocusDepth) ;
int ProjectionToDepth_2d_roof(PARAMETER_P data,double angle1,double angle2,double Projection,double * FocusDepth);
int flat_1d_pr_auto_depth(PARAMETER_P data, double angle1, double* Depth);



void focal_law(PARAMETER_P data) 
{ 
     PARAMETER_P p = (PARAMETER_P)(data);
     double a2,a3,a_;
  
     double maxtime;
     int i,j;

     int ElementStart = p->element_sel->primary_axis_s - 1;
     int SelectColumn  = p->element_sel->pri_axis_ape     ;
     int ElementStop  =   SelectColumn+ ElementStart      ;
     double depth =p->focal_point->focal_focus_point_start; 
     double ScrewAnlge  =  p->beam_angle->beam_skew_angle_start ;
     /*True depth（Yp）or Half Path（S）*/
     switch(p->probe_p->D1_D2)
     {
          case 0://1D
                if((p->beam_angle->beam_angle_sel==0))
                {
                    a2 = (p->beam_angle->beam_refrac_angle_start); 
                    a3 = (p->beam_angle->beam_refrac_angle_stop); 
                    a_ = (p->beam_angle->beam_refrac_angle_resolution);
                }
                else if((p->beam_angle->beam_angle_sel==1))
                {  
                    a2=(p->beam_angle->beam_pri_steer_angle_start);
                    a3=(p->beam_angle->beam_pri_steer_angle_stop);
                    a_=(p->beam_angle->beam_pri_steer_angle_resolution);
                }
                else if((p->beam_angle->beam_angle_sel==2))
                {
                    a2 = (p->beam_angle->beam_skew_angle_start);
                    a3 = (p->beam_angle->beam_skew_angle_stop); 
                    a_ = (p->beam_angle->beam_skew_angle_resolution);
                }
               break;
          case 1://2D
               if((p->beam_angle->beam_angle_sel==0))
                {
                    
                }
                else if((p->beam_angle->beam_angle_sel==1))
                {  
                    
                }
                else if((p->beam_angle->beam_angle_sel==2))
                {
                    
                }
                else if((p->beam_angle->beam_angle_sel==3))
                {
                    a2 = (p->beam_angle->beam_refrac_angle_start)     ; 
                    a3 = (p->beam_angle->beam_refrac_angle_stop)      ; 
                    a_ = (p->beam_angle->beam_refrac_angle_resolution);
                }
               break;                            
     }
	 
     //聚焦法则数据先清零 
     clear_focal_law(p);
     p->k = 0;    
     switch(p->probe_p->D1_D2)
     {
       case 0://1D
            switch(p->beam_angle->beam_type)
            {
                case 0://Azimuthal
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Refracted angle
                                current_focal_law(p);
                                break;
                          case 1://Primary steering angle
                                current_focal_law(p);
                                break;
                          case 2://Beam screw angle 
                                current_focal_law(p);
                                break;  
                      }
                      break;
                case 1://Linear
                       switch(p->beam_angle->beam_angle_sel)
                        {
                           case 0://Refracted
                                LinearFocusLaw(p, a2, 0);
                                break;
                           case 1://Primary steering
                                p->is_draw_focus = AngleSteerToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
                                     LinearFocusLaw(p, a2, 0);
                                break;
                           case 2://Beam skew
                                p->is_draw_focus = BeamSkewToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
                                     LinearFocusLaw(p, a2, 0);
                                break;  
                        }
                      break;
                case 2://Depth
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Refracted angle
							    DepthFocusLaw(p, a2, 0);
                                break;
                          case 1://Primary steering angle
	  							p->is_draw_focus = AngleSteerToRefract_1d_roof(p,a2,&a2);  
                                if(p->is_draw_focus)
	  								   DepthFocusLaw(p, a2, 0);
                                break;
                          case 2://Beam screw angle 
							    p->is_draw_focus = BeamSkewToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
								      DepthFocusLaw(p, a2, 0);
                                break;
                      }
                      break;
                case 3://Static
                       switch(p->beam_angle->beam_angle_sel)
                        {
                           case 0://Refracted
                                focal_law_type(p, a2, ScrewAnlge, depth) ;
                                break;
                           case 1://Primary steering
				p->is_draw_focus = AngleSteerToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
	                                 focal_law_type(p, a2, ScrewAnlge, depth) ;
                                break;
                           case 2://Beam skew
                                p->is_draw_focus = BeamSkewToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
	                                  focal_law_type(p, a2, ScrewAnlge, depth) ;
                                break;
                        }
                      break;              
            }
            break;
       case 1://2D
            switch(p->beam_angle->beam_type)
            {
                case 0://Azimuthal
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Primary/Secondary steering angle
                                current_focal_law(p);
                                break;
                          case 1://Primary/Beam skew angle
                                current_focal_law(p);
                                break;
                          case 2://Secondary/Refracted angle 
                                current_focal_law(p);
                                break;
                          case 3://Refracted/Beam skew angle 
                                current_focal_law(p);
                                break; 
                      } 
                      break;
                case 1://Linear
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Primary/Secondary steering angle
                              
                                break;
                          case 1://Primary/Beam skew angle

                                break;
                          case 2://Secondary/Refracted angle 

                                break;
                          case 3://Refracted/Beam skew angle 
                                LinearFocusLaw(p, a2, ScrewAnlge);
                                break; 
                      } 
                      break;
                case 2://Depth
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Primary/Secondary steering angle

                                break;
                          case 1://Primary/Beam skew angle

                                break;
                          case 2://Secondary/Refracted angle 

                                break;
                          case 3://Refracted/Beam skew angle 
							    DepthFocusLaw(p, a2, ScrewAnlge);
                                break; 
                      } 
                      break;
                case 3://Static
                      switch(p->beam_angle->beam_angle_sel)
                      {
                          case 0://Primary/Secondary steering angle
 
                                break;
                          case 1://Primary/Beam skew angle

                                break;
                          case 2://Secondary/Refracted angle 

                                break;
                          case 3://Refracted/Beam skew angle 
                                focal_law_type(p, a2, ScrewAnlge, depth) ;
                                break; 
                      } 
                      break;              
            }            
            
            break;              
     }     

    for(i=0;i<p->k+1;i++)
    {
        //数组每行第一个元素保存 G_Delay 
        p->G_delay[i] = (int)(p->G_Time[i]*1000000+0.5);
    }

    if((p->beam_angle->beam_type == 0) && (p->beam_angle->beam_refrac_angle_start != p->beam_angle->beam_refrac_angle_stop) && (p->wedge_p->wg_height != 0 ))
    {
          a2 = p->beam_angle->beam_refrac_angle_start * PI/180 ;
          a3 = p->beam_angle->beam_refrac_angle_stop * PI/180  ;
          a_ = fabs( p->BeamExitPoint_x[0] - p->BeamExitPoint_x[p->k]) ;
          if(a2 == 0)
          {
               p->Junction = a_/tan(a3) ;
          }
          else if (a3 == 0)
          {
               p->Junction = a_/tan(-a2);
          }
          else if((a2 < 0) && (a3 > 0))
          {
               p->Junction = a_/(tan(-a2)+tan(a3));
          }
          else
          {
               p->Junction = a_/fabs(tan(a3)-tan(a2))  ;
          }
          if(p->Junction > 20.0) p->Junction = 20.0 ;
    }
    else 
    {
          p->Junction  = 0.0 ;
    }
    if(fabs(a3) > fabs(a2))
    {
	p->Junction = 2 * p->Junction * cos(fabs(a3)) ;
    }
    else
    {
	p->Junction = 2 * p->Junction * cos(fabs(a2)) ;
    }
}

void current_focal_law(PARAMETER_P data)
{
    PARAMETER_P p = (PARAMETER_P)(data); 
    double diameter;
    double ScrewAnlge; 
    double depth;
    double a1,a2,a3,a_,a_tmp,sin_t;
    double screwStart, screwEnd, screwStep, screwTemp ;
    double c1,c2;
    int l;
    int temp1 , temp2 , temp3 ;
    int count ;
	
    count = 0;
    a1 = p->wedge_p->wg_wedge_angle; /*楔块角度*/
    ScrewAnlge  =  p->beam_angle->beam_skew_angle_start ;
    if( !strcmp(p->specimen->Inspection_od_id,"ID") ) // ID
       diameter= p->specimen->speci_inside_cylindrical;/*直径*/
    else
       diameter= p->specimen->speci_outside_cylindrical;/*直径*/
    /* 声速 */
    if(p->probe_p->transmi_trans_longi==0)
    {   
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
    }                                           
    else                                 
    {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
    }

    if(p->focal_point->focal_focus_type == 0 || p->focal_point->focal_focus_type == 1)
		depth = p->focal_point->focal_focus_point_start;//half path
    else 
		 depth = p->focal_point->offset_start    ;
	
    /*True depth（Yp）or Half Path（S）*/
    switch(p->probe_p->D1_D2)
       {
	  case 0://1D
		if((p->beam_angle->beam_angle_sel==0))
		{
		    a2 = (p->beam_angle->beam_refrac_angle_start);
		    a_tmp=a2; 
		    a3 = (p->beam_angle->beam_refrac_angle_stop); 
		    a_ = (p->beam_angle->beam_refrac_angle_resolution);
		}
		else if((p->beam_angle->beam_angle_sel==1))
		{  
		    a2=(p->beam_angle->beam_pri_steer_angle_start);
		    a_tmp=a2;
		    a3=(p->beam_angle->beam_pri_steer_angle_stop);
		    a_=(p->beam_angle->beam_pri_steer_angle_resolution);
		}
		else if((p->beam_angle->beam_angle_sel==2))
		{
		    a2 = (p->beam_angle->beam_skew_angle_start);
		    a_tmp=a2;
		    a3 = (p->beam_angle->beam_skew_angle_stop); 
		    a_ = (p->beam_angle->beam_skew_angle_resolution);
		}
	       break;
	  case 1://2D
	       if((p->beam_angle->beam_angle_sel==0))
		{
		    
		}
		else if((p->beam_angle->beam_angle_sel==1))
		{  
		    
		}
		else if((p->beam_angle->beam_angle_sel==2))
		{
		    
		}
		else if((p->beam_angle->beam_angle_sel==3))
		{
		    a2 = (p->beam_angle->beam_refrac_angle_start);
		    a_tmp = a2; 
		    a3 = (p->beam_angle->beam_refrac_angle_stop); 
		    a_ = (p->beam_angle->beam_refrac_angle_resolution);
								    screwStart = (p->beam_angle->beam_skew_angle_start);
		    screwTemp  = screwStart;
		    screwEnd  = (p->beam_angle->beam_skew_angle_stop); 
		    screwStep = (p->beam_angle->beam_skew_angle_resolution);     
		}
	       break;                            
       }
    
	
    if(a_ == 0) return;//kill the impossible thing    
    l = (int) ((a3 - a2) / a_  + 0.5 );
    ///只有 Azimuthal 时，才会有 
    if(p->beam_angle->beam_type == 0)//
    {
        
        switch(p->probe_p->D1_D2)
        {
           case 0://1D
                  switch(p->beam_angle->beam_angle_sel)
                  {
                       case 0://Refracted angle
                            for ( p->tmp_k = 0; p->tmp_k < l + 1; p->tmp_k ++ ) 
                            {
            	                  p->k     = p->tmp_k;
            	                  a2 = a_tmp + a_ * (p->k);
            	                  //printf("\n %d degree=%f \n",p->tmp_k, a2);
            	                  focal_law_type(p, a2, ScrewAnlge, depth) ;
                            }
                            break;
                       case 1://Primary steering angle
                            for ( p->tmp_k = 0; p->tmp_k < l + 1; p->tmp_k ++ ) 
                            {
            	                  p->k     = p->tmp_k;
            	                  a2 = a_tmp + a_ * (p->k);
//            	                  printf("\ndegree=%f \n",a2);
            					  				// angle transfer
            	                  p->is_draw_focus = AngleSteerToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
            	                         focal_law_type(p, a2, ScrewAnlge, depth) ;

                            }
                            break;
                       case 2://Beam screw angle 
                            for ( p->tmp_k = 0; p->tmp_k < l + 1; p->tmp_k ++ ) 
                            {
            	                  p->k     = p->tmp_k;
            	                  a2 = a_tmp + a_ * (p->k);
//            	                  printf("\ndegree=%f \n",a2);
            					  				// angle transfer
            	                  p->is_draw_focus = BeamSkewToRefract_1d_roof(p,a2,&a2);
                                if(p->is_draw_focus)
            	                          focal_law_type(p, a2, ScrewAnlge, depth);
                            }
                            break;  
                  } 
                  break;
           case 1://2D
                  switch(p->beam_angle->beam_angle_sel)
                  {
                      case 0://Primary/Secondary steering angle
                            
                            break;
                      case 1://Primary/Beam skew angle
                            
                            break;
                      case 2://Secondary/Refracted angle 
                            
                            break;
                      case 3://Refracted/Beam skew angle 
                            if(screwEnd < screwStart) break ;
                            temp3 =1+ (int)((screwEnd - screwStart) / screwStep) ;
														l = (l+1) * temp3 ;
                            for ( p->tmp_k = 0; p->tmp_k < l ; p->tmp_k ++ ) 
                            {
            	                  p->k     = p->tmp_k;
															  temp1 = p->tmp_k / temp3 ;
															  temp2 = p->tmp_k % temp3 ;
								  
            	                  a2 = a_tmp + a_ * temp1;
								  							screwTemp = screwStart + screwStep * temp2 ;
//            	                  printf("\ndegree=%f \n",a2);
            	                   
            	                  focal_law_type(p, a2, screwTemp, depth) ;
                            }
                            break; 
                 } 
                 break;              
         }     
    }
}


void LinearFocusLaw(PARAMETER_P data, double RefractAngle, double ScrewAngle)
{
	PARAMETER_P p = (PARAMETER_P)(data);
	double a1 = RefractAngle;
	double a2 = ScrewAngle;
	int l;

	int ElementStart = p->element_sel->primary_axis_s-  1   ;
	int ElementColumn= p->probe_p->ele_num_pri              ;
	int SelectColumn = p->element_sel->pri_axis_ape         ;
	int LinearStop   = p->element_sel->primary_axis_e       ;
	int ElementStop  =   SelectColumn+ LinearStop -1        ;
	int PriElementResolution = p->element_sel->primary_axis_r;
	if(ElementStop>ElementColumn)  
	    return  ;

	   switch(p->probe_p->D1_D2)
	   {
		  case 0://1D
                
				l =  1+(LinearStop -1 - ElementStart)/PriElementResolution  ;
        for ( p->tmp_k = 0; p->tmp_k < l ; p->tmp_k ++ ) 
        {        
            p->element_sel->primary_axis_s = ElementStart + p->tmp_k * PriElementResolution + 1;
					  if(p->element_sel->primary_axis_s > ElementColumn )  continue ;
					  p->k     = p->tmp_k;
            focal_law_type(p, a1, 0, p->focal_point->focal_focus_point_start);                        
        }
				p->element_sel->primary_axis_s = ElementStart + 1 ;
				break;
		  case 1://2D
		        l =  1+(LinearStop -1 - ElementStart)/PriElementResolution  ;
            for ( p->tmp_k = 0; p->tmp_k < l ; p->tmp_k ++ ) 
            {        
                p->element_sel->primary_axis_s = ElementStart + p->tmp_k * PriElementResolution + 1;
							  if(p->element_sel->primary_axis_s > ElementColumn )  continue ;
							  p->k     = p->tmp_k;
                focal_law_type(p, a1, a2, p->focal_point->focal_focus_point_start);                        
            }
						p->element_sel->primary_axis_s = ElementStart + 1 ;
		        break ;
		}	  
      
}

void DepthFocusLaw(PARAMETER_P data, double RefractAngle, double ScrewAngle)
{
      PARAMETER_P p = (PARAMETER_P)(data);
		  double a1 = RefractAngle;
		  double a2 = ScrewAngle;
		  int l;
      double DepthStart = p->focal_point->focal_focus_point_start  ;
		  double DepthEnd   = p->focal_point->focal_focus_point_stop   ;
		  double Resolution = p->focal_point->focal_focus_point_resolution;
      if(DepthStart > DepthEnd ) return ;
		  l = 1 + (int)(DepthEnd - DepthStart)/Resolution     ;
	
		  for ( p->tmp_k = 0; p->tmp_k < l ; p->tmp_k ++ ) 
		  {        
		      DepthEnd = DepthStart + Resolution * p->tmp_k  ;
			  	p->k     = p->tmp_k;
		      focal_law_type(p, a1, a2, DepthEnd);                        
		  }
}
/*聚焦类型函数*/
// input           data:  pointer to parameters of wedge  probe specimen beam  focuspoint
//         RefractAngle:   Refract angle  (-90  to  90 degree)
//          ScrewAngle :    Screw angle  (-180  to 180 degree)
//        FocusLocation:  Could be True depth and half path
//return    0  :  normal ending
//            1  :   sender wedge focus point error
//            2  :   receiver wedge focus point error
//          -1  :   refract point error 
void focal_law_type(PARAMETER_P data, double RefractAngle, double ScrewAnlge, double FocusLocation) 
{
     PARAMETER_P p = (PARAMETER_P)(data);
     double FocusDepth;
		 double temp;
		 double r1 = p->specimen->speci_outside_cylindrical/2;
		 double r2 = p->specimen->speci_inside_cylindrical/2 ;
		 double a1 = RefractAngle*PI/180;
		 double a2 = ScrewAnlge*PI/180;
		 double AngleProjection;
	 
     switch(p->probe_p->D1_D2)
     {                         
         case 0:
            /* 1-D 聚焦算法 */
		        if(p->probe_p->Pitch_Catch == 0)
		        {
	                      switch(p->specimen->flat_cylindrical)
	                      {
	                       /*如果选中 Flat*/				   
	                       case 0:
						   	    if(p->focal_point->focal_focus_type==0) {     //half path
                                    FocusDepth = FocusLocation*cos(a1);
                                    flat_1d_roof( p, RefractAngle, FocusDepth);
                                } else if(p->focal_point->focal_focus_type==1) {//true depth
                                    FocusDepth = FocusLocation;
                                    flat_1d_roof( p, RefractAngle, FocusDepth);
                                } else if(p->focal_point->focal_focus_type==2) {//projection
                                   if(!ProjectionToDepth_1d_roof(p,RefractAngle,FocusLocation,&FocusDepth))
                                             return;
                                   else 
                                             flat_1d_roof( p, RefractAngle, FocusDepth);   
                            }
                      			else if(p->focal_point->focal_focus_type==3)  //focal plane
                            { 
                                  if(!PlaneToDepth_1d_roof(p, RefractAngle, &FocusDepth)) return;
                                  else flat_1d_roof( p, RefractAngle, FocusDepth);   
                            }
							           
	                           break;
	                       case 1:  //COD
                               FocusDepth = FocusLocation;
                               cod_1d_eaglet(p, RefractAngle, FocusDepth);
                               break;
	                       case 2:  //CID
                               FocusDepth = FocusLocation;
                               cid_1d_eaglet(p, RefractAngle, FocusDepth);
                               break;
                           default:
                               break;
                          }
                } else {
                    if(!p->specimen->flat_cylindrical) {
                        if( p->focal_point->focal_focus_type==0) {  //half path
                            FocusDepth = FocusLocation*cos(a1);
                        } else if(p->focal_point->focal_focus_type==1) {    //true depth
                            FocusDepth = FocusLocation ;
                        } else if(p->focal_point->focal_focus_type==2) {    //Automatic
                            flat_1d_pr_auto_depth(p, RefractAngle , &FocusDepth); 
                        }
                        flat_1d_pr(p, RefractAngle, FocusDepth);
                    }
                }
        		break;
		    case 1:
		    	/* 2-D聚焦算法 */
			    if(p->probe_p->Pitch_Catch == 0)
					{
				  		if(p->specimen->flat_cylindrical == 0)
				  		{
					         if(     p->focal_point->focal_focus_type==0)//half path
							     {
                          FocusDepth = FocusLocation*cos(a1);
													flat_2d_roof(p, RefractAngle, ScrewAnlge, FocusDepth ); 	 
                   }
							     else if(p->focal_point->focal_focus_type==1)//true depth
							     {
								        FocusDepth = FocusLocation ;
												flat_2d_roof(p, RefractAngle, ScrewAnlge, FocusDepth ); 	 
							     }
                   else if(p->focal_point->focal_focus_type==2)//projection
                   {
                          if(ProjectionToDepth_2d_roof(p, RefractAngle, ScrewAnlge, FocusLocation, &FocusDepth))
			        flat_2d_roof(p, RefractAngle, ScrewAnlge, FocusDepth ); 	
                   } 
		   else   //focal plane
                   {
                        if(PlaneToDepth_2d_roof (p, RefractAngle, ScrewAnlge, &FocusDepth))
										            flat_2d_roof(p, RefractAngle, ScrewAnlge, FocusDepth ); 	  	
                   }
				  		}
						  else
				  		{
					  	 		if(!strcmp(p->specimen->Inspection_od_id,"OD"))   
					  	 		{
							        if(p->focal_point->focal_focus_type==0)
											{
											      if(a2 == 0)
											      {
															  temp = sqrt(r1*r1+FocusLocation*FocusLocation-2*r1*FocusLocation*cos(a1));
															  FocusDepth = r1 - temp;
											      }
						                else if(a1 != 0) 
													  {
												   	      temp = FocusLocation*cos(a1)           ;
																  AngleProjection = atan(tan(a1)*cos(a2)); 
																  FocusDepth = temp/cos(AngleProjection) ;
																  temp = sqrt(r1*r1+FocusDepth*FocusDepth-2*r1*FocusDepth*cos(AngleProjection));
																  FocusDepth = r1 - temp ;
													  }
												  	else FocusDepth  = FocusLocation ;
											}
						          else   FocusDepth = FocusLocation ;
				               		cylinder_2d_roof_od(p, RefractAngle, ScrewAnlge, FocusDepth); 
					  	 		}
									else
									{
									        if(p->focal_point->focal_focus_type==0)
													{
													      if(a2 == 0)
													      {
																		  temp = sqrt(r2*r2+FocusLocation*FocusLocation+2*r2*FocusLocation*cos(a1));
																		  FocusDepth = temp - r2 ;
													      }
								                else if(a1 != 0) 
															  {
														   	      temp = FocusLocation*cos(a1)           ;
																		  AngleProjection = atan(tan(a1)*cos(a2)); 
																		  FocusDepth = temp/cos(AngleProjection) ;
																		  temp = sqrt(r2*r2+FocusDepth*FocusDepth+2*r2*FocusDepth*cos(AngleProjection));
																		  FocusDepth = temp - r2 ;
															  }
															  else FocusDepth = FocusLocation ;
													}
							            else   FocusDepth = FocusLocation ;
					               				cylinder_2d_roof_id(p, RefractAngle, ScrewAnlge, FocusDepth ); 
									}
				  		}	  
					}
					else
					{
		  			if(p->specimen->flat_cylindrical == 0)
			      {
			             if(p->focal_point->focal_focus_type==0)
									 {
									   	   FocusDepth = FocusLocation*cos(a1);
									 }
								   else   FocusDepth = FocusLocation ;
				          			 flat_2d_roof_pr(p, RefractAngle, ScrewAnlge, FocusDepth ); 
		  			}
					  else
		  			{
			  	 			if(!strcmp(p->specimen->Inspection_od_id,"OD"))   
			  	 			{
								    if(p->focal_point->focal_focus_type==0)
							      {
							          if(a2 == 0)
							          {
											      temp = sqrt(r1*r1+FocusLocation*FocusLocation-2*r1*FocusLocation*cos(a1));
											      FocusDepth = r1 - temp;
							          }
		                     else if(a1 != 0) 
									      {
								   	          temp = FocusLocation*cos(a1)           ;
												  	  AngleProjection = atan(tan(a1)*cos(a2)); 
												  	  FocusDepth = temp/cos(AngleProjection) ;
												  	  temp = sqrt(r1*r1+FocusDepth*FocusDepth-2*r1*FocusDepth*cos(AngleProjection));
												      FocusDepth = r1 - temp ;
									      }
								        else FocusDepth  = FocusLocation ;
							      }
				            else   FocusDepth = FocusLocation ;
		               			  cylinder_2d_roof_od_pr(p, RefractAngle, ScrewAnlge, FocusDepth); 
			  	 			}
	         			else
	         			{
	         				 if(p->focal_point->focal_focus_type==0)
							     {
							         if(a2 == 0)
							         {
												     temp = sqrt(r2*r2+FocusLocation*FocusLocation+2*r2*FocusLocation*cos(a1));
												     FocusDepth = temp - r2 ;
							         }
		                   else if(a1 != 0) 
									     {
								   	         temp = FocusLocation*cos(a1)           ;
												     AngleProjection = atan(tan(a1)*cos(a2)); 
												     FocusDepth = temp/cos(AngleProjection) ;
												     temp = sqrt(r2*r2+FocusDepth*FocusDepth+2*r2*FocusDepth*cos(AngleProjection));
												     FocusDepth = temp - r2 ;
									     }
								       else FocusDepth = FocusLocation ;
							     }
			             else   FocusDepth = FocusLocation ;
	               			 cylinder_2d_roof_id_pr(p, RefractAngle, ScrewAnlge, FocusDepth); 	
	         			}
		 			  }	
    			}   
	    		break;
				default:
					break;
     }
}

void cid_1d_eaglet(PARAMETER_P data, double angle1, double depth)
{
    PARAMETER_P p = (PARAMETER_P) data ;
    int flg = 0;
    int ele_num_pri, primary_axis_s, primary_axis_e, eleStop;
    double probe_pri_axis_pitch;
    float ele_center;
    double c1, c2, k1;
    double angleI, angleR = angle1*PI/180;
    double angleRSin, angleRCos;
    double angleWedge = p->wedge_p->wg_wedge_angle*PI/180;
    double angleSin = sin(angleWedge), angleCos = cos(angleWedge);
    double wg_pri_elem_offset, wg_arc_radius, wg_arc_chord, wg_arc_point;
    double hi_base, hi_center, bevels;
    double circleCX, circleCY;
    double a, b, c, tmp, tmpR, tmpD;
    double focus_x;
    double focus_y;
    double focus_z;
    double horizontal_x;
    double horizontal_y;
    double horizontal_z;
    double timeDelay[MAXNUM];
    double y0, z0, t, t0, t1;
    float ass=0, test;

    double xStart, xEnd;
    double posX, posZ, lenX, tana;
    double dis1, dis2, dis1Tmp, dis2Tmp, minX, maxX;
    double x1, z1, sinap, sina, sinbp, sinb, sint, cost, angleb;
    double tmpC, tmpA, tmpK, tmpKC, tmpL, pos;
    int loop;

    int i, j;

    ele_num_pri = p->element_sel->pri_axis_ape;
    ele_center = (ele_num_pri-1)/2.0f;
    primary_axis_s = p->element_sel->primary_axis_s-1;
    primary_axis_e = p->element_sel->primary_axis_e;
    probe_pri_axis_pitch = p->probe_p->pri_axis_pitch;
    eleStop = primary_axis_s + ele_num_pri;
    if(p->probe_p->transmi_trans_longi==0) {
        c1  = p->wedge_p->wg_lon_vel ;
        c2  = p->specimen->speci_transverse_wave;
    } else  {
        c1  = p->wedge_p->wg_lon_vel ;
        c2  = p->specimen->speci_longitudinal_wave;
    }
    k1 = c1/c2;
    hi_base = p->wedge_p->wg_heigh_fir;
    bevels = probe_pri_axis_pitch*(ele_center+primary_axis_s);
    hi_center = hi_base + angleSin*bevels;
    wg_pri_elem_offset = -fabs(p->wedge_p->wg_pri_elem_offset_fir);

    wg_arc_radius = p->specimen->speci_outside_cylindrical/2;
    wg_arc_chord =  p->wedge_p->wg_length;
    tmpR = wg_arc_radius*wg_arc_radius;
    circleCX = -wg_arc_chord/2;
    circleCY = sqrt(tmpR-wg_arc_chord*wg_arc_chord/4);

    t0 = wg_pri_elem_offset - circleCX + angleCos*bevels;
    z0 = hi_center - circleCY;
    for (i=0; i<p->probe_p->ele_num_pri; i++) {
        p->probe_x[p->k][i] = wg_pri_elem_offset - circleCX + angleCos*i*probe_pri_axis_pitch;
        p->probe_z[p->k][i] = hi_base - circleCY + angleSin*i*probe_pri_axis_pitch;
    }
    p->probe_x[p->k][MAXNUM-1] = t0;
    p->probe_z[p->k][MAXNUM-1] = z0;
    p->probe_z[p->k][MAXNUM-2] = -circleCY;

    dis1 = t0*t0+z0*z0;
    dis1Tmp = sqrt(dis1)/(k1*wg_arc_radius);
    if (dis1Tmp > 1) {
        tmp = M_PI/2;
    } else {
        tmp = asin(sqrt(dis1)/(k1*wg_arc_radius));
    }
    if ((angleR > tmp) && (angleR < 2*tmp)) {
       angleR = 2*tmp-angleR;
       flg = 1;
    } else if (angleR < -tmp) {
       angleR = -angleR - 2*tmp;
       flg = -1;
    } else if (angleR > 2*tmp) {
        return;
    }
    angleRSin = sin(angleR);
    angleRCos = cos(angleR);

    tmp = angleRSin*k1*angleRSin*k1;
    a = tmpR*tmp;
    b = dis1*(1-tmp);
    c = a*(1-tmp);
    if (b<c) {
        return;
    }
    b = wg_arc_radius*sqrt(b-c);
    if (flg == 0) {
        t = a + b;
    } else if (flg>0) {
        t = a - b;
    }
    b = dis1+tmpR-2*t;

    c = t0*t;
    if (b<-0.0001) {
//        return 0;
        return;
    } else if (b<0.0001) {
        b = 0;
    }

    b = z0*wg_arc_radius*angleRSin*k1*sqrt(b);
    y0 = t0*t0+z0*z0;
    if (angle1 < 0) {
        horizontal_x = (c+b)/dis1;
    } else {
        horizontal_x = (c-b)/dis1;
    }
    horizontal_z = (t-t0*horizontal_x)/z0;
    p->horizontal_x[p->k][MAXNUM-1] = horizontal_x;
    p->horizontal_z[p->k][MAXNUM-1] = horizontal_z;
    a = (-horizontal_x*sqrt(tmpR-wg_arc_chord*wg_arc_chord/4)-wg_arc_chord*horizontal_z/2)/tmpR;
	p->field_distance[p->k] = -wg_arc_radius*asin(a);

    tmpD = wg_arc_radius+depth;
    tmp = wg_arc_radius*angleRSin/tmpD;
    tmp = asin(tmp)-angleR;
    t = wg_arc_radius*tmpD*cos(tmp);

    tmpD *= tmpD;
    b = tmpR*tmpD-t*t;
    if (b<-0.0001) {
        memset(p->horizontal_x[p->k], 0, MAXNUM*sizeof(double));
        memset(p->horizontal_z[p->k], 0, MAXNUM*sizeof(double));
        memset(p->probe_x[p->k], 0, MAXNUM*sizeof(double));
        memset(p->probe_z[p->k], 0, MAXNUM*sizeof(double));
        p->focus_x[p->k] = 0;
        p->focus_z[p->k] = 0;
//        return -1;
        return;
    } else if (b<0.0001) {
        b=0;
    }
    t0 = p->horizontal_x[p->k][MAXNUM-1];
    z0 = p->horizontal_z[p->k][MAXNUM-1];
    test = z0*p->probe_x[p->k][MAXNUM-1] - t0*p->probe_z[p->k][MAXNUM-1];
    a = t0*t;
    b = z0*sqrt(b);
    c = a + b;
    focus_x = c/tmpR;
    focus_z = (t-t0*focus_x)/z0;
    ass = test*(z0*focus_x - t0*focus_z);
    if (ass < 0.00001) {
        p->focus_x[p->k] = focus_x;
        p->focus_z[p->k] = focus_z;
    }

    c = a - b;
    focus_x = c/tmpR;
    focus_z = (t-t0*focus_x)/z0;
    ass = test*(z0*focus_x - t0*focus_z);
    if (ass<0.00001) {
        p->focus_x[p->k] = focus_x;
        p->focus_z[p->k] = focus_z;
    }

    tmpK = k1*k1;
    for (i=primary_axis_s; i<eleStop; i++) {
        t0 = p->probe_x[p->k][i];
        z0 = p->probe_z[p->k][i];
        x1 = p->focus_x[p->k];
        z1 = p->focus_z[p->k];
        xEnd = wg_arc_radius*x1/sqrt(x1*x1 + z1*z1);
        xStart = wg_arc_radius*t0/sqrt(t0*t0 + z0*z0);
        pos = (xEnd-xStart)/2;
        posX = xStart;
        dis1Tmp = 100;
        dis2Tmp = 100;
        dis1 = 0;
        dis2 = 0;
        for (loop = 0; loop < 30; loop++) {
            posX += pos;
            posZ = -sqrt(tmpR-posX*posX);
            dis1 = sqrt((posX-t0)*(posX-t0)+(posZ-z0)*(posZ-z0));
            sinap = (posZ*t0-posX*z0)/dis1;
            if (fabs(sinap) > wg_arc_radius) {
                pos = -fabs(pos)/2;
                continue;
            }
            sinap = sinap/k1;
            dis2 = sqrt((posX-x1)*(posX-x1)+(posZ-z1)*(posZ-z1));
            sinbp = (posX*z1-posZ*x1)/dis2;
            if (sinap>sinbp) {
                pos = -fabs(pos)/2;
            } else {
                pos = fabs(pos)/2;
            }

            if ((fabs(dis1-dis1Tmp) < SearchPathGate) &&
                    (fabs(dis2-dis2Tmp) < SearchPathGate)) {
                break;
            }
            dis1Tmp = dis1;
            dis2Tmp = dis2;
        }
        p->horizontal_x[p->k][i] = posX;
        p->horizontal_z[p->k][i] = posZ;
    }

	double timemax ;
	double timemin ;
	memset(timeDelay, 0, MAXNUM*sizeof(double));
    for(i = primary_axis_s; i< eleStop; i++) {
        dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])+
                (p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
        dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
                +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
       timeDelay[i] = dis1/c1+dis2/c2;
    }

    dis1 = sqrt((p->probe_x[p->k][MAXNUM-1]-p->horizontal_x[p->k][MAXNUM-1])*(p->probe_x[p->k][MAXNUM-1]-p->horizontal_x[p->k][MAXNUM-1])+
            (p->probe_z[p->k][MAXNUM-1]-p->horizontal_z[p->k][MAXNUM-1])*(p->probe_z[p->k][MAXNUM-1]-p->horizontal_z[p->k][MAXNUM-1]));
    dis2 = sqrt((p->horizontal_x[p->k][MAXNUM-1]-p->focus_x[p->k])*(p->horizontal_x[p->k][MAXNUM-1]-p->focus_x[p->k])
            +(p->horizontal_z[p->k][MAXNUM-1]-p->focus_z[p->k])*(p->horizontal_z[p->k][MAXNUM-1]-p->focus_z[p->k]));
    timeDelay[MAXNUM-1] = dis1/c1+dis2/c2;

    for(i = primary_axis_s, timemax = 0, timemin=1; i<eleStop ; i++) {
        if(timemax < timeDelay[i])  timemax = timeDelay[i] ;
    }

    for(i = primary_axis_s; i<eleStop ; i++) {
        p->timedelay[p->k][i] = (int)((timemax - timeDelay[i])*1000000+0.5);
    }

    timemax = (timemax - timeDelay[MAXNUM-1] + dis1/c1)*2;
    p->G_Time[p->k] = timemax;
}

void cod_1d_eaglet(PARAMETER_P data, double angle1, double depth)
{
	PARAMETER_P p = (PARAMETER_P) data ;
    int oddeven;
    int ele_num_pri, primary_axis_s, primary_axis_e, eleStop;
    double probe_pri_axis_pitch;
    float ele_center;
    double c1, c2, k1;
    double angleI, angleR = angle1*PI/180;
    double angleRSin = sin(angleR), angleRCos = cos(angleR);
    double angleWedge = p->wedge_p->wg_wedge_angle*PI/180;
    double angleSin = sin(angleWedge), angleCos = cos(angleWedge);
    double wg_pri_elem_offset, wg_arc_radius, wg_arc_chord;
    double hi_base, hi_center, bevels;
    double circleCX, circleCY;
    double a, b, c, tmp, tmpR, tmpD;
    double focus_x;
    double focus_y;
    double focus_z;
    double horizontal_x;
    double horizontal_y;
    double horizontal_z;
	double timeDelay[MAXNUM];
    double y0, z0, t, t0, t1;
    float ass=0, test;

    double xStart, xEnd;
    double posX, posZ, lenX, tana;
    double dis1, dis2, dis1Tmp, dis2Tmp, minX, maxX;
    double x1, z1, sina, sinb, sint, cost, angleb;
    double tmpC, tmpA, tmpK, tmpKC, tmpL, pos;
    int loop;

    int i, j;

    ele_num_pri = p->element_sel->pri_axis_ape;
    ele_center = (ele_num_pri-1)/2.0f;
    oddeven = ele_num_pri%2;
    primary_axis_s = p->element_sel->primary_axis_s-1;
    primary_axis_e = p->element_sel->primary_axis_e;
    probe_pri_axis_pitch = p->probe_p->pri_axis_pitch;
    eleStop = primary_axis_s + ele_num_pri;
	if(p->probe_p->transmi_trans_longi==0) {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */
    } else  {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
    }
    k1 = c1/c2;
    hi_base = p->wedge_p->wg_heigh_fir;
    bevels = probe_pri_axis_pitch*(ele_center+primary_axis_s);
    hi_center = hi_base + angleSin*bevels;
    wg_pri_elem_offset = -fabs(p->wedge_p->wg_pri_elem_offset_fir);

    wg_arc_radius = p->specimen->speci_outside_cylindrical/2;
    wg_arc_chord =  p->wedge_p->wg_length;
    circleCX = -wg_arc_chord/2;
    circleCY = -wg_arc_radius;

    if (depth > wg_arc_radius*(1-sin(angleR))) {
        return;
    }

    t0 = wg_pri_elem_offset - circleCX + angleCos*bevels;
    z0 = hi_center - circleCY;
    for (i=0; i<p->probe_p->ele_num_pri; i++) {
        p->probe_x[p->k][i] = wg_pri_elem_offset - circleCX + angleCos*i*probe_pri_axis_pitch;
        p->probe_z[p->k][i] = hi_base - circleCY + angleSin*i*probe_pri_axis_pitch;
    }
    p->probe_x[p->k][MAXNUM-1] = t0;
    p->probe_z[p->k][MAXNUM-1] = z0;
    p->probe_z[p->k][MAXNUM-2] = wg_arc_radius;

    dis1 = t0*t0+z0*z0;
    tmp = angleRSin*k1;
    tmp *= tmp;
    tmpR = wg_arc_radius*wg_arc_radius;
    a = tmpR*tmp;
    b = dis1*(1-tmp);
    c = a*(1-tmp);
    b = wg_arc_radius*sqrt(b-c);
    t = a + b;
    b = dis1+tmpR-2*t;

    c = t0*t;
    if (b<-0.0001) {
//        return 0;
        return;
    } else if (b<0.0001) {
        b = 0;
    }

    b = z0*wg_arc_radius*angleRSin*k1*sqrt(b);
    if (angle1 > 0) {
        horizontal_x = (c+b)/dis1;
    } else {
        horizontal_x = (c-b)/dis1;
    }
    horizontal_z = (t-t0*horizontal_x)/z0;
    p->horizontal_x[p->k][MAXNUM-1] = horizontal_x;
    p->horizontal_z[p->k][MAXNUM-1] = horizontal_z;
    a = (horizontal_x*wg_arc_chord/2+horizontal_z*sqrt(tmpR-wg_arc_chord*wg_arc_chord/4))/tmpR;
	p->field_distance[p->k] = -wg_arc_radius*acos(a);

    tmpD = wg_arc_radius-depth;
    tmp = wg_arc_radius*angleRSin/tmpD;
    tmp = asin(tmp)-angleR;
    t = wg_arc_radius*tmpD*cos(tmp);

    tmpD *= tmpD;
    b = tmpR*tmpD-t*t;
    if (b<-0.0001) {
        memset(p->horizontal_x[p->k], 0, MAXNUM*sizeof(double));
        memset(p->horizontal_z[p->k], 0, MAXNUM*sizeof(double));
        memset(p->probe_x[p->k], 0, MAXNUM*sizeof(double));
        memset(p->probe_z[p->k], 0, MAXNUM*sizeof(double));
        p->focus_x[p->k] = 0;
        p->focus_z[p->k] = 0;
//        return -1;
        return;
    } else if (b<0.0001) {
        b=0;
    }
    t0 = p->horizontal_x[p->k][MAXNUM-1];
    z0 = p->horizontal_z[p->k][MAXNUM-1];
    test = z0*p->probe_x[p->k][MAXNUM-1] - t0*p->probe_z[p->k][MAXNUM-1];
    a = t0*t;
    b = z0*sqrt(b);
    c = a + b;
    focus_x = c/tmpR;
    focus_z = (t-t0*focus_x)/z0;
    ass = test*(z0*focus_x - t0*focus_z);
    if (ass < 0.00001) {
        p->focus_x[p->k] = focus_x;
        p->focus_z[p->k] = focus_z;
    }

    c = a - b;
    focus_x = c/tmpR;
    focus_z = (t-t0*focus_x)/z0;
    ass = test*(z0*focus_x - t0*focus_z);
    if (ass<0.00001) {
        p->focus_x[p->k] = focus_x;
        p->focus_z[p->k] = focus_z;
    }

    tmpK = k1*k1;
    for (i=primary_axis_s; i<eleStop; i++) {
        t0 = p->probe_x[p->k][i];
        z0 = p->probe_z[p->k][i];
        x1 = p->focus_x[p->k];
        z1 = p->focus_z[p->k];
        xEnd = wg_arc_radius*x1/sqrt(x1*x1 + z1*z1);
        xStart = wg_arc_radius*t0/sqrt(t0*t0 + z0*z0);
        pos = (xEnd-xStart)/2;
        posX = xStart;
        dis1Tmp = 100;
        dis2Tmp = 100;
        dis1 = 0;
        dis2 = 0;
        for (loop = 0; loop < 30; loop++) {
            posX += pos;
            posZ = sqrt(tmpR-posX*posX);
            dis1 = sqrt((posX-t0)*(posX-t0)+(posZ-z0)*(posZ-z0));
            sina = (posX*z0-posZ*t0)/(wg_arc_radius*dis1);
            if (sina > 1) {
                pos = -fabs(pos)/2;
                continue;
            }
            sina = sina/k1;
            dis2 = sqrt((posX-x1)*(posX-x1)+(posZ-z1)*(posZ-z1));
            sinb = (posZ*x1-posX*z1)/(wg_arc_radius*dis2);
            if (sina>sinb) {
                pos = -fabs(pos)/2;
            } else {
                pos = fabs(pos)/2;
            }

            if ((fabs(dis1-dis1Tmp) < SearchPathGate) &&
                    (fabs(dis2-dis2Tmp) < SearchPathGate)) {
                break;
            }
            dis1Tmp = dis1;
            dis2Tmp = dis2;
        }
        p->horizontal_x[p->k][i] = posX;
        p->horizontal_z[p->k][i] = posZ;
    }

	double timemax ;
	double timemin ;
	memset(timeDelay, 0, MAXNUM*sizeof(double));
    for(i = primary_axis_s; i< eleStop; i++) {
        dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])+
                (p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
        dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
                +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
       timeDelay[i] = dis1/c1+dis2/c2;
    }
    dis1 = sqrt((p->probe_x[p->k][MAXNUM-1]-p->horizontal_x[p->k][MAXNUM-1])*(p->probe_x[p->k][MAXNUM-1]-p->horizontal_x[p->k][MAXNUM-1])+
            (p->probe_z[p->k][MAXNUM-1]-p->horizontal_z[p->k][MAXNUM-1])*(p->probe_z[p->k][MAXNUM-1]-p->horizontal_z[p->k][MAXNUM-1]));
    dis2 = sqrt((p->horizontal_x[p->k][MAXNUM-1]-p->focus_x[p->k])*(p->horizontal_x[p->k][MAXNUM-1]-p->focus_x[p->k])
            +(p->horizontal_z[p->k][MAXNUM-1]-p->focus_z[p->k])*(p->horizontal_z[p->k][MAXNUM-1]-p->focus_z[p->k]));
    timeDelay[MAXNUM-1] = dis1/c1+dis2/c2;

    for(i = primary_axis_s, timemax = 0, timemin=1; i<eleStop ; i++) {
        if(timemax < timeDelay[i])  timemax = timeDelay[i] ;
    }

    for(i = primary_axis_s; i<eleStop ; i++) {
        p->timedelay[p->k][i] = (int)((timemax - timeDelay[i])*1000000+0.5);
    }

    timemax = (timemax - timeDelay[MAXNUM-1] + dis1/c1)*2;
    p->G_Time[p->k] = timemax;
}

//*************************************************************************
//  Condition: true depth, refract angle, deflect angle
//  2d focus with roof angle
//*************************************************************************
/*
  Name:  flat_1d_roof
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   平面工件1D 带ROOF角 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
  angle1: refract angle
  depth : focus depth of sonic beam
*/
void flat_1d_roof(PARAMETER_P data, double angle1, double depth)
{
	PARAMETER_P p = (PARAMETER_P) data ;
	//**********************
	p->BeamRefAngle[p->k] = angle1;
	p->BeamSecSteeringAngle[p->k] = 0;
	//***********************
	double c1 ;			// wave speed
	double c2 ;
	double k1 ;  
	double h0 = p->wedge_p->wg_heigh_fir ;				      // first element high

  	int ElementStart = p->element_sel->primary_axis_s - 1;
  	int ElementColumn= p->probe_p->ele_num_pri           ;
	int SelectColumn  = p->element_sel->pri_axis_ape     ;
	int ElementStop  =   SelectColumn+ ElementStart      ;
    
	double Intervalx = p->probe_p->pri_axis_pitch  ;		  // x direction interval distance
	double pri_ele_size = p->probe_p->pri_ele_size ;      
	double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	double wg_pri_elem_offset_fir =  0 ; /*第一主轴阵元偏移*/
	double wg_pri_elem_offset_fir_PA = p->wedge_p->wg_pri_elem_offset_fir ; // add for PA calculation displayed in PA2000
        double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	double  a1 = angle1*PI/180;                                        // refract angle	
	double  roofangle = p->wedge_p->wg_roof_angle*PI/180.0	; // roofangle	 y-z platform
	double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180.0	; // wedgeangle  x-z platform	  
	double  FocusDepth = depth;							  //  true focus depth
	double xTemp, yTemp, zTemp ;
	double x0,y0,z0;										  // element center coordinate
	double xP, yP, zP; 									  // focus point coordinate
	// get focus point coordinate		   
	double tanw;
	double tanr;
	double para1,para2;
	double vx1,vy1,vz1,vx2,vy2,vz2;
	double desirex,desirey,desirez;
	double min_angle1 ;
	double min_angle2 ;
	double refract_x;
	double refract_y;
	double tempR;
  	double len1;
	double len2;
	double len3;
	// variable for search method  // 
  	double SearchLength , SearchPoint;
  	double TempLength;
  	double TempResult;
  	double sina;
  	double tana;    
  	double sinb;
  	double tanb;     
  	double sinTmp;
  	double cosTmp;
  	double tanTmp;
  	double disTmp;     
  	double xxx;
  	double yyy;
	int i, j; 
  	int count;
	// variable for refract point
  	double aa1,bb1;
	double aa2,bb2;
	//  time delay calculation
	double dis1;
	double dis2;
	double timemax ;
	double timemin ;

	if(ElementStop > ElementColumn)  return ;
    	z0 = Intervalx*ElementStart*sin(wedgeangle);
	x0 = Intervalx*ElementStart*cos(wedgeangle);
	
	double *time = malloc(ElementColumn * sizeof(double));
	memset(time,0x0,ElementColumn * sizeof(double));
	
	// kill the impossible condition
	if(wedgeangle == PI/2 || roofangle == PI/2)  return ;

	c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
	c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  

	if(c1 == 0) c1 = 1 ; // to prevent distance/c1 error
	// refract coefficient
	k1 =c1/c2 ; 	  
	// center probe element    (x0,y0,z0)			 
	z0 = -( z0 + h0 + Intervalx * ( SelectColumn - 1 ) * sin( wedgeangle ) / 2 );
	x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1) * cos(wedgeangle) / 2;
        if(roofangle<0)
		 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;a1=-a1;}
	y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir;
	// get focus point coordinate 
	// desirex desirey desirez : 入射点坐标          
	if(roofangle == 0)
	{
		TempLength = FocusDepth*tan(a1) ;
		p->focus_x[p->k] =  x0+TempLength ;
		TempLength =  fabs(z0)*tan(asin(sin(a1)*k1));
		p->focus_x[p->k] += TempLength ;
		p->focus_y[p->k] =  y0 ;
		p->focus_z[p->k] =  FocusDepth;
		xP = p->focus_x[p->k];
		yP = p->focus_y[p->k];
		zP = p->focus_z[p->k];
		//*************************************************
		//计算折射点和偏转角度，用作显示用
		p->BeamExitPoint_x[p->k] = x0 + TempLength  ;
		p->BeamExitPoint_y[p->k] = y0               ;
		p->BeamExitPoint_z[p->k] = 0                ;
		p->BeamSkewAngle[p->k]   = 0 		    ;
		p->BeamPriSteeringAngle[p->k] = (atan(-TempLength/z0) - wedgeangle)*180 / PI  ;
		//*********************************************************
		p->field_distance[p->k] = wg_pri_elem_offset_fir_PA + p->BeamExitPoint_x[p->k] ;
	}
	else
	{
	        if(wedgeangle==0)
		{
			desirex = x0 ;
			desirey = y0-fabs(z0)*tan(roofangle);
			desirez = 0;
		}
		else
		{
			tanw = -tan(wedgeangle);
			tanr = tan(roofangle)  ;
			vx1  = tanw*tanr       ;
			vy1  = -(1+tanw*tanw)  ;
			vz1  = -tanr           ;
		        // (x-para1)/vx2 = y-para2   
	     		//过探头与楔块面垂直面与Z=0面的交线方程
	    		para1 = x0             ;
	    		para2 = y0 + z0*vz1/vy1;
	    		vx2 = -vy1/vx1         ;
	    		//中心探头与交线的垂直点
	    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
	    		desirey = para2 + (desirex - para1)/vx2 ;
	    		desirez = 0 ;
		}
    		// 求最小入射角
		min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
    	min_angle2 = sin(min_angle1)/k1;
    	if(min_angle2>=1)
    	{
            // 如果最小入射角发生全反射取焦点为中心探头坐标
        	xP = x0;
        	yP = y0;
        	a1 = 0 ;
    	}
		else 
		{
	      if(asin(min_angle2)>=fabs(a1))
	      {
	          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
	          refract_x = desirex;
		      refract_y = desirey;
		      a1 = min_angle2 ;
	      }
          else
          {
              // 计算正常情况下焦点的坐标
              //refract point 折射点坐标 
              min_angle2 = asin(sin(a1)*k1);
              tempR = z0*tan(min_angle2);
		      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
		      if(a1>=0)
		      {
	                 refract_x = desirex-TempLength*vx2/sqrt(1+vx2*vx2);
			         refract_y = desirey-TempLength/sqrt(1+vx2*vx2);
              }
              else
              {
	                 refract_x = desirex+TempLength*vx2/sqrt(1+vx2*vx2);
			         refract_y = desirey+TempLength/sqrt(1+vx2*vx2);
              }
          }		  
          TempLength = FocusDepth*tan(a1) ;
		  len1 = refract_x - x0;
		  len2 = refract_y - y0;
		  len3 = sqrt(len1*len1+len2*len2);
          if(a1>=0)
          {
                xP = refract_x+TempLength*len1/len3;
		        yP = refract_y+TempLength*len2/len3;
          }
          else
          {
                xP = refract_x-TempLength*len1/len3;
		        yP = refract_y-TempLength*len2/len3;
          }
          }
          p->focus_x[p->k] =  xP ;
          p->focus_y[p->k] =  yP ;
          p->focus_z[p->k] =  FocusDepth;
	//*******************************
	//计算折射点和偏转角度，用作显示用
	p->BeamExitPoint_x[p->k] = refract_x   ;
	p->BeamExitPoint_y[p->k] = -refract_y  ;
	p->BeamExitPoint_z[p->k] = 0           ;
	p->BeamSkewAngle[p->k]   =  atan((y0 - refract_y)/(refract_x - x0))*180/PI  ;
	desirex = refract_x -x0; desirey = refract_y- y0 ; desirez = -z0  ;
	aa1 = -desirex+tan(wedgeangle)*desirez ;
	aa2 = sqrt(desirex*desirex+desirey*desirey+desirez*desirez)*sqrt(1+tan(wedgeangle)*tan(wedgeangle));
	p->BeamPriSteeringAngle[p->k] = acos(aa1/aa2)*180/PI -90  ;
	//*******************************
	 }	 
     //first element location 
     p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
     p->probe_z[p->k][0] =   -h0  ;	  
     for(i = ElementStart; i< ElementStop; i++)
     {
            //  get each element coordinates
            zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle);
            xTemp = p->probe_x[p->k][0]  + Intervalx*cos(wedgeangle)*i;
            yTemp = p->probe_y[p->k][0];
            p->probe_x[p->k][i] = xTemp;
            p->probe_y[p->k][i] = yTemp;
            p->probe_z[p->k][i] = zTemp;   

			if(zTemp>=0)
            {    // kill special situation when probe under the detect surface
                 p->horizontal_x[p->k][i] = xTemp  ;
                 p->horizontal_y[p->k][i] = yTemp  ;
                 p->horizontal_z[p->k][i] = zTemp  ; 
				 continue ;
            }			
            xxx = xP-xTemp ;
            yyy = yP-yTemp ;
            if(a1 != 0&&roofangle!=0)
            {
                   // 直线1 :过探头与WEDGE垂直面与Z=0 的交线
                   // 直线2 :探头和焦点连线在Z=0面上的投影
                   // 探头的入射点为直线1和直线2的交点
                   if(xxx==0)
                   {
                       p->horizontal_x[p->k][i] = xTemp  ;
                       if(wedgeangle != 0) 
                       p->horizontal_y[p->k][i] = (xTemp-para1)/vx2+para2;
                       else 
                       p->horizontal_y[p->k][i] = desirey ;
                       p->horizontal_z[p->k][i] = 0 ; 					   
                   }
			       else
				   {
				       if(wedgeangle==0)
				       {
						   p->horizontal_y[p->k][i] = y0-fabs(z0)*tan(roofangle)			   ;
						   p->horizontal_x[p->k][i] = xP+(p->horizontal_y[p->k][i]-yP)*(xP-xTemp)/(yP-yTemp);
                           p->horizontal_z[p->k][i] = 0                                      ;
				       }
				       else
					   {
					       aa1 = yyy/xxx ;
				           aa2 = 1/vx2   ;
					       p->horizontal_x[p->k][i] = (aa2*para1-aa1*xP+yP-para2)/(aa2-aa1)  ;
                           p->horizontal_y[p->k][i] = (p->horizontal_x[p->k][i]-para1)/vx2+para2   ;
                           p->horizontal_z[p->k][i] = 0                                      ;
				       }
	
			       }				  
            }
            else
            {
                   //A1 ==0 时按一维ROOF角等于0的情况作搜索
                   TempLength = sqrt(xxx*xxx+yyy*yyy);
                   SearchLength = TempLength/2 ;
 
                   if(TempLength==0||zTemp==0)
                   {
                       p->horizontal_x[p->k][i] = xTemp  ;
                       p->horizontal_y[p->k][i] = yTemp  ;
                       p->horizontal_z[p->k][i] = 0      ;     
                       continue ;     
                   }                      
                   // do search in temporary coordinate
                   SearchPoint = 0 ;
                   TempResult =  0 ;
                   count = 0;                          
                   do
                   {
                       SearchPoint += SearchLength ;
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                   }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;      
                 
                 p->horizontal_x[p->k][i] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_y[p->k][i] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_z[p->k][i] = 0 ;  	
            }
     }  
     //calculate the transfer time
     for(i = ElementStart; i< ElementStop ; i++)
     {     
              dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])
                             +(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])*(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])
                             +(p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
              dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
                            +(p->horizontal_y[p->k][i]-p->focus_y[p->k])*(p->horizontal_y[p->k][i]-p->focus_y[p->k])
                            +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
              time[i] = dis1/c1+dis2/c2;          
     }
     // get the time delay
     timemax = time[ElementStart];//ElementStart
     //timemin = time[ElementStart];
     for(i = ElementStart; i< ElementStop ; i++)
     {
	      if(timemax < time[i])  timemax = time[i] ;
     }  
     
	 for(i = ElementStart; i< ElementStop ; i++)
     {
        p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);     
     }
	 // calc beam delay 
	 // beam delay equal center probe wedge transfer time plus center probe delay  
	 if((SelectColumn - 1)%2)
	 {
		 timemin = 2 * timemax -  time[ ElementStart + (SelectColumn -1)/2 ] - time[ ElementStart + (SelectColumn - 1)/2 + 1];
	 }
	 else
	 {
		 timemin =  2 * (timemax - time[ ElementStart + (SelectColumn -1)/2]) ;
	 }

     timemax = fabs(timemin) + 2.0 * fabs(z0/(cos(asin(sin(a1)*k1))*c1))  ;
     p->G_Time[p->k] = timemax;
     
     free(time);
}

/*
  Name:  flat_2d_roof
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   平面工件2D  聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void flat_2d_roof(PARAMETER_P data, double angle1, double angle2, double depth)
{
     PARAMETER_P p = (PARAMETER_P) data ;
     
     double c1 ;            // wave speed
     double c2 ;
     double k1 ;  
     double h0 =  p->wedge_p->wg_heigh_fir ;                  // first element high

     int PriElementStart = p->element_sel->primary_axis_s -1  ;
     int PriElementNum   = p->probe_p->ele_num_pri            ;
     int SecElementStart = p->element_sel->seconary_axis_s -1 ;
	 int SecElementNum   = p->probe_p->ele_num_sec            ;
	 int PriElementSelect= p->element_sel->pri_axis_ape       ;
	 int SecElementSelect= p->element_sel->sec_axis_ape       ;
     int PriElementStop  = PriElementStart +  PriElementSelect;
	 int SecElementStop  = SecElementStart +  SecElementSelect;

     double Intervaly = p->probe_p->sec_axis_pitch ;          // y direction interval
     double Intervalx = p->probe_p->pri_axis_pitch ;          // x direction interval distance
     double pri_ele_size = p->probe_p->pri_ele_size ;
     double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
     double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
     double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
     double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
     double  a1 = angle1*PI/180;                              // refract angle
     double  a2 = angle2*PI/180;                              // deflect angle

     double  roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle   y-z platform
     double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform     
     double  FocusDepth = depth;                              //  true focus depth
     
     double xTemp, yTemp, zTemp ;
     double x0,y0,z0;                                         // element center coordinate
     double xP, yP, zP;                                       // focus point coordinate
     //  element offset in each direction
     double x;
     double y;
     double z;
     double d;
     double s_x;
     double s_y;
     double s_zy;
     double s_zx;     
     // variable for search method
     double SearchLength , SearchPoint;
     double TempLength;
     double TempResult;
     double sina;
     double tana;    
     double sinb;
     double tanb;
     
     double sinTmp;
     double cosTmp;
     double tanTmp;
     double disTmp;
     
     double xxx;
     double yyy;
     int i, j; 
     int count;
     
     double dis1;
     double dis2;
     double timemax ;
	 // variable for calc all angle and refract point
	 double tempangle;
	 double desirex, desirey, desirez ;
	 double aa1 , aa2 , refract_len;

	 if(PriElementStop > PriElementNum )  return ;
	 if(SecElementStop > SecElementNum )  return ;	 
     double *time = malloc(PriElementNum*SecElementNum*sizeof(double));
     memset(time,0x0,PriElementNum*SecElementNum*sizeof(double));
     
     // kill the impossible condition
	 if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;  SecElementStart = 0; SecElementStop = 1;}
     if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
 
     if(p->probe_p->transmi_trans_longi==0)
     {   
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
     }                                           
     else                                 
     {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
     }
     // refract coefficient
     k1 =c1/c2 ;      

     // element location offset calculate
     // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
     // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
     if((wedgeangle != 0) && (roofangle != 0))
     {
          x = 1;
          y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
          z = y*tan(roofangle) ;
          d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
          s_x  = -Intervaly/d     ;
          s_y  = -Intervaly*y/d   ;
          s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;          
     }
     else
     {   
          s_y = -Intervaly*cos(roofangle);
          s_x = 0 ;
          s_zy = -Intervaly*sin(roofangle);
     }     
	 //calc offset generated by element selection
	 z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
	 x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
	 y0 = s_y*SecElementStart   ;

     // center probe element    (x0,y0,z0)            
     z0 = -(z0 + h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect - 1)/2);
     x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect - 1)*s_x/2 ;
     if(roofangle<0)
		 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
	 y0 = y0 -wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect - 1)/2 ;   
     // get focus point coordinate 
     if(a1<0)
     {  
        a1 = -a1 ;
        if(a2>=0)  a2 = a2-PI ;
		else       a2 = PI+a2 ;
     }// 通过此处变换使支持 A1 (-90,90) A2(-180,180)     

	 //*******************************
	 //计算折射点和偏转角度，用作显示用
	 if(a1 == 0)
	 {
		 p->BeamExitPoint_x[p->k] = x0  ;
		 p->BeamExitPoint_y[p->k] = y0  ;
	 }
	 else
	 {
	     tempangle = asin(sin(a1)*k1) ;
		 refract_len = -tan(tempangle) * z0 ;
		 p->BeamExitPoint_x[p->k] = x0  + refract_len * sin(a2);
		 p->BeamExitPoint_y[p->k] = y0  + refract_len * cos(a2);
	 }
	 p->BeamExitPoint_z[p->k] = 0   ;
	 p->BeamSkewAngle[p->k]  =  angle2 ;
	 desirex = p->BeamExitPoint_x[p->k] -x0; desirey = p->BeamExitPoint_y[p->k] - y0 ; desirez = -z0  ;
	 aa1 = -desirex+tan(wedgeangle)*desirez ;
	 aa2 = sqrt(desirex*desirex+desirey*desirey+desirez*desirez)*sqrt(1+tan(wedgeangle)*tan(wedgeangle));
	 p->BeamPriSteeringAngle[p->k] = acos(aa1/aa2)*180/PI -90  ;
	 aa1 = -desirey + tan(roofangle)*desirez ;
	 aa2 = sqrt(desirex*desirex+desirey*desirey+desirez*desirez)*sqrt(1+tan(roofangle)*tan(roofangle));
	 p->BeamSecSteeringAngle[p->k] = acos(aa1/aa2)*180/PI -90  ;
	 //*******************************

	 yTemp = sqrt(sin(a2)*sin(a2)*FocusDepth*FocusDepth*tan(a1)*tan(a1))  ;         
     if(a2==PI/2||a2==-PI/2)
        xTemp = 0;
     else
        xTemp = sqrt(tan(a1)*tan(a1)*FocusDepth*FocusDepth - yTemp*yTemp) ;   
	 if(a2 < 0)   yTemp= -yTemp ;
     if(xTemp==0)
     {
         sinTmp = sqrt(tan(a1)*tan(a1)/(1+tan(a1)*tan(a1)));
         sinTmp = sinTmp*k1;
         tanTmp = sqrt(sinTmp*sinTmp/(1-sinTmp*sinTmp));   
         if(yTemp == 0)
         {          
	           xP =  x0 ;
	           yP =  y0 ;
	           zP =  FocusDepth;
         }
         else
         {
	           disTmp = fabs(z0)*tanTmp;
	           xP =  x0 ;
	           yP =  y0  - yTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp);
	           zP =  FocusDepth;
         }
     }
     else
     {          
         if( (a2 > PI/2) || (a2 < -PI/2))  xTemp = -xTemp ;
         //            
         sinTmp = sqrt(tan(a1)*tan(a1)/(1+tan(a1)*tan(a1)));
         sinTmp = sinTmp*k1;
         tanTmp = sqrt(sinTmp*sinTmp/(1-sinTmp*sinTmp));
         //
         if(yTemp == 0)
         {
	           if(xTemp>=0)
	               xP =  x0 + xTemp + fabs(z0)*tanTmp;
	           else
	               xP =  x0 + xTemp +  z0*tanTmp ;
	           yP =  y0 ;
        }
        else
        {
	           disTmp = fabs(z0)*tanTmp;
	           xP =  x0  + xTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp) ;
	           yP =  y0  - yTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp) ;
	           zP =  FocusDepth;
        }
     }     
     p->focus_x[p->k] =  xP ;
     p->focus_y[p->k] =  yP ;
     p->focus_z[p->k] =  FocusDepth;        
     //first element location 
     p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
     p->probe_z[p->k][0] =   -h0  ;                 
     for(i = PriElementStart; i< PriElementStop; i++)
     {
           for(j = SecElementStart; j< SecElementStop; j++)
           {
                 //  get elements' coordinates
                 zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
                 xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
                 yTemp = p->probe_y[p->k][0] + s_y*j   ;
                 p->probe_x[p->k][i*SecElementNum+j] = xTemp;
                 p->probe_y[p->k][i*SecElementNum+j] = yTemp;
                 p->probe_z[p->k][i*SecElementNum+j] = zTemp;
                                  
                 xxx = xP-xTemp ;
                 yyy = yP-yTemp ;
                 TempLength = sqrt(xxx*xxx+yyy*yyy);
                 SearchLength = TempLength/2 ;
                 
                 if(TempLength==0||zTemp==0)
                 {
                       p->horizontal_x[p->k][i*SecElementNum+j] = xTemp  ;
                       p->horizontal_y[p->k][i*SecElementNum+j] = yTemp  ;
                       p->horizontal_z[p->k][i*SecElementNum+j] = 0 ;     
                       continue ;     
                 }                      
                 // do search in temporary coordinate
                 SearchPoint = 0 ;
                 TempResult =  0 ;
                 count = 0;                
				 // 在以探头坐标为X Y 方向原点,探头到焦点方向为X方向的坐标上
				 // 搜索合适的入射点
                 do
                 {
                       SearchPoint += SearchLength ;      
                       //printf("search point :%f \n",SearchPoint);
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb))  ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;                 
                 // translate the search result  into common coordiniate
                 p->horizontal_x[p->k][i*SecElementNum+j] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_y[p->k][i*SecElementNum+j] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_z[p->k][i*SecElementNum+j] = 0 ;
           }
     }  
     //calculate the transfer time
     for(i=PriElementStart; i< PriElementStop ; i++)
     {
         for(j = SecElementStart; j< SecElementStop; j++)
         {
              dis1 = sqrt((p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])*(p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])
                             +(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])*(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])
                            +(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j])*(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j]));
              dis2 = sqrt((p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])
                            +(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])
                            +(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k]));
              time[i*SecElementNum+j] = dis1/c1+dis2/c2;
         }
     }
     // get the time delay
     timemax = time[PriElementStart*SecElementNum + SecElementStart]; 

     for(i=PriElementStart; i< PriElementStop ; i++)
     {
         for(j = SecElementStart; j< SecElementStop; j++)
         {
               if(timemax < time[i*SecElementNum + j ])  timemax = time[i*SecElementNum + j ] ;
         }
     }
/*
     print("***********\n")   ;
     print("    x1 x2 x3\n")  ;
     print("y1  0  3  6  \n") ;
     print("y2  1  4  7  \n") ;
     print("y3  2  5  8  \n") ;
     print("***********\n")   ;      
*/     
	 for(i=PriElementStart; i< PriElementStop ; i++)
     {
         for(j = SecElementStart; j< SecElementStop; j++)
         {
//              printf("No.[%d][%d],time = %f,Δt=%d\n",i+1,j+1,time[i*SecElementNum + j ],(int)((timemax - time[i*SecElementNum + j ])*1000000+0.5));
              p->timedelay[p->k][i*SecElementNum + j] = (int)((timemax - time[i*SecElementNum + j ])*1000000+0.5)                                  ;
         }
     }
     p->G_Time[p->k] = timemax;
     free(time);
}


/*
  Name:  flat_1d_roof_pr
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   平面工件1D 的双晶 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        depth : focus depth of sonic beam
*/

void flat_1d_pr(PARAMETER_P data, double angle1, double depth)
{
	PARAMETER_P p = (PARAMETER_P) data ;
	//**********************
	p->BeamRefAngle[p->k] = angle1;
	p->BeamSecSteeringAngle[p->k] = 0 ;
	//***********************
	double c1 ;			// wave speed
	double c2 ;
	double k1 ;  
	double h0 =  p->wedge_p->wg_heigh_fir ;				  // first element high
	int Column   = p->element_sel->pri_axis_ape;				  // element x direction numble
    if((Column) > (p->probe_p->ele_num_pri))
                  return;
	double Intervalx = p->probe_p->pri_axis_pitch ;		  // x direction interval distance
	double pri_ele_size = p->probe_p->pri_ele_size ;
	double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference ; /*主轴楔块参考位置*/
	double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference ; /*次轴楔块参考位置*/
	double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	double  a1 = angle1*PI/180;							  // refract angle	
	double  roofangle = p->wedge_p->wg_roof_angle*PI/180	; // roofangle	 y-z platform
	double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180	; // wedgeangle  x-z platform	  
    double separation = p->wedge_r->wg_separation          ;

	
	double  FocusDepth = depth;							  //  true focus depth
	double xTemp, yTemp, zTemp ;
	double x0,y0,z0;										  // element center coordinate
	double xP, yP, zP; 									  // focus point coordinate
	// get focus point coordinate		   
	double tanw;
	double tanr;
	double para1,para2;
	double vx1,vy1,vz1,vx2,vy2,vz2;
	double desirex,desirey,desirez;
	double min_angle1 ;
	double min_angle2 ;
	double refract_x;
	double refract_y;
	double tempR;
    double len1;
	double len2;
	double len3;
	// variable for search method  // 
    double SearchLength , SearchPoint;
    double TempLength;
    double TempResult;
    double sina;
    double tana;    
    double sinb;
    double tanb;     
    double sinTmp;
    double cosTmp;
    double tanTmp;
    double disTmp;     
    double xxx;
    double yyy;
    int i, j; 
    int count;
	// variable for refract point
    double aa1,bb1;
	double aa2,bb2;
	//  time delay calculation
	double dis1;
	double dis2;
	double timemax ;
	double *time = malloc(Column*sizeof(double));
	memset(time,0x0,Column*sizeof(double));
	
	// kill the impossible condition
	if(Column<=0) return ;
	if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
	
	if(p->probe_p->transmi_trans_longi==0)
    {   
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
    }                                           
    else                                 
    {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
    }

	// refract coefficient
	k1 =c1/c2 ; 	  
	// center probe element    (x0,y0,z0)			 
	z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2);
	x0 = wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(Column-1)*cos(wedgeangle)/2;
    if(roofangle<0)
		 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;a1=-a1;}
	y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir;
    // get focus point coordinate 
    // desirex desirey desirez : 入射点坐标          
    if(roofangle == 0)
    {
          TempLength = FocusDepth*tan(a1) ;
		  p->focus_x[p->k] =  x0+TempLength ;
		  TempLength =  fabs(z0)*tan(asin(a1*k1));
		  p->focus_x[p->k] += TempLength ;
		  p->focus_y[p->k] =  y0 ;
          p->focus_z[p->k] =  FocusDepth;
          xP = p->focus_x[p->k];
          yP = p->focus_y[p->k];
          zP = p->focus_z[p->k];
		  p->BeamExitPoint_x[p->k] = x0 + TempLength  ;
		  p->BeamExitPoint_y[p->k] = -y0              ;
		  p->BeamExitPoint_z[p->k] = 0                ;
		  p->BeamSkewAngle[p->k]   = 0 ;
		  p->BeamPriSteeringAngle[p->k] = (atan(-TempLength/z0) - wedgeangle)*180/PI  ;
    }
	else
	{
	    if(wedgeangle==0)
        {
            desirex = x0 ;
	   		desirey = y0-fabs(z0)*tan(roofangle);
	   		desirez = 0;
    	}
		else
		{
	    		tanw = -tan(wedgeangle);
	            tanr = tan(roofangle)  ;
	            vx1  = tanw*tanr       ;
	            vy1  = -(1+tanw*tanw)  ;
	            vz1  = -tanr           ;
		        // (x-para1)/vx2 = y-para2   
	     		//过探头与楔块面垂直面与Z=0面的交线方程
	    		para1 = x0             ;
	    		para2 = y0 + z0*vz1/vy1;
	    		vx2 = -vy1/vx1         ;
	    		//中心探头与交线的垂直点
	    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
	    		desirey = para2 + (desirex - para1)/vx2 ;
	    		desirez = 0 ;
		}
    		// 求最小入射角
		min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
    	min_angle2 = sin(min_angle1)/k1;
    	if(min_angle2>=1)
    	{
        // 如果最小入射角发生全反射取焦点为中心探头坐标
        	xP = x0;
        	yP = y0;
        	a1 = 0 ;
    	}
		else 
		{
	      if(asin(min_angle2)>=fabs(a1))
	      {
	          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
	          refract_x = desirex;
		      refract_y = desirey;
		      a1 = min_angle2 ;
	      }
          else
          {
              // 计算正常情况下焦点的坐标
              //refract point 折射点坐标 
              min_angle2 = asin(sin(a1)*k1);
              tempR = z0*tan(min_angle2);
		      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
		      if(a1>=0)
		      {
                 refract_x = desirex-TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey-TempLength/sqrt(1+vx2*vx2);
              }
              else
              {
                 refract_x = desirex+TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey+TempLength/sqrt(1+vx2*vx2);
              }
          }		  
          TempLength = FocusDepth*tan(a1) ;
		  len1 = refract_x - x0           ;
		  len2 = refract_y - y0           ;
		  len3 = sqrt(len1*len1+len2*len2);
          if(a1>=0)
          {
                xP = refract_x+TempLength*len1/len3;
		        yP = refract_y+TempLength*len2/len3;
          }
          else
          {
                xP = refract_x-TempLength*len1/len3;
		        yP = refract_y-TempLength*len2/len3;
          }
          }
          p->focus_x[p->k] =  xP ;
          p->focus_y[p->k] =  yP ;
          p->focus_z[p->k] =  FocusDepth;
		  //*******************************
		  p->BeamExitPoint_x[p->k] = refract_x   ;
		  p->BeamExitPoint_y[p->k] = refract_y  ;
		  p->BeamExitPoint_z[p->k] = 0           ;
		  p->BeamSkewAngle[p->k]   =  atan((y0 - refract_y)/(refract_x - x0))*180/PI  ;
          desirex = refract_x -x0; desirey = refract_y- y0 ; desirez = -z0  ;
		  aa1 = -desirex+tan(wedgeangle)*desirez ;
		  aa2 = sqrt(desirex*desirex+desirey*desirey+desirez*desirez)*sqrt(1+tan(wedgeangle)*tan(wedgeangle));
		  p->BeamPriSteeringAngle[p->k] = acos(aa1/aa2)*180/PI -90  ;
		  //*******************************
	 }
     //first element location 
     //print("In FocusLaw refract_x %f ,refract_y %f \n", refract_x, refract_y);
     p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
     p->probe_z[p->k][0] =   -h0  ;
	  
     for(i = 0; i< Column; i++)
     {
            //  get each element coordinates
            zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle);
            xTemp = p->probe_x[p->k][0] +  Intervalx*cos(wedgeangle)*i;
            yTemp = p->probe_y[p->k][0];
            p->probe_x[p->k][i] = xTemp;
            p->probe_y[p->k][i] = yTemp;
            p->probe_z[p->k][i] = zTemp;   

			if(zTemp>=0)
            {    // kill special situation when probe under the detect surface
                 p->horizontal_x[p->k][i] = xTemp  ;
                 p->horizontal_y[p->k][i] = yTemp  ;
                 p->horizontal_z[p->k][i] = zTemp  ; 
				 continue ;
            }			
            xxx = xP-xTemp ;
            yyy = yP-yTemp ;
            if(a1 != 0&&roofangle!=0)
            {
                   // 直线1 :过探头与WEDGE垂直面与Z=0 的交线
                   // 直线2 :探头和焦点连线在Z=0面上的投影
                   // 探头的入射点为直线1和直线2的交点
                   if(xxx==0)
                   {
                       p->horizontal_x[p->k][i] = xTemp  ;
                       if(wedgeangle != 0) 
                       p->horizontal_y[p->k][i] = (xTemp-para1)/vx2+para2;
                       else 
                       p->horizontal_y[p->k][i] = desirey ;
                       p->horizontal_z[p->k][i] = 0 ; 					   
                   }
			       else
				   {
				       if(wedgeangle==0)
				       {
						   p->horizontal_y[p->k][i] = y0-fabs(z0)*tan(roofangle)			   ;
						   p->horizontal_x[p->k][i] = xP+(p->horizontal_y[p->k][i]-yP)*(xP-xTemp)/(yP-yTemp);
                           p->horizontal_z[p->k][i] = 0                                      ;
				       }
				       else
					   {
					       aa1 = yyy/xxx ;
				           aa2 = 1/vx2   ;
					       p->horizontal_x[p->k][i] = (aa2*para1-aa1*xP+yP-para2)/(aa2-aa1)  ;
                           p->horizontal_y[p->k][i] = (p->horizontal_x[p->k][i]-para1)/vx2+para2   ;
                           p->horizontal_z[p->k][i] = 0                                      ;
				       }
	
			       }				  
            }
            else
            {
                   //A1 ==0 时按一维ROOF角等于0的情况作搜索
                   TempLength = sqrt(xxx*xxx+yyy*yyy);
                   SearchLength = TempLength/2 ;
 
                   if(TempLength==0||zTemp==0)
                   {
                       p->horizontal_x[p->k][i] = xTemp  ;
                       p->horizontal_y[p->k][i] = yTemp  ;
                       p->horizontal_z[p->k][i] = 0      ;     
                       continue ;     
                   }                      
                   // do search in temporary coordinate
                   SearchPoint = 0 ;
                   TempResult =  0 ;
                   count = 0;                          
                   do
                   {
                       SearchPoint += SearchLength ;      
                       //printf("search point :%f \n",SearchPoint);
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                   }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;      
                 
                 p->horizontal_x[p->k][i] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_y[p->k][i] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_z[p->k][i] = 0 ;  	
            }
     }  
     //calculate the transfer time
     for(i=0;i<Column;i++)
     {     
              dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])
                             +(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])*(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])
                             +(p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
              dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
                            +(p->horizontal_y[p->k][i]-p->focus_y[p->k])*(p->horizontal_y[p->k][i]-p->focus_y[p->k])
                            +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
              time[i] = dis1/c1+dis2/c2;          
     }
     // get the time delay
     timemax = time[0];
     for(i=0;i<Column;i++)
     {
           if(timemax < time[i])  timemax = time[i] ;                   
     } 
//     printf("\nTransmitte\n");
     for(i = 0 ;i<Column ; i++)
     {
          
//          printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
          p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
     }

	h0 =  p->wedge_r->wg_heigh_fir ;				  // first element high
	Column   = p->element_sel->pri_axis_ape;				  // element x direction numble
    if( Column > p->probe_p->ele_num_pri )
                  return;
	Intervalx = p->probe_p->pri_axis_pitch ;		  // x direction interval distance
	pri_ele_size = p->probe_p->pri_ele_size ;
	wg_pri_axis_reference  = p->wedge_r->wg_pri_axis_reference; /*主轴楔块参考位置*/
	wg_sec_axis_reference  = p->wedge_r->wg_sec_axis_reference; /*次轴楔块参考位置*/
	wg_pri_elem_offset_fir = p->wedge_r->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	wg_sec_elem_offset_fir = p->wedge_r->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	roofangle = p->wedge_r->wg_roof_angle*PI/180	; // roofangle	 y-z platform
	wedgeangle= p->wedge_r->wg_wedge_angle*PI/180	; // wedgeangle  x-z platform	  
    a1 = angle1*PI/180;
	z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2);
	x0 = wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(Column-1)*cos(wedgeangle)/2;
    if(roofangle<0)
		 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir; a1 = -a1;}
	
	y0 = y0-separation ;
    // get focus point coordinate 
    // desirex desirey desirez : 入射点坐标          
    if(roofangle == 0)
    {
          TempLength = FocusDepth*tan(a1) ;
		  p->focus_xr[p->k] =  x0+TempLength ;
		  TempLength =  fabs(z0)*tan(asin(a1*k1));
		  p->focus_xr[p->k] += TempLength ;
		  p->focus_yr[p->k] =  y0 ;
          p->focus_zr[p->k] =  FocusDepth;
          xP = p->focus_xr[p->k];
          yP = p->focus_yr[p->k];
          zP = p->focus_zr[p->k];
    }
	else
	{
	    if(wedgeangle==0)
        {
            desirex = x0 ;
	   		desirey = y0-fabs(z0)*tan(roofangle);
	   		desirez = 0;
    	}
		else
		{
	    		tanw = -tan(wedgeangle);
	            tanr = tan(roofangle)  ;
	            vx1  = tanw*tanr       ;
	            vy1  = -(1+tanw*tanw)  ;
	            vz1  = -tanr           ;
		        // (x-para1)/vx2 = y-para2   
	     		//过探头与楔块面垂直面与Z=0面的交线方程
	    		para1 = x0             ;
	    		para2 = y0 + z0*vz1/vy1;
	    		vx2 = -vy1/vx1         ;
	    		//中心探头与交线的垂直点
	    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
	    		desirey = para2 + (desirex - para1)/vx2 ;
	    		desirez = 0 ;
		}
    		// 求最小入射角
		min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
    	min_angle2 = sin(min_angle1)/k1;
    	if(min_angle2>=1)
    	{
        // 如果最小入射角发生全反射取焦点为中心探头坐标
        	xP = x0;
        	yP = y0;
        	a1 = 0 ;
    	}
		else 
		{
	      if(asin(min_angle2)>=fabs(a1))
	      {
	          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
	          refract_x = desirex;
		      refract_y = desirey;
		      a1 = min_angle2 ;
	      }
          else
          {
              // 计算正常情况下焦点的坐标
              //refract point 折射点坐标 
              min_angle2 = asin(sin(a1)*k1);
              tempR = z0*tan(min_angle2);
		      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
		      if(a1>=0)
		      {
                 refract_x = desirex-TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey-TempLength/sqrt(1+vx2*vx2);
              }
              else
              {
                 refract_x = desirex+TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey+TempLength/sqrt(1+vx2*vx2);
              }
          }		  
          TempLength = FocusDepth*tan(a1) ;
		  len1 = refract_x - x0;
		  len2 = refract_y - y0;
		  len3 = sqrt(len1*len1+len2*len2);
          if(a1>=0)
          {
                xP = refract_x+TempLength*len1/len3;
		        yP = refract_y+TempLength*len2/len3;
          }
          else
          {
                xP = refract_x-TempLength*len1/len3;
		        yP = refract_y-TempLength*len2/len3;
          }
          }
          p->focus_xr[p->k] =  xP ;
          p->focus_yr[p->k] =  yP ;
          p->focus_zr[p->k] =  FocusDepth;
	 }
     //first element location 
     p->probe_xr[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_yr[p->k][0] =   p->probe_y[p->k][0]-separation;
     p->probe_zr[p->k][0] =   -h0  ;
	  
     for(i = 0; i< Column; i++)
     {
            //  get each element coordinates
            zTemp = p->probe_zr[p->k][0]  - i*Intervalx*sin(wedgeangle);
            xTemp = p->probe_xr[p->k][0] +  Intervalx*cos(wedgeangle)*i;
            yTemp = p->probe_yr[p->k][0];
            p->probe_xr[p->k][i] = xTemp;
            p->probe_yr[p->k][i] = yTemp;
            p->probe_zr[p->k][i] = zTemp;   

			if(zTemp>=0)
            {    // kill special situation when probe under the detect surface
                 p->horizontal_xr[p->k][i] = xTemp  ;
                 p->horizontal_yr[p->k][i] = yTemp  ;
                 p->horizontal_zr[p->k][i] = zTemp  ; 
				 continue ;
            }			
            xxx = xP-xTemp ;
            yyy = yP-yTemp ;
            if(a1 != 0&&roofangle!=0)
            {
                   // 直线1 :过探头与WEDGE垂直面与Z=0 的交线
                   // 直线2 :探头和焦点连线在Z=0面上的投影
                   // 探头的入射点为直线1和直线2的交点
                   if(xxx==0)
                   {
                       p->horizontal_xr[p->k][i] = xTemp  ;
                       if(wedgeangle != 0) 
                       p->horizontal_yr[p->k][i] = (xTemp-para1)/vx2+para2;
                       else 
                       p->horizontal_yr[p->k][i] = desirey ;
                       p->horizontal_zr[p->k][i] = 0 ; 					   
                   }
			       else
				   {
				       if(wedgeangle==0)
				       {
						   p->horizontal_yr[p->k][i] = y0-fabs(z0)*tan(roofangle)			   ;
						   p->horizontal_xr[p->k][i] = xP+(p->horizontal_yr[p->k][i]-yP)*(xP-xTemp)/(yP-yTemp);
                           p->horizontal_zr[p->k][i] = 0                                      ;
				       }
				       else
					   {
					       aa1 = yyy/xxx ;
				           aa2 = 1/vx2   ;
					       p->horizontal_xr[p->k][i] = (aa2*para1-aa1*xP+yP-para2)/(aa2-aa1)  ;
                           p->horizontal_yr[p->k][i] = (p->horizontal_xr[p->k][i]-para1)/vx2+para2   ;
                           p->horizontal_zr[p->k][i] = 0                                      ;
				       }
	
			       }				  
            }
            else
            {
                   //A1 ==0 时按一维ROOF角等于0的情况作搜索
                   TempLength = sqrt(xxx*xxx+yyy*yyy);
                   SearchLength = TempLength/2 ;
 
                   if(TempLength==0||zTemp==0)
                   {
                       p->horizontal_xr[p->k][i] = xTemp  ;
                       p->horizontal_yr[p->k][i] = yTemp  ;
                       p->horizontal_zr[p->k][i] = 0      ;     
                       continue ;     
                   }                      
                   // do search in temporary coordinate
                   SearchPoint = 0 ;
                   TempResult =  0 ;
                   count = 0;                          
                   do
                   {
                       SearchPoint += SearchLength ;      
                       //printf("search point :%f \n",SearchPoint);
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                   }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;      
                 
                 p->horizontal_xr[p->k][i] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_yr[p->k][i] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_zr[p->k][i] = 0 ;  	
            }
     }  
     //calculate the transfer time
     for(i=0;i<Column;i++)
     {     
              dis1 = sqrt((p->probe_xr[p->k][i]-p->horizontal_xr[p->k][i])*(p->probe_xr[p->k][i]-p->horizontal_xr[p->k][i])
                             +(p->probe_yr[p->k][i]-p->horizontal_yr[p->k][i])*(p->probe_yr[p->k][i]-p->horizontal_yr[p->k][i])
                             +(p->probe_zr[p->k][i]-p->horizontal_zr[p->k][i])*(p->probe_zr[p->k][i]-p->horizontal_zr[p->k][i]));
              dis2 = sqrt((p->horizontal_xr[p->k][i]-p->focus_xr[p->k])*(p->horizontal_xr[p->k][i]-p->focus_xr[p->k])
                            +(p->horizontal_yr[p->k][i]-p->focus_yr[p->k])*(p->horizontal_yr[p->k][i]-p->focus_yr[p->k])
                            +(p->horizontal_zr[p->k][i]-p->focus_zr[p->k])*(p->horizontal_zr[p->k][i]-p->focus_zr[p->k]));
              time[i] = dis1/c1+dis2/c2;          
     }
     // get the time delay
     timemax = time[0];
     for(i=0;i<Column;i++)
     {
           if(timemax < time[i])  timemax = time[i] ;                   
     } 
//     printf("\nReceviver\n");
     for(i = 0 ;i<Column ; i++)
     {
          
//          printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
          p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
     }
     p->G_Time[p->k] = timemax;
	free(time);
}
/*
  Name:  flat_2d_roof_pr
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   平面工件2D 的双晶 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void flat_2d_roof_pr(PARAMETER_P data, double angle1, double angle2, double depth)
{
     PARAMETER_P p = (PARAMETER_P) data ;
     
     double c1 ;            // wave speed
     double c2 ;
     double k1 ;  
     double h0 =  p->wedge_p->wg_heigh_fir ;                  // first element high
     int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
     int Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
     if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
            return;
     
     double Intervaly = p->probe_p->sec_axis_pitch ;          // y direction interval
     double Intervalx = p->probe_p->pri_axis_pitch ;          // x direction interval distance
     double pri_ele_size = p->probe_p->pri_ele_size ;
     double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
     double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
     double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
     double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	 double separation  = p->wedge_r->wg_separation ;

     double  a1 = angle1*PI/180;                              // refract angle
     double  a2 = angle2*PI/180;                              // deflect angle
 
     double  roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle   y-z platform
     double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform     
     double  FocusDepth = depth;                              //  true focus depth
     
     double xTemp, yTemp, zTemp ;
     double x0,y0,z0;                                         // element center coordinate
     double xP, yP, zP;                                       // focus point coordinate
     //  element offset in each direction
     double x;
     double y;
     double z;
     double d;
     double s_x;
     double s_y;
     double s_zy;
     double s_zx;     
     // variable for search method
     double SearchLength , SearchPoint;
     double TempLength;
     double TempResult;
     double sina;
     double tana;    
     double sinb;
     double tanb;
     
     double sinTmp;
     double cosTmp;
     double tanTmp;
     double disTmp;
     
     double xxx;
     double yyy;
     int i, j; 
     int count;
     
     double dis1;
     double dis2;
     double timemax ;
	 double timemax1;
     double *time = malloc(2*Row*Column*sizeof(double));
     memset(time,0x0,2*Row*Column*sizeof(double));
     
     // kill the impossible condition
     if(Column<=0||Row<=0) return ;
	 if(p->probe_p->D1_D2 == 0)  Row = 1 ;
     if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
 
     if(p->probe_p->transmi_trans_longi==0)
    {   
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
    }                                           
    else                                 
    {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
    }
     // refract coefficient
     k1 =c1/c2 ;      

     // element location offset calculate
     if((wedgeangle != 0) && (roofangle != 0))
     {
          x = 1;
          y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
          z = y*tan(roofangle) ;
          d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
          s_x  = -Intervaly/d    ;
          s_y  = -Intervaly*y/d   ;
          s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;          
     }
     else
     {   
          s_y = -Intervaly*cos(roofangle);
          s_x = 0 ;
          s_zy = -Intervaly*sin(roofangle);
     }     
     // center probe element    (x0,y0,z0)            
     
     z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2 - s_zy*(Row-1)/2);
     x0 = wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(Column - 1)*cos(wedgeangle)/2 + (Row-1)*s_x/2 ;
     // y0   i s the middle point of two wedge
	 y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir - separation/2 ;   

    // get focus point coordinate 
     if(a1<0)
     {  a1 = -a1 ;
        if(a2>=0)  a2 = a2-PI ;
		else       a2 = PI+a2 ;
     }// 通过此处变换使支持 A1 (-90,90) A2(-180,180)          
     yTemp = sqrt(sin(a2)*sin(a2)*FocusDepth*FocusDepth*tan(a1)*tan(a1))  ;         
     if(a2==PI/2||a2==-PI/2)
        xTemp = 0;
     else
        xTemp = sqrt(tan(a1)*tan(a1)*FocusDepth*FocusDepth - yTemp*yTemp) ;   
	 if(a2 < 0)   yTemp= -yTemp ;
     if(xTemp==0)
     {
         sinTmp = sqrt(tan(a1)*tan(a1)/(1+tan(a1)*tan(a1)));
         sinTmp = sinTmp*k1;
         tanTmp = sqrt(sinTmp*sinTmp/(1-sinTmp*sinTmp));   
         if(yTemp == 0)
         {          
           xP =  x0 ;
           yP =  y0 ;
           zP =  FocusDepth;
         }
         else
         {
           disTmp = fabs(z0)*tanTmp;
           xP =  x0 ;
           yP =  y0  - yTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp);
           zP =  FocusDepth;
         }
     }
     else
     {          
         if( (a2 > PI/2) || (a2 < -PI/2))  xTemp = -xTemp ;
         //            
         sinTmp = sqrt(tan(a1)*tan(a1)/(1+tan(a1)*tan(a1)));
         sinTmp = sinTmp*k1;
         tanTmp = sqrt(sinTmp*sinTmp/(1-sinTmp*sinTmp));
         //
         if(yTemp == 0)
         {
           if(xTemp>=0)
               xP =  x0 + xTemp + fabs(z0)*tanTmp;
           else
               xP =  x0 + xTemp +  z0*tanTmp     ;
           yP =  y0 ;
        }
        else
        {
           disTmp = fabs(z0)*tanTmp;
           xP =  x0  + xTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp) ;
           yP =  y0  - yTemp*(sqrt(xTemp*xTemp+yTemp*yTemp)+disTmp)/sqrt(xTemp*xTemp+yTemp*yTemp) ;
           zP =  FocusDepth;
        }
     }         
     p->focus_x[p->k] =  xP ;
     p->focus_y[p->k] =  yP ;
     p->focus_z[p->k] =  FocusDepth;        
	 p->focus_xr[p->k] =  xP ;
     p->focus_yr[p->k] =  yP ;
     p->focus_zr[p->k] =  FocusDepth;        
     //first element location 
     p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
     p->probe_z[p->k][0] =   -h0  ;    
                
     for(i = 0; i< Column; i++)
     {
           for(j = 0; j< Row; j++)
           {
                 //  get elements' coordinates
                 zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
                 xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
                 yTemp = p->probe_y[p->k][0] + s_y*j   ;
                 p->probe_x[p->k][i*Row+j] = xTemp;
                 p->probe_y[p->k][i*Row+j] = yTemp;
                 p->probe_z[p->k][i*Row+j] = zTemp;
                                  
                 xxx = xP-xTemp ;
                 yyy = yP-yTemp ;
                 TempLength = sqrt(xxx*xxx+yyy*yyy);
                 SearchLength = TempLength/2 ;
                 
                 if(TempLength==0||zTemp==0)
                 {
                       p->horizontal_x[p->k][i*Row+j] = xTemp  ;
                       p->horizontal_y[p->k][i*Row+j] = yTemp  ;
                       p->horizontal_z[p->k][i*Row+j] = 0 ;     
                       continue ;     
                 }                      
                 // do search in temporary coordinate
                 SearchPoint = 0 ;
                 TempResult  = 0 ;
                 count = 0;                          
                 do
                 {
                       SearchPoint += SearchLength ;      
                       //printf("search point :%f \n",SearchPoint);
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;                 
                 // translate into common coordiniate
                 p->horizontal_x[p->k][i*Row+j] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_y[p->k][i*Row+j] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_z[p->k][i*Row+j] = 0 ;                 
           }
     }  
	 for(i=0;i<Column;i++)
     {
         for(j =0;j<Row; j++)
         {
              dis1 = sqrt((p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])*(p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])
                             +(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])*(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])
                            +(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j])*(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j]));
              dis2 = sqrt((p->horizontal_x[p->k][i*Row+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*Row+j]-p->focus_x[p->k])
                            +(p->horizontal_y[p->k][i*Row+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*Row+j]-p->focus_y[p->k])
                            +(p->horizontal_z[p->k][i*Row+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*Row+j]-p->focus_z[p->k]));
              time[i*Row+j] = dis1/c1+dis2/c2;
         }
     }

	 timemax = time[0];
     for(i=0;i<Column*Row;i++)
     {
           if(timemax < time[i])  timemax = time[i] ;                   
     }
/*     print("*************\n") ;
     print("    x1 x2 x3 \n")  ;
     print("y1  0  3  6  \n") ;
     print("y2  1  4  7  \n") ;
     print("y3  2  5  8  \n") ;
     print("***********\n");      
     
     print("Signal Element:\n");*/
     for(i = 0 ;i<Column*Row ; i++)
     {
//          printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
          p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
     }

     h0 =  p->wedge_r->wg_heigh_fir ;                  // first element high
     Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
     Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
     if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
                  return;
     
     Intervaly = p->probe_p->sec_axis_pitch ;          // y direction interval
     Intervalx = p->probe_p->pri_axis_pitch ;          // x direction interval distance
     pri_ele_size = p->probe_p->pri_ele_size ;
     wg_pri_axis_reference  = p->wedge_r->wg_pri_axis_reference; /*主轴楔块参考位置*/
     wg_sec_axis_reference  = p->wedge_r->wg_sec_axis_reference; /*次轴楔块参考位置*/
     wg_pri_elem_offset_fir = p->wedge_r->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
     wg_sec_elem_offset_fir = p->wedge_r->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	 roofangle = p->wedge_r->wg_roof_angle*PI/180   ; // roofangle   y-z platform
     wedgeangle= p->wedge_r->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform     

     if((wedgeangle != 0) && (roofangle != 0))
     {
          x = 1;
          y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
          z = y*tan(roofangle) ;
          d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
          s_x  = -Intervaly*x/d    ;
          s_y  = Intervaly*y/d     ;
          s_zy  = -Intervaly*(z-x*tan(wedgeangle))/d   ;          
     }
     else
     {   
          s_y = -Intervaly*cos(roofangle);
          s_x = 0 ;
          s_zy = -Intervaly*sin(roofangle);
     }  
	 
	 p->probe_xr[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
     p->probe_yr[p->k][0] =   p->probe_y[p->k][0]  - separation;
     p->probe_zr[p->k][0] =   -h0  ;    
                
     for(i = 0; i< Column; i++)
     {
           for(j = 0; j< Row; j++)
           {
                 //  get elements' coordinates
                 zTemp = p->probe_zr[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
                 xTemp = p->probe_xr[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
                 yTemp = p->probe_yr[p->k][0] - s_y*j   ;
                 p->probe_xr[p->k][i*Row+j] = xTemp;
                 p->probe_yr[p->k][i*Row+j] = yTemp;
                 p->probe_zr[p->k][i*Row+j] = zTemp;
                                  
                 xxx = xP-xTemp ;
                 yyy = yP-yTemp ;
                 TempLength = sqrt(xxx*xxx+yyy*yyy);
                 SearchLength = TempLength/2 ;
                 
                 if(TempLength==0||zTemp==0)
                 {
                       p->horizontal_xr[p->k][i*Row+j] = xTemp  ;
                       p->horizontal_yr[p->k][i*Row+j] = yTemp  ;
                       p->horizontal_zr[p->k][i*Row+j] = 0 ;     
                       continue ;     
                 }                      
                 // do search in temporary coordinate
                 SearchPoint = 0 ;
                 TempResult =  0 ;
                 count = 0;                          
                 do
                 {
                       SearchPoint += SearchLength ;      
                       //printf("search point :%f \n",SearchPoint);
                       tana = SearchPoint/zTemp;
                       sina = sqrt(tana*tana/(1+tana*tana));
                       sinb = sina/k1;
					   
                       if(sinb>=1)  
                       {
                           SearchLength = -fabs(SearchLength)/2 ;                   
                       }
                       else
                       {
                           tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                           TempResult=SearchPoint+tanb*FocusDepth ;
                           if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                           if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
                       }                   
                       count++;         
                 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;                 
                 // translate into common coordiniate
                 p->horizontal_xr[p->k][i*Row+j] = xTemp + SearchPoint*xxx/TempLength ;
                 p->horizontal_yr[p->k][i*Row+j] = yTemp + SearchPoint*yyy/TempLength ;
                 p->horizontal_zr[p->k][i*Row+j] = 0 ;                 
           }
     }  
     //calculate the transfer time

	 //calculate the transfer time
     for(i=0;i<Column;i++)
     {
         for(j =0;j<Row; j++)
         {
              dis1 = sqrt((p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])*(p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])
                             +(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])*(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])
                            +(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j])*(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j]));
              dis2 = sqrt((p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])*(p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])
                            +(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])*(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])
                            +(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k])*(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k]));
              time[i*Row+j] = dis1/c1+dis2/c2;          
         }
     }
     // get the time delay

	 timemax1 = time[0] ;
	 for(i=0;i<Column*Row;i++)
     {
           if(timemax1 < time[i])  timemax1 = time[i] ;                   
     }

//	 print("Receive Element:\n") ;
	 for(i = 0 ;i<Column*Row ; i++)
     {
//          printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
          p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
     }
     p->G_Time[p->k] = timemax;
     free(time);
}

/*
  Name:  cylinder_2d_roof_od
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   2 DIMENSION 带ROOF 角的 OD 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void cylinder_2d_roof_od (PARAMETER_P data, double angle1, double angle2, double depth)
{
		 PARAMETER_P p = (PARAMETER_P) data ;
		 
		 double c1 ;			 // wave speed
		 double c2 ;
		 double k1 ;  
		 double r  ;
		 double h0 =  p->wedge_p->wg_heigh_fir ;				   // first element high
//		 int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
//         int Column   =  p->element_sel->pri_axis_ape;                  // element x direction numble
//         if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
//            return;
		 int PriElementStart = p->element_sel->primary_axis_s - 1  ;
		 int PriElementNum	 = p->probe_p->ele_num_pri			  ;
		 int SecElementStart = p->element_sel->seconary_axis_s -1 ;
		 int SecElementNum	 = p->probe_p->ele_num_sec			  ;
		 int PriElementSelect= p->element_sel->pri_axis_ape 	  ;
		 int SecElementSelect= p->element_sel->sec_axis_ape 	  ;
		 int PriElementStop  = PriElementStart +  PriElementSelect;
		 int SecElementStop  = SecElementStart +  SecElementSelect;
		 
		 double Intervaly = p->probe_p->sec_axis_pitch ;		   // y direction interval
		 double Intervalx = p->probe_p->pri_axis_pitch ;		   // x direction interval distance
		 double pri_ele_size = p->probe_p->pri_ele_size ;        
		 double wg_pri_axis_reference	= p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
		 double wg_sec_axis_reference	= p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
		 double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
		 double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
         double length = p->wedge_p->wg_length ;                              // wedge length
		 double CylinderExtDiameter = p->specimen->speci_outside_cylindrical ;// outside cylinder diameter
		 	  
		 double  a1 = fabs(angle1*PI/180);						   // refract angle
		 double  a2 = fabs(angle2*PI/180);						   // deflect angle
	     if(a2>PI/2) a2 = PI-a2 ;
		 
		 double  roofangle = p->wedge_p->wg_roof_angle*PI/180	 ; // roofangle   y-z platform
		 double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180 ; // wedgeangle  x-z platform	   
		 double  FocusDepth = depth;							   //  true focus depth
 
		 // refraction calculation 
		 double aSinTemp;
		 double aProjection;
		 double bProjection;
		 double zHightTemp;
		 
		 double tempangle1;
		 double tempangle2;
		 double tempangle3;

		 double tempFocusxxx;
		 double tempFocusyyy;
		 double tempFocuszzz;
		 double angle_rotate;
		 
		 double xTemp, yTemp, zTemp ;
		 double x0,y0,z0;			  // element center coordinate
		 //  element offset in each direction
		 double x;
		 double y;
		 double z;
		 double d;
		 double s_x;
		 double s_y;
		 double s_zy;
		 double s_zx;	   
		 // variable for search method
		 double SearchLength , SearchAngle;
		 double TempAngle;
		 double TempResult;
		 double SkewAngleTemp;
		 double SearchPoint ;
		 double TempLength  ;
		 double tana,tanb,sina,sinb ;
		 
         double r_depth;
         double a;
		 double b;
		 
		 double xxx;
		 double yyy;
		 double zzz;
		 
		 int i, j; 
		 int count;
		 // variable for time calculation
		 double dis1;
		 double dis2;
		 double timemax ;
 
		double *time = malloc(PriElementNum*SecElementNum*sizeof(double));
		memset(time,0x0,PriElementNum*SecElementNum*sizeof(double));
	    if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;  SecElementStart = 0; SecElementStop = 1;}
		 
		if(PriElementStop > PriElementNum )  return ;
		if(SecElementStop > SecElementNum )  return ;	

		r = CylinderExtDiameter/2.0 ;
		// kill the impossible condition
		if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
	 
		if(p->probe_p->transmi_trans_longi==0)
        {   
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
        }                                           
        else                                 
        {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
        }
		 // refract coefficient
		 k1 = c1/c2 ;

		 // element location offset calculate
		 // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
         // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
		 if((wedgeangle != 0) && (roofangle != 0))
		 {
			  x = 1;
			  y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
			  z = y*tan(roofangle) ;
			  d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
			  s_x  = -Intervaly/d	 ;
			  s_y  = -Intervaly*y/d   ;
			  s_zy	= -Intervaly*(z-tan(wedgeangle))/d	 ;			
		 }
		 else
		 {	 
			  s_y = -Intervaly*cos(roofangle);
			  s_x = 0 ;
			  s_zy = -Intervaly*sin(roofangle);
		 }	   

	     //calc offset generated by element selection
		 z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
		 x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
		 y0 = s_y*SecElementStart   ;

		 // center probe element	(x0,y0,z0)			  
		 z0 = -(z0 +h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect -1)/2) -sqrt(CylinderExtDiameter*CylinderExtDiameter - length*length)/2;
		 x0 = x0 -length/2 + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect-1)*s_x/2 ;
         if(roofangle<0)
		     wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
		 y0 = y0 - wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect-1)/2 ;
         //以下和1维Cylinder情况类似.
         //只是将3维的坐标投影到X-Z面
         //空间角度和投影角偏转角之间的关系
         // tan(Projection) = tan(SpaceAngle)*cos(SkewAngle);
		 // get focus point coordinate 
		 aSinTemp = asin(k1*sin(a1));
		 aProjection = atan(tan(aSinTemp)*cos(a2));
		 zHightTemp = sqrt(z0*z0 + x0*x0);
		 
		 tempangle1 = asin(r*sin(aProjection)/zHightTemp);
		 tempangle2 = aProjection - tempangle1;   //tempangle2  keep for the below calculation
         // refract point
		 xxx = r * sin(tempangle2);
		 zzz = r * cos(tempangle2);
		 
         bProjection = atan(tan(a1)*cos(a2));
         tempangle3 = asin(sin(bProjection)*r/(r-FocusDepth));  // tempangle3 is sure to be large than 90 degree
		 tempangle1 = tempangle3 - bProjection               ;
		 // 当折射角和偏转角设定,既可求得最大聚焦深度.
		 // R*SIN(PROJECTION)
         //if(FocusDepth>r*sin(bProjection))  FocusDepth =  r*sin(bProjection);
		 //print("MaxDepth:%f\n", r*sin(bProjection))        ;  // when a1 and a2 is setted, there will exist a max focus depth		 
		 tempFocusxxx = (r-FocusDepth) * sin(tempangle1+tempangle2) ;
		 tempFocuszzz = (r-FocusDepth) * cos(tempangle1+tempangle2) ;
         tempFocusyyy = zHightTemp*sin(tempangle2)+sin(tempangle1)*(r-FocusDepth);         
		 tempFocusyyy = tempFocusyyy*tan(a2) ;
		 //之前  double  a1 = fabs(angle1*PI/180); 						   // refract angle
		 // 		   double	  a2 = fabs(angle2*PI/180); 						   // deflect angle
		 // 		   if(a2>PI/2)	a2 = PI-a2 ;
		 //当a1 a2	  都大于0时,相对于探头中心点的坐标系的入射点坐标
		 // TEMPFOCUSXXX TEMPFOCUSYYY TEMPFOCUSZZZ
		 //求得临时坐标后,再根据a1 a2的实际情况,将坐标的正负值设定
         if(angle1>=0)
         {
             if(angle2>=0)
             {
                 if(angle2>90)
                 {
                      tempFocusxxx=-tempFocusxxx;
                 }
             }
			 else
			 {
			     if(a2>PI/2)
			     {
			          tempFocusxxx=-tempFocusxxx;
			     }
				 tempFocusyyy = -tempFocusyyy;
			 }
         }
		 else
		 {
		     if(angle2>=0)
             {
                 if(angle2>90)
                 {
                      tempFocusyyy = -tempFocusyyy;
                 }
				 else
				 {
				      tempFocusyyy=-tempFocusyyy;
					  tempFocusxxx = -tempFocusxxx;
				 }
             }
			 else
			 {
			     if(a2<PI/2)
			     {
			          tempFocusxxx=-tempFocusxxx;
			     }		 
			 }
		 }
		 //坐标旋转,将焦点坐标由中心探头坐标旋转到实际坐标
		 p->focus_y[p->k]   = y0 - tempFocusyyy    ;
		 angle_rotate = -asin(x0/zHightTemp) ;
		 p->focus_x[p->k] = tempFocusxxx*cos(angle_rotate) - tempFocuszzz*sin(angle_rotate);
		 p->focus_z[p->k] = -(tempFocusxxx*sin(angle_rotate) + tempFocuszzz*cos(angle_rotate));
		 //第一个探头的坐标
		 p->probe_x[p->k][0] =   -length/2 + wg_pri_elem_offset_fir ;
		 p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
		 p->probe_z[p->k][0] =   -h0	- sqrt(CylinderExtDiameter*CylinderExtDiameter- length*length)/2;	  
		 tempangle2 = asin(p->focus_x[p->k]/(r-FocusDepth)) ;
		 r_depth    = r - FocusDepth ;
		 
		 for(i = PriElementStart; i< PriElementStop; i++)
		 {
			  for(j = SecElementStart; j< SecElementStop; j++)
			  {
					//get elements' coordinates
					zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
					xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
					yTemp = p->probe_y[p->k][0] + s_y*j;
					p->probe_x[p->k][i*SecElementNum+j] = xTemp  ;
					p->probe_y[p->k][i*SecElementNum+j] = yTemp  ;
					p->probe_z[p->k][i*SecElementNum+j] = zTemp  ;
					if((xTemp*xTemp+zTemp*zTemp)<r*r)
					{
					   	p->horizontal_x[p->k][i*SecElementNum+j] = xTemp  ;
					    p->horizontal_y[p->k][i*SecElementNum+j] = yTemp ;
					    p->horizontal_z[p->k][i*SecElementNum+j] = zTemp ;
					    continue ;
					}
                    count = 0;
					zHightTemp   = sqrt(xTemp*xTemp + zTemp*zTemp);
					angle_rotate = -asin(xTemp/zHightTemp)        ;
					tempangle3   = tempangle2 + angle_rotate      ;
                    if(tempangle3 == 0)
                    { // 当投影角为0时,在Y方向作搜索.类似于一维搜索
					    xxx = 0;
						zzz = r ;			
					    if(yTemp == p->focus_y[p->k])
					       yyy = yTemp ;
					    else 
					    {
					         zTemp = zHightTemp - r;
							 TempLength = fabs(yTemp-p->focus_y[p->k]);
							 SearchLength = TempLength/2 ;
							 SearchPoint = 0 ;
							 TempResult = 0 ;
                             do
                             {
                                  SearchPoint += SearchLength ;      
                                   //printf("search point :%f \n",SearchPoint);
                                  tana = SearchPoint/zTemp;
                                  sina = sqrt(tana*tana/(1+tana*tana));
                                  sinb = sina/k1;				   
                                  if(sinb>=1)  
                                  {
                                       SearchLength = -fabs(SearchLength)/2 ;                   
                                  }
                                  else
                                  {
                                       tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
                                       TempResult=SearchPoint+tanb*FocusDepth ;
                                       if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                                       if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                 
                                  }                   
                                  count++;         
                             }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;   

                             yyy = yTemp + SearchPoint*(p->focus_y[p->k]-yTemp)/fabs(p->focus_y[p->k] - yTemp); 
							 
					    }
                        
					    p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
					    p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
					    p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
						continue ;
                    }
					tempangle1   = fabs(tempangle3);
					SearchLength = tempangle1/2                   ;
					SearchAngle  = 0                              ;
					TempResult   = 0                              ;
					
					yyy = fabs(p->focus_y[p->k] - yTemp) ;
					//TempResult = TempLength + SearchGate ;
					do
					{// 搜索X-Z投影角
						  SearchAngle += SearchLength ; 	 
						  //printf("search point :%f \n",SearchPoint);
						  aProjection = atan(zHightTemp*sin(SearchAngle)/(zHightTemp*cos(SearchAngle)-r));
						  SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r-FocusDepth)*sin(tempangle1-SearchAngle)));
                          a = atan(tan(aProjection)/cos(SkewAngleTemp));
                          if(fabs(sin(a)/k1)>=1)
                          {
                                 SearchLength = -fabs(SearchLength)/2 ;
                          }
                          else
                          {						  
                             b = asin(sin(a)/k1)                          ;
						     bProjection = atan(tan(b)*cos(SkewAngleTemp));
                             TempResult  = r*sin(bProjection)/sin(bProjection+tempangle1-SearchAngle)  ;
						     if(TempResult>r_depth)   SearchLength = -fabs(SearchLength)/2 ;
                             if(TempResult<r_depth)   SearchLength =  fabs(SearchLength)/2 ;
                          }
						  count++;
					}while( (fabs(TempResult-r_depth) > SearchGate) && ( count<=20 )) ;	   
					// translate into common coordiniate
                    if(tempangle3<=0)
					       xxx = -r*sin(SearchAngle) ;
					else   
						   xxx = r*sin(SearchAngle);
					
					if(yTemp>p->focus_y[p->k])
					    yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
					else 
					    yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
                    zzz = r*cos(SearchAngle);
					p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
					p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
					p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate)) ;				   
			  }
		 }
		//calculate the transfer time
		for(i = PriElementStart; i< PriElementStop; i++)
		{
			for(j = SecElementStart; j< SecElementStop; j++)
			{
				 dis1 = sqrt((p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])*(p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])
								+(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])*(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])
							   +(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j])*(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j]));
				 dis2 = sqrt((p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])
							   +(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])
							   +(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k]));
				 time[i*SecElementNum+j] = dis1/c1+dis2/c2;		   
			}
		}
		// get the time delay
		timemax = time[PriElementStart*SecElementNum + SecElementStart]; 
		
		for(i=PriElementStart; i< PriElementStop ; i++)
		{
			for(j = SecElementStart; j< SecElementStop; j++)
			{
				  if(timemax < time[i*SecElementNum + j ])	timemax = time[i*SecElementNum + j ] ;
			}
		}
/*		
		print("***********\n")   ;
		print("	 x1 x2 x3\n")  ;
		print("y1  0	3  6  \n") ;
		print("y2  1	4  7  \n") ;
		print("y3  2	5  8  \n") ;
		print("***********\n")   ;	   
*/		
		for(i=PriElementStart; i< PriElementStop ; i++)
		{
			for(j = SecElementStart; j< SecElementStop; j++)
			{
//				 printf("No.[%d][%d],time = %f,Δt=%d\n",i+1,j+1,time[i*SecElementNum + j ],(int)((timemax - time[i*SecElementNum + j ])*1000000+0.5));
				 p->timedelay[p->k][i*SecElementNum + j] = (int)((timemax - time[i*SecElementNum + j ])*1000000+0.5)								   ;
			}
		}
		p->G_Time[p->k] = timemax;
		free(time);


}
/*
  Name:  cylinder_1d_od_zero
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 10 -05-11 13:40
  Description:   1 DIMENSION 带ROOF 角的 OD 聚焦法则  ROTATION Z = 0
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void cylinder_1d_od_zero (PARAMETER_P data, double angle1, double depth)
{
	 PARAMETER_P p = (PARAMETER_P) data ;
	 
	 double c1 ;			 // wave speed
	 double c2 ;
	 double k1 ;  
	 double h0 = p->wedge_p->wg_heigh_fir ;				   // first element high
	
	 int ElementStart = p->element_sel->primary_axis_s - 1;
	 int ElementColumn= p->probe_p->ele_num_pri 		  ;
	 int SelectColumn = p->element_sel->pri_axis_ape	  ;
	 int ElementStop  =   SelectColumn+ ElementStart	  ;
	 
	 double Intervalx = p->probe_p->pri_axis_pitch  ;		   // x direction interval distance
	 double pri_ele_size = p->probe_p->pri_ele_size ;		
	 double wg_pri_axis_reference	= p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	 double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	 double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	 double  a1 = angle1*PI/180;							   // refract angle  
     
	 double width = p->wedge_p->wg_width ;
	 double r = p->specimen->speci_outside_cylindrical / 2.0  ;
	 double zHeight = -sqrt(r*r - width*width/4);
	 
	 double wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform	   
	 double FocusDepth = depth;							   //  true focus depth
	
	 double x0,y0,z0;										   // element center coordinate
	 double xP, yP, zP;									   // focus point coordinate
	 // get focus point coordinate			
     double a2                   ;
	 double a2Projection         ;
	 double a1Projection         ;
     double HeightTmp            ;
	 double screwangle           ;
	 double FocusDistance        ;
     double TmpAngle1, TmpAngle2 ;
	 double InPointy, InPointz   ;
	 // variable for search method	// 
	 double xTemp, yTemp, zTemp  ;
	 double tl1, tl2, tl3   ; 
	 int i, j               ; 
	 double timemax         ;
	 double dis1  , dis2    ;
	 double TempLength ;
	 double xxx,yyy;
	 double SearchPoint , SearchLength  ,TempResult ;
	 double sina,tana,sinb , tanb;
	 int count ;
	 if(ElementStop > ElementColumn)  return ;
	 if(p->wedge_p->wg_roof_angle != 0 ) return ; //当ROOF 角不等于0 时,根本无法聚焦
	 z0 = Intervalx*ElementStart*sin(wedgeangle);
	 x0 = Intervalx*ElementStart*cos(wedgeangle);
	 
	 double *time = malloc(ElementColumn * sizeof(double));
	 memset(time,0x0,ElementColumn * sizeof(double));
	 
	 if(p->probe_p->transmi_trans_longi==0)
	 {	 
		 c1  = p->wedge_p->wg_lon_vel ; 			  /* 楔块纵波声速 */ 
		 c2  = p->specimen->speci_transverse_wave;	  /* 样本横波声速 */  
	 }											 
	 else								  
	 {
		 c1  = p->wedge_p->wg_lon_vel ; 			  /* 楔块纵波声速 */	
		 c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
	 }
	
	 // refract coefficient
	 k1 = c1/c2 ;	
	 a2 = asin(sin(a1)*k1) ;
	 // center probe element	(x0,y0,z0)			  
	 z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2) +zHeight;
	 x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
	 y0 = width/2 - wg_sec_elem_offset_fir;
	 // get focus point coordinate 
	 // desirex desirey desirez : 入射点坐标		
     if(y0 == 0)
     {
          TempLength = FocusDepth*tan(a1) ;
		  p->focus_x[p->k] =  x0+TempLength ;
		  TempLength =  fabs(z0 + r)*tan(asin(sin(a1)*k1));
		  p->focus_x[p->k] += TempLength ;
		  p->focus_y[p->k] =  y0 ;
          p->focus_z[p->k] =  FocusDepth - r;
          xP = p->focus_x[p->k];
          yP = p->focus_y[p->k];
          zP = FocusDepth;
		  p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
	      p->probe_y[p->k][0] =   width/2 - wg_sec_elem_offset_fir               ;
	      p->probe_z[p->k][0] =   -h0  + zHeight                                 ;
		  
          for(i = ElementStart; i< ElementStop; i++)
          {
            //  get each element coordinates
            zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle) + r;
            xTemp = p->probe_x[p->k][0]  + Intervalx*cos(wedgeangle)*i;
            yTemp = p->probe_y[p->k][0];
            p->probe_x[p->k][i] = xTemp;
            p->probe_y[p->k][i] = yTemp;
            p->probe_z[p->k][i] = zTemp - r;   

			if(zTemp>=0)
            {    // kill special situation when probe under the detect surface
                 p->horizontal_x[p->k][i] = xTemp  ;
                 p->horizontal_y[p->k][i] = yTemp  ;
                 p->horizontal_z[p->k][i] = zTemp  ; 
				 continue ;
            }			
            xxx = xP-xTemp ;
           //A1 ==0 时按一维ROOF角等于0的情况作搜索
            TempLength = fabs(xxx);
            SearchLength = TempLength/2 ;

            if(TempLength==0||zTemp==0)
            {
               p->horizontal_x[p->k][i] = xTemp  ;
               p->horizontal_y[p->k][i] = yTemp  ;
               p->horizontal_z[p->k][i] = 0      ;     
               continue ;     
            }                      
            // do search in temporary coordinate
            SearchPoint = 0 ;
            TempResult =  0 ;
            count = 0;                          
            do
            {
               SearchPoint += SearchLength ;      
               tana = SearchPoint/zTemp    ;
               sina = sqrt(tana*tana/(1+tana*tana));
               sinb = sina/k1;
			   
               if(sinb>=1)  
               {
                   SearchLength = -fabs(SearchLength)/2 ;                   
               }
               else
               {
                   tanb = sqrt(sinb*sinb/(1-sinb*sinb))   ;
                   TempResult=SearchPoint+tanb*FocusDepth ;
                   if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                   if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
               }                   
               count++;         
            }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;      
            p->horizontal_x[p->k][i] = xTemp + fabs(a1)*SearchPoint/a1;
            p->horizontal_y[p->k][i] = 0  ;
            p->horizontal_z[p->k][i] = -r ;  	
          }  
     }
	 else
	 {
		 a2Projection = fabs(asin(y0/r))                 ;
	     HeightTmp    = -z0 - sqrt(r*r-y0*y0)            ;
		 if(fabs(a2Projection) > fabs(a2)) return        ;
	     screwangle   = acos(tan(a2Projection)/tan(a2))  ;
	     a1Projection = atan(tan(a1)*cos(screwangle))    ;
		 TmpAngle1 = sin(a1Projection)*r/(r-FocusDepth)  ;
		 TmpAngle1 = TmpAngle1 - a1Projection            ;
		 TmpAngle2 = TmpAngle1 + a2Projection            ;
	 
		 InPointy = r*sin(a2Projection)                  ;
		 InPointz = -r*cos(a2Projection)                 ;

		 zP = -(r-FocusDepth)*cos(TmpAngle2)             ;
		 yP = (r-FocusDepth)*sin(TmpAngle2)              ;
		 FocusDistance = (r-FocusDepth) * sin(TmpAngle1)                             ;
		 xP = x0 + (FocusDistance + HeightTmp * sin(a2Projection)) * tan(screwangle) ;
		 
		 p->focus_x[p->k] = xP			      ;
		 p->focus_y[p->k] = yP			      ;
		 p->focus_z[p->k] = zP			      ;

		 //first element location 
		 p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
		 p->probe_y[p->k][0] =   width/2 - wg_sec_elem_offset_fir               ;
		 p->probe_z[p->k][0] =   -h0  + zHeight                                 ;
		 HeightTmp = sqrt(r*r - y0*y0)                                          ;

		 for(i = ElementStart; i< ElementStop; i++)
		  {
				 //  get each element coordinates
				 zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle);
				 xTemp = p->probe_x[p->k][0]  + Intervalx*cos(wedgeangle)*i;
				 yTemp = p->probe_y[p->k][0]                               ;

				 p->probe_x[p->k][i] = xTemp;
				 p->probe_y[p->k][i] = yTemp;
				 p->probe_z[p->k][i] = zTemp;
				
				 if((yTemp*yTemp+zTemp*zTemp)<r*r)
				 {
				   	p->horizontal_x[p->k][i] = xTemp ;
				    p->horizontal_y[p->k][i] = yTemp ;
				    p->horizontal_z[p->k][i] = zTemp ;
				    continue ;
				 }	
				 
				 p->horizontal_y[p->k][i] =  InPointy;
				 p->horizontal_z[p->k][i] =  InPointz;

				 tl1 = -zTemp - HeightTmp ;
				 tl1 = fabs(tl1 * sin(a2Projection)) ;
	             tl2 = xTemp - xP ;
				 p->horizontal_x[p->k][i] = xTemp - tl2*tl1/(FocusDistance + tl1)  ;
		  }	
	  }
	  //calculate the transfer time
	  for(i = ElementStart; i< ElementStop ; i++)
	  { 	
			   dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])
							  +(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])*(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])
							  +(p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
			   dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
							 +(p->horizontal_y[p->k][i]-p->focus_y[p->k])*(p->horizontal_y[p->k][i]-p->focus_y[p->k])
							 +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
			   time[i] = dis1/c1+dis2/c2;		   
	  }
	  // get the time delay
	  timemax = time[0];
	  for(i = ElementStart ; i< ElementStop ; i++)
	  {
			if(timemax < time[i])  timemax = time[i] ;					 
	  } 
	  for(i = ElementStart; i< ElementStop ; i++)
	  {
	//		 printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		   p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5); 	
	  }
	  p->G_Time[p->k] = timemax;
	  free(time);
}

/*
  Name:  cylinder_2d_roof_od_zero
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 10-05-11 13:40
  Description:   2 DIMENSION 带ROOF 角的 OD 聚焦法则 ROTATION Z = 0
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void cylinder_2d_roof_od_zero (PARAMETER_P data, double angle1, double angle2, double depth)
{
            // transfer z rotation  =0 to z rotation  = 90 
            // skew angle  = angle2 + 90 ;
            // rotate the coordinate of each element  x to y , y to x 
            //*********************************************
            angle2  += 90 ;
			//*********************************************
			 PARAMETER_P p = (PARAMETER_P) data ;
			 
			 double c1 ;			 // wave speed
			 double c2 ;
			 double k1 ;  
			 double r	;
			 double h0 =  p->wedge_p->wg_heigh_fir ;				   // first element high
	//		 int Row	   =  p->element_sel->sec_axis_ape	;				   // element y direction numble
	//		   int Column	 =	p->element_sel->pri_axis_ape;				   // element x direction numble
	//		   if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
	//			  return;
			 int PriElementStart = p->element_sel->primary_axis_s - 1  ;
			 int PriElementNum	 = p->probe_p->ele_num_pri			  ;
			 int SecElementStart = p->element_sel->seconary_axis_s -1 ;
			 int SecElementNum	 = p->probe_p->ele_num_sec			  ;
			 int PriElementSelect= p->element_sel->pri_axis_ape 	  ;
			 int SecElementSelect= p->element_sel->sec_axis_ape 	  ;
			 int PriElementStop  = PriElementStart +  PriElementSelect;
			 int SecElementStop  = SecElementStart +  SecElementSelect;
			 
			 double Intervaly = p->probe_p->sec_axis_pitch ;		   // y direction interval
			 double Intervalx = p->probe_p->pri_axis_pitch ;		   // x direction interval distance
			 double pri_ele_size = p->probe_p->pri_ele_size ;		  
			 double wg_pri_axis_reference	= p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
			 double wg_sec_axis_reference	= p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
			 double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
			 double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
			 double length = p->wedge_p->wg_length ;							   // wedge length
			 double CylinderExtDiameter = p->specimen->speci_outside_cylindrical ;// outside cylinder diameter
				  
			 double  a1 = fabs(angle1*PI/180); 					   // refract angle
			 double  a2 = fabs(angle2*PI/180); 					   // deflect angle
			 if(a2>PI/2) a2 = PI-a2 ;
			 
			 double  roofangle = p->wedge_p->wg_roof_angle*PI/180	 ; // roofangle   y-z platform
			 double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180 ; // wedgeangle  x-z platform	   
			 double  FocusDepth = depth;							   //  true focus depth
	 
			 // refraction calculation 
			 double aSinTemp;
			 double aProjection;
			 double bProjection;
			 double zHightTemp;
			 
			 double tempangle1;
			 double tempangle2;
			 double tempangle3;
	
			 double tempFocusxxx;
			 double tempFocusyyy;
			 double tempFocuszzz;
			 double angle_rotate;
			 
			 double xTemp, yTemp, zTemp ;
			 double x0,y0,z0;			  // element center coordinate
			 //  element offset in each direction
			 double x;
			 double y;
			 double z;
			 double d;
			 double s_x;
			 double s_y;
			 double s_zy;
			 double s_zx;	   
			 // variable for search method
			 double SearchLength , SearchAngle;
			 double TempAngle;
			 double TempResult;
			 double SkewAngleTemp;
			 double SearchPoint ;
			 double TempLength  ;
			 double tana,tanb,sina,sinb ;
			 
			 double r_depth;
			 double a;
			 double b;
			 
			 double xxx;
			 double yyy;
			 double zzz;
			 
			 int i, j; 
			 int count;
			 // variable for time calculation
			 double dis1;
			 double dis2;
			 double timemax ;
	 
			double *time = malloc(PriElementNum*SecElementNum*sizeof(double));
			memset(time,0x0,PriElementNum*SecElementNum*sizeof(double));
			if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;	SecElementStart = 0; SecElementStop = 1;}
			 
			if(PriElementStop > PriElementNum )  return ;
			if(SecElementStop > SecElementNum )  return ;	
	
			r = CylinderExtDiameter/2.0 ;
			// kill the impossible condition
			if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
		 
			if(p->probe_p->transmi_trans_longi==0)
			{	
				c1	= p->wedge_p->wg_lon_vel ;				 /* 楔块纵波声速 */ 
				c2	= p->specimen->speci_transverse_wave;	 /* 样本横波声速 */  
			}											
			else								 
			{
				c1	= p->wedge_p->wg_lon_vel ;				 /* 楔块纵波声速 */    
				c2	= p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
			}
			 // refract coefficient
			 k1 = c1/c2 ;
	
			 // element location offset calculate
			 // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
			 // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
			 if((wedgeangle != 0) && (roofangle != 0))
			 {
				  x = 1;
				  y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
				  z = y*tan(roofangle) ;
				  d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
				  s_x  = -Intervaly/d	 ;
				  s_y  = -Intervaly*y/d   ;
				  s_zy	= -Intervaly*(z-tan(wedgeangle))/d	 ;			
			 }
			 else
			 {	 
				  s_y = -Intervaly*cos(roofangle);
				  s_x = 0 ;
				  s_zy = -Intervaly*sin(roofangle);
			 }	   
	
			 //calc offset generated by element selection
			 z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
			 x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
			 y0 = s_y*SecElementStart	;
	
			 // center probe element	(x0,y0,z0)			  
			 z0 = -(z0 +h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect -1)/2) -sqrt(CylinderExtDiameter*CylinderExtDiameter - length*length)/2;
			 x0 = x0 -length/2 + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect-1)*s_x/2 ;
			 if(roofangle<0)
				 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
			 y0 = y0 - wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect-1)/2 ;
			 //以下和1维Cylinder情况类似.
			 //只是将3维的坐标投影到X-Z面
			 //空间角度和投影角偏转角之间的关系
			 // tan(Projection) = tan(SpaceAngle)*cos(SkewAngle);
			 // get focus point coordinate 
			 aSinTemp = asin(k1*sin(a1));
			 aProjection = atan(tan(aSinTemp)*cos(a2));
			 zHightTemp = sqrt(z0*z0 + x0*x0);
			 
			 tempangle1 = asin(r*sin(aProjection)/zHightTemp);
			 tempangle2 = aProjection - tempangle1;   //tempangle2	keep for the below calculation
			 // refract point
			 xxx = r * sin(tempangle2);
			 zzz = r * cos(tempangle2);
			 
			 bProjection = atan(tan(a1)*cos(a2));
			 tempangle3 = asin(sin(bProjection)*r/(r-FocusDepth));	// tempangle3 is sure to be large than 90 degree
			 tempangle1 = tempangle3 - bProjection				 ;
			 // 当折射角和偏转角设定,既可求得最大聚焦深度.
			 // R*SIN(PROJECTION)
			 //if(FocusDepth>r*sin(bProjection))  FocusDepth =	r*sin(bProjection);
			 //print("MaxDepth:%f\n", r*sin(bProjection)) 	   ;  // when a1 and a2 is setted, there will exist a max focus depth		 
			 tempFocusxxx = (r-FocusDepth) * sin(tempangle1+tempangle2) ;
			 tempFocuszzz = (r-FocusDepth) * cos(tempangle1+tempangle2) ;
			 tempFocusyyy = zHightTemp*sin(tempangle2)+sin(tempangle1)*(r-FocusDepth);		   
			 tempFocusyyy = tempFocusyyy*tan(a2) ;
			 //之前  double  a1 = fabs(angle1*PI/180); 						   // refract angle
			 // 		   double	  a2 = fabs(angle2*PI/180); 						   // deflect angle
			 // 		   if(a2>PI/2)	a2 = PI-a2 ;
			 //当a1 a2	  都大于0时,相对于探头中心点的坐标系的入射点坐标
			 // TEMPFOCUSXXX TEMPFOCUSYYY TEMPFOCUSZZZ
			 //求得临时坐标后,再根据a1 a2的实际情况,将坐标的正负值设定
			 if(angle1>=0)
			 {
				 if(angle2>=0)
				 {
					 if(angle2>90)
					 {
						  tempFocusxxx=-tempFocusxxx;
					 }
				 }
				 else
				 {
					 if(a2>PI/2)
					 {
						  tempFocusxxx=-tempFocusxxx;
					 }
					 tempFocusyyy = -tempFocusyyy;
				 }
			 }
			 else
			 {
				 if(angle2>=0)
				 {
					 if(angle2>90)
					 {
						  tempFocusyyy = -tempFocusyyy;
					 }
					 else
					 {
						  tempFocusyyy=-tempFocusyyy;
						  tempFocusxxx = -tempFocusxxx;
					 }
				 }
				 else
				 {
					 if(a2<PI/2)
					 {
						  tempFocusxxx=-tempFocusxxx;
					 }		 
				 }
			 }
			 //坐标旋转,将焦点坐标由中心探头坐标旋转到实际坐标
			 p->focus_y[p->k]	= y0 - tempFocusyyy    ;
			 angle_rotate = -asin(x0/zHightTemp) ;
			 p->focus_x[p->k] = tempFocusxxx*cos(angle_rotate) - tempFocuszzz*sin(angle_rotate);
			 p->focus_z[p->k] = -(tempFocusxxx*sin(angle_rotate) + tempFocuszzz*cos(angle_rotate));
			 //第一个探头的坐标
			 p->probe_x[p->k][0] =	 -length/2 + wg_pri_elem_offset_fir ;
			 p->probe_y[p->k][0] =	 -wg_sec_axis_reference - wg_sec_elem_offset_fir;
			 p->probe_z[p->k][0] =	 -h0	- sqrt(CylinderExtDiameter*CylinderExtDiameter- length*length)/2;	  
			 tempangle2 = asin(p->focus_x[p->k]/(r-FocusDepth)) ;
			 r_depth	= r - FocusDepth ;
			 
			 for(i = PriElementStart; i< PriElementStop; i++)
			 {
				  for(j = SecElementStart; j< SecElementStop; j++)
				  {
						//get elements' coordinates
						zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
						xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
						yTemp = p->probe_y[p->k][0] + s_y*j;
						p->probe_x[p->k][i*SecElementNum+j] = xTemp  ;
						p->probe_y[p->k][i*SecElementNum+j] = yTemp  ;
						p->probe_z[p->k][i*SecElementNum+j] = zTemp  ;
						if((xTemp*xTemp+zTemp*zTemp)<r*r)
						{
							p->horizontal_x[p->k][i*SecElementNum+j] = xTemp  ;
							p->horizontal_y[p->k][i*SecElementNum+j] = yTemp ;
							p->horizontal_z[p->k][i*SecElementNum+j] = zTemp ;
							continue ;
						}
						count = 0;
						zHightTemp	 = sqrt(xTemp*xTemp + zTemp*zTemp);
						angle_rotate = -asin(xTemp/zHightTemp)		  ;
						tempangle3	 = tempangle2 + angle_rotate	  ;
						if(tempangle3 == 0)
						{ // 当投影角为0时,在Y方向作搜索.类似于一维搜索
							xxx = 0;
							zzz = r ;			
							if(yTemp == p->focus_y[p->k])
							   yyy = yTemp ;
							else 
							{
								 zTemp = zHightTemp - r;
								 TempLength = fabs(yTemp-p->focus_y[p->k]);
								 SearchLength = TempLength/2 ;
								 SearchPoint = 0 ;
								 TempResult = 0 ;
								 do
								 {
									  SearchPoint += SearchLength ; 	 
									   //printf("search point :%f \n",SearchPoint);
									  tana = SearchPoint/zTemp;
									  sina = sqrt(tana*tana/(1+tana*tana));
									  sinb = sina/k1;				   
									  if(sinb>=1)  
									  {
										   SearchLength = -fabs(SearchLength)/2 ;					
									  }
									  else
									  {
										   tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
										   TempResult=SearchPoint+tanb*FocusDepth ;
										   if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
										   if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;				  
									  } 				  
									  count++;		   
								 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;   
	
								 yyy = yTemp + SearchPoint*(p->focus_y[p->k]-yTemp)/fabs(p->focus_y[p->k] - yTemp); 
								 
							}
							
							p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
							p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
							p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
							continue ;
						}
						tempangle1	 = fabs(tempangle3);
						SearchLength = tempangle1/2 				  ;
						SearchAngle  = 0							  ;
						TempResult	 = 0							  ;
						
						yyy = fabs(p->focus_y[p->k] - yTemp) ;
						//TempResult = TempLength + SearchGate ;
						do
						{// 搜索X-Z投影角
							  SearchAngle += SearchLength ; 	 
							  //printf("search point :%f \n",SearchPoint);
							  aProjection = atan(zHightTemp*sin(SearchAngle)/(zHightTemp*cos(SearchAngle)-r));
							  SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r-FocusDepth)*sin(tempangle1-SearchAngle)));
							  a = atan(tan(aProjection)/cos(SkewAngleTemp));
							  if(fabs(sin(a)/k1)>=1)
							  {
									 SearchLength = -fabs(SearchLength)/2 ;
							  }
							  else
							  { 					  
								 b = asin(sin(a)/k1)						  ;
								 bProjection = atan(tan(b)*cos(SkewAngleTemp));
								 TempResult  = r*sin(bProjection)/sin(bProjection+tempangle1-SearchAngle)  ;
								 if(TempResult>r_depth)   SearchLength = -fabs(SearchLength)/2 ;
								 if(TempResult<r_depth)   SearchLength =  fabs(SearchLength)/2 ;
							  }
							  count++;
						}while( (fabs(TempResult-r_depth) > SearchGate) && ( count<=20 )) ;    
						// translate into common coordiniate
						if(tempangle3<=0)
							   xxx = -r*sin(SearchAngle) ;
						else   
							   xxx = r*sin(SearchAngle);
						
						if(yTemp>p->focus_y[p->k])
							yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
						else 
							yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
						zzz = r*cos(SearchAngle);
						p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
						p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
						p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate)) ; 			   
				  }
			 }
			//calculate the transfer time
			for(i = PriElementStart; i< PriElementStop; i++)
			{
				for(j = SecElementStart; j< SecElementStop; j++)
				{
					 dis1 = sqrt((p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])*(p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])
									+(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])*(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])
								   +(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j])*(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j]));
					 dis2 = sqrt((p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])
								   +(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])
								   +(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k]));
					 time[i*SecElementNum+j] = dis1/c1+dis2/c2; 	   
				}
			}
			// get the time delay
			timemax = time[PriElementStart*SecElementNum + SecElementStart]; 
			
			for(i=PriElementStart; i< PriElementStop ; i++)
			{
				for(j = SecElementStart; j< SecElementStop; j++)
				{
					  if(timemax < time[i*SecElementNum + j ])	timemax = time[i*SecElementNum + j ] ;
				}
			}
/*			
			print("***********\n")   ;
			print("	 x1 x2 x3\n")  ;
			print("y1  0	3  6  \n") ;
			print("y2  1	4  7  \n") ;
			print("y3  2	5  8  \n") ;
			print("***********\n")   ;	   
*/			
			for(i=PriElementStart; i< PriElementStop ; i++)
			{
				for(j = SecElementStart; j< SecElementStop; j++)
				{
//					 printf("No.[%d][%d],time = %f,Δt=%d\n",i+1,j+1,time[i*SecElementNum + j ],(int)((timemax - time[i*SecElementNum + j ])*1000000+0.5));
					 p->timedelay[p->k][i*SecElementNum + j] = (int)((timemax - time[i*SecElementNum + j ])*1000000+0.5)								   ;
				}
			}
			p->G_Time[p->k] = timemax;
			free(time);
	
	
	}


/*
  Name:  cylinder_2d_roof_id
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   2 DIMENSION 带ROOF 角的 ID 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        angle2: skew angle
        depth : focus depth of sonic beam
*/
void cylinder_2d_roof_id(PARAMETER_P data, double angle1, double angle2 , double depth)
{
      
	   PARAMETER_P p = (PARAMETER_P) data ;
	   
	   double c1 ; 		   // wave speed
	   double c2 ;
	   double k1 ;  
	   double r  ;
	   double h0 =  p->wedge_p->wg_heigh_fir ; 				 // first element high
	   //int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
       //int Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
       //if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
       //     return;
	   int PriElementStart = p->element_sel->primary_axis_s -1	;
	   int PriElementNum   = p->probe_p->ele_num_pri			;
	   int SecElementStart = p->element_sel->seconary_axis_s -1 ;
	   int SecElementNum   = p->probe_p->ele_num_sec			;
	   int PriElementSelect= p->element_sel->pri_axis_ape		;
	   int SecElementSelect= p->element_sel->sec_axis_ape		;
	   int PriElementStop  = PriElementStart +	PriElementSelect;
	   int SecElementStop  = SecElementStart +	SecElementSelect;
	   
	   double Intervaly = p->probe_p->sec_axis_pitch ; 		 // y direction interval
	   double Intervalx = p->probe_p->pri_axis_pitch ; 		 // x direction interval distance
	   double pri_ele_size = p->probe_p->pri_ele_size ;		
	   double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	   double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	   double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	   double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	   double length = p->wedge_p->wg_length ; 							 // wedge length
	   double CylinderIntDiameter = p->specimen->speci_inside_cylindrical ;// outside cylinder diameter
	   
	  
	   double	a1 = fabs(angle1*PI/180); 							 // refract angle
	   double	a2 = fabs(angle2*PI/180); 							 // deflect angle
	   if(a2>PI/2)  a2 = PI-a2 ;
	   double	roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle	y-z platform
	   double	wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle	x-z platform	 
	   double	FocusDepth = depth; 							 //  true focus depth
	  
	   // refraction calculation 
	   double aSinTemp;
	   double aProjection;
	   double bProjection;
	   double zHightTemp;
	   
	   double tempangle1;
	   double tempangle2;
	   double tempangle3;
	  
	   double tempFocusxxx;
	   double tempFocusyyy;
	   double tempFocuszzz;
	   double angle_rotate;
	   
	   double xTemp, yTemp, zTemp ;
	   double x0,y0,z0;			                               // element center coordinate
	   //  element offset in each direction
	   double x;
	   double y;
	   double z;
	   double d;
	   double s_x;
	   double s_y;
	   double s_zy;
	   double s_zx;	 
	   // variable for search method
	   double SearchLength , SearchAngle;
	   double TempAngle;
	   double TempResult;
	   double SkewAngleTemp;
	   double SearchPoint ;
	   double TempLength  ;
	   double tana,tanb,sina,sinb ;
	   double r_depth;
	   double a;
	   double b;
	   
	   double xxx;
	   double yyy;
	   double zzz;
	   
	   int i, j; 
	   int count;
	   // variable for time calculation
	   double dis1;
	   double dis2;
	   double timemax ;
	   
	   double *time = malloc(PriElementNum*SecElementNum*sizeof(double));
	   memset(time,0x0,PriElementNum*SecElementNum*sizeof(double));
	   if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;  SecElementStart = 0; SecElementStop = 1;}
		
	   if(PriElementStop > PriElementNum )	return ;
	   if(SecElementStop > SecElementNum )	return ;   

	   r = CylinderIntDiameter/2.0 ;
	   // kill the impossible condition
	   if(wedgeangle == PI/2 || roofangle == PI/2)	return ;
	  
	   if(p->probe_p->transmi_trans_longi==0)
        {   
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
        }                                           
        else                                 
        {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
        }
	   // refract coefficient
	   k1 = c1/c2 ;
	   // element location offset calculate
	   // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
       // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
	   if((wedgeangle != 0) && (roofangle != 0))
	   {
			x = 1;
			y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
			z = y*tan(roofangle) ;
			d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
			s_x  = -Intervaly/d     ;
			s_y  = -Intervaly*y/d	;
			s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;		  
	   }
	   else
	   {   
			s_y = -Intervaly*cos(roofangle);
			s_x = 0 ;
			s_zy = -Intervaly*sin(roofangle);
	   }	 

	   //calc offset generated by element selection
	   z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
	   x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
	   y0 = s_y*SecElementStart   ;
	   // center probe element	  (x0,y0,z0)			
	   z0 = -(z0 + h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect-1)/2) + sqrt(CylinderIntDiameter*CylinderIntDiameter - length*length)/2;
	   x0 = x0 - length/2 + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect-1)*s_x/2 ;
       if(roofangle<0)
		 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
	   y0 = y0 - wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect-1)/2 ;
	  
	   // get focus point coordinate
       //以下和1维Cylinder情况类似.
       //只是将3维的坐标投影到X-Z面
       //空间角度和投影角偏转角之间的关系
       // tan(Projection) = tan(SpaceAngle)*cos(SkewAngle);
	   aSinTemp = asin(k1*sin(a1));
	   aProjection = atan(tan(aSinTemp)*cos(a2));
	   zHightTemp = sqrt(z0*z0 + x0*x0);
	   
	   tempangle1 = asin(r*sin(aProjection)/zHightTemp);
	   tempangle2 = tempangle1 - aProjection;	//tempangle2  keep for the below calculation
	   // refract point
	   xxx = r * sin(tempangle2);
	   zzz = r * cos(tempangle2);
	   
	   bProjection = atan(tan(a1)*cos(a2));
	   tempangle3 = asin(sin(bProjection)*r/(r+FocusDepth)); 
	   tempangle1 = bProjection - tempangle3			   ;  
	   
	   tempFocusxxx = (r+FocusDepth) * sin(tempangle1+tempangle2)              ;
	   tempFocuszzz = -(r+FocusDepth) * cos(tempangle1+tempangle2)             ;
	   tempFocusyyy = zHightTemp*sin(tempangle2)+sin(tempangle1)*(r+FocusDepth);
	   tempFocusyyy = tempFocusyyy*tan(a2)                                     ;
	   //之前  double	a1 = fabs(angle1*PI/180); 							 // refract angle
	   //            double	a2 = fabs(angle2*PI/180); 							 // deflect angle
	   //            if(a2>PI/2)  a2 = PI-a2 ;
	   //当a1 a2	都大于0时,相对于探头中心点的坐标系的入射点坐标
	   // TEMPFOCUSXXX TEMPFOCUSYYY TEMPFOCUSZZZ
	   //求得临时坐标后,再根据a1 a2的实际情况,将坐标的正负值设定
	   if(angle1>=0)
	   {
		   if(angle2>=0)
		   {
			   if(angle2>90)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }
		   }
		   else
		   {
			   if(a2>PI/2)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }
			   tempFocusyyy = -tempFocusyyy;
		   }
	   }
	   else
	   {
		   if(angle2>=0)
		   {
			   if(angle2>90)
			   {
					tempFocusyyy = -tempFocusyyy;
			   }
			   else
			   {
					tempFocusyyy=-tempFocusyyy;
					tempFocusxxx = -tempFocusxxx;
			   }
		   }
		   else
		   {
			   if(a2<PI/2)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }	   
		   }
	   }

	   p->focus_y[p->k]	= y0 - tempFocusyyy                                        ;
	   angle_rotate = asin(x0/zHightTemp)                                      ;
	   p->focus_x[p->k] = tempFocusxxx*cos(angle_rotate) - tempFocuszzz*sin(angle_rotate);
	   p->focus_z[p->k] = -(tempFocusxxx*sin(angle_rotate) + tempFocuszzz*cos(angle_rotate));
	   
	   p->probe_x[p->k][0] =	 -length/2 + wg_pri_elem_offset_fir ;
	   p->probe_y[p->k][0] =	 -wg_sec_axis_reference - wg_sec_elem_offset_fir;
	   p->probe_z[p->k][0] =	 -h0  + sqrt(CylinderIntDiameter*CylinderIntDiameter- length*length)/2;
	   tempangle2 = asin(p->focus_x[p->k]/(r+FocusDepth)) ;
	   r_depth	  = r + FocusDepth ;
	   
	   for(i = PriElementStart; i< PriElementStop; i++)
	   {
			for(j = SecElementStart; j< SecElementStop; j++)
			{
				  //get elements' coordinates
				  zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
				  xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
				  yTemp = p->probe_y[p->k][0] + s_y*j;
				  p->probe_x[p->k][i*SecElementNum+j] = xTemp  ;
				  p->probe_y[p->k][i*SecElementNum+j] = yTemp  ;
				  p->probe_z[p->k][i*SecElementNum+j] = zTemp  ;
				  if((xTemp*xTemp+zTemp*zTemp)>r*r)
				  {
					   	p->horizontal_x[p->k][i*SecElementNum+j] = xTemp ;
					    p->horizontal_y[p->k][i*SecElementNum+j] = yTemp ;
					    p->horizontal_z[p->k][i*SecElementNum+j] = zTemp ;
					    continue ;
				  }
                  count = 0;
				  zHightTemp   = sqrt(xTemp*xTemp + zTemp*zTemp);
				  angle_rotate = asin(xTemp/zHightTemp)		    ;
				  tempangle3   = tempangle2 - angle_rotate		;	  
				  
				  if(tempangle3 == 0)
				  {// 当投影角为0时,在Y方向作搜索.类似于一维搜索
					  xxx = 0;
					  zzz = -r ; 		  
					  if(yTemp == p->focus_y[p->k])
						 yyy = yTemp ;
					  else 
					  {    
						   zTemp = r - zHightTemp;
						   TempLength= fabs(yTemp-p->focus_y[p->k]);
						   SearchLength = TempLength/2 ;
						   SearchPoint  = 0 ;
						   TempResult = 0 ;
						   do
						   {
								SearchPoint += SearchLength ;	   
								tana = SearchPoint/zTemp;
								sina = sqrt(tana*tana/(1+tana*tana));
								sinb = sina/k1; 				 
								if(sinb>=1)  
								{
									 SearchLength = -fabs(SearchLength)/2 ; 				  
								}
								else
								{
									 tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
									 TempResult=SearchPoint+tanb*FocusDepth ;
									 if(TempResult>TempLength)	 SearchLength = -fabs(SearchLength)/2 ;
									 if(TempResult<TempLength)	 SearchLength =  fabs(SearchLength)/2 ; 				
								}					
								count++;		 
						   }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;	
						   yyy = yTemp + SearchPoint*(p->focus_y[p->k]-yTemp)/fabs(p->focus_y[p->k] - yTemp); 
					  }
					  p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
					  p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
					  p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
					  continue ;
				  }

				  tempangle1   = fabs(tempangle3)               ;
				  SearchLength = tempangle1/2					;
				  SearchAngle  = 0								;
				  TempResult   = 0								;
				  yyy = fabs(p->focus_y[p->k] - yTemp) ;
				  do
				  {// 搜索X-Z投影角
						SearchAngle += SearchLength ;	   
						//printf("search point :%f \n",SearchPoint);
						aProjection = atan(zHightTemp*sin(SearchAngle)/(r - zHightTemp*cos(SearchAngle)));
						SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r+FocusDepth)*sin(tempangle1-SearchAngle)));
						a = atan(tan(aProjection)/cos(SkewAngleTemp));
						if(fabs(sin(a)/k1)>=1)
						{
							   SearchLength = -fabs(SearchLength)/2 ;
						}
						else
						{						
						   b = asin(sin(a)/k1)							;
						   bProjection = atan(tan(b)*cos(SkewAngleTemp));
						   TempResult  = r*sin(bProjection)/sin(bProjection-tempangle1+SearchAngle);
						   if(TempResult>r_depth)	SearchLength =  fabs(SearchLength)/2 ;
						   if(TempResult<r_depth)	SearchLength =	-fabs(SearchLength)/2 ;
						}
						count++;
				  }while( (fabs(r_depth-TempResult) > SearchGate) && ( count<=20 )) ;	 
				  // translate into common coordiniate
                  xxx = (tempangle3<=0)?-r*sin(SearchAngle):r*sin(SearchAngle);				  
				  if(yTemp>p->focus_y[p->k])
					  yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
				  else 
					  yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
				  zzz = -r*cos(SearchAngle);
				  p->horizontal_x[p->k][i*SecElementNum+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
				  p->horizontal_y[p->k][i*SecElementNum+j] = yyy ;
				  p->horizontal_z[p->k][i*SecElementNum+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate) ) ;				 
			}
	   }
	  //calculate the transfer time
	  for(i = PriElementStart; i< PriElementStop; i++)
	  {
		  for(j = SecElementStart; j< SecElementStop; j++)
		  {
			   dis1 = sqrt((p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])*(p->probe_x[p->k][i*SecElementNum+j]-p->horizontal_x[p->k][i*SecElementNum+j])
							  +(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])*(p->probe_y[p->k][i*SecElementNum+j]-p->horizontal_y[p->k][i*SecElementNum+j])
							 +(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j])*(p->probe_z[p->k][i*SecElementNum+j]-p->horizontal_z[p->k][i*SecElementNum+j]));
			   dis2 = sqrt((p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*SecElementNum+j]-p->focus_x[p->k])
							 +(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*SecElementNum+j]-p->focus_y[p->k])
							 +(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*SecElementNum+j]-p->focus_z[p->k]));
			   time[i*SecElementNum+j] = dis1/c1+dis2/c2;		 
		  }
	  }
	  // get the time delay
	  timemax = time[PriElementStart*SecElementNum + SecElementStart]; 
	  
	  for(i=PriElementStart; i< PriElementStop ; i++)
	  {
		  for(j = SecElementStart; j< SecElementStop; j++)
		  {
				if(timemax < time[i*SecElementNum + j ])  timemax = time[i*SecElementNum + j ] ;
		  }
	  }
/*	  
	  print("***********\n") ;
	  print("    x1 x2 x3\n")  ;
	  print("y1  0  3  6	\n") ;
	  print("y2  1  4  7	\n") ;
	  print("y3  2  5  8	\n") ;
	  print("***********\n"); 	 
*/	  
	  for(i=PriElementStart; i< PriElementStop ; i++)
	  {
		  for(j = SecElementStart; j< SecElementStop; j++)
		  {
//			   printf("No.[%d][%d],time = %f,Δt=%d\n",i+1,j+1,time[i*SecElementNum + j ],(int)((timemax - time[i*SecElementNum + j ])*1000000+0.5));
			   p->timedelay[p->k][i*SecElementNum + j] = (int)((timemax - time[i*SecElementNum + j ])*1000000+0.5) 								 ;
		  }
	  }
	  p->G_Time[p->k] = timemax;
	  free(time);

}

/*
  Name:  cylinder_1d_id_zero
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 10 -05-11 13:40
  Description:   1 DIMENSION   iD 聚焦法则  ROTATION Z = 0
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
        depth : focus depth of sonic beam
*/
void cylinder_1d_id_zero (PARAMETER_P data, double angle1, double depth)
{
	 PARAMETER_P p = (PARAMETER_P) data ;
	 
	 double c1 ;			 // wave speed
	 double c2 ;
	 double k1 ;  
	 double h0 = p->wedge_p->wg_heigh_fir ;				   // first element high
	
	 int ElementStart = p->element_sel->primary_axis_s - 1;
	 int ElementColumn= p->probe_p->ele_num_pri 		  ;
	 int SelectColumn = p->element_sel->pri_axis_ape	  ;
	 int ElementStop  =   SelectColumn+ ElementStart	  ;
	 
	 double Intervalx = p->probe_p->pri_axis_pitch  ;		   // x direction interval distance
	 double pri_ele_size = p->probe_p->pri_ele_size ;		
	 double wg_pri_axis_reference	= p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	 double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	 double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	 double  a1 = angle1*PI/180;							   // refract angle  
     
	 double width = p->wedge_p->wg_width ;
	 double r = p->specimen->speci_inside_cylindrical / 2.0  ;
	 double zHeight = sqrt(r*r - width*width/4);
	 
	 double wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform	   
	 double FocusDepth = depth;							   //  true focus depth
	
	 double x0,y0,z0;										   // element center coordinate
	 double xP, yP, zP;									   // focus point coordinate
	 // get focus point coordinate			
     double a2                   ;
	 double a2Projection         ;
	 double a1Projection         ;
     double HeightTmp            ;
	 double screwangle           ;
	 double FocusDistance        ;
     double TmpAngle1, TmpAngle2 ;
	 double InPointy, InPointz   ;
	 // variable for search method	// 
	 double xTemp, yTemp, zTemp  ;
	 double tl1, tl2, tl3   ; 
	 int i, j               ; 
	 double timemax         ;
	 double dis1  , dis2    ;
	 double TempLength ;
	 double xxx,yyy;
	 double SearchPoint , SearchLength  ,TempResult ;
	 double sina,tana,sinb , tanb;
	 int count ;
	 if(ElementStop > ElementColumn)  return ;
	 if(p->wedge_p->wg_roof_angle != 0 ) return ; //当ROOF 角不等于0 时,根本无法聚焦
	 z0 = Intervalx*ElementStart*sin(wedgeangle);
	 x0 = Intervalx*ElementStart*cos(wedgeangle);
	 
	 double *time = malloc(ElementColumn * sizeof(double));
	 memset(time,0x0,ElementColumn * sizeof(double));
	 
	 if(p->probe_p->transmi_trans_longi==0)
	 {	 
		 c1  = p->wedge_p->wg_lon_vel ; 			  /* 楔块纵波声速 */ 
		 c2  = p->specimen->speci_transverse_wave;	  /* 样本横波声速 */  
	 }											 
	 else								  
	 {
		 c1  = p->wedge_p->wg_lon_vel ; 			  /* 楔块纵波声速 */	
		 c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
	 }
	
	 // refract coefficient
	 k1 = c1/c2 ;	
	 a2 = asin(sin(a1)*k1) ;
	 // center probe element	(x0,y0,z0)			  
	 z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2) +zHeight;
	 x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
	 y0 = width/2 - wg_sec_elem_offset_fir;
	 // get focus point coordinate 
	 // desirex desirey desirez : 入射点坐标		
     if(y0 == 0)
     {
          TempLength = FocusDepth*tan(a1) ;
		  p->focus_x[p->k] =  x0+TempLength ;
		  TempLength =  fabs(z0 - r)*tan(asin(sin(a1)*k1));
		  p->focus_x[p->k] += TempLength ;
		  p->focus_y[p->k] =  y0 ;
          p->focus_z[p->k] =  FocusDepth + r;
          xP = p->focus_x[p->k];
          yP = p->focus_y[p->k];
          zP = FocusDepth;
		  p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
	      p->probe_y[p->k][0] =   width/2 - wg_sec_elem_offset_fir               ;
	      p->probe_z[p->k][0] =   -h0  + zHeight                                 ;
		  
          for(i = ElementStart; i< ElementStop; i++)
          {
            //  get each element coordinates
            zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle) - r ;
            xTemp = p->probe_x[p->k][0]  + Intervalx*cos(wedgeangle)*i    ;
            yTemp = p->probe_y[p->k][0];
            p->probe_x[p->k][i] = xTemp;
            p->probe_y[p->k][i] = yTemp;
            p->probe_z[p->k][i] = zTemp + r ;   

			if(zTemp>=0)
            {    // kill special situation when probe under the detect surface
                 p->horizontal_x[p->k][i] = xTemp  ;
                 p->horizontal_y[p->k][i] = yTemp  ;
                 p->horizontal_z[p->k][i] = zTemp  ; 
				 continue ;
            }			
            xxx = xP-xTemp ;
           //A1 ==0 时按一维ROOF角等于0的情况作搜索
            TempLength = fabs(xxx);
            SearchLength = TempLength/2 ;

            if(TempLength==0||zTemp==0)
            {
               p->horizontal_x[p->k][i] = xTemp  ;
               p->horizontal_y[p->k][i] = yTemp  ;
               p->horizontal_z[p->k][i] = 0      ;     
               continue ;     
            }                      
            // do search in temporary coordinate
            SearchPoint = 0 ;
            TempResult =  0 ;
            count = 0;                          
            do
            {
               SearchPoint += SearchLength ;      
               tana = SearchPoint/zTemp    ;
               sina = sqrt(tana*tana/(1+tana*tana));
               sinb = sina/k1;
			   
               if(sinb>=1)  
               {
                   SearchLength = -fabs(SearchLength)/2 ;                   
               }
               else
               {
                   tanb = sqrt(sinb*sinb/(1-sinb*sinb))   ;
                   TempResult=SearchPoint+tanb*FocusDepth ;
                   if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
                   if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;                       
               }                   
               count++;         
            }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;      
            p->horizontal_x[p->k][i] = xTemp + fabs(a1)*SearchPoint/a1;
            p->horizontal_y[p->k][i] = 0  ;
            p->horizontal_z[p->k][i] = r ;  	
          }  
     }
	 else
	 {
		 a2Projection = fabs(asin(y0/r))                 ;
	     HeightTmp    = -z0 + sqrt(r*r-y0*y0)            ;
		 if(fabs(a2Projection) > fabs(a2)) return        ;
	     screwangle   = acos(tan(a2Projection)/tan(a2))  ;
	     a1Projection = atan(tan(a1)*cos(screwangle))    ;
		 TmpAngle1 = sin(a1Projection)*r/(r+FocusDepth)  ;
		 TmpAngle1 = a1Projection - TmpAngle1            ;
		 TmpAngle2 = a2Projection - TmpAngle1            ;
	 
		 InPointy = r*sin(a2Projection)                  ;
		 InPointz = r*cos(a2Projection)                  ;

		 zP = (r+FocusDepth)*cos(TmpAngle2)              ;
		 yP = (r+FocusDepth)*sin(TmpAngle2)              ;
		 FocusDistance = (r+FocusDepth) * sin(TmpAngle1) ;
		 xP = x0 + (FocusDistance + HeightTmp * sin(a2Projection)) * tan(screwangle) ;
		 
		 p->focus_x[p->k] = xP			      ;
		 p->focus_y[p->k] = yP			      ;
		 p->focus_z[p->k] = zP			      ;

		 //first element location 
		 p->probe_x[p->k][0] =   wg_pri_axis_reference + wg_pri_elem_offset_fir ;
		 p->probe_y[p->k][0] =   width/2 - wg_sec_elem_offset_fir               ;
		 p->probe_z[p->k][0] =   -h0  + zHeight                                 ;
		 HeightTmp = sqrt(r*r - y0*y0)                                          ;

		 for(i = ElementStart; i< ElementStop; i++)
		  {
				 //  get each element coordinates
				 zTemp = p->probe_z[p->k][0]  - i*Intervalx*sin(wedgeangle);
				 xTemp = p->probe_x[p->k][0]  + Intervalx*cos(wedgeangle)*i;
				 yTemp = p->probe_y[p->k][0]                               ;

				 p->probe_x[p->k][i] = xTemp;
				 p->probe_y[p->k][i] = yTemp;
				 p->probe_z[p->k][i] = zTemp;
				
				 if((yTemp*yTemp+zTemp*zTemp)>=r*r)
				 {
				   	p->horizontal_x[p->k][i] = xTemp ;
				    p->horizontal_y[p->k][i] = yTemp ;
				    p->horizontal_z[p->k][i] = zTemp ;
				    continue ;
				 }	
				 
				 p->horizontal_y[p->k][i] =  InPointy;
				 p->horizontal_z[p->k][i] =  InPointz;

				 tl1 = zTemp - HeightTmp ;
				 tl1 = fabs(tl1 * sin(a2Projection)) ;
	             tl2 = xTemp - xP ;
				 p->horizontal_x[p->k][i] = xTemp - tl2*tl1/(FocusDistance + tl1)  ;
		  }	
	  }
	  //calculate the transfer time
	  for(i = ElementStart; i< ElementStop ; i++)
	  { 	
			   dis1 = sqrt((p->probe_x[p->k][i]-p->horizontal_x[p->k][i])*(p->probe_x[p->k][i]-p->horizontal_x[p->k][i])
							  +(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])*(p->probe_y[p->k][i]-p->horizontal_y[p->k][i])
							  +(p->probe_z[p->k][i]-p->horizontal_z[p->k][i])*(p->probe_z[p->k][i]-p->horizontal_z[p->k][i]));
			   dis2 = sqrt((p->horizontal_x[p->k][i]-p->focus_x[p->k])*(p->horizontal_x[p->k][i]-p->focus_x[p->k])
							 +(p->horizontal_y[p->k][i]-p->focus_y[p->k])*(p->horizontal_y[p->k][i]-p->focus_y[p->k])
							 +(p->horizontal_z[p->k][i]-p->focus_z[p->k])*(p->horizontal_z[p->k][i]-p->focus_z[p->k]));
			   time[i] = dis1/c1+dis2/c2;		   
	  }
	  // get the time delay
	  timemax = time[0];
	  for(i = ElementStart ; i< ElementStop ; i++)
	  {
			if(timemax < time[i])  timemax = time[i] ;					 
	  } 
	  for(i = ElementStart; i< ElementStop ; i++)
	  {
	//		 printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		   p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5); 	
	  }
	  p->G_Time[p->k] = timemax;
	  free(time);
}


void cylinder_2d_roof_od_pr(PARAMETER_P data, double angle1, double angle2, double depth)
{
	 PARAMETER_P p = (PARAMETER_P) data ;
	 
	 double c1 ;			 // wave speed
	 double c2 ;
	 double k1 ;  
	 double r	;
	 double h0 =  p->wedge_p->wg_heigh_fir ;				   // first element high
	 int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
         int Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
         
         if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
         return;
	 
	 double Intervaly = p->probe_p->sec_axis_pitch ;		   // y direction interval
	 double Intervalx = p->probe_p->pri_axis_pitch ;		   // x direction interval distance
	 double pri_ele_size = p->probe_p->pri_ele_size ;		  
	 double wg_pri_axis_reference	= p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	 double wg_sec_axis_reference	= p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	 double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	 double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	 double length = p->wedge_p->wg_length ;							   // wedge length
	 double CylinderExtDiameter = p->specimen->speci_outside_cylindrical ;// outside cylinder diameter
	 double separation = p->wedge_r->wg_separation ;
	
	double  a1 = fabs(angle1*PI/180);						  // refract angle
	double  a2 = fabs(angle2*PI/180);						  // deflect angle
	if(a2>PI/2) a2 = PI-a2 ;

	 double  roofangle = p->wedge_p->wg_roof_angle*PI/180	 ; // roofangle   y-z platform
	 double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform	   
	 double  FocusDepth = depth;							   //  true focus depth
	
	 // refraction calculation 
	 double aSinTemp;
	 double aProjection;
	 double bProjection;
	 double zHightTemp;
	 
	 double tempangle1;
	 double tempangle2;
	 double tempangle3;
	
	 double tempFocusxxx;
	 double tempFocusyyy;
	 double tempFocuszzz;
	 double angle_rotate;
	 
	 double xTemp, yTemp, zTemp ;
	 double x0,y0,z0;			  // element center coordinate
	 //  element offset in each direction
	 double x;
	 double y;
	 double z;
	 double d;
	 double s_x;
	 double s_y;
	 double s_zy;
	 double s_zx;	   
	 // variable for search method
	 double SearchLength , SearchAngle;
	 double TempAngle;
	 double TempResult;
	 double SkewAngleTemp;
	 double SearchPoint ;
	 double TempLength  ;
	 double tana,tanb,sina,sinb ;
	 
	 double r_depth;
	 double a;
	 double b;
	 
	 double xxx;
	 double yyy;
	 double zzz;
	 
	 int i, j; 
	 int count;
	 // variable for time calculation
	 double dis1;
	 double dis2;
	 double timemax ;
	 double *time = malloc(2*Row*Column*sizeof(double));
	 memset(time,0x0,2*Row*Column*sizeof(double));
	 r = CylinderExtDiameter/2.0 ;
	 // kill the impossible condition
	 if(Column<=0||Row<=0) return ;
	 if(p->probe_p->D1_D2 == 0)  Row = 1 ;
	 if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
	
	 if(p->probe_p->transmi_trans_longi==0)
	{   
	c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
	c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
	}                                           
	else                                 
	{
		c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
		c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
	}
	 // refract coefficient
	 k1 = c1/c2 ;
	
	 // element location offset calculate
	 // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
	 // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
	 if((wedgeangle != 0) && (roofangle != 0))
	 {
		  x = 1;
		  y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
		  z = y*tan(roofangle) ;
		  d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
		  s_x  = -Intervaly/d	 ;
		  s_y  = -Intervaly*y/d   ;
		  s_zy	= -Intervaly*(z-tan(wedgeangle))/d	 ;			
	 }
	 else
	 {	 
		  s_y = -Intervaly*cos(roofangle);
		  s_x = 0 ;
		  s_zy = -Intervaly*sin(roofangle);
	 }	   
	 // center probe element	(x0,y0,z0)			  
	 z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2 - s_zy*(Row-1)/2) -sqrt(CylinderExtDiameter*CylinderExtDiameter - length*length)/2;
	 x0 = -length/2 + wg_pri_elem_offset_fir + Intervalx*(Column - 1)*cos(wedgeangle)/2 + (Row-1)*s_x/2 ;
	 if(roofangle<0)
		 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
	 y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir + -separation/2;
	 //以下和1维Cylinder情况类似.
	  //只是将3维的坐标投影到X-Z面
	  //空间角度和投影角偏转角之间的关系
	  // tan(Projection) = tan(SpaceAngle)*cos(SkewAngle);
	  // get focus point coordinate 
	 aSinTemp = asin(k1*sin(a1));
	 aProjection = atan(tan(aSinTemp)*cos(a2));
	 zHightTemp = sqrt(z0*z0 + x0*x0);
	 
	 tempangle1 = asin(r*sin(aProjection)/zHightTemp);
	 tempangle2 = aProjection - tempangle1;   //tempangle2	keep for the below calculation
	 // refract point
	 xxx = r * sin(tempangle2);
	 zzz = r * cos(tempangle2);
	 
	 bProjection = atan(tan(a1)*cos(a2));
	 tempangle3 = asin(sin(bProjection)*r/(r-FocusDepth));	// tempangle3 is sure to be large than 90 degree
	 tempangle1 = tempangle3 - bProjection				 ;
	 // 当折射角和偏转角设定,既可求得最大聚焦深度.
	 // R*SIN(PROJECTION)
	 //if(FocusDepth>r*sin(bProjection))  FocusDepth =	r*sin(bProjection);
	 //print("MaxDepth:%f\n", r*sin(bProjection)) 	   ;  // when a1 and a2 is setted, there will exist a max focus depth		 
	 tempFocusxxx = (r-FocusDepth) * sin(tempangle1+tempangle2) ;
	 tempFocuszzz = (r-FocusDepth) * cos(tempangle1+tempangle2) ;
	 tempFocusyyy = zHightTemp*sin(tempangle2)+sin(tempangle1)*(r-FocusDepth);		   
	 tempFocusyyy = tempFocusyyy*tan(a2) ;
	 //之前  double  a1 = fabs(angle1*PI/180); 						   // refract angle
	 // 		   double	  a2 = fabs(angle2*PI/180); 						   // deflect angle
	 // 		   if(a2>PI/2)	a2 = PI-a2 ;
	 //当a1 a2	  都大于0时,相对于探头中心点的坐标系的入射点坐标
	 // TEMPFOCUSXXX TEMPFOCUSYYY TEMPFOCUSZZZ
	 //求得临时坐标后,再根据a1 a2的实际情况,将坐标的正负值设定
	 if(angle1>=0)
	 {
		 if(angle2>=0)
		 {
			 if(angle2>90)
			 {
				  tempFocusxxx=-tempFocusxxx;
			 }
		 }
		 else
		 {
			 if(a2>PI/2)
			 {
				  tempFocusxxx=-tempFocusxxx;
			 }
			 tempFocusyyy = -tempFocusyyy;
		 }
	 }
	 else
	 {
		 if(angle2>=0)
		 {
			 if(angle2>90)
			 {
				  tempFocusyyy = -tempFocusyyy;
			 }
			 else
			 {
				  tempFocusyyy=-tempFocusyyy;
				  tempFocusxxx = -tempFocusxxx;
			 }
		 }
		 else
		 {
			 if(a2<PI/2)
			 {
				  tempFocusxxx=-tempFocusxxx;
			 }		 
		 }
	 }
	 //坐标旋转,将焦点坐标由中心探头坐标旋转到实际坐标
	p->focus_y[p->k]   = y0 - tempFocusyyy	 ;
	angle_rotate = -asin(x0/zHightTemp) ;
	p->focus_x[p->k] = tempFocusxxx*cos(angle_rotate) - tempFocuszzz*sin(angle_rotate);
	p->focus_z[p->k] = -(tempFocusxxx*sin(angle_rotate) + tempFocuszzz*cos(angle_rotate));

	p->focus_xr[p->k] = p->focus_x[p->k] ;
	p->focus_yr[p->k] = p->focus_y[p->k] ;
	p->focus_zr[p->k] = p->focus_z[p->k] ;

	p->probe_x[p->k][0] =   -length/2 + wg_pri_elem_offset_fir ;
	p->probe_y[p->k][0] =   -wg_sec_axis_reference - wg_sec_elem_offset_fir;
	p->probe_z[p->k][0] =   -h0	- sqrt(CylinderExtDiameter*CylinderExtDiameter- length*length)/2;	  
	tempangle2 = asin(p->focus_x[p->k]/(r-FocusDepth)) ;
	r_depth	= r - FocusDepth ;
	 
	 for(i = 0; i< Column; i++)
	 {
		  for(j = 0; j< Row; j++)
		  {
				//get elements' coordinates
				zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
				xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
				yTemp = p->probe_y[p->k][0] + s_y*j;
				p->probe_x[p->k][i*Row+j] = xTemp  ;
				p->probe_y[p->k][i*Row+j] = yTemp  ;
				p->probe_z[p->k][i*Row+j] = zTemp  ;
				if((xTemp*xTemp+zTemp*zTemp)<r*r)
				{
					p->horizontal_x[p->k][i*Row+j] = xTemp  ;
					p->horizontal_y[p->k][i*Row+j] = yTemp ;
					p->horizontal_z[p->k][i*Row+j] = zTemp ;
					continue ;
				}
				count = 0;
				zHightTemp	 = sqrt(xTemp*xTemp + zTemp*zTemp);
				angle_rotate = -asin(xTemp/zHightTemp)		  ;
				tempangle3	 = tempangle2 + angle_rotate	  ;
				if(tempangle3 == 0)
				{ // 当投影角为0时,在Y方向作搜索.类似于一维搜索
					xxx = 0;
					zzz = r ;			
					if(yTemp == p->focus_y[p->k])
					   yyy = yTemp ;
					else 
					{
						 zTemp = zHightTemp - r;
						 TempLength = fabs(yTemp-p->focus_y[p->k]);
						 SearchLength = TempLength/2 ;
						 SearchPoint = 0 ;
						 TempResult = 0 ;
						 do
						 {
							  SearchPoint += SearchLength ; 	 
							   //printf("search point :%f \n",SearchPoint);
							  tana = SearchPoint/zTemp;
							  sina = sqrt(tana*tana/(1+tana*tana));
							  sinb = sina/k1;				   
							  if(sinb>=1)  
							  {
								   SearchLength = -fabs(SearchLength)/2 ;					
							  }
							  else
							  {
								   tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
								   TempResult=SearchPoint+tanb*FocusDepth ;
								   if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
								   if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;				  
							  } 				  
							  count++;		   
						 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;   
	
						 yyy = yTemp + SearchPoint*(p->focus_y[p->k]-yTemp)/fabs(p->focus_y[p->k] - yTemp); 
						 
					}
					
					p->horizontal_x[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
					p->horizontal_y[p->k][i*Row+j] = yyy ;
					p->horizontal_z[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
					continue ;
				}
				tempangle1	 = fabs(tempangle3);
				SearchLength = tempangle1/2 				  ;
				SearchAngle  = 0							  ;
				TempResult	 = 0							  ;
				
				yyy = fabs(p->focus_y[p->k] - yTemp) ;
				//TempResult = TempLength + SearchGate ;
				do
				{// 搜索X-Z投影角
					  SearchAngle += SearchLength ; 	 
					  //printf("search point :%f \n",SearchPoint);
					  aProjection = atan(zHightTemp*sin(SearchAngle)/(zHightTemp*cos(SearchAngle)-r));
					  SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r-FocusDepth)*sin(tempangle1-SearchAngle)));
					  a = atan(tan(aProjection)/cos(SkewAngleTemp));
					  if(fabs(sin(a)/k1)>=1)
					  {
							 SearchLength = -fabs(SearchLength)/2 ;
					  }
					  else
					  { 					  
						 b = asin(sin(a)/k1)						  ;
						 bProjection = atan(tan(b)*cos(SkewAngleTemp));
						 TempResult  = r*sin(bProjection)/sin(bProjection+tempangle1-SearchAngle)  ;
						 if(TempResult>r_depth)   SearchLength = -fabs(SearchLength)/2 ;
						 if(TempResult<r_depth)   SearchLength =  fabs(SearchLength)/2 ;
					  }
					  count++;
				}while( (fabs(TempResult-r_depth) > SearchGate) && ( count<=20 )) ;    
				// translate into common coordiniate
				if(tempangle3<=0)
					   xxx = -r*sin(SearchAngle) ;
				else   
					   xxx = r*sin(SearchAngle);
				
				if(yTemp>p->focus_y[p->k])
					yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
				else 
					yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
				zzz = r*cos(SearchAngle);
				p->horizontal_x[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
				p->horizontal_y[p->k][i*Row+j] = yyy ;
				p->horizontal_z[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate)) ; 			   
		  }
	 }
	//calculate the transfer time
	for(i=0;i<Column;i++)
	{
		for(j =0;j<Row; j++)
		{
			 dis1 = sqrt((p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])*(p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])
							+(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])*(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])
						   +(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j])*(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j]));
			 dis2 = sqrt((p->horizontal_x[p->k][i*Row+j]-p->focus_x[p->k])*(p->horizontal_x[p->k][i*Row+j]-p->focus_x[p->k])
						   +(p->horizontal_y[p->k][i*Row+j]-p->focus_y[p->k])*(p->horizontal_y[p->k][i*Row+j]-p->focus_y[p->k])
						   +(p->horizontal_z[p->k][i*Row+j]-p->focus_z[p->k])*(p->horizontal_z[p->k][i*Row+j]-p->focus_z[p->k]));
			 time[i*Row+j] = dis1/c1+dis2/c2;		   
		}
	}
	// get the time delay
	timemax = time[0];
	for(i=0;i<Column*Row;i++)
	{
		  if(timemax < time[i])  timemax = time[i] ;				   
	}
/*	
	print("***********\n")   ;
	print("	 x1 x2 x3\n")  ;
	print("y1  0	3  6  \n") ;
	print("y2  1	4  7  \n") ;
	print("y3  2	5  8  \n") ;
	print("***********\n")   ;	   
	print("pitch\n")  */       ;
	for(i = 0 ;i<Column*Row ; i++)
	{
//		 printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		 p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
	}

	h0 =  p->wedge_r->wg_heigh_fir ;				  // first element high
	Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
    Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
    if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
              return;
	Intervaly = p->probe_p->sec_axis_pitch ;		  // y direction interval
	Intervalx = p->probe_p->pri_axis_pitch ;		  // x direction interval distance
	pri_ele_size = p->probe_p->pri_ele_size ;
	wg_pri_axis_reference  = p->wedge_r->wg_pri_axis_reference; /*主轴楔块参考位置*/
	wg_sec_axis_reference  = p->wedge_r->wg_sec_axis_reference; /*次轴楔块参考位置*/
	wg_pri_elem_offset_fir = p->wedge_r->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	wg_sec_elem_offset_fir = p->wedge_r->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	roofangle = p->wedge_r->wg_roof_angle*PI/180   ; // roofangle	y-z platform
	wedgeangle= p->wedge_r->wg_wedge_angle*PI/180  ; // wedgeangle	x-z platform	 
	
	if((wedgeangle != 0) && (roofangle != 0))
	{
		 x = 1;
		 y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
		 z = y*tan(roofangle) ;
		 d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
		 s_x  = -Intervaly*x/d	  ;
		 s_y  = Intervaly*y/d	  ;
		 s_zy  = -Intervaly*(z-x*tan(wedgeangle))/d   ; 		 
	}
	else
	{	
		 s_y = -Intervaly*cos(roofangle);
		 s_x = 0 ;
		 s_zy = -Intervaly*sin(roofangle);
	}  

	// center probe element    (x0,y0,z0)			 

	p->probe_xr[p->k][0] =   -length/2 + wg_pri_elem_offset_fir ;
	p->probe_yr[p->k][0] =   p->probe_y[p->k][0]  - separation;
	p->probe_zr[p->k][0] =   -h0	- sqrt(CylinderExtDiameter*CylinderExtDiameter- length*length)/2;	 
	//receiver time delay calculation 
	s_y = -s_y ;
	 for(i = 0; i< Column; i++)
	 {
		  for(j = 0; j< Row; j++)
		  {
				//get elements' coordinates
				zTemp = p->probe_zr[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
				xTemp = p->probe_xr[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
				yTemp = p->probe_yr[p->k][0] + s_y*j;
				p->probe_xr[p->k][i*Row+j] = xTemp  ;
				p->probe_yr[p->k][i*Row+j] = yTemp  ;
				p->probe_zr[p->k][i*Row+j] = zTemp  ;
				if((xTemp*xTemp+zTemp*zTemp)<r*r)
				{
					p->horizontal_xr[p->k][i*Row+j] = xTemp  ;
					p->horizontal_yr[p->k][i*Row+j] = yTemp ;
					p->horizontal_zr[p->k][i*Row+j] = zTemp ;
					continue ;
				}
				count = 0;
				zHightTemp	 = sqrt(xTemp*xTemp + zTemp*zTemp);
				angle_rotate = -asin(xTemp/zHightTemp)		  ;
				tempangle3	 = tempangle2 + angle_rotate	  ;
				if(tempangle3 == 0)
				{ // 当投影角为0时,在Y方向作搜索.类似于一维搜索
					xxx = 0;
					zzz = r ;			
					if(yTemp == p->focus_yr[p->k])
					   yyy = yTemp ;
					else 
					{
						 zTemp = zHightTemp - r;
						 TempLength = fabs(yTemp-p->focus_y[p->k]);
						 SearchLength = TempLength/2 ;
						 SearchPoint = 0 ;
						 TempResult = 0 ;
						 do
						 {
							  SearchPoint += SearchLength ; 	 
							   //printf("search point :%f \n",SearchPoint);
							  tana = SearchPoint/zTemp;
							  sina = sqrt(tana*tana/(1+tana*tana));
							  sinb = sina/k1;				   
							  if(sinb>=1)  
							  {
								   SearchLength = -fabs(SearchLength)/2 ;					
							  }
							  else
							  {
								   tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
								   TempResult=SearchPoint+tanb*FocusDepth ;
								   if(TempResult>TempLength)   SearchLength = -fabs(SearchLength)/2 ;
								   if(TempResult<TempLength)   SearchLength =  fabs(SearchLength)/2 ;				  
							  } 				  
							  count++;		   
						 }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;   
	
						 yyy = yTemp + SearchPoint*(p->focus_yr[p->k]-yTemp)/fabs(p->focus_yr[p->k] - yTemp); 
						 
					}
					
					p->horizontal_xr[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
					p->horizontal_yr[p->k][i*Row+j] = yyy ;
					p->horizontal_zr[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
					continue ;
				}
				tempangle1	 = fabs(tempangle3);
				SearchLength = tempangle1/2 				  ;
				SearchAngle  = 0							  ;
				TempResult	 = 0							  ;
				
				yyy = fabs(p->focus_yr[p->k] - yTemp) ;
				//TempResult = TempLength + SearchGate ;
				do
				{// 搜索X-Z投影角
					  SearchAngle += SearchLength ; 	 
					  //printf("search point :%f \n",SearchPoint);
					  aProjection = atan(zHightTemp*sin(SearchAngle)/(zHightTemp*cos(SearchAngle)-r));
					  SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r-FocusDepth)*sin(tempangle1-SearchAngle)));
					  a = atan(tan(aProjection)/cos(SkewAngleTemp));
					  if(fabs(sin(a)/k1)>=1)
					  {
							 SearchLength = -fabs(SearchLength)/2 ;
					  }
					  else
					  { 					  
						 b = asin(sin(a)/k1)						  ;
						 bProjection = atan(tan(b)*cos(SkewAngleTemp));
						 TempResult  = r*sin(bProjection)/sin(bProjection+tempangle1-SearchAngle)  ;
						 if(TempResult>r_depth)   SearchLength = -fabs(SearchLength)/2 ;
						 if(TempResult<r_depth)   SearchLength =  fabs(SearchLength)/2 ;
					  }
					  count++;
				}while( (fabs(TempResult-r_depth) > SearchGate) && ( count<=20 )) ;    
				// translate into common coordiniate
				if(tempangle3<=0)
					   xxx = -r*sin(SearchAngle) ;
				else   
					   xxx = r*sin(SearchAngle);
				
				if(yTemp>p->focus_yr[p->k])
					yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
				else 
					yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
				zzz = r*cos(SearchAngle);
				p->horizontal_xr[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)	;
				p->horizontal_yr[p->k][i*Row+j] = yyy ;
				p->horizontal_zr[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate)) ; 			   
		  }
	 }
	//calculate the transfer time
	for(i=0;i<Column;i++)
	{
		for(j =0;j<Row; j++)
		{
			 dis1 = sqrt((p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])*(p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])
							+(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])*(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])
						   +(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j])*(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j]));
			 dis2 = sqrt((p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])*(p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])
						   +(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])*(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])
						   +(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k])*(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k]));
			 time[i*Row+j] = dis1/c1+dis2/c2;		   
		}
	}

	// get the time delay
	timemax = time[0];
	for(i=0;i<Column*Row;i++)
	{
		  if(timemax < time[i])  timemax = time[i] ;				   
	}
	
//	printf("receiver\n") ;
 
	for(i =0 ;i<Column*Row ; i++)
	{
//		 printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		 p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
	}
	p->G_Time[p->k] = timemax;
	free(time);


}
void cylinder_2d_roof_id_pr(PARAMETER_P data, double angle1, double angle2, double depth)
{
   	   PARAMETER_P p = (PARAMETER_P) data ;
	   
	   double c1 ; 		   // wave speed
	   double c2 ;
	   double k1 ;  
	   double r  ;
	   double h0 =  p->wedge_p->wg_heigh_fir ; 				 // first element high
	   int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
       int Column   = p->element_sel->pri_axis_ape;                  // element x direction numble
       if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
                  return;

      	   
	   double Intervaly = p->probe_p->sec_axis_pitch ; 		 // y direction interval
	   double Intervalx = p->probe_p->pri_axis_pitch ; 		 // x direction interval distance
	   double pri_ele_size = p->probe_p->pri_ele_size ;		
	   double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	   double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	   double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	   double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	   double length = p->wedge_p->wg_length ; 							 // wedge length
	   double CylinderIntDiameter = p->specimen->speci_inside_cylindrical ;// outside cylinder diameter
	   double separation  = p->wedge_r->wg_separation ;
	  
	  double  a1 = fabs(angle1*PI/180);						// refract angle
	  double  a2 = fabs(angle2*PI/180);						// deflect angle
	  if(a2>PI/2) a2 = PI-a2 ;

	   double	roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle	y-z platform
	   double	wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle	x-z platform	 
	   double	FocusDepth = depth; 							 //  true focus depth
	  
	   // refraction calculation 
	   double aSinTemp;
	   double aProjection;
	   double bProjection;
	   double zHightTemp;
	   
	   double tempangle1;
	   double tempangle2;
	   double tempangle3;
	  
	   double tempFocusxxx;
	   double tempFocusyyy;
	   double tempFocuszzz;
	   double angle_rotate;
	   
	   double xTemp, yTemp, zTemp ;
	   double x0,y0,z0;			                               // element center coordinate
	   //  element offset in each direction
	   double x;
	   double y;
	   double z;
	   double d;
	   double s_x;
	   double s_y;
	   double s_zy;
	   double s_zx;	 
	   // variable for search method
	   double SearchLength , SearchAngle;
	   double TempAngle;
	   double TempResult;
	   double SkewAngleTemp;
	   double SearchPoint ;
	   double TempLength  ;
	   double tana,tanb,sina,sinb ;
	   double r_depth;
	   double a;
	   double b;
	   
	   double xxx;
	   double yyy;
	   double zzz;
	   
	   int i, j; 
	   int count;
	   // variable for time calculation
	   double dis1;
	   double dis2;
	   double timemax ;
	   double *time = malloc(2*Row*Column*sizeof(double));
	   memset(time,0x0,2*Row*Column*sizeof(double));
	   r = CylinderIntDiameter/2.0 ;
	   // kill the impossible condition
	   if(Column<=0||Row<=0) return ;
	   if(p->probe_p->D1_D2 == 0)  Row = 1 ;
	   if(wedgeangle == PI/2 || roofangle == PI/2)	return ;
	  
	   if(p->probe_p->transmi_trans_longi==0)
        {   
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
        }                                           
        else                                 
        {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
        }
	   // refract coefficient
	   k1 = c1/c2 ;
	   // element location offset calculate
	   // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
       // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
	   if((wedgeangle != 0) && (roofangle != 0))
	   {
			x = 1;
			y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
			z = y*tan(roofangle) ;
			d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
			s_x  = -Intervaly/d     ;
			s_y  = -Intervaly*y/d	;
			s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;		  
	   }
	   else
	   {   
			s_y = -Intervaly*cos(roofangle);
			s_x = 0 ;
			s_zy = -Intervaly*sin(roofangle);
	   }	 
	   // center probe element	  (x0,y0,z0)			
	   z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2 - s_zy*(Row-1)/2) + sqrt(CylinderIntDiameter*CylinderIntDiameter - length*length)/2;
	   x0 = -length/2 + wg_pri_elem_offset_fir + Intervalx*(Column - 1)*cos(wedgeangle)/2 + (Row-1)*s_x/2 ;
	   y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir -separation/2 ;
	  
	   // get focus point coordinate
       //以下和1维Cylinder情况类似.
       //只是将3维的坐标投影到X-Z面
       //空间角度和投影角偏转角之间的关系
       // tan(Projection) = tan(SpaceAngle)*cos(SkewAngle);
		   aSinTemp = asin(k1*sin(a1));
	   aProjection = atan(tan(aSinTemp)*cos(a2));
	   zHightTemp = sqrt(z0*z0 + x0*x0);
	   
	   tempangle1 = asin(r*sin(aProjection)/zHightTemp);
	   tempangle2 = tempangle1 - aProjection;	//tempangle2  keep for the below calculation
	   // refract point
	   xxx = r * sin(tempangle2);
	   zzz = r * cos(tempangle2);
	   
	   bProjection = atan(tan(a1)*cos(a2));
	   tempangle3 = asin(sin(bProjection)*r/(r+FocusDepth)); 
	   tempangle1 = bProjection - tempangle3			   ;  
	   
	   tempFocusxxx = (r+FocusDepth) * sin(tempangle1+tempangle2)              ;
	   tempFocuszzz = -(r+FocusDepth) * cos(tempangle1+tempangle2)             ;
	   tempFocusyyy = zHightTemp*sin(tempangle2)+sin(tempangle1)*(r+FocusDepth);
	   tempFocusyyy = tempFocusyyy*tan(a2)                                     ;
	   //之前  double	a1 = fabs(angle1*PI/180); 							 // refract angle
	   //            double	a2 = fabs(angle2*PI/180); 					 // deflect angle
	   //            if(a2>PI/2)  a2 = PI-a2 ;
	   //当a1 a2	都大于0时,相对于探头中心点的坐标系的入射点坐标
	   // TEMPFOCUSXXX TEMPFOCUSYYY TEMPFOCUSZZZ
	   //求得临时坐标后,再根据a1 a2的实际情况,将坐标的正负值设定
	   if(angle1>=0)
	   {
		   if(angle2>=0)
		   {
			   if(angle2>90)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }
		   }
		   else
		   {
			   if(a2>PI/2)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }
			   tempFocusyyy = -tempFocusyyy;
		   }
	   }
	   else
	   {
		   if(angle2>=0)
		   {
			   if(angle2>90)
			   {
					tempFocusyyy = -tempFocusyyy;
			   }
			   else
			   {
					tempFocusyyy=-tempFocusyyy;
					tempFocusxxx = -tempFocusxxx;
			   }
		   }
		   else
		   {
			   if(a2<PI/2)
			   {
					tempFocusxxx=-tempFocusxxx;
			   }	   
		   }
	   }

	   p->focus_y[p->k]	= y0 - tempFocusyyy                                        ;
	   angle_rotate = asin(x0/zHightTemp)                                      ;
	   p->focus_x[p->k] = tempFocusxxx*cos(angle_rotate) - tempFocuszzz*sin(angle_rotate);
	   p->focus_z[p->k] = -(tempFocusxxx*sin(angle_rotate) + tempFocuszzz*cos(angle_rotate));
	   p->focus_xr[p->k] = p->focus_x[p->k] ;
	   p->focus_yr[p->k] = p->focus_y[p->k] ;
	   p->focus_zr[p->k] = p->focus_z[p->k] ;
	   
	   p->probe_x[p->k][0] =	 -length/2 + wg_pri_elem_offset_fir ;
	   p->probe_y[p->k][0] =	 -wg_sec_axis_reference - wg_sec_elem_offset_fir;
	   p->probe_z[p->k][0] =	 -h0  + sqrt(CylinderIntDiameter*CylinderIntDiameter- length*length)/2;
	   tempangle2 = asin(p->focus_x[p->k]/(r+FocusDepth)) ;
	   r_depth	  = r + FocusDepth ;
	   for(i = 0; i< Column; i++)
	   {
			for(j = 0; j< Row; j++)
			{
				  //get elements' coordinates
				  zTemp = p->probe_z[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
				  xTemp = p->probe_x[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
				  yTemp = p->probe_y[p->k][0] + s_y*j;
				  p->probe_x[p->k][i*Row+j] = xTemp  ;
				  p->probe_y[p->k][i*Row+j] = yTemp  ;
				  p->probe_z[p->k][i*Row+j] = zTemp  ;
				  if((xTemp*xTemp+zTemp*zTemp)>r*r)
				  {
					   	p->horizontal_x[p->k][i*Row+j] = xTemp ;
					    p->horizontal_y[p->k][i*Row+j] = yTemp ;
					    p->horizontal_z[p->k][i*Row+j] = zTemp ;
					    continue ;
				  }
                  count = 0;
				  zHightTemp   = sqrt(xTemp*xTemp + zTemp*zTemp);
				  angle_rotate = asin(xTemp/zHightTemp)		    ;
				  tempangle3   = tempangle2 - angle_rotate		;	  
				  
				  if(tempangle3 == 0)
				  {// 当投影角为0时,在Y方向作搜索.类似于一维搜索
					  xxx = 0;
					  zzz = -r ; 		  
					  if(yTemp == p->focus_y[p->k])
						 yyy = yTemp ;
					  else 
					  {    
						   zTemp = r - zHightTemp;
						   TempLength= fabs(yTemp-p->focus_y[p->k]);
						   SearchLength = TempLength/2 ;
						   SearchPoint  = 0 ;
						   TempResult = 0 ;
						   do
						   {
								SearchPoint += SearchLength ;	   
								tana = SearchPoint/zTemp;
								sina = sqrt(tana*tana/(1+tana*tana));
								sinb = sina/k1; 				 
								if(sinb>=1)  
								{
									 SearchLength = -fabs(SearchLength)/2 ; 				  
								}
								else
								{
									 tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
									 TempResult=SearchPoint+tanb*FocusDepth ;
									 if(TempResult>TempLength)	 SearchLength = -fabs(SearchLength)/2 ;
									 if(TempResult<TempLength)	 SearchLength =  fabs(SearchLength)/2 ; 				
								}					
								count++;		 
						   }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;	
						   yyy = yTemp + SearchPoint*(p->focus_y[p->k]-yTemp)/fabs(p->focus_y[p->k] - yTemp); 
					  }
					  p->horizontal_x[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
					  p->horizontal_y[p->k][i*Row+j] = yyy ;
					  p->horizontal_z[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
					  continue ;
				  }

				  tempangle1   = fabs(tempangle3)               ;
				  SearchLength = tempangle1/2					;
				  SearchAngle  = 0								;
				  TempResult   = 0								;
				  yyy = fabs(p->focus_y[p->k] - yTemp) ;
				  do
				  {// 搜索X-Z投影角
						SearchAngle += SearchLength ;	   
						//printf("search point :%f \n",SearchPoint);
						aProjection = atan(zHightTemp*sin(SearchAngle)/(r - zHightTemp*cos(SearchAngle)));
						SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r+FocusDepth)*sin(tempangle1-SearchAngle)));
						a = atan(tan(aProjection)/cos(SkewAngleTemp));
						if(fabs(sin(a)/k1)>=1)
						{
							   SearchLength = -fabs(SearchLength)/2 ;
						}
						else
						{						
						   b = asin(sin(a)/k1)							;
						   bProjection = atan(tan(b)*cos(SkewAngleTemp));
						   TempResult  = r*sin(bProjection)/sin(bProjection-tempangle1+SearchAngle);
						   if(TempResult>r_depth)	SearchLength =  fabs(SearchLength)/2 ;
						   if(TempResult<r_depth)	SearchLength =	-fabs(SearchLength)/2 ;
						}
						count++;
				  }while( (fabs(r_depth-TempResult) > SearchGate) && ( count<=20 )) ;	 
				  // translate into common coordiniate
                  xxx = (tempangle3<=0)?-r*sin(SearchAngle):r*sin(SearchAngle);				  
				  if(yTemp>p->focus_y[p->k])
					  yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
				  else 
					  yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
				  zzz = -r*cos(SearchAngle);
				  p->horizontal_x[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
				  p->horizontal_y[p->k][i*Row+j] = yyy ;
				  p->horizontal_z[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate) ) ;				 
			}
	   }
	  //calculate the transfer time
	  for(i=0;i<Column;i++)
	  {
		  for(j =0;j<Row; j++)
		  {
			   dis1 = sqrt((p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])*(p->probe_x[p->k][i*Row+j]-p->horizontal_x[p->k][i*Row+j])
							  +(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])*(p->probe_y[p->k][i*Row+j]-p->horizontal_y[p->k][i*Row+j])
							 +(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j])*(p->probe_z[p->k][i*Row+j]-p->horizontal_z[p->k][i*Row+j]));
			   dis2 = sqrt((p->horizontal_x[i*Row+j]-p->focus_x)*(p->horizontal_x[i*Row+j]-p->focus_x)
							 +(p->horizontal_y[i*Row+j]-p->focus_y)*(p->horizontal_y[i*Row+j]-p->focus_y)
							 +(p->horizontal_z[i*Row+j]-p->focus_z)*(p->horizontal_z[i*Row+j]-p->focus_z));
			   time[i*Row+j] = dis1/c1+dis2/c2; 		 
		  }
	  }
	  // get the time delay
	  timemax = time[0];
	  for(i=0;i<Column*Row;i++)
	  {
			if(timemax < time[i])  timemax = time[i] ;					 
	  }
/*	  print("***********\n") ;
	  print("    x1 x2 x3\n")  ;
	  print("y1  1  4  7	\n") ;
	  print("y2  2  5  8	\n") ;
	  print("y3  3  6  9	\n") ;
	  print("***********\n"); 	 
	  print("Pitch\n");*/
	  for(i = 0 ;i<Column*Row ; i++)
	  {
//		  printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		  p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
	  }


	h0 =  p->wedge_r->wg_heigh_fir ;				  // first element high
	Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
    Column   =  p->element_sel->pri_axis_ape;                  // element x direction numble
    if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
              return;
              
	Intervaly = p->probe_p->sec_axis_pitch ;		  // y direction interval
	Intervalx = p->probe_p->pri_axis_pitch ;		  // x direction interval distance
	pri_ele_size = p->probe_p->pri_ele_size ;
	wg_pri_axis_reference  = p->wedge_r->wg_pri_axis_reference; /*主轴楔块参考位置*/
	wg_sec_axis_reference  = p->wedge_r->wg_sec_axis_reference; /*次轴楔块参考位置*/
	wg_pri_elem_offset_fir = p->wedge_r->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	wg_sec_elem_offset_fir = p->wedge_r->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	roofangle = p->wedge_r->wg_roof_angle*PI/180   ; // roofangle	y-z platform
	wedgeangle= p->wedge_r->wg_wedge_angle*PI/180  ; // wedgeangle	x-z platform	 
	
	if((wedgeangle != 0) && (roofangle != 0))
	{
		 x = 1;
		 y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
		 z = y*tan(roofangle) ;
		 d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
		 s_x  = -Intervaly*x/d	  ;
		 s_y  = Intervaly*y/d	  ;
		 s_zy  = -Intervaly*(z-x*tan(wedgeangle))/d   ; 		 
	}
	else
	{	
		 s_y = -Intervaly*cos(roofangle);
		 s_x = 0 ;
		 s_zy = -Intervaly*sin(roofangle);
	}  

	// center probe element    (x0,y0,z0)			 
	
	p->probe_xr[p->k][0] =   -length/2 + wg_pri_elem_offset_fir ;
	p->probe_yr[p->k][0] =   p->probe_y[p->k][0] - separation;
	p->probe_zr[p->k][0] =   -h0  + sqrt(CylinderIntDiameter*CylinderIntDiameter- length*length)/2;

      s_y = -s_y ;
	  for(i = 0; i< Column; i++)
	  {
		   for(j = 0; j< Row; j++)
		   {
				 //get elements' coordinates
				 zTemp = p->probe_zr[p->k][0] + j*s_zy - i*Intervalx*sin(wedgeangle);
				 xTemp = p->probe_xr[p->k][0] + Intervalx*cos(wedgeangle)*i + j*s_x ;
				 yTemp = p->probe_yr[p->k][0] + s_y*j;
				 p->probe_xr[p->k][i*Row+j] = xTemp  ;
				 p->probe_yr[p->k][i*Row+j] = yTemp  ;
				 p->probe_zr[p->k][i*Row+j] = zTemp  ;
				 if((xTemp*xTemp+zTemp*zTemp)>r*r)
				 {
					   p->horizontal_xr[p->k][i*Row+j] = xTemp ;
					   p->horizontal_yr[p->k][i*Row+j] = yTemp ;
					   p->horizontal_zr[p->k][i*Row+j] = zTemp ;
					   continue ;
				 }
				 count = 0;
				 zHightTemp   = sqrt(xTemp*xTemp + zTemp*zTemp);
				 angle_rotate = asin(xTemp/zHightTemp)		   ;
				 tempangle3   = tempangle2 - angle_rotate	   ;	 
				 
				 if(tempangle3 == 0)
				 {// 当投影角为0时,在Y方向作搜索.类似于一维搜索
					 xxx = 0;
					 zzz = -r ; 		 
					 if(yTemp == p->focus_yr[p->k])
						yyy = yTemp ;
					 else 
					 {	  
						  zTemp = r - zHightTemp;
						  TempLength= fabs(yTemp-p->focus_yr[p->k]);
						  SearchLength = TempLength/2 ;
						  SearchPoint  = 0 ;
						  TempResult = 0 ;
						  do
						  {
							   SearchPoint += SearchLength ;	  
							   tana = SearchPoint/zTemp;
							   sina = sqrt(tana*tana/(1+tana*tana));
							   sinb = sina/k1;					
							   if(sinb>=1)	
							   {
									SearchLength = -fabs(SearchLength)/2 ;					 
							   }
							   else
							   {
									tanb = sqrt(sinb*sinb/(1-sinb*sinb)) ;
									TempResult=SearchPoint+tanb*FocusDepth ;
									if(TempResult>TempLength)	SearchLength = -fabs(SearchLength)/2 ;
									if(TempResult<TempLength)	SearchLength =	fabs(SearchLength)/2 ;				   
							   }				   
							   count++; 		
						  }while( (fabs(TempResult-TempLength) > SearchGate) && ( count<=20 )) ;   
						  yyy = yTemp + SearchPoint*(p->focus_yr[p->k]-yTemp)/fabs(p->focus_yr[p->k] - yTemp); 
					 }
					 p->horizontal_xr[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
					 p->horizontal_yr[p->k][i*Row+j] = yyy ;
					 p->horizontal_zr[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate));
					 continue ;
				 }
	  
				 tempangle1   = fabs(tempangle3)			   ;
				 SearchLength = tempangle1/2				   ;
				 SearchAngle  = 0							   ;
				 TempResult   = 0							   ;
				 yyy = fabs(p->focus_yr[p->k] - yTemp) ;
				 do
				 {// 搜索X-Z投影角
					   SearchAngle += SearchLength ;	  
					   //printf("search point :%f \n",SearchPoint);
					   aProjection = atan(zHightTemp*sin(SearchAngle)/(r - zHightTemp*cos(SearchAngle)));
					   SkewAngleTemp = atan(yyy/(zHightTemp*sin(SearchAngle)+(r+FocusDepth)*sin(tempangle1-SearchAngle)));
					   a = atan(tan(aProjection)/cos(SkewAngleTemp));
					   if(fabs(sin(a)/k1)>=1)
					   {
							  SearchLength = -fabs(SearchLength)/2 ;
					   }
					   else
					   {					   
						  b = asin(sin(a)/k1)						   ;
						  bProjection = atan(tan(b)*cos(SkewAngleTemp));
						  TempResult  = r*sin(bProjection)/sin(bProjection-tempangle1+SearchAngle);
						  if(TempResult>r_depth)   SearchLength =  fabs(SearchLength)/2 ;
						  if(TempResult<r_depth)   SearchLength =  -fabs(SearchLength)/2 ;
					   }
					   count++;
				 }while( (fabs(r_depth-TempResult) > SearchGate) && ( count<=20 )) ;	
				 // translate into common coordiniate
				 xxx = (tempangle3<=0)?-r*sin(SearchAngle):r*sin(SearchAngle);				 
				 if(yTemp>p->focus_yr[p->k])
					 yyy = yTemp - sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp) ;
				 else 
					 yyy = yTemp + sin(SearchAngle)*zHightTemp*tan(SkewAngleTemp);
				 zzz = -r*cos(SearchAngle);
				 p->horizontal_xr[p->k][i*Row+j] = xxx*cos(angle_rotate)-zzz*sin(angle_rotate)  ;
				 p->horizontal_yr[p->k][i*Row+j] = yyy ;
				 p->horizontal_zr[p->k][i*Row+j] = -(xxx*sin(angle_rotate)+zzz*cos(angle_rotate) ) ;				
		   }
	  }

	  //calculate the transfer time
	  for(i=0;i<Column;i++)
	  {
		  for(j =0;j<Row; j++)
		  {
			   dis1 = sqrt((p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])*(p->probe_xr[p->k][i*Row+j]-p->horizontal_xr[p->k][i*Row+j])
							  +(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])*(p->probe_yr[p->k][i*Row+j]-p->horizontal_yr[p->k][i*Row+j])
							 +(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j])*(p->probe_zr[p->k][i*Row+j]-p->horizontal_zr[p->k][i*Row+j]));
			   dis2 = sqrt((p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])*(p->horizontal_xr[p->k][i*Row+j]-p->focus_xr[p->k])
							 +(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])*(p->horizontal_yr[p->k][i*Row+j]-p->focus_yr[p->k])
							 +(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k])*(p->horizontal_zr[p->k][i*Row+j]-p->focus_zr[p->k]));
			   time[i*Row+j] = dis1/c1+dis2/c2; 		 
		  }
	  }
	  
	  // get the time delay
	  timemax = time[0];
	  for(i=0;i<Column*Row;i++)
	  {
			if(timemax < time[i])  timemax = time[i] ;					 
	  }
	  
//	  print("receiver\n") ;
	  
	  for(i =0 ;i<Column*Row ; i++)
	  {
//		   printf("No.%d,time[%d] = %f,Δt=%d\n",i+1,i+1,time[i],(int)((timemax - time[i])*1000000+0.5));
		   p->timedelay[p->k][i] = (int)((timemax - time[i])*1000000+0.5);
	  }
	  p->G_Time[p->k] = timemax;
	  free(time);

}


//*********************************************
//          聚焦法则数据清零 
//*********************************************
void clear_focal_law(PARAMETER_P data)
{
    PARAMETER_P p = (PARAMETER_P) data ;
    int i,j;
    int Row      =  p->element_sel->sec_axis_ape  ;                  // element y direction numble
    int Column   = p->element_sel->pri_axis_ape;                     // element x direction numble
    if((Row*Column) > (p->probe_p->ele_num_pri * p->probe_p->ele_num_sec))
              return;
    
    for(i=0;i < p->k+1;i++)
    {
        //先清除焦点
        p->focus_x[i] = 0;
        p->focus_y[i] = 0;
        p->focus_z[i] = 0;
        
        p->focus_xr[i] = 0;
        p->focus_yr[i] = 0;
        p->focus_zr[i] = 0;
        
        for(j =0 ;j<(Column*Row) ; j++)
        {
            p->probe_x[i][j] = 0;
            p->probe_y[i][j] = 0;
            p->probe_z[i][j] = 0;
            
            p->probe_xr[i][j] = 0;
            p->probe_yr[i][j] = 0;
            p->probe_zr[i][j] = 0;
            
            p->horizontal_x[i][j] = 0;
            p->horizontal_y[i][j] = 0;
            p->horizontal_z[i][j] = 0;
            
            p->horizontal_xr[i][j] = 0;
            p->horizontal_yr[i][j] = 0;
            p->horizontal_zr[i][j] = 0;
        }    
    }

    for(i=0;i<MAXNUM;i++)
    {
	p->G_Time[i] = 0;
	p->G_delay[i] = 0;
	for(j=0;j<ELE_NUM;j++)
		p->timedelay[i][j] = 0;
    }
}

/*
  Name: ProjectionToDepth_1d_roof
  Copyright: Doppler
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the projection parameter into true depth for focuslaw calculation 
  input : data : void pointer to DRAW_UI structure
  return  FALSE : if fail
          TRUE    : if success
  the result will be store in FocusDepth pointer memory
*/

int ProjectionToDepth_1d_roof (PARAMETER_P data, double RefractAngle, double Projection, double* FocusDepth)
{
   	   PARAMETER_P p = (PARAMETER_P) data ;
	   // get the center probe coordinate
       double x0,y0,z0 ;
	   double wedgeangle = p->wedge_p->wg_wedge_angle*PI/180.0;
       double roofangle  = fabs(p->wedge_p->wg_roof_angle *PI/180.0);
	   double Intervalx  = p->probe_p->pri_axis_pitch;
	   double h0         = p->wedge_p->wg_heigh_fir  ;
	   double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir ;
	   double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir ;
	   double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	   double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/

	   double desirex, desirey, desirez  ;
	   double 	tanw  , tanr   , vx1    , vy1    ,vz1    ;
	   double para1, para2, vx2 ;
	   double min_angle1 , min_angle2 ;
	   double refract_x , refract_y ;
	   double tempR, TempLength ;
	   double len1, len2, len3 ;
	   
       int ElementStart = p->element_sel->primary_axis_s - 1 ;
       int ElementColumn= p->probe_p->ele_num_pri            ;
	   int SelectColumn = p->element_sel->pri_axis_ape       ;
	   int ElementStop  =   SelectColumn+ ElementStart       ;
	   if(ElementStop > ElementColumn) return FALSE          ;

	   double a1 , a2                                       ;
       double c1, c2 ; // velocity in the wedge and specimen
	   if(p->probe_p->transmi_trans_longi==0)
       {   
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
       }                                           
       else                                 
       {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
       }

	   a1  =  RefractAngle*PI/180.0                 ;
       a2  =  asin(sin(a1)*c1/c2)                     ;  

	//  middle probe offset to the wedge coordinate
	z0 = Intervalx*ElementStart*sin(wedgeangle);
	x0 = Intervalx*ElementStart*cos(wedgeangle);

	z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2);
	//x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
        x0 = x0 + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;       
	//if(roofangle<0)
	//	 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;}
	//   y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir;
       // Projection 如果和RefractAngle 不在同一方向
	   
	   if(roofangle == 0)
	   {
	             //roofangle == 0 时,简单三角关系可求出Projection / true depth 之间的关系
                 if( RefractAngle * (Projection - x0 - fabs(tan(a2)*z0) ) <= 0  || a1 == 0  )  
                  {
                 	*FocusDepth = 50 ;       
                        return FALSE;
                  }
				 *FocusDepth = Projection - x0 + tan(a2)*z0          ;
                 *FocusDepth = *FocusDepth / tan(a1)                 ;
                 if(*FocusDepth<=0) return FALSE                     ;
                 return TRUE                                         ;
	   }
	   else
	   {        //当roofangle 不等于0
                if(wedgeangle==0)
		        {
			            desirex = x0                            ;
				   		desirey = y0 - fabs(z0) * tan(roofangle);
				   		desirez = 0                             ;
		    	}
				else
				{
			    		tanw = -tan(wedgeangle);
			            tanr = tan(roofangle)  ;
			            vx1  = tanw*tanr       ;
			            vy1  = -(1+tanw*tanw)  ;
			            vz1  = -tanr           ;
				        // (x-para1)/vx2 = y-para2   
			     		//过探头与楔块面垂直面与Z=0面的交线方程
			    		para1 = x0             ;
			    		para2 = y0 + z0*vz1/vy1;
			    		vx2 = -vy1/vx1         ;
			    		//中心探头与交线的垂直点
			    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
			    		desirey = para2 + (desirex - para1)/vx2            ;
			    		desirez = 0                                        ;
				}                                  
		    	// 求最小入射角
				min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
		    	min_angle2 = sin(min_angle1) * c2 / c1                                               ;
		    	if(min_angle2>=1)
		    	{
		               // 如果最小入射角发生全反射
					   return FALSE ;
		    	}
				else 
				{
				      if(asin(min_angle2)>=fabs(a1))
				      {
					          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
					          refract_x = desirex;
						      refract_y = desirey;
						      a1 = min_angle2 ;
				      }
			          else
			          {
				              // 计算正常情况下焦点的坐标
				              //refract point 折射点坐标 
				              min_angle2 = asin(sin(a1)*c1/c2);
				              tempR = z0*tan(min_angle2);
						      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
						      if(a1 >= 0)
						      {
					                 refract_x = desirex-TempLength*vx2/sqrt(1+vx2*vx2);
							         refract_y = desirey-TempLength/sqrt(1+vx2*vx2)    ;
				              }
				              else
				              {
					                 refract_x = desirex+TempLength*vx2/sqrt(1+vx2*vx2);
							         refract_y = desirey+TempLength/sqrt(1+vx2*vx2);
				              }
			          }		  
					  // 求折射射线坐标直线方程
					  len1 = refract_x - x0;
					  len2 = refract_y - y0;
					  len3 = sqrt(len1*len1+len2*len2)/tan(a1);
                      //   (x - refract_x)   (y - refract_y)       z
                      //   ------------- =  -------------     =   ---
					  //           len1           len2            len3
                      //     Projection  is  in X direction , and the desire i depth in Z direction
                      if( len1 == 0 || a1*(Projection-refract_x) <=0 )   return FALSE   ;
                      *FocusDepth = fabs((Projection - refract_x)*len3/len1)            ;
					  return TRUE  ;
			    }
	   	}
}

/*
  Name: PlaneToDepth_1d_roof 
  Copyright: Doppler             
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the focus Plane parameter into true depth in calculation 
  input : data : void pointer to DRAW_UI structure
  return  FALSE : if fail
          TRUE  : if success
  the result will be store in FocusDepth pointer memory
*/
int PlaneToDepth_1d_roof (PARAMETER_P data, double RefractAngle, double* FocusDepth)
{
	PARAMETER_P p = (PARAMETER_P) data   ;
	// get the center probe coordinate
	double x0, y0, z0 ;
	double wedgeangle = p->wedge_p->wg_wedge_angle*PI/180.0;
	double roofangle  = p->wedge_p->wg_roof_angle*PI/180.0 ;
	double Intervalx  = p->probe_p->pri_axis_pitch;
	double h0         = p->wedge_p->wg_heigh_fir  ;
	double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir ;
	double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir ;
	double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	//  ssssssssssssssssssssssssssss
	double xPt1  = p->focal_point->offset_start ;
	double zPt1  = p->focal_point->depth_start  ;

	double xPt2  = p->focal_point->offset_end  ;
	double zPt2  = p->focal_point->depth_end   ;
	double k1, k2 ;  // beam line and focus plane line gradient
	double b1, b2 ;  // append coefficient ot beam line and focus plane line

	double desirex, desirey, desirez  ;
	double 	tanw  , tanr   , vx1    , vy1    ,vz1        ;
	double para1, para2, vx2 ;
	double min_angle1 , min_angle2 ;
	double refract_x , refract_y ;

	double tempR, TempLength ;
	double len1, len2, len3 ;
	   
	int ElementStart = p->element_sel->primary_axis_s - 1 ;
	int ElementColumn= p->probe_p->ele_num_pri            ;
	int SelectColumn = p->element_sel->pri_axis_ape       ;
	int ElementStop  =   SelectColumn+ ElementStart       ;
	double  a1 , a2  ;
	double xxx , zzz ; 

	double c1, c2 ; // velocity in the wedge and specimen
	if(p->probe_p->transmi_trans_longi==0)
       {   
            c1  = p->wedge_p->wg_lon_vel            ;    /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
       }                                           
       else                                 
       {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
       }

	   a1  =  RefractAngle*PI/180.0                 ;
       a2  =  asin(sin(a1)*c1/c2)                     ;  

	   //  middle probe offset to the wedge coordinate
	   z0 = Intervalx*ElementStart*sin(wedgeangle);
	   x0 = Intervalx*ElementStart*cos(wedgeangle);
	   
       z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2);
       //x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
       x0 = x0 + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
       //if(roofangle<0)
       // 	 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;}
       //y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir;

	   if(roofangle == 0)
	   {
             if(xPt2 != xPt1)
             {
                     k2  = (zPt2 - zPt1)/(xPt2 - xPt1)               ;
                     if(a1!=0) 
                     {                  
            			 k1  = 1/tan(a1)                             ;
            			 if( k1 == k2 ) 
  				 {
				 	*FocusDepth = 50 ;       
				        return FALSE;
				  }
                         b1 = -k1*(x0 + tan(a2) * fabs(z0))          ;
            			 b2 = -k2 * xPt1 + zPt1                      ;
            			 xxx = (b1-b2) / ( k2 - k1)                  ;
            			 zzz = k1 * (xxx - (x0 + tan(a2) * fabs(z0)));
            			 if(zzz <= 0)     
				 {
		                 	*FocusDepth = 50 ;       
		                        return FALSE;
		                 }
            			 *FocusDepth = zzz                           ;
                     }
                     else
                     {
                         zzz = k2*x0 +b2            ;
                         if(zzz<=0)     
			 {
		         	*FocusDepth = 50 ;       
		                return FALSE;
		          }
                         *FocusDepth = zzz          ;  
                     }
             }
             else
             {
                     if( a1 == 0 )   	   {
                 	*FocusDepth = 50 ;       
                        return FALSE;
                     }
                     k1 = 1/tan(a1)                               ;
                     zzz = k1 * (xPt1 - (x0 + tan(a2) * fabs(z0)));
                     if(zzz<=0)         {
                 	*FocusDepth = 50 ;       
                        return FALSE;
                     }
                     *FocusDepth = zzz                            ;
             }
			 return TRUE  ;
	   }
	   else
	   {        //当roofangle 不等于0
			    if(wedgeangle==0)
				{
					desirex = x0 ;
					desirey = y0 - fabs(z0) * tan(roofangle);
					desirez = 0  ;
				}
				else
				{
			    		tanw = -tan(wedgeangle);
			            tanr = tan(roofangle)  ;
			            vx1  = tanw*tanr       ;
			            vy1  = -(1+tanw*tanw)  ;
			            vz1  = -tanr           ;
				        // (x-para1)/vx2 = y-para2   
			     		//过探头与楔块面垂直面与Z=0面的交线方程
			    		para1 = x0             ;
			    		para2 = y0 + z0*vz1/vy1;
			    		vx2 = -vy1/vx1         ;
			    		//中心探头与交线的垂直点
			    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
			    		desirey = para2 + (desirex - para1)/vx2            ;
			    		desirez = 0                                        ;
				}                                  
		    	// 求最小入射角
				min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
		    	min_angle2 = sin(min_angle1)*c2/c1;
		    	if(min_angle2>=1)
		    	{
		            // 如果最小入射角发生全反射取焦点为中心探头坐�
					return FALSE ;
		    	}
				else 
				{
				      if(asin(min_angle2)>=fabs(a1))
				      {
				          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
				          refract_x = desirex;
					      refract_y = desirey;
					      a1 = min_angle2 ;
				      }
			          else
			          {
			              // 计算正常情况下焦点的坐标
			              //refract point 折射点坐标 
			              min_angle2 = asin(sin(a1)*c1/c2);
			              tempR = z0*tan(min_angle2);
					      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
					      if(a1 >= 0)
					      {
				                 refract_x = desirex - TempLength * vx2/sqrt(1+vx2*vx2);
						         refract_y = desirey - TempLength / sqrt(1+vx2*vx2)    ;
			              }
			              else
			              {
				                 refract_x = desirex + TempLength*vx2 / sqrt(1+vx2*vx2);
						         refract_y = desirey + TempLength / sqrt(1+vx2*vx2)    ;
			              }
			          }		  
					  // 求折射射线坐标直线方程
			          len1 = refract_x - x0;
					  len2 = refract_y - y0;
					  len3 = sqrt(len1*len1+len2*len2)/tan(a1);
                      //       (x - refract_x)       (y - refract_y)         z
                      //        ------------- =     -------------      =    ---
					  //               len1               len2              len3
                      //     Projection  is  in X direction , and the desire i depth in Z direction
                      //     a1 = atan(len1/len3);  
                      //     将折射角投影到 Y=0 上.则和ROOF角等于0时情况一样
					  if(xPt2 == xPt1)
					  {
                              k2  = (zPt2 - zPt1)/(xPt2 - xPt1) 	;
        					  k1  = len1/len3 						;
        					  k1  = 1/k1                            ;
        					  if( k1 == k2 )
        							return FALSE                    ;
        					  b1 = -k1*refract_x        			;
        					  b2 = -k2 * xPt1 + zPt1				;
        					  xxx = (b1-b2) / ( k2 - k1)			;
        					  zzz = k1 * (xxx - refract_x)          ;
        					  if( zzz<=0 )  return FALSE            ;
        					  *FocusDepth = zzz 					;
                      }
                      else
                      {
                              k1  = len1/len3 						;
        					  k1  = 1/k1                            ;
			                  zzz = k1 * (xPt1 - refract_x)         ;
        					  if( zzz<=0 )  return FALSE            ;
                      }
                      

			    }
	   	}
        return TRUE                           ;
}
/*
  Name: AngleSteerToRefract_1d_roof 
  Copyright: Doppler             
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the focus steer parameter into true depth in calculation 
  input : data : void pointer to DRAW_UI structure
  return  FALSE : if fail
          TRUE  : if success
  the result will be store in FocusDepth pointer memory
*/

int AngleSteerToRefract_1d_roof( PARAMETER_P data , double beam_steer_angle, double* RefractAngle)
{
   	   PARAMETER_P p = (PARAMETER_P) data ;
	   // get the center probe coordinate
       double x0,y0,z0 ;
	   double wedgeangle = p->wedge_p->wg_wedge_angle*PI/180.0;
       double roofangle  = p->wedge_p->wg_roof_angle*PI/180.0 ;
	   double Intervalx  = p->probe_p->pri_axis_pitch;
	   double h0         = p->wedge_p->wg_heigh_fir  ;
	   double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir ;
	   double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir ;

	   double desirex, desirey, desirez               ;
	   double desirex1, desirey1,desirez1             ;
	   double tanw  , tanr   , vx1    , vy1    ,vz1   ;
	   double para1, para2 ,vx2                       ;
	   double tempAngle                               ;
	   double  x1 , y1 , z1                           ;
	   double temp, TempLength                        ;
	   double len1, len2, len3                        ;

	   int ElementStart = p->element_sel->primary_axis_s - 1 ;
       int ElementColumn= p->probe_p->ele_num_pri            ;
	   int SelectColumn = p->element_sel->pri_axis_ape       ;
	   int ElementStop  =   SelectColumn+ ElementStart       ;
	   double a1 , a2  ;

	   a1 = beam_steer_angle * PI / 180.0                 ;
	   a2 = a1 + wedgeangle                                  ;
       
       double c1, c2 ; // velocity in the wedge and specimen
	   if(p->probe_p->transmi_trans_longi==0)
       {   
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
            c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
       }                                           
       else                                 
       {
            c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
            c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
       }
	   
       if(roofangle == 0)
       {
		    if(sin(a2)*c2/c1 >= 1)  return FALSE ;
		    *RefractAngle = asin(sin(a2)*c2/c1) * 180 / PI   ;
       }
	   else
	   {        //当roofangle 不等于0
			    //  middle probe offset to the wedge coordinate
	 		    z0 = Intervalx*ElementStart*sin(wedgeangle);
			    x0 = Intervalx*ElementStart*cos(wedgeangle);
			   
				z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2);
				x0 = x0 + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
				if(roofangle<0)
					{wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;}
				y0 = - wg_sec_elem_offset_fir;


				if(wedgeangle==0)
		        {
			            desirex = x0                             ;
				   		desirey = y0 - fabs(z0) * tan(roofangle) ;
				   		desirez = 0                              ;
		    	}
				else
				{
			    		tanw = -tan(wedgeangle)                  ;
			            tanr = tan(roofangle)                    ;
			            vx1  = tanw                              ;
			            vy1  = tanr                              ;
			            vz1  =  -1                               ;

				        //垂直探头直线. 过中心探头 到Z0 面的交点 
						//		 (x - x0)	         (y -  y0)		    z  - z0
						//		  ------------- =  ------------- =	     ---
						//		     vx1		        vy1 	         vz1
                        //  z = 0
                        desirex = x0 - z0 * vx1 / vz1   ;
						desirey = y0 - z0 * vy1 / vz1   ;
						desirez = 0 ;			 
						tanw = -tan(wedgeangle);
						tanr = tan(roofangle)  ;
						vx1  = tanw*tanr       ;
						vy1  = -(1+tanw*tanw)  ;
						vz1  = -tanr           ;
						// (x-para1)/vx2 = y-para2   
						//过探头与楔块面垂直面与Z=0面的交线方程
						para1 = x0             ;
						para2 = y0 + z0*vz1/vy1;
						vx2 = -vy1/vx1         ;
						//中心探头与交线的垂直点
						desirex1 = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
						desirey1 = para2 + (desirex1 - para1)/vx2 ;
						desirez1 = 0 ;
						
				}       
				TempLength = sqrt((desirex - x0)*(desirex - x0) + (desirey - y0)*(desirey - y0) + (desirez - z0)*(desirez - z0));
                temp  = sqrt((desirex1 - x0)*(desirex1 - x0) + (desirey1 - y0)*(desirey1 - y0) + (desirez1 - z0)*(desirez1 - z0));
				tempAngle = -acos(temp/TempLength)  ;
			
				temp = TempLength * tan(a1) ;
				x1   = x0 - temp * cos(wedgeangle)  ;
				y1   = y0                           ;
				z1   = z0 + temp * sin(wedgeangle)  ;
				if(z1 >= 0) return FALSE   ;
                a2   = sqrt((desirex - x1)*(desirex - x1) + (desirey - y1)*(desirey - y1))/sqrt((desirex - x1)*(desirex - x1) + (desirey - y1)*(desirey - y1) + (desirez - z1)*(desirez - z1)) ;
                
                if(a2*c2/c1>=1)  return FALSE ;
                if(a1 >= tempAngle)     
				      *RefractAngle  = asin(a2*c2/c1) * 180 / PI  ;
                else  
					  *RefractAngle  = -asin(a2*c2/c1)* 180 / PI  ;
	   	}
	   	return TRUE ;
}

/*
  Name: AngleSteerToRefract_1d_roof 
  Copyright: Doppler             
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the focus steer parameter into true depth in calculation 
  input : data : void pointer to DRAW_UI structure
  return  FALSE : if fail
          TRUE  : if success
  the result will be store in FocusDepth pointer memory
*/

int BeamSkewToRefract_1d_roof( PARAMETER_P data , double beam_skew_angle, double* RefractAngle)
{
		   PARAMETER_P p = (PARAMETER_P) data ;
		   //get the center probe coordinate
		   double x0 , y0, z0;
		   double wedgeangle = p->wedge_p->wg_wedge_angle*PI/180.0;
		   double roofangle  = p->wedge_p->wg_roof_angle*PI/180.0 ;
		   double Intervalx  = p->probe_p->pri_axis_pitch;
		   double h0		  = p->wedge_p->wg_heigh_fir  ;
		   double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir ;
		   double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir ;
	
		   double desirex, desirey, desirez 			 ;
		   double	tanw  , tanr   , vx1	, vy1	 ,vz1;
		   double  x1 , y1 , z1 						 ;
		   double temp, TempLength						 ;
		   double len1, len2, len3						 ;
	       double para1 ,para2                           ;
		   int ElementStart = p->element_sel->primary_axis_s - 1 ;
		   int ElementColumn= p->probe_p->ele_num_pri			 ;
		   int SelectColumn = p->element_sel->pri_axis_ape		 ;
		   int ElementStop	=	SelectColumn+ ElementStart		 ;
		   double a1 , a2 , vx2	                             ;
	
		   a1 = beam_skew_angle * PI/180.0					 ;

		   double c1, c2 ; // velocity in the wedge and specimen
		   if(p->probe_p->transmi_trans_longi==0)
		   {   
				c1	= p->wedge_p->wg_lon_vel ;				 /* 楔块纵波声速 */ 
				c2	= p->specimen->speci_transverse_wave;	 /* 样本横波声速 */  
		   }										   
		   else 								
		   {
				c1	= p->wedge_p->wg_lon_vel ;				 /* 楔块纵波声速 */    
				c2	= p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
		   }
		   
		   if(roofangle == 0)
		   {
				  return FALSE   ;
		   }
		   else
		   {	   //当roofangle 不等于0
				   //  middle probe offset to the wedge coordinate
					z0 = Intervalx*ElementStart*sin(wedgeangle)   ;
					x0 = Intervalx*ElementStart*cos(wedgeangle)   ;
				   
					z0 = -(z0 + h0 + Intervalx*(SelectColumn - 1)*sin(wedgeangle)/2)               ;
					x0 = x0 + wg_pri_elem_offset_fir + Intervalx*(SelectColumn-1)*cos(wedgeangle)/2;
					if(roofangle < 0)
						 { wg_sec_elem_offset_fir =  -wg_sec_elem_offset_fir; }
					y0 = -wg_sec_elem_offset_fir; 
					
    	    		tanw = -tan(wedgeangle);
    	            tanr = tan(roofangle)  ;
    	            vx1  = tanw*tanr       ;
    	            vy1  = -(1+tanw*tanw)  ;
    	            vz1  = -tanr           ;
    		        // (x-para1)/vx2 = y-para2   
    	     		// 过探头与楔块面垂直面与Z=0面的交线方程
    	    		para1 = x0             ;
    	    		para2 = y0 + z0*vz1/vy1;
    	    		if(vx1 != 0)    vx2 = -vy1/vx1  ;
					else return FALSE ;
				    desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
	    		    desirey = para2 + (desirex - para1)/vx2 ;
	    		    desirez = 0 ;
                    //      skew line
                    //      -tan(a1)(x-x0) = y-y0   
                    //      get the junction of SkewLine and  (x-para1)/vx2 = y-para2					
                    if(vx1 != 0)
                    {  // 当ROOF 角存在时,VX1不可能等于0
                            tanr = -tan(a1)         ;
                            tanw = 1/vx2            ;
                            
                            x1 = (para2 + tanr*x0 - y0 - tanw*para1) / (tanr-tanw) ;
                            y1 = tanr*(x1-x0) + y0  ;
                    }
                    else 
                    {
                            x1 = x0 + (y0 - para2)/tan(a1)  ;
                            y1 = para2                      ;
                    }
					a2   = sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1))/sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) +  z0*z0) ;
				    if(a2*c2/c1>=1) return FALSE;
					if(x1<desirex)  a2 = -a2  ;
                    *RefractAngle  = asin(a2*c2/c1)*180 / PI ;
			}
		    //print("TransferAngle = %f\n",*RefractAngle);
			return TRUE ;
}


/*
  Name: ProjectionToDepth_2d_roof
  Copyright: Doppler
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the projection parameter into true depth for focuslaw calculation 
  input : data : void pointer to DRAW_UI structure
            angle1 : refract angle 
            angle2 : skew angle 
            Projection : projection distance
  return  FALSE : if fail
          TRUE    : if success
  the result will be store in FocusDepth pointer memory
*/

int ProjectionToDepth_2d_roof (PARAMETER_P data, double angle1, double angle2, double Projection, double* FocusDepth)
{
	     PARAMETER_P p = (PARAMETER_P) data ;
	     
	     double c1 ;            // wave speed
	     double c2 ;
	     double k1 ;  
	     double h0 =  p->wedge_p->wg_heigh_fir ;                  // first element high

	     int PriElementStart = p->element_sel->primary_axis_s -1  ;
	     int PriElementNum   = p->probe_p->ele_num_pri            ;
	     int SecElementStart = p->element_sel->seconary_axis_s -1 ;
	     int SecElementNum   = p->probe_p->ele_num_sec            ;
	     int PriElementSelect= p->element_sel->pri_axis_ape       ;
	     int SecElementSelect= p->element_sel->sec_axis_ape       ;
	     int PriElementStop  = PriElementStart +  PriElementSelect;
	     int SecElementStop  = SecElementStart +  SecElementSelect;

	     double Intervaly = p->probe_p->sec_axis_pitch ;          // y direction interval
	     double Intervalx = p->probe_p->pri_axis_pitch ;          // x direction interval distance
	     double pri_ele_size = p->probe_p->pri_ele_size ;
	     double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	     double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	     double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	     double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
             if(angle1 == 0 || angle1 == 90 || fabs(angle2) == 90)  return FALSE  ;
	     double  a1 = angle1*PI/180;                              // refract angle
	     double  a2 = angle2*PI/180;                              // deflect angle
	     double   a1Projection               ;
	     double   a2Projection               ;

	     double  roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle   y-z platform
	     double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform     
	     
	     double x0,y0,z0;                                         // element center coordinate
	     //  element offset in each direction
	     double x;
	     double y;
	     double z;
	     double d;
	     double s_x;
	     double s_y;
	     double s_zy;
	     double s_zx;     
	     
	     if(PriElementStop > PriElementNum )  return FALSE;
	     if(SecElementStop > SecElementNum )  return FALSE;

	     // kill the impossible condition
		 if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;  SecElementStart = 0; SecElementStop = 1;}
	     if(wedgeangle == PI/2 || roofangle == PI/2)  return FALSE;
	 
	     if(p->probe_p->transmi_trans_longi==0)
	     {   
	        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
	        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
	     }                                           
	     else                                 
	     {
	        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
	        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
	     }
	     // refract coefficient    
		 a1Projection = atan(tan(a1)*cos(a2));
		 a2Projection = asin(sin(a1)*c1/c2) ;
		 a2Projection = atan(tan(a2Projection)*cos(a2));


	     // element location offset calculate
	     // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
	     // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
	     if((wedgeangle != 0) && (roofangle != 0))
	     {
	          x = 1;
	          y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
	          z = y*tan(roofangle) ;
	          d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
	          s_x  = -Intervaly/d    ;
	          s_y  = -Intervaly*y/d   ;
	          s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;          
	     }
	     else
	     {   
	          s_y = -Intervaly*cos(roofangle);
	          s_x = 0 ;
	          s_zy = -Intervaly*sin(roofangle);
	     }     
		 //calc offset generated by element selection

	 z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
	 x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
	 y0 = s_y*SecElementStart   ;

	     // center probe element    (x0,y0,z0)            
	  z0 = -(z0 + h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect - 1)/2);
	  x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect - 1)*s_x/2 ;
	  if(roofangle<0)
			 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
	  y0 = y0 -wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect - 1)/2 ;   
 
         *FocusDepth = (Projection - x0 + tan(a2Projection)*z0)/tan(a1Projection) ;
	 if(*FocusDepth<0) return FALSE ; 

          return TRUE ;
}

/*
  Name: PlaneToDepth_2d_roof 
  Copyright: Doppler             
  Author:    Shen Sheng
  Date: 22-04-11 09:19
  Description:  translate the focus Plane parameter into true depth in calculation 
  input : data : void pointer to DRAW_UI structure
            angle1 : refract angle
            angle2 : skew angle
  return  FALSE : if fail
          TRUE  : if success
  the result will be store in FocusDepth pointer memory
*/
int PlaneToDepth_2d_roof (PARAMETER_P data, double angle1, double angle2, double* FocusDepth)
{
	     PARAMETER_P p = (PARAMETER_P) data ;
	     
	     double c1 ;            // wave speed
	     double c2 ;
	     double h0 =  p->wedge_p->wg_heigh_fir ;                  // first element high

	     int PriElementStart = p->element_sel->primary_axis_s -1  ;
	     int PriElementNum   = p->probe_p->ele_num_pri            ;
	     int SecElementStart = p->element_sel->seconary_axis_s -1 ;
		 int SecElementNum   = p->probe_p->ele_num_sec            ;
		 int PriElementSelect= p->element_sel->pri_axis_ape       ;
		 int SecElementSelect= p->element_sel->sec_axis_ape       ;
	     int PriElementStop  = PriElementStart +  PriElementSelect;
		 int SecElementStop  = SecElementStart +  SecElementSelect;

	     double Intervaly = p->probe_p->sec_axis_pitch ;          // y direction interval
	     double Intervalx = p->probe_p->pri_axis_pitch ;          // x direction interval distance
	     double pri_ele_size = p->probe_p->pri_ele_size ;
	     double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference; /*主轴楔块参考位置*/
	     double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference; /*次轴楔块参考位置*/
	     double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	     double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	     double  a1 = angle1*PI/180;                              // refract angle
	     double  a2 = angle2*PI/180;                              // deflect angle
	     double   a1Projection               ;
		 double   a2Projection               ;

		 //  ssssssssssssssssssssssssssss
		 double xPt1  = p->focal_point->offset_start ;
		 double zPt1  = p->focal_point->depth_start  ;
		 
		 double xPt2  = p->focal_point->offset_end  ;
		 double zPt2  = p->focal_point->depth_end	 ;
		 double k1, k2 ;  // beam line and focus plane line gradient
		 double b1, b2 ;  // append coefficient ot beam line and focus plane line

	     double  roofangle = p->wedge_p->wg_roof_angle*PI/180   ; // roofangle   y-z platform
	     double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180  ; // wedgeangle  x-z platform     
	     
	     double xTemp, yTemp, zTemp ;
	     double x0,y0,z0;                                         // element center coordinate
	     double xP, yP, zP;                                       // focus point coordinate
	     //  element offset in each direction
	     double x;
	     double y;
	     double z;
	     double d;
	     double s_x;
	     double s_y;
	     double s_zy;
	     double s_zx;     
	
		 if(PriElementStop > PriElementNum )  return ;
		 if(SecElementStop > SecElementNum )  return ;	 
     
	     // kill the impossible condition
		 if(p->probe_p->D1_D2 == 0) {SecElementNum = 1;  SecElementStart = 0; SecElementStop = 1;}
	     if(wedgeangle == PI/2 || roofangle == PI/2)  return ;
	 
	     if(p->probe_p->transmi_trans_longi==0)
	     {   
	        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
	        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
	     }                                           
	     else                                 
	     {
	        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
	        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
	     }
		 a1Projection = atan(tan(a1)*cos(a2));
		 a2Projection = asin(sin(a1)*c1/c2) ;
		 a2Projection = atan(tan(a2Projection)*cos(a2));

	     // element location offset calculate
	     // 在ROOF角不等于0时,在垂直于WEDGE 角上边方向上每走一段距离
	     // 对应的 X Y Z 三个方向上都会有一个位置变化SX SY SZ
	     if((wedgeangle != 0) && (roofangle != 0))
	     {
	          x = 1;
	          y = cos(roofangle)/(sin(roofangle)*sin(wedgeangle)*cos(wedgeangle)) ;
	          z = y*tan(roofangle) ;
	          d = sqrt((y*y)/(cos(roofangle)*cos(roofangle)) - 1/(cos(wedgeangle)*cos(wedgeangle))); 
	          s_x  = -Intervaly/d    ;
	          s_y  = -Intervaly*y/d   ;
	          s_zy  = -Intervaly*(z-tan(wedgeangle))/d   ;          
	     }
	     else
	     {   
	          s_y = -Intervaly*cos(roofangle);
	          s_x = 0 ;
	          s_zy = -Intervaly*sin(roofangle);
	     }     
		 //calc offset generated by element selection

		 z0 = SecElementStart*s_zy - PriElementStart*Intervalx*sin(wedgeangle);
		 x0 = Intervalx*cos(wedgeangle)*PriElementStart + SecElementStart*s_x ;
		 y0 = s_y*SecElementStart	;
		 
		 // center probe element	(x0,y0,z0)			  
		 z0 = -(z0 + h0 + Intervalx*(PriElementSelect - 1)*sin(wedgeangle)/2 - s_zy*(SecElementSelect - 1)/2);
		 x0 = x0 + wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(PriElementSelect - 1)*cos(wedgeangle)/2 + (SecElementSelect - 1)*s_x/2 ;
		 if(roofangle<0)
			 wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;
		 y0 = y0 -wg_sec_axis_reference - wg_sec_elem_offset_fir + s_y*(SecElementSelect - 1)/2 ;	

       
		if(xPt2 != xPt1)
		{
				k2	= (zPt2 - zPt1)/(xPt2 - xPt1)	  ;
				k1	= 1/tan(a1Projection)  		      ;
				if( k1 == k2 )
					  return FALSE					  ;
				b1 = -k1*(x0 + fabs(z0)*tan(a2Projection))  ;
				b2 = -k2 * xPt1 + zPt1				  ;
				x = (b1-b2) / ( k2 - k1)			  ;
				z = k1 * (x - x0 - fabs(z0)*tan(a2Projection)) ;
				if( z<=0 )  return FALSE			  ;
				*FocusDepth = z					      ;
		}
		else
		{
                k1 = 1/tan(a1Projection)              ;
				*FocusDepth = k1 * (xPt1 - x0 - tan(a2Projection)*fabs(z0));
				if(*FocusDepth<0) return FALSE	      ; 
		}
		return TRUE 						          ;

}




/*
  Name:  flat_1d_pr_auto_depth
  Copyright: Dopplor 
  Author: Sheng Shen
  Date: 17-03-11 13:40
  Description:   平面工件1D 的双晶 聚焦法则 
  Input:  data: structure point to wedge probe .... coefficients
        angle1: refract angle
  return : Depth  
*/

int flat_1d_pr_auto_depth(PARAMETER_P data, double angle1, double* Depth)
{
	PARAMETER_P p = (PARAMETER_P) data ;
	
	double c1 ;			// wave speed
	double c2 ;
	double k1 ;  
	double h0 =  p->wedge_p->wg_heigh_fir ;				  // first element high
	int Column   = p->element_sel->pri_axis_ape;				  // element x direction numble
	double Intervalx = p->probe_p->pri_axis_pitch ;		  // x direction interval distance
	double pri_ele_size = p->probe_p->pri_ele_size ;
	double wg_pri_axis_reference  = p->wedge_p->wg_pri_axis_reference ; /*主轴楔块参考位置*/
	double wg_sec_axis_reference  = p->wedge_p->wg_sec_axis_reference ; /*次轴楔块参考位置*/
	double wg_pri_elem_offset_fir = p->wedge_p->wg_pri_elem_offset_fir;/*第一主轴阵元偏移*/
	double wg_sec_elem_offset_fir = p->wedge_p->wg_sec_elem_offset_fir;/*第一次轴阵元偏移*/
	double  a1 = angle1*PI/180;							  // refract angle	
	double  roofangle = p->wedge_p->wg_roof_angle*PI/180	; // roofangle	 y-z platform
	double  wedgeangle= p->wedge_p->wg_wedge_angle*PI/180	; // wedgeangle  x-z platform	  
    double separation = p->wedge_r->wg_separation          ;
	double x0,y0,z0;										  // element center coordinate
	double xP, yP, zP; 									  // focus point coordinate
	// get focus point coordinate		   
	double tanw;
	double tanr;
	double para1,para2;
	double vx1,vy1,vz1,vx2,vy2,vz2;
	double desirex,desirey,desirez;
	double min_angle1 ;
	double min_angle2 ;
	double refract_x;
	double refract_y;
	double tempR;
  double TempLength ;
	double len1 , len2;
	
	// kill the impossible condition
	if(Column<=0) return FALSE;
	if(wedgeangle == PI/2 || roofangle == PI/2)  return FALSE ;
	if(roofangle <=  0)  return FALSE ;
	if(p->probe_p->transmi_trans_longi==0)
    {   
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */ 
        c2  = p->specimen->speci_transverse_wave;    /* 样本横波声速 */  
    }                                           
    else                                 
    {
        c1  = p->wedge_p->wg_lon_vel ;               /* 楔块纵波声速 */    
        c2  = p->specimen->speci_longitudinal_wave;  /* 样本纵波声速 */
    }

	// refract coefficient
	k1 = c1/c2 ; 	  
	// center probe element    (x0,y0,z0)			 
	z0 = -(h0 + Intervalx*(Column - 1)*sin(wedgeangle)/2);
	x0 = wg_pri_axis_reference + wg_pri_elem_offset_fir + Intervalx*(Column-1)*cos(wedgeangle)/2;
    if(roofangle<0)
		 {wg_sec_elem_offset_fir = -wg_sec_elem_offset_fir;a1=-a1;}
	y0 = -wg_sec_axis_reference - wg_sec_elem_offset_fir;
    // get focus point coordinate 
    // desirex desirey desirez : 入射点坐标          

	if(wedgeangle==0)
    {
            desirex = x0 ;
	   		desirey = y0 - fabs(z0)*tan(roofangle);
	   		desirez = 0  ;
    }
	else
	{
	    	tanw = -tan(wedgeangle);
	        tanr = tan(roofangle)  ;
	        vx1  = tanw*tanr       ;
	        vy1  = -(1+tanw*tanw)  ;
	        vz1  = -tanr           ;
	        // (x-para1)/vx2 = y-para2   
     		//过探头与楔块面垂直面与Z=0面的交线方程
    		para1 = x0             ;
    		para2 = y0 + z0*vz1/vy1;
    		vx2 = -vy1/vx1         ;
    		//中心探头与交线的垂直点
    		desirex = (vx2*x0+y0+para1/vx2 - para2)/(vx2+1/vx2);
    		desirey = para2 + (desirex - para1)/vx2 ;
    		desirez = 0 ;
	}
    // 求最小入射角
	min_angle1 = atan(sqrt((desirex-x0)*(desirex-x0)+(desirey-y0)*(desirey-y0))/fabs(z0));
	min_angle2 = sin(min_angle1)/k1;
	if(min_angle2>=1)
	{
         // 如果最小入射角发生全反射取焦点为中心探头坐标
         return   FALSE ;
	}
	else 
	{
	      if(asin(min_angle2)>=fabs(a1))
	      {
	          //如果 最小折射角大于给定折射角,取垂直点作为中心探头入射点 
	          refract_x = desirex;
		      refract_y = desirey;
		      a1 = min_angle2 ;
	      }
          else
          {
              // 计算正常情况下焦点的坐标
              //refract point 折射点坐标 
              min_angle2 = asin(sin(a1)*k1);
              tempR = z0*tan(min_angle2);
		      TempLength = sqrt(tempR*tempR - (desirex-x0)*(desirex-x0)-(desirey-y0)*(desirey-y0));
		      if(a1>=0)
		      {
                 refract_x = desirex-TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey-TempLength/sqrt(1+vx2*vx2);
              }
              else
              {
                 refract_x = desirex+TempLength*vx2/sqrt(1+vx2*vx2);
		         refract_y = desirey+TempLength/sqrt(1+vx2*vx2);
              }
          }		  
    }
    
    //print("refract_x %f ,refract_y %f \n", refract_x, refract_y);
    
	separation = separation/2   ;
        len1 = fabs(x0 - refract_x) ;
	if(len1 >= separation)  return FALSE ;
	len2 = fabs(y0-refract_y)   ;
	TempLength = len1*separation/len2 ;
	len1 = TempLength - len1 ;
	len2 = separation - len2 ;
	TempLength = sqrt(len1*len1 + len2*len2) ;
	*Depth   = TempLength /tan(a1) ;
	return TRUE ;
}


