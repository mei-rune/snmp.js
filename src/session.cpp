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
#include <memory>
#include "session.h"



class Callable {

private:
    v8::Persistent<v8::Object> cb_;
    v8::Persistent<v8::Value> pdu_;
    netsnmp_pdu* native_;
public:
    Callable(v8::Handle<v8::Object> cb, v8::Handle<v8::Value> pdu, netsnmp_pdu* native)
        : cb_( v8::Persistent<v8::Object>::New(cb))
        , pdu_(v8::Persistent<v8::Value>(pdu))
        , native_(0) {
    }

    virtual ~Callable() {
    }

    static int OnEvent(int code, netsnmp_session *sess, int reqid,
                       netsnmp_pdu *pdu, void *ctxt) {
        v8::HandleScope scope;
        std::auto_ptr<Callable> wrap(reinterpret_cast<Callable*>(ctxt));
        assert(0 != wrap.get());

        v8::TryCatch try_catch;
        v8::Handle<v8::Value> args[] = {
            from_int32(code), wrap->pdu_, Pdu::fromPdu(pdu)
        };

        // get process from global scope.
        v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
        v8::Local<v8::Object> process = global->Get(process_symbol)->ToObject();


        wrap->cb_->CallAsFunction(process, 3, args);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
        return 1;
    }

};

Session* Session::ToSession(void* s) {
    return CONTAINING_RECORD(s,Session, session_);
}


void Session::Initialize(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
    t->SetClassName(Session_symbol);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    SNMP_SET_ACCESSOR(t, version);
    SNMP_SET_ACCESSOR(t, retries);
    SNMP_SET_ACCESSOR(t, timeout);
    SNMP_SET_ACCESSOR(t, flags);
    SNMP_SET_ACCESSOR(t, peername);
    SNMP_SET_ACCESSOR(t, remote_port);
    SNMP_SET_ACCESSOR(t, localname);
    SNMP_SET_ACCESSOR(t, local_port);

    SNMP_SET_ACCESSOR(t, sessid);
    SNMP_SET_ACCESSOR(t, community);
    SNMP_SET_ACCESSOR(t, rcvMsgMaxSize);
    SNMP_SET_ACCESSOR(t, sndMsgMaxSize);

    SNMP_SET_ACCESSOR(t, isAuthoritative);
    SNMP_SET_ACCESSOR(t, contextEngineID);
    SNMP_SET_ACCESSOR(t, engineBoots);
    SNMP_SET_ACCESSOR(t, engineTime);
    SNMP_SET_ACCESSOR(t, contextName);
    SNMP_SET_ACCESSOR(t, securityEngineID);
    SNMP_SET_ACCESSOR(t, securityName);
    SNMP_SET_ACCESSOR(t, securityAuthProto);
    SNMP_SET_ACCESSOR(t, securityAuthKey);
    SNMP_SET_ACCESSOR(t, securityAuthLocalKey);
    SNMP_SET_ACCESSOR(t, securityPrivProto);
    SNMP_SET_ACCESSOR(t, securityPrivKey);
    SNMP_SET_ACCESSOR(t, securityPrivLocalKey);
    SNMP_SET_ACCESSOR(t, securityModel);
    SNMP_SET_ACCESSOR(t, securityLevel);
    SNMP_SET_ACCESSOR(t, paramName);

    NODE_SET_PROTOTYPE_METHOD(t, "open", Open);
    NODE_SET_PROTOTYPE_METHOD(t, "onData", onData);
    NODE_SET_PROTOTYPE_METHOD(t, "readData", readData);
    NODE_SET_PROTOTYPE_METHOD(t, "sendPdu", sendPdu);
    NODE_SET_PROTOTYPE_METHOD(t, "sendNativePdu", sendNativePdu);
    NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
    target->Set(Session_symbol, t->GetFunction());
}

v8::Handle<v8::Value> Session::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    if(1 < args.Length()) {
        return ThrowError("Must pass a arguments object to constructor.");
    }

    Session* session = new Session();
    session->Wrap(args.This());
    if (args.Length() > 0) {
        if(!args[0]->IsObject()) {
            return ThrowTypeError("Arguments type must be an object");
        }

        v8::Handle<v8::Object> obj = args[0]->ToObject();
        session->arguments_.version = to_int32(obj, "version", session->arguments_.version);
    }
    return args.This();
}

v8::Handle<v8::Value> Session::Open(const v8::Arguments& args) {
    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 != wrap->session_) {
        return ThrowError("Session already opened.");
    }
    wrap->session_ = snmp_sess_open(&wrap->arguments_);
    if(0 == wrap->session_) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));

    }
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::sendNativePdu(const v8::Arguments& args) {
    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 == wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(2 != args.Length()) {
        return ThrowError("Must pass pdu and cb arguments to sendNativePdu.");
    }

    v8::Handle<v8::Object> cb = args[1]->ToObject();
    if (!cb->IsCallable()) {
        return ThrowError("Must pass pdu and cb arguments to sendNativePdu.");
    }

    UNWRAP(Pdu, pdu, args[0]->ToObject());

    std::auto_ptr<Callable> callable(new Callable(cb, args[0], 0));

    if(0 == snmp_sess_async_send(wrap->session_, pdu->native(),
                                 Callable::OnEvent, callable.get())) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    callable.release();
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::sendPdu(const v8::Arguments& args) {
    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 == wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(2 != args.Length()) {
        return ThrowError("Must pass pdu and cb arguments to sendPdu.");
    }

    v8::Handle<v8::Object> cb = args[1]->ToObject();
    if (!cb->IsCallable()) {
        return ThrowError("Must pass pdu and cb arguments to sendPdu.");
    }

    netsnmp_pdu* pdu = snmp_pdu_create(SNMP_MSG_GET);
    v8::Handle<v8::Value> ret = Pdu::toPdu(args[0], pdu);
    if(!ret->IsUndefined()) {
        return ret;
    }


    std::auto_ptr<Callable> callable(new Callable(cb, args[0], pdu));

    if(0 != snmp_sess_async_send(wrap->session_, pdu,
                                 Callable::OnEvent, callable.get())) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    callable.release();
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::readData(const v8::Arguments& args) {

    fd_set fdset;
    struct timeval timeout;
    int block = 1;
    int numfds = 0;

    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 == wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(0 != args.Length()) {
        return ThrowError("Must not pass any arguments to readData.");
    }

    FD_ZERO(&fdset);

    /**
    * block input:  set to 1 if input timeout value is undefined
    * set to 0 if input timeout value is defined
    * block output: set to 1 if output timeout value is undefined
    * set to 0 if output rimeout vlaue id defined
    */
    snmp_sess_select_info(wrap->session_, &numfds, &fdset,
                          &timeout, &block);

    if(-1 == snmp_sess_read(wrap->session_, &fdset)) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::selectInfo(const v8::Arguments& args) {

    fd_set fdset;
    struct timeval timeout;
    int block = 1;
    int numfds = 0;

    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 != wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(0 != args.Length()) {
        return ThrowError("Must not pass any arguments to readData.");
    }

    FD_ZERO(&fdset);

    /**
    * block input:  set to 1 if input timeout value is undefined
    * set to 0 if input timeout value is defined
    * block output: set to 1 if output timeout value is undefined
    * set to 0 if output rimeout vlaue id defined
    */
    if(1 != snmp_sess_select_info(wrap->session_, &numfds, &fdset,
                                  &timeout, &block)) {
        return v8::Undefined();
    }

    v8::Handle<v8::Object> ret = v8::Object::New();
    // NOTICE: this is hack.
    ret->Set(socket_symbol, from_int32(numfds-1));

    if(0 == block) {
        v8::Handle<v8::Object> timeout_v8 = v8::Object::New();
        timeout_v8->Set(tv_sec_symbol,  from_long(timeout.tv_sec));
        timeout_v8->Set(tv_usec_symbol, from_long(timeout.tv_usec));
        ret->Set(timeout_symbol, timeout_v8);
    }

    return scope.Close(ret);
}

v8::Handle<v8::Value> Session::onData(const v8::Arguments& args) {

    fd_set fdset;
    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    if(0 != wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(0 != args.Length()) {
        return ThrowError("Must not pass any arguments to onData.");
    }
    FD_ZERO(&fdset);

    if(-1 == snmp_sess_read(wrap->session_, &fdset)) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::Close(const v8::Arguments& args) {
    v8::HandleScope scope;
    UNWRAP(Session, wrap, args.This());
    wrap->close();
    return v8::Undefined();
}
