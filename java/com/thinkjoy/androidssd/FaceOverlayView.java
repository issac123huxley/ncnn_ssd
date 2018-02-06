package com.thinkjoy.androidssd;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import java.util.List;


/**
 * Created by thinkjoy on 17-8-9.
 */

public class FaceOverlayView extends View {

    private Paint mPaint;
    private Paint mFacePaint;
    private Paint mTextPaint;
    private int imageWidth;
    private int imageHeight;
    private int winWidth;
    private int winHeight;
    private float left, right, bottom, top;
    private long processTime;

    private List<Object> objectList;

    public FaceOverlayView(Context context) {
        super(context);
    }
    public FaceOverlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public FaceOverlayView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }
    void init() {
        mPaint = new Paint();
        mPaint.setAntiAlias(true);  //
        mPaint.setDither(true);
        mPaint.setColor(Color.GREEN);   //颜色
        mPaint.setStrokeWidth(5);
        mPaint.setStyle(Paint.Style.STROKE);  //控制是否空心

        mTextPaint = new Paint();
        mTextPaint.setAntiAlias(true);
        mTextPaint.setDither(true);
        mTextPaint.setTextSize(60);
        mTextPaint.setColor(Color.GREEN);
        mTextPaint.setStyle(Paint.Style.FILL);

        mFacePaint = new Paint();
        mFacePaint.setAntiAlias(true);
        mFacePaint.setDither(true);
        mFacePaint.setColor(Color.WHITE);
        mFacePaint.setStrokeWidth(5);
        mFacePaint.setStyle(Paint.Style.STROKE);


    }
    @Override
    protected void onDraw(Canvas canvas) {
        winHeight = getHeight();
        winWidth = getWidth();

            float ratex = (float)winWidth / imageWidth;
            float ratey = (float)winHeight / imageHeight;
            if (left > 0.5) {

//                canvas.drawRect(left * ratex,  top * ratey, right * ratex,  bottom * ratey, mPaint);
                canvas.drawText("TIME:"+Long.toString(processTime) + "ms", 20, 160
                        , mTextPaint);
            } else {
                canvas.drawText("TIME:"+Long.toString(processTime) + "ms", 20, 160, mTextPaint);
                canvas.drawRect(0, 0, 0, 0, mPaint);
            }
            if (objectList != null) {
                for (int i = 0; i < objectList.size(); ++i) {
                    if (objectList.get(i).prob > 0.5) {
                        int left = objectList.get(i).left;
                        int top = objectList.get(i).top;
                        int right = objectList.get(i).right;
                        int bottom = objectList.get(i).bottom;
                        canvas.drawRect(left * ratex, top * ratey, right * ratex, bottom * ratey, mPaint);
                        String label = Object.CLASS_NAME[objectList.get(i).class_id] + objectList.get(i).prob;
                        canvas.drawText(label, left * ratex , top * ratey + 50, mTextPaint);
                    }
                }
            }

    }


    public void setImageSize(int imageWidth, int imageHeight) {
        this.imageWidth = imageWidth;
        this.imageHeight = imageHeight;
    }

    public void setObjectList(List<Object> objectList, int time) {
        this.objectList = objectList;
        this.processTime = time;
    }

}
