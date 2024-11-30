#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* fileName;
} FileUpload;

FileUpload* create_file_upload(const char* fileName);
void destroy_file_upload(FileUpload* upload);
const char* get_file_name(const FileUpload* upload);
void set_file_name(FileUpload* upload, const char* newFileName);
bool upload_file(const FileUpload* upload, const char* sourcePath, const char* destinationPath);
json_t* file_upload_to_json(const FileUpload* upload);
FileUpload* file_upload_from_json(const json_t* json);

#endif // FILEUPLOAD_H