# se conecta al jtag y manipula los regs de control del cpu 

import cmd, sys

class Debugger(cmd.Cmd):
    intro = 'Welcome to the SISA debugger.\n'
    prompt = '(sisa-dbg) '
    file = None

    # ----- basic turtle commands -----
    def do_dummy(self, arg):
        'Dummy command'
        print(arg.split())
    
    def precmd(self, line):
        line = line.lower()
        if self.file and 'playback' not in line:
            print(line, file=self.file)
        return line
    def close(self):
        if self.file:
            self.file.close()
            self.file = None

if __name__ == '__main__':
    Debugger().cmdloop()