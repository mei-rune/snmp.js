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
        assert.equal('aacc', pdu.community);

        console.log("ent");
        pdu.enterprise = [2, 3, 5, 6, -1];
        assert.deepEqual([2, 3, 5, 6, -1], pdu.enterprise);

        pdu.trap_type = 16;
        assert.equal(16, pdu.trap_type);

        pdu.specific_type = 17;
        assert.equal(17, pdu.specific_type);


        //	SNMP_SET_ACCESSOR(t, agent_addr)

        pdu.contextEngineID = 'contextEngineID';
        assert.equal('contextEngineID', pdu.contextEngineID);

        pdu.contextName = 'contextName';
        assert.equal('contextName', pdu.contextName);

        pdu.securityEngineID = 'securityEngineID';
        assert.equal('securityEngineID', pdu.securityEngineID);

        pdu.securityName = 'securityName';
        assert.equal('securityName', pdu.securityName);

        pdu.priority = 18;
        assert.equal(18, pdu.priority);

        pdu.range_subid = 19;
        assert.equal(19, pdu.range_subid);

        pdu.close();
    },

    'create pdu with vb': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu = snmp.createPdu(snmp.Constants.SNMP_MSG_GET);
        var vb = pdu.variableBindings;
        vb.add([1,2,3,4,5], snmp.DATA_TYPE.ASN_INTEGER, 23);
        assert.equal(vb.get([1,2,3,4,5]).value, 23);
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