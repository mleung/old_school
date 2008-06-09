#include <my_global.h>
#include <mysql.h>

// This is the max size for all our query string buffers.
#define MAX_QUERY_SIZE 1001

// Connects to the databaes and returns the connection (MYSQL) object.
MYSQL* do_connect(char *server, char *user, char *pwd, char *database);

// Does a select query, and returns the results.
MYSQL_RES* find(char *table, char *fields, char *conditions, MYSQL *conn, char *limit, char *joins);

// Inserts a record into the database.
long insert(char *table, char *fields, char *values, MYSQL *conn);

// Updates a record.
long update(char *table, char *statment, char *conditions, MYSQL *conn);

// Deletes a record from the database;
long delete(char *table, char *conditions, MYSQL *conn);

// Runs an arbitrary query.
long run_query(MYSQL *conn, char *query);