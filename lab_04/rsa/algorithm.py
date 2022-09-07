from os import read, write
import sys

from rsa.utils import safe_random, get_random_prime, euclidean_gcd, invmod



# GCD(e, phi) == 1
def get_openexp(phi):
    gcd = 0
    number = -1

    while (gcd != 1):
        number = safe_random(2, phi)
        gcd = euclidean_gcd(phi, number)

    return number


# (e * d) mod(phi) = 1
def get_secretexp(e, phi):
    return invmod(e, phi)


def rsa_register():
    p = get_random_prime(1024)  # 1, (2 ** 1024 - 1)
    q = get_random_prime(1024)  # 1, (2 ** 1024 - 1)
    n = p * q  # bit_length(n) = 2048
    euler_phi = (p - 1) * (q - 1)

    e = get_openexp(euler_phi)
    d  = get_secretexp(e, euler_phi)
    return {
        "private": (d, n),
        "public": (e, n),
    }


def _encrypt(alice, byte):
    return pow(byte, alice["public"][0], alice["public"][1])


def _decrypt(alice, byte):
    return pow(byte, alice["private"][0], alice["private"][1])


def _process_file(alice, filename_dst, filename_src, func):
    if func == _encrypt:
        read_length = 1
        write_length = 256
    else:
        read_length = 256
        write_length = 1

    with open(filename_src, "rb") as fsrc, open(filename_dst, "wb") as fdst:
        while True:
            current_byte = fsrc.read(read_length)
            if not current_byte:
                break
            current = int.from_bytes(current_byte, byteorder=sys.byteorder)
            cipher = func(alice, current)
            fdst.write(cipher.to_bytes(length=write_length, byteorder=sys.byteorder))


def rsa_encrypt(alice, filename_dst, filename_src):
    return _process_file(alice, filename_dst, filename_src, _encrypt)


def rsa_decrypt(alice, filename_dst, filename_src):
    return _process_file(alice, filename_dst, filename_src, _decrypt)
    