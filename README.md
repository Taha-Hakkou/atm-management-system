# atm-management-system

## Provided application:

* An ATM management system where users can do the following actions:
    - Login/Register
    - Create a new account
    - Check the details of existing accounts
    - Update information of existing accounts
    - Remove existing accounts
    - Check list of owned accounts
    - Make transactions

* The application provided will just handle the __login__, the __creation of new accounts__ and __checking the list of owned accounts__ but you can optimise and refactor the code. The rest of the features must be implemented.

* Link: [atm-system](https://assets.01-edu.org/atm-system/atm-system.zip)

    ```
    .
    |
    ├── data
    │   ├── records.txt
    │   └── users.txt
    ├── Makefile
    └── src
        ├── auth.c
        ├── header.h
        ├── main.c
        └── system.c
    ```

* The data folder presented above will contain information about the users and their accounts:
    - users.txt (id, name, password)
    - records.txt (id, user_id, user name, account id, date of creation, country, phone nº, balance, type of account)


## NCurses

* [ncurses tutorial (3 part series)](https://dev.to/tbhaxor/introduction-to-ncurses-part-1-1bk5)

## Make

make -f Makefile

## valgrind and memory leaks