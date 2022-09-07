import argparse
from enigma import Enigma


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--src', type=str)
    parser.add_argument('--dst', type=str, default="cipher.txt")
    args = parser.parse_args()

    enigma_machine = Enigma(n_rotors=5)     
    enigma_machine.save_configuration()    
            
    enigma_machine.process_file(args.dst, args.src)  

    enigma_decipher = Enigma(from_config=True)
    enigma_decipher.process_file(args.src + "_deciphered", args.dst)


if __name__ == "__main__":
    main()
