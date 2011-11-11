
#include "snmp.h"


class Pdu : node::ObjectWrap {
public:
    static void Initialize(v8::Handle<v8::Object> target);

    virtual ~Pdu() {
		close();
	}

private:

	Pdu(int type) : native_(0){
		native_ = snmp_pdu_create(type);
	}

    void close() {
		if (NULL != native_) {
			snmp_free_pdu(native_);
			native_ = NULL;
		}
    }
		
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> Close(const v8::Arguments& args);

	netsnmp_pdu* native_;
};

/*
 * Context methods.
 */

void Pdu::Initialize(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
}

v8::Handle<v8::Value> Pdu::New(const v8::Arguments& args) {
    v8::HandleScope scope;
	if(1 != args.Length()) {
		return ThrowError("Must pass a integer argument to constructor.");
	}
	v8::Handle<v8::Value> type = args[0];
	if(!type->IsNumber()){
			return ThrowTypeError("arguments type must be integer");
	}

    Pdu* pdu = new Pdu(type->ToInt32()->Value());
    pdu->Wrap(args.This());
    return args.This();
}

v8::Handle<v8::Value> Pdu::Close(const v8::Arguments& args) {
    v8::HandleScope scope;
	ObjectWrap::Unwrap<Pdu>(args.This())->close();
    return v8::Undefined();
}



void pdu_initialize(v8::Handle<v8::Object> target) {
	Pdu::Initialize(target);
}
