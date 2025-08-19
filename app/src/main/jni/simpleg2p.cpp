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

#include "simpleg2p.h"

#include <stdio.h>
#include <string.h>

#include <map>
#include <vector>
#include <string>

void SimpleG2P::load(const char* lang)
{
    std::string en_bin_path = "en-word_id.bin";
    std::string lang_bin_path = lang + std::string("-word_id.bin");

    // load dict buffer
    {
        FILE* fp = fopen(en_bin_path.c_str(), "rb");

        fseek(fp, 0, SEEK_END);

        size_t len = ftell(fp);

        rewind(fp);

        en_dictbinbuf.resize(len);
        fread(en_dictbinbuf.data(), 1, len, fp);

        fclose(fp);
    }

    // build dict
    {
        const unsigned char* p = en_dictbinbuf.data();

        const char* word = (const char*)p;

        for (size_t i = 0; i < en_dictbinbuf.size(); i++)
        {
            if (en_dictbinbuf[i] == 0xff)
            {
                unsigned int first_char = get_first_char(word);

                en_dict[first_char].push_back(word);

                word = (const char*)(p + i + 1);
            }
        }
    }

    if (!lang || strcmp(lang, "en") == 0)
        return;

    // load dict buffer
    {
        FILE* fp = fopen(lang_bin_path.c_str(), "rb");

        fseek(fp, 0, SEEK_END);

        size_t len = ftell(fp);

        rewind(fp);

        dictbinbuf.resize(len);
        fread(dictbinbuf.data(), 1, len, fp);

        fclose(fp);
    }

    // build dict
    {
        const unsigned char* p = dictbinbuf.data();

        const char* word = (const char*)p;

        for (size_t i = 0; i < dictbinbuf.size(); i++)
        {
            if (dictbinbuf[i] == 0xff)
            {
                unsigned int first_char = get_first_char(word);

                dict[first_char].push_back(word);

                word = (const char*)(p + i + 1);
            }
        }
    }
}

void SimpleG2P::load(AAssetManager* mgr, const char* lang)
{
    std::string en_bin_path = "en-word_id.bin";
    std::string lang_bin_path = lang + std::string("-word_id.bin");

    // load dict buffer
    {
        AAsset* asset = AAssetManager_open(mgr, en_bin_path.c_str(), AASSET_MODE_BUFFER);

        size_t len = AAsset_getLength64(asset);

        en_dictbinbuf.resize(len);

        AAsset_read(asset, en_dictbinbuf.data(), len);

        AAsset_close(asset);
    }

    // build dict
    {
        const unsigned char* p = en_dictbinbuf.data();

        const char* word = (const char*)p;

        for (size_t i = 0; i < en_dictbinbuf.size(); i++)
        {
            if (en_dictbinbuf[i] == 0xff)
            {
                unsigned int first_char = get_first_char(word);

                en_dict[first_char].push_back(word);

                word = (const char*)(p + i + 1);
            }
        }
    }

    if (!lang || strcmp(lang, "en") == 0)
        return;

    // load dict buffer
    {
        AAsset* asset = AAssetManager_open(mgr, lang_bin_path.c_str(), AASSET_MODE_BUFFER);

        size_t len = AAsset_getLength64(asset);

        dictbinbuf.resize(len);

        AAsset_read(asset, dictbinbuf.data(), len);

        AAsset_close(asset);
    }

    // build dict
    {
        const unsigned char* p = dictbinbuf.data();

        const char* word = (const char*)p;

        for (size_t i = 0; i < dictbinbuf.size(); i++)
        {
            if (dictbinbuf[i] == 0xff)
            {
                unsigned int first_char = get_first_char(word);

                dict[first_char].push_back(word);

                word = (const char*)(p + i + 1);
            }
        }
    }
}

void SimpleG2P::clear()
{
    en_dictbinbuf.clear();
    en_dict.clear();

    dictbinbuf.clear();
    dict.clear();
}

void SimpleG2P::find(const char* word, const unsigned char*& ids) const
{
    ids = 0;

    unsigned int first_char = get_first_char(word);

    if (isdigit(word[0]))
    {
        if (dict.find(first_char) != dict.end())
        {
            const std::vector<const char*>& wordlist = dict.at(first_char);

            for (size_t i = 0; i < wordlist.size(); i++)
            {
                if (strcasecmp(wordlist[i], word) == 0)
                {
                    // hit
                    ids = (const unsigned char*)(wordlist[i] + strlen(wordlist[i]) + 1);
                    return;
                }
            }
        }

        // fallback to en
        if (en_dict.find(first_char) == en_dict.end())
        {
            return;
        }

        const std::vector<const char*>& wordlist = en_dict.at(first_char);

        for (size_t i = 0; i < wordlist.size(); i++)
        {
            if (strcasecmp(wordlist[i], word) == 0)
            {
                // hit
                ids = (const unsigned char*)(wordlist[i] + strlen(wordlist[i]) + 1);
                return;
            }
        }
    }
    else if (isalpha(word[0]))
    {
        if (en_dict.find(first_char) == en_dict.end())
        {
            return;
        }

        const std::vector<const char*>& wordlist = en_dict.at(first_char);

        for (size_t i = 0; i < wordlist.size(); i++)
        {
            if (strcasecmp(wordlist[i], word) == 0)
            {
                // hit
                ids = (const unsigned char*)(wordlist[i] + strlen(wordlist[i]) + 1);
                return;
            }
        }
    }
    else
    {
        if (dict.find(first_char) == dict.end())
        {
            return;
        }

        const std::vector<const char*>& wordlist = dict.at(first_char);

        for (size_t i = 0; i < wordlist.size(); i++)
        {
            if (strcasecmp(wordlist[i], word) == 0)
            {
                // hit
                ids = (const unsigned char*)(wordlist[i] + strlen(wordlist[i]) + 1);
                return;
            }
        }
    }
}

static bool is_word_eos(const char* word)
{
    if (((const unsigned char*)word)[0] < 128)
    {
        const char c = word[0];
        return c == ',' || c == '.' || c == ';' || c == '?' || c == '!';
    }

    return strcmp(word, "，") == 0
        || strcmp(word, "。") == 0
        || strcmp(word, "、") == 0
        || strcmp(word, "；") == 0
        || strcmp(word, "：") == 0
        || strcmp(word, "？") == 0
        || strcmp(word, "！") == 0;
}

void SimpleG2P::phonemize(const char* text, std::vector<int>& sequence_ids) const
{
    const int ID_PAD = 0; // interleaved
    const int ID_BOS = 1; // beginning of sentence
    const int ID_EOS = 2; // end of sentence
    const int ID_SPACE = 3; // space

    bool last_char_is_control = false;
    bool sentence_begin = true;
    bool sentence_end = true;

    char word[256];

    const char* p = text;
    while (*p)
    {
        if (sentence_end && !last_char_is_control)
        {
            sequence_ids.push_back(ID_BOS);
            sequence_ids.push_back(ID_PAD);
            sentence_end = false;
        }

        if (sentence_begin || last_char_is_control)
        {
            // the very first word
        }
        else
        {
            // space id
            sequence_ids.push_back(ID_SPACE);
            sequence_ids.push_back(ID_PAD);
        }

        if (isalnum((unsigned char)*p))
        {
            char* pword = word;

            // alpha or number
            *pword++ = *p++;

            // consume word
            int wordlen = 1;
            while (isalnum((unsigned char)*p) && wordlen < 233)
            {
                *pword++ = *p++;
                wordlen++;
            }

            *pword = '\0';

            if (is_word_eos(word))
            {
                if (!sentence_end)
                    sequence_ids.push_back(ID_EOS);
                sentence_end = true;
                last_char_is_control = false;
                sentence_begin = false;
                continue;
            }

            const unsigned char* ids = 0;
            this->find(word, ids);
            if (ids)
            {
                const unsigned char* pids = ids;
                while (*pids != 0xff)
                {
                    sequence_ids.push_back(*pids);
                    sequence_ids.push_back(ID_PAD);
                    pids++;
                }
            }
            else
            {
                // no such word, spell alphabet one by one
                char tmp[2] = {'\0', '\0'};
                for (size_t i = 0; i < strlen(word); i++)
                {
                    tmp[0] = word[i];
                    this->find(tmp, ids);
                    if (ids)
                    {
                        const unsigned char* pids = ids;
                        while (*pids != 0xff)
                        {
                            sequence_ids.push_back(*pids);
                            sequence_ids.push_back(ID_PAD);
                            pids++;
                        }
                        if (i + 1 != strlen(word))
                        {
                            sequence_ids.push_back(ID_SPACE);
                            sequence_ids.push_back(ID_PAD);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "word char %c not recognized\n", word[i]);
                    }
                }
            }

            last_char_is_control = false;
            sentence_begin = false;
            continue;
        }

        int len = get_char_width(p);
        if (len > 1)
        {
            char* pword = word;
            int wordlen = 0;

            for (int i = 0; i < len; i++)
            {
                *pword++ = *p++;
                wordlen++;
            }

            *pword = '\0';

            if (is_word_eos(word))
            {
                if (!sentence_end)
                    sequence_ids.push_back(ID_EOS);
                sentence_end = true;
                last_char_is_control = false;
                sentence_begin = false;
                continue;
            }

            const unsigned char* ids = 0;
            this->find(word, ids);
            while (ids && wordlen < 233)
            {
                char* pword0 = pword;
                const char* p0 = p;

                len = get_char_width(p);
                if (len > 1)
                {
                    for (int i = 0; i < len; i++)
                    {
                        *pword++ = *p++;
                        wordlen++;
                    }
                    *pword = '\0';
                }
                else
                {
                    break;
                }

                const unsigned char* ids2 = 0;
                this->find(word, ids2);
                if (ids2)
                {
                    ids = ids2;
                }
                else
                {
                    *pword0 = '\0';
                    p = p0;
                    break;
                }
            }

            if (ids)
            {
                const unsigned char* pids = ids;
                while (*pids != 0xff)
                {
                    sequence_ids.push_back(*pids);
                    sequence_ids.push_back(ID_PAD);
                    pids++;
                }
                sentence_begin = false;
                last_char_is_control = false;
            }
            else
            {
                fprintf(stderr, "word %s not recognized\n", word);
                last_char_is_control = false;
            }
        }
        else
        {
            // skip control character
            p++;
            last_char_is_control = true;
        }
    }

    if (!sentence_end)
        sequence_ids.push_back(ID_EOS);
}

int SimpleG2P::get_char_width(const char* pchar) const
{
    unsigned char c = ((const unsigned char*)pchar)[0];
    if (c < 128)
        return 1;
    if ((c & 0xe0) == 0xc0)
        return 2;
    if ((c & 0xf0) == 0xe0)
        return 3;
    if ((c & 0xf8) == 0xf0)
        return 4;

    return 1; // fallback
}

unsigned int SimpleG2P::get_first_char(const char* word) const
{
    const unsigned char* pword = (const unsigned char*)word;
    unsigned int c = pword[0];
    if (c >= 128)
    {
        // utf-8
        if ((c & 0xe0) == 0xc0)
        {
            c = (pword[0] << 8) | pword[1];
        }
        else if ((c & 0xf0) == 0xe0)
        {
            c = (pword[0] << 16) | (pword[1] << 8) | pword[2];
        }
        else if ((c & 0xf8) == 0xf0)
        {
            c = (pword[0] << 24) | (pword[1] << 16) | (pword[2] << 8) | pword[3];
        }
    }
    else
    {
        c = toupper((unsigned char)word[0]);
    }

    return c;
}
