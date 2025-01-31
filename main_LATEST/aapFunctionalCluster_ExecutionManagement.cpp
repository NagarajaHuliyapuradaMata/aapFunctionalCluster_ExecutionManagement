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

#include "infClientSwcApplControlMotor.hpp"
#include "ProjectSystem.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cmath>

double t = 0;
std::string stringNameVersionRelease    = "RmainLATEST";
std::string stringNameFileInputs        = "log_pProjectVirtualEcu_DigitalTwinMotorPMSM.csv";
std::string stringNameFileOutputs       = "log_pProjectVirtualEcu_DigitalTwinMotorPMSM_RmainLATEST.csv";
std::string stringNameFileConfiguration = "CfgSwcApplDigitalTwinMotorPMSM.hex";

void print_usage(
      int   argc
   ,  char* argv[]
){
   std::cerr
      << "Usage: " << argv[0]               << std::endl
      << "\t[-i <name_file_input>]"         << std::endl
      << "\t[-o <name_file_output>]"        << std::endl
      << "\t[-c <name_file_configuration>]" << std::endl
      << std::endl;
}

typedef enum{
      SwcServiceSchm_eMode_OFFLINE = 0
   ,  SwcServiceSchm_eMode_ONLINE
}Type_SwcServiceSchm_eMode;
Type_SwcServiceSchm_eMode geMode;

int process_argument_commandline(
      int   argc
   ,  char* argv[]
){
   if(1 < argc){
      int  opt;
      bool flag_i = false;
      bool flag_o = false;
      bool flag_c = false;
      std::string argValue_i;
      std::string argValue_o;
      std::string argValue_c;

      while(
            -1
         != (opt = getopt(argc, argv, "i:o:c:"))
      ){
         switch(opt){
            case 'i': flag_i = true; argValue_i = optarg; break;
            case 'o': flag_o = true; argValue_o = optarg; break;
            case 'c': flag_c = true; argValue_c = optarg; break;
            default:
               print_usage(argc, argv);
               return EXIT_FAILURE;
         }
      }

      if(flag_i){
         if(!argValue_i.empty()){
            stringNameFileInputs = argValue_i;
         }
         else{
            print_usage(argc, argv);
         }
      }
      else{
         std::cout << "option -i is required in OFFLINE mode!" << std::endl;
         return EXIT_FAILURE;
      }

      if(flag_o){
         if(!argValue_o.empty()){
            std::cout << "Value for -o: " << argValue_o << std::endl;
         }
         else{
            print_usage(argc, argv);
         }
      }
      else{
         size_t mid = stringNameFileInputs.find('.');
         stringNameFileOutputs  = stringNameFileInputs.substr(0,mid-0);
         stringNameFileOutputs += '_' + stringNameVersionRelease;
         stringNameFileOutputs += stringNameFileInputs.substr(mid);
      }

      if(flag_c){
         if(!argValue_c.empty()){
            stringNameFileConfiguration = argValue_c;
         }
         else{
            print_usage(argc, argv);
         }
      }
      else{
         std::cout << "Default configuration file name: " << stringNameFileConfiguration << std::endl;
      }

      geMode = SwcServiceSchm_eMode_OFFLINE;
   }
   else{
      geMode = SwcServiceSchm_eMode_ONLINE;
   }
   return EXIT_SUCCESS;
}

int main(
      int   argc
   ,  char* argv[]
){
   if(EXIT_SUCCESS == process_argument_commandline(argc, argv)){
      SwcServiceEcuM.vInitFunction();
      if(SwcServiceSchm_eMode_OFFLINE == geMode){
         pinfClientSwcApplControlMotor->InitFunction();
      }
      else{
         cpstinfClientSwcServiceEthTp->vInitFunction();
         cpstinfClientSwcServiceDcm->vInitFunction();
      }

      do{
         if(SwcServiceSchm_eMode_OFFLINE == geMode){
            pinfClientSwcApplControlMotor->MainFunction();
            t += double_dt;
         }
         else{
            cpstinfClientSwcServiceEthTp->vMainFunction();
            cpstinfClientSwcServiceDcm->vMainFunction();
         }
      }while(
            (FALSE == SwcServiceEcuM.bGetRequestShutdown())
         && (0.5 > t)
      );
      if(SwcServiceSchm_eMode_OFFLINE == geMode){
         pinfClientSwcApplControlMotor->DeInitFunction();
      }
      else{
         cpstinfClientSwcServiceEthTp->vDeInitFunction();
      }
      return EXIT_SUCCESS;
   }
   else{
      return EXIT_FAILURE;
   }
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
