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

#ifndef SIMPLEG2P_H
#define SIMPLEG2P_H

#include <stdio.h>
#include <string.h>

#include <map>
#include <vector>
#include <string>

#include <android/asset_manager.h>

class SimpleG2P
{
public:
    void load(const char* lang);
    void load(AAssetManager* mgr, const char* lang);

    void clear();

    void find(const char* word, const unsigned char*& ids) const;
    void phonemize(const char* text, std::vector<int>& sequence_ids) const;

protected:
    int get_char_width(const char* pchar) const;
    unsigned int get_first_char(const char* word) const;

protected:
    std::vector<unsigned char> en_dictbinbuf;
    std::map<unsigned int, std::vector<const char*> > en_dict;

    std::vector<unsigned char> dictbinbuf;
    std::map<unsigned int, std::vector<const char*> > dict;
};

#endif // SIMPLEG2P_H
