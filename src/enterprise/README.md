macOS
==

1. Download from www.codesynthesis.com/products/odb/download.xhtml: 
 + odb-2.4.0-i686-macosx binary
 + the common runtime library libodb-2.4.0
 + the database runtime libodb-pgsql-2.4.0 (we will add other databases after prototyping)
 + libodb-boost-2.4.0 profile library
 + optionally the examples odb-examples-2.4.0 
 
 Linux
 ==

0.

`sudo apt-get update && sudo apt-get upgrade && sudo apt-get install nano tmux git build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev
`


1. `sudo apt-get install libodb-2.4 libodb-dev libodb-pgsql-2.4 libodb-pgsql-dev libpq5 libodb-boost-2.4 libodb-boost-dev`

2. Decompress and move the resulting folders to a more permanent folder than Downloads.

3. In your shell config (zshrc in my case), add the ODB bin subdirectory to your path, like so <code>export PATH=/Users/pierrerochard/src/odb-2.4.0-i686-macosx/bin:$HOME/.cargo/bin:$HOME/bin:/usr/local/bin:$PATH</code>

4. In libodb, libodb-pgsql, and libodb-boost run <code>./configure  CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -stdlib=libc++"</code> <code>make -j8</code> and <code>make install</code>

5. If you want to install the examples configure with these flags <code>./configure  CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -stdlib=libc++" --with-database=pgsql</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/addresses.h</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/blocks.h</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/inputs.h</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/outputs.h</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/scripts.h</code>

* <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/transactions.h</code>

Build
==
1. `./autogen.sh`
2. `./configure --disable-wallet --without-gui --enable-debug --with-incompatible-bdb`
3. `make -j 8`