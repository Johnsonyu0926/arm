#ifndef AUDIOPLAYUTILS_H
#define AUDIOPLAYUTILS_H

void AudioPlayUtil_tts_loop_play(const char *txt, bool async_start, int speed, int voice_type);
void AudioPlayUtil_tts_num_play(int duration, const char *txt, bool async_start, int speed, int voice_type);
void AudioPlayUtil_tts_time_play(int duration, const char *txt, bool async_start, int speed, int voice_type);
void AudioPlayUtil_audio_stop();

#endif // AUDIOPLAYUTILS_H