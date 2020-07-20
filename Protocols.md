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

## WebSocket Messages
  * The name of the command is the `type`.

### `names`: Loads an entire user list.
  * `names`: JSON

### `name`: Displays the user's name.
  * `name`:
  * `id`:

### `himado`: Update of a user's himado.
  * `name`:
  * `id`:
  * `himado`:

### `chat`: Loads an entire chat log.
  * `type`: chat
  * `id`: id of chat
  * `chat`: JSON

### `new_chat`: Append a new chat.
  * `chat`: id of chat
  * `msg`: JSON

### `notification`: Notification from server.
  * `msg`: Content of notification.