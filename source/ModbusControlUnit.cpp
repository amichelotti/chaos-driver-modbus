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

namespace own =  chaos::driver::modbus;

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
	slave_id = -1;
	regadd = -1;
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
	addAttributeToDataSet("slaveID",
						  "Slave ID",
						  DataType::TYPE_INT32,
						  DataType::Input);
	
	// example
	
	addAttributeToDataSet("Register1",
						  "register1",
						  DataType::TYPE_INT32,
						  DataType::Input);
	
	
	addAttributeToDataSet("Out1",
						  "Register out",
						  DataType::TYPE_DOUBLE,
						  DataType::Output);
	
	
}

void own::ModbusControlUnit::unitInputAttributeChangedHandler() throw(CException) {
	
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
	
	
	RangeValueInfo vi;
	getAttributeRangeValueInfo("slaveID", vi);
	if(vi.defaultValue.size()==0){
		throw chaos::CException(1, "You must define a slaveID for the CU", __FUNCTION__);
	}
	slave_id = boost::lexical_cast<int>(vi.defaultValue);
	getAttributeRangeValueInfo("Register1",vi);
	
	if(vi.defaultValue.size()==0){
		throw chaos::CException(1, "You must define a register value  for the CU", __FUNCTION__);
	}
	regadd = boost::lexical_cast<int>(vi.defaultValue);
	
	SCCUAPP << "CU slave:"<< slave_id <<" address:"<<regadd;
	
	o_out_1 = getAttributeCache()->getRWPtr<double>(chaos::common::data::cache::AttributeValueSharedCache::SVD_OUTPUT, "sinWave");
}

void own::ModbusControlUnit::unitRun() throw(CException) {
	uint32_t data;
	float* dataf=(float*)&data;
	//assume 32 bit floating point registers
	driver->read_input_registers(regadd,2,(uint16_t*)&data,slave_id);
	
	*o_out_1 = *dataf;
	
	//flag output attribute cache as changed
	getAttributeCache()->setOutputDomainAsChanged();
	
	//put the messageID for test the lost of package
	//acquiredData->addDoubleValue("Out1", *dataf);
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