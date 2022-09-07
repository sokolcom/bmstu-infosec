from .base_component import BaseComponent


class Reflector(BaseComponent):
    def get_value(self, input_value):
        idx = 0
        while (input_value != self._commutations[idx]) and (idx < len(self._commutations)):
            idx += 1

        # Pair = ([0], [1]), ([2], [3]), .... ([even], [odd]), even = 2n, odd = 2n + 1
        return_value = self._commutations[idx - 1] if (idx % 2) else self._commutations[idx + 1]
        return return_value
