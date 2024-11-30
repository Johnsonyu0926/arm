#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <stddef.h>
#include <jansson.h>

typedef struct {
    int type;
    char* fileName;
    size_t size;
} AudioFile;

AudioFile* create_audio_file(int type, const char* fileName, size_t size);
void destroy_audio_file(AudioFile* file);
json_t* audio_file_to_json(const AudioFile* file);
AudioFile* audio_file_from_json(const json_t* json);

#endif // AUDIOFILE_H