#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <systemd/sd-id128.h>
#include <openssl/evp.h>


#define SUCCESS 0
#define OPENSSL_ERROR 1
#define SD_ID128_ERROR 2


#ifndef MID
#define MID "deadbeef"  // Stub
#endif



int8_t generate_sha256(unsigned char **digest, const char *message) {
    EVP_MD_CTX *mdctx;

	if (!(mdctx = EVP_MD_CTX_new())) {
        return OPENSSL_ERROR;
    }

	if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        return OPENSSL_ERROR;
    }

	if (1 != EVP_DigestUpdate(mdctx, message, strlen(message))) {
        return OPENSSL_ERROR;
    }

    uint32_t len;
	if (1 != EVP_DigestFinal_ex(mdctx, *digest, &len)) {
        return OPENSSL_ERROR;
    }

	EVP_MD_CTX_free(mdctx);
    return SUCCESS;
}


int8_t check_mid() {
    sd_id128_t sd_mid;
    int8_t code = sd_id128_get_machine(&sd_mid);
    if (code) {
        return SD_ID128_ERROR;
    }
    const char *mid = SD_ID128_CONST_STR(sd_mid);

    unsigned char digest_mid[EVP_MAX_MD_SIZE] = { 0 };
    unsigned char *digest_ptr = digest_mid;
    code = generate_sha256(&digest_ptr, mid);
    if (code) {
        return OPENSSL_ERROR;
    }

    unsigned char digest_defined[EVP_MAX_MD_SIZE] = { 0 };
    digest_ptr = digest_defined;
    code = generate_sha256(&digest_ptr, MID);
    if (code) {
        return OPENSSL_ERROR;
    }

    return strcmp(digest_mid, digest_defined);
}


int32_t main() {
    printf("### INFORMATION SECURITY <lab_01>: ");
    if (!check_mid()) {
        printf("### ACCESS GRANTED! ###\n");
    } else {
        printf("### ACCESS DENIED! ###\n");  
    }

    return SUCCESS;
}
