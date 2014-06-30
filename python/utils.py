# -*- coding: utf-8 -*-

from pymodbus.client.sync import ModbusSerialClient

import readline
import atexit
import os

class DemeterClient(object):
    def __init__(self, unit, modbus_config):
        self.client = ModbusSerialClient(**modbus_config)
        self.unit = unit
        self.valid_commands = {
            'read_holding_registers': self.read_holding_registers,
            'write_registers': self.write_holding_registers,
            'read_input_registers': self.read_input_registers,
            'read_coils': self.read_coils,
            'write_coils': self.write_coils
        }

    def is_valid(self, command):
        return command in self.valid_commands

    def read_holding_registers(self, arguments):
        return self.client.read_holding_registers(address=int(arguments[0]), count=int(arguments[1]), unit=self.unit)

    def write_holding_registers(self, arguments):
        print "write_registers(%s, %s)" % (arguments[0], [int(x) for x in arguments[1:]])
        return self.client.write_registers(int(arguments[0]), [int(x) for x in arguments[1:]], unit=self.unit)

    def read_input_registers(self, arguments):
        return self.client.read_input_registers(address=int(arguments[0]), count=int(arguments[1]), unit=self.unit)

    def read_coils(self, arguments):
        return self.client.read_coils(address=int(arguments[0]), count=int(arguments[1]), unit=self.unit)

    def write_coils(self, arguments):
        return self.client.write_coils(address=int(arguments[0]), values=[int(x) for x in arguments[1:]], unit=self.unit)

    def dummy(self, arguments):
        print "funcion invalida"
        return None

    def execute(self, command, arguments):
        return self.valid_commands.get(command, self.dummy)(arguments)

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
