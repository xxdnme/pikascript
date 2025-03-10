#include "PikaStdData_Dict.h"
#include "BaseObj.h"
#include "PikaObj.h"
#include "PikaStdData_Tuple.h"
#include "PikaStdData_dict_items.h"
#include "PikaStdData_dict_keys.h"
#include "PikaStdLib_SysObj.h"
#include "dataStrs.h"

Arg* PikaStdData_Dict_get(PikaObj* self, char* key) {
    PikaDict* dict = obj_getPtr(self, "dict");
    Arg* res = dict_getArg(dict, key);
    if (NULL == res) {
        obj_setErrorCode(self, PIKA_RES_ERR_RUNTIME_ERROR);
        __platform_printf("KeyError: %s\n", key);
    }
    return arg_copy(res);
}

void PikaStdData_Dict___init__(PikaObj* self) {
    __vm_Dict___init__(self);
}

void PikaStdData_Dict_set(PikaObj* self, char* key, Arg* arg) {
    __vm_Dict_set(self, arg, key);
}

void PikaStdData_Dict_remove(PikaObj* self, char* key) {
    PikaDict* dict = obj_getPtr(self, "dict");
    PikaDict* keys = obj_getPtr(self, "_keys");
    dict_removeArg(dict, dict_getArg(dict, key));
    dict_removeArg(keys, dict_getArg(keys, key));
}

Arg* PikaStdData_Dict___iter__(PikaObj* self) {
    obj_setInt(self, "__iter_i", 0);
    return arg_newRef(self);
}

Arg* PikaStdData_Dict___next__(PikaObj* self) {
    int __iter_i = args_getInt(self->list, "__iter_i");
    PikaDict* keys = obj_getPtr(self, "_keys");
    Arg* res = arg_copy(args_getArgByidex(&keys->super, __iter_i));
    if (NULL == res) {
        return arg_newNull();
    }
    args_setInt(self->list, "__iter_i", __iter_i + 1);
    return res;
}

void PikaStdData_Dict___setitem__(PikaObj* self, Arg* __key, Arg* __val) {
    PikaStdData_Dict_set(self, obj_getStr(self, "__key"),
                         obj_getArg(self, "__val"));
}

Arg* PikaStdData_Dict___getitem__(PikaObj* self, Arg* __key) {
    return PikaStdData_Dict_get(self, obj_getStr(self, "__key"));
}

void PikaStdData_Dict___del__(PikaObj* self) {
    PikaDict* dict = obj_getPtr(self, "dict");
    PikaDict* keys = obj_getPtr(self, "_keys");
    dict_deinit(dict);
    if (NULL != keys) {
        dict_deinit(keys);
    }
}

void PikaStdData_dict_keys___init__(PikaObj* self, PikaObj* dict) {
    obj_setPtr(self, "dictptr", dict);
}

PikaObj* PikaStdData_Dict_keys(PikaObj* self) {
    PikaObj* dict_keys = newNormalObj(New_PikaStdData_dict_keys);
    PikaStdData_dict_keys___init__(dict_keys, self);
    return dict_keys;
}

PikaObj* PikaStdData_Dict_items(PikaObj* self) {
    PikaObj* dict_items = newNormalObj(New_PikaStdData_dict_items);
    PikaStdData_dict_keys___init__(dict_items, self);
    return dict_items;
}

Arg* PikaStdData_dict_keys___iter__(PikaObj* self) {
    obj_setInt(self, "__iter_i", 0);
    return arg_newRef(self);
}

Arg* PikaStdData_dict_keys___next__(PikaObj* self) {
    int __iter_i = args_getInt(self->list, "__iter_i");
    PikaObj* dictptr = obj_getPtr(self, "dictptr");
    PikaDict* keys = obj_getPtr(dictptr, "_keys");
    Arg* res = arg_copy(args_getArgByidex(&keys->super, __iter_i));
    if (NULL == res) {
        return arg_newNull();
    }
    args_setInt(self->list, "__iter_i", __iter_i + 1);
    return res;
}

char* PikaStdLib_SysObj_str(PikaObj* self, Arg* arg);
char* PikaStdData_dict_keys___str__(PikaObj* self) {
    Arg* str_arg = arg_newStr("dict_keys([");
    PikaObj* dictptr = obj_getPtr(self, "dictptr");
    PikaDict* keys = obj_getPtr(dictptr, "_keys");

    int i = 0;
    while (PIKA_TRUE) {
        Arg* item = args_getArgByidex(&keys->super, i);
        if (NULL == item) {
            break;
        }
        if (i != 0) {
            str_arg = arg_strAppend(str_arg, ", ");
        }
        char* item_str = PikaStdLib_SysObj_str(self, item);
        if (arg_getType(item) == ARG_TYPE_STRING) {
            str_arg = arg_strAppend(str_arg, "'");
        }
        str_arg = arg_strAppend(str_arg, item_str);
        if (arg_getType(item) == ARG_TYPE_STRING) {
            str_arg = arg_strAppend(str_arg, "'");
        }
        i++;
    }

    str_arg = arg_strAppend(str_arg, "])");
    obj_setStr(self, "_buf", arg_getStr(str_arg));
    arg_deinit(str_arg);
    return obj_getStr(self, "_buf");
}

char* PikaStdData_Dict___str__(PikaObj* self) {
    Arg* str_arg = arg_newStr("{");

    PikaDict* keys = obj_getPtr(self, "_keys");
    PikaDict* dict = obj_getPtr(self, "dict");

    int i = 0;
    while (PIKA_TRUE) {
        Arg* item_key = args_getArgByidex(&keys->super, i);
        Arg* item_val = args_getArgByidex(&dict->super, i);
        if (NULL == item_key) {
            break;
        }
        if (i != 0) {
            str_arg = arg_strAppend(str_arg, ", ");
        }
        char* key_str = PikaStdLib_SysObj_str(self, item_key);
        str_arg = arg_strAppend(str_arg, "'");
        str_arg = arg_strAppend(str_arg, key_str);
        str_arg = arg_strAppend(str_arg, "'");
        str_arg = arg_strAppend(str_arg, ": ");

        char* val_str = PikaStdLib_SysObj_str(self, item_val);
        if (arg_getType(item_val) == ARG_TYPE_STRING) {
            str_arg = arg_strAppend(str_arg, "'");
        }
        str_arg = arg_strAppend(str_arg, val_str);
        if (arg_getType(item_val) == ARG_TYPE_STRING) {
            str_arg = arg_strAppend(str_arg, "'");
        }
        i++;
    }

    str_arg = arg_strAppend(str_arg, "}");
    obj_setStr(self, "_buf", arg_getStr(str_arg));
    arg_deinit(str_arg);
    return obj_getStr(self, "_buf");
}

int PikaStdData_Dict___len__(PikaObj* self) {
    PikaDict* dict = obj_getPtr(self, "dict");
    return args_getSize(&dict->super);
}

int PikaStdData_dict_keys___len__(PikaObj* self) {
    PikaObj* dictptr = obj_getPtr(self, "dictptr");
    PikaDict* keys = obj_getPtr(dictptr, "_keys");
    return args_getSize(&keys->super);
}

int dict_contains(PikaDict* dict, Arg* key) {
    int i = 0;
    while (PIKA_TRUE) {
        Arg* item = args_getArgByidex(&dict->super, i);
        if (NULL == item) {
            break;
        }
        if (arg_isEqual(item, key)) {
            return PIKA_TRUE;
        }
        i++;
    }
    return PIKA_FALSE;
}

int PikaStdData_Dict___contains__(PikaObj* self, Arg* val) {
    PikaDict* dict = obj_getPtr(self, "_keys");
    return dict_contains(dict, val);
}

Arg* PikaStdData_dict_items___iter__(PikaObj* self) {
    obj_setInt(self, "__iter_i", 0);
    return arg_newRef(self);
}

int PikaStdData_dict_items___len__(PikaObj* self) {
    PikaObj* dictptr = obj_getPtr(self, "dictptr");
    PikaDict* keys = obj_getPtr(dictptr, "_keys");
    return args_getSize(&keys->super);
}

Arg* PikaStdData_dict_items___next__(PikaObj* self) {
    int __iter_i = args_getInt(self->list, "__iter_i");
    PikaObj* dictptr = obj_getPtr(self, "dictptr");
    PikaDict* keys = obj_getPtr(dictptr, "_keys");
    PikaDict* dict = obj_getPtr(dictptr, "dict");
    Arg* key = args_getArgByidex(&keys->super, __iter_i);
    Arg* val = args_getArgByidex(&dict->super, __iter_i);
    if (NULL == key) {
        return arg_newNull();
    }
    PikaObj* tuple = newNormalObj(New_PikaStdData_Tuple);
    PikaStdData_Tuple___init__(tuple);
    PikaList* list = obj_getPtr(tuple, "list");
    list_append(list, key);
    list_append(list, val);
    args_setInt(self->list, "__iter_i", __iter_i + 1);
    return arg_newObj(tuple);
}

char* PikaStdData_dict_items___str__(PikaObj* self) {
    Arg* str_arg = arg_newStr("dict_items([");
    int i = 0;
    obj_setInt(self, "__iter_i", 0);
    while (PIKA_TRUE) {
        Arg* item = PikaStdData_dict_items___next__(self);
        if (arg_getType(item) == ARG_TYPE_NONE) {
            arg_deinit(item);
            break;
        }
        if (i != 0) {
            str_arg = arg_strAppend(str_arg, ", ");
        }
        char* item_str = PikaStdLib_SysObj_str(self, item);
        str_arg = arg_strAppend(str_arg, item_str);
        i++;
        arg_deinit(item);
    }
    str_arg = arg_strAppend(str_arg, "])");
    obj_setStr(self, "_buf", arg_getStr(str_arg));
    arg_deinit(str_arg);
    return obj_getStr(self, "_buf");
}
