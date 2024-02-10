/*!
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2015 Ivan Vashchaev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 4244)
#elif __MINGW64__

#elif __clang__

#else //Linux
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>
namespace gason
{

    enum JsonTag {
        JSON_NUMBER = 0,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT,
        JSON_TRUE,
        JSON_FALSE,
        JSON_NULL = 0xF
    };

    struct JsonNode;

    #define JSON_VALUE_PAYLOAD_MASK 0x00007FFFFFFFFFFFULL
    #define JSON_VALUE_NAN_MASK 0x7FF8000000000000ULL
    #define JSON_VALUE_TAG_MASK 0xF
    #define JSON_VALUE_TAG_SHIFT 47

    union JsonValue {
        uint64_t ival;
        double fval;

        JsonValue(double x)
                : fval(x) {
        }
        JsonValue(JsonTag tag = JSON_NULL, void *payload = nullptr) {
            assert((uintptr_t)payload <= JSON_VALUE_PAYLOAD_MASK);
            ival = JSON_VALUE_NAN_MASK | ((uint64_t)tag << JSON_VALUE_TAG_SHIFT) | (uintptr_t)payload;
        }
        bool isDouble() const {
            return (int64_t)ival <= (int64_t)JSON_VALUE_NAN_MASK;
        }
        JsonTag getTag() const {
            return isDouble() ? JSON_NUMBER : JsonTag((ival >> JSON_VALUE_TAG_SHIFT) & JSON_VALUE_TAG_MASK);
        }
        uint64_t getPayload() const {
            assert(!isDouble());
            return ival & JSON_VALUE_PAYLOAD_MASK;
        }
        double toNumber() const {
            assert(getTag() == JSON_NUMBER);
            return fval;
        }
        char *toString() const {
            assert(getTag() == JSON_STRING);
            return (char *)getPayload();
        }
        JsonNode *toNode() const {
            assert(getTag() == JSON_ARRAY || getTag() == JSON_OBJECT);
            return (JsonNode *)getPayload();
        }
    };

    struct JsonNode {
        JsonValue value;
        JsonNode *next;
        char *key;
    };

    struct JsonIterator {
        JsonNode *p;

        void operator++() {
            p = p->next;
        }
        bool operator!=(const JsonIterator &x) const {
            return p != x.p;
        }
        JsonNode *operator*() const {
            return p;
        }
        JsonNode *operator->() const {
            return p;
        }
    };

    inline JsonIterator begin(JsonValue o) {
        return JsonIterator{o.toNode()};
    }
    inline JsonIterator end(JsonValue) {
        return JsonIterator{nullptr};
    }

    #define JSON_ERRNO_MAP(XX)                           \
        XX(OK, "ok")                                     \
        XX(BAD_NUMBER, "bad number")                     \
        XX(BAD_STRING, "bad string")                     \
        XX(BAD_IDENTIFIER, "bad identifier")             \
        XX(STACK_OVERFLOW, "stack overflow")             \
        XX(STACK_UNDERFLOW, "stack underflow")           \
        XX(MISMATCH_BRACKET, "mismatch bracket")         \
        XX(UNEXPECTED_CHARACTER, "unexpected character") \
        XX(UNQUOTED_KEY, "unquoted key")                 \
        XX(BREAKING_BAD, "breaking bad")                 \
        XX(ALLOCATION_FAILURE, "allocation failure")

    enum JsonErrno {
    #define XX(no, str) JSON_##no,
        JSON_ERRNO_MAP(XX)
    #undef XX
    };

    const char *jsonStrError(int err);

    class JsonAllocator {
            struct Zone {
                Zone *next;
                size_t used;
            } *head;

        public:
            JsonAllocator() : head(nullptr) {};
            JsonAllocator(const JsonAllocator &) = delete;
            JsonAllocator &operator=(const JsonAllocator &) = delete;
            JsonAllocator(JsonAllocator &&x) : head(x.head) {
                x.head = nullptr;
            }
            JsonAllocator &operator=(JsonAllocator &&x) {
                head = x.head;
                x.head = nullptr;
                return *this;
            }
            ~JsonAllocator() {
                deallocate();
            }
            void *allocate(size_t size);
            void deallocate();
    };

    int jsonParse(char *str, char **endptr, JsonValue *value, JsonAllocator &allocator);

    /*** End of inlined file: gason.h ***/

    #include <stdlib.h>

    #define JSON_ZONE_SIZE 4096
    //#define JSON_ZONE_SIZE 16384
    #define JSON_STACK_SIZE 32

        inline const char *jsonStrError(int err) {
            switch (err) {
    #define XX(no, str) \
        case JSON_##no: \
            return str;
                JSON_ERRNO_MAP(XX)
    #undef XX
                default:
                    return "unknown";
            }
        }

        inline void *JsonAllocator::allocate(size_t size) {
            size = (size + 7) & ~7;

            if (head && head->used + size <= JSON_ZONE_SIZE) {
                char *p = (char *)head + head->used;
                head->used += size;
                return p;
            }

            size_t allocSize = sizeof(Zone) + size;
            Zone *zone = (Zone *)malloc(allocSize <= JSON_ZONE_SIZE ? JSON_ZONE_SIZE : allocSize);
            if (zone == nullptr)
                return nullptr;
            zone->used = allocSize;
            if (allocSize <= JSON_ZONE_SIZE || head == nullptr) {
                zone->next = head;
                head = zone;
            } else {
                zone->next = head->next;
                head->next = zone;
            }
            return (char *)zone + sizeof(Zone);
        }

        inline void JsonAllocator::deallocate() {
            while (head) {
                Zone *next = head->next;
                free(head);
                head = next;
            }
        }

        static inline bool isspace(char c) {
            return c == ' ' || (c >= '\t' && c <= '\r');
        }

        static inline bool isdelim(char c) {
            return c == ',' || c == ':' || c == ']' || c == '}' || isspace(c) || !c;
        }

        static inline bool isdigit(char c) {
            return c >= '0' && c <= '9';
        }

        static inline bool isxdigit(char c) {
            return (c >= '0' && c <= '9') || ((c & ~' ') >= 'A' && (c & ~' ') <= 'F');
        }

        static inline int char2int(char c) {
            if (c <= '9')
                return c - '0';
            return (c & ~' ') - 'A' + 10;
        }

        static double string2double(char *s, char **endptr) {
            char ch = *s;
            if (ch == '-')
                ++s;

            double result = 0;
            while (isdigit(*s))
                result = (result * 10) + (*s++ - '0');

            if (*s == '.') {
                ++s;

                double fraction = 1;
                while (isdigit(*s)) {
                    fraction *= 0.1;
                    result += (*s++ - '0') * fraction;
                }
            }

            if (*s == 'e' || *s == 'E') {
                ++s;

                double base = 10;
                if (*s == '+')
                    ++s;
                else if (*s == '-') {
                    ++s;
                    base = 0.1;
                }

                unsigned int exponent = 0;
                while (isdigit(*s))
                    exponent = (exponent * 10) + (*s++ - '0');

                double power = 1;
                for (; exponent; exponent >>= 1, base *= base)
                    if (exponent & 1)
                        power *= base;

                result *= power;
            }

            *endptr = s;
            return ch == '-' ? -result : result;
        }

        static inline JsonNode *insertAfter(JsonNode *tail, JsonNode *node) {
            if (!tail)
                return node->next = node;
            node->next = tail->next;
            tail->next = node;
            return node;
        }

        static inline JsonValue listToValue(JsonTag tag, JsonNode *tail) {
            if (tail) {
                auto head = tail->next;
                tail->next = nullptr;
                return JsonValue(tag, head);
            }
            return JsonValue(tag, nullptr);
        }

        inline int jsonParse(char *s, char **endptr, JsonValue *value, JsonAllocator &allocator) {
            JsonNode *tails[JSON_STACK_SIZE];
            JsonTag tags[JSON_STACK_SIZE];
            char *keys[JSON_STACK_SIZE];
            JsonValue o;
            int pos = -1;
            bool separator = true;
            JsonNode *node;
            *endptr = s;

            while (*s) {
                while (isspace(*s)) {
                    ++s;
                    if (!*s) break;
                }
                *endptr = s++;
                switch (**endptr) {
                    case '-':
                        if (!isdigit(*s) && *s != '.') {
                            *endptr = s;
                            return JSON_BAD_NUMBER;
                        }
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        o = JsonValue(string2double(*endptr, &s));
                        if (!isdelim(*s)) {
                            *endptr = s;
                            return JSON_BAD_NUMBER;
                        }
                        break;
                    case '"':
                        o = JsonValue(JSON_STRING, s);
                        for (char *it = s; *s; ++it, ++s) {
                            int c = *it = *s;
                            if (c == '\\') {
                                c = *++s;
                                switch (c) {
                                    case '\\':
                                    case '"':
                                    case '/':
                                        *it = c;
                                        break;
                                    case 'b':
                                        *it = '\b';
                                        break;
                                    case 'f':
                                        *it = '\f';
                                        break;
                                    case 'n':
                                        *it = '\n';
                                        break;
                                    case 'r':
                                        *it = '\r';
                                        break;
                                    case 't':
                                        *it = '\t';
                                        break;
                                    case 'u':
                                        c = 0;
                                        for (int i = 0; i < 4; ++i) {
                                            if (isxdigit(*++s)) {
                                                c = c * 16 + char2int(*s);
                                            } else {
                                                *endptr = s;
                                                return JSON_BAD_STRING;
                                            }
                                        }
                                        if (c < 0x80) {
                                            *it = c;
                                        } else if (c < 0x800) {
                                            *it++ = 0xC0 | (c >> 6);
                                            *it = 0x80 | (c & 0x3F);
                                        } else {
                                            *it++ = 0xE0 | (c >> 12);
                                            *it++ = 0x80 | ((c >> 6) & 0x3F);
                                            *it = 0x80 | (c & 0x3F);
                                        }
                                        break;
                                    default:
                                        *endptr = s;
                                        return JSON_BAD_STRING;
                                }
                            } else if ((unsigned int)c < ' ' || c == '\x7F') {
                                *endptr = s;
                                return JSON_BAD_STRING;
                            } else if (c == '"') {
                                *it = 0;
                                ++s;
                                break;
                            }
                        }
                        if (!isdelim(*s)) {
                            *endptr = s;
                            return JSON_BAD_STRING;
                        }
                        break;
                    case 't':
                        if (!(s[0] == 'r' && s[1] == 'u' && s[2] == 'e' && isdelim(s[3])))
                            return JSON_BAD_IDENTIFIER;
                        o = JsonValue(JSON_TRUE);
                        s += 3;
                        break;
                    case 'f':
                        if (!(s[0] == 'a' && s[1] == 'l' && s[2] == 's' && s[3] == 'e' && isdelim(s[4])))
                            return JSON_BAD_IDENTIFIER;
                        o = JsonValue(JSON_FALSE);
                        s += 4;
                        break;
                    case 'n':
                        if (!(s[0] == 'u' && s[1] == 'l' && s[2] == 'l' && isdelim(s[3])))
                            return JSON_BAD_IDENTIFIER;
                        o = JsonValue(JSON_NULL);
                        s += 3;
                        break;
                    case ']':
                        if (pos == -1)
                            return JSON_STACK_UNDERFLOW;
                        if (tags[pos] != JSON_ARRAY)
                            return JSON_MISMATCH_BRACKET;
                        o = listToValue(JSON_ARRAY, tails[pos--]);
                        break;
                    case '}':
                        if (pos == -1)
                            return JSON_STACK_UNDERFLOW;
                        if (tags[pos] != JSON_OBJECT)
                            return JSON_MISMATCH_BRACKET;
                        if (keys[pos] != nullptr)
                            return JSON_UNEXPECTED_CHARACTER;
                        o = listToValue(JSON_OBJECT, tails[pos--]);
                        break;
                    case '[':
                        if (++pos == JSON_STACK_SIZE)
                            return JSON_STACK_OVERFLOW;
                        tails[pos] = nullptr;
                        tags[pos] = JSON_ARRAY;
                        keys[pos] = nullptr;
                        separator = true;
                        continue;
                    case '{':
                        if (++pos == JSON_STACK_SIZE)
                            return JSON_STACK_OVERFLOW;
                        tails[pos] = nullptr;
                        tags[pos] = JSON_OBJECT;
                        keys[pos] = nullptr;
                        separator = true;
                        continue;
                    case ':':
                        if (separator || keys[pos] == nullptr)
                            return JSON_UNEXPECTED_CHARACTER;
                        separator = true;
                        continue;
                    case ',':
                        if (separator || keys[pos] != nullptr)
                            return JSON_UNEXPECTED_CHARACTER;
                        separator = true;
                        continue;
                    case '\0':
                        continue;
                    default:
                        return JSON_UNEXPECTED_CHARACTER;
                }

                separator = false;

                if (pos == -1) {
                    *endptr = s;
                    *value = o;
                    return JSON_OK;
                }

                if (tags[pos] == JSON_OBJECT) {
                    if (!keys[pos]) {
                        if (o.getTag() != JSON_STRING)
                            return JSON_UNQUOTED_KEY;
                        keys[pos] = o.toString();
                        continue;
                    }
                    if ((node = (JsonNode *) allocator.allocate(sizeof(JsonNode))) == nullptr)
                        return JSON_ALLOCATION_FAILURE;
                    tails[pos] = insertAfter(tails[pos], node);
                    tails[pos]->key = keys[pos];
                    keys[pos] = nullptr;
                } else {
                    if ((node = (JsonNode *) allocator.allocate(sizeof(JsonNode) - sizeof(char *))) == nullptr)
                        return JSON_ALLOCATION_FAILURE;
                    tails[pos] = insertAfter(tails[pos], node);
                }
                tails[pos]->value = o;
            }
            return JSON_BREAKING_BAD;
        }
}

#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(pop)
#elif __MINGW64__

#elif __clang__

#else //Linux
    #pragma GCC diagnostic pop
#endif