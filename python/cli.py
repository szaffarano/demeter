#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
    'write_registers': []
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
            print("Holding registers: %s" % response.registers)
