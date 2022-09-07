#include <stdint.h>
#include <string.h>

#include "../include/define.h"
#include "../include/utils.h"
#include "../include/crypto.h"




int32_t main(int32_t argc, char **argv) {
    const char *filename = get_filename(argc, argv);

    const char ciphered[] = "ciphered"; 
    const char deciphered[] = "deciphered"; 

    int code = encrypt(filename, ciphered);
    printf("ENCRYPTION DONE! CODE = <%d>!\n", code);
    code = decrypt(ciphered, deciphered);
    printf("DECRYPTION DONE! CODE = <%d>!\n", code);

    return SUCCESS;
}