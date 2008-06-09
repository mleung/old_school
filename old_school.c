
/*
 *  old_school.c
 *  old_school - Wraps the MySQL C API in an easy to use library. 
 *  
 *  Created by Michael Leung on 6/6/08.
 *  Copyright 2008. All rights reserved.
 *
 *  Compile this bad boy on OS X: gcc -I/usr/local/mysql/include old_school.c -L/usr/local/mysql/lib -lmysqlclient -lm -o test
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "old_school.h"
#include "utilities.c"

int main(int argc, char** argv) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    // TODO: Extract this out to a config file and read it. 
    if (conn = do_connect("localhost", "root", NULL, "beermenus_development")) {
        if (res = find("bars", "id, name", "id = 1", conn, NULL, NULL)) {
            while ((row = mysql_fetch_row(res)) != NULL) {
            	printf("%s \n", row[1]);
            }
        }   
        mysql_free_result(res);
        
        long rowsAffected = insert("bars", "id, name", "99999, 'poop'", conn);
        printf("%d rows affected by insert\n", rowsAffected);
        
        rowsAffected = update("bars", "name = 'poopy!'", "name = 'poop'", conn);
        printf("%d rows affected by update\n", rowsAffected);

        rowsAffected = delete("bars", "name = 'poopy!'", conn);
        printf("%d rows affected by delete\n", rowsAffected);
        
        mysql_close(conn);
    }
}

 MYSQL* do_connect(char *server, char *user, char *pwd, char *database) {

 	MYSQL *conn;
	conn = mysql_init(NULL);
	
	if (conn == NULL) {
        printf("An error occurred while trying to connect to MYSQL. So sorry.");
        return NULL;
	}

 	if (!mysql_real_connect(conn, server, user, pwd, database, 0, NULL, 0)) {
 		fprintf(stderr, "%s\n", mysql_error(conn));
        return NULL;
 	}
 	
    return conn;
   
 }
 
 // Think about passing in a struct here soon. The param list is getting a bit out of control. 
 MYSQL_RES* find(char *table, char *fields, char *conditions, MYSQL *conn, char *limit, char *joins) {
     // TODO: Add in joins.
     
     if (!valid_string(table)) {
         printf("You must supply a table name");
         return NULL;
     }
     
     // TODO: Check connection before using.

     char query[MAX_QUERY_SIZE] = "";
     MYSQL_RES *res;
     char *fieldList;
     char *selectStatement;
     char *fromStatement;
     
     fieldList = fields != NULL ? fields : "*";
    
     // This builds up the query string so the user doesn't have to.
     strcat(query,  "SELECT ");
     strcat(query, fieldList);
     strcat(query, " FROM ");
     strcat(query, table);
     if (valid_string(joins)) {
         // Concat a space so the user can just type like "INNER JOIN blah ON blah.id = blahblah.id"
         strcat(query, " ");
         strcat(query, joins);
     }
     if (valid_string(conditions)) {
         strcat(query, " WHERE ");
         strcat(query, conditions);
     }
     if (valid_string(limit)) {
         strcat(query, " LIMIT ");
         strcat(query, limit);
     }
     
     // Call and return the find query.     
     if (run_query(conn, query)) {
         return mysql_use_result(conn);
     }
     
     return NULL;
 }
 
long insert(char *table, char *fields, char *values, MYSQL *conn) {
    /* TODO: Extract this to a separate function, as it's used elsewhere. 
    *  In fact, maybe we should have a generic function for checking a value
    *  and printing a message.
    */
    if (!valid_string(table)) {
        printf("You must supply a table to insert into.");
        return 0;
    }
    if (!valid_string(fields)) {
        printf("You must supply a list of fields to insert.");
        return 0;
    }
    if (!valid_string(values)) {
        printf("You must supply a list of values to insert.");
        return 0;
    }

    char query[MAX_QUERY_SIZE] = "";
   
    // This builds up the query string so the user doesn't have to.
    strcat(query, "INSERT INTO ");
    strcat(query, table);
    strcat(query, "(");
    strcat(query, fields);
    strcat(query, ")");
    strcat(query, " VALUES(");
    strcat(query, values);
    strcat(query, ")");
    
    return run_query(conn, query);
}

long update(char *table, char *statement, char *conditions, MYSQL *conn) {
    // TODO: Separate these into two checks, and printf the appropriate message.
    if (!valid_string(table)) {
        printf("You must supply a table to update.");
        return 0;
    }
    if (!valid_string(statement)) {
        printf("You must supply an update statment.");
        return 0;
    }
    
    char query[MAX_QUERY_SIZE] = "";
   
    // This builds up the query string so the user doesn't have to.
    strcat(query, "UPDATE ");
    strcat(query, table);
    strcat(query, " SET ");
    strcat(query, statement);
    // FIXME: This is being used in other places verbatim. Not too DRY, my man. Extract method.
    if (valid_string(conditions)) {
        strcat(query, " WHERE ");
        strcat(query, conditions);
    }

    return run_query(conn, query);
}

long delete(char *table, char *conditions, MYSQL *conn) {
    if (!valid_string(table)) {
        printf("You must supply a table to delete from.");
        return 0;
    }

    char query[MAX_QUERY_SIZE] = "";

    // This builds up the query string so the user doesn't have to.
    strcat(query, "DELETE FROM ");
    strcat(query, table);
    // FIXME: This is being used in other places verbatim. Not too DRY, my man. Extract method.
    if (valid_string(conditions)) {
        strcat(query, " WHERE ");
        strcat(query, conditions);
    }

    return run_query(conn, query);
    
}

long run_query(MYSQL *conn, char *query) {
    if (mysql_query(conn, query)) {
    	fprintf(stderr, "%s\n", mysql_error(conn));
    	return 0;
    }
    
    return (long) mysql_affected_rows(conn);
}
 
 