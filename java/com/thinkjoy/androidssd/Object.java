package com.thinkjoy.androidssd;

/**
 * Created by thinkjoy on 12/7/17.
 */

public class Object {
    public static String CLASS_NAME[] = {"background",
            "person", "bicycle", "bird", "boat",
            "bottle", "bus", "car", "cat", "chair",
            "cow", "diningtable", "dog", "horse",
            "motorbike", "person", "pottedplant",
            "sheep", "sofa", "train", "tvmonitor"};
    int left;
    int top;
    int right;
    int bottom;
    int class_id;
    float prob;
    public Object(int left, int top, int right, int bottom, int class_id, float prob) {
        this.left = left;
        this.right = right;
        this.bottom = bottom;
        this.top = top;
        this.class_id = class_id;
        this.prob = prob;
    }
}
