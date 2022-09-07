import sys
from Crypto.Cipher import AES

import ecc.config as cfg
import ecc.ec_math as ec_math


cipherpubkeys = []


def _generate_keypair():
    private_key = cfg.safe_random(1, cfg.SUBGROUP_ORDER)
    public_key = ec_math.scalar_mult(private_key, cfg.BASE_POINT)
    return (private_key, public_key)


def ecc_register():
    priv_alice, pub_alice = _generate_keypair()
    alice = {
        "private": priv_alice,
        "public": pub_alice,
    }
    return alice


def derive_encrypt_key(alice):
    k = cfg.safe_random(1, cfg.SUBGROUP_ORDER)
    cpubx, cpuby = ec_math.scalar_mult(k, cfg.BASE_POINT)
    symkey, _ = ec_math.scalar_mult(k, alice["public"])
    return {
        "symm_key": symkey,
        "cipherpubkey": (cpubx, cpuby),
    }


def decrypt_derive_key(cipherpubkey, alice):
    symkey, _ = ec_math.scalar_mult(alice["private"], cipherpubkey)
    return {
        "symm_key": symkey,
    }


def encrypt_bytes(current_byte, alice):
    obj = derive_encrypt_key(alice)
    cipherpubkeys.append(obj["cipherpubkey"])
    aes_obj = AES.new(obj["symm_key"].to_bytes(32, sys.byteorder), AES.MODE_ECB)
    return aes_obj.encrypt(current_byte.to_bytes(16, sys.byteorder))
    

def decrypt_bytes(current_byte, alice, idx):
    obj = decrypt_derive_key(cipherpubkeys[idx], alice)
    aes_obj = AES.new(obj["symm_key"].to_bytes(32, sys.byteorder), AES.MODE_ECB)
    return aes_obj.decrypt(current_byte.to_bytes(16, sys.byteorder))
    

def ecc_encrypt(alice, filename_dst, filename_src):
    with open(filename_src, "rb") as fsrc, open(filename_dst, "wb") as fdst:
        while True:
            current_bytes = fsrc.read(16)
            if not current_bytes:
                break
            current = int.from_bytes(current_bytes, byteorder=sys.byteorder)
            cipher = encrypt_bytes(current, alice)
            fdst.write(cipher)


def ecc_decrypt(alice, filename_dst, filename_src):
    with open(filename_src, "rb") as fsrc, open(filename_dst, "wb") as fdst:
        idx = 0
        # print(len(cipherpubkeys))
        while True:
            current_bytes = fsrc.read(16)
            if not current_bytes:
                break
            current = int.from_bytes(current_bytes, byteorder=sys.byteorder)
            cipher = decrypt_bytes(current, alice, idx)
            fdst.write(cipher)
            idx += 1
