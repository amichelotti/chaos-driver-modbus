/*
 *	modbusDriver
 *	!CHAOS
 *	Created by Andrea Michelotti.
 *
 *    	Copyright 2014 INFN, National Institute of Nuclear Physics
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
#ifndef __driver_modbus_h__
#define __driver_modbus_h__


// include your class/functions headers here

#include <chaos/cu_toolkit/driver_manager/driver/AbstractDriverPlugin.h>
#include <driver/modbus/core/ChaosModbusInterface.h>
#include <common/modbus/ModBus.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

//this need to be out the nasmespace
DEFINE_CU_DRIVER_DEFINITION_PROTOTYPE(modbusDriver)
namespace chaos{
namespace driver {
	namespace modbus {
		namespace cu_driver = chaos::cu::driver_manager::driver;
		
		/*
		 driver definition
		 */
		class modbusDriver: ADD_CU_DRIVER_PLUGIN_SUPERCLASS {
			 void driverInit(const char *initParameter) throw(chaos::CException);
			void driverInit(const chaos::common::data::CDataWrapper& json) throw(chaos::CException);
			void driverDeinit() throw(chaos::CException);

			::common::modbus::AbstractModbusChannel_psh driver;
             boost::mutex lock;
                 
		public:
			modbusDriver();
			~modbusDriver();
			//! Execute a command
			cu_driver::MsgManagmentResultType::MsgManagmentResult execOpcode(cu_driver::DrvMsgPtr cmd);
		};
	}
}
}

#endif
