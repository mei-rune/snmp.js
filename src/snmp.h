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

//
//template<typename T, typename FN = void (*)(T*)>
//class auto_free
//{
//public:
//	auto_free(FN fn) : ptr_(0), fn_(fn) {
//	}
//    
//	auto_free(T* ptr, FN fn) : ptr_(ptr), fn_(fn) {
//	}
//
//	~auto_free(){
//		if(0 != ptr_) {
//			(*fn)(ptr_);
//		}
//	}
//
//	void reset(T* ptr) {
//		if(0 != ptr_) {
//			(*fn)(ptr_);
//		}
//		ptr_ = ptr;
//	}
//
//	T* release(){
//		void* ptr = ptr_;
//		ptr_ = 0;
//		return ptr;
//	}
//
//	T*& get() {
//		return ptr_;
//	}
//
//private:
//	auto_free(auto_free&);
//	auto_free& operator=(auto_free&);
//
//	T* ptr_;
//	FN fn_;
//};

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

inline v8::Handle<v8::Value> from_uchar(u_char* value, size_t len) {
    return v8::String::New((const char*)value, len);
}



inline int get_int_value(v8::Handle<v8::Object>& obj, const char* key, int defaultValue) {
    v8::Handle<v8::Value> result = obj->Get(v8::String::New(key));
	if(!result->IsNumber()){
		return defaultValue;
	}
	return result->ToInt32()->Value();
}

inline v8::Handle<v8::Value> oid_to_value(oid* name, size_t len) {
	v8::Handle<v8::Array> ret = v8::Array::New(len);
    for(size_t i = 0; i < len; ++ i) {
		ret->Set(i, v8::Int32::New(name[i]));
	}
	return ret;
}

inline oid* value_to_oid(v8::Handle<v8::Value>& s, oid* out, size_t* len) {
	if(s->IsString()){
		v8::String::Utf8Value u8(s->ToString());
		oid* buf = out;
		while(NULL == snmp_parse_oid(*u8, buf, len)){
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

#define UNWRAP(type, wrap, ref) \
  type* wrap = ObjectWrap::Unwrap<type>(ref)



#define SNMP_DEFINE_ACCESSOR_SYMBOL(name)   \
        static v8::Persistent<v8::String> name##_symbol;

#define SNMP_SET_ACCESSOR(target, name)   \
    name##_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol(#name));  \
	target->PrototypeTemplate()->SetAccessor(name##_symbol, Get_##name, Set_##name);


#define SNMP_SET_READONLY_ACCESSOR(target, name)   \
    name##_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol(#name));  \
	target->PrototypeTemplate()->SetAccessor(name##_symbol, Get_##name, 0);      \
	

#define SNMP_ACCESSOR_DEFINE_GET(this_type, value_type,  name)             \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName \
                                        , const v8::AccessorInfo& info) {  \
		v8::HandleScope scope;                                             \
		UNWRAP(this_type, wrap, info.This());                              \
		return scope.Close(v8::value_type::New(wrap->native_->name));      \
	}

#define SNMP_ACCESSOR_DEFINE_SET(this_type, value_type,  name)             \
    static void Set_##name(v8::Local<v8::String> propertyName              \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                             \
		UNWRAP(this_type, wrap, info.This());                              \
        wrap->native_->name = value->value_type##Value();                  \
	}

#define SNMP_ACCESSOR_DEFINE_GET_OID(this_type, name, len)                 \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName \
                                        , const v8::AccessorInfo& info) {  \
		v8::HandleScope scope;                                             \
		UNWRAP(this_type, wrap, info.This());                              \
		v8::Handle<v8::Array> ret = v8::Array::New(wrap->native_->len);    \
        for(size_t i = 0; i < wrap->native_->len; ++ i) {                  \
			ret->Set(i, v8::Int32::New(wrap->native_->name[i]));           \
		}                                                                  \
		return scope.Close(ret);                                           \
	}

#define SNMP_ACCESSOR_DEFINE_SET_OID(this_type, name, len)                 \
    static void Set_##name(v8::Local<v8::String> propertyName              \
           , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                             \
		UNWRAP(this_type, wrap, info.This());                                     \
		oid* new_value = wrap->native_->name;                              \
        size_t new_len = wrap->native_->len;                               \
		new_value = value_to_oid(value, new_value, &new_len);              \
		if(new_value != wrap->native_->name) free(wrap->native_->name);    \
        wrap->native_->name = new_value;                                   \
        wrap->native_->len = new_len;                                      \
	}

	
#define SNMP_ACCESSOR_DEFINE_GET_STRING(char_type, this_type, name, len)          \
	static v8::Handle<v8::Value> Get_##name(v8::Local<v8::String> propertyName    \
                                           , const v8::AccessorInfo& info) {      \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                            \
		return scope.Close(v8::String::New((const char*)wrap->native_->name       \
		                                               , wrap->native_->len));    \
	}

#define SNMP_ACCESSOR_DEFINE_SET_STRING(char_type, this_type, name, len)          \
    static void Set_##name(v8::Local<v8::String> propertyName                     \
                  , v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
		v8::HandleScope scope;                                                    \
		UNWRAP(this_type, wrap, info.This());                                            \
		v8::String::Utf8Value u8(value->ToString());                              \
		if(0 != wrap->native_->name) free(wrap->native_->name);                   \
        wrap->native_->name = (char_type*)strdup(*u8);                            \
        wrap->native_->len = u8.length();                                         \
	}



#define SNMP_ACCESSOR_DEFINE(this_type, value_type,  name)                        \
  SNMP_ACCESSOR_DEFINE_SET(this_type, value_type,  name)                          \
  SNMP_ACCESSOR_DEFINE_GET(this_type, value_type,  name)      


#define SNMP_ACCESSOR_DEFINE_OID(this_type, name, len)                            \
  SNMP_ACCESSOR_DEFINE_SET_OID(this_type, name, len)                              \
  SNMP_ACCESSOR_DEFINE_GET_OID(this_type, name, len)      
	

#define SNMP_ACCESSOR_DEFINE_USTRING(this_type, name, len)                        \
  SNMP_ACCESSOR_DEFINE_SET_STRING(u_char, this_type, name, len)                   \
  SNMP_ACCESSOR_DEFINE_GET_STRING(u_char, this_type, name, len)      

#define SNMP_ACCESSOR_DEFINE_STRING(this_type, name, len)                         \
  SNMP_ACCESSOR_DEFINE_SET_STRING(char, this_type, name, len)                     \
  SNMP_ACCESSOR_DEFINE_GET_STRING(char, this_type, name, len)        

void session_initialize(v8::Handle<v8::Object> target);
void pdu_initialize(v8::Handle<v8::Object> target);

#endif // _snmp_js_h