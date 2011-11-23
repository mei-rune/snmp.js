
#include "snmp.h"



class Request {

private:
    v8::Persistent<v8::Object> cb_;
    v8::Persistent<v8::Object> pdu_;

    Request() : native_(0){
    }

    virtual ~Request() {
    }

//#define NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE	1
//#define NETSNMP_CALLBACK_OP_TIMED_OUT		2
//#define NETSNMP_CALLBACK_OP_SEND_FAILED		3
//#define NETSNMP_CALLBACK_OP_CONNECT		4
//#define NETSNMP_CALLBACK_OP_DISCONNECT		5

    static int OnEvent(int code, netsnmp_session *sess, int reqid,
                                          netsnmp_pdu *pdu, void *ctxt) {
        HandleScope scope;
        std::auto_ptr<Request> wrap(reinterpret_cast<Request*>(ctxt));
        assert(0 != wrap.get());

		TryCatch try_catch;

        if(pdu_.IsEmpty()) {
            wrap->cb->CallAsFunction(process, 1, fatchPdu(pdu));
		} else {
            wrap->cb->CallAsFunction(process, 1, wrap->pdu_);
		}
		if (try_catch.HasCaught()) {
		   FatalException(try_catch);
		}
    }

};


class Session : node::ObjectWrap {
private:
	netsnmp_session arguments_;
	void* session_;

	Session() : session_(0){
	}

    void open() {
		if (NULL == session_) {
			session_ = snmp_sess_init(&arguments_);
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
			session->arguments_.version = get_int_value(obj, "version", session->arguments_.version);
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

        if (!args[1]->IsCallable()) {
		    return ThrowError("Must pass pdu and cb arguments to constructor.");
        }

        UNWRAP(Pdu, pdu, args[0]);
        v8::Handle<v8::Object> cb = args[1]->ToObject();


        if(0 != snmp_sess_async_send(wrap->session_, netsnmp_pdu *,
                                         netsnmp_callback, void *)) {
            return ThrowError(snmp_api_errstring(wrap->arguments_.s_snmp_errno));                              	
        }

        return v8::Undefined();
    }

	static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
		v8::HandleScope scope;
		UNWRAP(Session, wrap, args.This());
		wrap->close();
		return v8::Undefined();
	}
};

void session_initialize(v8::Handle<v8::Object> target) {
	Session::Initialize(target);
}
