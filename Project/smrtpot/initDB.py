#!/usr/bin/env python3
# You can use sqlitebrowser for viewing the database in human readable format:
# https://sqlitebrowser.org/
import sqlite3
import os.path

def createDB(db):
    schema = './schema.sql'
    conn = sqlite3.connect(db)
    print("Opened database successfully");

    print("Creating Tables if not already created...")
    with open(schema) as fp:
        conn.executescript(fp.read())

    conn.close()
    print("Closed database successfully")

def seedDB(db):
    seed = "./seed.sql"
    conn = sqlite3.connect(db)
    print("Opened database successfully");

    print("Inserting some plants...")

    with open(seed) as fp:
        conn.executescript(fp.read())

    conn.close()
    print("Closed database successfully")

# if not already created, create db file
DB = "./database.db"

if not os.path.isfile(DB):
    file = open(DB, 'w+')
    file.close()
    createDB(DB)
else:
    print("Overwrite existing database?(Y/N)")
    answer = input()

    if answer.lower() == "y":
        os.remove(DB)
        file = open(DB, 'w+')
        file.close()
        createDB(DB)

if os.path.isfile(DB):
    print("\nRun seed script?(Y/N)")
    answer = input()

    if answer.lower() == "y":
        seedDB(DB)
