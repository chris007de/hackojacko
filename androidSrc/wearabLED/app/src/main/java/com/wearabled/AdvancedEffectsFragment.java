package com.wearabled;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.media.AudioRecord;

import org.jtransforms.fft.DoubleFFT_1D;

/**
 * Created by tapion on 27.12.2014.
 */
public class AdvancedEffectsFragment  extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.fragment_advanced, container, false);

        return rootView;
    }
}
