package org.qbitscoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class QbitscoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File qbitscoinDir = new File(getFilesDir().getAbsolutePath() + "/.qbitscoin");
        if (!qbitscoinDir.exists()) {
            qbitscoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
