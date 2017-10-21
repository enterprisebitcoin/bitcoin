/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "eAddresses" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "eAddresses" (
  "name" TEXT NOT NULL,
  "purpose" TEXT NOT NULL,
  "is_used" BOOLEAN NOT NULL DEFAULT FALSE,
  "id" SERIAL NOT NULL PRIMARY KEY,
  "address" TEXT NOT NULL);

CREATE UNIQUE INDEX "eAddresses_address_i"
  ON "eAddresses" ("address");

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

