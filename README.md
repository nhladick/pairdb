# Pairdb
Pairdb is an interactive key-value database for use at the command line. Pairdb can manage multiple named database tables containing pairs of strings. Upon starting the program, a table must be created or a previously saved table must be chosen to work with. Users can add entries to a table, delete entries from a table, find entries within a table, and save a table to disk. Pairdb manages database files with its own file format. See syntax for database operations below.

`newtbl table_name`

Creates a new table in memory and sets as current table to be used for subsequent commands. Command fails if a table with *table_name* already exists. If used when another table is already set as current table, that table is saved before switching to the new table.

`use table_name`

Loads a previously saved table into memory and sets as current table to be used for subsequent commands. Command fails if a table with *table_name* does not exist. If used when another table is already set as current table, that table is saved before switching to the table specified.

`save`

Saves current table to disk.

`lstbls`

Prints list of all saved tables.

`drop table_name`

Drops table by deleting file associated with *table_name* and removing *table_name* from database table list. If *table_name* refers to the current active table, all table data is cleared from memory, and another table must be created or selected to perform any table operations.

`add key val`

Adds *key-value* pair to current table. Command fails if *key* already exists in current table.

`get key`

Returns *val* associated with previously added *key* in current table.

`del key`

Deletes *key-value* pair associated with *key* from current table.

`lsdata`

Lists all key-value pairs in current table.

`help`

Prints information on commands

`quit`

Quit interactive program and save current table to disk.

|Command                      |Result                                                     |
|-----------------------------|-----------------------------------------------------------|
|`newtbl table_name`          |Creates a new table in memory and sets as current table to be used for subsequent commands. Command fails if a table with *table_name* already exists. If used when another table is already set as current table, that table is saved before switching to the new table.|
|`use table_name`             |Loads a previously saved table into memory and sets as current table to be used for subsequent commands. Command fails if a table with *table_name* does not exist. If used when another table is already set as current table, that table is saved before switching to the table specified.|
|`save`                       |Saves current table to disk.|
|`lstbls`                     |Prints list of all saved tables.|
|`drop table_name`            |Drops table by deleting file associated with *table_name* and removing *table_name* from database table list. If *table_name* refers to the current active table, all table data is cleared from memory, and another table must be created or selected to perform any table operations.|
|`add key val`                |Adds *key-value* pair to current table. Command fails if *key* already exists in current table.|
|`get key`                    |Returns *val* associated with previously added *key* in current table.|
|`del key`                    |Deletes *key-value* pair associated with *key* from current table.|
|`lsdata`                     |Lists all key-value pairs in current table.|
|`help`                       |Prints information on commands|
|`quit`                       |Quit interactive program and save current table to disk.|

Any of the above commands can be placed in an input file with one command per line (separated by the newline character). Use the quit command on the final line. All data will be saved to disk in the table(s) specified.

Example:

file: input.txt

            newtbl table1
            add key1 val1
            add key2 val2
            quit

command line:

            pairdb < input.txt

