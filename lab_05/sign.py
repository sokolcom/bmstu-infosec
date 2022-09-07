from Crypto.Hash import SHA256
from Crypto.PublicKey import ECC
from Crypto.Signature import DSS


if __name__ == "__main__":
    content = None
    with open("document.txt", "rb") as fin:
        content = fin.read()

    key = None
    with open('keys/privatekey.pem', "rt") as fin:
        key = ECC.import_key(fin.read())

    hashed = SHA256.new(content)
    signer = DSS.new(key, 'fips-186-3')
    signature = signer.sign(hashed)

    with open("signature.txt", "wb") as fout:
        fout.write(signature)
