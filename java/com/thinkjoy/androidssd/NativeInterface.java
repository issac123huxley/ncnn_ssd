package com.thinkjoy.androidssd;

import android.content.Context;
import android.graphics.Bitmap;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

/**
 * Created by thinkjoy on 12/7/17.
 */

public class NativeInterface {
    private Context mContext;
    public native void init(String path1, String path2);
    public native void detect(byte[] imageArray, int rotation, int imageFormat, int imageWidth, int imageHeight, List<Object> object_list);
    static {
        System.loadLibrary("ssdmobilenet");
    }
    private static NativeInterface mInterface;
    public static NativeInterface getInstance(Context context) {
        if (mInterface == null) {
            mInterface = new NativeInterface();
            mInterface.mContext = context;
        }
        return mInterface;
    }

    public void init() {
//        String path1 = getPath("mobilenet_ssd_voc_ncnn.bin");
//        String path2 = getPath("mobilenet_ssd_voc_ncnn.param");
        String path1 = getPath("ncnn.bin");
        String path2 = getPath("ncnn.proto");
        init(path1, path2);
    }
    private String getPath(String filename) {

        try {
            InputStream is = mContext.getAssets().open(filename);
            File cascadeDir = mContext.getDir("weights", Context.MODE_PRIVATE);
            File mCascadeFile = new File(cascadeDir, filename);
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
            return mCascadeFile.getPath();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }

    }
}
