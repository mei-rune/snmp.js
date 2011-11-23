
var native = require('./snmp'),
    util   = require('util');

exports.Native = native;
exports.Constants = native.Constants;



var arrayEqual = function(a, b)  {
   if (a.length != b.length) { return false; } 

   for (var i = 0; i < a.length; ++ i) {
       if (a[i] !== b[i]) {
           return false;
       } 
   }
   return true;
};

var readOnly = function(name, value) {
   return { set : function(new_value, cb) {
                       var err = new Error(name + " is readonly.");
                       if(cb){ cb(err); return; } else { throw err; }
                   },
            get : function() { return value; },
          };
};

var defineReadOnlyProperty = function(obj, name, value){
    Object.defineProperty(obj, name, readOnly(name, value) );
}

exports.PDUTYPE = { };
defineReadOnlyProperty(exports.PDUTYPE, "GET", native.Constants.SNMP_MSG_GET);
defineReadOnlyProperty(exports.PDUTYPE, "GETNEXT", native.Constants.SNMP_MSG_GETNEXT);
defineReadOnlyProperty(exports.PDUTYPE, "NEXT", native.Constants.SNMP_MSG_GETNEXT);
defineReadOnlyProperty(exports.PDUTYPE, "SET", native.Constants.SNMP_MSG_SET);
defineReadOnlyProperty(exports.PDUTYPE, "RESPONSE", native.Constants.SNMP_MSG_RESPONSE);
defineReadOnlyProperty(exports.PDUTYPE, "BULK", native.Constants.SNMP_MSG_GETBULK);
defineReadOnlyProperty(exports.PDUTYPE, "GETBULK", native.Constants.SNMP_MSG_GETBULK);
defineReadOnlyProperty(exports.PDUTYPE, "TRAP", native.Constants.SNMP_MSG_TRAP);
defineReadOnlyProperty(exports.PDUTYPE, "INFORM", native.Constants.SNMP_MSG_INFORM);
defineReadOnlyProperty(exports.PDUTYPE, "TRAP2", native.Constants.SNMP_MSG_TRAP2);
defineReadOnlyProperty(exports.PDUTYPE, "REPORT", native.Constants.SNMP_MSG_REPORT);

exports.SNMP_VSERION = {};
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_1", native.Constants.SNMP_VERSION_1);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_2c", native.Constants.SNMP_VERSION_2c);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_2u", native.Constants.SNMP_VERSION_2u);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_3", native.Constants.SNMP_VERSION_3);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_sec", native.Constants.SNMP_VERSION_sec);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_2p", native.Constants.SNMP_VERSION_2p);
defineReadOnlyProperty(exports.SNMP_VSERION, "SNMP_VERSION_2star", native.Constants.SNMP_VERSION_2star);

exports.DATA_TYPE = { };
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_INTEGER", native.Constants.ASN_INTEGER);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_UNSIGNED", native.Constants.ASN_UNSIGNED);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_TIMETICKS", native.Constants.ASN_TIMETICKS);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_COUNTER", native.Constants.ASN_COUNTER);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OBJECT_ID", native.Constants.ASN_OBJECT_ID);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_PRIV_IMPLIED_OBJECT_ID", native.Constants.ASN_PRIV_IMPLIED_OBJECT_ID);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_PRIV_INCL_RANGE", native.Constants.ASN_PRIV_INCL_RANGE);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_PRIV_EXCL_RANGE", native.Constants.ASN_PRIV_EXCL_RANGE);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_IPADDRESS", native.Constants.ASN_IPADDRESS);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_PRIV_IMPLIED_OCTET_STR", native.Constants.ASN_PRIV_IMPLIED_OCTET_STR);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OCTET_STR", native.Constants.ASN_OCTET_STR);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_BIT_STR", native.Constants.ASN_BIT_STR);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OPAQUE", native.Constants.ASN_OPAQUE);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_NSAP", native.Constants.ASN_NSAP);
defineReadOnlyProperty(exports.DATA_TYPE, "SNMP_NOSUCHOBJECT", native.Constants.SNMP_NOSUCHOBJECT);
defineReadOnlyProperty(exports.DATA_TYPE, "SNMP_NOSUCHINSTANCE", native.Constants.SNMP_NOSUCHINSTANCE);
defineReadOnlyProperty(exports.DATA_TYPE, "SNMP_ENDOFMIBVIEW",  native.Constants.SNMP_ENDOFMIBVIEW);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_NULL", native.Constants.ASN_NULL);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OPAQUE_I64", native.Constants.ASN_OPAQUE_I64);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OPAQUE_U64", native.Constants.ASN_OPAQUE_U64);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_COUNTER64", native.Constants.ASN_COUNTER64);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OPAQUE_FLOAT", native.Constants.ASN_OPAQUE_FLOAT);
defineReadOnlyProperty(exports.DATA_TYPE, "ASN_OPAQUE_DOUBLE", native.Constants.ASN_OPAQUE_DOUBLE);



exports.createPdu = function (pduType) {
    switch(pduType) {
       case "get":
          pduType = native.Constants.SNMP_MSG_GET;
          break;
       case "getnext":
       case "next":
          pduType = native.Constants.SNMP_MSG_GETNEXT;
          break;
       case "set":
          pduType = native.Constants.SNMP_MSG_SET;
          break;
       case "respone":
          pduType = native.Constants.SNMP_MSG_RESPONSE;
          break;
       case "getbulk":
       case "bulk":
          pduType = native.Constants.SNMP_MSG_GETBULK;
          break;
       case "trap":
          pduType = native.Constants.SNMP_MSG_TRAP;
          break;
       case "inform":
          pduType = native.Constants.SNMP_MSG_INFORM;
          break;
       case "trap2":
          pduType = native.Constants.SNMP_MSG_TRAP2;
          break;
       case "report":
          pduType = native.Constants.SNMP_MSG_REPORT;
          break;
    };

    var pdu = new native.Pdu(pduType);

    var variableBings = {
       clear : function() {
           native.clearVariableList(pdu);
       }, 

       add : function(name) {
            type = arguments[1];
            value = arguments[2];
            if(value) {
                return native.appendVariableList(pdu, name, type, value);
            } else if(type){
                value = type;
                return native.appendVariableList(pdu, name, value);
            } else {
                return native.appendVariableList(pdu, name, null);
            }
       },

       get : function(name) {
           if(typeof(name) === 'number') {
                return native.getVariableList(pdu, idx);
           }

           if(!util.isArray(name)) {
               throw "Must pass a int or int Array.";
           }

           for(var idx = 0;  ; ++ idx) {
               var vb = native.getVariableList(pdu, idx);
               if(!vb) return ;
               if(arrayEqual(vb.oid, name)) {
                   return vb;
               }
           }
       },
    };

    defineReadOnlyProperty(pdu, "variableBindings", variableBings);
    //pdu.__defineSetter__("variableBindings", readOnly("SNMP_VERSION_1"));

    return pdu;
};

