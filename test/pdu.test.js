module.exports = {
    'require pdu': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu = new snmp.Pdu(snmp.SNMP_VERSION_1);
        pdu.close();
    },
    'new pdu with no argument': function (beforeExit, assert) {
        var snmp = require('snmp');
        try {
            var pdu = new snmp.Pdu();
            assert.fail();
        } catch (e) {
            assert.equal('Must pass a integer argument to constructor.', e.message);
        }
    },
    'new pdu with no argument': function (beforeExit, assert) {
        var snmp = require('snmp');
        var pdu = new snmp.Pdu(snmp.SNMP_VERSION_1);
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
        assert.equal(-11, pdu.time);

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
};