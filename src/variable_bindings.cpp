
#include "snmp.h"


class variable_bindings : node::ObjectWrap {
public:
    static void Initialize(v8::Handle<v8::Object> target);

    virtual ~variable_bindings() {
		close();
	}

private:

	variable_bindings(int type) : native_(0){
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

void variable_bindings::Initialize(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
}

v8::Handle<v8::Value> variable_bindings::New(const v8::Arguments& args) {
    v8::HandleScope scope;
	if(1 != args.Length()) {
		return ThrowError("Must pass a integer argument to constructor.");
	}
	v8::Handle<v8::Value> type = args[0];
	if(!type->IsNumber()){
			return ThrowTypeError("arguments type must be integer");
	}

    variable_bindings* vb = new variable_bindings(type->ToInt32()->Value());
    vb->Wrap(args.This());
    return args.This();
}

v8::Handle<v8::Value> variable_bindings::Close(const v8::Arguments& args) {
    v8::HandleScope scope;
	ObjectWrap::Unwrap<variable_bindings>(args.This())->close();
    return v8::Undefined();
}



void variable_bindings_initialize(v8::Handle<v8::Object> target) {
	variable_bindings::Initialize(target);
}
