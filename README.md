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

## Implementation Details
Each database table is implemented using a hash table with quadratic probing. A table is set to double in size when the load factor (number of table entries / total table buckets) reaches 0.60. The Fowler/Noll/Vo hash function provides a fast and simple hash value for each key. When probing for open buckets upon key insertion, the term (i(i + 1)) / 2 is added to the hash value, and this sum is used to index into the table:

Index(i) = HASH(key) + (i(i + 1)) / 2) modulo S, for i = 0, 1, 2, 3,...

where Index is the table index, HASH is the hash function, key is the key to be inserted, and S is the table size.

The maximum number of probing iterations reached during a key insertion is saved in each table struct. When determining whether a key exists in a table or searching for a key, the initial index (Index(0) shown above) is checked first. If the key is not found at Index(0), the Index function iterates until i reaches the saved maximum probing depth value plus a constant offset value. This ensures the search algorithm does not degrade to linear time while providing a reasonable range within which to search.

An offset value is used because the required probing depth to find a given key after insertion is likely to change when a table is resized. However, it is very unlikey the probing depth will increase, as the load factor decreases with a table resize. The constant offset value should be between 10-30. In tests involving thousands of strings, maximum probing depth typically ranged from around 10 to around 17. Since 10-17 more iterations is not too expensive, the sum of the maximum depth and this offset is used to balance the probability of finding the key and avoiding undesirable time complexity.

The probability of reaching a probing depth of around 30 seems low. In the worst case scenario, a table is around half full during an insertion. Assuming a half full table, assuming the FNV hash disperses values well, and assuming quadratic probing acts as a form of "random selection," there is a 50% chance of choosing an occupied table bucket on the first attempt. The probability of consecutively choosing another occupied bucket will be (1/2) * ((occupied buckets - 1) / (total buckets - 1)). As an example, consider a 64 bucket table with 32 occupied buckets. The probability of choosing consecutive occupied buckets while probing is:

(32/64)(31/63)(30/62)...

According to these assumptions, the probability of choosing 10 consecutive buckets in this situation would be about 0.0004258; the probability of choosing 20 consecutive buckets would be about 1.1508E-8. The probability of choosing 20 consecutive occupied buckets in a larger table that is half full, one with 8,192 buckets, is about 9.3176E-7.

Since these probabilities assume very favorable conditions, I used an offset value of 20 to be added to the maximum probing depth that will likely be around 10-20, so each search loop, if needed, will likely run a maximum of about 30-50 times.

## Limitations and Future Improvements
In the current implementation, when a table is updated, pairdb writes the entire table to disk when saving rather than updating only the data that have changed. For small tables, the performance penalty is not noticeable, but a future version of pairdb should address this limitation.

Pairdb has some limitations related to text input. Command history and command autocompletion is not supported, but in the future, support can be added with the inclusion of a library like ncurses. Additionally, pairdb does not support the input of tab characters or escaping quotation marks. Future versions should have a broader range of permissible input values.

In the future, I would like to add command line support so pairdb can be used one command at a time, without an interactive mode. This would make using pairdb in bash scripts easier and cleaner.

Future versions may also support consuming and writing data in common formats, such as CSV, XML, and JSON.
