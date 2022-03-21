#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define CONFIGFILE "config.txt"

enum {
    OPEN_FILE_ERR = -1,
    PARSE_FILE_ERR = -2,
    MAXPATH = 128,
    MAXSTR = 32,
};

typedef struct InterestedFieldsType InterestedFieldsType;
typedef struct FieldsListType FieldsListType;
typedef struct ColumnType ColumnType;
typedef struct DataListType DataListType;

struct DataListType {
    char data[MAXSTR];
    DataListType* next;
};

struct ColumnType {
    DataListType* first;
};

struct FieldsListType {
    char field_name[MAXSTR];   // name of the csv field (collumn)
    int column;                // number of the collumn
    ColumnType collumn;
    FieldsListType* next;
};

struct InterestedFieldsType {
    FieldsListType* first;
};

int
fields_empty(char * field, char * collumn)
{
    // Returns 1 if field or collumn is empty
    int field_isempty, collumn_isempty;

    field_isempty = strncmp(field, "", strlen(""));
    collumn_isempty = strncmp(collumn, "", strlen(""));

    return field_isempty || collumn_isempty;
}

void
exit_program(int st_code, InterestedFieldsType * fields_list)
{
    char *msg = NULL;

    switch(st_code) {
    case OPEN_FILE_ERR:
        msg = (char*)malloc((sizeof(char) * strlen("[ERR] Can't open the config file")) + 1);
        strcpy(msg, "[ERR] Can't open the config file");
        break;
    case PARSE_FILE_ERR:
        msg = (char*)malloc((sizeof(char) * strlen("[ERR] Error parsing config file")) + 1);
        strcpy(msg, "[ERR] Error parsing config file");
        break;
    default:
        msg = (char*)malloc((sizeof(char) * strlen("[ERR] Unkown Error")) + 1);
        strcpy(msg, "[ERR] Unkown Error");
    }
    err(1, "%s", msg);
}

void
save_fields(char * field, char * collumn, InterestedFieldsType * fields)
{
    if (fields_empty(field, collumn)) {
        printf("Campo Vacio");
        return;
    }
}

int
read_until(FILE * fd, char end_c, char * buffer, int * is_eof)
{
    char c;
    int finish = 0;
    int buffer_pos = 0;

    if (is_eof != NULL) {
        *is_eof = 0;
    }

    while (!finish) {
        c = fgetc(fd);

        // What happen if I find an EOF:

        if (c == EOF && is_eof == NULL) {
            // error only if I had read some chars before the eof
            return buffer_pos == 0;
        }
        if (c == EOF && is_eof != NULL) {
            *is_eof = 1;
            buffer[buffer_pos] = '\0';   // End the string
            return 1;
        }
        // What happen if I find any other char (included EOL):

        if (c != '\n' && c != end_c) {
            buffer[buffer_pos] = c;
            buffer_pos++;
        }
        if (c == end_c) {
            buffer[buffer_pos] = '\0';
            buffer_pos++;
            finish = 1;
        }
    }
    return 1;
}

int
get_field(FILE * fd, char * buffer)
{
    char end_char = ' ';
    return read_until(fd, end_char, buffer, NULL);
}

int get_collumn(FILE * fd, char * buffer, int * is_eof)
{
    char end_char = '\n';   // EOL character for UNIX
    return read_until(fd, end_char, buffer, is_eof);
}

int
config(char * csv_file, InterestedFieldsType * fields)
{
    /*
     * Returns 0 if everything is ok. Returns < 0 otherwise
     */

    FILE *fd;
    char field[MAXSTR], collumn[MAXSTR];

    fd = fopen(csv_file, "r");
    if (fd == NULL) {
        return OPEN_FILE_ERR;
    }
    // Parse file and save the config params

    int is_eof = 0;
    while (!is_eof) {
        memset(field, '\0', MAXSTR);
        memset(collumn, '\0', MAXSTR);
        if (!get_field(fd, field)) {
            fclose(fd);
            return PARSE_FILE_ERR;
        }
        if (!get_collumn(fd, collumn, &is_eof)) {
            fclose(fd);
            return PARSE_FILE_ERR;
        }

        // [If field and collumn are ok, continue the program execution]
        save_fields(field, collumn, fields);
    }

    fclose(fd);
    return 0;
}

int
main(int argc, char *argv[])
{
    char csv_file[MAXPATH];
    InterestedFieldsType interested_fields;
    int status;

    // Variables initialization

    interested_fields.first = NULL;
    memset(csv_file, '\0', MAXPATH);

    if (strncpy(csv_file, CONFIGFILE, (sizeof(char) * strlen(CONFIGFILE)) + 1) == NULL) {
        err(1, "[ERR] Invalid Config File\n");
    }

    // Init the configuration

    status = config(csv_file, &interested_fields);
    if (status < 0)
        exit_program(status, &interested_fields);

    exit(EXIT_SUCCESS);
}