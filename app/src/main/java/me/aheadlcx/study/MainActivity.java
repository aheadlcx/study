package me.aheadlcx.study;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.security.NoSuchAlgorithmException;

import me.aheadlcx.study.ndk.ContextImpl;
import me.aheadlcx.study.ndk.Methodmanager;
import me.aheadlcx.study.ndk.People;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        TextView txtShow = (TextView) findViewById(R.id.txtShow);
        txtShow.setText(People.sayPeople());
        txtShow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String name = People.getName();
                Log.i(TAG, "onClick: name = " + name);
            }
        });
        findViewById(R.id.txtPack).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean checkPackageName = People.checkPackageName("me.aheadlcx.study");
                Log.i(TAG, "onClick: checkPackageName" + checkPackageName);
            }
        });

        findViewById(R.id.txtPackError).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean checkPackageName = People.checkPackageName("me.aheadlcx.study2");
                Log.i(TAG, "onClick: checkPackageName error" + checkPackageName);
            }
        });
        findViewById(R.id.txtSign).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                ContextImpl.getSign(MainActivity.this);
//                ContextImpl.getInstance().init(MainActivity.this);
                People.checkSign(MainActivity.this);
            }
        });
        ContextImpl.getInstance().init(this);
        findViewById(R.id.txtMD5).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    testMD5();
                } catch (NoSuchAlgorithmException e) {
                    e.printStackTrace();
                }
            }
        });
        findViewById(R.id.txtExit).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                People.exit(1);
                People.exitReturn(1);
            }
        });

        findViewById(R.id.txtShowHello).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                People.showHello(" world", 5);
            }
        });
        findViewById(R.id.txtCheckIsDebug).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                People.checkIsDebug();
            }
        });
        findViewById(R.id.txtCheckDebugOnce).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                People.checkIsDebugOnce();
            }
        });

        findViewById(R.id.txtNativeReg).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String stringFromC = Methodmanager.getStringFromC();
                Log.i(TAG, "onClick: stringFromC = " + stringFromC);
            }
        });

        findViewById(R.id.txtMyName).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String myName = Methodmanager.getMyName();
                Log.i(TAG, "onClick: myName = " + myName);
            }
        });
    }

    private void testMD5() throws NoSuchAlgorithmException {

    }

    private <T> T a(Context context){
        return (T) "s";
    }

    private String getSign(Context context){
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager
                    .GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            String sign = new String(signatures[0].toByteArray());
            Log.i(TAG, "getSign: " + sign);
            return sign;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
