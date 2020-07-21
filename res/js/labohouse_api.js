// The websocket
var ws = undefined;
var dbg_mode = false;

function dbg(m) {
    if (dbg_mode) {
        console.log(m);
    }
}

function err(m) {
    console.error(m);
}

function openSocket() {
    if (ws != undefined) {
        err("WS: Still open");
        return;
    }
    dbg("WS: Try Open.");
    ws = new WebSocket("ws:funnypigrun.dns-cloud.net:12273");
    ws.onmessage = function (e) {
        wsEventHandler(JSON.parse(e.data));
    };
    ws.onopen = function () {
        var j = { "type": "cookie", "cookie": document.cookie };
        ws.send(JSON.stringify(j));
        dbg("WS: Opened.");
        transMain();
    };
    ws.onclose = function (e) {
        dbg("WS: Closed. Code: " + e.code + ", Reason: " + e.reason);
        ws = undefined;
        transRegister();
    };
    ws.onerror = function (e) {
        err(e);
    }
};

function wsEventHandler(m) {
    dbg("WS: Message.");
    dbg(m);

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
    if(type == "name"){
        displayName(m);
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

    if (type == "notification") {
        showNotification(m.msg);
        return;
    }

    if (type == "quote") {
        showQuote(m.author, m.quote);
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
    j["msg"] = m;

    ws.send(JSON.stringify(j));
}

function sendHimado(h) {
    var j = {};
    j["type"] = "himado";
    j["himado"] = parseInt(h);

    g_himado = j.himado;

    ws.send(JSON.stringify(j));
}

function sendSubhimado(s){
    var j = {};
    j["type"] = "subhimado";
    j["subhimado"] = s;
    ws.send(JSON.stringify(j));
}

function addWatchlist(id) {
    var j = {};
    j["type"] = "add_watchlist";
    j["id"] = id;

    ws.send(JSON.stringify(j));
}

function addTimeRange(start, end, himado) {
    var j = {};
    j["type"] = "add_timerange";
    j["start"] = start;
    j["end"] = end;
    j["himado"] = himado;

    ws.send(JSON.stringify(j));
}

function removeTimeRange(start, end) {
    var j = {};
    j["type"] = "remove_timerange";
    j["start"] = start;
    j["end"] = end;

    ws.send(JSON.stringify(j));
}

function addTimer(mins, himado) {
    var j = {};
    j["type"] = "add_timer";
    j["duration"] = mins;
    j["himado"] = himado;

    ws.send(JSON.stringify(j));
}

function removeTimer() {
    var j = {};
    j["type"] = "remove_timer";
    ws.send(JSON.stringify(j));
}