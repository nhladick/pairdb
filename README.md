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
Each database table is implemented using a hash table with quadratic probing. A table is set to double in size when the load factor (number of table entries / total table buckets) reaches 0.60. The Fowler/Noll/Vo hash function provides a fast and simple hash value for each key. When probing for open buckets upon key insertion, the following function is used to index into the table:

Index(i) = HASH(key) + (i(i + 1)) / 2) modulo S, for i = 0, 1, 2, 3,...

where Index is the table index, HASH is the hash function, key is the key to be inserted, S is the table size, and i is the probe iteration number.

The maximum number of probing iterations reached during a key insertion is saved in each table struct. When searching for a key, the initial index (Index(0) shown above) is checked first. If the key is not found at Index(0), the Index function iterates until the key is found or until i reaches the saved maximum probing depth value. This guarantees that there will be no false negatives.

In theory, the maximum probing depth that can be reached is floor(load factor * table size). This would occur when the table has one element less than maximum capacity according to the load factor (i.e., the table will be expanded if another element is added after the current addition), the hash results in a collision, and probing continues until all occupied buckets have been visited, after which the new element is inserted.

However, the probability of reaching this maximum probing depth in practice seems low. Assume we have a table of size m, let n be the number of entries in the table, and let L be the table load factor. In the situation described above, n = floor(L * m). Assuming the FNV hash disperses values well, and assuming quadratic probing acts as a form of "random selection," the probability of a collision when inserting the next key is n / m. The probability of visiting an occupied bucket after the initial collision is (n / m) * ((n - 1) / (m - 1)). This pattern continues, giving the probability of visiting every occupied bucket in this situation as (n! / m!). For a table of size 16, the probability of visiting every occupied bucket when there are 9 entries is approximately 1.7344E-8. For a table of size 32, the probability of visiting every occupied bucket when there are 19 entries is approximately 4.623E-19. The probability of visiting 10 occupied buckets in a row under these assumptions for a table of size 8,192 is approximately 0.006.

As these calculations assume ideal conditions, this hash table implementation was tested and benchmarked with varying numbers of strings of different lengths made up of pseudorandom sequences of characters. After multiple trials in which about 900,000 strings were inserted, the table size was 2,097,152 (2 to the power of 21), and the maximum probing depth ranged from 25-29 iterations. This means a maximum of roughly 0.0014% of the table buckets were searched when the full maximum probing depth had to be used.

## Limitations and Future Improvements
In the current implementation, when a table is updated, pairdb writes the entire table to disk when saving rather than updating only the data that have changed. For small tables, the performance penalty is not noticeable, but a future version of pairdb should address this limitation.

Pairdb has some limitations related to text input. Command history and command autocompletion is not supported, but in the future, support can be added with the inclusion of a library like ncurses. Additionally, pairdb does not support the input of tab characters or escaping quotation marks. Future versions should have a broader range of permissible input values.

In the future, I would like to add command line support so pairdb can be used one command at a time, without an interactive mode. This would make using pairdb in bash scripts easier and cleaner.

Future versions may also support consuming and writing data in common formats, such as CSV, XML, and JSON.
