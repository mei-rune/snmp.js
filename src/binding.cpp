
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

#include "snmp.h"
#include "pdu.cpp"
#include "session.cpp"


static v8::Handle<v8::Value> ParseOid(const v8::Arguments& args) {
		v8::HandleScope scope;
		
	oid name_loc[MAX_OID_LEN];
	oid* name = name_loc;
	size_t name_len = sizeof(name_loc);

	name = to_oid(args[0], name, &name_len);
	if(0 == name) {
		return v8::Undefined();
	}

	v8::Handle<v8::Value> ret = from_oid(name, name_len);
	if(name != name_loc) {
		free(name);
	}
	return scope.Close(ret);
}


// --- Entry point ---
#ifdef _MSC_VER 
extern "C" __declspec (dllexport)
#else
extern
#endif
void init(v8::Handle<v8::Object> target) {
    // init net-snmp
    netsnmp_session session;
    snmp_sess_init(&session);

    v8::HandleScope scope;

	
	v8::Local<v8::Object> constants = v8::Object::New();    

    NODE_DEFINE_CONSTANT(constants, ASN_BOOLEAN);
    NODE_DEFINE_CONSTANT(constants, ASN_INTEGER);
    NODE_DEFINE_CONSTANT(constants, ASN_BIT_STR);
    NODE_DEFINE_CONSTANT(constants, ASN_OCTET_STR);
    NODE_DEFINE_CONSTANT(constants, ASN_NULL);
    NODE_DEFINE_CONSTANT(constants, ASN_OBJECT_ID);
    NODE_DEFINE_CONSTANT(constants, ASN_SEQUENCE);
    NODE_DEFINE_CONSTANT(constants, ASN_SET);

    NODE_DEFINE_CONSTANT(constants, ASN_UNIVERSAL);
    NODE_DEFINE_CONSTANT(constants, ASN_APPLICATION);
    NODE_DEFINE_CONSTANT(constants, ASN_CONTEXT);
    NODE_DEFINE_CONSTANT(constants, ASN_PRIVATE);
		
    NODE_DEFINE_CONSTANT(constants, ASN_PRIMITIVE);
    NODE_DEFINE_CONSTANT(constants, ASN_CONSTRUCTOR);
	
    NODE_DEFINE_CONSTANT(constants, ASN_LONG_LEN);
    NODE_DEFINE_CONSTANT(constants, ASN_EXTENSION_ID);
    NODE_DEFINE_CONSTANT(constants, ASN_BIT8);


	
    NODE_DEFINE_CONSTANT(constants, SNMP_PORT);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_PORT);
	
    /*
     * versions based on version field 
     */
#ifndef NETSNMP_DISABLE_SNMPV1
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_1);
#endif
#ifndef NETSNMP_DISABLE_SNMPV2C
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_2c);
#endif
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_2u);
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_3);
	
    /*
     * versions not based on a version field 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_sec);
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_2p);
    NODE_DEFINE_CONSTANT(constants, SNMP_VERSION_2star);


	
    /*
     * PDU types in SNMPv1, SNMPsec, SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_GET);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_GETNEXT);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_RESPONSE);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_SET);

    /*
     * PDU types in SNMPv1 and SNMPsec 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_TRAP);

    /*
     * PDU types in SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_GETBULK);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_INFORM);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_TRAP2);

	
    /*
     * PDU types in SNMPv2u, SNMPv2*, and SNMPv3 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_REPORT);


	
    /*
     * Exception values for SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_NOSUCHOBJECT);
    NODE_DEFINE_CONSTANT(constants, SNMP_NOSUCHINSTANCE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ENDOFMIBVIEW);
	
    /*
     * Error codes (the value of the field error-status in PDUs) 
     */

    /*
     * in SNMPv1, SNMPsec, SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 PDUs 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_NOERROR);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_TOOBIG);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_NOSUCHNAME);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_BADVALUE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_READONLY);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_GENERR);
	
    /*
     * in SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 PDUs 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_NOACCESS);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_WRONGTYPE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_WRONGLENGTH);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_WRONGENCODING);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_WRONGVALUE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_NOCREATION);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_INCONSISTENTVALUE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_RESOURCEUNAVAILABLE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_COMMITFAILED);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_UNDOFAILED);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_AUTHORIZATIONERROR);
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_NOTWRITABLE);
	
    /*
     * in SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 PDUs 
     */	
    NODE_DEFINE_CONSTANT(constants, SNMP_ERR_INCONSISTENTNAME);




	
    /*
     * values of the generic-trap field in trap PDUs 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_COLDSTART);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_WARMSTART);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_LINKDOWN);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_LINKUP);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_AUTHFAIL);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_EGPNEIGHBORLOSS);
    NODE_DEFINE_CONSTANT(constants, SNMP_TRAP_ENTERPRISESPECIFIC);

    /*
     * row status values 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_NONEXISTENT);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_ACTIVE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_NOTINSERVICE);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_NOTREADY);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_CREATEANDGO);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_CREATEANDWAIT);
    NODE_DEFINE_CONSTANT(constants, SNMP_ROW_DESTROY);

    /*
     * row storage values 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_NONE);
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_OTHER);
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_VOLATILE);
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_NONVOLATILE);
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_PERMANENT);
    NODE_DEFINE_CONSTANT(constants, SNMP_STORAGE_READONLY);

    /*
     * message processing models 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_MP_MODEL_SNMPv1);
    NODE_DEFINE_CONSTANT(constants, SNMP_MP_MODEL_SNMPv2c);
    NODE_DEFINE_CONSTANT(constants, SNMP_MP_MODEL_SNMPv2u);
    NODE_DEFINE_CONSTANT(constants, SNMP_MP_MODEL_SNMPv3);
    NODE_DEFINE_CONSTANT(constants, SNMP_MP_MODEL_SNMPv2p);

    /*
     * security values 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_ANY);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_SNMPv1);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_SNMPv2c);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_USM);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_TSM);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_MODEL_SNMPv2p);

    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_LEVEL_NOAUTH);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_LEVEL_AUTHNOPRIV);
    NODE_DEFINE_CONSTANT(constants, SNMP_SEC_LEVEL_AUTHPRIV);

    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_FLAG_AUTH_BIT);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_FLAG_PRIV_BIT);
    NODE_DEFINE_CONSTANT(constants, SNMP_MSG_FLAG_RPRT_BIT);

    /*
     * control PDU handling characteristics 
     */
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_RESPONSE_PDU );
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_EXPECT_RESPONSE);
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_FORCE_PDU_COPY);
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_ALWAYS_IN_VIEW);
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_PDU_TIMEOUT);
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_ONE_PASS_ONLY);
    NODE_DEFINE_CONSTANT(constants, UCD_MSG_FLAG_TUNNELED);

    /*
     * view status 
     */
    NODE_DEFINE_CONSTANT(constants, SNMP_VIEW_INCLUDED);
    NODE_DEFINE_CONSTANT(constants, SNMP_VIEW_EXCLUDED);

    /*
     * basic oid values 
     */
    SNMP_DEFINE_CONSTANT_VALUE(target, SNMP_OID_INTERNET,    newIntArray(1, 3, 6, 1));
    SNMP_DEFINE_CONSTANT_VALUE(target, SNMP_OID_ENTERPRISES, newIntArray(SNMP_OID_INTERNET, 4, 1));
    SNMP_DEFINE_CONSTANT_VALUE(target, SNMP_OID_MIB2,        newIntArray(SNMP_OID_INTERNET, 2, 1));
    SNMP_DEFINE_CONSTANT_VALUE(target, SNMP_OID_SNMPV2,      newIntArray(SNMP_OID_INTERNET, 6));
    SNMP_DEFINE_CONSTANT_VALUE(target, SNMP_OID_SNMPMODULES, newIntArray(SNMP_OID_SNMPV2, 3));

    /*
     * lengths as defined by TCs 
     */
    NODE_DEFINE_CONSTANT(constants, SNMPADMINLENGTH);


        /*
     * This routine must be supplied by the application:
     *
     * int callback(operation, session, reqid, pdu, magic)
     * int operation;
     * netsnmp_session *session;    The session authenticated under.
     * int reqid;                       The request id of this pdu (0 for TRAP)
     * netsnmp_pdu *pdu;        The pdu information.
     * void *magic                      A link to the data for this routine.
     *
     * Returns 1 if request was successful, 0 if it should be kept pending.
     * Any data in the pdu must be copied because it will be freed elsewhere.
     * Operations are defined below:
     */

    NODE_DEFINE_CONSTANT(constants, NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE);
    NODE_DEFINE_CONSTANT(constants, NETSNMP_CALLBACK_OP_TIMED_OUT);
    NODE_DEFINE_CONSTANT(constants, NETSNMP_CALLBACK_OP_SEND_FAILED);
    NODE_DEFINE_CONSTANT(constants, NETSNMP_CALLBACK_OP_CONNECT);
    NODE_DEFINE_CONSTANT(constants, NETSNMP_CALLBACK_OP_DISCONNECT);

	

	target->Set(v8::String::NewSymbol("Constants"), constants);
	
	target->Set(v8::String::NewSymbol("parseOid"), v8::FunctionTemplate::New(ParseOid)->GetFunction());
	

    Session::Initialize(target);
    Pdu::Initialize(target);

    //session_initialize(target);
	//pdu_initialize(target);

	
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
