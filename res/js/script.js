// Run on page load.
window.onload = function () {
    // Initializations before the body loads.
    openSocket(); // Login attempt with current Cookie.
    window.addEventListener("focus", onFocus);
    window.addEventListener("blur", onBlur);

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
    document.getElementById("chatbox").addEventListener("keydown", function (e) {
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

// Called when registering new user.
function btnRegisterUser() {
    g_name = document.getElementById("name").value;
    g_id = document.getElementById("id").value;
    registerUser(g_id, g_name);
};

// Called when going to the main page.
function transMain() {
    document.getElementById("page::register").style.display = "none";
    document.getElementById("page::main").style.display = "block";
};

// Called when the server sends us our information.
function displayName(user) {
    g_name = user.name;
    g_id = user.id;
    document.getElementById("block::name").innerText = g_name + "#" + g_id;
};

// Called when going to the register page.
function transRegister() {
    document.getElementById("page::main").style.display = "none";
    document.getElementById("page::register").style.display = "block";
};

// Called when changing himado of user.
function btnSetHimado() {
    var h = document.getElementById("himado").value;
    sendHimado(h);
}

// Store list of all known users.
var g_names;

// Called when a user changes status.
function changeUserStatus(m) {
    if (m.id == g_id) {
        document.getElementById("himado").selectedIndex = function () {       // If self.
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
        var list = g_names[s];
        var i = list.findIndex(u => u.name === m.name);
        if (i !== undefined) {
            list.splice(i, 1);
        }
    }

    if (g_names[m.himado] == undefined) {
        g_names[m.himado] = [];
    }
    g_names[m.himado].push({ "name": m.name, "id": m.id });
    reloadUsers(g_names);
};

// Called when loading an entire user list
function reloadUsers(m) {
    g_names = m;
    displayUsers(m);
};

// Called when updating the display for users
function displayUsers(m) {
    var members = document.getElementById("list::members");
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
}

// Called when sending a chat message.
function btnSendChat() {
    var m = document.getElementById("chatbox").value
    sendChat(m);
    document.getElementById("chatbox").value = "";
};

// Called when appending a line to the chat.
function appendChat(m) {
    var chat = document.getElementById("block::chat_main");

    var line = document.createElement("div");
    var time = document.createElement("div");
    time.className = "inline-div margin"
    time.innerText = m.time;
    var user = document.createElement("div");
    user.innerText = m.user.name + ":";
    user.className = "tooltip inline-div margin";
    var user_hover = document.createElement("span");
    user_hover.innerText = m.user.name + "#" + m.user.id;
    user_hover.className = "tooltiptext";
    user.appendChild(user_hover);

    var msg = document.createElement("div");
    msg.innerText = m.msg;
    msg.className = "inline-div margin";

    line.appendChild(time);
    line.appendChild(user);
    line.appendChild(msg);

    chat.appendChild(line);
};

function reloadChat(m) {
    for (var i in m.chat) {
        var l = m.chat[i];
        appendChat(l);
    }
};

function searchUser() {
    var query = document.getElementById("member_search").value;
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
};

function resetNotification() {
    alert("I notify you no more.");
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
    g_busyTimer = window.setTimeout(goInactive, 2000); // Timeout is in ms
};


function resetAFKTimer() {
    window.clearTimeout(g_AFKTimer);
    g_AFKTimer = window.setTimeout(goAFK, 10000);
};

