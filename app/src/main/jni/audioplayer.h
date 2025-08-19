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

#pragma once
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <cstdint>

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();

    bool init(int channels = 1);

    // 16bit, 22050Hz
    bool play(const int16_t* data, size_t size);

    void stop();

private:
    static void bufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void* ctx);

    SLObjectItf engineObj = nullptr;
    SLEngineItf engine = nullptr;
    SLObjectItf outputMixObj = nullptr;
    SLObjectItf playerObj = nullptr;
    SLPlayItf playerPlay = nullptr;
    SLAndroidSimpleBufferQueueItf playerBufferQueue = nullptr;

    const int16_t* pcmData = nullptr;
    size_t pcmSize = 0;
    size_t pcmOffset = 0;
    int channelCount = 1;
    bool playing = false;
};
