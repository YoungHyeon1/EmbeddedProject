package com.example.embeddedapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.splashscreen.SplashScreen;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;


public class MainActivity extends AppCompatActivity {
    private List permissionList;
    BluetoothAdapterView mMyAdapter;

    private String[] permissions = {
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.BLUETOOTH_SCAN,
            Manifest.permission.BLUETOOTH_ADVERTISE,
            Manifest.permission.BLUETOOTH_CONNECT
    };
    private BluetoothAdapter bluetoothAdapter;
    private ListView bt_list_view;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
//        SplashScreen splashScreen = SplashScreen.installSplashScreen(this);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        bt_list_view = (ListView) findViewById(R.id.listview);
        try{
            bluetoothPermission();
        }catch (Exception e)
        {
            Log.d("Permission Error", String.valueOf(e));
        }
        Log.d("TEST", "OnCreate");
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(receiver, filter);
        //moveList(1);

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        Log.d("RequestPermision Result", String.valueOf(requestCode));
        boolean is_permission = false;
        if(!checkPermission()){
            AlertDialog.Builder alert = new AlertDialog.Builder(this);
            alert.setPositiveButton("확인", null);
            alert.setMessage("권한을 허용하지 않으면 앱 사용이 제한됩니다.");
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            for (String permission : permissions) {
                if (shouldShowRequestPermissionRationale(permission)) {
                    showMessageOKCancel("permission을 허용해야 앱을 사용 할 수 있습니다.\n허용하시겠습니까?",
                            new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                                        requestPermissions(permissions,
                                                1023);
                                    }
                                }
                            });
                }
            }
        }
    }

    public void requestPermission(){
        Log.d("Request Start", String.valueOf(permissionList));
        try{
            ActivityCompat.requestPermissions(this, (String[]) permissionList.toArray(new String[permissionList.size()]), 1023);
        }catch (Exception e){
            Log.d("Error Msg", String.valueOf(e));
        }
        Log.d("Request End", "TEST");
    }

    private void showMessageOKCancel(String message, DialogInterface.OnClickListener okListener) {
        new AlertDialog.Builder(MainActivity.this)
                .setMessage(message)
                .setPositiveButton("OK", okListener)
                .setNegativeButton("Cancel", null)
                .create()
                .show();
    }
    public boolean checkPermission(){
        int result;
        permissionList = new ArrayList<>();

        // 위에서 배열로 선언한 권한 중 허용되지 않은 권한이 있는지 체크
        for(String pm : permissions){
            result = ContextCompat.checkSelfPermission(this, pm);
            Log.d("checkPermission", String.valueOf(result));
            if(result != PackageManager.PERMISSION_GRANTED){
                permissionList.add(pm);
            }
        }

        if(!permissionList.isEmpty()){
            return false;
        }
        return true;
    }
    private void bluetoothPermission() {
        if (!checkPermission()){
            requestPermission();
        }

        BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        if (Build.VERSION.SDK_INT >= 31) {
            bluetoothAdapter = bluetoothManager.getAdapter();
        } else {
            bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        }
        mMyAdapter = new BluetoothAdapterView(MainActivity.this, bluetoothAdapter);
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1023);
            mMyAdapter.addItem("TEST Name", "TestAddress");
            bt_list_view.setAdapter(mMyAdapter);
            bluetoothAdapter.startDiscovery();
        }else{
            mMyAdapter.addItem("TEST Name", "TestAddress");
            bt_list_view.setAdapter(mMyAdapter);
            bluetoothAdapter.startDiscovery();

        }
        if (ContextCompat.checkSelfPermission(
                MainActivity.this,
                Manifest.permission.BLUETOOTH_SCAN
        ) == PackageManager.PERMISSION_DENIED) {
            if (Build.VERSION.SDK_INT >= 31) {
                ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.BLUETOOTH_SCAN}, 100);
                Log.d("Bluetooth Request", "DENIED");
            }
        }

    }
        private final BroadcastReceiver receiver = new BroadcastReceiver() {

            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    try{
                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                        String deviceName = device.getName();
                        String deviceHardwareAddress = device.getAddress(); // MAC address
                        Log.d("Bluetooth TEST", deviceName);
                        mMyAdapter.addItem(deviceName, deviceHardwareAddress);
                        bt_list_view.setAdapter(mMyAdapter);
                    } catch (SecurityException e){
                        Toast.makeText(getApplicationContext(), "블루투스를 활성화가 필요합니다", Toast.LENGTH_LONG).show();
                    }

                }
            }
        };
}