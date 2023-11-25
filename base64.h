//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef STRUCTTOBASE64_BASE64_H
#define STRUCTTOBASE64_BASE64_H

#include "cstring"
#include "cstdint"

uint8_t *decodeBase64(const char *base64String, size_t length);

char *encodeBase64(uint8_t *data, size_t length);

#endif //STRUCTTOBASE64_BASE64_H
