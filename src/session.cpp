#ifndef __session__cpp__
#define __session__cpp__ 1

#include "snmp.h"
#include "pdu.cpp"
#include <memory>


static v8::Persistent<v8::String> process_symbol;

class Callable {

private:
    v8::Persistent<v8::Object> cb_;
    v8::Persistent<v8::Object> pdu_;
public:
    Callable() {
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
            from_int32(code), wrap->pdu_.IsEmpty()?Pdu::fatchPdu(pdu):wrap->pdu_
        };

        // get process from global scope.
        v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
        v8::Local<v8::Object> process = global->Get(process_symbol)->ToObject();


        wrap->cb_->CallAsFunction(process, 2, args);
		if (try_catch.HasCaught()) {
		   node::FatalException(try_catch);
		}
    }

};


class Session : node::ObjectWrap {
private:
	netsnmp_session arguments_;
	void* session_;

	Session() : session_(0){
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

    static void Initialize(v8::Handle<v8::Object> target) {
		v8::HandleScope scope;


        process_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("process"));

		v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
		t->SetClassName(v8::String::NewSymbol("Session"));
		t->InstanceTemplate()->SetInternalFieldCount(1);

		NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
		target->Set(v8::String::NewSymbol("Session"), t->GetFunction());
	}

    static v8::Handle<v8::Value> New(const v8::Arguments& args){
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

    static v8::Handle<v8::Value> Open(const v8::Arguments& args) {
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

    static v8::Handle<v8::Value> sendNativePdu(const v8::Arguments& args) {
        v8::HandleScope scope;
        UNWRAP(Session, wrap, args.This());
        if(0 != wrap->session_) {
            return ThrowError("Session already opened.");
        }

        if(2 != args.Length()) {
		    return ThrowError("Must pass pdu and cb arguments to constructor.");
        }

        v8::Handle<v8::Object> cb = args[1]->ToObject();
        if (!cb->IsCallable()) {
		    return ThrowError("Must pass pdu and cb arguments to constructor.");
        }

        UNWRAP(Pdu, pdu, args[0]->ToObject());

        std::auto_ptr<Callable> callable(new Callable());

        if(0 != snmp_sess_async_send(wrap->session_, pdu->native(),
                                         Callable::OnEvent, callable.get())) {
            return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));                              	
        }
        callable.release();
        return v8::Undefined();
    }

	static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
		v8::HandleScope scope;
		UNWRAP(Session, wrap, args.This());
		wrap->close();
		return v8::Undefined();
	}
};

#endif //   __session__cpp__
