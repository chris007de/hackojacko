package com.wearabled;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
import org.jtransforms.fft.DoubleFFT_1D;
import android.os.AsyncTask;

/**
 * Created by tapion on 27.12.2014.
 */
public class BeatDetector extends AsyncTask <Void, Integer, Void> {
    private AudioRecord mAudioRec = null;
    private static final int RECORDER_SAMPLERATE = 44100;
    private static final int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO;
    private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;
    private byte[] mSamples = null;
    private final int BUFFER_SIZE = 2048;

    BeatDetector() {

        mSamples = new byte[1024];
        mAudioRec = new AudioRecord(MediaRecorder.AudioSource.MIC,
                RECORDER_SAMPLERATE, RECORDER_CHANNELS,
                RECORDER_AUDIO_ENCODING, BUFFER_SIZE);
    }

    public Void doInBackground(Void... params) {
        Log.d("BeatDetector", "Starting Beat detection...");
        double[] fftBuffer = new double [BUFFER_SIZE * 2];
        double[] spectrum = new double[BUFFER_SIZE / 2];

        mAudioRec.startRecording();
        while (true) {
            Log.d("BeatDetector", "Starting iteration");
            mAudioRec.read(mSamples, 0, BUFFER_SIZE);
            Log.d("BeatDetector", "Read Samples...");
            DoubleFFT_1D fft1d = new DoubleFFT_1D(BUFFER_SIZE);
            fft1d.realForward(fftBuffer);
            Log.d("BeatDetector", "finished FFT");

            int j = 0;
            for (int i = 2; i < BUFFER_SIZE - 1; i += 2) {
                spectrum[j] = Math.sqrt( Math.pow(fftBuffer[i], 2) + Math.pow(fftBuffer[ i + 1], 2) );
                float freq = (j * RECORDER_SAMPLERATE ) / BUFFER_SIZE;
                //Log.d("FFT Results", "Freq: " + String.valueOf(freq) + " Energy: " + spectrum[j] );
                j++;
            }
        }

    }

    public void stopDetection() {
        Log.d("BeatDetector", "Stopping Beat detection...");
        mAudioRec.stop();
        mAudioRec.release();
    }
}
