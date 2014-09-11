/*
 *	ModbusControlUnit
 *	!CHAOS
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

#include "ModbusControlUnit.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

using namespace chaos;

using namespace chaos::common::data;
using namespace chaos::common::batch_command;

using namespace chaos::cu::driver_manager::driver;

namespace own =  ::driver::modbus;

#define SCCUAPP LAPP_ << "[ModbusControlUnit - " << getCUID() << "] - "

PUBLISHABLE_CONTROL_UNIT_IMPLEMENTATION(own::ModbusControlUnit)

/*
 Construct a new CU with an identifier
 */
own::ModbusControlUnit::ModbusControlUnit(const string& _control_unit_id,
														const string& _control_unit_param,
														const ControlUnitDriverList& _control_unit_drivers):
//call base constructor
chaos::cu::control_manager::RTAbstractControlUnit(_control_unit_id,
												  _control_unit_param,
												  _control_unit_drivers){
	
}

/*
 Base destructor
 */
own::ModbusControlUnit::~ModbusControlUnit() {
	
}


/*
 Return the default configuration
 */
void own::ModbusControlUnit::unitDefineActionAndDataset() throw(chaos::CException) {
       //set it has default
    
    //setup the dataset
	addAttributeToDataSet("current",
                          "current",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
    addAttributeToDataSet("current_sp",
                          "current Set Point",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
    addAttributeToDataSet("voltage",
                          "voltage",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
    addAttributeToDataSet("polarity",
                          "polarity",
                          DataType::TYPE_INT32,
                          DataType::Output);
	
    addAttributeToDataSet("alarms",
                          "Alarms",
                          DataType::TYPE_INT64,
                          DataType::Output);
	
	addAttributeToDataSet("status_id",
                          "status_id",
                          DataType::TYPE_INT32,
                          DataType::Output);

    addAttributeToDataSet("status",
                          "status",
                          DataType::TYPE_STRING,
                          DataType::Output,256);
	
	addAttributeToDataSet("dev_state",
                          "Bit field device state",
                          DataType::TYPE_INT64,
                          DataType::Output);
	
	addAttributeToDataSet("cmd_last_error",
                          "Last Erroro occurred",
                          DataType::TYPE_STRING,
                          DataType::Output,256);
	/*
     * JAVASCRIPT INTERFACE
     */
    addAttributeToDataSet("on",
                          "power supply is on",
                          DataType::TYPE_INT32,
                          DataType::Output);
	addAttributeToDataSet("stby",
                          "power supply is on standby",
                          DataType::TYPE_INT32,
                          DataType::Output);
    addAttributeToDataSet("alarm",
                          "power supply alarm",
                          DataType::TYPE_INT32,
                          DataType::Output);
    
    
    ///
	addAttributeToDataSet("slope_up",
                          "The gain of the noise of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);
	
	addAttributeToDataSet("slope_down",
                          "The gain of the noise of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);
	
	addAttributeToDataSet("driver_timeout",
                          "Driver timeout in milliseconds",
                          DataType::TYPE_INT32,
                          DataType::Input);
	
	addAttributeToDataSet("command_timeout",
                          "General command timeout in microseconds",
                          DataType::TYPE_INT32,
                          DataType::Input);
	
	addAttributeToDataSet("delta_setpoint",
                          "Delta of the setpoint",
                          DataType::TYPE_INT32,
                          DataType::Input);
	
	addAttributeToDataSet("setpoint_affinity",
                          "Delta of the setpoint",
                          DataType::TYPE_INT32,
                          DataType::Input);
	//define the custom share, across slow command, variable
}

void own::ModbusControlUnit::defineSharedVariable() {
	
}

// Abstract method for the initialization of the control unit
void own::ModbusControlUnit::unitInit() throw(CException) {
	SCCUAPP "unitInit";
    
    
    
	chaos::cu::driver_manager::driver::DriverAccessor * driver_accessor=AbstractControlUnit::getAccessoInstanceByIndex(0);
	if(driver_accessor==NULL){
        throw chaos::CException(1, "Cannot retrieve the requested driver", __FUNCTION__);
    }

	driver = new chaos::driver::modbus::ChaosModbusInterface(driver_accessor);
	if(driver==NULL){
		throw chaos::CException(1, "Cannot allocate driver resources", __FUNCTION__);
	}
    
   }

void own::ModbusControlUnit::unitRun() throw(CException) {
	
}

// Abstract method for the start of the control unit
void own::ModbusControlUnit::unitStart() throw(CException) {
	
}

// Abstract method for the stop of the control unit
void own::ModbusControlUnit::unitStop() throw(CException) {
	
}

// Abstract method for the deinit of the control unit
void own::ModbusControlUnit::unitDeinit() throw(CException) {
	
}