#ifndef __pdu__cpp__
#define __pdu__cpp__ 1

#include "snmp.h"



static v8::Persistent<v8::Function> constructor;

class Pdu : node::ObjectWrap {

private:
    netsnmp_pdu* native_;

    Pdu(int type) : native_(0) {
        native_ = snmp_pdu_create(type);
    }

    void close() {
        if (NULL != native_) {
            snmp_free_pdu(native_);
            native_ = NULL;
        }
    }

public:

    virtual ~Pdu() {
        close();
    }


    netsnmp_pdu* native() {
        return native_;
    }

    static void Initialize(v8::Handle<v8::Object> target) {
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

        constructor = v8::Persistent<v8::Function>::New(t->GetFunction());

        target->Set(v8::String::NewSymbol("appendVariableList"),  v8::FunctionTemplate::New(Append)->GetFunction());
        target->Set(v8::String::NewSymbol("getVariableList"),  v8::FunctionTemplate::New(Get)->GetFunction());
        target->Set(v8::String::NewSymbol("clearVariableList"),  v8::FunctionTemplate::New(Clear)->GetFunction());

        target->Set(v8::String::NewSymbol("toNativePdu"),  v8::FunctionTemplate::New(toNativePdu)->GetFunction());
        target->Set(v8::String::NewSymbol("fromNativePdu"),  v8::FunctionTemplate::New(fromNativePdu)->GetFunction());
    }

    static v8::Handle<v8::Value> New(const v8::Arguments& args) {
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

    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, version)

    //static v8::Handle<v8::Value> Get_version(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //	return scope.Close(v8::Int32::New(wrap->native_->version));
    //}

    //static void Set_version(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //  wrap->native_->version = value->Int32Value();
    //}

    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, command)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, reqid)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, msgid)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, transid)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, sessid)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, errstat)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, errindex)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, ulong, time)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, ulong, flags)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, securityModel)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, securityLevel)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, msgParseModel)
    SNMP_ACCESSOR_DEFINE_GET_OID(Pdu, native_->, tDomain, tDomainLen)
    SNMP_ACCESSOR_DEFINE_USTRING(Pdu, native_->, community, community_len)


    //static v8::Handle<v8::Value> Get_community(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //	return node::Buffer::New((char*)wrap->native_->community, sizeof(u_char)*wrap->native_->community_len)->handle_;
    //}

    //static void Set_community(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //	UNWRAP(node::Buffer, buffer, value->ToObject());
    //
    //	if(0 == wrap->native_->community
    //	  || wrap->native_->community_len < node::Buffer::Length(buffer)) {
    //	    free(wrap->native_->community);
    //        wrap->native_->community = (u_char*)malloc(node::Buffer::Length(buffer) + 4);
    //	}

    //	memcpy(wrap->native_->community, node::Buffer::Data(buffer), node::Buffer::Length(buffer));
    //    wrap->native_->community_len = node::Buffer::Length(buffer);
    //}

    SNMP_ACCESSOR_DEFINE_OID(Pdu, native_->, enterprise, enterprise_length)

    //
    //static v8::Handle<v8::Value> Get_enterprise(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //	v8::Handle<v8::Array> ret = v8::Array::New(wrap->native_->enterprise_length);
//       for(size_t i = 0; i < wrap->native_->enterprise_length; ++ i) {
    //		ret->Set(i, v8::Int32::New(wrap->native_->enterprise[i]));
    //	}
    //	return scope.Close(ret);
    //}
    //
//   static void Set_enterprise(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, info.This());
    //	oid* new_value = wrap->native_->enterprise;
//       size_t new_len = wrap->native_->enterprise_length;
    //	new_value = to_oid(value, new_value, &new_len);
    //	if(new_value != wrap->native_->enterprise) free(wrap->native_->enterprise);
//       wrap->native_->enterprise = new_value;
//       wrap->native_->enterprise_length = new_len;
    //}

    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, trap_type)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, specific_type)

    SNMP_ACCESSOR_DEFINE_IP4(Pdu, native_->, agent_addr)

    SNMP_ACCESSOR_DEFINE_USTRING(Pdu, native_->, securityEngineID, securityEngineIDLen)
    SNMP_ACCESSOR_DEFINE_USTRING(Pdu, native_->, contextEngineID, contextEngineIDLen)
    SNMP_ACCESSOR_DEFINE_STRING(Pdu, native_->, contextName, contextNameLen)
    SNMP_ACCESSOR_DEFINE_STRING(Pdu, native_->, securityName, securityNameLen)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, priority)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, range_subid)

    //static v8::Handle<v8::Value> Get_version(const v8::Arguments& args){
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, args.This());
    //	return v8::Int32::New(wrap->native_->version);
    //}
    //
    //static v8::Handle<v8::Value> Set_version(const v8::Arguments& args){
    //	v8::HandleScope scope;
    //	UNWRAP(Pdu, wrap, args.This());
    //
    //	v8::Handle<v8::Value> value = args[0];
    //	if(!value->IsInt32()){
    //		return ThrowTypeError("arguments type must be integer");
    //	}

    //	wrap->native_->version = value->Int32Value();
    //	return v8::Undefined();
    //}


    static v8::Handle<v8::Value> fromNativeVB(const netsnmp_variable_list* vb) {

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

    static bool convertValue(netsnmp_variable_list* vars
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

    static v8::Handle<v8::Value> AppendVB(netsnmp_pdu* pdu
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

    static v8::Handle<v8::Value> Append(const v8::Arguments& args) {
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


    static v8::Handle<v8::Value> Get(const v8::Arguments& args) {
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
    static v8::Handle<v8::Value> Clear(const v8::Arguments& args) {
        v8::HandleScope scope;
        if(1 != args.Length()) {
            return ThrowError("Must pass pdu arguments to get.");
        }

        UNWRAP(Pdu, wrap, args[0]->ToObject());
        snmp_free_varbind(wrap->native_->variables);
        wrap->native_->variables = 0;
        return v8::Undefined();
    }

    static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
        v8::HandleScope scope;
        UNWRAP(Pdu, wrap, args.This());
        wrap->close();
        return v8::Undefined();
    }

    static v8::Handle<v8::Value> fromPdu(const netsnmp_pdu *pdu) {

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

    static v8::Handle<v8::Value> toPdu(v8::Handle<v8::Value> v, netsnmp_pdu *pdu) {
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
    static v8::Handle<v8::Value> toNativePdu(const v8::Arguments& args) {
        v8::HandleScope scope;
        if(1 != args.Length()) {
            return ThrowError("Must pass js object arguments to convert.");
        }
        v8::Handle<v8::Value> argv[] = { v8::Integer::New(SNMP_MSG_GET) };
        v8::Local<v8::Object> obj = constructor->NewInstance(1, argv);
        UNWRAP(Pdu, wrap, obj);
        v8::Handle<v8::Value> ret = toPdu(args[0], wrap->native_);
        if(ret->IsUndefined()) {
            return scope.Close(obj);
        }
        return ret;
    }
    /** this is function for test */
    static v8::Handle<v8::Value> fromNativePdu(const v8::Arguments& args) {
        v8::HandleScope scope;
        if(1 != args.Length()) {
            return ThrowError("Must pass js object arguments to convert.");
        }
        UNWRAP(Pdu, wrap, args[0]->ToObject());
        return scope.Close(fromPdu(wrap->native_));
    }

};

#endif //__pdu__cpp__ 