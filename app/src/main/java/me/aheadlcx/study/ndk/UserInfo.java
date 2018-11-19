package me.aheadlcx.study.ndk;

/**
 * Description:
 * author: aheadlcx
 * Date:2018/5/24 上午9:16
 */
public class UserInfo {
    static {
        System.load("JniTest");
    }

    public static native String sayHello();
}
