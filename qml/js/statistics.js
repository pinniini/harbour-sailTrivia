/*
  Statistics (database) handler functions for SailTrivia-game.
  */

.pragma library

.import QtQuick.LocalStorage 2.0 as Sql

var initialized = false;

// ----------------------------------------------------------
// Database handling.

// Initializes the database.
function initializeDatabase()
{
    console.log("Initializing database...")
    var db = Sql.LocalStorage.openDatabaseSync("SailTrivia", "1.0", "SailTrivia database.", 1000000);
    db.transaction(
                function(tx) {
                    // Create Statistics-table if it doesn't already exist.
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Statistics(key TEXT, numericValue INT, textValue TEXT)');

                    // Insert statistics.
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "gamesStarted", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="gamesStarted")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "gamesPlayed", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="gamesPlayed")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "gamesAborted", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="gamesAborted")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "totalCount", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="totalCount")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "correctCount", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="correctCount")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "incorrectCount", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="incorrectCount")');
                    tx.executeSql('INSERT INTO Statistics(key, numericValue, textValue) SELECT "skippedCount", 0, "0" WHERE NOT EXISTS(SELECT 1 FROM Statistics WHERE key="skippedCount")');
                }
                )
    console.log("Database initialized...")
    initialized = true
}

// Returns the database or false if database is not initialized and for some reason initializing does not work.
function getDatabase()
{
    // Check if the database is initialized or not.
    if(!initialized)
    {
        console.log("Database not initialized...")
        initializeDatabase()

        if(initialized)
        {
            var db = Sql.LocalStorage.openDatabaseSync("SailTrivia", "1.0", "SailTrivia database.", 1000000);
            return db
        }
        else
            return false
    }
    else // Database initialized.
    {
        var dab = Sql.LocalStorage.openDatabaseSync("SailTrivia", "1.0", "SailTrivia database.", 1000000);
        return dab
    }
}

function getStatistics() {
    var db = getDatabase()
    if (db)
    {
        var statistics
        db.transaction(
            function(tx) {
                statistics = tx.executeSql('SELECT rowid AS id, key, numericValue, textValue FROM Statistics')
            }
        )
        return statistics
    }
    else
        return false
}

function getStatisticsForKey(key) {
    var db = getDatabase()
    if(db)
    {
        var statistics
        db.transaction(
            function(tx) {
                statistics = tx.executeSql('SELECT rowid AS id, key, numericValue, textValue FROM Statistics WHERE key=?', [key])
            }
        )
        return statistics
    }
    else
        return false
}

function upsertStatistic(key, numericValue, textValue) {
    var db = getDatabase()
    var id = -1

    if (db) {
        var changedCount = 0
        db.transaction(
            function(tx) {
                var result = tx.executeSql('SELECT rowid AS id FROM Statistics WHERE key=?', [key])

                // No rows found -> insert.
                if (result.rows.length === 0) {
                    var res = tx.executeSql('INSERT INTO Statistics (key, numericValue, textValue) VALUES (?,?,?)', [key, numericValue, textValue])
                    id = parseInt(res.insertId)
                    if (id === NaN) {
                        return false
                    }
                }
                // Row found -> update.
                else {
                    var resu = tx.executeSql('UPDATE Statistics SET numericValue=?, textValue=? where key=?', [numericValue, textValue, key])
                    if (resu.rowsAffected === 1) {
                        id = result.rows[0].id
                    }
                    else {
                        return false
                    }
                }
            }
        )
        return id
    }
    else {
        return false
    }
}
