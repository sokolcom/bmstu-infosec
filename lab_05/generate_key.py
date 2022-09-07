from Crypto.PublicKey import ECC


if __name__ == "__main__":
    key = ECC.generate(curve="secp256r1")

    with open("keys/pubkey.pem","wt") as fpub,  open("keys/privatekey.pem","wt") as fpriv:
        fpub.write(key.public_key().export_key(format="PEM"))
        fpriv.write(key.export_key(format="PEM"))
