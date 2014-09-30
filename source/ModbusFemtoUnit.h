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
#ifndef __PowerSupply__ModbusFemtoUnit__
#define __PowerSupply__ModbusFemtoUnit__

#include <chaos/cu_toolkit/ControlManager/RTAbstractControlUnit.h>
#include <driver/modbus/core/ChaosModbusInterface.h>
#include "ModbusControlUnit.h"

using namespace boost::msm::front::euml;
using namespace boost::msm::front;
//namespace mpl = boost::mpl;
#define MAX_REGISTERS 16
namespace driver {
	namespace modbus {
		
		class ModbusFemtoUnit : public ModbusControlUnit {
			PUBLISHABLE_CONTROL_UNIT_INTERFACE(ModbusFemtoUnit)

			

		protected:
                    
                    static const unsigned U1N_REG=221;
                    static const unsigned U2N_REG=223;
                    static const unsigned U3N_REG=225;
                    static const unsigned I1_REG=233;
                    static const unsigned I2_REG=235;
                    static const unsigned I3_REG=237;
                    
                    static const unsigned PFS_REG=283;
                    static const unsigned PS_REG=277;
                     
                      
                     // DA SOSTITUIRE
                     static const unsigned E1_REG=239;
                     

			/*
			 Define the Control Unit Dataset and Actions
			 */
			void unitDefineActionAndDataset()throw(chaos::CException);
			
			void defineSharedVariable();

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
			static boost::mutex slock;
		public:
			/*
			 Construct a new CU with an identifier
			 */
			ModbusFemtoUnit(const string& _control_unit_id,
									 const string& _control_unit_param,
									 const ControlUnitDriverList& _control_unit_drivers);
			
			/*
			 Base destructor
			 */
			~ModbusFemtoUnit();
		};
	}
}

#endif /* defined(__PowerSupply__ModbusControlUnit__) */
