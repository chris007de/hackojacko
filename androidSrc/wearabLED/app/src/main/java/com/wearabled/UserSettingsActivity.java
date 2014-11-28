package com.wearabled;

import android.os.Bundle;
import android.preference.PreferenceActivity;

/**
 * Created by Patrick Rehm on 28.11.2014.
 */
public class UserSettingsActivity extends PreferenceActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        addPreferencesFromResource(R.xml.preferences);

    }
}
