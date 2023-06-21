package com.example.embeddedapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

public class BlueToothViews extends AppCompatActivity {
    static boolean _is_read = false;

    BluetoothAdapter bluetoothAdapter;
    BluetoothSocket bluetoothSocket;
    BluetoothDevice bluetoothDevice;
    BluetoothClient bluetooth_client;
    static TextView temperature_view;
    static TextView humidity_view;
    static TextView door_status_view;
    static String bluetoothData;
    private Timer layout_timer;
    static LinearLayout rx_layout;
    static LinearLayout tx_layout;

    Button open_btn;
    Button close_btn;
    Button finish_btn;

    int[] txs = new int[]{1,2,5,8};
    int[] txe = new int[]{2,5,8,9};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blue_tooth_views);
        Intent intent = getIntent();
        String bluetooth_mac_address = intent.getStringExtra("mac_address");
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothDevice = bluetoothAdapter.getRemoteDevice(bluetooth_mac_address);
        temperature_view = findViewById(R.id.temperature);
        humidity_view = findViewById(R.id.humidity);
        door_status_view = findViewById(R.id.door_status);
        rx_layout = findViewById(R.id.rx_layout);
        tx_layout = findViewById(R.id.tx_layout);
        bluetooth_client = new BluetoothClient(bluetoothDevice);
        open_btn = findViewById(R.id.door_open);
        close_btn = findViewById(R.id.door_close);
        finish_btn = findViewById(R.id.mcu_finish);
        bluetooth_client.start();
        layout_timer = new Timer();
        layout_timer.schedule(task_rxtx,200,100);

        open_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                bluetooth_client.bt_thread.
                bluetooth_client.bt_inner_thread.write("\u00021\u0003".getBytes());
                serial_layout();
            }
        });

        close_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                bluetooth_client.bt_inner_thread.write("\u00020\u0003".getBytes());
                serial_layout();
            }
        });
    }

    void serial_layout(){
        tx_layout.setBackgroundResource(R.drawable.tx_layout);
    }



    TimerTask task_rxtx = new TimerTask() {
        @Override
        public void run() {
            BlueToothViews.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    rx_layout.setBackgroundResource(R.drawable.offer_layout);
                    tx_layout.setBackgroundResource(R.drawable.offer_layout);
                }
            });
        }
    };
    public static Handler mHand = new Handler()
    {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case 0:
                    Log.d("Message State", String.valueOf(msg.what));
                    String recive_data = new String((byte[]) msg.obj, 0, msg.arg1);
                    bluetoothData += recive_data;
                    if (bluetoothData.contains("\u0002"))
                    {
                        bluetoothData = bluetoothData.substring(bluetoothData.indexOf("\u0002"));
                        if (bluetoothData.contains("\u0003"))
                        {
                            rx_layout.setBackgroundResource(R.drawable.rx_layout);
                            dataParsing(bluetoothData);
                            Log.d("NOWDATA",bluetoothData);
                            bluetoothData = "";
                        }
                    }
                    break;
                case 1:
                    Log.d("Bluetooth_Write",String.valueOf(msg));
                    break;
            }

        }

    };

    static void dataParsing(String _message){
        try{

            temperature_view.setText(_message.substring(2,4)+"."+_message.charAt(4)+"°C");
            humidity_view.setText(_message.substring(5,7)+"."+_message.charAt(7)+"%");

//            humidity_view.setText(_message.substring(5,8) + "%");
            Log.d("Parse Data", _message.substring(1,2));
            if (_message.charAt(1) == '1'){
                door_status_view.setText("OPEN");
            }else{
                door_status_view.setText("CLOSE");
            }
        }catch (Exception e){Log.d("Error", String.valueOf(e));}
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        bluetooth_client.cancel();
    }
}