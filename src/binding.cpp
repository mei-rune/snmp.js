
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


SNMP_DEFINE_SYMBOL(Pdu);
SNMP_DEFINE_SYMBOL(oid);
SNMP_DEFINE_SYMBOL(type);
SNMP_DEFINE_SYMBOL(value);

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
SNMP_DEFINE_SYMBOL(variableBindings);


SNMP_DEFINE_SYMBOL(Session);
SNMP_DEFINE_SYMBOL(process);

SNMP_DEFINE_SYMBOL(retries);
SNMP_DEFINE_SYMBOL(timeout);
SNMP_DEFINE_SYMBOL(peername);
SNMP_DEFINE_SYMBOL(remote_port);
SNMP_DEFINE_SYMBOL(localname);
SNMP_DEFINE_SYMBOL(local_port);

SNMP_DEFINE_SYMBOL(rcvMsgMaxSize);
SNMP_DEFINE_SYMBOL(sndMsgMaxSize);

SNMP_DEFINE_SYMBOL(isAuthoritative);
SNMP_DEFINE_SYMBOL(engineBoots);
SNMP_DEFINE_SYMBOL(engineTime);
SNMP_DEFINE_SYMBOL(securityAuthProto);
SNMP_DEFINE_SYMBOL(securityAuthKey);
SNMP_DEFINE_SYMBOL(securityAuthLocalKey);
SNMP_DEFINE_SYMBOL(securityPrivProto);
SNMP_DEFINE_SYMBOL(securityPrivKey);
SNMP_DEFINE_SYMBOL(securityPrivLocalKey);
SNMP_DEFINE_SYMBOL(paramName);



SNMP_DEFINE_SYMBOL(socket);
SNMP_DEFINE_SYMBOL(tv_sec);
SNMP_DEFINE_SYMBOL(tv_usec);

SNMP_DEFINE_SYMBOL(require);
SNMP_DEFINE_SYMBOL(dgram);
SNMP_DEFINE_SYMBOL(udp4);
SNMP_DEFINE_SYMBOL(udp6);
SNMP_DEFINE_SYMBOL(createSocket);
SNMP_DEFINE_SYMBOL(bind);


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


    SNMP_INIT_SYMBOL(Pdu);
    SNMP_INIT_SYMBOL(oid);
    SNMP_INIT_SYMBOL(type);
    SNMP_INIT_SYMBOL(value);

    SNMP_INIT_SYMBOL(version);
    SNMP_INIT_SYMBOL(command);
    SNMP_INIT_SYMBOL(reqid);
    SNMP_INIT_SYMBOL(msgid);
    SNMP_INIT_SYMBOL(transid);
    SNMP_INIT_SYMBOL(sessid);
    SNMP_INIT_SYMBOL(errstat);
    SNMP_INIT_SYMBOL(errindex);
    SNMP_INIT_SYMBOL(time);
    SNMP_INIT_SYMBOL(flags);
    SNMP_INIT_SYMBOL(securityModel);
    SNMP_INIT_SYMBOL(securityLevel);
    SNMP_INIT_SYMBOL(msgParseModel);
    SNMP_INIT_SYMBOL(tDomain);
    SNMP_INIT_SYMBOL(community);
    SNMP_INIT_SYMBOL(enterprise);
    SNMP_INIT_SYMBOL(trap_type);
    //SNMP_INIT_SYMBOL(agent_addr);
    SNMP_INIT_SYMBOL(specific_type);
    SNMP_INIT_SYMBOL(contextEngineID);
    SNMP_INIT_SYMBOL(contextName);
    SNMP_INIT_SYMBOL(securityEngineID);
    SNMP_INIT_SYMBOL(securityName);
    SNMP_INIT_SYMBOL(priority);
    SNMP_INIT_SYMBOL(range_subid);
    SNMP_INIT_SYMBOL(variableBindings);


    SNMP_INIT_SYMBOL(Session);
    SNMP_INIT_SYMBOL(process);

    SNMP_INIT_SYMBOL(retries);
    SNMP_INIT_SYMBOL(timeout);
    SNMP_INIT_SYMBOL(peername);
    SNMP_INIT_SYMBOL(remote_port);
    SNMP_INIT_SYMBOL(localname);
    SNMP_INIT_SYMBOL(local_port);

    SNMP_INIT_SYMBOL(sessid);
    SNMP_INIT_SYMBOL(rcvMsgMaxSize);
    SNMP_INIT_SYMBOL(sndMsgMaxSize);

    SNMP_INIT_SYMBOL(isAuthoritative);
    SNMP_INIT_SYMBOL(engineBoots);
    SNMP_INIT_SYMBOL(engineTime);
    SNMP_INIT_SYMBOL(securityAuthProto);
    SNMP_INIT_SYMBOL(securityAuthKey);
    SNMP_INIT_SYMBOL(securityAuthLocalKey);
    SNMP_INIT_SYMBOL(securityPrivProto);
    SNMP_INIT_SYMBOL(securityPrivKey);
    SNMP_INIT_SYMBOL(securityPrivLocalKey);
    SNMP_INIT_SYMBOL(securityModel);
    SNMP_INIT_SYMBOL(securityLevel);
    SNMP_INIT_SYMBOL(paramName);

    SNMP_INIT_SYMBOL(socket);
    SNMP_INIT_SYMBOL(tv_sec);
    SNMP_INIT_SYMBOL(tv_usec);

	SNMP_INIT_SYMBOL(require);
	SNMP_INIT_SYMBOL(dgram);
	SNMP_INIT_SYMBOL(udp4);
	SNMP_INIT_SYMBOL(udp6);
	SNMP_INIT_SYMBOL(createSocket);
	SNMP_INIT_SYMBOL(bind);

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
                     ) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
