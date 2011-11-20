
var native = require('./snmp'),
    util = require('util');

var Pdu = function (type) {
    native.Pdu.call(this, type);
};

var Pdu = native.Pdu;

Pdu.prototype.aa = function () { }

//util.inherits(Pdu, native.Pdu);


exports = module.exports = Pdu;