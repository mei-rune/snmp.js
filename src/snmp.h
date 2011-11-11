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

#define ThrowTypeError(x) v8::ThrowException(v8::Exception::TypeError(v8::String::New(x)))


inline int get_int_value(v8::Handle<v8::Object>& obj, const char* key, int defaultValue) {
    v8::Handle<v8::Value> result = obj->Get(v8::String::New(key));
	if(!result->IsNumber()){
		return defaultValue;
	}
	return result->ToInt32()->Value();
}




void session_initialize(v8::Handle<v8::Object> target);

#endif // _snmp_js_h