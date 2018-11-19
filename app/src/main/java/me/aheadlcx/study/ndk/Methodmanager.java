package me.aheadlcx.study.ndk;

/**
 * Description:
 * author: aheadlcx
 * Date:2018/11/19 上午10:43
 */
public class Methodmanager {

    static {
        System.loadLibrary("methodmanager");
    }

    public static native String getStringFromC();
    public static native String getMyName();
}
