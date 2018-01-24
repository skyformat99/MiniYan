package com.mini.app.core;

import com.mini.app.http.HttpResponseData;
import com.mini.app.http.HttpUtil;
import com.mini.app.utils.MiniLog;
import com.mini.app.utils.MiniUtils;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;

/*
 * MiniBitmap
 * @Author Yan bo
 * @Time 2014-5-4
 * @Copyright Reserved
 * @Descrption Android 2D Bitmap Interface Implements
 */
public class MiniBitmap {

    private static final String TAG = "MiniBitmap";
    private Bitmap mBitmap = null;

    public MiniBitmap() {
    }

    public void loadImage(String url) {
        HttpResponseData data = HttpUtil.getFileStream(url);
        if (data == null) {
            throw new RuntimeException("stream is null");
        } else {
            try {
                MiniLog.d(TAG, "stream is not null");
                byte[] bitmapData = MiniUtils.readStream(data.getStream());
                if (bitmapData != null) {
                    mBitmap = BitmapFactory.decodeByteArray(bitmapData, 0,
                            bitmapData.length);
                }
                //mBitmap = BitmapFactory.decodeStream(is);
                data.close();
            } catch (Exception e) {
                e.printStackTrace();
            }

        }
    }

    // 得到图片字节流 数组大小
    public int getWidth() {
        if (mBitmap != null) {
            return mBitmap.getWidth();
        }

        return 0;
    }

    public int getHeight() {
        if (mBitmap != null) {
            return mBitmap.getHeight();
        }

        return 0;
    }

    public void recycle() {
        if (mBitmap != null && !mBitmap.isRecycled()) {
            mBitmap.recycle();
            mBitmap = null;
        }

        //System.gc();
    }

    public Bitmap getBitmap() {
        return mBitmap;
    }

    // 绘制文字部分
    public void drawText(String text,
                         int width, int height,
                         int align, int textSize,
                         int textColor,
                         int textStyle,
                         int bgColor) {

        mBitmap = Bitmap.createBitmap(
                width,
                height,
                Config.ARGB_8888);

        Canvas canvas = new Canvas(mBitmap);
        canvas.drawColor(Color.argb(0, 0, 0, 0));

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        switch (textStyle) {
            case 0:
                break;
            case MiniGraphicsContext.PEN_STYLE_ITALIC:
                paint.setTextSkewX(-0.5f);
                break;
            case MiniGraphicsContext.PEN_STYLE_BOLD:
                paint.setFakeBoldText(true);
                break;
            case MiniGraphicsContext.PEN_STYLE_UNDERLINE:
                paint.setUnderlineText(true);
                break;
        }

        paint.setTextSize(textSize);
        paint.setColor(Color.rgb(
                Color.red(textColor),
                Color.green(textColor),
                Color.blue(textColor)));

        MiniLog.d(TAG, "gl drawText=" + text);
        MiniUtils.drawText(text,
                new Rect(0, 0, width, height),
                align,
                canvas,
                paint);
    }
}
