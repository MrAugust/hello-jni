/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <assert.h>

//��־�����ڶ�̬ע��ı��ط����е��ã�������ԣ���� DYNAMIC_REGISTER �ĺ�
#define TAG "HELLO"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

//���Ժ꣺ʹ�ö�̬ע�᱾�ط���
#define DYNAMIC_REGISTER
//���Ժ꣺so�����
#define CRASH

#ifdef DYNAMIC_REGISTER

JavaVM *g_jvm;

# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

jstring native_stringFromJNI( JNIEnv* env, jobject thiz )
{
	LOGV("native_stringFromJNI");
	LOGD("native_stringFromJNI");
	LOGI("native_stringFromJNI");
	LOGW("native_stringFromJNI");
	LOGE("native_stringFromJNI");

#ifdef CRASH
	char* cTemp = NULL;
	cTemp[100] = 1;
#endif

	return (*env)->NewStringUTF(env, "Hello from JNI ! DYNAMIC_REGISTER");
}

static const char *classPathName = "com/example/hellojni/HelloJni";

static JNINativeMethod methods[] = {
		{"stringFromJNI", "()Ljava/lang/String;", (void*)native_stringFromJNI},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jclass clazz;
	//��ȡJNI��������
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6) != JNI_OK) {
		return JNI_ERR;
	}

	//ע�᱾�ط���.Load Ŀ����
	clazz = (*env)->FindClass(env,classPathName);
	if (clazz == NULL) {
		return JNI_ERR;
	}
	//ע�᱾��native����
	if((*env)->RegisterNatives(env, clazz, methods, NELEM(methods)) < 0) {
		return JNI_ERR;
	}

	return JNI_VERSION_1_6;
}

#else

//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
//{
//	JNIEnv* env = NULL;
//
//	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6) != JNI_OK) {
//		return JNI_ERR;
//	}
//	assert(env != NULL);
//
//	g_jvm = vm;
//
//	// ʹ��(*env)->FindClass�õ�jclass
//	// ʹ��(*env)->RegisterNativesע�᱾�ط���
//	// ����ע������ʹ�õ�ע�᱾�ط���Ϊ��̬ע��JNI���������2.��̬ע�ᣩ
//	return JNI_VERSION_1_6;
//}

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #define ABI "armeabi-v7a/NEON"
    #else
      #define ABI "armeabi-v7a"
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__mips__)
   #define ABI "mips"
#else
   #define ABI "unknown"
#endif

    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
}

#endif
