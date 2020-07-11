# LaboHouse Protocols

## Rules for all protocols
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

### Query username
  * Method: POST
  * URL: /name
    * Parameters
      * Cookie: \<session cookie\>
  * Response
    * name: \<display name\>

### Change 'HIMADO'
  * Method: POST
  * URL: /sethimado
    * Parameters:
      * Himado: \<himado in integer\>
      * Cookie: \<session cookie\>
  * Response

### Get 'HIMADO'
  * Method: POST
  * URL: /gethimado
    * Parameters:
      * Cookie: \<session cookie\>
  * Response
    * Himado: \<your himado (string)\>

### Query list of users
  * Method: POST
  * URL: /names
    * Parameters
  * Response
    * Json:  { 'Each HIMADO' { \<Users\> } }

### Chat to main
  * Method: POST
  * URL: /chat_main
    * Parameters
      * Cookie: \<session cookie\>
  * Response

### Refresh chat
  * Method: POST
  * URL: /chat_main_get
    * Parameters
  * Response