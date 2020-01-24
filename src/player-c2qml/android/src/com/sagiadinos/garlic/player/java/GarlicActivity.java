package com.sagiadinos.garlic.player.java;


import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

public class GarlicActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static  GarlicActivity m_instance;
    private static boolean is_foreground        = false;

    public GarlicActivity()
    {
        m_instance = this;
    }

    public void registerBroadcastReceiver()
    {
        IntentFilter filter = new IntentFilter("com.sagiadinos.garlic.player.java.ConfigReceiver");
        ConfigReceiver MyConfigReceiver = new ConfigReceiver();
        registerReceiver(MyConfigReceiver, filter);

        IntentFilter filter2 = new IntentFilter("com.sagiadinos.garlic.player.java.SmilIndexReceiver");
        SmilIndexReceiver MySmilIndexReceiver = new SmilIndexReceiver();
        registerReceiver(MySmilIndexReceiver, filter2);
    }

    public static void rebootOS()
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.RebootReceiver");
        m_instance.sendBroadcast(intent);

    }
    public static void installSoftware(String apk_path)
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.InstallAppReceiver");
        intent.putExtra("apk_path", apk_path);
        m_instance.sendBroadcast(intent);
    }

    public static void closePlayerCorrect()
    {
         Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.PlayerClosedReceiver");
         intent.addFlags(Intent.FLAG_INCLUDE_STOPPED_PACKAGES);
         m_instance.sendBroadcast(intent);
    }

    public static void startSecondApp(String package_name)
    {
         Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.SecondAppReceiver");
         intent.putExtra("package_name", package_name);
         m_instance.sendBroadcast(intent);
    }

}
 
