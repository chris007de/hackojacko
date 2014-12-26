package com.wearabled;
import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.preference.PreferenceManager;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.graphics.Color;
import android.os.PowerManager.WakeLock;

import com.larswerkman.holocolorpicker.ColorPicker;
import com.larswerkman.holocolorpicker.ValueBar;

/*
 * \TODO Add Toast for unconnected message sends
 */
public class LightingFragment extends Fragment implements ColorPicker.OnColorChangedListener, SensorEventListener {
    public LightingFragment() {

    }

    private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private ColorPicker mPicker = null;

    private long lastUpdate = 0;
    private float last_x, last_y, last_z;
    private static final int SHAKE_THRESHOLD = 600;

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor sensor = event.sensor;

        if (sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];

            long curTime = System.currentTimeMillis();

            if ((curTime - lastUpdate) > 100) {
                long diffTime = (curTime - lastUpdate);
                lastUpdate = curTime;

                float speed = Math.abs(x + y + z - last_x - last_y - last_z)/ diffTime * 10000;

                if (speed > SHAKE_THRESHOLD) {
                    int rRandom = (int) (Math.random() * 255);
                    int gRandom = (int) (Math.random() * 255);
                    int bRandom = (int) (Math.random() * 255);

                    Log.d ("RandColor: ", "R: " + rRandom + " G: " + gRandom + " B: " + bRandom);
                    if (null != mPicker) {
                        mPicker.setColor(Color.rgb(rRandom, gRandom, bRandom));
                    }

                }
                last_x = x;
                last_y = y;
                last_z = z;
            }
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.fragment_lighting, container, false);

        Button lightButton = (Button) rootView.findViewById(R.id.makeLights);
        lightButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset(HackoJackoProtocol.P_ALL_ON);
            }
        });

        Button darkButton = (Button) rootView.findViewById(R.id.lightOff);
        darkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset(HackoJackoProtocol.P_ALL_OFF);
            }
        });

       Button blinkButton = (Button) rootView.findViewById(R.id.blink);
        blinkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset(HackoJackoProtocol.P_BLINK);
            }
        });

        Button runButton = (Button) rootView.findViewById(R.id.run);
        runButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset(HackoJackoProtocol.P_RUN);
            }
        });

        Button randomButton = (Button) rootView.findViewById(R.id.random);
        randomButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset(HackoJackoProtocol.P_RANDOM);
            }
        });

        Switch danceSwitch = (Switch) rootView.findViewById((R.id.sensorSwitch));
        danceSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    activateDanceMode();
                } else {
                    disableDanceMode();
                }
            }
        });

        mPicker = (ColorPicker) rootView.findViewById(R.id.picker);
        ValueBar valueBar = (ValueBar) rootView.findViewById(R.id.valuebar);
        mPicker.addValueBar(valueBar);
        mPicker.getColor();
        mPicker.setOldCenterColor(mPicker.getColor());
        mPicker.setOnColorChangedListener(this);
        mPicker.setShowOldCenterColor(false);

        mSensorManager = (SensorManager)
                getActivity().getSystemService(getActivity().getApplicationContext().SENSOR_SERVICE);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);


        return  rootView;
    }

    private void activateDanceMode() {
        PowerManager pm = (PowerManager)
                getActivity().getSystemService(getActivity().getApplicationContext().POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "DanceDance");
        wl.acquire();
        mSensorManager.registerListener(this,mAccelerometer , SensorManager.SENSOR_DELAY_UI);
        mPicker.setClickable(false);
    }

    private void disableDanceMode() {
        PowerManager pm = (PowerManager)
                getActivity().getSystemService(getActivity().getApplicationContext().POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "DanceDance");
        wl.release();
        mSensorManager.unregisterListener(this);
        mPicker.setClickable(true);
    }

    @Override
    public void onColorChanged(int color) {

        //int noOfLeds = 1;
        //SharedPreferences SP = PreferenceManager.getDefaultSharedPreferences(getActivity());
        //String noLeds = SP.getString("noLEDs","0");
        //try {
        //    noOfLeds = Integer.parseInt(noLeds);
        //} catch (NumberFormatException e) {
        //}
        //Log.d("onColorChanged: ", noLeds);
        HackoJackoProtocol.sendColorCommand(color);
    }
}
