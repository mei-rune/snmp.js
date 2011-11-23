#ifndef __pdu__cpp__
#define __pdu__cpp__ 1

#include "snmp.h"

static v8::Persistent<v8::String> pdu_symbol;
static v8::Persistent<v8::String> oid_symbol;
static v8::Persistent<v8::String> type_symbol;
static v8::Persistent<v8::String> value_symbol;

SNMP_DEFINE_SYMBOL(version);
SNMP_DEFINE_SYMBOL(command);
SNMP_DEFINE_SYMBOL(reqid);
SNMP_DEFINE_SYMBOL(msgid);
SNMP_DEFINE_SYMBOL(transid);
SNMP_DEFINE_SYMBOL(sessid);
SNMP_DEFINE_SYMBOL(errstat);
SNMP_DEFINE_SYMBOL(errindex);
SNMP_DEFINE_SYMBOL(time);
SNMP_DEFINE_SYMBOL(flags);
SNMP_DEFINE_SYMBOL(securityModel);
SNMP_DEFINE_SYMBOL(securityLevel);
SNMP_DEFINE_SYMBOL(msgParseModel);
SNMP_DEFINE_SYMBOL(tDomain);
SNMP_DEFINE_SYMBOL(community);
SNMP_DEFINE_SYMBOL(enterprise);
SNMP_DEFINE_SYMBOL(trap_type);
//SNMP_DEFINE_SYMBOL(agent_addr);
SNMP_DEFINE_SYMBOL(specific_type);
SNMP_DEFINE_SYMBOL(contextEngineID);
SNMP_DEFINE_SYMBOL(contextName);
SNMP_DEFINE_SYMBOL(securityEngineID);
SNMP_DEFINE_SYMBOL(securityName);
SNMP_DEFINE_SYMBOL(priority);
SNMP_DEFINE_SYMBOL(range_subid);


class Pdu : node::ObjectWrap {

private:
    netsnmp_pdu* native_;

	Pdu(int type) : native_(0){
		native_ = snmp_pdu_create(type);
	}

    void close() {
		if (NULL != native_) {
			snmp_free_pdu(native_);
			native_ = NULL;
		}
    }

public:

    virtual ~Pdu() {
		close();
	}


    netsnmp_pdu* native() {
    	return native_;
	}

    static void Initialize(v8::Handle<v8::Object> target) {
		v8::HandleScope scope;

		
        pdu_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("Pdu"));
        oid_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("oid"));
        type_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("type"));
        value_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("value"));

		v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);    
		t->SetClassName(pdu_symbol);
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


		target->Set(pdu_symbol, t->GetFunction());
		
		target->Set(v8::String::NewSymbol("appendVariableList"),  v8::FunctionTemplate::New(Append)->GetFunction());
		target->Set(v8::String::NewSymbol("getVariableList"),  v8::FunctionTemplate::New(Get)->GetFunction());
		target->Set(v8::String::NewSymbol("clearVariableList"),  v8::FunctionTemplate::New(Clear)->GetFunction());

		
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
	//	UNWRAP(Pdu, wrap, info.This());
	//	return scope.Close(v8::Int32::New(wrap->native_->version));               
	//}

    //static void Set_version(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) { 
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info.This());
    //  wrap->native_->version = value->Int32Value();
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

	
	//static v8::Handle<v8::Value> Get_community(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info.This());
	//	return node::Buffer::New((char*)wrap->native_->community, sizeof(u_char)*wrap->native_->community_len)->handle_;
	//}

    //static void Set_community(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
	//	v8::HandleScope scope; 
	//	UNWRAP(Pdu, wrap, info.This());
	//	UNWRAP(node::Buffer, buffer, value->ToObject());
	//	     
	//	if(0 == wrap->native_->community
	//	  || wrap->native_->community_len < node::Buffer::Length(buffer)) {
	//	    free(wrap->native_->community);
    //        wrap->native_->community = (u_char*)malloc(node::Buffer::Length(buffer) + 4);
	//	}

	//	memcpy(wrap->native_->community, node::Buffer::Data(buffer), node::Buffer::Length(buffer));
    //    wrap->native_->community_len = node::Buffer::Length(buffer);
	//}

	SNMP_ACCESSOR_DEFINE_OID(Pdu, enterprise, enterprise_length)

	//
	//static v8::Handle<v8::Value> Get_enterprise(v8::Local<v8::String> propertyName, const v8::AccessorInfo& info) {
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info.This());
	//	v8::Handle<v8::Array> ret = v8::Array::New(wrap->native_->enterprise_length);
 //       for(size_t i = 0; i < wrap->native_->enterprise_length; ++ i) {
	//		ret->Set(i, v8::Int32::New(wrap->native_->enterprise[i]));
	//	}
	//	return scope.Close(ret);
	//}
	//
 //   static void Set_enterprise(v8::Local<v8::String> propertyName, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {   \
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, info.This());
	//	oid* new_value = wrap->native_->enterprise;
 //       size_t new_len = wrap->native_->enterprise_length;
	//	new_value = to_oid(value, new_value, &new_len);
	//	if(new_value != wrap->native_->enterprise) free(wrap->native_->enterprise);
 //       wrap->native_->enterprise = new_value;
 //       wrap->native_->enterprise_length = new_len;
	//}

	SNMP_ACCESSOR_DEFINE(Pdu, Int32, trap_type)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, specific_type)

	//	SNMP_SET_ACCESSOR(t, agent_addr)
	
	SNMP_ACCESSOR_DEFINE_USTRING(Pdu, securityEngineID, securityEngineIDLen)
	SNMP_ACCESSOR_DEFINE_USTRING(Pdu, contextEngineID, contextEngineIDLen)
	SNMP_ACCESSOR_DEFINE_STRING(Pdu, contextName, contextNameLen)
	SNMP_ACCESSOR_DEFINE_STRING(Pdu, securityName, securityNameLen)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, priority)
	SNMP_ACCESSOR_DEFINE(Pdu, Int32, range_subid)

	//static v8::Handle<v8::Value> Get_version(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, args.This());
	//	return v8::Int32::New(wrap->native_->version);
	//}
	//
	//static v8::Handle<v8::Value> Set_version(const v8::Arguments& args){
	//	v8::HandleScope scope;
	//	UNWRAP(Pdu, wrap, args.This());
	//	
	//	v8::Handle<v8::Value> value = args[0];
	//	if(!value->IsInt32()){
	//		return ThrowTypeError("arguments type must be integer");
	//	}

	//	wrap->native_->version = value->Int32Value();
	//	return v8::Undefined();
	//}
	
	
	static v8::Handle<v8::Value> GetVB(const netsnmp_variable_list* vb) {

		v8::Handle<v8::Object> ret = v8::Object::New();
		ret->Set(oid_symbol, from_oid(vb->name, vb->name_length));
		ret->Set(type_symbol, v8::Int32::New(vb->type));

		v8::Handle<v8::Value> exception;
		switch (vb->type) {
			case ASN_INTEGER:
			case ASN_UNSIGNED:
			case ASN_TIMETICKS:
			case ASN_COUNTER: {
				ret->Set(value_symbol, v8::Uint32::New(*(vb->val.integer)));
				break;
			}
			case ASN_OBJECT_ID:
			case ASN_PRIV_IMPLIED_OBJECT_ID:
			case ASN_PRIV_INCL_RANGE:
			case ASN_PRIV_EXCL_RANGE: {
				ret->Set(value_symbol, from_oid(vb->val.objid, vb->val_len));
				break;
			}
			case ASN_IPADDRESS: /* snmp_build_var_op treats IPADDR like a string */
			case ASN_PRIV_IMPLIED_OCTET_STR:
			case ASN_OCTET_STR:
			case ASN_BIT_STR:
			case ASN_OPAQUE:
			case ASN_NSAP: {
				ret->Set(value_symbol, v8::String::New((const char*)vb->val.string, vb->val_len));
				break;
			}
			case SNMP_NOSUCHOBJECT:
			case SNMP_NOSUCHINSTANCE:
			case SNMP_ENDOFMIBVIEW:
			case ASN_NULL: {
				ret->Set(value_symbol, v8::Null());
				break;
			}
		#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
			case ASN_OPAQUE_I64:
			case ASN_OPAQUE_U64:
		#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */
			case ASN_COUNTER64: {
				int64_t v = 0;
				memmove(&v, vb->val.counter64, vb->val_len);
				ret->Set(value_symbol, from_int64(v));
				break;
			}
		#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
			case ASN_OPAQUE_FLOAT: {
				ret->Set(value_symbol, v8::Number::New(*(vb->val.floatVal)));
				break;
			}

			case ASN_OPAQUE_DOUBLE: {
				ret->Set(value_symbol, v8::Number::New(*(vb->val.doubleVal)));
				break;
			}

		#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */

			default: {
				exception = v8::String::New("Internal error in type switching.");
				goto failure;
			}
		}

		return ret;
failure:
		return v8::ThrowException(exception);
	}
	
	static v8::Handle<v8::Value> AppendVB(netsnmp_pdu* pdu
		, v8::Handle<v8::Value>& hOid
		, v8::Handle<v8::Value>& hType
		, v8::Handle<v8::Value>& value) {

		oid     name_loc[MAX_OID_LEN];
		oid*    name = name_loc;
		size_t  name_len = sizeof(name_loc);

		if(NULL == to_oid(hOid, name, &name_len)) {
			return ThrowTypeError("argument name must be a int array.");
		}
		
		u_char type = (u_char)hType->Uint32Value();
		if('=' == type) {
		    v8::String::Utf8Value u8(value->ToString());
			int ret = snmp_add_var(pdu, name, name_len, type,  *u8);

			if(0 != ret){
				if(name != name_loc) {
					free(name);
				}
				std::string err = "argument value style error - ";
				err.append(snmp_api_errstring(ret));
				return ThrowSyntaxError(err.c_str());
			}
			return v8::Undefined();
		}

		netsnmp_variable_list *vars = 0;
		vars = snmp_varlist_add_variable(&vars, name, name_len, type, NULL, 0);
		if(name != name_loc) {
			free(name);
		}
		if(0 == vars) {
			return ThrowError("memory allocate error.");
		}

		v8::Handle<v8::Value> exception;
		switch (type) {
			case ASN_INTEGER:
			case ASN_UNSIGNED:
			case ASN_TIMETICKS:
			case ASN_COUNTER: {
				if (ASN_INTEGER == type) {
					*(vars->val.integer) = value->Int32Value();
				} else {
					*(vars->val.integer) = value->Uint32Value();
				}
				vars->val_len = 4;
				break;
            }
			case ASN_OBJECT_ID:
			case ASN_PRIV_IMPLIED_OBJECT_ID:
			case ASN_PRIV_INCL_RANGE:
			case ASN_PRIV_EXCL_RANGE: {
				name = vars->val.objid;
				name_len = sizeof(vars->buf);

				if(NULL == to_oid(value, name, &name_len)) {
					exception = v8::String::New("argument value must be a int array.");
					goto failure;
				}

				vars->val.objid = name;
				vars->val_len = name_len;
				break;
            }
			case ASN_IPADDRESS: /* snmp_build_var_op treats IPADDR like a string */

				/** FALL THROUGH */
			case ASN_PRIV_IMPLIED_OCTET_STR:
			case ASN_OCTET_STR:
			case ASN_BIT_STR:
			case ASN_OPAQUE:
			case ASN_NSAP: {
				v8::String::Utf8Value u8(value->ToString());
				vars->val_len = u8.length();
				if (ASN_IPADDRESS == type && 4 != vars->val_len) {
					exception = v8::String::New("ipaddress length must be equal 4.");
					goto failure;
				}

				if (vars->val_len >= sizeof(vars->buf)) {
					vars->val.string = (u_char *) malloc(vars->val_len + 1);
				}
				if (vars->val.string == NULL) {
					exception = v8::String::New("no storage for string.");
					goto failure;
				}
				memmove(vars->val.string, *u8, vars->val_len);
				/*
				 * Make sure the string is zero-terminated; some bits of code make
				 * this assumption.  Easier to do this here than fix all these wrong
				 * assumptions.  
				 */
				vars->val.string[vars->val_len] = '\0';
				break;
						   }
			case SNMP_NOSUCHOBJECT:
			case SNMP_NOSUCHINSTANCE:
			case SNMP_ENDOFMIBVIEW:
			case ASN_NULL:
				vars->val_len = 0;
				vars->val.string = NULL;
				break;

		#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
			case ASN_OPAQUE_I64:
			case ASN_OPAQUE_U64:
		#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */
			case ASN_COUNTER64: {
				int64_t v = value->IntegerValue();
				vars->val_len = sizeof(int64_t);
				memmove(vars->val.counter64, &v, vars->val_len);
				break;
			}
		#ifdef NETSNMP_WITH_OPAQUE_SPECIAL_TYPES
			case ASN_OPAQUE_FLOAT: {
				float v = (float)value->NumberValue();
				vars->val_len = sizeof(float);
				memmove(vars->val.floatVal, &v, vars->val_len);
				break;
			}

			case ASN_OPAQUE_DOUBLE: {
				double v = value->NumberValue();
				vars->val_len = sizeof(double);
				memmove(vars->val.counter64, &v, vars->val_len);
				break;
			}

		#endif                          /* NETSNMP_WITH_OPAQUE_SPECIAL_TYPES */

			default: {
				exception = v8::String::New("Internal error in type switching.");
				goto failure;
			}
		}

		if (0 == pdu->variables) {
			pdu->variables = vars;
		} else {
			netsnmp_variable_list* vtmp = pdu->variables;
			for (; vtmp->next_variable;
				 vtmp = vtmp->next_variable);

			vtmp->next_variable = vars;
		}

		return v8::Undefined();
failure:
		if(0 != vars) {
			snmp_free_var(vars);
		}
		return v8::ThrowException(exception);
	}

	static v8::Handle<v8::Value> Append(const v8::Arguments& args) {
		v8::HandleScope scope;
		if(3 > args.Length()) {
			return ThrowError("Must pass pdu, name, value and option type arguments to set.");
		}
		if(4 < args.Length()) {
			return ThrowError("Arguments too much.");
		}
		
		v8::Handle<v8::Value> type = v8::Undefined();
		v8::Local<v8::Value> value;


		if(3 == args.Length()) {
			value = args[2];
		} else {
			type  = args[2];
			value = args[3];
		}

		UNWRAP(Pdu, wrap, args[0]->ToObject());
		return AppendVB(wrap->native_, args[1], type, value); 
	}

	
	static v8::Handle<v8::Value> Get(const v8::Arguments& args) {
		v8::HandleScope scope;
		if(2 != args.Length()) {
			return ThrowError("Must pass pdu, and index arguments to get.");
		}
		

		UNWRAP(Pdu, wrap, args[0]->ToObject());
		int idx = args[1]->Int32Value();
		
		netsnmp_variable_list* vtmp = wrap->native_->variables;
		for(int i = 0; i < idx; ++ i) {
			if(0 == vtmp) {
				return ThrowRangeError("Index too big.");
			}
			vtmp = vtmp->next_variable;
		}
		if(0 == vtmp) {
			return v8::Undefined();
		}

		return GetVB(vtmp); 
	}
	static v8::Handle<v8::Value> Clear(const v8::Arguments& args) {
		v8::HandleScope scope;
		if(1 != args.Length()) {
			return ThrowError("Must pass pdu arguments to get.");
		}

		UNWRAP(Pdu, wrap, args[0]->ToObject());
		snmp_free_varbind(wrap->native_->variables);
		wrap->native_->variables = 0;
		return v8::Undefined();
	}

	static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
		v8::HandleScope scope;
		UNWRAP(Pdu, wrap, args.This());
		
		wrap->close();
		return v8::Undefined();
	}

	static v8::Handle<v8::Value> fatchPdu(const netsnmp_pdu *pdu) {
		v8::Handle<v8::Object> result = v8::Object::New();
		result->Set(version_symbol, from_long(pdu->version));
		result->Set(command_symbol, from_long(pdu->command));
		result->Set(reqid_symbol, from_long(pdu->reqid));
		result->Set(msgid_symbol, from_long(pdu->msgid));
		result->Set(transid_symbol, from_long(pdu->transid));
		result->Set(sessid_symbol, from_long(pdu->sessid));
		result->Set(errstat_symbol, from_long(pdu->errstat));
		result->Set(errindex_symbol, from_long(pdu->errindex));
		result->Set(time_symbol, from_ulong(pdu->time));
		result->Set(flags_symbol, from_ulong(pdu->flags));

		result->Set(securityModel_symbol, from_int32(pdu->securityModel));
		result->Set(securityLevel_symbol, from_int32(pdu->securityLevel));
		result->Set(msgParseModel_symbol, from_int32(pdu->msgParseModel));

		if(0 < pdu->community_len) {
			result->Set(community_symbol, from_uchar(pdu->community, pdu->community_len));
		}
		if(SNMP_MSG_TRAP == pdu->command) {
			if(0 < pdu->enterprise_length) {
				result->Set(enterprise_symbol, from_oid(pdu->enterprise, pdu->enterprise_length));
			}

		    result->Set(trap_type_symbol, from_long(pdu->trap_type));
		    result->Set(specific_type_symbol, from_long(pdu->specific_type));
		    //result->Set(agent_addr_symbol, from_long(pdu->agent_addr));
		}

		return v8::Undefined();
	}
};

#endif //__pdu__cpp__ 