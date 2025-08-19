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

package com.tencent.piperncnn;

import android.Manifest;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.NumberPicker;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.text.Editable;
import android.text.TextWatcher;

import java.util.Random;

public class MainActivity extends Activity
{
    private PiperNcnn piperncnn = new PiperNcnn();
    private String text;

    private EditText textInput;
    private EditText editSpeaker;
    private int speakerMin = 0;
    private int speakerMax = 0;

    private Spinner spinnerLang;
    private Spinner spinnerCPUGPU;
    private int current_lang = 0;
    private int current_speaker = 0;
    private int current_cpugpu = 0;

    private String[] randomEnTexts = {
        "The quick brown fox jumps over the lazy dog.",
        "This is a sample sentence for testing purposes.",
        "Android development can be both fun and challenging.",
        "Please enter your username and password.",
        "How are you doing today?",
        "The weather is nice and sunny.",
        "I would like a cup of coffee, please.",
        "Open the window to let in some fresh air.",
        "She sells seashells by the seashore.",
        "Practice makes perfect.",
        "Can you help me with my homework?",
        "The train will arrive at the station soon.",
        "My favorite color is blue.",
        "Tomorrow is another day.",
        "It is important to back up your data regularly.",
        "He plays the guitar very well.",
        "The cat is sleeping on the sofa.",
        "Let’s go for a walk in the park.",
        "The book is on the table.",
        "Thank you for your help!",
        "After several rounds of testing and improvement, the project finally achieved the desired results, making everyone on the team feel proud.",
        "Taking time to be with your family amidst a busy work schedule is both important and meaningful.",
        "If you encounter a problem you can’t solve, try discussing it with your colleagues; you might get new inspiration.",
        "He carefully read every document to ensure he could deliver a thorough and organized report at the meeting.",
        "Due to unexpected weather changes, the planned outdoor activities had to be canceled, which made everyone a bit disappointed.",
        "The restaurant offers a wide variety of dishes, catering to people’s preferences for both Chinese and Western cuisine.",
        "The development of the internet has greatly changed the way people live and access information.",
        "Maintaining curiosity and a positive attitude is very important when learning new things.",
        "He expressed his ideas fluently during the speech and received a big round of applause from the audience.",
        "When facing stress, proper exercise and rest can help relieve tension.",
        "She carefully prepared for the performance, hoping to give the audience a wonderful show.",
        "Thanks to the joint efforts of all team members, the project was completed a week ahead of schedule.",
        "Please strictly follow the operating procedures to ensure safety at every stage of the work process.",
        "When night falls, the city lights up, creating a beautiful scenery in the darkness.",
        "He continuously challenges himself to improve his professional skills and overall competence.",
        "We should cherish the time spent with friends, as beautiful memories can always warm our hearts.",
        "As society develops, people are paying more and more attention to healthy living.",
        "This journey allowed me to experience different cultures and customs, and I learned a lot.",
        "When faced with a choice, carefully weighing the pros and cons is key to making the right decision.",
        "I hope that in the days to come, we can all achieve our goals and dreams.",
        "The sun was shining brightly as we walked through the park this morning.",
        "She spent several hours reading her favorite novel by the window.",
        "Technology continues to advance at an unprecedented pace in our daily lives.",
        "The meeting was postponed due to unforeseen circumstances.",
        "He enjoys listening to music while working on his assignments.",
        "Please remember to save your work frequently to avoid losing important data.",
        "The children were excited to visit the new science museum in the city.",
        "After dinner, we decided to go for a relaxing walk along the river.",
        "Learning a new language can open many doors to different cultures and opportunities.",
        "The movie received excellent reviews for its compelling storyline and visual effects.",
        "She carefully planned her trip to ensure everything would go smoothly.",
        "Sometimes, taking a short break can help you regain focus and productivity.",
        "He was proud of his achievements and looked forward to new challenges.",
        "The library offers a quiet environment for students to study and do research.",
        "The chef prepared a delicious meal using fresh ingredients from the market.",
        "If you have any questions, please do not hesitate to ask for assistance.",
        "The project was completed on time thanks to everyone’s hard work and dedication.",
        "They watched the sunset together, enjoying the peaceful moment in silence.",
        "Traveling allows us to broaden our horizons and gain new experiences.",
        "She hopes to continue learning and growing throughout her career."
    };

    private String[] randomZhTexts = {
        "今天天气真好。",
        "我喜欢学习编程。",
        "请问你叫什么名字？",
        "祝你生活愉快！",
        "这本书非常有趣。",
        "我们一起去吃饭吧。",
        "你会说英语吗？",
        "时间过得真快。",
        "他正在看电视。",
        "你喜欢什么运动？",
        "今天是星期几？",
        "明天有重要的会议。",
        "我家有一只小猫。",
        "这个问题很简单。",
        "请把门关上。",
        "现在几点了？",
        "我想喝一杯水。",
        "你的手机响了。",
        "这里风景很美。",
        "谢谢你的帮助！",
        "今天的天气格外晴朗，阳光透过窗户洒在书桌上，让人感到温暖和惬意。",
        "我一直对人工智能领域充满兴趣，希望将来能够从事相关的研究与开发工作。",
        "请问您能详细介绍一下这个产品的功能和使用方法吗？",
        "每当夜深人静的时候，我喜欢一个人静静地思考人生的意义和未来的方向。",
        "他花了很长时间终于完成了这项复杂的工程项目，取得了大家的认可和赞赏。",
        "虽然遇到了许多困难和挫折，但她始终没有放弃自己的梦想。",
        "我们计划下个月一起去旅行，欣赏大自然的美景，并放松身心。",
        "你觉得这部电影的剧情如何，演员们的表演是否让你感到满意？",
        "由于天气原因，原定于今天举行的运动会被推迟到了下周一。",
        "通过不断努力学习，他的英语水平有了显著的提高。",
        "如果你有任何问题或建议，欢迎随时与我们联系，我们会尽快回复你。",
        "在这个信息化时代，掌握计算机技能变得越来越重要。",
        "每天抽出时间锻炼身体，有助于保持健康的生活状态。",
        "她认真地听取了老师的建议，并在考试中取得了优异的成绩。",
        "我们小组正在讨论如何更有效地完成这项团队合作任务。",
        "随着科技的发展，智能家居产品逐渐走进了千家万户。",
        "请按照说明书上的步骤正确安装和使用本设备，以确保安全。",
        "他在会议上提出了许多有建设性的意见，受到了同事们的好评。",
        "生活中难免会遇到挫折，但只要坚持不懈，总会看到希望的曙光。",
        "由于系统升级，部分功能暂时无法使用，给您带来的不便敬请谅解。",
        "经过多次实验和改进，这个项目终于取得了理想的成果，让团队成员们都感到非常欣慰。",
        "在繁忙的工作之余，抽出时间陪伴家人是一件非常重要且有意义的事情。",
        "如果遇到无法解决的问题，不妨与同事交流一下，或许能够获得新的灵感和思路。",
        "他认真阅读了每一份资料，确保在会议上能够做出充分而有条理的汇报。",
        "由于突发的天气变化，原定的户外活动不得不临时取消，大家都感到有些遗憾。",
        "这家餐厅的菜品丰富多样，无论是中餐还是西餐都能满足不同客人的需求。",
        "互联网的发展极大地改变了人们的生活方式和信息获取的渠道。",
        "在学习新知识的过程中，保持好奇心和积极的态度非常重要。",
        "他在演讲中流利地表达了自己的观点，赢得了听众的阵阵掌声。",
        "面对压力时，适当的运动和休息有助于缓解紧张的情绪。",
        "她用心准备了这次演出，希望能够给大家带来一场精彩的表演。",
        "通过团队成员的共同努力，项目进度比预期提前了一个星期完成。",
        "请严格遵守操作规程，确保工作过程中每一个环节的安全。",
        "每当夜晚降临，城市的灯光点缀在黑夜中，显得格外美丽。",
        "他不断挑战自我，努力提高自己的专业技能和综合素质。",
        "我们应该珍惜与朋友相处的时光，因为美好的回忆总能温暖人心。",
        "随着社会的发展，人们对健康生活的关注度逐渐提高。",
        "这次旅行让我见识了不同的文化和风土人情，收获颇丰。",
        "在面对选择时，仔细权衡利弊是做出正确决策的关键。",
        "希望在未来的日子里，我们都能够实现自己的目标和梦想。",
        "你说职能部门，一届一届换了多少足球协会主席了，改过吗？换汤不换药。卡马乔也有理由说，我带的是什么队，我带的是西班牙队，你这批人是什么人啊，你们叫我带。中国足球现在什么水平，就这么几个人，赵鹏什么的都在踢中卫，他能踢吗？踢不了！没这个能力知道吗？在这样下去就要输越南了，泰国队输完输越南，再输缅甸，接下来没人输了。谢天谢地，像这样的比赛本身就没有打好基础，你能保证2016或者2017年关键比赛能赢啊？务实一点，我劝你们把自己战术打法，足球理念先搞懂，小高带的蛮好的，你把他换了干什么，你在合肥输个1比5，你倒告诉我怎么解释，脸都不要了。"
    };

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        textInput = (EditText) findViewById(R.id.textInput);

        Button buttonRandomText = (Button) findViewById(R.id.buttonRandomText);
        buttonRandomText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {

                if (current_lang == 0)
                {
                    int index = new Random().nextInt(randomEnTexts.length);
                    textInput.setText(randomEnTexts[index]);
                }
                if (current_lang == 1)
                {
                    int index = new Random().nextInt(randomZhTexts.length);
                    textInput.setText(randomZhTexts[index]);
                }
            }
        });

        editSpeaker = findViewById(R.id.editSpeaker);
        editSpeaker.setText(String.valueOf(current_speaker));

        editSpeaker.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
            }

            @Override
            public void afterTextChanged(Editable s) {
                String val = s.toString().trim();
                try
                {
                    int speakerId;
                    if (val.isEmpty())
                        speakerId = 0;
                    else
                        speakerId = Integer.parseInt(val);

                    if (speakerId < speakerMin) speakerId = speakerMin;
                    if (speakerId > speakerMax) speakerId = speakerMax;
                    if (speakerId != current_speaker)
                    {
                        current_speaker = speakerId;
                    }
                    if (!val.equals(String.valueOf(speakerId)))
                    {
                        editSpeaker.removeTextChangedListener(this);
                        editSpeaker.setText(String.valueOf(speakerId));
                        editSpeaker.setSelection(editSpeaker.getText().length());
                        editSpeaker.addTextChangedListener(this);
                    }
                }
                catch (NumberFormatException e)
                {
                    editSpeaker.removeTextChangedListener(this);
                    editSpeaker.setText(String.valueOf(current_speaker));
                    editSpeaker.setSelection(editSpeaker.getText().length());
                    editSpeaker.addTextChangedListener(this);
                }
            }
        });

        Button buttonPlay1x = (Button) findViewById(R.id.buttonPlay1x);
        buttonPlay1x.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {

                text = textInput.getText().toString();

                if (!text.trim().isEmpty())
                    piperncnn.synthesize(text, current_speaker, 1.0);
            }
        });

        Button buttonPlay0p5x = (Button) findViewById(R.id.buttonPlay0p5x);
        buttonPlay0p5x.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {

                text = textInput.getText().toString();

                if (!text.trim().isEmpty())
                    piperncnn.synthesize(text, current_speaker, 2.0);
            }
        });

        Button buttonPlay2x = (Button) findViewById(R.id.buttonPlay2x);
        buttonPlay2x.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {

                text = textInput.getText().toString();

                if (!text.trim().isEmpty())
                    piperncnn.synthesize(text, current_speaker, 0.5);
            }
        });

        spinnerLang = (Spinner) findViewById(R.id.spinnerLang);
        spinnerLang.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id)
            {
                if (position != current_lang)
                {
                    current_lang = position;
                    reload();
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
            }
        });

        spinnerCPUGPU = (Spinner) findViewById(R.id.spinnerCPUGPU);
        spinnerCPUGPU.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id)
            {
                if (position != current_cpugpu)
                {
                    current_cpugpu = position;
                    reload();
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
            }
        });

        reload();

        textInput.requestFocus();
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
    }

    private void reload()
    {
        boolean ret_init = piperncnn.loadModel(getAssets(), current_lang, current_cpugpu);
        if (!ret_init)
        {
            Log.e("MainActivity", "piperncnn loadModel failed");
        }

        if (current_lang == 0)
        {
            speakerMin = 0;
            speakerMax = 903;
            editSpeaker.setHint("0 ~ 903");
        }
        if (current_lang == 1)
        {
            speakerMin = 0;
            speakerMax = 0;
            editSpeaker.setHint("0");
        }
        current_speaker = speakerMin;
        editSpeaker.setText(String.valueOf(current_speaker));
    }

    @Override
    public void onResume()
    {
        super.onResume();
    }

    @Override
    public void onPause()
    {
        super.onPause();
    }
}
