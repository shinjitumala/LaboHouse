# LaboHouse Protocols

## Rules for all protocols
  * Any response that is not 'OK 200' is considered an error.
  * Any header parameters are required unless specified otherwise.
  * All error responses contain the error message in the header.
    * error: \<error reason\>

### registerName()
  * Register a new User
  * args:
    * ID:
    * Display name:
  
### getName()
  * return: User's display name.

### setHimado() 
  * Change 'HIMADO'
  * args:
    * Himado: \<himado in integer\>

### getHimado()
  * Get 'HIMADO'
  * return: \<your himado (string)\>

### getUsers()
  * Query list of users
  * return:  { 'Each HIMADO' { \<Users\> } }

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