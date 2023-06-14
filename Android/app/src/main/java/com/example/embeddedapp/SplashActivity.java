package com.example.embeddedapp;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.splashscreen.SplashScreen;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class SplashActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
//        SplashScreen splashScreen = SplashScreen.installSplashScreen(this);
//        setTheme(R.style.Theme_App);
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_splash);
        startActivity(new Intent(getApplicationContext(), MainActivity.class));

    }
}