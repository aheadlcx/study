package me.aheadlcx.study.ndk;

import android.content.Context;

/**
 * Description:
 * author: aheadlcx
 * Date:2018/5/24 上午10:47
 */
public class People {
    static {
        System.loadLibrary("people");
    }
    public static void init(){

    }
    public static native String sayPeople();
    public static native String getName();
    public static native boolean checkPackageName(String packageName);
    public static native boolean checkSign(Context context);
    public static native void exit(int status);
    public static native void exitReturn(int status);
    public static native void showHello(String status, int index);
    public static native void checkIsDebug();
    public static native void checkIsDebugOnce();

}
