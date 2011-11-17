module.exports = {
    'require session': function (beforeExit, assert) {
        var snmp = require('snmp');
        var session = new snmp.Session();
    }
};