package com.example.embeddedapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        moveList(1);
    }

    private void moveList(int sec){
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Intent list_view = new Intent(getApplicationContext(), BlueToothList.class);
                startActivity(list_view);
                finish();
            }
        }, 1000*sec);
    }

}