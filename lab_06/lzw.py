import sys
import math
from io import StringIO

import argparse


def lzw_compress(uncompressed):
    """Compress a bytes to a list of output symbols."""
 
    # Build the dictionary.
    dict_size = 256
    dictionary = { chr(i): i for i in range(dict_size) }
 
    w = u""
    result = []
    for c in uncompressed:
        # print(type(w), type(c))
        wc = w + chr(c)
        if wc in dictionary:
            w = wc
        else:
            result.append(dictionary[w])
            # Add wc to the dictionary.
            dictionary[wc] = dict_size
            dict_size += 1
            w = chr(c)
 
    # Output the code for w.
    if w:
        result.append(dictionary[w])
    return result
 
 
def lzw_decompress(compressed):
    """Decompress a list of output ks to a string."""

    dict_size = 256
    dictionary = { i: chr(i) for i in range(dict_size) }
 
    result = StringIO()
    w = chr(compressed.pop(0))
    result.write(w)
    for k in compressed:
        if k in dictionary:
            entry = dictionary[k]
        elif k == dict_size:
            entry = w + w[0]
        else:
            raise ValueError('Bad compressed k: %s' % k)
        result.write(entry)
 
        # Add w+entry[0] to the dictionary.
        dictionary[dict_size] = w + entry[0]
        dict_size += 1
 
        w = entry
    return result.getvalue()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--src', type=str, default="data/document.txt")
    parser.add_argument('--cpr', type=str, default="compressed")
    parser.add_argument('-dcpr', type=str, default="decompressed")
    args = parser.parse_args()

    with open(args.src, "rb") as fin:
        content = fin.read()

    compressed = lzw_compress(content)
    max_size = math.ceil(max(compressed, key=lambda x: x.bit_length()).bit_length() / 8)
    compressed_tobytes = list(map(lambda x: x.to_bytes(max_size, sys.byteorder), compressed))
    with open(args.cpr, "wb") as fout:
        fout.write(b"".join(compressed_tobytes))

    decompressed = lzw_decompress(compressed)
    with open(args.dcpr, "wb") as fout:
        fout.write(decompressed.encode())

    assert(content == decompressed.encode())


if __name__ == "__main__":
    main()