#!/bin/sh
if [ -z "$1" ]; then
echo "usage is $0 <metadataserver>"
exit 1
fi


echo "Using metadata server $1 log into modbusControlUnit-`hostname`.log"

modbusControlUnit --metadata-server localhost:5000 --log-on-console --unit_server_alias MODBUS-`hostname` --unit_server_enable true >& modbusControlUnit-`hostname`.log &



