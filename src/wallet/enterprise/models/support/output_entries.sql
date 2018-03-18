/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "wallet"."eOutputEntries" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "wallet"."eOutputEntries" (
  "etransaction_id" INTEGER NOT NULL,
  "vector" INTEGER NOT NULL,
  "amount" BIGINT NOT NULL,
  "category" TEXT NOT NULL,
  "destination" TEXT NOT NULL,
  "id" SERIAL NOT NULL PRIMARY KEY);

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

