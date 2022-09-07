from .base_component import BaseComponent


class Rotor(BaseComponent):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._n_rotations = 0

    def rotate(self):
        temp_value = self._commutations[-1]
        temp_slice = self._commutations[:-1]
        self._commutations = [temp_value, ] + temp_slice

        self._n_rotations =  (self._n_rotations + 1) % self._size
        return self._n_rotations == 0

    # Output byte based on input one
    def get_value(self, input_value):
        return self._commutations[input_value]
    
    # Input byte based on output one
    def get_flipped_value(self, input_value):
        idx = 0
        while (input_value != self._commutations[idx]) and (idx < len(self._commutations)):
            idx += 1
        
        return idx