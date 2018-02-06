package com.thinkjoy.androidssd;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {

    private NativeInterface mInterface;
    CameraPreview mCameraPreview;
    FaceOverlayView fv_draw_rect;
    //    static {
//        System.loadLibrary("gesture");
//    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏
        setContentView(R.layout.activity_main);
        mInterface = NativeInterface.getInstance(this);
        mInterface.init();

        mCameraPreview = (CameraPreview)findViewById(R.id.cv_camera_preview);
        fv_draw_rect = (FaceOverlayView)findViewById(R.id.fv_draw_rect);
        mCameraPreview.setFaceOverlayView(fv_draw_rect);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if(getRequestedOrientation()!= ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE){
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
    }
}
