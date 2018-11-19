package me.aheadlcx.study.ndk;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Log;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateFactory;

import me.aheadlcx.study.App;

/**
 * Description:
 * author: aheadlcx
 * Date:2018/5/24 下午5:33
 */
public class ContextImpl {
    private static final String TAG = "ContextImpl";
    private Context mContext;
    private static ContextImpl sContextImpl;

    public static ContextImpl getInstance() {
        if (sContextImpl == null) {
            return new ContextImpl().init(App.mContext);
        }
        return sContextImpl;
    }

    private ContextImpl() {
    }

    public ContextImpl init(Context context) {
        mContext = context;
        return this;

    }

    public static String getSign() {
        return getSign(getInstance().mContext);
    }

    public static String getSign(Context context) {
        if (context == null) {
            Log.e(TAG, "getSign: context == null");
            return "error";
        }
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager
                    .GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            md5.reset();
            md5.update(signatures[0].toByteArray());
            byte[] digest = md5.digest();
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < digest.length; i++) {
                if (Integer.toHexString(0xFF & digest[i]).length() == 1) {
                    builder.append("0").append(Integer.toHexString(0xFF & digest[i]));
                } else {
                    builder.append(Integer.toHexString(0xFF & digest[i]));
                }
            }

            Log.i(TAG, "getSign: " + builder.toString());
            return builder.toString();
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            return "error";
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
            return "error";
        }
    }
}
