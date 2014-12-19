/*
 *	modbusDriver.cpp
 *	!CHOAS
 *	Created by Andrea Michelotti
 *
 *    	Copyright 2013 INFN, National Institute of Nuclear Physics
 *
 *    	Licensed under the Apache License, Version 2.0 (the "License");
 *    	you may not use this file except in compliance with the License.
 *    	You may obtain a copy of the License at
 *
 *    	http://www.apache.org/licenses/LICENSE-2.0
 *
 *    	Unless required by applicable law or agreed to in writing, software
 *    	distributed under the License is distributed on an "AS IS" BASIS,
 *    	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    	See the License for the specific language governing permissions and
 *    	limitations under the License.
 */
#include "modbusDriver.h"
#include <common/modbus/modbus.h>
#include <string>

#include <chaos/cu_toolkit/driver_manager/driver/AbstractDriverPlugin.h>
#include <boost/regex.hpp>

#define modbusLAPP_		LAPP_ << "[modbusDriver] "
#define modbusLDBG_		LDBG_ << "[modbusDriver] "
#define modbusLERR_		LERR_ << "[modbusDriver] "



//GET_PLUGIN_CLASS_DEFINITION
//we need only to define the driver because we don't are makeing a plugin
OPEN_CU_DRIVER_PLUGIN_CLASS_DEFINITION(modbusDriver, 1.0.0, chaos::driver::modbus::modbusDriver)
REGISTER_CU_DRIVER_PLUGIN_CLASS_INIT_ATTRIBUTE(modbusDriver, http_address/dnsname:port)
CLOSE_CU_DRIVER_PLUGIN_CLASS_DEFINITION

//register the two plugin
OPEN_REGISTER_PLUGIN
REGISTER_PLUGIN(driver::modbus::modbusDriver)
CLOSE_REGISTER_PLUGIN

using namespace chaos::driver::modbus;

//default constructor definition
DEFAULT_CU_DRIVER_PLUGIN_CONSTRUCTOR_WITH_NS(chaos::driver::modbus, modbusDriver) {
	driver=boost::shared_ptr< ::common::modbus::ModBusDrv>();
}

//default descrutcor
modbusDriver::~modbusDriver() {
	
}

void modbusDriver::driverInit(const char *initParameter) throw(chaos::CException) {
	boost::smatch match;
	std::string inputStr = initParameter;
	LDBG_ << __FUNCTION__ <<" initialisation string:\""<<inputStr<<"\"";
    if(driver.get()){
        throw chaos::CException(1, "Already Initialised", __FUNCTION__);
    }
    driver = ::common::modbus::ModBusDrv::getInstance(inputStr);
    
        

    if(driver.get() == NULL){
         throw chaos::CException(1, "Cannot allocate resources for Modbus driver", "modbusDriver::driverInit");
    }

    LDBG_ << __FUNCTION__ <<" driver count:"<<driver.use_count()<<" pointer:x"<<hex<<driver.get()<<dec;

    if(driver->initFromParams()<=0){
        driverDeinit();
        throw chaos::CException(1, "Cannot initialize modbus driver", "modbusDriver::driverInit");
    }
    
     if(driver->connect()==false){
         driverDeinit();
         throw chaos::CException(1, "cannot perform modbus connect", __FUNCTION__);

      }
    
}

void modbusDriver::driverDeinit() throw(chaos::CException) {
    
    LDBG_<< "Deinit modbus, deallocating driver x"<<hex<<driver.get()<<dec<<" that is used by :"<<driver.use_count()-1;
    if(driver.use_count()==2){
            ::common::modbus::ModBusDrv::removeInstance(driver);

            driver.reset();
            LDBG_<<"driver deallocated: "<<driver.use_count();
     }
    
}

cu_driver::MsgManagmentResultType::MsgManagmentResult  modbusDriver::execOpcode(cu_driver::DrvMsgPtr cmd){
    boost::mutex::scoped_lock my_lock(lock);
    cu_driver::MsgManagmentResultType::MsgManagmentResult result = cu_driver::MsgManagmentResultType::MMR_EXECUTED;
    chaos::driver::modbus::modbus_iparams_t * in= (chaos::driver::modbus::modbus_iparams_t *) cmd->inputData;
    chaos::driver::modbus::modbus_oparams_t * out =(chaos::driver::modbus::modbus_oparams_t *) cmd->resultData;
    switch(cmd->opcode){
        case OP_INIT:
            modbusLDBG_<< "Init";
            break;
        
        case OP_DEINIT:
            modbusLDBG_<< "DeInit";
            break;
        case OP_CONNECT:
            modbusLDBG_ << "Connecting..";
            out->ret= driver->connect();
            if(out->ret){
                 modbusLDBG_ << "OK";
            } else {
                modbusLDBG_ << "FAILED";

            }
            break;
            
            case OP_CLOSE:
            modbusLDBG_ << "Closing..";
            driver->close();
            
            break;
        case OP_FLUSH:
            modbusLDBG_ << "Flushing data..";
            out->ret= driver->flush();
            if(out->ret){
                modbusLDBG_ << "OK";
            } else {
                modbusLDBG_ << "FAILED";
                
            }
            break;
        case OP_SET_WRITE_TIMEO:{
            int timeo= in->param0;
            modbusLDBG_ << "Set Write TimeOut to:"<<timeo;
            driver->set_write_timeo(timeo);
            break;
        }
        case OP_SET_READ_TIMEO:{
            int timeo= in->param0;
            modbusLDBG_ << "Set Read TimeOut to:"<<timeo;
            driver->set_read_timeo(timeo);
            break;

        }
        case OP_GET_WRITE_TIMEO:{
            out->ret = driver->get_write_timeo();
            modbusLDBG_ << "Get Write TimeOut:"<<out->ret;
            break;

        }
        case OP_GET_READ_TIMEO:{
            out->ret = driver->get_read_timeo();
            modbusLDBG_ << "Get Read TimeOut:"<<out->ret;
            break;
            
        }
            
        case OP_READ_COIL_STATUS:{
            uint8_t* buf=(uint8_t*)(out + 1);
              modbusLDBG_ << "Read Coil status:@"<<in->regadd<<" sizeb:"<<in->sizeb<<" slave:"<<in->slaveid<<" @x"<<hex<<buf;
            out->ret=driver->read_coil_status(in->regadd, in->sizeb,buf,in->slaveid);
            break;
        }
        case OP_READ_INPUT_STATUS:{
            uint8_t* buf=(uint8_t*)(out+ 1);
            modbusLDBG_ << "Read Input status:@"<<in->regadd<<" sizeb:"<<in->sizeb<<" slave:"<<in->slaveid<<" @x"<<hex<<buf;
            out->ret=driver->read_input_status(in->regadd, in->sizeb,buf,in->slaveid);
            break;
        }
        case OP_READ_HOLDING_REGISTERS:{
            uint16_t* buf=(uint16_t*)(out + 1);
            modbusLDBG_ << "Read holding register:@"<<dec<<in->regadd<<" sizeb:"<<in->sizeb<<" slave:"<<in->slaveid<<" @x"<<hex<<buf;
            out->ret=driver->read_holding_registers(in->regadd, in->sizeb>>1,buf,in->slaveid);
            break;
        }
        case OP_READ_INPUT_REGISTERS:{
            uint16_t* buf=(uint16_t*)(out + 1);
            int ret;
            modbusLDBG_ << "Read input register:@"<<dec<<in->regadd<<" sizeb:"<<in->sizeb<<" slave:"<<in->slaveid<<" @x"<<hex<<buf;
            //float* data=(float*)buf;
            ret = driver->read_input_registers(in->regadd, in->sizeb>>1,(uint16_t*)buf,in->slaveid);
           
            //modbusLDBG_ << " DATA:"<<*data;
            out->ret= ret;
            break;
        }
        case OP_REPORT_SLAVE:{
            uint8_t* buf=(uint8_t*)(out+1);
            modbusLDBG_ << "Report slave";

            out->ret = driver->report_slave(buf,in->slaveid);
            break;
        }
            
        case OP_FORCE_SINGLE_COIL:{
            uint8_t value = *(uint8_t*)(in+1);
            modbusLDBG_ << "Force Single Coil:@"<<dec<<in->regadd<<" value:x"<<hex<<value<<" slave:"<<dec<<in->slaveid;

            out->ret = driver->force_single_coil(in->regadd, value,in->slaveid);
            break;
        }
        case OP_PRESET_SINGLE_REGISTER:{
            uint16_t value = *(uint16_t*)(in+1);
            modbusLDBG_ << "Preset single register:@"<<dec<<in->regadd<<" value:x"<<hex<<value<<" slave:"<<dec<<in->slaveid;

            out->ret = driver->preset_single_register(in->regadd, value,in->slaveid);
            break;
        }

        case OP_FORCE_MULTIPLE_COILS:{
            uint8_t* buf = (uint8_t*)(in+1);
            modbusLDBG_ << "Force Multiple Coil:@"<<in->regadd<<" buf:x"<<hex<<buf<<" sizeb:"<<in->sizeb<<" slave:"<<dec<<in->slaveid;

            out->ret = driver->force_multiple_coils(in->regadd, in->sizeb,buf,in->slaveid);
            break;
        }
            
        case OP_FORCE_MULTIPLE_REGISTERS:{
            uint16_t* buf = (uint16_t*)(in+1);
            modbusLDBG_ << "Force Multiple Registers:@"<<dec<<in->regadd<<" buf:x"<<hex<<buf<<" sizeb:"<<in->sizeb<<" slave:"<<dec<<in->slaveid;

            out->ret = driver->force_multiple_registers(in->regadd, in->sizeb,buf,in->slaveid);
            break;
        }
        case OP_WRITE_AND_READ_REGISTERS:{
            uint16_t *ibuf = (uint16_t *)(in+1);
            uint16_t *obuf = (uint16_t *)(out+1);
            modbusLDBG_ << "Write Read Registers:w@"<<in->regadd<<" wbuf:x"<<hex<<ibuf<<" wsizeb:"<<in->sizeb<<"r@"<<in->regadd2<<" rbuf:x"<<hex<<obuf<<" rsizeb:"<<in->sizeb2<<" slave:"<<dec<<in->slaveid;

            out->ret = driver->write_and_read_registers(in->regadd, in->sizeb, ibuf, in->regadd2, in->sizeb2, obuf,in->slaveid);

            
        };
    
    }
    
    return result;

}
