/*
 * Copyright (c) 2011 runner.mei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "pdu.h"



static v8::Persistent<v8::Function> pdu_constructor;

void Pdu::Initialize(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);


    t->SetClassName(Pdu_symbol);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "close", Close);

    SNMP_SET_ACCESSOR(t, version);
    SNMP_SET_ACCESSOR(t, command)
    SNMP_SET_ACCESSOR(t, reqid)
    SNMP_SET_ACCESSOR(t, msgid)
    SNMP_SET_ACCESSOR(t, transid)
    SNMP_SET_ACCESSOR(t, sessid)
    SNMP_SET_ACCESSOR(t, errstat)
    SNMP_SET_ACCESSOR(t, errindex)
    SNMP_SET_ACCESSOR(t, time)
    SNMP_SET_ACCESSOR(t, flags)
    SNMP_SET_ACCESSOR(t, securityModel)
    SNMP_SET_ACCESSOR(t, securityLevel)
    SNMP_SET_ACCESSOR(t, msgParseModel)
    SNMP_SET_READONLY_ACCESSOR(t, tDomain)
    SNMP_SET_ACCESSOR(t, community)
    SNMP_SET_ACCESSOR(t, enterprise)
    SNMP_SET_ACCESSOR(t, trap_type)
    //SNMP_SET_ACCESSOR(t, agent_addr)
    SNMP_SET_ACCESSOR(t, specific_type)
    SNMP_SET_ACCESSOR(t, contextEngineID)
    SNMP_SET_ACCESSOR(t, contextName)
    SNMP_SET_ACCESSOR(t, securityEngineID)
    SNMP_SET_ACCESSOR(t, securityName)
    SNMP_SET_ACCESSOR(t, priority)
    SNMP_SET_ACCESSOR(t, range_subid)


    target->Set(Pdu_symbol, t->GetFunction());

    pdu_constructor = v8::Persistent<v8::Function>::New(t->GetFunction());

    target->Set(v8::String::NewSymbol("appendVariableList"),  v8::FunctionTemplate::New(AppendToVariableList)->GetFunction());
    target->Set(v8::String::NewSymbol("getVariableList"),  v8::FunctionTemplate::New(GetFromVariableList)->GetFunction());
    target->Set(v8::String::NewSymbol("clearVariableList"),  v8::FunctionTemplate::New(ClearVariableList)->GetFunction());

    target->Set(v8::String::NewSymbol("toNativePdu"),  v8::FunctionTemplate::New(toNativePdu)->GetFunction());
    target->Set(v8::String::NewSymbol("fromNativePdu"),  v8::FunctionTemplate::New(fromNativePdu)->GetFunction());
}


v8::Handle<v8::Value> Pdu::New(netsnmp_pdu* native) {
  v8::HandleScope scope;

  v8::Local<v8::Value> argv[1] = { v8::Integer::New(0) };
  v8::Local<v8::Object> instance = pdu_constructor->NewInstance(1, argv);
  UNWRAP(Pdu, wrap, instance);
  wrap->reset(native, false);
  return scope.Close(instance);
}

v8::Handle<v8::Value> Pdu::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(1 != args.Length()) {
        return ThrowError("Must pass a integer argument to constructor.");
    }
    v8::Handle<v8::Value> type = args[0];
    if(!type->IsNumber()) {
        return ThrowTypeError("arguments type must be integer");
    }

    Pdu* pdu = new Pdu(type->Int32Value());
    pdu->Wrap(args.This());
    return args.This();
}

v8::Handle<v8::Value> Pdu::fromNativeVB(const netsnmp_variable_list* vb) {

    v8::Handle<v8::Object> ret = v8::Object::New();
    ret->Set(oid_symbol, from_oid(vb->name, vb->name_length));
    ret->Set(type_symbol, from_uint8(vb->type));

    v8::Handle<v8::Value> exception;
    switch (vb->type) {
    case ASN_INTEGER:
    case ASN_UNSIGNED:
    case ASN_TIMETICKS:
    case ASN_COUNTER: {
        ret->Set(value_symbol, from_long(*(vb->val.integer)));
        break;
    }
    case ASN_OBJECT_ID:
    case ASN_PRIV_IMPLIED_OBJECT_ID:
    case ASN_PRIV_INCL_RANGE:
    case ASN_PRIV_EXCL_RANGE: {
        ret->Set(value_symbol, from_oid(vb->val.objid, vb->val_len));
        break;
    }
    case ASN_IPADDRESS: /* snmp_build_var_op treats IPADDR like a string */
    case ASN_PRIV_IMPLIED_OCTET_STR:
    case ASN_OCTET_STR:
    case ASN_BIT_STR:
    case ASN_OPAQUE:
    case ASN_NSAP: {
        ret->Set(value_symbol, from_ustring(vb->val.string, vb->val_len));
        break;
    }
    case SNMP_NOSUCHOBJECT:
    case SNMP_NOSUCHINSTANCE:
    case SNMP_ENDOFMIBVIEW:
    case ASN_NULL: {
        ret->Set(value_symbol, v8::Null());
        break;
    }
#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_I64:
    case ASN_OPAQUE_U64:
#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */
    case ASN_COUNTER64: {
        int64_t v = 0;
        memmove(&v, vb->val.counter64, vb->val_len);
        ret->Set(value_symbol, from_int64(v));
        break;
    }
#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT: {
        ret->Set(value_symbol, from_float(*(vb->val.floatVal)));
        break;
    }

    case ASN_OPAQUE_DOUBLE: {
        ret->Set(value_symbol, from_double(*(vb->val.doubleVal)));
        break;
    }

#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */

    default: {
        exception = v8::String::New("Internal error in type switching.");
        goto failure;
    }
    }

    return ret;
failure:
    return v8::ThrowException(exception);
}

bool Pdu::convertValue(netsnmp_variable_list* vars
                       , u_char type
                       , v8::Handle<v8::Value>& value
                       , v8::Handle<v8::Value>& exception) {

    switch (type) {
    case ASN_INTEGER:
    case ASN_UNSIGNED:
    case ASN_TIMETICKS:
    case ASN_COUNTER: {
        if (ASN_INTEGER == type) {
            *(vars->val.integer) = value->Int32Value();
        } else {
            *(vars->val.integer) = value->Uint32Value();
        }
        vars->val_len = 4;
        break;
    }
    case ASN_OBJECT_ID:
    case ASN_PRIV_IMPLIED_OBJECT_ID:
    case ASN_PRIV_INCL_RANGE:
    case ASN_PRIV_EXCL_RANGE: {
        size_t name_len = sizeof(vars->buf);
        oid* name = to_oid(value, vars->val.objid, &name_len);
        if(NULL == name) {
            exception = v8::String::New("argument value must be a int array.");
            goto failure;
        }

        vars->val.objid = name;
        vars->val_len = name_len;
        break;
    }
    case ASN_IPADDRESS: /* snmp_build_var_op treats IPADDR like a string */

        /** FALL THROUGH */
    case ASN_PRIV_IMPLIED_OCTET_STR:
    case ASN_OCTET_STR:
    case ASN_BIT_STR:
    case ASN_OPAQUE:
    case ASN_NSAP: {
        v8::String::Utf8Value u8(value->ToString());
        vars->val_len = u8.length();
        if (ASN_IPADDRESS == type && 4 != vars->val_len) {
            exception = v8::String::New("ipaddress length must be equal 4.");
            goto failure;
        }

        if (sizeof(vars->buf) < vars->val_len) {
            vars->val.string = (u_char *) malloc(vars->val_len + 1);
        }
        if (NULL == vars->val.string) {
            exception = v8::String::New("no storage for string.");
            goto failure;
        }
        memmove(vars->val.string, *u8, vars->val_len);
        /*
        * Make sure the string is zero-terminated; some bits of code make
        * this assumption.  Easier to do this here than fix all these wrong
        * assumptions.
        */
        vars->val.string[vars->val_len] = '\0';
        break;
    }
    case SNMP_NOSUCHOBJECT:
    case SNMP_NOSUCHINSTANCE:
    case SNMP_ENDOFMIBVIEW:
    case ASN_NULL:
        vars->val_len = 0;
        vars->val.string = NULL;
        break;

#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_I64:
    case ASN_OPAQUE_U64:
#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */
    case ASN_COUNTER64: {
        int64_t v = to_int64(value);
        vars->val_len = sizeof(int64_t);
        memmove(vars->val.counter64, &v, vars->val_len);
        break;
    }
#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT: {
        (*vars->val.floatVal) = to_float(value);
        vars->val_len = sizeof(float);
        break;
    }
    case ASN_OPAQUE_DOUBLE: {
        (*vars->val.doubleVal) = to_double(value);
        vars->val_len = sizeof(double);
        break;
    }

#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */
    default: {
        exception = v8::String::New("Internal error in type switching.");
        goto failure;
    }
    }

    return true;
failure:
    return false;
}

v8::Handle<v8::Value> Pdu::AppendVB(netsnmp_pdu* pdu
                                    , v8::Handle<v8::Value>& hOid
                                    , v8::Handle<v8::Value>& hType
                                    , v8::Handle<v8::Value>& value) {

    oid     name_loc[MAX_OID_LEN];

    size_t  name_len = sizeof(name_loc);
    oid*    name = to_oid(hOid, name_loc, &name_len);
    if(NULL == name) {
        return ThrowTypeError("argument name must be oid.");
    }

    u_char type = to_uint8(hType);
    if('=' == type) {
        v8::String::Utf8Value u8(value->ToString());
        int ret = snmp_add_var(pdu, name, name_len, type,  *u8);

        if(name != name_loc) {
            free(name);
        }

        if(0 != ret) {
            std::string err = "argument value style error - ";
            err.append(snmp_api_errstring(ret));
            return ThrowSyntaxError(err.c_str());
        }
        return v8::Undefined();
    }

    netsnmp_variable_list *vars = 0;
    vars = snmp_varlist_add_variable(&vars
                                     , name, name_len, type, NULL, 0);
    if(name != name_loc) {
        free(name);
    }
    if(0 == vars) {
        return ThrowError("memory allocate error.");
    }

    v8::Handle<v8::Value> ex;
    if(!convertValue(vars
                     , type
                     , value
                     , ex)) {

        if(0 != vars) {
            snmp_free_var(vars);
        }
        return v8::ThrowException(ex);
    }

    if (0 == pdu->variables) {
        pdu->variables = vars;
    } else {
        netsnmp_variable_list* vtmp = pdu->variables;
        for (; vtmp->next_variable;
                vtmp = vtmp->next_variable);

        vtmp->next_variable = vars;
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> Pdu::AppendToVariableList(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(3 > args.Length()) {
        return ThrowError("Must pass pdu, name, value and option type arguments to set.");
    }
    if(4 < args.Length()) {
        return ThrowError("Arguments too much.");
    }

    v8::Handle<v8::Value> type = v8::Undefined();
    v8::Local<v8::Value> value;


    if(3 == args.Length()) {
        value = args[2];
    } else {
        type  = args[2];
        value = args[3];
    }

    UNWRAP(Pdu, wrap, args[0]->ToObject());
    return AppendVB(wrap->native_, args[1], type, value);
}


v8::Handle<v8::Value> Pdu::GetFromVariableList(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(2 != args.Length()) {
        return ThrowError("Must pass pdu, and index arguments to get.");
    }


    UNWRAP(Pdu, wrap, args[0]->ToObject());
    int idx = args[1]->Int32Value();

    netsnmp_variable_list* vtmp = wrap->native_->variables;
    for(int i = 0; i < idx; ++ i) {
        if(0 == vtmp) {
            return ThrowRangeError("Index too big.");
        }
        vtmp = vtmp->next_variable;
    }
    if(0 == vtmp) {
        return v8::Undefined();
    }

    return scope.Close(fromNativeVB(vtmp));
}
v8::Handle<v8::Value> Pdu::ClearVariableList(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(1 != args.Length()) {
        return ThrowError("Must pass pdu arguments to get.");
    }

    UNWRAP(Pdu, wrap, args[0]->ToObject());
    snmp_free_varbind(wrap->native_->variables);
    wrap->native_->variables = 0;
    return v8::Undefined();
}

v8::Handle<v8::Value> Pdu::Close(const v8::Arguments& args) {
    v8::HandleScope scope;
    UNWRAP(Pdu, wrap, args.This());
    wrap->close();
    return v8::Undefined();
}

v8::Handle<v8::Value> Pdu::fromPdu(const netsnmp_pdu *pdu) {

    v8::Handle<v8::Object> result = v8::Object::New();
    C2JS_LONG(pdu, result, version);
    C2JS_LONG(pdu, result, command);
    C2JS_LONG(pdu, result, reqid);
    C2JS_LONG(pdu, result, msgid);
    C2JS_LONG(pdu, result, transid);
    C2JS_LONG(pdu, result, sessid);

    C2JS_LONG(pdu, result, errstat);
    C2JS_LONG(pdu, result, errindex);

    C2JS_ULONG(pdu, result, time);
    C2JS_ULONG(pdu, result, flags);

    C2JS_INT32(pdu, result, securityModel);
    C2JS_INT32(pdu, result, securityLevel);
    C2JS_INT32(pdu, result, msgParseModel);



    if(SNMP_MSG_TRAP == pdu->command) {

        C2JS_OID(pdu, result, enterprise, enterprise_length);

        C2JS_LONG(pdu, result, trap_type);
        C2JS_LONG(pdu, result, specific_type);
        C2JS_IP4(pdu, result, agent_addr);
    }

    if(SNMP_VERSION_3 == pdu->version) {

        C2JS_USTRING(pdu, result, contextEngineID, contextEngineIDLen);
        C2JS_STRING(pdu, result, contextName, contextNameLen);

        C2JS_USTRING(pdu, result, securityEngineID, securityEngineIDLen);
        C2JS_STRING(pdu, result, securityName, securityNameLen);

    } else {
        C2JS_USTRING(pdu, result, community, community_len);
    }



    C2JS_INT32(pdu, result, priority);
    C2JS_INT32(pdu, result, range_subid);


    if(0 == pdu->variables) {
        return result;
    }


    v8::Handle<v8::Array> variableBindings = v8::Array::New();
    result->Set(variableBindings_symbol, variableBindings);

    int idx = 0;
    netsnmp_variable_list* vb = pdu->variables;
    while(0 != vb) {
        variableBindings->Set(idx, fromNativeVB(vb));

        ++ idx;
        vb = vb->next_variable;
    }

    return result;
}

v8::Handle<v8::Value> Pdu::toPdu(v8::Handle<v8::Value> v, netsnmp_pdu *pdu) {
    v8::Handle<v8::Object> args = v->ToObject();
    JS2C_LONG(pdu, args, version);
    JS2C_LONG(pdu, args, command);
    JS2C_LONG(pdu, args, reqid);
    JS2C_LONG(pdu, args, msgid);
    JS2C_LONG(pdu, args, transid);
    JS2C_LONG(pdu, args, sessid);
    JS2C_LONG(pdu, args, errstat);
    JS2C_LONG(pdu, args, errindex);
    JS2C_LONG(pdu, args, time);
    JS2C_LONG(pdu, args, flags);

    JS2C_INT32(pdu, args, securityModel);
    JS2C_INT32(pdu, args, securityLevel);
    JS2C_INT32(pdu, args, msgParseModel);


    if(SNMP_MSG_TRAP == pdu->command) {
        JS2C_OID(pdu, args, enterprise, enterprise_length);
        JS2C_LONG(pdu, args, trap_type);
        JS2C_LONG(pdu, args, specific_type);
        JS2C_IP4(pdu, args, agent_addr);
    }

    if(SNMP_VERSION_3 == pdu->version) {
        JS2C_USTRING(pdu, args, contextEngineID, contextEngineIDLen);
        JS2C_STRING(pdu, args, contextName, contextNameLen);
        JS2C_USTRING(pdu, args, securityEngineID, securityEngineIDLen);
        JS2C_STRING(pdu, args, securityName, securityNameLen);
    } else {
        JS2C_USTRING(pdu, args, community, community_len);
    }



    JS2C_INT32(pdu, args, priority);
    JS2C_INT32(pdu, args, range_subid);


    if(!args->Has(variableBindings_symbol)) {
        return v8::Undefined();
    }
    v8::Local<v8::Value> vl = args->Get(variableBindings_symbol);
    if(!vl->IsArray()) {
        return ThrowTypeError("Feild 'variableBindings' must be Array.");
    }

    v8::Handle<v8::Array> array = vl.As<v8::Array>();
    for(int i =0; i < array->Length(); ++ i) {
        v8::Handle<v8::Object> variableBinding = array->Get(i)->ToObject();

        v8::Handle<v8::Value> err = AppendVB(pdu, variableBinding->Get(oid_symbol)
                                             , variableBinding->Get(type_symbol)
                                             , variableBinding->Get(value_symbol));
        if(!err->IsUndefined()) {
            return err;
        }
    }

    return v8::Undefined();
}


/** this is function for test */
v8::Handle<v8::Value> Pdu::toNativePdu(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(1 != args.Length()) {
        return ThrowError("Must pass js object arguments to convert.");
    }
    v8::Handle<v8::Value> argv[] = { v8::Integer::New(SNMP_MSG_GET) };
    v8::Local<v8::Object> obj = pdu_constructor->NewInstance(1, argv);
    UNWRAP(Pdu, wrap, obj);
    v8::Handle<v8::Value> ret = toPdu(args[0], wrap->native_);
    if(ret->IsUndefined()) {
        return scope.Close(obj);
    }
    return ret;
}
/** this is function for test */
v8::Handle<v8::Value> Pdu::fromNativePdu(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(1 != args.Length()) {
        return ThrowError("Must pass js object arguments to convert.");
    }
    UNWRAP(Pdu, wrap, args[0]->ToObject());
    return scope.Close(fromPdu(wrap->native_));
}