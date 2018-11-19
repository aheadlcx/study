//
// Created by apple on 2018/11/19.
//

#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>


#define LOG_TAG "来自jni:"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


jstring returnString(JNIEnv *env,jobject jobj){
    char* str = "I come from C＋＋";
    return env->NewStringUTF(str);
}

jstring returnName(JNIEnv *env,jobject jobj){
    char* str = "I am aheadlcx";
    return env->NewStringUTF(str);
}

static JNINativeMethod gMethods[] = {
        {"getStringFromC","()Ljava/lang/String;",(void *)returnString },
        {"getMyName", "()Ljava/lang/String;", (void *)returnName}
};

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm,void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env,JNI_VERSION_1_6) != JNI_OK){
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass("me/aheadlcx/study/ndk/Methodmanager");
    if (javaClass == NULL){
        return JNI_ERR;
    }
    jint methodSize = sizeof(gMethods) / sizeof(gMethods[0]);
    if (env->RegisterNatives(javaClass,gMethods, methodSize) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_me_aheadlcx_study_ndk_getMyAge(JNIEnv *env, jclass type) {
    return env->NewStringUTF("I am 18 and a bueatiful life");
}
