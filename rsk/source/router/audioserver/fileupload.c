#include "fileupload.h"
#include <sys/stat.h>
#include <unistd.h>

// 创建文件上传对象
FileUpload* create_file_upload(const char* fileName) {
    FileUpload* upload = (FileUpload*)malloc(sizeof(FileUpload));
    if (!upload) return NULL;
    upload->fileName = strdup(fileName);
    return upload;
}

// 销毁文件上传对象
void destroy_file_upload(FileUpload* upload) {
    if (upload) {
        free(upload->fileName);
        free(upload);
    }
}

// 获取文件名
const char* get_file_name(const FileUpload* upload) {
    return upload->fileName;
}

// 设置文件名
void set_file_name(FileUpload* upload, const char* newFileName) {
    free(upload->fileName);
    upload->fileName = strdup(newFileName);
}

// 上传文件
bool upload_file(const FileUpload* upload, const char* sourcePath, const char* destinationPath) {
    struct stat st;
    if (stat(sourcePath, &st) != 0) {
        fprintf(stderr, "Source file does not exist: %s\n", sourcePath);
        return false;
    }

    char* destDir = strdup(destinationPath);
    char* lastSlash = strrchr(destDir, '/');
    if (lastSlash) {
        *lastSlash = '\0';
        mkdir(destDir, 0755);
    }
    free(destDir);

    FILE* source = fopen(sourcePath, "rb");
    if (!source) {
        fprintf(stderr, "Failed to open source file: %s\n", sourcePath);
        return false;
    }

    FILE* dest = fopen(destinationPath, "wb");
    if (!dest) {
        fprintf(stderr, "Failed to open destination file: %s\n", destinationPath);
        fclose(source);
        return false;
    }

    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    bool success = (ferror(source) == 0 && ferror(dest) == 0);
    fclose(source);
    fclose(dest);
    return success;
}

// 将文件上传对象转换为JSON对象
json_t* file_upload_to_json(const FileUpload* upload) {
    json_t* json = json_object();
    json_object_set_new(json, "fileName", json_string(upload->fileName));
    return json;
}

// 从JSON对象创建文件上传对象
FileUpload* file_upload_from_json(const json_t* json) {
    const char* fileName = json_string_value(json_object_get(json, "fileName"));
    return create_file_upload(fileName);
}