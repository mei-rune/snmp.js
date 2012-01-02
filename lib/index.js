
var native = require('./snmp'),
    util = require('util'),
    dgram = require('dgram');

exports.Native = native;
exports.Constants = native.Constants;



var arrayEqual = function(a, b)  {
   var i;
   if (a.length !== b.length) { return false; } 

   for (i = 0; i < a.length; i+=1) {
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
            get : function() { return value; }
          };
};

var defineReadOnlyProperty = function(obj, name, value){
    Object.defineProperty(obj, name, readOnly(name, value) );
};

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

exports.SNMP_VERSION = {};
defineReadOnlyProperty(exports.SNMP_VERSION, "v1", native.Constants.SNMP_VERSION_1);
defineReadOnlyProperty(exports.SNMP_VERSION, "v2c", native.Constants.SNMP_VERSION_2c);
defineReadOnlyProperty(exports.SNMP_VERSION, "v2u", native.Constants.SNMP_VERSION_2u);
defineReadOnlyProperty(exports.SNMP_VERSION, "v3", native.Constants.SNMP_VERSION_3);
defineReadOnlyProperty(exports.SNMP_VERSION, "vsec", native.Constants.SNMP_VERSION_sec);
defineReadOnlyProperty(exports.SNMP_VERSION, "v2p", native.Constants.SNMP_VERSION_2p);
defineReadOnlyProperty(exports.SNMP_VERSION, "v2star", native.Constants.SNMP_VERSION_2star);

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


var stringToPduType = function(pduType){
  
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
    }
    return pduType;
}


var VariableBindings = function(pdu) {

  this.clear = function() {
           native.clearVariableList(pdu);
       };

  this.add = function(name, type, value) {
            if(value || type){
                return native.appendVariableList(pdu, name, type, value);
            } else {
                return native.appendVariableList(pdu, name, null);
            }
       };

  this.get = function(name) {
           var vb, idx;

           if(typeof(name) === 'number') {
                return native.getVariableList(pdu, name);
           }

           if(!util.isArray(name)) {
               throw "Must pass a int or int Array.";
           }

           idx = 0;
           while(true) {
              vb = native.getVariableList(pdu, idx);
              if(!vb) { return ; }
              if(arrayEqual(vb.oid, name)) {
                  return vb;
              }
              idx += 1;
           }
       };
};

// for test
exports.createPduFromObject = function (obj) {
  var pdu;

  obj.command = stringToPduType(obj.command);
  pdu = native.toNativePdu(obj);

  defineReadOnlyProperty(pdu, "variableBindings", new VariableBindings(pdu));
  return pdu;
}

exports.createPdu = function (pduType) {
    var pdu, variableBings;
    
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
    }

    pdu = new native.Pdu(pduType);


    defineReadOnlyProperty(pdu, "variableBindings", new VariableBindings(pdu));

    pdu.toJS = function(){
      return native.fromNativePdu(pdu);
    }

    return pdu;
};


exports.createSession = function () {
    var session = new native.Session();
    session.on_open = function (type, port, address) {
        var timeout;

        this.client = dgram.createSocket(type, function (msg, rinfo) {
            session._onData(msg, rinfo);
        });

        timeout = session.timeout / 1000;
        if (timeout < 1000) {
            timeout = 1000;
        } else if (timeout > 10000) {
            timeout = 10000;
        }

        this.timer = setInterval(function () { session.on_timeout(); }, timeout);
        //this.client.bind(port, address);
    };

    session.on_close = function () {
        this.client.close();
        clearInterval(this.timer);
    };

    session.on_timeout = function () {
        session._onTimeout();
    };

    session.on_send = function (message, port, address) {
        this.client.send(message, 0, message.length, port, address, function (err, bytes) {
            if (err) {
                session._onData("", { "port": 0, "address": "0.0.0.0" });
            }
        });
    };

    session.send = function (pdu, cb) {
        if (pdu instanceof native.Pdu) {
            this._sendNativePdu(pdu, cb);
        } else {
            this._sendPdu(pdu, cb);
        }
    };

    session.sendSync = function (pdu) {
        var response;
        var e;

        var cb = function (err, request, res) {
            if (err) {
                e = new Error(err);
            } else {
                response = res;
            }
        }
        this.send(pdu, cb);
        this._readData();
        if (e) {
            throw e;
        }
        return response;
    };

    session.open = function () {
        var s;
        var idx = this.peername.indexOf(":");
        if (-1 != idx) {
            s = this.peername.substr(0, idx);
            switch (s) {
                case "udp":
                    this.peername = "mei" + this.peername.slice(idx, -1);
                    break;
                case "udp6":
                    this.peername = "mei6" + this.peername.slice(idx, -1);
                    break;
            }
        } else {
            this.peername = "mei:" + this.peername;
        }
        return this._open();
    };


    session.openSync = function () {
        var s;
        var idx = this.peername.indexOf(":");
        if (-1 != idx) {
            s = this.peername.substr(0, idx);
            switch (s) {
                case "mei":
                    this.peername = this.peername.slice(idx + 1, -1);
                    break;
                case "mei6":
                    this.peername = this.peername.slice(idx + 1, -1);
                    break;
            }
        }
        return this._open();
    };

    session.close = function () {
        process.nextTick(function () {
            session._close();
        });
    };

    return session;
};
