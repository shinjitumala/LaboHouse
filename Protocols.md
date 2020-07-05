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
    * name: \<requested display name of new User\>
  * Response:
    * Set-Cookie: \<your new id\>
    * name: \<your name\>

### Query username
  * Method: POST
  * URL: /name
    * Parameters
        * Cookie: \<your user id which is also the cookie\>
  * Response
    * name: \<your display name\>

### Change 'HIMADO'
  * Method: POST
  * URL: /sethimado
    * Parameters:
      * Himado: \<himado in integer\>
      * Cookie: \<your user id which is also the cookie\>
  * Response

### Get 'HIMADO'
  * Method: POST
  * URL: /gethimado
    * Parameters:
      * Cookie: \<your user id which is also the cookie\>
  * Response
    * Himado: \<your himado (string)\>

### Query list of users
  * Method: POST
  * URL: /names
    * Parameters
  * Response
    * Json: \<array of registered names\>

### Query list of users separated by 'HIMADO'
  * Method: POST
  * URL: /names_sorted
    * Parameters
  * Response
    * Json:  { 'Each HIMADO' { \<array of users with 'HIMADO'\> } }

### Echo
  * Method: POST
  * URL: /echo
    * Parameters
      * text: \<The text to be echoed\>
  * Response

### Chat to main
  * Method: POST
  * URL: /chat_main
    * Parameters
      * Cookie: \<your user id which is also the cookie\>
  * Response

### Refresh chat
  * Method: POST
  * URL: /chat_main_get
    * Parameters
  * Response