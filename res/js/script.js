// Macro
var E = function (id) { return document.getElementById(id); };
var C = function (e) { return document.createElement(e); };
var hide = function (id) { E(id).classList.add("hidden"); };
var show = function (id) { E(id).classList.remove("hidden"); };
function create_icon(cl) {
    var i = C("i");
    i.classList = cl;
    return i;
};
function free_icon() {
    return create_icon("fa fa-check-circle");
}
function easy_icon() {
    return create_icon("fa fa-exclamation-circle");
}
function busy_icon() {
    return create_icon("fa fa-minus-circle");
}
function offline_icon() {
    return create_icon("fa fa-power-off");
}
var icons = [free_icon, easy_icon, busy_icon, offline_icon];
function tooltip(value, tip) {
    var x = C("a");
    x.setAttribute("data-toggle", "tooltip");
    x.setAttribute("title", tip);
    x.innerText = value;
    $(x).tooltip();
    return x;
}

// Run on page load.
window.onload = function () {
    // Hide main page.
    hide("P#main");

    // Initializations before the body loads.
    openSocket(); // Login attempt with current Cookie.

    // Initializations that run after loading the body.
    window.document.body.onload = init();
};

function enableEventListeners() {
    window.addEventListener("focus", onFocus);
    window.addEventListener("blur", onBlur);
    window.addEventListener("mousemove", resetAFKTimer);
    window.addEventListener("keypress", resetAFKTimer);
};

function disableEventListeners() {
    window.removeEventListener("focus", onFocus);
    window.removeEventListener("blur", onBlur);
    window.removeEventListener("mousemove", resetAFKTimer);
    window.removeEventListener("keypress", resetAFKTimer);
};

// Store user's name
var g_name;
// Store user's id
var g_id;
// User's himado
var g_himado;

// Initializations after the body loads.
function init() {
    // Add event listener for chatbox.
    E("chatbox").addEventListener("keydown", function (e) {
        if (e.which === 13) {
            e.preventDefault();
            btnSendChat();
        }
    }, false);
};

// Called when add user whom you wanna watch.
function btnAddWatchList() {
    id = E("watchID").value;
    addWatchlist(id);
}

function btnRemoveWatchList() {
    id = E("watchID").value;
    removeWatchlist(id);
}

function btnAddTimerange() {
    var start = $("#tp_timerange_start").datetimepicker("viewDate").format("HH:mm");
    var end = $("#tp_timerange_end").datetimepicker("viewDate").format("HH:mm");
    var himado = E("timerange_himado").value;
    addTimerange(start, end, himado);
}

function btnRemoveTimerange() {
    var start = $("#tp_timerange_start").datetimepicker("viewDate").format("HH:mm");
    var end = $("#tp_timerange_end").datetimepicker("viewDate").format("HH:mm");
    removeTimerange(start, end);
}

function btnAddTimer() {
    addTimer(parseInt(E("timer_min").value), parseInt(E("timer_himado").value));
}

function btnRemoveTimer() {
    removeTimer();
}

// Called when registering new user.
function btnRegisterUser() {
    g_name = E("name").value;
    g_id = E("id").value;

    // Show loading
    var b = E("btn_join");
    b.innerText = "";
    var s = C("div");
    s.classList.add("spinner-border");
    s.classList.add("text-light");
    b.appendChild(s);
    b.disabled = true;

    registerUser(g_id, g_name);
};

var g_toast_count = 0;

function createToast(body, header) {
    var p = C("div");
    p.classList = "toast fade show border-danger align-self-end";
    p.setAttribute("style", "border-width: 3px !important");
    p.setAttribute("data-autohide", "false");
    var h = C("div");
    h.classList = "border-bottom p-2 bg-warning"
    var ht = C("strong");
    ht.classList = "mr-auto m-1 text-white font-weight-bolder";
    ht.innerText = header;
    var btn = C("button");
    btn.setAttribute("type", "button");
    btn.classList = "mr-auto close";
    btn.setAttribute("data-dismiss", "toast");
    btn.innerText = "x";
    btn.setAttribute("onclick", "clearToast()");
    h.appendChild(ht);
    h.appendChild(btn);
    var b = C("div");
    b.classList = "toast-body";
    b.innerText = body;
    p.appendChild(h);
    p.appendChild(b);
    $(p).toast("show");
    g_toast_count++;
    return p;
};

function clearToast(){
    g_toast_count--;
    if(g_toast_count == 0){
        var ts = document.getElementsByClassName("toast");
        for(var i = 0; i < ts.length; i++){
            $(ts[i]).remove();
        }
        resetNotification();
    }
};

function enableJoinButton() {
    var b = E("btn_join");
    b.disabled = false;
    b.innerText = "Join now!";
}

// Called when going to the main page.
function transMain() {
    hide("P#register");
    show("P#main");
};

// Called when the server sends us our information.
function displayName(user) {
    var x = E("block::name");
    x.innerText = " " + g_name + "#" + g_id;
    var y = tooltip("", user.substatus)
    y.appendChild(icons[to_int(g_himado)]());
    x.prepend(y);
};

// Called when going to the register page.
function transRegister() {
    show("P#register");
    hide("P#main");
};

// Called when changing himado of user.
function btnSetHimado() {
    var h = E("himado").value;
    sendHimado(h);
}

// Store list of all known users.
var g_names;

function to_int(himado) {
    // If self.
    if (himado == "Free") {
        return 0;
    }
    if (himado == "Easy") {
        return 1;
    }
    if (himado == "Busy") {
        return 2;
    }
    if (himado == "Offline") {
        return 3;
    }
    return undefined;
}

var g_notify_free = true;

// Called when a user changes status.
function changeUserStatus(m) {
    if (m.self) {
        E("himado").selectedIndex = to_int(m.himado);
        g_id = m.id;
        g_name = m.name;
        g_himado = m.himado;
        displayName(m);
    }
    if (!m.self && g_notify_free && m.himado == "Free" && g_himado == "Free") {
        showNotification("Someone is 'Free': " + m.name);
    }

    for (s in g_names) {
        var i = g_names[s].findIndex(u => u.id === m.id);
        if (i > -1) {
            g_names[s].splice(i, 1);
        }
    }

    if (g_names[m.himado] == undefined) {
        g_names[m.himado] = [];
    }
    g_names[m.himado].push(m);
    displayUsers(g_names);
    chatChange();
};

// Called when loading an entire user list
function reloadUsers(m) {
    g_names = m;
    displayUsers(m);
};

function clearTooltips() {
    var x = document.getElementsByClassName("tooltip");
    for (var i = 0; i < x.length; i++) {
        $(x[i]).remove();
    }
}

// Called when updating the display for users
function displayUsers(m) {
    clearTooltips();

    // Member lists.
    var members = E("list::members");
    members.innerText = ""; // Clear content

    function create_list(himado, icon, color) {
        var header = C("h3");
        header.innerText = ' ' + himado;
        header.classList.add("pt-2");
        header.classList.add("pl-2");
        header.classList.add("mx-2");
        header.classList.add("rounded");
        header.classList.add(color);
        header.prepend(icon);
        var list = C("ul");
        header.appendChild(list);
        for (var i in m[himado]) {
            var name = m[himado][i].name + "#" + m[himado][i].id;
            var ml = C("li");
            ml.appendChild(tooltip(m[himado][i].name, name + ": " + m[himado][i].substatus));
            list.appendChild(ml);
        }
        return header;
    };

    // Force the HIMADO order.
    members.appendChild(create_list("Free", free_icon(), "bg-success"));
    members.appendChild(create_list("Easy", easy_icon(), "bg-info"));
    members.appendChild(create_list("Busy", busy_icon(), "bg-warning"));
    members.appendChild(create_list("Offline", offline_icon(), "bg-secondary"));

    // Setup dropdown menu.
    var dropdown = E("watchID");
    dropdown.innerText = "";
    for (var himado in m) {
        for (var i in m[himado]) {
            if (m[himado][i].id == g_id) {
                // Skip self.
                continue;
            }
            var e = C("option");
            e.value = m[himado][i].id;
            e.innerText = m[himado][i].name + "#" + m[himado][i].id;
            dropdown.appendChild(e);
        }
    }
}

// Called when sending a chat message.
function btnSendChat() {
    var m = E("chatbox").value
    var c = E("chat_type").value
    sendChat(c, m);
    E("chatbox").value = "";
};

function btnSendChatFree() {
    var x = E("chatbox");
    var y = x.value;
    x.value = "@Free " + y;
    btnSendChat();
}

function btnSendChatEasy() {
    var x = E("chatbox");
    var y = x.value;
    x.value = "@Easy " + y;
    btnSendChat();
}

function btnSendChatBusy() {
    var x = E("chatbox");
    var y = x.value;
    x.value = "@Busy " + y;
    btnSendChat();
}

var g_chat = { "All": [], "Free": [], "Easy": [], "Busy": [] };

function refreshChat(chat) {
    clearTooltips();
    var c = E("block::chat_main");
    c.innerText = "";

    for (var i in g_chat[chat]) {
        var m = g_chat[chat][i];
        printChatLine(m);
    }
}

function getHimado(id) {
    for (var h in g_names) {
        for (var i in g_names[h]) {
            if (g_names[h][i].id == id) {
                return h;
            }
        }
    }
    return undefined;
}

// Called when appending a line to the chat.
function appendChat(c, m) {
    g_chat[c].push(m);
};

function printChatLine(m) {
    var chat = E("block::chat_main");

    var line = C("div");
    var time = C("i");
    time.innerText = m.time;
    var user = tooltip(" " + m.user.name, m.user.name + "#" + m.user.id + ": " + m.user.substatus)
    var h = to_int(getHimado(m.user.id));
    user.prepend(icons[h]());
    var msg = C("i");
    msg.innerText = ": " + m.msg;

    line.appendChild(time);
    line.appendChild(user);
    line.appendChild(msg);

    chat.appendChild(line);

    var s = E("B#chat");
    s.scrollTop = s.scrollHeight;

};

function reloadChat(m) {
    var c = m.name;
    g_chat[c] = m.chat;
};

function searchUser() {
    var query = E("member_search").value;
    var filtered = {};
    for (s in g_names) {
        var visible = [];
        for (var i in g_names[s]) {
            var m = g_names[s][i];
            if (m.name.includes(query) || m.id.includes(query) || (m.name + "#" + m.id).includes(query)) {
                visible.push(m);
            }
        }
        filtered[s] = visible;
    }

    displayUsers(filtered);
};

function showNotification(m) {
    document.title = "\* LaboHouse";
    var n = new Date;
    E("toast").appendChild(createToast(m, "Notification [" + n.getHours() + ":" + n.getMinutes() +
        "]"));
};

function resetNotification() {
    document.title = "LaboHouse";
};

function showQuote(author, quote) {
    E("GERO").innerText = "\"" + quote + "\" - " + author;
};

// Refreshes the entire main page.
function refresh_main() {
    getNames();
    queryUser();
    getChat();
    timer_on = true;
};

var g_busyTimer;
var g_AFKTimer;

// Set this to false if you want to stop auto focus and afk detection.
var g_autoChangeOff = false;

function onBlur() {
    if (ws == undefined || g_autoChangeOff) {
        return;
    }
    if (g_himado !== "Easy") {
        sendHimado(1);
    }
    resetBusyTimer();
};
function onFocus() {
    if (ws == undefined || g_autoChangeOff) {
        return;
    }
    window.clearTimeout(g_busyTimer);
    if (g_himado !== "Free") {
        sendHimado(0);
    }
    resetNotification();
    resetAFKTimer();
};

function goInactive() {
    if (ws == undefined || g_himado == "Busy" || g_autoChangeOff) {
        return;
    }
    sendHimado(2);
};
function goAFK() {
    if (ws == undefined || g_himado == "Offline" || g_autoChangeOff) {
        return;
    }
    sendHimado(2);
};

function resetBusyTimer() {
    window.clearTimeout(g_busyTimer);
    g_busyTimer = window.setTimeout(goInactive, 120000); // Timeout is in ms
};


function resetAFKTimer() {
    window.clearTimeout(g_AFKTimer);
    g_AFKTimer = window.setTimeout(goAFK, 300000);
};

function btnRename() {
    var x = E("rename");
    rename(x.value);
    x.value = "";
}

function btnSubstatus() {
    var x = E("substatus");
    substatus(x.value);
    x.value = "";
}

function chatChange() {
    var ct = E("chat_type").value;
    refreshChat(ct);
}

// Timepickers
$(function () {
    $('#tp_timerange_start').datetimepicker({
        format: 'LT'
    });
});
$(function () {
    $('#tp_timerange_end').datetimepicker({
        format: 'LT'
    });
});