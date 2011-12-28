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
public:

    class SwapScope {
    public:
        SwapScope(Session* session, const v8::Arguments& args)
            : session_(session)
            , exception_(v8::Undefined())
            , args(args)
			, thisObject(args.This()){
            session_->swap_ = this;
        }

        ~SwapScope() {
            session_->swap_ = NULL;
        }

        v8::Handle<v8::Value> Close(v8::Handle<v8::Value> value) {
            if(exception_->IsUndefined()) {
                return scope.Close(value);
            }
            return exception_;
        }

        void error(const char* fmt, ...) {
			char buf[1024];
			va_list args;
			va_start(args, fmt);

#ifdef _MSC_VER
			_vsnprintf(
#else
			vsnprintf(
#endif
				buf, 1024, fmt, args);

            exception_ = ThrowError(buf);
			va_end(args);
        }

        void error(v8::Handle<v8::Value> e) {
			exception_ = e;
		}

        bool hasException() {
            return !exception_->IsUndefined();
        }

        v8::Handle<v8::Value> getException() {
            return exception_;
        }

        v8::HandleScope scope;
        v8::Handle<v8::Object> thisObject;

        const v8::Arguments& args;
    private:
        Session* session_;
        v8::Handle<v8::Value> exception_;
        SwapScope(SwapScope& scope);
        SwapScope& operator=(SwapScope& scope);
    };

private:

    netsnmp_session arguments_;
    void* session_;
    SwapScope* swap_;

    Session() : session_(0), swap_(0) {
        snmp_sess_init(&arguments_);
		arguments_.myvoid = this;
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

	void makeCallback(v8::Handle<v8::Object> object,
							  v8::Handle<v8::String> method,
                              int argc,
                              v8::Handle<v8::Value> argv[]) {
	    v8::TryCatch try_catch;

		v8::Local<v8::Value> callback = object->Get(method);
		if (!callback->IsFunction()) {
			v8::String::Utf8Value u8(method);
			this->swap_->error("Object #<Session> has no method '%s'.", *u8);
			return;
		}
		v8::Local<v8::Function>::Cast(callback)->Call(object, argc, argv);
        if (try_catch.HasCaught()) {
			this->swap_->error(try_catch.Exception());
        }
    }

    void on_open(Stream* stream) {
        char dst[50];

        netsnmp_indexed_addr_pair* pair = (netsnmp_indexed_addr_pair*)stream->transport()->data;
		
		struct sockaddr_in* in4 = &pair->local_addr.sin;
		struct sockaddr_in6* in6 = &pair->local_addr.sin6;
		void* addr = (AF_INET ==  pair->local_addr.sa.sa_family)?(void*)&in4->sin_addr:(void*)&in6->sin6_addr;
        const char *to_addr = inet_ntop( pair->local_addr.sa.sa_family, addr, dst, 50);

        v8::Handle<v8::Value> argv[] = {
            (AF_INET == pair->local_addr.sa.sa_family)?udp4_symbol:udp6_symbol
            , from_uint16(ntohs((AF_INET == pair->local_addr.sa.sa_family)?in4->sin_port:in6->sin6_port))
            , from_string(to_addr, strlen(to_addr))
        };

        makeCallback(swap_->thisObject, on_open_symbol, 3, argv);
    }

    void on_close(Stream* stream) {
        v8::Handle<v8::Value> argv[] = { v8::Undefined() };
        makeCallback(swap_->thisObject, on_close_symbol, 0, argv);
    }

    void on_send(Stream* stream, const void *buf, int size,
                 struct sockaddr *to, socklen_t to_len) {
        char dst[50];
		struct sockaddr_in* in4 = (struct sockaddr_in*)to;
		struct sockaddr_in6* in6 = (struct sockaddr_in6*)to;
		void* addr = (AF_INET == to->sa_family)?(void*)&in4->sin_addr:(void*)&in6->sin6_addr;

        const char *to_addr = inet_ntop(to->sa_family, addr, dst, 50);
        v8::Handle<v8::Value> argv[] = {
            from_ustring((const unsigned char*)buf, size)
            , from_uint16(ntohs((AF_INET == to->sa_family)?in4->sin_port:in6->sin6_port))
            , from_string(to_addr, strlen(to_addr))
        };

        v8::Handle<node::Buffer> outgoing;
        makeCallback(swap_->thisObject, on_send_symbol, 3, argv);
    }

    void on_recv(Stream* stream, void *buf, int *size,
                 struct sockaddr *from, socklen_t *from_len) {
        netsnmp_indexed_addr_pair* pair = (netsnmp_indexed_addr_pair*)stream->transport()->data;
        v8::Handle<v8::Value> msg = swap_->args[0];
        v8::Handle<v8::Object> rinfo = swap_->args[1]->ToObject();
        v8::String::Utf8Value u8(rinfo->Get(address_symbol));

        *from_len = inet_pton(pair->local_addr.sa.sa_family, *u8, from);

        if(msg->IsObject() && node::Buffer::HasInstance(msg->ToObject())) {
            UNWRAP(node::Buffer, buffer, msg->ToObject());
            if(node::Buffer::Length(buffer) > *size) {
                *size = 0;
                swap_->error("msg length too big.");
                return ;
            }
            *size = node::Buffer::Length(buffer);
            memcpy(buf, node::Buffer::Data(buffer), *size);
        } else {
            v8::String::Utf8Value u8(msg->ToString());
            if(u8.length() > *size) {
                *size = 0;
                swap_->error("msg length too big.");
                return ;
            }
            *size = u8.length();
            memcpy(buf, *u8, *size);
        }
    }

    static Session* ToSession(netsnmp_session* session);

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
