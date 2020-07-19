// The websocket
var ws;

function openSocket() {
    if (ws != undefined) {
        console.error("WS: Still open");
        return;
    }
    console.log("WS: Try Open.");
    ws = new WebSocket("ws:funnypigrun.dns-cloud.net:12273");
    ws.onmessage = function (e) {
        wsEventHandler(JSON.parse(e.data));
    };
    ws.onopen = function () {
        var j = { "type": "cookie", "args": { "Cookie": document.cookie } }
        ws.send(JSON.stringify(j));
        console.log("WS: Opened.");
        transMain();
    };
    ws.onclose = function (e) {
        console.log("WS: Closed. Code: " + e.code + ", Reason: " + e.reason);
        ws = undefined;
        transRegister();
    };
    ws.onerror = function (e) {
        console.error(e);
    }
};

function wsEventHandler(m) {
    console.log("WS: Message.");
    console.log(m);

    var type = m.type;
    if (type == null) {
        console.error("'type' missing: " + type);
        return;
    }

    if (type == "himado") {
        changeUserStatus(m);
        return;
    }
    if (type == "names") {
        reloadUsers(m.names);
        return;
    }

    if (type == "new_chat") {
        // New chat message
        appendChat(m);
        return;
    }
    if (type == "chat") {
        // Refresh chat.
        reloadChat(m);
        return;
    }

    console.error("Unhandled: " + type);
}

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
            openSocket();
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

function sendChat(m) {
    var j = {};
    j["type"] = "chat";
    j["args"] = { "msg": m };

    ws.send(JSON.stringify(j));
}

function sendHimado(h) {
    var j = {};
    j["type"] = "set_himado";
    j["args"] = { "himado": parseInt(h) };

    ws.send(JSON.stringify(j));
}