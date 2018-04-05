macOS
==

Follow the instructions in src/enterprise/README.md

1. <code>odb -d pgsql  -I src/ --profile boost/uuid --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix wallet/enterprise/models/ --schema wallet --generate-query --generate-schema -o src/wallet/enterprise/models src/wallet/enterprise/models/addresses.h</code>

2. <code>odb -d pgsql  -I src/ --profile boost/uuid --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix wallet/enterprise/models/ --schema wallet --generate-query --generate-schema -o src/wallet/enterprise/models src/wallet/enterprise/models/wallets.h</code>

3. <code>odb -d pgsql  -I src/ --profile boost/uuid --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix wallet/enterprise/models/ --schema wallet --generate-query --generate-schema -o src/wallet/enterprise/models src/wallet/enterprise/models/wallet_transactions.h</code>

4. <code>odb -d pgsql  -I src/ --profile boost/uuid --changelog-suffix -odb.xml --sql-suffix -odb.sql --include-prefix wallet/enterprise/views/ --schema wallet --generate-query --generate-schema -o src/wallet/enterprise/views src/wallet/enterprise/views/watch_only_addresses.h</code>
