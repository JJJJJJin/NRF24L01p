/**
 * @file nrf_security.h
 * @brief Payload encryption and decryption interface.
 *
 * These functions are currently unimplemented stubs.  Populate the
 * corresponding .c file with a suitable cipher when encryption is required.
 */

#ifndef NRF_SECURITY_H
#define NRF_SECURITY_H

#include <stdint.h>

/**
 * @brief Encrypt a plaintext payload in-place or into a separate buffer.
 *
 * @param plaintext  Input data to encrypt.
 * @param ciphertext Output buffer for the encrypted result.
 */
void nrf24l01p_security_encrypt(const uint8_t *plaintext, uint8_t *ciphertext);

/**
 * @brief Decrypt a received payload in-place or into a separate buffer.
 *
 * @param ciphertext Input data to decrypt.
 * @param plaintext  Output buffer for the decrypted result.
 */
void nrf24l01p_security_decrypt(const uint8_t *ciphertext, uint8_t *plaintext);

#endif /* NRF_SECURITY_H */
