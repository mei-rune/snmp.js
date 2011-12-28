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


#ifndef __pdu__h__
#define __pdu__h__ 1

#include "snmp.h"


class Pdu : node::ObjectWrap {

private:
    netsnmp_pdu* native_;

    Pdu(int type) : native_(0) {
		printf("c %d\n", this);
        native_ = snmp_pdu_create(type);
    }

    void close() {
        if (NULL != native_) {
            snmp_free_pdu(native_);
            native_ = NULL;
        }
    }

public:
	// it only use for Callable
	void internalClear(){
        native_ = NULL;
	}

    virtual ~Pdu() {
		printf("d %d\n", this);
        close();
    }

    netsnmp_pdu* native() {
        return native_;
    }

    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> New(const v8::Arguments& args);


    static v8::Handle<v8::Value> fromNativeVB(const netsnmp_variable_list* vb);

    static bool convertValue(netsnmp_variable_list* vars
                             , u_char type
                             , v8::Handle<v8::Value>& value
                             , v8::Handle<v8::Value>& exception);

    static v8::Handle<v8::Value> AppendVB(netsnmp_pdu* pdu
                                          , v8::Handle<v8::Value>& hOid
                                          , v8::Handle<v8::Value>& hType
                                          , v8::Handle<v8::Value>& value);

    static v8::Handle<v8::Value> AppendToVariableList(const v8::Arguments& args);


    static v8::Handle<v8::Value> GetFromVariableList(const v8::Arguments& args);
    static v8::Handle<v8::Value> ClearVariableList(const v8::Arguments& args);
    static v8::Handle<v8::Value> Close(const v8::Arguments& args);

    static v8::Handle<v8::Value> fromPdu(const netsnmp_pdu *pdu);
    static v8::Handle<v8::Value> toPdu(v8::Handle<v8::Value> v, netsnmp_pdu *pdu);

    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, version)
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
    SNMP_ACCESSOR_DEFINE_OID(Pdu, native_->, enterprise, enterprise_length)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, trap_type)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, long, specific_type)

    SNMP_ACCESSOR_DEFINE_IP4(Pdu, native_->, agent_addr)

    SNMP_ACCESSOR_DEFINE_USTRING(Pdu, native_->, securityEngineID, securityEngineIDLen)
    SNMP_ACCESSOR_DEFINE_USTRING(Pdu, native_->, contextEngineID, contextEngineIDLen)
    SNMP_ACCESSOR_DEFINE_STRING(Pdu, native_->, contextName, contextNameLen)
    SNMP_ACCESSOR_DEFINE_STRING(Pdu, native_->, securityName, securityNameLen)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, priority)
    SNMP_ACCESSOR_DEFINE(Pdu, native_->, int32, range_subid)

    /** this is function for test */
    static v8::Handle<v8::Value> toNativePdu(const v8::Arguments& args);
    /** this is function for test */
    static v8::Handle<v8::Value> fromNativePdu(const v8::Arguments& args);
};

#endif //__pdu__cpp__ 