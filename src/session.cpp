
#include "snmp.h"

class Session : node::ObjectWrap {
friend class Socket;
public:
    static void Initialize(v8::Handle<v8::Object> target);

    virtual ~Session() {
		close();
	}

private:

	Session() : session_(0){
		snmp_sess_init(&arguments_);
	}

    void close() {
		if (NULL != session_) {
			snmp_sess_close(session_);
			session_ = NULL;
		}
    }
		
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Close(const v8::Arguments& args);

	netsnmp_session arguments_;
	void* session_;
};

/*
 * Context methods.
 */

void Session::Initialize(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
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
			return ThrowTypeError("arguments type must be an object");
		}
		
		v8::Handle<v8::Object> obj = args[0]->ToObject();
		session->arguments_.version = get_int_value(obj, "version", session->arguments_.version);
	}
    return args.This();
}

v8::Handle<v8::Value> Session::Close(const v8::Arguments& args) {
    v8::HandleScope scope;
	ObjectWrap::Unwrap<Session>(args.This())->close();
    return v8::Undefined();
}



void session_initialize(v8::Handle<v8::Object> target) {
	Session::Initialize(target);
}
