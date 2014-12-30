package com.wearabled;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ToggleButton;
import android.widget.CompoundButton;
import android.util.Log;
import com.wearabled.BeatDetector;



/**
 * Created by tapion on 27.12.2014.
 */
public class AdvancedEffectsFragment  extends Fragment {

    private BeatDetector mDetector = null;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.fragment_advanced, container, false);

        ToggleButton beatOn = (ToggleButton) rootView.findViewById(R.id.beatDetectorButton);
        beatOn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    enableBeatDetection();
                } else {
                    disableBeatDetection();
                }
            }
        });

        return rootView;
    }

    private void enableBeatDetection() {
        Log.d ("Advanced Effects", "Trying to enable beat detection...");
        mDetector = new BeatDetector();
        mDetector.execute();
        //mDetect or.startDetection();

    }

    private void disableBeatDetection() {
        Log.d ("Advanced Effects", "Disabling beat detection...");
        if (mDetector != null) {
            mDetector.cancel(true);
        }
        mDetector = null;
    }
}
