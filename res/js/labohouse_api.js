function openSocket() {
    ws = new WebSocket("ws:funnypigrun.dns-cloud.net:12273");
    ws.onmessage = function (e) {
        console.log("WS: msg - " + e.data);
    };
    ws.onopen = function () {
        var j = { "type": "cookie", "args": { "Cookie": document.cookie } }
        ws.send(JSON.stringify(j));
        console.log("WS: open");
    };
    ws.onclose = function () {
        ws = undefined;
        console.log("WS: close");
    };
};

function registerUser(id, name, success = function () { }, fail = function () { }) {
    $.ajax({
        type: 'POST',
        url: "/register",
        headers: {
            "id": id,
            "name": name,
        },
        success: function () {
            success();
        },
        error: function (res) {
            ajax_error(res);
            fail();
        },
    });
}

function ajax_error(res) {
    if (res.readyState == 4) {
        window.alert("[Error] " + res.getResponseHeader("error"));
    } else if (res.readyState == 0) {
        window.alert("Network error.");
    }
}