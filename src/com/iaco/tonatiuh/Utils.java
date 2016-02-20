/*
  Copyright (C) 2015 Othon Insauste
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
 
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  Othon Insauste othonic@gmail.com
*/
package com.iaco.tonatiuh;

import java.io.*;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import android.os.Environment;


/** Utils functions. */
public class Utils {


    /** open file */
    public static FileInputStream tryToGetLocalFile(Context ctx, String filename) {
        try {
            FileInputStream fin = ctx.openFileInput(filename);
            return fin;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }

    /** svg image on sdcard */
    public static void unpackOnSdCard(AssetManager assetManager) throws IOException {
        if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED)==0) {
            File sdcard = Environment.getExternalStorageDirectory();
            String irrlichtPath = sdcard.getAbsoluteFile() + "/Irrlicht/";
            File irrlichtDir = new File(irrlichtPath);
            if (irrlichtDir.exists() && !irrlichtDir.isDirectory()) {
                throw new IOException("Irrlicht exists and is not a directory on SD Card");
            } else if (!irrlichtDir.exists()) {
                irrlichtDir.mkdirs();
            }
            // Note: /sdcard/irrlicht dir exists
            String[] filenames = assetManager.list("data");
            for(String filename:filenames) {
                InputStream inputStream = assetManager.open("data/" + filename);
                OutputStream outputStream = new FileOutputStream(irrlichtPath + "/" + filename);
                // copy
                byte[] buffer = new byte[4096];
                int length;
                while ( (length = inputStream.read(buffer)) > 0 ) {
                    outputStream.write(buffer, 0, length);
                }
                outputStream.flush();
                outputStream.close();
                inputStream.close();
            }           
        } else {
            throw new IOException("SD Card not available");
        }

    }

    public static boolean isSdCardAvailable() {
        if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED)==0) {
            return true;
        } else {
            return false;
        }
    }


    /** compute md5 */
    public static String getMd5Hash(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] messageDigest = md.digest(input.getBytes());
            BigInteger number = new BigInteger(1, messageDigest);
            String md5 = number.toString(16);
            while (md5.length() < 32) {
                md5 = "0" + md5;
            }
            return md5;
        } catch (NoSuchAlgorithmException e) {
            Log.e("MD5", e.getMessage());
            return null;
        }
    }

    /** read binary input stream */
    private static byte[] readBinaryInput(InputStream in) throws IOException {
        ByteArrayOutputStream bo = new ByteArrayOutputStream();
        byte[] tmpStorage = new byte[4096];
        int pos = 0;
        for (int n; (n = in.read(tmpStorage)) != -1;) {
            bo.write(tmpStorage, 0, n);
            pos += n;
        }
        return bo.toByteArray();
    }

    /** read string input stream */
    private static String readStringInput(InputStream in) throws IOException {
        StringBuilder sb = new StringBuilder();

        byte[] buffer = new byte[4096];
        for (int n; (n = in.read(buffer)) != -1;) {
            sb.append(new String(buffer, 0, n));
        }
        return sb.toString();
    }

}
