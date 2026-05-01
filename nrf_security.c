/**
 * @file nrf_security.c
 * @brief Payload encryption and decryption stubs.
 *
 * Implement a cipher here when encryption is required.
 */

#include <stdint.h>

#include "nrf_security.h"

void nrf24l01p_security_encrypt(const uint8_t *plaintext, uint8_t *ciphertext)
{
    (void)plaintext;
    (void)ciphertext;
}

void nrf24l01p_security_decrypt(const uint8_t *ciphertext, uint8_t *plaintext)
{
    (void)ciphertext;
    (void)plaintext;
}
