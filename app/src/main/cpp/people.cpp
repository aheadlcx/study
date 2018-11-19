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
const char *RELEASE_SIGN =
        "7642E4EBB1BCCC5AA2E6D8F25AE3C652";

void ByteToHexStr(const char *source, char *dest, int sourceLen) {
    short i;
    char highByte, lowByte;

    for (i = 0; i < sourceLen; i++) {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f;
        highByte += 0x30;

        if (highByte > 0x39) {
            dest[i * 2] = highByte + 0x07;
        } else {
            dest[i * 2] = highByte;
        }

        lowByte += 0x30;
        if (lowByte > 0x39) {
            dest[i * 2 + 1] = lowByte + 0x07;
        } else {
            dest[i * 2 + 1] = lowByte;
        }
    }
}

// byte数组转MD5字符串
extern "C"
jstring ToMd5(JNIEnv *env, jbyteArray source) {
    // MessageDigest类
    jclass classMessageDigest = env->FindClass("java/security/MessageDigest");
    // MessageDigest.getInstance()静态方法
    jmethodID midGetInstance = env->GetStaticMethodID(classMessageDigest, "getInstance",
                                                      "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    // MessageDigest object
    jobject objMessageDigest = env->CallStaticObjectMethod(classMessageDigest, midGetInstance,
                                                           env->NewStringUTF("md5"));

    // update方法，这个函数的返回值是void，写V
    jmethodID midUpdate = env->GetMethodID(classMessageDigest, "update", "([B)V");
    env->CallVoidMethod(objMessageDigest, midUpdate, source);

    // digest方法
    jmethodID midDigest = env->GetMethodID(classMessageDigest, "digest", "()[B");
    jbyteArray objArraySign = (jbyteArray) env->CallObjectMethod(objMessageDigest, midDigest);

    jsize intArrayLength = env->GetArrayLength(objArraySign);
    jbyte *byte_array_elements = env->GetByteArrayElements(objArraySign, NULL);
    size_t length = (size_t) intArrayLength * 2 + 1;
    char *char_result = (char *) malloc(length);
    memset(char_result, 0, length);

// 将byte数组转换成16进制字符串，发现这里不用强转，jbyte和unsigned char应该字节数是一样的
    ByteToHexStr((const char *) byte_array_elements, char_result, intArrayLength);
    // 在末尾补\0
    *(char_result + intArrayLength * 2) = '\0';

    jstring stringResult = env->NewStringUTF(char_result);
    // release
    env->ReleaseByteArrayElements(objArraySign, byte_array_elements, JNI_ABORT);
    // 释放指针使用free
    free(char_result);
    env->DeleteLocalRef(classMessageDigest);
    env->DeleteLocalRef(objMessageDigest);

    return stringResult;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_me_aheadlcx_study_ndk_People_sayPeople(JNIEnv *env, jclass type) {
    LOGE("我在 CPP 里面了。");
    return env->NewStringUTF("hello 52pojie");
}
extern "C"
JNIEXPORT jstring JNICALL
Java_me_aheadlcx_study_ndk_People_getName(JNIEnv *env, jclass type) {
    LOGE("hello aheadlcx");
    return env->NewStringUTF("my name is aheadlcx");
}extern "C"
JNIEXPORT jboolean JNICALL
Java_me_aheadlcx_study_ndk_People_checkPackageName(JNIEnv *env, jclass type, jstring packageName_) {
    const char *packageName = env->GetStringUTFChars(packageName_, 0);
    int i = strcmp(packageName, "me.aheadlcx.study");
    env->ReleaseStringUTFChars(packageName_, packageName);
    if (i == 0) {
        LOGE("packageName check sucess");
        return static_cast<jboolean>(true);
    } else {
        return static_cast<jboolean>(false);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_me_aheadlcx_study_ndk_People_checkSign(JNIEnv *env, jclass type, jobject context) {
    jclass context_clazz = env->GetObjectClass(context);
    jmethodID getPackageManager = env->GetMethodID(context_clazz,
                                                   "getPackageManager",
                                                   "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(context,
                                                   getPackageManager);

    // context.getPackageName()
    jmethodID getPackageName = env->GetMethodID(context_clazz,
                                                "getPackageName", "()Ljava/lang/String;");
    jstring packageName = (jstring) env->CallObjectMethod(context,
                                                          getPackageName);

    // packageManager->getPackageInfo(packageName, GET_SIGNATURES);
    jclass package_manager_clazz = env->GetObjectClass(packageManager);
    jmethodID getPackageInfo = env->GetMethodID(package_manager_clazz,
                                                "getPackageInfo",
                                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jint flags = 0x00000040;
    jobject packageInfo = env->CallObjectMethod(packageManager,
                                                getPackageInfo, packageName, flags);

    jthrowable exception = env->ExceptionOccurred();
    env->ExceptionClear();
    if (exception) {
        return NULL;
    }

    // packageInfo.signatures[0]
    jclass package_info_clazz = env->GetObjectClass(packageInfo);
    jfieldID fid = env->GetFieldID(package_info_clazz, "signatures",
                                   "[Landroid/content/pm/Signature;");
    jobjectArray signatures = (jobjectArray) env->GetObjectField(
            packageInfo, fid);
    jobject signature = env->GetObjectArrayElement(signatures, 0);

    // signature.toByteArray()
    jclass signature_clazz = env->GetObjectClass(signature);
    jmethodID signature_toByteArray = env->GetMethodID(signature_clazz,
                                                       "toByteArray", "()[B");
    jbyteArray mjbyteArray = (jbyteArray) env->CallObjectMethod(
            signature, signature_toByteArray);
    jstring pJstring = ToMd5(env, mjbyteArray);

    char *c_msg = (char *) env->GetStringUTFChars(pJstring, 0);
    LOGE("sign result %s for log", c_msg);
    int i = strcmp(c_msg, RELEASE_SIGN);
    if (i == 0) {
        LOGE("packageName check sucess");
        return static_cast<jboolean>(true);
    } else {
        LOGE("packageName check faild");
        return static_cast<jboolean>(false);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_me_aheadlcx_study_ndk_People_exit(JNIEnv *env, jclass type, jint status) {
    exit(status);
}extern "C"
JNIEXPORT void JNICALL
Java_me_aheadlcx_study_ndk_People_exitReturn(JNIEnv *env, jclass type, jint status) {
    unlockpt(status);
}extern "C"
JNIEXPORT void JNICALL
Java_me_aheadlcx_study_ndk_People_showHello(JNIEnv *env, jclass type, jstring status_, jint index) {
    const char *status = env->GetStringUTFChars(status_, 0);
    LOGE("showHello  ", status);
    env->ReleaseStringUTFChars(status_, status);
}

//获取TracePid
int get_number_for_str(char *str) {
    if (str == NULL) {
        return -1;
    }
    char result[20];
    int count = 0;
    while (*str != '\0') {
        if (*str >= 48 && *str <= 57) {
            result[count] = *str;
            count++;
        }
        str++;
    }
    int val = atoi(result);
    return val;
}

//开启循环轮训检查TracePid字段
void *thread_function(void *argv) {
    int pid = getpid();
    char file_name[20] = {'\0'};
    sprintf(file_name, "/proc/%d/status", pid);
    char linestr[256];
    int i = 0, traceid;
    FILE *fp;
    while (1) {
        i = 0;
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            break;
        }
        while (!feof(fp)) {
            fgets(linestr, 256, fp);
            if (i == 5) {
                traceid = get_number_for_str(linestr);
                LOGE("traceId:%d", traceid);
                if (traceid > 1000) {
                    LOGE("I was be traced...trace pid:%d", traceid);
                    //华为P9会主动给app附加一个进程，暂且认为小于1000的是系统的
//                    exit(0);
                }
                break;
            }
            i++;
        }
        fclose(fp);
        sleep(5);
    }
    return ((void *) 0);
}

void create_thread_check_traceid() {
    pthread_t t_id;
    int err = pthread_create(&t_id, NULL, thread_function, NULL);
    if (err != 0) {
        LOGE("create thread fail: %s\n", strerror(err));
    } else {
        LOGE("create thread success");
    }
}


void *thread_function_once() {
    int pid = getpid();
    char file_name[20] = {'\0'};
    sprintf(file_name, "/proc/%d/status", pid);
    char linestr[256];
    int i = 0, traceid;
    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        return ((void *) 0);
    }
    while (!feof(fp)) {
        fgets(linestr, 256, fp);
        if (i == 5) {
            traceid = get_number_for_str(linestr);
            LOGE("traceId once :%d", traceid);
            if (traceid > 1000) {
                LOGE("I was be traced... >  1000  trace pid:%d", traceid);
                //华为P9会主动给app附加一个进程，暂且认为小于1000的是系统的
//                    exit(0);
            } else if (traceid >= 0 ){
                LOGE("I was be traced...  0  ~  1000  trace pid:%d", traceid);
            }
            break;
        }
        i++;
    }
    fclose(fp);
    return ((void *) 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_me_aheadlcx_study_ndk_People_checkIsDebug(JNIEnv *env, jclass type) {
    create_thread_check_traceid();
    // TODO

}extern "C"
JNIEXPORT void JNICALL
Java_me_aheadlcx_study_ndk_People_checkIsDebugOnce(JNIEnv *env, jclass type) {
    thread_function_once();
    // TODO

}

