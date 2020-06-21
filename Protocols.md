# LaboHouse Protocols

* User-data
  * Register a new name
    - Request
      - POST /register
      * data-values:
        - name: name
    * Response
      * 200 OK: Success.
      * 403 Forbidden: Name is already taken.
      * data-values:
        * Set-Cookie: your temporary identifier

  * Get name
    - Request
      * POST /name
      * data-values:
        * Cookie: 
    - Reponse
      * 200 OK: Success.
      * 404 Not Found: Cookie not recognized.
      * data-values:
        * name: your name if it exists
    