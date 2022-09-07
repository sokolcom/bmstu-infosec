from Crypto.Hash import SHA256
from Crypto.PublicKey import ECC
from Crypto.Signature import DSS


if __name__ == "__main__":
    content = None
    with open("document.txt", "rb") as fin:
        content = fin.read()

    key = None
    with open('keys/pubkey.pem', "rt") as fin:
        key = ECC.import_key(fin.read())

    hashed = SHA256.new(content)
    verifier = DSS.new(key, 'fips-186-3')
    signature = None
    with open("signature.txt", "rb") as fin:
        signature = fin.read()


    try:
        verifier.verify(hashed, signature)
        print("VERIFICATION SUCCEEDED!")
    except ValueError:
        print("VERIFICATION FAILED!")
