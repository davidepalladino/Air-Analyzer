package it.davidepalladino.airanalyzer.view.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.viewpager.widget.ViewPager;

import com.jjoe64.graphview.GraphView;

public class ViewPagerRoom extends ViewPager {
    public ViewPagerRoom(@NonNull Context context) {
        super(context);
    }

    public ViewPagerRoom(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected boolean canScroll(View v, boolean checkV, int dx, int x, int y) {
        if (v instanceof GraphView) {
            return true;
        }

        return super.canScroll(v, checkV, dx, x, y);
    }
}
