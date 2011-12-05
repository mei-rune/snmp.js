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

inline v8::Handle<v8::Value> from_uint16(uint16_t value) {
    return v8::Integer::NewFromUnsigned(value);
}

inline v8::Handle<v8::Value> from_int16(int16_t value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_ushort(uint16_t value) {
    return v8::Integer::NewFromUnsigned(value);
}

inline v8::Handle<v8::Value> from_int8(int8_t value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_uint8(uint8_t value) {
    return v8::Integer::New(value);
}
inline v8::Handle<v8::Value> from_uchar(u_char value) {
    return v8::Integer::New(value);
}

inline v8::Handle<v8::Value> from_ustring(const u_char* value, size_t len) {
    return node::Buffer::New((char*)value, len)->handle_;
}
inline v8::Handle<v8::Value> from_string(const char* value, size_t len) {
    return v8::String::New((const char*)value, len);
}


inline v8::Handle<v8::Value> from_size_t(size_t value) {
    return from_uint32(value);
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

inline u_char to_uchar(v8::Handle<v8::Value>& obj, u_char defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (u_char)obj->Int32Value();
}

inline uint8_t to_uint8(v8::Handle<v8::Value>& obj, uint8_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (uint8_t)obj->Int32Value();
}


inline int8_t to_int16(v8::Handle<v8::Value>& obj, int16_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (int8_t)obj->Int32Value();
}

inline uint8_t to_uint16(v8::Handle<v8::Value>& obj, uint16_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return (uint8_t)obj->Int32Value();
}
inline uint8_t to_ushort(v8::Handle<v8::Value>& obj, uint16_t defaultValue = 0) {
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

inline size_t to_size_t(v8::Handle<v8::Value>& obj, size_t defaultValue = 0) {
    if(!obj->IsNumber()) {
        return defaultValue;
    }
    return obj->IntegerValue();
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


#define SNMP_DECLARE_SYMBOL(name) extern v8::Persistent<v8::String> name##_symbol;
#define SNMP_DEFINE_SYMBOL(name) v8::Persistent<v8::String> name##_symbol;
#define SNMP_INIT_SYMBOL(name) name##_symbol = NODE_PSYMBOL(#name);



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
			memcpy(pdu->name,  *u8, pdu->len);                                      \
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
	target->PrototypeTemplate()->SetAccessor(name##_symbol                        \
	                                            , Get_##name, Set_##name);


#define SNMP_SET_READONLY_ACCESSOR(target, name)                                  \
	target->PrototypeTemplate()->SetAccessor(name##_symbol, Get_##name);

#define SNMP_ACCESSOR_DEFINE_GET(this_type, inner, value_type,  name)             \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                        , const v8::AccessorInfo& info) {         \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_##value_type(wrap->inner name));                  \
	}


#define SNMP_ACCESSOR_DEFINE_SET(this_type, inner, value_type,  name)             \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {          \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
        wrap->inner name = to_##value_type(value);                                \
	}


#define SNMP_ACCESSOR_DEFINE_GET_OID(this_type, inner, name, len)                 \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                        , const v8::AccessorInfo& info) {         \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_oid(wrap->inner name, wrap->inner len));          \
	}


#define SNMP_ACCESSOR_DEFINE_SET_OID(this_type, inner, name, len)                 \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {          \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		oid* new_val = to_oid(value, wrap->inner name, &(wrap->inner len));       \
		if(NULL == new_val) {                                                     \
			return; /* ThrowTypeError("argument name must be oid."); */           \
		}                                                                         \
	    if(new_val != wrap->inner name) {                                         \
			if(0 != wrap->inner name) { free(wrap->inner name); }                 \
			wrap->inner name = new_val;                                           \
	    }                                                                         \
	}


#define SNMP_ACCESSOR_DEFINE_GET_STRING(this_type, inner, name, len)              \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                           , const v8::AccessorInfo& info) {      \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_string(wrap->inner name  , wrap->inner len));     \
	}


#define SNMP_ACCESSOR_DEFINE_SET_STRING(this_type, inner, name, len)              \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                  , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		v8::String::Utf8Value u8(value->ToString());                              \
		COPY_BYTES(char, wrap, inner name, inner len, *u8, u8.length());          \
	}




#define SNMP_ACCESSOR_DEFINE_GET_STRING_LENLESS(this_type, inner, name)           \
    static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                           , const v8::AccessorInfo& info) {      \
        v8::HandleScope scope;                                                    \
        UNWRAP(this_type, wrap, info.This());                                     \
        return scope.Close(from_string(wrap->inner name                           \
                                              , strlen(wrap->inner name)));       \
    }


#define SNMP_ACCESSOR_DEFINE_SET_STRING_LENLESS(this_type, inner, name)           \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                  , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
        v8::HandleScope scope;                                                    \
        UNWRAP(this_type, wrap, info.This());                                     \
        v8::String::Utf8Value u8(value->ToString());                              \
        wrap->inner name = (char*)malloc(sizeof(char) * u8.length());             \
        memcpy(wrap->inner name, *u8, u8.length());                               \
        wrap->inner name[u8.length()] = 0;                                        \
    }


#define SNMP_ACCESSOR_DEFINE_GET_USTRING(this_type, inner, name, len)             \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                              , const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		return scope.Close(from_ustring(wrap->inner name                          \
		                                , wrap->inner len));                      \
	}

#define SNMP_ACCESSOR_DEFINE_SET_USTRING(this_type, inner, name, len)             \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                   , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {  \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                     \
		                                                                          \
		if(node::Buffer::HasInstance(value->ToObject())) {                        \
		    UNWRAP(node::Buffer, buffer, value->ToObject());                      \
			COPY_BYTES(u_char, wrap                                               \
                     , inner name, inner len, node::Buffer::Data(buffer)          \
                     , node::Buffer::Length(buffer));                             \
		} else {                                                                  \
	        v8::String::Utf8Value u8(value->ToString());                          \
			COPY_BYTES(u_char, wrap, inner name, inner len, *u8, u8.length());    \
		}                                                                         \
	}


#define SNMP_ACCESSOR_DEFINE_SET_FIXED_USTRING(this_type, inner, name, len, fixed) \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                   , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {  \
        v8::HandleScope scope;                                                    \
        UNWRAP(this_type, wrap, info.This());                                     \
                                                                                  \
        if(node::Buffer::HasInstance(value->ToObject())) {                        \
            UNWRAP(node::Buffer, buffer, value->ToObject());                      \
            if(fixed < node::Buffer::Length(buffer)) {                            \
 /* ThrowError("Argument '" #name "' too length, exceed limit '" #fixed "'");   */\
                wrap->inner len = fixed;                                          \
                memcpy(wrap->inner name, node::Buffer::Data(buffer), fixed);      \
				return;                                                           \
            }                                                                     \
            wrap->inner len = node::Buffer::Length(buffer);                       \
            memcpy(wrap->inner name, node::Buffer::Data(buffer)                   \
                                      , wrap->inner len);                         \
        } else {                                                                  \
            v8::String::Utf8Value u8(value->ToString());                          \
            if(fixed < u8.length()) {                                             \
 /* ThrowError("Argument '" #name "' too length, exceed limit '" #fixed "'");   */\
                wrap->inner len = fixed;                                          \
                memcpy(wrap->inner name, *u8, fixed);                             \
				return;                                                           \
            }                                                                     \
            wrap->inner len = u8.length();                                        \
            memcpy(wrap->inner name, *u8, wrap->inner len);                       \
        }                                                                         \
    }

#define SNMP_ACCESSOR_DEFINE(this_type, value_type, inner, name)                  \
  SNMP_ACCESSOR_DEFINE_SET(this_type, value_type, inner, name)                    \
  SNMP_ACCESSOR_DEFINE_GET(this_type, value_type, inner, name)


#define SNMP_ACCESSOR_DEFINE_OID(this_type, inner, name, len)                     \
  SNMP_ACCESSOR_DEFINE_SET_OID(this_type, inner, name, len)                       \
  SNMP_ACCESSOR_DEFINE_GET_OID(this_type, inner, name, len)


#define SNMP_ACCESSOR_DEFINE_USTRING(this_type, inner, name, len)                 \
  SNMP_ACCESSOR_DEFINE_SET_USTRING(this_type, inner, name, len)                   \
  SNMP_ACCESSOR_DEFINE_GET_USTRING(this_type, inner, name, len)


#define SNMP_ACCESSOR_DEFINE_STRING(this_type, inner, name, len)                  \
  SNMP_ACCESSOR_DEFINE_SET_STRING(this_type, inner, name, len)                    \
  SNMP_ACCESSOR_DEFINE_GET_STRING(this_type, inner, name, len)


#define SNMP_ACCESSOR_DEFINE_STRING_LENLESS(this_type, inner, name)               \
  SNMP_ACCESSOR_DEFINE_SET_STRING_LENLESS(this_type, inner, name)                 \
  SNMP_ACCESSOR_DEFINE_GET_STRING_LENLESS(this_type, inner, name)



#define SNMP_ACCESSOR_DEFINE_FIXED_USTRING(this_type, inner, name, len, fixed)    \
  SNMP_ACCESSOR_DEFINE_SET_FIXED_USTRING(this_type, inner, name, len, fixed)      \
  SNMP_ACCESSOR_DEFINE_GET_USTRING(this_type, inner, name, len)



#define SNMP_ACCESSOR_DEFINE_SET_IP4(this_type, inner, name)
#define SNMP_ACCESSOR_DEFINE_GET_IP4(this_type, inner, name)

#define SNMP_ACCESSOR_DEFINE_IP4(this_type, inner, name)                         \
  SNMP_ACCESSOR_DEFINE_SET_IP4(this_type, inner, name)                           \
  SNMP_ACCESSOR_DEFINE_GET_IP4(this_type, inner, name)





SNMP_DECLARE_SYMBOL(Pdu);
SNMP_DECLARE_SYMBOL(oid);
SNMP_DECLARE_SYMBOL(type);
SNMP_DECLARE_SYMBOL(value);

SNMP_DECLARE_SYMBOL(version);
SNMP_DECLARE_SYMBOL(command);
SNMP_DECLARE_SYMBOL(reqid);
SNMP_DECLARE_SYMBOL(msgid);
SNMP_DECLARE_SYMBOL(transid);
SNMP_DECLARE_SYMBOL(sessid);
SNMP_DECLARE_SYMBOL(errstat);
SNMP_DECLARE_SYMBOL(errindex);
SNMP_DECLARE_SYMBOL(time);
SNMP_DECLARE_SYMBOL(flags);
SNMP_DECLARE_SYMBOL(securityModel);
SNMP_DECLARE_SYMBOL(securityLevel);
SNMP_DECLARE_SYMBOL(msgParseModel);
SNMP_DECLARE_SYMBOL(tDomain);
SNMP_DECLARE_SYMBOL(community);
SNMP_DECLARE_SYMBOL(enterprise);
SNMP_DECLARE_SYMBOL(trap_type);
//SNMP_DECLARE_SYMBOL(agent_addr);
SNMP_DECLARE_SYMBOL(specific_type);
SNMP_DECLARE_SYMBOL(contextEngineID);
SNMP_DECLARE_SYMBOL(contextName);
SNMP_DECLARE_SYMBOL(securityEngineID);
SNMP_DECLARE_SYMBOL(securityName);
SNMP_DECLARE_SYMBOL(priority);
SNMP_DECLARE_SYMBOL(range_subid);
SNMP_DECLARE_SYMBOL(variableBindings);


SNMP_DECLARE_SYMBOL(Session);
SNMP_DECLARE_SYMBOL(process);

SNMP_DECLARE_SYMBOL(retries);
SNMP_DECLARE_SYMBOL(timeout);
SNMP_DECLARE_SYMBOL(peername);
SNMP_DECLARE_SYMBOL(remote_port);
SNMP_DECLARE_SYMBOL(localname);
SNMP_DECLARE_SYMBOL(local_port);

SNMP_DECLARE_SYMBOL(rcvMsgMaxSize);
SNMP_DECLARE_SYMBOL(sndMsgMaxSize);

SNMP_DECLARE_SYMBOL(isAuthoritative);
SNMP_DECLARE_SYMBOL(engineBoots);
SNMP_DECLARE_SYMBOL(engineTime);
SNMP_DECLARE_SYMBOL(securityAuthProto);
SNMP_DECLARE_SYMBOL(securityAuthKey);
SNMP_DECLARE_SYMBOL(securityAuthLocalKey);
SNMP_DECLARE_SYMBOL(securityPrivProto);
SNMP_DECLARE_SYMBOL(securityPrivKey);
SNMP_DECLARE_SYMBOL(securityPrivLocalKey);
SNMP_DECLARE_SYMBOL(securityModel);
SNMP_DECLARE_SYMBOL(securityLevel);
SNMP_DECLARE_SYMBOL(paramName);


SNMP_DECLARE_SYMBOL(socket);
SNMP_DECLARE_SYMBOL(tv_sec);
SNMP_DECLARE_SYMBOL(tv_usec);

#endif // _snmp_js_h