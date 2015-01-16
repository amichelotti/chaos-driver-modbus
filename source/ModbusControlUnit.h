/*
 *	ModbusControlUnit.h
 *	!CHOAS
 *	Created by Claudio Bisegni.
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
#ifndef __PowerSupply__ModbusControlUnit__
#define __PowerSupply__ModbusControlUnit__

#include <chaos/cu_toolkit/ControlManager/RTAbstractControlUnit.h>
#include <driver/modbus/core/ChaosModbusInterface.h>



#define MAX_REGISTERS 16

#define MODBUS_PUSH_UNSIGNED_REGISTER(REG_NAME,slave_id,driver,swap,factor,data) {\
uint16_t data16[2];\
uint32_t* fdata=(uint32_t*)data16;\
int ret;\
ret=driver->read_input_registers(REG_NAME ## _REG,2,data16,slave_id);\
if(swap){uint16_t tmp = data16[0];data16[0]=data16[1];data16[1]=tmp;}\
if(ret>0){*data = (uint32_t)(((float)*fdata)*factor);LDBG_ << "pushing " #REG_NAME << ":"<<*data;} else {LAPP_<<" error reading " #REG_NAME " returned:"<<ret;}}

#define MODBUS_PUSH_FLOAT_REGISTER(REG_NAME,slave_id,driver,swap,factor,data) {\
uint16_t data16[2];\
float* fdata=(float*)data16;\
int ret;\
ret=driver->read_input_registers(REG_NAME ## _REG,2,data16,slave_id);\
if(swap){uint16_t tmp = data16[0];data16[0]=data16[1];data16[1]=tmp;}\
if(ret>0){*data = (*fdata * factor);LDBG_ << "pushing " #REG_NAME << ":"<<*data;} else {LAPP_<<" error reading " #REG_NAME " returned:"<<ret;}}
namespace chaos{
	namespace driver {
		namespace modbus {
			
			class ModbusControlUnit : public ::chaos::cu::control_manager::RTAbstractControlUnit {
				PUBLISHABLE_CONTROL_UNIT_INTERFACE(ModbusControlUnit)
				
			protected:
				double *o_out_1;
				
				// init paramter
				string device_hw;
				int slave_id;
				int regadd;
				
				chaos::driver::modbus::ChaosModbusInterface *driver;
				
				/*
				 Define the Control Unit Dataset and Actions
				 */
				void unitDefineActionAndDataset()throw(chaos::CException);
				
				/*(Optional)
				 Initialize the Control Unit and all driver, with received param from MetadataServer
				 */
				void unitInit() throw(chaos::CException);
				/*(Optional)
				 Execute the work, this is called with a determinated delay, it must be as fast as possible
				 */
				void unitStart() throw(chaos::CException);
				/*(Optional)
				 The Control Unit will be stopped
				 */
				void unitStop() throw(chaos::CException);
				/*(Optional)
				 The Control Unit will be deinitialized and disposed
				 */
				void unitDeinit() throw(chaos::CException);
				
				void unitRun() throw(chaos::CException);
				
				void unitInputAttributeChangedHandler() throw(chaos::CException);
			public:
				/*
				 Construct a new CU with an identifier
				 */
				ModbusControlUnit(const string& _control_unit_id,
								  const string& _control_unit_param,
								  const ControlUnitDriverList& _control_unit_drivers);
				
				/*
				 Base destructor
				 */
				~ModbusControlUnit();
			};
		}
	}
}

#endif /* defined(__PowerSupply__ModbusControlUnit__) */
