// Filename: yang_json.cpp
// Score: 95

#include <iostream>
#include <cstring>
#include <climits>
#include <cstdint>

// Define constants and types
#define Yang_JSON_Invalid 0
#define Yang_JSON_NULL 1
#define Yang_JSON_False 2
#define Yang_JSON_True 3
#define Yang_JSON_Number 4
#define Yang_JSON_String 5
#define Yang_JSON_Array 6
#define Yang_JSON_Object 7
#define Yang_JSON_Raw 8
#define Yang_JSON_IsReference 256
#define Yang_JSON_StringIsConst 512
#define Yang_JSON_NESTING_LIMIT 1000

typedef int yangbool;
#define yangtrue 1
#define yangfalse 0

struct internal_hooks {
    void* (*allocate)(size_t size);
    void (*deallocate)(void* pointer);
    void* (*reallocate)(void* pointer, size_t size);
};

struct YangJson {
    struct YangJson* next;
    struct YangJson* prev;
    struct YangJson* child;
    int type;
    char* valuestring;
    int valueint;
    double valuedouble;
    char* string;
    char* outString;
};

struct parse_buffer {
    const uint8_t* content;
    size_t length;
    size_t offset;
    int depth;
    internal_hooks hooks;
};

struct printbuffer {
    uint8_t* buffer;
    size_t length;
    size_t offset;
    int depth;
    yangbool format;
    internal_hooks hooks;
};

static internal_hooks global_hooks = { malloc, free, realloc };
static struct {
    const uint8_t* json;
    size_t position;
} global_error = { NULL, 0 };

static void* yang_json_malloc(size_t size) {
    return global_hooks.allocate(size);
}

static void yang_json_free(void* object) {
    global_hooks.deallocate(object);
}

static YangJson* yang_json_new_item(const internal_hooks* hooks) {
    YangJson* node = (YangJson*)hooks->allocate(sizeof(YangJson));
    if (node) {
        memset(node, 0, sizeof(YangJson));
    }
    return node;
}

static void yang_json_delete(YangJson* item) {
    YangJson* next;
    while (item) {
        next = item->next;
        if (!(item->type & Yang_JSON_IsReference) && item->child) {
            yang_json_delete(item->child);
        }
        if (!(item->type & Yang_JSON_IsReference) && item->valuestring) {
            global_hooks.deallocate(item->valuestring);
        }
        if (!(item->type & Yang_JSON_StringIsConst) && item->string) {
            global_hooks.deallocate(item->string);
        }
        global_hooks.deallocate(item);
        item = next;
    }
}

static uint8_t* ensure(printbuffer* p, size_t needed) {
    uint8_t* newbuffer;
    size_t newsize;
    if (!p || !p->buffer) return NULL;
    if (p->length > p->offset + needed) return p->buffer + p->offset;
    newsize = p->length * 2;
    if (newsize < p->length + needed) newsize = p->length + needed;
    if (p->hooks.reallocate) {
        newbuffer = (uint8_t*)p->hooks.reallocate(p->buffer, newsize);
        if (!newbuffer) {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;
            return NULL;
        }
    } else {
        newbuffer = (uint8_t*)p->hooks.allocate(newsize);
        if (!newbuffer) {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;
            return NULL;
        }
        memcpy(newbuffer, p->buffer, p->offset);
        p->hooks.deallocate(p->buffer);
    }
    p->length = newsize;
    p->buffer = newbuffer;
    return newbuffer + p->offset;
}

static void update_offset(printbuffer* buffer) {
    if (buffer && buffer->buffer) {
        buffer->offset = strlen((const char*)buffer->buffer);
    }
}

static yangbool parse_value(YangJson* const item, parse_buffer* const input_buffer);
static yangbool print_value(const YangJson* const item, printbuffer* const output_buffer);
static yangbool parse_string(YangJson* const item, parse_buffer* const input_buffer);
static yangbool print_string(const YangJson* const item, printbuffer* const p);
static yangbool parse_array(YangJson* const item, parse_buffer* const input_buffer);
static yangbool print_array(const YangJson* const item, printbuffer* const output_buffer);
static yangbool parse_object(YangJson* const item, parse_buffer* const input_buffer);
static yangbool print_object(const YangJson* const item, printbuffer* const output_buffer);

static uint8_t* print(YangJson* item, yangbool format, const internal_hooks* const hooks) {
    static const size_t default_buffer_size = 256;
    printbuffer buffer[1];
    uint8_t* printed = NULL;
    memset(buffer, 0, sizeof(buffer));
    buffer->buffer = (uint8_t*)hooks->allocate(default_buffer_size);
    buffer->length = default_buffer_size;
    buffer->format = format;
    buffer->hooks = *hooks;
    if (!buffer->buffer) goto fail;
    if (!print_value(item, buffer)) goto fail;
    update_offset(buffer);
    if (hooks->reallocate) {
        printed = (uint8_t*)hooks->reallocate(buffer->buffer, buffer->offset + 1);
        if (!printed) goto fail;
        buffer->buffer = NULL;
    } else {
        printed = (uint8_t*)hooks->allocate(buffer->offset + 1);
        if (!printed) goto fail;
        memcpy(printed, buffer->buffer, buffer->offset + 1);
        hooks->deallocate(buffer->buffer);
    }
    yang_json_free(item->outString);
    item->outString = (char*)printed;
    return printed;
fail:
    if (buffer->buffer) hooks->deallocate(buffer->buffer);
    if (printed) hooks->deallocate(printed);
    return NULL;
}

char* yang_json_print(YangJson* item) {
    return (char*)print(item, yangtrue, &global_hooks);
}

char* yang_json_printUnformatted(YangJson* item) {
    return (char*)print(item, yangfalse, &global_hooks);
}

char* yang_json_printBuffered(YangJson* item, int prebuffer, yangbool fmt) {
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    if (prebuffer < 0) return NULL;
    p.buffer = (uint8_t*)global_hooks.allocate((size_t)prebuffer);
    if (!p.buffer) return NULL;
    p.length = (size_t)prebuffer;
    p.offset = 0;
    p.noalloc = yangfalse;
    p.format = fmt;
    p.hooks = global_hooks;
    if (!print_value(item, &p)) {
        global_hooks.deallocate(p.buffer);
        return NULL;
    }
    yang_json_free(item->outString);
    item->outString = (char*)p.buffer;
    return (char*)p.buffer;
}

yangbool yang_json_printPreallocated(YangJson* item, char* buffer, const int length, const yangbool format) {
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    if ((length < 0) || (buffer == NULL)) return yangfalse;
    p.buffer = (uint8_t*)buffer;
    p.length = (size_t)length;
    p.offset = 0;
    p.noalloc = yangtrue;
    p.format = format;
    p.hooks = global_hooks;
    return print_value(item, &p);
}

static yangbool parse_value(YangJson* const item, parse_buffer* const input_buffer) {
    if (!input_buffer || !input_buffer->content) return yangfalse;
    if (strncmp((const char*)buffer_at_offset(input_buffer), "null", 4) == 0) {
        item->type = Yang_JSON_NULL;
        input_buffer->offset += 4;
        return yangtrue;
    }
    if (strncmp((const char*)buffer_at_offset(input_buffer), "false", 5) == 0) {
        item->type = Yang_JSON_False;
        input_buffer->offset += 5;
        return yangtrue;
    }
    if (strncmp((const char*)buffer_at_offset(input_buffer), "true", 4) == 0) {
        item->type = Yang_JSON_True;
        item->valueint = 1;
        input_buffer->offset += 4;
        return yangtrue;
    }
    if (buffer_at_offset(input_buffer)[0] == '\"') return parse_string(item, input_buffer);
    if ((buffer_at_offset(input_buffer)[0] == '-') || ((buffer_at_offset(input_buffer)[0] >= '0') && (buffer_at_offset(input_buffer)[0] <= '9'))) return parse_number(item, input_buffer);
    if (buffer_at_offset(input_buffer)[0] == '[') return parse_array(item, input_buffer);
    if (buffer_at_offset(input_buffer)[0] == '{') return parse_object(item, input_buffer);
    return yangfalse;
}

static yangbool print_value(const YangJson* const item, printbuffer* const output_buffer) {
    uint8_t* output = NULL;
    if (!item || !output_buffer) return yangfalse;
    switch (item->type & 0xFF) {
        case Yang_JSON_NULL:
            output = ensure(output_buffer, 5);
            if (!output) return yangfalse;
            strncpy((char*)output, "null");
            return yangtrue;
        case Yang_JSON_False:
            output = ensure(output_buffer, 6);
            if (!output) return yangfalse;
            strncpy((char*)output, "false");
            return yangtrue;
        case Yang_JSON_True:
            output = ensure(output_buffer, 5);
            if (!output) return yangfalse;
            strncpy((char*)output, "true");
            return yangtrue;
        case Yang_JSON_Number:
            return print_number(item, output_buffer);
        case Yang_JSON_Raw:
            if (!item->valuestring) return yangfalse;
            output = ensure(output_buffer, strlen(item->valuestring) + 1);
            if (!output) return yangfalse;
            strncpy((char*)output, item->valuestring);
            return yangtrue;
        case Yang_JSON_String:
            return print_string(item, output_buffer);
        case Yang_JSON_Array:
            return print_array(item, output_buffer);
        case Yang_JSON_Object:
            return print_object(item, output_buffer);
        default:
            return yangfalse;
    }
}

static yangbool parse_string(YangJson* const item, parse_buffer* const input_buffer) {
    const uint8_t* input_pointer = buffer_at_offset(input_buffer) + 1;
    const uint8_t* input_end = buffer_at_offset(input_buffer) + 1;
    uint8_t* output_pointer = NULL;
    uint8_t* output = NULL;
    if (buffer_at_offset(input_buffer)[0] != '\"') goto fail;
    size_t allocation_length = 0;
    size_t skipped_bytes = 0;
    while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"')) {
        if (input_end[0] == '\\') {
            if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length) goto fail;
            skipped_bytes++;
            input_end++;
        }
        input_end++;
    }
    if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"')) goto fail;
    allocation_length = (size_t)(input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
    output = (uint8_t*)input_buffer->hooks.allocate(allocation_length + 1);
    if (!output) goto fail;
    output_pointer = output;
    while (input_pointer < input_end) {
        if (*input_pointer != '\\') {
            *output_pointer++ = *input_pointer++;
        } else {
            uint8_t sequence_length = 2;
            if ((input_end - input_pointer) < 1) goto fail;
            switch (input_pointer[1]) {
                case 'b':
                    *output_pointer++ = '\b';
                    break;
                case 'f':
                    *output_pointer++ = '\f';
                    break;
                case 'n':
                    *output_pointer++ = '\n';
                    break;
                case 'r':
                    *output_pointer++ = '\r';
                    break;
                case 't':
                    *output_pointer++ = '\t';
                    break;
                case '\"':
                case '\\':
                case '/':
                    *output_pointer++ = input_pointer[1];
                    break;
                case 'u':
                    sequence_length = utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
                    if (sequence_length == 0) goto fail;
                    break;
                default:
                    goto fail;
            }
            input_pointer += sequence_length;
        }
    }
    *output_pointer = '\0';
    item->type = Yang_JSON_String;
    item->valuestring = (char*)output;
    input_buffer->offset = (size_t)(input_end - input_buffer->content);
    input_buffer->offset++;
    return yangtrue;
fail:
    if (output) input_buffer->hooks.deallocate(output);
    if (input_pointer) input_buffer->offset = (size_t)(input_pointer - input_buffer->content);
    return yangfalse;
}

static yangbool print_string(const YangJson* const item, printbuffer* const p) {
    return print_string_ptr((uint8_t*)item->valuestring, p);
}

static yangbool parse_array(YangJson* const item, parse_buffer* const input_buffer) {
    YangJson* head = NULL;
    YangJson* current_item = NULL;
    if (input_buffer->depth >= Yang_JSON_NESTING_LIMIT) return yangfalse;
    input_buffer->depth++;
    if (buffer_at_offset(input_buffer)[0] != '[') goto fail;
    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ']')) goto success;
    if (cannot_access_at_index(input_buffer, 0)) {
        input_buffer->offset--;
        goto fail;
    }
    input_buffer->offset--;
    do {
        YangJson* new_item = yang_json_new_item(&(input_buffer->hooks));
        if (!new_item) goto fail;
        if (!head) {
            current_item = head = new_item;
        } else {
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_value(current_item, input_buffer)) goto fail;
        buffer_skip_whitespace(input_buffer);
    } while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));
    if (cannot_access_at_index(input_buffer, 0) || buffer_at_offset(input_buffer)[0] != ']') goto fail;
success:
    input_buffer->depth--;
    if (head) head->prev = current_item;
    item->type = Yang_JSON_Array;
    item->child = head;
    input_buffer->offset++;
    return yangtrue;
fail:
    if (head) yang_json_delete(head);
    return yangfalse;
}

static yangbool print_array(const YangJson* const item, printbuffer* const output_buffer) {
    uint8_t* output_pointer = NULL;
    size_t length = 0;
    YangJson* current_element = item->child;
    if (!output_buffer) return yangfalse;
    output_pointer = ensure(output_buffer, 1);
    if (!output_pointer) return yangfalse;
    *output_pointer = '[';
    output_buffer->offset++;
    output_buffer->depth++;
    while (current_element) {
        if (!print_value(current_element, output_buffer)) return yangfalse;
        update_offset(output_buffer);
        if (current_element->next) {
            length = (size_t)(output_buffer->format ? 2 : 1);
            output_pointer = ensure(output_buffer, length + 1);
            if (!output_pointer) return yangfalse;
            *output_pointer++ = ',';
            if (output_buffer->format) *output_pointer++ = ' ';
            *output_pointer = '\0';
            output_buffer->offset += length;
        }
        current_element = current_element->next;
    }
    output_pointer = ensure(output_buffer, 2);
    if (!output_pointer) return yangfalse;
    *output_pointer++ = ']';
    *output_pointer = '\0';
    output_buffer->depth--;
    return yangtrue;
}

static yangbool parse_object(YangJson* const item, parse_buffer* const input_buffer) {
    YangJson* head = NULL;
    YangJson* current_item = NULL;
    if (input_buffer->depth >= Yang_JSON_NESTING_LIMIT) return yangfalse;
    input_buffer->depth++;
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '{')) goto fail;
    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '}')) goto success;
    if (cannot_access_at_index(input_buffer, 0)) {
        input_buffer->offset--;
        goto fail;
    }
    input_buffer->offset--;
    do {
        YangJson* new_item = yang_json_new_item(&(input_buffer->hooks));
        if (!new_item) goto fail;
        if (!head) {
            current_item = head = new_item;
        } else {
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_string(current_item, input_buffer)) goto fail;
        buffer_skip_whitespace(input_buffer);
        current_item->string = current_item->valuestring;
        current_item->valuestring = NULL;
        if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != ':')) goto fail;
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
    } while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '}')) {
        goto fail; // expected end of object
    }

success:
    input_buffer->depth--;

    if (head != NULL) {
        head->prev = current_item;
    }

    item->type = Yang_JSON_Object;
    item->child = head;
    input_buffer->offset++;
    return yangtrue;

fail:
    if (head != NULL) {
        yang_json_delete(head);
    }

    return yangfalse;
}

static yangbool print_object(const YangJson* const item, printbuffer* const output_buffer) {
    uint8_t* output_pointer = NULL;
    size_t length = 0;
    YangJson* current_item = item->child;

    if (output_buffer == NULL) {
        return yangfalse;
    }

    length = (size_t)(output_buffer->format ? 2 : 1); // fmt: {\n
    output_pointer = ensure(output_buffer, length + 1);
    if (output_pointer == NULL) {
        return yangfalse;
    }

    *output_pointer++ = '{';
    output_buffer->depth++;
    if (output_buffer->format) {
        *output_pointer++ = '\n';
    }
    output_buffer->offset += length;

    while (current_item) {
        if (output_buffer->format) {
            size_t i;
            output_pointer = ensure(output_buffer, output_buffer->depth);
            if (output_pointer == NULL) {
                return yangfalse;
            }
            for (i = 0; i < output_buffer->depth; i++) {
                *output_pointer++ = '\t';
            }
            output_buffer->offset += output_buffer->depth;
        }

        // print key
        if (!print_string_ptr((uint8_t*)current_item->string, output_buffer)) {
            return yangfalse;
        }
        update_offset(output_buffer);

        length = (size_t)(output_buffer->format ? 2 : 1);
        output_pointer = ensure(output_buffer, length);
        if (output_pointer == NULL) {
            return yangfalse;
        }
        *output_pointer++ = ':';
        if (output_buffer->format) {
            *output_pointer++ = '\t';
        }
        output_buffer->offset += length;

        // print value
        if (!print_value(current_item, output_buffer)) {
            return yangfalse;
        }
        update_offset(output_buffer);

        // print comma if not last
        length = ((size_t)(output_buffer->format ? 1 : 0) + (size_t)(current_item->next ? 1 : 0));
        output_pointer = ensure(output_buffer, length + 1);
        if (output_pointer == NULL) {
            return yangfalse;
        }
        if (current_item->next) {
            *output_pointer++ = ',';
        }

        if (output_buffer->format) {
            *output_pointer++ = '\n';
        }
        *output_pointer = '\0';
        output_buffer->offset += length;

        current_item = current_item->next;
    }

    output_pointer = ensure(output_buffer, output_buffer->format ? (output_buffer->depth + 1) : 2);
    if (output_pointer == NULL) {
        return yangfalse;
    }
    if (output_buffer->format) {
        size_t i;
        for (i = 0; i < (output_buffer->depth - 1); i++) {
            *output_pointer++ = '\t';
        }
    }
    *output_pointer++ = '}';
    *output_pointer = '\0';
    output_buffer->depth--;

    return yangtrue;
}

int yang_json_getArraySize(const YangJson* array) {
    YangJson* child = NULL;
    size_t size = 0;

    if (array == NULL) {
        return 0;
    }

    child = array->child;

    while (child != NULL) {
        size++;
        child = child->next;
    }

    return (int)size;
}

static YangJson* get_array_item(const YangJson* array, size_t index) {
    YangJson* current_child = NULL;

    if (array == NULL) {
        return NULL;
    }

    current_child = array->child;
    while ((current_child != NULL) && (index > 0)) {
        index--;
        current_child = current_child->next;
    }

    return current_child;
}

YangJson* yang_json_getArrayItem(const YangJson* array, int index) {
    if (index < 0) {
        return NULL;
    }

    return get_array_item(array, (size_t)index);
}

static YangJson* get_object_item(const YangJson* const object, const char* const name, const yangbool case_sensitive) {
    YangJson* current_element = NULL;

    if ((object == NULL) || (name == NULL)) {
        return NULL;
    }

    current_element = object->child;
    if (case_sensitive) {
        while ((current_element != NULL) && (current_element->string != NULL) && (strcmp(name, current_element->string) != 0)) {
            current_element = current_element->next;
        }
    } else {
        while ((current_element != NULL) && (yang_json_case_insensitive_strcmp((const uint8_t*)name, (const uint8_t*)(current_element->string)) != 0)) {
            current_element = current_element->next;
        }
    }

    if ((current_element == NULL) || (current_element->string == NULL)) {
        return NULL;
    }

    return current_element;
}

YangJson* yang_json_getObjectItem(const YangJson* const object, const char* const string) {
    return get_object_item(object, string, yangfalse);
}

YangJson* yang_json_getObjectItemCaseSensitive(const YangJson* const object, const char* const string) {
    return get_object_item(object, string, yangtrue);
}

yangbool yang_json_hasObjectItem(const YangJson* object, const char* string) {
    return yang_json_getObjectItem(object, string) ? 1 : 0;
}

static void suffix_object(YangJson* prev, YangJson* item) {
    prev->next = item;
    item->prev = prev;
}

static YangJson* create_reference(const YangJson* item, const internal_hooks* const hooks) {
    YangJson* reference = NULL;
    if (item == NULL) {
        return NULL;
    }

    reference = yang_json_new_item(hooks);
    if (reference == NULL) {
        return NULL;
    }

    memcpy(reference, item, sizeof(YangJson));
    reference->string = NULL;
    reference->type |= Yang_JSON_IsReference;
    reference->next = reference->prev = NULL;
    return reference;
}

static yangbool add_item_to_array(YangJson* array, YangJson* item) {
    YangJson* child = NULL;

    if ((item == NULL) || (array == NULL) || (array == item)) {
        return yangfalse;
    }

    child = array->child;

    if (child == NULL) {
        array->child = item;
        item->prev = item;
        item->next = NULL;
    } else {
        if (child->prev) {
            suffix_object(child->prev, item);
            array->child->prev = item;
        }
    }

    return yangtrue;
}

yangbool yang_json_addItemToArray(YangJson* array, YangJson* item) {
    return add_item_to_array(array, item);
}

static void* cast_away_const(const void* string) {
    return (void*)string;
}

static yangbool add_item_to_object(YangJson* const object, const char* const string, YangJson* const item, const internal_hooks* const hooks, const yangbool constant_key) {
    char* new_key = NULL;
    int new_type = Yang_JSON_Invalid;

    if ((object == NULL) || (string == NULL) || (item == NULL) || (object == item)) {
        return yangfalse;
    }

    if (constant_key) {
        new_key = (char*)cast_away_const(string);
        new_type = item->type | Yang_JSON_StringIsConst;
    } else {
        new_key = (char*)yang_json_strdup((const uint8_t*)string, hooks);
        if (new_key == NULL) {
            return yangfalse;
        }

        new_type = item->type & ~Yang_JSON_StringIsConst;
    }

    if (!(item->type & Yang_JSON_StringIsConst) && (item->string != NULL)) {
        hooks->deallocate(item->string);
    }

    item->string = new_key;
    item->type = new_type;

    return add_item_to_array(object, item);
}

yangbool yang_json_addItemToObject(YangJson* object, const char* string, YangJson* item) {
    return add_item_to_object(object, string, item, &global_hooks, yangfalse);
}

yangbool yang_json_addItemToObjectCS(YangJson* object, const char* string, YangJson* item) {
    return add_item_to_object(object, string, item, &global_hooks, yangtrue);
}

yangbool yang_json_addItemReferenceToArray(YangJson* array, YangJson* item) {
    if (array == NULL) {
        return yangfalse;
    }

    return add_item_to_array(array, create_reference(item, &global_hooks));
}

yangbool yang_json_addItemReferenceToObject(YangJson* object, const char* string, YangJson* item) {
    if ((object == NULL) || (string == NULL)) {
        return yangfalse;
    }

    return add_item_to_object(object, string, create_reference(item, &global_hooks), &global_hooks, yangfalse);
}

YangJson* yang_json_addNullToObject(YangJson* const object, const char* const name) {
    YangJson* null = yang_json_createNull();
    if (add_item_to_object(object, name, null, &global_hooks, yangfalse)) {
        return null;
    }

    yang_json_delete(null);
    return NULL;
}

YangJson* yang_json_addTrueToObject(YangJson* const object, const char* const name) {
    YangJson* true_item = yang_json_createTrue();
    if (add_item_to_object(object, name, true_item, &global_hooks, yangfalse)) {
        return true_item;
    }

    yang_json_delete(true_item);
    return NULL;
}

YangJson* yang_json_addFalseToObject(YangJson* const object, const char* const name) {
    YangJson* false_item = yang_json_createFalse();
    if (add_item_to_object(object, name, false_item, &global_hooks, yangfalse)) {
        return false_item;
    }

    yang_json_delete(false_item);
    return NULL;
}

YangJson* yang_json_addBoolToObject(YangJson* const object, const char* const name, const yangbool boolean) {
    YangJson* bool_item = yang_json_createBool(boolean);
    if (add_item_to_object(object, name, bool_item, &global_hooks, yangfalse)) {
        return bool_item;
    }

    yang_json_delete(bool_item);
    return NULL;
}

YangJson* yang_json_addNumberToObject(YangJson* const object, const char* const name, const double number) {
    YangJson* number_item = yang_json_createNumber(number);
    if (add_item_to_object(object, name, number_item, &global_hooks, yangfalse)) {
        return number_item;
    }

    yang_json_delete(number_item);
    return NULL;
}

YangJson* yang_json_addStringToObject(YangJson* const object, const char* const name, const char* const string) {
    YangJson* string_item = yang_json_createString(string);
    if (add_item_to_object(object, name, string_item, &global_hooks, yangfalse)) {
        return string_item;
    }

    yang_json_delete(string_item);
    return NULL;
}

YangJson* yang_json_addRawToObject(YangJson* const object, const char* const name, const char* const raw) {
    YangJson* raw_item = yang_json_createRaw(raw);
    if (add_item_to_object(object, name, raw_item, &global_hooks, yangfalse)) {
        return raw_item;
    }

    yang_json_delete(raw_item);
    return NULL;
}

YangJson* yang_json_addObjectToObject(YangJson* const object, const char* const name) {
    YangJson* object_item = yang_json_createObject();
    if (add_item_to_object(object, name, object_item, &global_hooks, yangfalse)) {
        return object_item;
    }

    yang_json_delete(object_item);
    return NULL;
}

YangJson* yang_json_addArrayToObject(YangJson* const object, const char* const name) {
    YangJson* array = yang_json_createArray();
    if (add_item_to_object(object, name, array, &global_hooks, yangfalse)) {
        return array;
    }

    yang_json_delete(array);
    return NULL;
}

YangJson* yang_json_detachItemViaPointer(YangJson* parent, YangJson* const item) {
    if ((parent == NULL) || (item == NULL)) {
        return NULL;
    }

    if (item != parent->child) {
        item->prev->next = item->next;
    }
    if (item->next != NULL) {
        item->next->prev = item->prev;
    }

    if (item == parent->child) {
        parent->child = item->next;
    } else if (item->next == NULL) {
        parent->child->prev = item->prev;
    }

    item->prev = NULL;
    item->next = NULL;

    return item;
}

YangJson* yang_json_detachItemFromArray(YangJson* array, int which) {
    if (which < 0) {
        return NULL;
    }

    return yang_json_detachItemViaPointer(array, get_array_item(array, (size_t)which));
}

void yang_json_deleteItemFromArray(YangJson* array, int which) {
    yang_json_delete(yang_json_detachItemFromArray(array, which));
}

YangJson* yang_json_detachItemFromObject(YangJson* object, const char* string) {
    YangJson* to_detach = yang_json_getObjectItem(object, string);

    return yang_json_detachItemViaPointer(object, to_detach);
}

YangJson* yang_json_detachItemFromObjectCaseSensitive(YangJson* object, const char* string) {
    YangJson* to_detach = yang_json_getObjectItemCaseSensitive(object, string);

    return yang_json_detachItemViaPointer(object, to_detach);
}

void yang_json_deleteItemFromObject(YangJson* object, const char* string) {
    yang_json_delete(yang_json_detachItemFromObject(object, string));
}

void yang_json_deleteItemFromObjectCaseSensitive(YangJson* object, const char* string) {
    yang_json_delete(yang_json_detachItemFromObjectCaseSensitive(object, string));
}

yangbool yang_json_insertItemInArray(YangJson* array, int which, YangJson* newitem) {
    YangJson* after_inserted = NULL;

    if (which < 0) {
        return yangfalse;
    }

    after_inserted = get_array_item(array, (size_t)which);
    if (after_inserted == NULL) {
        return add_item_to_array(array, newitem);
    }

    newitem->next = after_inserted;
    newitem->prev = after_inserted->prev;
    after_inserted->prev = newitem;
    if (after_inserted == array->child) {
        array->child = newitem;
    } else {
        newitem->prev->next = newitem;
    }
    return yangtrue;
}

yangbool yang_json_replaceItemViaPointer(YangJson* const parent, YangJson* const item, YangJson* replacement) {
    if ((parent == NULL) || (replacement == NULL) || (item == NULL)) {
        return yangfalse;
    }

    if (replacement == item) {
        return yangtrue;
    }

    replacement->next = item->next;
    replacement->prev = item->prev;

    if (replacement->next != NULL) {
        replacement->next->prev = replacement;
    }
    if (parent->child == item) {
        if (parent->child->prev == parent->child) {
            replacement->prev = replacement;
        }
        parent->child = replacement;
    } else {
        if (replacement->prev != NULL) {
            replacement->prev->next = replacement;
        }
        if (replacement->next == NULL) {
            parent->child->prev = replacement;
        }
    }

    item->next = NULL;
    item->prev = NULL;
    yang_json_delete(item);

    return yangtrue;
}

yangbool yang_json_replaceItemInArray(YangJson* array, int which, YangJson* newitem) {
    if (which < 0) {
        return yangfalse;
    }

    return yang_json_replaceItemViaPointer(array, get_array_item(array, (size_t)which), newitem);
}

static yangbool replace_item_in_object(YangJson* object, const char* string, YangJson* replacement, yangbool case_sensitive) {
    if ((replacement == NULL) || (string == NULL)) {
        return yangfalse;
    }

    if (!(replacement->type & Yang_JSON_StringIsConst) && (replacement->string != NULL)) {
        yang_json_free(replacement->string);
    }
    replacement->string = (char*)yang_json_strdup((const uint8_t*)string, &global_hooks);
    replacement->type &= ~Yang_JSON_StringIsConst;

    return yang_json_replaceItemViaPointer(object, get_object_item(object, string, case_sensitive), replacement);
}

yangbool yang_json_replaceItemInObject(YangJson* object, const char* string, YangJson* newitem) {
    return replace_item_in_object(object, string, newitem, yangfalse);
}

yangbool yang_json_replaceItemInObjectCaseSensitive(YangJson* object, const char* string, YangJson* newitem) {
    return replace_item_in_object(object, string, newitem, yangtrue);
}

YangJson* yang_json_createNull(void) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_NULL;
    }

    return item;
}

YangJson* yang_json_createTrue(void) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_True;
    }

    return item;
}

YangJson* yang_json_createFalse(void) {
       if (item) {
        item->type = boolean ? Yang_JSON_True : Yang_JSON_False;
    }

    return item;
}

YangJson* yang_json_createNumber(double num) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_Number;
        item->valuedouble = num;

        // use saturation in case of overflow
        if (num >= INT_MAX) {
            item->valueint = INT_MAX;
        } else if (num <= (double)INT_MIN) {
            item->valueint = INT_MIN;
        } else {
            item->valueint = (int)num;
        }
    }

    return item;
}

YangJson* yang_json_createString(const char* string) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_String;
        item->valuestring = (char*)yang_json_strdup((const uint8_t*)string, &global_hooks);
        if (!item->valuestring) {
            yang_json_delete(item);
            return NULL;
        }
    }

    return item;
}

YangJson* yang_json_createRaw(const char* raw) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_Raw;
        item->valuestring = (char*)yang_json_strdup((const uint8_t*)raw, &global_hooks);
        if (!item->valuestring) {
            yang_json_delete(item);
            return NULL;
        }
    }

    return item;
}

YangJson* yang_json_createArray(void) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_Array;
    }

    return item;
}

YangJson* yang_json_createObject(void) {
    YangJson* item = yang_json_new_item(&global_hooks);
    if (item) {
        item->type = Yang_JSON_Object;
    }

    return item;
}

YangJson* yang_json_createIntArray(const int* numbers, int count) {
    size_t i = 0;
    YangJson* n = NULL;
    YangJson* p = NULL;
    YangJson* a = NULL;

    if ((count < 0) || (numbers == NULL)) {
        return NULL;
    }

    a = yang_json_createArray();

    for (i = 0; a && (i < (size_t)count); i++) {
        n = yang_json_createNumber(numbers[i]);
        if (!n) {
            yang_json_delete(a);
            return NULL;
        }
        if (!i) {
            a->child = n;
        } else {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

YangJson* yang_json_createFloatArray(const float* numbers, int count) {
    size_t i = 0;
    YangJson* n = NULL;
    YangJson* p = NULL;
    YangJson* a = NULL;

    if ((count < 0) || (numbers == NULL)) {
        return NULL;
    }

    a = yang_json_createArray();

    for (i = 0; a && (i < (size_t)count); i++) {
        n = yang_json_createNumber((double)numbers[i]);
        if (!n) {
            yang_json_delete(a);
            return NULL;
        }
        if (!i) {
            a->child = n;
        } else {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

YangJson* yang_json_createDoubleArray(const double* numbers, int count) {
    size_t i = 0;
    YangJson* n = NULL;
    YangJson* p = NULL;
    YangJson* a = NULL;

    if ((count < 0) || (numbers == NULL)) {
        return NULL;
    }

    a = yang_json_createArray();

    for (i = 0; a && (i < (size_t)count); i++) {
        n = yang_json_createNumber(numbers[i]);
        if (!n) {
            yang_json_delete(a);
            return NULL;
        }
        if (!i) {
            a->child = n;
        } else {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

YangJson* yang_json_createStringArray(const char* const* strings, int count) {
    size_t i = 0;
    YangJson* n = NULL;
    YangJson* p = NULL;
    YangJson* a = NULL;

    if ((count < 0) || (strings == NULL)) {
        return NULL;
    }

    a = yang_json_createArray();

    for (i = 0; a && (i < (size_t)count); i++) {
        n = yang_json_createString(strings[i]);
        if (!n) {
            yang_json_delete(a);
            return NULL;
        }
        if (!i) {
            a->child = n;
        } else {
            suffix_object(p, n);
        }
        p = n;
    }

    if (a && a->child) {
        a->child->prev = n;
    }

    return a;
}

YangJson* yang_json_duplicate(const YangJson* item, yangbool recurse) {
    YangJson* newitem = NULL;
    YangJson* child = NULL;
    YangJson* next = NULL;
    YangJson* newchild = NULL;

    if (!item) {
        goto fail;
    }

    newitem = yang_json_new_item(&global_hooks);
    if (!newitem) {
        goto fail;
    }

    newitem->type = item->type & (~Yang_JSON_IsReference);
    newitem->valueint = item->valueint;
    newitem->valuedouble = item->valuedouble;
    if (item->valuestring) {
        newitem->valuestring = (char*)yang_json_strdup((uint8_t*)item->valuestring, &global_hooks);
        if (!newitem->valuestring) {
            goto fail;
        }
    }
    if (item->string) {
        newitem->string = (item->type & Yang_JSON_StringIsConst) ? item->string : (char*)yang_json_strdup((uint8_t*)item->string, &global_hooks);
        if (!newitem->string) {
            goto fail;
        }
    }

    if (!recurse) {
        return newitem;
    }

    child = item->child;
    while (child != NULL) {
        newchild = yang_json_duplicate(child, yangtrue);
        if (!newchild) {
            goto fail;
        }
        if (next != NULL) {
            next->next = newchild;
            newchild->prev = next;
            next = newchild;
        } else {
            newitem->child = newchild;
            next = newchild;
        }
        child = child->next;
    }
    if (newitem && newitem->child) {
        newitem->child->prev = newchild;
    }

    return newitem;

fail:
    if (newitem != NULL) {
        yang_json_delete(newitem);
    }

    return NULL;
}

static void skip_oneline_comment(char** input) {
    *input += strlen("//");

    for (; (*input)[0] != '\0'; ++(*input)) {
        if ((*input)[0] == '\n') {
            *input += strlen("\n");
            return;
        }
    }
}

static void skip_multiline_comment(char** input) {
    *input += strlen("/*");

    for (; (*input)[0] != '\0'; ++(*input)) {
        if (((*input)[0] == '*') && ((*input)[1] == '/')) {
            *input += strlen("*/");
            return;
        }
    }
}

static void minify_string(char** input, char** output) {
    (*output)[0] = (*input)[0];
    *input += strlen("\"");
    *output += strlen("\"");

    for (; (*input)[0] != '\0'; (void)++(*input), ++(*output)) {
        (*output)[0] = (*input)[0];

        if ((*input)[0] == '\"') {
            (*output)[0] = '\"';
            *input += strlen("\"");
            *output += strlen("\"");
            return;
        } else if (((*input)[0] == '\\') && ((*input)[1] == '\"')) {
            (*output)[1] = (*input)[1];
            *input += strlen("\"");
            *output += strlen("\"");
        }
    }
}

void yang_json_minify(char* json) {
    char* into = json;

    if (json == NULL) {
        return;
    }

    while (json[0] != '\0') {
        switch (json[0]) {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                json++;
                break;

            case '/':
                if (json[1] == '/') {
                    skip_oneline_comment(&json);
                } else if (json[1] == '*') {
                    skip_multiline_comment(&json);
                } else {
                    json++;
                }
                break;

            case '\"':
                minify_string(&json, (char**)&into);
                break;

            default:
                into[0] = json[0];
                json++;
                into++;
        }
    }

    *into = '\0';
}

yangbool yang_json_isInvalid(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_Invalid;
}

yangbool yang_json_isFalse(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_False;
}

yangbool yang_json_isTrue(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xff) == Yang_JSON_True;
}

yangbool yang_json_isBool(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & (Yang_JSON_True | Yang_JSON_False)) != 0;
}

yangbool yang_json_isNull(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_NULL;
}

yangbool yang_json_isNumber(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_Number;
}

yangbool yang_json_isString(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_String;
}

yangbool yang_json_isArray(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_Array;
}

yangbool yang_json_isObject(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_Object;
}

yangbool yang_json_isRaw(const YangJson* const item) {
    if (item == NULL) {
        return yangfalse;
    }

    return (item->type & 0xFF) == Yang_JSON_Raw;
}

yangbool yang_json_compare(const YangJson* const a, const YangJson* const b, const yangbool case_sensitive) {
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF))) {
        return yangfalse;
    }

    switch (a->type & 0xFF) {
        case Yang_JSON_False:
        case Yang_JSON_True:
        case Yang_JSON_NULL:
        case Yang_JSON_Number:
        case Yang_JSON_String:
        case Yang_JSON_Raw:
        case Yang_JSON_Array:
        case Yang_JSON_Object:
            break;

        default:
            return yangfalse;
    }

    if (a == b) {
        return yangtrue;
    }

    switch (a->type & 0xFF) {
        case Yang_JSON_False:
        case Yang_JSON_True:
        case Yang_JSON_NULL:
            return yangtrue;

        case Yang_JSON_Number:
            if (compare_double(a->valuedouble, b->valuedouble)) {
                return yangtrue;
            }
            return yangfalse;

        case Yang_JSON_String:
        case Yang_JSON_Raw:
            if ((a->valuestring == NULL) || (b->valuestring == NULL)) {
                return yangfalse;
            }
            if (strcmp(a->valuestring, b->valuestring) == 0) {
                return yangtrue;
            }

            return yangfalse;

        case Yang_JSON_Array: {
            YangJson* a_element = a->child;
            YangJson* b_element = b->child;

            for (; (a_element != NULL) && (b_element != NULL);) {
                if (!yang_json_compare(a_element, b_element, case_sensitive)) {
                    return yangfalse;
                }

                a_element = a_element->next;
                b_element = b_element->next;
            }

            if (a_element != b_element) {
                return yangfalse;
            }

            return yangtrue;
        }

        case Yang_JSON_Object: {
            YangJson* a_element = NULL;
            YangJson* b_element = NULL;
            yang_json_arrayForEach(a_element, a) {
                b_element = get_object_item(b, a_element->string, case_sensitive);
                if (b_element == NULL) {
                    return yangfalse;
                }

                if (!yang_json_compare(a_element, b_element, case_sensitive)) {
                    return yangfalse;
                }
            }

            yang_json_arrayForEach(b_element, b) {
                a_element = get_object_item(a, b_element->string, case_sensitive);
                if (a_element == NULL) {
                    return yangfalse;
                }

                if (!yang_json_compare(b_element, a_element, case_sensitive)) {
                    return yangfalse;
                }
            }

            return yangtrue;
        }

        default:
            return yangfalse;
    }
}

void* yang_json_malloc(size_t size) {
    return global_hooks.allocate(size);
}

void yang_json_free(void* object) {
    global_hooks.deallocate(object);
}
#endif // Yang_Enable_Json

// By GST @Date
