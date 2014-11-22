package com.wearabled;
import android.os.Handler;
import com.wearabled.ConnectionThread;

public class HandleHelper {
    static ConnectionThread mConnectionThread;

    public static void setConnectionThread(ConnectionThread handler)
    {
        mConnectionThread = handler;
    }

    public static ConnectionThread getConnectionThread()
    {
        return mConnectionThread;
    }
}


