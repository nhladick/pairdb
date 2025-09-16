# Pairdb
**Copyright (C) 2025 Nikolai Hladick**

---

*This is a personal learning project I completed to gain experience in program design, text parsing, software testing, and the implementation of data structures in a practical context. In a real-world environment, I would use popular, well-tested libraries to implement application components where appropriate.* 

---

Pairdb is an interactive key-value database for use at the command line. Pairdb can manage multiple named database tables containing pairs of strings. Upon starting the program, a table must be created or a previously saved table must be chosen to work with. Users can add entries to a table, delete entries from a table, find entries within a table, and save a table to disk. After completing the [build and installation steps](#build-and-usage), the executable can be run with the command `pairdb` at the command line. See the list of commands for table operations below.

## Commands

`newtbl table_name`

Creates a new table in memory and sets it as the current table to be used for subsequent commands. The command fails if a table with *table_name* already exists. If used when another table is already set as the current table, that table is saved before switching to the new table.

`use table_name`

Loads a previously saved table into memory and sets it as the current table to be used for subsequent commands. The command fails if a table with *table_name* does not exist. If used when another table is already set as the current table, that table is saved before switching to the table specified.

`save`

Saves current table to disk.

`lstbls`

Prints list of all saved tables.

`drop table_name`

Drops table by deleting the file associated with *table_name* and removing *table_name* from the database table list. If *table_name* refers to the current active table, all table data is cleared from memory, and another table must be created or selected to perform any table operations.

`add key val`

Adds *key-value* pair to the current table. The command fails if *key* already exists in the current table.

`get key`

Returns *val* associated with previously added *key* in the current table.

`del key`

Deletes *key-value* pair associated with *key* from the current table.

`lsdata`

Lists all key-value pairs in the current table.

`help`

Prints information on commands.

`quit`

Quit interactive program and save current table to disk.


Any of the above commands can be placed in an input file with one command per line (separated by the newline character). Use the quit command on the final line. All data will be saved to disk in the table(s) specified.

Example:

file: input.txt

    newtbl table1
    add key1 val1
    add key2 val2
    quit

command line:

    pairdb < input.txt

## Build and Usage
* Clone the repository: `git clone https://github.com/nhladick/pairdb`
* Navigate to the pairdb directory and run `make`
* Tests can be run with the provided script: `source test-pairdb.sh`. The script downloads three files from the [Unity testing framework](https://github.com/ThrowTheSwitch/Unity). Results are written to `/test/test_output.txt`
* Run `make install`. The `pairdb` executable will be moved to the `~/bin` directory. This directory will be created if it does not exist. Ensure this directory is on your path to use the executable. A directory `~/pairdb-data` will be created. Pairdb uses this directory to save and manage table files and application data.
* Run with `pairdb`

## Dependencies
* This project uses the [Unity testing framework](https://github.com/ThrowTheSwitch/Unity). Unity is covered under the MIT License.
* The hash table implementation uses the [Fowler/Noll/Vo hash function](https://github.com/lcn2/fnv/blob/master/hash_32a.c). This function is in the public domain.

## Notes
* The table name field is limited to 30 characters, and the key and value fields are limited to 98 characters each. Exceeding these limits results in undefined behavior.
* All provided strings other than commands may include spaces if the string is enclosed in single (') or double (") quotation marks. Pairdb does not currently support any escape characters for including quotation marks within strings.
* Pairdb does not accept the tab character within an input string.
* This tool is currently intended for use on Unix/Linux systems, as it depends on the /dev/urandom device file and POSIX functions included in unistd.h.
* Pairdb is intended for use with relatively small data sets.
* This is a personal learning project.
