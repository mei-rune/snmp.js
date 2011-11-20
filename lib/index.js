
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

var readOnly = function(name) {
   return function(value, cb) {
       var err = new Error(name + " is readonly.");
       if(cb){
          cb(err);
          return;
       }
       throw err;
   }
};

exports.PDUTYPE = { };
Object.defineProperty(exports.PDUTYPE, "GET", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_GET; } } );
Object.defineProperty(exports.PDUTYPE, "GETNEXT", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_GETNEXT; } } );
Object.defineProperty(exports.PDUTYPE, "NEXT", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_GETNEXT; } } );
Object.defineProperty(exports.PDUTYPE, "SET", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_SET; } } );
Object.defineProperty(exports.PDUTYPE, "RESPONSE", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_RESPONSE; } } );
Object.defineProperty(exports.PDUTYPE, "BULK", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_GETBULK; } } );
Object.defineProperty(exports.PDUTYPE, "GETBULK", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_GETBULK; } } );
Object.defineProperty(exports.PDUTYPE, "TRAP", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_TRAP; } } );
Object.defineProperty(exports.PDUTYPE, "INFORM", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_INFORM; } } );
Object.defineProperty(exports.PDUTYPE, "TRAP2", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_TRAP2; } } );
Object.defineProperty(exports.PDUTYPE, "REPORT", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_MSG_REPORT; } } );



exports.SNMP_VSERION = {};
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_1", { set : readOnly("SNMP_VERSION_1"), get : function() { return native.Constants.SNMP_VERSION_1; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_2c", { set : readOnly("SNMP_VERSION_2c"), get : function() { return native.Constants.SNMP_VERSION_2c; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_2u", { set : readOnly("SNMP_VERSION_2u"), get : function() { return native.Constants.SNMP_VERSION_2u; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_3", { set : readOnly("SNMP_VERSION_3"), get : function() { return native.Constants.SNMP_VERSION_3; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_sec", { set : readOnly("SNMP_VERSION_sec"), get : function() { return native.Constants.SNMP_VERSION_sec; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_2p", { set : readOnly("SNMP_VERSION_2p"), get : function() { return native.Constants.SNMP_VERSION_2p; } } );
Object.defineProperty(exports.SNMP_VSERION, "SNMP_VERSION_2star", { set : readOnly("SNMP_VERSION_2star"), get : function() { return native.Constants.SNMP_VERSION_2star; } } );


exports.DATA_TYPE = { };

Object.defineProperty(exports.DATA_TYPE, "ASN_INTEGER", { set : readOnly("ASN_INTEGER"), get : function() { return native.Constants.ASN_INTEGER; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_UNSIGNED", { set : readOnly("ASN_UNSIGNED"), get : function() { return native.Constants.ASN_UNSIGNED; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_TIMETICKS", { set : readOnly("ASN_TIMETICKS"), get : function() { return native.Constants.ASN_TIMETICKS; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_COUNTER", { set : readOnly("ASN_COUNTER"), get : function() { return native.Constants.ASN_COUNTER; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_OBJECT_ID", { set : readOnly("ASN_OBJECT_ID"), get : function() { return native.Constants.ASN_OBJECT_ID; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_PRIV_IMPLIED_OBJECT_ID", { set : readOnly("ASN_PRIV_IMPLIED_OBJECT_ID"), get : function() { return native.Constants.ASN_PRIV_IMPLIED_OBJECT_ID; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_PRIV_INCL_RANGE", { set : readOnly("ASN_PRIV_INCL_RANGE"), get : function() { return native.Constants.ASN_PRIV_INCL_RANGE; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_PRIV_EXCL_RANGE", { set : readOnly("ASN_PRIV_EXCL_RANGE"), get : function() { return native.Constants.ASN_PRIV_EXCL_RANGE; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_IPADDRESS", { set : readOnly("ASN_IPADDRESS"), get : function() { return native.Constants.ASN_IPADDRESS; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_PRIV_IMPLIED_OCTET_STR", { set : readOnly("ASN_PRIV_IMPLIED_OCTET_STR"), get : function() { return native.Constants.ASN_PRIV_IMPLIED_OCTET_STR; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_OCTET_STR", { set : readOnly("ASN_OCTET_STR"), get : function() { return native.Constants.ASN_OCTET_STR; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_BIT_STR", { set : readOnly("ASN_BIT_STR"), get : function() { return native.Constants.ASN_BIT_STR; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_OPAQUE", { set : readOnly("ASN_OPAQUE"), get : function() { return native.Constants.ASN_OPAQUE; } } );
Object.defineProperty(exports.DATA_TYPE, "ASN_NSAP", { set : readOnly("ASN_NSAP"), get : function() { return native.Constants.ASN_NSAP; } } );
Object.defineProperty(exports.DATA_TYPE, "SNMP_NOSUCHOBJECT", { set : readOnly("SNMP_NOSUCHOBJECT"), get : function() { return native.Constants.SNMP_NOSUCHOBJECT; } } );
Object.defineProperty(exports.DATA_TYPE, "SNMP_NOSUCHINSTANCE", { set : readOnly("SNMP_NOSUCHINSTANCE"), get : function() { return native.Constants.SNMP_NOSUCHINSTANCE; } } );
Object.defineProperty(exports.DATA_TYPE, "SNMP_ENDOFMIBVIEW", { set : readOnly("SNMP_ENDOFMIBVIEW"), get : function() { return native.Constants.SNMP_ENDOFMIBVIEW; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_NULL", { set : readOnly("ASN_NULL"), get : function() { return native.Constants.ASN_NULL; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_OPAQUE_I64", { set : readOnly("ASN_OPAQUE_I64"), get : function() { return native.Constants.ASN_OPAQUE_I64; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_OPAQUE_U64", { set : readOnly("ASN_OPAQUE_U64"), get : function() { return native.Constants.ASN_OPAQUE_U64; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_COUNTER64", { set : readOnly("ASN_COUNTER64"), get : function() { return native.Constants.ASN_COUNTER64; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_OPAQUE_FLOAT", { set : readOnly("ASN_OPAQUE_FLOAT"), get : function() { return native.Constants.ASN_OPAQUE_FLOAT; } });
Object.defineProperty(exports.DATA_TYPE, "ASN_OPAQUE_DOUBLE", { set : readOnly("ASN_OPAQUE_DOUBLE"), get : function() { return native.Constants.ASN_OPAQUE_DOUBLE; } });



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

    Object.defineProperty(pdu, "variableBindings", { set : readOnly("variableBindings"), get : function() { return variableBings; } });
    //pdu.__defineSetter__("variableBindings", readOnly("SNMP_VERSION_1"));

    return pdu;
};

