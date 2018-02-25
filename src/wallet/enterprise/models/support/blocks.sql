/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "wallet"."eBlocks" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "wallet"."eBlocks" (
  "id" SERIAL NOT NULL PRIMARY KEY,
  "hash" TEXT NOT NULL,
  "time" BIGINT NOT NULL,
  "height" BIGINT NOT NULL);

CREATE UNIQUE INDEX "eBlocks_hash_i"
  ON "wallet"."eBlocks" ("hash");

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

