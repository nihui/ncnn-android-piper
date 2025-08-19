// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2025 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "audioplayer.h"

#include <string.h>

#define SL_CHECK(x) if((x)!=SL_RESULT_SUCCESS) return false;

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
    stop();
    if (playerObj)      { (*playerObj)->Destroy(playerObj); }
    if (outputMixObj)   { (*outputMixObj)->Destroy(outputMixObj); }
    if (engineObj)      { (*engineObj)->Destroy(engineObj); }
}

bool AudioPlayer::init(int channels)
{
    channelCount = channels;

    // 1. Create engine
    SL_CHECK(slCreateEngine(&engineObj, 0, nullptr, 0, nullptr, nullptr));
    SL_CHECK((*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE));
    SL_CHECK((*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine));

    // 2. Create output mix
    SL_CHECK((*engine)->CreateOutputMix(engine, &outputMixObj, 0, nullptr, nullptr));
    SL_CHECK((*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE));

    // 3. Configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2
    };
    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM,
        (SLuint32)channelCount,
        SL_SAMPLINGRATE_22_05,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        channelCount == 2 ? SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT : SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // 4. Configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObj};
    SLDataSink audioSnk = {&loc_outmix, nullptr};

    // 5. Create audio player
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    SL_CHECK((*engine)->CreateAudioPlayer(engine, &playerObj, &audioSrc, &audioSnk,
                                          1, ids, req));
    SL_CHECK((*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE));
    SL_CHECK((*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerPlay));
    SL_CHECK((*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &playerBufferQueue));

    // 6. Register callback
    (*playerBufferQueue)->RegisterCallback(playerBufferQueue, bufferQueueCallback, this);

    return true;
}

bool AudioPlayer::play(const int16_t* data, size_t size)
{
    if (!playerBufferQueue || !playerPlay || !data || size == 0)
        return false;

    stop();

    pcmData = data;
    pcmSize = size;
    pcmOffset = 0;
    playing = true;

    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);

    // TODO slice parts
    size_t send = pcmSize * sizeof(int16_t);
    if (send > 0) {
        (*playerBufferQueue)->Enqueue(playerBufferQueue, (void*)pcmData, send);
        pcmOffset += pcmSize;
    }

    return true;
}

void AudioPlayer::stop()
{
    if (playerPlay) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
    }
    if (playerBufferQueue) {
        (*playerBufferQueue)->Clear(playerBufferQueue);
    }
    playing = false;
    pcmData = nullptr;
    pcmSize = 0;
    pcmOffset = 0;
}

void AudioPlayer::bufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void* ctx)
{
    // TODO slice parts
}
