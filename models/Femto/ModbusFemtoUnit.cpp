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
using namespace chaos::common::data::cache;
using namespace chaos::common::batch_command;

using namespace chaos::cu::driver_manager::driver;


#define SCCUAPP LAPP_ << "[ModbusFemtoUnit - " << getCUID() << "] - "

PUBLISHABLE_CONTROL_UNIT_IMPLEMENTATION(chaos::driver::modbus::ModbusFemtoUnit)

/*
 Construct a new CU with an identifier
 */
ChaosMutex chaos::driver::modbus::ModbusFemtoUnit::slock;

chaos::driver::modbus::ModbusFemtoUnit::ModbusFemtoUnit(const string& _control_unit_id,
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
chaos::driver::modbus::ModbusFemtoUnit::~ModbusFemtoUnit() {
	
}


/*
 Return the default configuration
 */
void chaos::driver::modbus::ModbusFemtoUnit::unitDefineActionAndDataset() throw(chaos::CException) {
       //set it has default
   setDefaultScheduleDelay(2000);

    //setup the dataset
   addAttributeToDataSet("slaveID",
                          "Slave ID",
                          DataType::TYPE_INT32,
                          DataType::Input);
    
   // output
   
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
   
   addAttributeToDataSet("PFS",
                          "Total Power Factor",
                          DataType::TYPE_DOUBLE, 
                          DataType::Output);
   
   addAttributeToDataSet("PS",
                          "Total Power Active",
                           DataType::TYPE_DOUBLE,
                           DataType::Output);
   
   addAttributeToDataSet("TIMES",
                          "Life Time",
                           DataType::TYPE_INT32,
                           DataType::Output);
    
    addAttributeToDataSet("EAS",
                          "Total imported active",
                           DataType::TYPE_INT32,
                           DataType::Output);
}

void chaos::driver::modbus::ModbusFemtoUnit::defineSharedVariable() {
	
}

// Abstract method for the initialization of the control unit
void chaos::driver::modbus::ModbusFemtoUnit::unitInit() throw(CException) {
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

void chaos::driver::modbus::ModbusFemtoUnit::unitRun() throw(CException) {
    ChaosLockGuard l(slock);

    driver->connect();
	
	double *o_U1N = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "U1N");
	
	double *o_U2N = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "U2N");
	
	double *o_U3N = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "U3N");
	
	
	double *o_I1 = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "I1");
	
	double *o_I2 = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "I2");
	
	double *o_I3 = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "I3");
	
	double *o_E1 = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "E1");
	
	double *o_PFS = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "PFS");
	
	double *o_PS = getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "PS");
	
	int32_t *o_TIMES = getAttributeCache()->getRWPtr<int32_t>(DOMAIN_OUTPUT, "TIMES");
	
	int32_t *o_EAS = getAttributeCache()->getRWPtr<int32_t>(DOMAIN_OUTPUT, "EAS");
	
    MODBUS_PUSH_FLOAT_REGISTER(U1N,slave_id,driver,1,1.0,o_U1N);
    MODBUS_PUSH_FLOAT_REGISTER(U2N,slave_id,driver,1,1.0,o_U2N);
    MODBUS_PUSH_FLOAT_REGISTER(U3N,slave_id,driver,1,1.0,o_U3N);
    MODBUS_PUSH_FLOAT_REGISTER(I1,slave_id,driver,1,1.0,o_I1);
    MODBUS_PUSH_FLOAT_REGISTER(I2,slave_id,driver,1,1.0,o_I2);
    MODBUS_PUSH_FLOAT_REGISTER(I3,slave_id,driver,1,1.0,o_I3);
    MODBUS_PUSH_FLOAT_REGISTER(E1,slave_id,driver,1,1.0,o_E1);
    MODBUS_PUSH_FLOAT_REGISTER(PFS,slave_id,driver,1,1.0,o_PFS);
    MODBUS_PUSH_FLOAT_REGISTER(PS,slave_id,driver,1,1.0,o_PS);
	
	MODBUS_PUSH_UNSIGNED_REGISTER(TIMES,slave_id,driver,1,1.0,o_TIMES);
	MODBUS_PUSH_UNSIGNED_REGISTER(EAS,slave_id,driver,1,0.01,o_EAS);
	
	//notify change on cache
	getAttributeCache()->setOutputDomainAsChanged();
    //submit acquired data
    driver->close();


}

// Abstract method for the start of the control unit
void chaos::driver::modbus::ModbusFemtoUnit::unitStart() throw(CException) {
	
}

// Abstract method for the stop of the control unit
void chaos::driver::modbus::ModbusFemtoUnit::unitStop() throw(CException) {
	
}

// Abstract method for the deinit of the control unit
void chaos::driver::modbus::ModbusFemtoUnit::unitDeinit() throw(CException) {
	
}
