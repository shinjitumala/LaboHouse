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

### Set 'HIMADO'
  * type: set_himado
  * args:
    * himado: \<himado in integer\>

### Chat
  * type: chat
  * args:
    * chat: \<id of chat\>
    * msg: \<message\>

## WebSocket Messages

### names
  * type: names
  * names: JSON

### 'HIMADO' change
  * type: himado
  * name:
  * himado:

### chat
  * type: chat
  * chat: \<id of chat\>

### new chat
  * type: new_chat
  * chat: \<id of chat\>
  * msg: JSON