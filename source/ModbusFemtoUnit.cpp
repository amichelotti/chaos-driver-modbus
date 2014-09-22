/*
 *	ModbusFemtoUnit
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

#include "ModbusFemtoUnit.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

using namespace chaos;
    
using namespace chaos::common::data;
using namespace chaos::common::batch_command;

using namespace chaos::cu::driver_manager::driver;

namespace own =  ::driver::modbus;

#define SCCUAPP LAPP_ << "[ModbusFemtoUnit - " << getCUID() << "] - "

PUBLISHABLE_CONTROL_UNIT_IMPLEMENTATION(own::ModbusFemtoUnit)

/*
 Construct a new CU with an identifier
 */
own::ModbusFemtoUnit::ModbusFemtoUnit(const string& _control_unit_id,
														const string& _control_unit_param,
														const ControlUnitDriverList& _control_unit_drivers):
//call base constructor
ModbusControlUnit(_control_unit_id,
												  _control_unit_param,
												  _control_unit_drivers){
    slave_id = -1;	
    regadd = -1;
}

/*
 Base destructor
 */
own::ModbusFemtoUnit::~ModbusFemtoUnit() {
	
}


/*
 Return the default configuration
 */
void own::ModbusFemtoUnit::unitDefineActionAndDataset() throw(chaos::CException) {
       //set it has default
    
    //setup the dataset
   addAttributeToDataSet("slaveID",
                          "Slave ID",
                          DataType::TYPE_INT32,
                          DataType::Input);
    
   // example 
   
     addAttributeToDataSet("U1N",
                          "Phase to Neutral Voltage1 RMS",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
   
      addAttributeToDataSet("U2N",
                          "Phase to Neutral Voltage2 RMS",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
       addAttributeToDataSet("U3N",
                          "Phase to Neutral Voltage3 RMS",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
       
       
        addAttributeToDataSet("I1",
                          "Phase current 1",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
   
      addAttributeToDataSet("I2",
                          "Phase Current 2",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
       addAttributeToDataSet("I3",
                          "Phase Current3",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
       
   addAttributeToDataSet("E1",
                          "Energy 1",
                          DataType::TYPE_DOUBLE,
                          DataType::Output);
    
    
}

void own::ModbusFemtoUnit::defineSharedVariable() {
	
}

// Abstract method for the initialization of the control unit
void own::ModbusFemtoUnit::unitInit() throw(CException) {
	SCCUAPP "unitInit";
    
    
    
	chaos::cu::driver_manager::driver::DriverAccessor * driver_accessor=AbstractControlUnit::getAccessoInstanceByIndex(0);
	if(driver_accessor==NULL){
        throw chaos::CException(1, "Cannot retrieve the requested driver", __FUNCTION__);
    }

	driver = new chaos::driver::modbus::ChaosModbusInterface(driver_accessor);
	if(driver==NULL){
		throw chaos::CException(1, "Cannot allocate driver resources", __FUNCTION__);
	}
    
        
        RangeValueInfo vi;
        getAttributeRangeValueInfo("slaveID", vi);
        if(vi.defaultValue.size()==0){
              throw chaos::CException(1, "You must define a slaveID for the CU", __FUNCTION__);
        }
        slave_id =  boost::lexical_cast<int>(vi.defaultValue);
        SCCUAPP << "CU slave:"<< slave_id;
       
        
   }

void own::ModbusFemtoUnit::unitRun() throw(CException) {
    
    CDataWrapper *acquiredData = getNewDataWrapper();
    if(!acquiredData) return;
    
    MODBUS_PUSH_FLOAT_REGISTER(U1N,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(U2N,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(U3N,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(I1,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(I2,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(I3,slave_id,driver,acquiredData);
    MODBUS_PUSH_FLOAT_REGISTER(E1,slave_id,driver,acquiredData);

    //submit acquired data
    pushDataSet(acquiredData);
	
}

// Abstract method for the start of the control unit
void own::ModbusFemtoUnit::unitStart() throw(CException) {
	
}

// Abstract method for the stop of the control unit
void own::ModbusFemtoUnit::unitStop() throw(CException) {
	
}

// Abstract method for the deinit of the control unit
void own::ModbusFemtoUnit::unitDeinit() throw(CException) {
	
}