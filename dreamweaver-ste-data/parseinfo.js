var l = console.log.bind(console);
var fs = require('fs');

if (!process.argv[2]) {
    process.exit();
}

var file = process.argv[2];

/**
 * Decrypt the encrypted dreamweaver password.
 * http://stackoverflow.com/questions/17992802/how-to-decrypt-dreamweaver-site-passwords
 *
 * @param {string} hash - a string which is the encrypted password.
 * @return {string} - the plain text password.
 */
function decriptPassword(hash) {
    var i,
        plainPasswd = '';

    for (i = 0; i < hash['length']; i = (i + 2)) {
        plainPasswd += String.fromCharCode(parseInt(hash[i] + '' + hash[i + 1], 16) - (i / 2));
    }

    return plainPasswd;
}

/**
 * The an attribute's value from a string.
 *
 * @param {string} str - the string which contains the attribute.
 * @param {string} attr - the attribute name.
 * @return {string} - the value of the attribute attr.
 */
function getValue(str, attr) {
    // ex:
    //      host="([^"]+)"
    //        pw="([^"]+)"
    var re = new RegExp(attr + '="([^"]+)"');
    return str.match(re)[1];
}

/**
 * Display an object's key/value pairs.
 *
 * @param {object} values.
 */
function displayValues(values) {
    l('------------------------------------------');
    for (var key in values) {
        l(key, ':', values[key]);
    }
    l('\n');
}

/**
 * Get the attribute values from the line.
 *
 * @param {string} line - the line containing the attribute/value pairs.
 * @return {object} - the object with key/value pairs.
 */
function getValues(line) {
    values = {};
    values.host = getValue(line, 'host');
    values.user = getValue(line, 'user');
    values.pw = decriptPassword(getValue(line, 'pw'));
    values.docroot = getValue(line, 'remoteroot');

    return values;
}

fs.readFile(file, 'utf8', function (err, data) {
    if (err) {
        l(err);
        return;
    }

    var lines = data.split('\n'),
        values;

    for (var i = 0; i < lines.length; ++i) {
        if (lines[i].indexOf('<server ') !== -1) {
            values = getValues(lines[i]);
            break;
        };
    }

    displayValues(values);
});



