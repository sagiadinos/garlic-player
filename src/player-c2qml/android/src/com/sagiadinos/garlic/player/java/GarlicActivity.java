/*
 garlic-player: SMIL Player for Digital Signage
 Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
 This file is part of the garlic-player source code

 This program is free software: you can redistribute it and/or  modify
 it under the terms of the GNU Affero General Public License, version 3,
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

package com.sagiadinos.garlic.player.java;

import android.content.Context;
import android.os.Environment;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.PowerManager;
import java.util.concurrent.ExecutionException;

public class GarlicActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static  GarlicActivity m_instance;
    private AsyncTask<Void, Void, String> runningTaskSmilIndex;
    private AsyncTask<Void, Void, String> runningTaskUUID;
    private AsyncTask<Void, Void, String> runningTaskLauncherVersion;
    private String content_url      = null;
    private String uuid             = null;
    private String launcher_version = null;

    public GarlicActivity()
    {
        m_instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        if (isLauncherInstalled())
        {
            retrieveSmilIndex();
            retrieveUUID();
            retrieveLauncherVersion();
        }
  //      PowerManager mgr = (PowerManager) getSystemService(Context.POWER_SERVICE);
  //      PowerManager.WakeLock wakeLock = mgr.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "MA_PLAYER:GARLIC_PLAYER");
  //      wakeLock.acquire();
    }

    public void registerBroadcastReceiver()
    {

        IntentFilter filter = new IntentFilter("com.sagiadinos.garlic.player.java.ConfigReceiver");
        ConfigReceiver MyConfigReceiver = new ConfigReceiver();
        registerReceiver(MyConfigReceiver, filter);

        // needed when USB Stick enters with local SMIL File
        IntentFilter filter2 = new IntentFilter("com.sagiadinos.garlic.player.java.SmilIndexReceiver");
        SmilIndexReceiver MySmilIndexReceiver = new SmilIndexReceiver();
        registerReceiver(MySmilIndexReceiver, filter2);
    }

    public boolean isLauncherInstalled()
    {
        PackageManager pm = getPackageManager();
        try
        {
            pm.getPackageInfo("com.sagiadinos.garlic.launcher", PackageManager.GET_META_DATA);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            return false;
        }
        return true;
    }

    public String getContentUrlFromLauncher()
    {
        return content_url;
    }

    public String getUUIDFromLauncher()
    {
        return uuid;
    }

    public String getLauncherVersion()
    {
        return launcher_version;
    }

    public static boolean isPackageInstalled(Context c, String targetPackage)
    {
        PackageManager pm = m_instance.getPackageManager();
        try
        {
            pm.getPackageInfo(targetPackage, PackageManager.GET_META_DATA);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            return false;
        }
        return true;
    }

    public static void setScreenOff()
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.CommandReceiver");
        intent.putExtra("command", "screen_off");
        m_instance.sendBroadcast(intent);
    }

    public static void setScreenOn()
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.CommandReceiver");
        intent.putExtra("command", "screen_on");
        m_instance.sendBroadcast(intent);
    }

    public static void rebootOS(String task_id)
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.RebootReceiver");
        intent.putExtra("task_id", task_id);
        m_instance.sendBroadcast(intent);

/*        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.CommandReceiver");
        intent.putExtra("command", "reboot");
        intent.putExtra("task_id", task_id);
        m_instance.sendBroadcast(intent);
*/    }

    public static void applyConfig(String config_path)
    {
        Intent intent = new Intent("com.sagiadinos.garlic.launcher.receiver.ConfigXMLReceiver");
        intent.putExtra("config_path", config_path);
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

    private void retrieveSmilIndex()
    {
        try
        {
            if (runningTaskSmilIndex != null)
                runningTaskSmilIndex.cancel(true);

            runningTaskSmilIndex = new LongOperation(this, "smil_content_url");
            content_url = runningTaskSmilIndex.execute().get();
        }
        catch (InterruptedException | ExecutionException e)
        {
            e.printStackTrace();
        }
    }

    private void retrieveUUID()
    {
        try
        {
            if (runningTaskUUID != null)
                runningTaskUUID.cancel(true);

            runningTaskUUID = new LongOperation(this, "uuid");
            uuid = runningTaskUUID.execute().get();
        }
        catch (InterruptedException | ExecutionException e)
        {
            e.printStackTrace();
        }
    }

    private void retrieveLauncherVersion()
    {
        try
        {
            if (runningTaskLauncherVersion != null)
                runningTaskLauncherVersion.cancel(true);
            runningTaskLauncherVersion = new LongOperation(this, "launcher_version");
            launcher_version = runningTaskLauncherVersion.execute().get();
        }
        catch (InterruptedException | ExecutionException e)
        {
            e.printStackTrace();
        }
    }

    private class LongOperation extends AsyncTask<Void, Void, String>
    {
        private String  appendable_path;
        private Context ctx;
        public LongOperation (Context context, String ap)
        {
            ctx = context;
            appendable_path = ap;
        }

        @Override
        protected String doInBackground(Void... params)
        {
            final String PROVIDER_NAME = "com.sagiadinos.garlic.launcher.SettingsProvider";
            ContentResolver contentResolver = ctx.getContentResolver();
            Uri BASE_URI = Uri.parse("content://" + PROVIDER_NAME);
            String value = "";

            Uri uri = BASE_URI.buildUpon().appendPath(appendable_path).build();

            Cursor cursor = contentResolver.query(uri, null, null, null, null);

            if (cursor.moveToFirst())
            {
                value = cursor.getString(0);
            }
            cursor.close();

            return value;
        }

  /*
       Maybe it is better to block the ui thread until the task are completed

        @Override
        protected void onPostExecute(String result)
        {
            switch (appendable_path)
            {
                case "smil_content_url":
                    content_url = result;
                    break;

                case "uuid":
                    uuid = result;
                    break;
                case "launcher_version":
                    launcher_version = result;
                    break;
            }
        }
*/
    }
}
