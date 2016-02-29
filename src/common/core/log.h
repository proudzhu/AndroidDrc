#ifndef LE_FX_ENGINE_DSP_CORE_LOG_H_
#define LE_FX_ENGINE_DSP_CORE_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdio>

#define LOGV ALOGV
#define LOGD ALOGD
#define LOGE ALOGE
#define LOGW ALOGW
#define LOGI ALOGI

#define ALOGV printf
#define ALOGD printf
#define ALOGE printf
#define ALOGW printf
#define ALOGI printf

#define LoggerError printf

#ifdef __cplusplus
}
#endif

#endif
