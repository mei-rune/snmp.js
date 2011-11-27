
var util = require('util');

var assertBuffer = function(assert, actual, expected) {
          assert.ok(Buffer.isBuffer(actual), "This isn`t a Buffer.");
          if(!Buffer.isBuffer(expected)) {
                assert.equal(actual.toString(), expected);
          } else {
                assert.deepEqual(actual, expected);
          }
        };

var fillPdu = function(snmp, pdu) {

        pdu.command = "get";
        pdu.reqid = 4;
        pdu.msgid = 5;
        pdu.transid = 6;
        pdu.sessid = 7;
        pdu.errstat = 8;
        pdu.errindex = 9;
        pdu.time = 4294967285;
        pdu.flags = 12;
        pdu.securityModel = 13;
        pdu.securityLevel = 14;
        pdu.msgParseModel = 15;
        pdu.community = 'aacc';
        pdu.enterprise = [2, 3, 5, 6, -1];
        pdu.trap_type = 16;
        pdu.specific_type = 17;
        pdu.contextEngineID = 'contextEngineID';
        pdu.contextName = 'contextName';
        pdu.securityEngineID = 'securityEngineID';
        pdu.securityName = 'securityName';
        pdu.priority = 18;
        pdu.range_subid = 19;

        if(pdu.variableBindings) {
                pdu.variableBindings.add([1,2,3,4,6], snmp.DATA_TYPE.ASN_INTEGER, 12);
                pdu.variableBindings.add([1,2,4,4,6], snmp.DATA_TYPE.ASN_INTEGER, 12);
                pdu.variableBindings.add([1,2,5,4,6], snmp.DATA_TYPE.ASN_INTEGER, 'asdfasf');
                
        } else {
                pdu.variableBindings = [
                        {oid:[1,2,3,4,6], type:snmp.DATA_TYPE.ASN_INTEGER, value:12},
                        {oid:[1,2,4,4,6], type:snmp.DATA_TYPE.ASN_INTEGER, value:12},
                        {oid:[1,2,5,4,6], type:snmp.DATA_TYPE.ASN_OCTET_STR, value:"asdfasf"},
                ];
        }
}

var assertPdu = function(assert, snmp, actual, expected) {
        var i;

        assert.ok(actual !== null, "result is null");
        assert.equal(actual.version, expected.version);
        assert.equal(actual.command, expected.command);
        assert.equal(actual.reqid, expected.reqid);
        assert.equal(actual.msgid, expected.msgid);
        assert.equal(actual.transid, expected.transid);
        assert.equal(actual.sessid, expected.sessid);
        assert.equal(actual.errstat, expected.errstat);
        assert.equal(actual.errindex, expected.errindex);
        assert.equal(actual.time, expected.time);
        assert.equal(actual.flags, expected.flags);
        assert.equal(actual.securityModel, expected.securityModel);
        assert.equal(actual.securityLevel, expected.securityLevel);
        assert.equal(actual.msgParseModel, expected.msgParseModel);

        if(snmp.PDUTYPE.TRAP === expected.command) {
                assert.deepEqual(actual.enterprise, expected.enterprise);
                assert.equal(actual.trap_type, expected.trap_type);
                assert.equal(actual.specific_type, expected.specific_type);
        }


        if(snmp.SNMP_VERSION.v3 === expected.version) {
                assertBuffer(assert, actual.contextEngineID, expected.contextEngineID);
                assert.equal(actual.contextName, expected.contextName);
                assertBuffer(assert, actual.securityEngineID,  expected.securityEngineID);
                assert.equal(actual.securityName, expected.securityName);
                assert.equal(actual.priority, expected.priority);
                assert.equal(actual.range_subid, expected.range_subid);
        } else {
                assertBuffer(assert, actual.community, expected.community);
        }


        for(i =0; i < 3; ++ i) {
                var a = util.isArray(actual.variableBindings)? actual.variableBindings[i]:actual.variableBindings.get(i);
                var b = util.isArray(expected.variableBindings)? expected.variableBindings[i]:expected.variableBindings.get(i);
                assert.deepEqual(a.oid, b.oid);
                assert.deepEqual(a.type, b.type);
                if(i === 3) { assertBuffer(a.value, b.value); }
                else { assert.equal(a.value, b.value); }
        }
}

module.exports = {
    'require pdu': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        pdu.close();
    },
    'new pdu with no argument': function (beforeExit, assert) {
        var snmp = require('snmp');
        try {
            var pdu = new snmp.Native.Pdu();
            assert.fail();
        } catch (e) {
            assert.equal('Must pass a integer argument to constructor.', e.message);
        }
    },
    'create pdu': function (beforeExit, assert) {
        var snmp = require('snmp');
        assert.equal(snmp.createPdu("get").command, snmp.Constants.SNMP_MSG_GET);
        assert.equal(snmp.createPdu("getnext").command, snmp.Constants.SNMP_MSG_GETNEXT);
        assert.equal(snmp.createPdu("next").command, snmp.Constants.SNMP_MSG_GETNEXT);
        assert.equal(snmp.createPdu("set").command, snmp.Constants.SNMP_MSG_SET);
        assert.equal(snmp.createPdu("respone").command, snmp.Constants.SNMP_MSG_RESPONSE);
        assert.equal(snmp.createPdu("getbulk").command, snmp.Constants.SNMP_MSG_GETBULK);
        assert.equal(snmp.createPdu("bulk").command, snmp.Constants.SNMP_MSG_GETBULK);
        assert.equal(snmp.createPdu("trap").command, snmp.Constants.SNMP_MSG_TRAP);
        assert.equal(snmp.createPdu("inform").command, snmp.Constants.SNMP_MSG_INFORM);
        assert.equal(snmp.createPdu("trap2").command, snmp.Constants.SNMP_MSG_TRAP2);
        assert.equal(snmp.createPdu("report").command, snmp.Constants.SNMP_MSG_REPORT);

    },
    'new pdu with get argument': function (beforeExit, assert) {
        

        var snmp = require('snmp');
        var pdu = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        pdu.version = -11;
        assert.equal(-11, pdu.version);


        pdu.command = -3;
        assert.equal(-3, pdu.command);

        pdu.reqid = 4;
        assert.equal(4, pdu.reqid);

        pdu.msgid = 5;
        assert.equal(5, pdu.msgid);

        pdu.transid = 6;
        assert.equal(6, pdu.transid);

        pdu.sessid = 7;
        assert.equal(7, pdu.sessid);

        pdu.errstat = 8;
        assert.equal(8, pdu.errstat);

        pdu.errindex = 9;
        assert.equal(9, pdu.errindex);

        pdu.time = -11;
        assert.notEqual(-11, pdu.time);
        assert.equal(4294967285, pdu.time);

        pdu.flags = 12;
        assert.equal(12, pdu.flags);

        pdu.securityModel = 13;
        assert.equal(13, pdu.securityModel);

        pdu.securityLevel = 14;
        assert.equal(14, pdu.securityLevel);

        pdu.msgParseModel = 15;
        assert.equal(15, pdu.msgParseModel);

        //pdu.tDomain = [2, 3, 5, 6, -1];
        //assert.equal([2, 3, 5, 6, -1], pdu.tDomain);


        pdu.community = 'aacc';
        assertBuffer(assert, pdu.community, 'aacc');

        pdu.enterprise = [2, 3, 5, 6, -1];
        assert.deepEqual([2, 3, 5, 6, -1], pdu.enterprise);

        pdu.trap_type = 16;
        assert.equal(16, pdu.trap_type);

        pdu.specific_type = 17;
        assert.equal(17, pdu.specific_type);


        //	SNMP_SET_ACCESSOR(t, agent_addr)

        pdu.contextEngineID = 'contextEngineID';
        assertBuffer(assert, pdu.contextEngineID, 'contextEngineID');

        pdu.contextName = 'contextName';
        assert.equal('contextName', pdu.contextName);

        pdu.securityEngineID = 'securityEngineID';
        assertBuffer(assert, pdu.securityEngineID, 'securityEngineID');

        pdu.securityName = 'securityName';
        assert.equal('securityName', pdu.securityName);

        pdu.priority = 18;
        assert.equal(18, pdu.priority);

        pdu.range_subid = 19;
        assert.equal(19, pdu.range_subid);

        pdu.close();
    },

    'create v1 pdu with js object': function (beforeExit, assert) {

        var snmp = require('snmp');
        var pdu = { version : snmp.SNMP_VERSION.v1 };
        var newvalue;
        fillPdu(snmp, pdu);
        newvalue = snmp.createPduFromObject(pdu);
        assertPdu(assert, snmp, newvalue, pdu);
        newvalue.community = "aaa";
        newvalue.close();
    },

    'create v3 pdu with js object': function (beforeExit, assert) {

        var snmp = require('snmp');
        var pdu = { version : snmp.SNMP_VERSION.v3 };
        var newvalue;
        fillPdu(snmp, pdu);
        newvalue = snmp.createPduFromObject(pdu);
        assertPdu(assert, snmp, newvalue, pdu);
        newvalue.securityName = "aaa";
        newvalue.close();
    },

    'create v1 jsPdu with pdu': function (beforeExit, assert) {

        var snmp = require('snmp');

        var pdu, newvalue;

        pdu  = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        pdu.version = snmp.SNMP_VERSION.v1;
        
        fillPdu(snmp, pdu);
        
        newvalue = pdu.toJS();
        
        assertPdu(assert, snmp, pdu, newvalue);

        pdu.close();
    },

    'create v3 jsPdu with pdu': function (beforeExit, assert) {

        var snmp = require('snmp');

        var pdu, newvalue;

        pdu  = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        pdu.version = snmp.SNMP_VERSION.v3;
        
        fillPdu(snmp, pdu);
        
        newvalue = pdu.toJS();
        
        assertPdu(assert, snmp, pdu, newvalue);

        pdu.close();
    },

    'create pdu with error oid js': function (beforeExit, assert) {

        var snmp = require('snmp');

        var snmp = require('snmp');
        var pdu = { version : snmp.SNMP_VERSION.v3 };
        var newvalue;
        fillPdu(snmp, pdu);
        pdu.command = "trap";


        pdu.enterprise = "sfasfasf";
        console.log("aaa");
        try {
                newvalue = snmp.createPduFromObject(pdu);
                newvalue.close();
                assert.ok(false, "expected a TypeError.");
        }catch(e) {
                assert.equal(e.message, "argument enterprise must be oid.");
        }
    },





    'create pdu with vb': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu, vb, a;
        pdu = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        vb = pdu.variableBindings;
        vb.add([1,2,3,4,5], snmp.DATA_TYPE.ASN_INTEGER, 23);
        a = vb.get([1,2,3,4,5]);
        assert.notEqual(a, null);
        assert.equal(a.value, 23);
        vb.clear();
        vb.add([1,2,3,4,5], snmp.DATA_TYPE.ASN_INTEGER, "24");
        assert.equal(vb.get([1,2,3,4,5]).value, 24);

        
        vb.clear();
        vb.add([1,2,3,4,5], snmp.DATA_TYPE.ASN_INTEGER, "aaa24");
        assert.equal(vb.get([1,2,3,4,5]).value, 0);

        
        vb.clear();
        vb.add("1.2.3.4.5", snmp.DATA_TYPE.ASN_INTEGER, "24");
        assert.equal(vb.get([1,2,3,4,5]).value, 24);

    },

    'set vb': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        try {
           pdu.variableBindings = 23;
           assert.fail("test variableBindings is readOnly");
        } catch (e) {
           assert.equal(e.message, "variableBindings is readonly.");
        }
    },
};