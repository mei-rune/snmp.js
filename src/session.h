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
#ifndef __session__h__
#define __session__h__ 1

#include "snmp.h"
#include "pdu.h"
#include "stream_adapter.h"

class Session : node::ObjectWrap {
private:
    netsnmp_session arguments_;
    void* session_;

    Session() : session_(0) {
        snmp_sess_init(&arguments_);
    }

    void open() {
        if (NULL == session_) {
            session_ = snmp_sess_open(&arguments_);
        }
    }

    void close() {
        if (NULL != session_) {
            snmp_sess_close(session_);
            session_ = NULL;
        }
    }

public:

    virtual ~Session() {
        close();
    }

    void on_open(Stream* stream) {
        // TODO
    }

    void on_close(Stream* stream) {
        // TODO
    }

    void on_send(Stream* stream) {
        // TODO
    }

    static Session* ToSession(void* session);

    static void Initialize(v8::Handle<v8::Object> target);

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> Open(const v8::Arguments& args);

    static v8::Handle<v8::Value> sendNativePdu(const v8::Arguments& args);

    static v8::Handle<v8::Value> sendPdu(const v8::Arguments& args) ;

    static v8::Handle<v8::Value> readData(const v8::Arguments& args);

    static v8::Handle<v8::Value> selectInfo(const v8::Arguments& args);

    static v8::Handle<v8::Value> onData(const v8::Arguments& args);

    static v8::Handle<v8::Value> Close(const v8::Arguments& args);

    SNMP_ACCESSOR_DEFINE(Session, arguments_. , long,  version);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , int32,  retries);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , long,  timeout);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , ulong,  flags);
    SNMP_ACCESSOR_DEFINE_STRING_LENLESS(Session, arguments_. , peername);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , ushort,  remote_port);
    SNMP_ACCESSOR_DEFINE_STRING_LENLESS(Session, arguments_. , localname);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , ushort,  local_port);

    SNMP_ACCESSOR_DEFINE(Session, arguments_. , long,  sessid);
    SNMP_ACCESSOR_DEFINE_USTRING(Session, arguments_. , community, community_len);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , size_t,  rcvMsgMaxSize);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , size_t,  sndMsgMaxSize);

    SNMP_ACCESSOR_DEFINE(Session, arguments_. , uchar,  isAuthoritative);
    SNMP_ACCESSOR_DEFINE_USTRING(Session, arguments_. , contextEngineID, contextEngineIDLen);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , uint32,  engineBoots);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , uint32,  engineTime);
    SNMP_ACCESSOR_DEFINE_STRING(Session, arguments_. , contextName, contextNameLen);
    SNMP_ACCESSOR_DEFINE_USTRING(Session, arguments_. , securityEngineID, securityEngineIDLen);
    SNMP_ACCESSOR_DEFINE_STRING(Session, arguments_. , securityName, securityNameLen);
    SNMP_ACCESSOR_DEFINE_OID(Session, arguments_. , securityAuthProto, securityAuthProtoLen);
    SNMP_ACCESSOR_DEFINE_FIXED_USTRING(Session, arguments_. , securityAuthKey, securityAuthKeyLen, USM_AUTH_KU_LEN);
    SNMP_ACCESSOR_DEFINE_USTRING(Session, arguments_. , securityAuthLocalKey, securityAuthLocalKeyLen);
    SNMP_ACCESSOR_DEFINE_OID(Session, arguments_. , securityPrivProto, securityPrivProtoLen);
    SNMP_ACCESSOR_DEFINE_FIXED_USTRING(Session, arguments_. , securityPrivKey, securityPrivKeyLen, USM_PRIV_KU_LEN);
    SNMP_ACCESSOR_DEFINE_USTRING(Session, arguments_. , securityPrivLocalKey, securityPrivLocalKeyLen);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , int32,  securityModel);
    SNMP_ACCESSOR_DEFINE(Session, arguments_. , int32,  securityLevel);
    SNMP_ACCESSOR_DEFINE_STRING_LENLESS(Session, arguments_. , paramName);
};

#endif //   __session__h__
