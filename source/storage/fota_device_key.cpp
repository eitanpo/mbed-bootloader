// ----------------------------------------------------------------------------
// Copyright 2021 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------


#include <inttypes.h>
#include <stddef.h>
#include "fota_device_key.h"
#include "DirectAccessDevicekey.h"

/**
* @brief Function to get the device root of trust
* @details The device root of trust should be a 128 bit value. It should never leave the device.
*          It should be unique to the device. It should have enough entropy to avoid conventional
*          entropy attacks. The porter should implement the following device signature to provide
*          device root of trust on different platforms.
*
* @param key_buf buffer to be filled with the device root of trust.
* @param length  length of the buffer provided to make sure no overflow occurs.
*
* @return 0 on success, non-zero on failure.
*/

int8_t fota_get_device_key_128bit(uint8_t *key_buf, uint32_t length)
{
    int8_t error = 0;
    uint32_t tdb_start_offset = 0;
    uint32_t tdb_end_offset = 0;
    size_t actual_len_bytes = 0;

    // Get TDB parameters
    if (error == 0) {
        error = get_expected_internal_TDBStore_position(&tdb_start_offset,
                                                        &tdb_end_offset);
    }

    // Read ROT    
    if (error == 0) {
        error = direct_access_to_devicekey(tdb_start_offset,
                                           tdb_end_offset,
                                           key_buf,
                                           length,
                                           &actual_len_bytes);
        if (actual_len_bytes != length) {
            error = -1;
        }
    }

    return error;
}
