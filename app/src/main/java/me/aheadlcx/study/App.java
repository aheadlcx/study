package me.aheadlcx.study;

import android.app.Application;
import android.content.Context;

/**
 * Description:
 * author: aheadlcx
 * Date:2018/5/24 下午6:18
 */
public class App extends Application {
    public static Context mContext;
    @Override
    public void onCreate() {
        super.onCreate();
        mContext = this.getApplicationContext();
    }
}
