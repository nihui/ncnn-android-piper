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

#ifndef PIPER_H
#define PIPER_H

#include <net.h>

#include "simpleg2p.h"

class Piper
{
public:
    int load(const char* lang, bool use_gpu = false);
    int load(AAssetManager* mgr, const char* lang, bool use_gpu = false);

    void synthesize(const char* text, int speaker, float noise_scale, float length_scale, float noise_scale_w, std::vector<short>& pcm);

protected:
    void path_attention(const ncnn::Mat& logw, const ncnn::Mat& m_p, const ncnn::Mat& logs_p, float noise_scale, float length_scale, ncnn::Mat& z_p);

protected:
    SimpleG2P g2p;
    bool has_multi_speakers;
    ncnn::Net emb_g;
    ncnn::Net enc_p;
    ncnn::Net dp;
    ncnn::Net flow;
    ncnn::Net dec;
};

#endif // PIPER_H
