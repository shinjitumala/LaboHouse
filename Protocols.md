# LaboHouse Protocols

## HTML: Rules
  * Any response that is not 'OK 200' is considered an error.
  * Any header parameters are required unless specified otherwise.
  * All error responses contain the error message in the header.
    * `error`: error reason

### Register a new User
  * Method: `POST`
  * URL: `/register`
  * Header Parameters:
    * `id`: requested id
    * `name`: requested display name
  * Response:
    * `Set-Cookie`: session cookie
    * `name`: display name

## WebSocket Commands
  * The name of the command is the `type`

### `cookie`: Sends the users cookie.
  * `cookie`: User's cookie. Connection will be closed if this is invalid.

### `himado`: Changes user's himado.
  * `himado`: himado in integer

### `chat`: Sends a new chat message.
  * `chat`: id of chat
  * `msg`: message

### `add_watchlist`: Add new user to watchlist.
  * `id`: id of User to be added to watchlist.

### `remove_watchlist`: Remove new user from watchlist.
  * `id`: id of User to be removed from watchlist.

### `add_timerange`: Add a new timerange.
  * `start`: `hh:mm`
  * `end`: `hh:mm`
  * `himado`: Himado set during that time.

### `remove_timerange`: Remove an existing timerange.
  * `start`: `hh:mm`
  * `end`: `hh:mm`

### `add_timer`: Add a timed himado setting.
  * `duration`: in minutes.
  * `himado`: himado to set when timer expires.

### `subhimado`: Change substatus.
  * `subhimado`:

### `remove_timer`: Removes the currently running timer.

## WebSocket Messages
  * The name of the command is the `type`.

### `names`: Loads an entire user list.
  * `names`: JSON

### `himado`: Update of a user's himado.
  * `name`:
  * `id`:
  * `himado`:
  * `subhimado`:

### `chat`: Loads an entire chat log.
  * `type`: chat
  * `id`: id of chat
  * `chat`: JSON

### `new_chat`: Append a new chat.
  * `chat`: id of chat
  * `msg`: JSON

### `notification`: Notification from server.
  * `msg`: Content of notification.

### `quote`: Today's quote.
  * `author`: Who said it.
  * `quote`: What was said.

### `timeranges`: Your timeranges.
  * `trs`: JSON timeranges.

### `rename`: Change display name.
  * `name`: The new name

### `substatus`: Change substatus.
  * `substatus`: The new substatus.