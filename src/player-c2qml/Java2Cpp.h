#ifndef JAVA2CPP_H
#define JAVA2CPP_H

#include <jni.h>
#include "lib_facade.h"


#ifdef __cplusplus
extern "C" {
#endif

// Todo find a bedder solution
LibFacade *GlobalLibfacede;

void setGlobalLibFaceForJava(LibFacade *glf)
{
    GlobalLibfacede = glf;
}

JNIEXPORT void JNICALL Java_com_sagiadinos_garlic_player_java_ConfigReceiver_getConfigPath(
        JNIEnv *env /*env*/,
        jobject /*this_obj*/,
        jstring path)
{
    const char *str = env->GetStringUTFChars(path, NULL);
    GlobalLibfacede->setConfigFromExternal(str);

}

JNIEXPORT void JNICALL Java_com_sagiadinos_garlic_player_java_SmilIndexReceiver_getSmilIndexPath(
        JNIEnv *env /*env*/,
        jobject /*this_obj*/,
        jstring path)
{
    const char *str = env->GetStringUTFChars(path, NULL);
    GlobalLibfacede->setIndexFromExternal(str);
}


#ifdef __cplusplus
}
#endif
#endif // JAVA2CPP_H
