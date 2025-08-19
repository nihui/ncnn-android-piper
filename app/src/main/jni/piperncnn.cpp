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

#include <android/asset_manager_jni.h>

#include <android/log.h>

#include <jni.h>

#include <string>
#include <vector>

#include <platform.h>
#include <benchmark.h>

#include "piper.h"
#include "audioplayer.h"

static Piper* g_piper = 0;
static int g_langid = 0;
static AudioPlayer* g_player = 0;
static std::vector<short> g_pcm;
static ncnn::Mutex lock;

extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    __android_log_print(ANDROID_LOG_DEBUG, "ncnn", "JNI_OnLoad");

    g_player = new AudioPlayer;
    g_player->init(1); // mono channel

    ncnn::create_gpu_instance();

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    __android_log_print(ANDROID_LOG_DEBUG, "ncnn", "JNI_OnUnload");

    {
        ncnn::MutexLockGuard g(lock);

        delete g_piper;
        g_piper = 0;
    }

    ncnn::destroy_gpu_instance();

    delete g_player;
    g_player = 0;
}

// public native boolean loadModel(AssetManager mgr, int langid, int cpugpu);
JNIEXPORT jboolean JNICALL Java_com_tencent_piperncnn_PiperNcnn_loadModel(JNIEnv* env, jobject thiz, jobject assetManager, jint langid, jint cpugpu)
{
    if (langid < 0 || langid > 1 || cpugpu < 0 || cpugpu > 2)
    {
        return JNI_FALSE;
    }

    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

    __android_log_print(ANDROID_LOG_DEBUG, "ncnn", "loadModel %p", mgr);

    const char* langtypes[2] =
    {
        "en",
        "zh"
    };

    const int speakertypes[5] =
    {
        0,
        1,
        2,
        3,
        4
    };

    bool use_gpu = (int)cpugpu == 1;
    bool use_turnip = (int)cpugpu == 2;

    // reload
    {
        ncnn::MutexLockGuard g(lock);

        {
            static int old_langid = 0;
            static int old_cpugpu = 0;
            if (langid != old_langid || cpugpu != old_cpugpu)
            {
                // model or cpugpu changed
                delete g_piper;
                g_piper = 0;
            }
            old_langid = langid;
            old_cpugpu = cpugpu;

            ncnn::destroy_gpu_instance();

            if (use_turnip)
            {
                ncnn::create_gpu_instance("libvulkan_freedreno.so");
            }
            else if (use_gpu)
            {
                ncnn::create_gpu_instance();
            }

            if (!g_piper)
            {
                g_piper = new Piper;

                // g_piper->load(mgr, langtypes[(int)langid], use_gpu || use_turnip);
                g_piper->load(mgr, langtypes[(int)langid], false);

                g_langid = langid;
            }
        }
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_tencent_piperncnn_PiperNcnn_synthesize(JNIEnv* env, jobject obj, jstring text, jint speakerid, jdouble length_scale)
{
    const char* c_text = env->GetStringUTFChars(text, nullptr);

    // synthesize
    {
        ncnn::MutexLockGuard g(lock);

        if (g_piper)
        {
            g_player->stop();

            float noise_scale = 0.667f;
            float noise_scale_w = 0.8f;
            if (g_langid == 0)
            {
                noise_scale = 0.333f;
                noise_scale_w = 0.333f;
            }
            if (g_langid == 1)
            {
                noise_scale = 0.667f;
                noise_scale_w = 0.8f;
            }

            g_piper->synthesize(c_text, (int)speakerid, noise_scale, (float)length_scale, noise_scale_w, g_pcm);

            NCNN_LOGE("pcm.size() = %d", (int)g_pcm.size());

            g_player->play(g_pcm.data(), g_pcm.size());
        }
    }

    env->ReleaseStringUTFChars(text, c_text);

    return JNI_TRUE;
}

}
