
#include "snmp.h"


class Pdu : node::ObjectWrap {
public:
    static void Initialize(v8::Handle<v8::Object> target){
		v8::HandleScope scope;

		v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);    
		t->SetClassName(v8::String::NewSymbol("Pdu"));
		t->InstanceTemplate()->SetInternalFieldCount(1);

		NODE_SET_PROTOTYPE_METHOD(t, "close", Close);

		SNMP_SET_ACCESSOR(t, version);
		SNMP_SET_ACCESSOR(t, command)
		SNMP_SET_ACCESSOR(t, reqid)
		SNMP_SET_ACCESSOR(t, msgid)
		SNMP_SET_ACCESSOR(t, transid)
		SNMP_SET_ACCESSOR(t, sessid)
		SNMP_SET_ACCESSOR(t, errstat)
		SNMP_SET_ACCESSOR(t, errindex)
		SNMP_SET_ACCESSOR(t, time)
		SNMP_SET_ACCESSOR(t, flags)
		SNMP_SET_ACCESSOR(t, securityModel)
		SNMP_SET_ACCESSOR(t, securityLevel)
		SNMP_SET_ACCESSOR(t, msgParseModel)
		SNMP_SET_READONLY_ACCESSOR(t, tDomain)
		SNMP_SET_ACCESSOR(t, community)
		SNMP_SET_ACCESSOR(t, enterprise)
		SNMP_SET_ACCESSOR(t, trap_type)
		//SNMP_SET_ACCESSOR(t, agent_addr)
		SNMP_SET_ACCESSOR(t, specific_type)
		SNMP_SET_ACCESSOR(t, contextEngineID)
		SNMP_SET_ACCESSOR(t, contextName)
		SNMP_SET_ACCESSOR(t, securityEngineID)
		SNMP_SET_ACCESSOR(t, securityName)
		SNMP_SET_ACCESSOR(t, priority)
		SNMP_SET_ACCESSOR(t, range_subid)


		target->Set(v8::String::NewSymbol("Pdu"), t->GetFunction());
	}

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
		
    static v8::Handle<v8::Value> New(const v8::Arguments& args) {
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
	
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, version)

	//static v8::Handle<v8::Value> Get_version(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
	//	v8::HandleScope scope;
	//	Pdu* wrap = ObjectWrap::Unwrap<Pdu>(info.This());
	//	return scope.Close(v8::Int32::New(wrap->native_->version));               
	//}

 //   static void Set_version(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) { 
	//	v8::HandleScope scope;
	//	Pdu* wrap = ObjectWrap::Unwrap<Pdu>(info.This());
 //       wrap->native_->version = value->Int32Value();
	//}

	SNMP_ACCESSOR_DEFINE(Pdu, Int32, command)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, reqid)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, msgid)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, transid)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, sessid)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, errstat)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, errindex)
	SNMP_ACCESSOR_DEFINE(Pdu, Uint32, time)
	SNMP_ACCESSOR_DEFINE(Pdu, Uint32, flags)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, securityModel)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, securityLevel)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, msgParseModel)
	SNMP_ACCESSOR_DEFINE_GET_OID(Pdu, tDomain, tDomainLen)
	SNMP_ACCESSOR_DEFINE_USTRING(Pdu, community, community_len)
	SNMP_ACCESSOR_DEFINE_OID(Pdu, enterprise, enterprise_length)

	//
	//static v8::Handle<v8::Value> Get_enterprise(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info);
	//	v8::Handle<v8::Array> ret = v8::Array::New(wrap->native_->enterprise_length);
 //       for(size_t i = 0; i < wrap->native_->enterprise_length; ++ i) {
	//		ret->Set(i, v8::Int32::New(wrap->native_->enterprise[i]));
	//	}
	//	return scope.Close(ret);
	//}
	//
 //   static void Set_enterprise(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info);
	//	oid* new_value = wrap->native_->enterprise;
 //       size_t new_len = wrap->native_->enterprise_length;
	//	new_value = oid_parse(value, new_value, &new_len);
	//	if(new_value != wrap->native_->enterprise) free(wrap->native_->enterprise);
 //       wrap->native_->enterprise = new_value;
 //       wrap->native_->enterprise_length = new_len;
	//}

	SNMP_ACCESSOR_DEFINE(Pdu, Int32, trap_type)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, specific_type)

	//	SNMP_SET_ACCESSOR(t, agent_addr)

	SNMP_ACCESSOR_DEFINE_USTRING(Pdu, contextEngineID, contextEngineIDLen)
	SNMP_ACCESSOR_DEFINE_STRING(Pdu, contextName, contextNameLen)
	SNMP_ACCESSOR_DEFINE_USTRING(Pdu, securityEngineID, securityEngineIDLen)
	SNMP_ACCESSOR_DEFINE_STRING(Pdu, securityName, securityNameLen)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, priority)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, range_subid)

	//static v8::Handle<v8::Value> Get_version(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, args);
	//	return v8::Int32::New(wrap->native_->version);
	//}
	//
	//static v8::Handle<v8::Value> Set_version(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, args);
	//	
	//	v8::Handle<v8::Value> value = args[0];
	//	if(!value->IsInt32()){
	//		return ThrowTypeError("arguments type must be integer");
	//	}

	//	wrap->native_->version = value->Int32Value();
	//	return v8::Undefined();
	//}

	static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
		v8::HandleScope scope;
		ObjectWrap::Unwrap<Pdu>(args.This())->close();
		return v8::Undefined();
	}

	netsnmp_pdu* native_;
};


void pdu_initialize(v8::Handle<v8::Object> target) {
	Pdu::Initialize(target);
}



	//static v8::Handle<v8::Value> Get(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(VariableBindings, wrap, args);

	//	return v8::Undefined();
	//}

	//static v8::Handle<v8::Value> Append(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(VariableBindings, wrap, args);
	//	if(2 > args.Length()) {
	//		return ThrowError("Must pass name, value and option type arguments to set.");
	//	}
	//	if(3 < args.Length()) {
	//		return ThrowError("Arguments too much.");
	//	}
	//	oid     name_loc[1024];
	//	oid*    name = name_loc;
	//	size_t  name_len = 1024;

	//	if(NULL ==    oid_parse(args[0], name, &name_len)) {
	//		return ThrowError("argument name must be a int array.");
	//	}
	//	
	//	u_char type = '=';
	//	v8::Handle<v8::Value> value;


	//	if(2 == args.Length()) {
	//		value = args[1];
	//	} else {
	//		type = (u_char)args[1]->Uint32Value();
	//		value = args[2];
	//	}

	//	if('=' == type) {
	//		int ret = snmp_add_var(netsnmp_pdu *, const oid *, size_t, char,
	//			const char *);

	//		if(0 != ret){
	//				snmp_api_errstring(err);

	//			std::string err
	//			return ThrowError("argument value must be a int array.");
	//		}
	//	}


	//	return v8::Undefined();
	//}