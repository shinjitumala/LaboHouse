# LaboHouse Protocols

## HTML: Rules
  * Any response that is not 'OK 200' is considered an error.
  * Any header parameters are required unless specified otherwise.
  * All error responses contain the error message in the header.
    * error: \<error reason\>

### Register a new User
  * Method: POST
  * URL: /register
  * Header Parameters:
    * id: \<requested id\>
    * name: \<requested display name\>
  * Response:
    * Set-Cookie: \<session cookie\>
    * name: \<display name\>

## WebSocket Commands

### set_himado
  * Description: Changes user's himado.
  * args:
    * himado: \<himado in integer\>

### chat
  * Description: Sends a new chat message.
  * args:
    * chat: \<id of chat\>
    * msg: \<message\>

## WebSocket Messages

### names
  * Description: Loads an entire user list.
  * names: JSON

### name
  * Description: Displays the user's name.
  * name:
  * id:

### himado
  * Description: Update of a user's himado.
  * name:
  * id:
  * himado:

### chat
  * Description: Loads an entire chat log.
  * type: chat
  * id: \<id of chat\>
  * chat: JSON

### new_chat
  * Description: Append a new chat.
  * chat: \<id of chat\>
  * msg: JSON