// Macro
var E = function (id) { return document.getElementById(id); };
var hide = function (id) { E(id).classList.add("hidden"); };
var show = function (id) { E(id).classList.remove("hidden"); };

// Run on page load.
window.onload = function () {
    // Hide main page.
    hide("P#main");

    // Initializations before the body loads.
    openSocket(); // Login attempt with current Cookie.
    window.addEventListener("focus", onFocus);
    window.addEventListener("blur", onBlur);
    window.addEventListener("mousemove", resetAFKTimer);
    window.addEventListener("keypress", resetAFKTimer);

    // Initializations that run after loading the body.
    window.document.body.onload = init();

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

    // Request notification permission    
    if (Notification.permission !== "granted") {
        // Not working =(
        Notification.requestPermission().then(p => { console.log(p); });
    }
};

// Called when add user whom you wanna watch.
function btnAddWatchList() {
    id = E("watchID").value;
    addWatchlist(id);
}

function btnRemoveWatchList() {
    id = E("watchID").value;
    addWatchlist(id);
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

// Called when registering new user.
function btnRegisterUser() {
    g_name = E("name").value;
    g_id = E("id").value;
    registerUser(g_id, g_name);
};

// Called when going to the main page.
function transMain() {
    hide("P#register");
    show("P#main");
};

// Called when the server sends us our information.
function displayName(user) {
    g_name = user.name;
    g_id = user.id;
    E("block::name").innerText = g_name + "#" + g_id;
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

// Called when a user changes status.
function changeUserStatus(m) {
    if (m.id == g_id) {
        E("himado").selectedIndex = function () {       // If self.
            if (m.himado == "Free") {
                return 0;
            }
            if (m.himado == "Easy") {
                return 1;
            }
            if (m.himado == "Busy") {
                return 2;
            }
            if (m.himado == "Offline") {
                return 3;
            }
        }();
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
    g_names[m.himado].push({ "name": m.name, "id": m.id });
    displayUsers(g_names);
};

// Called when loading an entire user list
function reloadUsers(m) {
    g_names = m;
    displayUsers(m);
};

// Called when updating the display for users
function displayUsers(m) {
    // Member lists.
    var members = E("list::members");
    members.innerText = ""; // Clear content

    function create_list(himado) {
        var header = document.createElement("h3");
        header.innerText = himado;
        var list = document.createElement("ul");
        header.appendChild(list);
        for (var i in m[himado]) {
            var name = m[himado][i].name + "#" + m[himado][i].id;
            var ml = document.createElement("li");
            ml.innerText = name;
            list.appendChild(ml);
        }
        return header;
    };

    // Force the HIMADO order.
    members.appendChild(create_list("Free"));
    members.appendChild(create_list("Easy"));
    members.appendChild(create_list("Busy"));
    members.appendChild(create_list("Offline"));

    // Setup dropdown menu.
    var dropdown = E("watchID");
    dropdown.innerText = "";
    for (var himado in m) {
        for (var i in m[himado]) {
            if (m[himado][i].id == g_id) {
                // Skip self.
                continue;
            }
            var e = document.createElement("option");
            e.value = m[himado][i].id;
            e.innerText = m[himado][i].name + "#" + m[himado][i].id;
            dropdown.appendChild(e);
        }
    }
}

// Called when sending a chat message.
function btnSendChat() {
    var m = E("chatbox").value
    sendChat(m);
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

// Called when appending a line to the chat.
function appendChat(m) {
    var chat = E("block::chat_main");

    var line = document.createElement("div");
    var time = document.createElement("i");
    time.innerText = m.time;
    var user = document.createElement("i");
    user.innerText = m.user.name;
    var msg = document.createElement("i");
    msg.innerText = ": " + m.msg;

    line.appendChild(time);
    line.appendChild(user);
    line.appendChild(msg);

    chat.appendChild(line);

    var s =E("B#chat");
    s.scrollTop = s.scrollHeight;
};

function reloadChat(m) {
    for (var i in m.chat) {
        var l = m.chat[i];
        appendChat(l);
    }
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
    alert(m);
    document.title = "\* LaboHouse";
};

function resetNotification() {
    document.title = "LaboHouse";
};

function showQuote(author, quote) {
    alert("\"" + quote + "\" - " + author);
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
    sendHimado(1);
    resetBusyTimer();
};
function onFocus() {
    if (ws == undefined || g_autoChangeOff) {
        return;
    }
    window.clearTimeout(g_busyTimer);
    if (g_himado !== 0) {
        sendHimado(0);
    }
    resetNotification();
    resetAFKTimer();
};

function goInactive() {
    if (ws == undefined || g_himado == 2 || g_autoChangeOff) {
        return;
    }
    sendHimado(2);
};
function goAFK() {
    if (ws == undefined || g_himado == 3 || g_autoChangeOff) {
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