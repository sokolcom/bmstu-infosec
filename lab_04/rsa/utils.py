import os
import math
import sys
import random
 
 
def nbit_random(n):
    return random.randrange(2**(n - 1) + 1, 2**n - 1)
 

def get_lowlvl_prime(n):
    '''Generate a prime candidate divisible by first primes'''

    # Pre generated primes
    first_primes_list = [
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103,
        107, 109, 113, 127, 131, 137, 139,
        149, 151, 157, 163, 167, 173, 179,
        181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257,
        263, 269, 271, 277, 281, 283, 293,
        307, 311, 313, 317, 331, 337, 347, 349
    ]

    while True:
        pc = nbit_random(n)
 
        # Test divisibility by pre-generated primes
        for divisor in first_primes_list:
            if (pc % divisor == 0) and (divisor**2 <= pc):
                break
            else:
                return pc
 

def rabinmiller_test(mrc):
    '''Run 20 iterations of Rabin Miller Primality test'''
    max_divisions2 = 0
    ec = mrc - 1
    while ec % 2 == 0:
        ec >>= 1
        max_divisions2 += 1
    assert(2**max_divisions2 * ec == mrc - 1)
 
    def trial_composite(round_tester):
        if pow(round_tester, ec, mrc) == 1:
            return False
        for i in range(max_divisions2):
            if pow(round_tester, 2**i * ec, mrc) == mrc - 1:
                return False
        return True
 
    # Set number of trials here
    n_trials = 20
    for i in range(n_trials):
        round_tester = random.randrange(2, mrc)
        if trial_composite(round_tester):
            return False
    return True
 

def get_random_prime(nbit):
    while True:
        prime_candidate = get_lowlvl_prime(nbit)
        if not rabinmiller_test(prime_candidate):
            continue
        else:
            return prime_candidate


# if __name__ == '__main__':
#     while True:
#         n = 1024
#         prime_candidate = getLowLevelPrime(n)
#         if not isMillerRabinPassed(prime_candidate):
#             continue
#         else:
#             print(n, "bit prime is: \n", prime_candidate)
#             break

def _random(bytes_amount):
    return int.from_bytes(os.urandom(bytes_amount), sys.byteorder, signed=False)


# Cryptographically secure random number generation using os.urandom()
def safe_random(a, b):
    bytes_amount = math.ceil(b.bit_length() / 8)

    x = _random(bytes_amount)
    while (x < a) or (x > b):
        x = _random(bytes_amount)
    return x


def is_prime(number):
    if number > 1:
        for i in range(2, int(math.sqrt(number)) + 1):
            if (number % i == 0):
                return False
        return True
    return False


def _get_random_prime(a, b):
    number = safe_random(a, b)
    while not (is_prime(number)):
        number = safe_random(a, b)

    return number


def euclidean_gcd(a, b): 
    while (b != 0):
        temp = a
        a = b
        b = temp % b

    return a
    
# Расширенный алгоритм Евклида
# t: (a * t) mod n == 1
def invmod(k, modulo):
    # Extended Euclidean algorithm
    s, old_s = 0, 1
    r, old_r = modulo, k

    while r:
        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s

    x = old_s  # (x * k) % p == 1
    return x % modulo