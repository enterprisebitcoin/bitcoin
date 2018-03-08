macOS
==

1. Download from www.codesynthesis.com/products/odb/download.xhtml: 
 + odb-2.4.0-i686-macosx binary
 + the common runtime library libodb-2.4.0
 + the database runtime libodb-pgsql-2.4.0 (we will add other databases after prototyping)
 + libodb-boost-2.4.0 profile library
 + optionally the examples odb-examples-2.4.0 

2. Decompress and move the resulting folders to a more permanent folder than Downloads.

3. In your shell config (zshrc in my case), add the ODB bin subdirectory to your path, like so <code>export PATH=/Users/pierrerochard/src/odb-2.4.0-i686-macosx/bin:$HOME/.cargo/bin:$HOME/bin:/usr/local/bin:$PATH</code>

4. In libodb, libodb-pgsql, and libodb-boost run <code>./configure  CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -stdlib=libc++"</code> <code>make -j8</code> and <code>make install</code>

5. If you want to install the examples configure with these flags <code>./configure  CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -stdlib=libc++" --with-database=pgsql</code>

6. <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/blocks.h</code>

7. <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/outputs.h</code>

8. <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/scripts.h</code>

9. <code>odb -d pgsql --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix enterprise/models/ --schema bitcoin --generate-query --generate-schema -o src/enterprise/models src/enterprise/models/transactions.h</code>
