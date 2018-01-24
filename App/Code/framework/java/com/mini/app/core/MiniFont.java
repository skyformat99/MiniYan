package com.mini.app.core;

import com.mini.app.utils.MiniLog;
import com.mini.app.utils.MiniUtils;

import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Paint.FontMetrics;
import android.text.TextPaint;

import java.util.ArrayList;
import java.util.List;

public class MiniFont {
    private Paint mFontPaint = null;
    private List<LineText> mList = null;
    
    public MiniFont() {
    	mFontPaint = new Paint();
    }
    
    public int getFontWidth(String ch, int size) {
        Rect bounds = new Rect();
        mFontPaint.setTextSize(size);
    	//mFontPaint.getTextBounds(ch, 0, ch.length(), bounds);
        //float width = mFontPaint.measureText(ch);
        //MiniLog.d("yanbo", "Text ch="+ch+" width="+width);
    	//return bounds.width();
        return (int) mFontPaint.measureText(ch);
    }
    
    public int getFontHeight(int size) {
    	MiniLog.d("MiniFont", "getFontHeight");
        mFontPaint.setTextSize(size);
		FontMetrics fontMetrics = mFontPaint.getFontMetrics();
        return (int)(fontMetrics.bottom - fontMetrics.top);
    }
    
    public int getTextWidth(String text, int size) {
    	return getFontWidth(text, size);
    }

    private int getFontWidth(char[] chars, int index, int fontSize) {
        mFontPaint.setTextSize(fontSize);
        Rect bounds = new Rect();
        //return (int) mFontPaint.measureText(chars, index, 1);
        //mFontPaint.getTextBounds(chars, index, 1, bounds);
        float[] width = new float[1];
        mFontPaint.getTextWidths(chars, index, 1, width);
        //return bounds.width();
        return (int) Math.ceil(width[0]);
    }

    public int getLineSize() {
        if (mList != null) {
            return mList.size();
        }

        return 0;
    }

    public String getLineText(int index) {
        return mList.get(index).mText;
    }

    public int getLineWidth(int index) {
        return mList.get(index).mWidth;
    }

    public int calcTextLine(String text, int maxWidth, int fontSize) {
        if (mList == null) {
            mList = new ArrayList<LineText>();
        } else {
            mList.clear();
        }

        char[] charArray = text.toCharArray();
        int length = text.length();
        int currentLineWidth = 0;
        //String lineText = null;
        //int startPos = 0;
        int maxLineWidth = 0;
        StringBuilder builder = new StringBuilder();
        MiniLog.d("libmini", "java text="+text);
        for (int index = 0; index < length; index++) {
            //lineText = text.substring(startPos, index + 1);
            //int width = getTextWidth(lineText, fontSize);
            int width = getFontWidth(charArray, index, fontSize);
            MiniLog.d("libmini", "text="+charArray[index] +" and width="+width);
            if (currentLineWidth + width <= maxWidth) {
                builder.append(charArray[index]);
                currentLineWidth += width;
            } else {
                if (maxLineWidth < currentLineWidth) {
                    maxLineWidth = currentLineWidth;
                }
                mList.add(new LineText(builder.toString(), currentLineWidth));
                currentLineWidth = 0;
                builder.delete(0, builder.length());
            }
        }

        if (currentLineWidth > 0) {
            if (maxLineWidth < currentLineWidth) {
                maxLineWidth = currentLineWidth;
            }

            mList.add(new LineText(builder.toString(), currentLineWidth));
            builder.delete(0, builder.length());
        }

        return maxLineWidth;
    }

    private static class LineText {
        public String mText;
        public int mWidth;

        public LineText(String text, int width) {
            mText = text;
            mWidth = width;
        }
    }
}
