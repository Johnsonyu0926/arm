#include "add_custom_audio_file.h"

// CustomAudioFile functions
CustomAudioFile* create_custom_audio_file(const char* id, const char* name, const char* path) {
    CustomAudioFile* file = (CustomAudioFile*)malloc(sizeof(CustomAudioFile));
    if (!file) return NULL;
    file->id = strdup(id);
    file->name = strdup(name);
    file->path = strdup(path);
    return file;
}

void destroy_custom_audio_file(CustomAudioFile* file) {
    if (file) {
        free(file->id);
        free(file->name);
        free(file->path);
        free(file);
    }
}

const char* get_id(const CustomAudioFile* file) {
    return file->id;
}

void set_id(CustomAudioFile* file, const char* new_id) {
    free(file->id);
    file->id = strdup(new_id);
}

const char* get_name(const CustomAudioFile* file) {
    return file->name;
}

void set_name(CustomAudioFile* file, const char* new_name) {
    free(file->name);
    file->name = strdup(new_name);
}

const char* get_path(const CustomAudioFile* file) {
    return file->path;
}

void set_path(CustomAudioFile* file, const char* new_path) {
    free(file->path);
    file->path = strdup(new_path);
}

json_t* custom_audio_file_to_json(const CustomAudioFile* file) {
    json_t* json = json_object();
    json_object_set_new(json, "id", json_string(file->id));
    json_object_set_new(json, "name", json_string(file->name));
    json_object_set_new(json, "path", json_string(file->path));
    return json;
}

CustomAudioFile* custom_audio_file_from_json(const json_t* json) {
    const char* id = json_string_value(json_object_get(json, "id"));
    const char* name = json_string_value(json_object_get(json, "name"));
    const char* path = json_string_value(json_object_get(json, "path"));
    return create_custom_audio_file(id, name, path);
}

// CustomAudioFileManager functions
CustomAudioFileManager* create_custom_audio_file_manager(const char* config_path) {
    CustomAudioFileManager* manager = (CustomAudioFileManager*)malloc(sizeof(CustomAudioFileManager));
    if (!manager) return NULL;
    manager->config_path = strdup(config_path);
    manager->files = NULL;
    manager->file_count = 0;
    return manager;
}

void destroy_custom_audio_file_manager(CustomAudioFileManager* manager) {
    if (manager) {
        free(manager->config_path);
        for (size_t i = 0; i < manager->file_count; ++i) {
            destroy_custom_audio_file(manager->files[i]);
        }
        free(manager->files);
        free(manager);
    }
}

bool load_custom_audio_file_manager(CustomAudioFileManager* manager) {
    FILE* file = fopen(manager->config_path, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", manager->config_path);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    size_t index;
    json_t* value;
    json_array_foreach(json, index, value) {
        CustomAudioFile* audio_file = custom_audio_file_from_json(value);
        add_file_to_manager(manager, audio_file);
    }

    json_decref(json);
    return true;
}

bool save_custom_audio_file_manager(const CustomAudioFileManager* manager) {
    json_t* json = json_array();
    for (size_t i = 0; i < manager->file_count; ++i) {
        json_t* file_json = custom_audio_file_to_json(manager->files[i]);
        json_array_append_new(json, file_json);
    }

    FILE* file = fopen(manager->config_path, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", manager->config_path);
        json_decref(json);
        return false;
    }

    if (json_dumpf(json, file, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving config file\n");
        fclose(file);
        json_decref(json);
        return false;
    }

    fclose(file);
    json_decref(json);
    return true;
}

void add_file_to_manager(CustomAudioFileManager* manager, CustomAudioFile* file) {
    manager->files = (CustomAudioFile**)realloc(manager->files, (manager->file_count + 1) * sizeof(CustomAudioFile*));
    manager->files[manager->file_count++] = file;
}

void remove_file_from_manager(CustomAudioFileManager* manager, const char* id) {
    for (size_t i = 0; i < manager->file_count; ++i) {
        if (strcmp(manager->files[i]->id, id) == 0) {
            destroy_custom_audio_file(manager->files[i]);
            for (size_t j = i; j < manager->file_count - 1; ++j) {
                manager->files[j] = manager->files[j + 1];
            }
            --manager->file_count;
            manager->files = (CustomAudioFile**)realloc(manager->files, manager->file_count * sizeof(CustomAudioFile*));
            break;
        }
    }
}

CustomAudioFile* find_file_in_manager(const CustomAudioFileManager* manager, const char* id) {
    for (size_t i = 0; i < manager->file_count; ++i) {
        if (strcmp(manager->files[i]->id, id) == 0) {
            return manager->files[i];
        }
    }
    return NULL;
}