from random import shuffle
import sys


class BaseComponent:
    _size = 256

    def __init__(self, from_config=False, fstream=None):
        if not from_config:
            self._commutations = [i for i in range(BaseComponent._size)]
            shuffle(self._commutations)
        else:
            self._commutations = [int.from_bytes(fstream.read(1), byteorder=sys.byteorder) for _ in range(BaseComponent._size)]

    def save_configuration(self, fstream=None):
        list(map(lambda x: fstream.write(x.to_bytes(1, byteorder=sys.byteorder)), self._commutations))
    
    def get_value(self, input_value):
        raise NotImplementedError