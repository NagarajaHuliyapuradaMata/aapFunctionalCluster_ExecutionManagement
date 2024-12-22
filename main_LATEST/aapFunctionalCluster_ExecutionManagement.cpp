/******************************************************************************/
/* File   : aapFunctionalCluster_ExecutionManagement.cpp                      */
/* Author : Nagaraja HULIYAPURADA-MATA                                        */
/* Date   : 01.02.1982                                                        */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "TypesStd.hpp"

#include "interface_ExecutionManagement_ExecutionClient.hpp"

#include "infClientSwcServiceDcm.hpp"
#include "infClientSwcServiceEcuM.hpp"
#include "infClientSwcServiceEthTp.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/
class aapFunctionalCluster_ExecutionManagement:
      public interface_ExecutionManagement_ExecutionClient
{
   private:
      boolean bRequestShutdown;

   public:
      void    Create               (void);
      Result  ReportExecutionState (ExecutionState ValueExecutionState);
};

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
aapFunctionalCluster_ExecutionManagement ExecutionManagement;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
void aapFunctionalCluster_ExecutionManagement::Create(void){
}

Result aapFunctionalCluster_ExecutionManagement::ReportExecutionState(
   ExecutionState ValueExecutionState
){
   UNUSED(ValueExecutionState);
   return 0;
}

/******************************************************************************/
/* Debug code - START                                                         */
/******************************************************************************/
#include "infClientSwcApplDigitalTwinMotorPMSM.hpp"
#include "ProjectSystem.hpp"
double t = 0;
Type_infClientSwcApplDigitalTwinMotorPMSM_stInputs  SwcApplDigitalTwinMotorPMSM_stInputs;
Type_infClientSwcApplDigitalTwinMotorPMSM_stOutputs SwcApplDigitalTwinMotorPMSM_stOutputs;

#include <iostream>
void LogInit(void){
   std::cout << "t"
      << "\t" << "Tm"
      << "\t" << "Va"
      << "\t" << "Vb"
      << "\t" << "Vc"
      << "\t" << "Ia"
      << "\t" << "Ib"
      << "\t" << "Ic"
      << std::endl;
}
void LogUpdate(void){
   std::cout << t
      << "\t" << SwcApplDigitalTwinMotorPMSM_stInputs.Tm
      << "\t" << SwcApplDigitalTwinMotorPMSM_stInputs.Va
      << "\t" << SwcApplDigitalTwinMotorPMSM_stInputs.Vb
      << "\t" << SwcApplDigitalTwinMotorPMSM_stInputs.Vc
      << "\t" << SwcApplDigitalTwinMotorPMSM_stOutputs.Ia
      << "\t" << SwcApplDigitalTwinMotorPMSM_stOutputs.Ib
      << "\t" << SwcApplDigitalTwinMotorPMSM_stOutputs.Ic
      << std::endl;
}
/******************************************************************************/
/* Debug code - END                                                           */
/******************************************************************************/
typedef enum{
      SwcServiceSchm_eMode_OFFLINE = 0
   ,  SwcServiceSchm_eMode_ONLINE
}Type_SwcServiceSchm_eMode;

#include <cmath>
int main(
      int   argc
   ,  char* argv[]
){
   Type_SwcServiceSchm_eMode leMode;
   if(1 < argc){
      leMode = SwcServiceSchm_eMode_OFFLINE;
   }
   else{
      leMode = SwcServiceSchm_eMode_ONLINE;
   }

   SwcServiceEcuM.vInitFunction();
   if(SwcServiceSchm_eMode_OFFLINE == leMode){
      LogInit();
      pinfClientSwcApplDigitalTwinMotorPMSM->InitFunction(
            &SwcApplDigitalTwinMotorPMSM_stInputs
         ,  &SwcApplDigitalTwinMotorPMSM_stOutputs
      );
   }
   else{
      cpstinfClientSwcServiceEthTp->vInitFunction();
      cpstinfClientSwcServiceDcm->vInitFunction();
   }

   do{
      if(SwcServiceSchm_eMode_OFFLINE == leMode){
         SwcApplDigitalTwinMotorPMSM_stInputs.Tm = 1;
         SwcApplDigitalTwinMotorPMSM_stInputs.Va = 220.0*sqrt(2.0)*sin(2*pi*50*t);
         SwcApplDigitalTwinMotorPMSM_stInputs.Vb = 220.0*sqrt(2.0)*sin(2*pi*50*t - 2*pi/3.0);
         SwcApplDigitalTwinMotorPMSM_stInputs.Vc = 220.0*sqrt(2.0)*sin(2*pi*50*t + 2*pi/3.0);
         pinfClientSwcApplDigitalTwinMotorPMSM->MainFunction();
         LogUpdate();
         t += dt;
      }
      else{
         cpstinfClientSwcServiceEthTp->vMainFunction();
         cpstinfClientSwcServiceDcm->vMainFunction();
      }
   }while(
         (FALSE == SwcServiceEcuM.bGetRequestShutdown())
      && (0.5 > t)
   );
   if(SwcServiceSchm_eMode_OFFLINE == leMode){
   }
   else{
      cpstinfClientSwcServiceEthTp->vDeInitFunction();
   }
   return 0;
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
