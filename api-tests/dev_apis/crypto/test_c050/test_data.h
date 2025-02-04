/** @file
 * Copyright (c) 2019-2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"
#include "test_crypto_common.h"

typedef struct {
    char                    test_desc[75];
    psa_key_type_t          key_type;
    uint8_t                 key_data[32];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    uint8_t                 input[32];
    size_t                  input_length;
    size_t                  output_size;
    uint8_t                 expected_output[32];
    size_t                  expected_output_length;
    psa_status_t            expected_status;
} test_data;

#if ((defined(ARCH_TEST_AES_128) && (defined(ARCH_TEST_CBC_NO_PADDING) || \
      defined(ARCH_TEST_CBC_PKCS7) || defined(ARCH_TEST_CIPHER_MODE_CTR))) || \
     (defined(ARCH_TEST_CBC_NO_PADDING) && (defined(ARCH_TEST_DES_1KEY) || \
      defined(ARCH_TEST_DES_2KEY) || defined(ARCH_TEST_DES_3KEY))))
static const test_data check1[] = {
#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CBC_NO_PADDING
{"Test psa_cipher_encrypt - Encrypt - AES CBC_NO_PADDING\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a},
 16, PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16),
{0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d},
32, PSA_SUCCESS
},

{"Test psa_cipher_encrypt - Encrypt - AES CBC_NO_PADDING (Short input)\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0x6b, 0xc1, 0xbe, 0xe2, 0x23}, 5,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(5),
{0x6b, 0xc1, 0xbe, 0xe2, 0x23}, 21, PSA_ERROR_INVALID_ARGUMENT
},
#endif

#ifdef ARCH_TEST_CBC_PKCS7
{"Test psa_cipher_encrypt - Encrypt - AES CBC_PKCS7\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_PKCS7,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17, 0x2a}, 16,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16),
{0xa0, 0x76, 0xec, 0x9d, 0xfb, 0xe4, 0x7d, 0x52, 0xaf, 0xc3, 0x57, 0x33, 0x6f,
 0x20, 0x74, 0x3b, 0xca, 0x7e, 0x8a, 0x15, 0xdc, 0x3c, 0x77, 0x64, 0x36, 0x31,
 0x42, 0x93, 0x03, 0x1c, 0xd4, 0xf3}, 48, PSA_SUCCESS
},

{"Test psa_cipher_encrypt - Encrypt - AES CBC_PKCS7 (Short input)\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_PKCS7,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17}, 15,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(15),
{0x62, 0x79, 0xb4, 0x9d, 0x7f, 0x7a, 0x8d, 0xd8, 0x7b, 0x68, 0x51, 0x75, 0xd4,
 0x27, 0x6e, 0x24}, 32, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CIPHER_MODE_CTR
{"Test psa_cipher_encrypt - Encrypt - AES CTR\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17, 0x2a}, 16,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16),
{0x8f, 0x94, 0x08, 0xfe, 0x80, 0xa8, 0x1d, 0x3e, 0x81, 0x3d, 0xa3, 0xc7, 0xb0,
 0xb2, 0xbd, 0x32}, 32, PSA_SUCCESS
},

{"Test psa_cipher_encrypt - Encrypt - AES CTR (short input)\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17}, 15,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(15),
{0x8f, 0x94, 0x08, 0xfe, 0x80, 0xa8, 0x1d, 0x3e, 0x81, 0x3d, 0xa3, 0xc7, 0xb0,
 0xb2, 0xbd}, 31, PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_encrypt - Encrypt - DES CBC (nopad)\n", PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e}, DES_8B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0xed, 0xa4, 0x01, 0x12, 0x39, 0xbc, 0x3a, 0xc9}, 8,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(8),
{0x64, 0xf9, 0x17, 0xb0, 0x15, 0x2f, 0x8f, 0x05}, 16, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_cipher_encrypt - Encrypt - 2-key 3DE -CBC (nopad)\n", PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce}, DES3_2B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0xed, 0xa4, 0x01, 0x12, 0x39, 0xbc, 0x3a, 0xc9}, 8,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(8),
{0x5d, 0x06, 0x52, 0x42, 0x9c, 0x5b, 0x0a, 0xc7}, 16, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_cipher_encrypt - Encrypt - 3-key 3DE -CBC (nopad)\n", PSA_KEY_TYPE_DES,
{0x01, 0x02, 0x04, 0x07, 0x08, 0x0b, 0x0d, 0x0e, 0xc1, 0xc2, 0xc4, 0xc7, 0xc8,
 0xcb, 0xcd, 0xce, 0x31, 0x32, 0x34, 0x37, 0x38, 0x3b, 0x3d, 0x3e}, DES3_3B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0xed, 0xa4, 0x01, 0x12, 0x39, 0xbc, 0x3a, 0xc9}, 8,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16),
{0x81, 0x7c, 0xa7, 0xd6, 0x9b, 0x80, 0xd8, 0x6a}, 16, PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CBC_PKCS7
{"Test psa_cipher_encrypt - small output buffer size\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_PKCS7,
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17, 0x2a}, 16, 15,
{0xa0, 0x76, 0xec, 0x9d, 0xfb, 0xe4, 0x7d, 0x52, 0xaf, 0xc3, 0x57, 0x33, 0x6f,
 0x20, 0x74, 0x3b, 0xca, 0x7e, 0x8a, 0x15, 0xdc, 0x3c, 0x77, 0x64, 0x36, 0x31,
 0x42, 0x93, 0x03, 0x1c, 0xd4, 0xf3}, 16, PSA_ERROR_BUFFER_TOO_SMALL
},
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
{"Test psa_cipher_encrypt - Decrypt - AES CBC_NO_PADDING\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING,
{0xA0, 0x76, 0xEC, 0x9D, 0xFB, 0xE4, 0x7D, 0x52, 0xAF, 0xC3, 0x57, 0x33, 0x6F,
 0x20, 0x74, 0x3B},
 16,  PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16),
{0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73,
 0x93, 0x17, 0x2a}, 0, PSA_ERROR_NOT_PERMITTED
},
#endif

#ifdef ARCH_TEST_ARIA
{"Test psa_cipher_encrypt - Encrypt - AES ARIA\n", PSA_KEY_TYPE_ARIA,
{0x00, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
0x0c, 0x0d, 0x0e, 0x0f}, AES_16B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
16, PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(16), {0xd7, 0x18, 0xfb, 0xd6, 0xab, 0x64, 0x4c, 0x73, 0x9d,
0xa9, 0x5f, 0x3b, 0xe6, 0x45, 0x17, 0x78}, 32, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_CCM_STAR_NO_TAG
{"Test psa_cipher_encrypt - Encrypt - CCM_STAR_NO_TAG\n", PSA_KEY_TYPE_AES,
{0x90, 0x92, 0x9a, 0x4b, 0x0a, 0xc6, 0x5b, 0x35, 0x0a, 0xd1, 0x59, 0x16, 0x11, 0xfe, 0x48, 0x29},
AES_16B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CCM_STAR_NO_TAG,
{0x4b, 0xfe, 0x4e, 0x35, 0x78, 0x4f, 0x0a, 0x65, 0xb5, 0x45, 0x47, 0x7e, 0x5e, 0x2f, 0x4b, 0xae,
0x0e, 0x1e, 0x6f, 0xa7, 0x17, 0xea, 0xf2, 0xcb}, 24, PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(24),
{0xa1, 0x6a, 0x2e, 0x74, 0x1f, 0x1c, 0xd9, 0x71, 0x72, 0x85, 0xb6, 0xd8, 0x82, 0xc1, 0xfc, 0x53,
0x65, 0x5e, 0x97, 0x73, 0x76, 0x1a, 0xd6, 0x97}, 37, PSA_SUCCESS},
#endif
#endif

#ifdef ARCH_TEST_CHACHA20
{"Test psa_cipher_encrypt - Encrypt - CHACHA20 stream cipher\n", PSA_KEY_TYPE_CHACHA20,
{0x1c, 0x92, 0x40, 0xa5, 0xeb, 0x55, 0xd3, 0x8a, 0xf3, 0x33, 0x88, 0x86, 0x04, 0xf6, 0xb5, 0xf0,
0x47, 0x39, 0x17, 0xc1, 0x40, 0x2b, 0x80, 0x09, 0x9d, 0xca, 0x5c, 0xbc, 0x20, 0x70, 0x75, 0xc0},
32, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_STREAM_CIPHER,
{0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x2E}, 15,
PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(15),
{0xdf, 0x2a, 0x1b, 0xaf, 0x8a, 0xcc, 0x1f, 0xf9, 0x25, 0x6d, 0x6b, 0x86, 0xb7, 0x5d, 0xd7}, 27,
PSA_SUCCESS
}
#endif

#ifdef ARCH_TEST_XCHACHA20
{"Test psa_cipher_encrypt - Encrypt - XCHACHA20 stream cipher\n", PSA_KEY_TYPE_XCHACHA20,
{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f},
32, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_STREAM_CIPHER,
{0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x2E}, 15,
PSA_CIPHER_ENCRYPT_OUTPUT_MAX_SIZE(15),
{0x9c, 0xd4, 0xb4, 0x42, 0xb0, 0xd7, 0x1e, 0xe5, 0x00, 0x91, 0x02, 0x6b, 0xe5, 0x74, 0x09}, 39,
PSA_SUCCESS
}
#endif
};
#endif
