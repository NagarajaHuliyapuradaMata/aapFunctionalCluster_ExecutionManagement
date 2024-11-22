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

int main(
       int   argc
   ,   char* argv[]
){
   SwcServiceEcuM.vInitFunction();
   cpstinfClientSwcServiceEthTp->vInitFunction();
   cpstinfClientSwcServiceDcm->vInitFunction();
   do{
      cpstinfClientSwcServiceEthTp->vMainFunction();
      cpstinfClientSwcServiceDcm->vMainFunction();
   }while(FALSE == SwcServiceEcuM.bGetRequestShutdown());
   cpstinfClientSwcServiceEthTp->vDeInitFunction();
   return 0;
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
