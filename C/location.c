#include <stdbool.h>  // bool type
#include <stdio.h>    // file
#include <sys/stat.h> // stat

bool _location_file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void write_location(char *location) {
    FILE *fp = fopen("location.txt", "a");
    fprintf(fp, "%s\n", location);
    fclose(fp);
}

void delete_location(void) {
    remove("location.txt");
}

bool exists_location(void) {
    return _location_file_exists("location.txt");
}
