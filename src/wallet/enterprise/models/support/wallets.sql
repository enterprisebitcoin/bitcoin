/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "wallet"."eWallets" CASCADE;

DROP TABLE IF EXISTS "schema_version";

CREATE TABLE "wallet"."eWallets" (
  "id" SERIAL NOT NULL PRIMARY KEY,
  "wallet_id" UUID NOT NULL,
  "name" TEXT NOT NULL,
  "description" TEXT NOT NULL);

CREATE UNIQUE INDEX "eWallets_wallet_id_i"
  ON "wallet"."eWallets" ("wallet_id");

CREATE TABLE "schema_version" (
  "name" TEXT NOT NULL PRIMARY KEY,
  "version" BIGINT NOT NULL,
  "migration" BOOLEAN NOT NULL);

INSERT INTO "schema_version" (
  "name", "version", "migration")
  VALUES ('', 1, FALSE);

