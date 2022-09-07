import sys 

from .rotor import Rotor
from .reflector import Reflector



class Enigma:
    def __init__(self, n_rotors=3, from_config=False, filename="enigma_config.txt"):
        if not from_config:
            self.reflector = Reflector()
            self.rotors = [Rotor() for _ in range(n_rotors)]
        else:
            with open(filename, "rb") as fin:
                n = int.from_bytes(fin.read(1), byteorder=sys.byteorder)
                self.reflector = Reflector(from_config=True, fstream=fin)
                self.rotors = [Rotor(from_config=True, fstream=fin) for _ in range(n)]

    def save_configuration(self, filename="enigma_config.txt"):
        with open(filename, "wb") as fout:
            fout.write(len(self.rotors).to_bytes(1, byteorder=sys.byteorder))
            self.reflector.save_configuration(fout)
            list(map(lambda x: x.save_configuration(fout), self.rotors))

    def process_byte(self, value):
        for rotor in self.rotors:
            value = rotor.get_value(value)

        value = self.reflector.get_value(value)

        for rotor in reversed(self.rotors):
            value = rotor.get_flipped_value(value)

        for rotor in self.rotors:
            if not rotor.rotate():
                break
        
        return value

    def process_file(self, filename_dst, filename_src):
        with open(filename_src, "rb") as fsrc, open(filename_dst, "wb") as fdst:
            while True:
                current_byte = fsrc.read(1)
                if not current_byte:
                    break
                current = int.from_bytes(current_byte, byteorder=sys.byteorder)
                fdst.write(self.process_byte(current).to_bytes(length=1, byteorder=sys.byteorder))
