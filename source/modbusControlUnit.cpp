/*
 *	modbusControlUnit.cpp
 *	!CHOAS
 *	Created by Bisegni Claudio.
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

#include "modbusControlUnit.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace chaos::common::data;
using namespace chaos::cu::control_manager::slow_command;
using namespace chaos::cu::driver_manager::driver;
using namespace driver::modbus;

#define modbusCUAPP_ LAPP_ << "[modbusControlUnit] - "


/*
 Construct a new CU with an identifier
 */
modbusControlUnit::modbusControlUnit(string _device_id):device_id(_device_id) {
}

modbusControlUnit::~modbusControlUnit() {
	
}

/*
 Return the default configuration
 */
void modbusControlUnit::unitDefineActionAndDataset() throw(chaos::CException) {
}

void modbusControlUnit::unitDefineDriver(std::vector<chaos::cu::driver_manager::driver::DrvRequestInfo>& neededDriver) {
	chaos::cu::driver_manager::driver::DrvRequestInfo drv1 = {"modbusDriver", "1.0.0", "parameter" };
	neededDriver.push_back(drv1);
}

// Abstract method for the initialization of the control unit
void modbusControlUnit::unitInit() throw(chaos::CException) {
	modbusCUAPP_ "unitInit";
	//plc_s7_accessor = AbstractControlUnit::getAccessoInstanceByIndex(0);
}

// Abstract method for the start of the control unit
void modbusControlUnit::unitStart() throw(chaos::CException) {
	modbusCUAPP_ "unitStart";
}


//intervalled scheduled method
void modbusControlUnit::unitRun() throw(chaos::CException) {
	modbusCUAPP_ "unitRun";
}

// Abstract method for the stop of the control unit
void modbusControlUnit::unitStop() throw(chaos::CException) {
	modbusCUAPP_ "unitStop";
}

// Abstract method for the deinit of the control unit
void modbusControlUnit::unitDeinit() throw(chaos::CException) {
	modbusCUAPP_ "unitDeinit";
}
