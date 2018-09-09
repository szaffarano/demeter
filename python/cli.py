#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import argparse

from utils import Console, DemeterClient
from pymodbus.pdu import ExceptionResponse
from pymodbus.register_write_message import WriteMultipleRegistersResponse
from pymodbus.register_read_message import ReadRegistersResponseBase
from pymodbus.bit_read_message import ReadBitsResponseBase
from pymodbus.bit_write_message import WriteMultipleCoilsResponse

LOG_FILENAME = '/tmp/modbus-cli.log'
HISTORY_FILENAME = '/tmp/.modbus_history'

MODBUS_CLIENT_ID = 0x03

# Configuración del log
logging.basicConfig(
    filename=LOG_FILENAME,
    level=logging.DEBUG,
)

# Opciones para el autocompletado
autocompletions = {
    'read_coils': [],
    'read_discrete_inputs': [],
    'read_holding_registers': [],
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
    'temperatura': [],
    'humedad': [],
    'luz': [],
}


def __configure_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("-d", "--device",
                        help="Path to the bluetooth device to use",
                        default="/dev/rfcomm1")

    parser.add_argument("-m", "--method",
                        help="Connection method",
                        default="rtu")

    parser.add_argument("-s", "--stop",
                        help="Stop bits",
                        default=2)

    parser.add_argument("-S", "--size",
                        help="Byte size",
                        default=8)

    parser.add_argument("-p", "--parity",
                        help="If it use parity",
                        default="N")

    parser.add_argument("-b", "--bauds",
                        help="Baud rate",
                        default=9600)

    parser.add_argument("-t", "--timeout",
                        help="Timeout",
                        default=1)
    return parser


if __name__ == '__main__':
    parser = __configure_args()
    args = parser.parse_args()

    # Configuración de Modbus
    modbus_config = {
        'method': args.method,
        'port': args.device,
        'stopbits': int(args.stop),
        'bytesize': int(args.size),
        'parity': args.parity,
        'baudrate': int(args.bauds),
        'timeout': int(args.timeout)
    }

    demeter = DemeterClient(MODBUS_CLIENT_ID, modbus_config)
    console = Console(autocompletions, HISTORY_FILENAME)

    while True:
        cli = console.input('modbus> ')

        command = cli[0]
        arguments = cli[1:] if len(cli) > 1 else []

        if not demeter.is_valid(command):
            print("%s: comando desconocido." % command)
            continue

        response = demeter.execute(command, arguments)
        if response is None:
            print("respuesta nula")
        elif isinstance(response, ExceptionResponse):
            print("Error en la ejecución: %s" % response)
        else:
            if isinstance(response, WriteMultipleRegistersResponse):
                print("Se escribieron %s registros" % response.count)
            elif isinstance(response, ReadRegistersResponseBase):
                print("Se leyeron %s registros: %s" %
                      (len(response.registers), response.registers))
            elif isinstance(response, ReadBitsResponseBase):
                print("Se leyeron %s coils: %s" %
                      (len(response.bits), response.bits))
            elif isinstance(response, WriteMultipleCoilsResponse):
                print("Se escribieron %s coils" % response.count)
            else:
                print(response)
