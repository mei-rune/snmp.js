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
            v8::Undefined(), wrap->pdu_, Pdu::fromPdu(pdu)
        };


        switch(code) {
        case NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE:
            args[0] = v8::Undefined();
            break;
        case NETSNMP_CALLBACK_OP_TIMED_OUT:
            args[0] = from_string("timeout", 7);
            break;
        case NETSNMP_CALLBACK_OP_SEND_FAILED:
            args[0] = from_string("send_error", 10);
            break;
        case NETSNMP_CALLBACK_OP_CONNECT:
            args[0] = from_string("connect", 7);
            break;
        case NETSNMP_CALLBACK_OP_DISCONNECT:
            args[0] = from_string("disconnect", 10);
            break;
        default:
            args[0] = from_string("unknown", 7);
            break;
        }

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

Session* Session::ToSession(netsnmp_session* s) {
    return (Session*)s->myvoid;
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

    NODE_SET_PROTOTYPE_METHOD(t, "_open", Open);
    NODE_SET_PROTOTYPE_METHOD(t, "_onData", onData);
    NODE_SET_PROTOTYPE_METHOD(t, "_onTimeout", onTimeout);
    NODE_SET_PROTOTYPE_METHOD(t, "_readData", readData);
    NODE_SET_PROTOTYPE_METHOD(t, "_sendPdu", sendPdu);
    NODE_SET_PROTOTYPE_METHOD(t, "_sendNativePdu", sendNativePdu);
    NODE_SET_PROTOTYPE_METHOD(t, "_close", Close);
    target->Set(Session_symbol, t->GetFunction());
}

v8::Handle<v8::Value> Session::New(const v8::Arguments& args) {
    v8::HandleScope scope;

    if(0 != args.Length()) {
        return ThrowError("Must not pass any arguments to New().");
    }

    Session* session = new Session();
    session->Wrap(args.This());
    return args.This();
}

v8::Handle<v8::Value> Session::Open(const v8::Arguments& args) {
    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
    if(0 != wrap->session_) {
        return ThrowError("Session already opened.");
    }
    wrap->session_ = snmp_sess_open(&wrap->arguments_);
    if(0 == wrap->session_) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    if(scope.hasException()) {
        return scope.getException();
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::sendNativePdu(const v8::Arguments& args) {
    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
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

    std::auto_ptr<netsnmp_pdu> copy(snmp_clone_pdu(pdu->native()));
    std::auto_ptr<Callable> callable(new Callable(cb, args[0], copy.get()));
    if(0 == snmp_sess_async_send(wrap->session_, copy.get(),
                                 Callable::OnEvent, callable.get())) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    callable.release();
    copy.release();

    if(scope.hasException()) {
        return scope.getException();
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::sendPdu(const v8::Arguments& args) {
    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
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

    std::auto_ptr<netsnmp_pdu> pdu(snmp_pdu_create(SNMP_MSG_GET));
    v8::Handle<v8::Value> ret = Pdu::toPdu(args[0], pdu.get());
    if(!ret->IsUndefined()) {
        return ret;
    }


    std::auto_ptr<Callable> callable(new Callable(cb, args[0], pdu.get()));

    if(0 != snmp_sess_async_send(wrap->session_, pdu.get(),
                                 Callable::OnEvent, callable.get())) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
    callable.release();
    pdu.release();

    if(scope.hasException()) {
        return scope.getException();
    }
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
    if(0 == snmp_sess_select_info(wrap->session_, &numfds, &fdset,
		&timeout, &block)) {
		snmp_sess_timeout(wrap->session_);
		goto error;
	}

	if(0 == block) {
		int ret = select(numfds, &fdset, 0, 0, &timeout);
		if(SOCKET_ERROR == ret || 0 == ret) {
			snmp_sess_timeout(wrap->session_);
			goto error;
		}
	}

    if(-1 == snmp_sess_read(wrap->session_, &fdset)) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }
error:
    return v8::Undefined();
}

v8::Handle<v8::Value> Session::onTimeout(const v8::Arguments& args) {
    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
    if(0 == wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(0 != args.Length()) {
        return ThrowError("Must not pass any arguments to onTimeout.");
    }

	snmp_sess_timeout(wrap->session_);

    if(scope.hasException()) {
        return scope.getException();
    }
	return v8::Undefined();
}

v8::Handle<v8::Value> Session::onData(const v8::Arguments& args) {

    fd_set fdset;
    struct timeval timeout;
    int block = 1;
    int numfds = 0;

    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
    if(0 == wrap->session_) {
        return ThrowError("Session hasn't opened.");
    }

    if(2 != args.Length()) {
        return ThrowError("Must pass the msg and rinfo arguments to onData.");
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


    if(-1 == snmp_sess_read(wrap->session_, &fdset)) {
        return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));
    }

    if(0 == block) {
        v8::Handle<v8::Object> ret = v8::Object::New();
        v8::Handle<v8::Object> timeout_v8 = v8::Object::New();
        timeout_v8->Set(tv_sec_symbol,  from_long(timeout.tv_sec));
        timeout_v8->Set(tv_usec_symbol, from_long(timeout.tv_usec));
        ret->Set(timeout_symbol, timeout_v8);
        return scope.Close(ret);
    } else {
        return v8::Undefined();
    }
}

v8::Handle<v8::Value> Session::Close(const v8::Arguments& args) {
    UNWRAP(Session, wrap, args.This());
    SwapScope scope(wrap, args);
    wrap->close();

    if(scope.hasException()) {
        return scope.getException();
    }
    return v8::Undefined();
}
