/*
 garlic-player: SMIL Player for Digital Signage
 Copyright (C) 2021 Nikolaos Saghiadinos <ns@smil-control.com>
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
import android.content.Intent;
import android.content.IntentFilter;

public class PhilipsLauncher implements LauncherInterface
{
    Context MyContext;
    private String content_url      = "http://indexes.smil-control.com";
    private String uuid             = "";
    private String launcher_version = "philips";

    public PhilipsLauncher(Context c)
    {
        MyContext = c;
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

    public String getLauncherName()
    {
        return "philips";
    }

    public void setScreenOff()
    {
    }

    public void setScreenOn()
    {
    }

    public void rebootOS(String task_id)
    {
        Intent intent = new Intent();
        intent.setAction("php.intent.action.REBOOT");
//        intent.addFlags(Intent.FLAG_RECEIVER_INCLUDE_BACKGROUND);
        intent.addFlags(0x1000000);
        MyContext.sendBroadcast(intent);
    }

    public void installSoftware(String apk_path)
    {
        Intent intent = new Intent();
        intent.setAction("php.intent.action.UPDATE_APK");
        intent.putExtra("filePath", apk_path);
        intent.putExtra("keep", false);
        intent.putExtra("packageName", "com.sagiadinos.garlic.player");
        intent.putExtra("activityName", "com.sagiadinos.garlic.player.java.GarlicActivity");
        intent.putExtra("isAllowDowngrade", true); // allow downgrade
//        intent.addFlags(Intent.FLAG_RECEIVER_INCLUDE_BACKGROUND);
        intent.addFlags(0x1000000);
        MyContext.sendBroadcast(intent);
    }

    private void retrieveSmilIndex()
    {
    }

    private void retrieveUUID()
    {
    }

    private void retrieveLauncherVersion()
    {
    }


}
