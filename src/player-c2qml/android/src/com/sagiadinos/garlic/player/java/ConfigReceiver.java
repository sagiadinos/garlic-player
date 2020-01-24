package com.sagiadinos.garlic.player.java;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;

public class ConfigReceiver extends BroadcastReceiver
{
    String path;

    @Override
    public void onReceive(Context context, Intent intent)
    {
        if (intent == null || intent.getAction() == null)
        {
            return;
        }
        if (!intent.getAction().equals("com.sagiadinos.garlic.player.java.ConfigReceiver")
            || !context.getPackageName().equals("com.sagiadinos.garlic.player"))
        {
            return;
        }

       path = intent.getStringExtra("config_path");
       System.out.println(path + " is cumming");

       getConfigPath(path);
    }

    public static native void getConfigPath(String path);
}
