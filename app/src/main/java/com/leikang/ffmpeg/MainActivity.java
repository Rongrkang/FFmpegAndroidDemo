package com.leikang.ffmpeg;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.MediaCodec;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private final static String TAG = "MainActivity";

    static {
        System.loadLibrary("ffmpeg");
    }

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(runnable).start();
            }
        });

        mSurfaceView = (SurfaceView) findViewById(R.id.surfaceView);
        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    private final Runnable runnable = new Runnable() {
        @Override
        public void run() {
            File cacheFile = getAssetsCacheFile(MainActivity.this, "video.mp4");

//            File ouputFile = new File(cacheFile.getParent(), "abc.gif");
//            if (ouputFile.exists()) {
//                ouputFile.delete();
//            }
//
//            final String cmd = "ffmpeg -i " + cacheFile.getAbsolutePath() + " -vframes 100 -y -f gif -s 768*432 "
//                    + ouputFile.getAbsolutePath();

            File ouputFile = new File(cacheFile.getParent(), "output.mp4");
            if (ouputFile.exists()) {
                ouputFile.delete();
            }

            final String cmd = "ffmpeg -i " + cacheFile.getAbsolutePath() + " -y -c:v libx264 -c:a aac -vf scale=480:-2 -preset ultrafast -crf 28 -b:a 128k " + ouputFile.getAbsolutePath();


//            File ouputFile = new File(cacheFile.getParent(), "abc.avi");
//            if (ouputFile.exists()) {
//                ouputFile.delete();
//            }
//
//            String cmd = "ffmpeg -i " + cacheFile.getAbsolutePath() + " " + ouputFile.getAbsolutePath();

//            File ouputFile = new File(cacheFile.getParent(), "abc.aac");
//            if (ouputFile.exists()) {
//                ouputFile.delete();
//            }
//
//            String cmd = "ffmpeg -i " + cacheFile.getAbsolutePath() + " -vn -y -acodec copy " + ouputFile.getAbsolutePath();

            int exec = exec(cmd);

            Log.d(TAG, "exec result: " + exec);

        }
    };

    private final Runnable playRunnable = new Runnable() {
        @Override
        public void run() {
//            String url = "http://file.dakawengu.com/image/2018-06-04/1528109031677.mp4";
            String url = "http://jwlive.appwuhan.com/jw/20171114092020_703_5a0a44d4343a6_703.m3u8?auth_key=2141342770-488503361-0-361185abd485d16ed17157945a5c646f";
            playVideo(url, mSurfaceHolder.getSurface());
        }
    };


    public File getAssetsCacheFile(Context context, String fileName) {
        File cacheFile = new File(context.getExternalFilesDir(null), fileName);
        try {
            if (!cacheFile.exists()) {
                InputStream inputStream = context.getAssets().open(fileName);
                try {
                    FileOutputStream outputStream = new FileOutputStream(cacheFile);
                    try {
                        byte[] buf = new byte[1024];
                        int len;
                        while ((len = inputStream.read(buf)) > 0) {
                            outputStream.write(buf, 0, len);
                        }
                    } finally {
                        outputStream.close();
                    }
                } finally {
                    inputStream.close();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return cacheFile;
    }


    private int exec(String cmd) {
        String regulation = "[ \\t]+";
        final String[] split = cmd.split(regulation);
        return exec(split);
    }


    public native int exec(String[] cmd);

    public native int playVideo(String url, Object surface);


}
