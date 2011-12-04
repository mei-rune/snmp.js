var util = require('util');

var assertBuffer = function(assert, actual, expected) {
          assert.ok(Buffer.isBuffer(actual), "This isn`t a Buffer.");
          if(!Buffer.isBuffer(expected)) {
                assert.equal(actual.toString(), expected);
          } else {
                assert.deepEqual(actual, expected);
          }
        };


        module.exports = {
            'require session': function (beforeExit, assert) {
                var snmp = require('snmp');
                var session = new snmp.Native.Session();
            },
            'test session attributes': function (beforeExit, assert) {
                var snmp = require('snmp');
                var session = new snmp.Native.Session();

                session.version = 1;
                session.retries = 2;
                session.timeout = 3;
                session.flags = 4;
                session.peername = "5";
                session.remote_port = 6;
                session.localname = "7";
                session.local_port = 8;

                session.sessid = 9;
                session.community = "aa";
                session.rcvMsgMaxSize = 10;
                session.sndMsgMaxSize = 11;

                session.isAuthoritative = 12;
                session.contextEngineID = "13";
                session.engineBoots = 14;
                session.engineTime = 15;
                session.contextName = "16";
                session.securityEngineID = "17";
                session.securityName = "18";

                session.securityAuthProto = [1, 23, 4, 5, 19];
                session.securityAuthKey = "20";
                session.securityAuthLocalKey = "21";
                session.securityPrivProto = [1, 23, 4, 5, 22];
                session.securityPrivKey = "23";
                session.securityPrivLocalKey = "24";
                session.securityModel = 25;
                session.securityLevel = 26;
                session.paramName = "27";



                assert.equal(session.version, 1);
                assert.equal(session.retries, 2);
                assert.equal(session.timeout, 3);
                assert.equal(session.flags, 4);
                console.log(session.peername);
                assert.equal(session.peername, "5");
                assert.equal(session.remote_port, 6);
                assert.equal(session.localname, "7");
                assert.equal(session.local_port, 8);

                assert.equal(session.sessid, 9);
                assertBuffer(assert, session.community, "aa");
                assert.equal(session.rcvMsgMaxSize, 10);
                assert.equal(session.sndMsgMaxSize, 11);

                assert.equal(session.isAuthoritative, 12);
                assertBuffer(assert, session.contextEngineID, "13");
                assert.equal(session.engineBoots, 14);
                assert.equal(session.engineTime, 15);
                assert.equal(session.contextName, "16");
                assertBuffer(assert, session.securityEngineID, "17");
                assert.equal(session.securityName, "18");

                assert.deepEqual(session.securityAuthProto, [1, 23, 4, 5, 19]);
                assertBuffer(assert, session.securityAuthKey, "20");
                assertBuffer(assert, session.securityAuthLocalKey, "21");
                assert.deepEqual(session.securityPrivProto, [1, 23, 4, 5, 22]);
                assertBuffer(assert, session.securityPrivKey, "23");
                assertBuffer(assert, session.securityPrivLocalKey, "24");
                assert.equal(session.securityModel, 25);
                assert.equal(session.securityLevel, 26);
                assert.equal(session.paramName, "27");

            }
        };