/// Standard function used to display HTTP request errors.
function show_error(res, status) {
    window.alert(status + ": " + res.getResponseHeader("error"));
}

function noop() { };

function registerName() {
    $.ajax({
        type: 'POST',
        url: "/register",
        headers: {
            'id': document.getElementById("id").value,
            'name': document.getElementById("name").value,
        },

        success: function (data, status, req) {
            refresh_main();

            // Hide the registration page and show the main page.
            document.getElementById("page::register").style.display = "none";
            document.getElementById("page::main").style.display = "block";
        },
        error: function (res, error, status) {
            show_error(res, status)
        }
    });
}

function queryUser(callback = noop) {
    $.ajax({
        type: 'POST',
        url: "/name",

        success: function (data, status, req) {
            var name = req.getResponseHeader("name");
            // Changing the name display.
            document.getElementById("block::name").innerText = "";
            document.getElementById("block::name").innerText = name;

            callback();

            // Hide the registration page and show the main page.
            document.getElementById("page::register").style.display = "none";
            document.getElementById("page::main").style.display = "block";
        },
        error: function (res, error, status) {
            // Do nothing 
        }
    });
}

function registerHima(value = document.getElementById("himado").value) {
    document.getElementById("himado").value = value;
    $.ajax({
        type: 'POST',
        url: "/sethimado",
        headers: {
            'Himado': value
        },

        success: function (data, status, req) {
            getHima();
            refresh_main();
        },
        error: function (res, error, status) {
            show_error(res, status)
        }
    });
}

function getHima() {
    $.ajax({
        type: 'POST',
        url: '/gethimado',
        success: function (data, status, req) {
            var himado = req.getResponseHeader("Himado");
        },
        error: function (res, error, status) {
            show_error(res, status)
        }
    });
}

// list of members
var data_members = {};

function display_members(data) {
    var members = document.getElementById("list::members");
    members.innerText = ""; // Clear content

    function create_list(himado) {
        var header = document.createElement("h3");
        header.innerText = himado;
        var list = document.createElement("ul");
        header.appendChild(list);
        for (var i in data[himado]) {
            var m = data[himado][i];
            var ml = document.createElement("li");
            ml.innerText = m;
            list.appendChild(ml);
        }
        return header;
    };

    for (s in data) {
        members.appendChild(create_list(s));
    }
}

function getNames() {
    $.ajax({
        type: 'POST',
        url: '/names',
        success: function (data, status, req) {
            display_members(data);

            data_members = data;
        },
        error: function (res, error, status) {
            show_error(res, status)
        }
    });
}

function searchUser() {
    var query = document.getElementById("member_search").value;
    var filtered = {};
    for (s in data_members) {
        var visible = [];
        for (var i in data_members[s]) {
            var m = data_members[s][i];
            if (m.includes(query)) {
                visible.push(m);
            }
        }
        filtered[s] = visible;
    }

    display_members(filtered);
};

function sendChat() {
    $.ajax({
        type: 'POST',
        url: '/chat_main',
        headers: { 'message': document.getElementById("chatbox").value },
        success: function (data, status, req) {
            document.getElementById("chatbox").value = "";
            getChat();
        },
        error: function (r, e, s) {
            show_error(r, s);
        }
    });
}

function chatOnEnter(e) {
    if (e.which === 13) {
        e.preventDefault();
        sendChat();
    }
};

function getChat() {
    $.ajax({
        type: 'POST',
        url: '/chat_main_get',
        success: function (data, status, req) {
            var chat = document.getElementById("block::chat_main");
            chat.innerText = ""; // Reset chat

            var text = "";
            for (var i in data) {
                var m = data[i];
                text += m.time;
                text += m.user + ": ";
                text += m.msg + "\n";
            }
            chat.innerText = text;
        },
        error: function (r, e, s) {
            show_error(r, s);
        }
    });
};


// Refreshes the entire main page.
function refresh_main() {
    getNames();
    queryUser();
    getChat();
    timer_on = true;
};

var timer;
var timer_on = false;
// Run on page load.
function init() {
    queryUser(refresh_main);

    addEventListener("mousemove", resetTimer, false);
    addEventListener("mousedown", resetTimer, false);
    addEventListener("keypress", resetTimer, false);
    addEventListener("DOMMouseScroll", resetTimer, false);
    addEventListener("mousewheel", resetTimer, false);
    addEventListener("touchmove", resetTimer, false);
    addEventListener("MSPointerMove", resetTimer, false);

    document.getElementById("chatbox").addEventListener("keypress", chatOnEnter);

    resetTimer();
};

function goInactive() {
    registerHima(2);
};

function resetTimer() {
    if (!timer_on) {
        return;
    }
    window.clearTimeout(timer);
    timer = window.setTimeout(goInactive, 2000); // Timeout is in ms
}

window.onload = init;
