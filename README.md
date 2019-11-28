# BloomFilter
An implementation of basic Bloom Filter and Block Bloom Filter in C++

The repository is available [here](https://github.com/kzintas/BloomFilter)

### Command Line Interface

To make the command line interface work, execute the following two commands.  
```` bash
 shopt -s expand_aliases
source .bash_aliases 
````

### Basic Bloom Filter

#### Insert

To insert in the Basic Bloom Filter, run-

````bash
bf build -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
`````

This command reads in the keys in the file `<key file>`, which contains `<num. distinct keys>` distinct input keys, and constructs a bloom filter with a target false positive rate of `<fpr>`. The constructed Bloom filter is written to the file `<output file>`.

#### Query
To query the Basic Bloom Filter, run-

````bash
bf query -i <input file> -q <queries>
`````
This command  reads in the serialized input file (created by the build command above), and issue each of the queries in the query file.

### Blocked Bloom Filter

#### Insert

To insert in the Blocked Bloom Filter, run-

````bash
bbf build -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
`````

This command reads in the keys in the file `<key file>`, which contains `<num. distinct keys>` distinct input keys, and constructs a blocked bloom filter with a target false positive rate of `<fpr>`. The constructed Bloom filter is written to the file `<output file>`.

#### Query
To query the Blocked Bloom Filter, run-

````bash
bbf query -i <input file> -q <queries>
`````
This command  reads in the serialized input file (created by the build command above), and issue each of the queries in the query file.
