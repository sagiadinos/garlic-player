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
import android.util.Log;

import com.sagiadinos.garlic.player.java.SICPDef.SICPCommand;
import com.sagiadinos.garlic.player.java.SocketClient.SocketResponse;

public class PhilipsLauncher implements LauncherInterface
{
    Context MyContext;
    private static final String TAG = "PhilipsLauncher";
    private SocketClient mSocketClient;
    private String content_url      = "https://indexes.smil-control.com";
    private String uuid             = "";
    private String serial_number    = "";
    private String launcher_version = "";
    private String model_number     = "";

    private SocketResponse mSocketResponse = new SocketResponse()
    {

        @Override
        public void sockResponse(byte[] ret)
        {
         //   SICPDef.toHexString(ret);
            if (!checkSICPCallback(ret))
                return;

                if (ret[3] == 21) // HEX = 15 for serial
                {
                    serial_number = SICPDef.convertBytesToString(ret);
                    Log.d(TAG, "Serial Number:" + serial_number);
                    uuid = model_number + "-" + serial_number;
                }
                else if (ret[3] == -95) // HEX = A1 for serial)
                {
                    if (ret[0] == 12) // Firmware has datasize 12 (7 for Firmware +5 )
                    {
                        launcher_version = SICPDef.convertBytesToString(ret);
                        Log.d(TAG, "Launcher version:" + launcher_version);
                    }
                    else if (ret[0] == 15) // Model number has datasize 15 (10 for model number +5 )
                    {
                        model_number = SICPDef.convertBytesToString(ret);
                        Log.d(TAG, "Model number:" + model_number);
                        uuid = model_number + "-" + serial_number;
                    }
                    else
                        Log.d(TAG, "Unknown info:" + SICPDef.convertBytesToString(ret));

                }
                else
                    Log.d(TAG, "Unknown value:" + SICPDef.convertBytesToString(ret));
        }
    };

    public PhilipsLauncher(Context c)
    {
        MyContext = c;
        mSocketClient = new SocketClient(mSocketResponse);
        mSocketClient.start();
        try
        {
            Thread.sleep(10000);
            retrieveUUID();
            retrieveLauncherVersion();
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }

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
        byte[] sicpCmd = SICPDef.getSICPCommand(SICPCommand.SICP_COMMAND_SET_BACKLIGHT_DISABLE);
        sendSICPCommand(sicpCmd);
    }

    public void setScreenOn()
    {
        byte[] sicpCmd = SICPDef.getSICPCommand(SICPCommand.SICP_COMMAND_SET_BACKLIGHT_ENABLE);
        sendSICPCommand(sicpCmd);
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
        byte[] sicpCmd = SICPDef.getSICPCommand(SICPCommand.SICP_COMMAND_GET_SERIAL_CODE);
        sendSICPCommand(sicpCmd);

        sicpCmd = SICPDef.getSICPCommand(SICPCommand.SICP_COMMAND_GET_MODEL_NUMBER);
        sendSICPCommand(sicpCmd);
    }

    private void retrieveLauncherVersion()
    {
        byte[] sicpCmd = SICPDef.getSICPCommand(SICPCommand.SICP_COMMAND_GET_FIRMWARE_BUILD);
        sendSICPCommand(sicpCmd);
    }

    private void sendSICPCommand(byte[] sicpData)
    {
        mSocketClient.sendSICPCommand(sicpData);
    }


    private boolean checkSICPCallback(byte[] ret)
    {
        // 1. check checkSum
        final byte originalCheckSum = ret[ret.length - 1]; // The last byte is checksum.
        final byte checkSum = SICPDef.getDataCheckSum(ret);
        if (originalCheckSum != checkSum)
        {
            Log.w(TAG, "checkSICPCallback, checkSum Error...");
            return false;
        }
        return true;
    }

}
