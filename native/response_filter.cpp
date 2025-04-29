#include "response_filter.h"

#include "jni_util.h"
#include "util.h"


ResponseFilter::ResponseFilter(JNIEnv* env, jobject handler): handle_(env, handler) {}

bool ResponseFilter::InitFilter() {
  ScopedJNIEnv env;
  if (!env)
    return false;

  jboolean result = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "initFilter", "()Z", Boolean, result);

  return (result != JNI_FALSE);
}

CefResponseFilter::FilterStatus ResponseFilter::Filter(
    void* data_in,
    size_t data_in_size,
    size_t& data_in_read,
    void* data_out,
    size_t data_out_size,
    size_t& data_out_written) {
  ScopedJNIEnv env;
  if (!env) {
    return RESPONSE_FILTER_ERROR;
  }

  // 创建 ByteBuffer，只在有数据时才创建
  jobject jdata_in = (data_in && data_in_size > 0)
                         ? env->NewDirectByteBuffer(data_in, data_in_size)
                         : nullptr;
  jobject jdata_out = (data_out && data_out_size > 0)
                          ? env->NewDirectByteBuffer(data_out, data_out_size)
                          : nullptr;

  // 创建用于接收输出参数的数组
  jlongArray jdata_in_read_array = env->NewLongArray(1);
  jlong temp_in_read = static_cast<jlong>(data_in_read);
  env->SetLongArrayRegion(jdata_in_read_array, 0, 1, &temp_in_read);

  jlongArray jdata_out_written_array = env->NewLongArray(1);
  jlong temp_out_written = static_cast<jlong>(data_out_written);
  env->SetLongArrayRegion(jdata_out_written_array, 0, 1, &temp_out_written);

  // 调用Java方法
  jint result = RESPONSE_FILTER_ERROR;
  JNI_CALL_METHOD(env, handle_, "filter",
                  "(Ljava/nio/ByteBuffer;J[JLjava/nio/ByteBuffer;J[J)I", Int,
                  result, jdata_in, static_cast<jlong>(data_in_size),
                  jdata_in_read_array, jdata_out,
                  static_cast<jlong>(data_out_size), jdata_out_written_array);

  // 获取Java方法更新的输出参数
  jlong* in_read_elements =
      env->GetLongArrayElements(jdata_in_read_array, nullptr);
  if (in_read_elements) {
    data_in_read = static_cast<size_t>(in_read_elements[0]);
    env->ReleaseLongArrayElements(jdata_in_read_array, in_read_elements,
                                  JNI_ABORT);
  }

  jlong* out_written_elements =
      env->GetLongArrayElements(jdata_out_written_array, nullptr);
  if (out_written_elements) {
    data_out_written = static_cast<size_t>(out_written_elements[0]);
    env->ReleaseLongArrayElements(jdata_out_written_array, out_written_elements,
                                  JNI_ABORT);
  }

  // 清理JNI引用
  if (jdata_in)
    env->DeleteLocalRef(jdata_in);
  if (jdata_out)
    env->DeleteLocalRef(jdata_out);
  env->DeleteLocalRef(jdata_in_read_array);
  env->DeleteLocalRef(jdata_out_written_array);

  return static_cast<FilterStatus>(result);
}
