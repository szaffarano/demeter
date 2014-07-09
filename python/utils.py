# -*- coding: utf-8 -*-

from pymodbus.client.sync import ModbusSerialClient

import readline
import atexit
import os
from pymodbus.register_read_message import ReadRegistersResponseBase

class DemeterClient(object):
    def __init__(self, unit, modbus_config):
        self.client = ModbusSerialClient(**modbus_config)
        self.unit = unit
        self.valid_commands = {
            'read_holding_registers': self.read_holding_registers,
            'write_registers': self.write_holding_registers,
            'read_input_registers': self.read_input_registers,
            'read_coils': self.read_coils,
            'write_coils': self.write_coils,
            'get_datetime': self.get_datetime,
            'set_datetime': self.set_datetime,
            'get_loginterval': self.get_loginterval,
            'set_loginterval': self.set_loginterval,
            'read_event': self.read_event,
            'write_event': self.write_event,
            'enable_event': self.enable_event,
            'disable_event': self.disable_event,
            'read_events': self.read_events,
            'disable_event': self.disable_event,
            'enable_event': self.enable_event,
            'disable_relay': self.disable_relay,
            'enable_relay': self.enable_relay,
            'temperatura': self.get_temperature,
            'humedad': self.get_humidity,
            'luz': self.get_light,
        }

    def is_valid(self, command):
        return command in self.valid_commands

    def read_holding_registers(self, arguments):
        if len(arguments) != 2:
            return "Modo de uso: read_holding_registers <start-address> <registers>"

        for i in range(0, len(arguments)):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]

        return self.client.read_holding_registers(address=values[0], count=values[1], unit=self.unit)

    def write_holding_registers(self, arguments):
        if len(arguments) < 2:
            return "Modo de uso: write_holding_registers <start-address> <register-1> <register-2> ... <register-n>"

        for i in range(0, len(arguments)):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]
        
        return self.client.write_registers(values[0], values[1:], unit=self.unit)

    def get_temperature(self, arguments):
        value = self.client.read_input_registers(address=0, count=1, unit=self.unit)
        if isinstance(value, ReadRegistersResponseBase):
            value = "%d,%d °C" % (value.registers[0]/10, value.registers[0]%10)
        return value

    def get_humidity(self, arguments):
        value = self.client.read_input_registers(address=1, count=1, unit=self.unit)
        if isinstance(value, ReadRegistersResponseBase):
            value = "%d,%d %%" % (value.registers[0]/10, value.registers[0]%10)
        return value

    def get_light(self, arguments):
        value = self.client.read_input_registers(address=2, count=1, unit=self.unit)
        if isinstance(value, ReadRegistersResponseBase):
            value = value.registers[0]
        return value

    def read_input_registers(self, arguments):
        if len(arguments) != 2:
            return "Modo de uso: read_input_registers <start-address> <registers>"

        for i in range(0, len(arguments)):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]
        
        return self.client.read_input_registers(address=values[0], count=values[1], unit=self.unit)

    def read_coils(self, arguments):
        if len(arguments) != 2:
            return "Modo de uso: read_coils <start-address> <coils>"

        for i in range(0, len(arguments)):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]
        
        return self.client.read_coils(address=values[0], count=values[1], unit=self.unit)

    def write_coils(self, arguments):
        if len(arguments) < 2:
            return "Modo de uso: write_coils <start-address> <bit-1> <bit-2> ... <bit-n>"

        for i in range(0, len(arguments)):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]
        
        return self.client.write_coils(address=values[0], values=values[1:], unit=self.unit)

    def get_datetime(self, arguments):
        response = self.client.read_holding_registers(address=1, count=6, unit=self.unit)
        if isinstance(response, ReadRegistersResponseBase):
            r = response.registers
            return "%s/%02d/%02d %02d:%02d:%02d" % (r[0], r[1], r[2], r[3], r[4], r[5])
        return response

    def set_datetime(self, arguments):
        # yyy:mm:dd hh:mm:ss
        if len(arguments) != 6:
            return "Modo de uso: set_datetime <yyyy> <mm> <dd> <hh> <mm> <ss>"

        for i in range(0, 6):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        values = [int(x) for x in arguments]

        return self.client.write_registers(address=1, values=values, unit=self.unit)

    def get_loginterval(self, arguments):
        response = self.client.read_holding_registers(address=0, count=1, unit=self.unit)
        if isinstance(response, ReadRegistersResponseBase):
            return response.registers[0]
        return response

    def set_loginterval(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: set_loginterval <segunos>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        return self.client.write_registers(address=0, values=[int(arguments[0])], unit=self.unit)

    def read_event(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: read_event <numero de evento>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        number = int(arguments[0])
        response = self.client.read_holding_registers(address=number * 6 + 7, count=6, unit=self.unit)
        if isinstance(response, ReadRegistersResponseBase):
            return self.__print_event(number, response.registers)
        return response

    def write_event(self, arguments):
        # nro de evento + 6 campos
        if len(arguments) != (1 + 6):
            return "Modo de uso: write_event <numero de evento> <hh>:<mm>:<ss> <duracion> <relay> <1|0>"

        for i in range(0, 7):
            if not self.__is_number(arguments[i]):
                return "%s: se esperaba un entero" % arguments[i]

        number = int(arguments[0])

        values = [int(x) for x in arguments[1:]]
        if values[5] not in [0,1]:
            return "%s: el valor debe ser cero o uno" % values[5]

        return self.client.write_registers(address=number * 6 + 7, values=values, unit=self.unit)

    def disable_event(self, arguments):
        # nro de evento
        if len(arguments) != 1:
            return "Modo de uso: disable_event <numero de evento>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[i]

        number = int(arguments[0])

        return self.client.write_registers(address=number * 6 + 5, values=[0], unit=self.unit)

    def enable_event(self, arguments):
        # nro de evento
        if len(arguments) != 1:
            return "Modo de uso: disable_event <numero de evento>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[i]

        number = int(arguments[0])

        return self.client.write_registers(address=number * 6 + 5, values=[0], unit=self.unit)

    def read_events(self, arguments):
        response = self.client.read_holding_registers(address=7, count=6 * 10, unit=self.unit)
        if isinstance(response, ReadRegistersResponseBase):
            str = ""
            for i in range(0, 10):
                str += self.__print_event(i, response.registers[i * 6:(i + 1) * 6])
                str += "\n"
            return str
        return response

    def disable_event(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: disable_event <numero de evento>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        number = int(arguments[0])

        return self.client.write_registers(address=number * 6 + 7 + 5, values=[0], unit=self.unit)

    def enable_event(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: enable_event <numero de evento>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        number = int(arguments[0])

        return self.client.write_registers(address=number * 6 + 7 + 5, values=[1], unit=self.unit)

    def disable_relay(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: disable_relay <numero de relay>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        number = int(arguments[0])
        if number not in [0, 1]:
            return "%d: solo hay dos relays" % number
        
        return self.client.write_coils(address=number, values=[0], unit=self.unit)

    def enable_relay(self, arguments):
        if len(arguments) != 1:
            return "Modo de uso: disable_relay <numero de relay>"

        if not self.__is_number(arguments[0]):
            return "%s: se esperaba un entero" % arguments[0]

        number = int(arguments[0])
        if number not in [0, 1]:
            return "%d: solo hay dos relays" % number

        return self.client.write_coils(address=number, values=[1], unit=self.unit)

    def dummy(self, arguments):
        return None

    def execute(self, command, arguments):
        return self.valid_commands.get(command, self.dummy)(arguments)

    def __print_event(self, number, data):
        return "Evento #%d (%s) => Inicia a las %02d:%02d:%02d hs., dura: %d segs. y ejecuta en relay #%d" % (
                                    number, ("habilitado" if data[5] != 0 else "deshabilitado"), data[0],
                                    data[1], data[2], data[3], data[4])
    def __is_number(self, str):
        try:
            int(str)
        except ValueError:
            return False
        return True

class Console(object):
    def __init__(self, autocompletions, history_file):
        self.load_history_file(history_file)
        atexit.register(self.save_history_file, history_file)

        readline.set_completer(BufferAwareCompleter(autocompletions).complete)
        readline.parse_and_bind('tab: complete')

    def input(self, prompt):
        line = ''
        while True:
            try:
                line = raw_input(prompt)
                if line != '':
                    return line.split()
            except EOFError:
                import sys
                print("bye")
                sys.exit(0)
            except KeyboardInterrupt:
                print ""

    def load_history_file(self, filename):
        if os.path.exists(filename):
            readline.read_history_file(filename)

    def save_history_file(self, filename):
        readline.write_history_file(filename)


class BufferAwareCompleter(object):

    def __init__(self, options):
        self.options = options
        self.current_candidates = []
        return

    def complete(self, text, state):
        response = None
        if state == 0:
            # This is the first time for this text, so build a match list.
            origline = readline.get_line_buffer()
            begin = readline.get_begidx()
            end = readline.get_endidx()
            being_completed = origline[begin:end]
            words = origline.split()

            if not words:
                self.current_candidates = sorted(self.options.keys())
            else:
                try:
                    if begin == 0:
                        # first word
                        candidates = self.options.keys()
                    else:
                        # later word
                        first = words[0]
                        candidates = self.options[first]

                    if being_completed:
                        # match options with portion of input
                        # being completed
                        self.current_candidates = [ w for w in candidates
                                                    if w.startswith(being_completed) ]
                    else:
                        # matching empty string so use all candidates
                        self.current_candidates = candidates

                except (KeyError, IndexError), err:
                    self.current_candidates = []

        try:
            response = self.current_candidates[state]
        except IndexError:
            response = None
        return response
