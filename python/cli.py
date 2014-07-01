#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pymodbus.register_write_message import WriteMultipleRegistersResponse
from pymodbus.register_read_message import ReadHoldingRegistersResponse,\
    ReadRegistersResponseBase
from pymodbus.bit_read_message import ReadBitsResponseBase
from pymodbus.bit_write_message import WriteMultipleCoilsResponse

LOG_FILENAME = '/tmp/modbus-cli.log'
HISTORY_FILENAME = '/tmp/.modbus_history'

MODBUS_CLIENT_ID = 0x03

import logging
from utils import Console, DemeterClient
from pymodbus.pdu import ExceptionResponse

# Configuración del log
logging.basicConfig(
    filename=LOG_FILENAME,
    level=logging.DEBUG,
)

# Configuración de Modbus
modbus_config = {
     'method': 'rtu',
     'port': '/dev/rfcomm0',
     'stopbits': 2,
     'bytesize': 8,
     'parity': 'N',
     'baudrate': 9600,
     'timeout': 1
}

# Opciones para el autocompletado
autocompletions = {
    'read_coils':[],
    'read_discrete_inputs':[],
    'read_holding_registers':[],
    'read_input_registers': [],
    'write_coils': [],
    'write_registers': [],
    
    'get_datetime': [],
    'set_datetime': [],
    'get_loginterval': [],
    'set_loginterval': [],
    'read_event': [],
    'write_event': [],
    'read_events': [],
    'disable_event': [],
    'enable_event': [],
    'disable_relay': [],
    'enable_relay': [],
}

if __name__ == '__main__':
    demeter = DemeterClient(MODBUS_CLIENT_ID, modbus_config)
    console = Console(autocompletions, HISTORY_FILENAME)

    while True:
        cli = console.input('modbus> ')

        command = cli[0]
        arguments = cli[1:] if len(cli) > 1 else []

        if not demeter.is_valid(command):
            print "%s: comando desconocido." % command
            continue

        response = demeter.execute(command, arguments)
        if response is None:
            print "respuesta nula"
        elif isinstance(response, ExceptionResponse):
            print("Error en la ejecución: %s" % response)
        else:
            if isinstance(response, WriteMultipleRegistersResponse):
                print("Se escribieron %s registros" % response.count)
            elif isinstance(response, ReadRegistersResponseBase):
                print("Se leyeron %s registros: %s" % (len(response.registers), response.registers))
            elif isinstance(response, ReadBitsResponseBase):
                print("Se leyeron %s coils: %s" % (len(response.bits), response.bits))
            elif isinstance(response, WriteMultipleCoilsResponse):
                print("Se escribieron %s coils" % response.count)
            else:
                print response
