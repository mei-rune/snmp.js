#ifndef _snmp_js_h
#define _snmp_js_h  1


#ifndef _WIN32_WINNT
# define _WIN32_WINNT   0x0502
#endif


#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <net-snmp/net-snmp-config.h>


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <node_version.h>

#include <net-snmp/utilities.h>
#include <net-snmp/net-snmp-includes.h>


#define ThrowError(x) v8::ThrowException(v8::Exception::Error(v8::String::New(x)))

#define ThrowRangeError(x) v8::ThrowException(v8::Exception::RangeError(v8::String::New(x)))
#define ThrowReferenceError(x) v8::ThrowException(v8::Exception::ReferenceError(v8::String::New(x)))
#define ThrowSyntaxError(x) v8::ThrowException(v8::Exception::SyntaxError(v8::String::New(x)))
#define ThrowTypeError(x) v8::ThrowException(v8::Exception::TypeError(v8::String::New(x)))


inline v8::Handle<v8::Value> newIntArray(int v1) {
    v8::Handle<v8::Array> result = v8::Array::New(1);
    result->Set(0, v8::Integer::New(v1));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2) {
    v8::Handle<v8::Array> result = v8::Array::New(2);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2, int v3) {
    v8::Handle<v8::Array> result = v8::Array::New(3);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    result->Set(2, v8::Integer::New(v3));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2
        , int v3, int v4) {
    v8::Handle<v8::Array> result = v8::Array::New(4);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    result->Set(2, v8::Integer::New(v3));
    result->Set(3, v8::Integer::New(v4));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2
        , int v3, int v4, int v5) {
    v8::Handle<v8::Array> result = v8::Array::New(5);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    result->Set(2, v8::Integer::New(v3));
    result->Set(3, v8::Integer::New(v4));
    result->Set(4, v8::Integer::New(v5));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2
        , int v3, int v4, int v5, int v6) {
    v8::Handle<v8::Array> result = v8::Array::New(6);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    result->Set(2, v8::Integer::New(v3));
    result->Set(3, v8::Integer::New(v4));
    result->Set(4, v8::Integer::New(v5));
    result->Set(5, v8::Integer::New(v6));
    return result;
}

inline v8::Handle<v8::Value> newIntArray(int v1, int v2
        , int v3, int v4, int v5, int v6, int v7) {
    v8::Handle<v8::Array> result = v8::Array::New(7);
    result->Set(0, v8::Integer::New(v1));
    result->Set(1, v8::Integer::New(v2));
    result->Set(2, v8::Integer::New(v3));
    result->Set(3, v8::Integer::New(v4));
    result->Set(4, v8::Integer::New(v5));
    result->Set(5, v8::Integer::New(v6));
    result->Set(6, v8::Integer::New(v7));
    return result;
}

inline v8::Handle<v8::Value> from_float(float value) {
    return v8::Number::New(value);
}

inline v8::Handle<v8::Value> from_double(double value) {
    return v8::Number::New(value);
}

inline v8::Handle<v8::Value> from_uint64(uint64_t value) {
    if(value > 0)
        return v8::Integer::NewFromUnsigned(value);
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_int64(int64_t value) {
    if(value > 0)
        return v8::Integer::NewFromUnsigned(value);
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_ulong(u_long value) {
    return v8::Integer::NewFromUnsigned(value);
}

inline v8::Handle<v8::Value> from_long(long value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_uint32(uint32_t value) {
    return v8::Integer::NewFromUnsigned(value);
}

inline v8::Handle<v8::Value> from_int32(int32_t value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_int8(int8_t value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_uint8(uint8_t value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_ustring(const u_char* value, size_t len) {
    return node::Buffer::New((char*)value, len)->handle_;
}
inline v8::Handle<v8::Value> from_string(const char* value, size_t len) {
    return v8::String::New((const char*)value, len);
}

inline v8::Handle<v8::Value> from_oid(const oid* name, size_t len) {
    if(0 == name) {
        return v8::Null();
    }
    v8::Handle<v8::Array> ret = v8::Array::New(len);
    for(size_t i = 0; i < len; ++ i) {
        ret->Set(i, v8::Int32::New(name[i]));
    }
    return ret;
}

inline int8_t to_int8(v8::Handle<v8::Value>& obj, int8_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (int8_t)obj->Int32Value();
}

inline uint8_t to_uint8(v8::Handle<v8::Value>& obj, uint8_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (uint8_t)obj->Int32Value();
}

inline int to_int32(v8::Handle<v8::Value>& obj, int defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->Int32Value();
}

inline int to_int32(v8::Handle<v8::Object>& obj, const char* key
                    , int defaultValue = 0) {
    return to_int32(obj->Get(v8::String::New(key)), defaultValue);
}

inline long to_long(v8::Handle<v8::Value>& obj, long defaultValue = 0) {
    return to_int32(obj, defaultValue);
}

inline uint32_t to_uint32(v8::Handle<v8::Value>& obj, uint32_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->Uint32Value();
}

inline uint32_t to_uint32(v8::Handle<v8::Object>& obj, const char* key
                          , uint32_t defaultValue = 0) {
    return to_uint32(obj->Get(v8::String::New(key)), defaultValue);
}

inline u_long to_ulong(v8::Handle<v8::Value>& obj, u_long defaultValue = 0) {
    return to_uint32(obj, defaultValue);
}

inline int64_t to_int64(v8::Handle<v8::Value>& obj, int64_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->IntegerValue();
}

inline int64_t to_int64(v8::Handle<v8::Object>& obj, const char* key
                        , int64_t defaultValue = 0) {
    return to_int64(obj->Get(v8::String::New(key)), defaultValue);
}

inline uint64_t to_uint64(v8::Handle<v8::Value>& obj, uint64_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->IntegerValue();
}

inline uint64_t to_uint64(v8::Handle<v8::Object>& obj, const char* key
                          , uint64_t defaultValue = 0) {
    return to_uint64(obj->Get(v8::String::New(key)), defaultValue);
}

inline float to_float(v8::Handle<v8::Value>& obj, float defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->NumberValue();
}

inline double to_double(v8::Handle<v8::Value>& obj, double defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->NumberValue();
}

inline oid* to_oid(v8::Handle<v8::Value>& s, oid* out, size_t* len) {
    if(s->IsString()) {
        v8::String::Utf8Value u8(s->ToString());
        oid* buf = out;
        if(0 == (*len)) {
            buf = (oid*)malloc(sizeof(oid) * MAX_OID_LEN);
            *len = MAX_OID_LEN;
        }
        while(NULL == snmp_parse_oid(*u8, buf, len)) {
            if(buf != out) {
                free(buf);
            }

            if((*len) > MAX_OID_LEN) {
                return 0;
            }

            (*len) *= 2;
            buf = (oid*)malloc(sizeof(oid) * *len);
        }
        return buf;
    } else if(s->IsArray()) {
        v8::Handle<v8::Array> ints = v8::Handle<v8::Array>::Cast(s);
        if(ints->Length() > *len) {
            out = (oid*)malloc(sizeof(oid) * ints->Length() + 1);
        }

        (*len) = ints->Length();

        for(int i =0; i < (*len); ++ i) {
            out[i] = ints->Get(i)->Int32Value();
        }
        return out;
    } else {
        return 0;
    }
}

#define UNWRAP(type, wrap, ref) type* wrap = ObjectWrap::Unwrap<type>(ref)


#define SNMP_DEFINE_CONSTANT_VALUE(target, constant_name, constant_value)         \
  (target)->Set(v8::String::NewSymbol(#constant_name),                            \
                constant_value,                                                   \
                static_cast<v8::PropertyAttribute>(                               \
                    v8::ReadOnly|v8::DontDelete))


#define SNMP_DEFINE_SYMBOL(name) static v8::Persistent<v8::String> name##_symbol;



#define C2JS_LONG(pdu, obj, name) obj->Set(name##_symbol, from_long(pdu->name))
#define JS2C_LONG(pdu, obj, name) if(obj->Has(name##_symbol)) { pdu->name = to_long(obj->Get(name##_symbol)); }
#define C2JS_ULONG(pdu, obj, name) obj->Set(name##_symbol, from_ulong(pdu->name))
#define JS2C_ULONG(pdu, obj, name) if(obj->Has(name##_symbol)) { pdu->name = to_ulong(obj->Get(name##_symbol)); }
#define C2JS_INT32(pdu, obj, name) obj->Set(name##_symbol, from_int32(pdu->name))
#define JS2C_INT32(pdu, obj, name) if(obj->Has(name##_symbol)) { pdu->name = to_int32(obj->Get(name##_symbol)); }


#define C2JS_USTRING(pdu, obj, name, len) if(0 < pdu->len) {                        \
			obj->Set(name##_symbol, from_ustring(pdu->name, pdu->len));             \
		}

#define COPY_BYTES(char_type, pdu, name, len, data, data_len)                       \
			if(pdu->len < data_len) {                                               \
				if(0 != pdu->name) { free(pdu->name); }                             \
				pdu->name = (char_type*)malloc(data_len);                           \
			}                                                                       \
			pdu->len = data_len;                                                    \
			memcpy(pdu->name, data, pdu->len);                                      \
 
#define JS2C_USTRING(pdu, obj, name, len) if(obj->Has(name##_symbol)) { 		    \
			v8::Handle<v8::Value> aa_value = obj->Get(name##_symbol);               \
		    if(node::Buffer::HasInstance(aa_value->ToObject())) {                   \
		        UNWRAP(node::Buffer, buffer, aa_value->ToObject());                 \
				COPY_BYTES(u_char, pdu, name, len, node::Buffer::Data(buffer)       \
                                    , node::Buffer::Length(buffer));                \
			} else {                                                                \
	            v8::String::Utf8Value u8(aa_value->ToString());                     \
				COPY_BYTES(u_char, pdu, name, len, *u8, u8.length());               \
			}                                                                       \
		}

#define C2JS_STRING(pdu, obj, name, len) if(0 < pdu->len) {                         \
			obj->Set(name##_symbol, from_string(pdu->name, pdu->len));              \
		}

#define JS2C_STRING(pdu, obj, name, len) if(obj->Has(name##_symbol)) { 				\
	        v8::String::Utf8Value u8(obj->Get(name##_symbol)->ToString());          \
			if(pdu->len < u8.length()) {                                            \
				if(0 != pdu->name) { free(pdu->name); }                             \
				pdu->name = (char*)malloc(u8.length());                             \
			}                                                                       \
			pdu->len = u8.length();                                                 \
			memcpy(pdu->name,  *u8, pdu->len);                                     \
		}

#define C2JS_OID(pdu, obj, name, len) if(0 < pdu->len) {                            \
			obj->Set(name##_symbol, from_oid(pdu->name, pdu->len));                 \
		}

#define JS2C_OID(pdu, obj, name, len) if(obj->Has(name##_symbol)) { 				\
	        oid* new_val = to_oid(obj->Get(name##_symbol), pdu->name, &pdu->len);   \
			if(NULL == new_val) {                                                   \
				return ThrowTypeError("argument " #name " must be oid.");           \
			}                                                                       \
	        if(new_val != pdu->name) {                                              \
				if(0 != pdu->name) { free(pdu->name); }                             \
				pdu->name = new_val;                                                \
	        }                                                                       \
		}


#define C2JS_IP4(pdu, obj, name)

#define JS2C_IP4(pdu, obj, name)



#define SNMP_SET_ACCESSOR(target, name)                                           \
    name##_symbol = NODE_PSYMBOL(#name);                                          \
	target->PrototypeTemplate()->SetAccessor(name##_symbol                        \
	                                            , Get_##name, Set_##name);


#define SNMP_SET_READONLY_ACCESSOR(target, name)                                  \
    name##_symbol = NODE_PSYMBOL(#name);                                          \
	target->PrototypeTemplate()->SetAccessor(name##_symbol, Get_##name);

#define SNMP_ACCESSOR_DEFINE_GET(this_type, value_type,  name)                    \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                        , const v8::AccessorInfo& info) {         \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_##value_type(wrap->native_->name));               \
	}


#define SNMP_ACCESSOR_DEFINE_SET(this_type, value_type,  name)                    \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {          \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
        wrap->native_->name = to_##value_type(value);                             \
	}


#define SNMP_ACCESSOR_DEFINE_GET_OID(this_type, name, len)                        \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                        , const v8::AccessorInfo& info) {         \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_oid(wrap->native_->name, wrap->native_->len));    \
	}


#define SNMP_ACCESSOR_DEFINE_SET_OID(this_type, name, len)                        \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {          \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		oid* new_val = to_oid(value, wrap->native_->name, &(wrap->native_->len)); \
		if(NULL == new_val) {                                                     \
			return; /* ThrowTypeError("argument name must be oid."); */           \
		}                                                                         \
	    if(new_val != wrap->native_->name) {                                      \
			if(0 != wrap->native_->name) { free(wrap->native_->name); }           \
			wrap->native_->name = new_val;                                        \
	    }                                                                         \
	}


#define SNMP_ACCESSOR_DEFINE_GET_STRING(char_type, this_type, name, len)          \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                           , const v8::AccessorInfo& info) {      \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_string(wrap->native_->name                        \
		                                               , wrap->native_->len));    \
	}


#define SNMP_ACCESSOR_DEFINE_SET_STRING(char_type, this_type, name, len)          \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                  , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		v8::String::Utf8Value u8(value->ToString());                              \
		COPY_BYTES(char, wrap->native_, name, len, *u8, u8.length());             \
	}



#define SNMP_ACCESSOR_DEFINE_GET_USTRING(this_type, name, len)                    \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                              , const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                                    \
		UNWRAP(Pdu, wrap, info.This());                                           \
		return scope.Close(from_ustring(wrap->native_->name                       \
		                                , wrap->native_->len));                   \
	}

#define SNMP_ACCESSOR_DEFINE_SET_USTRING(this_type, name, len)                    \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                   , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {  \
		v8::HandleScope scope;                                                    \
		UNWRAP(Pdu, wrap, info.This());                                           \
		                                                                          \
		if(node::Buffer::HasInstance(value->ToObject())) {                        \
		    UNWRAP(node::Buffer, buffer, value->ToObject());                      \
			COPY_BYTES(u_char, wrap->native_                                      \
                     , name, len, node::Buffer::Data(buffer)                      \
                     , node::Buffer::Length(buffer));                             \
		} else {                                                                  \
	        v8::String::Utf8Value u8(value->ToString());                          \
			COPY_BYTES(u_char, wrap->native_, name, len, *u8, u8.length());       \
		}                                                                         \
	}

#define SNMP_ACCESSOR_DEFINE(this_type, value_type,  name)                        \
  SNMP_ACCESSOR_DEFINE_SET(this_type, value_type,  name)                          \
  SNMP_ACCESSOR_DEFINE_GET(this_type, value_type,  name)


#define SNMP_ACCESSOR_DEFINE_OID(this_type, name, len)                            \
  SNMP_ACCESSOR_DEFINE_SET_OID(this_type, name, len)                              \
  SNMP_ACCESSOR_DEFINE_GET_OID(this_type, name, len)


#define SNMP_ACCESSOR_DEFINE_USTRING(this_type, name, len)                        \
  SNMP_ACCESSOR_DEFINE_SET_USTRING(this_type, name, len)                          \
  SNMP_ACCESSOR_DEFINE_GET_USTRING(this_type, name, len)


#define SNMP_ACCESSOR_DEFINE_STRING(this_type, name, len)                         \
  SNMP_ACCESSOR_DEFINE_SET_STRING(char, this_type, name, len)                     \
  SNMP_ACCESSOR_DEFINE_GET_STRING(char, this_type, name, len)



#define SNMP_ACCESSOR_DEFINE_SET_IP4(this_type, name)
#define SNMP_ACCESSOR_DEFINE_GET_IP4(this_type, name)

#define SNMP_ACCESSOR_DEFINE_IP4(this_type, name)                           \
  SNMP_ACCESSOR_DEFINE_SET_IP4(this_type, name)                             \
  SNMP_ACCESSOR_DEFINE_GET_IP4(this_type, name)


#endif // _snmp_js_h