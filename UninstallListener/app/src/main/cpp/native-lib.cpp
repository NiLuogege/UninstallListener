#include <jni.h>
#include <string>
#include <unistd.h>
#include <android/log.h>
#include <sys/inotify.h>
#include <stdlib.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_ERROR, "mytest",__VA_ARGS__)
extern "C"
void
Java_com_example_well_uninstalllistener_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    LOGD("进来了1");
    //新建进程
    pid_t pid = fork();
    if (pid < 0) {
        //新建进程出错
        LOGD("新建进程出错");
    } else if (pid == 0) {
        //新建进程成功
        int fileDescriptor = inotify_init();//初始化文件监听器

        if (fileDescriptor < 0) {
            //初始化失败
            LOGD("初始化失败,释放资源");
            exit(0);
        }
        int watchDescriptor = inotify_add_watch(fileDescriptor,
                                                "/data/data/com.example.well.uninstalllistener",
                                                IN_DELETE);
        if (watchDescriptor < 0) {
            LOGD("监听失败");
        }
        void *p_buf = malloc(sizeof(struct inotify_event));//开辟一个事件的大小
        size_t readBytes = read(fileDescriptor, p_buf,
                                sizeof(struct inotify_event));//一直去读这个监听的文件,这个函数是阻塞的
        free(p_buf);//释放开辟的内存

        //网页跳转的代码
        execlp("am", "am", "start", "--user", "0", "-a", "android.intent.action.VIEW", "-d",
               "http://www.dongnaoedu.com", (char *) NULL);

        //执行完毕后退出进程
        exit(0);

    } else if (pid > 0) {
        //父进程
        LOGD("到了父进程");
    }


}
